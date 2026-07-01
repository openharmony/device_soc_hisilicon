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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"
#include "loadbmp.h"
#include "sdk_module_init.h"

#define SAMPLE_VGS_READ_PATH "./data/input/1920_1080_p420.yuv"
#define SAMPLE_VGS_READ_TILE_PATH "./data/input/Tile_1280x720_P420.yuv"
#define SAMPLE_VGS_SAVE_PATH "./data/output"
#define SAMPLE_VGS_BITMAP_PATH "./data/input/mm.bmp"
#define FILE_NAME_LENGTH 128 /* max filename length is 128 */
#define MAX_LUMA_NUM 4

typedef enum {
    SAMPLE_VGS_OSD_NORM = 0,
    SAMPLE_VGS_OSD_CLUT2,
    SAMPLE_VGS_OSD_CLUT4,
    SAMPLE_VGS_OSD_BUTT
} sample_vgs_osd_type;

typedef struct {
    td_bool scale;
    td_bool cover;
    td_bool mosaic;
    td_bool corner_rect_en;
    td_bool osd;
    td_bool draw_line;
    td_bool rotate;
    td_bool luma;
    td_bool stitch;

    ot_vgs_scale_coef_mode *vgs_scl_coef_mode;
    ot_cover *vgs_add_cover;
    ot_mosaic *vgs_add_mosaic;
    ot_corner_rect_attr *corner_rect_attr;
    ot_corner_rect *corner_rect;
    ot_vgs_osd *vgs_add_osd;
    ot_vgs_line *vgs_draw_line;
    ot_rotation *rotation_angle;
    td_u32 luma_num;
    ot_rect *luma_rect;
    td_u64 *luma_data;
    td_u32 stitch_num;

    ot_rgn_handle *rgn_handle[SAMPLE_VGS_OSD_BUTT];
    sample_vb_base_info *in_img_vb_info;
    sample_vb_base_info *out_img_vb_info;
    td_s32 sample_num;
} sample_vgs_func_param;

typedef struct {
    ot_vb_blk vb_handle;
    td_void *virt_addr;
    td_u32 vb_size;
    td_bool vb_used;
} sample_vgs_vb_info;

typedef struct {
    td_bool fill_en;
    td_u32 fill_color;
    ot_size osd_size;
    td_u32 stride;
    ot_pixel_format img_pixel_format;
} sample_vgs_osd_load_bmp_info;

sample_vgs_vb_info g_in_img_vb_info;
sample_vgs_vb_info g_out_img_vb_info;
static td_u32 g_vgs_sample_signal_flag = 0;

/* function : show usage */
static td_void sample_vgs_usage(td_char *s_prg_nm)
{
    printf("\n/*****************************************/\n");
    printf("usage: %s <index>\n", s_prg_nm);
    printf("index:\n");
    printf("\t0) FILE -> VGS(scale) -> FILE.\n");
    printf("\t1) FILE -> VGS(cover+OSD+clut+mosaic+corner_rect) -> FILE.\n");
    printf("\t2) FILE -> VGS(draw_line) -> FILE.\n");
    printf("\t3) FILE -> VGS(rotate) -> FILE.\n");
    printf("\t4) FILE -> VGS(luma) -> FILE + data.\n");
    printf("\t5) FILE -> VGS(stitch) -> FILE.\n");
    printf("/*****************************************/\n");
    return;
}

/* function : to process abnormal case */
static td_void sample_vgs_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_vgs_sample_signal_flag = 1;
    }
}

static td_void sample_vgs_get_yuv_buffer_cfg(const sample_vb_base_info *vb_base_info, ot_vb_calc_cfg *vb_cal_config)
{
    ot_pic_buf_attr buf_attr;
    buf_attr.width = vb_base_info->width;
    buf_attr.height = vb_base_info->height;
    buf_attr.pixel_format = vb_base_info->pixel_format;
    buf_attr.bit_width = OT_DATA_BIT_WIDTH_8;
    buf_attr.compress_mode = vb_base_info->compress_mode;
    buf_attr.align = vb_base_info->align;
    ot_common_get_pic_buf_cfg(&buf_attr, vb_cal_config);
    return;
}

static td_void sample_vgs_set_frame_info(const ot_vb_calc_cfg *vb_cal_config, const sample_vb_base_info *vb_info,
    const sample_vgs_vb_info *vgs_vb_info, td_phys_addr_t phys_addr, ot_video_frame_info *frame_info)
{
    frame_info->mod_id = OT_ID_VGS;
    frame_info->pool_id = ss_mpi_vb_handle_to_pool_id(vgs_vb_info->vb_handle);

    frame_info->video_frame.width       = vb_info->width;
    frame_info->video_frame.height      = vb_info->height;
    frame_info->video_frame.field        = OT_VIDEO_FIELD_FRAME;
    frame_info->video_frame.pixel_format  = vb_info->pixel_format;
    frame_info->video_frame.video_format  = vb_info->video_format;
    frame_info->video_frame.compress_mode = vb_info->compress_mode;
    frame_info->video_frame.dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    frame_info->video_frame.color_gamut   = OT_COLOR_GAMUT_BT601;

    frame_info->video_frame.header_stride[0]  = vb_cal_config->head_stride;
    frame_info->video_frame.header_stride[1]  = vb_cal_config->head_stride;
    frame_info->video_frame.header_phys_addr[0] = phys_addr;
    frame_info->video_frame.header_phys_addr[1] = frame_info->video_frame.header_phys_addr[0] +
                                                  vb_cal_config->head_y_size;
    frame_info->video_frame.header_virt_addr[0] = vgs_vb_info->virt_addr;
    frame_info->video_frame.header_virt_addr[1] = frame_info->video_frame.header_virt_addr[0] +
                                                  vb_cal_config->head_y_size;

    frame_info->video_frame.stride[0]  = vb_cal_config->main_stride;
    frame_info->video_frame.stride[1]  = vb_cal_config->main_stride;
    frame_info->video_frame.phys_addr[0] = frame_info->video_frame.header_phys_addr[0] + vb_cal_config->head_size;
    frame_info->video_frame.phys_addr[1] = frame_info->video_frame.phys_addr[0] + vb_cal_config->main_y_size;
    frame_info->video_frame.virt_addr[0] = frame_info->video_frame.header_virt_addr[0] + vb_cal_config->head_size;
    frame_info->video_frame.virt_addr[1] = frame_info->video_frame.virt_addr[0] + vb_cal_config->main_y_size;

    return;
}

static td_s32 sample_vgs_get_frame_vb(const sample_vb_base_info *vb_info, const ot_vb_calc_cfg *vb_cal_config,
    ot_video_frame_info *frame_info, sample_vgs_vb_info *vgs_vb_info)
{
    td_phys_addr_t phys_addr;

    vgs_vb_info->vb_handle = ss_mpi_vb_get_blk(OT_VB_INVALID_POOL_ID, vb_cal_config->vb_size, TD_NULL);
    if (vgs_vb_info->vb_handle == OT_VB_INVALID_HANDLE) {
        sample_print("mpi_vb_get_block failed!\n");
        return TD_FAILURE;
    }
    vgs_vb_info->vb_used = TD_TRUE;

    phys_addr = ss_mpi_vb_handle_to_phys_addr(vgs_vb_info->vb_handle);
    if (phys_addr == 0) {
        sample_print("mpi_vb_handle2_phys_addr failed!\n");
        ss_mpi_vb_release_blk(vgs_vb_info->vb_handle);
        vgs_vb_info->vb_used = TD_FALSE;
        return TD_FAILURE;
    }

    vgs_vb_info->virt_addr = (td_u8*)ss_mpi_sys_mmap(phys_addr, vb_cal_config->vb_size);
    if (vgs_vb_info->virt_addr == TD_NULL) {
        sample_print("mpi_sys_mmap failed!\n");
        ss_mpi_vb_release_blk(vgs_vb_info->vb_handle);
        vgs_vb_info->vb_used = TD_FALSE;
        return TD_FAILURE;
    }
    vgs_vb_info->vb_size = vb_cal_config->vb_size;

    sample_vgs_set_frame_info(vb_cal_config, vb_info, vgs_vb_info, phys_addr, frame_info);

    return TD_SUCCESS;
}

