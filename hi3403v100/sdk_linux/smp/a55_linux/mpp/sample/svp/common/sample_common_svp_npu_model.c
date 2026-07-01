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

#include "sample_common_svp_npu_model.h"
#include <limits.h>
#include "svp_acl_rt.h"
#include "svp_acl_ext.h"
#include "sample_common_svp.h"
#include <math.h>

#define SAMPLE_SVP_NPU_H_DIM_IDX 2
#define SAMPLE_SVP_NPU_W_DIM_IDX 3
#define SAMPLE_SVP_NPU_DIM_NUM 4
static sample_svp_npu_model_info g_svp_npu_model[SAMPLE_SVP_NPU_MAX_MODEL_NUM] = {0};

static td_s32 sample_svp_npu_get_model_base_info(td_u32 model_index)
{
    svp_acl_error ret;

    g_svp_npu_model[model_index].input_num = svp_acl_mdl_get_num_inputs(g_svp_npu_model[model_index].model_desc);
    sample_svp_check_exps_return(g_svp_npu_model[model_index].input_num < SAMPLE_SVP_NPU_EXTRA_INPUT_NUM + 1,
        TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "get input num failed!\n");

    g_svp_npu_model[model_index].output_num = svp_acl_mdl_get_num_outputs(g_svp_npu_model[model_index].model_desc);
    sample_svp_check_exps_return(g_svp_npu_model[model_index].output_num < 1,
        TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "get output num failed!\n");

    ret = svp_acl_mdl_get_input_index_by_name(g_svp_npu_model[model_index].model_desc,
        SVP_ACL_DYNAMIC_TENSOR_NAME, &g_svp_npu_model[model_index].dynamic_batch_idx);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get dynamic batch idx failed, model id is %u, error code is %d!\n", model_index, ret);

    return TD_SUCCESS;
}

static td_s32 sample_svp_npu_get_line_num_and_line_byte_num(const sample_svp_npu_task_info *task, td_u32 idx,
    td_bool is_input, td_u32 *total_line_num, td_u32 *line_byte_num)
{
    td_s32 ret;
    td_u32 i;
    svp_acl_mdl_io_dims dims;
    svp_acl_data_type data_type;
    size_t data_size;

    if (is_input == TD_TRUE) {
        ret = svp_acl_mdl_get_input_dims(g_svp_npu_model[task->cfg.model_idx].model_desc, idx, &dims);
    } else {
        ret = svp_acl_mdl_get_output_dims(g_svp_npu_model[task->cfg.model_idx].model_desc, idx, &dims);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get %u-th input/output dims failed!\n", idx);

    if (is_input == TD_TRUE) {
        data_type = svp_acl_mdl_get_input_data_type(g_svp_npu_model[task->cfg.model_idx].model_desc, idx);
    } else {
        data_type = svp_acl_mdl_get_output_data_type(g_svp_npu_model[task->cfg.model_idx].model_desc, idx);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get %u-th input/output data type failed!\n", idx);

    data_size = svp_acl_data_type_size(data_type);
    sample_svp_check_exps_return(data_size == 0, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "get data size failed!\n");

    *line_byte_num = dims.dims[dims.dim_count - 1] *
        ((data_size + SAMPLE_SVP_NPU_BYTE_BIT_NUM - 1) / SAMPLE_SVP_NPU_BYTE_BIT_NUM);

    *total_line_num = 1;
    for (i = 0; i < dims.dim_count - 1; i++) {
        *total_line_num *= dims.dims[i];
    }
    /* lstm xt line num */
    if ((task->cfg.total_t != 0) && (idx == 0)) {
        sample_svp_check_exps_return(task->cfg.total_t > dims.dims[0], TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "total t(%u) can't be greater than max total t(%ld)!\n", task->cfg.total_t, dims.dims[0]);
        *total_line_num /= dims.dims[0];
        *total_line_num *= task->cfg.total_t;
    }
    return TD_SUCCESS;
}

static td_s32 sample_svp_check_task_cfg(const sample_svp_npu_task_info *task)
{
    sample_svp_check_exps_return(task == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "task is NULL!\n");

    sample_svp_check_exps_return(task->cfg.max_batch_num == 0, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "max_batch_num(%u) is 0!\n", task->cfg.max_batch_num);

    sample_svp_check_exps_return(task->cfg.dynamic_batch_num == 0, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "dynamic_batch_num(%u) is 0!\n", task->cfg.dynamic_batch_num);

    sample_svp_check_exps_return(task->cfg.total_t != 0 && task->cfg.dynamic_batch_num != 1, TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "dynamic_batch_num(%u) should be 1 when total_t(%u) is not 0!\n",
        task->cfg.dynamic_batch_num, task->cfg.total_t);

    sample_svp_check_exps_return((task->cfg.is_cached != TD_TRUE && task->cfg.is_cached != TD_FALSE), TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "is_cached(%u) should be [%u, %u]!\n", task->cfg.is_cached, TD_FALSE, TD_TRUE);

    sample_svp_check_exps_return(task->cfg.model_idx >= SAMPLE_SVP_NPU_MAX_MODEL_NUM, TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "model_idx(%u) should be less than %u!\n",
        task->cfg.model_idx, SAMPLE_SVP_NPU_MAX_MODEL_NUM);

    sample_svp_check_exps_return(g_svp_npu_model[task->cfg.model_idx].model_desc == TD_NULL, TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "%u-th model_desc is NULL!\n", task->cfg.model_idx);
    return TD_SUCCESS;
}

static td_s32 sample_svp_npu_load_one_input(const td_char *src, const sample_svp_npu_task_info *task, td_u32 index)
{
    td_s32 ret;
    td_u32 line;
    td_u32 total_line_num;
    td_u32 line_byte_num;
    td_char path[PATH_MAX] = { 0 };
    size_t stride;
    size_t size;
    FILE *fp = TD_NULL;
    td_void *data = TD_NULL;
    svp_acl_data_buffer *data_buffer = TD_NULL;

    sample_svp_check_exps_return(realpath(src, path) == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Invalid file!\n");

    fp = fopen(path, "rb");
    sample_svp_check_exps_return(fp == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "open file failed!\n");

    ret = sample_svp_npu_get_line_num_and_line_byte_num(task, index, TD_TRUE, &total_line_num, &line_byte_num);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end, SAMPLE_SVP_ERR_LEVEL_ERROR, "get line num failed!\n");

    data_buffer = svp_acl_mdl_get_dataset_buffer(task->input_dataset, index);
    sample_svp_check_exps_goto(data_buffer == TD_NULL, end, SAMPLE_SVP_ERR_LEVEL_ERROR, "get data buffer NULL!\n");

    data = svp_acl_get_data_buffer_addr(data_buffer);
    sample_svp_check_exps_goto(data == TD_NULL, end, SAMPLE_SVP_ERR_LEVEL_ERROR, "get data addr NULL!\n");

    stride = svp_acl_get_data_buffer_stride(data_buffer);
    sample_svp_check_exps_goto(stride == 0, end, SAMPLE_SVP_ERR_LEVEL_ERROR, "get data stride failed!\n");

    size = svp_acl_get_data_buffer_size(data_buffer);
    sample_svp_check_exps_goto(size < (td_u64)task->cfg.dynamic_batch_num * total_line_num * stride, end,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "%u-th data buffer size(%lu) is less than needed(%llu)!\n",
        index, size, (td_u64)task->cfg.dynamic_batch_num * total_line_num * stride);

    for (line = 0; line < task->cfg.dynamic_batch_num * total_line_num; line++) {
        ret = fread(data + line * stride, line_byte_num, 1, fp);
        sample_svp_check_exps_goto(ret != 1, end, SAMPLE_SVP_ERR_LEVEL_ERROR, "Read file failed!\n");
    }
    if (task->cfg.is_cached == TD_TRUE) {
        (td_void)svp_acl_rt_mem_flush(data, task->cfg.dynamic_batch_num * total_line_num * stride);
    }
    (td_void)fclose(fp);
    return TD_SUCCESS;

end:
    if (fp != TD_NULL) {
        (td_void)fclose(fp);
    }
    return TD_FAILURE;
}

td_s32 sample_common_svp_npu_get_input_data(const td_char *src[], td_u32 file_num,
    const sample_svp_npu_task_info *task)
{
    td_u32 i;
    size_t input_num;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }
    sample_svp_check_exps_return(src == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "src is NULL!\n");

    input_num = g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM;
    sample_svp_check_exps_return(input_num != file_num, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "input file num(%u) should be equal to %lu!\n", file_num, input_num);

    for (i = 0; i < file_num; i++) {
        sample_svp_check_exps_return(sample_svp_npu_load_one_input(src[i], task, i) != TD_SUCCESS, TD_FAILURE,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "load input file failed!\n");
    }
    return TD_SUCCESS;
}

static td_s32 sample_svp_npu_read_model(const td_char *model_path, td_u32 model_index, td_bool is_cached)
{
    FILE *fp = TD_NULL;
    td_s32 ret;

    /* Get model file size */
    fp = fopen(model_path, "rb");
    sample_svp_check_exps_return(fp == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "open model file failed, model file is %s!\n", model_path);

    ret = fseek(fp, 0L, SEEK_END);
    sample_svp_check_exps_goto(ret == -1, end_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "fseek failed!\n");

    g_svp_npu_model[model_index].model_mem_size = ftell(fp);
    sample_svp_check_exps_goto(g_svp_npu_model[model_index].model_mem_size <= 0, end_0,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "ftell failed!\n");

    ret = fseek(fp, 0L, SEEK_SET);
    sample_svp_check_exps_goto(ret == -1, end_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "fseek failed!\n");

    /* malloc model file mem */
    if (is_cached == TD_TRUE) {
        ret = svp_acl_rt_malloc_cached(&g_svp_npu_model[model_index].model_mem_ptr,
            g_svp_npu_model[model_index].model_mem_size, SVP_ACL_MEM_MALLOC_NORMAL_ONLY);
    } else {
        ret = svp_acl_rt_malloc(&g_svp_npu_model[model_index].model_mem_ptr,
            g_svp_npu_model[model_index].model_mem_size, SVP_ACL_MEM_MALLOC_NORMAL_ONLY);
    }
    sample_svp_check_exps_goto(ret != SVP_ACL_SUCCESS, end_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "malloc mem failed, erroe code %d!\n", ret);

    ret = fread(g_svp_npu_model[model_index].model_mem_ptr, g_svp_npu_model[model_index].model_mem_size, 1, fp);
    sample_svp_check_exps_goto(ret != 1, end_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "read model file failed!\n");

    if (is_cached == TD_TRUE) {
        ret = svp_acl_rt_mem_flush(g_svp_npu_model[model_index].model_mem_ptr,
            g_svp_npu_model[model_index].model_mem_size);
        sample_svp_check_exps_goto(ret != SVP_ACL_SUCCESS, end_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "flush mem failed!, error code is %d\n", ret);
    }
    (td_void)fclose(fp);
    return TD_SUCCESS;
end_1:
    (td_void)svp_acl_rt_free(g_svp_npu_model[model_index].model_mem_ptr);
end_0:
    (td_void)fclose(fp);
    return TD_FAILURE;
}

static td_s32 sample_svp_npu_create_desc(td_u32 model_index)
{
    svp_acl_error ret;

    g_svp_npu_model[model_index].model_desc = svp_acl_mdl_create_desc();
    sample_svp_check_exps_return(g_svp_npu_model[model_index].model_desc == TD_NULL, TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "create model description failed!\n");

    ret = svp_acl_mdl_get_desc(g_svp_npu_model[model_index].model_desc, g_svp_npu_model[model_index].model_id);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "get model description failed, error code is %d!\n", ret);

    sample_svp_trace_info("create model description success!\n");

    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_load_model(const td_char *model_path, td_u32 model_index, td_bool is_cached)
{
    td_s32 ret;

    sample_svp_check_exps_return(g_svp_npu_model[model_index].is_load_flag == TD_TRUE, TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "%u-th node has already loaded a model!\n", model_index);

    ret = sample_svp_npu_read_model(model_path, model_index, is_cached);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "query model failed, model file is %s!\n", model_path);

    ret = svp_acl_mdl_load_from_mem(g_svp_npu_model[model_index].model_mem_ptr,
        g_svp_npu_model[model_index].model_mem_size, &g_svp_npu_model[model_index].model_id);
    sample_svp_check_exps_goto(ret != SVP_ACL_SUCCESS, end_0,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "load model from mem failed, error code is %d!\n", ret);

    ret = sample_svp_npu_create_desc(model_index);
    sample_svp_check_exps_goto(ret != SVP_ACL_SUCCESS, end_0,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "create desc failed, model file is %s!\n", model_path);

    ret = sample_svp_npu_get_model_base_info(model_index);
    sample_svp_check_exps_goto(ret != SVP_ACL_SUCCESS, end_1,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "get model base info failed, model file is %s!\n", model_path);

    sample_svp_trace_info("load mem_size:%lu, id:%d!\n", g_svp_npu_model[model_index].model_mem_size,
        g_svp_npu_model[model_index].model_id);

    g_svp_npu_model[model_index].is_load_flag = TD_TRUE;
    sample_svp_trace_info("load model %s success!\n", model_path);

    return TD_SUCCESS;
end_1:
    (td_void)svp_acl_mdl_destroy_desc(g_svp_npu_model[model_index].model_desc);
    g_svp_npu_model[model_index].model_desc = TD_NULL;
end_0:
    (td_void)svp_acl_rt_free(g_svp_npu_model[model_index].model_mem_ptr);
    g_svp_npu_model[model_index].model_mem_ptr = TD_NULL;
    g_svp_npu_model[model_index].model_mem_size = 0;
    return TD_FAILURE;
}

