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

#include "drv_klad_sw_utils.h"
#include "hal_klad.h"
#include "ot_debug_klad.h"
#include "klad_cfg.h"
#include "klad_reg_base.h"
#include "securec.h"
#include "drv_osal_init.h"
#include "cipher_ext.h"

#define KLAD_INVALID_VAL        0xffffffff

static klad_sw_mgmt g_klad_sw_mgmt[KLAD_MAX_SW_HANDLE];

static klad_sw_mgmt *_get_klad_sw_mgmt(td_u32 id)
{
    return &g_klad_sw_mgmt[id];
}

static td_void klad_sw_release_all(td_void)
{
    td_u32 i;
    klad_sw_mgmt *mgmt = TD_NULL;

    ot_klad_enter();
    (td_void)memset_s(g_klad_sw_mgmt, sizeof(g_klad_sw_mgmt), 0, sizeof(g_klad_sw_mgmt));
    for (i = 0; i < KLAD_MAX_SW_HANDLE; i++) {
        mgmt = _get_klad_sw_mgmt(i);
        mgmt->klad = KLAD_INVALID_VAL;
        mgmt->target = KLAD_INVALID_VAL;
        mgmt->open = TD_FALSE;
        mgmt->attach = TD_FALSE;
        mgmt->config = TD_FALSE;
    }
    ot_klad_exit();
}

static td_s32 klad_get_idle_handle(td_handle *klad)
{
    td_u32 i;
    klad_sw_mgmt *mgmt = TD_NULL;

    for (i = 0; i < KLAD_MAX_SW_HANDLE; i++) {
        mgmt = _get_klad_sw_mgmt(i);
        if (mgmt->open == TD_FALSE) {
            *klad = i;
            return TD_SUCCESS;
        }
    }

    ot_klad_error("klad is busy!\n");
    return OT_ERR_KLAD_DEVICE_BUSY;
}

#define klad_chk_handle(handle)                                 \
    do {                                                        \
        chk_handle_modid(handle, OT_ID_KLAD);                   \
        chk_handle_private_data(handle, OT_PRIVATE_ID_KLAD);    \
        chk_handle_chnid(handle, KLAD_MAX_SW_HANDLE);           \
    } while (0)

td_s32 klad_sw_init(td_void)
{
    td_s32 ret;
    td_u32 i;
    klad_sw_mgmt *mgmt = TD_NULL;

    ret = hal_klad_init();
    ot_klad_func_fail_return(hal_klad_init, ret != TD_SUCCESS, ret);

    (td_void)memset_s(g_klad_sw_mgmt, sizeof(g_klad_sw_mgmt), 0, sizeof(g_klad_sw_mgmt));
    for (i = 0; i < KLAD_MAX_SW_HANDLE; i++) {
        mgmt = _get_klad_sw_mgmt(i);
        mgmt->klad = KLAD_INVALID_VAL;
        mgmt->target = KLAD_INVALID_VAL;
        mgmt->open = TD_FALSE;
        mgmt->attach = TD_FALSE;
        mgmt->config = TD_FALSE;
        mgmt->owner = 0;
    }

    return ret;
}

td_void klad_sw_deinit(td_void)
{
    hal_klad_deinit();
    klad_sw_release_all();
}

td_s32 klad_sw_create(td_handle *klad)
{
    td_s32 ret;
    td_handle id;
    klad_sw_mgmt *mgmt = TD_NULL;

    ot_klad_enter();

    ret = klad_get_idle_handle(&id);
    ot_klad_func_fail_return(klad_get_idle_handle, ret != TD_SUCCESS, ret);

    /* open *klad handle */
    mgmt = _get_klad_sw_mgmt(id);
    (td_void)memset_s(mgmt, sizeof(klad_sw_mgmt), 0, sizeof(klad_sw_mgmt));
    mgmt->klad = id;
    mgmt->target = KLAD_INVALID_VAL;
    mgmt->open = TD_TRUE;
    mgmt->attach = TD_FALSE;
    mgmt->config = TD_FALSE;
    crypto_get_owner(&mgmt->owner);

    *klad = td_handle_init(OT_ID_KLAD, OT_PRIVATE_ID_KLAD, id);

    ot_klad_exit();

    return ret;
}

