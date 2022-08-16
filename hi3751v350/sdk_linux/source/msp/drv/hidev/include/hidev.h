/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __HIDEV_H__
#define __HIDEV_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HIDEV_DEVICE_NAME "hi_dev"

enum {
    HIDEV_PM_SUSPEND,
    HIDEV_PM_RESUME,
    HIDEV_PM_LOWPOWER_ENTER,
    HIDEV_PM_LOWPOWER_EXIT,
    HIDEV_PM_POWEROFF,
    HIDEV_PM_CMD_MAX
};

struct hidev_pm_cmd {
    hi_u32 cmd;
};

struct hidev_block_mem {
    hi_ulong phys_addr;
    hi_ulong size;
};

#define HIDEV_GET_PM_CMD _IOR('P', 1, struct hidev_pm_cmd)
#define HIDEV_FREE_BLOCK_MEM _IOW('P', 2, struct hidev_block_mem)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
