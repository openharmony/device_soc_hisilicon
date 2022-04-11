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

#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>

#include "securec.h"
#include "hi_osal.h"
#include "hiproc.h"


typedef struct {
    wait_queue_head_t wq_for_read;
    wait_queue_head_t wq_for_write;
    wait_queue_head_t wq_for_cmd;
} proc_k_para;
static hi_s32 g_cmd_condition = 0;
static hi_s32 g_read_condition = 0;
static hi_s32 g_write_condition = 0;
static hi_s32 g_cmd_exit_condition = 0;

typedef struct entrylist {
    hi_u32 private_data;
    hi_char entry_name[MAX_PROC_NAME_LEN];
    struct entrylist *next;
} hi_entry_list;

static hi_entry_list *g_proc_head = HI_NULL;

static osal_mutex g_mutex = {
    .mutex = HI_NULL
};

static hi_proc_para proc_wait;
static proc_k_para g_proc_k_para;

static hi_s32 g_dbg_flag = 1;
#define hiproc_dbg(params...) do { \
        if (g_dbg_flag) {         \
            osal_printk(params);  \
        }                         \
    } while (0)

hi_void osal_proc_exit(hi_void);
static hi_s32 proc_read(void *seqfile, void *private);
static hi_s32 proc_write(osal_proc_entry *entry, const hi_char *buf, hi_s32 count, hi_s64 *ppos);

