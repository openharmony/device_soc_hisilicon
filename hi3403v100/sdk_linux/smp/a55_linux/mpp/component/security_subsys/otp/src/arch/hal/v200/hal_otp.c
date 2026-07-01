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

#include "hal_otp.h"
#include "drv_lib.h"
#include "drv_osal_init.h"
#include "securec.h"
#include "ot_debug_otp.h"
#include "otp_reg_base.h"
#include "drv_ioctl_otp.h"
#include "hal_otp_define.h"

#ifdef OT_OTP_V200

typedef enum {
    OTP_WORK_MODE_READ_LOCK_STA = 0,
    OTP_WORK_MODE_LOAD_CIPHER_KEY,
    OTP_WORK_MODE_BURN_KEY,
    OTP_WORK_MODE_VERIFY_KEY_CRC,
    OTP_WORK_MODE_ENABLE_FLAG_CFG,
    OTP_WORK_MODE_WRITE_USER_ROOM,
    OTP_WORK_MODE_READ_USER_ROOM,
    OTP_WORK_MODE_BUTT,
} hal_otp_work_mode;

#define _hal_otp_read_sta_data(sta)          hal_otp_read_word(OTP_USER_LOCK_STA_DATA, sta)
#define _hal_otp_read_rev_data(data)         hal_otp_read_word(OTP_USER_REV_RDATA, data)
#define _hal_otp_read_ctrl_sta(sta)          hal_otp_read_word(OTP_USER_CTRL_STA, sta)

#define _hal_otp_set_rev_addr(offset)        hal_otp_write_word(OTP_USER_REV_ADDR, offset)
#define _hal_otp_set_rev_data(data)          hal_otp_write_word(OTP_USER_REV_WDATA, data)
#define _hal_otp_set_work_mode(mode)         hal_otp_write_word(OTP_USER_WORK_MODE, mode)
#define _hal_otp_set_key_index(idx)          hal_otp_write_word(OTP_USER_KEY_INDEX, idx)
#define _hal_otp_set_flag_index(idx)         hal_otp_write_word(OTP_USER_FLAG_INDEX, idx)
#define _hal_otp_set_flag_value(value)       hal_otp_write_word(OTP_USER_FLAG_VALUE, value)
#define _hal_otp_set_sta_addr(addr)          hal_otp_write_word(OTP_USER_LOCK_STA_ADDR, addr)
#define _hal_otp_op_start()                  hal_otp_write_word(OTP_USER_OP_START, OTP_OP_START_VAL)

static td_s32 _hal_otp_wait_op_free(td_void)
{
    otp_user_ctrl_sta ctrl_sta;
    td_u32 time_out_cnt = OTP_WAIT_TIME_OUT;

    while (time_out_cnt--) {
        _hal_otp_read_ctrl_sta(&ctrl_sta.u32);

        if (ctrl_sta.bits.otp_op_busy == 0) {
            return TD_SUCCESS;
        }
        otp_udelay(1);
    }

    ot_otp_error("otp wait op timeout!\n");
    return OT_ERR_OTP_WAIT_TIMEOUT;
}

static td_s32 _hal_otp_wait_cmd_finish(td_void)
{
    return _hal_otp_wait_op_free();
}

static td_s32 _hal_otp_get_user_lock_sta(td_u32 addr, td_u32 *sta)
{
    td_s32 ret;

    ot_otp_enter();

    /* 1. wait otp ctrl free */
    ret = _hal_otp_wait_op_free();
    ot_otp_func_fail_return(_hal_otp_wait_op_free, ret != TD_SUCCESS, ret);

    /* 2. set work mode to read lock sta */
    _hal_otp_set_work_mode(OTP_WORK_MODE_READ_LOCK_STA);

    /* 3. set sta address */
    _hal_otp_set_sta_addr(addr);

    /* 4. start otp ctrl */
    _hal_otp_op_start();

    /* 5. wait user cmd finish */
    ret = _hal_otp_wait_cmd_finish();
    ot_otp_func_fail_return(_hal_otp_wait_cmd_finish, ret != TD_SUCCESS, ret);

    /* 6. read lock sta data */
    _hal_otp_read_sta_data(sta);

    ot_otp_exit();

    return ret;
}

/*
 * write key: Writes key from a high address to a low address by word
 * calculate crc: calculate crc according to the key write sequence by byte
 */
