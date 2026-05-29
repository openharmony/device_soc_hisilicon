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
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/kmod.h>
#include "securec.h"
#include "ot_media.h"
#include "ot_osal.h"
#include "osal_ioctl.h"

#define DRVAL_DEBUG    0
#define ARG_BUF_TEMP_SIZE 256

#define get_file_return(file)     \
    do {                          \
        if (osal_get_file_ref(file) < 0) \
            return (-1);          \
    } while (0)

#define put_file_return(file)     \
    do {                          \
        if (osal_put_file_ref(file) < 0) \
            return (-1);          \
    } while (0)

typedef struct osal_coat_dev {
    struct osal_dev osal_dev;
    struct ot_media_device ot_media_dev;
} osal_coat_dev_t;

struct osal_private_data {
    struct osal_dev *dev;
    void *data;
    struct osal_poll table;
    int f_ref_cnt;
};

spinlock_t g_f_lock;

void osal_device_init(void)
{
    spin_lock_init(&g_f_lock);
}

void osal_device_exit(void)
{
}

static int osal_get_file_ref(struct file *file)
{
    struct osal_private_data *pdata = NULL;

    spin_lock(&g_f_lock);
    pdata = file->private_data;
    if (pdata == NULL) {
        spin_unlock(&g_f_lock);
        return -1;
    }

    pdata->f_ref_cnt++;
    spin_unlock(&g_f_lock);

    return 0;
}

static int osal_put_file_ref(struct file *file)
{
    struct osal_private_data *pdata = NULL;

    spin_lock(&g_f_lock);
    pdata = file->private_data;
    if (pdata == NULL) {
        spin_unlock(&g_f_lock);
        return -1;
    }

    pdata->f_ref_cnt--;
    spin_unlock(&g_f_lock);

    return 0;
}

static int osal_open(struct inode *inode, struct file *file)
{
    struct ot_media_device *ot_media = NULL;
    osal_coat_dev_t *coat_dev = NULL;
    struct osal_private_data *pdata = NULL;

    osal_unused(inode);

    ot_media = (struct ot_media_device *)file->private_data;
    if (ot_media == NULL) {
        osal_trace("%s - get ot_media device error!\n", __FUNCTION__);
        return -1;
    }
    coat_dev = osal_container_of(ot_media, struct osal_coat_dev, ot_media_dev);
    pdata = (struct osal_private_data *)kmalloc(sizeof(struct osal_private_data), GFP_KERNEL);
    if (pdata == NULL) {
        osal_trace("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }
    if (DRVAL_DEBUG) {
        osal_trace("%s - file->private_data=%pK!\n", __FUNCTION__, pdata);
    }

    (void)memset_s(pdata, sizeof(struct osal_private_data), 0, sizeof(struct osal_private_data));

    file->private_data = pdata;
    pdata->dev = &(coat_dev->osal_dev);
    if (coat_dev->osal_dev.fops->open != NULL) {
        return coat_dev->osal_dev.fops->open((void *)&(pdata->data));
    }
    return 0;
}

static ssize_t osal_read(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    struct osal_private_data *pdata = file->private_data;
    int ret = 0;

    get_file_return(file);

    if (pdata->dev->fops->read != NULL) {
        ret = pdata->dev->fops->read(buf, (int)size, (long *)offset, (void *)&(pdata->data));
    }

    put_file_return(file);
    return ret;
}

static ssize_t osal_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    struct osal_private_data *pdata = file->private_data;
    int ret = -1;

    get_file_return(file);
    if (pdata->dev->fops->write != NULL) {
        ret = pdata->dev->fops->write(buf, (int)size, (long *)offset, (void *)&(pdata->data));
    }
    put_file_return(file);
    return ret;
}

static loff_t osal_llseek(struct file *file, loff_t offset, int whence)
{
    struct osal_private_data *pdata = file->private_data;
    int ret = 0;

    get_file_return(file);
    if (DRVAL_DEBUG) {
        osal_trace("%s - file->private_data=%pK!\n", __FUNCTION__, pdata);
    }

    if (whence == SEEK_SET) {
        if (pdata->dev->fops->llseek != NULL) {
            ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_SET, (void *)&(pdata->data));
        }
    } else if (whence == SEEK_CUR) {
        if (pdata->dev->fops->llseek != NULL) {
            ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_CUR, (void *)&(pdata->data));
        }
    } else if (whence == SEEK_END) {
        if (pdata->dev->fops->llseek != NULL) {
            ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_END, (void *)&(pdata->data));
        }
    }

    put_file_return(file);
    return (loff_t)ret;
}

