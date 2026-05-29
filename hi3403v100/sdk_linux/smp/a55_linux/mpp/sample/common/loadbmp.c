/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "loadbmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "securec.h"
#include <limits.h>
#include "sample_comm.h"

#define BITS_NUM_PER_BYTE 8
#define BYTE_PER_PIX_1555 2
#define BYTE_PER_PIX_8888 4
#define PIX_PER_BYTE_CLUT2 4
#define PIX_PER_BYTE_CLUT4 2
#define STRIDE_ALIGN 4
#define MAX_BIT_COUNT 32
#define MAX_OFF_BITS 118
#define MAX_WIDTH 16384
#define MAX_HEIGHT 8192
#ifndef align_up
#define align_up(x, a) ((((x) + ((a) - 1)) / (a)) * (a))
#endif

osd_component_info g_osd_comp_info[OSD_COLOR_FORMAT_BUTT] = {
    { 0, 4, 4, 4 }, /* RGB444 */
    { 4, 4, 4, 4 }, /* ARGB4444 */
    { 0, 5, 5, 5 }, /* RGB555 */
    { 0, 5, 6, 5 }, /* RGB565 */
    { 1, 5, 5, 5 }, /* ARGB1555 */
    { 0, 0, 0, 0 }, /* RESERVED */
    { 0, 8, 8, 8 }, /* RGB888 */
    { 8, 8, 8, 8 }  /* ARGB8888 */
};

td_u16 osd_make_color_u16(td_u8 r, td_u8 g, td_u8 b, osd_component_info comp_info)
{
    td_u8 r1;
    td_u8 g1;
    td_u8 b1;
    td_u16 pixel = 0;
    td_u32 tmp = 15; /* 15bit color data */

    r1 = r >> (BITS_NUM_PER_BYTE - comp_info.r_len);
    g1 = g >> (BITS_NUM_PER_BYTE - comp_info.g_len);
    b1 = b >> (BITS_NUM_PER_BYTE - comp_info.b_len);
    while (comp_info.a_len) {
        pixel |= (1 << tmp);
        tmp--;
        comp_info.a_len--;
    }

    pixel |= (r1 | (g1 << comp_info.b_len) | (b1 << (comp_info.b_len + comp_info.g_len)));
    return pixel;
}

td_s32 get_bmp_info(const td_char *filename, osd_bit_map_file_header *bmp_file_header, osd_bit_map_info *bmp_info)
{
    FILE *file = TD_NULL;
    td_u16 bf_type;
    td_char path[PATH_MAX] = {0};

    check_null_ptr_return(filename);
    check_null_ptr_return(bmp_file_header);
    check_null_ptr_return(bmp_info);

    if (strlen(filename) >= PATH_MAX) {
        printf("file name Extra long\n");
        return TD_FAILURE;
    }
    if (realpath(filename, path) == TD_NULL) {
        return TD_FAILURE;
    }

    file = fopen(path, "rb");
    if (file == TD_NULL) {
        printf("Open file failed:%s!\n", filename);
        goto read_bmp_failed;
    }

    if (fread(&bf_type, 1, sizeof(bf_type), file) != sizeof(bf_type)) {
        fclose(file);
        goto read_bmp_failed;
    }
    if (bf_type != 0x4d42) { /* BM */
        printf("not bitmap file\n");
        fclose(file);
        goto read_bmp_failed;
    }

    if (fread(bmp_file_header, 1, sizeof(osd_bit_map_file_header), file) != sizeof(osd_bit_map_file_header) ||
        fread(bmp_info, 1, sizeof(osd_bit_map_info), file) != sizeof(osd_bit_map_info)) {
        fclose(file);
        goto read_bmp_failed;
    }

    if (fclose(file) != 0) {
        goto read_bmp_failed;
    }
    return TD_SUCCESS;

read_bmp_failed:
    return TD_FAILURE;
}

