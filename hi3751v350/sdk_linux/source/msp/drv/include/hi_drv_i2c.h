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
#ifndef __HI_DRV_I2C_H__
#define __HI_DRV_I2C_H__

#include "hi_type.h"
#include "drv_i2c_struct.h"

#define HI_I2C_SPACE_SIZE 0x1000

#define HI_I2C_REG_REMAP(pvAddr, u32RegAddr) \
    do { \
        (pvAddr) = osal_ioremap_nocache((u32RegAddr), HI_I2C_SPACE_SIZE); \
        if (NULL == (pvAddr)) { \
            HI_LOG_ERR("ioremap REG_ADDR[0x%x] failure!\n", (u32RegAddr)); \
            return HI_FAILURE; \
        } \
    } while (0)

#define HI_I2C_ERR_PRINT(exp, ret, fmt...) \
    do {                                   \
        if ((exp)) {                       \
            HI_LOG_ERR(fmt);               \
            return (ret);                  \
        }                                  \
    } while (0)

#define HI_I2C_INFO_PRINT(exp, ret, fmt...) \
    do {                                    \
        if ((exp)) {                        \
            HI_LOG_INFO(fmt);               \
            return (ret);                   \
        }                                   \
    } while (0)

#define HI_I2C_REG_UNMAP(pvAddr) osal_iounmap(pvAddr)


hi_s32 hi_drv_i2c_init(hi_void);
hi_void hi_drv_i2c_de_init(hi_void);

hi_s32 hi_drv_i2c_write_config(hi_u32 i2c_num, hi_u8 i2c_dev_addr);
hi_s32 hi_drv_i2c_write(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr,
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len);
hi_s32 hi_drv_i2c_read(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr,
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len);
hi_s32 hi_drv_i2c_read_si_labs(hi_u32 i2c_num, hi_u8 i2c_dev_addr,
    hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len);
hi_s32 hi_i2c_write_sony(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num,
                         hi_u8 *p_data, hi_u32 data_len, hi_u8 u8_mode);
hi_s32 hi_i2c_read_sony(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num,
    hi_u8 *p_data, hi_u32 data_len, hi_u8 u8_mode);
hi_s32 hi_drv_i2c_read_directly(hi_u32 i2c_num, hi_u8 i2c_dev_addr,
    hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len);
hi_s32 hi_drv_i2c_write_no_stop(HI_U32 i2c_num, HI_U8 i2c_dev_addr,
    HI_U32 i2c_reg_addr, HI_U32 i2c_reg_addr_byte_num, HI_U8 *p_data, HI_U32 data_len);

typedef hi_s32 (*fn_i2c_writeconfig)(hi_u32, hi_u8);
typedef hi_s32 (*fn_i2c_write)(hi_u32, hi_u8, hi_u32, hi_u32, hi_u8 *, hi_u32);
typedef hi_s32 (*fn_i2c_read)(hi_u32, hi_u8, hi_u32, hi_u32, hi_u8 *, hi_u32);

typedef struct {
    fn_i2c_writeconfig pfn_i2c_write_config;
    fn_i2c_write pfn_i2c_write;
    fn_i2c_read pfn_i2c_read;
    fn_i2c_write pfn_i2c_write_nostop;
    fn_i2c_read pfn_i2c_read_directly;
    fn_i2c_read pfn_i2c_read_si_labs;
} i2c_ext_func;

hi_void i2c_drv_get_i2c_ext_func(i2c_ext_func **st_i2c_export_funcs);

#endif