td_s32 klad_sw_destroy(td_handle klad)
{
    td_u32 id;
    td_s32 ret = TD_SUCCESS;
    klad_sw_mgmt *mgmt = TD_NULL;
    CRYPTO_OWNER owner;

    ot_klad_enter();

    klad_chk_handle(klad);

    id = td_handle_get_chnid(klad);
    mgmt = _get_klad_sw_mgmt(id);

    crypto_get_owner(&owner);
    ot_klad_formula_fail_return(mgmt->open != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->klad != id, OT_ERR_KLAD_INVALID_HANDLE);
    ot_klad_formula_fail_return(mgmt->attach != TD_FALSE, OT_ERR_KLAD_INVALID_PARAM);
    if (mgmt->owner != owner) {
        ot_klad_error("illegal uuid");
        return TD_FAILURE;
    }

    /* close *klad handle */
    (td_void)memset_s(mgmt, sizeof(klad_sw_mgmt), 0, sizeof(klad_sw_mgmt));
    mgmt->klad = KLAD_INVALID_VAL;
    mgmt->target = KLAD_INVALID_VAL;
    mgmt->open = TD_FALSE;
    mgmt->attach = TD_FALSE;
    mgmt->config = TD_FALSE;
    mgmt->owner = 0;

    ot_klad_exit();

    return ret;
}

td_s32 klad_sw_attach(td_handle klad, td_handle target)
{
    td_u32 klad_id;
    td_u32 target_id;
    td_s32 ret = TD_SUCCESS;
    klad_sw_mgmt *mgmt = TD_NULL;

    ot_klad_enter();

    klad_chk_handle(klad);

    klad_id = td_handle_get_chnid(klad);
    mgmt = _get_klad_sw_mgmt(klad_id);

    target_id = td_handle_get_chnid(target);
    if (td_handle_get_modid(target) == OT_ID_KEYSLOT) {
        ot_klad_formula_fail_return(td_handle_get_private_data(target) != OT_PRIVATE_ID_KEYSLOT,
            OT_ERR_KLAD_INVALID_HANDLE);
        ot_klad_formula_fail_return(target_id >= KEYSLOT_MAX_HANDLE, OT_ERR_KLAD_INVALID_HANDLE);
    } else {
        ot_klad_error("invalid target 0x%x\n", target);
        return OT_ERR_KLAD_INVALID_HANDLE;
    }

    ot_klad_formula_fail_return(mgmt->open != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->klad != klad_id, OT_ERR_KLAD_INVALID_PARAM);

    mgmt->target = target_id;
    mgmt->attach = TD_TRUE;

    ot_klad_exit();
    return ret;
}

td_s32 klad_sw_detach(td_handle klad, td_handle target)
{
    td_u32 klad_id;
    td_u32 target_id;
    td_s32 ret = TD_SUCCESS;
    klad_sw_mgmt *mgmt = TD_NULL;

    ot_klad_enter();

    klad_chk_handle(klad);

    klad_id = td_handle_get_chnid(klad);
    mgmt = _get_klad_sw_mgmt(klad_id);

    target_id = td_handle_get_chnid(target);
    if (td_handle_get_modid(target) == OT_ID_KEYSLOT) {
        ot_klad_formula_fail_return(td_handle_get_private_data(target) != OT_PRIVATE_ID_KEYSLOT,
            OT_ERR_KLAD_INVALID_HANDLE);
        ot_klad_formula_fail_return(target_id >= KEYSLOT_MAX_HANDLE, OT_ERR_KLAD_INVALID_HANDLE);
    } else {
        ot_klad_error("invalid target 0x%x\n", target);
        return OT_ERR_KLAD_INVALID_HANDLE;
    }

    ot_klad_formula_fail_return(mgmt->open != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->attach != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->klad != klad_id, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->target != target_id, OT_ERR_KLAD_INVALID_PARAM);

    mgmt->target = KLAD_INVALID_VAL;
    mgmt->attach = TD_FALSE;

    ot_klad_exit();
    return ret;
}

