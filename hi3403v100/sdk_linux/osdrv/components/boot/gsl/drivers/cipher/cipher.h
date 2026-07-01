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

#ifndef __CIPHER__
#define __CIPHER__

#define CIPHER_ERR_NULL_POINT      TD_FAILURE
#define CIPHER_ERR_INVALID_PARAM   TD_FAILURE
#define CIPHER_ERR_BUSY            TD_FAILURE
#define CIPHER_ERR_TIMEOUT         TD_FAILURE
#define CIPHER_ERR_MALLOC          TD_FAILURE
#define CIPHER_ERR_INIT            TD_FAILURE
#define CIPHER_ERR_ATTACK          TD_FAILURE
#define CIPHER_ERR_CHECK           TD_FAILURE

#define SHA256_LEN                    0x20
#define HASH_BLOCK_SIZE               64
#define SHA3_BLOCK_SIZE               136

#define DMA_DECRYPT_CHANNEL           2
#define DMA_GENERAL_CHANNEL           1
#define HASH_HARD_CHN                 1

typedef enum {
    SYMC_ALG_AES = 0x02,
    SYMC_ALG_DMA = 0x03,
    SYMC_ALG_SM4 = 0x05,
} symc_alg;

typedef enum {
    SYMC_MODE_ECB = 0x01,
    SYMC_MODE_CBC = 0x03,
} symc_mode;

typedef enum {
    MODE_HASH = 0x00,
    MODE_HMAC = 0x01,
} hash_mode;

typedef enum {
    HASH_ALG_SHA256  = 0x01,
    HASH_ALG_SM3     = 0x02,
} hash_alg;

typedef struct {
    uint32_t chn;
    uint32_t dst_addr;
    uint32_t src_addr;
    uint32_t length;
    uint32_t iv_addr;
    uint32_t iv_length;
    symc_alg alg;
    symc_mode mode;
} spacc_decrypt_params;

void cipher_reset(void);
int spacc_init(uint32_t key_addr);
int spacc_deinit(void);

int drv_spacc_decrypt(spacc_decrypt_params decrypt_params);

int calc_sha(uint32_t src_addr, uint32_t src_len, uint8_t *data_sha, uint32_t data_sha_len);

int secure_authenticate(const uint8_t * const key, const uint32_t data, const uint32_t len,
    const uint8_t * const signature);
#endif
