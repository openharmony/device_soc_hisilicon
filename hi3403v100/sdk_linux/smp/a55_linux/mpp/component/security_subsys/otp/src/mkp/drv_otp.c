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

#include "drv_otp.h"
#include "drv_lib.h"
#include "drv_osal_init.h"
#include "securec.h"
#include "hal_otp.h"
#include "ot_debug_otp.h"
#include "drv_ioctl_otp.h"

#define OEM_ROOT_SYMC_KEY_NAME_LEN 18
#define OEM_ROOT_SYMC_KEY0 "oem_root_symc_key0"
#define OEM_ROOT_SYMC_KEY1 "oem_root_symc_key1"
#define OEM_ROOT_SYMC_KEY2 "oem_root_symc_key2"
#define OEM_ROOT_SYMC_KEY3 "oem_root_symc_key3"

typedef struct {
    td_u32 offset;
    td_u8 *value;
    td_u32 value_len;
    td_u32 *word_value;
    td_u32 word_len;
    td_u32 byte_len;
} drv_data_param_s;

typedef enum {
    DRV_DATA_TYPE_ONEWAY,
    DRV_DATA_TYPE_LOCKABLE,
} drv_data_type_e;

static otp_data_item *_drv_otp_match_field_name(const td_char *field_name)
{
    td_u32 i;
    otp_data_item *item = otp_get_data_item();

    for (i = 0; i < otp_get_data_item_num(); i++) {
        if (strncmp(field_name, item[i].field_name, strlen(item[i].field_name) + 1) != 0) {
            continue;
        }

        ot_otp_info("match %d field name %s\n", i, field_name);
        return &item[i];
    }
    ot_otp_error("field name %s don't match\n", field_name);
    return TD_NULL;
}

static td_s32 _drv_otp_chk_data_param(const otp_data_item *item, td_u32 offset, td_u32 value_len)
{
    /* offset must word align */
    ot_otp_formula_fail_return(!align_word(offset), OT_ERR_OTP_INVALID_PARAM);

    /* avoid a + b overflow */
    ot_otp_formula_fail_return(offset + value_len < value_len, OT_ERR_OTP_INVALID_PARAM);

    /* check user name real max size: offset + value_len can't longer than max size */
    ot_otp_formula_fail_return(offset + value_len > byte_number(item->bit_width), OT_ERR_OTP_INVALID_PARAM);

    return TD_SUCCESS;
}

static td_s32 _drv_otp_init_data_param(drv_data_param_s *user_data,
    const otp_data_item *item, td_u32 offset, td_u8 *value, td_u32 value_len)
{
    td_s32 ret;

    ret = _drv_otp_chk_data_param(item, offset, value_len);
    ot_otp_func_fail_return(_drv_otp_chk_data_param, ret != TD_SUCCESS, ret);

    (td_void)memset_s(user_data, sizeof(drv_data_param_s), 0, sizeof(drv_data_param_s));
    user_data->offset = offset;
    user_data->value = value;
    user_data->value_len = value_len;
    user_data->word_len = word_number(value_len);
    user_data->byte_len = user_data->word_len * WORD_BYTE_WIDTH;

    return TD_SUCCESS;
}

static td_s32 _drv_otp_set_user_data(drv_data_param_s *user_data, const otp_data_item *item)
{
    td_u32 i;
    td_u32 word_offset;
    td_s32 ret;

    ret = memcpy_s((td_u8 *)user_data->word_value, user_data->byte_len,
        user_data->value, user_data->value_len);
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    ot_otp_print_arr_u32("set user data", user_data->word_value, user_data->word_len);

    /* tee cpu can set ree data, ree cpu can't set tee data */
    if ((item->attr & OTP_ATTR_REE_USER_DATA) || (otp_is_secure_cpu() &&
        ((item->attr & OTP_ATTR_TEE_DATA_REE_READ) || (item->attr & OTP_ATTR_TEE_USER_DATA)))) {
        for (i = 0; i < user_data->word_len; i++) {
            /* user data word offset is sum of the input offset address plus the data offset address */
            word_offset = (user_data->offset + byte_number(item->offset)) / WORD_BYTE_WIDTH + i;

            ret = hal_otp_set_user_data_word(word_offset, user_data->word_value[i], TD_FALSE);
            ot_otp_func_fail_return(hal_otp_set_user_data_word, ret != TD_SUCCESS, ret);
        }
    } else {
        ot_otp_error("%s don't match attribution %x\n", item->field_name, item->attr);
        return OT_ERR_OTP_INVALID_FIELD_NAME;
    }

    return TD_SUCCESS;
}

