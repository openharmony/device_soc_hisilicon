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

#include "ot_osal.h"
#include "ot_type.h"
#include "ot_common.h"
#include "ot_debug.h"
#include "mod_ext.h"
#include "proc_ext.h"
#include "vb_ext.h"

static td_u32 g_logbuflen = 0;

struct osal_list_head g_mod_list;

td_char *cmpi_get_module_name(ot_mod_id mod_id)
{
    umap_module *tmp = TD_NULL;

    osal_list_for_each_entry(tmp, &g_mod_list, list) {
        if (tmp->mod_id == mod_id) {
            return tmp->mod_name;
        }
    }

    return NULL;
}

td_void *cmpi_get_module_func_by_id(ot_mod_id mod_id)
{
    umap_module *tmp = NULL;
    td_bool find = TD_FALSE;

    osal_list_for_each_entry(tmp, &g_mod_list, list) {
        if (tmp->mod_id == mod_id) {
            find = TD_TRUE;
            break;
        }
    }

    return (find == TD_TRUE) ? tmp->export_funcs : NULL;
}

/* just notify all modules to stop working.
 * after received notice "MOD_NOTICE_STOP", modules will
 * finish the task which is running and then stand by.
 * NOTE! returning from this call do __NOT__ mean
 * the module already stopped! you should invoke query_state
 * to check if the module is really stopped!
 */
td_void cmpi_stop_modules(td_void)
{
    umap_module *tmp = TD_NULL;

    osal_list_for_each_entry_reverse(tmp, &g_mod_list, list) {
        /* is the module registered */
        if (!tmp->pfn_notify) {
            continue;
        }

        tmp->pfn_notify(MOD_NOTICE_STOP);
    }
}

/* if all registered modules are free, return 0, otherwise return -1. */
td_s32 cmpi_query_modules(td_void)
{
    umap_module *tmp = TD_NULL;

    mod_state state;

    osal_list_for_each_entry_reverse(tmp, &g_mod_list, list) {
        /* is the module registered */
        if (!tmp->pfn_query_state) {
            continue;
        }

        state = MOD_STATE_BUSY;

        tmp->pfn_query_state(&state);

        if (state == MOD_STATE_FREE) {
            continue;
        }

        OT_TRACE(OT_DBG_INFO, OT_ID_CMPI,
                 "MOD [%s] is busy!\n", tmp->mod_name);
        return -1;
    }

    return 0;
}

/* caller must guarantee that this function can _not_
 * be invoked consecutively more than one time without calling
 * cmpi_init_modules.
 */
td_void cmpi_exit_modules(td_void)
{
    umap_module *tmp = TD_NULL;

    osal_list_for_each_entry_reverse(tmp, &g_mod_list, list) {
        /* is the module registered */
        if (!tmp->inited) {
            OT_TRACE(OT_DBG_ERR, OT_ID_CMPI, "MOD[%s] already exited!\n",
                     tmp->mod_name);
            continue;
        }

        tmp->pfn_exit();
        tmp->inited = TD_FALSE;

        OT_TRACE(OT_DBG_DEBUG, OT_ID_CMPI,
                 "MOD[%s] exit OK!\n", tmp->mod_name);
    }
    cmpi_log_sys_deinit();
}

/* this function has similar requirement than cmpi_exit_modules.  */
td_s32 cmpi_init_modules(td_void)
{
    umap_module *tmp = TD_NULL;
    umap_module *exit_tmp = TD_NULL;

    osal_list_for_each_entry(tmp, &g_mod_list, list) {
        /* already been inited */
        if (tmp->inited) {
            continue;
        }

        if (tmp->pfn_init(tmp->data) < 0) {
            OT_TRACE(OT_DBG_ERR, OT_ID_CMPI,
                     "MOD[%s] pfn_init failed!\n", tmp->mod_name);

            goto fail;
        }

        tmp->inited = TD_TRUE;
    }
    cmpi_log_sys_init();

    return 0;

fail:

    /* release modules inited before. */
    osal_list_for_each_entry_reverse(exit_tmp, &tmp->list, list) {
        if (!exit_tmp->inited) {
            continue;
        }

        exit_tmp->pfn_exit();

        exit_tmp->inited = TD_FALSE;
    }

    return -1;
}

td_s32 cmpi_register_module(umap_module *module)
{
    umap_module *tmp = TD_NULL;
    td_s32 ret;

    if (module == TD_NULL ||
        module->pfn_init == TD_NULL ||
        module->pfn_exit == TD_NULL) {
        OT_TRACE(OT_DBG_ERR, OT_ID_CMPI, "null ptr!\n");
        return -1;
    }

    ot_assert(module->mod_id < MAX_MPP_MODULES);

    /* check if module already registered */
    osal_list_for_each_entry(tmp, &g_mod_list, list) {
        if (tmp->mod_id == module->mod_id) {
            OT_TRACE(OT_DBG_ERR, OT_ID_CMPI, "MOD[%s] already registered!\n", module->mod_name);
            return -1;
        }
    }

    /* warn: pfn_init can't get mod_name */
    ret = module->pfn_init(module->data);
    if (ret < 0) {
        OT_TRACE(OT_DBG_ERR, OT_ID_CMPI, "MOD[%s] pfn_init failed!\n", module->mod_name);
        return -1;
    }

    module->inited = TD_TRUE;

    osal_list_add_tail(&module->list, &g_mod_list);

    return ret;
}

td_void cmpi_unregister_module(ot_mod_id mod_id)
{
    umap_module *tmp = TD_NULL;
    umap_module *_tmp = TD_NULL;

    ot_assert(mod_id < MAX_MPP_MODULES);

    osal_list_for_each_entry_safe(tmp, _tmp, &g_mod_list, list) {
        if (tmp->mod_id == mod_id) {
            if (tmp->inited) {
                tmp->pfn_exit();
            }

            osal_list_del(&tmp->list);

            OT_TRACE(OT_DBG_DEBUG,
                     OT_ID_CMPI, "MOD[%s] unregister OK!\n", tmp->mod_name);
            return;
        }
    }

    OT_TRACE(OT_DBG_ERR, OT_ID_CMPI, "MOD[%d] already unregister!\n", mod_id);
}

int comm_init(void)
{
    OSAL_INIT_LIST_HEAD(&g_mod_list);
    if (vb_init()) {
        OT_TRACE(OT_DBG_ERR, OT_ID_CMPI, "vb_init failed!\n");
    }
    if (cmpi_log_init(g_logbuflen)) {
        OT_TRACE(OT_DBG_ERR, OT_ID_CMPI, "log_init failed!\n");
        goto OUT;
    }

    OT_TRACE(OT_DBG_DEBUG, OT_ID_CMPI, "load base ... OK!\n");
    return TD_SUCCESS;

OUT:
    return TD_FAILURE;
}

void comm_exit(void)
{
    cmpi_log_exit();
    vb_exit();
    OT_TRACE(OT_DBG_DEBUG, OT_ID_CMPI, "Unload base ... OK!\n");
    return;
}
