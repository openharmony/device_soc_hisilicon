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

#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>

#include "ot_osal.h"
#include "ot_proc.h"
#include "securec.h"

static struct osal_dev *g_ot_proc_dev = NULL;

typedef struct {
    wait_queue_head_t wq_for_read;
    wait_queue_head_t wq_for_write;
    wait_queue_head_t wq_for_cmd;
} proc_k_para;
static td_s32 g_cmd_condition = 0;
static td_s32 g_read_condition = 0;
static td_s32 g_write_condition = 0;
static td_s32 g_cmd_exit_condition = 0;

typedef struct entrylist {
    td_u32 private_data;
    td_char entry_name[MAX_PROC_NAME_LEN];
    struct entrylist *next;
} ot_entry_list;

static ot_entry_list *g_proc_head = NULL;

static osal_mutex_t g_mutex = {
    .mutex = NULL
};

static ot_proc_para proc_wait;
static proc_k_para g_proc_k_para;

static td_s32 g_dbg_flag = 0;
#define ot_proc_dbg(params...) do { \
        if (g_dbg_flag) {         \
            osal_printk(params);  \
        }                         \
    } while (0)

td_void osal_proc_exit(td_void);
static td_s32 proc_read(osal_proc_entry_t *entry);
static td_s32 proc_write(osal_proc_entry_t *entry, const td_char *buf, td_s32 count, td_s64 *ppos);

static ot_entry_list *add_list(const td_char *name, const td_u32 private_data)
{
    ot_entry_list *tmp;
    tmp = (ot_entry_list *)osal_kmalloc(sizeof(ot_entry_list), osal_gfp_kernel);
    if (tmp == NULL) {
        return tmp;
    }
    (void)memset_s(tmp->entry_name, sizeof(tmp->entry_name), 0, sizeof(tmp->entry_name));
    (void)memcpy_s(tmp->entry_name, sizeof(tmp->entry_name), name, strlen(name) + 1);
    tmp->private_data = private_data;
    tmp->next = g_proc_head;
    g_proc_head = tmp;
    return tmp;
}

static td_void get_list_and_delete_node(const td_u32 private_data)
{
    ot_entry_list *tmp = g_proc_head;
    ot_entry_list *pre = g_proc_head;
    ot_entry_list *tmp1 = NULL;
    while (tmp != NULL) {
        if (tmp->private_data == private_data) {
            if (tmp == pre) { // del head
                g_proc_head = tmp->next;
                tmp1 = g_proc_head;
                pre = g_proc_head;
            } else {
                pre->next = tmp->next;
                tmp1 = pre->next;
            }
            osal_remove_proc_entry(tmp->entry_name, NULL);
            osal_kfree(tmp);
            tmp = tmp1;
            continue;
        }
        pre = tmp;
        tmp = tmp->next;
    }
    return;
}

static td_s32 del_list_node(const td_u32 private_data, const td_char *name)
{
    ot_entry_list *tmp = g_proc_head;
    ot_entry_list *pre = g_proc_head;
    while (tmp != NULL) {
        if (tmp->private_data == private_data && (!strncmp(tmp->entry_name, name, strlen(name)))) {
            if (tmp == pre) { // del head
                g_proc_head = tmp->next;
            } else {
                pre->next = tmp->next;
            }
            osal_kfree(tmp);
            tmp = NULL;
            return 0;
        }
        pre = tmp;
        tmp = tmp->next;
    }
    return -1;
}

static td_void del_list(td_void)
{
    ot_entry_list *tmp = g_proc_head;
    ot_entry_list *tmp2 = NULL;

    while (tmp != NULL) {
        tmp2 = tmp->next;
        osal_kfree(tmp);
        tmp = tmp2;
    }
    g_proc_head = NULL;
}

static td_s32 ot_proc_open(td_void *private_data)
{
    ot_proc_dbg("Enter ot_proc_open\n");
    osal_mutex_init(&g_mutex);
    init_waitqueue_head(&g_proc_k_para.wq_for_read);
    init_waitqueue_head(&g_proc_k_para.wq_for_write);
    init_waitqueue_head(&g_proc_k_para.wq_for_cmd);
    return 0;
}

