/*
 * Hostapd
 * Copyright (c) 2003-2016, Jouni Malinen <j@w1.fi>
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
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This file implements functions for registering and unregistering
 * %wpa_supplicant interfaces. In addition, this file contains number of
 * functions for managing network connections.
 */

#ifndef _HOSTAPD_IF_H_
#define _HOSTAPD_IF_H_

#include "ap/ap_config.h"
#include "ap/hostapd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

void hostapd_main(const char *ifname);
void hostapd_global_deinit(void);
void hostapd_global_interfaces_deinit(void);
void hostapd_interface_deinit_free(struct hostapd_iface *iface);
int hostapd_global_ctrl_iface_init(struct hapd_interfaces *interface);
void hostapd_global_ctrl_iface_deinit(struct hapd_interfaces *interfaces);
struct hapd_interfaces * hostapd_get_interfaces(void);
struct hostapd_config * hostapd_config_read2(const char *fname);
int hostapd_stop(void);
int hostapd_ctrl_iface_init(struct hostapd_data *hapd);
void hostapd_ctrl_iface_deinit(struct hostapd_data *hapd);
void hostapd_pre_quit(struct hapd_interfaces *interfaces);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
