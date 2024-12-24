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
 * Description: cipher driver klad header. \n
 *
 * History: \n
 * 2023-03-22, Create file. \n
 */
#ifndef DRV_KLAD_H
#define DRV_KLAD_H

#include "td_type.h"
#include "hal_klad.h"
#include "hal_rkp.h"
#include "crypto_km_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_klad_create(td_handle *klad_handle);

td_s32 drv_klad_destroy(td_handle klad_handle);

td_s32 drv_klad_attach(td_handle klad_handle, crypto_klad_dest klad_dest, td_handle keyslot_handle);

td_s32 drv_klad_detach(td_handle klad_handle, crypto_klad_dest klad_dest, td_handle keyslot_handle);

td_s32 drv_klad_set_attr(td_handle klad_handle, const crypto_klad_attr *klad_attr);

td_s32 drv_klad_get_attr(td_handle klad_handle, crypto_klad_attr *attr);

td_s32 drv_klad_set_clear_key(td_handle klad_handle, const crypto_klad_clear_key *clear_key);

td_s32 drv_klad_set_effective_key(td_handle klad_handle, const crypto_klad_effective_key *content_key);

td_s32 drv_kdf_update(crypto_kdf_otp_key otp_key, crypto_kdf_update_alg alg);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif