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
 * Description: key manager kernel API header file.
 *
 * Create: 2023-05-26
*/

#ifndef KAPI_KM_H
#define KAPI_KM_H

#include "crypto_km_struct.h"

#define KAPI_KEYSLOT_MODULE_ID              0x03
#define KAPI_KLAD_MODULE_ID                 0x04

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 kapi_km_deinit(td_void);

/* Keyslot. */
td_s32 kapi_keyslot_create(td_handle *kapi_keyslot_handle, crypto_keyslot_type keyslot_type);
td_s32 kapi_keyslot_destroy(td_handle kapi_keyslot_handle);

/* Klad. */
td_s32 kapi_klad_create(td_handle *kapi_klad_handle);
td_s32 kapi_klad_destroy(td_handle kapi_klad_handle);

td_s32 kapi_klad_attach(td_handle kapi_klad_handle, crypto_klad_dest klad_type,
    td_handle kapi_keyslot_handle);
td_s32 kapi_klad_detach(td_handle kapi_klad_handle, crypto_klad_dest klad_type,
    td_handle kapi_keyslot_handle);

td_s32 kapi_klad_set_attr(td_handle kapi_klad_handle, const crypto_klad_attr *attr);
td_s32 kapi_klad_get_attr(td_handle kapi_klad_handle, crypto_klad_attr *attr);

td_s32 kapi_klad_set_effective_key(td_handle kapi_klad_handle, const crypto_klad_effective_key *effective_key);

td_s32 kapi_klad_set_clear_key(td_handle kapi_klad_handle, const crypto_klad_clear_key *key);

td_s32 kapi_kdf_update(crypto_kdf_otp_key otp_key, crypto_kdf_update_alg alg);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif