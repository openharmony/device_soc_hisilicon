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
#include "sample_mau_dist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>
#include <float.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ot_common_svp.h"
#include "ss_mpi_mau.h"
#include "sample_common_svp.h"
#include "sample_common_mau.h"

static sample_svp_mau_dist g_cos_dist = {0};
static sample_svp_mau_dist g_euclid_dist = {0};
static td_bool g_stop_cos_signal = TD_FALSE;
static td_bool g_stop_euclid_signal = TD_FALSE;

static td_void sample_svp_mau_dist_get_mem_size(const sample_svp_mau_dist *mau_dist,
    td_u32 *size)
{
    td_u32 total_size = 0;
    /* left matrix */
    total_size += mau_dist->src.left_matrix.stride * mau_dist->src.left_matrix.shape.whc.height;
    /* right matrix */
    total_size += mau_dist->src.right_matrix.stride * mau_dist->src.right_matrix.shape.whc.height;

    /* left matrix idx */
    if (mau_dist->ctrl.has_left_idx == TD_TRUE) {
        total_size += mau_dist->src_idx.left_matrix.stride *
            mau_dist->src_idx.left_matrix.shape.whc.height;
    }
    /* right matrix idx */
    if (mau_dist->ctrl.has_right_idx == TD_TRUE) {
        total_size += mau_dist->src_idx.right_matrix.stride *
            mau_dist->src_idx.right_matrix.shape.whc.height;
    }

    /*  dst matrix */
    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_OP_RESULT) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        total_size += mau_dist->dst.op_result.stride * mau_dist->dst.op_result.shape.whc.height;
    }
    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_TOP_N) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        total_size += mau_dist->dst.top_n.stride * mau_dist->dst.top_n.shape.whc.height;
        total_size += mau_dist->dst.top_n_idx.stride * mau_dist->dst.top_n_idx.shape.whc.height;
    }

    *size = total_size;
}

static td_void sample_svp_mau_dist_set_mem_addr(sample_svp_mau_dist *mau_dist,
    td_u64 phys_addr, td_u64 virt_addr)
{
    td_u32 total_size = 0;

    /* left matrix */
    mau_dist->src.left_matrix.phys_addr = phys_addr + total_size;
    mau_dist->src.left_matrix.virt_addr = virt_addr + total_size;
    total_size += mau_dist->src.left_matrix.stride * mau_dist->src.left_matrix.shape.whc.height;

    /* right matrix */
    mau_dist->src.right_matrix.phys_addr = phys_addr + total_size;
    mau_dist->src.right_matrix.virt_addr = virt_addr + total_size;
    total_size += mau_dist->src.right_matrix.stride * mau_dist->src.right_matrix.shape.whc.height;

    /* left matrix idx */
    if (mau_dist->ctrl.has_left_idx == TD_TRUE) {
        mau_dist->src_idx.left_matrix.phys_addr = phys_addr + total_size;
        mau_dist->src_idx.left_matrix.virt_addr = virt_addr + total_size;
        total_size += mau_dist->src_idx.left_matrix.stride *
            mau_dist->src_idx.left_matrix.shape.whc.height;
    }

    /* right matrix idx */
    if (mau_dist->ctrl.has_right_idx == TD_TRUE) {
        mau_dist->src_idx.right_matrix.phys_addr = phys_addr + total_size;
        mau_dist->src_idx.right_matrix.virt_addr = virt_addr + total_size;
        total_size += mau_dist->src_idx.right_matrix.stride *
            mau_dist->src_idx.right_matrix.shape.whc.height;
    }

    /* dst matrix */
    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_OP_RESULT) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        mau_dist->dst.op_result.phys_addr = phys_addr + total_size;
        mau_dist->dst.op_result.virt_addr = virt_addr + total_size;
        total_size += mau_dist->dst.op_result.stride * mau_dist->dst.op_result.shape.whc.height;
    }
    /* dst top_n */
    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_TOP_N) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        mau_dist->dst.top_n.phys_addr = phys_addr + total_size;
        mau_dist->dst.top_n.virt_addr = virt_addr + total_size;
        total_size += mau_dist->dst.top_n.stride * mau_dist->dst.top_n.shape.whc.height;

        mau_dist->dst.top_n_idx.phys_addr = phys_addr + total_size;
        mau_dist->dst.top_n_idx.virt_addr = virt_addr + total_size;
    };
}

