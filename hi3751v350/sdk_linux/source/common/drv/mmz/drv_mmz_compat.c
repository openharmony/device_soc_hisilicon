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

#include <linux/string.h>
#include <linux/errno.h>
#include <linux/compat.h>
#include <linux/uaccess.h>
#include <linux/securec.h>

#include "drv_mmz_compat.h"
#include "drv_mmz_ioctl.h"

static int get_meminfo_from_user(const struct compat_mmb_info *compat_mmb, struct mmb_info *mmi)
{
    int ret = 0;

    if ((compat_mmb == NULL) || (mmi == NULL)) {
        ret = -EFAULT;
        return ret;
    }

    ret = strncpy_s(mmi->mmz_name, HIL_MAX_NAME_LEN, compat_mmb->mmz_name, (HIL_MAX_NAME_LEN - 1));
    if (ret != EOK) {
        hi_mmz_error("strncpy_s fail\n");
        return -EFAULT;
    }

    ret = strncpy_s(mmi->mmb_name, HIL_MAX_NAME_LEN, compat_mmb->mmb_name, (HIL_MAX_NAME_LEN - 1));
    if (ret != EOK) {
        hi_mmz_error("strncpy_s fail\n");
        return -EFAULT;
    }

    mmi->phys_addr = compat_mmb->phys_addr;
    mmi->smmu_addr = compat_mmb->smmu_addr;
    mmi->align = compat_mmb->align;
    mmi->size = compat_mmb->size;
    mmi->mapped = compat_ptr(compat_mmb->mapped);

    mmi->prot = compat_mmb->prot;
    mmi->flags = compat_mmb->flags;

    return 0;
}

static int put_meminfo_to_user(struct compat_mmb_info *compat_mmb, const struct mmb_info *mmi)
{
    if ((compat_mmb == NULL) || (mmi == NULL)) {
        return -EFAULT;
    }

    compat_mmb->phys_addr = mmi->phys_addr;
    compat_mmb->smmu_addr = mmi->smmu_addr;
    compat_mmb->size = mmi->size;
    compat_mmb->mapped = ptr_to_compat(mmi->mapped);

    return 0;
}

typedef struct {
    unsigned int  compat_cmd;
    char         *compat_cmd_str;
    unsigned int  mmb_cmd;
} userdev_cmd_info;

static userdev_cmd_info g_userdev_cmd_info_normal[] = {
    { COMPAT_IOC_MMB_ALLOC,                 "COMPAT_IOC_MMB_ALLOC",                 IOC_MMB_ALLOC },
    { COMPAT_IOC_MMB_FREE,                  "COMPAT_IOC_MMB_FREE",                  IOC_MMB_FREE },
    { COMPAT_IOC_MMB_USER_REMAP,            "COMPAT_IOC_MMB_USER_REMAP",            IOC_MMB_USER_REMAP },
    { COMPAT_IOC_MMB_USER_REMAP_CACHED,     "COMPAT_IOC_MMB_USER_REMAP_CACHED",     IOC_MMB_USER_REMAP_CACHED },
    { COMPAT_IOC_MMB_USER_UNMAP,            "COMPAT_IOC_MMB_USER_UNMAP",            IOC_MMB_USER_UNMAP },
    { COMPAT_IOC_MMB_USER_GETPHYADDR,       "COMPAT_IOC_MMB_USER_GETPHYADDR",       IOC_MMB_USER_GETPHYADDR },
    { COMPAT_IOC_MMB_USER_CMA_MAPTO_SMMU,   "COMPAT_IOC_MMB_USER_CMA_MAPTO_SMMU",   IOC_MMB_USER_CMA_MAPTO_SMMU },
    { COMPAT_IOC_MMB_USER_CMA_UNMAPTO_SMMU, "COMPAT_IOC_MMB_USER_CMA_UNMAPTO_SMMU", IOC_MMB_USER_CMA_UNMAPTO_SMMU }
};

static long compat_mmz_userdev_normal(struct file *filp, unsigned int cmd, struct mmb_info *mmi)
{
    long ret = 0;
    unsigned int index;
    unsigned int max_index;

    if (filp == NULL || mmi == NULL) {
        ret = -EFAULT;
        goto exit;
    }

    max_index = sizeof(g_userdev_cmd_info_normal) / sizeof(userdev_cmd_info);
    for (index = 0; index < max_index; index++) {
        if (_IOC_NR(cmd) == _IOC_NR(g_userdev_cmd_info_normal[index].compat_cmd)) {
            ret = mmz_userdev_ioctl_m(filp->f_path.dentry->d_inode, filp,
                                      g_userdev_cmd_info_normal[index].mmb_cmd, mmi);
            if (ret) {
                hi_mmz_error("%s failed!\n", g_userdev_cmd_info_normal[index].compat_cmd_str);
                ret = -EFAULT;
                goto exit;
            }
            break;
        }
    }

    if (index == max_index) {
        hi_mmz_error("%s CMD FAILED!\n", __func__);
        ret = -ENOIOCTLCMD;
    }

exit:
    return ret;
}

