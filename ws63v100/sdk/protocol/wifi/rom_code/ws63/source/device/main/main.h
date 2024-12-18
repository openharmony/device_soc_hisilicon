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
 * Description: Head file of MAIN element structure and its corresponding enumeration.
 */

#ifndef MAIN_H
#define MAIN_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_ext_if_rom.h"
#include "hcc_cfg_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAIN_H

/*****************************************************************************
  10 函数声明
*****************************************************************************/
osal_void cali_closeout_mem_cfg(osal_void);
osal_void send_bsp_ready(osal_void);
osal_void device_main_init(osal_void);
osal_void device_main_exit(osal_void);
osal_s32  device_module_init(osal_void);
osal_s32  dmac_main_init(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
