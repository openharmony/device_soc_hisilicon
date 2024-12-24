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

#ifndef MSG_ARP_OFFLOAD_ROM_H
#define MSG_ARP_OFFLOAD_ROM_H

#include "osal_types.h"
#include "oal_types_device_rom.h"

#define MAC_MAX_IPV4_ENTRIES 8
#define MAC_MAX_IPV6_ENTRIES 8

typedef union {
    osal_u32 value;
    osal_u8 auc_value[OAL_IPV4_ADDR_SIZE];
} un_ipv4_addr;

typedef struct {
    un_ipv4_addr local_ip;
    un_ipv4_addr mask;
} mac_vap_ipv4_addr_stru;

typedef struct {
    osal_u8 ip_addr[OAL_IPV6_ADDR_SIZE];
} mac_vap_ipv6_addr_stru;

typedef struct {
    mac_vap_ipv4_addr_stru ipv4_entry[MAC_MAX_IPV4_ENTRIES];
    mac_vap_ipv6_addr_stru ipv6_entry[MAC_MAX_IPV6_ENTRIES];
} mac_vap_ip_entries_stru;

typedef struct {
    mac_vap_ip_entries_stru ip_addr;
    oal_bool_enum_uint8 arp_offload_switch;
    osal_u16 free_arp_interval;
    osal_u8 resv[1];
} mac_arp_offload_param_sync;

#endif