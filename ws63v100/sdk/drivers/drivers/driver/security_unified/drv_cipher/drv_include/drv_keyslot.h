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
 *
 * Description: cipher driver keyslot header. \n
 *
 * History: \n
 * 2023-03-22, Create file. \n
 */
#ifndef DRV_KEYSLOT_H
#define DRV_KEYSLOT_H

#include "td_type.h"
#include "hal_keyslot.h"
#include "crypto_km_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_keyslot_init(td_void);

td_s32 drv_keyslot_deinit(td_void);

td_s32 drv_keyslot_create(td_handle *keyslot_handle, crypto_keyslot_type keyslot_type);

td_s32 drv_keyslot_destroy(td_handle keyslot_handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif