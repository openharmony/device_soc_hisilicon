/*
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
 * limitations under the License..
 * Description: the head File of sha256
 */

#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include "std_def.h"

#define SHA256_HASH_SIZE 32

/* Hash size in 32-bit words */
#define SHA256_HASH_WORDS 8

#define SHA256_BUFFER_WORDS 16
#define SHA256_BUFFER_BYTES 64

struct _sha256_context {
    uint64_t total_length;
    uint32_t hash[SHA256_HASH_WORDS];
    uint32_t buffer_length;

    union {
        uint32_t words[SHA256_BUFFER_WORDS];
        uint8_t bytes[SHA256_BUFFER_BYTES];
    } buffer;
#ifdef RUNTIME_ENDIAN
    int little_endian;
#endif /* RUNTIME_ENDIAN */
};

typedef struct _sha256_context sha256_context_t;

void sha256_init(sha256_context_t *sc);
void SHA256Update(sha256_context_t *sc, const void *vdata, uint32_t len);

void sha256_final(sha256_context_t *sc, uint8_t hash[SHA256_HASH_SIZE], uint32_t hash_len);
void sha256_hash(const uint8_t *in_buff, uint32_t in_buff_len, uint8_t *out_buff, uint32_t out_buff_len);

#endif
