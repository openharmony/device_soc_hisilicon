/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: WAPI APIs
 */

#ifndef WAPI_API_H
#define WAPI_API_H

#include "wpa_supplicant_i.h"

int wapi_init_iface(struct wpa_supplicant *wpa);
int wapi_deinit_iface(struct wpa_supplicant *wpa);
int wapi_parse_wapi_ie(const unsigned char *wapi_ie, unsigned int ie_len,
    struct wpa_ie_data *ie_data);
int wapi_asue_event_assoc(struct wpa_supplicant *wpa,
    struct wpa_bss *bss, struct wpa_ssid *ssid,
    struct wpa_driver_associate_params *params);
int wapi_asue_event_disassoc(struct wpa_supplicant *wpa,
    unsigned short reason);
int wapi_asue_event(struct wpa_supplicant *wpa,
    enum wpa_event_type event, void *data);
char * wpa_supplicant_wapi_ie_txt(char *pos, char *end,
    const unsigned char *ie, size_t ie_len);

#endif /* end of WAPI_API_H */
