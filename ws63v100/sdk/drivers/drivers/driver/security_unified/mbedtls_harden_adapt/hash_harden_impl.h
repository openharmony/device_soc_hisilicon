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
 * Description: mbedtls hash harden api.
 *
 * Create: 2023-12-23
*/

#ifndef HASH_HARDEN_IMPL_H
#define HASH_HARDEN_IMPL_H

#include "mbedtls_harden_adapt_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

int mbedtls_alt_hash_start_impl(mbedtls_alt_hash_clone_ctx *clone_ctx, mbedtls_alt_hash_type hash_type);

int mbedtls_alt_hash_update_impl(mbedtls_alt_hash_clone_ctx *clone_ctx,
    const unsigned char *data, unsigned int data_len);

int mbedtls_alt_hash_finish_impl(mbedtls_alt_hash_clone_ctx *clone_ctx, unsigned char *out, unsigned int out_len);

int mbedtls_alt_hash_init(void);

int mbedtls_alt_hash_deinit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif