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

#ifndef __DRV_I2C_H_
#define __DRV_I2C_H_

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#if defined(HI_FPGA)
#define I2C_DFT_SYSCLK (12500000)
#else
#define I2C_DFT_SYSCLK (100000000)
#endif

#define I2C_DFT_RATE (100000)

#define I2C0_PHY_ADDR  (0xF8B10000)
#define I2C1_PHY_ADDR  (0xF8B11000)
#define I2C2_PHY_ADDR  (0xF8B12000)
#define I2C3_PHY_ADDR  (0xF8B13000)
#define I2C_TUNER_ADDR (0xF8B1D000)
#define I2C_DTMB_ADDR  (0xFF2F1000)
#define I2C_ADC_ADDR   (0xFF2F2000)

#define I2C_CTRL_REG  (0x000)
#define I2C_COM_REB   (0x004)
#define I2C_ICR_REG   (0x008)
#define I2C_SR_REG    (0x00C)
#define I2C_SCL_H_REG (0x010)
#define I2C_SCL_L_REG (0x014)
#define I2C_TXR_REG   (0x018)
#define I2C_RXR_REG   (0x01C)

#define READ_OPERATION  (1)
#define WRITE_OPERATION 0xfe

/* I2C_CTRL_REG */
#define I2C_ENABLE           (1 << 8)
#define I2C_UNMASK_TOTAL     (1 << 7)
#define I2C_UNMASK_START     (1 << 6)
#define I2C_UNMASK_END       (1 << 5)
#define I2C_UNMASK_SEND      (1 << 4)
#define I2C_UNMASK_RECEIVE   (1 << 3)
#define I2C_UNMASK_ACK       (1 << 2)
#define I2C_UNMASK_ARBITRATE (1 << 1)
#define I2C_UNMASK_OVER      (1 << 0)
#define I2C_UNMASK_ALL       (I2C_UNMASK_START | I2C_UNMASK_END |  \
                        I2C_UNMASK_SEND | I2C_UNMASK_RECEIVE |  \
                        I2C_UNMASK_ACK | I2C_UNMASK_ARBITRATE |  \
                        I2C_UNMASK_OVER)

/* I2C_COM_REB */
#define I2C_SEND_ACK (~(1 << 4))
#define I2C_START    (1 << 3)
#define I2C_READ     (1 << 2)
#define I2C_WRITE    (1 << 1)
#define I2C_STOP     (1 << 0)

/* I2C_ICR_REG */
#define I2C_CLEAR_START     (1 << 6)
#define I2C_CLEAR_END       (1 << 5)
#define I2C_CLEAR_SEND      (1 << 4)
#define I2C_CLEAR_RECEIVE   (1 << 3)
#define I2C_CLEAR_ACK       (1 << 2)
#define I2C_CLEAR_ARBITRATE (1 << 1)
#define I2C_CLEAR_OVER      (1 << 0)
#define I2C_CLEAR_ALL       (I2C_CLEAR_START | I2C_CLEAR_END |  \
                       I2C_CLEAR_SEND | I2C_CLEAR_RECEIVE |  \
                       I2C_CLEAR_ACK | I2C_CLEAR_ARBITRATE |  \
                       I2C_CLEAR_OVER)

/* I2C_SR_REG */
#define I2C_BUSY           (1 << 7)
#define I2C_START_INTR     (1 << 6)
#define I2C_END_INTR       (1 << 5)
#define I2C_SEND_INTR      (1 << 4)
#define I2C_RECEIVE_INTR   (1 << 3)
#define I2C_ACK_INTR       (1 << 2)
#define I2C_ARBITRATE_INTR (1 << 1)
#define I2C_OVER_INTR      (1 << 0)

hi_void i2c_drv_set_rate(hi_u32 i2c_num, hi_u32 i2c_rate);
hi_s32 i2c_drv_get_rate(hi_u32 i2c_num);
hi_s32 hi_drv_i2c_cmd_write(unsigned int cmd, hi_void *arg, hi_void *private_data);
hi_s32 hi_drv_i2c_cmd_read(unsigned int cmd, hi_void *arg, hi_void *private_data);
hi_s32 hi_drv_i2c_cmd_set_rate(unsigned int cmd, hi_void *arg, hi_void *private_data);

#define I2C0 (0)
#define I2C1 (1)
#define I2C2 (2)
#define I2C3 (3)
#define I2C7 (4)

hi_s32 i2c_pm_resume(hi_void *private_data);
hi_s32 i2c_pm_suspend(hi_void *private_data);
hi_s32 i2c_drv_lowpower_enter(hi_void *private_data);
hi_s32 i2c_drv_lowpower_exit(hi_void *private_data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
