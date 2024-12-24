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

#ifndef MBEDTLS_HASH_HARDEN_COMMON_H
#define MBEDTLS_HASH_HARDEN_COMMON_H

#include "cipher_adapt.h"
#include "mbedtls/platform.h"
#include "md_wrap.h"

typedef struct {
    mbedtls_md_type_t md_type;
    crypto_hash_type hash_type;
    size_t olen;
    size_t block_size;
} md_type_info;

typedef struct {
    crypto_hash_type hash_type;
    km_keyslot_engine key_engine;
} kslot_hmac_engine_map;

typedef struct {
    crypto_hash_type hmac_type;
    crypto_hash_type hash_type;
} hmac_hash_map;

void get_km_keyslot_engine(crypto_hash_type hash_type, km_keyslot_engine *key_engine);

void get_hash_info( mbedtls_md_type_t md_type, crypto_hash_type *hash_type, unsigned int *olen,
    unsigned int *block_size);

int get_hmac_hashed_key( crypto_hash_attr *hmac_attr, unsigned char *key, size_t keylen );

#endif /* MBEDTLS_HASH_HARDEN_COMMON_H */