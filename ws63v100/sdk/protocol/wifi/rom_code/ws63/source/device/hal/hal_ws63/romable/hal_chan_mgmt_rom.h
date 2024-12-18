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
 * Description: Header file for hal_irq_rom.c.
 */

#ifndef __HAL_CHAN_MGMT_ROM_H__
#define __HAL_CHAN_MGMT_ROM_H__

#include "hal_ext_if_rom.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_CHAN_H

typedef osal_u32 (*hal_set_primary_channel_cb)(hal_to_dmac_device_stru *hal_device, osal_u8 channel_num,
    wlan_channel_band_enum_uint8 band, osal_u8 channel_idx, wlan_channel_bandwidth_enum_uint8 bandwidth);
typedef osal_u32 (*hal_set_channel_freq_cb)(hal_to_dmac_device_stru *hal_device,
    wlan_channel_band_enum_uint8 band);
typedef osal_u32 (*hal_set_bandwidth_mode_cb)(hal_to_dmac_device_stru *hal_device,
    wlan_channel_bandwidth_enum_uint8 bandwidth);

#endif /* __HAL_CHAN_MGMT_ROM_H__ */