static td_s32 _drv_otp_get_user_data(drv_data_param_s *user_data, const otp_data_item *item)
{
    td_u32 i;
    td_u32 word_offset;
    td_s32 ret;

    /* tee cpu can get ree data, ree cpu only get tee data which allowed */
    if ((item->attr & OTP_ATTR_REE_USER_DATA) || (item->attr & OTP_ATTR_TEE_DATA_REE_READ) ||
        (otp_is_secure_cpu() && (item->attr & OTP_ATTR_TEE_USER_DATA))) {
        for (i = 0; i < user_data->word_len; i++) {
            /* user data word offset is sum of the input offset address plus the data offset address */
            word_offset = (user_data->offset + byte_number(item->offset)) / WORD_BYTE_WIDTH + i;

            ret = hal_otp_get_user_data_word(word_offset, &user_data->word_value[i]);
            ot_otp_func_fail_return(hal_otp_get_user_data_word, ret != TD_SUCCESS, ret);
        }
    } else {
        ot_otp_error("%s don't match attribution %x\n", item->field_name, item->attr);
        return OT_ERR_OTP_INVALID_FIELD_NAME;
    }

    ret = memcpy_s(user_data->value, user_data->value_len, (td_u8 *)user_data->word_value, user_data->value_len);
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    return TD_SUCCESS;
}

