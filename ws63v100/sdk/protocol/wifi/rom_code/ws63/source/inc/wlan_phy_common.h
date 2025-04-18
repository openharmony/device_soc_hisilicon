﻿/*
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
 * Description: wlan phy
 * Date: 2022-11-26
 */

#ifndef __WLAN_PHY_COMMON_H__
#define __WLAN_PHY_COMMON_H__

#include "wlan_phy_common_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// 仅11n时有效
typedef enum {
    WLAN_PHY_SMOOTH_TYPE_NOT_RECOMMEDED = 0,
    WLAN_PHY_SMOOTH_TYPE_RECOMMEDED     = 1,
    WLAN_PHY_SMOOTH_TYPE_BUTT
} wlan_phy_smoothing_mode_enum;

// 仅11n时有效
typedef enum {
    WLAN_PHY_UPLINK_FLAG_VALID      = 0,
    WLAN_PHY_UPLINK_FLAG_INVALID    = 1,
    WLAN_PHY_UPLINK_FLAG_BUTT
} wlan_phy_uplink_flag_enum;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_phy_common.h */
