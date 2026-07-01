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

#ifndef __SS_MPI_KLAD_H__
#define __SS_MPI_KLAD_H__

#include "ot_common_klad.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_klad_init(td_void);

td_s32 ss_mpi_klad_deinit(td_void);

td_s32 ss_mpi_klad_create(td_handle *klad);

td_s32 ss_mpi_klad_destroy(td_handle klad);

td_s32 ss_mpi_klad_attach(td_handle klad, td_handle target);

td_s32 ss_mpi_klad_detach(td_handle klad, td_handle target);

td_s32 ss_mpi_klad_set_attr(td_handle klad, const ot_klad_attr *attr);

td_s32 ss_mpi_klad_get_attr(td_handle klad, ot_klad_attr *attr);

td_s32 ss_mpi_klad_set_session_key(td_handle klad, const ot_klad_session_key *key);

td_s32 ss_mpi_klad_set_content_key(td_handle klad, const ot_klad_content_key *key);

td_s32 ss_mpi_klad_set_clear_key(td_handle klad, const ot_klad_clear_key *key);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_KLAD_H__ */