static int osal_release(struct inode *inode, struct file *file)
{
    int ret = 0;
    struct osal_private_data *pdata = file->private_data;

    osal_unused(inode);
    get_file_return(file);

    if (DRVAL_DEBUG) {
        osal_trace("%s - file->private_data=%pK!\n", __FUNCTION__, pdata);
    }

    if (pdata->dev->fops->release != NULL) {
        ret = pdata->dev->fops->release((void *)&(pdata->data));
    }

    put_file_return(file);
    if (ret != 0) {
        osal_trace("%s - release failed!\n", __FUNCTION__);
        return ret;
    }

    spin_lock(&g_f_lock);
    if (pdata->f_ref_cnt != 0) {
        osal_trace("%s - release failed!\n", __FUNCTION__);
        spin_unlock(&g_f_lock);
        return -1;
    }
    kfree(file->private_data);
    file->private_data = NULL;
    spin_unlock(&g_f_lock);

    return 0;
}

static int osal_unlocked_ioctl_cmd_none(unsigned int cmd, unsigned long arg,
    const struct osal_private_data *pdata)
{
    if (pdata->dev->fops->unlocked_ioctl == NULL) {
        return -1;
    }

    return pdata->dev->fops->unlocked_ioctl(cmd, arg, (void *)&(pdata->data));
}

static int osal_unlocked_ioctl_cmd_write(unsigned int cmd, unsigned long arg,
    const struct osal_private_data *pdata)
{
    int ret = -1;
    char *kbuf = NULL;
    char arg_buf[ARG_BUF_TEMP_SIZE] = {}; /* temp to prevent tiny vmalloc */

    if ((char *)(uintptr_t)arg == NULL) {
        return -1;
    }

    if (OSAL_IOC_SIZE(cmd) <= ARG_BUF_TEMP_SIZE) {
        kbuf = arg_buf;
    } else {
        kbuf = (char *)kmalloc(OSAL_IOC_SIZE(cmd), GFP_KERNEL);
        if (kbuf == NULL) {
            osal_trace("%s - kmalloc failed!\n", __FUNCTION__);
            return -1;
        }
    }

    if (copy_from_user(kbuf, (char *)(uintptr_t)arg, OSAL_IOC_SIZE(cmd))) {
        ret = -1;
        goto OUT;
    }

    if (pdata->dev->fops->unlocked_ioctl == NULL) {
        ret = -1;
    } else {
        ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)(uintptr_t)kbuf, (void *)&(pdata->data));
    }

OUT:
    if ((kbuf != NULL) && (OSAL_IOC_SIZE(cmd) > ARG_BUF_TEMP_SIZE)) {
        kfree(kbuf);
        kbuf = NULL;
    }

    return ret;
}

static int osal_unlocked_ioctl_cmd_read(unsigned int cmd, unsigned long arg,
    const struct osal_private_data *pdata)
{
    int ret = -1;
    char *kbuf = NULL;
    char arg_buf[ARG_BUF_TEMP_SIZE] = {}; /* temp to prevent tiny vmalloc */

    if ((char *)(uintptr_t)arg == NULL) {
        return -1;
    }

    if (OSAL_IOC_SIZE(cmd) <= ARG_BUF_TEMP_SIZE) {
        kbuf = arg_buf;
    } else {
        kbuf = kmalloc(OSAL_IOC_SIZE(cmd), GFP_KERNEL);
        if (kbuf == NULL) {
            osal_trace("%s - kmalloc failed!\n", __FUNCTION__);
            return -1;
        }
    }

    if (pdata->dev->fops->unlocked_ioctl == NULL) {
        ret = -1;
    } else {
        ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)(uintptr_t)kbuf, (void *)&(pdata->data));
        if (ret == 0) {
            if (copy_to_user((char *)(uintptr_t)arg, kbuf, OSAL_IOC_SIZE(cmd))) {
                ret = -1;
            }
        }
    }

    if ((kbuf != NULL) && (OSAL_IOC_SIZE(cmd) > ARG_BUF_TEMP_SIZE)) {
        kfree(kbuf);
        kbuf = NULL;
    }
    return ret;
}

