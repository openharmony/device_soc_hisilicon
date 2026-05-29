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

#ifndef __SS_MPI_REGION_H__
#define __SS_MPI_REGION_H__

#include "ot_debug.h"
#include "ot_common_region.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_rgn_create(ot_rgn_handle handle, const ot_rgn_attr *rgn_attr);
td_s32 ss_mpi_rgn_destroy(ot_rgn_handle handle);

td_s32 ss_mpi_rgn_get_attr(ot_rgn_handle handle, ot_rgn_attr *rgn_attr);
td_s32 ss_mpi_rgn_set_attr(ot_rgn_handle handle, const ot_rgn_attr *rgn_attr);

td_s32 ss_mpi_rgn_set_bmp(ot_rgn_handle handle, const ot_bmp *bmp);

td_s32 ss_mpi_rgn_attach_to_chn(ot_rgn_handle handle, const ot_mpp_chn *chn, const ot_rgn_chn_attr *chn_attr);
td_s32 ss_mpi_rgn_detach_from_chn(ot_rgn_handle handle, const ot_mpp_chn *chn);

td_s32 ss_mpi_rgn_set_display_attr(ot_rgn_handle handle, const ot_mpp_chn *chn, const ot_rgn_chn_attr *chn_attr);
td_s32 ss_mpi_rgn_get_display_attr(ot_rgn_handle handle, const ot_mpp_chn *chn, ot_rgn_chn_attr *chn_attr);

td_s32 ss_mpi_rgn_get_canvas_info(ot_rgn_handle handle, ot_rgn_canvas_info *canvas_info);
td_s32 ss_mpi_rgn_update_canvas(ot_rgn_handle handle);

td_s32 ss_mpi_rgn_batch_begin(ot_rgn_handle_grp *grp, td_u32 handle_num, const ot_rgn_handle handle[]);
td_s32 ss_mpi_rgn_batch_end(ot_rgn_handle_grp grp);

td_s32 ss_mpi_rgn_close_fd(td_void);

#ifdef __cplusplus
}
#endif

#endif