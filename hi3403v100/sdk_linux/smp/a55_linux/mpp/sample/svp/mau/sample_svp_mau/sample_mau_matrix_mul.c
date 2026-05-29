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
#include "sample_mau_matrix_mul.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ot_common_svp.h"
#include "ss_mpi_mau.h"
#include "sample_common_svp.h"
#include "sample_common_mau.h"

static sample_svp_mau_matrix_mul_info g_matrix_mul = {0};
static td_bool g_stop_signal = TD_FALSE;

static td_void sample_svp_mau_matrix_mul_get_mem_size(sample_svp_mau_matrix_mul_info *matrix_mul,
    td_u32 *size)
{
    td_u32 total_size = 0;
    /* left matrix */
    total_size += matrix_mul->src.left_matrix.stride * matrix_mul->src.left_matrix.shape.whc.height;
    /* right matrix */
    total_size += matrix_mul->src.right_matrix.stride * matrix_mul->src.right_matrix.shape.whc.height;

    /* left matrix idx */
    if (matrix_mul->ctrl.has_left_idx == TD_TRUE) {
        total_size += matrix_mul->src_idx.left_matrix.stride *
            matrix_mul->src_idx.left_matrix.shape.whc.height;
    }
    /* right matrix idx */
    if (matrix_mul->ctrl.has_right_idx == TD_TRUE) {
        total_size += matrix_mul->src_idx.right_matrix.stride *
            matrix_mul->src_idx.right_matrix.shape.whc.height;
    }
    /* chip dst matrix */
    total_size += matrix_mul->chip_dst.stride * matrix_mul->chip_dst.shape.whc.height;

    *size = total_size;
}

static td_void sample_svp_mau_matrix_mul_set_mem_addr(sample_svp_mau_matrix_mul_info *matrix_mul,
    td_u64 phys_addr, td_u64 virt_addr)
{
    td_u32 total_size = 0;

    /* left matrix */
    matrix_mul->src.left_matrix.phys_addr = phys_addr + total_size;
    matrix_mul->src.left_matrix.virt_addr = virt_addr + total_size;
    total_size += matrix_mul->src.left_matrix.stride * matrix_mul->src.left_matrix.shape.whc.height;

    /* right matrix */
    matrix_mul->src.right_matrix.phys_addr = phys_addr + total_size;
    matrix_mul->src.right_matrix.virt_addr = virt_addr + total_size;
    total_size += matrix_mul->src.right_matrix.stride * matrix_mul->src.right_matrix.shape.whc.height;

    /* left matrix idx */
    if (matrix_mul->ctrl.has_left_idx == TD_TRUE) {
        matrix_mul->src_idx.left_matrix.phys_addr = phys_addr + total_size;
        matrix_mul->src_idx.left_matrix.virt_addr = virt_addr + total_size;
        total_size += matrix_mul->src_idx.left_matrix.stride *
            matrix_mul->src_idx.left_matrix.shape.whc.height;
    }
    /* right matrix idx */
    if (matrix_mul->ctrl.has_right_idx == TD_TRUE) {
        matrix_mul->src_idx.right_matrix.phys_addr = phys_addr + total_size;
        matrix_mul->src_idx.right_matrix.virt_addr = virt_addr + total_size;
        total_size += matrix_mul->src_idx.right_matrix.stride *
            matrix_mul->src_idx.right_matrix.shape.whc.height;
    }
    /* chip dst matrix */
    matrix_mul->chip_dst.phys_addr = phys_addr + total_size;
    matrix_mul->chip_dst.virt_addr = virt_addr + total_size;
}

static td_s32 sample_svp_mau_matrix_mul_malloc_mem(sample_svp_mau_matrix_mul_info *matrix_mul)
{
    td_s32 ret;
    td_phys_addr_t phys_addr;
    td_void *virt_addr = TD_NULL;
    ot_svp_mem_info mem_info;

    /* malloc mem */
    sample_svp_mau_matrix_mul_get_mem_size(matrix_mul, &(mem_info.size));

    ret = sample_common_svp_malloc_cached("svp_mau_mul", TD_NULL, &phys_addr, (td_void **)(&virt_addr), mem_info.size);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, Malloc memory failed!\n");
    mem_info.phys_addr = (td_u64)phys_addr;
    mem_info.virt_addr = sample_svp_convert_ptr_to_addr(td_u64, virt_addr);
    ret = memset_s(sample_svp_convert_addr_to_ptr(td_void, mem_info.virt_addr), mem_info.size, 0, mem_info.size);
    sample_svp_check_exps_return(ret != EOK, OT_ERR_SVP_MAU_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "set mem_info failed!\n");
    ret = sample_common_svp_flush_cache(phys_addr,
        sample_svp_convert_addr_to_ptr(td_void, mem_info.virt_addr), mem_info.size);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, flush cache failed!\n");

    /* set addr */
    sample_svp_mau_matrix_mul_set_mem_addr(matrix_mul, mem_info.phys_addr, mem_info.virt_addr);

    return TD_SUCCESS;
}

