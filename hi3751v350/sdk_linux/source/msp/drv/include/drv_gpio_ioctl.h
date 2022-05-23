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

#ifndef __DRV_GPIO_IOCTL_H__
#define __DRV_GPIO_IOCTL_H__

#include "drv_gpio_struct.h"

typedef enum {
    GPIO_OUTPUTTYPE_CMOS,
    GPIO_OUTPUTTYPE_OD,
    GPIO_OUTPUTTYPE_BUTT,
} gpio_outputtype;

typedef struct {
    hi_u32 gpio_no;
    gpio_outputtype en_output_type;
} gpio_output_type;

typedef struct {
    hi_u32 gpio_no;
    hi_bool b_input;
} gpio_direct;

typedef struct {
    hi_u32 gpio_no;
    hi_bool b_high_volt;
} gpio_value;


/* Ioctl definitions */
#define CMD_GPIO_SET_INT_TYPE   _IOW(HI_ID_GPIO, 0x1, gpio_data)        /* set interruput condition */
#define CMD_GPIO_SET_INT_ENABLE _IOW(HI_ID_GPIO, 0x2, gpio_data)        /* set interruput enable or disable */
#define CMD_GPIO_GET_INT        _IOWR(HI_ID_GPIO, 0x3, gpio_int)         /* get interruput occur */
#define CMD_GPIO_GET_GPIONUM    _IOR(HI_ID_GPIO, 0x4, gpio_get_gpionum) /* get gpio max number and group number */
#define CMD_GPIO_SET_OUTPUTTYPE _IOW(HI_ID_GPIO, 0x5, gpio_output_type)
#define CMD_GPIO_GET_OUTPUTTYPE _IOWR(HI_ID_GPIO, 0x6, gpio_output_type)
#define CMD_GPIO_SET_DIRECT     _IOW(HI_ID_GPIO, 0x7, gpio_direct)
#define CMD_GPIO_GET_DIRECT     _IOWR(HI_ID_GPIO, 0x8, gpio_direct)
#define CMD_GPIO_SET_VALUE      _IOW(HI_ID_GPIO, 0x9, gpio_value)
#define CMD_GPIO_GET_VALUE      _IOWR(HI_ID_GPIO, 0xa, gpio_value)

#endif /* End of #ifndef __HI_DRV_GPIO_H__ */
