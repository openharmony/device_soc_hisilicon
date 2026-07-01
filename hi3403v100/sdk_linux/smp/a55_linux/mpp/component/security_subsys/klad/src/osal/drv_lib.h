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

#ifndef DRV_LIB_H
#define DRV_LIB_H

#include "ot_type.h"

td_s32 klad_get_cpu_secure_sta(td_void);

td_bool klad_is_secure_cpu(td_void);

td_u32 ot_klad_get_klad_irq(td_void);

td_u32 ot_klad_get_rkp_irq(td_void);

extern int check_otp_cmd_mode(void);

/* must match kernel dts */
#define RKP_INT_NAME        (klad_is_secure_cpu() ? "sec_rkp" : "nsec_rkp")

#define KLAD_INT_NAME       (klad_is_secure_cpu() ? "sec_klad" : "nsec_klad")

#endif /* DRV_LIB_H */