static td_s32 ot_proc_release(td_void *private_data)
{
    ot_proc_dbg("Enter ot_proc_release\n");
    get_list_and_delete_node((td_u32)(uintptr_t)private_data);
    return 0;
}

static int check_validate_name(char const *name)
{
    unsigned int index;

    for (index = 0; (index < MAX_PROC_NAME_LEN - 1) && (*(name + index) != '\0'); index++) {
        if ((*(name + index) >= 'a' && *(name + index) <= 'z') ||
            (*(name + index) >= '0' && *(name + index) <= '9') || // number 0~9
            *(name + index) == '_') {
            continue;
        } else {
            return -1;
        }
    }
    return 0;
}

static td_slong ot_proc_ioctl_wait_cmd(td_ulong para)
{
    ot_proc_para *user_proc_info = NULL;
    wait_event_interruptible(g_proc_k_para.wq_for_cmd, g_cmd_condition != 0);

    if (g_cmd_exit_condition == 1) {
        g_cmd_condition = 0;
        g_cmd_exit_condition = 0;
        return TD_FAILURE;
    }

    user_proc_info = (ot_proc_para *)((uintptr_t)para);
    smp_rmb();
    if (((strlen(proc_wait.cmd) > 0) && (strlen(proc_wait.entry.name) > 0))) {
        (void)memcpy_s(&(user_proc_info->cmd), sizeof(user_proc_info->cmd), proc_wait.cmd,
            sizeof(proc_wait.cmd));
        (void)memcpy_s(&(user_proc_info->entry), sizeof(osal_proc_entry_t), &(proc_wait.entry),
            sizeof(osal_proc_entry_t));
        (void)memcpy_s(&(user_proc_info->write_buf.buf), sizeof(user_proc_info->write_buf.buf),
            proc_wait.write_buf.buf, sizeof(user_proc_info->write_buf.buf));
        user_proc_info->write_buf.ppos = proc_wait.write_buf.ppos;
        user_proc_info->write_buf.count = proc_wait.write_buf.count;
        g_cmd_condition = 0;
        return TD_SUCCESS;
    } else {
        if (g_cmd_condition != 0) {
            osal_printk("entry_name=%s, cmd=%s, cond=%d\n", proc_wait.entry.name, proc_wait.cmd,
                g_cmd_condition);
        }
        return TD_FAILURE;
    }
}

static td_s32 ot_proc_ioctl_create(td_ulong para, td_void *private_data)
{
    ot_proc_name *proc_name = (ot_proc_name *)((uintptr_t)para);
    osal_proc_entry_t *entry_info = NULL;

    proc_name->name[sizeof(proc_name->name) - 1] = '\0';
    if (check_validate_name(proc_name->name) != TD_SUCCESS) {
        osal_printk("[%s,line:%d]Error: invalied name.\n", OT_PROC_PFX, __LINE__);
        return TD_SUCCESS;
    }
    entry_info = osal_create_proc_entry(proc_name->name, NULL);
    if (entry_info == NULL) {
        osal_printk("[%s,line:%d]Error: can't create proc entry\n", OT_PROC_PFX, __LINE__);
        return TD_FAILURE;
    }
    add_list(entry_info->name, (td_u32)(uintptr_t)private_data);
    entry_info->read = proc_read;
    entry_info->write = proc_write;
    return TD_SUCCESS;
}

static td_s32 ot_proc_ioctl_remove(td_ulong para, td_void *private_data)
{
    td_s32 ret;
    ot_proc_name *proc_name = (ot_proc_name *)((uintptr_t)para);

    proc_name->name[sizeof(proc_name->name) - 1] = '\0';
    if (check_validate_name(proc_name->name) != TD_SUCCESS) {
        osal_printk("[%s,line:%d]Error: invalied name.\n", OT_PROC_PFX, __LINE__);
        return TD_SUCCESS;
    }
    osal_remove_proc_entry(proc_name->name, NULL);
    ret = del_list_node((td_u32)(uintptr_t)private_data, proc_name->name);
    if (ret != 0) {
        osal_printk("ot_proc delete entry failed\n");
    }
    return ret;
}

