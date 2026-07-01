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

#ifndef DRV_IOCTL_OTP_H
#define DRV_IOCTL_OTP_H

#include <asm-generic/ioctl.h>
#include "ot_common_otp.h"
#include "ot_common.h"
#include "mkp_ioctl.h"
#include "dev_ext.h"
#include "osal_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* word index number. */
#define WORD_IDX_0                      0
#define WORD_IDX_1                      1
#define WORD_IDX_2                      2
#define WORD_IDX_3                      3
#define WORD_IDX_4                      4
#define WORD_IDX_5                      5
#define WORD_IDX_6                      6
#define WORD_IDX_7                      7

/* byte bit width */
#define BYTE_BIT_WIDTH                  8

/* word byte width */
#define WORD_BYTE_WIDTH                 4

#define WORD_BIT_WIDTH                  32

/* byte offset */
#define OFFSET_0_BYTE                   0
#define OFFSET_1_BYTE                   8
#define OFFSET_2_BYTE                   16
#define OFFSET_3_BYTE                   24

/* Boundary value 1. */
#define BOUND_VAL_1                     1

/* multiple value */
#define MUL_VAL_1                       1
#define MUL_VAL_2                       2
#define MUL_VAL_3                       3
#define MUL_VAL_4                       4

#define OTP_PRODUCT_PV_MAX_NUM          500

/* otp user data max 16k bytes */
#define OTP_USER_DATA_MAX_SIZE          0x4000

#define rem(value, base)            ((value) % (base))
#define round(value, base)          ((value) / (base))

#define bit_mask(width) \
    (((width) == WORD_BIT_WIDTH) ? 0xFFFFFFFFU : ~(0xFFFFFFFFU << (width)))

#define data_bit_mask(value, offset, width) \
    (((value) & bit_mask(width)) << (rem(offset, WORD_BIT_WIDTH)))

#define data_bit_restore(value, offset, width) \
    (((value) >> rem(offset, WORD_BIT_WIDTH)) & bit_mask(width))

#define align_word(offset)          (((offset) & 0x03) == 0)
#define align_16_bytes(offset)      (((offset) & 0x0f) == 0)
#define word_number(byte)           \
    ((rem(byte, WORD_BYTE_WIDTH) == 0) ? round(byte, WORD_BYTE_WIDTH) : (round(byte, WORD_BYTE_WIDTH) + 1))

#define byte_number(bit)            \
    ((rem(bit, BYTE_BIT_WIDTH) == 0) ? round(bit, BYTE_BIT_WIDTH) : (round(bit, BYTE_BIT_WIDTH) + 1))

#define otp_array_size(x)               (sizeof(x) / sizeof((x)[0]))

/* ----------- IOCTL STRUCT CONFIG ----------- */
typedef struct {
    td_char field_name[OT_OTP_PV_NAME_MAX_LEN];
    td_u32 offset;
    td_u8 *value;
    td_u32 value_len;
} otp_user_data;

typedef struct {
    ot_otp_burn_pv_item *pv;
    td_u32 num;
} otp_product_pv;

typedef struct {
    td_char field_name[OT_OTP_PV_NAME_MAX_LEN];
    td_bool status;
} otp_key_verify_status;

typedef struct {
    td_char field_name[OT_OTP_PV_NAME_MAX_LEN];
    td_u32 offset;
    td_u32 value_len;
    ot_otp_lock_status lock;
} otp_user_data_lock;

/* ----------- IOCTL CMD CONFIG ----------- */
typedef enum {
    OTP_IOC_NR_SET_USER_DATA,
    OTP_IOC_NR_GET_USER_DATA,
    OTP_IOC_NR_SET_USER_DATA_LOCK,
    OTP_IOC_NR_GET_USER_DATA_LOCK,
    OTP_IOC_NR_BURN_PRODUCT_PV,
    OTP_IOC_NR_READ_PRODUCT_PV,
    OTP_IOC_NR_KEY_VERIFY,
    OTP_IOC_NR_BUTT,
} otp_ioc_nr;

#define CMD_OTP_SET_USER_DATA       OSAL_IOW (IOC_TYPE_OTP, OTP_IOC_NR_SET_USER_DATA, otp_user_data)
#define CMD_OTP_GET_USER_DATA       OSAL_IOWR(IOC_TYPE_OTP, OTP_IOC_NR_GET_USER_DATA, otp_user_data)
#define CMD_OTP_SET_USER_DATA_LOCK  OSAL_IOW (IOC_TYPE_OTP, OTP_IOC_NR_SET_USER_DATA_LOCK, otp_user_data_lock)
#define CMD_OTP_GET_USER_DATA_LOCK  OSAL_IOWR(IOC_TYPE_OTP, OTP_IOC_NR_GET_USER_DATA_LOCK, otp_user_data_lock)
#define CMD_OTP_BURN_PRODUCT_PV     OSAL_IOW (IOC_TYPE_OTP, OTP_IOC_NR_BURN_PRODUCT_PV, otp_product_pv)
#define CMD_OTP_READ_PRODUCT_PV     OSAL_IOWR(IOC_TYPE_OTP, OTP_IOC_NR_READ_PRODUCT_PV, otp_product_pv)
#define CMD_OTP_KEY_VERIFY          OSAL_IOWR(IOC_TYPE_OTP, OTP_IOC_NR_KEY_VERIFY, otp_key_verify_status)
#define CMD_OTP_COUNT               OTP_IOC_NR_BUTT

#ifdef __cplusplus
}
#endif

#endif /* DRV_IOCTL_OTP_H */
