/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 */

#ifndef SAMPLE_UTILS_H
#define SAMPLE_UTILS_H

#include <stdio.h>

#include "sample_log.h"
#include "ot_type.h"
#include "ss_mpi_cipher.h"

#define array_size(x)       (sizeof(x) / sizeof((x)[0]))

/**
* The function is used to generate random data for the following scenarios:
* 1. The key used in hmac.
* 2. The clear key and iv used in symmetric algorithm like AES.
* 3. The derivatin materials, session key, content key and iv used in klad.
*/
td_s32 get_random_data(td_u8 *buffer, td_u32 size);

typedef enum {
    RSA_TYPE_2048,
    RSA_TYPE_4096
} rsa_type_enum;

#define RSA_2048_KEY_LEN    256
#define RSA_4096_KEY_LEN    512


/**
 * The function is used to generate public-private key pair for rsa.
 */
td_s32 get_rsa_key(ot_cipher_rsa_public_key *public_key, rsa_type_enum rsa_type);

#endif