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
#include <semaphore.h>
#include <pthread.h>
#include <limits.h>

#define OT_SAMPLE_IVE_LK_MAX_POINT_NUM      500
#define OT_SAMPLE_IVE_LK_MIN_DIST           5
#define OT_SAMPLE_IVE_LK_PYR_NUM            4
#define OT_SAMPLE_IVE_LK_QUERY_SLEEP        100
#define OT_SAMPLE_IVE_LK_MIN_EIG_VALUE      100
#define OT_SAMPLE_IVE_LK_ITER_CNT           10
#define OT_SAMPLE_IVE_LK_EPS                2
#define OT_SAMPLE_IVE_LEFT_SHIFT_SEVEN      7
#define OT_SAMPLE_IVE_ST_QUALITY_LEVEL      25
#define OT_SAMPLE_IVE_LK_D1_WIDTH           720
#define OT_SAMPLE_IVE_LK_D1_HEIGHT          576
#define OT_SAMPLE_IVE_LK_MAX_LEVEL          3
#define OT_SAMPLE_IVE_LK_FRAME_NUM          10

typedef struct {
    ot_svp_src_img prev_pyr[OT_SAMPLE_IVE_LK_PYR_NUM];
    ot_svp_src_img next_pyr[OT_SAMPLE_IVE_LK_PYR_NUM];
    ot_svp_src_mem_info prev_points;
    ot_svp_mem_info next_points;
    ot_svp_dst_mem_info status;
    ot_svp_dst_mem_info err;
    ot_ive_lk_optical_flow_pyr_ctrl lk_pyr_ctrl;

    ot_svp_src_img src;
    ot_svp_img dst;
    ot_svp_dst_mem_info corner;
    ot_ive_st_cand_corner_ctrl cand_corner_ctrl;
    ot_ive_st_corner_ctrl corner_ctrl;

    ot_svp_img pyr_tmp;
    ot_svp_img src_yuv;
    FILE *fp_src;
} ot_sample_ive_st_lk_inf;

static ot_sample_ive_st_lk_inf g_lk_info;
static td_bool g_stop_signal = TD_FALSE;

static td_s32 sample_ive_st_lk_dma(ot_ive_handle *ive_handle, ot_svp_src_img *src,
    ot_svp_dst_img *dst, ot_ive_dma_ctrl *dma_ctrl, td_bool is_instant)
{
    td_s32 ret;
    ot_svp_src_data data_src;
    ot_svp_dst_data data_dst;

    data_src.virt_addr = src->virt_addr[0];
    data_src.phys_addr = src->phys_addr[0];
    data_src.width = src->width;
    data_src.height = src->height;
    data_src.stride = src->stride[0];

    data_dst.virt_addr = dst->virt_addr[0];
    data_dst.phys_addr = dst->phys_addr[0];
    data_dst.width = dst->width;
    data_dst.height = dst->height;
    data_dst.stride = dst->stride[0];
    ret = ss_mpi_ive_dma(ive_handle, &data_src, &data_dst, dma_ctrl, is_instant);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_dma failed!\n", ret);

    return ret;
}


/*
 * function : Copy pyr
 */
static td_void sample_ive_st_lk_copy_pyr(ot_svp_src_img pyr_src[], ot_svp_dst_img pyr_dst[],
    td_u8 max_level)
{
    td_u8 i;
    td_s32 ret;
    ot_ive_handle handle;

    ot_ive_dma_ctrl dma_ctrl;
    (td_void)memset_s(&dma_ctrl, sizeof(dma_ctrl), 0, sizeof(dma_ctrl));
    dma_ctrl.mode = OT_IVE_DMA_MODE_DIRECT_COPY;

    for (i = 0; i <= max_level; i++) {
        ret = sample_ive_st_lk_dma(&handle, &pyr_src[i], &pyr_dst[i], &dma_ctrl, TD_FALSE);
        if (ret != TD_SUCCESS) {
            sample_svp_trace_err("sample_ive_st_lk_dma fail,Error(%d)\n", ret);
            break;
        }
    }
}

/*
 * function : St lk uninit
 */
