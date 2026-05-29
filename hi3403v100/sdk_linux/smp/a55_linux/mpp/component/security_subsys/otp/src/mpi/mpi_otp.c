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

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "ot_mpi_otp.h"
#include "drv_ioctl_otp.h"
#include "securec.h"
#include "ot_debug_otp.h"

#define OTP_MUTEX_T                     pthread_mutex_t
#define otp_mutex_lock(x)               (td_void)pthread_mutex_lock(x)
#define otp_mutex_unlock(x)             (td_void)pthread_mutex_unlock(x)
#define otp_open(name, flags, mode)     open(name, flags, mode)
#define otp_close(fd)                   close(fd)
#define otp_ioctl(fd, request, var)     ioctl(fd, request, var)

typedef struct {
    OTP_MUTEX_T lock;
    td_s32 ref_count;
    td_s32 otp_fd;
} mpi_otp_mgmt;

static mpi_otp_mgmt g_mpi_otp_mgmt = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .ref_count = -1,
    .otp_fd = -1,
};

#define OTP_INIT_MAX_NUM     0x7FFFFFFF

#define _mpi_otp_not_init_return(ref_count)                     \
    do {                                                        \
        if ((ref_count) < 0) {                                  \
            ot_otp_error("otp init counter %d!\n", ref_count);  \
            return OT_ERR_OTP_NOT_INIT;                         \
        }                                                       \
    } while (0)

static mpi_otp_mgmt *_mpi_otp_get_mgmt(td_void)
{
    return &g_mpi_otp_mgmt;
}

td_s32 ot_mpi_otp_init(td_void)
{
    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    otp_mutex_lock(&mgmt->lock);

    if (mgmt->ref_count >= 0) {
        if (mgmt->ref_count < OTP_INIT_MAX_NUM) {
            mgmt->ref_count++;
        }
        otp_mutex_unlock(&mgmt->lock);
        return TD_SUCCESS;
    }

    mgmt->otp_fd = otp_open("/dev/"UMAP_DEVNAME_OTP_BASE, O_RDWR, 0);
    if (mgmt->otp_fd < 0) {
        otp_mutex_unlock(&mgmt->lock);
        ot_otp_error("open device /dev/%s failed\n", UMAP_DEVNAME_OTP_BASE);
        return OT_ERR_OTP_FAILED_INIT;
    }

    mgmt->ref_count = 0;

    otp_mutex_unlock(&mgmt->lock);

    return TD_SUCCESS;
}

td_s32 ot_mpi_otp_deinit(td_void)
{
    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    otp_mutex_lock(&mgmt->lock);

    if (mgmt->ref_count != 0) {
        if (mgmt->ref_count < OTP_INIT_MAX_NUM) {
            mgmt->ref_count--;
        }
        otp_mutex_unlock(&mgmt->lock);
        return TD_SUCCESS;
    }

    if (otp_close(mgmt->otp_fd) < 0) {
        mgmt->otp_fd = -1;
        mgmt->ref_count = -1;
        otp_mutex_unlock(&mgmt->lock);
        ot_otp_error("close /dev/%s failed\n", UMAP_DEVNAME_OTP_BASE);
        return OT_ERR_OTP_FAILED_INIT;
    }

    mgmt->otp_fd = -1;
    mgmt->ref_count = -1;

    otp_mutex_unlock(&mgmt->lock);

    return TD_SUCCESS;
}

