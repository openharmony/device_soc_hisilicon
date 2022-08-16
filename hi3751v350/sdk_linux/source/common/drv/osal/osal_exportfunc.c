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

#include "hi_osal.h"

#include <linux/slab.h>
#include <linux/spinlock.h>
#include "securec.h"

int hi_drv_module_register(unsigned int module_id, const char *pu8_module_name, void *p_func);
int hi_drv_module_un_register(unsigned int module_id);
int hi_drv_module_get_function(unsigned int module_id, void **pp_func);

typedef struct exportfunc_node_ {
    unsigned int module_id;
    void *func;
    struct osal_list_head node;
}exportfunc_node;

static DEFINE_SPINLOCK(g_func_lock);
static OSAL_LIST_HEAD(g_func_list);

void *osal_exportfunc_find_node(unsigned int module_id, struct osal_list_head *list)
{
    exportfunc_node *func_node = NULL;

    osal_list_for_each_entry(func_node, list, node) {
        if (func_node->module_id == module_id) {
            return func_node;
        }
    }

    return NULL;
}

int osal_exportfunc_register(unsigned int module_id, const char *name, void *func)
{
    int ret = 0;
    unsigned long flags;
    exportfunc_node *func_node_new  = NULL;
    exportfunc_node *func_node_find = NULL;

#ifndef CFG_HI_USER_DRV
    return hi_drv_module_register(module_id, name, func);
#endif

    func_node_new = kmalloc(sizeof(exportfunc_node), GFP_KERNEL);
    if (func_node_new == NULL) {
        return -1;
    }

    ret = memset_s(func_node_new, sizeof(exportfunc_node), 0, sizeof(exportfunc_node));
    if (ret != 0) {
        return -1;
    }

    spin_lock_irqsave(&g_func_lock, flags);

    func_node_find = osal_exportfunc_find_node(module_id, &g_func_list);
    if (func_node_find != NULL) {
        kfree(func_node_new);
        spin_unlock_irqrestore(&g_func_lock, flags);
        return -1;
    }

    func_node_new->func = func;
    func_node_new->module_id = module_id;

    osal_list_add_tail(&(func_node_new->node), &g_func_list);

    spin_unlock_irqrestore(&g_func_lock, flags);

    return 0;
}
EXPORT_SYMBOL(osal_exportfunc_register);

int osal_exportfunc_unregister(unsigned int module_id)
{
    unsigned long flags;
    exportfunc_node *func_node_find = NULL;

#ifndef CFG_HI_USER_DRV
    return hi_drv_module_un_register(module_id);
#endif

    spin_lock_irqsave(&g_func_lock, flags);

    func_node_find = osal_exportfunc_find_node(module_id, &g_func_list);
    if (func_node_find != NULL) {
        spin_unlock_irqrestore(&g_func_lock, flags);
        return -1;
    }

    osal_list_del(&func_node_find->node);
    kfree(func_node_find);

    spin_unlock_irqrestore(&g_func_lock, flags);

    return 0;
}
EXPORT_SYMBOL(osal_exportfunc_unregister);

int osal_exportfunc_get(unsigned int module_id, void **func)
{
    unsigned long flags;
    exportfunc_node *func_node_find = NULL;

    if (func == NULL) {
        return -1;
    }

#ifndef CFG_HI_USER_DRV
    return hi_drv_module_get_function(module_id, func);
#endif

    spin_lock_irqsave(&g_func_lock, flags);

    func_node_find = osal_exportfunc_find_node(module_id, &g_func_list);
    if (func_node_find == NULL) {
        *func = NULL;
        spin_unlock_irqrestore(&g_func_lock, flags);
        return -1;
    }

    *func = func_node_find->func;

    spin_unlock_irqrestore(&g_func_lock, flags);

    return 0;
}
EXPORT_SYMBOL(osal_exportfunc_get);
