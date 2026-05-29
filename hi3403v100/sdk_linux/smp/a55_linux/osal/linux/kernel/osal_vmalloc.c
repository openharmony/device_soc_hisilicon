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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include "ot_osal.h"

void *osal_vmalloc(unsigned long size)
{
    if (size == 0) {
        return NULL;
    }
    return vmalloc(size);
}
EXPORT_SYMBOL(osal_vmalloc);

void osal_vfree(const void *addr)
{
    vfree(addr);
}
EXPORT_SYMBOL(osal_vfree);

void *osal_kmalloc(unsigned long size, unsigned int osal_gfp_flag)
{
    if (size == 0) {
        return NULL;
    }
    if (osal_gfp_flag == osal_gfp_kernel) {
        return kmalloc(size, GFP_KERNEL);
    } else if (osal_gfp_flag == osal_gfp_atomic) {
        return kmalloc(size, GFP_ATOMIC);
    } else {
        return NULL;
    }
}
EXPORT_SYMBOL(osal_kmalloc);

void osal_kfree(const void *addr)
{
    kfree(addr);
}

EXPORT_SYMBOL(osal_kfree);
