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

#ifndef SYS_H
#define SYS_H

#include "ot_type.h"
#include "ot_common.h"
#include "valg_plat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    td_s32 *ddr_data_buff;
    td_s32 buff_len;
    td_s32 value_count;
} ddr_occupy;

typedef struct {
    osal_semaphore_t sem;
    osal_atomic_t user_ref;
    td_u32 state;
    ot_mpp_sys_cfg sys_cfg;
    td_s32 time_zone;
} sys_context;

typedef struct {
    struct osal_list_head list;
    td_char mmz_name[OT_MAX_MMZ_NAME_LEN];
} sys_mem_node;

typedef struct {
    td_char *mmz_name;
} sys_mod_chn_mmz;

typedef struct {
    td_u32 max_dev_cnt;
    td_u32 max_chn_cnt;

    sys_mod_chn_mmz *mmz_tbl;
} sys_mem_ctx;

typedef struct {
    td_s32 set_value; /* customer value meaning: whether check is ok. */
    td_u32 set_count; /* customer set count */

    td_s32 connect_status; /* 0: not ok, 1: ok, other: reserve */
} sys_customer_check;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
