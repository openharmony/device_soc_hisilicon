/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __FM11NT081D_H_
#define __FM11NT081D_H_

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HINFC_DEVICE_NAME "hi_fm11nt081d"
#define HI_ID_NFC 'Y'
#define HAVE_UNLOCKED_IOCTL 1

#define NFC_MAX_CMD 2
#define NFC_READ              _IOW(HI_ID_NFC, 0, int)
#define NFC_WRITE             _IOR(HI_ID_NFC, 1, int)

#define NFC_DBG_FLAG 1

#ifdef NFC_DBG_FLAG
#define NFC_DBG(fmt, arg...) printk(KERN_INFO "nfc: " fmt "\n", ##arg)
#define NFC_INFO(fmt, arg...) printk(KERN_INFO "nfc: " fmt "\n", ##arg)
#define NFC_WARN(fmt, arg...) printk(KERN_WARNING "nfc: " fmt "\n", ##arg)
#define NFC_ERR(fmt, arg...) printk(KERN_ERR "nfc: " fmt "\n", ##arg)
#else
#define NFC_DBG(fmt, arg...)
#define NFC_INFO(fmt, arg...)
#define NFC_WARN(fmt, arg...)
#define NFC_ERR(fmt, arg...)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
