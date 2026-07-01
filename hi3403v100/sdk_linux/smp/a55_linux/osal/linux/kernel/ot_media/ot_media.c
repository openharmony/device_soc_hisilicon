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
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>

#include "base.h"

static OSAL_LIST_HEAD(ot_media_list);
static DEFINE_MUTEX(ot_media_sem);

/*
 * Assigned numbers, used for dynamic minors
 */
#define DYNAMIC_MINORS 128 /* like dynamic majors */
#define DYNAMIC_MINORS_TO_BYTES (DYNAMIC_MINORS / 8)
static unsigned char g_ot_media_minors[DYNAMIC_MINORS_TO_BYTES]; /* 8: bitmap 1Byte has 8 bit */

static int ot_media_open(struct inode *inode, struct file *file)
{
    unsigned int minor = iminor(inode);
    struct ot_media_device *c = NULL;
    int err = -ENODEV;
    const struct file_operations *old_fops = NULL;
    const struct file_operations *new_fops = NULL;

    mutex_lock(&ot_media_sem);

    osal_list_for_each_entry(c, &ot_media_list, list) {
        if (c->minor == (unsigned int)minor) {
            new_fops = fops_get(c->fops);
            break;
        }
    }

    if (new_fops == NULL) {
        mutex_unlock(&ot_media_sem);
        request_module("char-major-%d-%u", OT_MEDIA_DEVICE_MAJOR, minor);
        mutex_lock(&ot_media_sem);

        osal_list_for_each_entry(c, &ot_media_list, list) {
            if (c->minor == (unsigned int)minor) {
                new_fops = fops_get(c->fops);
                break;
            }
        }

        if (new_fops == NULL) {
            goto fail;
        }
    }

    err = 0;

    old_fops = file->f_op;
    file->f_op = new_fops;
    if (file->f_op->open) {
        file->private_data = c;
        err = file->f_op->open(inode, file);
        if (err) {
            fops_put(file->f_op);
            file->private_data = NULL;
            file->f_op = fops_get(old_fops);
        }
    }

    fops_put(old_fops);
fail:
    mutex_unlock(&ot_media_sem);
    return err;
}

static struct file_operations g_ot_media_fops = {
    .owner = THIS_MODULE,
    .open = ot_media_open,
};

int ot_media_search_minors(struct ot_media_device *ot_media)
{
    int i = DYNAMIC_MINORS;
    int index;

    while (--i >= 0) {
        index = (unsigned int)i >> 3;  /* 3: left shift 3bit to locate the index of char array */
        if (index >= DYNAMIC_MINORS_TO_BYTES) {
            return -1;
        }
        if ((g_ot_media_minors[index] &
            (1 << ((unsigned int)i & 7))) == 0) { /* 7: locate the bit in bitmap */
            break;
        }
    }
    if (i < 0) {
        return -1;
    }
    ot_media->minor = i;

    return 0;
}

/*
 * ot_media_register - register a ot_media device
 * @ot_media: device structure
 *
 * Register a ot_media device with the kernel. If the minor
 * number is set to %OT_MEDIA_DYNAMIC_MINOR a minor number is assigned
 * and placed in the minor field of the structure. For other cases
 * the minor number requested is used.
 *
 * The structure passed is linked into the kernel and may not be
 * destroyed until it has been unregistered.
 *
 * A zero is returned on success and a negative errno code for
 * failure.
 */
int ot_media_register(struct ot_media_device *ot_media)
{
    struct ot_media_device *ptmp = NULL;
    struct ot_media_driver *pdrv = NULL;
    int err;

    if ((ot_media == NULL) || (ot_media->owner == NULL)) {
        return -EBUSY;
    }
    mutex_lock(&ot_media_sem);

    /* check if registered */
    osal_list_for_each_entry(ptmp, &ot_media_list, list) {
        if (ptmp->minor == ot_media->minor) {
            mutex_unlock(&ot_media_sem);
            return -EBUSY;
        }
    }

    /* check minor */
    if (ot_media->minor == OT_MEDIA_DYNAMIC_MINOR) {
        if (ot_media_search_minors(ot_media) != 0) {
            mutex_unlock(&ot_media_sem);
            return -EBUSY;
        }
    }

    if (ot_media->minor < DYNAMIC_MINORS) {
        g_ot_media_minors[ot_media->minor >> 3] |= 1 << (ot_media->minor & 7); /* 3, 7: bitmap write set bit 1 */
    }

    /* device register */
    err = ot_media_device_register(ot_media);
    if (err < 0) {
        g_ot_media_minors[ot_media->minor >> 3] &= ~(1 << (ot_media->minor & 7)); /* 3, 7: bitmap write set bit 0 */
        goto out;
    }

    /* driver register */
    pdrv = ot_media_driver_register(ot_media->devfs_name, ot_media->owner);
    if (IS_ERR(pdrv)) {
        ot_media_device_unregister(ot_media);
        g_ot_media_minors[ot_media->minor >> 3] &= ~(1 << (ot_media->minor & 7)); /* 3, 7: bitmap write set bit 0 */
        err = PTR_ERR(pdrv);
        goto out;
    }

    ot_media->driver = pdrv;

    /*
     * Add it to the front, so that later devices can "override"
     * earlier defaults
     */
    osal_list_add(&ot_media->list, &ot_media_list);

out:
    mutex_unlock(&ot_media_sem);
    return err;
}
EXPORT_SYMBOL(ot_media_register);

/*
 * ot_media_unregister - unregister a ot_media device
 * @ot_media: device to unregister
 *
 * Unregister a ot_media device that was previously
 * successfully registered with ot_media_register(). Success
 * is indicated by a zero return, a negative errno code
 * indicates an error.
 */
int ot_media_unregister(struct ot_media_device *ot_media)
{
    struct ot_media_device *ptmp = NULL;
    struct ot_media_device *_ptmp = NULL;

    if (ot_media == NULL) {
        return -EINVAL;
    }

    if (osal_list_empty(&ot_media->list)) {
        return -EINVAL;
    }

    mutex_lock(&ot_media_sem);

    osal_list_for_each_entry_safe(ptmp, _ptmp, &ot_media_list, list) {
        /* if found, unregister device & driver */
        if (ptmp->minor == ot_media->minor) {
            osal_list_del(&ot_media->list);

            ot_media_driver_unregister(ot_media->driver);

            ot_media->driver = NULL;

            ot_media_device_unregister(ot_media);

            g_ot_media_minors[ot_media->minor >> 3] &= ~(1 << (ot_media->minor & 7)); /* 3, 7: bitmap write set bit 0 */

            break;
        }
    }

    mutex_unlock(&ot_media_sem);

    return 0;
}
EXPORT_SYMBOL(ot_media_unregister);

int ot_media_init(void)
{
    int ret;
    /* 1 */
    ret = ot_media_bus_init();
    if (ret) {
        goto err0;
    }

    /* 2 */
    ret = -EIO;

    if (register_chrdev(OT_MEDIA_DEVICE_MAJOR, "ot_media", &g_ot_media_fops)) {
        goto err1;
    }

    printk("Module ot_media: init ok\n");

    return 0;

    /* 3 */
err1:
    ot_media_bus_exit();
err0:
    return ret;
}

void ot_media_exit(void)
{
    /* 0 */
    if (!osal_list_empty(&ot_media_list)) {
        printk("!!! Module ot_media: sub module in list\n");
        return;
    }

    unregister_chrdev(OT_MEDIA_DEVICE_MAJOR, "ot_media");

    ot_media_bus_exit();

    printk("!!! Module ot_media: exit ok\n");
}
