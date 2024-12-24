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

#ifndef MBEDTLS_HASH_HARDEN_H
#define MBEDTLS_HASH_HARDEN_H

#include "hash_harden_common.h"

typedef struct {
    void *ctx;
    unsigned int hash_handle;
    crypto_buf_attr src_buf;
    crypto_hash_attr hash_attr;
    int is_smaller;
    unsigned char is_used;
} hash_harden_context;

int hash_harden_can_do( crypto_hash_type hash_type );

int check_ctx_go_harden( const void *ctx );

int hash_init_harden( void );

void hash_free_harden( void *ctx );

int hash_starts_harden( void *ctx, crypto_hash_type hash_type, int is_smaller );

int hash_update_harden( void *ctx, const unsigned char *input, size_t ilen );

int hash_finish_harden( void *ctx, unsigned char *output );

void hash_clone_harden( void *dst, const void *src, crypto_hash_type bigger, crypto_hash_type smaller);

void hmac_free_harden( void *ctx );

int hmac_harden_can_do( const mbedtls_md_info_t *md_info, int hmac );

int hmac_setup_harden( mbedtls_md_context_t *ctx, const mbedtls_md_info_t *md_info );

int hmac_starts_harden( void *ctx, const unsigned char *key, size_t keylen);

int hmac_update_harden( void *ctx, const unsigned char *input, size_t ilen );

int hmac_finish_harden( mbedtls_md_context_t *ctx, unsigned char *output );

int hmac_reset_harden( mbedtls_md_context_t *ctx );

#endif /* MBEDTLS_HASH_HARDEN_H */