static td_s32 sample_svp_mau_dist_malloc_mem(sample_svp_mau_dist *mau_dist)
{
    td_s32 ret;
    td_phys_addr_t phys_addr;
    td_void *virt_addr = TD_NULL;
    ot_svp_mem_info mem_info;

    /* malloc mem */
    sample_svp_mau_dist_get_mem_size(mau_dist, &(mem_info.size));

    ret = sample_common_svp_malloc_cached("svp_mau_dist", TD_NULL,
        &phys_addr, (td_void **)(&virt_addr), mem_info.size);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, Malloc memory failed!\n");
    mem_info.phys_addr = (td_u64)phys_addr;
    mem_info.virt_addr = sample_svp_convert_ptr_to_addr(td_u64, virt_addr);

    ret = memset_s(sample_svp_convert_addr_to_ptr(td_void, mem_info.virt_addr), mem_info.size, 0, mem_info.size);
    sample_svp_check_exps_return(ret != EOK, OT_ERR_SVP_MAU_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "set mem_info failed!\n");
    ret = sample_common_svp_flush_cache(mem_info.phys_addr,
        sample_svp_convert_addr_to_ptr(td_void, mem_info.virt_addr), mem_info.size);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, flush cache failed!\n");

    /* set addr */
    sample_svp_mau_dist_set_mem_addr(mau_dist, mem_info.phys_addr, mem_info.virt_addr);

    return TD_SUCCESS;
}

static td_void sample_svp_mau_dist_set_result_dim_info(sample_svp_mau_dist *mau_dist, td_u32 top_n)
{
    td_u32 byte_num = sizeof(td_u32);
    td_u32 top_n_width;

    /* op_result */
    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_OP_RESULT) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        mau_dist->dst.op_result.num = 1;
        mau_dist->dst.op_result.shape.whc.chn = 1;
        mau_dist->dst.op_result.shape.whc.width = ((mau_dist->ctrl.has_right_idx == TD_TRUE) ?
            mau_dist->src_idx.right_matrix.shape.whc.width : mau_dist->src.right_matrix.shape.whc.height);
        mau_dist->dst.op_result.shape.whc.height = ((mau_dist->ctrl.has_left_idx == TD_TRUE) ?
            mau_dist->src_idx.left_matrix.shape.whc.width : mau_dist->src.left_matrix.shape.whc.height);
        mau_dist->dst.op_result.type = OT_SVP_BLOB_TYPE_FP32;
        mau_dist->dst.op_result.stride =
            sample_common_svp_align(mau_dist->dst.op_result.shape.whc.width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);
    }

    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_TOP_N) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        /* topN */
        top_n_width = sample_svp_mau_min(SAMPLE_SVP_MAU_MAX_TOP_N, top_n);
        top_n_width = sample_svp_mau_min(top_n_width, (mau_dist->ctrl.has_right_idx == TD_TRUE) ?
            mau_dist->src_idx.right_matrix.shape.whc.width : mau_dist->src.right_matrix.shape.whc.height);

        mau_dist->dst.top_n.num = 1;
        mau_dist->dst.top_n.shape.whc.chn = 1;
        mau_dist->dst.top_n.shape.whc.width = top_n_width;
        mau_dist->dst.top_n.shape.whc.height = ((mau_dist->ctrl.has_left_idx == TD_TRUE) ?
            mau_dist->src_idx.left_matrix.shape.whc.width : mau_dist->src.left_matrix.shape.whc.height);
        mau_dist->dst.top_n.type = OT_SVP_BLOB_TYPE_FP32;
        mau_dist->dst.top_n.stride =
            sample_common_svp_align(top_n_width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);

        /* topN idx */
        mau_dist->dst.top_n_idx.num = 1;
        mau_dist->dst.top_n_idx.shape.whc.chn = 1;
        mau_dist->dst.top_n_idx.shape.whc.width = top_n_width;
        mau_dist->dst.top_n_idx.shape.whc.height = mau_dist->dst.top_n.shape.whc.height;
        mau_dist->dst.top_n_idx.type = OT_SVP_BLOB_TYPE_U32;
        mau_dist->dst.top_n_idx.stride =
            sample_common_svp_align(top_n_width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);
    }
}

