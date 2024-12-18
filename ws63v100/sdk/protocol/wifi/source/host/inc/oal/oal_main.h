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
 * Description: main head file.
 */

#ifndef __OAL_MAIN_H__
#define __OAL_MAIN_H__

#include "oal_ext_if.h"
#include "oal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define WLAN_INIT_DEVICE_RADIO_CAP  (1)  /* WLAN使能2G */

extern osal_s32  oal_main_init_etc(osal_void);
extern osal_void  oal_main_exit_etc(osal_void);
extern osal_u32 oal_chip_get_version_etc(osal_void);
extern osal_u8 oal_chip_get_device_num_etc(osal_u32 chip_ver);
extern osal_u8 oal_board_get_service_vap_start_id(osal_void);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of oal_main */
