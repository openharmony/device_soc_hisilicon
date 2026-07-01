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

#ifndef DRV_KLAD_SW_UTILS_H
#define DRV_KLAD_SW_UTILS_H

#include "ot_common_klad.h"

/* task_tgid_nr(current): Thread group ID
 * current->pid         : Process ID
 */
#if !defined(__LITEOS__)
#define CRYPTO_OWNER                        pid_t
#define crypto_get_owner(x)                 *(x) = task_tgid_nr(current)
#else
#define CRYPTO_OWNER                        td_u32
#define crypto_get_owner(x)                 *(x) = 0
#endif

/* content key: set n stage common route klad */
typedef struct {
    ot_klad_alg_type alg;           /* klad algorithm */
    td_u32 key_size;                /* klad key size */
    td_u8 key[OT_KLAD_MAX_KEY_LEN]; /* klad key */
} klad_common_key;

typedef struct {
    td_u32 klad_level;
    ot_klad_rootkey_attr key_attr;
    ot_klad_crypto_alg crypto_alg;
    klad_common_key klad_key[OT_KLAD_LEVEL_BUTT];
} klad_common_slot;

typedef struct {
    ot_klad_clear_key clear_key;
} klad_clear_slot;

typedef struct {
    td_handle klad;
    td_handle target;
    td_bool open;
    td_bool attach;
    td_bool config;
    ot_klad_type type;
    CRYPTO_OWNER owner;                 /* user ID */
    union {
        klad_common_slot common_slot;
        klad_clear_slot clear_slot;
    } klad_slot;
} klad_sw_mgmt;

td_s32 klad_sw_init(td_void);
td_void klad_sw_deinit(td_void);
td_s32 klad_sw_create(td_handle *klad);
td_s32 klad_sw_destroy(td_handle klad);
td_s32 klad_sw_attach(td_handle klad, td_handle target);
td_s32 klad_sw_detach(td_handle klad, td_handle target);
td_s32 klad_sw_set_attr(td_handle klad, const ot_klad_attr *attr);
td_s32 klad_sw_get_attr(td_handle klad, ot_klad_attr *attr);
td_s32 klad_sw_set_session_key(td_handle klad, const ot_klad_session_key *key);
td_s32 klad_sw_set_content_key(td_handle klad, const ot_klad_content_key *key);
td_s32 klad_sw_set_clear_key(td_handle klad, const ot_klad_clear_key *key);
td_void klad_sw_release(td_void);

#endif /* DRV_KLAD_SW_UTILS_H */
