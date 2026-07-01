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
#include <math.h>
#include <limits.h>

#define OT_SAMPLE_IVE_CANNY_QUERY_SLEEP  100
#define OT_SAMPLE_IVE_CANNY_HIGH_THR     150
#define OT_SAMPLE_IVE_CANNY_LOW_THR      50
#define OT_SAMPLE_IVE_THR_U16_HIGH_THR   100
#define OT_SAMPLE_IVE_THR_U16_LOW_THR    100
#define OT_SAMPLE_IVE_THR_U16_MAX_VAL    255
#define OT_SAMPLE_IVE_CANNY_EDGE_NUM     3
#define OT_SAMPLE_IVE_D1_WIDTH           720
#define OT_SAMPLE_IVE_D1_HEIGHT          576

typedef struct {
    ot_svp_src_img src;
    ot_svp_dst_img edge;
    ot_svp_dst_img mag;
    ot_svp_mem_info stack;
    ot_ive_canny_hys_edge_ctrl canny_hys_edge_ctrl;
    ot_ive_mag_and_ang_ctrl mag_and_ang_ctrl;
    ot_ive_threshold_u16_ctrl thr_u16_ctrl;
    FILE *fp_src;
    FILE *fp_dst;
} ot_sample_ive_canny_info;

static ot_sample_ive_canny_info g_canny_info;
static td_bool g_stop_signal = TD_FALSE;

/*
 * function : Canny uninit
 */
static td_void sample_ive_canny_uninit(ot_sample_ive_canny_info *canny_info)
{
    sample_svp_mmz_free(canny_info->src.phys_addr[0], canny_info->src.virt_addr[0]);
    sample_svp_mmz_free(canny_info->edge.phys_addr[0], canny_info->edge.virt_addr[0]);
    sample_svp_mmz_free(canny_info->mag.phys_addr[0], canny_info->mag.virt_addr[0]);
    sample_svp_mmz_free(canny_info->stack.phys_addr, canny_info->stack.virt_addr);
    sample_svp_mmz_free(canny_info->canny_hys_edge_ctrl.mem.phys_addr,
        canny_info->canny_hys_edge_ctrl.mem.virt_addr);

    sample_svp_close_file(canny_info->fp_src);
    sample_svp_close_file(canny_info->fp_dst);
}

static td_void sample_ive_canny_ctrl_init(ot_sample_ive_canny_info *canny_info)
{
    canny_info->canny_hys_edge_ctrl.high_threshold = OT_SAMPLE_IVE_CANNY_HIGH_THR;
    canny_info->canny_hys_edge_ctrl.low_threshold = OT_SAMPLE_IVE_CANNY_LOW_THR;
    canny_info->mag_and_ang_ctrl.out_ctrl = OT_IVE_MAG_AND_ANG_OUT_CTRL_MAG;
    canny_info->mag_and_ang_ctrl.threshold = 0;
    canny_info->thr_u16_ctrl.mode = OT_IVE_THRESHOLD_U16_MODE_U16_TO_U8_MIN_MID_MAX;
    canny_info->thr_u16_ctrl.high_threshold = OT_SAMPLE_IVE_THR_U16_HIGH_THR;
    canny_info->thr_u16_ctrl.low_threshold = OT_SAMPLE_IVE_THR_U16_LOW_THR;
    canny_info->thr_u16_ctrl.max_val = OT_SAMPLE_IVE_THR_U16_MAX_VAL;
    canny_info->thr_u16_ctrl.mid_val = 0;
    canny_info->thr_u16_ctrl.min_val = 0;
}

/*
 * function : Canny init
 */
