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

#ifndef GFBG_PROC_H
#define GFBG_PROC_H

#include "ot_type.h"
#include "ot_osal.h"

#define GFBG_ENTRY_NAME_RANGE 32

typedef td_s32 (*gfbg_proc_show)(struct osal_proc_dir_entry *entry);

typedef td_s32 (*gfbg_proc_write)(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *);

typedef td_s32 (*gfbg_proc_ioctl)(unsigned int cmd, unsigned long arg, td_u32 *private_data);

typedef struct {
    td_char entry_name[GFBG_ENTRY_NAME_RANGE];
    struct osal_proc_dir_entry *entry;
    gfbg_proc_show show;
    gfbg_proc_write write;
    gfbg_proc_ioctl ioctl;
    td_void *data;
} gfbg_proc_item;

extern td_void gfbg_proc_add_module(const td_char *, gfbg_proc_show, gfbg_proc_write, gfbg_proc_ioctl, td_void *);
extern td_void gfbg_proc_remove_module(const td_char *);
extern td_void gfbg_proc_remove_all_module(td_void);

extern td_void gfbg_proc_init(td_void);

#endif /* GFBG_PROC_H */