td_s32 klad_sw_set_attr(td_handle klad, const ot_klad_attr *attr)
{
    td_u32 id;
    td_s32 ret = TD_SUCCESS;
    klad_sw_mgmt *mgmt = TD_NULL;
    klad_common_slot *com_slot = TD_NULL;

    ot_klad_enter();

    klad_chk_handle(klad);

    id = td_handle_get_chnid(klad);
    mgmt = _get_klad_sw_mgmt(id);

    ot_klad_formula_fail_return(mgmt->open != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->klad != id, OT_ERR_KLAD_INVALID_PARAM);

    mgmt->type = attr->klad_cfg.klad_type;
    if (mgmt->type == OT_KLAD_TYPE_COMMON) {
        com_slot = &mgmt->klad_slot.common_slot;
        ret = memcpy_s(&com_slot->key_attr, sizeof(ot_klad_rootkey_attr),
            &attr->klad_cfg.rootkey_attr, sizeof(ot_klad_rootkey_attr));
        ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);
    }

    mgmt->config = TD_TRUE;

    ot_klad_exit();
    return ret;
}

td_s32 klad_sw_get_attr(td_handle klad, ot_klad_attr *attr)
{
    td_u32 id;
    td_s32 ret = TD_SUCCESS;
    klad_sw_mgmt *mgmt = TD_NULL;

    ot_klad_enter();

    klad_chk_handle(klad);

    id = td_handle_get_chnid(klad);
    mgmt = _get_klad_sw_mgmt(id);

    ot_klad_formula_fail_return(mgmt->open != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->config != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->klad != id, OT_ERR_KLAD_INVALID_PARAM);

    attr->klad_cfg.klad_type = mgmt->type;
    if (mgmt->type == OT_KLAD_TYPE_COMMON) {
        klad_common_slot *com_slot = TD_NULL;
        com_slot = &mgmt->klad_slot.common_slot;
        ret = memcpy_s(&attr->klad_cfg.rootkey_attr, sizeof(ot_klad_rootkey_attr),
            &com_slot->key_attr, sizeof(ot_klad_rootkey_attr));
        ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);
    }

    ot_klad_exit();
    return ret;
}

