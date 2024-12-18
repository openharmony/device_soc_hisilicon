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
 * Description: security_unified kernel system init API header file.
 *
 * Create: 2023-05-26
*/

#ifndef KAPI_INNER_H
#define KAPI_INNER_H

#include "crypto_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 kapi_cipher_hash_env_init(td_void);

td_s32 kapi_cipher_hash_env_deinit(td_void);

td_s32 kapi_cipher_symc_env_init(td_void);

td_s32 kapi_cipher_symc_env_deinit(td_void);

td_s32 kapi_cipher_trng_env_init(td_void);

td_s32 kapi_cipher_trng_env_deinit(td_void);

td_s32 kapi_cipher_pke_env_init(td_void);

td_s32 kapi_cipher_pke_env_deinit(td_void);

td_s32 kapi_km_env_init(td_void);

td_s32 kapi_km_env_deinit(td_void);

td_s32 kapi_env_init(td_void);

td_void kapi_env_deinit(td_void);

td_void inner_kapi_trng_lock(td_void);

td_void inner_kapi_trng_unlock(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif