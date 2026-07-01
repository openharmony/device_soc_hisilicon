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

#define OT_SAMPLE_IVE_TEST_MEM_QUERY_SLEEP      100
#define OT_SAMPLE_IVE_TEST_MEM_QUERY_D1_WIDTH   720
#define OT_SAMPLE_IVE_TEST_MEM_QUERY_D1_HEIGHT  576

typedef struct {
    ot_svp_src_img src1;
    ot_svp_src_img src2;
    ot_svp_dst_img dst;
    ot_svp_dst_mem_info hist;
    ot_ive_sub_ctrl sub_ctrl;
    FILE *fp_src;
    FILE *fp_dst;
} ot_test_memory_info;

static ot_test_memory_info g_test_mem_info;
static td_bool g_stop_signal = TD_FALSE;

/*
 * function : test memory uninit
 */
static td_void sample_ive_test_memory_uninit(ot_test_memory_info *test_mem)
{
    sample_svp_check_exps_return_void(test_mem == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");
    sample_svp_mmz_free(test_mem->src1.phys_addr[0], test_mem->src1.virt_addr[0]);
    sample_svp_mmz_free(test_mem->src2.phys_addr[0], test_mem->src2.virt_addr[0]);
    sample_svp_mmz_free(test_mem->dst.phys_addr[0], test_mem->dst.virt_addr[0]);
    sample_svp_mmz_free(test_mem->hist.phys_addr, test_mem->hist.virt_addr);

    sample_svp_close_file(test_mem->fp_src);
    sample_svp_close_file(test_mem->fp_dst);
}

/*
 * function : test memory init
 */
static td_s32 sample_ive_test_memory_init(ot_test_memory_info *test_mem, td_u32 width, td_u32 height)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    td_u32 size;
    td_char path[PATH_MAX] = {0};
    const td_char *src_file = "./data/input/testmem/test_mem_in.yuv";

    sample_svp_check_exps_return(test_mem == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");
    sample_svp_check_exps_return((strlen(src_file) > PATH_MAX) || (realpath(src_file, path) == TD_NULL),
        OT_ERR_IVE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR, "invalid file!\n");

    (td_void)memset_s(test_mem, sizeof(ot_test_memory_info), 0, sizeof(ot_test_memory_info));

    ret = sample_common_ive_create_image(&(test_mem->src1), OT_SVP_IMG_TYPE_U8C1, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create src1 image failed!\n", ret);

    ret = sample_common_ive_create_image(&(test_mem->src2), OT_SVP_IMG_TYPE_U8C1, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create src2 image failed!\n", ret);

    ret = sample_common_ive_create_image(&(test_mem->dst), OT_SVP_IMG_TYPE_U8C1, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create dst image failed!\n", ret);

    size = OT_IVE_HIST_NUM * sizeof(td_u32);
    ret = sample_common_ive_create_mem_info(&(test_mem->hist), size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create hist meminfo failed!\n", ret);

    test_mem->sub_ctrl.mode = OT_IVE_SUB_MODE_ABS;

    /* open src file */
    ret = TD_FAILURE;
    test_mem->fp_src = fopen(path, "rb");
    sample_svp_check_exps_goto(test_mem->fp_src == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,Open file %s failed!\n", path);

    /* open dst file */
    sample_svp_check_exps_goto(realpath("./data/output/testmem", path) == NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "invalid file!\n");
    ret = strcat_s(path, PATH_MAX, "/test_mem_out.yuv");
    sample_svp_check_exps_goto(ret != EOK, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "strcat_s failed!\n");
    ret = TD_FAILURE;
    test_mem->fp_dst = fopen(path, "wb");
    sample_svp_check_exps_goto(test_mem->fp_dst == TD_NULL, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,Open file %s failed!\n", path);

    return TD_SUCCESS;
fail:
    sample_ive_test_memory_uninit(test_mem);
    return ret;
}

static td_void sample_ive_test_memory_init_data(const ot_test_memory_info *test_mem, ot_svp_src_data *src_data,
    ot_svp_dst_data *dst_data)
{
    src_data->virt_addr = test_mem->src1.virt_addr[0];
    src_data->phys_addr = test_mem->src1.phys_addr[0];
    src_data->width = test_mem->src1.width;
    src_data->height = test_mem->src1.height;
    src_data->stride = test_mem->src1.stride[0];

    dst_data->virt_addr = test_mem->src2.virt_addr[0];
    dst_data->phys_addr = test_mem->src2.phys_addr[0];
    dst_data->width = test_mem->src2.width;
    dst_data->height = test_mem->src2.height;
    dst_data->stride = test_mem->src2.stride[0];
}
/*
 * function : test memory
 */
static td_s32 sample_ive_test_memory_proc(ot_test_memory_info *test_mem)
{
    td_s32 ret;
    td_u32 *hist_ptr = TD_NULL;
    td_u32 i;
    td_u32 j;
    ot_ive_handle handle;
    td_bool is_instant = TD_FALSE;
    td_bool is_finish = TD_FALSE;
    ot_svp_src_data src_data;
    ot_svp_dst_data dst_data;
    ot_ive_dma_ctrl dma_ctrl;

    sample_svp_check_exps_return(test_mem == TD_NULL, OT_ERR_IVE_NULL_PTR,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "test_mem can't be null\n");

    for (j = 0; (j < 1) && (g_stop_signal == TD_FALSE); j++) {
        ret = sample_common_ive_read_file(&(test_mem->src1), test_mem->fp_src);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "read src file failed!\n");

        dma_ctrl.mode = OT_IVE_DMA_MODE_DIRECT_COPY;

        sample_ive_test_memory_init_data(test_mem, &src_data, &dst_data);

        ret = ss_mpi_ive_dma(&handle, &src_data, &dst_data, &dma_ctrl, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "ss_mpi_ive_dma failed!\n");

        ret = ss_mpi_ive_sub(&handle, &test_mem->src1, &test_mem->src2, &test_mem->dst,
            &test_mem->sub_ctrl, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_sub failed!\n", ret);

        is_instant = TD_TRUE;
        ret = ss_mpi_ive_hist(&handle, &test_mem->dst, &test_mem->hist, is_instant);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "ss_mpi_ive_hist failed!\n");

        ret = ss_mpi_ive_query(handle, &is_finish, TD_TRUE);
        while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
            usleep(OT_SAMPLE_IVE_TEST_MEM_QUERY_SLEEP);
            ret = ss_mpi_ive_query(handle, &is_finish, TD_TRUE);
        }
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "ss_mpi_ive_query failed!\n");

        ret = sample_common_ive_write_file(&test_mem->dst, test_mem->fp_dst);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "write dst file failed!\n");

        hist_ptr = sample_svp_convert_addr_to_ptr(td_u32, test_mem->hist.virt_addr);
        if (hist_ptr[0] != (test_mem->src1.width * test_mem->src1.height)) {
            ret = TD_FAILURE;
            sample_svp_trace_err("Test mem error,hist_ptr[0] = %d\n", hist_ptr[0]);
            for (i = 1; i < OT_IVE_HIST_NUM; i++) {
                sample_svp_check_exps_trace(hist_ptr[i] != 0, SAMPLE_SVP_ERR_LEVEL_ERROR,
                    "Test mem error, hist_ptr[%d] = %d\n", i, hist_ptr[i]);
            }
        }
    }
    return TD_SUCCESS;
}

static td_void sample_ive_test_mem_stop(td_void)
{
    sample_ive_test_memory_uninit(&g_test_mem_info);
    (td_void)memset_s(&g_test_mem_info, sizeof(g_test_mem_info), 0, sizeof(g_test_mem_info));
    sample_common_ive_mpi_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}
/*
 * function : Show test memory sample
 */
td_void sample_ive_test_memory(td_void)
{
    td_s32 ret;
    const td_u32 width = OT_SAMPLE_IVE_TEST_MEM_QUERY_D1_WIDTH;
    const td_u32 height = OT_SAMPLE_IVE_TEST_MEM_QUERY_D1_HEIGHT;

    (td_void)memset_s(&g_test_mem_info, sizeof(g_test_mem_info), 0, sizeof(g_test_mem_info));
    ret = sample_common_ive_check_mpi_init();
    sample_svp_check_exps_return_void(ret != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "ive_check_mpi_init failed!\n");

    ret = sample_ive_test_memory_init(&g_test_mem_info, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, test_memory_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_ive_test_memory_init failed!\n", ret);

    ret = sample_ive_test_memory_proc(&g_test_mem_info);
    if (g_stop_signal == TD_TRUE) {
        sample_ive_test_mem_stop();
        return;
    }
    if (ret == TD_SUCCESS) {
        sample_svp_trace_info("Process success!\n");
    } else {
        sample_svp_trace_err("test_memory process failed\n");
    }

    g_stop_signal = TD_TRUE;
    sample_ive_test_memory_uninit(&g_test_mem_info);
    (td_void)memset_s(&g_test_mem_info, sizeof(g_test_mem_info), 0, sizeof(g_test_mem_info));

test_memory_fail:
    g_stop_signal = TD_TRUE;
    sample_common_ive_mpi_exit();
}

/*
 * function : TestMemory sample signal handle
 */
td_void sample_ive_test_memory_handle_sig(td_void)
{
    g_stop_signal = TD_TRUE;
}
