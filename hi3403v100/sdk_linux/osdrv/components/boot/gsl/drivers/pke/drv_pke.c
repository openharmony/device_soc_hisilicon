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
#include <types.h>
#include <common.h>
#include <lib.h>
#include <sys_reg.h>
#include <platform.h>
#include <securecutil.h>
#include <cipher.h>
#include "rsa_pad_pss.h"
#include "drv_pke.h"
#include "ot_drv_pke.h"

/************************* Internal Structure Definition *********************/
#define BYTE_BITS 8

int32_t drv_rsa_verify_hash(const uint8_t *n, const uint8_t *e,
    const uint8_t *hash, const unsigned char *sign)
{
    uint8_t em[RSA_KEY_LEN_4096];
    uint8_t sign_hash[HASH_LEN_256];
    uint32_t klen = RSA_KEY_LEN_4096;
    rsa_padding_s pad;
    uint32_t stepcount = 0;
    int ret;

    if (sign == NULL)
        return TD_FAILURE;

    if (memset_s(em, RSA_KEY_LEN_4096, 0x00, RSA_KEY_LEN_4096) != EOK)
        return TD_FAILURE;

    if (memset_s(sign_hash, HASH_LEN_256, 0x0, HASH_LEN_256) != EOK)
        return TD_FAILURE;

    em[0] = 0x55; /* 55: force failure if RSA_Calc skipped in later padding check */
    em[1] = 0x00; /* 00: force failure if RSA_Calc skipped in later padding check */
    em[2] = 0x12; /* 12: force failure if RSA_Calc skipped in later padding check */

    /* Confirm digital signature */
    if (TD_SUCCESS != drv_ifep_rsa_exp_mod(n, e, sign, em, klen))
        return TD_FAILURE;
    else
        stepcount += STEP_AUTH;  /* brief: stepcount = 1 */

    if (TD_SUCCESS == memcmp(sign, em, RSA_KEY_LEN_4096))
        return TD_FAILURE;
    else
        stepcount += STEP_AUTH;  /* brief: stepcount = 2 */

    if (memset_s(&pad, sizeof(rsa_padding_s), 0, sizeof(rsa_padding_s)) != EOK)
        return TD_FAILURE;

    pad.em_bit = rsa_get_bit_num(n, klen);
    pad.klen = klen;
    pad.hlen = HASH_LEN_256;
    pad.in_data = em;
    pad.in_len = RSA_KEY_LEN_4096;

    ret = rsa_padding_check_pkcs1_pss(&pad, hash);
    if (ret != 0)
        return TD_FAILURE;

    return TD_SUCCESS;
}