static td_s32 sample_ive_canny_init(ot_sample_ive_canny_info *canny_info, td_u32 width,
    td_u32 height, td_char canny_complete)
{
    td_s32 ret;
    td_u32 size;
    td_char path[PATH_MAX] = {0};
    td_char tmp_file[PATH_MAX] = {0};
    td_s8 mask[OT_IVE_MASK_NUM] =   { 0,  0, 0, 0, 0, 0, -1, 0, 1, 0, 0, -2, 0, 2, 0, 0, -1, 0, 1, 0, 0,  0, 0, 0, 0 };
    const td_char *src_file = "./data/input/canny/canny.yuv";

    sample_svp_check_exps_return((strlen(src_file) > PATH_MAX) || (realpath(src_file, path) == TD_NULL),
        OT_ERR_IVE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");

    (td_void)memset_s(canny_info, sizeof(ot_sample_ive_canny_info), 0, sizeof(ot_sample_ive_canny_info));
    ret = memcpy_s(canny_info->canny_hys_edge_ctrl.mask, OT_IVE_MASK_NUM, mask, OT_IVE_MASK_NUM);
    sample_svp_check_exps_return(ret != EOK, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "memcpy_s mask failed!\n");
    ret = memcpy_s(canny_info->mag_and_ang_ctrl.mask, OT_IVE_MASK_NUM, mask, OT_IVE_MASK_NUM);
    sample_svp_check_exps_return(ret != EOK, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "memcpy_s mask failed!\n");

    sample_ive_canny_ctrl_init(canny_info);

    ret = sample_common_ive_create_image(&canny_info->src, OT_SVP_IMG_TYPE_U8C1, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,create src img failed!\n");

    ret = sample_common_ive_create_image(&canny_info->edge, OT_SVP_IMG_TYPE_U8C1, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,create edge img failed!\n");

    ret = sample_common_ive_create_image(&canny_info->mag, OT_SVP_IMG_TYPE_U16C1, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,create mag img failed!\n");

    size = canny_info->src.stride[0] * canny_info->src.height * sizeof(ot_svp_point_u16) +
        sizeof(ot_ive_canny_stack_size);
    ret = sample_common_ive_create_mem_info(&canny_info->stack, size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,create stack mem_info failed!\n");

    size = canny_info->src.stride[0] * canny_info->src.height * OT_SAMPLE_IVE_CANNY_EDGE_NUM;
    ret = sample_common_ive_create_mem_info(&canny_info->canny_hys_edge_ctrl.mem, size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,create canny_hys_edge_ctrl.mem failed!\n");

    /* src file */
    ret = TD_FAILURE;
    canny_info->fp_src = fopen(path, "rb");
    sample_svp_check_exps_goto(canny_info->fp_src == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    /* dst file */
    sample_svp_check_exps_goto(realpath("./data/output/canny", path) == TD_NULL, fail,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");
    ret = snprintf_s(tmp_file, sizeof(tmp_file) - 1, sizeof(tmp_file) - 1, "/cannyout_complete_%c.yuv", canny_complete);
    sample_svp_check_exps_goto((ret < 0) || (ret > (td_s32)(sizeof(tmp_file) - 1)), fail,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,snprintf_s src file name failed!\n");

    ret = strcat_s(path, PATH_MAX, tmp_file);
    sample_svp_check_exps_goto(ret != EOK, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "strcat_s failed!\n");
    ret = TD_FAILURE;
    canny_info->fp_dst = fopen(path, "wb");
    sample_svp_check_exps_goto(canny_info->fp_dst == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    return TD_SUCCESS;
fail:
    sample_ive_canny_uninit(canny_info);
    return ret;
}

/*
 * function : show complete canny sample
 */
static td_s32 sample_ive_complete_canny(ot_sample_ive_canny_info *canny_info)
{
    td_s32 ret;
    td_s32 i;
    td_bool is_instant = TD_TRUE;
    td_bool is_block = TD_TRUE;
    td_bool is_finish = TD_FALSE;
    ot_ive_handle handle;

    for (i = 0; (i < 1) && (g_stop_signal == TD_FALSE); i++) {
        ret = sample_common_ive_read_file(&(canny_info->src), canny_info->fp_src);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Read src file failed!\n", ret);

        ret = ss_mpi_ive_canny_hys_edge(&handle, &canny_info->src, &canny_info->edge, &canny_info->stack,
            &canny_info->canny_hys_edge_ctrl, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_canny_hys_edge failed!\n", ret);

        ret = ss_mpi_ive_query(handle, &is_finish, is_block);
        while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
            usleep(OT_SAMPLE_IVE_CANNY_QUERY_SLEEP);
            ret = ss_mpi_ive_query(handle, &is_finish, is_block);
        }
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_query failed!\n", ret);

        ret = ss_mpi_ive_canny_edge(&canny_info->edge, &canny_info->stack);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_canny_edge failed!\n", ret);

        ret = sample_common_ive_write_file(&canny_info->edge, canny_info->fp_dst);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Write edge file failed!\n", ret);
    }
    return TD_SUCCESS;
}


/*
 * function : show part canny sample
 */
static td_s32 sample_ive_part_canny(ot_sample_ive_canny_info *canny_info)
{
    td_s32 ret;
    td_s32 i;
    td_bool is_instant = TD_FALSE;
    td_bool is_block = TD_TRUE;
    td_bool is_finish = TD_FALSE;
    ot_ive_handle handle;

    for (i = 0; (i < 1) && (g_stop_signal == TD_FALSE); i++) {
        ret = sample_common_ive_read_file(&canny_info->src, canny_info->fp_src);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Read src file failed!\n", ret);

        ret = ss_mpi_ive_mag_and_ang(&handle, &canny_info->src, &canny_info->mag, TD_NULL,
            &canny_info->mag_and_ang_ctrl, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_mag_and_ang failed!\n", ret);

        is_instant = TD_TRUE;
        ret = ss_mpi_ive_threshold_u16(&handle, &canny_info->mag, &canny_info->edge,
            &canny_info->thr_u16_ctrl, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_threshold_u16 failed!\n", ret);

        ret = ss_mpi_ive_query(handle, &is_finish, is_block);
        while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
            usleep(OT_SAMPLE_IVE_CANNY_QUERY_SLEEP);
            ret = ss_mpi_ive_query(handle, &is_finish, is_block);
        }
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_query failed!\n", ret);

        ret = sample_common_ive_write_file(&canny_info->edge, canny_info->fp_dst);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),Write edge file failed!\n", ret);
    }
    return TD_SUCCESS;
}

static td_void sample_ive_canny_stop(td_void)
{
    sample_ive_canny_uninit(&g_canny_info);
    (td_void)memset_s(&g_canny_info, sizeof(g_canny_info), 0, sizeof(g_canny_info));
    sample_common_ive_mpi_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

/*
 * function : show canny sample
 */
td_void sample_ive_canny(td_char canny_complete)
{
    const td_u16 width = OT_SAMPLE_IVE_D1_WIDTH;
    const td_u16 height = OT_SAMPLE_IVE_D1_HEIGHT;
    td_s32 ret;

    (td_void)memset_s(&g_canny_info, sizeof(g_canny_info), 0, sizeof(g_canny_info));

    ret = sample_common_ive_check_mpi_init();
    sample_svp_check_exps_return_void(ret != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "ive_check_mpi_init failed!\n");

    ret = sample_ive_canny_init(&g_canny_info, width, height, canny_complete);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, canny_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_ive_canny_init failed!\n", ret);

    if (canny_complete == '0') {
        ret = sample_ive_part_canny(&g_canny_info);
    } else {
        ret = sample_ive_complete_canny(&g_canny_info);
    }

    if (g_stop_signal == TD_TRUE) {
        sample_ive_canny_stop();
        return;
    }

    if (ret == TD_SUCCESS) {
        sample_svp_trace_info("Process success!\n");
    }

    g_stop_signal = TD_TRUE;
    sample_ive_canny_uninit(&g_canny_info);
    (td_void)memset_s(&g_canny_info, sizeof(g_canny_info), 0, sizeof(g_canny_info));

canny_fail:
    g_stop_signal = TD_TRUE;
    sample_common_ive_mpi_exit();
}

/*
 * function :Canny sample signal handle
 */
td_void sample_ive_canny_handle_sig(td_void)
{
    g_stop_signal = TD_TRUE;
}
