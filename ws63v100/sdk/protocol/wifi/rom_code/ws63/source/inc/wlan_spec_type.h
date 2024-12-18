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
 * Description: Header files shared by wlan_spec.h and wlan_spec_rom.h.
 * Create: 2022-11-26
 */

#ifndef __WLAN_SPEC_TYPE_H__
#define __WLAN_SPEC_TYPE_H__

#include "wlan_spec_type_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    枚举定义
*****************************************************************************/
/* hal device id 枚举 */
typedef enum {
    HAL_DEVICE_ID_MASTER        = 0,    /* master的hal device id */
    HAL_DEVICE_ID_SLAVE         = 1,    /* slave的hal device id */

    HAL_DEVICE_ID_BUTT                  /* 最大id */
} hal_device_id_enum;
typedef osal_u8 hal_device_id_enum_enum_uint8;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_spec.h */
