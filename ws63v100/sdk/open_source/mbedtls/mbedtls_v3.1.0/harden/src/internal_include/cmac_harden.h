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

#ifndef MBEDTLS_CMAC_HARDEN_H
#define MBEDTLS_CMAC_HARDEN_H

#include "cipher_adapt.h"
#include "mbedtls/cmac.h"
#include "string.h"

#ifdef MBEDTLS_CMAC_ALT

typedef struct cmac_harden_context{
    unsigned int symc_handle;
    unsigned int keyslot_handle;
    mbedtls_cipher_context_t *ctx;
    unsigned char key[32];
    unsigned int key_length;
    unsigned char is_update;
    unsigned char is_finish;
    unsigned char use_software;
    unsigned char is_used;
} cmac_harden_context;

int mbedtls_cipher_cmac_starts_harden( mbedtls_cipher_context_t *ctx,
                                const unsigned char *key, size_t keybits,
                                mbedtls_cipher_type_t type );

int mbedtls_cipher_cmac_update_harden( mbedtls_cipher_context_t *ctx,
                                        const unsigned char *input, size_t ilen );

int mbedtls_cipher_cmac_finish_harden( mbedtls_cipher_context_t *ctx,
                                        unsigned char *output );

int mbedtls_cipher_cmac_reset_harden( mbedtls_cipher_context_t *ctx );

#endif

#endif