static td_bool is_support_bmp_file(const osd_bit_map_info *bmp_info, td_u16 bpp_threshold)
{
    td_u16 bpp;

    bpp = bmp_info->bmp_header.bi_bit_count / BITS_NUM_PER_BYTE;
    if (bpp_threshold != 0 && bpp < bpp_threshold) {
        printf("bitmap format not supported!\n");
        return TD_FALSE;
    }

    if (bmp_info->bmp_header.bi_compression != 0) {
        printf("not support compressed bitmap file!\n");
        return TD_FALSE;
    }

    if (bmp_info->bmp_header.bi_height < 0) {
        printf("bmp_info.bmp_header.bi_height < 0\n");
        return TD_FALSE;
    }

    return TD_TRUE;
}

static td_u8 *read_bmp_raw_data(const osd_bit_map_file_header *bmp_file_header, FILE *file, td_u32 bmp_data_size)
{
    td_u8 *orig_bmp_buf = NULL;

    if (bmp_data_size == 0 || bmp_file_header->bf_off_bits > bmp_file_header->bf_size ||
        bmp_data_size > (bmp_file_header->bf_size - bmp_file_header->bf_off_bits)) {
        printf("bmp data size invalid!\n");
        return NULL;
    }

    orig_bmp_buf = (td_u8 *)malloc(bmp_data_size);
    if (orig_bmp_buf == NULL) {
        printf("not enough memory to malloc!\n");
        return NULL;
    }

    if (fseek(file, bmp_file_header->bf_off_bits, 0) != 0) {
        free(orig_bmp_buf);
        return NULL;
    }
    if (fread(orig_bmp_buf, 1, bmp_data_size, file) != bmp_data_size) {
        printf("fread error!line:%d\n", __LINE__);
        perror("fread:");
        free(orig_bmp_buf);
        return NULL;
    }
    return orig_bmp_buf;
}

static td_s32 read_bmp_data(const osd_bit_map_file_header *bmp_file_header, const osd_bit_map_info *bmp_info,
    FILE *file, osd_logo *video_logo)
{
    td_u8 *rgb_buf = video_logo->rgb_buf;
    td_u8 *orig_bmp_buf = NULL;
    td_u32 bmp_data_stride;
    td_u32 bmp_data_size;
    td_u16 bpp;
    td_u16 dst_bpp;
    td_u16 i;
    td_u16 j;
    td_s32 ret;

    bpp = bmp_info->bmp_header.bi_bit_count / BITS_NUM_PER_BYTE;
    dst_bpp = (bpp > 2) ? 4 : 2; /* RGB1555: 2byte, RGB8888: 4byte */

    video_logo->width = bmp_info->bmp_header.bi_width;
    video_logo->height = bmp_info->bmp_header.bi_height;
    if (video_logo->stride == 0) {
        video_logo->stride = video_logo->width * dst_bpp;
    }

    bmp_data_stride = video_logo->width * bpp;
    bmp_data_stride = align_up(bmp_data_stride, STRIDE_ALIGN);
    bmp_data_size = video_logo->height * bmp_data_stride;

    /* RGB8888 or RGB1555 */
    orig_bmp_buf = read_bmp_raw_data(bmp_file_header, file, bmp_data_size);
    if (orig_bmp_buf == NULL) {
        return TD_FAILURE;
    }

    for (i = 0; i < video_logo->height; i++) {
        for (j = 0; j < video_logo->width; ++j) {
            ret = memcpy_s(rgb_buf + i * video_logo->stride + j * dst_bpp, bpp,
                orig_bmp_buf + ((video_logo->height - 1) - i) * bmp_data_stride + j * bpp, bpp);
            if (ret != EOK) {
                free(orig_bmp_buf);
                printf("copy bmp failed!line:%d\n", __LINE__);
                return TD_FAILURE;
            }
            if (dst_bpp == 4) {                                               /* 4: RGB8888 */
                *(rgb_buf + i * video_logo->stride + j * dst_bpp + 3) = 0x80; /* 3: alpha offset */
            }
        }
    }

    free(orig_bmp_buf);

    return TD_SUCCESS;
}

