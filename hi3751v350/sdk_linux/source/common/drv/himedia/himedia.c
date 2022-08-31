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
#include <linux/module.h>
#include <linux/version.h>
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
#include <linux/delay.h>

#include "himedia.h"
#include "himedia_base.h"


#define mkstr(exp)        #exp
#define mk_marco_to_str(exp) mkstr(exp)
#define VERSION_STRING    ("SDK_VERSION:[" mk_marco_to_str(SDK_VERSION) "] Build Time:[" __DATE__ ", " __TIME__ "]")

/*
 * Head entry for the doubly linked himedia_device list
 */
static LIST_HEAD(himedia_list);
static DEFINE_MUTEX(himedia_sem);

/*
 * Assigned numbers, used for dynamic minors
 */
#define DYNAMIC_MINORS 128 /* like dynamic majors */
static unsigned char g_himedia_minors[DYNAMIC_MINORS / 8]; /* 8 表示8为粒度 */

#ifdef CONFIG_PROC_FS
#if (1 == HI_PROC_SUPPORT)
static void *himedia_seq_start(struct seq_file *seq, loff_t *pos)
{
    mutex_lock(&himedia_sem);
    return seq_list_start(&himedia_list, *pos);
}

static void *himedia_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
    return seq_list_next(v, &himedia_list, pos);
}

static void himedia_seq_stop(struct seq_file *seq, void *v)
{
    mutex_unlock(&himedia_sem);
}

static int himedia_seq_show(struct seq_file *seq, void *v)
{
    const pm_device *p = list_entry(v, pm_device, list);
    if (p == NULL) {
        return -1;
    }
    seq_printf(seq, "%3i %s\n", p->minor, (char *)p->name ? (char *)p->name : "");
    return 0;
}

static struct seq_operations g_himedia_seq_ops = {
    .start = himedia_seq_start,
    .next = himedia_seq_next,
    .stop = himedia_seq_stop,
    .show = himedia_seq_show,
};

static int himedia_seq_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &g_himedia_seq_ops);
}

#if defined(LINUX_VERSION_CODE) && (LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0))
static struct file_operations g_himedia_proc_fops = {
    .owner = THIS_MODULE,
    .open = himedia_seq_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release,
};
#else
static struct proc_ops g_himedia_proc_fops = {
    .proc_open = himedia_seq_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = seq_release,
};
#endif
#endif
#endif

static int himedia_open(struct inode *inode, struct file *file)
{
    int minor = iminor(inode);
    pm_device *c = NULL;
    int err = -1;
    const struct file_operations *old_fops = NULL;
    const struct file_operations *new_fops = NULL;

    mutex_lock(&himedia_sem);

    list_for_each_entry(c, &himedia_list, list)
    {
        if (c->minor == minor) {
            new_fops = fops_get(c->app_ops);
            break;
        }
    }

    if (new_fops == NULL) {
        goto fail;
    }

    err = 0;
    old_fops = file->f_op;
    file->f_op = new_fops;
    if (file->f_op->open) {
        file->private_data = c->dev;
        err = file->f_op->open(inode, file);
        if (err) {
            fops_put(file->f_op);
            file->f_op = fops_get(old_fops);
        }
    }
    fops_put(old_fops);
fail:
    mutex_unlock(&himedia_sem);
    return err;
}

static struct class *g_himedia_class;

static struct file_operations g_himedia_fops = {
    .owner = THIS_MODULE,
    .open = himedia_open,
};

/**
 *    hi_drv_pm_register    -    register a himedia device
 *    @himedia: device structure
 *
 *    Register a himedia device with the kernel. If the minor
 *    number is set to %HIMEDIA_DYNAMIC_MINOR a minor number is assigned
 *    and placed in the minor field of the structure. For other cases
 *    the minor number requested is used.
 *
 *    The structure passed is linked into the kernel and may not be
 *    destroyed until it has been unregistered.
 *
 *    A zero is returned on success and a negative errno code for
 *    failure.
 */