static td_s32 sample_vgs_convert_chroma_planar_to_sp42x(FILE *file, td_u8 *chroma_data,
    td_u32 luma_stride, td_u32 chroma_width, td_u32 chroma_height)
{
    td_u32 chroma_stride = luma_stride >> 1;
    td_u8 *planar_buffer = TD_NULL;
    td_u8 *dst = TD_NULL;
    td_u32 row;
    td_u32 list;

    planar_buffer = (td_u8*)malloc(chroma_stride);
    if (planar_buffer == TD_NULL) {
        sample_print("vgs malloc failed!\n");
        return TD_FAILURE;
    }
    if (memset_s(planar_buffer, chroma_stride, 0, chroma_stride) != EOK) {
        sample_print("vgs memset_s failed!\n");
        free(planar_buffer);
        planar_buffer = TD_NULL;
        return TD_FAILURE;
    }

    /* U */
    dst = chroma_data + 1;
    for (row = 0; row < chroma_height; ++row) {
        if (fread(planar_buffer, chroma_width, 1, file) != 1) {
            free(planar_buffer);
            return TD_FAILURE;
        }
        for (list = 0; list < chroma_stride; ++list) {
            *dst = *(planar_buffer + list);
            dst += 2; /* traverse 2 steps away to the next U-component data */
        }
        dst = chroma_data + 1;
        dst += (row + 1) * luma_stride;
    }

    /* V */
    dst = chroma_data;
    for (row = 0; row < chroma_height; ++row) {
        if (fread(planar_buffer, chroma_width, 1, file) != 1) {
            free(planar_buffer);
            return TD_FAILURE;
        }
        for (list = 0; list < chroma_stride; ++list) {
            *dst = *(planar_buffer + list);
            dst += 2; /* traverse 2 steps away to the next V-component data */
        }
        dst = chroma_data;
        dst += (row + 1) * luma_stride;
    }

    free(planar_buffer);
    planar_buffer = TD_NULL;
    return TD_SUCCESS;
}