td_s32 load_bmp(const td_char *filename, osd_logo *video_logo)
{
    osd_bit_map_file_header bmp_file_header;
    osd_bit_map_info bmp_info;
    FILE *file = TD_NULL;
    td_char path[PATH_MAX] = {0};

    if (filename == TD_NULL || video_logo == TD_NULL) {
        printf("load_bmp: null ptr args!\n");
        return TD_FAILURE;
    }
    if (strlen(filename) >= PATH_MAX) {
        printf("file name Extra long\n");
        return TD_FAILURE;
    }
    if (realpath(filename, path) == TD_NULL) {
        return TD_FAILURE;
    }
    if (get_bmp_info(path, &bmp_file_header, &bmp_info) < 0) {
        goto read_bmp_failed;
    }
    if (bmp_info.bmp_header.bi_bit_count > MAX_BIT_COUNT || bmp_info.bmp_header.bi_width > MAX_WIDTH ||
        bmp_info.bmp_header.bi_height > MAX_HEIGHT || bmp_file_header.bf_off_bits > MAX_OFF_BITS) {
        printf("bmp info error!");
        goto read_bmp_failed;
    }
    if (is_support_bmp_file(&bmp_info, 2) != TD_TRUE) { /* each pixel should takes 2 (or more) bytes */
        goto read_bmp_failed;
    }

    file = fopen(path, "rb");
    if (file == TD_NULL) {
        printf("Open file failed:%s!\n", filename);
        goto read_bmp_failed;
    }

    if (read_bmp_data(&bmp_file_header, &bmp_info, file, video_logo) != TD_SUCCESS) {
        (td_void)fclose(file);
        goto read_bmp_failed;
    }
    if (fclose(file) != 0) {
        goto read_bmp_failed;
    }
    return TD_SUCCESS;

read_bmp_failed:
    return TD_FAILURE;
}

static td_void updata_osd_logo_size_info(const osd_bit_map_info *bmp_info, osd_color_format fmt, osd_logo *video_logo)
{
    video_logo->width = bmp_info->bmp_header.bi_width;
    video_logo->height = bmp_info->bmp_header.bi_height;

    switch (fmt) {
        case OSD_COLOR_FORMAT_RGB444:
        case OSD_COLOR_FORMAT_RGB555:
        case OSD_COLOR_FORMAT_RGB565:
        case OSD_COLOR_FORMAT_RGB1555:
        case OSD_COLOR_FORMAT_RGB4444:
            video_logo->stride = video_logo->width * BYTE_PER_PIX_1555;
            return;
        case OSD_COLOR_FORMAT_RGB888:
        case OSD_COLOR_FORMAT_RGB8888:
            video_logo->stride = video_logo->width * BYTE_PER_PIX_8888;
            return;
        case OSD_COLOR_FORMAT_CLUT2:
            video_logo->stride = video_logo->width / PIX_PER_BYTE_CLUT2;
            return;
        case OSD_COLOR_FORMAT_CLUT4:
            video_logo->stride = video_logo->width / PIX_PER_BYTE_CLUT4;
            return;
        default:
            printf("file(%s), line(%d), no such format!\n", __FILE__, __LINE__);
            return;
    }
}

static td_s32 copy_original_bmp_data(td_u16 bpp, const td_u8 *data, const bmp_data_size_info *data_size_info,
    osd_logo *video_logo)
{
    td_u32 i;
    td_u32 j;
    td_s32 ret;
    td_u8 *rgb_buf = video_logo->rgb_buf;

    for (i = 0; i < data_size_info->height; ++i) {
        for (j = 0; j < data_size_info->width; ++j) {
            ret = memcpy_s(rgb_buf + i * video_logo->stride + j * bpp, bpp,
                data + ((data_size_info->height - 1) - i) * data_size_info->stride + j * bpp, bpp);
            if (ret != EOK) {
                printf("copy bmp failed!line:%d\n", __LINE__);
                return TD_FAILURE;
            }
        }
    }

    return TD_SUCCESS;
}