static hi_entry_list *add_list(const hi_char *name, const hi_u32 private_data)
{
    hi_entry_list *tmp;
    tmp = (hi_entry_list *)osal_kmalloc(0, sizeof(hi_entry_list), OSAL_GFP_KERNEL);
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

static hi_void get_list_and_delete_node(const hi_u32 private_data)
{
    hi_entry_list *tmp = g_proc_head;
    hi_entry_list *pre = g_proc_head;
    hi_entry_list *tmp1 = HI_NULL;
    while (tmp != HI_NULL) {
        if (tmp->private_data == private_data) {
            if (tmp == pre) { // del head
                g_proc_head = tmp->next;
                tmp1 = g_proc_head;
                pre = g_proc_head;
            } else {
                pre->next = tmp->next;
                tmp1 = pre->next;
            }
            osal_proc_remove(tmp->entry_name, strlen(tmp->entry_name));
            osal_kfree(0, tmp);
            tmp = tmp1;
            continue;
        }
        pre = tmp;
        tmp = tmp->next;
    }
    return;
}

static hi_s32 del_list_node(const hi_u32 private_data, const hi_char *name)
{
    hi_entry_list *tmp = g_proc_head;
    hi_entry_list *pre = g_proc_head;
    while (tmp != HI_NULL) {
        if (tmp->private_data == private_data && (!strncmp(tmp->entry_name, name, strlen(name)))) {
            if (tmp == pre) { // del head
                g_proc_head = tmp->next;
            } else {
                pre->next = tmp->next;
            }
            osal_kfree(0, tmp);
            tmp = HI_NULL;
            return HI_SUCCESS;
        }
        pre = tmp;
        tmp = tmp->next;
    }
    return HI_FAILURE;
}

static hi_void del_list(hi_void)
{
    hi_entry_list *tmp = g_proc_head;
    hi_entry_list *tmp2 = HI_NULL;

    while (tmp != HI_NULL) {
        tmp2 = tmp->next;
        osal_kfree(0, tmp);
        tmp = tmp2;
    }
    g_proc_head = HI_NULL;
}

static hi_s32 hiproc_open(hi_void *private_data)
{
    hiproc_dbg("Enter hiproc_open\n");
    osal_mutex_init(&g_mutex);
    init_waitqueue_head(&g_proc_k_para.wq_for_read);
    init_waitqueue_head(&g_proc_k_para.wq_for_write);
    init_waitqueue_head(&g_proc_k_para.wq_for_cmd);
    return HI_SUCCESS;
}

static hi_s32 hiproc_release(hi_void *private_data)
{
    hiproc_dbg("Enter hiproc_release\n");
    get_list_and_delete_node((hi_u32)(uintptr_t)private_data);
    return HI_SUCCESS;
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
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 hiproc_create_proc_entry(unsigned int cmd, hi_void *para, hi_void *private_data)
{
    hi_proc_name *proc_name = NULL;
    osal_proc_entry *entry_info = HI_NULL;
    proc_name = (hi_proc_name *)((uintptr_t)para);
    proc_name->name[sizeof(proc_name->name) - 1] = '\0';
    if (check_validate_name(proc_name->name) != HI_SUCCESS) {
        osal_printk("[%s,line:%d]Error: invalied name.\n", HIPROC_PFX, __LINE__);
        return HI_FAILURE;
    }
    entry_info = osal_proc_add(proc_name->name, strlen(proc_name->name));
    if (entry_info == HI_NULL) {
        osal_printk("[%s,line:%d]Error: can't create proc entry\n", HIPROC_PFX, __LINE__);
        return HI_FAILURE;
    }
    add_list(entry_info->name, (hi_u32)(uintptr_t)private_data);
    entry_info->read = proc_read;
    entry_info->write = proc_write;
    return HI_SUCCESS;
}

static hi_s32 hiproc_remove_proc_entry(unsigned int cmd, hi_void *para, hi_void *private_data)
{
    hi_proc_name *proc_name = NULL;
    hi_s32 ret;
    proc_name = (hi_proc_name *)((uintptr_t)para);
    proc_name->name[sizeof(proc_name->name) - 1] = '\0';
    if (check_validate_name(proc_name->name) != HI_SUCCESS) {
        osal_printk("[%s,line:%d]Error: invalied name.\n", HIPROC_PFX, __LINE__);
        return HI_FAILURE;
    }
    osal_proc_remove(proc_name->name, strlen(proc_name->name));
    ret = del_list_node((hi_u32)(uintptr_t)private_data, proc_name->name);
    if (ret != 0) {
        osal_printk("hiproc delete entry failed\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hiproc_get_cmd(unsigned int cmd, hi_void *para, hi_void *private_data)
{
    hi_proc_para *user_proc_info = HI_NULL;
    wait_event_interruptible(g_proc_k_para.wq_for_cmd, g_cmd_condition != 0);
    if (g_cmd_exit_condition == 1) {
        g_cmd_condition = 0;
        g_cmd_exit_condition = 0;
        return HI_FAILURE;
    }
    g_cmd_condition = 0;

    user_proc_info = (hi_proc_para *)((uintptr_t)para);
    if (((strlen(proc_wait.cmd) > 0) && (strlen(proc_wait.entry.name) > 0))) {
        (void)memcpy_s(&(user_proc_info->cmd), sizeof(user_proc_info->cmd), proc_wait.cmd,
            sizeof(proc_wait.cmd));
        (void)memcpy_s(&(user_proc_info->entry), sizeof(osal_proc_entry), &(proc_wait.entry),
            sizeof(osal_proc_entry));
        (void)memcpy_s(&(user_proc_info->write_buf.buf), sizeof(user_proc_info->write_buf.buf),
            proc_wait.write_buf.buf, sizeof(user_proc_info->write_buf.buf));
        user_proc_info->write_buf.ppos = proc_wait.write_buf.ppos;
        user_proc_info->write_buf.count = proc_wait.write_buf.count;
    } else {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hiproc_wake_read_task(unsigned int cmd, hi_void *para, hi_void *private_data)
{
    hi_proc_show_buf *show_buf = HI_NULL;
    show_buf = (hi_proc_show_buf *)((uintptr_t)para);
    if (strlen(proc_wait.entry.name) <= 0) {
        osal_printk("entry info is HI_NULL\n");
        return HI_FAILURE;
    }
    if (proc_wait.entry.read != HI_NULL) {
        osal_printk("entry read is not NULL\n");
        return HI_FAILURE;
    }
    if (show_buf == HI_NULL) {
        osal_printk("show_buf is HI_NULL\n");
        return HI_FAILURE;
    }
    if (show_buf->size > HI_PROC_BUF_SIZE) {
        osal_printk("show_buf (size = %u) is overflow\n", show_buf->size);
        return HI_FAILURE;
    }
    if (show_buf->buf == HI_NULL) {
        osal_printk("show_buf is NULL\n");
        return HI_FAILURE;
    }
    proc_wait.entry.read = osal_kmalloc(0, show_buf->size, OSAL_GFP_KERNEL);
    if (proc_wait.entry.read) {
        if (osal_copy_from_user(proc_wait.entry.read, (hi_void __user *)show_buf->buf, show_buf->size)) {
            osal_kfree(0, proc_wait.entry.read);
            proc_wait.entry.read = HI_NULL;
        }
    }
    g_read_condition = 1;
    wake_up_interruptible(&(g_proc_k_para.wq_for_read));
    return HI_SUCCESS;
}

static hi_s32 hiproc_wake_write_task(unsigned int cmd, hi_void *para, hi_void *private_data)
{
    g_write_condition = 1;
    wake_up_interruptible(&(g_proc_k_para.wq_for_write));
    return HI_SUCCESS;
}

static hi_s32 hiproc_wake_get_cmd(unsigned int cmd, hi_void *para, hi_void *private_data)
{
    g_cmd_condition = 1;
    g_cmd_exit_condition = 1;
    wake_up_interruptible(&(g_proc_k_para.wq_for_cmd));
    return HI_SUCCESS;
}

static osal_ioctl_cmd g_hiproc_cmd[] = {
    {USER_CREATE_PROC_ENTRY, hiproc_create_proc_entry},
    {USER_REMOVE_PROC_ENTRY, hiproc_remove_proc_entry},
    {USER_PROC_GET_CMD, hiproc_get_cmd},
    {USER_PROC_WAKE_READ_TASK, hiproc_wake_read_task},
    {USER_PROC_WAKE_WRITE_TASK, hiproc_wake_write_task},
    {USER_PROC_WAKE_GET_CMD, hiproc_wake_get_cmd},
};

static osal_fileops g_hiproc_fops = {
    .open = hiproc_open,
    .release = hiproc_release,
    .cmd_list = g_hiproc_cmd,
    .cmd_cnt = sizeof(g_hiproc_cmd) / sizeof(g_hiproc_cmd[0]),
};

static osal_dev g_hiproc_dev = {
    .name = HIPROC_DEVICE_NAME,
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &g_hiproc_fops,
};

static hi_s32 hiproc_init(hi_void)
{
    if (osal_dev_register(&g_hiproc_dev) != 0) {
        osal_printk("[%s,line:%d]Error: can't register\n", HIPROC_PFX, __LINE__);
        return HI_FAILURE;
    }

    hiproc_dbg("hi_proc init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return HI_SUCCESS;
}

static hi_s32 proc_read(void *seqfile, void *private)
{
    struct seq_file *s = seqfile;
    osal_proc_entry *entry_info = s->private;
    DEFINE_WAIT(wait);

    osal_mutex_lock(&g_mutex);
    /* only these two parameters are used */
    (void)memcpy_s(&(proc_wait.entry.name), sizeof(proc_wait.entry.name), entry_info->name,
        sizeof(proc_wait.entry.name));
    (void)memcpy_s(&(proc_wait.cmd), sizeof(proc_wait.cmd), HI_USER_PROC_READ_CMD, strlen(HI_USER_PROC_READ_CMD) + 1);

    g_cmd_condition = 1;
    wake_up_interruptible(&(g_proc_k_para.wq_for_cmd));
    wait_event_interruptible(g_proc_k_para.wq_for_read, g_read_condition != 0);
    g_read_condition = 0;

    if (proc_wait.entry.read != HI_NULL) {
        osal_proc_print(seqfile, "%s", (hi_char *)proc_wait.entry.read);
        osal_kfree(0, proc_wait.entry.read);
        proc_wait.entry.read = HI_NULL;
    }
    (void)memset_s(&(proc_wait.entry), sizeof(osal_proc_entry), 0, sizeof(osal_proc_entry));
    (void)memset_s(&(proc_wait.cmd), sizeof(proc_wait.cmd), 0, sizeof(proc_wait.cmd));
    osal_mutex_unlock(&g_mutex);
    return HI_SUCCESS;
}

static hi_s32 proc_write(osal_proc_entry *entry, const hi_char *buf, hi_s32 count, hi_s64 *ppos)
{
    DEFINE_WAIT(wait);
    osal_mutex_lock(&g_mutex);
    (void)memcpy_s(&(proc_wait.cmd), sizeof(proc_wait.cmd), HI_USER_PROC_WRITE_CMD, strlen(HI_USER_PROC_WRITE_CMD) + 1);
    (void)memcpy_s(&(proc_wait.entry), sizeof(osal_proc_entry), entry, sizeof(osal_proc_entry));
    osal_copy_from_user(proc_wait.write_buf.buf, buf, sizeof(proc_wait.write_buf.buf));
    proc_wait.write_buf.ppos = *ppos;
    proc_wait.write_buf.count = count;

    g_cmd_condition = 1;
    wake_up_interruptible(&(g_proc_k_para.wq_for_cmd));
    wait_event_interruptible(g_proc_k_para.wq_for_write, g_write_condition != 0);
    g_write_condition = 0;

    (void)memset_s(&(proc_wait.entry), sizeof(osal_proc_entry), 0, sizeof(osal_proc_entry));
    (void)memset_s(&(proc_wait.cmd), sizeof(proc_wait.cmd), 0, sizeof(proc_wait.cmd));
    (void)memset_s(proc_wait.write_buf.buf, sizeof(proc_wait.write_buf.buf), 0, sizeof(proc_wait.write_buf.buf));
    proc_wait.write_buf.ppos = 0;
    proc_wait.write_buf.count = 0;
    osal_mutex_unlock(&g_mutex);
    return count;
}

static hi_s32 drv_common_module_init(hi_void)
{
    hi_s32 ret;

    hiproc_dbg("Enter drv_common_module_init\n");
    ret = hiproc_init();
    if (ret != 0) {
        osal_printk("%s - drv_common_module_init error!\n", __FUNCTION__);
    }
    return ret;
}

static hi_void drv_common_module_exit(hi_void)
{
    hiproc_dbg("Enter drv_common_module_exit\n");
    osal_mutex_destory(&g_mutex);
    osal_dev_unregister(&g_hiproc_dev);
    del_list();
    return;
}

module_init(drv_common_module_init);
module_exit(drv_common_module_exit);
