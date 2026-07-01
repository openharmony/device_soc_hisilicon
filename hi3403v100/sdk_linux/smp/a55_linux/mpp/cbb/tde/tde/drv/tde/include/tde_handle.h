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
#ifndef TDE_HANDLE_H
#define TDE_HANDLE_H

/*********************************add include here**********************************************/

#include "tde_osilist.h"
#include "tde_define.h"
#include "osal_list.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TDE_MAX_HANDLE_VALUE 0x7fffffff

typedef struct {
    struct osal_list_head list_head;
    osal_spinlock_t lock;
    td_s32 handle;
    td_void *res;
} tde_handle_mgr;

td_bool initial_handle(td_void);
td_void get_handle(tde_handle_mgr *res, td_s32 *handle);
td_bool tde_query_handle(td_s32 handle, tde_handle_mgr **res);
td_bool release_handle(td_s32 handle);
td_void destroy_handle(td_void);

tde_handle_mgr *tde_get_handle_list(td_void);
td_void tde_lock_handle_list(td_ulong *lockflags);
td_void tde_unlock_handle_list(td_ulong *lockflags);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TDE_HANDLE_H */