static td_s32 copy_clut_bmp_data(td_u16 bpp, osd_color_format fmt, const td_u8 *data,
    const bmp_data_size_info *data_size_info, osd_logo *video_logo)
{
    td_u32 i;
    td_u32 j;
    td_s32 ret;
    td_u8 *rgb_buf = video_logo->rgb_buf;
    td_u32 width = data_size_info->width;

    if (fmt == OSD_COLOR_FORMAT_CLUT4) {
        width /= PIX_PER_BYTE_CLUT4;
    } else if (fmt == OSD_COLOR_FORMAT_CLUT2) {
        width /= PIX_PER_BYTE_CLUT2;
    } else {
    }

    for (i = 0; i < data_size_info->height; ++i) {
        for (j = 0; j < width; ++j) {
            ret = memcpy_s(rgb_buf + i * video_logo->stride + j, bpp,
                data + ((data_size_info->height - 1) - i) * data_size_info->stride + j, bpp);
            if (ret != EOK) {
                printf("copy bmp failed!line:%d\n", __LINE__);
                return TD_FAILURE;
            }
        }
    }

    return TD_SUCCESS;
}

static td_s32 copy_2byte_bmp(td_u16 bpp, osd_color_format fmt, const td_u8 *data,
    const bmp_data_size_info *data_size_info, osd_logo *video_logo)
{
    td_u32 i;
    td_u32 j;
    td_u8 *rgb_buf = video_logo->rgb_buf;
    td_u8 r;
    td_u8 g;
    td_u8 b;
    td_u8 *start = TD_NULL;
    td_u16 *dst = TD_NULL;

    /* start color convert */
    for (i = 0; i < data_size_info->height; ++i) {
        for (j = 0; j < data_size_info->width; ++j) {
            start = (td_u8 *)(data + ((data_size_info->height - 1) - i) * data_size_info->stride + j * bpp);
            dst = (td_u16 *)(rgb_buf + i * video_logo->stride + j * 2); /* 2 bytes */
            r = *(start);
            g = *(start + 1);
            b = *(start + 2); /* 2 bytes offset */
            *dst = osd_make_color_u16(r, g, b, g_osd_comp_info[fmt]);
        }
    }
    return TD_SUCCESS;
}

static td_s32 copy_4byte_bmp(td_u16 bpp, const td_u8 *data, const bmp_data_size_info *data_size_info,
    osd_logo *video_logo)
{
    td_u32 i;
    td_u32 j;
    td_s32 ret;
    td_u8 *rgb_buf = video_logo->rgb_buf;

    for (i = 0; i < data_size_info->height; ++i) {
        for (j = 0; j < data_size_info->width; ++j) {
            ret = memcpy_s(rgb_buf + i * video_logo->stride + j * 4, bpp, /* offset 4 bytes */
                data + ((data_size_info->height - 1) - i) * data_size_info->stride + j * bpp, bpp);
            if (ret != EOK) {
                printf("copy bmp failed!line:%d\n", __LINE__);
                return TD_FAILURE;
            }
            *(rgb_buf + i * video_logo->stride + j * 4 + 3) = 0xff; /* 4 bytes data, alpha offset is 3 */
        }
    }
    return TD_SUCCESS;
}

static td_s32 copy_original_bmp_data_and_fill_alpha(td_u16 bpp, osd_color_format fmt, const td_u8 *data,
    const bmp_data_size_info *data_size_info, osd_logo *video_logo)
{
    switch (fmt) {
        case OSD_COLOR_FORMAT_RGB444:
        case OSD_COLOR_FORMAT_RGB555:
        case OSD_COLOR_FORMAT_RGB565:
        case OSD_COLOR_FORMAT_RGB1555:
        case OSD_COLOR_FORMAT_RGB4444:
            return copy_2byte_bmp(bpp, fmt, data, data_size_info, video_logo);
        case OSD_COLOR_FORMAT_RGB888:
        case OSD_COLOR_FORMAT_RGB8888:
            return copy_4byte_bmp(bpp, data, data_size_info, video_logo);
        default:
            printf("file(%s), line(%d), no such format!\n", __FILE__, __LINE__);
            return TD_FAILURE;
    }
}

