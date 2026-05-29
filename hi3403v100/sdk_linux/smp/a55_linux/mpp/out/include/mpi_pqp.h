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

#ifndef MPI_PQP_H
#define MPI_PQP_H

#include "common_pqp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 mpi_pqp_init(td_void);

td_s32 mpi_pqp_deinit(td_void);

td_s32 mpi_pqp_load_param(const pqp_mem_info *param_buf, pqp_param_handle *param_handle);

td_s32 mpi_pqp_unload_param(pqp_param_handle param_handle);

td_s32 mpi_pqp_get_sect_num(pqp_param_handle param_handle, td_u32 *sect_num);

td_s32 mpi_pqp_get_input_num(pqp_param_handle param_handle, td_u32 sect_idx, td_u32 *input_num);

td_s32 mpi_pqp_get_output_num(pqp_param_handle param_handle, td_u32 sect_idx, td_u32 *output_num);

td_s32 mpi_pqp_get_input_data_info(pqp_param_handle param_handle, td_u32 sect_idx, td_u32 input_idx,
    pqp_data_info *data_info);

td_s32 mpi_pqp_get_output_data_info(pqp_param_handle param_handle, td_u32 sect_idx, td_u32 output_idx,
    pqp_data_info *data_info);

td_s32 mpi_pqp_get_ping_pong_buf_size(pqp_param_handle param_handle, td_u32 *size);

td_s32 mpi_pqp_get_task_buf_size(pqp_param_handle param_handle, td_u32 sect_idx, td_u32 *size);

td_s32 mpi_pqp_va_page_sync(td_u64 va, td_u32 len);

td_s32 mpi_pqp_send_task(pqp_param_handle param_handle, const pqp_inout_data *inout_data, const pqp_task_ctrl *ctrl,
    pqp_task_handle *task_handle);

td_s32 mpi_pqp_query_task(pqp_task_handle task_handle, td_s32 milli_sec, td_bool *is_finish);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* MPI_PQP_H */
