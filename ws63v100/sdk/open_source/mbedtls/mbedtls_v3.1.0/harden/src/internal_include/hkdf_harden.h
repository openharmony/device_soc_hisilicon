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

#ifndef MBEDTLS_HKDF_HARDEN_H
#define MBEDTLS_HKDF_HARDEN_H

#include "mbedtls/hkdf.h"

int hkdf_harden_can_do( const mbedtls_md_info_t *md );

int hkdf_extract_harden( const mbedtls_md_info_t *md,
        const unsigned char *salt, size_t salt_len,
        const unsigned char *ikm, size_t ikm_len,
        unsigned char *prk );

int hkdf_expand_harden( const mbedtls_md_info_t *md, const unsigned char *prk,
        size_t prk_len, const unsigned char *info,
        size_t info_len, unsigned char *okm, size_t okm_len );

int hkdf_harden( const mbedtls_md_info_t *md, const unsigned char *salt,
        size_t salt_len, const unsigned char *ikm, size_t ikm_len,
        const unsigned char *info, size_t info_len,
        unsigned char *okm, size_t okm_len );

#endif /* MBEDTLS_HKDF_HARDEN_H */