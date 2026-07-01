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


#ifndef _MMZ_COMM__
#define _MMZ_COMM__

#include <linux/version.h>
#include <linux/mm.h>
#include "osal_mmz.h"

#define error_mmz(s...)                                         \
    do {                                                        \
        osal_trace(KERN_ERR "mmz_userdev:%s: ", __FUNCTION__);  \
        osal_trace(s);                                          \
    } while (0)

#define warning(s...)                                               \
    do {                                                            \
        osal_trace(KERN_WARNING "mmz_userdev:%s: ", __FUNCTION__);  \
        osal_trace(s);                                              \
    } while (0)

unsigned long mmz_align_power2(unsigned long size);
ot_mmb_t *do_mmb_alloc(const ot_mmz_alloc_para_in *alloc_para, ot_mmz_t *fixed_mmz,
    unsigned long fixed_start, unsigned long size);
unsigned long _strtoul_ex(const char *s, char **ep, unsigned int base);


#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
static inline void mmz_map_down(struct mm_struct *mm)
{
    down_read(&mm->mmap_lock);
}

static inline void mmz_map_up(struct mm_struct *mm)
{
    up_read(&mm->mmap_lock);
}
#else
static inline void mmz_map_down(struct mm_struct *mm)
{
    down_read(&mm->mmap_sem);
}

static inline void mmz_map_up(struct mm_struct *mm)
{
    up_read(&mm->mmap_sem);
}
#endif

#endif
