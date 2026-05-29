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

#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <asm/io.h>

#include "ot_debug.h"
#include "ot_osal.h"
#include "securec.h"
#include "drv_osal_chip.h"
#include "drv_cipher_kapi.h"
#include "drv_cipher_debug.h"

#define crypto_ioremap_nocache(addr, size)  osal_ioremap_nocache(addr, size)
#define crypto_iounmap(addr, size)          osal_iounmap((td_void *)(addr), size)

#define crypto_read(addr)                   readl((volatile td_void *)(addr))
#define crypto_write(addr, val)             writel(val, (volatile td_void *)(addr))

#define crypto_msleep(msec)                 osal_msleep(msec)
#define crypto_udelay(msec)                 osal_udelay(msec)

#define MAX_MALLOC_BUF_SIZE                 0x10000
td_void *crypto_calloc(size_t n, size_t size);
#define crypto_malloc(x)                    ((x) > 0 ? osal_kmalloc((x), osal_gfp_kernel) : TD_NULL)
#define crypto_free(x)              \
    do {                            \
        if ((x) != TD_NULL) {       \
            osal_kfree((x));        \
            x = TD_NULL;            \
        }                           \
    } while (0)

#define CRYPTO_QUEUE_HEAD                   osal_wait_t
#define crypto_queue_init(x)                osal_wait_init(x)
#define crypto_queue_wait_up(x)             osal_wakeup(x)
#define crypto_queue_deinit(x)              osal_wait_destroy(x)

#define crypto_queue_wait_timeout(head, func, param, time) \
    osal_wait_event_timeout_uninterruptible(head, func, param, time)

#define crypto_request_irq(irq, func, name) \
    osal_request_irq(irq, (td_void *)(func), TD_NULL, name, (td_void *)(name))

#define crypto_free_irq(irq, name)          \
    osal_free_irq(irq, (td_void *)(name))

#define CRYPTO_MUTEX                        osal_semaphore_t
#define crypto_mutex_init(x)                osal_sema_init(x, 1)
#define crypto_mutex_lock(x)                osal_down(x)
#define crypto_mutex_unlock(x)              osal_up(x)
#define crypto_mutex_destroy(x)             osal_sema_destroy(x)

/* task_tgid_nr(current): Thread group ID
 * current->pid         : Process ID
 */
#if !defined(__LITEOS__)
#define CRYPTO_OWNER                        pid_t
#define crypto_get_owner(x)                 *(x) = task_tgid_vnr(current)
#else
#define CRYPTO_OWNER                        td_u32
#define crypto_get_owner(x)                 *(x) = 0
#endif

#define CRYPTO_IRQRETURN_T                  enum osal_irqreturn

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
#define OT_PROC_SUPPORT
#define CRYPTO_PROC_PRINT                   osal_seq_printf
#endif

#define crypto_flush_dcache_all()

#define CIPHER_CHECK_MMZ_PHY

#endif /* DRV_OSAL_INIT_H */