static hi_s32 pm_register_get_media_minor(pm_device *himedia)
{
    pm_device *c = NULL;
    hi_s32 i = DYNAMIC_MINORS;

    list_for_each_entry(c, &himedia_list, list) {
        if (c->minor == himedia->minor) {
        return -EBUSY;
        }
    }
    if (himedia->minor == HIMEDIA_DYNAMIC_MINOR) {
        while (--i >= 0)
            if ((g_himedia_minors[(hi_u32)i >> 3] & (1 << ((hi_u32)i & 7))) == 0) { /* 3 1 7 表示偏移位数 */
                break;
            }

        if (i < 0) {
            return -EBUSY;
        }
        himedia->minor = i;
    }

    if (himedia->minor < DYNAMIC_MINORS) {
        g_himedia_minors[himedia->minor >> 3] |= 1 << (himedia->minor & 7);  /* 3 1 7 表示偏移位数 */
    }
    return HI_SUCCESS;
}

static hi_s32 pm_register_add_media_device(pm_device *himedia, pm_basedev **bdev)
{
    hi_s32 ret;

    if (bdev == NULL) {
        return HI_FAILURE;
    }

    *bdev = himedia_device_alloc(himedia->name, -1);
    if (*bdev == NULL) {
        return -ENOMEM;
    }
    ret = himedia_device_add(*bdev);
    if (ret) {
        himedia_device_put(*bdev);
        return ret;
    }
    return HI_SUCCESS;
}


static hi_s32 pm_register_device_create(pm_device *himedia,
    pm_basedev *bdev, dev_t *dev, struct device **adev)
{
    hi_s32 ret;

    *dev = MKDEV(HIMEDIA_DEVICE_MAJOR, himedia->minor);
    *adev = device_create(g_himedia_class, &(bdev->dev), *dev, NULL,
        "%s", himedia->name);
    if (IS_ERR(*adev)) {
        ret = PTR_ERR(*adev);
        return ret;
    }

    return HI_SUCCESS;
}


static hi_s32 pm_register_device_register(pm_device *himedia,
    struct device *adev, pm_basedev *bdev)
{
    pm_basedrv *bdrv = NULL;
    hi_s32 ret;

    bdrv = himedia_driver_alloc(himedia->name, himedia->owner, himedia->base_ops);
    if (bdrv == NULL) {
        return -ENOMEM;
    }

    ret = himedia_driver_register(bdrv);
    if (ret) {
        himedia_driver_release(bdrv);
        return ret;
    }

    himedia->app_device = adev;
    himedia->base_device = bdev;
    himedia->base_driver = bdrv;

    return HI_SUCCESS;
}
hi_s32 hi_drv_pm_register(pm_device *himedia)
{
    int ret;
    dev_t dev;
    struct device *adev = NULL;
    pm_basedev *bdev = NULL;

    if ((himedia == NULL) || (himedia->name == NULL) || (himedia->app_ops == NULL)) {
        return -EINVAL;
    }

#ifdef MODULE
    if (himedia->owner == NULL) {
        return -EINVAL;
    }
#endif
    mutex_lock(&himedia_sem);
    // 1
    ret = pm_register_get_media_minor(himedia);
    if (ret != HI_SUCCESS) {
        goto out;
    }

    // 2 base device, then class = NULL;
    ret = pm_register_add_media_device(himedia, &bdev);
    if (ret != HI_SUCCESS) {
        goto err0;
    }

    // 3 app class
    ret = pm_register_device_create(himedia, bdev, &dev, &adev);
    if (ret != HI_SUCCESS) {
        goto err1;
    }

    // 4 base driver
    ret = pm_register_device_register(himedia, adev, bdev);
    if (ret != HI_SUCCESS) {
        goto err2;
    }
    /*
     * Add it to the front, so that later devices can "override"
     * earlier defaults
     */
    INIT_LIST_HEAD(&himedia->list);
    list_add(&himedia->list, &himedia_list);
    goto out;

err2:
    device_destroy(g_himedia_class, dev);
err1:
    himedia_device_unregister(bdev);
err0:
    g_himedia_minors[himedia->minor >> 3] &= ~(1 << (himedia->minor & 7)); /* 3 1 7 表示偏移位数 */
out:
    mutex_unlock(&himedia_sem);
    return ret;
}

