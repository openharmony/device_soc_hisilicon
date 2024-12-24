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
 * Description: mbedtls harden adapt internal header file.
 *
 * Create: 2023-05-10
*/

#ifndef MBEDTLS_PBKDF2_HMAC_HARDEN_H
#define MBEDTLS_PBKDF2_HMAC_HARDEN_H

#include "mbedtls/pkcs5.h"

int pbkdf2_hmac_harden_can_do( mbedtls_md_context_t *ctx, unsigned int iteration_count );

int pbkdf2_hmac_harden( mbedtls_md_context_t *ctx,
        const unsigned char *password,
        size_t plen, const unsigned char *salt, size_t slen,
        unsigned int iteration_count,
        uint32_t key_length, unsigned char *output );

#endif /* MBEDTLS_PBKDF2_HMAC_HARDEN_H */