static long compat_mmz_userdev_ioctl_m(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct compat_mmb_info compat_mmb;
    struct mmb_info mmi;
    long ret;

    if (sizeof(compat_mmb) != _IOC_SIZE(cmd) || arg == 0) {
        hi_mmz_warn("_IOC_SIZE(cmd) = %d, arg = 0x%08lX\n", _IOC_SIZE(cmd), arg);
        ret = -EINVAL;
        goto exit;
    }

    if (memset_s(&compat_mmb, sizeof(compat_mmb), 0, sizeof(compat_mmb)) != EOK ||
        memset_s(&mmi, sizeof(mmi), 0, sizeof(mmi)) != EOK) {
        hi_mmz_warn("memset failed\n");
        return -EINVAL;
    }

    if (copy_from_user(&compat_mmb, (void __user *)(uintptr_t)arg, _IOC_SIZE(cmd))) {
        hi_mmz_warn("copy_from_user failed!\n");
        ret = -EFAULT;
        goto exit;
    }

    ret = get_meminfo_from_user(&compat_mmb, &mmi);
    if (ret) {
        hi_mmz_warn("get_meminfo_from_user failed\n");
        ret = -EFAULT;
        goto exit;
    }
    ret = compat_mmz_userdev_normal(filp, cmd, &mmi);
    if (ret) {
        hi_mmz_warn("compat_mmz_userdev_normal failed!\n");
        goto exit;
    }

    if (_IOC_DIR(cmd) & _IOC_READ) {
        ret = put_meminfo_to_user(&compat_mmb, &mmi);
        if (ret) {
            hi_mmz_warn("put_meminfo_to_user failed!\n");
            ret = -EFAULT;
            goto exit;
        }
        if (copy_to_user((void __user *)(uintptr_t)arg, &compat_mmb, _IOC_SIZE(cmd))) {
            hi_mmz_warn("copy_from_user failed!\n");
            ret = -EFAULT;
            goto exit;
        }
    }
exit:
    return ret;
}

static long compat_mmz_userdev_ioctl_c(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long ret = 0;
    struct compat_mmb_info compat_mmb;
    struct mmb_info mmi;

    switch (_IOC_NR(cmd)) {
        case _IOC_NR(COMPAT_IOC_MMB_FLUSH_DCACHE): {
            if (arg == 0) {
                mmz_flush_all_dcache();
                return 0;
            }

            if (sizeof(compat_mmb) != _IOC_SIZE(cmd)) {
                hi_mmz_warn("_IOC_SIZE(cmd) = %d, arg = 0x%08lX\n", _IOC_SIZE(cmd), arg);
                return -EINVAL;
            }
            ret = memset_s(&compat_mmb, sizeof(compat_mmb), 0, sizeof(compat_mmb));
            if (ret != EOK) {
                hi_mmz_warn("memset failed\n");
                return -EINVAL;
            }
            ret = memset_s(&mmi, sizeof(mmi), 0, sizeof(mmi));
            if (ret != EOK) {
                hi_mmz_warn("memset failed\n");
                return -EINVAL;
            }

            if (copy_from_user(&compat_mmb, (void __user *)(uintptr_t)arg, _IOC_SIZE(cmd))) {
                hi_mmz_warn("copy_from_user failed!\n");
                return -EFAULT;
            }

            ret = get_meminfo_from_user(&compat_mmb, &mmi);
            if (ret) {
                hi_mmz_warn("get_meminfo_from_user failed\n");
                return -EFAULT;
            }
            mmz_flush_dcache_mmb(&mmi);
            break;
        }
        default: {
            return -ENOIOCTLCMD;
        }
    }

    return ret;
}

long compat_mmz_userdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long ret = 0;
    struct mmz_userdev_info *pmu = NULL;

    if (filp == NULL || filp->private_data == NULL || filp->f_op == NULL) {
        hi_mmz_error("invaled params!\n");
        return -EINVAL;
    }

    if (!filp->f_op->unlocked_ioctl) {
        return -ENOTTY;
    }

    pmu = filp->private_data;

    down(&pmu->sem);
    if (_IOC_TYPE(cmd) == 'm') {
        ret = compat_mmz_userdev_ioctl_m(filp, cmd, arg);
        if (ret) {
            hi_mmz_warn("compat_mmz_userdev_ioctl_m failed!\n");
            goto exit;
        }
    } else if (_IOC_TYPE(cmd) == 'c') {
        ret = compat_mmz_userdev_ioctl_c(filp, cmd, arg);
        if (ret) {
            hi_mmz_error("compat_mmz_userdev_ioctl_c failed!\n");
            goto exit;
        }
    } else {
        ret = -ENOTTY;
    }

exit:
    up(&pmu->sem);
    return ret;
}