td_void sample_common_svp_npu_unload_model(td_u32 model_index)
{
    svp_acl_error ret;

    sample_svp_check_exps_return_void(g_svp_npu_model[model_index].is_load_flag != TD_TRUE,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "%u-th node has not loaded a model!\n", model_index);

    ret = svp_acl_mdl_unload(g_svp_npu_model[model_index].model_id);
    if (ret != SVP_ACL_SUCCESS) {
        sample_svp_trace_err("unload model failed, model_id is %u, error code is %d!\n",
            g_svp_npu_model[model_index].model_id, ret);
    }

    if (g_svp_npu_model[model_index].model_desc != TD_NULL) {
        (td_void)svp_acl_mdl_destroy_desc(g_svp_npu_model[model_index].model_desc);
        g_svp_npu_model[model_index].model_desc = TD_NULL;
    }

    if (g_svp_npu_model[model_index].model_mem_ptr != TD_NULL) {
        (td_void)svp_acl_rt_free(g_svp_npu_model[model_index].model_mem_ptr);
        g_svp_npu_model[model_index].model_mem_ptr = TD_NULL;
        g_svp_npu_model[model_index].model_mem_size = 0;
    }

    g_svp_npu_model[model_index].is_load_flag = TD_FALSE;
    sample_svp_trace_info("unload model SUCCESS, model id is %u!\n", g_svp_npu_model[model_index].model_id);
}

static td_s32 sample_svp_npu_malloc_mem(td_void **buffer, td_u32 buffer_size, td_bool is_cached)
{
    svp_acl_error ret;

    if (is_cached == TD_TRUE) {
        ret = svp_acl_rt_malloc_cached(buffer, buffer_size, SVP_ACL_MEM_MALLOC_NORMAL_ONLY);
    } else {
        ret = svp_acl_rt_malloc(buffer, buffer_size, SVP_ACL_MEM_MALLOC_NORMAL_ONLY);
    }
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "can't malloc buffer, size is %u, error code is %d!\n", buffer_size, ret);

    (td_void)memset_s(*buffer, buffer_size, 0, buffer_size);
    if (is_cached == TD_TRUE) {
        (td_void)svp_acl_rt_mem_flush(*buffer, buffer_size);
    }
    return ret;
}

