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
#include "ot_drv_klad.h"

#include <linux/kernel.h>
#include "ot_debug_klad.h"
#include "kapi_klad.h"

#define crypto_unused(x)    ((x) = (x))

td_s32 ot_drv_klad_create(td_handle *klad)
{
    ot_klad_formula_fail_return(klad == TD_NULL, OT_ERR_KLAD_NULL_PTR);
    return kapi_klad_create(klad);
}
EXPORT_SYMBOL(ot_drv_klad_create);

td_s32 ot_drv_klad_destroy(td_handle klad)
{
    return kapi_klad_destroy(klad);
}
EXPORT_SYMBOL(ot_drv_klad_destroy);

td_s32 ot_drv_klad_attach(td_handle klad, td_handle target)
{
    return kapi_klad_attach(klad, target);
}
EXPORT_SYMBOL(ot_drv_klad_attach);

td_s32 ot_drv_klad_detach(td_handle klad, td_handle target)
{
    return kapi_klad_detach(klad, target);
}
EXPORT_SYMBOL(ot_drv_klad_detach);

td_s32 ot_drv_klad_set_attr(td_handle klad, const ot_klad_attr *attr)
{
    ot_klad_formula_fail_return(attr == TD_NULL, OT_ERR_KLAD_NULL_PTR);
    ot_klad_formula_fail_return(attr->klad_cfg.klad_type != OT_KLAD_TYPE_COMMON &&
        attr->klad_cfg.klad_type != OT_KLAD_TYPE_CLEARCW, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(attr->klad_cfg.rootkey_attr.key_sel < 0 ||
        attr->klad_cfg.rootkey_attr.key_sel > OT_KLAD_ROOTKEY_SEL_VENDOR, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(attr->klad_cfg.rootkey_attr.key_secure != OT_KLAD_ROOTKEY_SEC_REE &&
        attr->klad_cfg.rootkey_attr.key_secure != OT_KLAD_ROOTKEY_SEC_TEE, OT_ERR_KLAD_INVALID_PARAM);

    return kapi_klad_set_attr(klad, attr);
}
EXPORT_SYMBOL(ot_drv_klad_set_attr);

td_s32 ot_drv_klad_get_attr(td_handle klad, ot_klad_attr *attr)
{
    ot_klad_formula_fail_return(attr == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    return kapi_klad_get_attr(klad, attr);
}
EXPORT_SYMBOL(ot_drv_klad_get_attr);

td_s32 ot_drv_klad_set_session_key(td_handle klad, const ot_klad_session_key *key)
{
    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    return kapi_klad_set_session_key(klad, key);
}
EXPORT_SYMBOL(ot_drv_klad_set_session_key);

td_s32 ot_drv_klad_set_content_key(td_handle klad, const ot_klad_content_key *key)
{
    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);
    ot_klad_formula_fail_return(key->alg != OT_KLAD_ALG_TYPE_AES &&
        key->alg != OT_KLAD_ALG_TYPE_SM4, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(key->crypto_alg != OT_KLAD_CRYPTO_ALG_AES &&
        key->crypto_alg != OT_KLAD_CRYPTO_ALG_SM4, OT_ERR_KLAD_INVALID_PARAM);

    return kapi_klad_set_content_key(klad, key);
}
EXPORT_SYMBOL(ot_drv_klad_set_content_key);


td_s32 ot_drv_klad_set_clear_key(td_handle klad, const ot_klad_clear_key *key)
{
    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    return kapi_klad_set_clear_key(klad, key);
}
EXPORT_SYMBOL(ot_drv_klad_set_clear_key);