static td_void sample_svp_mau_dist_set_dim_info(sample_svp_mau_dist *mau_dist,
    sample_svp_mau_dim *mau_dim)
{
    td_u32 byte_num = sizeof(td_u32);

    /* left matrix */
    mau_dist->src.left_matrix.num = 1;
    mau_dist->src.left_matrix.shape.whc.chn = 1;
    mau_dist->src.left_matrix.shape.whc.width = mau_dim->left_matrix_width;
    mau_dist->src.left_matrix.shape.whc.height = mau_dim->left_matrix_height;
    mau_dist->src.left_matrix.type = OT_SVP_BLOB_TYPE_FP32;
    mau_dist->src.left_matrix.stride =
        sample_common_svp_align(mau_dist->src.left_matrix.shape.whc.width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);

    /* right matrix */
    mau_dist->src.right_matrix.num = 1;
    mau_dist->src.right_matrix.shape.whc.chn = 1;
    mau_dist->src.right_matrix.shape.whc.width = mau_dim->right_matrix_width;
    mau_dist->src.right_matrix.shape.whc.height = mau_dim->right_matrix_height;
    mau_dist->src.right_matrix.type = OT_SVP_BLOB_TYPE_FP32;
    mau_dist->src.right_matrix.stride =
        sample_common_svp_align(mau_dist->src.right_matrix.shape.whc.width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);

    /* left matrix idx */
    if (mau_dist->ctrl.has_left_idx == TD_TRUE) {
        mau_dist->src_idx.left_matrix.num = 1;
        mau_dist->src_idx.left_matrix.shape.whc.chn = 1;
        mau_dist->src_idx.left_matrix.shape.whc.width = mau_dim->left_idx_width;
        mau_dist->src_idx.left_matrix.shape.whc.height = 1;
        mau_dist->src_idx.left_matrix.type = OT_SVP_BLOB_TYPE_U32;
        mau_dist->src_idx.left_matrix.stride =
            sample_common_svp_align(mau_dim->left_idx_width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);
    }

    /* right matrix idx */
    if (mau_dist->ctrl.has_right_idx == TD_TRUE) {
        mau_dist->src_idx.right_matrix.num = 1;
        mau_dist->src_idx.right_matrix.shape.whc.chn = 1;
        mau_dist->src_idx.right_matrix.shape.whc.width = mau_dim->right_idx_width;
        mau_dist->src_idx.right_matrix.shape.whc.height = 1;
        mau_dist->src_idx.right_matrix.type = OT_SVP_BLOB_TYPE_U32;
        mau_dist->src_idx.right_matrix.stride =
            sample_common_svp_align(mau_dim->right_idx_width * byte_num, SAMPLE_SVP_MAU_ALIGN_16);
    }

    /* dist result */
    sample_svp_mau_dist_set_result_dim_info(mau_dist, mau_dim->top_n);
}

static td_s32 sample_svp_mau_normalize_matrix(const ot_svp_blob *matrix, td_bool check_zero)
{
    td_float *matrix_data = TD_NULL;
    td_u32 i;
    td_u32 j;
    td_u32 stride;
    td_float sum;
    td_u32 size;
    td_s32 ret;

    stride = matrix->stride / sizeof(td_u32);
    matrix_data = sample_svp_convert_addr_to_ptr(td_float, matrix->virt_addr);
    for (i = 0; i < matrix->shape.whc.height; i++) {
        sum = 0.0f;
        for (j = 0; j < matrix->shape.whc.width; j++) {
            sum += matrix_data[j] * matrix_data[j];
        }
        sum = sqrt(sum);
        sum = (sum < FLT_EPSILON) ? FLT_EPSILON : sum;
        if ((check_zero == TD_TRUE) && (sum == 0.0f)) {
            return TD_FAILURE;
        }
        for (j = 0; j < matrix->shape.whc.width; j++) {
            matrix_data[j] = matrix_data[j] / sum;
        }
        matrix_data += stride;
    }
    size = matrix->stride * matrix->shape.whc.height;
    ret = sample_common_svp_flush_cache(matrix->phys_addr,
        sample_svp_convert_addr_to_ptr(td_void, matrix->virt_addr), size);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, flush cache failed!\n");

    return TD_SUCCESS;
}