static td_void sample_svp_npu_destroy_data_buffer(svp_acl_data_buffer *input_data)
{
    td_void *data = svp_acl_get_data_buffer_addr(input_data);
    (td_void)svp_acl_rt_free(data);
    (td_void)svp_acl_destroy_data_buffer(input_data);
}

static svp_acl_data_buffer *sample_common_svp_npu_create_input_data_buffer(sample_svp_npu_task_info *task, td_u32 idx)
{
    size_t buffer_size;
    size_t stride;
    td_void *input_buffer = TD_NULL;
    svp_acl_data_buffer *input_data = TD_NULL;

    stride = svp_acl_mdl_get_input_default_stride(g_svp_npu_model[task->cfg.model_idx].model_desc, idx);
    sample_svp_check_exps_return(stride == 0, input_data, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get %u-th input stride failed!\n", idx);

    buffer_size = svp_acl_mdl_get_input_size_by_index(g_svp_npu_model[task->cfg.model_idx].model_desc, idx);
    if (idx < g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM) {
        buffer_size *= task->cfg.max_batch_num;
    }
    sample_svp_check_exps_return((buffer_size == 0 || buffer_size > SAMPLE_SVP_NPU_MAX_MEM_SIZE), input_data,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "buffer_size(%lu) can't be 0 and should be less than %u!\n",
        buffer_size, SAMPLE_SVP_NPU_MAX_MEM_SIZE);

    if (sample_svp_npu_malloc_mem(&input_buffer, (td_u32)buffer_size, task->cfg.is_cached) != TD_SUCCESS) {
        sample_svp_trace_err("%u-th input malloc mem failed!\n", idx);
        return input_data;
    }

    input_data = svp_acl_create_data_buffer(input_buffer, buffer_size, stride);
    if (input_data == TD_NULL) {
        sample_svp_trace_err("can't create %u-th input data buffer!\n", idx);
        (td_void)svp_acl_rt_free(input_buffer);
        return input_data;
    }
    if (idx == g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM) {
        task->task_buf_ptr = input_buffer;
        task->task_buf_size = buffer_size;
        task->task_buf_stride = stride;
    } else if (idx == g_svp_npu_model[task->cfg.model_idx].input_num - 1) {
        task->work_buf_ptr = input_buffer;
        task->work_buf_size = buffer_size;
        task->work_buf_stride = stride;
    }
    return input_data;
}

static svp_acl_data_buffer *sample_common_svp_npu_create_output_data_buffer(const sample_svp_npu_task_info *task,
    td_u32 idx)
{
    size_t buffer_size;
    size_t stride;
    td_void *output_buffer = TD_NULL;
    svp_acl_data_buffer *output_data = TD_NULL;

    stride = svp_acl_mdl_get_output_default_stride(g_svp_npu_model[task->cfg.model_idx].model_desc, idx);
    sample_svp_check_exps_return(stride == 0, output_data, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get %u-th output stride failed!\n", idx);

    buffer_size = svp_acl_mdl_get_output_size_by_index(g_svp_npu_model[task->cfg.model_idx].model_desc, idx) *
        (td_u64)task->cfg.max_batch_num;
    sample_svp_check_exps_return((buffer_size == 0 || buffer_size > SAMPLE_SVP_NPU_MAX_MEM_SIZE), output_data,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "buffer_size(%lu) can't be 0 and should be less than %u!\n",
        buffer_size, SAMPLE_SVP_NPU_MAX_MEM_SIZE);

    if (sample_svp_npu_malloc_mem(&output_buffer, buffer_size, task->cfg.is_cached) != TD_SUCCESS) {
        sample_svp_trace_err("%u-th output malloc mem failed!\n", idx);
        return output_data;
    }

    output_data = svp_acl_create_data_buffer(output_buffer, buffer_size, stride);
    if (output_data == TD_NULL) {
        sample_svp_trace_err("can't create %u-th output data buffer!\n", idx);
        (td_void)svp_acl_rt_free(output_buffer);
        return output_data;
    }
    return output_data;
}

td_s32 sample_common_svp_npu_create_input(sample_svp_npu_task_info *task)
{
    svp_acl_error ret;
    td_u32 i;
    svp_acl_data_buffer *input_data = TD_NULL;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }

    task->input_dataset = svp_acl_mdl_create_dataset();
    sample_svp_check_exps_return(task->input_dataset == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "create input dataset failed!\n");

    for (i = 0; i < g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM; i++) {
        input_data = sample_common_svp_npu_create_input_data_buffer(task, i);
        if (input_data == TD_NULL) {
            sample_svp_trace_err("create %u-th input data buffer failed!\n", i);
            (td_void)sample_common_svp_npu_destroy_input(task);
            return TD_FAILURE;
        }

        ret = svp_acl_mdl_add_dataset_buffer(task->input_dataset, input_data);
        if (ret != SVP_ACL_SUCCESS) {
            sample_svp_trace_err("add %u-th input data buffer failed!\n", i);
            (td_void)sample_svp_npu_destroy_data_buffer(input_data);
            (td_void)sample_common_svp_npu_destroy_input(task);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_create_output(sample_svp_npu_task_info *task)
{
    svp_acl_error ret;
    td_u32 i;
    svp_acl_data_buffer *output_data = TD_NULL;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }

    task->output_dataset = svp_acl_mdl_create_dataset();
    sample_svp_check_exps_return(task->input_dataset == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "create output dataset failed!\n");

    for (i = 0; i < g_svp_npu_model[task->cfg.model_idx].output_num; i++) {
        output_data = sample_common_svp_npu_create_output_data_buffer(task, i);
        if (output_data == TD_NULL) {
            sample_svp_trace_err("create %u-th output data buffer failed!\n", i);
            (td_void)sample_common_svp_npu_destroy_input(task);
            return TD_FAILURE;
        }
        ret = svp_acl_mdl_add_dataset_buffer(task->output_dataset, output_data);
        if (ret != SVP_ACL_SUCCESS) {
            sample_svp_trace_err("add %u-th output data buffer failed!\n", i);
            (td_void)sample_svp_npu_destroy_data_buffer(output_data);
            (td_void)sample_common_svp_npu_destroy_output(task);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

td_void sample_common_svp_npu_destroy_input(sample_svp_npu_task_info *task)
{
    td_u32 i;
    size_t input_num;
    svp_acl_data_buffer *data_buffer = TD_NULL;
    td_void *data = TD_NULL;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return;
    }

    if (task->input_dataset == TD_NULL) {
        return;
    }

    input_num = svp_acl_mdl_get_dataset_num_buffers(task->input_dataset);
    for (i = 0; i < input_num; i++) {
        data_buffer = svp_acl_mdl_get_dataset_buffer(task->input_dataset, i);
        if (i < g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM) {
            data = svp_acl_get_data_buffer_addr(data_buffer);
            (td_void)svp_acl_rt_free(data);
        }
        (td_void)svp_acl_destroy_data_buffer(data_buffer);
    }
    (td_void)svp_acl_mdl_destroy_dataset(task->input_dataset);
    task->input_dataset = TD_NULL;
}

td_void sample_common_svp_npu_destroy_output(sample_svp_npu_task_info *task)
{
    td_u32 i;
    size_t output_num;
    svp_acl_data_buffer *data_buffer = TD_NULL;
    td_void *data = TD_NULL;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return;
    }

    if (task->output_dataset == TD_NULL) {
        return;
    }

    output_num = svp_acl_mdl_get_dataset_num_buffers(task->output_dataset);

    for (i = 0; i < output_num; i++) {
        data_buffer = svp_acl_mdl_get_dataset_buffer(task->output_dataset, i);
        data = svp_acl_get_data_buffer_addr(data_buffer);
        (td_void)svp_acl_rt_free(data);
        (td_void)svp_acl_destroy_data_buffer(data_buffer);
    }

    (td_void)svp_acl_mdl_destroy_dataset(task->output_dataset);
    task->output_dataset = TD_NULL;
}

td_void sample_common_svp_npu_destroy_task_buf(sample_svp_npu_task_info *task)
{
    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return;
    }

    if (task->task_buf_ptr == TD_NULL) {
        return;
    }
    (td_void)svp_acl_rt_free(task->task_buf_ptr);
    task->task_buf_ptr = TD_NULL;
    task->task_buf_stride = 0;
    task->task_buf_size = 0;
}

td_void sample_common_svp_npu_destroy_work_buf(sample_svp_npu_task_info *task)
{
    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return;
    }

    if (task->work_buf_ptr == TD_NULL) {
        return;
    }
    (td_void)svp_acl_rt_free(task->work_buf_ptr);
    task->work_buf_ptr = TD_NULL;
    task->work_buf_stride = 0;
    task->work_buf_size = 0;
}

td_s32 sample_common_svp_npu_create_task_buf(sample_svp_npu_task_info *task)
{
    size_t num;
    svp_acl_data_buffer *task_buf = TD_NULL;
    svp_acl_error ret;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }
    sample_svp_check_exps_return(task->input_dataset == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "input_dataset is NULL!\n");

    num = svp_acl_mdl_get_dataset_num_buffers(task->input_dataset);
    sample_svp_check_exps_return(num != g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM,
        TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "num of data buffer(%lu) should be %lu when create task buf!\n",
        num, g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM);

    task_buf = sample_common_svp_npu_create_input_data_buffer(task,
        g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM);
    sample_svp_check_exps_return(task_buf == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "create task buf failed!\n");

    ret = svp_acl_mdl_add_dataset_buffer(task->input_dataset, task_buf);
    if (ret != SVP_ACL_SUCCESS) {
        sample_svp_trace_err("add task buf failed!\n");
        (td_void)sample_svp_npu_destroy_data_buffer(task_buf);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_create_work_buf(sample_svp_npu_task_info *task)
{
    size_t num;
    svp_acl_data_buffer *work_buf = TD_NULL;
    svp_acl_error ret;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }

    sample_svp_check_exps_return(task->input_dataset == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "input_dataset is NULL!\n");

    num = svp_acl_mdl_get_dataset_num_buffers(task->input_dataset);
    sample_svp_check_exps_return(num != g_svp_npu_model[task->cfg.model_idx].input_num - 1,
        TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "num of data buffer(%lu) should be %lu when create work buf!\n",
        num, g_svp_npu_model[task->cfg.model_idx].input_num - 1);

    work_buf = sample_common_svp_npu_create_input_data_buffer(task, g_svp_npu_model[task->cfg.model_idx].input_num - 1);
    sample_svp_check_exps_return(work_buf == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "create work buf failed!\n");

    ret = svp_acl_mdl_add_dataset_buffer(task->input_dataset, work_buf);
    if (ret != SVP_ACL_SUCCESS) {
        sample_svp_trace_err("add work buf failed!\n");
        (td_void)sample_svp_npu_destroy_data_buffer(work_buf);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_get_work_buf_info(const sample_svp_npu_task_info *task,
    td_u32 *work_buf_size, td_u32 *work_buf_stride)
{
    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }

    *work_buf_stride = (td_u32)svp_acl_mdl_get_input_default_stride(g_svp_npu_model[task->cfg.model_idx].model_desc,
        g_svp_npu_model[task->cfg.model_idx].input_num - 1);
    if (*work_buf_stride == 0) {
        sample_svp_trace_err("get work buf stride failed!\n");
        return TD_FAILURE;
    }

    *work_buf_size = (td_u32)svp_acl_mdl_get_input_size_by_index(g_svp_npu_model[task->cfg.model_idx].model_desc,
        g_svp_npu_model[task->cfg.model_idx].input_num - 1);
    if (*work_buf_size == 0) {
        sample_svp_trace_err("get work buf size failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_share_work_buf(const sample_svp_npu_shared_work_buf *shared_work_buf,
    const sample_svp_npu_task_info *task)
{
    svp_acl_error ret;
    svp_acl_data_buffer *work_buf = TD_NULL;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }

    sample_svp_check_exps_return(shared_work_buf == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "shared_work_buf is NULL!\n");

    sample_svp_check_exps_return(shared_work_buf->work_buf_ptr == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "work_buf_ptr is NULL!\n");

    sample_svp_check_exps_return(task->work_buf_ptr != TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "task has created work buf!\n");

    work_buf = svp_acl_create_data_buffer(shared_work_buf->work_buf_ptr, shared_work_buf->work_buf_size,
        shared_work_buf->work_buf_stride);
    sample_svp_check_exps_return(work_buf == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "create work buf failed!\n");

    ret = svp_acl_mdl_add_dataset_buffer(task->input_dataset, work_buf);
    if (ret != SVP_ACL_SUCCESS) {
        sample_svp_trace_err("add work buf failed!\n");
        (td_void)svp_acl_destroy_data_buffer(work_buf);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* print the top n confidence values with indexes */
static td_void sample_svp_npu_sort_output_result(const td_float *src, td_u32 src_len,
    sample_svp_npu_top_n_result *dst, td_u32 dst_len)
{
    td_u32 i;
    td_u32 j;
    td_u32 index;
    td_bool charge;

    for (i = 0; i < src_len; i++) {
        charge = TD_FALSE;

        for (j = 0; j < dst_len; j++) {
            if (src[i] > dst[j].score) {
                index = j;
                charge = TD_TRUE;
                break;
            }
        }

        if (charge == TD_TRUE) {
            for (j = dst_len - 1; j > index; j--) {
                dst[j].score = dst[j - 1].score;
                dst[j].class_id = dst[j - 1].class_id;
            }
            dst[index].score = src[i];
            dst[index].class_id = i;
        }
    }
}

td_void sample_common_svp_npu_output_classification_result(const sample_svp_npu_task_info *task)
{
    svp_acl_data_buffer *data_buffer = TD_NULL;
    td_void *data = TD_NULL;
    td_u32 i;
    td_u32 j;
    td_u32 n;
    svp_acl_error ret;
    size_t stride;
    svp_acl_mdl_io_dims dims;
    sample_svp_npu_top_n_result top[SAMPLE_SVP_NPU_SHOW_TOP_NUM] = { 0 };

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return;
    }

    sample_svp_check_exps_return_void(task->output_dataset == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "task->output_dataset is NULL!\n");

    for (i = 0; i < svp_acl_mdl_get_dataset_num_buffers(task->output_dataset); i++) {
        data_buffer = svp_acl_mdl_get_dataset_buffer(task->output_dataset, i);
        sample_svp_check_exps_return_void(data_buffer == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get %u-th data buffer is NULL!\n", i);

        data = svp_acl_get_data_buffer_addr(data_buffer);
        sample_svp_check_exps_return_void(data == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get %u-th data addr is NULL!\n", i);

        stride = svp_acl_get_data_buffer_stride(data_buffer);
        sample_svp_check_exps_return_void(data == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get %u-th data stride is 0!\n", i);

        ret = svp_acl_mdl_get_output_dims(g_svp_npu_model[task->cfg.model_idx].model_desc, i, &dims);
        sample_svp_check_exps_return_void(data == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get %u-th output dims failed, error code is %d!\n", i, ret);

        for (n = 0; n < task->cfg.dynamic_batch_num; n++) {
            (td_void)sample_svp_npu_sort_output_result(data, (td_u32)dims.dims[dims.dim_count - 1],
                top, SAMPLE_SVP_NPU_SHOW_TOP_NUM);
            sample_svp_trace_info("%u-th batch result:\n", n);
            for (j = 0; j < SAMPLE_SVP_NPU_SHOW_TOP_NUM; j++) {
                sample_svp_trace_info("top %d: value[%lf], class_id[%u]!\n", j, top[j].score, top[j].class_id);
            }
            data += stride;
            (td_void)memset_s(top, sizeof(top), 0, sizeof(top));
        }
    }

    sample_svp_trace_info("output data success!\n");
}

td_s32 sample_common_svp_npu_set_dynamic_batch(const sample_svp_npu_task_info *task)
{
    svp_acl_error ret;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }
    ret = svp_acl_mdl_set_dynamic_batch_size(g_svp_npu_model[task->cfg.model_idx].model_id,
        task->input_dataset, g_svp_npu_model[task->cfg.model_idx].dynamic_batch_idx, task->cfg.dynamic_batch_num);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "set dynamic batch size failed, model id is %u, error code is %d!\n",
        g_svp_npu_model[task->cfg.model_idx].model_id, ret);

    if (task->cfg.total_t != 0) {
        ret = svp_acl_mdl_set_total_t(g_svp_npu_model[task->cfg.model_idx].model_id,
        task->input_dataset, task->cfg.total_t);
        sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "set total_t failed, model id is %u, error code is %d!\n",
            g_svp_npu_model[task->cfg.model_idx].model_id, ret);
    }

    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_model_execute(const sample_svp_npu_task_info *task)
{
    svp_acl_data_buffer *data_buffer = TD_NULL;
    td_void *data = TD_NULL;
    size_t size;
    td_u32 i;
    svp_acl_error ret;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }

    ret = svp_acl_mdl_execute(g_svp_npu_model[task->cfg.model_idx].model_id, task->input_dataset, task->output_dataset);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "svp_acl_mdl_execute failed, model_id is %u, error code is %d!\n",
        g_svp_npu_model[task->cfg.model_idx].model_id, ret);

    if (task->cfg.is_cached == TD_TRUE) {
        for (i = 0; i < g_svp_npu_model[task->cfg.model_idx].output_num; i++) {
            data_buffer = svp_acl_mdl_get_dataset_buffer(task->output_dataset, i);
            sample_svp_check_exps_return(data_buffer == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "get %u-th output data_buffer is NULL!\n", i);

            data = svp_acl_get_data_buffer_addr(data_buffer);
            sample_svp_check_exps_return(data == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "get %u-th output data is NULL!\n", i);

            size = svp_acl_get_data_buffer_size(data_buffer) / task->cfg.max_batch_num * task->cfg.dynamic_batch_num;
            sample_svp_check_exps_return(size == 0, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "get %u-th output data size is 0!\n", i);

            ret = svp_acl_rt_mem_flush(data, size);
            sample_svp_check_exps_return(data_buffer == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "flush %u-th output data failed, error code is %d!\n", i, ret);
        }
    }
    return ret;
}

static td_void sample_svp_npu_dump_dataset(const sample_svp_npu_task_info *task, const td_char *file_name,
    td_bool is_input)
{
    td_u32 i;
    td_u32 line;
    td_u32 total_line_num;
    td_u32 line_byte_num;
    td_u32 data_num;
    svp_acl_error ret;
    size_t size;
    size_t stride;
    svp_acl_data_buffer *data_buffer = TD_NULL;
    svp_acl_mdl_dataset *dataset = TD_NULL;
    td_void *data = TD_NULL;
    FILE *fp = TD_NULL;
    td_char name[SAMPLE_SVP_NPU_MAX_NAME_LEN] = {'\0'};

    data_num = is_input == TD_TRUE ? g_svp_npu_model[task->cfg.model_idx].input_num - SAMPLE_SVP_NPU_EXTRA_INPUT_NUM :
        g_svp_npu_model[task->cfg.model_idx].output_num;
    dataset = is_input == TD_TRUE ? task->input_dataset : task->output_dataset;

    for (i = 0; i < data_num; i++) {
        ret = sample_svp_npu_get_line_num_and_line_byte_num(task, i, is_input, &total_line_num, &line_byte_num);
        sample_svp_check_exps_return_void(ret != TD_SUCCESS, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get line num and line byte num failed!\n");

        data_buffer = svp_acl_mdl_get_dataset_buffer(dataset, i);
        sample_svp_check_exps_return_void(data_buffer == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get %u-th data_buffer failed!\n", i);

        data = svp_acl_get_data_buffer_addr(data_buffer);
        sample_svp_check_exps_return_void(data == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get %u-th data addr failed!\n", i);

        stride = svp_acl_get_data_buffer_stride(data_buffer);
        sample_svp_check_exps_return_void(stride == 0, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get %u-th data stride failed!\n", i);

        size = svp_acl_get_data_buffer_size(data_buffer);
        sample_svp_check_exps_return_void(size == 0, SAMPLE_SVP_ERR_LEVEL_ERROR, "get %u-th data size failed!\n", i);

        sample_svp_check_exps_return_void(size < (td_u64)task->cfg.dynamic_batch_num * total_line_num * stride,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "%u-th data buffer size(%lu) is less than needed(%llu)!\n",
            i, size, (td_u64)task->cfg.dynamic_batch_num * total_line_num * stride);

        ret = snprintf_s(name, SAMPLE_SVP_NPU_MAX_NAME_LEN - 1, SAMPLE_SVP_NPU_MAX_NAME_LEN - 1, file_name, i);
        sample_svp_check_exps_return_void(ret < 0, SAMPLE_SVP_ERR_LEVEL_ERROR, "create file name failed!\n");

        fp = fopen(name, "wb");
        sample_svp_check_exps_return_void(fp == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "open file failed!\n");

        for (line = 0; line < task->cfg.dynamic_batch_num * total_line_num; line++) {
            ret = fwrite(data, line_byte_num, 1, fp);
            if (ret != 1) {
                fclose(fp);
                sample_svp_check_exps_return_void(ret != 1, SAMPLE_SVP_ERR_LEVEL_ERROR, "write file failed!\n");
            }
            data += stride;
        }
        (td_void)fclose(fp);
    }
}

td_void sample_common_svp_npu_dump_task_data(const sample_svp_npu_task_info *task)
{
    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return;
    }

    sample_svp_check_exps_return_void(task->input_dataset == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "task->input_dataset is NULL!\n");
    sample_svp_check_exps_return_void(task->output_dataset == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "task->input_dataset is NULL!\n");

    (td_void)sample_svp_npu_dump_dataset(task, "input_%u.bin", TD_TRUE);
    (td_void)sample_svp_npu_dump_dataset(task, "output_%u.bin", TD_FALSE);
}

td_s32 sample_common_svp_npu_update_input_data_buffer_info(td_u8 *virt_addr, td_u32 size, td_u32 stride, td_u32 idx,
    const sample_svp_npu_task_info *task)
{
    svp_acl_data_buffer *data_buffer = TD_NULL;
    svp_acl_error ret;

    data_buffer = svp_acl_mdl_get_dataset_buffer(task->input_dataset, idx);
    sample_svp_check_exps_return(data_buffer == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get %u-th data_buffer failed!\n", idx);
    ret = svp_acl_update_data_buffer(data_buffer, (td_void *)virt_addr, size, stride);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "update data buffer failed!\n");
    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_get_input_data_buffer_info(const sample_svp_npu_task_info *task, td_u32 idx,
    td_u8 **virt_addr, td_u32 *size, td_u32 *stride)
{
    svp_acl_data_buffer *data_buffer = TD_NULL;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }
    sample_svp_check_exps_return(virt_addr == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "virt_addr is NULL!\n");
    sample_svp_check_exps_return(size == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "size is NULL!\n");
    sample_svp_check_exps_return(stride == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "size is NULL!\n");

    data_buffer = svp_acl_mdl_get_dataset_buffer(task->input_dataset, idx);
    sample_svp_check_exps_return(data_buffer == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get 0-th data_buffer failed!\n");
    *size = (td_u32)svp_acl_get_data_buffer_size(data_buffer);
    *stride = (td_u32)svp_acl_get_data_buffer_stride(data_buffer);
    *virt_addr = (td_u8 *)svp_acl_get_data_buffer_addr(data_buffer);
    return TD_SUCCESS;
}

static td_s32 sample_svp_npu_get_roi_num(const sample_svp_npu_task_info *task, sample_svp_npu_detection_info *info,
    ot_sample_svp_rect_info *rect_info)
{
    svp_acl_error ret;
    size_t num_idx;
    svp_acl_mdl_io_dims dims;
    svp_acl_data_buffer *data_buffer = TD_NULL;
    td_float *data = TD_NULL;
    td_float total_num = 0.0;
    td_u32 start = 0;
    td_u32 i;

    ret = svp_acl_mdl_get_output_index_by_name(g_svp_npu_model[task->cfg.model_idx].model_desc,
        info->num_name, &num_idx);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get num idx failed!\n");

    ret = svp_acl_mdl_get_output_dims(g_svp_npu_model[task->cfg.model_idx].model_desc, num_idx, &dims);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get num dims failed!\n");

    data_buffer = svp_acl_mdl_get_dataset_buffer(task->output_dataset, num_idx);
    sample_svp_check_exps_return(data_buffer == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get num data_buffer is NULL!\n");

    data = (td_float *)svp_acl_get_data_buffer_addr(data_buffer);
    sample_svp_check_exps_return(data == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "get num data is NULL!\n");

    if (info->has_background == TD_TRUE) {
        info->roi_offset = (td_u32)(*data);
        start++;
    }

    for (i = start; i < dims.dims[dims.dim_count - 1]; i++) {
        total_num += *(data + i);
    }
    rect_info->num = (td_u32)total_num;
    return TD_SUCCESS;
}

static td_s32 sample_svp_npu_get_roi(const sample_svp_npu_task_info *task, const sample_svp_npu_detection_info *info,
    const ot_video_frame_info *proc_frame, const ot_video_frame_info *show_frame, ot_sample_svp_rect_info *rect_info)
{
    svp_acl_error ret;
    size_t roi_idx;
    size_t stride;
    svp_acl_data_buffer *data_buffer = TD_NULL;
    td_float *x_min = TD_NULL;
    td_float *y_min = TD_NULL;
    td_float *x_max = TD_NULL;
    td_float *y_max = TD_NULL;
    td_u32 roi_offset = info->roi_offset;
    td_u32 i;

    ret = svp_acl_mdl_get_output_index_by_name(g_svp_npu_model[task->cfg.model_idx].model_desc,
        info->roi_name, &roi_idx);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get roi idx failed!\n");

    data_buffer = svp_acl_mdl_get_dataset_buffer(task->output_dataset, roi_idx);
    sample_svp_check_exps_return(data_buffer == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get roi data_buffer is NULL!\n");

    stride = svp_acl_get_data_buffer_stride(data_buffer);
    sample_svp_check_exps_return(stride == 0, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "get roi stride is 0!\n");

    x_min = (td_float *)svp_acl_get_data_buffer_addr(data_buffer);
    sample_svp_check_exps_return(x_min == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "get roi data is NULL!\n");
    y_min = x_min + stride / sizeof(td_float);
    x_max = y_min + stride / sizeof(td_float);
    y_max = x_max + stride / sizeof(td_float);

    for (i = 0; i < rect_info->num; i++) {
        rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].x = (td_u32)((td_float)x_min[roi_offset] /
            proc_frame->video_frame.width * show_frame->video_frame.width) & (~1);
        rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].y = (td_u32)((td_float)y_min[roi_offset] /
            proc_frame->video_frame.height * show_frame->video_frame.height) & (~1);

        rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].x = (td_u32)((td_float)x_max[roi_offset] /
            proc_frame->video_frame.width * show_frame->video_frame.width) & (~1);
        rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].y =
            rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].y;

        rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].x =
            rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].x;
        rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].y = (td_u32)((td_float)y_max[roi_offset] /
            proc_frame->video_frame.height * show_frame->video_frame.height) & (~1);

        rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_LEFT_BOTTOM].x =
            rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].x;
        rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_LEFT_BOTTOM].y =
            rect_info->rect[i].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].y;
        roi_offset++;
    }
    return TD_SUCCESS;
}