static td_void sample_svp_mau_matrix_mul_set_dim_info(sample_svp_mau_matrix_mul_info *matrix_mul,
    sample_svp_mau_dim *mau_dim)
{
    td_u32 byte_num = sizeof(td_u32);

    /* left matrix */
    matrix_mul->src.left_matrix.num = 1;
    matrix_mul->src.left_matrix.shape.whc.chn = 1;
    matrix_mul->src.left_matrix.shape.whc.width = mau_dim->left_matrix_width;
    matrix_mul->src.left_matrix.shape.whc.height = mau_dim->left_matrix_height;
    matrix_mul->src.left_matrix.type = OT_SVP_BLOB_TYPE_FP32;
    matrix_mul->src.left_matrix.stride =
        sample_common_svp_align(mau_dim->left_matrix_width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);

    /* right matrix */
    matrix_mul->src.right_matrix.num = 1;
    matrix_mul->src.right_matrix.shape.whc.chn = 1;
    matrix_mul->src.right_matrix.shape.whc.width = mau_dim->right_matrix_width;
    matrix_mul->src.right_matrix.shape.whc.height = mau_dim->right_matrix_height;
    matrix_mul->src.right_matrix.type = OT_SVP_BLOB_TYPE_FP32;
    matrix_mul->src.right_matrix.stride =
        sample_common_svp_align(mau_dim->right_matrix_width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);

    /* left matrix idx */
    if (matrix_mul->ctrl.has_left_idx == TD_TRUE) {
        matrix_mul->src_idx.left_matrix.num = 1;
        matrix_mul->src_idx.left_matrix.shape.whc.chn = 1;
        matrix_mul->src_idx.left_matrix.shape.whc.width = mau_dim->left_idx_width;
        matrix_mul->src_idx.left_matrix.shape.whc.height = 1;
        matrix_mul->src_idx.left_matrix.type = OT_SVP_BLOB_TYPE_U32;
        matrix_mul->src_idx.left_matrix.stride =
            sample_common_svp_align(mau_dim->left_idx_width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);
    }

    /* right matrix idx */
    if (matrix_mul->ctrl.has_right_idx == TD_TRUE) {
        matrix_mul->src_idx.right_matrix.num = 1;
        matrix_mul->src_idx.right_matrix.shape.whc.chn = 1;
        matrix_mul->src_idx.right_matrix.shape.whc.width = mau_dim->right_idx_width;
        matrix_mul->src_idx.right_matrix.shape.whc.height = 1;
        matrix_mul->src_idx.right_matrix.type = OT_SVP_BLOB_TYPE_U32;
        matrix_mul->src_idx.right_matrix.stride =
            sample_common_svp_align(mau_dim->right_idx_width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);
    }

    /* op_result */
    matrix_mul->chip_dst.num = 1;
    matrix_mul->chip_dst.shape.whc.chn = 1;
    matrix_mul->chip_dst.shape.whc.width = ((matrix_mul->ctrl.has_right_idx == TD_TRUE) ?
        matrix_mul->src_idx.right_matrix.shape.whc.width : matrix_mul->src.right_matrix.shape.whc.height);
    matrix_mul->chip_dst.shape.whc.height = ((matrix_mul->ctrl.has_left_idx == TD_TRUE) ?
        matrix_mul->src_idx.left_matrix.shape.whc.width : matrix_mul->src.left_matrix.shape.whc.height);
    matrix_mul->chip_dst.type = OT_SVP_BLOB_TYPE_FP32;
    matrix_mul->chip_dst.stride =
        sample_common_svp_align(matrix_mul->chip_dst.shape.whc.width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);
}

