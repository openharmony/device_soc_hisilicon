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

#ifndef DRV_KS_H
#define DRV_KS_H

#include "ot_type.h"
#include "ot_common_cipher.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 drv_kc_slot_init(td_void);

td_void drv_kc_slot_deinit(td_void);

td_s32 drv_kc_slot_lock(const ot_keyslot_attr *attr, td_u32 *slot_num);

td_s32 drv_kc_slot_unlock(td_u32 slot_num);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* DRV_KS_H */