static td_s32 sample_svp_mau_dist_vectorization(const ot_svp_mau_src_double_matrix *src)
{
    td_s32 ret;

    ret = sample_svp_mau_normalize_matrix(&src->left_matrix, TD_TRUE);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, left matrix vectorization failed!\n");

    ret = sample_svp_mau_normalize_matrix(&src->right_matrix, TD_FALSE);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, right matrix vectorization failed!\n");

    return TD_SUCCESS;
}
static td_s32 sample_svp_mau_dist_check_mul_info(const sample_svp_mau_dist *mau_dist)
{
    sample_svp_check_exps_return(mau_dist == TD_NULL, OT_ERR_SVP_MAU_NULL_PTR,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, matrix_mul is TD_NULL!\n");

    sample_svp_check_exps_return((mau_dist->is_record_mem_info != TD_FALSE) &&
        (mau_dist->is_record_mem_info != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),mau_dist->is_record_mem_info(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dist->is_record_mem_info, TD_FALSE, TD_TRUE);
    sample_svp_check_exps_return((mau_dist->ctrl.fp32_to_fp16_en != TD_FALSE) &&
        (mau_dist->ctrl.fp32_to_fp16_en != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),mau_dist->ctrl.fp32_to_fp16_en(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dist->ctrl.fp32_to_fp16_en, TD_FALSE, TD_TRUE);
    sample_svp_check_exps_return((mau_dist->ctrl.is_instant != TD_FALSE) &&
        (mau_dist->ctrl.is_instant != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),mau_dist->ctrl.is_instant(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dist->ctrl.is_instant, TD_FALSE, TD_TRUE);
    sample_svp_check_exps_return((mau_dist->ctrl.mau_id < OT_SVP_MAU_ID_0) ||
        (mau_dist->ctrl.mau_id >= OT_SVP_MAU_ID_BUTT), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),mau_dist->ctrl.mau_id(%d) must be [%d, %d)\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dist->ctrl.mau_id, OT_SVP_MAU_ID_0, OT_SVP_MAU_ID_BUTT);
    sample_svp_check_exps_return((mau_dist->ctrl.out_type < OT_SVP_MAU_OUT_OP_RESULT) ||
        (mau_dist->ctrl.out_type > OT_SVP_MAU_OUT_BOTH), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x), mau_dist->ctrl.out_type(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dist->ctrl.out_type, OT_SVP_MAU_OUT_OP_RESULT, OT_SVP_MAU_OUT_BOTH);
    sample_svp_check_exps_return((mau_dist->ctrl.has_left_idx != TD_FALSE) &&
        (mau_dist->ctrl.has_left_idx != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),mau_dist->ctrl.has_left_idx(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dist->ctrl.has_left_idx, TD_FALSE, TD_TRUE);
    sample_svp_check_exps_return((mau_dist->ctrl.has_right_idx != TD_FALSE) &&
        (mau_dist->ctrl.has_right_idx != TD_TRUE), OT_ERR_SVP_MAU_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),matrix_mul->ctrl.has_right_idx(%d) must be [%d, %d]\n",
        OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dist->ctrl.has_right_idx, TD_FALSE, TD_TRUE);

    return TD_SUCCESS;
}
static td_s32 sample_svp_mau_dist_check_param(const sample_svp_mau_dist *mau_dist,
    const sample_svp_mau_dim *mau_dim)
{
    td_s32 ret;

    ret = sample_svp_mau_dist_check_mul_info(mau_dist);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), sample_svp_mau_dist_check_mul_info fail\n", ret);

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

    if (mau_dist->ctrl.has_left_idx == TD_TRUE) {
        sample_svp_check_exps_return((mau_dim->left_idx_width == 0) ||
            (mau_dim->left_idx_width > SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x),mau_dim->left_idx_width(%u) must be (0, %u]\n",
            OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dim->left_idx_width, SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT);
    }
    if (mau_dist->ctrl.has_right_idx == TD_TRUE) {
        sample_svp_check_exps_return((mau_dim->right_idx_width == 0) ||
            (mau_dim->right_idx_width > SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT), OT_ERR_SVP_MAU_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x),mau_dim->right_idx_width(%u) must be (0, %u]\n",
            OT_ERR_SVP_MAU_ILLEGAL_PARAM, mau_dim->right_idx_width, SAMPLE_SVP_MAU_MATRIX_MAX_HEIGHT);
    }
    return TD_SUCCESS;
}


/* function :set the matrix dim info, malloc the memory and get matix data */
static td_s32 sample_svp_mau_dist_init(sample_svp_mau_dist *mau_dist,
    sample_svp_mau_dim *mau_dim)
{
    td_s32 ret;
    ot_svp_mem_info mem_info;

    ret = sample_svp_mau_dist_check_param(mau_dist, mau_dim);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), sample_svp_mau_dist_check_param fail\n", ret);
    /* set matrix info */
    sample_svp_mau_dist_set_dim_info(mau_dist, mau_dim);

    /* malloc   memory */
    ret = sample_svp_mau_dist_malloc_mem(mau_dist);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), sample_svp_mau_dist_malloc_mem failed!\n", ret);

    if (mau_dist->is_record_mem_info == TD_TRUE) {
        if (mau_dist->ctrl.has_left_idx == TD_TRUE) {
            mem_info.phys_addr = mau_dist->src_idx.left_matrix.phys_addr;
            mem_info.virt_addr = mau_dist->src_idx.left_matrix.virt_addr;
            mem_info.size = mau_dist->src_idx.left_matrix.stride;
            ret = ss_mpi_svp_mau_add_mem_info(&mem_info);
            sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Error(%#x), ss_mpi_svp_mau_add_mem_info failed!\n", ret);
        }
        if (mau_dist->ctrl.has_right_idx == TD_TRUE) {
            mem_info.phys_addr = mau_dist->src_idx.right_matrix.phys_addr;
            mem_info.virt_addr = mau_dist->src_idx.right_matrix.virt_addr;
            mem_info.size = mau_dist->src_idx.right_matrix.stride;
            ret = ss_mpi_svp_mau_add_mem_info(&mem_info);
            sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Error(%#x), ss_mpi_svp_mau_add_mem_info failed!\n", ret);
        }
    }

    /* get matrix data */
    ret = sample_svp_mau_generate_matrix_data(&(mau_dist->src));
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), gen rand data failed!\n", ret);
    return TD_SUCCESS;
}

