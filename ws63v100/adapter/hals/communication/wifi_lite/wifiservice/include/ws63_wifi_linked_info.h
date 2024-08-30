/*
# Copyright (C) 2024 HiHope Open Source Organization .
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/

/**
* @defgroup middleware_service_wifi_linked_info WiFi Linked Info
* @ingroup middleware_service_wifi
* @{
*/

#ifndef SERVICE_WIFI_LINKED_INFO_H
#define SERVICE_WIFI_LINKED_INFO_H
#include "wifi_device_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @if Eng
 * @brief  WiFi connection states.
 * @else
 * @brief  WiFi的连接状态。
 * @endif
 */
typedef enum {
    WS63_WIFI_DISCONNECTED,  /*!< @if Eng Disconnected.
                             @else   断连。  @endif */
    WS63_WIFI_CONNECTED,     /*!< @if Eng Connected.
                             @else   已连接。  @endif */
    WS63_WIFI_CONNECTING,    /*!< @if Eng Connecting.
                             @else   连接中。  @endif */
    WS63_WIFI_CONN_STATUS_BUTT,
} wifi_conn_state_enum;

/**
 * @if Eng
 * @brief  The information about the AP connected to this station.
 * @else
 * @brief  有关连接到此STA的AP信息。
 * @endif
 */
typedef struct {
    int8_t ssid[WIFI_MAX_SSID_LEN];     /*!< @if Eng Service set ID (SSID).
                                             @else   SSID。  @endif */
    uint8_t bssid[WIFI_MAC_LEN];        /*!< @if Eng Basic service set ID (BSSID).
                                             @else   BSSID。  @endif */
    int8_t  reserved;
    int32_t rssi;                       /*!< @if Eng Received signal strength indicator (RSSI).
                                             @else   RSSI。  @endif */
    int32_t channel_num;                /*!< @if Eng WiFi channel_num information of AP.
                                             @else   AP的WiFi信道信息。  @endif */
    int32_t snr;                        /*!< @if Eng WiFi snr information of AP.
                                             @else   AP的WiFi信噪比信息。  @endif */
    wifi_conn_state_enum conn_state;    /*!< @if Eng WiFi connection state.
                                             @else   WiFi的连接状态。  @endif */
} wifi_linked_info_stru;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // SERVICE_WIFI_LINKED_INFO_H
/** @} */