static td_float sample_svp_npu_calc_iou(const sample_svp_npu_roi_box* box1, const sample_svp_npu_roi_box* box2)
{
    td_float area1 = box1->area;
    td_float area2 = box2->area;
    td_float xx1 = (box1->lx > box2->lx) ? box1->lx : box2->lx;
    td_float yy1 = (box1->ly > box2->ly) ? box1->ly : box2->ly;
    td_float xx2 = (box1->rx < box2->rx) ? box1->rx : box2->rx;
    td_float yy2 = (box1->ry < box2->ry) ? box1->ry : box2->ry;
    td_float w = (0.0f > (xx2 - xx1 + 1)) ? 0.0f : (xx2 - xx1 + 1);
    td_float h = (0.0f > (yy2 - yy1 + 1)) ? 0.0f : (yy2 - yy1 + 1);
    td_float inter = w * h;
    td_float ovr = inter /(area1 + area2 - inter);
    return ovr;
}

static td_void sample_svp_npu_multiclass_nms(sample_svp_npu_roi_box *roi_box, td_u32 *roi_box_num,
    sample_svp_npu_valid_box* valid_box, td_u32 valid_box_num)
{
    float nms_thr = 0.45;
    for (td_u32 i = 0; i < valid_box_num; i++) {
        sample_svp_npu_valid_box *item = &valid_box[i];
        td_float box_x_center = item->xcenter;
        td_float box_y_center = item->ycenter;
        td_float box_width = item->w;
        td_float box_height = item->h;

        td_float x1 = (box_x_center - box_width / 2);
        td_float y1 = (box_y_center - box_height / 2);
        td_float x2 = (box_x_center + box_width / 2);
        td_float y2 = (box_y_center + box_height / 2);
        td_float area = (x2 - x1 + 1) * (y2 - y1 + 1);
        td_bool keep = TD_TRUE;
        /* lx, ly, rx, ry, score, class id, area */
        sample_svp_npu_roi_box box = {x1, y1, x2, y2, item->score, item->class_id, area};
        for (td_u32 j = 0; j < *roi_box_num; j++) {
            if (sample_svp_npu_calc_iou(&box, &roi_box[j]) > nms_thr) {
                keep = TD_FALSE;
                break;
            }
        }
        if (keep) {
            if (*roi_box_num >= OT_SVP_RECT_NUM) {
                sample_svp_trace_err("roi_box_num [%u] is large than %u!\n", *roi_box_num, OT_SVP_RECT_NUM);
                return;
            }
            roi_box[*roi_box_num].lx = box.lx;
            roi_box[*roi_box_num].ly = box.ly;
            roi_box[*roi_box_num].rx = box.rx;
            roi_box[*roi_box_num].ry = box.ry;
            roi_box[*roi_box_num].score = box.score;
            roi_box[*roi_box_num].class_id = box.class_id;
            roi_box[*roi_box_num].area = box.area;
            *roi_box_num = *roi_box_num + 1;
        }
    }
}