static td_s32 read_bmp_data_ex(const osd_bit_map_file_header *bmp_file_header, const osd_bit_map_info *bmp_info,
    FILE *file, osd_logo *video_logo, osd_color_format fmt)
{
    td_u8 *orig_bmp_buf = NULL;
    td_u16 bpp = bmp_info->bmp_header.bi_bit_count / BITS_NUM_PER_BYTE;
    td_u32 bmp_data_stride;
    td_u32 bmp_data_size;
    td_s32 ret = TD_SUCCESS;
    bmp_data_size_info data_size_info;

    updata_osd_logo_size_info(bmp_info, fmt, video_logo);

    bmp_data_stride = (bpp == 0) ? video_logo->stride : (video_logo->width * bpp);
    bmp_data_stride = align_up(bmp_data_stride, STRIDE_ALIGN);
    bmp_data_size = video_logo->height * bmp_data_stride;

    data_size_info.width = video_logo->width;
    data_size_info.height = video_logo->height;
    data_size_info.stride = bmp_data_stride;

    orig_bmp_buf = (td_u8 *)malloc(bmp_data_size);
    if (orig_bmp_buf == NULL) {
        printf("not enough memory to malloc!\n");
        return TD_FAILURE;
    }

    if (fseek(file, bmp_file_header->bf_off_bits, 0) != 0) {
        free(orig_bmp_buf);
        return TD_FAILURE;
    }
    if (fread(orig_bmp_buf, 1, bmp_data_size, file) != bmp_data_size) {
        printf("fread error!line:%d\n", __LINE__);
        perror("fread:");
        free(orig_bmp_buf);
        return TD_FAILURE;
    }

    /* copy bmp data to rgb_buf according bpp (and fmt) */
    if (bpp == 2 || bpp == 4) { /* each pixel takes 2 (or 4) bytes */
        ret = copy_original_bmp_data(bpp, orig_bmp_buf, &data_size_info, video_logo);
        goto copy_over_exit;
    }

    if (bpp <= 1) { /* such as clut2 or clut 4, or 2BPP... */
        ret = copy_clut_bmp_data(bpp, fmt, orig_bmp_buf, &data_size_info, video_logo);
        goto copy_over_exit;
    }

    /* bpp should equal to 3 here */
    ret = copy_original_bmp_data_and_fill_alpha(bpp, fmt, orig_bmp_buf, &data_size_info, video_logo);

copy_over_exit:
    free(orig_bmp_buf);
    return ret;
}

td_s32 load_bmp_ex(const td_char *filename, osd_logo *video_logo, osd_color_format fmt)
{
    osd_bit_map_file_header bmp_file_header;
    osd_bit_map_info bmp_info;
    FILE *file = TD_NULL;
    td_char path[PATH_MAX] = {0};

    if (filename == TD_NULL || video_logo == TD_NULL) {
        printf("load_bmp_ex: null ptr args!\n");
        return TD_FAILURE;
    }
    if (strlen(filename) >= PATH_MAX) {
        printf("file name Extra long\n");
        return TD_FAILURE;
    }
    if (realpath(filename, path) == TD_NULL) {
        return TD_FAILURE;
    }
    if (get_bmp_info(path, &bmp_file_header, &bmp_info) < 0) {
        goto read_bmp_failed;
    }
    if (bmp_info.bmp_header.bi_bit_count > MAX_BIT_COUNT || bmp_info.bmp_header.bi_width > MAX_WIDTH ||
        bmp_info.bmp_header.bi_height > MAX_HEIGHT || bmp_file_header.bf_off_bits > MAX_OFF_BITS) {
        printf("bmp info error!");
        goto read_bmp_failed;
    }
    if (is_support_bmp_file(&bmp_info, 0) != TD_TRUE) {
        goto read_bmp_failed;
    }

    file = fopen(path, "rb");
    if (file == TD_NULL) {
        printf("Open file failed:%s!\n", filename);
        goto read_bmp_failed;
    }

    if (read_bmp_data_ex(&bmp_file_header, &bmp_info, file, video_logo, fmt) != TD_SUCCESS) {
        (td_void)fclose(file);
        goto read_bmp_failed;
    }
    if (fclose(file) != 0) {
        goto read_bmp_failed;
    }
    return TD_SUCCESS;

read_bmp_failed:
    return TD_FAILURE;
}

