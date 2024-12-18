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
 * Description: dbac algorithm
 */

#ifndef ALG_DBAC_SOFTAP_ROM_H
#define ALG_DBAC_SOFTAP_ROM_H

#ifdef _PRE_WLAN_FEATURE_DBAC
/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "oal_types_device_rom.h"
#include "alg_main_rom.h"
#include "alg_dbac_rom.h"
#include "mac_device_rom.h"
#include "frw_ext_if_rom.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define DBAC_SOFTAP_ONE_PACKET_DURATION 20

osal_void alg_dbac_sta_softap_tbtt_isr(alg_dbac_mgr_stru *dbac_mgr, osal_bool is_led_tbtt);
osal_u32 alg_dbac_sta_softap_timer_isr(alg_dbac_mgr_stru *dbac_mgr);
osal_u32 alg_dbac_parse_type_sta_ap_vap(const dmac_vap_stru *dmac_vap0, const dmac_vap_stru *dmac_vap1,
    alg_dbac_type_enum_uint8 *dbac_type, osal_u8 *led_vapid, osal_u8 *flw_vapid);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _PRE_WLAN_FEATURE_DBAC */
#endif
