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

#ifndef __DRV_BASE_EXT_H__
#define __DRV_BASE_EXT_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct platform_driver g_hi_virt_pltm_driver;

int hi_get_irq_byname(char *name);

#ifdef HI_UNIFIED_IMAGE_SUPPORT
extern struct platform_driver g_hi_unifiedimg_pltm_driver;
/**
 * hi_get_dts_config_u32array_byname - Used to get the u32 array information from DTS
 *
 * @node_name: ptr to node name in device tree, example: panel
 * @name: ptr to propertiy name in device tree, example: i2c_reg_addr_offset
 * @out_values: ptr to u32 array
 * @num: num of u32 array
 * returns: if fail, return <0, if success, return the real len of array.
 */
hi_s32 hi_get_dts_config_u32array_byname(hi_char *node_name,
    hi_char *name, hi_u32 *out_values, hi_u32 num);

/**
 * hi_get_dts_config_bool_byname - Used to get the bool information from DTS
 *
 * @node_name: ptr to node name in device tree, example: panel
 * @name: ptr to propertiy name in device tree, example: pwm_support
 * returns: if exist name, return non-NULL, if not exist name, return NULL.
 */
hi_bool hi_get_dts_config_bool_byname(hi_char *node_name, hi_char *name);

/**
 * hi_get_dts_config_u32_byname - Used to get u32 information from DTS
 *
 * @node_name: ptr to node name in device tree, example: panel
 * @name: ptr to propertiy name in device tree, example: pwm_value
 * @out_value: ptr to u32 value in device tree
 * returns: if exist name, return 0, if not exist name, return non 0.
 */
hi_s32 hi_get_dts_config_u32_byname(hi_char *node_name,
    hi_char *name, hi_u32 *out_value);

/**
 * hi_get_dts_config_string_byname - Used to get string information from DTS
 *
 * @node_name: ptr to node name in device tree, example: panel
 * @name: ptr to propertiy name in device tree, example: pwm_value
 * @out_string: ptr to string value in device tree
 * returns: if exist name, return >=0, if not exist name, return <0.
 */
hi_s32 hi_get_dts_config_string_byname(hi_char *node_name,
    hi_char *name, const hi_char **out_string);
#endif

#ifdef CMN_TEST_SUPPORTED
hi_void hi_drv_test_init(HI_VOID);
hi_void hi_drv_test_exit(HI_VOID);
#endif

#ifdef __cplusplus
}
#endif

#endif

