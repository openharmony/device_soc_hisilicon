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

#include <linux/miscdevice.h>
#include "securec.h"
#include "hi_osal.h"
#include "hidev.h"

static osal_semaphore g_hidev_pm_sem = {0};
static hi_u32 g_hidev_pm_cmd = -1;
static hi_s32 g_dbg_flag = 1;
#define hidev_dbg(params...) do { \
        if (g_dbg_flag) {         \
            osal_printk(params);  \
        }                         \
    } while (0)

static hi_s32 hidev_open(hi_void *private_data)
{
    hidev_dbg("Enter hidev_open\n");
    return HI_SUCCESS;
}

static hi_s32 hidev_release(hi_void *private_data)
{
    hidev_dbg("Enter hidev_release\n");
    return HI_SUCCESS;
}

static hi_s32 hidev_get_pm_cmd(unsigned int cmd, hi_void *para, hi_void *private_data)
{
    struct hidev_pm_cmd *pm_cmd = para;
    hi_s32 ret;

    ret = osal_sem_down_interruptible(&g_hidev_pm_sem);
    if (ret == 0) {
        hidev_dbg("hidev get pm cmd %d.\n", g_hidev_pm_cmd);
    }
    pm_cmd->cmd = g_hidev_pm_cmd;
    return ret;
}

static hi_s32 hidev_free_block_mem(unsigned int cmd, hi_void *para, hi_void *private_data)
{
    struct hidev_block_mem *block_mem = para;
    hidev_dbg("hidev_free_block_mem physaddr 0x%x, size %u.\n", block_mem->phys_addr, block_mem->size);
    osal_blockmem_free(block_mem->phys_addr, block_mem->size);
    return HI_SUCCESS;
}

static hi_s32 hidev_pm_suspend(hi_void *private_data)
{
    /* userspace drv suspend called from userspace directly */
    return HI_SUCCESS;
}

static hi_s32 hidev_pm_resume(hi_void *private_data)
{
    hi_s32 i;
    g_hidev_pm_cmd = HIDEV_PM_RESUME;
    hidev_dbg("hidev enter pm cmd %d.\n", g_hidev_pm_cmd);
    osal_sem_up(&g_hidev_pm_sem);
    return HI_SUCCESS;
}

static hi_s32 hidev_pm_lowpower_enter(hi_void *private_data)
{
    g_hidev_pm_cmd = HIDEV_PM_LOWPOWER_ENTER;
    hidev_dbg("hidev enter pm cmd %d.\n", g_hidev_pm_cmd);
    osal_sem_up(&g_hidev_pm_sem);
    return HI_SUCCESS;
}

static hi_s32 hidev_pm_lowpower_exit(hi_void *private_data)
{
    g_hidev_pm_cmd = HIDEV_PM_LOWPOWER_EXIT;
    hidev_dbg("hidev enter pm cmd %d.\n", g_hidev_pm_cmd);
    osal_sem_up(&g_hidev_pm_sem);
    return HI_SUCCESS;
}

static hi_s32 hidev_pm_poweroff(hi_void *private_data)
{
    g_hidev_pm_cmd = HIDEV_PM_POWEROFF;
    hidev_dbg("hidev enter pm cmd %d.\n", g_hidev_pm_cmd);
    osal_sem_up(&g_hidev_pm_sem);
    return HI_SUCCESS;
}

static osal_pmops g_hidev_pmops = {
    .pm_suspend = hidev_pm_suspend,
    .pm_resume = hidev_pm_resume,
    .pm_lowpower_enter = hidev_pm_lowpower_enter,
    .pm_lowpower_exit = hidev_pm_lowpower_exit,
    .pm_poweroff = hidev_pm_poweroff,
};

static osal_ioctl_cmd g_hidev_cmd[] = {
    {HIDEV_GET_PM_CMD, hidev_get_pm_cmd},
    {HIDEV_FREE_BLOCK_MEM, hidev_free_block_mem},
};

static osal_fileops g_hidev_fops = {
    .open = hidev_open,
    .release = hidev_release,
    .cmd_list = g_hidev_cmd,
    .cmd_cnt = sizeof(g_hidev_cmd) / sizeof(g_hidev_cmd[0]),
};

static osal_dev g_hidev_dev = {
    .name = HIDEV_DEVICE_NAME,
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &g_hidev_fops,
    .pmops = &g_hidev_pmops,
};

static hi_s32 hidev_init(hi_void)
{
    if (osal_dev_register(&g_hidev_dev) != 0) {
        osal_printk("hidev register %s failed.\n", g_hidev_dev.name);
        return HI_FAILURE;
    }

    osal_sem_init(&g_hidev_pm_sem, 0);

    hidev_dbg("hi_dev init ok.\n");
    return HI_SUCCESS;
}

static hi_void hidev_deinit(hi_void)
{
    osal_dev_unregister(&g_hidev_dev);

    osal_sem_destory(&g_hidev_pm_sem);

    hidev_dbg("hi_dev deinit ok.\n");
}

static hi_s32 hi_dev_module_init(hi_void)
{
    hi_s32 ret;
    ret = hidev_init();
    return ret;
}

static hi_void hi_dev_module_exit(hi_void)
{
    hidev_deinit();
    return;
}

module_init(hi_dev_module_init);
module_exit(hi_dev_module_exit);
