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

#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#include "hi_type.h"
#include "drv_gpio_ioctl.h"
#include "hi_drv_gpio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum  {
    GPIO_DRV_CHIP_V350 = 0x350,
    GPIO_DRV_CHIP_BUTT
} gpio_drv_chip;

hi_s32 drv_gpio_query_int(gpio_int *p_gpio_int_value);

#if 1
hi_s32 drv_gpio_open(hi_void *private_data);
hi_s32 drv_gpio_close(hi_void *private_data);
#endif

hi_s32 drv_gpio_set_output_type(hi_u32 gpio_no, gpio_outputtype en_output_type);
hi_s32 drv_gpio_get_output_type(hi_u32 gpio_no, gpio_outputtype *p_en_output_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
