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
 * Description: hmac_btcoex_btsta.c的头文件
 * Date: 2023-01-16 15:01
 */

#ifndef __HMAC_BTCOEX_BTSTA_H__
#define __HMAC_BTCOEX_BTSTA_H__

#include "oam_ext_if.h"
#include "mac_vap_ext.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void hmac_btcoex_status_dispatch(hmac_vap_stru *hmac_vap,
    const hal_btcoex_btble_status_stru *status_old, hal_btcoex_btble_status_stru *status_new);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of __HMAC_BTCOEX_BTSTA_H__ */