static td_s32 _hal_otp_write_key_data(const td_u8 *u8_key, td_u32 u8_klen, td_u32 key_index)
{
    td_s32 ret;
    td_u32 u32_key[OTP_USER_KEY_MAX_WORDS];
    td_u8 u8_tmp[OTP_USER_KEY_MAX_BYTES];
    td_u32 crc;
    td_u32 i;
    td_u32 u32_klen;

    ot_otp_formula_fail_return(!align_word(u8_klen), OT_ERR_OTP_INVALID_PARAM);

    (td_void)memset_s(u32_key, sizeof(u32_key), 0, sizeof(u32_key));
    (td_void)memset_s(u8_tmp, sizeof(u8_tmp), 0, sizeof(u8_tmp));

    u32_klen = u8_klen / WORD_BYTE_WIDTH;

    /* v200 OTP SYMC KEY load RKP module,
       RKP module use otp symc key isn't different from v100 OTP KEY */
    if (key_index >= 6 && key_index <= 10) { /* 6,10 -> OEM SYMC KEY[0~3] + VENDOR SYMC KEY */
        for (i = 0; i < u32_klen; i++) {
            u32_key[u32_klen - i - 1] = ((td_u32)u8_key[i * WORD_BYTE_WIDTH + 0] << 24) | /* 0, 24 */
                                        ((td_u32)u8_key[i * WORD_BYTE_WIDTH + 1] << 16) | /* 1, 16 */
                                        ((td_u32)u8_key[i * WORD_BYTE_WIDTH + 2] << 8) |  /* 2, 8 */
                                        ((td_u32)u8_key[i * WORD_BYTE_WIDTH + 3] << 0);   /* 3, 0 */
        }
    } else {
        ret = memcpy_s((td_u8 *)u32_key, sizeof(u32_key), u8_key, u8_klen);
        ot_otp_func_fail_return(memcpy_s, ret != EOK, OT_ERR_OTP_FAILED_SEC_FUNC);
    }

    for (i = 0; i < u32_klen; i++) {
        hal_otp_write_word(OTP_USER_KEY_DATA0 + i * WORD_BYTE_WIDTH, u32_key[i]);

        ret = ot_otp_word_big_endian(u32_key[i], &u8_tmp[i * WORD_BYTE_WIDTH], WORD_BYTE_WIDTH);
        if (ret != TD_SUCCESS) {
            (td_void)memset_s(u32_key, sizeof(u32_key), 0, sizeof(u32_key));
            ot_otp_error("call %s failed, ret = 0x%08X\n", "ot_otp_word_big_endian", ret);
            return ret;
        }
    }

    crc = (0x0000ffff & ot_otp_crc16_modbus(u8_tmp, u8_klen));
    hal_otp_write_word(OTP_USER_KEY_DATA0 + i * WORD_BYTE_WIDTH, crc);

    (td_void)memset_s(u32_key, sizeof(u32_key), 0, sizeof(u32_key));
    return TD_SUCCESS;
}

static td_void _hal_otp_write_flag_value(td_u32 value)
{
    otp_user_flag_value flag_value;

    flag_value.u32 = 0;
    flag_value.bits.otp_flag_region_pgm_data = (value & 0x01);

    _hal_otp_set_flag_value(flag_value.u32);
}

td_s32 hal_otp_get_key_lock_sta(td_u32 offset, td_bool *lock_sta)
{
    td_s32 ret;
    td_u32 lock_data = 0;

    /* 0 - key lock sta, 1 - flag lock sta, 2~1C - user data lock sta */
    ret = _hal_otp_get_user_lock_sta(0, &lock_data);
    ot_otp_func_fail_return(_hal_otp_get_user_lock_sta, ret != TD_SUCCESS, ret);

    if (lock_data & (1 << (offset % WORD_BIT_WIDTH))) {
        *lock_sta = TD_TRUE;
    } else {
        *lock_sta = TD_FALSE;
    }
    return TD_SUCCESS;
}

