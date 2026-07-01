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

#include "kapi_klad.h"
#include "drv_osal_init.h"
#include "drv_klad_sw_utils.h"
#include "ot_debug_klad.h"
#include "klad_reg_base.h"

#define kapi_klad_not_init_return(init)         \
    do {                                        \
        if ((init) == TD_FALSE) {                 \
            ot_klad_error("KLAD isn't init\n"); \
            return OT_ERR_KLAD_NOT_INIT;        \
        }                                       \
    } while (0)

typedef struct {
    td_bool init;
    KLAD_MUTEX_T lock;
} kapi_klad_mgmt;
static kapi_klad_mgmt g_kapi_klad_mgmt;

static kapi_klad_mgmt *_get_kapi_klad_mgmt(td_void)
{
    return &g_kapi_klad_mgmt;
}

td_s32 kapi_klad_init(td_void)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = TD_NULL;

    ret = klad_sw_init();
    ot_klad_func_fail_return(klad_sw_init, ret != TD_SUCCESS, ret);

    mgmt = _get_kapi_klad_mgmt();
    mgmt->init = TD_TRUE;
    klad_mutex_init(&mgmt->lock);

    return TD_SUCCESS;
}

td_void kapi_klad_deinit(td_void)
{
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    klad_sw_deinit();

    mgmt->init = TD_FALSE;
    klad_mutex_destroy(&mgmt->lock);
}

td_s32 kapi_klad_create(td_handle *klad)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    ot_klad_formula_fail_return(klad == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_create(klad);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_s32 kapi_klad_destroy(td_handle klad)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_destroy(klad);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_s32 kapi_klad_attach(td_handle klad, td_handle target)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_attach(klad, target);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_s32 kapi_klad_detach(td_handle klad, td_handle target)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_detach(klad, target);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_s32 kapi_klad_set_attr(td_handle klad, const ot_klad_attr *attr)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    ot_klad_formula_fail_return(attr == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_set_attr(klad, attr);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_s32 kapi_klad_get_attr(td_handle klad, ot_klad_attr *attr)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    ot_klad_formula_fail_return(attr == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_get_attr(klad, attr);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_s32 kapi_klad_set_session_key(td_handle klad, const ot_klad_session_key *key)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);
    /* 16 byte:logic only supports 128bit session key */
    ot_klad_formula_fail_return(key->key_size != 16, OT_ERR_KLAD_INVALID_PARAM);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_set_session_key(klad, key);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_s32 kapi_klad_set_content_key(td_handle klad, const ot_klad_content_key *key)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_set_content_key(klad, key);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_s32 kapi_klad_set_clear_key(td_handle klad, const ot_klad_clear_key *key)
{
    td_s32 ret;
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    kapi_klad_not_init_return(mgmt->init);

    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    klad_mutex_lock(&mgmt->lock);

    ret = klad_sw_set_clear_key(klad, key);

    klad_mutex_unlock(&mgmt->lock);

    return ret;
}

td_void kapi_klad_release(td_void)
{
    kapi_klad_mgmt *mgmt = _get_kapi_klad_mgmt();

    klad_mutex_lock(&mgmt->lock);

    klad_sw_release();

    klad_mutex_unlock(&mgmt->lock);
}
