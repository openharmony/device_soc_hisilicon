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
 * Description: WAPI Implementation
 */

#ifndef WAI_SM_H
#define WAI_SM_H

int wai_sm_process(struct wpa_supplicant *wpa, const struct wpabuf *waibuf);
int wapi_psk_to_bk(struct wpa_supplicant *wpa, const struct wpa_ssid *ssid);

#endif /* end of wai_sm.h */