static int osal_unlocked_ioctl_cmd_read_write(unsigned int cmd, unsigned long arg,
    const struct osal_private_data *pdata)
{
    int ret = -1;
    char *kbuf = NULL;
    char arg_buf[ARG_BUF_TEMP_SIZE] = {}; /* temp to prevent tiny vmalloc */

    if ((char *)(uintptr_t)arg == NULL) {
        return -1;
    }

    if (OSAL_IOC_SIZE(cmd) <= ARG_BUF_TEMP_SIZE) {
        kbuf = arg_buf;
    } else {
        kbuf = (char *)kmalloc(OSAL_IOC_SIZE(cmd), GFP_KERNEL);
        if (kbuf == NULL) {
            osal_trace("%s - kmalloc failed!\n", __FUNCTION__);
            return -1;
        }
    }

    if (copy_from_user(kbuf, (char *)(uintptr_t)arg, OSAL_IOC_SIZE(cmd))) {
        ret = -1;
        goto OUT;
    }

    if (pdata->dev->fops->unlocked_ioctl == NULL) {
        ret = -1;
    } else {
        ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)(uintptr_t)kbuf, (void *)&(pdata->data));
        if (ret == 0) {
            if (copy_to_user((char *)(uintptr_t)arg, kbuf, OSAL_IOC_SIZE(cmd))) {
                ret = -1;
            }
        }
    }

OUT:
    if ((kbuf != NULL) && (OSAL_IOC_SIZE(cmd) > ARG_BUF_TEMP_SIZE)) {
        kfree(kbuf);
        kbuf = NULL;
    }
    return ret;
}

static long osal_do_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = -1;
    struct osal_private_data *pdata = file->private_data;

    if (DRVAL_DEBUG) {
        osal_trace("%s - file->private_data=%pK!\n", __FUNCTION__, pdata);
    }

    if (((OSAL_IOC_SIZE(cmd) == 0) && (OSAL_IOC_DIR(cmd) != OSAL_IOC_NONE))) {
        return -1;
    }

    if ((OSAL_IOC_DIR(cmd) != OSAL_IOC_NONE) && (((char *)(uintptr_t)arg) == NULL)) {
        osal_trace("%s - Input param err,it is null!\n", __FUNCTION__);
        return -1;
    }

    if (OSAL_IOC_DIR(cmd) == OSAL_IOC_NONE) {
        ret = osal_unlocked_ioctl_cmd_none(cmd, arg, pdata);
    } else if (OSAL_IOC_DIR(cmd) == OSAL_IOC_WRITE) {
        ret = osal_unlocked_ioctl_cmd_write(cmd, arg, pdata);
    } else if (OSAL_IOC_DIR(cmd) == OSAL_IOC_READ) {
        ret = osal_unlocked_ioctl_cmd_read(cmd, arg, pdata);
    } else if (OSAL_IOC_DIR(cmd) == (OSAL_IOC_READ + OSAL_IOC_WRITE)) {
        ret = osal_unlocked_ioctl_cmd_read_write(cmd, arg, pdata);
    }

    return ret;
}

static long osal_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long ret;

    get_file_return(file);

    ret = osal_do_unlocked_ioctl(file, cmd, arg);
    put_file_return(file);

    return ret;
}

#ifdef CONFIG_COMPAT
static int osal_compat_ioctl_check_none(unsigned int cmd, unsigned long arg,
    const struct osal_private_data *pdata)
{
    if (pdata->dev->fops->compat_ioctl == NULL) {
        return -1;
    } else {
        return pdata->dev->fops->compat_ioctl(cmd, arg, (void *)&(pdata->data));
    }
}

static int osal_compat_ioctl_check_write(unsigned int cmd, unsigned long arg,
    const struct osal_private_data *pdata)
{
    int ret = -1;
    char *kbuf = NULL;

    if ((char *)(uintptr_t)arg == NULL) {
        return -1;
    }

    kbuf = (char *)vmalloc(OSAL_IOC_SIZE(cmd));
    if (kbuf == NULL) {
        osal_trace("%s - vmalloc failed!\n", __FUNCTION__);
        return -1;
    }

    if (copy_from_user(kbuf, (char *)(uintptr_t)arg, OSAL_IOC_SIZE(cmd))) {
        vfree(kbuf);
        return -1;
    }

    if (pdata->dev->fops->compat_ioctl == NULL) {
        vfree(kbuf);
        return -1;
    } else {
        ret = pdata->dev->fops->compat_ioctl(cmd, (unsigned long)(uintptr_t)kbuf, (void *)&(pdata->data));
    }

    vfree(kbuf);
    return ret;
}

