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
 * Description: algorithm temp protect rom
 */

#ifndef ALG_TEMP_PROTECT_ROM_H
#define ALG_TEMP_PROTECT_ROM_H

/*****************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "dmac_ext_if_rom.h"
#include "frw_msg_rom.h"
#include "osal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_TEMP_PROTECT
/*****************************************************************************
  10 函数声明
******************************************************************************/
osal_u32 alg_temp_init(osal_void);
osal_u32 alg_temp_exit(osal_void);
osal_s32 dmac_alg_temp_protect_state_switch(dmac_vap_stru *dmac_vap, frw_msg *msg);
osal_s32 dmac_alg_temp_protect_pa_off_switch(dmac_vap_stru *dmac_vap, frw_msg *msg);
osal_bool alg_temp_protect_is_over_state(osal_void);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

