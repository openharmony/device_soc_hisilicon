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

#ifndef __MEDIA_MEM_H__
#define __MEDIA_MEM_H__

#include "osal_mmz.h"

ot_mmb_t *media_mem_get_mmb_by_handle(const void *mem_handle);
ot_mmb_t *media_mem_get_mmb_by_phys(unsigned long phys_addr);
ot_mmb_t *media_mem_get_mmb_by_phys_and_size(unsigned long phys_addr, unsigned long size);
void media_mem_put_mmb(ot_mmb_t *mmb);

void media_mem_clear_pid_in_all_mmb(int pid);

/* function returns the index of pid array */
static inline int find_pid_in_mmb(const ot_mmb_t *mmb, int pid)
{
    int i;
    for (i = 0; i < mmb->pid_num; ++i) {
        if (mmb->pid[i] == pid) {
            return i;
        }
    }
    return -1;
}

static inline void clear_pid_in_mmb(ot_mmb_t *mmb, int pos)
{
    for (mmb->pid_num--; pos < mmb->pid_num; ++pos) {
        mmb->pid[pos] = mmb->pid[pos + 1];
    }
    mmb->pid[mmb->pid_num] = -1;
}

bool media_mem_is_check_pid(void);

#endif
