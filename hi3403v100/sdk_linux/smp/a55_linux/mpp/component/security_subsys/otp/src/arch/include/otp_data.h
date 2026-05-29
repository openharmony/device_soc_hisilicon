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

#ifndef OTP_DATA_H
#define OTP_DATA_H

#include "ot_type.h"
#include "ot_common_otp.h"

/* otp attr */
#define OTP_ATTR_BURN_KEY           0x00000001 /* burn key */
#define OTP_ATTR_VERIFY_KEY         0x00000002 /* verify key */
#define OTP_ATTR_LOAD_KEY           0x00000004 /* load key */

#define OTP_ATTR_SPECIFY_FLAG       0x00000100 /* otp specify flag */
#define OTP_ATTR_DATA_FLAG_LOCKABLE 0x00000200 /* otp user data flag lockable */
#define OTP_ATTR_DATA_FLAG_ONEWARY  0x00000400 /* otp user data flag oneway */

#define OTP_ATTR_REE_USER_DATA      0x00010000 /* ree user self-define data */
#define OTP_ATTR_TEE_DATA_REE_READ  0x00020000 /* tee user data, ree can read, not write or lock */
#define OTP_ATTR_TEE_USER_DATA      0x00040000 /* tee user self-define data, ree can't access */

typedef struct {
    td_char field_name[OT_OTP_PV_NAME_MAX_LEN]; /* OTP fuse name */
    /* offset point to bit offset for the user data or point to index for the key and specify flag */
    td_u32 offset;
    td_u32 bit_width;                           /* OTP bit width */
    td_bool small_endian;                       /* OTP data is stored in big-endian or small-endian mode */
    td_u32 attr;                                /* OTP fuse name attribution */
} otp_data_item;

otp_data_item *otp_get_data_item(td_void);

td_u32 otp_get_data_item_num(td_void);

#endif /* OTP_DATA_H */