static td_s32 read_bmp_canvas(const osd_bit_map_file_header *bmp_file_header, const osd_bit_map_info *bmp_info,
    FILE *file, osd_logo *video_logo, osd_color_format fmt)
{
    td_u8 *orig_bmp_buf = NULL;
    td_u16 bpp = bmp_info->bmp_header.bi_bit_count / BITS_NUM_PER_BYTE;
    td_u32 bmp_data_size;
    td_s32 ret = TD_SUCCESS;
    bmp_data_size_info data_size_info;

    data_size_info.width = bmp_info->bmp_header.bi_width;
    data_size_info.height = bmp_info->bmp_header.bi_height;

    if (bpp == 0) {
        if (fmt == OSD_COLOR_FORMAT_CLUT2) {
            data_size_info.stride = data_size_info.width / PIX_PER_BYTE_CLUT2;
        } else if (fmt == OSD_COLOR_FORMAT_CLUT4) {
            data_size_info.stride = data_size_info.width / PIX_PER_BYTE_CLUT4;
        } else {
            data_size_info.stride = video_logo->stride;
        }
    } else {
        data_size_info.stride = data_size_info.width * bpp;
        data_size_info.stride = align_up(data_size_info.stride, STRIDE_ALIGN);
    }

    bmp_data_size = data_size_info.height * data_size_info.stride;
    orig_bmp_buf = (td_u8 *)malloc(bmp_data_size);
    if (orig_bmp_buf == NULL) {
        printf("not enough memory to malloc!\n");
        return TD_FAILURE;
    }

    if (fseek(file, bmp_file_header->bf_off_bits, 0) != 0) {
        free(orig_bmp_buf);
        return TD_FAILURE;
    }
    if (fread(orig_bmp_buf, 1, bmp_data_size, file) != bmp_data_size) {
        printf("fread error!line:%d\n", __LINE__);
        perror("fread:");
        free(orig_bmp_buf);
        return TD_FAILURE;
    }

    /* copy bmp data to rgb_buf according bpp (and fmt) */
    if (bpp == 2 || bpp == 4) { /* each pixel takes 2 (or 4) bytes */
        ret = copy_original_bmp_data(bpp, orig_bmp_buf, &data_size_info, video_logo);
        goto copy_over_exit;
    }

    if (bpp <= 1) { /* such as clut2 or clut 4, or 2BPP... */
        ret = copy_clut_bmp_data(bpp, fmt, orig_bmp_buf, &data_size_info, video_logo);
        goto copy_over_exit;
    }

    /* bpp should equal to 3 here */
    ret = copy_original_bmp_data_and_fill_alpha(bpp, fmt, orig_bmp_buf, &data_size_info, video_logo);

copy_over_exit:
    free(orig_bmp_buf);
    return ret;
}

td_s32 load_bmp_canvas(const td_char *filename, osd_logo *video_logo, osd_color_format fmt)
{
    osd_bit_map_file_header bmp_file_header;
    osd_bit_map_info bmp_info;
    FILE *file = TD_NULL;
    td_char path[PATH_MAX] = {0};

    if (filename == TD_NULL || video_logo == TD_NULL) {
        printf("load_bmp_canvas: null ptr args!\n");
        return TD_FAILURE;
    }
    if (strlen(filename) >= PATH_MAX) {
        printf("file name Extra long\n");
        return TD_FAILURE;
    }
    if (realpath(filename, path) == TD_NULL) {
        return TD_FAILURE;
    }
    if (get_bmp_info(path, &bmp_file_header, &bmp_info) < 0) {
        goto read_bmp_failed;
    }
    if (bmp_info.bmp_header.bi_bit_count > MAX_BIT_COUNT || bmp_info.bmp_header.bi_width > MAX_WIDTH ||
        bmp_info.bmp_header.bi_height > MAX_HEIGHT || bmp_file_header.bf_off_bits > MAX_OFF_BITS) {
        printf("bmp info error!");
        goto read_bmp_failed;
    }
    if (is_support_bmp_file(&bmp_info, 0) != TD_TRUE) {
        goto read_bmp_failed;
    }

    file = fopen(path, "rb");
    if (file == TD_NULL) {
        printf("Open file failed:%s!\n", filename);
        goto read_bmp_failed;
    }

    if (read_bmp_canvas(&bmp_file_header, &bmp_info, file, video_logo, fmt) != TD_SUCCESS) {
        (td_void)fclose(file);
        goto read_bmp_failed;
    }
    if (fclose(file) != 0) {
        goto read_bmp_failed;
    }
    return TD_SUCCESS;

read_bmp_failed:
    return TD_FAILURE;
}

