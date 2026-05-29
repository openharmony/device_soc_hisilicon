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

#ifndef DRV_OSAL_INIT_H
#define DRV_OSAL_INIT_H

#include <linux/delay.h>
#include <asm/io.h>
#include "ot_osal.h"

#define OTP_MUTEX_T                         osal_mutex_t
#define otp_mutex_init(x)                   osal_mutex_init(x)
#define otp_mutex_lock(x)                   osal_mutex_lock(x)
#define otp_mutex_unlock(x)                 osal_mutex_unlock(x)
#define otp_mutex_destroy(x)                osal_mutex_destroy(x)

#define otp_ioremap_nocache(addr, size)     osal_ioremap_nocache(addr, size)
#define otp_iounmap(addr, size)             osal_iounmap(addr, size)

#define otp_malloc(x)                       osal_vmalloc(x)
#define otp_free(x)                         \
    do {                                    \
        if (x) {                            \
            osal_vfree(x);                  \
            x = TD_NULL;                    \
        }                                   \
    } while (0)

#define otp_write(addr, data)               writel(data, addr)
#define otp_read(addr)                      readl(addr)

#define otp_udelay(x)                       osal_udelay(x)

#endif /* DRV_OSAL_INIT_H */
