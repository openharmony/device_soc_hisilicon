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
#ifndef __DRV_I2C_IOCTL_H__
#define __DRV_I2C_IOCTL_H__

#include "drv_i2c_struct.h"

typedef struct {
    hi_u32 i2c_num;
    hi_u8 i2c_dev_addr;
    hi_u32 i2c_reg_addr;
    hi_u32 i2c_reg_count;
    hi_u64 p_data;
    hi_u32 data_len;
} i2c_data;

typedef struct {
    hi_u32 i2c_num;
    hi_u32 i2c_rate;
} i2c_rate;

typedef struct {
    hi_u32 i2c_num;
    hi_u32 scl_gpio_no;
    hi_u32 sda_gpio_no;
    hi_bool b_used;
    hi_u32 count;
} i2c_gpio;

/* Ioctl definitions */
#define CMD_I2C_READ _IOW(HI_ID_I2C, 0x1, i2c_data) // 1:check keyup
#define CMD_I2C_WRITE _IOW(HI_ID_I2C, 0x2, i2c_data) // 1:check repkey, 0:hardware behave
#define CMD_I2C_SET_RATE _IOW(HI_ID_I2C, 0x3, i2c_rate)
#define CMD_I2C_CONFIG _IOWR(HI_ID_I2C, 0x4, i2c_gpio) // config  gpioi2c
#define CMD_I2C_DESTROY _IOW(HI_ID_I2C, 0x5, i2c_gpio)      // destroy gpioi2c
#define CMD_I2C_READ_DIRECT _IOW(HI_ID_I2C, 0x6, i2c_data)

#endif /* End of #ifndef __DRV_I2C_IOCTL_H__ */
