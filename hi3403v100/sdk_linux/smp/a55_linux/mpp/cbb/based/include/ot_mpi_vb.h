/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef OT_MPI_VB_H
#define OT_MPI_VB_H

#include "ot_common_vb.h"
#include "ot_common_video.h"

#ifdef __cplusplus
extern "C" {
#endif

ot_vb_pool ot_mpi_vb_create_pool(const ot_vb_pool_cfg *vb_pool_cfg);
ot_vb_pool ot_mpi_vb_create_ext_pool(const ot_vb_pool_cfg *vb_pool_cfg);
td_s32 ot_mpi_vb_destroy_pool(ot_vb_pool pool);

ot_vb_blk ot_mpi_vb_get_blk(ot_vb_pool pool, td_u64 blk_size, const td_char *mmz_name);
td_s32 ot_mpi_vb_release_blk(ot_vb_blk vb_blk);

ot_vb_blk ot_mpi_vb_insert_buf_to_ext_pool(ot_vb_pool pool, td_phys_addr_t phys_addr, td_u64 size, td_bool is_filled);
td_s32 ot_mpi_vb_delete_buf_from_ext_pool(ot_vb_blk block, td_s32 milli_sec);

ot_vb_blk ot_mpi_vb_phys_addr_to_handle(td_phys_addr_t phys_addr);
td_phys_addr_t ot_mpi_vb_handle_to_phys_addr(ot_vb_blk vb_blk);
ot_vb_pool ot_mpi_vb_handle_to_pool_id(ot_vb_blk vb_blk);

td_s32 ot_mpi_vb_inquire_user_cnt(ot_vb_blk vb_blk);

td_s32 ot_mpi_vb_get_supplement_addr(ot_vb_blk vb_blk, ot_video_supplement *supplement);

td_s32 ot_mpi_vb_set_supplement_cfg(const ot_vb_supplement_cfg *supplement_cfg);
td_s32 ot_mpi_vb_get_supplement_cfg(ot_vb_supplement_cfg *supplement_cfg);

td_s32 ot_mpi_vb_init(td_void);
td_s32 ot_mpi_vb_exit(td_void);

td_s32 ot_mpi_vb_set_cfg(const ot_vb_cfg *vb_cfg);
td_s32 ot_mpi_vb_get_cfg(ot_vb_cfg *vb_cfg);

td_s32 ot_mpi_vb_get_pool_info(ot_vb_pool pool, ot_vb_pool_info *pool_info);

td_s32 ot_mpi_vb_init_mod_common_pool(ot_vb_uid vb_uid);
td_s32 ot_mpi_vb_exit_mod_common_pool(ot_vb_uid vb_uid);

td_s32 ot_mpi_vb_set_mod_pool_cfg(ot_vb_uid vb_uid, const ot_vb_cfg *vb_cfg);
td_s32 ot_mpi_vb_get_mod_pool_cfg(ot_vb_uid vb_uid, ot_vb_cfg *vb_cfg);

#ifdef __cplusplus
}
#endif

#endif /* OT_MPI_VB_H */