inline static td_float sample_svp_npu_sig_mod(td_float a)
{
    return 1.0f / (1.0f + exp(-a));
}

static td_void sample_svp_npu_get_max_score_and_idx(td_u32 obj_score_idx, td_u32 chn_step, const td_float* out_data,
    td_float *max_cls_score, td_u32 *max_cls_id)
{
    td_u32 cls_score_idx = obj_score_idx + chn_step;
    for (td_u32 c = 0; c < SAMPLE_SVP_NPU_YOLO_CLASS_NUM; c++) {
        td_float cls_score_val = out_data[cls_score_idx];
        if (cls_score_val > *max_cls_score) {
            *max_cls_score = cls_score_val;
            *max_cls_id = c;
        }
        cls_score_idx += chn_step;
    }
}
static td_u32 g_sample_svp_npu_anchor_grids[SAMPLE_SVP_NPU_YOLO_CPU_OUT_NUM][SAMPLE_SVP_NPU_YOLO_CLASS_MEMBER_NUM] = {
    {116, 90, 156, 198, 373, 326},
    {30, 61, 62, 45, 59, 119},
    {10, 13, 16, 30, 33, 23}
};

static td_void sample_svp_npu_process_per_dectection_inner(const sample_svp_npu_detection_inner_param *inner_param,
    td_float *grids_x, td_float *grids_y, sample_svp_npu_valid_box* valid_box, td_u32 *valid_box_num)
{
    td_float box_hieght;
    td_u32 expanded_strides[SAMPLE_SVP_NPU_YOLO_CPU_OUT_NUM] = { 32, 16, 8 };
    td_u32 out_height_idx = inner_param->out_height_idx;
    td_u32 chn_step = inner_param->chn_step;
    td_float score_thr = inner_param->score_thr;
    td_float *out_data = inner_param->out_data;
    size_t w_stride_offset = inner_param->w_stride_offset;
    td_u32 obj_score_offset = inner_param->obj_score_offset;
    td_u32 offset = out_height_idx * inner_param->w_stride_offset;
    for (td_u32 j = 0; j < inner_param->out_width; j++) {
        for (td_u32 k = 0; k < SAMPLE_SVP_NPU_YOLO_SCALE_SIZE; k++) {
            offset = j + out_height_idx * w_stride_offset + k * chn_step * SAMPLE_SVP_NPU_YOLO_OUT_PARM_NUM;
            td_u32 obj_score_idx = offset + obj_score_offset;
            td_float obj_score_val = sample_svp_npu_sig_mod(out_data[obj_score_idx]);
            if (obj_score_val <= score_thr) {
                continue;
            }
            if (*valid_box_num >= OT_SVP_RECT_NUM) {
                sample_svp_trace_err("valid_box_num [%u] is large than %u!\n", *valid_box_num, OT_SVP_RECT_NUM);
                return;
            }
            /* max score */
            td_float max_cls_score = 0.0f;
            td_u32 max_cls_id = 0;
            sample_svp_npu_get_max_score_and_idx(obj_score_idx, chn_step, out_data, &max_cls_score, &max_cls_id);

            td_float confidence_score = sample_svp_npu_sig_mod(max_cls_score) * obj_score_val;
            if (confidence_score > score_thr) {
                /* gen box  info */
                td_u32 x_center_idx = offset;
                td_u32 y_center_idx = x_center_idx + chn_step;
                td_u32 box_width_idx = y_center_idx + chn_step;
                td_u32 box_height_idx = box_width_idx + chn_step;
                td_float x_center = (sample_svp_npu_sig_mod(out_data[x_center_idx]) * SAMPLE_SVP_NPU_DOUBLE +
                    grids_x[j]) * expanded_strides[inner_param->idx];
                td_float y_center = (sample_svp_npu_sig_mod(out_data[y_center_idx]) * SAMPLE_SVP_NPU_DOUBLE +
                    grids_y[out_height_idx]) * expanded_strides[inner_param->idx];
                td_float tmp_value = sample_svp_npu_sig_mod(out_data[box_width_idx]) * SAMPLE_SVP_NPU_DOUBLE;
                td_float box_width = tmp_value * tmp_value  * g_sample_svp_npu_anchor_grids[inner_param->idx][(k << 1)];
                tmp_value = sample_svp_npu_sig_mod(out_data[box_height_idx]) * SAMPLE_SVP_NPU_DOUBLE;
                box_hieght = tmp_value * tmp_value  * g_sample_svp_npu_anchor_grids[inner_param->idx][(k << 1) +1];
                valid_box[*valid_box_num].score = confidence_score;
                valid_box[*valid_box_num].xcenter = x_center;
                valid_box[*valid_box_num].ycenter = y_center;
                valid_box[*valid_box_num].w = box_width;
                valid_box[*valid_box_num].h = box_hieght;
                valid_box[*valid_box_num].class_id = (td_float)max_cls_id;
                *valid_box_num = *valid_box_num + 1;
            }
        }
    }
}

