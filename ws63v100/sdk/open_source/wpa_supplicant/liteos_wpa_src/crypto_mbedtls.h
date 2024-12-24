/*
 * Wrapper functions for crypto libraries
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
 *
 */

#ifndef CRYPTO_MBEDTLS_H
#define CRYPTO_MBEDTLS_H

#include "library/common.h"
#include "mbedtls/ecp.h"
#include "mbedtls/bignum.h"

struct crypto_ec {
	mbedtls_ecp_group ecp;
	mbedtls_mpi a;
};

#define crypto_bignum mbedtls_mpi
#define crypto_ec_point mbedtls_ecp_point
#define AES_128_ALT_BLOCK_SIZE 16
#define AES_256_ALT_BLOCK_SIZE 32
#define AES_128_CRYPTO_LEN 128
#define DHM_PARM_MEM 2048
#define DHM_PARM_G_LEN 2

#endif