td_s32 klad_sw_set_session_key(td_handle klad, const ot_klad_session_key *session_key)
{
    td_u32 id;
    td_s32 ret;
    klad_sw_mgmt *mgmt = TD_NULL;
    klad_common_slot *com_slot = TD_NULL;

    ot_klad_enter();

    klad_chk_handle(klad);

    id = td_handle_get_chnid(klad);
    mgmt = _get_klad_sw_mgmt(id);

    ot_klad_formula_fail_return(session_key->level >= OT_KLAD_LEVEL_BUTT, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(session_key->alg >= OT_KLAD_ALG_TYPE_BUTT, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(session_key->key_size > OT_KLAD_MAX_KEY_LEN, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->open != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->attach != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->config != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->klad != id, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->type != OT_KLAD_TYPE_COMMON, OT_ERR_KLAD_INVALID_PARAM);

    com_slot = &mgmt->klad_slot.common_slot;

    /* session level must add 1 level by level */
    ot_klad_formula_fail_return(session_key->level != com_slot->klad_level, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(session_key->level + 1 > KLAD_MAX_SESSION_LEVEL, OT_ERR_KLAD_INVALID_PARAM);

    com_slot->klad_key[com_slot->klad_level].alg = session_key->alg;
    com_slot->klad_key[com_slot->klad_level].key_size = session_key->key_size;
    ret = memcpy_s(com_slot->klad_key[com_slot->klad_level].key, OT_KLAD_MAX_KEY_LEN,
        session_key->key, session_key->key_size);
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    /* klad level must add 1 level by level */
    com_slot->klad_level++;

    ot_klad_exit();
    return ret;
}

td_s32 klad_sw_set_content_key(td_handle klad, const ot_klad_content_key *content_key)
{
    td_u32 id;
    td_s32 ret;
    klad_sw_mgmt *mgmt = TD_NULL;
    klad_common_slot *com_slot = TD_NULL;

    ot_klad_enter();

    klad_chk_handle(klad);

    id = td_handle_get_chnid(klad);
    mgmt = _get_klad_sw_mgmt(id);

    ot_klad_formula_fail_return(content_key->alg >= OT_KLAD_ALG_TYPE_BUTT, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(content_key->key_size > OT_KLAD_MAX_KEY_LEN, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->open != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->attach != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->config != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->klad != id, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->type != OT_KLAD_TYPE_COMMON, OT_ERR_KLAD_INVALID_PARAM);

    com_slot = &mgmt->klad_slot.common_slot;
    ot_klad_formula_fail_return(com_slot->klad_level != KLAD_MAX_SESSION_LEVEL, OT_ERR_KLAD_INVALID_PARAM);

    com_slot->crypto_alg = content_key->crypto_alg;
    com_slot->klad_key[com_slot->klad_level].alg = content_key->alg;
    com_slot->klad_key[com_slot->klad_level].key_size = content_key->key_size;
    ret = memcpy_s(com_slot->klad_key[com_slot->klad_level].key, OT_KLAD_MAX_KEY_LEN,
        content_key->key, content_key->key_size);
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    /* klad level must add content level */
    com_slot->klad_level++;

    ret = hal_klad_common_route_startup(mgmt->target, com_slot);
    ot_klad_func_fail_goto(hal_klad_common_route_startup, ret != TD_SUCCESS, ret, exit__);

exit__:
    /* clean key */
    (td_void)memset_s(com_slot->klad_key, sizeof(com_slot->klad_key), 0, sizeof(com_slot->klad_key));

    ot_klad_exit();
    return ret;
}

td_s32 klad_sw_set_clear_key(td_handle klad, const ot_klad_clear_key *key)
{
    td_u32 id;
    td_s32 ret;
    klad_sw_mgmt *mgmt = TD_NULL;
    klad_clear_slot *clr_slot = TD_NULL;

    ot_klad_enter();

    klad_chk_handle(klad);

    id = td_handle_get_chnid(klad);
    mgmt = _get_klad_sw_mgmt(id);

    ot_klad_formula_fail_return(mgmt->open != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->attach != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->config != TD_TRUE, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->klad != id, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(mgmt->type != OT_KLAD_TYPE_CLEARCW, OT_ERR_KLAD_INVALID_PARAM);

    clr_slot = &mgmt->klad_slot.clear_slot;
    ret = memcpy_s(&clr_slot->clear_key, sizeof(ot_klad_clear_key),
        key, sizeof(ot_klad_clear_key));
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    ret = hal_klad_clear_route_startup(mgmt->target, clr_slot);
    ot_klad_func_fail_goto(hal_klad_clear_route_startup, ret != TD_SUCCESS, ret, exit__);

exit__:
    /* clean key */
    (td_void)memset_s(&clr_slot->clear_key, sizeof(ot_klad_clear_key), 0, sizeof(ot_klad_clear_key));

    ot_klad_exit();
    return ret;
}

td_void klad_sw_release(td_void)
{
    td_u32 i;
    klad_sw_mgmt *mgmt = TD_NULL;
    CRYPTO_OWNER owner;
    ot_klad_enter();
    for (i = 0; i < KLAD_MAX_SW_HANDLE; i++) {
        crypto_get_owner(&owner);
        mgmt = _get_klad_sw_mgmt(i);
        if (owner != mgmt->owner) {
            continue;
        }

        (td_void)memset_s(mgmt, sizeof(klad_sw_mgmt), 0, sizeof(klad_sw_mgmt));
        mgmt->klad = KLAD_INVALID_VAL;
        mgmt->target = KLAD_INVALID_VAL;
        mgmt->open = TD_FALSE;
        mgmt->attach = TD_FALSE;
        mgmt->config = TD_FALSE;
        mgmt->owner = 0;
    }
    ot_klad_exit();
}

