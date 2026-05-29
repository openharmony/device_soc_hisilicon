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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ot_common_cipher.h"

#include "sample_utils.h"

enum {
    TEST_CIPHER_CLEARKEY = 1,
    TEST_CIPHER_ROOTKEY,
    TEST_DMA,
    TEST_HASH,
    TEST_RNG,
    TEST_RSA_CRYPTO,
    TEST_RSA_SIGN
};

td_s32 sample_cipher_clearkey(td_void);
td_s32 sample_cipher_rootkey(td_void);
td_s32 sample_dma(td_void);
td_s32 sample_hash(td_void);
td_s32 sample_rng(td_void);
td_s32 sample_rsa_crypto(td_void);
td_s32 sample_rsa_sign_verify(td_void);

static void sample_cipher_usage(void)
{
    printf("\n##########----Run a cipher sample:for example, ./sample 1----###############\n");
    printf("    You can selset a cipher sample to run as fllow:\n"
           "    [1] CIPHER AES ClearKey\n"
           "    [2] CIPHER AES RootKey\n"
           "    [3] CIPHER DMA\n"
           "    [4] HASH\n"
           "    [5] RNG\n"
           "    [6] RSA-ENC\n"
           "    [7] RSA-SIGN\n");
}

int main(int argc, char *argv[])
{
    td_s32 ret;
    td_u32 func_num;

    if (argc != 2) { /* 2: 2 arg num */
        sample_cipher_usage();
        return TD_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) {    /* 2: 2 chars */
        sample_cipher_usage();
        return TD_FAILURE;
    }

    func_num = strtol(argv[1], TD_NULL, 0);
    switch (func_num) {
        case TEST_CIPHER_CLEARKEY:
            ret = sample_cipher_clearkey();
            break;
        case TEST_CIPHER_ROOTKEY:
            ret = sample_cipher_rootkey();
            break;
        case TEST_DMA:
            ret = sample_dma();
            break;
        case TEST_HASH:
            ret = sample_hash();
            break;
        case TEST_RNG:
            ret = sample_rng();
            break;
        case TEST_RSA_CRYPTO:
            ret = sample_rsa_crypto();
            break;
        case TEST_RSA_SIGN:
            ret = sample_rsa_sign_verify();
            break;
        default:
            sample_log("Invalid function number: %u\n", func_num);
            sample_cipher_usage();
            return TD_FAILURE;
    }
    if (ret != TD_SUCCESS) {
        sample_log("fund_id %d failed\n", func_num);
    }

    return TD_SUCCESS;
}

