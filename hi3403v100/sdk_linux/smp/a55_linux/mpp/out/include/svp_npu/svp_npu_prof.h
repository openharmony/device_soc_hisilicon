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

#ifndef  SVP_NPU_PROF_H
#define  SVP_NPU_PROF_H

#include "svp_npu_model_common.h"
#include "svp_npu_common.h"
#include "svp_npu_json.h"
#include "ot_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define PROF_PATH_MAX_LEN   4096
#define PROF_DEFAULT_LEN  128
#define PROF_MAX_SAMPLE_INTERVAL    9999

#define PROF_MODEL_TYPE  SVP_NPU_MODEL_PROFILE

#ifdef PROF_USER_DEBUG
#define svp_npu_prof_info_trace(fmt, ...) OT_INFO_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define svp_npu_prof_info_trace(fmt, ...)
#endif

typedef enum {
    SVP_NPU_PROF_WORK_MODE_OFF = 0,
    SVP_NPU_PROF_WORK_MODE_CMD,
    SVP_NPU_PROF_WORK_MODE_API_CTRL,
    SVP_NPU_PROF_WORK_MODE_SUBSCRIBE,
    SVP_NPU_PROF_WORK_MODE_JSON,
} prof_work_mode;

typedef struct {
    td_u32 model_id;
    td_u32 channel;
    td_u32 op_num;
    td_u32 desc_size;
    td_u64 start;
    td_u64 end;
} prof_subscribe_node;

#define PROF_MAX_SUBSCRIBE_INFO_NUM 64

typedef struct {
    prof_subscribe_node sub_node[PROF_MAX_SUBSCRIBE_INFO_NUM];
    td_bool map_model[SVP_NPU_MAX_MODEL_NUM];
    td_u32 model_num;
} prof_subscribe_info;

typedef struct {
    td_char result_path[PROF_PATH_MAX_LEN];
    td_u32 dev_id_list[SVP_NPU_DEVICE_ID_BUTT];
    td_u32 dev_num;

    td_u32 channels;

    td_bool prof_en;
    td_u32 interval;
    td_u32 data_len[SVP_NPU_MAX_MODEL_NUM][2];

    prof_subscribe_info *sub_info;
    const td_void *fd;
} svp_npu_mpi_prof_info;

typedef struct {
    td_u32 dev_id;
    const td_void *fd;
} prof_thread_param;

td_void mpi_svp_npu_prof_init(td_void);
td_void mpi_svp_npu_prof_exit(td_void);

td_void svp_npu_json_prof_init(const svp_npu_json_node *root_node);
td_void svp_npu_json_prof_exit(td_void);

td_s32 mpi_svp_npu_prof_start(td_u32 dev_id, td_u32 channels, svp_npu_prof_switch switch_mode,
    td_u32 model_id, td_u32 interval);
td_s32 mpi_svp_npu_prof_stop(td_u32 dev_id, td_u32 channels, svp_npu_prof_switch switch_mode, td_u32 model_id);
td_s32 mpi_svp_npu_prof_update_info(td_u32 dev_id, td_u32 model_id, const svp_npu_prof_model_update_info *info);

prof_work_mode svp_npu_prof_get_work_mode(td_void);
td_bool svp_npu_prof_is_init(td_void);
td_bool svp_npu_model_prof_is_enable(td_u32 model_id);
td_bool svp_npu_prof_is_acl_api_enable(td_void);

td_void svp_npu_prof_create_lstm_cont(td_u32 dev_id, const td_u8 *model_name,
    const svp_npu_prof_poll_attr *poll_attr, const td_u8 *layer_ptr, td_u32 size);

td_void svp_npu_prof_sem_wait_data(td_void);
td_void svp_npu_prof_sem_post_data(td_void);

td_void svp_npu_prof_match_model(const svp_npu_model_dynamic_info *dynamic_info, const td_u8 *name,
    td_u32 model_id);

td_void svp_npu_prof_model_unload(td_u32 model_id, const svp_npu_model_info *model_info);

/* ACL API SAVE */
td_s32 svp_npu_prof_save_init(const td_char *base_path);
td_s32 svp_npu_prof_save_exit(td_void);
td_s32 svp_npu_prof_save_start(const td_u32 *dev_id_list, td_u32 dev_num, td_u32 channels);
td_s32 svp_npu_prof_save_stop(td_u32 *dev_id_list, td_u32 dev_num, td_u32 channels);

/* ACL SUBSCRIBE */
td_s32 svp_npu_prof_subscribe_init(td_u32 model_id, td_u32 channels, const td_void *fd);
td_s32 svp_npu_prof_subscribe_exit(td_u32 model_id);

td_s32 svp_npu_prof_subscribe_get_op_desc_size(td_u32 *op_desc_size);
td_s32 svp_npu_prof_subscribe_get_op_num(const td_void *op_info, td_ulong op_info_len, uint32_t *op_number);
td_s32 svp_npu_prof_subscribe_get_op_type_len(const td_void *op_info, td_ulong op_info_len,
    uint32_t index, td_u32 *op_type_len);
td_s32 svp_npu_prof_subscribe_get_op_type(const td_void *op_info, td_ulong op_info_len, uint32_t index,
    td_char *op_type, td_u32 op_type_len);
td_s32 svp_npu_prof_subscribe_get_op_name_len(const td_void *op_info, td_ulong op_info_len,
    uint32_t index, td_u32 *op_name_len);
td_s32 svp_npu_prof_subscribe_get_op_name(const td_void *op_info, td_ulong op_info_len, td_u32 index,
    td_char *op_name, td_u32 op_name_len);

td_u64 svp_npu_prof_subscribe_get_op_start(const td_void *op_info, td_ulong op_info_len, td_u32 index);
td_u64 svp_npu_prof_subscribe_get_op_end(const td_void *op_info, td_ulong op_info_len, td_u32 index);
td_u64 svp_npu_prof_subscribe_get_op_duration(const td_void *op_info, td_ulong op_info_len, td_u32 index);

td_u32 svp_npu_prof_subscribe_get_model_id(const td_void *op_info, td_ulong op_info_len, td_u32 index);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_PROF_H */