static td_void sample_svp_mau_dist_deinit(sample_svp_mau_dist *mau_dist)
{
    ot_svp_mem_info mem_info;

    if (mau_dist->is_record_mem_info == TD_TRUE) {
        if (mau_dist->ctrl.has_left_idx == TD_TRUE) {
            mem_info.phys_addr = mau_dist->src_idx.left_matrix.phys_addr;
            mem_info.virt_addr = mau_dist->src_idx.left_matrix.virt_addr;
            mem_info.size = mau_dist->src_idx.left_matrix.stride;
            ss_mpi_svp_mau_rm_mem_info(&mem_info);
        }
        if (mau_dist->ctrl.has_right_idx == TD_TRUE) {
            mem_info.phys_addr = mau_dist->src_idx.right_matrix.phys_addr;
            mem_info.virt_addr = mau_dist->src_idx.right_matrix.virt_addr;
            mem_info.size = mau_dist->src_idx.right_matrix.stride;
            ss_mpi_svp_mau_rm_mem_info(&mem_info);
        }
    }
    sample_svp_mmz_free(mau_dist->src.left_matrix.phys_addr, mau_dist->src.left_matrix.virt_addr);
}

static td_void sample_svp_mau_cos_dist_stop(td_void)
{
    g_stop_cos_signal = TD_FALSE;
    sample_svp_mau_dist_deinit(&g_cos_dist);
    (td_void)memset_s(&g_cos_dist, sizeof(sample_svp_mau_dist), 0, sizeof(sample_svp_mau_dist));
    sample_common_svp_mau_mpi_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

static td_void sample_svp_mau_euclid_dist_stop(td_void)
{
    g_stop_euclid_signal = TD_FALSE;
    sample_svp_mau_dist_deinit(&g_euclid_dist);
    (td_void)memset_s(&g_euclid_dist, sizeof(sample_svp_mau_dist), 0, sizeof(sample_svp_mau_dist));
    sample_common_svp_mau_mpi_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

static td_s32 sample_svp_mau_cos_dist_proc(sample_svp_mau_dist *mau_dist)
{
    td_s32 ret;
    td_bool is_finish = TD_FALSE;
    ot_svp_mau_handle handle = 0;

    /* hardware execution */
    printf("ss_mpi_svp_mau_cos_dist start\n");
    ret = ss_mpi_svp_mau_cos_dist(&handle, &mau_dist->src,
        &mau_dist->src_idx, &mau_dist->ctrl, &mau_dist->dst);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), ss_mpi_svp_mau_cos_dist failed!\n", ret);

    ret = ss_mpi_svp_mau_query(mau_dist->ctrl.mau_id, handle, &is_finish, TD_TRUE);
    while (ret == OT_ERR_SVP_MAU_QUERY_TIMEOUT) {
        usleep(SAMPLE_SVP_MAU_QUERY_SLEEP_TIME);
        ret = ss_mpi_svp_mau_query(mau_dist->ctrl.mau_id, handle, &is_finish, TD_TRUE);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_svp_mau_query failed!\n", ret);

    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_OP_RESULT) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        ret = sample_common_svp_flush_cache(mau_dist->dst.op_result.phys_addr,
            sample_svp_convert_addr_to_ptr(td_void, mau_dist->dst.op_result.virt_addr),
            mau_dist->dst.op_result.stride * mau_dist->dst.op_result.shape.whc.height);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,flush cache failed!\n");
    }
    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_TOP_N) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        ret = sample_common_svp_flush_cache(mau_dist->dst.top_n.phys_addr,
            sample_svp_convert_addr_to_ptr(td_void, mau_dist->dst.top_n.virt_addr),
            mau_dist->dst.top_n.stride * mau_dist->dst.top_n.shape.whc.height);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,flush cache failed!\n");

        ret = sample_common_svp_flush_cache(mau_dist->dst.top_n_idx.phys_addr,
            sample_svp_convert_addr_to_ptr(td_void, mau_dist->dst.top_n_idx.virt_addr),
            mau_dist->dst.top_n_idx.stride * mau_dist->dst.top_n_idx.shape.whc.height);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,flush cache failed!\n");
    }
    return TD_SUCCESS;
}

