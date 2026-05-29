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

#ifndef __SS_MPI_SNAP_H__
#define __SS_MPI_SNAP_H__

#include "ot_common_snap.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_snap_set_pipe_attr(ot_vi_pipe vi_pipe, const ot_snap_attr *snap_attr);
td_s32 ss_mpi_snap_get_pipe_attr(ot_vi_pipe vi_pipe, ot_snap_attr *snap_attr);

td_s32 ss_mpi_snap_enable_pipe(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_snap_disable_pipe(ot_vi_pipe vi_pipe);

td_s32 ss_mpi_snap_trigger_pipe(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SS_MPI_SNAP_H__ */