#ifdef OT_OTP_V200
static td_s32 _drv_otp_burn_data_flag(const ot_otp_burn_pv_item *pv_item,
    const otp_data_item *data_item, drv_data_type_e type)
{
    td_s32 ret;
    td_u32 value = 0;

    /* data flag length can't be larger than 4bytes */
    ot_otp_formula_fail_return(pv_item->value_len > WORD_BIT_WIDTH, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(data_item->bit_width > WORD_BIT_WIDTH, OT_ERR_OTP_INVALID_PARAM);

    ret = memcpy_s((td_u8 *)&value, sizeof(td_u32), pv_item->value, byte_number(pv_item->value_len));
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    value = data_bit_mask(value, data_item->offset, data_item->bit_width);
    ot_otp_info("data flag offset %u, data %08x\n", data_item->offset, value);

    if (type == DRV_DATA_TYPE_ONEWAY) {
        ret = hal_otp_set_user_data_word(data_item->offset / WORD_BIT_WIDTH, value, TD_FALSE);
        ot_otp_func_fail_return(hal_otp_set_user_data_word, ret != TD_SUCCESS, ret);
    } else { /* DRV_DATA_TYPE_LOCKABLE */
        td_bool lock;
        ret = hal_otp_get_user_data_lock_sta(data_item->offset / WORD_BIT_WIDTH, &lock);
        ot_otp_func_fail_return(hal_otp_get_user_data_lock_sta, ret != TD_SUCCESS, ret);
        if (lock == TD_TRUE) {
            ot_otp_error("flag %s has been locked\n", pv_item->field_name);
            return OT_ERR_OTP_ZONE_LOCKED;
        }

        ret = hal_otp_set_user_data_word(data_item->offset / WORD_BIT_WIDTH, value, pv_item->lock);
        ot_otp_func_fail_return(hal_otp_set_user_data_word, ret != TD_SUCCESS, ret);
    }

    return TD_SUCCESS;
}

static td_s32 _drv_otp_read_data_flag(ot_otp_burn_pv_item *pv_item,
    const otp_data_item *data_item, drv_data_type_e type)
{
    td_s32 ret;
    td_u32 value;

    /* data flag length can't be larger than 4bytes */
    ot_otp_formula_fail_return(pv_item->value_len > WORD_BIT_WIDTH, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(data_item->bit_width > WORD_BIT_WIDTH, OT_ERR_OTP_INVALID_PARAM);

    if (type == DRV_DATA_TYPE_LOCKABLE) {
        ret = hal_otp_get_user_data_lock_sta(data_item->offset / WORD_BIT_WIDTH, &pv_item->lock);
        ot_otp_func_fail_return(hal_otp_get_user_data_lock_sta, ret != TD_SUCCESS, ret);
    }

    ret = hal_otp_get_user_data_word(data_item->offset / WORD_BIT_WIDTH, &value);
    ot_otp_func_fail_return(hal_otp_get_user_data_word, ret != TD_SUCCESS, ret);

    value = data_bit_restore(value, data_item->offset, data_item->bit_width);
    ot_otp_info("data flag offset %u, data %08x, lock_sta %d\n", data_item->offset, value, pv_item->lock);

    ret = memcpy_s(pv_item->value, byte_number(pv_item->value_len),
        (td_u8 *)&value, byte_number(data_item->bit_width));
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    return TD_SUCCESS;
}

static td_void _drv_otp_cfg_user_data_lock(td_bool lock_sta, td_u32 index, ot_otp_lock_status *lock)
{
    /* init lock value when index == 0, index = 0 must be called */
    if (index == 0) {
        if (lock_sta == TD_TRUE) {
            *lock = OT_OTP_STA_ALL_LOCKED;
        } else {
            *lock = OT_OTP_STA_ALL_UNLOCKED;
        }
    } else {
        if ((lock_sta == TD_TRUE && *lock == OT_OTP_STA_ALL_UNLOCKED) ||
            (lock_sta == TD_FALSE && *lock == OT_OTP_STA_ALL_LOCKED)) {
            *lock = OT_OTP_STA_PARTIAL_LOCKED;
        }
    }
}
#endif

td_s32 drv_otp_init(void)
{
    td_s32 ret;

    ret = hal_otp_init();
    ot_otp_func_fail_return(hal_otp_init, ret != TD_SUCCESS, ret);

    return TD_SUCCESS;
}

td_void drv_otp_deinit(void)
{
    hal_otp_deinit();
}

td_s32 drv_otp_set_user_data(const td_char *field_name,
    td_u32 offset, const td_u8 *value, td_u32 value_len)
{
    td_s32 ret;
    const otp_data_item *item = TD_NULL;
    drv_data_param_s user_data;

    ot_otp_enter();

    /* match field name */
    item = _drv_otp_match_field_name(field_name);
    ot_otp_func_fail_return(_drv_otp_match_field_name, item == TD_NULL, OT_ERR_OTP_INVALID_FIELD_NAME);

    ret = _drv_otp_init_data_param(&user_data, item, offset, (td_u8 *)value, value_len);
    ot_otp_func_fail_return(_drv_otp_init_data_param, ret != TD_SUCCESS, ret);

    user_data.word_value = otp_malloc(user_data.byte_len);
    ot_otp_func_fail_return(otp_malloc, user_data.word_value == TD_NULL, OT_ERR_OTP_FAILED_MEM);
    (td_void)memset_s(user_data.word_value, user_data.byte_len, 0, user_data.byte_len);

    ret = _drv_otp_set_user_data(&user_data, item);

    otp_free(user_data.word_value);

    ot_otp_exit();

    return ret;
}

td_s32 drv_otp_get_user_data(const td_char *field_name,
    td_u32 offset, td_u8 *value, td_u32 value_len)
{
    td_s32 ret;
    const otp_data_item *item = TD_NULL;
    drv_data_param_s user_data;

    ot_otp_enter();

    /* match field name */
    item = _drv_otp_match_field_name(field_name);
    ot_otp_func_fail_return(_drv_otp_match_field_name, item == TD_NULL, OT_ERR_OTP_INVALID_FIELD_NAME);

    ret = _drv_otp_init_data_param(&user_data, item, offset, value, value_len);
    ot_otp_func_fail_return(_drv_otp_init_data_param, ret != TD_SUCCESS, ret);

    user_data.word_value = otp_malloc(user_data.byte_len);
    ot_otp_func_fail_return(otp_malloc, user_data.word_value == TD_NULL, OT_ERR_OTP_FAILED_MEM);
    (td_void)memset_s(user_data.word_value, user_data.byte_len, 0, user_data.byte_len);

    ret = _drv_otp_get_user_data(&user_data, item);

    otp_free(user_data.word_value);

    ot_otp_exit();
    return ret;
}

td_s32 drv_otp_set_user_data_lock(const td_char *field_name,
    td_u32 offset, td_u32 value_len)
{
#ifdef OT_OTP_V200
    td_s32 ret;
    td_u32 i;
    td_u32 word_offset;
    td_u32 value;
    td_bool lock_sta;
    const otp_data_item *item = TD_NULL;

    item = _drv_otp_match_field_name(field_name);
    ot_otp_func_fail_return(_drv_otp_match_field_name, item == TD_NULL, OT_ERR_OTP_INVALID_FIELD_NAME);

    ret = _drv_otp_chk_data_param(item, offset, value_len);
    ot_otp_func_fail_return(_drv_otp_chk_data_param, ret != TD_SUCCESS, ret);

    /* tee cpu can set ree data lock, ree cpu can't set tee data lock */
    if ((item->attr == OTP_ATTR_REE_USER_DATA) ||
        (otp_is_secure_cpu() && (item->attr == OTP_ATTR_TEE_USER_DATA))) {
        for (i = 0; i < word_number(value_len); i++) {
            /* user data word offset is sum of the input offset address plus the data offset address */
            word_offset = round(offset + byte_number(item->offset), WORD_BYTE_WIDTH) + i;

            /* get word offset lock status */
            ret = hal_otp_get_user_data_lock_sta(word_offset, &lock_sta);
            ot_otp_func_fail_return(hal_otp_get_user_data_lock_sta, ret != TD_SUCCESS, ret);
            if (lock_sta == TD_TRUE) {
                ot_otp_warn("user data %u had locked\n", word_offset);
                continue;
            }

            /* get word offset value */
            ret = hal_otp_get_user_data_word(word_offset, &value);
            ot_otp_func_fail_return(hal_otp_set_user_data_word, ret != TD_SUCCESS, ret);

            /* lock word offset value */
            ret = hal_otp_set_user_data_word(word_offset, value, TD_TRUE);
            ot_otp_func_fail_return(hal_otp_set_user_data_word, ret != TD_SUCCESS, ret);
        }
    } else {
        ot_otp_error("%s don't match attribution %x\n", item->field_name, item->attr);
        return OT_ERR_OTP_INVALID_FIELD_NAME;
    }
    return TD_SUCCESS;
#else
    ot_unused(field_name);
    ot_unused(offset);
    ot_unused(value_len);
    ot_otp_error("Unsupported set user data lock!\n");
    return OT_ERR_OTP_FUNC_UNSUPPORT;
#endif
}

td_s32 drv_otp_get_user_data_lock(const td_char *field_name,
    td_u32 offset, td_u32 value_len, ot_otp_lock_status *lock)
{
#ifdef OT_OTP_V200
    td_s32 ret;
    td_u32 i;
    td_u32 word_offset;
    td_bool lock_sta;
    const otp_data_item *item = TD_NULL;
    ot_otp_lock_status tmp_lock = OT_OTP_STA_ALL_UNLOCKED;

    item = _drv_otp_match_field_name(field_name);
    ot_otp_func_fail_return(_drv_otp_match_field_name, item == TD_NULL, OT_ERR_OTP_INVALID_FIELD_NAME);

    ret = _drv_otp_chk_data_param(item, offset, value_len);
    ot_otp_func_fail_return(_drv_otp_chk_data_param, ret != TD_SUCCESS, ret);

    /* tee cpu can set gee data lock, ree cpu can't get tee data lock */
    if ((item->attr == OTP_ATTR_REE_USER_DATA) ||
        (otp_is_secure_cpu() && (item->attr == OTP_ATTR_TEE_USER_DATA))) {
        for (i = 0; i < word_number(value_len); i++) {
            /* user data word offset is sum of the input offset address plus the data offset address */
            word_offset = round(offset + byte_number(item->offset), WORD_BYTE_WIDTH) + i;

            /* get word offset lock status */
            ret = hal_otp_get_user_data_lock_sta(word_offset, &lock_sta);
            ot_otp_func_fail_return(hal_otp_get_user_data_lock_sta, ret != TD_SUCCESS, ret);

            /* get lock status */
            _drv_otp_cfg_user_data_lock(lock_sta, i, &tmp_lock);
        }
        *lock = tmp_lock;
    } else {
        ot_otp_error("%s don't match attribution %x\n", item->field_name, item->attr);
        return OT_ERR_OTP_INVALID_FIELD_NAME;
    }

    return ret;
#else
    ot_unused(field_name);
    ot_unused(offset);
    ot_unused(value_len);
    ot_unused(lock);
    ot_otp_error("Unsupported set user data lock!\n");
    return OT_ERR_OTP_FUNC_UNSUPPORT;
#endif
}

static td_bool drv_otp_check_field_name(const ot_otp_burn_pv_item *pv)
{
    td_s32 value_len = sizeof(pv->value) / sizeof(pv->value[0]);
    td_s32 i;
    if (strcmp(pv->field_name, OEM_ROOT_SYMC_KEY0) == 0 ||
        strcmp(pv->field_name, OEM_ROOT_SYMC_KEY1) == 0 ||
        strcmp(pv->field_name, OEM_ROOT_SYMC_KEY2) == 0 ||
        strcmp(pv->field_name, OEM_ROOT_SYMC_KEY3) == 0) {
        for (i = 0; i < value_len; i++) {
            if (pv->value[i] != 0x00) {
                return TD_TRUE;
            }
        }
        return TD_FALSE;
    } else {
        return TD_TRUE;
    }
}

td_s32 drv_otp_burn_product_pv(const ot_otp_burn_pv_item *pv, td_u32 num)
{
    td_s32 ret;
    td_u32 i;
    const otp_data_item *item = TD_NULL;

    for (i = 0; i < num; i++) {
        item = _drv_otp_match_field_name(pv[i].field_name);
        ot_otp_func_fail_return(_drv_otp_match_field_name, item == TD_NULL, OT_ERR_OTP_INVALID_FIELD_NAME);
        /* check k0-k3 is all 0 */
        ot_otp_formula_fail_return(drv_otp_check_field_name(&pv[i]) == TD_FALSE, OT_ERR_OTP_INVALID_PARAM);
        /* value_len must is equal to byte len */
        ot_otp_formula_fail_return(pv[i].value_len != item->bit_width, OT_ERR_OTP_INVALID_PARAM);

        if (item->attr & OTP_ATTR_BURN_KEY) {
            ret = hal_otp_burn_key(item, pv[i].value, byte_number(pv[i].value_len));
            ot_otp_func_fail_return(hal_otp_burn_key, ret != TD_SUCCESS, ret);
        } else if (item->attr & OTP_ATTR_SPECIFY_FLAG) {
            ret = hal_otp_enable_flag(item, pv[i].value[0], pv[i].lock);
            ot_otp_func_fail_return(hal_otp_enable_flag, ret != TD_SUCCESS, ret);
#ifdef OT_OTP_V200
        } else if (item->attr & OTP_ATTR_DATA_FLAG_ONEWARY) {
            ret = _drv_otp_burn_data_flag(&pv[i], item, DRV_DATA_TYPE_ONEWAY);
            ot_otp_func_fail_return(_drv_otp_burn_data_flag, ret != TD_SUCCESS, ret);
        } else if (item->attr & OTP_ATTR_DATA_FLAG_LOCKABLE) {
            ret = _drv_otp_burn_data_flag(&pv[i], item, DRV_DATA_TYPE_LOCKABLE);
            ot_otp_func_fail_return(_drv_otp_burn_data_flag, ret != TD_SUCCESS, ret);
#endif
        } else {
            ot_otp_error("%s don't match attribution %x\n", item->field_name, item->attr);
            return OT_ERR_OTP_INVALID_FIELD_NAME;
        }
    }

    return TD_SUCCESS;
}

td_s32 drv_otp_read_product_pv(ot_otp_burn_pv_item *pv, td_u32 num)
{
    td_u32 i;
    td_s32 ret;
    const otp_data_item *item = TD_NULL;

    for (i = 0; i < num; i++) {
        item = _drv_otp_match_field_name(pv[i].field_name);
        ot_otp_func_fail_return(_drv_otp_match_field_name, item == TD_NULL, OT_ERR_OTP_INVALID_FIELD_NAME);

        /* clean pv[i].value data */
        (td_void)memset_s(pv[i].value, sizeof(pv[i].value), 0, sizeof(pv[i].value));

        if (item->attr & OTP_ATTR_BURN_KEY) {
            /* read burn key only get lock flag, value_len can be 0 */
            ret = hal_otp_get_key_lock_sta(item->offset, &pv[i].lock);
            ot_otp_func_fail_return(hal_otp_get_key_lock_sta, ret != TD_SUCCESS, ret);
        } else if (item->attr & OTP_ATTR_SPECIFY_FLAG) {
            /* read specify flag only get lock flag, value_len can be 0 */
            ret = hal_otp_get_flag_lock_sta(item->offset, &pv[i].lock);
            ot_otp_func_fail_return(hal_otp_get_flag_lock_sta, ret != TD_SUCCESS, ret);
#ifdef OT_OTP_V200
        } else if (item->attr & OTP_ATTR_DATA_FLAG_ONEWARY) {
            /* get user data flag, value_len must is equal to byte len */
            ot_otp_formula_fail_return(pv[i].value_len != item->bit_width, OT_ERR_OTP_INVALID_PARAM);

            ret = _drv_otp_read_data_flag(&pv[i], item, DRV_DATA_TYPE_ONEWAY);
            ot_otp_func_fail_return(_drv_otp_read_data_flag, ret != TD_SUCCESS, ret);
        } else if (item->attr & OTP_ATTR_DATA_FLAG_LOCKABLE) {
            /* get user data flag, value_len must is equal to byte len */
            ot_otp_formula_fail_return(pv[i].value_len != item->bit_width, OT_ERR_OTP_INVALID_PARAM);

            ret = _drv_otp_read_data_flag(&pv[i], item, DRV_DATA_TYPE_LOCKABLE);
            ot_otp_func_fail_return(_drv_otp_read_data_flag, ret != TD_SUCCESS, ret);
#endif
        } else {
            ot_otp_error("%s don't match attribution %x\n", item->field_name, item->attr);
            return OT_ERR_OTP_INVALID_FIELD_NAME;
        }
    }
    return TD_SUCCESS;
}

td_s32 drv_otp_get_key_verify_status(const td_char *key_name, td_bool *status)
{
    td_s32 ret;
    const otp_data_item *item = TD_NULL;

    item = _drv_otp_match_field_name(key_name);
    ot_otp_func_fail_return(_drv_otp_match_field_name, item == TD_NULL, OT_ERR_OTP_INVALID_FIELD_NAME);

    if (item->attr & OTP_ATTR_VERIFY_KEY) {
        ret = hal_otp_verify_key(item->offset, status);
        ot_otp_func_fail_return(hal_otp_verify_key, ret != TD_SUCCESS, ret);
    } else {
        ot_otp_error("%s don't match attribution %x\n", item->field_name, item->attr);
        return OT_ERR_OTP_INVALID_FIELD_NAME;
    }

    return TD_SUCCESS;
}

#ifdef OT_OTP_V100
td_s32 drv_otp_load_key_to_klad(const td_char *key_name)
{
    td_s32 ret;
    const otp_data_item *item = TD_NULL;

    item = _drv_otp_match_field_name(key_name);
    ot_otp_func_fail_return(_drv_otp_match_field_name, item == TD_NULL, OT_ERR_OTP_INVALID_FIELD_NAME);

    if (item->attr & OTP_ATTR_LOAD_KEY) {
        ret = hal_otp_load_key_to_klad(item->offset);
        ot_otp_func_fail_return(_drv_otp_match_field_name, ret != TD_SUCCESS, ret);
    } else {
        ot_otp_error("%s don't match attribution %x\n", item->field_name, item->attr);
        return OT_ERR_OTP_INVALID_FIELD_NAME;
    }

    return ret;
}
#endif
