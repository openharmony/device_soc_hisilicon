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

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/list.h>
#include <asm/cacheflush.h>
#include <linux/version.h>
#include "securec.h"
#include "allocator.h"
#include "media_mem.h"

unsigned long mmz_align_power2(unsigned long size)
{
    unsigned int i;
    unsigned long tmp_size = size;
    if ((tmp_size - 1) & tmp_size) {
        for (i = 1; i < 32; i++) { /* 32: the max size is 2^(32-1) */
            if (!((tmp_size >> i) & ~0)) {
                tmp_size = 1 << i;
                break;
            }
        }
    }
    return tmp_size;
}

static int add_mmb_to_list(ot_mmb_t* mmb)
{
    ot_mmb_t* p = NULL;

    /* add mmb sorted */
    osal_list_for_each_entry(p, &mmb->zone->mmb_list, list)
    {
        if (mmb->phys_addr < p->phys_addr)
            break;
        if (mmb->phys_addr == p->phys_addr)
            osal_trace(KERN_ERR "ERROR:cma allocator bad in %s(%s, %d)",
                    mmb->zone->name,  __func__, __LINE__);
    }
    osal_list_add(&mmb->list, p->list.prev);

    mmz_trace(1, OT_MMB_FMT_S, ot_mmb_fmt_arg(mmb));

    return 0;
}

ot_mmb_t *do_mmb_alloc(const ot_mmz_alloc_para_in *alloc_para, ot_mmz_t *fixed_mmz,
    unsigned long fixed_start, unsigned long size)
{
    ot_mmb_t *mmb = NULL;
    errno_t err_value;
    const char *name = alloc_para->buf_name;
    int i;

    mmb = kmalloc(sizeof(ot_mmb_t), GFP_KERNEL);
    if (mmb == NULL) {
        return NULL;
    }

    (void)memset_s(mmb, sizeof(ot_mmb_t), 0, sizeof(ot_mmb_t));

    mmb->zone      = fixed_mmz;
    mmb->phys_addr = fixed_start;
    mmb->length    = size;
    mmb->order     = alloc_para->order;

    mmb->kernel_only = alloc_para->kernel_only;
    if (media_mem_is_check_pid()) {
        mmb->check_pid = 1;
        mmb->pid_num   = 1;
        mmb->pid[0]    = osal_get_current_tgid();
        for (i = 1; i < OT_MMB_MAX_PID_NUM; ++i) {
            mmb->pid[i] = -1;
        }
    }

    if (name != NULL) {
        err_value = strncpy_s(mmb->name, OT_MMB_NAME_LEN, name, OT_MMB_NAME_LEN - 1);
    } else {
        err_value = strncpy_s(mmb->name, OT_MMB_NAME_LEN, "<null>", OT_MMB_NAME_LEN - 1);
    }

    if ((err_value != EOK) || add_mmb_to_list(mmb)) {
        kfree(mmb);
        return NULL;
    }

    return mmb;
}

unsigned long _strtoul_ex(const char *s, char **ep, unsigned int base)
{
    char *_end_p = NULL;
    unsigned long value;

    value = simple_strtoul(s, &_end_p, base);

    switch (*_end_p) {
        case 'm':
            fallthrough;
        case 'M':
            value <<= 10; /* 10: 1M=1024k, left shift 10bit */
            fallthrough;
        case 'k':
            fallthrough;
        case 'K':
            value <<= 10; /* 10: 1K=1024Byte, left shift 10bit */
            if (ep != NULL) {
                (*ep) = _end_p + 1;
            }
            fallthrough;
        default:
            break;
    }

    return value;
}
