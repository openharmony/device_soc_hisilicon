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

#ifndef __SS_MPI_PCIV_H__
#define __SS_MPI_PCIV_H__

#include "ot_type.h"
#include "ot_common_pciv.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_pciv_malloc_chn_buf(ot_pciv_chn chn, td_u32 blk_size, td_u32 blk_cnt, td_phys_addr_t phys_addr[]);
td_s32 ss_mpi_pciv_free_chn_buf(ot_pciv_chn chn, td_u32 blk_cnt);
td_s32 ss_mpi_pciv_create_chn(ot_pciv_chn chn, const ot_pciv_attr *attr);
td_s32 ss_mpi_pciv_destroy_chn(ot_pciv_chn chn);
td_s32 ss_mpi_pciv_start_chn(ot_pciv_chn chn);
td_s32 ss_mpi_pciv_stop_chn(ot_pciv_chn chn);
td_s32 ss_mpi_pciv_set_chn_attr(ot_pciv_chn chn, const ot_pciv_attr *attr);
td_s32 ss_mpi_pciv_get_chn_attr(ot_pciv_chn chn, ot_pciv_attr *attr);
td_s32 ss_mpi_pciv_show_chn(ot_pciv_chn chn);
td_s32 ss_mpi_pciv_hide_chn(ot_pciv_chn chn);

td_s32 ss_mpi_pciv_create_window_vb(const ot_pciv_window_vb_cfg *cfg);
td_s32 ss_mpi_pciv_destroy_window_vb(td_void);
td_s32 ss_mpi_pciv_malloc_window_buf(td_u32 blk_size, td_u32 blk_cnt, td_phys_addr_t phys_addr[]);
td_s32 ss_mpi_pciv_free_window_buf(td_u32 blk_cnt, const td_phys_addr_t phys_addr[]);
td_s32 ss_mpi_pciv_dma_task(const ot_pciv_dma_task *task);

td_s32 ss_mpi_pciv_get_local_id(td_s32 *id);
td_s32 ss_mpi_pciv_enum_chip(ot_pciv_enum_chip *chips);
td_s32 ss_mpi_pciv_get_window_base(td_s32 chip_id, ot_pciv_window_base *base);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_PCIV_H__ */