static td_s32 sample_svp_mau_matrix_check_mul_info(const sample_svp_mau_matrix_mul_info *matrix_mul)
{
    sample_svp_check_exps_return(matrix_mul == TD_NULL, OT_ERR_SVP_MAU_NULL_PTR,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, matrix_mul is TD_NULL!\n");

    sample_svp_check_exps_return((matrix_mul->is_record_mem_info != TD_FALSE) &&
        (matrix_mul->is_record_mem_info != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),matrix_mul->is_record_mem_info(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, matrix_mul->is_record_mem_info, TD_FALSE, TD_TRUE);
    sample_svp_check_exps_return((matrix_mul->ctrl.fp32_to_fp16_en != TD_FALSE) &&
        (matrix_mul->ctrl.fp32_to_fp16_en != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),matrix_mul->ctrl.fp32_to_fp16_en(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, matrix_mul->ctrl.fp32_to_fp16_en, TD_FALSE, TD_TRUE);
    sample_svp_check_exps_return((matrix_mul->ctrl.is_instant != TD_FALSE) &&
        (matrix_mul->ctrl.is_instant != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),matrix_mul->ctrl.is_instant(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, matrix_mul->ctrl.is_instant, TD_FALSE, TD_TRUE);
    sample_svp_check_exps_return((matrix_mul->ctrl.mau_id < OT_SVP_MAU_ID_0) ||
        (matrix_mul->ctrl.mau_id >= OT_SVP_MAU_ID_BUTT), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),matrix_mul->ctrl.mau_id(%d) must be [%d, %d)\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, matrix_mul->ctrl.mau_id, OT_SVP_MAU_ID_0, OT_SVP_MAU_ID_BUTT);

    sample_svp_check_exps_return(matrix_mul->ctrl.out_type != OT_SVP_MAU_OUT_OP_RESULT, OT_ERR_SVP_MAU_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x), matrix_mul->ctrl.out_type(%d) must be %d\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, matrix_mul->ctrl.out_type, OT_SVP_MAU_OUT_OP_RESULT);
    sample_svp_check_exps_return((matrix_mul->ctrl.has_left_idx != TD_FALSE) &&
        (matrix_mul->ctrl.has_left_idx != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),matrix_mul->ctrl.has_left_idx(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, matrix_mul->ctrl.has_left_idx, TD_FALSE, TD_TRUE);
    sample_svp_check_exps_return((matrix_mul->ctrl.has_right_idx != TD_FALSE) &&
        (matrix_mul->ctrl.has_right_idx != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),matrix_mul->ctrl.has_right_idx(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, matrix_mul->ctrl.has_right_idx, TD_FALSE, TD_TRUE);

    return TD_SUCCESS;
}
static td_s32 sample_svp_mau_matrix_check_param(const sample_svp_mau_matrix_mul_info *matrix_mul,
    const sample_svp_mau_dim *mau_dim)
{
    td_s32 ret;

    ret = sample_svp_mau_matrix_check_mul_info(matrix_mul);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), sample_svp_mau_matrix_check_mul_info fail\n", ret);

    sample_svp_check_exps_return(mau_dim == TD_NULL, OT_ERR_SVP_MAU_NULL_PTR,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, mau_dim is TD_NULL!\n");
    sample_svp_check_exps_return(mau_dim->left_matrix_width != mau_dim->right_matrix_width,
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, the width of left_matrix must equal to right_matrix's!\n");

    sample_svp_check_exps_return((mau_dim->left_matrix_height == 0) ||
        (mau_dim->left_matrix_height > SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,mau_dim->left_matrix_height(%u) must be (0, %u]\n",
        mau_dim->left_matrix_height, SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT);
    sample_svp_check_exps_return((mau_dim->left_matrix_width == 0) ||
        (mau_dim->left_matrix_width > SAMPLE_SVP_MAU_MATRIX_MAX_FP32_WIDTH), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x),mau_dim->left_matrix_width(%u) must be (0, %u]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dim->left_matrix_width, SAMPLE_SVP_MAU_MATRIX_MAX_FP32_WIDTH);
    sample_svp_check_exps_return((mau_dim->right_matrix_height == 0) ||
        (mau_dim->right_matrix_height > SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,mau_dim->right_matrix_height(%u) must be (0, %u]\n",
        mau_dim->right_matrix_height, SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT);
    sample_svp_check_exps_return((mau_dim->right_matrix_width == 0) ||
        (mau_dim->right_matrix_width > SAMPLE_SVP_MAU_MATRIX_MAX_FP32_WIDTH), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x),mau_dim->right_matrix_width(%u) must be (0, %u]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dim->right_matrix_width, SAMPLE_SVP_MAU_MATRIX_MAX_FP32_WIDTH);

    if (matrix_mul->ctrl.has_left_idx == TD_TRUE) {
        sample_svp_check_exps_return((mau_dim->left_idx_width == 0) ||
            (mau_dim->left_idx_width > SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x),mau_dim->left_idx_width(%u) must be (0, %u]\n",
            OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dim->left_idx_width, SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT);
    }
    if (matrix_mul->ctrl.has_right_idx == TD_TRUE) {
        sample_svp_check_exps_return((mau_dim->right_idx_width == 0) ||
            (mau_dim->right_idx_width > SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x),mau_dim->right_idx_width(%u) must be (0, %u]\n",
            OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dim->right_idx_width, SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT);
    }
    return TD_SUCCESS;
}
/* function :set the matrix dim info, malloc the memory and get matix data */
static td_s32 sample_svp_mau_matrix_mul_init(sample_svp_mau_matrix_mul_info *matrix_mul,
    sample_svp_mau_dim *mau_dim)
{
    td_s32 ret;
    ot_svp_mem_info mem_info;

    ret = sample_svp_mau_matrix_check_param(matrix_mul, mau_dim);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), sample_svp_mau_matrix_check_param fail\n", ret);
    /* set matrix info */
    sample_svp_mau_matrix_mul_set_dim_info(matrix_mul, mau_dim);

    /* malloc memory */
    ret = sample_svp_mau_matrix_mul_malloc_mem(matrix_mul);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), sample_svp_mau_matrix_mul_malloc_mem failed!\n", ret);

    if (matrix_mul->is_record_mem_info == TD_TRUE) {
        if (matrix_mul->ctrl.has_left_idx == TD_TRUE) {
            mem_info.phys_addr = matrix_mul->src_idx.left_matrix.phys_addr;
            mem_info.virt_addr = matrix_mul->src_idx.left_matrix.virt_addr;
            mem_info.size = matrix_mul->src_idx.left_matrix.stride;
            ret = ss_mpi_svp_mau_add_mem_info(&mem_info);
            sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Error(%#x), ss_mpi_svp_mau_add_mem_info failed!\n", ret);
        }
        if (matrix_mul->ctrl.has_right_idx == TD_TRUE) {
            mem_info.phys_addr = matrix_mul->src_idx.right_matrix.phys_addr;
            mem_info.virt_addr = matrix_mul->src_idx.right_matrix.virt_addr;
            mem_info.size = matrix_mul->src_idx.right_matrix.stride;
            ret = ss_mpi_svp_mau_add_mem_info(&mem_info);
            sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Error(%#x), ss_mpi_svp_mau_add_mem_info failed!\n", ret);
        }
    }

    /* get matrix data */
    ret = sample_svp_mau_generate_matrix_data(&(matrix_mul->src));
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), gen rand data failed!\n", ret);

    return TD_SUCCESS;
}

static td_void sample_svp_mau_matrix_mul_deinit(sample_svp_mau_matrix_mul_info *matrix_mul)
{
    ot_svp_mem_info mem_info;

    if (matrix_mul->is_record_mem_info == TD_TRUE) {
        if (matrix_mul->ctrl.has_left_idx == TD_TRUE) {
            mem_info.phys_addr = matrix_mul->src_idx.left_matrix.phys_addr;
            mem_info.virt_addr = matrix_mul->src_idx.left_matrix.virt_addr;
            mem_info.size = matrix_mul->src_idx.left_matrix.stride;
            ss_mpi_svp_mau_rm_mem_info(&mem_info);
        }
        if (matrix_mul->ctrl.has_right_idx == TD_TRUE) {
            mem_info.phys_addr = matrix_mul->src_idx.right_matrix.phys_addr;
            mem_info.virt_addr = matrix_mul->src_idx.right_matrix.virt_addr;
            mem_info.size = matrix_mul->src_idx.right_matrix.stride;
            ss_mpi_svp_mau_rm_mem_info(&mem_info);
        }
    }

    sample_svp_mmz_free(matrix_mul->src.left_matrix.phys_addr, matrix_mul->src.left_matrix.virt_addr);
}

static td_void sample_svp_mau_matrix_mul_stop(td_void)
{
    g_stop_signal = TD_FALSE;
    sample_svp_mau_matrix_mul_deinit(&g_matrix_mul);
    (td_void)memset_s(&g_matrix_mul, sizeof(sample_svp_mau_matrix_mul_info),
        0, sizeof(sample_svp_mau_matrix_mul_info));
    sample_common_svp_mau_mpi_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

static td_s32 sample_svp_mau_matrix_mul_proc(sample_svp_mau_matrix_mul_info *matrix_mul)
{
    td_s32 ret;
    ot_svp_mau_handle handle = 0;
    td_bool is_finish = TD_FALSE;

    if (g_stop_signal == TD_TRUE) {
        sample_svp_mau_matrix_mul_stop();
        return TD_FAILURE;
    }
    /* hardware execution */
    printf("ss_mpi_svp_mau_matrix_mul start\n");
    ret = ss_mpi_svp_mau_matrix_mul(&handle, &matrix_mul->src, &matrix_mul->src_idx,
        &matrix_mul->ctrl, &matrix_mul->chip_dst);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), ss_mpi_svp_mau_matrix_mul failed!\n", ret);

    ret = ss_mpi_svp_mau_query(matrix_mul->ctrl.mau_id, handle, &is_finish, TD_TRUE);
    while (ret == OT_ERR_SVP_MAU_QUERY_TIMEOUT) {
        usleep(SAMPLE_SVP_MAU_QUERY_SLEEP_TIME);
        ret = ss_mpi_svp_mau_query(matrix_mul->ctrl.mau_id, handle, &is_finish, TD_TRUE);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_svp_mau_query failed!\n", ret);

    ret = sample_common_svp_flush_cache(matrix_mul->chip_dst.phys_addr,
        sample_svp_convert_addr_to_ptr(td_void, matrix_mul->chip_dst.virt_addr),
        matrix_mul->chip_dst.shape.whc.height * matrix_mul->chip_dst.stride);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, flush cache failed!\n");

    return TD_SUCCESS;
}

static td_void sample_svp_mau_matrix_mul_print_idx_info(const sample_svp_mau_matrix_mul_info *matrix_mul)
{
    td_u32 j;
    td_u32 *idx_data = TD_NULL;
    /* left idx */
    if (matrix_mul->ctrl.has_left_idx == TD_TRUE) {
        printf("==== The data of left_idx====\n");
        idx_data = sample_svp_convert_addr_to_ptr(td_u32, matrix_mul->src_idx.left_matrix.virt_addr);
        for (j = 0; j < matrix_mul->src_idx.left_matrix.shape.whc.width; j++) {
            printf("%8u", idx_data[j]);
        }
        printf("\n");
    }

    /* right idx */
    if (matrix_mul->ctrl.has_right_idx == TD_TRUE) {
        printf("==== The data of right_idx====\n");
        idx_data = sample_svp_convert_addr_to_ptr(td_u32, matrix_mul->src_idx.right_matrix.virt_addr);
        for (j = 0; j < matrix_mul->src_idx.right_matrix.shape.whc.width; j++) {
            printf("%8u", idx_data[j]);
        }
        printf("\n");
    }
}

static td_void sample_svp_mau_matrix_mul_print_result(const sample_svp_mau_matrix_mul_info *matrix_mul)
{
    td_float *matrix_data = TD_NULL;
    td_u32 i;
    td_u32 j;
    td_u32 stride;

    /* left matrix */
    printf("==== The data of left matrix====\n");
    stride = matrix_mul->src.left_matrix.stride / sizeof(td_u32);
    matrix_data = sample_svp_convert_addr_to_ptr(td_float, matrix_mul->src.left_matrix.virt_addr);
    for (i = 0; i < matrix_mul->src.left_matrix.shape.whc.height; i++) {
        for (j = 0; j < matrix_mul->src.left_matrix.shape.whc.width; j++) {
            printf("%f ", matrix_data[j]);
        }
        printf("\n");
        matrix_data += stride;
    }

    /* right matrix */
    printf("==== The data of right matrix====\n");
    stride = matrix_mul->src.right_matrix.stride / sizeof(td_u32);
    matrix_data = sample_svp_convert_addr_to_ptr(td_float, matrix_mul->src.right_matrix.virt_addr);
    for (i = 0; i < matrix_mul->src.right_matrix.shape.whc.height; i++) {
        for (j = 0; j < matrix_mul->src.right_matrix.shape.whc.width; j++) {
            printf("%f ", matrix_data[j]);
        }
        printf("\n");
        matrix_data += stride;
    }

    /* idx info */
    sample_svp_mau_matrix_mul_print_idx_info(matrix_mul);

    /* op result */
    printf("==== The data of result matrix====\n");
    stride = matrix_mul->chip_dst.stride / sizeof(td_u32);
    matrix_data = sample_svp_convert_addr_to_ptr(td_float, matrix_mul->chip_dst.virt_addr);
    for (i = 0; i < matrix_mul->chip_dst.shape.whc.height; i++) {
        for (j = 0; j < matrix_mul->chip_dst.shape.whc.width; j++) {
            printf("%f ", matrix_data[j]);
        }
        printf("\n");
        matrix_data += stride;
    }
}

/* function : show the sample of mau matrix multiplication */
td_void sample_svp_mau_matrix_mul(td_void)
{
    td_s32 ret;
    sample_svp_mau_dim mau_dim;

    ret = sample_common_svp_mau_check_mau_mpi_init();
    sample_svp_check_exps_return_void(ret != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "mau_check_mau_mpi_init failed!\n");

    /* set ctrl info */
    g_matrix_mul.is_record_mem_info = TD_FALSE;
    g_matrix_mul.ctrl.out_type = OT_SVP_MAU_OUT_OP_RESULT;
    g_matrix_mul.ctrl.fp32_to_fp16_en = TD_FALSE; /* left_matrix and right_matrix are fp32 */
    g_matrix_mul.ctrl.is_instant = TD_TRUE;
    g_matrix_mul.ctrl.mau_id = OT_SVP_MAU_ID_0;
    g_matrix_mul.ctrl.has_left_idx = TD_FALSE;
    g_matrix_mul.ctrl.has_right_idx = TD_FALSE;

    /* the dim of left_marix, right_matrix */
    mau_dim.left_matrix_height = SAMPLE_SVP_MAU_LEFT_MATRIX_HEIGHT;
    mau_dim.left_matrix_width = SAMPLE_SVP_MAU_LEFT_MATRIX_WIDTH;
    mau_dim.right_matrix_height = SAMPLE_SVP_MAU_RIGHT_MATRIX_HEIGHT;
    mau_dim.right_matrix_width = SAMPLE_SVP_MAU_RIGHT_MATRIX_WIDTH;
    mau_dim.left_idx_width = SAMPLE_SVP_MAU_MATRIX_IDX_WIDTH;
    mau_dim.right_idx_width = SAMPLE_SVP_MAU_MATRIX_IDX_WIDTH;

    /* init */
    ret = sample_svp_mau_matrix_mul_init(&g_matrix_mul, &mau_dim);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_matrix_mul_init failed!\n", ret);

    if (g_stop_signal == TD_TRUE) {
        sample_svp_mau_matrix_mul_stop();
        return;
    }
    /* set idx */
    ret = sample_svp_mau_set_idx_info(&(g_matrix_mul.src_idx), &(g_matrix_mul.ctrl), &(g_matrix_mul.src));
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_matrix_mul_set_idx_info failed!\n", ret);

    /* the process of mau matrix product */
    ret = sample_svp_mau_matrix_mul_proc(&g_matrix_mul);
    sample_svp_check_exps_return_void(ret == TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "svp_mau_matrix_mul_proc failed!\n");
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_matrix_mul_process failed!\n", ret);

    /* print the result */
    if (g_stop_signal == TD_TRUE) {
        sample_svp_mau_matrix_mul_stop();
        return;
    }
    sample_svp_mau_matrix_mul_print_result(&g_matrix_mul);

fail:
    /* deinit */
    sample_svp_mau_matrix_mul_deinit(&g_matrix_mul);
    sample_common_svp_mau_mpi_exit();
}

/* function : mau_matrix_mul sample signal handle */
td_void sample_svp_mau_matrix_mul_handle_sig(td_void)
{
    g_stop_signal = TD_TRUE;
}