td_s32 ot_mpi_otp_set_user_data(const td_char *field_name,
    td_u32 offset, const td_u8 *value, td_u32 value_len)
{
    td_s32 ret;
    otp_user_data user_data;
    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    _mpi_otp_not_init_return(mgmt->ref_count);

    ot_otp_formula_fail_return(field_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(field_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(value == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(value_len == 0, OT_ERR_OTP_INVALID_PARAM);

    (td_void)memset_s(&user_data, sizeof(otp_user_data), 0, sizeof(otp_user_data));

    ret = memcpy_s(user_data.field_name, sizeof(user_data.field_name) - 1, field_name, strlen(field_name));
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    user_data.value = (td_u8 *)value;
    user_data.offset = offset;
    user_data.value_len = value_len;

    ret = otp_ioctl(mgmt->otp_fd, CMD_OTP_SET_USER_DATA, &user_data);
    ot_otp_func_fail_return(otp_ioctl, ret != TD_SUCCESS, ret);

    return TD_SUCCESS;
}

td_s32 ot_mpi_otp_get_user_data(const td_char *field_name,
    td_u32 offset, td_u8 *value, td_u32 value_len)
{
    td_s32 ret;
    otp_user_data user_data;
    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    _mpi_otp_not_init_return(mgmt->ref_count);

    ot_otp_formula_fail_return(field_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(field_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(value == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(value_len == 0, OT_ERR_OTP_INVALID_PARAM);

    (td_void)memset_s(&user_data, sizeof(otp_user_data), 0, sizeof(otp_user_data));

    ret = memcpy_s(user_data.field_name, sizeof(user_data.field_name) - 1, field_name, strlen(field_name));
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    user_data.value = value;
    user_data.offset = offset;
    user_data.value_len = value_len;

    ret = otp_ioctl(mgmt->otp_fd, CMD_OTP_GET_USER_DATA, &user_data);
    ot_otp_func_fail_return(otp_ioctl, ret != TD_SUCCESS, ret);

    return ret;
}

td_s32 ot_mpi_otp_set_user_data_lock(const td_char *field_name,
    td_u32 offset, td_u32 value_len)
{
    td_s32 ret;
    otp_user_data_lock data_lock;

    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    _mpi_otp_not_init_return(mgmt->ref_count);

    ot_otp_formula_fail_return(field_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(field_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(value_len == 0, OT_ERR_OTP_INVALID_PARAM);

    (td_void)memset_s(&data_lock, sizeof(otp_user_data_lock), 0, sizeof(otp_user_data_lock));
    data_lock.offset = offset;
    data_lock.value_len = value_len;
    ret = memcpy_s(data_lock.field_name, sizeof(data_lock.field_name) - 1, field_name, strlen(field_name));
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    ret = otp_ioctl(mgmt->otp_fd, CMD_OTP_SET_USER_DATA_LOCK, &data_lock);
    ot_otp_func_fail_return(otp_ioctl, ret != TD_SUCCESS, ret);

    return ret;
}

td_s32 ot_mpi_otp_get_user_data_lock(const td_char *field_name,
    td_u32 offset, td_u32 value_len, ot_otp_lock_status *lock)
{
    td_s32 ret;
    otp_user_data_lock data_lock;

    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    _mpi_otp_not_init_return(mgmt->ref_count);

    ot_otp_formula_fail_return(field_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(field_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(value_len == 0, OT_ERR_OTP_INVALID_PARAM);
    ot_otp_formula_fail_return(lock == TD_NULL, OT_ERR_OTP_NULL_PTR);

    (td_void)memset_s(&data_lock, sizeof(otp_user_data_lock), 0, sizeof(otp_user_data_lock));
    data_lock.offset = offset;
    data_lock.value_len = value_len;
    ret = memcpy_s(data_lock.field_name, sizeof(data_lock.field_name) - 1, field_name, strlen(field_name));
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    ret = otp_ioctl(mgmt->otp_fd, CMD_OTP_GET_USER_DATA_LOCK, &data_lock);
    ot_otp_func_fail_return(otp_ioctl, ret != TD_SUCCESS, ret);

    *lock = data_lock.lock;

    return ret;
}

td_s32 ot_mpi_otp_burn_product_pv(const ot_otp_burn_pv_item *pv, td_u32 num)
{
    td_s32 i;
    td_s32 ret;
    otp_product_pv product_pv;
    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    _mpi_otp_not_init_return(mgmt->ref_count);

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

    (td_void)memset_s(&product_pv, sizeof(otp_product_pv), 0, sizeof(otp_product_pv));

    product_pv.pv = (ot_otp_burn_pv_item *)pv;
    product_pv.num = num;

    ret = otp_ioctl(mgmt->otp_fd, CMD_OTP_BURN_PRODUCT_PV, &product_pv);
    ot_otp_func_fail_return(otp_ioctl, ret != TD_SUCCESS, ret);

    return ret;
}

td_s32 ot_mpi_otp_read_product_pv(ot_otp_burn_pv_item *pv, td_u32 num)
{
    td_s32 i;
    td_s32 ret;
    otp_product_pv product_pv;
    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    _mpi_otp_not_init_return(mgmt->ref_count);

    ot_otp_formula_fail_return(pv == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return((num == 0) || (num > OTP_PRODUCT_PV_MAX_NUM), OT_ERR_OTP_INVALID_PARAM);

    for (i = 0; i < num; i++) {
        ot_otp_formula_fail_return(pv[i].burn == TD_TRUE, OT_ERR_OTP_INVALID_PARAM);
        /* read pv value_len could be 0 */
        ot_otp_formula_fail_return(
            pv[i].value_len > (OT_OTP_PV_VALUE_MAX_LEN * BYTE_BIT_WIDTH), OT_ERR_OTP_INVALID_PARAM);
    }

    (td_void)memset_s(&product_pv, sizeof(otp_product_pv), 0, sizeof(otp_product_pv));

    product_pv.pv = pv;
    product_pv.num = num;

    ret = otp_ioctl(mgmt->otp_fd, CMD_OTP_READ_PRODUCT_PV, &product_pv);
    ot_otp_func_fail_return(otp_ioctl, ret != TD_SUCCESS, ret);

    return ret;
}

td_s32 ot_mpi_otp_get_key_verify_status(const td_char *key_name, td_bool *status)
{
    td_s32 ret;
    otp_key_verify_status key_sta;
    mpi_otp_mgmt *mgmt = _mpi_otp_get_mgmt();

    _mpi_otp_not_init_return(mgmt->ref_count);

    ot_otp_formula_fail_return(key_name == TD_NULL, OT_ERR_OTP_NULL_PTR);
    ot_otp_formula_fail_return(
        strlen(key_name) >= OT_OTP_PV_NAME_MAX_LEN, OT_ERR_OTP_INVALID_FIELD_NAME);
    ot_otp_formula_fail_return(status == TD_NULL, OT_ERR_OTP_NULL_PTR);

    (td_void)memset_s(&key_sta, sizeof(otp_key_verify_status), 0, sizeof(otp_key_verify_status));

    ret = memcpy_s(key_sta.field_name, sizeof(key_sta.field_name) - 1, key_name, strlen(key_name));
    ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);

    ret = otp_ioctl(mgmt->otp_fd, CMD_OTP_KEY_VERIFY, &key_sta);
    ot_otp_func_fail_return(otp_ioctl, ret != TD_SUCCESS, ret);

    *status = key_sta.status;

    return ret;
}