static td_void sample_svp_npu_process_per_dectection(const sample_svp_npu_task_info *task, td_u32 idx,
    sample_svp_npu_valid_box* valid_box, td_u32 *valid_box_num)
{
    td_u32 out_height;
    svp_acl_mdl_io_dims out_dims;
    sample_svp_npu_detection_inner_param inner_param;
    svp_acl_data_buffer* data_buffer = TD_NULL;
    td_u32 h_sizes[SAMPLE_SVP_NPU_YOLO_CPU_OUT_NUM] = { 20, 40, 80 };
    td_u32 w_sizes[SAMPLE_SVP_NPU_YOLO_CPU_OUT_NUM] = { 20, 40, 80 };
    /* gen grids */
    td_float grids_x[SAMPLE_SVP_NPU_YOLO_CLASS_NUM] = {0};
    td_float grids_y[SAMPLE_SVP_NPU_YOLO_CLASS_NUM] = {0};

    svp_acl_mdl_get_output_dims(g_svp_npu_model[task->cfg.model_idx].model_desc, idx, &out_dims);
    data_buffer = svp_acl_mdl_get_dataset_buffer(task->output_dataset, idx);
    out_height = out_dims.dims[out_dims.dim_count - SAMPLE_SVP_NPU_DOUBLE];
    inner_param.out_width = out_dims.dims[out_dims.dim_count - 1];
    if (out_height > SAMPLE_SVP_NPU_YOLO_CLASS_NUM || inner_param.out_width > SAMPLE_SVP_NPU_YOLO_CLASS_NUM) {
        sample_svp_trace_err("out_width[%u] or out_width[%u] is large than %u!\n",
            out_height, inner_param.out_width, SAMPLE_SVP_NPU_YOLO_CLASS_NUM);
        return;
    }

    inner_param.score_thr = SAMPLE_SVP_NPU_YOLO_SCORE_THRESHOLE;
    inner_param.idx = idx;
    inner_param.out_data = (td_float *)svp_acl_get_data_buffer_addr(data_buffer);
    inner_param.w_stride_offset =
        svp_acl_mdl_get_output_default_stride(g_svp_npu_model[task->cfg.model_idx].model_desc, idx) / sizeof(td_float);
    inner_param.chn_step = out_height * inner_param.w_stride_offset;

    for (td_u32 i = 0; i < h_sizes[idx]; i++) {
        grids_y[i] = i - SAMPLE_SVP_NPU_YOLO_SCORE_THRESHOLE;
    }
    for (td_u32 i = 0; i < w_sizes[idx]; i++) {
        grids_x[i] = i - SAMPLE_SVP_NPU_YOLO_SCORE_THRESHOLE;
    }
    inner_param.obj_score_offset = sizeof(td_float) * inner_param.chn_step;
    for (td_u32 i = 0; i < out_height; i++) {
        inner_param.out_height_idx = i;
        sample_svp_npu_process_per_dectection_inner(&inner_param, grids_x, grids_y, valid_box, valid_box_num);
    }
}

