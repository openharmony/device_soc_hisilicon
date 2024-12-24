/**
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
 * Description: BLE config wifi client scan
*/

#include "errcode.h"
#include "bts_def.h"
#include "bts_le_gap.h"
#include "ble_wifi_cfg_scan.h"

static uint16_t scan_interval = 0x48;
static uint16_t scan_window = 0x48;
static uint8_t  scan_type = 0x00;
static uint8_t  scan_phy = 0x01;
static uint8_t  scan_filter_policy = 0x00;

errcode_t ble_wifi_set_scan_parameters(void)
{
    gap_ble_scan_params_t ble_device_scan_params = { 0 };
    ble_device_scan_params.scan_interval = scan_interval;
    ble_device_scan_params.scan_window = scan_window;
    ble_device_scan_params.scan_type = scan_type;
    ble_device_scan_params.scan_phy = scan_phy;
    ble_device_scan_params.scan_filter_policy = scan_filter_policy;
    return gap_ble_set_scan_parameters(&ble_device_scan_params);
}

errcode_t ble_wifi_device_start_scan(void)
{
    return gap_ble_start_scan();
}