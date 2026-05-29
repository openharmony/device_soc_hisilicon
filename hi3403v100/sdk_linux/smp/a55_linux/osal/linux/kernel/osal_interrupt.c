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

#include "ot_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/interrupt.h>

int osal_request_irq(unsigned int irq, osal_irq_handler_t handler, osal_irq_handler_t thread_fn, const char *name,
                     void *dev)
{
    unsigned long flags = IRQF_SHARED;

    return request_threaded_irq(irq, (irq_handler_t)handler, (irq_handler_t)thread_fn, flags, name, dev);
}
EXPORT_SYMBOL(osal_request_irq);
void osal_free_irq(unsigned int irq, void *dev)
{
    free_irq(irq, dev);
}
EXPORT_SYMBOL(osal_free_irq);

int osal_in_interrupt(void)
{
    return in_interrupt();
}
EXPORT_SYMBOL(osal_in_interrupt);