static td_s32 sample_svp_mau_euclid_dist_proc(sample_svp_mau_dist *mau_dist)
{
    td_s32 ret;
    td_bool is_finish = TD_FALSE;
    ot_svp_mau_handle handle = 0;

    /* hardware   execution */
    printf("ss_mpi_svp_mau_euclid_dist start\n");
    ret = ss_mpi_svp_mau_euclid_dist(&handle, &mau_dist->src,
        &mau_dist->src_idx, &mau_dist->ctrl, &mau_dist->dst);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x), ss_mpi_svp_mau_euclid_dist failed!\n", ret);

    ret = ss_mpi_svp_mau_query(mau_dist->ctrl.mau_id, handle, &is_finish, TD_TRUE);
    while (ret == OT_ERR_SVP_MAU_QUERY_TIMEOUT) {
        usleep(SAMPLE_SVP_MAU_QUERY_SLEEP_TIME);
        ret = ss_mpi_svp_mau_query(mau_dist->ctrl.mau_id, handle, &is_finish, TD_TRUE);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_svp_mau_query failed!\n", ret);

    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_OP_RESULT) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        ret = sample_common_svp_flush_cache(mau_dist->dst.op_result.phys_addr,
            sample_svp_convert_addr_to_ptr(td_void, mau_dist->dst.op_result.virt_addr),
            mau_dist->dst.op_result.stride * mau_dist->dst.op_result.shape.whc.height);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,flush cache failed!\n");
    }
    if ((mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_TOP_N) ||
        (mau_dist->ctrl.out_type == OT_SVP_MAU_OUT_BOTH)) {
        ret = sample_common_svp_flush_cache(mau_dist->dst.top_n.phys_addr,
            sample_svp_convert_addr_to_ptr(td_void, mau_dist->dst.top_n.virt_addr),
            mau_dist->dst.top_n.stride * mau_dist->dst.top_n.shape.whc.height);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,flush cache failed!\n");

        ret = sample_common_svp_flush_cache(mau_dist->dst.top_n_idx.phys_addr,
            sample_svp_convert_addr_to_ptr(td_void, mau_dist->dst.top_n_idx.virt_addr),
            mau_dist->dst.top_n_idx.stride * mau_dist->dst.top_n_idx.shape.whc.height);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,flush cache failed!\n");
    }

    return TD_SUCCESS;
}

static td_void sample_svp_mau_dist_print_dist_result(const ot_svp_mau_dist_result *dst,
    const ot_svp_mau_ctrl *ctrl)
{
    td_float *matrix_data = TD_NULL;
    td_u32 *idx_data = TD_NULL;
    td_u32 i;
    td_u32 j;
    td_u32 stride;

    /* op result */
    if ((ctrl->out_type == OT_SVP_MAU_OUT_OP_RESULT) || (ctrl->out_type == OT_SVP_MAU_OUT_BOTH)) {
        printf("==== The data of op_result matrix====\n");
        stride = dst->op_result.stride / sizeof(td_u32);
        matrix_data = sample_svp_convert_addr_to_ptr(td_float, dst->op_result.virt_addr);
        for (i = 0; i < dst->op_result.shape.whc.height; i++) {
            for (j = 0; j < dst->op_result.shape.whc.width; j++) {
                printf("%f ", matrix_data[j]);
            }
            printf("\n");
            matrix_data += stride;
        }
    }

    /* top_n_result */
    if ((ctrl->out_type == OT_SVP_MAU_OUT_TOP_N) || (ctrl->out_type == OT_SVP_MAU_OUT_BOTH)) {
        /* top_n */
        printf("==== The data of top_n====\n");
        stride = dst->top_n.stride / sizeof(td_float);
        matrix_data = sample_svp_convert_addr_to_ptr(td_float, dst->top_n.virt_addr);
        for (i = 0; i < dst->top_n.shape.whc.height; i++) {
            for (j = 0; j < dst->top_n.shape.whc.width; j++) {
                printf("%f ", matrix_data[j]);
            }
            printf("\n");
            matrix_data += stride;
        }

        /* top_n_idx */
        printf("==== The data of top_n_idx====\n");
        stride = dst->top_n_idx.stride / sizeof(td_u32);
        idx_data = sample_svp_convert_addr_to_ptr(td_u32, dst->top_n_idx.virt_addr);
        for (i = 0; i < dst->top_n_idx.shape.whc.height; i++) {
            for (j = 0; j < dst->top_n_idx.shape.whc.width; j++) {
                printf("%8u", idx_data[j]);
            }
            printf("\n");
            idx_data += stride;
        }
    }
}

static td_void sample_svp_mau_dist_print_result(const sample_svp_mau_dist *mau_dist)
{
    td_float *matrix_data = TD_NULL;
    td_u32 *idx_data = TD_NULL;
    td_u32 i;
    td_u32 j;
    td_u32 stride;

    /* left matrix */
    printf("==== The data of left matrix====\n");
    stride = mau_dist->src.left_matrix.stride / sizeof(td_u32);
    matrix_data = sample_svp_convert_addr_to_ptr(td_float, mau_dist->src.left_matrix.virt_addr);
    for (i = 0; i < mau_dist->src.left_matrix.shape.whc.height; i++) {
        for (j = 0; j < mau_dist->src.left_matrix.shape.whc.width; j++) {
            printf("%f ", matrix_data[j]);
        }
        printf("\n");
        matrix_data += stride;
    }

    /* right matrix */
    printf("==== The data of right matrix====\n");
    stride = mau_dist->src.right_matrix.stride / sizeof(td_u32);
    matrix_data = sample_svp_convert_addr_to_ptr(td_float, mau_dist->src.right_matrix.virt_addr);
    for (i = 0; i < mau_dist->src.right_matrix.shape.whc.height; i++) {
        for (j = 0; j < mau_dist->src.right_matrix.shape.whc.width; j++) {
            printf("%f ", matrix_data[j]);
        }
        printf("\n");
        matrix_data += stride;
    }

    /* left idx */
    if (mau_dist->ctrl.has_left_idx == TD_TRUE) {
        printf("==== The data of left_idx====\n");
        idx_data = sample_svp_convert_addr_to_ptr(td_u32, mau_dist->src_idx.left_matrix.virt_addr);
        for (j = 0; j < mau_dist->src_idx.left_matrix.shape.whc.width; j++) {
            printf("%8u", idx_data[j]);
        }
        printf("\n");
    }

    /* right idx */
    if (mau_dist->ctrl.has_right_idx == TD_TRUE) {
        printf("==== The data of right_idx====\n");
        idx_data = sample_svp_convert_addr_to_ptr(td_u32, mau_dist->src_idx.right_matrix.virt_addr);
        for (j = 0; j < mau_dist->src_idx.right_matrix.shape.whc.width; j++) {
            printf("%8u", idx_data[j]);
        }
        printf("\n");
    }

    /* dist result */
    sample_svp_mau_dist_print_dist_result(&(mau_dist->dst), &(mau_dist->ctrl));
}

