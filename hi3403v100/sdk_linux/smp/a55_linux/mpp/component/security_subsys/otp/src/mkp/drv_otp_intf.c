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

#include "drv_otp_intf.h"
#include "kapi_otp.h"
#include "drv_lib.h"
#include "drv_ioctl_otp.h"
#include "drv_osal_init.h"
#include "kapi_otp.h"
#include "securec.h"
#include "ot_debug_otp.h"
#include "osal_ioctl.h"

typedef td_s32 (*otp_ioctl_func)(td_void *argp);
typedef struct {
    const char *name;
    otp_ioctl_func func;
    td_u32 cmd;
} intf_ioctl_func;

static td_s32 _intf_otp_get_user_data(otp_user_data *user_data, td_u8 *value, td_u32 value_len)
{
    td_s32 ret;

    /* set array last data '\0', avoid out-of-bounds array access */
    user_data->field_name[OT_OTP_PV_NAME_MAX_LEN - 1] = '\0';

    ret = kapi_otp_get_user_data(user_data->field_name, user_data->offset, value, value_len);
    ot_otp_func_fail_return(kapi_otp_get_user_data, ret != 0, ret);

    ret = otp_copy_to_user(user_data->value, user_data->value_len, value, value_len);
    ot_otp_func_fail_return(otp_copy_to_user, ret != 0, OT_ERR_OTP_INVALID_PARAM);

    return ret;
}

static td_s32 _intf_otp_set_user_data(otp_user_data *user_data, td_u8 *value, td_u32 value_len)
{
    td_s32 ret;

    ret = otp_copy_from_user(value, value_len, user_data->value, user_data->value_len);
    ot_otp_func_fail_return(otp_copy_from_user, ret != 0, OT_ERR_OTP_INVALID_PARAM);

    /* set array last data '\0', avoid out-of-bounds array access */
    user_data->field_name[OT_OTP_PV_NAME_MAX_LEN - 1] = '\0';

    ret = kapi_otp_set_user_data(user_data->field_name, user_data->offset, value, value_len);
    ot_otp_func_fail_return(kapi_otp_set_user_data, ret != 0, ret);

    return ret;
}