td_s32 hal_otp_get_flag_lock_sta(td_u32 offset, td_bool *lock_sta)
{
    td_s32 ret;
    td_u32 lock_data = 0;

    /* 0 - key lock sta, 1 - flag lock sta, 2~1C - user data lock sta */
    ret = _hal_otp_get_user_lock_sta(1, &lock_data);
    ot_otp_func_fail_return(_hal_otp_get_user_lock_sta, ret != TD_SUCCESS, ret);

    if (lock_data & (1 << (offset % WORD_BIT_WIDTH))) {
        *lock_sta = TD_TRUE;
    } else {
        *lock_sta = TD_FALSE;
    }

    return TD_SUCCESS;
}

td_s32 hal_otp_get_user_data_lock_sta(td_u32 index, td_bool *lock_sta)
{
    td_s32 ret;
    td_u32 lock_data = 0;

    /* 0 - key lock sta, 1 - flag lock sta, 2~1C - user data lock sta */
    ret = _hal_otp_get_user_lock_sta(index / WORD_BIT_WIDTH + 2, &lock_data);
    ot_otp_func_fail_return(_hal_otp_get_user_lock_sta, ret != TD_SUCCESS, ret);

    ot_otp_info("index %d, lock_data %08x\n", index, lock_data);
    if (lock_data & (1 << (index % WORD_BIT_WIDTH))) {
        *lock_sta = TD_TRUE;
    } else {
        *lock_sta = TD_FALSE;
    }

    return TD_SUCCESS;
}

td_s32 hal_otp_set_user_data_word(td_u32 index, td_u32 value, td_bool lock)
{
    td_s32 ret;
    td_bool lock_sta;
    otp_user_work_mode mode;

    /* 1. check user data lock status */
    ret = hal_otp_get_user_data_lock_sta(index, &lock_sta);
    ot_otp_func_fail_return(hal_otp_get_user_data_lock_sta, ret != TD_SUCCESS, ret);
    if (lock_sta == TD_TRUE) {
        ot_otp_error("user data offset %u has been locked!!!\n", index);
        return OT_ERR_OTP_ZONE_LOCKED;
    }

    /* 2. wait otp ctrl free */
    ret = _hal_otp_wait_op_free();
    ot_otp_func_fail_return(_hal_otp_wait_op_free, ret != TD_SUCCESS, ret);

    /* 3. set rev addr */
    _hal_otp_set_rev_addr(index);

    /* 4. write rev data */
    _hal_otp_set_rev_data(value);

    /* 5. set work mode to write user room */
    mode.u32 = 0;
    mode.bits.user_reserved_lock_en = (td_u32)lock & 0x01;
    mode.bits.otp_user_work_mode = OTP_WORK_MODE_WRITE_USER_ROOM;
    _hal_otp_set_work_mode(mode.u32);

    /* 6. start otp ctrl */
    _hal_otp_op_start();

    /* 7. wait user cmd finish */
    ret = _hal_otp_wait_cmd_finish();
    ot_otp_func_fail_return(_hal_otp_wait_cmd_finish, ret != TD_SUCCESS, ret);

    return ret;
}

td_s32 hal_otp_get_user_data_word(td_u32 index, td_u32 *value)
{
    td_s32 ret;

    /* 1. wait otp ctrl free */
    ret = _hal_otp_wait_op_free();
    ot_otp_func_fail_return(_hal_otp_wait_op_free, ret != TD_SUCCESS, ret);

    /* 2. set rev addr */
    _hal_otp_set_rev_addr(index);

    /* 3. set work mode to read user room */
    _hal_otp_set_work_mode(OTP_WORK_MODE_READ_USER_ROOM);

    /* 4. start otp ctrl */
    _hal_otp_op_start();

    /* 5. wait user cmd finish */
    ret = _hal_otp_wait_cmd_finish();
    ot_otp_func_fail_return(_hal_otp_wait_cmd_finish, ret != TD_SUCCESS, ret);

    /* 6. read rev data */
    _hal_otp_read_rev_data(value);

    return ret;
}

