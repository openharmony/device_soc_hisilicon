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
 * Description: station_info.h 的头文件
 */


/**
* @defgroup middleware_service_wifi_station_info WiFi Station Info
* @ingroup middleware_service_wifi
* @{
*/

#ifndef SERVICE_WIFI_STATION_INFO_H
#define SERVICE_WIFI_STATION_INFO_H
#include "wifi_device_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @if Eng
 * @brief  Returns the information about the STA connected to the AP.
 * @else
 * @brief  返回与AP相连的STA信息。
 * @endif
 */
typedef struct {
    uint8_t mac_addr[WIFI_MAC_LEN];     /*!< @if Eng MAC address.
                                             @else  MAC地址。  @endif */
    int8_t rssi;                        /*!< @if Eng Received signal strength indicator (RSSI).
                                             @else  RSSI。  @endif */
    int8_t rsv;                         /*!< @if Eng Reserved.
                                             @else  保留字段。  @endif */
    uint32_t best_rate;                 /*!< @if Eng Best sending rate (kbps).
                                             @else  softap上一次接收相连的station报文最佳发送速率值。  @endif */
} wifi_sta_info_stru;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // SERVICE_WIFI_STATION_INFO_H
/** @} */
