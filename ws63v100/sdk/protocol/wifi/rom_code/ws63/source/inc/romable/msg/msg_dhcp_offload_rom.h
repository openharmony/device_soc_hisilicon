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
 * Description: Header file
 */

#ifndef MSG_DHCP_OFFLOAD_ROM_H
#define MSG_DHCP_OFFLOAD_ROM_H
 
#include "mac_frame_common_rom.h"
 
typedef struct {
    osal_u8 cipher_key_type;
    osal_u8 cipher_protocol_type;
    osal_u8 cipher_key_id;
    osal_u8 ra_lut_index;
} mac_cipher_info_stru;
 
typedef struct {
    dhcp_record_frame dhcp_request;
    mac_ieee80211_frame_stru dhcp_request_header;
    osal_u32 lease_time;
    osal_u32 request_ip;
    osal_u32 server_ip;
    osal_u32 dhcp_offload_timeout;
    osal_u32 dhcp_renew_retry;
    mac_cipher_info_stru cipher_info;
} mac_dhcp_offload_param_sync;
 
#endif