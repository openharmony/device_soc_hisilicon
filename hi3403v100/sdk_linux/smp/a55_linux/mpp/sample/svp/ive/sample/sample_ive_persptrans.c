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

#define OT_SAMPLE_IVE_PSP_ROI_NUM               64
#define OT_SAMPLE_IVE_PSP_QUERY_SLEEP           100
#define OT_SAMPLE_IVE_PSP_POINT_PAIR_NUM        5
#define OT_PSP_NUM_TWO                          2
#define OT_PSP_NUM_THREE                        3
#define OT_PSP_NUM_FOUR                         4
#define OT_SAMPLE_IVE_PSP_LEFT_SHIT             2
#define OT_SAMPLE_IVE_PSP_SRC_WIDTH             250
#define OT_SAMPLE_IVE_PSP_SRC_HEIGHT            250
#define OT_SAMPLE_IVE_PSP_DST_WIDTH             96
#define OT_SAMPLE_IVE_PSP_DST_HEIGHT            112
#define OT_SAMPLE_IVE_PSP_MAX_POINT_PAIR_NUM    68
#define OT_SAMPLE_IVE_PSP_ROI_WIDTH             250
#define OT_SAMPLE_IVE_PSP_ROI_HEIGHT            250

typedef struct {
    ot_svp_src_img src;
    ot_svp_rect_u32 roi[OT_SAMPLE_IVE_PSP_ROI_NUM];
    td_u16 roi_num;
    ot_svp_dst_img dst[OT_SAMPLE_IVE_PSP_ROI_NUM];
    ot_svp_src_mem_info point_pair[OT_SAMPLE_IVE_PSP_ROI_NUM];
    ot_ive_persp_trans_ctrl persp_trans_ctrl;

    FILE *fp_src;
    FILE *fp_dst;
} ot_sample_ive_persp_trans_info;

typedef struct {
    ot_svp_rect_u32 roi[OT_SAMPLE_IVE_PSP_ROI_NUM];
    td_u16 roi_num;
    td_u16 max_point_pair_num;
} ot_sample_ive_psp_roi_info;

static ot_sample_ive_persp_trans_info g_persp_trans;
static td_bool g_stop_signal = TD_FALSE;

