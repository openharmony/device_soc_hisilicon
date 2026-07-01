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

#include "kapi_otp.h"
#include "drv_ioctl_otp.h"
#include "drv_osal_init.h"
#include "drv_otp.h"
#include "ot_debug_otp.h"

static td_bool g_otp_init = TD_FALSE;

static OTP_MUTEX_T g_otp_mutex;
#define kapi_otp_lock()                      otp_mutex_lock(&g_otp_mutex)
#define kapi_otp_unlock()                    otp_mutex_unlock(&g_otp_mutex)

#define kapi_otp_not_init_return()  \
    do { \
        if (g_otp_init == TD_FALSE) { \
            ot_otp_error("otp isn't init\n"); \
            return OT_ERR_OTP_NOT_INIT; \
        } \
    } while (0)

td_s32 kapi_otp_init(td_void)
{
    td_s32 ret;

    ret = drv_otp_init();
    ot_otp_func_fail_return(drv_otp_init, ret != TD_SUCCESS, ret);

    otp_mutex_init(&g_otp_mutex);

    g_otp_init = TD_TRUE;

    return TD_SUCCESS;
}

td_s32 kapi_otp_deinit(td_void)
{
    drv_otp_deinit();

    otp_mutex_destroy(&g_otp_mutex);

    g_otp_init = TD_FALSE;

    return TD_SUCCESS;
}

td_s32 kapi_otp_set_user_data(const td_char *field_name,
    td_u32 offset, const td_u8 *value, td_u32 value_len)
{
    td_s32 ret;

    kapi_otp_not_init_return();

    ot_otp_formula_fail_return(field_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(field_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(value == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(value_len == 0, OT_ERR_OTP_INVALID_PARAM);

    kapi_otp_lock();

    ret = drv_otp_set_user_data(field_name, offset, value, value_len);

    kapi_otp_unlock();

    return ret;
}

td_s32 kapi_otp_get_user_data(const td_char *field_name,
    td_u32 offset, td_u8 *value, td_u32 value_len)
{
    td_s32 ret;

    kapi_otp_not_init_return();

    ot_otp_formula_fail_return(field_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(field_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(value == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(value_len == 0, OT_ERR_OTP_INVALID_PARAM);

    kapi_otp_lock();

    ret = drv_otp_get_user_data(field_name, offset, value, value_len);

    kapi_otp_unlock();

    return ret;
}

td_s32 kapi_otp_set_user_data_lock(const td_char *field_name,
    td_u32 offset, td_u32 value_len)
{
    td_s32 ret;

    kapi_otp_not_init_return();

    ot_otp_formula_fail_return(field_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(field_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(value_len == 0, OT_ERR_OTP_INVALID_PARAM);

    kapi_otp_lock();

    ret = drv_otp_set_user_data_lock(field_name, offset, value_len);

    kapi_otp_unlock();

    return ret;
}

td_s32 kapi_otp_get_user_data_lock(const td_char *field_name,
    td_u32 offset, td_u32 value_len, ot_otp_lock_status *lock)
{
    td_s32 ret;

    kapi_otp_not_init_return();

    ot_otp_formula_fail_return(field_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(field_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(value_len == 0, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(lock == TD_NULL, OT_ERR_OTP_NULL_PTR);

    kapi_otp_lock();

    ret = drv_otp_get_user_data_lock(field_name, offset, value_len, lock);

    kapi_otp_unlock();

    return ret;
}

td_s32 kapi_otp_burn_product_pv(const ot_otp_burn_pv_item *pv, td_u32 num)
{
    td_s32 ret;
    td_u32 i;

    kapi_otp_not_init_return();

    ot_otp_formula_fail_return(pv == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return((num == 0) || (num > OTP_PRODUCT_PV_MAX_NUM), OT_ERR_OTP_INVALID_PARAM);

    for (i = 0; i < num; i++) {
        ot_otp_formula_fail_return(pv[i].burn != TD_TRUE, OT_ERR_OTP_INVALID_PARAM);
        /* burn pv value_len couldn't be 0 */
        ot_otp_formula_fail_return(pv[i].value_len == 0, OT_ERR_OTP_INVALID_PARAM);
        ot_otp_formula_fail_return(
            pv[i].value_len > (OT_OTP_PV_VALUE_MAX_LEN * BYTE_BIT_WIDTH), OT_ERR_OTP_INVALID_PARAM);
        ot_otp_formula_fail_return(pv[i].lock != TD_TRUE && pv[i].lock != TD_FALSE, OT_ERR_OTP_INVALID_PARAM);
    }

    kapi_otp_lock();

    ret = drv_otp_burn_product_pv(pv, num);

    kapi_otp_unlock();

    return ret;
}

td_s32 kapi_otp_read_product_pv(ot_otp_burn_pv_item *pv, td_u32 num)
{
    td_s32 ret;
    td_u32 i;

    kapi_otp_not_init_return();

    ot_otp_formula_fail_return(pv == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return((num == 0) || (num > OTP_PRODUCT_PV_MAX_NUM), OT_ERR_OTP_INVALID_PARAM);

    for (i = 0; i < num; i++) {
        ot_otp_formula_fail_return(pv[i].burn == TD_TRUE, OT_ERR_OTP_INVALID_PARAM);
        /* read pv value_len could be 0 */
        ot_otp_formula_fail_return(
            pv[i].value_len > (OT_OTP_PV_VALUE_MAX_LEN * BYTE_BIT_WIDTH), OT_ERR_OTP_INVALID_PARAM);
    }

    kapi_otp_lock();

    ret = drv_otp_read_product_pv(pv, num);

    kapi_otp_unlock();

    return ret;
}

td_s32 kapi_otp_get_key_verify_status(const td_char *key_name, td_bool *status)
{
    td_s32 ret;

    kapi_otp_not_init_return();

    ot_otp_formula_fail_return(key_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(key_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(status == TD_NULL, OT_ERR_OTP_NULL_PTR);

    kapi_otp_lock();

    ret = drv_otp_get_key_verify_status(key_name, status);

    kapi_otp_unlock();

    return ret;
}

#ifdef OT_OTP_V100
td_s32 kapi_otp_load_key_to_klad(const td_char *key_name)
{
    td_s32 ret;

    kapi_otp_not_init_return();

    ot_otp_formula_fail_return(key_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(key_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);

    kapi_otp_lock();

    ret = drv_otp_load_key_to_klad(key_name);

    kapi_otp_unlock();

    return ret;
}
#endif