static td_void sample_ive_st_lk_uninit(ot_sample_ive_st_lk_inf *lk_info)
{
    td_u16 i;

    sample_svp_check_exps_return_void(lk_info == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");

    for (i = 0; i <= lk_info->lk_pyr_ctrl.max_level; i++) {
        sample_svp_mmz_free(lk_info->prev_pyr[i].phys_addr[0], lk_info->prev_pyr[i].virt_addr[0]);
        sample_svp_mmz_free(lk_info->next_pyr[i].phys_addr[0], lk_info->next_pyr[i].virt_addr[0]);
    }

    sample_svp_mmz_free(lk_info->prev_points.phys_addr, lk_info->prev_points.virt_addr);
    sample_svp_mmz_free(lk_info->next_points.phys_addr, lk_info->next_points.virt_addr);
    sample_svp_mmz_free(lk_info->status.phys_addr, lk_info->status.virt_addr);
    sample_svp_mmz_free(lk_info->err.phys_addr, lk_info->err.virt_addr);

    sample_svp_mmz_free(lk_info->src.phys_addr[0], lk_info->src.virt_addr[0]);
    sample_svp_mmz_free(lk_info->dst.phys_addr[0], lk_info->dst.virt_addr[0]);
    sample_svp_mmz_free(lk_info->corner.phys_addr, lk_info->corner.virt_addr);

    sample_svp_mmz_free(lk_info->cand_corner_ctrl.mem.phys_addr, lk_info->cand_corner_ctrl.mem.virt_addr);

    sample_svp_mmz_free(lk_info->pyr_tmp.phys_addr[0], lk_info->pyr_tmp.virt_addr[0]);
    sample_svp_mmz_free(lk_info->src_yuv.phys_addr[0], lk_info->src_yuv.virt_addr[0]);

    sample_svp_close_file(lk_info->fp_src);
}

static td_s32 sample_ive_lk_param_init(ot_sample_ive_st_lk_inf *lk_info, ot_size src_size, td_u8 max_level)
{
    td_s32 ret;
    td_u32 size;
    td_u32 i;
    lk_info->lk_pyr_ctrl.out_mode = OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH;
    lk_info->lk_pyr_ctrl.use_init_flow = TD_TRUE;
    lk_info->lk_pyr_ctrl.points_num = OT_SAMPLE_IVE_LK_MAX_POINT_NUM;
    lk_info->lk_pyr_ctrl.max_level = max_level;
    lk_info->lk_pyr_ctrl.min_eig_val_threshold = OT_SAMPLE_IVE_LK_MIN_EIG_VALUE;
    lk_info->lk_pyr_ctrl.iter_cnt = OT_SAMPLE_IVE_LK_ITER_CNT;
    lk_info->lk_pyr_ctrl.eps = OT_SAMPLE_IVE_LK_EPS;
    /* Init Pyr */
    for (i = 0; i <= max_level; i++) {
        ret = sample_common_ive_create_image(&lk_info->prev_pyr[i], OT_SVP_IMG_TYPE_U8C1,
            src_size.width >> i, src_size.height >> i);
        sample_svp_check_exps_goto(ret != TD_SUCCESS, lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Create prevPyr[%d] image failed!\n", ret, i);

        ret = sample_common_ive_create_image(&lk_info->next_pyr[i], OT_SVP_IMG_TYPE_U8C1,
            lk_info->prev_pyr[i].width, lk_info->prev_pyr[i].height);
        sample_svp_check_exps_goto(ret != TD_SUCCESS, lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Create nextPyr[%d] image failed!\n", ret, i);
    }
    /* Init prev pts */
    size = sizeof(ot_svp_point_s25q7) * OT_SAMPLE_IVE_LK_MAX_POINT_NUM;
    size = sample_common_ive_calc_stride(size, OT_IVE_ALIGN);
    ret = sample_common_ive_create_mem_info(&(lk_info->prev_points), size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create prevPts mem info failed!\n", ret);
    /* Init next pts */
    ret = sample_common_ive_create_mem_info(&(lk_info->next_points), size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create nextPts mem info failed!\n", ret);

    /* Init status */
    size = sizeof(td_u8) * OT_SAMPLE_IVE_LK_MAX_POINT_NUM;
    size = sample_common_ive_calc_stride(size, OT_IVE_ALIGN);
    ret = sample_common_ive_create_mem_info(&(lk_info->status), size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create status mem info failed!\n", ret);

    /* Init err */
    size = sizeof(td_u9q7) * OT_SAMPLE_IVE_LK_MAX_POINT_NUM;
    size = sample_common_ive_calc_stride(size, OT_IVE_ALIGN);
    ret = sample_common_ive_create_mem_info(&(lk_info->err), size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create err mem info failed!\n", ret);

lk_init_fail:
    if (ret != TD_SUCCESS) {
        sample_ive_st_lk_uninit(lk_info);
    }
    return ret;
}

static td_s32 sample_ive_st_param_init(ot_sample_ive_st_lk_inf *lk_info, ot_size src_size, ot_size pyr_size)
{
    td_s32 ret;
    td_u32 size;

    ot_unused(pyr_size);
    /* Init St */
    ret = sample_common_ive_create_image(&lk_info->src, OT_SVP_IMG_TYPE_U8C1, src_size.width, src_size.height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, st_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create src image failed!\n", ret);

    ret = sample_common_ive_create_image(&lk_info->dst, OT_SVP_IMG_TYPE_U8C1, src_size.width, src_size.height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, st_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create dst image failed!\n", ret);

    lk_info->cand_corner_ctrl.quality_level = OT_SAMPLE_IVE_ST_QUALITY_LEVEL;
    size = sizeof(td_u32) * sample_common_ive_calc_stride(src_size.width, OT_IVE_ALIGN) * src_size.height +
        sizeof(ot_ive_st_max_eig_val);
    ret = sample_common_ive_create_mem_info(&(lk_info->cand_corner_ctrl.mem), size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, st_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create CandiCornerCtrl.stMem mem info failed!\n", ret);

    size = sizeof(ot_ive_st_corner_info);
    ret = sample_common_ive_create_mem_info(&(lk_info->corner), size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, st_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create corner mem info failed!\n", ret);

    lk_info->corner_ctrl.max_corner_num = OT_SAMPLE_IVE_LK_MAX_POINT_NUM;
    lk_info->corner_ctrl.min_dist = OT_SAMPLE_IVE_LK_MIN_DIST;
st_init_fail:
    if (ret != TD_SUCCESS) {
        sample_ive_st_lk_uninit(lk_info);
    }
    return ret;
}

/*
 * function : St lk init
 */
static td_s32 sample_ive_st_lk_init(ot_sample_ive_st_lk_inf *lk_info, ot_size src_size,
    ot_size pyr_size, td_u8 max_level)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    td_char path[PATH_MAX] = {0};
    const td_char *src_file = "./data/input/stlk/st_lk_720x576_420sp.yuv";

    sample_svp_check_exps_return(lk_info == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");
    sample_svp_check_exps_return((strlen(src_file) > PATH_MAX) || (realpath(src_file, path) == TD_NULL),
        OT_ERR_IVE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");

    /* max_level can't be large than OT_SAMPLE_IVE_LK_PYR_NUM */
    sample_svp_check_exps_return(max_level > (OT_SAMPLE_IVE_LK_PYR_NUM - 1), TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "max_level can't be larger than %u\n", (OT_SAMPLE_IVE_LK_PYR_NUM - 1));
    (td_void)memset_s(lk_info, sizeof(ot_sample_ive_st_lk_inf), 0, sizeof(ot_sample_ive_st_lk_inf));
    /* lk param init */
    ret = sample_ive_lk_param_init(lk_info, src_size, max_level);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "sample_ive_lk_param_init failed\n");

    /* st param init */
    ret = sample_ive_st_param_init(lk_info, src_size, pyr_size);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "sample_ive_st_param_init failed\n");

    /* init pyr assist buff */
    ret = sample_common_ive_create_image(&lk_info->pyr_tmp, OT_SVP_IMG_TYPE_U8C1, pyr_size.width, pyr_size.height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, st_lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create pyrTmp image failed!\n", ret);

    ret = sample_common_ive_create_image(&lk_info->src_yuv, OT_SVP_IMG_TYPE_YUV420SP, src_size.width, src_size.height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, st_lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create srcYuv image failed!\n", ret);

    /* open file */
    lk_info->fp_src = fopen(path, "rb");
    sample_svp_check_exps_goto(lk_info->fp_src == TD_NULL, st_lk_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,Open file %s failed!\n", path);

    ret = TD_SUCCESS;

st_lk_init_fail:
    if (ret != TD_SUCCESS) {
        sample_ive_st_lk_uninit(lk_info);
    }
    return ret;
}


/*
 * function : Pyr down
 */
static td_s32 sample_ive_st_lk_pyr_down(ot_sample_ive_st_lk_inf *lk_info, ot_svp_src_img *src,
    ot_svp_dst_img *dst)
{
    td_s32 ret;
    ot_ive_handle handle;
    ot_ive_dma_ctrl dma_ctrl = { OT_IVE_DMA_MODE_INTERVAL_COPY, 0, 2, 1, 2 };
    ot_ive_filter_ctrl filter_ctrl = {
        { 1, 2, 3, 2, 1, 2, 5, 6, 5, 2, 3, 6, 8, 6, 3, 2, 5, 6, 5, 2, 1, 2, 3, 2, 1 },
        7
    };

    lk_info->pyr_tmp.width = src->width;
    lk_info->pyr_tmp.height = src->height;

    ret = ss_mpi_ive_filter(&handle, src, &lk_info->pyr_tmp, &filter_ctrl, TD_FALSE);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_filter failed!\n", ret);

    ret = sample_ive_st_lk_dma(&handle, &lk_info->pyr_tmp, dst, &dma_ctrl, TD_FALSE);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_ive_st_lk_dma failed!\n", ret);

    return ret;
}


static td_s32 sample_ive_query_task(ot_ive_handle handle)
{
    td_s32 ret;
    td_bool is_block = TD_TRUE;
    td_bool is_finish = TD_FALSE;
    ret = ss_mpi_ive_query(handle, &is_finish, is_block);
    while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
        usleep(OT_SAMPLE_IVE_LK_QUERY_SLEEP);
        ret = ss_mpi_ive_query(handle, &is_finish, is_block);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_query failed!\n", ret);
    return TD_SUCCESS;
}

static td_s32 sample_ive_proc_frame(td_u32 i, ot_ive_handle handle, ot_sample_ive_st_lk_inf *lk)
{
    td_s32 ret;
    td_u32 k;
    td_u32 rect_num;

    ot_ive_st_corner_info *corner_info =
        sample_svp_convert_addr_to_ptr(ot_ive_st_corner_info, lk->corner.virt_addr);
    ot_svp_point_s25q7 *next_points =
        sample_svp_convert_addr_to_ptr(ot_svp_point_s25q7, lk->next_points.virt_addr);

    if (i == 0) {
        ret = ss_mpi_ive_st_cand_corner(&handle, &lk->next_pyr[0], &lk->dst, &lk->cand_corner_ctrl, TD_TRUE);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_st_cand_corner failed!\n", ret);

        ret = sample_ive_query_task(handle);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),sample_ive_query_task failed!\n", ret);

        ret = ss_mpi_ive_st_corner(&lk->dst, &lk->corner, &lk->corner_ctrl);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_st_corner failed!\n", ret);

        lk->lk_pyr_ctrl.points_num = corner_info->corner_num;
        for (k = 0; k < lk->lk_pyr_ctrl.points_num; k++) {
            next_points[k].x = (td_s32)(corner_info->corner[k].x << OT_SAMPLE_IVE_LEFT_SHIFT_SEVEN);
            next_points[k].y = (td_s32)(corner_info->corner[k].y << OT_SAMPLE_IVE_LEFT_SHIFT_SEVEN);
        }
    } else {
        ret = ss_mpi_ive_lk_optical_flow_pyr(&handle, lk->prev_pyr, lk->next_pyr, &lk->prev_points,
            &lk->next_points, &lk->status, &lk->err, &lk->lk_pyr_ctrl, TD_TRUE);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_lk_optical_flow_pyr failed!\n", ret);

        ret = sample_ive_query_task(handle);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),sample_ive_query_task failed!\n", ret);

        rect_num = 0;
        for (k = 0; k < lk->lk_pyr_ctrl.points_num; k++) {
            if ((sample_svp_convert_addr_to_ptr(td_u8, lk->status.virt_addr))[k] == 0) {
                continue;
            }
            next_points[rect_num].x = next_points[k].x;
            next_points[rect_num].y = next_points[k].y;
            rect_num++;
        }

        lk->lk_pyr_ctrl.points_num = rect_num;
    }
    return TD_SUCCESS;
}
/*
 * function : St lk proc
 */
static td_s32 sample_ive_st_lk_proc(ot_sample_ive_st_lk_inf *lk)
{
    const td_u32 frame_num = OT_SAMPLE_IVE_LK_FRAME_NUM;
    td_u32 i;
    td_u32 k;
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    ot_ive_handle handle;
    ot_ive_dma_ctrl dma_ctrl;

    sample_svp_check_exps_return(lk == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");

    (td_void)memset_s(&dma_ctrl, sizeof(dma_ctrl), 0, sizeof(dma_ctrl));
    dma_ctrl.mode = OT_IVE_DMA_MODE_DIRECT_COPY;

    for (i = 0; (i < frame_num) && (g_stop_signal == TD_FALSE); i++) {
        sample_svp_trace_info("Proc frame %d\n", i);
        ret = sample_common_ive_read_file(&lk->src_yuv, lk->fp_src);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Read src file failed!\n", ret);

        ret = sample_ive_st_lk_dma(&handle, &lk->src_yuv, &lk->next_pyr[0], &dma_ctrl, TD_FALSE);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),sample_ive_st_lk_dma failed!\n", ret);

        /* buid pyr */
        for (k = 1; k <= lk->lk_pyr_ctrl.max_level; k++) {
            ret = sample_ive_st_lk_pyr_down(lk, &lk->next_pyr[k - 1], &lk->next_pyr[k]);
            sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Error(%#x),sample_ive_st_lk_pyr_down %d failed!\n", ret, k);
        }
        /* process frame */
        ret = sample_ive_proc_frame(i, handle, lk);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),sample_ive_proc_frame failed!\n", ret);

        ret = memcpy_s(sample_svp_convert_addr_to_ptr(td_void, lk->prev_points.virt_addr),
            sizeof(ot_svp_point_s25q7) * lk->lk_pyr_ctrl.points_num,
            sample_svp_convert_addr_to_ptr(td_void, lk->next_points.virt_addr),
            sizeof(ot_svp_point_s25q7) * lk->lk_pyr_ctrl.points_num);
        sample_svp_check_exps_return(ret != EOK, OT_ERR_IVE_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,memcpy_s lk points failed!\n");

        sample_ive_st_lk_copy_pyr(lk->next_pyr, lk->prev_pyr, lk->lk_pyr_ctrl.max_level);
    }

    return TD_SUCCESS;
}

static td_void sample_ive_lk_stop(td_void)
{
    sample_ive_st_lk_uninit(&g_lk_info);
    (td_void)memset_s(&g_lk_info, sizeof(g_lk_info), 0, sizeof(g_lk_info));
    sample_common_ive_mpi_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}
/*
 * function : show St Lk sample
 */
td_void sample_ive_st_lk(td_void)
{
    ot_size src_size = { OT_SAMPLE_IVE_LK_D1_WIDTH, OT_SAMPLE_IVE_LK_D1_HEIGHT };
    ot_size pyr_size = { OT_SAMPLE_IVE_LK_D1_WIDTH, OT_SAMPLE_IVE_LK_D1_HEIGHT };

    td_s32 ret;
    const td_u8 max_level = OT_SAMPLE_IVE_LK_MAX_LEVEL;

    (td_void)memset_s(&g_lk_info, sizeof(g_lk_info), 0, sizeof(g_lk_info));
    ret = sample_common_ive_check_mpi_init();
    sample_svp_check_exps_return_void(ret != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "ive_check_mpi_init failed!\n");

    ret = sample_ive_st_lk_init(&g_lk_info, src_size, pyr_size, max_level);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, st_lk_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_ive_st_lk_init failed!\n", ret);

    ret = sample_ive_st_lk_proc(&g_lk_info);
    if (g_stop_signal == TD_TRUE) {
        sample_ive_lk_stop();
        return;
    }
    if (ret == TD_SUCCESS) {
        sample_svp_trace_info("Process success!\n");
    } else {
        sample_svp_trace_err("st_lk process failed\n");
    }

    g_stop_signal = TD_TRUE;
    sample_ive_st_lk_uninit(&g_lk_info);
    (td_void)memset_s(&g_lk_info, sizeof(g_lk_info), 0, sizeof(g_lk_info));

st_lk_fail:
    g_stop_signal = TD_TRUE;
    sample_common_ive_mpi_exit();
}

/*
 * function : St_Lk sample signal handle
 */
td_void sample_ive_st_lk_handle_sig(td_void)
{
    g_stop_signal = TD_TRUE;
}
