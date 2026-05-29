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
#include "sample_common_ive.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_sys.h"
#include "ot_common_vgs.h"
#include "ot_common_vi.h"
#include "ot_common_vo.h"
#include "ss_mpi_sys.h"

#define OT_SAMPLE_IVE_QUERY_SLEEP   100
#define OT_SAMPLE_IVE_MAX_WIDTH     4096
#define OT_SAMPLE_IVE_MAX_HEIGHT    4096

static td_bool g_is_mpi_init = TD_FALSE;

/*
 * function : Mpi check init
 */
td_s32 sample_common_ive_check_mpi_init(td_void)
{
    if (g_is_mpi_init == TD_FALSE) {
        if (sample_common_ive_mpi_init() != TD_SUCCESS) {
            sample_svp_trace_err("Ive mpi init failed!\n");
            return TD_FALSE;
        }
        g_is_mpi_init = TD_TRUE;
    }
    return TD_TRUE;
}

/*
 * function :mpi_init
 */
td_s32 sample_common_ive_mpi_init(td_void)
{
    td_s32 ret;

    ss_mpi_sys_exit();

    ret = ss_mpi_sys_init();
    if (ret != TD_SUCCESS) {
        sample_svp_trace_err("ss_mpi_sys_init fail,Error(%#x)\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

/*
 * function : Mpi exit
 */
td_void sample_common_ive_mpi_exit(td_void)
{
    if (g_is_mpi_init == TD_TRUE) {
        if (ss_mpi_sys_exit() != TD_SUCCESS) {
            sample_svp_trace_err("Sys exit failed!\n");
            return;
        }
    }
    g_is_mpi_init = TD_FALSE;
    sample_svp_trace_info("ive mpi exit ok!\n");
}

static td_void sample_comm_ive_get_loop_info(const ot_svp_img *img, ot_sample_rw_image_loop_info *loop_info)
{
    loop_info->ele_size = 1;
    loop_info->loop_c = 1;
    loop_info->loop_h[0] = img->height;
    switch (img->type) {
        case OT_SVP_IMG_TYPE_U8C1:
        case OT_SVP_IMG_TYPE_S8C1: {
        }
            break;
        case OT_SVP_IMG_TYPE_YUV420SP: {
            loop_info->ele_size = 1;
            loop_info->loop_c = OT_SAMPLE_IVE_IMAGE_CHN_TWO;
            loop_info->loop_h[1] = img->height / OT_SAMPLE_IVE_DIV_TWO;
        }
            break;
        case OT_SVP_IMG_TYPE_YUV422SP: {
            loop_info->loop_c = OT_SAMPLE_IVE_IMAGE_CHN_TWO;
            loop_info->loop_h[1] = img->height;
        }
            break;
        case OT_SVP_IMG_TYPE_U8C3_PACKAGE: {
            loop_info->ele_size = sizeof(td_u8) + sizeof(td_u16);
        }
            break;
        case OT_SVP_IMG_TYPE_U8C3_PLANAR: {
            loop_info->loop_c = OT_SAMPLE_IVE_IMAGE_CHN_THREE;
            loop_info->loop_h[1] = img->height;
            loop_info->loop_h[OT_SAMPLE_IVE_IMAGE_CHN_TWO] = img->height;
        }
            break;
        case OT_SVP_IMG_TYPE_S16C1:
        case OT_SVP_IMG_TYPE_U16C1: {
            loop_info->ele_size = sizeof(td_u16);
        }
            break;
        case OT_SVP_IMG_TYPE_U32C1:
        case OT_SVP_IMG_TYPE_S32C1: {
            loop_info->ele_size = sizeof(td_u32);
        }
            break;
        case OT_SVP_IMG_TYPE_S64C1:
        case OT_SVP_IMG_TYPE_U64C1: {
            loop_info->ele_size = sizeof(td_u64);
        }
            break;
        default:
            break;
    }
}

/*
 * function :Read file
 */
td_s32 sample_common_ive_read_file(ot_svp_img *img, FILE *fp)
{
    td_u8 *ptr_tmp = TD_NULL;
    td_u16 c;
    td_u16 h;
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    ot_sample_rw_image_loop_info loop_info = {0};

    sample_svp_check_exps_return(img == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_DEBUG, "img can't be null\n");
    sample_svp_check_exps_return(fp == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_DEBUG, "fp can't be null\n");

    ret = fgetc(fp);
    sample_svp_check_exps_return(ret == EOF, ret, SAMPLE_SVP_ERR_LEVEL_DEBUG, "fgetc fp failed!\n");
    if (feof(fp)) {
        sample_svp_trace_err("end of file!\n");
        ret = fseek(fp, 0, SEEK_SET);
        if (ret != 0) {
            sample_svp_trace_err("fseek failed!\n");
            return ret;
        }
    } else {
        ret = fseek(fp, -1, SEEK_CUR);
        if (ret != 0) {
            sample_svp_trace_err("fseek failed!\n");
            return ret;
        }
    }
    sample_comm_ive_get_loop_info(img, &loop_info);
    for (c = 0; (c < loop_info.loop_c) && (c < OT_SVP_IMG_STRIDE_NUM) && (c < OT_SVP_IMG_ADDR_NUM); c++) {
        ptr_tmp = sample_svp_convert_addr_to_ptr(td_u8, img->virt_addr[c]);
        sample_svp_check_exps_return(ptr_tmp == 0, OT_ERR_IVE_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_DEBUG, "ptr_tmp can't be 0\n");
        for (h = 0; h < loop_info.loop_h[c]; h++) {
            if (fread(ptr_tmp, img->width * loop_info.ele_size, 1, fp) != 1) {
                sample_svp_trace_err("Read file fail\n");
                return OT_ERR_IVE_ILLEGAL_PARAM;
            }
            ptr_tmp += img->stride[c] * loop_info.ele_size;
        }
    }

    return TD_SUCCESS;
}

/*
 * function :Write file
 */
td_s32 sample_common_ive_write_file(ot_svp_img *img, FILE *fp)
{
    td_u16 c;
    td_u16 h;
    td_u8 *ptr_tmp = TD_NULL;
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    ot_sample_rw_image_loop_info loop_info = {0};

    sample_svp_check_exps_return(img == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_DEBUG, "img can't be null\n");
    sample_svp_check_exps_return(fp == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_DEBUG, "fp can't be null\n");
    ret = OT_ERR_IVE_ILLEGAL_PARAM;
    sample_svp_check_exps_return((td_u64)img->phys_addr == 0, ret, SAMPLE_SVP_ERR_LEVEL_DEBUG,
        "img->phys_addr can't be 0\n");
    sample_svp_check_exps_return((td_u64)img->virt_addr == 0, ret, SAMPLE_SVP_ERR_LEVEL_DEBUG,
        "img->phys_addr can't be 0\n");

    sample_comm_ive_get_loop_info(img, &loop_info);
    for (c = 0; (c < loop_info.loop_c) && (c < OT_SVP_IMG_STRIDE_NUM) && (c < OT_SVP_IMG_ADDR_NUM); c++) {
        ptr_tmp = sample_svp_convert_addr_to_ptr(td_u8, img->virt_addr[c]);
        sample_svp_check_exps_return(ptr_tmp == 0, ret, SAMPLE_SVP_ERR_LEVEL_DEBUG, "ptr_tmp can't be 0\n");
        for (h = 0; h < loop_info.loop_h[c]; h++) {
            if (fwrite(ptr_tmp, img->width * loop_info.ele_size, 1, fp) != 1) {
                sample_svp_trace_err("Write file fail\n");
                return ret;
            }
            ptr_tmp += img->stride[c] * loop_info.ele_size;
        }
    }
    return TD_SUCCESS;
}

/*
 * function :Calc stride
 */
td_u32 sample_common_ive_calc_stride(td_u32 width, td_u8 align)
{
    sample_svp_check_exps_return(align == 0, OT_ERR_IVE_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_DEBUG, "align can't be 0\n");
    sample_svp_check_exps_return((width > OT_SAMPLE_IVE_MAX_WIDTH) || (width < 1), OT_ERR_IVE_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_DEBUG, "width(%u) must be in [1, %u]", width, OT_SAMPLE_IVE_MAX_WIDTH);
    return (width + (align - width % align) % align);
}

static td_void sample_comm_ive_get_thresh(ot_ive_ccblob *blob, td_u16 area_thr_step, td_u16 rect_max_num,
    td_u16 *thresh)
{
    td_u32 i;
    td_u16 num;
    td_u16 thr = blob->info.bits.cur_area_threshold;
    do {
        num = 0;
        thr += area_thr_step;
        for (i = 0; i < blob->info.bits.rgn_num; i++) {
            if (blob->rgn[i].area > thr) {
                num++;
            }
        }
    } while (num > rect_max_num);

    *thresh = thr;
}

/*
 * function : judge if rect is valid
 */
static td_void sample_common_ive_is_rect_valid(ot_sample_svp_rect_info *rect, td_u32 num, td_bool *valid)
{
    td_u32 j;
    td_u32 k;
    for (j = 0; j < (OT_POINT_NUM - 1); j++) {
        for (k = j + 1; k < OT_POINT_NUM; k++) {
            if ((rect->rect[num].point[j].x == rect->rect[num].point[k].x) &&
                (rect->rect[num].point[j].y == rect->rect[num].point[k].y)) {
                *valid = TD_FALSE;
                break;
            }
        }
    }
}

/*
 * function : Copy blob to rect
 */
td_s32 sample_common_ive_blob_to_rect(ot_ive_ccblob *blob, ot_sample_svp_rect_info *rect,
    td_u16 rect_max_num, td_u16 area_thr_step, ot_sample_src_dst_size src_dst_size)
{
    td_u16 num;
    td_u16 i;
    td_u16 thr = 0;
    td_bool valid;
    sample_svp_check_exps_return(blob == TD_NULL, OT_ERR_IVE_NULL_PTR,
        SAMPLE_SVP_ERR_LEVEL_DEBUG, "blob can't be null\n");
    sample_svp_check_exps_return(rect == TD_NULL, OT_ERR_IVE_NULL_PTR,
        SAMPLE_SVP_ERR_LEVEL_DEBUG, "rect can't be null\n");

    if (blob->info.bits.rgn_num > rect_max_num) {
        sample_comm_ive_get_thresh(blob, area_thr_step, rect_max_num, &thr);
    }

    num = 0;
    for (i = 0; i < blob->info.bits.rgn_num; i++) {
        if (blob->rgn[i].area <= thr) {
            continue;
        }
        sample_svp_check_exps_return(num > (OT_SVP_RECT_NUM - 1), TD_FAILURE,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "num is larger than %u\n", OT_SVP_RECT_NUM - 1);
        rect->rect[num].point[OT_SAMPLE_POINT_IDX_ZERO].x = (td_u32)((td_float)blob->rgn[i].left /
            (td_float)src_dst_size.src.width * (td_float)src_dst_size.dst.width) & (~1);
        rect->rect[num].point[OT_SAMPLE_POINT_IDX_ZERO].y = (td_u32)((td_float)blob->rgn[i].top /
            (td_float)src_dst_size.src.height * (td_float)src_dst_size.dst.height) & (~1);

        rect->rect[num].point[OT_SAMPLE_POINT_IDX_ONE].x = (td_u32)((td_float)blob->rgn[i].right /
            (td_float)src_dst_size.src.width * (td_float)src_dst_size.dst.width) & (~1);
        rect->rect[num].point[OT_SAMPLE_POINT_IDX_ONE].y = (td_u32)((td_float)blob->rgn[i].top /
            (td_float)src_dst_size.src.height * (td_float)src_dst_size.dst.height) & (~1);

        rect->rect[num].point[OT_SAMPLE_POINT_IDX_TWO].x = (td_u32)((td_float)blob->rgn[i].right /
            (td_float)src_dst_size.src.width * (td_float)src_dst_size.dst.width) & (~1);
        rect->rect[num].point[OT_SAMPLE_POINT_IDX_TWO].y = (td_u32)((td_float)blob->rgn[i].bottom /
            (td_float)src_dst_size.src.height * (td_float)src_dst_size.dst.height) & (~1);

        rect->rect[num].point[OT_SAMPLE_POINT_IDX_THREE].x = (td_u32)((td_float)blob->rgn[i].left /
            (td_float)src_dst_size.src.width * (td_float)src_dst_size.dst.width) & (~1);
        rect->rect[num].point[OT_SAMPLE_POINT_IDX_THREE].y = (td_u32)((td_float)blob->rgn[i].bottom /
            (td_float)src_dst_size.src.height * (td_float)src_dst_size.dst.height) & (~1);

        valid = TD_TRUE;
        sample_common_ive_is_rect_valid(rect, num, &valid);
        if (valid == TD_TRUE) {
            num++;
        }
    }

    rect->num = num;
    return TD_SUCCESS;
}


static td_s32 sample_comm_ive_set_image_addr(ot_svp_img *img, const ot_sample_rw_image_loop_info *loop_info,
    td_bool is_mmz_cached)
{
    td_u32 c;
    td_u32 size = 0;
    td_s32 ret;
    td_void *virt_addr = TD_NULL;
    for (c = 0; (c < loop_info->loop_c) && (c < OT_MAX_LOOP_IMG_H) && (c < OT_SVP_IMG_STRIDE_NUM); c++) {
        size += img->stride[0] * loop_info->loop_h[c] * loop_info->ele_size;
        img->stride[c] = img->stride[0];
    }

    if (is_mmz_cached == TD_FALSE) {
        ret = ss_mpi_sys_mmz_alloc((td_phys_addr_t *)&img->phys_addr[0], (td_void **)&virt_addr,
            TD_NULL, TD_NULL, size);
    } else {
        ret = ss_mpi_sys_mmz_alloc_cached((td_phys_addr_t *)&img->phys_addr[0], (td_void **)&virt_addr,
            TD_NULL, TD_NULL, size);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "mmz malloc fail\n");
    img->virt_addr[OT_SAMPLE_ADDR_IDX_ZERO] = sample_svp_convert_ptr_to_addr(td_u64, virt_addr);

    if (img->type != OT_SVP_IMG_TYPE_U8C3_PACKAGE) {
        for (c = 1; (c < loop_info->loop_c) && (c < OT_MAX_LOOP_IMG_H) && (c < OT_SVP_IMG_STRIDE_NUM); c++) {
            img->phys_addr[c] = img->phys_addr[c - 1] + img->stride[c - 1] * img->height;
            img->virt_addr[c] = img->virt_addr[c - 1] + img->stride[c - 1] * img->height;
        }
    } else {
        img->virt_addr[OT_SAMPLE_ADDR_IDX_ONE] = img->virt_addr[OT_SAMPLE_ADDR_IDX_ZERO] + 1;
        img->virt_addr[OT_SAMPLE_ADDR_IDX_TWO] = img->virt_addr[OT_SAMPLE_ADDR_IDX_ONE] + 1;
        img->phys_addr[OT_SAMPLE_ADDR_IDX_ONE] = img->phys_addr[OT_SAMPLE_ADDR_IDX_ZERO] + 1;
        img->phys_addr[OT_SAMPLE_ADDR_IDX_TWO] = img->phys_addr[OT_SAMPLE_ADDR_IDX_ONE] + 1;
    }
    return TD_SUCCESS;
}

/*
 * function : Create ive image
 */
static td_s32 sample_common_ive_create_image_flag(ot_svp_img *img, ot_svp_img_type type,
    td_u32 width, td_u32 height, td_bool is_mmz_cached)
{
    td_s32 ret = OT_ERR_IVE_ILLEGAL_PARAM;
    ot_sample_rw_image_loop_info loop_info = {0};
    sample_svp_check_exps_return(img == TD_NULL, OT_ERR_IVE_NULL_PTR,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "img can't be null\n");
    sample_svp_check_exps_return((type < 0) || (type >= OT_SVP_IMG_TYPE_BUTT), ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "type(%u) must be in [0, %u)!\n", type, OT_SVP_IMG_TYPE_BUTT);
    sample_svp_check_exps_return(width > OT_SAMPLE_IVE_MAX_WIDTH, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "width(%u) must be in [1, %u]!\n", width, OT_SAMPLE_IVE_MAX_WIDTH);
    sample_svp_check_exps_return(width > OT_SAMPLE_IVE_MAX_HEIGHT, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "width(%u) must be in [1, %u]!\n", width, OT_SAMPLE_IVE_MAX_HEIGHT);

    img->type = type;
    img->width = width;
    img->height = height;
    img->stride[0] = sample_common_ive_calc_stride(img->width, OT_IVE_ALIGN);

    switch (type) {
        case OT_SVP_IMG_TYPE_U8C1:
        case OT_SVP_IMG_TYPE_S8C1:
        case OT_SVP_IMG_TYPE_YUV420SP:
        case OT_SVP_IMG_TYPE_YUV422SP:
        case OT_SVP_IMG_TYPE_S16C1:
        case OT_SVP_IMG_TYPE_U16C1:
        case OT_SVP_IMG_TYPE_U8C3_PACKAGE:
        case OT_SVP_IMG_TYPE_S32C1:
        case OT_SVP_IMG_TYPE_U32C1:
        case OT_SVP_IMG_TYPE_S64C1:
        case OT_SVP_IMG_TYPE_U64C1: {
            sample_comm_ive_get_loop_info(img, &loop_info);
            ret = sample_comm_ive_set_image_addr(img, &loop_info, is_mmz_cached);
            sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Err:%#x,sample_comm_ive_set_image_addr failed\n", ret);
            break;
            }
        case OT_SVP_IMG_TYPE_YUV420P:
            break;
        case OT_SVP_IMG_TYPE_YUV422P:
            break;
        case OT_SVP_IMG_TYPE_S8C2_PACKAGE:
            break;
        case OT_SVP_IMG_TYPE_S8C2_PLANAR:
            break;
        case OT_SVP_IMG_TYPE_U8C3_PLANAR:
            break;
        default:
            break;
    }

    return TD_SUCCESS;
}

td_s32 sample_common_ive_create_image(ot_svp_img *img, ot_svp_img_type type,
    td_u32 width, td_u32 height)
{
    return sample_common_ive_create_image_flag(img, type, width, height, TD_FALSE);
}

/*
 * function : Create memory info
 */
td_s32 sample_common_ive_create_mem_info(ot_svp_mem_info *mem_info, td_u32 size)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    td_void *virt_addr = TD_NULL;

    sample_svp_check_exps_return(mem_info == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "mem_info can't be null\n");

    mem_info->size = size;
    ret = ss_mpi_sys_mmz_alloc((td_phys_addr_t *)&mem_info->phys_addr, (td_void **)&virt_addr, TD_NULL, TD_NULL, size);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Mmz Alloc fail,Error(%#x)\n", ret);
    mem_info->virt_addr = sample_svp_convert_ptr_to_addr(td_u64, virt_addr);

    return TD_SUCCESS;
}
/*
 * function : Create ive image by cached
 */
td_s32 sample_common_ive_create_image_by_cached(ot_svp_img *img, ot_svp_img_type type,
    td_u32 width, td_u32 height)
{
    return sample_common_ive_create_image_flag(img, type, width, height, TD_TRUE);
}

/*
 * function : Dma frame info to ive image
 */
td_s32 sample_common_ive_dma_image(ot_video_frame_info *frame_info, ot_svp_dst_img *dst,
    td_bool is_instant)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    ot_ive_handle handle;
    ot_svp_src_data src_data;
    ot_svp_dst_data dst_data;
    ot_ive_dma_ctrl ctrl = { OT_IVE_DMA_MODE_DIRECT_COPY, 0, 0, 0, 0 };
    td_bool is_finish = TD_FALSE;
    td_bool is_block = TD_TRUE;

    sample_svp_check_exps_return(frame_info == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "frame_info can't be null\n");
    sample_svp_check_exps_return(dst == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "dst can't be null\n");
    sample_svp_check_exps_return(frame_info->video_frame.virt_addr == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "frame_info->video_frame.virt_addr can't be null\n");
    ret = OT_ERR_IVE_ILLEGAL_PARAM;
    sample_svp_check_exps_return((td_u64)frame_info->video_frame.phys_addr == 0, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "frame_info->video_frame.virt_addr can't be 0\n");
    sample_svp_check_exps_return((td_u64)dst->virt_addr == 0, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "dst->virt_addr can't be 0\n");
    sample_svp_check_exps_return((td_u64)dst->phys_addr == 0, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "dst->phys_addr can't be 0\n");

    /* fill src */
    src_data.virt_addr = sample_svp_convert_ptr_to_addr(td_u64, frame_info->video_frame.virt_addr[0]);
    src_data.phys_addr = frame_info->video_frame.phys_addr[0];
    src_data.width = frame_info->video_frame.width;
    src_data.height = frame_info->video_frame.height;
    src_data.stride = frame_info->video_frame.stride[0];

    /* fill dst */
    dst_data.virt_addr = dst->virt_addr[0];
    dst_data.phys_addr = dst->phys_addr[0];
    dst_data.width = dst->width;
    dst_data.height = dst->height;
    dst_data.stride = dst->stride[0];

    ret = ss_mpi_ive_dma(&handle, &src_data, &dst_data, &ctrl, is_instant);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_dma failed!\n", ret);

    if (is_instant == TD_TRUE) {
        ret = ss_mpi_ive_query(handle, &is_finish, is_block);
        while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
            usleep(OT_SAMPLE_IVE_QUERY_SLEEP);
            ret = ss_mpi_ive_query(handle, &is_finish, is_block);
        }
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_query failed!\n", ret);
    }

    return TD_SUCCESS;
}
