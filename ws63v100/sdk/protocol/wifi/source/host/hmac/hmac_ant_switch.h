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
 * Description: the head file for hmac ant switch
 * Create: 2022-5-10
 */

#ifndef __HMAC_ANT_SWITCH_H__
#define __HMAC_ANT_SWITCH_H__
#include "mac_user_ext.h"
#include "mac_vap_ext.h"
#include "hal_common_ops.h"

osal_void hmac_calc_rssi_th(const hmac_user_stru *hmac_user, hal_rssi_stru *hal_rssi);
osal_void hmac_ant_rx_frame(hal_to_dmac_device_stru *hal_dev, const hmac_vap_stru *hmac_vap);
osal_void hmac_ant_tbtt_process(hal_to_dmac_device_stru *hal_dev);
#endif

