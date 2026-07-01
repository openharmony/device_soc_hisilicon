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


#ifndef __OT_IRQ_USER_H__
#define __OT_IRQ_USER_H__

#include <linux/types.h>
#include "ot_type.h"
#include "ot_osal.h"
#include "ot_debug.h"
#include "ot_comm_irq.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ot_irq_err_trace(fmt, ...) \
    OT_ERR_TRACE(OT_ID_IRQ, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define ot_irq_warn_trace(fmt, ...) \
    OT_WARN_TRACE(OT_ID_IRQ, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define ot_irq_debug_trace(fmt, ...) \
    OT_DEBUG_TRACE(OT_ID_IRQ, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

int ot_irq_request_irq(unsigned int irq, osal_irq_handler_t handler, osal_irq_handler_t thread_fn, const char *name,
    ot_irq_arg *dev);
void ot_irq_free_irq(unsigned int irq, ot_irq_arg *dev);
int ot_irq_in_interrupt(void);
int ot_irq_platform_get_irq(ot_irq_arg *dev, unsigned int num);
int ot_irq_platform_get_irq_byname(ot_irq_arg *dev, const char *name);
void *ot_irq_mmapdev(void *addr, unsigned long length, int prot, int flags, int fd, unsigned long offset);
int ot_irq_platform_set_irq_reg(unsigned int irq_num, td_int_reg_info *reg_info);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __OT_IRQ_USER_H__ */
