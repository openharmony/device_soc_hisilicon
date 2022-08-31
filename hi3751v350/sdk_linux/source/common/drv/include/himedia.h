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

#ifndef __HIMEDIA_H__
#define __HIMEDIA_H__

#include <asm/types.h>
#include <asm/atomic.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/major.h>
#include <linux/fs.h>


#include "hi_type.h"
#include "hi_module.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

#define HIMEDIA_DEVICE_NAME_MAX_LEN 64

#define HIMEDIA_DEVICE_MAJOR  218
#define HIMEDIA_DYNAMIC_MINOR 255

typedef struct tag_pm_basedev {
    HI_S32 id;
    const HI_S8 *name;
    struct device dev;
} pm_basedev;

#define TO_PM_BASEDEV(x) container_of((x), pm_basedev, dev)

typedef struct tag_pm_baseops {
    HI_S32 (*probe)(pm_basedev *);
    HI_S32 (*remove)(pm_basedev *);
    HI_VOID (*shutdown)(pm_basedev *);
    HI_S32 (*prepare)(pm_basedev *);
    HI_VOID (*complete)(pm_basedev *);
    HI_S32 (*suspend)(pm_basedev *, pm_message_t state);
    HI_S32 (*suspend_late)(pm_basedev *, pm_message_t state);
    HI_S32 (*resume_early)(pm_basedev *);
    HI_S32 (*resume)(pm_basedev *);
} pm_baseops;

typedef struct tag_pm_basedrv {
    HI_S32(*probe)(pm_basedev *);
    HI_S32 (*remove)(pm_basedev *);
    HI_VOID (*shutdown)(pm_basedev *);
    HI_S32 (*suspend)(pm_basedev *, pm_message_t state);
    HI_S32 (*suspend_late)(pm_basedev *, pm_message_t state);
    HI_S32 (*resume_early)(pm_basedev *);
    HI_S32 (*resume)(pm_basedev *);
    struct device_driver driver;
} pm_basedrv;

#define to_himedia_basedrv(drv) container_of((drv), pm_basedrv, driver)

typedef struct hi_umap_device {
    HI_CHAR devfs_name[HIMEDIA_DEVICE_NAME_MAX_LEN]; /* devfs */
    HI_S32 minor;
    struct module *owner;
    struct file_operations *fops;
    pm_baseops *drvops;
} umap_device, *ptr_umap_device;

typedef struct tag_pm_device {
    HI_U32 minor;
    const HI_S8 *name;
    struct module *owner;
    const struct file_operations *app_ops;
    pm_baseops *base_ops;
    struct list_head list;
    struct device *app_device;
    pm_basedev *base_device;
    pm_basedrv *base_driver;
    umap_device *umap_dev;
    void *dev;
} pm_device;


HI_S32 drv_pm_mod_init(HI_VOID);
HI_VOID drv_pm_mod_exit(HI_VOID);
HI_S32 hi_drv_pm_register(pm_device *);
HI_S32 hi_drv_pm_un_register(pm_device *);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif /* __HIMEDIA_H__ */

