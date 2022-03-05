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

#ifndef __HI_DRV_GPIO_I2C_H__
#define __HI_DRV_GPIO_I2C_H__

#include <linux/kernel.h>
#include "hi_type.h"

hi_u32 hi_drv_i2c_get_chip_version(hi_void);
hi_s32 hi_drv_gpioi2c_init(hi_void);
hi_void hi_drv_gpioi2c_de_init(hi_void);

hi_s32 hi_drv_gpioi2c_sccb_read(hi_u32 i2c_num, HI_U8 u8_devaddr, HI_U8 u8_reg_address, HI_U8 *pu8_data);

hi_s32 hi_drv_gpioi2c_read(hi_u32 i2c_num, HI_U8 u8_devaddr, HI_U8 u8_reg_address, HI_U8 *pu8_data);
hi_s32 hi_drv_gpioi2c_write(hi_u32 i2c_num, HI_U8 u8_devaddr, HI_U8 u8_reg_address, HI_U8 u8_data);
hi_s32 hi_drv_gpioi2c_create_gpio_i2c(hi_u32 *pu32_i2c_num, hi_u32 scl_gpio_no, hi_u32 sda_gpio_no);
hi_s32 hi_drv_gpioi2c_destroy_gpio_i2c(hi_u32 i2c_num);

hi_s32 hi_drv_gpioi2c_read_ext(hi_u32 i2c_num, HI_U8 devaddress, hi_u32 address, hi_u32 addresslen,
    HI_U8 *p_data, hi_u32 data_len);
hi_s32 hi_drv_gpioi2c_read_ext_directly(hi_u32 i2c_num, HI_U8 devaddress, hi_u32 address,
    hi_u32 addresslen, HI_U8 *p_data, hi_u32 data_len);
hi_s32 hi_drv_gpioi2c_write_ext(hi_u32 i2c_num, HI_U8 devaddress, hi_u32 address,
    hi_u32 addresslen, HI_U8 *p_data, hi_u32 data_len);
hi_s32 hi_drv_gpioi2c_write_ext_no_stop(hi_u32 i2c_num, HI_U8 devaddress,
    hi_u32 address, hi_u32 addresslen, HI_U8 *p_data, hi_u32 data_len);

int hi_drv_gpioi2c_cmd_read(unsigned int cmd, hi_void *arg, hi_void *private_data);
int hi_drv_gpioi2c_cmd_write(unsigned int cmd, hi_void *arg, hi_void *private_data);
int hi_drv_gpioi2c_cmd_config(unsigned int cmd, hi_void *arg, hi_void *private_data);
int hi_drv_gpioi2c_cmd_destroy(unsigned int cmd, hi_void *arg, hi_void *private_data);

typedef enum  {
    I2C_DRV_CHIP_V350 = 0x350,

    I2C_DRV_CHIP_BUTT
} i2c_drv_chip;


typedef hi_s32 (*fn_gpio_i2c_write)(hi_u32 i2c_num, hi_u8 dev_addr, hi_u8 reg_address, hi_u8 data);
typedef hi_s32 (*fn_gpio_i2c_write_ext)(hi_u32, hi_u8, hi_u32, hi_u32, hi_u8 *, hi_u32);

typedef hi_s32 (*fn_gpio_i2c_read)(hi_u32 i2c_num, hi_u8 dev_addr, hi_u8 reg_address, hi_u8 *p_data);
typedef hi_s32 (*fn_gpio_i2c_read_ext)(hi_u32, hi_u8, hi_u32, hi_u32, hi_u8 *, hi_u32);

typedef hi_s32 (*fn_gpio_i2c_sccb_read)(hi_u32 i2c_num, hi_u8 dev_addr, hi_u8 reg_address, hi_u8 *p_data);


typedef hi_s32 (*fn_gpio_i2c_create_channel)(hi_u32 *, hi_u32, hi_u32);
typedef hi_s32 (*fn_gpio_i2c_destroy_channel)(hi_u32);
typedef hi_s32 (*fn_gpio_i2c_is_used)(hi_u32, HI_BOOL *);

typedef struct {
    fn_gpio_i2c_write pfn_gpio_i2c_write;
    fn_gpio_i2c_write_ext pfn_gpio_i2c_write_ext;
    fn_gpio_i2c_write_ext pfn_gpio_i2c_write_ext_nostop;
    fn_gpio_i2c_read pfn_gpio_i2c_read;
    fn_gpio_i2c_read_ext pfn_gpio_i2c_read_ext;
    fn_gpio_i2c_read_ext pfn_gpio_i2c_read_ext_di_rectly;

    fn_gpio_i2c_sccb_read pfn_gpio_i2c_sccb_read;
    fn_gpio_i2c_create_channel pfn_gpio_i2c_create_channel;
    fn_gpio_i2c_destroy_channel pfn_gpio_i2c_destroy_channel;
    fn_gpio_i2c_is_used pfn_gpio_i2c_is_used;
} gpio_i2c_ext_func;

#endif
