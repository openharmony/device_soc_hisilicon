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

#ifndef __SS_MPI_HNR_H__
#define __SS_MPI_HNR_H__

#include "ot_common_vi.h"
#include "ot_common_hnr.h"
#include "ot_common_vb.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_hnr_init(td_void);
td_void ss_mpi_hnr_exit(td_void);

td_s32 ss_mpi_hnr_load_cfg(const ot_hnr_cfg *cfg, td_s32 *cfg_id);
td_s32 ss_mpi_hnr_unload_cfg(td_s32 cfg_id);

td_s32 ss_mpi_hnr_set_alg_cfg(ot_vi_pipe vi_pipe, const ot_hnr_alg_cfg *cfg);
td_s32 ss_mpi_hnr_get_alg_cfg(ot_vi_pipe vi_pipe, ot_hnr_alg_cfg *cfg);

td_s32 ss_mpi_hnr_enable(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_hnr_disable(ot_vi_pipe vi_pipe);

td_s32 ss_mpi_hnr_set_attr(ot_vi_pipe vi_pipe, const ot_hnr_attr *attr);
td_s32 ss_mpi_hnr_get_attr(ot_vi_pipe vi_pipe, ot_hnr_attr *attr);

td_s32 ss_mpi_hnr_set_input_depth(ot_vi_pipe vi_pipe, td_u32 depth);

td_s32 ss_mpi_hnr_set_thread_attr(const ot_hnr_thread_attr *thread_attr);
td_s32 ss_mpi_hnr_get_thread_attr(ot_hnr_thread_attr *thread_attr);

td_s32 ss_mpi_hnr_attach_out_vb_pool(ot_vi_pipe vi_pipe, ot_vb_pool vb_pool);
td_s32 ss_mpi_hnr_detach_out_vb_pool(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SS_MPI_HNR_H__ */