/**
 *    hi_drv_pm_un_register - unregister a himedia device
 *    @himedia: device to unregister
 *
 *    Unregister a himedia device that was previously
 *    successfully registered with hi_drv_pm_register(). Success
 *    is indicated by a zero return, a negative errno code
 *    indicates an error.
 */
hi_s32 hi_drv_pm_un_register(pm_device *himedia)
{
    hi_u32 i;

    if (g_himedia_class == NULL || himedia == NULL || (himedia->name == NULL) || (himedia->app_ops == NULL)) {
        return -EINVAL;
    }
#ifdef MODULE
    if (himedia->owner == NULL) {
        return -EINVAL;
    }
#endif
    i = himedia->minor;
    // 0
    if (list_empty(&himedia->list)) {
        return -EINVAL;
    }
    if ((i >= DYNAMIC_MINORS) || (i < 0)) {
        return -EINVAL;
    }
    // 1
    mutex_lock(&himedia_sem);
    // 1.0
    list_del(&himedia->list);
    // 1.1
    if (himedia->base_driver) {
        himedia_driver_unregister(himedia->base_driver);
        himedia_driver_release(himedia->base_driver);
        himedia->base_driver = NULL;
    }
    // 1.2
    if (himedia->app_device) {
        device_destroy(g_himedia_class, MKDEV(HIMEDIA_DEVICE_MAJOR, himedia->minor));
        himedia->app_device = NULL;
    }
    // 1.3
    if (himedia->base_device) {
        himedia_device_unregister(himedia->base_device);
        himedia->base_device = NULL;
    }
    // 1.4
    g_himedia_minors[i >> 3] &= ~(1 << (i & 7)); /* 3 1 7表示偏移位数 */
    mutex_unlock(&himedia_sem);
    return 0;
}

hi_s32 drv_pm_mod_init(hi_void)
{
    int ret;
    // 0
#if !(0 == HI_PROC_SUPPORT)
    proc_create("himedia", 0, NULL, &g_himedia_proc_fops);
#endif
    // 1
    ret = himedia_bus_init();
    if (ret) {
        goto err0;
    }
    // 2
    g_himedia_class = class_create(THIS_MODULE, "himedia_class");
    ret = PTR_ERR(g_himedia_class);
    if (IS_ERR(g_himedia_class)) {
        goto err1;
    }
    // 3
    ret = -EIO;
    if (register_chrdev(HIMEDIA_DEVICE_MAJOR, "himedia_char_dev", &g_himedia_fops)) {
        goto err2;
    }
#if defined(MODULE)
    printk("Load hi_media.ko success.\t(%s)\n", VERSION_STRING);
#endif
    return 0;
    // 4
err2:

    printk("!!! Module himedia: unable to get major %d for himedia devices\n", HIMEDIA_DEVICE_MAJOR);

    class_destroy(g_himedia_class);
err1:
    himedia_bus_exit();
err0:
#if !(0 == HI_PROC_SUPPORT)
    remove_proc_entry("himedia", NULL);
#endif
    return ret;
}

hi_void drv_pm_mod_exit(hi_void)
{
    // 0
    if (list_empty(&himedia_list) == 0) {
        return;
    }
    // 1
    unregister_chrdev(HIMEDIA_DEVICE_MAJOR, "himedia");
    // 2
    class_destroy(g_himedia_class);
    // 3
    himedia_bus_exit();
    // 4
#if !(0 == HI_PROC_SUPPORT)
    remove_proc_entry("himedia", NULL);
#endif
#if defined(MODULE)
    printk("remove hi_media.ko success.\n");
#endif
    return;
}

#ifdef MODULE
module_init(drv_pm_mod_init);
module_exit(drv_pm_mod_exit);
#else
#ifdef CFG_HI_USER_DRV
subsys_initcall(drv_pm_mod_init);
module_exit(drv_pm_mod_exit);
#endif
#endif

EXPORT_SYMBOL(hi_drv_pm_register);
EXPORT_SYMBOL(hi_drv_pm_un_register);
EXPORT_SYMBOL(drv_pm_mod_init);
EXPORT_SYMBOL(drv_pm_mod_exit);

MODULE_LICENSE("GPL");