static int osal_compat_ioctl_check_read(unsigned int cmd, unsigned long arg,
    const struct osal_private_data *pdata)
{
    int ret = -1;
    char *kbuf = NULL;

    if ((char *)(uintptr_t)arg == NULL) {
        return -1;
    }

    kbuf = (char *)vmalloc(OSAL_IOC_SIZE(cmd));
    if (kbuf == NULL) {
        osal_trace("%s - vmalloc failed!\n", __FUNCTION__);
        return -1;
    }

    if (pdata->dev->fops->compat_ioctl == NULL) {
        vfree(kbuf);
        return -1;
    } else {
        ret = pdata->dev->fops->compat_ioctl(cmd, (unsigned long)(uintptr_t)kbuf, (void *)&(pdata->data));
        if (ret == 0) {
            if (copy_to_user((char *)(uintptr_t)arg, kbuf, OSAL_IOC_SIZE(cmd))) {
                vfree(kbuf);
                return -1;
            }
        }
    }

    vfree(kbuf);
    return ret;
}

static int osal_compat_ioctl_check_read_write(unsigned int cmd, unsigned long arg,
    const struct osal_private_data *pdata)
{
    int ret = -1;
    char *kbuf = NULL;

    if ((char *)(uintptr_t)arg == NULL) {
        return -1;
    }

    kbuf = (char *)vmalloc(OSAL_IOC_SIZE(cmd));
    if (kbuf == NULL) {
        osal_trace("%s - vmalloc failed!\n", __FUNCTION__);
        return -1;
    }

    if (copy_from_user(kbuf, (char *)(uintptr_t)arg, OSAL_IOC_SIZE(cmd))) {
        vfree(kbuf);
        return -1;
    }

    if (pdata->dev->fops->compat_ioctl == NULL) {
        vfree(kbuf);
        return -1;
    } else {
        ret = pdata->dev->fops->compat_ioctl(cmd, (unsigned long)(uintptr_t)kbuf, (void *)&(pdata->data));
        if (ret == 0) {
            if (copy_to_user((char *)(uintptr_t)arg, kbuf, OSAL_IOC_SIZE(cmd))) {
                vfree(kbuf);
                return -1;
            }
        }
    }

    vfree(kbuf);
    return ret;
}

static long osal_do_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = -1;
    struct osal_private_data *pdata = file->private_data;

    if (DRVAL_DEBUG) {
        osal_trace("%s - file->private_data=%pK!\n", __FUNCTION__, pdata);
    }

    if (((OSAL_IOC_SIZE(cmd) == 0) && (OSAL_IOC_DIR(cmd) != OSAL_IOC_NONE))) {
        return -1;
    }

    if ((OSAL_IOC_DIR(cmd) != OSAL_IOC_NONE) && (((char *)(uintptr_t)arg) == NULL)) {
        osal_trace("%s - Input param err,it is null!\n", __FUNCTION__);
        return -1;
    }

    if (OSAL_IOC_DIR(cmd) == OSAL_IOC_NONE) {
        ret = osal_compat_ioctl_check_none(cmd, arg, pdata);
    } else if (OSAL_IOC_DIR(cmd) == OSAL_IOC_WRITE) {
        ret = osal_compat_ioctl_check_write(cmd, arg, pdata);
    } else if (OSAL_IOC_DIR(cmd) == OSAL_IOC_READ) {
        ret = osal_compat_ioctl_check_read(cmd, arg, pdata);
    } else if (OSAL_IOC_DIR(cmd) == (OSAL_IOC_READ + OSAL_IOC_WRITE)) {
        ret = osal_compat_ioctl_check_read_write(cmd, arg, pdata);
    }

    return ret;
}

static long osal_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long ret;

    get_file_return(file);

    ret = osal_do_compat_ioctl(file, cmd, arg);
    put_file_return(file);

    return ret;
}