static td_void sample_svp_npu_filter_yolo_box(const sample_svp_npu_task_info *task,
    sample_svp_npu_valid_box* valid_box, td_u32 *valid_box_num)
{
    td_u32 out_num = svp_acl_mdl_get_num_outputs(g_svp_npu_model[task->cfg.model_idx].model_desc);
    if (out_num > SAMPLE_SVP_NPU_YOLO_CPU_OUT_NUM) {
        sample_svp_trace_err("output num [%u] is large than 3!\n", out_num);
        return;
    }

    /* gen box */
    for (td_u32 n = 0; n < out_num; n++) {
        sample_svp_npu_process_per_dectection(task, n, valid_box, valid_box_num);
    }
}

static td_void sample_svp_npu_fill_rect_info(const ot_video_frame_info *proc_frame,
    const ot_video_frame_info *show_frame, const sample_svp_npu_roi_box *roi_box,
    td_u32 roi_box_num, ot_sample_svp_rect_info *rect_info)
{
    td_u32 rect_num = 0;
    td_float w_multiples = (td_float)show_frame->video_frame.width / (td_float)proc_frame->video_frame.width;
    td_float h_multiples = (td_float)show_frame->video_frame.height / (td_float)proc_frame->video_frame.height;
    for (td_u32 i = 0; i < roi_box_num; i++) {
        if (roi_box[i].score > 1.0f || roi_box[i].score < 0.0f) {
            continue;
        }
        rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].x = (td_u32)(roi_box[i].lx * w_multiples) & (~1);
        rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].y = (td_u32)(roi_box[i].ly * h_multiples) & (~1);
        rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].x = (td_u32)(roi_box[i].rx * w_multiples) & (~1);
        rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].y =
            rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].y;
        rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].x =
            rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].x;
        rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].y =
            (td_u32)(roi_box[i].ry * h_multiples) & (~1);
        rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_LEFT_BOTTOM].x =
            rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].x;
        rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_LEFT_BOTTOM].y =
            rect_info->rect[rect_num].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].y;
        rect_num++;
    }
    rect_info->num = rect_num;
}