static td_s32 ot_proc_ioctl_wake_read(td_ulong para)
{
    ot_proc_show_buf *show_buf = (ot_proc_show_buf *)((uintptr_t)para);

    if (strlen(proc_wait.entry.name) <= 0) {
        osal_printk("entry info is NULL\n");
        return TD_FAILURE;
    }
    if (proc_wait.entry.read != NULL) {
        osal_printk("entry read is not NULL\n");
        return TD_FAILURE;
    }
    if (show_buf == NULL) {
        osal_printk("show_buf is NULL\n");
        return TD_FAILURE;
    }
    if (show_buf->size > OT_PROC_BUF_SIZE) {
        osal_printk("show_buf (size = %u) is overflow\n", show_buf->size);
        return TD_SUCCESS;
    }
    if (show_buf->buf == NULL) {
        osal_printk("show_buf is NULL\n");
        return TD_SUCCESS;
    }
    proc_wait.entry.read = osal_kmalloc(show_buf->size, osal_gfp_kernel);
    if (proc_wait.entry.read) {
        if (osal_copy_from_user(proc_wait.entry.read, (td_void __user *)show_buf->buf, show_buf->size)) {
            osal_kfree(proc_wait.entry.read);
            proc_wait.entry.read = NULL;
        }
    }
    g_read_condition = 1;
    wake_up_interruptible(&(g_proc_k_para.wq_for_read));
    return TD_SUCCESS;
}

static td_slong ot_proc_ioctl(td_u32 cmd, td_ulong para, td_void *private_data)
{
    td_s32 ret = 0;
    DEFINE_WAIT(wait);

    switch (cmd) {
        case USER_CREATE_PROC_ENTRY:
            ret = ot_proc_ioctl_create(para, private_data);
            break;
        case USER_REMOVE_PROC_ENTRY:
            ret = ot_proc_ioctl_remove(para, private_data);
            break;
        case USER_PROC_GET_CMD:
            ret = ot_proc_ioctl_wait_cmd(para);
            break;
        case USER_PROC_WAKE_READ_TASK:
            ret = ot_proc_ioctl_wake_read(para);
            break;
        case USER_PROC_WAKE_WRITE_TASK:
            g_write_condition = 1;
            wake_up_interruptible(&(g_proc_k_para.wq_for_write));
            break;
        case USER_PROC_WAKE_GET_CMD:
            g_cmd_condition = 1;
            g_cmd_exit_condition = 1;
            smp_wmb();
            wake_up_interruptible(&(g_proc_k_para.wq_for_cmd));
            break;
        default:
            osal_printk("[%s,line:%d]Error: Inappropriate ioctl for device. cmd=%u\n", OT_PROC_PFX, __LINE__, cmd);
            ret = TD_FAILURE;
            break;
    }
    return ret;
}

static struct osal_fileops g_ot_proc_fops = {
    open : ot_proc_open,
    release : ot_proc_release,
    unlocked_ioctl : ot_proc_ioctl,
};

