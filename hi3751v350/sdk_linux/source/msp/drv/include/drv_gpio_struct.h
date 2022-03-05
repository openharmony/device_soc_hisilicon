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

#ifndef __DRV_GPIO_STRUCT_H__
#define __DRV_GPIO_STRUCT_H__

#include "hi_type.h"

typedef enum {
    HI_GPIO_0_VIR_ADDR = 0,
    HI_GPIO_1_VIR_ADDR,
    HI_GPIO_2_VIR_ADDR,
    HI_GPIO_3_VIR_ADDR,
    HI_GPIO_4_VIR_ADDR,
    HI_GPIO_5_VIR_ADDR,
    HI_GPIO_6_VIR_ADDR,
    HI_GPIO_7_VIR_ADDR,
    HI_GPIO_8_VIR_ADDR,
    HI_GPIO_9_VIR_ADDR,

    /* V350 :GPIO_10-GPIO_15 is reserved */
    HI_GPIO_10_VIR_ADDR,
    HI_GPIO_11_VIR_ADDR,
    HI_GPIO_12_VIR_ADDR,
    HI_GPIO_13_VIR_ADDR,
    HI_GPIO_14_VIR_ADDR,
    HI_GPIO_15_VIR_ADDR,

    HI_GPIO_18_VIR_ADDR, /* STB_GPIO_PWM18 */
    HI_GPIO_19_VIR_ADDR, /* STB_GPIO_PWM19 */
    HI_GPIO_20_VIR_ADDR, /* STB_GPIO_PWM20 */
    /* V350 :GPIO_21、GPIO_22 is reserved */
    HI_GPIO_21_VIR_ADDR, /* STB_GPIO_PWM21 */
    HI_GPIO_22_VIR_ADDR, /* STB_GPIO_PWM22 */

    HI_GPIO_VIR_ADDR_BUTT,
} gpio_viraddr_index;

typedef enum {
    GPIO_INTTYPE_UP,
    /** <spring by the up edge */ /** < CNcomment:上升沿触发 */
    GPIO_INTTYPE_DOWN,
    /** <spring by the down edge */ /** < CNcomment:下降沿触发 */
    GPIO_INTTYPE_UPDOWN,
    /** <spring by both the up and down edge */ /** < CNcomment:双沿触发 */
    GPIO_INTTYPE_HIGH,
    /** <spring by the high level */ /** < CNcomment:高电平触发 */
    GPIO_INTTYPE_LOW,
    /** <spring by the low level */ /** < CNcomment:低电平触发 */
    GPIO_INTTYPE_BUTT,
    /** <Invalid value */ /** < CNcomment:非法边界值 */
} gpio_inttpye;

typedef struct {
    HI_U8 gpio_grp_num; /* gpio group number */
    HI_U8 gpio_max_num; /* gpio  max number */
} gpio_get_gpionum;

typedef struct {
    hi_u32 gpio_no;
    hi_u32 b_enable;
    gpio_inttpye en_int_type; /* gpio interrupt trigger mode */
} gpio_data;

typedef struct {
    hi_u32 gpio_no;
    hi_u32 timeout_ms;
} gpio_int;

#endif /* End of #ifndef __HI_GPIO_STRUCT_H__ */