td_s32 hal_otp_verify_key(td_u32 key_index, td_bool *flag)
{
    td_s32 ret;
    otp_user_ctrl_sta ctrl_sta;

    ot_otp_enter();

    /* 1. wait otp ctrl free */
    ret = _hal_otp_wait_op_free();
    ot_otp_func_fail_return(_hal_otp_wait_op_free, ret != TD_SUCCESS, ret);

    /* 2. set key index */
    _hal_otp_set_key_index(key_index);

    /* 3. set work mode to verify key crc */
    _hal_otp_set_work_mode(OTP_WORK_MODE_VERIFY_KEY_CRC);

    /* 4. start otp ctrl */
    _hal_otp_op_start();

    /* 5. wait user cmd finish  & check key crc flag */
    ret = _hal_otp_wait_cmd_finish();
    ot_otp_func_fail_return(_hal_otp_wait_cmd_finish, ret != TD_SUCCESS, ret);

    _hal_otp_read_ctrl_sta(&ctrl_sta.u32);
    if (ctrl_sta.bits.key_crc_check_ok_flag == 1) {
        *flag = TD_TRUE;
        ot_otp_info("verify key crc %d ok\n", key_index);
    } else {
        *flag = TD_FALSE;
        ot_otp_info("verify key crc %d failed\n", key_index);
    }

    ot_otp_exit();

    return ret;
}

td_s32 hal_otp_burn_key(const otp_data_item *data_item, const td_u8 *key, td_u32 klen)
{
    td_s32 ret;
    otp_user_work_mode mode;
    td_bool lock_sta;

    ot_otp_enter();

    /* 1. check user key status */
    ret = hal_otp_get_key_lock_sta(data_item->offset, &lock_sta);
    ot_otp_func_fail_return(hal_otp_get_key_lock_sta, ret != TD_SUCCESS, ret);
    if (lock_sta == TD_TRUE) {
        ot_otp_error("key slot %s has been locked!!!\n", data_item->field_name);
        return OT_ERR_OTP_ZONE_LOCKED;
    }

    /* 2. wait otp ctrl free */
    ret = _hal_otp_wait_op_free();
    ot_otp_func_fail_return(_hal_otp_wait_op_free, ret != TD_SUCCESS, ret);

    /* 3. set user key index */
    _hal_otp_set_key_index(data_item->offset);

    /* 4. write key data */
    ret = _hal_otp_write_key_data(key, klen, data_item->offset);
    ot_otp_func_fail_return(_hal_otp_write_key_data, ret != TD_SUCCESS, ret);

    /* 5. set work mode */
    mode.u32 = 0;
    mode.bits.otp_user_work_mode = OTP_WORK_MODE_BURN_KEY;
    mode.bits.user_reserved_lock_en = 1;
    _hal_otp_set_work_mode(mode.u32);

    /* 6. start otp ctrl */
    _hal_otp_op_start();

    /* 7. wait user cmd finish */
    ret = _hal_otp_wait_cmd_finish();
    ot_otp_func_fail_return(_hal_otp_wait_cmd_finish, ret != TD_SUCCESS, ret);

    ot_otp_exit();

    return ret;
}

td_s32 hal_otp_enable_flag(const otp_data_item *data_item, td_u32 flag, td_bool lock)
{
    td_s32 ret;
    otp_user_work_mode mode;
    td_bool lock_sta;

    ot_otp_enter();

    /* 1. check flag status */
    ret = hal_otp_get_flag_lock_sta(data_item->offset, &lock_sta);
    ot_otp_func_fail_return(hal_otp_get_flag_lock_sta, ret != TD_SUCCESS, ret);
    if (lock_sta == TD_TRUE) {
        ot_otp_error("flag %s has been locked!!!\n", data_item->field_name);
        return OT_ERR_OTP_ZONE_LOCKED;
    }

    /* 2. wait otp ctrl free */
    ret = _hal_otp_wait_op_free();
    ot_otp_func_fail_return(_hal_otp_wait_op_free, ret != TD_SUCCESS, ret);

    /* 3. set flag index */
    _hal_otp_set_flag_index(data_item->offset);

    /* 4. set flag value */
    _hal_otp_write_flag_value(flag);
    ot_otp_func_fail_return(_hal_otp_write_flag_value, ret != TD_SUCCESS, ret);

    /* 5. set work mode to enable flag config */
    mode.u32 = 0;
    mode.bits.otp_user_work_mode = OTP_WORK_MODE_ENABLE_FLAG_CFG;
    mode.bits.user_reserved_lock_en = lock;
    _hal_otp_set_work_mode(mode.u32);

    /* 6. start otp ctrl */
    _hal_otp_op_start();

    /* 7. wait user cmd finish */
    ret = _hal_otp_wait_cmd_finish();
    ot_otp_func_fail_return(_hal_otp_wait_cmd_finish, ret != TD_SUCCESS, ret);

    ot_otp_exit();

    return ret;
}

#endif