#endif

static unsigned int osal_poll(struct file *file, struct poll_table_struct *table)
{
    struct osal_private_data *pdata = file->private_data;
    struct osal_poll t;
    unsigned int ret = 0;

    get_file_return(file);

    if (DRVAL_DEBUG) {
        osal_trace("%s - table=%pK, file=%pK!\n", __FUNCTION__, table, file);
    }
    t.poll_table = table;
    t.data = file;
    if (pdata->dev->fops->poll != NULL) {
        ret = pdata->dev->fops->poll(&t, (void *)&(pdata->data));
    }

    put_file_return(file);
    return ret;
}

int valid_mmap_phys_addr_range(unsigned long pfn, size_t size)
{
    /* check physical addr greater than the max addr supported by the system */
    if ((pfn + (size >> PAGE_SHIFT)) > (1 + ((~0UL) >> PAGE_SHIFT))) {
        osal_trace("--%s - %d--!\n", __FUNCTION__, __LINE__);
        return 0;
    }

    /* It's necessary for the variable "size" to align 4k(page_size). */
#define PAGE_SIZE_MASK 0xfffffffffffff000ULL
    if ((unsigned long)size & (~PAGE_SIZE_MASK)) {
        osal_trace("--%s - %d--!\n", __FUNCTION__, __LINE__);
        return 0;
    }
#undef PAGE_SIZE_MASK

    return 1;
}

static int osal_mmap(struct file *file, struct vm_area_struct *vm)
{
    struct osal_vm osal_vm;
    struct osal_private_data *pdata = file->private_data;
    osal_vm.vm = vm;

    if (!valid_mmap_phys_addr_range(vm->vm_pgoff, vm->vm_end - vm->vm_start)) {
        osal_trace("\n%s - invalid argument   size=%ld!!!\n", __FUNCTION__, vm->vm_end - vm->vm_start);
        return -EINVAL;
    }

    if (DRVAL_DEBUG) {
        osal_trace("%s - start=%lx, end=%lx!, off=%lx\n", __FUNCTION__, vm->vm_start, vm->vm_end, vm->vm_pgoff);
    }
    if (pdata->dev->fops->mmap != NULL) {
        return pdata->dev->fops->mmap(&osal_vm, vm->vm_start, vm->vm_end, vm->vm_pgoff, (void *)&(pdata->data));
    }
    return 0;
}

static struct file_operations g_osal_fops = {
    .owner = THIS_MODULE,
    .open = osal_open,
    .read = osal_read,
    .write = osal_write,
    .llseek = osal_llseek,
    .unlocked_ioctl = osal_unlocked_ioctl,
    .release = osal_release,
    .poll = osal_poll,
    .mmap = osal_mmap,
#ifdef CONFIG_COMPAT
    .compat_ioctl = osal_compat_ioctl,
#endif
};

osal_dev_t *osal_createdev(const char *name)
{
    osal_coat_dev_t *pdev = NULL;
    errno_t err_value;

    if (name == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return NULL;
    }
    pdev = (osal_coat_dev_t *)kmalloc(sizeof(osal_coat_dev_t), GFP_KERNEL);
    if (pdev == NULL) {
        osal_trace("%s - kmalloc error!\n", __FUNCTION__);
        return NULL;
    }
    (void)memset_s(pdev, sizeof(osal_coat_dev_t), 0, sizeof(osal_coat_dev_t));
    err_value = strncpy_s(pdev->osal_dev.name, OSAL_MAX_DEV_NAME_LEN, name, sizeof(pdev->osal_dev.name) - 1);
    if (err_value != EOK) {
        kfree(pdev);
        osal_trace("%s - strncpy_s failed!\n", __FUNCTION__);
        return NULL;
    }

    pdev->osal_dev.dev = pdev;
    return &(pdev->osal_dev);
}
EXPORT_SYMBOL(osal_createdev);