static td_s32 intf_otp_set_user_data(td_void *argp)
{
    td_s32 ret;
    otp_user_data *user_data = (otp_user_data *)argp;
    td_u8 *value = TD_NULL;

    ot_otp_formula_fail_return(user_data == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(user_data->value == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(user_data->value_len == 0, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(user_data->value_len > OTP_USER_DATA_MAX_SIZE, OT_ERR_OTP_INVALID_PARAM);

    value = otp_malloc(user_data->value_len);
    ot_otp_func_fail_return(otp_malloc, value == TD_NULL, OT_ERR_OTP_FAILED_MEM);
    (td_void)memset_s(value, user_data->value_len, 0, user_data->value_len);

    ret = _intf_otp_set_user_data(user_data, value, user_data->value_len);

    otp_free(value);

    return ret;
}

static td_s32 intf_otp_get_user_data(td_void *argp)
{
    td_s32 ret;
    otp_user_data *user_data = (otp_user_data *)argp;
    td_u8 *value = TD_NULL;

    ot_otp_formula_fail_return(user_data == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(user_data->value == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(user_data->value_len == 0, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(user_data->value_len > OTP_USER_DATA_MAX_SIZE, OT_ERR_OTP_INVALID_PARAM);

    value = otp_malloc(user_data->value_len);
    ot_otp_func_fail_return(otp_malloc, value == TD_NULL, OT_ERR_OTP_FAILED_MEM);
    (td_void)memset_s(value, user_data->value_len, 0, user_data->value_len);

    ret = _intf_otp_get_user_data(user_data, value, user_data->value_len);

    otp_free(value);

    return ret;
}

static td_s32 intf_otp_set_user_data_lock(td_void *argp)
{
    td_s32 ret;
    otp_user_data_lock *data_lock = (otp_user_data_lock *)argp;

    ot_otp_formula_fail_return(data_lock == TD_NULL, OT_ERR_OTP_NULL_PTR);

    /* set array last data '\0', avoid out-of-bounds array access */
    data_lock->field_name[OT_OTP_PV_NAME_MAX_LEN - 1] = '\0';

    ret = kapi_otp_set_user_data_lock(data_lock->field_name,
        data_lock->offset, data_lock->value_len);
    ot_otp_func_fail_return(kapi_otp_set_user_data_lock, ret != TD_SUCCESS, ret);

    return ret;
}

static td_s32 intf_otp_get_user_data_lock(td_void *argp)
{
    td_s32 ret;
    otp_user_data_lock *data_lock = (otp_user_data_lock *)argp;

    ot_otp_formula_fail_return(data_lock == TD_NULL, OT_ERR_OTP_NULL_PTR);

    /* set array last data '\0', avoid out-of-bounds array access */
    data_lock->field_name[OT_OTP_PV_NAME_MAX_LEN - 1] = '\0';

    ret = kapi_otp_get_user_data_lock(data_lock->field_name,
        data_lock->offset, data_lock->value_len, &data_lock->lock);
    ot_otp_func_fail_return(kapi_otp_get_user_data_lock, ret != TD_SUCCESS, ret);

    return ret;
}

static td_s32 _intf_otp_read_product_pv(otp_product_pv *product_pv,
    ot_otp_burn_pv_item *item, td_u32 item_len)
{
    td_s32 ret;
    td_u32 i;

    ret = otp_copy_from_user(item, item_len, product_pv->pv, item_len);
    ot_otp_func_fail_return(otp_copy_from_user, ret != 0, OT_ERR_OTP_INVALID_PARAM);

    /* set array last data '\0', avoid out-of-bounds array access */
    for (i = 0; i < product_pv->num; i++) {
        item[i].field_name[OT_OTP_PV_NAME_MAX_LEN - 1] = '\0';
    }

    ret = kapi_otp_read_product_pv(item, product_pv->num);
    ot_otp_func_fail_return(kapi_otp_read_product_pv, ret != TD_SUCCESS, ret);

    ret = otp_copy_to_user(product_pv->pv, item_len, item, item_len);
    ot_otp_func_fail_return(otp_copy_to_user, ret != 0, OT_ERR_OTP_INVALID_PARAM);

    return ret;
}

static td_s32 _intf_otp_burn_product_pv(const otp_product_pv *product_pv,
    ot_otp_burn_pv_item *item, td_u32 item_len)
{
    td_s32 ret;
    td_u32 i;

    ret = otp_copy_from_user(item, item_len, product_pv->pv, item_len);
    ot_otp_func_fail_return(otp_copy_from_user, ret != 0, OT_ERR_OTP_INVALID_PARAM);

    /* set array last data '\0', avoid out-of-bounds array access */
    for (i = 0; i < product_pv->num; i++) {
        item[i].field_name[OT_OTP_PV_NAME_MAX_LEN - 1] = '\0';
    }

    ret = kapi_otp_burn_product_pv(item, product_pv->num);
    ot_otp_func_fail_return(kapi_otp_burn_product_pv, ret != TD_SUCCESS, ret);

    /* Ensure clean key when burning the key */
    (td_void)memset_s(item, item_len, 0, item_len);

    return ret;
}

static td_s32 intf_otp_burn_product_pv(td_void *argp)
{
    td_s32 ret;
    otp_product_pv *product_pv = (otp_product_pv *)argp;
    ot_otp_burn_pv_item *item = TD_NULL;
    td_u32 item_len;

    ot_otp_formula_fail_return(product_pv == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(product_pv->pv == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(product_pv->num == 0, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(product_pv->num > OTP_PRODUCT_PV_MAX_NUM, OT_ERR_OTP_INVALID_PARAM);

    item_len = sizeof(ot_otp_burn_pv_item) * product_pv->num;
    item = otp_malloc(item_len);
    ot_otp_func_fail_return(otp_malloc, item == TD_NULL, OT_ERR_OTP_FAILED_MEM);
    (td_void)memset_s(item, item_len, 0, item_len);

    ret = _intf_otp_burn_product_pv(product_pv, item, item_len);

    otp_free(item);

    return ret;
}

static td_s32 intf_otp_read_product_pv(td_void *argp)
{
    td_s32 ret;
    otp_product_pv *product_pv = (otp_product_pv *)argp;
    ot_otp_burn_pv_item *item = TD_NULL;
    td_u32 item_len;

    ot_otp_formula_fail_return(product_pv == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(product_pv->pv == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(product_pv->num == 0, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(product_pv->num > OTP_PRODUCT_PV_MAX_NUM, OT_ERR_OTP_INVALID_PARAM);

    item_len = sizeof(ot_otp_burn_pv_item) * product_pv->num;
    item = otp_malloc(item_len);
    ot_otp_func_fail_return(otp_malloc, item == TD_NULL, OT_ERR_OTP_FAILED_MEM);
    (td_void)memset_s(item, item_len, 0, item_len);

    ret = _intf_otp_read_product_pv(product_pv, item, item_len);

    otp_free(item);

    return ret;
}

static td_s32 intf_otp_get_key_verify_status(td_void *argp)
{
    td_s32 ret;
    otp_key_verify_status *key_sta = (otp_key_verify_status *)argp;

    ot_otp_formula_fail_return(key_sta == TD_NULL, OT_ERR_OTP_NULL_PTR);

    /* set array last data '\0', avoid out-of-bounds array access */
    key_sta->field_name[OT_OTP_PV_NAME_MAX_LEN - 1] = '\0';

    ret = kapi_otp_get_key_verify_status(key_sta->field_name, &key_sta->status);
    ot_otp_func_fail_return(kapi_otp_get_key_verify_status, ret != TD_SUCCESS, ret);

    return ret;
}

static intf_ioctl_func g_otp_func[CMD_OTP_COUNT] = {
    {"set_user_data",         intf_otp_set_user_data,           CMD_OTP_SET_USER_DATA},
    {"get_user_data",         intf_otp_get_user_data,           CMD_OTP_GET_USER_DATA},
    {"set_user_data_lock",    intf_otp_set_user_data_lock,      CMD_OTP_SET_USER_DATA_LOCK},
    {"get_user_data_lock",    intf_otp_get_user_data_lock,      CMD_OTP_GET_USER_DATA_LOCK},
    {"burn_product_pv",       intf_otp_burn_product_pv,         CMD_OTP_BURN_PRODUCT_PV},
    {"read_product_pv",       intf_otp_read_product_pv,         CMD_OTP_READ_PRODUCT_PV},
    {"key_verify_status",     intf_otp_get_key_verify_status,   CMD_OTP_KEY_VERIFY},
};

td_s32 intf_otp_ioctl(td_u32 cmd, td_void *argp)
{
    td_s32 ret;
    td_u32 nr;

    ot_otp_enter();

    nr = OSAL_IOC_NR(cmd);

    ot_otp_formula_fail_return(argp == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(nr >= CMD_OTP_COUNT, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(cmd != g_otp_func[nr].cmd, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(g_otp_func[nr].func == TD_NULL, OT_ERR_OTP_NULL_PTR);

    ot_otp_info("cmd %x, nr %d, dir %x\n", cmd, nr, OSAL_IOC_DIR(cmd));

    ret = g_otp_func[nr].func(argp);
    if (ret != TD_SUCCESS) {
        ot_otp_error("call intf func failed, cmd %x, nr %d\n", cmd, nr);
        return ret;
    }

    ot_otp_exit();

    return ret;
}

td_s32 otp_entry(td_void)
{
    td_s32 ret;

    ret = kapi_otp_init();
    ot_otp_func_fail_return(kapi_otp_init, ret != TD_SUCCESS, ret);

    return TD_SUCCESS;
}

td_void otp_exit(td_void)
{
    kapi_otp_deinit();
}
