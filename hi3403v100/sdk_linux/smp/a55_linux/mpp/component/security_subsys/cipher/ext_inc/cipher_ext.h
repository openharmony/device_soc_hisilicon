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

#ifndef CIPHER_EXT_H
#define CIPHER_EXT_H

typedef enum {
    OT_PRIVATE_ID_SYMC,
    OT_PRIVATE_ID_HASH,
    OT_PRIVATE_ID_KEYSLOT,
    OT_PRIVATE_ID_KLAD,
    OT_PRIVATE_ID_BUTT,
} ot_private_id;

/* -------------------------------------------------------------------------------------------------------------
 * Definition of basic data types. The data types are applicable to both the application layer and kernel codes.
 * -------------------------------------------------------------------------------------------------------------
 *
 * define of TD_HANDLE :
 * bit31                                                                bit0
 *   |<----- 8bit ----->|<----- 8bit ----->|<---------- 16bit ---------->|
 *   |-------------------------------------------------------------------|
 *   |     ot_mod_id    | mod defined data |           chan_id           |
 *   |-------------------------------------------------------------------|
 * mod defined data: private data define by each module(for example: sub-mod id), usually, set to 0.
 */
#define td_handle_init(mod, private_data, chnid) \
    (td_handle)((((mod) & 0xff) << 24) | ((((private_data) & 0xff) << 16)) | (((chnid) & 0xffff)))

#define td_handle_get_modid(handle)         (((handle) >> 24) & 0xff)

#define td_handle_get_private_data(handle)  (((handle) >> 16) & 0xff)

#define td_handle_get_chnid(handle)         (((handle)) & 0xffff)

#endif /* CIPHER_EXT_H */
