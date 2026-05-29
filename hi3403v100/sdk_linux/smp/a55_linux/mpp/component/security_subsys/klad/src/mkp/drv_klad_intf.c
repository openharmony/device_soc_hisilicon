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

#include "drv_klad_intf.h"
#include "ot_debug_klad.h"
#include "drv_ioctl_klad.h"
#include "ot_drv_klad.h"
#include "kapi_klad.h"
#include "securec.h"
#include "osal_ioctl.h"

typedef struct {
    const char *name;
    td_s32 (*func)(td_void *);
    td_u32 cmd;
} intf_klad_func;

static td_s32 intf_klad_create(td_void *argp)
{
    td_s32 ret;
    klad_ctl_handle *handle = (klad_ctl_handle *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(handle == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_create(&handle->klad);
    ot_klad_func_fail_return(ot_drv_klad_create, ret != TD_SUCCESS, ret);
    ot_klad_exit();

    return TD_SUCCESS;
}

static td_s32 intf_klad_destroy(td_void *argp)
{
    td_s32 ret;
    klad_ctl_handle *handle = (klad_ctl_handle *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(handle == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_destroy(handle->klad);
    ot_klad_func_fail_return(ot_drv_klad_destroy, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

static td_s32 intf_klad_attach(td_void *argp)
{
    td_s32 ret;
    klad_ctl_target *ctl_target = (klad_ctl_target *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(ctl_target == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_attach(ctl_target->klad, ctl_target->target);
    ot_klad_func_fail_return(ot_drv_klad_attach, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

static td_s32 intf_klad_detach(td_void *argp)
{
    td_s32 ret;
    klad_ctl_target *ctl_target = (klad_ctl_target *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(ctl_target == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_detach(ctl_target->klad, ctl_target->target);
    ot_klad_func_fail_return(ot_drv_klad_detach, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

static td_s32 intf_klad_set_attr(td_void *argp)
{
    td_s32 ret;
    klad_ctl_attr *ctl_attr = (klad_ctl_attr *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(ctl_attr == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_set_attr(ctl_attr->klad, &ctl_attr->attr);
    ot_klad_func_fail_return(ot_drv_klad_set_attr, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

static td_s32 intf_klad_get_attr(td_void *argp)
{
    td_s32 ret;
    klad_ctl_attr *ctl_attr = (klad_ctl_attr *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(ctl_attr == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_get_attr(ctl_attr->klad, &ctl_attr->attr);
    ot_klad_func_fail_return(ot_drv_klad_get_attr, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

static td_s32 intf_klad_set_session_key(td_void *argp)
{
    td_s32 ret;
    klad_ctl_session_key *session_key = (klad_ctl_session_key *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(session_key == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_set_session_key(session_key->klad, &session_key->key);
    ot_klad_func_fail_return(ot_drv_klad_set_session_key, ret != TD_SUCCESS, ret);

    /* clean key */
    (td_void)memset_s(&session_key->key, sizeof(ot_klad_session_key), 0, sizeof(ot_klad_session_key));

    ot_klad_exit();

    return TD_SUCCESS;
}

static td_s32 intf_klad_set_content_key(td_void *argp)
{
    td_s32 ret;
    klad_ctl_content_key *content_key = (klad_ctl_content_key *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(content_key == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_set_content_key(content_key->klad, &content_key->key);
    ot_klad_func_fail_goto(ot_drv_klad_set_content_key, ret != TD_SUCCESS, ret, exit__);

exit__:
    /* clean key */
    (td_void)memset_s(&content_key->key, sizeof(ot_klad_content_key), 0, sizeof(ot_klad_content_key));

    ot_klad_exit();

    return ret;
}

static td_s32 intf_klad_set_clear_key(td_void *argp)
{
    td_s32 ret;
    klad_ctl_clear_key *clear_key = (klad_ctl_clear_key *)argp;

    ot_klad_enter();

    ot_klad_formula_fail_return(clear_key == TD_NULL, OT_ERR_KLAD_INVALID_PARAM);

    ret = ot_drv_klad_set_clear_key(clear_key->klad, &clear_key->key);
    ot_klad_func_fail_goto(ot_drv_klad_set_clear_key, ret != TD_SUCCESS, ret, exit__);

exit__:

    /* clean key */
    (td_void)memset_s(&clear_key->key, sizeof(ot_klad_clear_key), 0, sizeof(ot_klad_clear_key));

    ot_klad_exit();

    return ret;
}

static intf_klad_func g_klad_func[] = {
    {"KladCreate",        intf_klad_create,          CMD_KLAD_CREATE_HANDLE},
    {"KladDestroy",       intf_klad_destroy,         CMD_KLAD_DESTROY_HANDLE},
    {"KladAttach",        intf_klad_attach,          CMD_KLAD_ATTACH_TARGET},
    {"KladDetach",        intf_klad_detach,          CMD_KLAD_DETACH_TARGET},
    {"KladSetAttr",       intf_klad_set_attr,        CMD_KLAD_SET_ATTR},
    {"KladGetAttr",       intf_klad_get_attr,        CMD_KLAD_GET_ATTR},
    {"KladSetSessionKey", intf_klad_set_session_key, CMD_KLAD_SESSION_KEY},
    {"KladSetContentKey", intf_klad_set_content_key, CMD_KLAD_CONTENT_KEY},
    {"KladSetClearKey",   intf_klad_set_clear_key,   CMD_KLAD_CLEAR_KEY},
};

td_s32 intf_klad_ioctl(td_u32 cmd, td_void *param)
{
    td_s32 ret;
    td_u32 nr;

    ot_klad_enter();

    nr = OSAL_IOC_NR(cmd);

    ot_klad_formula_fail_return(param == TD_NULL, OT_ERR_KLAD_NULL_PTR);
    ot_klad_formula_fail_return(nr >= CMD_KLAD_MAX_NUM, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(cmd != g_klad_func[nr].cmd, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(g_klad_func[nr].func == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    ot_klad_info("cmd %x, nr %u, dir %x\n", cmd, nr, OSAL_IOC_DIR(cmd));

    ret = g_klad_func[nr].func(param);
    if (ret != TD_SUCCESS) {
        ot_klad_error("call intf func failed, cmd %x, nr %d\n", cmd, nr);
        return ret;
    }

    ot_klad_exit();

    return ret;
}

td_s32 klad_entry(td_void)
{
    td_s32 ret;

    ret = kapi_klad_init();
    ot_klad_func_fail_return(kapi_klad_init, ret != TD_SUCCESS, ret);

    return TD_SUCCESS;
}

td_void klad_exit(td_void)
{
    kapi_klad_deinit();
}
