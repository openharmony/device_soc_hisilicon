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

#ifndef _LINUX_OT_MEDIA_DEVICE_H_
#define _LINUX_OT_MEDIA_DEVICE_H_

#include <linux/module.h>
#include <linux/major.h>
#include <linux/device.h>
#include "osal_list.h"

#define OT_MEDIA_DEVICE_MAJOR     218
#define OT_MEDIA_DYNAMIC_MINOR    255

struct ot_media_device;

#define OT_MEDIA_MAX_DEV_NAME_LEN 32

struct ot_media_driver {
    struct device_driver driver;
    char name[OT_MEDIA_MAX_DEV_NAME_LEN];
};

#define to_ot_media_driver(drv) \
    container_of((drv), struct ot_media_driver, driver)

struct ot_media_device {
    struct osal_list_head list;

    char devfs_name[OT_MEDIA_MAX_DEV_NAME_LEN];

    unsigned int minor;

    struct device device;

    struct module *owner;

    const struct file_operations *fops;

    /* for internal use */
    struct ot_media_driver *driver;
};

#define to_ot_media_device(dev) \
    container_of((dev), struct ot_media_device, device)

int ot_media_register(struct ot_media_device *pdev);

int ot_media_unregister(struct ot_media_device *pdev);

#endif /* _LINUX_OT_MEDIA_DEVICE_H_ */
