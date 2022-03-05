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
#ifndef __DRV_AMP_EXT_H__
#define __DRV_AMP_EXT_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/** defines the info of amp config. */
/** c_ncomment:定义功放的配置信息 */
typedef struct amp_info {
    /** <device type of amp */
    /** <c_ncomment:功放的型号 */
    hi_u8 device_type;
    /** <device address of amp when communicate with i2c */
    /** <c_ncomment:功放使用I2C通信时使用的器件地址 */
    hi_u8 device_addr;
    /** <GPIO output mode polarity,it is 0 normally */
    /** <c_ncomment:GPIO配置为输出模式的配置，一般配置为0 */
    hi_u8 gpio_output_polarity;
    /** <GPIO number of amp reset pin,if it is GPIO1_5,then the result is 1*8+5=13 */
    /** <c_ncomment:功放的复位管脚的GPIO号，如GPIO1_5,则为1*8+5=13 */
    hi_u8 reset_gpio_num;
    /** <polarity of reset pin when amp reset  */
    /** <c_ncomment:功放复位时复位管脚的电平 */
    hi_u8 reset_polarity;
    /** <GPIO number of amp mute pin,if it is GPIO1_5, then the result is 1*8+5=13 */
    /** <c_ncomment:功放的静音管脚的GPIO号，如GPIO1_5,则为1*8+5=13 */
    hi_u8 hw_mute_gpio_num;
    /** <polarity of mute pin when amp mute is enable */
    /** <c_ncomment:功放静音时复位管脚的电平 */
    hi_u8 hw_mute_polarity;
    /** <I2C channel number that amp use */
    /** <c_ncomment:功放使用的I2C通道号 */
    hi_u8 i2c_num;
} amp_info;

typedef hi_s32 (*fn_amp_init)(void);

typedef hi_s32 (*fn_amp_de_init)(hi_bool suspend);

typedef hi_s32 (*fn_amp_set_mute)(hi_bool mute);

typedef hi_void (*fn_amp_show_info)(void);

typedef struct tag_amp_export_func {
    fn_amp_init amp_init;
    fn_amp_de_init amp_de_init;
    fn_amp_set_mute amp_set_mute;
    fn_amp_show_info amp_show_info;
} amp_export_func;

hi_s32  amp_drv_mod_init(void);
hi_void amp_drv_mod_exit(void);
hi_s32  hi_drv_amp_init(hi_u32 sampel_rate, hi_bool resume);
hi_void hi_drv_amp_deinit(hi_bool suspend);
hi_void hi_drv_amp_set_mute(hi_bool mute);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_AMP_EXT_H__ */