static td_s32 cmp_valid_box(const void *item1, const void *item2)
{
    sample_svp_npu_valid_box *box1 = (sample_svp_npu_valid_box *)item1;
    sample_svp_npu_valid_box *box2 = (sample_svp_npu_valid_box *)item2;
    if (box1->score > box2->score) {
        return 1;
    }
    return 0;
}

static td_s32 cmp_roi_box(const void *item1, const void *item2)
{
    sample_svp_npu_roi_box *box1 = (sample_svp_npu_roi_box *)item1;
    sample_svp_npu_roi_box *box2 = (sample_svp_npu_roi_box *)item2;
    if (box1->class_id < box2->class_id) {
        return 1;
    } else if (box1->class_id == box2->class_id) {
        return box1->score > box2->score;
    }
    return 0;
}

static td_s32 sample_svp_npu_get_roi_by_cpu(const sample_svp_npu_task_info *task,
    const sample_svp_npu_detection_info *info, const ot_video_frame_info *proc_frame,
    const ot_video_frame_info *show_frame, ot_sample_svp_rect_info *rect_info)
{
    td_u32 valid_box_num = 0;
    td_u32 roi_box_num = 0;
    sample_svp_npu_valid_box valid_box[OT_SVP_RECT_NUM] = {0};
    sample_svp_npu_roi_box roi_box[OT_SVP_RECT_NUM] = {0};

    sample_svp_npu_filter_yolo_box(task, valid_box, &valid_box_num);
    qsort((td_void *)valid_box, valid_box_num, sizeof(sample_svp_npu_valid_box), cmp_valid_box);

    sample_svp_npu_multiclass_nms(roi_box, &roi_box_num, valid_box, valid_box_num);
    if (roi_box_num == 0) {
        sample_svp_trace_info("No target object is detected !!!\n");
        return TD_SUCCESS;
    }
    qsort((td_void *)roi_box, roi_box_num, sizeof(sample_svp_npu_roi_box), cmp_roi_box);
    sample_svp_npu_fill_rect_info(proc_frame, show_frame, roi_box, roi_box_num, rect_info);

    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_roi_to_rect(const sample_svp_npu_task_info *task,
    sample_svp_npu_detection_info *detection_info, const ot_video_frame_info *proc_frame,
    const ot_video_frame_info *show_frame, ot_sample_svp_rect_info *rect_info)
{
    td_s32 ret;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }
    sample_svp_check_exps_return(detection_info == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "detection_info is NULL!\n");
    sample_svp_check_exps_return(proc_frame == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "proc_frame is NULL!\n");
    sample_svp_check_exps_return(show_frame == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "show_frame is NULL!\n");
    sample_svp_check_exps_return(rect_info == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "rect_info is NULL!\n");

    detection_info->roi_offset = 0;
    ret = sample_svp_npu_get_roi_num(task, detection_info, rect_info);
    sample_svp_check_exps_return(ret != TD_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get roi num failed!\n");

    ret = sample_svp_npu_get_roi(task, detection_info, proc_frame, show_frame, rect_info);
    sample_svp_check_exps_return(ret != TD_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get roi failed!\n");

    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_roi_to_rect_cpu(const sample_svp_npu_task_info *task,
    sample_svp_npu_detection_info *detection_info, const ot_video_frame_info *proc_frame,
    const ot_video_frame_info *show_frame, ot_sample_svp_rect_info *rect_info)
{
    td_s32 ret;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }
    sample_svp_check_exps_return(detection_info == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "detection_info is NULL!\n");
    sample_svp_check_exps_return(proc_frame == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "proc_frame is NULL!\n");
    sample_svp_check_exps_return(show_frame == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "show_frame is NULL!\n");
    sample_svp_check_exps_return(rect_info == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "rect_info is NULL!\n");

    ret = sample_svp_npu_get_roi_by_cpu(task, detection_info, proc_frame, show_frame, rect_info);
    sample_svp_check_exps_return(ret != TD_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get roi failed!\n");

    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_set_threshold(sample_svp_npu_threshold threshold[], td_u32 threshold_num,
    const sample_svp_npu_task_info *task)
{
    td_u32 i;
    td_u32 n;
    svp_acl_error ret;
    svp_acl_data_buffer *data_buffer = TD_NULL;
    td_float *data = TD_NULL;
    size_t idx;
    size_t size;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }
    sample_svp_check_exps_return(threshold == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "threshold is NULL!\n");
    sample_svp_check_exps_return(threshold_num == 0, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "threshold_num is 0!\n");

    for (i = 0; i < threshold_num; i++) {
        ret = svp_acl_mdl_get_input_index_by_name(g_svp_npu_model[task->cfg.model_idx].model_desc,
            threshold[i].name, &idx);
        sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get roi idx failed!\n");

        data_buffer = svp_acl_mdl_get_dataset_buffer(task->input_dataset, idx);
        sample_svp_check_exps_return(data_buffer == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get roi data_buffer is NULL!\n");

        size = svp_acl_get_data_buffer_size(data_buffer);
        sample_svp_check_exps_return(size < SAMPLE_SVP_NPU_THRESHOLD_NUM * sizeof(td_float), TD_FAILURE,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "get size(%lu) is less than %lu!\n",
            size, SAMPLE_SVP_NPU_THRESHOLD_NUM * sizeof(td_float));

        data = (td_float *)svp_acl_get_data_buffer_addr(data_buffer);
        sample_svp_check_exps_return(data == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get roi data is NULL!\n");
        n = 0;
        data[n++] = threshold[i].nms_threshold;
        data[n++] = threshold[i].score_threshold;
        data[n++] = threshold[i].min_height;
        data[n++] = threshold[i].min_width;
    }
    return TD_SUCCESS;
}

td_s32 sample_common_svp_npu_check_has_aicpu_task(const sample_svp_npu_task_info *task, td_bool *has_aicpu_task)
{
    td_u32 aicpu_task_num;
    svp_acl_error ret;

    if (sample_svp_check_task_cfg(task) != TD_SUCCESS) {
        sample_svp_trace_err("check task cfg failed!\n");
        return TD_FAILURE;
    }
    sample_svp_check_exps_return(has_aicpu_task == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "has_aicpu_task is NULL!\n");

    ret = svp_acl_ext_get_mdl_aicpu_task_num(g_svp_npu_model[task->cfg.model_idx].model_id, &aicpu_task_num);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get aicpu task num failed, error code is %d!\n", ret);

    *has_aicpu_task = (aicpu_task_num == 0) ? TD_FALSE : TD_TRUE;
    return TD_SUCCESS;
}

sample_svp_npu_model_info* sample_common_svp_npu_get_model_info(td_u32 model_idx)
{
    sample_svp_check_exps_return(model_idx >= SAMPLE_SVP_NPU_MAX_MODEL_NUM, NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "model_idx(%u) must be [0, %u)!\n", model_idx, SAMPLE_SVP_NPU_MAX_MODEL_NUM);
    return &g_svp_npu_model[model_idx];
}

td_s32 sample_common_svp_npu_get_input_resolution(td_u32 model_idx, td_u32 input_idx,
    ot_size *pic_size)
{
    svp_acl_error ret;
    svp_acl_mdl_io_dims dims = {0};

    ret = svp_acl_mdl_get_input_dims(g_svp_npu_model[model_idx].model_desc, input_idx, &dims);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get num dims failed!\n");
    pic_size->height = dims.dims[dims.dim_count - SAMPLE_SVP_NPU_H_DIM_IDX]; // NCHW
    pic_size->width = dims.dims[dims.dim_count - 1]; // NCHW
    return TD_SUCCESS;
}