static td_s32 sample_vgs_read_file_to_sp42_x(FILE *file, ot_video_frame *frame)
{
    td_u8 *luma = (td_u8*)(td_uintptr_t)frame->virt_addr[0];
    td_u8 *chroma = (td_u8*)(td_uintptr_t)frame->virt_addr[1];
    td_u32 luma_width = frame->width;
    td_u32 chroma_width = luma_width >> 1;
    td_u32 luma_height = frame->height;
    td_u32 chroma_height = luma_height;
    td_u32 luma_stride = frame->stride[0];

    td_u8 *dst = TD_NULL;
    td_u32 row;

    if (frame->video_format == OT_VIDEO_FORMAT_LINEAR) {
        /* Y */
        dst = luma;
        for (row = 0; row < luma_height; ++row) {
            fread(dst, luma_width, 1, file);
            dst += luma_stride;
        }

        if (OT_PIXEL_FORMAT_YUV_400 == frame->pixel_format) {
            return TD_SUCCESS;
        } else if (OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420 == frame->pixel_format) {
            chroma_height = chroma_height >> 1;
        }
        if (sample_vgs_convert_chroma_planar_to_sp42x(
            file, chroma, luma_stride, chroma_width, chroma_height) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    } else {
        fread(luma, luma_stride * luma_height * 3 / 2, 1, file); /* Tile 64x16 size = stride x height * 3 / 2 */
    }
    return TD_SUCCESS;
}

static td_s32 sample_vgs_get_frame_from_file(sample_vgs_func_param *param, ot_vb_calc_cfg *in_img_vb_cal_config,
    ot_video_frame_info *frame_info)
{
    td_s32 ret = TD_FAILURE;
    td_char sz_in_file_name[FILE_NAME_LENGTH] = SAMPLE_VGS_READ_PATH;
    FILE *file_read = TD_NULL;

    if (param->in_img_vb_info->video_format == OT_VIDEO_FORMAT_TILE_64x16) {
        if (snprintf_s(sz_in_file_name, FILE_NAME_LENGTH, FILE_NAME_LENGTH - 1,
            "%s", SAMPLE_VGS_READ_TILE_PATH) == -1) {
            sample_print("set input file name fail\n");
            goto EXIT;
        }
    }

    file_read = fopen(sz_in_file_name, "rb");
    if (file_read == TD_NULL) {
        sample_print("can't open file %s\n", sz_in_file_name);
        goto EXIT;
    }

    ret = sample_vgs_get_frame_vb(param->in_img_vb_info, in_img_vb_cal_config, frame_info, &g_in_img_vb_info);
    if (ret != TD_SUCCESS) {
        goto EXIT1;
    }

    ret = sample_vgs_read_file_to_sp42_x(file_read, &frame_info->video_frame);
    if (ret != TD_SUCCESS) {
        goto EXIT2;
    } else {
        goto EXIT1;
    }

EXIT2:
    ss_mpi_sys_munmap(frame_info->video_frame.header_virt_addr[0], in_img_vb_cal_config->vb_size);
    ss_mpi_vb_release_blk(g_in_img_vb_info.vb_handle);
    g_in_img_vb_info.vb_used = TD_FALSE;
EXIT1:
    if (fclose(file_read) != 0 && ret == TD_SUCCESS) {
        ret = TD_FAILURE;
    }
EXIT:
    return ret;
}

static td_s32 sample_vgs_convert_chroma_sp42x_to_planar(FILE *file, td_u8 *chroma_data,
    td_u32 luma_stride, td_u32 chroma_width, td_u32 chroma_height)
{
    td_u8 *planar_buffer = TD_NULL;
    td_u8 *dst = TD_NULL;
    td_u32 row;
    td_u32 list;

    if (chroma_width == 0) {
        return TD_FAILURE;
    }
    planar_buffer = (td_u8 *)malloc(chroma_width);
    if (planar_buffer == TD_NULL) {
        sample_print("vgs malloc failed!.\n");
        return TD_FAILURE;
    }

    if (memset_s(planar_buffer, chroma_width, 0, chroma_width) != EOK) {
        sample_print("vgs memset_s failed!\n");
        free(planar_buffer);
        planar_buffer = TD_NULL;
        return TD_FAILURE;
    }

    /* U */
    for (row = 0; row < chroma_height; ++row) {
        dst = chroma_data + row * luma_stride + 1;
        for (list = 0; list < chroma_width; ++list) {
            *(planar_buffer + list) = *dst;
            dst += 2; /* traverse 2 steps away to the next U-component data */
        }
        if (fwrite(planar_buffer, 1, chroma_width, file) != chroma_width) {
            free(planar_buffer);
            return TD_FAILURE;
        }
    }

    /* V */
    for (row = 0; row < chroma_height; ++row) {
        dst = chroma_data + row * luma_stride;
        for (list = 0; list < chroma_width; ++list) {
            *(planar_buffer + list) = *dst;
            dst += 2; /* traverse 2 steps away to the next V-component data */
        }
        if (fwrite(planar_buffer, 1, chroma_width, file) != chroma_width) {
            free(planar_buffer);
            return TD_FAILURE;
        }
    }

    free(planar_buffer);
    planar_buffer = TD_NULL;
    return TD_SUCCESS;
}

static td_s32 sample_vgs_save_sp42_x_to_planar(FILE *file, ot_video_frame *frame)
{
    td_u32 luma_width = frame->width;
    td_u32 chroma_width = luma_width >> 1;
    td_u32 luma_height = frame->height;
    td_u32 chroma_height = luma_height;
    td_u32 luma_stride = frame->stride[0];
    td_u8 *luma = (td_u8*)(td_uintptr_t)frame->virt_addr[0];
    td_u8 *chroma = (td_u8*)(td_uintptr_t)frame->virt_addr[1];

    td_u8 *dst = TD_NULL;
    td_u32 row;

    /* Y */
    dst = luma;
    for (row = 0; row < luma_height; ++row) {
        if (fwrite(dst, 1, luma_width, file) != luma_width) {
            return TD_FAILURE;
        }
        dst += luma_stride;
    }

    if (OT_PIXEL_FORMAT_YUV_400 == frame->pixel_format) {
        return TD_SUCCESS;
    } else if (OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420 == frame->pixel_format) {
        chroma_height = chroma_height >> 1;
    }

    if (sample_vgs_convert_chroma_sp42x_to_planar(
        file, chroma, luma_stride, chroma_width, chroma_height) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_vgs_set_color_format(ot_pixel_format pixel_format, osd_surface *surface)
{
    if (OT_PIXEL_FORMAT_ARGB_1555 == pixel_format) {
        surface->color_format = OSD_COLOR_FORMAT_RGB1555;
    } else if (OT_PIXEL_FORMAT_ARGB_4444 == pixel_format) {
        surface->color_format = OSD_COLOR_FORMAT_RGB4444;
    } else if (OT_PIXEL_FORMAT_ARGB_8888 == pixel_format) {
        surface->color_format = OSD_COLOR_FORMAT_RGB8888;
    } else {
        sample_print("pixel format is not support!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_vgs_load_bmp(const td_char *sz_file_name, ot_bmp *bitmap,
    const sample_vgs_osd_load_bmp_info *load_info)
{
    osd_surface surface;
    osd_bit_map_file_header bmp_file_header;
    osd_bit_map_info bmp_info;
    td_u32 i;
    td_u32 j;
    td_u16 *temp = TD_NULL;
    canvas_size_info canvas_size;

    canvas_size.height = load_info->osd_size.height;
    canvas_size.width = load_info->osd_size.width;
    canvas_size.stride = load_info->stride;

    if (get_bmp_info(sz_file_name, &bmp_file_header, &bmp_info) < 0) {
        sample_print("get_bmp_info err!\n");
        return TD_FAILURE;
    }

    if (bitmap->data == TD_NULL) {
        sample_print("bitmap's data is null!\n");
        return TD_FAILURE;
    }

    if (sample_vgs_set_color_format(load_info->img_pixel_format, &surface) < 0) {
        sample_print("pixel format is not support!\n");
        return TD_FAILURE;
    }

    if (create_surface_by_canvas(sz_file_name, &surface, (td_u8*)bitmap->data, &canvas_size)) {
        return TD_FAILURE;
    }

    bitmap->width = surface.width;
    bitmap->height = surface.height;
    bitmap->pixel_format = load_info->img_pixel_format;

    temp = (td_u16*)bitmap->data;

    if (load_info->fill_en != TD_TRUE) {
        return TD_SUCCESS;
    }

    for (i = 0; i < bitmap->height; ++i) {
        for (j = 0; j < bitmap->width; ++j) {
            if (load_info->fill_color == *temp) {
                *temp &= 0x7FFF;
            }
            temp++;
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_vgs_prepare_osd_info(ot_vgs_osd *vgs_add_osd, ot_rgn_handle *rgn_handle)
{
    td_s32 ret;

    ot_rgn_attr rgn_attr;
    ot_rgn_canvas_info rgn_canvas_info;
    ot_bmp bitmap;
    sample_vgs_osd_load_bmp_info load_info = {0};

    rgn_attr.type = OT_RGN_OVERLAYEX;
    rgn_attr.attr.overlayex.pixel_format = vgs_add_osd->pixel_format;
    rgn_attr.attr.overlayex.bg_color = vgs_add_osd->bg_color;
    rgn_attr.attr.overlayex.size.width = vgs_add_osd->rect.width;
    rgn_attr.attr.overlayex.size.height = vgs_add_osd->rect.height;
    rgn_attr.attr.overlayex.canvas_num = 2; /* 2 canvas */
    ret = ss_mpi_rgn_create(*rgn_handle, &rgn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_rgn_create failed, ret:0x%x", ret);
        return ret;
    }

    ret = ss_mpi_rgn_get_canvas_info(*rgn_handle, &rgn_canvas_info);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_rgn_get_canvas_info failed, ret:0x%x", ret);
        return ret;
    }

    bitmap.data = rgn_canvas_info.virt_addr;
    load_info.fill_en = TD_FALSE;
    load_info.fill_color = 0x0;
    load_info.osd_size.width = vgs_add_osd->rect.width;
    load_info.osd_size.height = vgs_add_osd->rect.height;
    load_info.stride = rgn_canvas_info.stride;
    load_info.img_pixel_format = vgs_add_osd->pixel_format;
    ret = sample_vgs_load_bmp(SAMPLE_VGS_BITMAP_PATH, &bitmap, &load_info);
    if (ret != TD_SUCCESS) {
        sample_print("sample_vgs_load_bmp failed, ret:0x%x", ret);
        return ret;
    }

    ret = ss_mpi_rgn_update_canvas(*rgn_handle);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_rgn_update_canvas failed, ret:0x%x", ret);
        return ret;
    }

    vgs_add_osd->phys_addr = rgn_canvas_info.phys_addr;
    vgs_add_osd->stride = rgn_canvas_info.stride;

    return ret;
}

static td_void sample_vgs_get_clut_data(td_u32 i, td_u32 j, td_u8 clut_value, td_u8 *clut_data)
{
    if ((i > 50 && i < 150) && (j > 50 && j < 150)) { /* 50-150 set alpha 255 */
        *clut_data = 0xFF; /* alpha 255 */
    } else {
        *clut_data = clut_value;
    }
    return;
}

static td_s32 sample_vgs_get_clut2_data(const ot_rgn_canvas_info rgn_canvas_info)
{
    td_u8 *clut_data = TD_NULL;
    td_u8 clut_value;
    td_u32 i;
    td_u32 j;

    if ((rgn_canvas_info.size.height - 1) * rgn_canvas_info.stride + (rgn_canvas_info.size.width - 1) / 4 > /* para 4 */
        rgn_canvas_info.size.height * rgn_canvas_info.stride) {
        printf("osd info exceeds canvas buffer size!");
        return OT_INVALID_VALUE;
    }

    for (i = 0; i < rgn_canvas_info.size.height; i++) {
        for (j = 0; j < rgn_canvas_info.size.width; j++) {
            clut_value = ((j / 64) << 6) + ((j / 64) << 4) + ((j / 64) << 2) + j / 64; /* 2,4,6,64 are params */
            clut_data = rgn_canvas_info.virt_addr + i * rgn_canvas_info.stride + j / 4; /* 4: CLut2 2Bit */
            sample_vgs_get_clut_data(i, j, clut_value, clut_data);
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_vgs_get_clut4_data(const ot_rgn_canvas_info rgn_canvas_info)
{
    td_u8 *clut_data = TD_NULL;
    td_u8 clut_value;
    td_u32 i;
    td_u32 j;

    if ((rgn_canvas_info.size.height - 1) * rgn_canvas_info.stride + (rgn_canvas_info.size.width - 1) / 2 > /* para 2 */
        rgn_canvas_info.size.height * rgn_canvas_info.stride) {
        printf("osd info exceeds canvas buffer size!");
        return OT_INVALID_VALUE;
    }

    for (i = 0; i < rgn_canvas_info.size.height; i++) {
        for (j = 0; j < rgn_canvas_info.size.width; j++) {
            clut_value = ((j / 16) << 4) + j / 16; /* 4,16 are parameters */
            clut_data = rgn_canvas_info.virt_addr + i * rgn_canvas_info.stride + j / 2; /* 2: CLut4 4Bit */
            sample_vgs_get_clut_data(i, j, clut_value, clut_data);
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_vgs_set_osd_info(const ot_rgn_canvas_info rgn_canvas_info, ot_vgs_osd *vgs_add_osd)
{
    if (vgs_add_osd->pixel_format == OT_PIXEL_FORMAT_ARGB_CLUT2) {
        vgs_add_osd->clut[0] = 0xFF0000FF;
        vgs_add_osd->clut[1] = 0x8000FF00;
        vgs_add_osd->clut[2] = 0x80FF0000; /* OSD clut 2 */
        vgs_add_osd->clut[3] = 0x00FFFFFF; /* OSD clut 3 */

        return sample_vgs_get_clut2_data(rgn_canvas_info);
    } else { /* color data transformation parameters */
        vgs_add_osd->clut[0] = 0xFFFF0000;
        vgs_add_osd->clut[1] = 0xFF00FF00;
        vgs_add_osd->clut[2] = 0xFF0000FF; /* 2 OSD clut data index */
        vgs_add_osd->clut[3] = 0xFFFFFF00; /* 3 OSD clut data index */
        vgs_add_osd->clut[4] = 0xFFFF00FF; /* 4 OSD clut data index */
        vgs_add_osd->clut[5] = 0xFF00FFFF; /* 5 OSD clut data index */
        vgs_add_osd->clut[6] = 0x7FFF0000; /* 6 OSD clut data index */
        vgs_add_osd->clut[7] = 0x7F00FF00; /* 7 OSD clut data index */
        vgs_add_osd->clut[8] = 0x7F0000FF; /* 8 OSD clut data index */
        vgs_add_osd->clut[9] = 0x7F7F7F00; /* 9 OSD clut data index */
        vgs_add_osd->clut[10] = 0x7F7F007F; /* 10 OSD clut data index */
        vgs_add_osd->clut[11] = 0x7F007F7F; /* 11 OSD clut data index */
        vgs_add_osd->clut[12] = 0x7FFFFFFF; /* 12 OSD clut data index */
        vgs_add_osd->clut[13] = 0x7F7F7F7F; /* 13 OSD clut data index */
        vgs_add_osd->clut[14] = 0xFFFFFFFF; /* 14 OSD clut data index */
        vgs_add_osd->clut[15] = 0x00FFFFFF; /* 15 OSD clut data index */

        return sample_vgs_get_clut4_data(rgn_canvas_info);
    }
}

static td_s32 sample_vgs_prepare_osd_clut_info(ot_vgs_osd *vgs_add_osd, ot_rgn_handle *rgn_handle)
{
    td_s32 ret;
    ot_rgn_attr rgn_attr;
    ot_rgn_canvas_info rgn_canvas_info;
    ot_size size;

    size.width = vgs_add_osd->rect.width;
    size.height = vgs_add_osd->rect.height;

    rgn_attr.type = OT_RGN_OVERLAYEX;
    rgn_attr.attr.overlayex.pixel_format = vgs_add_osd->pixel_format;
    rgn_attr.attr.overlayex.bg_color = vgs_add_osd->bg_color;
    rgn_attr.attr.overlayex.size.width = size.width;
    rgn_attr.attr.overlayex.size.height = size.height;
    rgn_attr.attr.overlayex.canvas_num = 1;

    ret = ss_mpi_rgn_create(*rgn_handle, &rgn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_rgn_create failed, ret:0x%x", ret);
        return ret;
    }

    ret = ss_mpi_rgn_get_canvas_info(*rgn_handle, &rgn_canvas_info);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_rgn_get_canvas_info failed, ret:0x%x", ret);
        return ret;
    }

    ret = sample_vgs_set_osd_info(rgn_canvas_info, vgs_add_osd);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_rgn_set_osd_info failed, ret:0x%x", ret);
        return ret;
    }

    ret = ss_mpi_rgn_update_canvas(*rgn_handle);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_rgn_update_canvas failed, ret:0x%x", ret);
        return ret;
    }

    vgs_add_osd->phys_addr = rgn_canvas_info.phys_addr;
    vgs_add_osd->stride = rgn_canvas_info.stride;

    return ret;
}

static td_s32 sample_vgs_add_osd_task(const sample_vgs_func_param *param, ot_vgs_handle h_handle,
    ot_vgs_task_attr *vgs_task_attr)
{
    td_s32 ret;
    ret = sample_vgs_prepare_osd_info(&param->vgs_add_osd[SAMPLE_VGS_OSD_NORM],
        param->rgn_handle[SAMPLE_VGS_OSD_NORM]);
    if (ret != TD_SUCCESS) {
        sample_print("sample_vgs_prepare_osd_info failed, ret:0x%x", ret);
        return ret;
    }

    ret = sample_vgs_prepare_osd_clut_info(&param->vgs_add_osd[SAMPLE_VGS_OSD_CLUT2],
        param->rgn_handle[SAMPLE_VGS_OSD_CLUT2]);
    if (ret != TD_SUCCESS) {
        sample_print("sample_vgs_prepare_osd_clut2 failed, ret:0x%x", ret);
        return ret;
    }

    ret = sample_vgs_prepare_osd_clut_info(&param->vgs_add_osd[SAMPLE_VGS_OSD_CLUT4],
        param->rgn_handle[SAMPLE_VGS_OSD_CLUT4]);
    if (ret != TD_SUCCESS) {
        sample_print("sample_vgs_prepare_osd_clut4 failed, ret:0x%x", ret);
        return ret;
    }

    ret = ss_mpi_vgs_add_osd_task(h_handle, vgs_task_attr, param->vgs_add_osd, 3); /* add 3 OSD task */
    if (ret != TD_SUCCESS) {
        sample_print("mpi_vgs_add_osd_task failed, ret:0x%x", ret);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 sample_vgs_add_stitch_task(const sample_vgs_func_param *param, ot_vgs_handle h_handle,
    ot_vgs_stitch_task_attr *vgs_task_attr)
{
    td_s32 ret;
    ret = ss_mpi_vgs_add_stitch_task(h_handle, vgs_task_attr, param->stitch_num);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_vgs_add_stitch_task failed, ret:0x%x", ret);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 sample_vgs_add_case2_task(const sample_vgs_func_param *param, ot_vgs_handle h_handle,
    ot_vgs_task_attr *vgs_task_attr)
{
    td_s32 ret;
    if (param->cover) {
        ret = ss_mpi_vgs_add_cover_task(h_handle, vgs_task_attr, param->vgs_add_cover, 1);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_vgs_add_cover_task failed, ret:0x%x", ret);
            return ret;
        }
    }

    if (param->mosaic) {
        ret = ss_mpi_vgs_add_mosaic_task(h_handle, vgs_task_attr, param->vgs_add_mosaic, 1);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_vgs_add_mosaic_task failed, ret:0x%x", ret);
            return ret;
        }
    }

    if (param->corner_rect_en) {
        ret = ss_mpi_vgs_add_corner_rect_task(h_handle, vgs_task_attr,
            param->corner_rect, param->corner_rect_attr, 1);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_vgs_add_mosaic_task failed, ret:0x%x", ret);
            return ret;
        }
    }

    if (param->osd) {
        ret = sample_vgs_add_osd_task(param, h_handle, vgs_task_attr);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_vgs_add_task(const sample_vgs_func_param *param, ot_vgs_handle h_handle,
    ot_vgs_task_attr *vgs_task_attr)
{
    td_s32 ret;
    if (param->scale) {
        ret = ss_mpi_vgs_add_scale_task(h_handle, vgs_task_attr, *param->vgs_scl_coef_mode);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_vgs_add_scale_task failed, ret:0x%x", ret);
            return ret;
        }
    }
    ret = sample_vgs_add_case2_task(param, h_handle, vgs_task_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample_vgs_add_case2_task failed, ret:0x%x", ret);
        return ret;
    }

    if (param->draw_line) {
        ret = ss_mpi_vgs_add_draw_line_task(h_handle, vgs_task_attr, param->vgs_draw_line, 1);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_vgs_add_draw_line_task failed, ret:0x%x", ret);
            return ret;
        }
    }

    if (param->rotate) {
        ret = ss_mpi_vgs_add_rotation_task(h_handle, vgs_task_attr, *param->rotation_angle);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_vgs_add_rotation_task failed, ret:0x%x", ret);
            return ret;
        }
    }

    if (param->luma) {
        ret = ss_mpi_vgs_add_luma_task(h_handle, vgs_task_attr, param->luma_rect, param->luma_num, param->luma_data);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_vgs_add_draw_line_task failed, ret:0x%x", ret);
            return ret;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_vgs_get_in_out_frame(sample_vgs_func_param *param,
    ot_vgs_task_attr *vgs_task_attr, td_bool same_vb)
{
    ot_vb_calc_cfg in_img_vb_cal_config = {0};
    ot_vb_calc_cfg out_img_vb_cal_config = {0};
    td_s32 ret;

    sample_vgs_get_yuv_buffer_cfg(param->in_img_vb_info, &in_img_vb_cal_config);
    ret = sample_vgs_get_frame_from_file(param, &in_img_vb_cal_config, &vgs_task_attr->img_in);
    if (ret != TD_SUCCESS) {
        sample_print("sample_vgs_get_frame_from_file failed, ret:0x%x\n", ret);
        return TD_FAILURE;
    }

    if (same_vb) {
        if (memcpy_s(&vgs_task_attr->img_out, sizeof(ot_video_frame_info),
            &vgs_task_attr->img_in, sizeof(ot_video_frame_info)) != EOK) {
            sample_print("memcpy_s failed\n");
            return TD_FAILURE;
        }
    } else {
        sample_vgs_get_yuv_buffer_cfg(param->out_img_vb_info, &out_img_vb_cal_config);
        ret = sample_vgs_get_frame_vb(param->out_img_vb_info, &out_img_vb_cal_config,
            &vgs_task_attr->img_out, &g_out_img_vb_info);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_vgs_get_stitch_in_out_frame(sample_vgs_func_param *param, ot_vgs_stitch_task_attr *vgs_task_attr)
{
    ot_vb_calc_cfg in_img_vb_cal_config = {0};
    ot_vb_calc_cfg out_img_vb_cal_config = {0};
    td_s32 ret;
    td_u32 i;
    td_u32 stitch_num;

    stitch_num = (param->stitch_num > OT_VGS_STITCH_NUM) ? OT_VGS_STITCH_NUM : (param->stitch_num);
    sample_vgs_get_yuv_buffer_cfg(param->in_img_vb_info, &in_img_vb_cal_config);
    ret = sample_vgs_get_frame_from_file(param, &in_img_vb_cal_config, &vgs_task_attr->img_in[0]);
    if (ret != TD_SUCCESS) {
        sample_print("sample_vgs_get_frame_from_file failed, ret:0x%x\n", ret);
        return TD_FAILURE;
    }

    for (i = 1; i < stitch_num; i++) {
        /* img_in[i] can be different frame */
        if (memcpy_s(&vgs_task_attr->img_in[i], sizeof(ot_video_frame_info),
            &vgs_task_attr->img_in[0], sizeof(ot_video_frame_info)) != EOK) {
            sample_print("memcpy_s failed\n");
            return TD_FAILURE;
        }
    }

    sample_vgs_get_yuv_buffer_cfg(param->out_img_vb_info, &out_img_vb_cal_config);
    ret = sample_vgs_get_frame_vb(param->out_img_vb_info, &out_img_vb_cal_config,
        &vgs_task_attr->img_out, &g_out_img_vb_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 sample_vgs_init_sys_and_vb(const sample_vgs_func_param *param,
    td_bool same_vb, FILE **file_write)
{
    ot_vb_calc_cfg in_img_vb_cal_config = {0};
    ot_vb_calc_cfg out_img_vb_cal_config = {0};
    td_char sz_out_file_name[FILE_NAME_LENGTH];
    ot_vb_cfg vb_conf = {0};
    td_s32 ret;

    vb_conf.max_pool_cnt = same_vb ? 1 : 2; /* there are 2 pools in 1 vb */
    sample_vgs_get_yuv_buffer_cfg(param->in_img_vb_info, &in_img_vb_cal_config);
    vb_conf.common_pool[0].blk_size = in_img_vb_cal_config.vb_size;
    vb_conf.common_pool[0].blk_cnt = 2; /* there are 2 blks in 1 pool */

    if (!same_vb) {
        sample_vgs_get_yuv_buffer_cfg(param->out_img_vb_info, &out_img_vb_cal_config);
        vb_conf.common_pool[1].blk_size = out_img_vb_cal_config.vb_size;
        vb_conf.common_pool[1].blk_cnt = 2; /* there are 2 blks in 1 pool */
    }

    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_sys_init failed, ret:0x%x\n", ret);
        return ret;
    }

    if (same_vb) {
        if (snprintf_s(sz_out_file_name, FILE_NAME_LENGTH, FILE_NAME_LENGTH - 1,
            "%s/vgs_sample%d_%dx%d_p420.yuv", SAMPLE_VGS_SAVE_PATH,
            param->sample_num, param->in_img_vb_info->width, param->in_img_vb_info->height) == -1) {
            sample_print("set output file name fail!\n");
            return TD_FAILURE;
        }
    } else {
        if (snprintf_s(sz_out_file_name, FILE_NAME_LENGTH, FILE_NAME_LENGTH - 1,
            "%s/vgs_sample%d_%dx%d_p420.yuv", SAMPLE_VGS_SAVE_PATH,
            param->sample_num, param->out_img_vb_info->width, param->out_img_vb_info->height) == -1) {
            sample_print("set output file name fail!\n");
            return TD_FAILURE;
        }
    }

    *file_write = fopen(sz_out_file_name, "w+");
    if (*file_write == TD_NULL) {
        sample_print("can't open file %s\n", sz_out_file_name);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void sample_vgs_common_function_exit_release(const sample_vgs_func_param *param,
    ot_vgs_task_attr *vgs_task_attr, td_bool same_vb, FILE **file_write)
{
    td_u32 i;
    ot_vb_calc_cfg in_img_vb_cal_config = {0};
    ot_vb_calc_cfg out_img_vb_cal_config = {0};
    if (!same_vb && g_out_img_vb_info.vb_used == TD_TRUE) {
        sample_vgs_get_yuv_buffer_cfg(param->out_img_vb_info, &out_img_vb_cal_config);
        ss_mpi_sys_munmap(vgs_task_attr->img_out.video_frame.header_virt_addr[0], out_img_vb_cal_config.vb_size);
        ss_mpi_vb_release_blk(g_out_img_vb_info.vb_handle);
        g_out_img_vb_info.vb_used = TD_FALSE;
    }
    if (*file_write != TD_NULL) {
        fclose(*file_write);
        *file_write = TD_NULL;
    }
    if (g_in_img_vb_info.vb_used == TD_TRUE) {
        sample_vgs_get_yuv_buffer_cfg(param->in_img_vb_info, &in_img_vb_cal_config);
        ss_mpi_sys_munmap(vgs_task_attr->img_in.video_frame.header_virt_addr[0], in_img_vb_cal_config.vb_size);
        ss_mpi_vb_release_blk(g_in_img_vb_info.vb_handle);
        g_in_img_vb_info.vb_used = TD_FALSE;
    }
    for (i = 0; i < SAMPLE_VGS_OSD_BUTT; i++) {
        if (param->rgn_handle[i] != TD_NULL) {
            ss_mpi_rgn_destroy(*param->rgn_handle[i]);
        }
    }
}

static td_void sample_vgs_common_function_stitch_exit_release(const sample_vgs_func_param *param,
    ot_vgs_stitch_task_attr *vgs_task_attr, td_bool same_vb, FILE **file_write)
{
    ot_vb_calc_cfg in_img_vb_cal_config = {0};
    ot_vb_calc_cfg out_img_vb_cal_config = {0};
    if (!same_vb && g_out_img_vb_info.vb_used == TD_TRUE) {
        sample_vgs_get_yuv_buffer_cfg(param->out_img_vb_info, &out_img_vb_cal_config);
        ss_mpi_sys_munmap(vgs_task_attr->img_out.video_frame.header_virt_addr[0], out_img_vb_cal_config.vb_size);
        ss_mpi_vb_release_blk(g_out_img_vb_info.vb_handle);
        g_out_img_vb_info.vb_used = TD_FALSE;
    }
    if (*file_write != TD_NULL) {
        fclose(*file_write);
        *file_write = TD_NULL;
    }
    if (g_in_img_vb_info.vb_used == TD_TRUE) {
        sample_vgs_get_yuv_buffer_cfg(param->in_img_vb_info, &in_img_vb_cal_config);
        ss_mpi_sys_munmap(vgs_task_attr->img_in[0].video_frame.header_virt_addr[0], in_img_vb_cal_config.vb_size);
        ss_mpi_vb_release_blk(g_in_img_vb_info.vb_handle);
        g_in_img_vb_info.vb_used = TD_FALSE;
    }
}

static td_s32 sample_vgs_common_function(sample_vgs_func_param *param)
{
    td_s32 ret;
    td_bool same_vb = TD_FALSE;
    ot_vgs_handle h_handle = -1;
    ot_vgs_task_attr vgs_task_attr = {0};
    FILE *file_write = TD_NULL;

    if (param == TD_NULL || param->in_img_vb_info == TD_NULL) {
        return TD_FAILURE;
    }

    if (param->in_img_vb_info != TD_NULL && param->out_img_vb_info == TD_NULL) {
        same_vb = TD_TRUE;
    }

    /* step1: init SYS and common VB */
    ret = sample_vgs_init_sys_and_vb(param, same_vb, &file_write);
    if (ret != TD_SUCCESS) {
        goto exit_and_release;
    }

    /* step2: get frame */
    ret = sample_vgs_get_in_out_frame(param, &vgs_task_attr, same_vb);
    if (ret != TD_SUCCESS) {
        goto exit_and_release;
    }

    /* step3: create VGS job */
    ret = ss_mpi_vgs_begin_job(&h_handle);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_vgs_begin_job failed, ret:0x%x", ret);
        goto exit_and_release;
    }

    /* step4: add VGS task */
    ret = sample_vgs_add_task(param, h_handle, &vgs_task_attr);
    if (ret != TD_SUCCESS) {
        ss_mpi_vgs_cancel_job(h_handle);
        goto exit_and_release;
    }

    /* step5: start VGS work */
    ret = ss_mpi_vgs_end_job(h_handle);
    if (ret != TD_SUCCESS) {
        ss_mpi_vgs_cancel_job(h_handle);
        sample_print("mpi_vgs_end_job failed, ret:0x%x", ret);
        goto exit_and_release;
    }

    /* step6: save the frame to file */
    ret = sample_vgs_save_sp42_x_to_planar(file_write, &vgs_task_attr.img_out.video_frame);
    if (ret != TD_SUCCESS) {
        goto exit_and_release;
    }
    if (fflush(file_write) != 0) {
        ret = TD_FAILURE;
    }

    /* step7: exit */
exit_and_release:
    sample_vgs_common_function_exit_release(param, &vgs_task_attr, same_vb, &file_write);
    sample_comm_sys_exit();
    return ret;
}

static td_s32 sample_vgs_stitch_function(sample_vgs_func_param *param)
{
    td_s32 ret;
    td_bool same_vb = TD_FALSE;
    ot_vgs_handle h_handle = -1;
    ot_vgs_stitch_task_attr vgs_stitch_task_attr = {0};
    FILE *file_write = TD_NULL;

    if (param == TD_NULL || param->in_img_vb_info == TD_NULL) {
        return TD_FAILURE;
    }

    if (param->in_img_vb_info != TD_NULL && param->out_img_vb_info == TD_NULL) {
        same_vb = TD_TRUE;
    }

    /* step1: init SYS and common VB */
    ret = sample_vgs_init_sys_and_vb(param, same_vb, &file_write);
    if (ret != TD_SUCCESS) {
        goto exit_and_release;
    }

    /* step2: get frame */
    ret = sample_vgs_get_stitch_in_out_frame(param, &vgs_stitch_task_attr);
    if (ret != TD_SUCCESS) {
        goto exit_and_release;
    }

    /* step3: create VGS job */
    ret = ss_mpi_vgs_begin_job(&h_handle);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_vgs_begin_job failed, ret:0x%x", ret);
        goto exit_and_release;
    }

    /* step4: add VGS task */
    ret = sample_vgs_add_stitch_task(param, h_handle, &vgs_stitch_task_attr);
    if (ret != TD_SUCCESS) {
        ss_mpi_vgs_cancel_job(h_handle);
        goto exit_and_release;
    }

    /* step5: start VGS work */
    ret = ss_mpi_vgs_end_job(h_handle);
    if (ret != TD_SUCCESS) {
        ss_mpi_vgs_cancel_job(h_handle);
        sample_print("mpi_vgs_end_job failed, ret:0x%x", ret);
        goto exit_and_release;
    }

    /* step6: save the frame to file */
    ret = sample_vgs_save_sp42_x_to_planar(file_write, &vgs_stitch_task_attr.img_out.video_frame);
    if (ret != TD_SUCCESS) {
        goto exit_and_release;
    }
    if (fflush(file_write) != 0) {
        ret = TD_FAILURE;
    }

    /* step7: exit */
exit_and_release:
    sample_vgs_common_function_stitch_exit_release(param, &vgs_stitch_task_attr, same_vb, &file_write);
    sample_comm_sys_exit();
    return ret;
}

static td_s32 sample_vgs_scale(td_void)
{
    td_s32 ret;
    sample_vgs_func_param vgs_func_param = {0};
    sample_vb_base_info in_img_vb_info;
    sample_vb_base_info out_img_vb_info;
    ot_vgs_scale_coef_mode vgs_scl_coef_mode = OT_VGS_SCALE_COEF_NORM;

    in_img_vb_info.video_format = OT_VIDEO_FORMAT_TILE_64x16;
    in_img_vb_info.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    in_img_vb_info.width = 1280; /* 1280:720P width */
    in_img_vb_info.height = 720; /* 720:720P height */
    in_img_vb_info.align = 0;
    in_img_vb_info.compress_mode = OT_COMPRESS_MODE_NONE;

    if (memcpy_s(&out_img_vb_info, sizeof(sample_vb_base_info), &in_img_vb_info, sizeof(sample_vb_base_info)) != EOK) {
        sample_print("memcpy_s failed\n");
        return TD_FAILURE;
    }
    out_img_vb_info.video_format = OT_VIDEO_FORMAT_LINEAR;
    out_img_vb_info.width = 720; /* 720:D1 width */
    out_img_vb_info.height = 576; /* 576:D1 height */

    vgs_func_param.scale = TD_TRUE;
    vgs_func_param.vgs_scl_coef_mode = &vgs_scl_coef_mode;
    vgs_func_param.in_img_vb_info = &in_img_vb_info;
    vgs_func_param.out_img_vb_info = &out_img_vb_info;
    vgs_func_param.sample_num = 0;

    ret = sample_vgs_common_function(&vgs_func_param);
    if (ret != TD_SUCCESS) {
        sample_print("VGS sample %d failed, ret:0x%x", vgs_func_param.sample_num, ret);
    }

    return ret;
}

static td_void sample_vgs_case2_set_osd_stat(ot_vgs_osd *vgs_add_osd, td_u32 max_osd_cnt,
                                             ot_rgn_handle *rgn_handle, td_u32 handle_cnt)
{
    td_u32 i;

    for (i = 0; i < max_osd_cnt && i < handle_cnt; i++) {
        vgs_add_osd[i].rect.x = 500 + i * 300; /* cover x-component starts from 500 at a step of 300 */
        vgs_add_osd[i].rect.y = 100 + i * 200; /* cover y-component starts from 100 at a step of 200 */
        vgs_add_osd[i].rect.width = 256; /* cover's size is 256x200 */
        vgs_add_osd[i].rect.height = 200; /* cover's size is 256x200 */
        vgs_add_osd[i].bg_color = 0x0;
        vgs_add_osd[i].pixel_format = OT_PIXEL_FORMAT_ARGB_CLUT2;
        vgs_add_osd[i].phys_addr = 0;
        vgs_add_osd[i].stride = 0;
        vgs_add_osd[i].bg_alpha = 255; /* alpha 255 */
        vgs_add_osd[i].fg_alpha = 128; /* alpha 128 */
        vgs_add_osd[i].osd_inverted_color = OT_VGS_OSD_INVERTED_COLOR_NONE;
        rgn_handle[i] = i;
        switch (i) {
            case SAMPLE_VGS_OSD_NORM:
                vgs_add_osd[i].rect.width = 180; /* cover's size is 180x144 */
                vgs_add_osd[i].rect.height = 144; /* cover's size is 180x144 */
                vgs_add_osd[i].bg_color = 0xFF0000;
                vgs_add_osd[i].pixel_format = OT_PIXEL_FORMAT_ARGB_8888;
                break;
            case SAMPLE_VGS_OSD_CLUT2:
                vgs_add_osd[i].rect.width = 256; /* cover's size is 256x200 */
                vgs_add_osd[i].rect.height = 200; /* cover's size is 256x200 */
                vgs_add_osd[i].bg_color = 0x0;
                vgs_add_osd[i].pixel_format = OT_PIXEL_FORMAT_ARGB_CLUT2;
                break;
            case SAMPLE_VGS_OSD_CLUT4:
                vgs_add_osd[i].rect.width = 256; /* cover's size is 256x200 */
                vgs_add_osd[i].rect.height = 200; /* cover's size is 256x200 */
                vgs_add_osd[i].bg_color = 0x0;
                vgs_add_osd[i].pixel_format = OT_PIXEL_FORMAT_ARGB_CLUT4;
                break;
            default:
                printf("OSD type error!");
                break;
        }
    }
}

static td_void sample_vgs_case2_set_cover_mosaic_corner_rect(ot_cover *vgs_add_cover,
    ot_mosaic *vgs_add_mosaic, ot_corner_rect *corner_rect, ot_corner_rect_attr *corner_rect_attr)
{
    vgs_add_cover->type = OT_COVER_RECT;
    vgs_add_cover->color = 0x00FF00;
    vgs_add_cover->rect.x = 200; /* cover x-component is 200 */
    vgs_add_cover->rect.y = 300; /* cover y-component is 300 */
    vgs_add_cover->rect.width = 400; /* cover's size is 400x300 */
    vgs_add_cover->rect.height = 300; /* cover's size is 400x300 */

    vgs_add_mosaic->blk_size = OT_MOSAIC_BLK_SIZE_32;
    vgs_add_mosaic->rect.x = 1400; /* 1400 mosaic x position */
    vgs_add_mosaic->rect.y = 700; /* 700 mosaic y position */
    vgs_add_mosaic->rect.width = 400; /* mosaic's size is 400x300 */
    vgs_add_mosaic->rect.height = 300; /* mosaic's size is 400x300 */

    corner_rect->thick = 10; /* 10 corner rect thick */
    corner_rect->hor_len = 80; /* 80 corner rect horizontal length */
    corner_rect->ver_len = 40; /* 40 corner rect vertical length */
    corner_rect->rect.x = 1300; /* 1300 corner rect x position  */
    corner_rect->rect.y = 100; /* 100 corner rect y position */
    corner_rect->rect.width = 400; /* 400 corner rect width */
    corner_rect->rect.height = 300; /* 300 corner rect height */

    corner_rect_attr->color = 0xFFFF00;
    corner_rect_attr->corner_rect_type = OT_CORNER_RECT_TYPE_CORNER;
}

static td_s32 sample_vgs_cover_osd(td_void)
{
    td_s32 ret;
    ot_rgn_handle rgn_handle[SAMPLE_VGS_OSD_BUTT];
    sample_vgs_func_param vgs_func_param = {0};
    sample_vb_base_info in_img_vb_info;
    ot_vgs_osd vgs_add_osd[SAMPLE_VGS_OSD_BUTT];
    ot_cover vgs_add_cover;
    ot_mosaic vgs_add_mosaic;
    ot_corner_rect corner_rect;
    ot_corner_rect_attr corner_rect_attr;

    in_img_vb_info.video_format = OT_VIDEO_FORMAT_LINEAR;
    in_img_vb_info.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    in_img_vb_info.width = 1920; /* 1920:1080P width */
    in_img_vb_info.height = 1080; /* 1080:1080P height */
    in_img_vb_info.align = 0;
    in_img_vb_info.compress_mode = OT_COMPRESS_MODE_NONE;

    sample_vgs_case2_set_cover_mosaic_corner_rect(&vgs_add_cover, &vgs_add_mosaic,
        &corner_rect, &corner_rect_attr);
    sample_vgs_case2_set_osd_stat(vgs_add_osd, SAMPLE_VGS_OSD_BUTT, rgn_handle, SAMPLE_VGS_OSD_BUTT);

    vgs_func_param.cover = TD_TRUE;
    vgs_func_param.vgs_add_cover = &vgs_add_cover;

    vgs_func_param.mosaic = TD_TRUE;
    vgs_func_param.vgs_add_mosaic = &vgs_add_mosaic;

    vgs_func_param.corner_rect_en = TD_TRUE;
    vgs_func_param.corner_rect = &corner_rect;
    vgs_func_param.corner_rect_attr = &corner_rect_attr;

    vgs_func_param.osd = TD_TRUE;
    vgs_func_param.vgs_add_osd = vgs_add_osd;

    vgs_func_param.rgn_handle[SAMPLE_VGS_OSD_NORM] = &rgn_handle[SAMPLE_VGS_OSD_NORM];
    vgs_func_param.rgn_handle[SAMPLE_VGS_OSD_CLUT2] = &rgn_handle[SAMPLE_VGS_OSD_CLUT2];
    vgs_func_param.rgn_handle[SAMPLE_VGS_OSD_CLUT4] = &rgn_handle[SAMPLE_VGS_OSD_CLUT4];
    vgs_func_param.in_img_vb_info = &in_img_vb_info;
    vgs_func_param.out_img_vb_info = TD_NULL;
    vgs_func_param.sample_num = 1;

    ret = sample_vgs_common_function(&vgs_func_param);
    if (ret != TD_SUCCESS) {
        sample_print("VGS sample %d failed, ret:0x%x", vgs_func_param.sample_num, ret);
    }

    return ret;
}

static td_s32 sample_vgs_draw_line(td_void)
{
    td_s32 ret;
    sample_vgs_func_param vgs_func_param = {0};
    sample_vb_base_info in_img_vb_info;
    ot_vgs_line vgs_draw_line;

    in_img_vb_info.video_format = OT_VIDEO_FORMAT_LINEAR;
    in_img_vb_info.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    in_img_vb_info.width = 1920; /* 1920:1080P width */
    in_img_vb_info.height = 1080; /* 1080:1080P height */
    in_img_vb_info.align = 0;
    in_img_vb_info.compress_mode = OT_COMPRESS_MODE_NONE;

    vgs_draw_line.start_point.x = 50; /* line starts from 50,50 */
    vgs_draw_line.start_point.y = 50; /* line starts from 50,50 */
    vgs_draw_line.end_point.x = 1600; /* line ends at 1600,900 */
    vgs_draw_line.end_point.y = 900; /* line ends at 1600,900 */
    vgs_draw_line.thick = 2; /* line width 2 */
    vgs_draw_line.color = 0x0000FF;

    vgs_func_param.draw_line = TD_TRUE;
    vgs_func_param.vgs_draw_line = &vgs_draw_line;
    vgs_func_param.in_img_vb_info = &in_img_vb_info;
    vgs_func_param.out_img_vb_info = TD_NULL;
    vgs_func_param.sample_num = 2; /* line width 2 */

    ret = sample_vgs_common_function(&vgs_func_param);
    if (ret != TD_SUCCESS) {
        sample_print("VGS sample %d failed, ret:0x%x", vgs_func_param.sample_num, ret);
    }

    return ret;
}

static td_s32 sample_vgs_luma(td_void)
{
    td_s32 ret;
    sample_vgs_func_param vgs_func_param = {0};
    sample_vb_base_info in_img_vb_info;
    td_u32 i;
    ot_rect luma_rect[MAX_LUMA_NUM];
    td_u64 luma_data[MAX_LUMA_NUM];
    in_img_vb_info.video_format = OT_VIDEO_FORMAT_LINEAR;
    in_img_vb_info.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    in_img_vb_info.width = 1920; /* 1920:1080P width */
    in_img_vb_info.height = 1080; /* 1080:1080P height */
    in_img_vb_info.align = 0;
    in_img_vb_info.compress_mode = OT_COMPRESS_MODE_NONE;

    for (i = 0; i < MAX_LUMA_NUM; i++) {
        luma_rect[i].x = 100 + 50 * i; /* luma x-component starts from 100 at a step of 50 */
        luma_rect[i].y = 100 + 50 * i; /* luma y-component starts from 100 at a step of 50 */
        luma_rect[i].width = 40; /* 40x40 luma region */
        luma_rect[i].height = 40; /* 40x40 luma region */
    }
    if (memset_s(luma_data, sizeof(td_u64) * MAX_LUMA_NUM, 0, sizeof(td_u64) * MAX_LUMA_NUM) != EOK) {
        sample_print("vgs memset_s failed!\n");
        return TD_FAILURE;
    };
    vgs_func_param.luma = TD_TRUE;
    vgs_func_param.luma_rect = luma_rect;
    vgs_func_param.luma_data = luma_data;
    vgs_func_param.luma_num = MAX_LUMA_NUM;
    vgs_func_param.in_img_vb_info = &in_img_vb_info;
    vgs_func_param.out_img_vb_info = TD_NULL;
    vgs_func_param.sample_num = 4; /* 4 is parameter */

    ret = sample_vgs_common_function(&vgs_func_param);
    if (ret != TD_SUCCESS) {
        sample_print("VGS sample %d failed, ret:0x%x", vgs_func_param.sample_num, ret);
    }
    for (i = 0; i < MAX_LUMA_NUM; i++) {
        sample_print("VGS sample luma_rect: %d,luma_data:%llu\n", i, luma_data[i]);
    }

    return ret;
}

static td_s32 sample_vgs_rotate(td_void)
{
    td_s32 ret;
    sample_vgs_func_param vgs_func_param = {0};
    ot_rotation rotation_angle = OT_ROTATION_90;
    sample_vb_base_info in_img_vb_info;
    sample_vb_base_info out_img_vb_info;

    in_img_vb_info.video_format = OT_VIDEO_FORMAT_LINEAR;
    in_img_vb_info.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    in_img_vb_info.width = 1920; /* 1920:1080P width */
    in_img_vb_info.height = 1080; /* 1080:1080P height */
    in_img_vb_info.align = 0;
    in_img_vb_info.compress_mode = OT_COMPRESS_MODE_NONE;

    if (memcpy_s(&out_img_vb_info, sizeof(sample_vb_base_info), &in_img_vb_info, sizeof(sample_vb_base_info)) != EOK) {
        sample_print("memcpy_s failed\n");
        return TD_FAILURE;
    };
    out_img_vb_info.width = 1080; /* 1080:1080P height */
    out_img_vb_info.height = 1920; /* 1920:1080P width */

    vgs_func_param.rotate = TD_TRUE;
    vgs_func_param.rotation_angle = &rotation_angle;
    vgs_func_param.in_img_vb_info = &in_img_vb_info;
    vgs_func_param.out_img_vb_info = &out_img_vb_info;
    vgs_func_param.sample_num = 3; /* 3 rotation task */

    ret = sample_vgs_common_function(&vgs_func_param);
    if (ret != TD_SUCCESS) {
        sample_print("VGS sample %d failed, ret:0x%x", vgs_func_param.sample_num, ret);
    }

    return ret;
}

static td_s32 sample_vgs_stitch(td_void)
{
    td_s32 ret;
    sample_vgs_func_param vgs_func_param = {0};
    sample_vb_base_info in_img_vb_info;
    sample_vb_base_info out_img_vb_info;

    in_img_vb_info.video_format = OT_VIDEO_FORMAT_LINEAR;
    in_img_vb_info.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    in_img_vb_info.width = 1920; /* 1920:1080P width */
    in_img_vb_info.height = 1080; /* 1080:1080P height */
    in_img_vb_info.align = 0;
    in_img_vb_info.compress_mode = OT_COMPRESS_MODE_NONE;

    if (memcpy_s(&out_img_vb_info, sizeof(sample_vb_base_info), &in_img_vb_info, sizeof(sample_vb_base_info)) != EOK) {
        sample_print("memcpy_s failed\n");
        return TD_FAILURE;
    }
    out_img_vb_info.video_format = OT_VIDEO_FORMAT_LINEAR;
    out_img_vb_info.width = in_img_vb_info.width * 4; /* 4 pic nums */

    vgs_func_param.stitch = TD_TRUE;
    vgs_func_param.stitch_num = 4; /* 4 pic nums */

    vgs_func_param.in_img_vb_info = &in_img_vb_info;
    vgs_func_param.out_img_vb_info = &out_img_vb_info;
    vgs_func_param.sample_num = 5; /* 5 stitch sample index */

    ret = sample_vgs_stitch_function(&vgs_func_param);
    if (ret != TD_SUCCESS) {
        sample_print("VGS sample %d failed, ret:0x%x", vgs_func_param.sample_num, ret);
    }
    return ret;
}


static td_s32 sample_vgs_proc_task(td_s32 task_index)
{
    td_s32 ret;
    switch (task_index) {
        case 0: /* 0 scale sample */
            ret = sample_vgs_scale();
            break;
        case 1: /* 1 cover and osd sample */
            ret = sample_vgs_cover_osd();
            break;
        case 2: /* 2 draw line sample */
            ret = sample_vgs_draw_line();
            break;
        case 3: /* 3 rotate sample */
            ret = sample_vgs_rotate();
            break;
        case 4: /* 4 luma sample */
            ret = sample_vgs_luma();
            break;
        case 5: /* 5 stitch sample */
            ret = sample_vgs_stitch();
            break;
        default:
            ret = TD_FAILURE;
            break;
    }
    return ret;
}

/*
 * function : main()
 * description : video vgs sample
 */
#ifdef __LITEOS__
td_s32 app_main(td_s32 argc, td_char *argv[])
#else
td_s32 main(td_s32 argc, td_char *argv[])
#endif
{
    td_s32 ret = TD_FAILURE;
    td_s32 index;

    if (argc != 2) { /* 2:arv num */
        sample_vgs_usage(argv[0]);
        return ret;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arv len */
        sample_vgs_usage(argv[0]);
        return TD_SUCCESS;
    }
    if ((strlen(argv[1]) > 1) ||
        (strncmp(argv[1], "0", 1) && strncmp(argv[1], "1", 1) && strncmp(argv[1], "2", 1) &&
        strncmp(argv[1], "3", 1) && strncmp(argv[1], "4", 1) && strncmp(argv[1], "5", 1))) {
        sample_print("the index is invalid!\n");
        sample_vgs_usage(argv[0]);
        return TD_FAILURE;
    }

    g_vgs_sample_signal_flag = 0;

#ifndef __LITEOS__
    sample_sys_signal(&sample_vgs_handle_sig);
#endif
#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif
    index = atoi(argv[1]);
    ret = sample_vgs_proc_task(index);

    if (g_vgs_sample_signal_flag == 1) {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
        exit(-1);
    }

#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
    if (ret == TD_SUCCESS) {
        sample_print("vgs sample %d exit normally!\n", index);
    } else {
        sample_print("vgs sample %d exit abnormally!\n", index);
    }
    return ret;
}
