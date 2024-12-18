/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: general purpose register header file.
 */

#ifndef HAL_GP_REG_H
#define HAL_GP_REG_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "osal_types.h"
#ifdef BUILD_UT
#include "wifi_ut_stub.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 宏定义
*****************************************************************************/
#ifndef BUILD_UT
#define GLB_CTRL_RB_BASE_ADDR   (0x40000000)
#else
#define GLB_CTRL_RB_BASE_ADDR   (osal_u32)g_glb_ctrl_rb_addr
#endif

#define GLB_COMMON_REG0_ADDR    (GLB_CTRL_RB_BASE_ADDR + 0x4)
#define GLB_COMMON_REG1_ADDR    (GLB_CTRL_RB_BASE_ADDR + 0x8)
#define GLB_COMMON_REG2_ADDR    (GLB_CTRL_RB_BASE_ADDR + 0xC)
#define GLB_COMMON_REG3_ADDR    (GLB_CTRL_RB_BASE_ADDR + 0x10)

#define GLB_COMMON_REG_NUM      4
#define GLB_COMMON_REG_ADDR_MIN (GLB_COMMON_REG0_ADDR)
#define GLB_COMMON_REG_ADDR_MAX (GLB_COMMON_REG3_ADDR)

/*****************************************************************************
  3 函数声明
*****************************************************************************/
osal_void hal_gp_set_btcoex_wifi_status(osal_u32 val);
osal_u32 hal_gp_get_btcoex_wifi_status(osal_void);
osal_u32 hal_gp_get_btcoex_bt_status(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* end of hal_gp_reg_rom.h */