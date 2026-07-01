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

#define OT_SAMPLE_IVE_SOBEL_QUERY_SLEEP         100
#define OT_SAMPLE_IVE_SOBEL_SRC_WIDTH           720
#define OT_SAMPLE_IVE_SOBEL_SRC_HEIGHT          576

typedef struct {
    ot_svp_src_img src1;
    ot_svp_src_img src2;
    ot_svp_dst_img dst_h1;
    ot_svp_dst_img dst_h2;
    ot_svp_dst_img dst_v1;
    ot_svp_dst_img dst_v2;
    ot_ive_sobel_ctrl sobel_ctrl;
    FILE *fp_src;
    FILE *fp_dst_h1;
    FILE *fp_dst_h2;
    FILE *fp_dst_v1;
    FILE *fp_dst_v2;
} ot_sample_ive_sobel_info;

static td_bool g_is_flush_cached = TD_TRUE;
static ot_sample_ive_sobel_info g_sobel_info;
static td_bool g_stop_signal = TD_FALSE;

/*
 * function : show Sobel uninit
 */
static td_void sample_ive_sobel_uninit(ot_sample_ive_sobel_info *sobel)
{
    sample_svp_check_exps_return_void(sobel == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");

    sample_svp_mmz_free(sobel->src1.phys_addr[0], sobel->src1.virt_addr[0]);
    sample_svp_mmz_free(sobel->src2.phys_addr[0], sobel->src2.virt_addr[0]);
    sample_svp_mmz_free(sobel->dst_h1.phys_addr[0], sobel->dst_h1.virt_addr[0]);
    sample_svp_mmz_free(sobel->dst_h2.phys_addr[0], sobel->dst_h2.virt_addr[0]);
    sample_svp_mmz_free(sobel->dst_v1.phys_addr[0], sobel->dst_v1.virt_addr[0]);
    sample_svp_mmz_free(sobel->dst_v2.phys_addr[0], sobel->dst_v2.virt_addr[0]);

    sample_svp_close_file(sobel->fp_src);
    sample_svp_close_file(sobel->fp_dst_h1);
    sample_svp_close_file(sobel->fp_dst_h2);
    sample_svp_close_file(sobel->fp_dst_v1);
    sample_svp_close_file(sobel->fp_dst_v2);
}

static td_s32 sample_ive_sobel_create_img(ot_sample_ive_sobel_info *sobel, td_u32 width, td_u32 height)
{
    td_s32 ret;
    ret = sample_common_ive_create_image_by_cached(&(sobel->src1), OT_SVP_IMG_TYPE_U8C1, width, height);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create src1 image failed!\n", ret);

    ret = sample_common_ive_create_image_by_cached(&(sobel->src2), OT_SVP_IMG_TYPE_U8C1, width, height);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create src2 image failed!\n", ret);

    ret = sample_common_ive_create_image(&(sobel->dst_h1), OT_SVP_IMG_TYPE_S16C1, width, height);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create dstH1 image failed!\n", ret);

    ret = sample_common_ive_create_image(&(sobel->dst_h2), OT_SVP_IMG_TYPE_S16C1, width, height);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create dstH2 image failed!\n", ret);

    ret = sample_common_ive_create_image(&(sobel->dst_v1), OT_SVP_IMG_TYPE_S16C1, width, height);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create dstV1 image failed!\n", ret);

    ret = sample_common_ive_create_image(&(sobel->dst_v2), OT_SVP_IMG_TYPE_S16C1, width, height);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create dstV2 image failed!\n", ret);
    return TD_SUCCESS;
}

/*
 * function : show Sobel init
 */
static td_s32 sample_ive_sobel_init(ot_sample_ive_sobel_info *sobel, td_u32 width, td_u32 height)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    td_char path[PATH_MAX] = {0};
    td_s8 mask[OT_IVE_MASK_NUM] = { 0, 0, 0, 0, 0, 0, -1, 0, 1, 0, 0, -2, 0, 2, 0, 0, -1, 0, 1, 0, 0, 0, 0, 0, 0 };
    const td_char *src_sobel = "./data/input/sobel/sobel.yuv";
    const td_char *file = "./data/output/sobel";

    sample_svp_check_exps_return(sobel == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");
    (td_void)memset_s(sobel, sizeof(ot_sample_ive_sobel_info), 0, sizeof(ot_sample_ive_sobel_info));

    sobel->sobel_ctrl.out_ctrl = OT_IVE_SOBEL_OUT_CTRL_BOTH;
    ret = memcpy_s(sobel->sobel_ctrl.mask, OT_IVE_MASK_NUM, mask, OT_IVE_MASK_NUM);
    sample_svp_check_exps_return(ret != EOK, OT_ERR_IVE_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "memcpy_s mask failed!\n");

    ret = sample_ive_sobel_create_img(sobel, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Sobel create image failed!\n", ret);

    ret = TD_FAILURE;
    sample_svp_check_exps_goto((realpath(src_sobel, path) == NULL) || (strlen(src_sobel) > PATH_MAX),
        fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");
    sobel->fp_src = fopen(path, "rb");
    sample_svp_check_exps_goto(sobel->fp_src == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    /* sobelh1 */
    sample_svp_check_exps_goto(realpath(file, path) == NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");
    ret = strcat_s(path, PATH_MAX, "/sobelh1.dat");
    sample_svp_check_exps_goto(ret != EOK, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "strcat_s failed!\n");
    ret = TD_FAILURE;
    sobel->fp_dst_h1 = fopen(path, "wb");
    sample_svp_check_exps_goto(sobel->fp_dst_h1 == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    /* sobelh2 */
    sample_svp_check_exps_goto(realpath(file, path) == NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");
    ret = strcat_s(path, PATH_MAX, "/sobelh2.dat");
    sample_svp_check_exps_goto(ret != EOK, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "strcat_s failed!\n");
    ret = TD_FAILURE;
    sobel->fp_dst_h2 = fopen(path, "wb");
    sample_svp_check_exps_goto(sobel->fp_dst_h2 == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    /* sobelv1 */
    sample_svp_check_exps_goto(realpath(file, path) == NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");
    ret = strcat_s(path, PATH_MAX, "/sobelv1.dat");
    sample_svp_check_exps_goto(ret != EOK, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "strcat_s failed!\n");
    ret = TD_FAILURE;
    sobel->fp_dst_v1 = fopen(path, "wb");
    sample_svp_check_exps_goto(sobel->fp_dst_v1 == TD_NULL, fail,  SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    /* sobelv2 */
    sample_svp_check_exps_goto(realpath(file, path) == NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");
    ret = strcat_s(path, PATH_MAX, "/sobelv2.dat");
    sample_svp_check_exps_goto(ret != EOK, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "strcat_s failed!\n");
    ret = TD_FAILURE;
    sobel->fp_dst_v2 = fopen(path, "wb");
    sample_svp_check_exps_goto(sobel->fp_dst_v2 == TD_NULL, fail,  SAMPLE_SVP_ERR_LEVEL_ERROR, "Open file failed!\n");

    return TD_SUCCESS;
fail:
    sample_ive_sobel_uninit(sobel);
    return ret;
}

static td_s32 sample_ive_sobel_query_task(ot_ive_handle handle)
{
    td_s32 ret;
    td_bool is_block = TD_TRUE;
    td_bool is_finish = TD_FALSE;

    ret = ss_mpi_ive_query(handle, &is_finish, is_block);
    while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
        usleep(OT_SAMPLE_IVE_SOBEL_QUERY_SLEEP);
        ret = ss_mpi_ive_query(handle, &is_finish, is_block);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_query failed!\n", ret);

    return TD_SUCCESS;
}
/*
 * function : show Sobel proc
 */
static td_s32 sample_ive_sobel_proc(ot_sample_ive_sobel_info *sobel)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    td_bool is_instant = TD_TRUE;
    ot_ive_handle handle;
    td_u32 i;

    sample_svp_check_exps_return(sobel == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");

    for (i = 0; (i < 1) && (g_stop_signal == TD_FALSE); i++) {
        ret = sample_common_ive_read_file(&(sobel->src1), sobel->fp_src);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "read src file failed!\n");

        ret = memcpy_s(sample_svp_convert_addr_to_ptr(td_void, sobel->src2.virt_addr[0]),
            sobel->src2.stride[0] * sobel->src2.height,
            sample_svp_convert_addr_to_ptr(td_void, sobel->src1.virt_addr[0]),
            sobel->src2.stride[0] * sobel->src2.height);
        sample_svp_check_exps_return(ret != EOK, OT_ERR_IVE_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "memcpy_s src failed!\n");

        ret = ss_mpi_sys_flush_cache(sobel->src1.phys_addr[0], sample_svp_convert_addr_to_ptr(td_void,
            sobel->src1.virt_addr[0]), sobel->src1.stride[0] * sobel->src1.height);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_sys_flush_cache failed!\n", ret);

        ret = ss_mpi_ive_sobel(&handle, &sobel->src1, &sobel->dst_h1, &sobel->dst_v1, &sobel->sobel_ctrl, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "ss_mpi_ive_sobel failed!\n");

        ret = sample_ive_sobel_query_task(handle);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "sobel_query_task failed!\n");

        ret = sample_common_ive_write_file(&sobel->dst_h1, sobel->fp_dst_h1);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "write dstH1 failed!\n");

        ret = sample_common_ive_write_file(&sobel->dst_v1, sobel->fp_dst_v1);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "write dstV1 failed!\n");

        /* The result of sobel my be error,if you do not call ss_mpi_sys_flush_cache to flush cache */
        if (g_is_flush_cached == TD_TRUE) {
            ret = ss_mpi_sys_flush_cache(sobel->src2.phys_addr[0], sample_svp_convert_addr_to_ptr(td_void,
                sobel->src2.virt_addr[0]), sobel->src2.stride[0] * sobel->src2.height);
            sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Error(%#x),ss_mpi_sys_flush_cache failed!\n", ret);
        }

        ret = ss_mpi_ive_sobel(&handle, &sobel->src2, &sobel->dst_h2, &sobel->dst_v2, &sobel->sobel_ctrl, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "ss_mpi_ive_sobel failed!\n");

        ret = sample_ive_sobel_query_task(handle);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "sobel_query_task failed!\n");

        ret = sample_common_ive_write_file(&sobel->dst_h2, sobel->fp_dst_h2);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "write dstH2 failed!\n");

        ret = sample_common_ive_write_file(&sobel->dst_v2, sobel->fp_dst_v2);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "write dstV2 failed!\n");
    }
    return TD_SUCCESS;
}

static td_void sample_ive_sobel_stop(td_void)
{
    sample_ive_sobel_uninit(&g_sobel_info);
    (td_void)memset_s(&g_sobel_info, sizeof(g_sobel_info), 0, sizeof(g_sobel_info));
    sample_common_ive_mpi_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}
/*
 * function : show Sobel sample
 */
td_void sample_ive_sobel(td_void)
{
    td_s32 ret;
    const td_u32 width = OT_SAMPLE_IVE_SOBEL_SRC_WIDTH;
    const td_u32 height = OT_SAMPLE_IVE_SOBEL_SRC_HEIGHT;

    (td_void)memset_s(&g_sobel_info, sizeof(g_sobel_info), 0, sizeof(g_sobel_info));
    ret = sample_common_ive_check_mpi_init();
    sample_svp_check_exps_return_void(ret != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "ive_check_mpi_init failed!\n");

    ret = sample_ive_sobel_init(&g_sobel_info, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, sobel_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_ive_sobel_init failed!\n", ret);

    ret = sample_ive_sobel_proc(&g_sobel_info);
    if (g_stop_signal == TD_TRUE) {
        sample_ive_sobel_stop();
        return;
    }
    if (ret == TD_SUCCESS) {
        sample_svp_trace_info("Process success!\n");
    } else {
        sample_svp_trace_err("sobel process failed!\n");
    }

    g_stop_signal = TD_TRUE;
    sample_ive_sobel_uninit(&g_sobel_info);
    (td_void)memset_s(&g_sobel_info, sizeof(g_sobel_info), 0, sizeof(g_sobel_info));

sobel_fail:
    g_stop_signal = TD_TRUE;
    sample_common_ive_mpi_exit();
}


/*
 * function : Sobel sample signal handle
 */
td_void sample_ive_sobel_handle_sig(td_void)
{
    g_stop_signal = TD_TRUE;
}
