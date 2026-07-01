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

#ifndef __OT_IR_HAL_H__
#define __OT_IR_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OT_IR_DEVICE_IRQ_NO 55 /* 55 is IR Interrupt source */
#define IR_REG_BASE 0x110f0000 /* 0x110f0000 is IR reg base Address */

#define IR_CRG_ADDR 0x11014640 /* 0x11014640 is IR CRG Address */
#define IR_BIT 4 /* 4bit is IR CRG Clock enable */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
