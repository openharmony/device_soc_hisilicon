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
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/version.h>
#include "securec.h"
#include "base.h"
#include "ot_osal.h"

static void ot_media_bus_release(struct device *dev)
{
    osal_unused(dev);
    return;
}

struct device g_ot_media_bus = {
    .init_name = "ot_media",
    .release = ot_media_bus_release
};

/* bus match & uevent */
static int ot_media_match(struct device *dev, struct device_driver *drv)
{
    struct ot_media_device *pdev = to_ot_media_device(dev);
    return (strncmp(pdev->devfs_name, drv->name, sizeof(pdev->devfs_name)) == 0);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
static int ot_media_uevent(struct device *dev, struct kobj_uevent_env *env)
#else
static int ot_media_uevent(const struct device *dev, struct kobj_uevent_env *env)
#endif
{
    struct ot_media_device *pdev = to_ot_media_device(dev);
    add_uevent_var(env, "MODALIAS=ot_media:%s", pdev->devfs_name);
    return 0;
}

struct bus_type g_ot_media_bus_type = {
    .name = "ot_media",
    .match = ot_media_match,
    .uevent = ot_media_uevent,
};

int ot_media_bus_init(void)
{
    int ret;
    ret = device_register(&g_ot_media_bus);
    if (ret) {
        return ret;
    }

    ret = bus_register(&g_ot_media_bus_type);
    if (ret) {
        goto error;
    }

    return 0;
error:
    device_unregister(&g_ot_media_bus);
    return ret;
}

void ot_media_bus_exit(void)
{
    bus_unregister(&g_ot_media_bus_type);
    device_unregister(&g_ot_media_bus);
}

static void ot_media_device_release(struct device *dev)
{
    osal_unused(dev);
    return;
}

int ot_media_device_register(struct ot_media_device *pdev)
{
    if (pdev == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    dev_set_name(&pdev->device, "%s", pdev->devfs_name);

    pdev->device.devt = MKDEV(OT_MEDIA_DEVICE_MAJOR, pdev->minor);
    pdev->device.release = ot_media_device_release;
    pdev->device.bus = &g_ot_media_bus_type;

    return device_register(&pdev->device);
}

void ot_media_device_unregister(struct ot_media_device *pdev)
{
    if (pdev == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    device_unregister(&pdev->device);
}

struct ot_media_driver *ot_media_driver_register(const char *name, struct module *owner)
{
    int ret;
    struct ot_media_driver *pdrv = NULL;

    if ((name == NULL) || (owner == NULL)) {
        return ERR_PTR(-EINVAL);
    }

    pdrv = kzalloc(sizeof(struct ot_media_driver) + strnlen(name, OT_MEDIA_MAX_DEV_NAME_LEN), GFP_KERNEL);
    if (pdrv == NULL) {
        return ERR_PTR(-ENOMEM);
    }

    /* init driver object */
    if (strncpy_s(pdrv->name, OT_MEDIA_MAX_DEV_NAME_LEN, name, strnlen(name, OT_MEDIA_MAX_DEV_NAME_LEN - 1)) != EOK) {
        kfree(pdrv);
        pdrv = NULL;
        return ERR_PTR(-EINVAL);
    }
    pdrv->name[OT_MEDIA_MAX_DEV_NAME_LEN - 1] = '\0';

    pdrv->driver.name = pdrv->name;
    pdrv->driver.owner = owner;
    pdrv->driver.bus = &g_ot_media_bus_type;

    ret = driver_register(&pdrv->driver);
    if (ret) {
        kfree(pdrv);
        pdrv = NULL;
        return ERR_PTR(ret);
    }

    return pdrv;
}

void ot_media_driver_unregister(struct ot_media_driver *pdrv)
{
    if (pdrv != NULL) {
        driver_unregister(&pdrv->driver);
        kfree(pdrv);
        pdrv = NULL;
    }
}