static td_void sample_svp_mau_cos_dist_set_info(td_char idx_flag, sample_svp_mau_dim *mau_dim)
{
    /* set ctrl info */
    g_cos_dist.is_record_mem_info = TD_TRUE;
    g_cos_dist.ctrl.out_type = OT_SVP_MAU_OUT_OP_RESULT;
    g_cos_dist.ctrl.fp32_to_fp16_en = TD_FALSE;
    g_cos_dist.ctrl.is_instant = TD_TRUE;
    g_cos_dist.ctrl.mau_id = OT_SVP_MAU_ID_0;
    g_cos_dist.ctrl.has_left_idx = TD_FALSE;
    g_cos_dist.ctrl.has_right_idx = TD_FALSE;

    /* the dim of left_marix, right_matrix,left_idx_width, right_idx_width */
    mau_dim->left_matrix_height =    SAMPLE_SVP_MAU_LEFT_MATRIX_HEIGHT;
    mau_dim->left_matrix_width =     SAMPLE_SVP_MAU_LEFT_MATRIX_WIDTH;
    mau_dim->right_matrix_height =   SAMPLE_SVP_MAU_RIGHT_MATRIX_HEIGHT;
    mau_dim->right_matrix_width =    SAMPLE_SVP_MAU_RIGHT_MATRIX_WIDTH;
    mau_dim->left_idx_width =        SAMPLE_SVP_MAU_MATRIX_IDX_WIDTH;
    mau_dim->right_idx_width =       SAMPLE_SVP_MAU_MATRIX_IDX_WIDTH;
    mau_dim->top_n =                 SAMPLE_SVP_MAU_DIST_TOP_N;

    if (idx_flag == '0') { /* '0':left_idx */
        g_cos_dist.ctrl.has_left_idx = TD_TRUE;
    } else if (idx_flag == '1') { /* '1':right_idx */
        g_cos_dist.ctrl.has_right_idx = TD_TRUE;
    } else { /* left_idx and right_idx */
        g_cos_dist.ctrl.has_left_idx = TD_TRUE;
        g_cos_dist.ctrl.has_right_idx = TD_TRUE;
    }
    return;
}
/*
 * function : show the sample of mau cosine distance
 */
td_void sample_svp_mau_cos_dist(td_char idx_flag)
{
    td_s32 ret;
    sample_svp_mau_dim mau_dim;

    if ((idx_flag != '0') && (idx_flag  != '1') && (idx_flag != '2')) {
        return;
    }

    ret = sample_common_svp_mau_check_mau_mpi_init();
    sample_svp_check_exps_return_void(ret != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "mau_check_mau_mpi_init failed!\n");
    sample_svp_mau_cos_dist_set_info(idx_flag, &mau_dim);

    /* init */
    ret = sample_svp_mau_dist_init(&g_cos_dist, &mau_dim);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_dist_init failed!\n", ret);

    if (g_stop_cos_signal == TD_TRUE) {
        sample_svp_mau_cos_dist_stop();
        return;
    }
    /* set idx */
    ret = sample_svp_mau_set_idx_info(&(g_cos_dist.src_idx), &(g_cos_dist.ctrl), &(g_cos_dist.src));
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_set_idx_info failed!\n", ret);

    if (g_stop_cos_signal == TD_TRUE) {
        sample_svp_mau_cos_dist_stop();
        return;
    }
    /* normalize the data of src_matrix */
    ret = sample_svp_mau_dist_vectorization(&(g_cos_dist.src));
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_dist_data_normalization failed!\n", ret);

    if (g_stop_cos_signal == TD_TRUE) {
        sample_svp_mau_cos_dist_stop();
        return;
    }
    /* the process of mau cosine distance */
    ret = sample_svp_mau_cos_dist_proc(&g_cos_dist);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_cos_dist_process failed!\n", ret);
    if (g_stop_cos_signal == TD_TRUE) {
        sample_svp_mau_cos_dist_stop();
        return;
    }
    /* print the result */
    sample_svp_mau_dist_print_result(&g_cos_dist);

fail:
    /* deinit */
    sample_svp_mau_dist_deinit(&g_cos_dist);
    sample_common_svp_mau_mpi_exit();
}