static td_void sample_ive_persp_trans_uninit(ot_sample_ive_persp_trans_info *psp_info)
{
    td_u16 i;

    sample_svp_check_exps_return_void(psp_info == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");

    sample_svp_mmz_free(psp_info->src.phys_addr[0], psp_info->src.virt_addr[0]);

    for (i = 0; i < psp_info->roi_num; i++) {
        sample_svp_mmz_free(psp_info->dst[i].phys_addr[0], psp_info->dst[i].virt_addr[0]);
        sample_svp_mmz_free(psp_info->point_pair[i].phys_addr, psp_info->point_pair[i].virt_addr);
    }

    sample_svp_close_file(psp_info->fp_src);
    sample_svp_close_file(psp_info->fp_dst);
}

static td_s32 sample_ive_persp_trans_proc(ot_sample_ive_persp_trans_info *persp_trans)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    ot_ive_handle handle;
    td_bool is_finish = TD_FALSE;
    td_bool is_block = TD_TRUE;
    td_bool is_instant = TD_TRUE;
    td_u32 i;

    sample_svp_check_exps_return(persp_trans == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");
    for (i = 0; (i < 1) && (g_stop_signal == TD_FALSE); i++) {
        ret = sample_common_ive_read_file(&(persp_trans->src), persp_trans->fp_src);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Read src file failed!\n", ret);

        ret = ss_mpi_ive_persp_trans(&handle, &persp_trans->src, persp_trans->roi, persp_trans->point_pair,
            persp_trans->dst, &persp_trans->persp_trans_ctrl, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_persp_trans failed!\n", ret);

        ret = ss_mpi_ive_query(handle, &is_finish, is_block);
        while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
            usleep(OT_SAMPLE_IVE_PSP_QUERY_SLEEP);
            ret = ss_mpi_ive_query(handle, &is_finish, is_block);
        }
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_query failed!\n", ret);

        ret = sample_common_ive_write_file(&persp_trans->dst[0], persp_trans->fp_dst);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Read src file failed!\n", ret);
    }
    return TD_SUCCESS;
}

static td_void sample_ive_persp_trans_ctrl_init(ot_sample_ive_persp_trans_info *psp_info,
    ot_sample_ive_psp_roi_info psp_roi)
{
    psp_info->persp_trans_ctrl.alg_mode = OT_IVE_PERSP_TRANS_ALG_MODE_AFFINE;
    psp_info->persp_trans_ctrl.csc_mode = OT_IVE_PERSP_TRANS_CSC_MODE_NONE;
    psp_info->persp_trans_ctrl.roi_num = psp_roi.roi_num;
    psp_info->persp_trans_ctrl.point_pair_num = OT_SAMPLE_IVE_PSP_POINT_PAIR_NUM;
    psp_info->roi_num = psp_roi.roi_num;
}

static td_s32 sample_ive_persp_trans_init(ot_sample_ive_persp_trans_info *psp,
    ot_sample_src_dst_size data, ot_sample_ive_psp_roi_info psp_roi,
    const td_char *src_file, const td_char *dst_file)
{
    td_s32 ret = OT_ERR_IVE_ILLEGAL_PARAM;
    td_u32 size;
    td_u32 i;
    td_u32 j;
    td_char path[PATH_MAX] = {0};
    td_u16 mark[] = { 107, 109, 30, 52, 149, 117, 66, 52, 123, 135, 48, 72, 99, 157, 34, 92, 144, 157, 63, 92 };
    ot_ive_persp_trans_point_pair *tmp = TD_NULL;

    sample_svp_check_exps_return(psp == TD_NULL, OT_ERR_IVE_NULL_PTR, SAMPLE_SVP_ERR_LEVEL_ERROR, "psp is null\n");
    sample_svp_check_exps_return((strlen(src_file) > PATH_MAX) || (realpath(src_file, path) == TD_NULL),
        ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");

    size = sizeof(ot_svp_rect_u32) * psp_roi.roi_num;
    ret = memcpy_s(psp->roi, sizeof(ot_svp_rect_u32) * OT_SAMPLE_IVE_PSP_ROI_NUM, psp_roi.roi, size);
    sample_svp_check_exps_return(ret != EOK, OT_ERR_IVE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR, "copy failed!\n");

    ret = sample_common_ive_create_image(&(psp->src), OT_SVP_IMG_TYPE_YUV420SP, data.src.width, data.src.height);
    sample_svp_check_failed_err_level_goto(ret, fail, "Error(%#x),Create src image failed!\n", ret);

    for (i = 0; i < psp_roi.roi_num; i++) {
        ret = sample_common_ive_create_image(&(psp->dst[i]), OT_SVP_IMG_TYPE_YUV420SP, data.dst.width, data.dst.height);
        sample_svp_check_failed_err_level_goto(ret, fail, "Error(%#x),Create src image failed!\n", ret);
    }

    size = sizeof(ot_ive_persp_trans_point_pair) * psp_roi.max_point_pair_num;
    for (i = 0; i < psp_roi.roi_num; i++) {
        ret = sample_common_ive_create_mem_info(&(psp->point_pair[i]), size);
        sample_svp_check_failed_err_level_goto(ret, fail, "Error(%#x),Create src image failed!\n", ret);
    }

    sample_ive_persp_trans_ctrl_init(psp, psp_roi);
    for (i = 0; i < psp_roi.roi_num; i++) {
        tmp = (ot_ive_persp_trans_point_pair *)(td_uintptr_t)psp->point_pair[i].virt_addr;
        for (j = 0; (j < psp->persp_trans_ctrl.point_pair_num) && (j < OT_SAMPLE_IVE_PSP_POINT_PAIR_NUM); j++) {
            tmp->src_point.x = mark[j * OT_PSP_NUM_FOUR] << OT_SAMPLE_IVE_PSP_LEFT_SHIT;
            tmp->src_point.y = mark[j * OT_PSP_NUM_FOUR + 1] << OT_SAMPLE_IVE_PSP_LEFT_SHIT;
            tmp->dst_point.x = mark[j * OT_PSP_NUM_FOUR + OT_PSP_NUM_TWO] << OT_SAMPLE_IVE_PSP_LEFT_SHIT;
            tmp->dst_point.y = mark[j * OT_PSP_NUM_FOUR + OT_PSP_NUM_THREE] << OT_SAMPLE_IVE_PSP_LEFT_SHIT;
            tmp++;
        }
    }

    /* open src file */
    ret = TD_FAILURE;
    psp->fp_src = fopen(path, "rb");
    sample_svp_check_exps_goto(psp->fp_src == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    /* open dst file */
    sample_svp_check_exps_goto(realpath(dst_file, path) == NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");
    ret = strcat_s(path, PATH_MAX, "/Amelia_Vega_Affine_96x112_420sp.yuv");
    sample_svp_check_exps_goto(ret != EOK, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "strcat_s failed!\n");
    ret = TD_FAILURE;
    psp->fp_dst = fopen(path, "wb");
    sample_svp_check_exps_goto(psp->fp_dst == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    return TD_SUCCESS;
fail:
    sample_ive_persp_trans_uninit(psp);
    return ret;
}

static td_void sample_ive_persp_trans_stop(td_void)
{
    sample_ive_persp_trans_uninit(&g_persp_trans);
    (td_void)memset_s(&g_persp_trans, sizeof(g_persp_trans), 0, sizeof(g_persp_trans));
    sample_common_ive_mpi_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}
td_void sample_ive_persp_trans(td_void)
{
    td_s32 ret;
    const td_char *src_file = "./data/input/psp/src/Amelia_Vega_250x250_420sp.yuv";
    const td_char *dst_file = "./data/output/psp";
    ot_sample_src_dst_size data;
    data.src.width = OT_SAMPLE_IVE_PSP_SRC_WIDTH;
    data.src.height = OT_SAMPLE_IVE_PSP_SRC_HEIGHT;
    data.dst.width = OT_SAMPLE_IVE_PSP_DST_WIDTH;
    data.dst.height = OT_SAMPLE_IVE_PSP_DST_HEIGHT;

    ot_sample_ive_psp_roi_info psp_roi;
    psp_roi.roi_num = 1;
    psp_roi.max_point_pair_num = OT_SAMPLE_IVE_PSP_MAX_POINT_PAIR_NUM;
    psp_roi.roi[0].x = 0;
    psp_roi.roi[0].y = 0;
    psp_roi.roi[0].width = OT_SAMPLE_IVE_PSP_ROI_WIDTH;
    psp_roi.roi[0].height = OT_SAMPLE_IVE_PSP_ROI_HEIGHT;

    (td_void)memset_s(&g_persp_trans, sizeof(g_persp_trans), 0, sizeof(g_persp_trans));
    ret = sample_common_ive_check_mpi_init();
    sample_svp_check_exps_return_void(ret != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "ive_check_mpi_init failed!\n");

    ret = sample_ive_persp_trans_init(&g_persp_trans, data, psp_roi, src_file, dst_file);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, persp_trans_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_ive_persp_trans_init failed!\n", ret);

    ret = sample_ive_persp_trans_proc(&g_persp_trans);
    if (g_stop_signal == TD_TRUE) {
        sample_ive_persp_trans_stop();
        return;
    }
    if (ret == TD_SUCCESS) {
        sample_svp_trace_info("Process success!\n");
    }

    g_stop_signal = TD_TRUE;
    sample_ive_persp_trans_uninit(&g_persp_trans);
    (td_void)memset_s(&g_persp_trans, sizeof(g_persp_trans), 0, sizeof(g_persp_trans));

persp_trans_fail:
    g_stop_signal = TD_TRUE;
    sample_common_ive_mpi_exit();
}

/*
 * function : PerspTrans sample signal handle
 */
td_void sample_ive_persp_trans_handle_sig(td_void)
{
    g_stop_signal = TD_TRUE;
}