td_char *get_ext_name(const td_char *filename)
{
    td_char *pret = TD_NULL;
    size_t len;

    if (filename == TD_NULL) {
        printf("filename can't be null!");
        return TD_NULL;
    }

    len = strlen(filename);
    while (len) {
        pret = (td_char *)(filename + len);
        if (*pret == '.') {
            return (pret + 1);
        }

        len--;
    }

    return pret;
}

td_s32 load_image(const td_char *filename, osd_logo *video_logo)
{
    td_char *ext = get_ext_name(filename);
    if (ext == TD_NULL) {
        printf("get_ext_name error!\n");
        return -1;
    }

    if (strcmp(ext, "bmp") == 0) {
        if (load_bmp(filename, video_logo) != 0) {
            printf("load_bmp error!\n");
            return -1;
        }
    } else {
        printf("not supported image file!\n");
        return -1;
    }

    return 0;
}

td_s32 load_image_ex(const td_char *filename, osd_logo *video_logo, osd_color_format fmt)
{
    td_char *ext = get_ext_name(filename);
    if (ext == TD_NULL) {
        printf("load_image_ex error!\n");
        return -1;
    }

    if (strcmp(ext, "bmp") == 0) {
        if (load_bmp_ex(filename, video_logo, fmt) != 0) {
            printf("load_bmp_ex error!\n");
            return -1;
        }
    } else {
        printf("not supported image file!\n");
        return -1;
    }

    return 0;
}

td_s32 load_canvas_ex(const td_char *filename, osd_logo *video_logo, osd_color_format fmt)
{
    td_char *ext = get_ext_name(filename);
    if (ext == TD_NULL) {
        printf("load_canvas_ex error!\n");
        return -1;
    }

    if (strcmp(ext, "bmp") == 0) {
        if (load_bmp_canvas(filename, video_logo, fmt) != 0) {
            printf("OSD_LoadBMP error!\n");
            return -1;
        }
    } else {
        printf("not supported image file!\n");
        return -1;
    }

    return 0;
}

td_s32 load_bit_map_to_surface(const td_char *file_name, const osd_surface *surface, td_u8 *virt)
{
    osd_logo logo;
    check_null_ptr_return(file_name);
    check_null_ptr_return(surface);
    logo.stride = surface->stride;
    logo.rgb_buf = virt;

    return load_image(file_name, &logo);
}

td_s32 create_surface_by_bit_map(const td_char *file_name, osd_surface *surface, td_u8 *virt)
{
    osd_logo logo;

    check_null_ptr_return(file_name);
    check_null_ptr_return(surface);
    logo.rgb_buf = virt;
    if (load_image_ex(file_name, &logo, surface->color_format) < 0) {
        printf("load bmp error!\n");
        return -1;
    }

    surface->height = logo.height;
    surface->width = logo.width;
    surface->stride = logo.stride;

    return 0;
}

td_s32 create_surface_by_canvas(const td_char *file_name, osd_surface *surface, td_u8 *virt,
    const canvas_size_info *canvas_size)
{
    osd_logo logo;

    check_null_ptr_return(file_name);
    check_null_ptr_return(surface);
    check_null_ptr_return(canvas_size);
    logo.rgb_buf = virt;
    logo.width = canvas_size->width;
    logo.height = canvas_size->height;
    logo.stride = canvas_size->stride;
    if (load_canvas_ex(file_name, &logo, surface->color_format) < 0) {
        printf("load bmp error!\n");
        return -1;
    }

    surface->height = canvas_size->height;
    surface->width = canvas_size->width;
    surface->stride = canvas_size->stride;

    return 0;
}