static td_void sample_svp_mau_euclid_dist_set_info(td_char out_type_flag, sample_svp_mau_dim *mau_dim)
{
    /* set ctrl info */
    g_euclid_dist.is_record_mem_info = TD_FALSE;
    g_euclid_dist.ctrl.fp32_to_fp16_en = TD_FALSE;
    g_euclid_dist.ctrl.is_instant = TD_TRUE;
    g_euclid_dist.ctrl.mau_id = OT_SVP_MAU_ID_0;
    g_euclid_dist.ctrl.has_left_idx = TD_FALSE;
    g_euclid_dist.ctrl.has_right_idx = TD_FALSE;

    /* the dim of left_marix, right_matrix,left_idx_width, right_idx_width */
    mau_dim->left_matrix_height =    SAMPLE_SVP_MAU_LEFT_MATRIX_HEIGHT;
    mau_dim->left_matrix_width =     SAMPLE_SVP_MAU_LEFT_MATRIX_WIDTH;
    mau_dim->right_matrix_height =   SAMPLE_SVP_MAU_RIGHT_MATRIX_HEIGHT;
    mau_dim->right_matrix_width =    SAMPLE_SVP_MAU_RIGHT_MATRIX_WIDTH;
    mau_dim->left_idx_width =        SAMPLE_SVP_MAU_MATRIX_IDX_WIDTH;
    mau_dim->right_idx_width =       SAMPLE_SVP_MAU_MATRIX_IDX_WIDTH;
    mau_dim->top_n =                 SAMPLE_SVP_MAU_DIST_TOP_N;

    if (out_type_flag == '0') { /* '0':OT_SVP_MAU_OUT_OP_RESULT */
        g_euclid_dist.ctrl.out_type = OT_SVP_MAU_OUT_OP_RESULT;
    } else if (out_type_flag == '1') { /* '1':OT_SVP_MAU_OUT_TOP_N */
        g_euclid_dist.ctrl.out_type = OT_SVP_MAU_OUT_TOP_N;
    } else { /* '2':OT_SVP_MAU_OUT_BOTH */
        g_euclid_dist.ctrl.out_type = OT_SVP_MAU_OUT_BOTH;
    }
    return;
}
/*
 * function : show the sample of mau euclidean distance
 */
td_void sample_svp_mau_euclid_dist(td_char out_type_flag)
{
    td_s32 ret;
    sample_svp_mau_dim mau_dim;

    if ((out_type_flag != '0') && (out_type_flag  != '1') && (out_type_flag != '2')) {
        return;
    }
    ret = sample_common_svp_mau_check_mau_mpi_init();
    sample_svp_check_exps_return_void(ret != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "mau_check_mau_mpi_init failed!\n");
    sample_svp_mau_euclid_dist_set_info(out_type_flag, &mau_dim);

    /* init */
    ret = sample_svp_mau_dist_init(&g_euclid_dist, &mau_dim);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_dist_init failed!\n", ret);
    if (g_stop_euclid_signal == TD_TRUE) {
        sample_svp_mau_euclid_dist_stop();
        return;
    }
    /* set idx */
    ret = sample_svp_mau_set_idx_info(&(g_euclid_dist.src_idx), &(g_euclid_dist.ctrl), &(g_euclid_dist.src));
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_set_idx_info failed!\n", ret);
    if (g_stop_euclid_signal == TD_TRUE) {
        sample_svp_mau_euclid_dist_stop();
        return;
    }
    /* the process of mau euclidean distance */
    ret = sample_svp_mau_euclid_dist_proc(&g_euclid_dist);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_svp_mau_euclid_dist_process failed!\n", ret);
    if (g_stop_euclid_signal == TD_TRUE) {
        sample_svp_mau_euclid_dist_stop();
        return;
    }
    /* print the result */
    sample_svp_mau_dist_print_result(&g_euclid_dist);

fail:
    /* deinit */
    sample_svp_mau_dist_deinit(&g_euclid_dist);
    sample_common_svp_mau_mpi_exit();
}

/*
 * function : mau_cos_dist sample signal handle
 */
td_void sample_svp_mau_cos_dist_handle_sig(td_void)
{
    g_stop_cos_signal = TD_TRUE;
}

/*
 * function : mau_euclid_dist sample signal handle
 */
td_void sample_svp_mau_euclid_dist_handle_sig(td_void)
{
    g_stop_euclid_signal = TD_TRUE;
}