td_s32 ot_proc_init(td_void)
{
    g_ot_proc_dev = osal_createdev(OT_PROC_DEVICE_NAME);
    if (g_ot_proc_dev == NULL) {
        osal_printk("[%s,line:%d]Error: can't create dev\n", OT_PROC_PFX, __LINE__);
        return -1;
    }

    g_ot_proc_dev->minor = MISC_DYNAMIC_MINOR;
    g_ot_proc_dev->fops = &g_ot_proc_fops;
    if (osal_registerdevice(g_ot_proc_dev) != 0) {
        osal_printk("[%s,line:%d]Error: can't register\n", OT_PROC_PFX, __LINE__);
        osal_destroydev(g_ot_proc_dev);
        return -1;
    }

    ot_proc_dbg("ot_proc init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return 0;
}

static td_s32 proc_read(osal_proc_entry_t *entry)
{
    struct seq_file *s = entry->seqfile;
    osal_proc_entry_t *entry_info = s->private;
    DEFINE_WAIT(wait);

    osal_mutex_lock(&g_mutex);
    /* only these two parameters are used */
    (void)memcpy_s(&(proc_wait.entry.name), sizeof(proc_wait.entry.name), entry_info->name,
        sizeof(proc_wait.entry.name));
    (void)memcpy_s(&(proc_wait.cmd), sizeof(proc_wait.cmd), OT_USER_PROC_READ_CMD, strlen(OT_USER_PROC_READ_CMD) + 1);
    smp_wmb();
    g_cmd_condition = 1;
    wake_up_interruptible(&(g_proc_k_para.wq_for_cmd));
    wait_event_interruptible(g_proc_k_para.wq_for_read, g_read_condition != 0);
    g_read_condition = 0;

    if (proc_wait.entry.read != NULL) {
        osal_seq_printf(entry, "%s", (td_char *)proc_wait.entry.read);
        osal_kfree(proc_wait.entry.read);
        proc_wait.entry.read = NULL;
    }
    (void)memset_s(&(proc_wait.entry), sizeof(osal_proc_entry_t), 0, sizeof(osal_proc_entry_t));
    (void)memset_s(&(proc_wait.cmd), sizeof(proc_wait.cmd), 0, sizeof(proc_wait.cmd));
    osal_mutex_unlock(&g_mutex);
    return 0;
}

static td_s32 proc_write(osal_proc_entry_t *entry, const td_char *buf, td_s32 count, td_s64 *ppos)
{
    DEFINE_WAIT(wait);
    osal_mutex_lock(&g_mutex);
    (void)memcpy_s(&(proc_wait.cmd), sizeof(proc_wait.cmd), OT_USER_PROC_WRITE_CMD, strlen(OT_USER_PROC_WRITE_CMD) + 1);
    (void)memcpy_s(&(proc_wait.entry), sizeof(osal_proc_entry_t), entry, sizeof(osal_proc_entry_t));
    osal_copy_from_user(proc_wait.write_buf.buf, buf, sizeof(proc_wait.write_buf.buf));
    proc_wait.write_buf.ppos = *ppos;
    proc_wait.write_buf.count = count;
    smp_wmb();
    g_cmd_condition = 1;
    wake_up_interruptible(&(g_proc_k_para.wq_for_cmd));
    wait_event_interruptible(g_proc_k_para.wq_for_write, g_write_condition != 0);
    g_write_condition = 0;

    (void)memset_s(&(proc_wait.entry), sizeof(osal_proc_entry_t), 0, sizeof(osal_proc_entry_t));
    (void)memset_s(&(proc_wait.cmd), sizeof(proc_wait.cmd), 0, sizeof(proc_wait.cmd));
    (void)memset_s(proc_wait.write_buf.buf, sizeof(proc_wait.write_buf.buf), 0, sizeof(proc_wait.write_buf.buf));
    proc_wait.write_buf.ppos = 0;
    proc_wait.write_buf.count = 0;
    osal_mutex_unlock(&g_mutex);
    return count;
}

td_s32 drv_common_module_init(td_void)
{
    td_s32 ret;

    ot_proc_dbg("Enter drv_common_module_init\n");
    ret = ot_proc_init();
    if (ret != 0) {
        osal_printk("%s - drv_common_module_init error!\n", __FUNCTION__);
    }
    return ret;
}

td_void drv_common_module_exit(td_void)
{
    ot_proc_dbg("Enter drv_common_module_exit\n");
    osal_mutex_destroy(&g_mutex);
    osal_deregisterdevice(g_ot_proc_dev);
    osal_destroydev(g_ot_proc_dev);
    del_list();
    return;
}

module_init(drv_common_module_init);
module_exit(drv_common_module_exit);
MODULE_LICENSE("Proprietary");