int osal_destroydev(osal_dev_t *osal_dev)
{
    osal_coat_dev_t *pdev = NULL;
    if (osal_dev == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    pdev = osal_dev->dev;
    if (pdev == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    kfree(pdev);
    return 0;
}
EXPORT_SYMBOL(osal_destroydev);

int osal_registerdevice(osal_dev_t *osal_dev)
{
    struct ot_media_device *ot_media = NULL;
    errno_t err_value;

    if ((osal_dev == NULL) || (osal_dev->fops == NULL) || (osal_dev->dev == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    ot_media = &(((osal_coat_dev_t *)(osal_dev->dev))->ot_media_dev);
    if (osal_dev->minor != 0) {
        ot_media->minor = osal_dev->minor;
    } else {
        ot_media->minor = OT_MEDIA_DYNAMIC_MINOR;
    }
    ot_media->owner = THIS_MODULE;
    ot_media->fops = &g_osal_fops;
    err_value = strncpy_s(ot_media->devfs_name, OT_MEDIA_MAX_DEV_NAME_LEN, osal_dev->name,
        sizeof(ot_media->devfs_name) - 1);
    if (err_value != EOK) {
        osal_trace("%s - strncpy_s failed!\n", __FUNCTION__);
        return -1;
    }
    return ot_media_register(ot_media);
}
EXPORT_SYMBOL(osal_registerdevice);

void osal_deregisterdevice(osal_dev_t *pdev)
{
    if ((pdev == NULL) || (pdev->dev == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    ot_media_unregister((struct ot_media_device *)&(((osal_coat_dev_t *)(pdev->dev))->ot_media_dev));
}
EXPORT_SYMBOL(osal_deregisterdevice);

void osal_poll_wait(osal_poll_t *table, const osal_wait_t *wait)
{
    if ((table == NULL) || (table->data == NULL) || (table->poll_table == NULL) ||
        (wait == NULL) || (wait->wait == NULL)) {
        osal_trace("%s - input param is null-!\n", __FUNCTION__);
        return;
    }

    if (DRVAL_DEBUG) {
        osal_trace("%s - call poll_wait +!, table=%pK, file=%pK\n", __FUNCTION__, table->poll_table, table->data);
    }

    poll_wait((struct file *)table->data, (wait_queue_head_t *)(wait->wait), table->poll_table);

    if (DRVAL_DEBUG) {
        osal_trace("%s - call poll_wait -!\n", __FUNCTION__);
    }
}
EXPORT_SYMBOL(osal_poll_wait);

void osal_pgprot_noncached(osal_vm_t *vm)
{
    struct vm_area_struct *v = NULL;

    if ((vm == NULL) || (vm->vm == NULL)) {
        osal_trace("%s - input error -!\n", __FUNCTION__);
        return;
    }
    v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = pgprot_noncached(v->vm_page_prot);
}

EXPORT_SYMBOL(osal_pgprot_noncached);

void osal_pgprot_cached(osal_vm_t *vm)
{
    struct vm_area_struct *v = NULL;

    if ((vm == NULL) || (vm->vm == NULL)) {
        osal_trace("%s - input error -!\n", __FUNCTION__);
        return;
    }
    v = (struct vm_area_struct *)(vm->vm);

#ifdef CONFIG_64BIT
    v->vm_page_prot = __pgprot(pgprot_val(v->vm_page_prot) |
                               PTE_VALID | PTE_DIRTY | PTE_AF);
#else

    v->vm_page_prot = __pgprot(pgprot_val(v->vm_page_prot) | L_PTE_PRESENT |
                               L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED);
#endif
}
EXPORT_SYMBOL(osal_pgprot_cached);

void osal_pgprot_writecombine(osal_vm_t *vm)
{
    struct vm_area_struct *v = NULL;

    if ((vm == NULL) || (vm->vm == NULL)) {
        osal_trace("vm(NULL) invalid!\n");
        return;
    }
    v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = pgprot_writecombine(v->vm_page_prot);
}
EXPORT_SYMBOL(osal_pgprot_writecombine);

int osal_remap_pfn_range(osal_vm_t *vm, unsigned long addr, unsigned long pfn, unsigned long size)
{
    struct vm_area_struct *v = NULL;

    if ((vm == NULL) || (vm->vm == NULL)) {
        osal_trace("%s - input error -!\n", __FUNCTION__);
        return -EPERM;
    }

    if (size == 0) {
        return -EPERM;
    }
    v = (struct vm_area_struct *)(vm->vm);
    return remap_pfn_range(v, addr, pfn, size, v->vm_page_prot);
}
EXPORT_SYMBOL(osal_remap_pfn_range);

