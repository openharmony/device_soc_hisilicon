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
#include <securecutil.h>
#include <cipher.h>
#include "rsa_pad_pss.h"

/* Constants for reported magic-number values. */
#define RSA_PAD_PSS_VALUE_2                    2

#define CIPHER_MAX_RSA_KEY_LEN      512
#define HASH_RESULT_MAX_LEN         32
#define PADDING1_SIZE 8
#define MFG1_CNT_LEN 4 /* Counter length of MFG1 function */

/* convert number of bits to the number of bytes */
#define n_bits_to_n_bytes(bit_num) (((bit_num) + 7) / 8)

typedef struct {
    uint8_t *masked_db;
    uint8_t *masked_seed;
    uint8_t salt[HASH_RESULT_MAX_LEN];
    uint32_t msb_bits;
    uint32_t slen;
    uint32_t key_len;
} rsa_pkcs1_pss_s;

uint32_t rsa_get_bit_num(const uint8_t *big_num, uint32_t num_len)
{
    static const int8_t bits[16] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4}; /* 16 bits size */
    uint32_t i;

    if (big_num == NULL) {
        return 0;
    }

    for (i = 0; i < num_len; i++) {
        uint32_t num;
        num = bits[(big_num[i] & 0xF0) >> 4]; /* 4 right shift */

        if (num > 0)
            return (num_len - i - 1) * 8 + num + 4; /* 8, 4 */

        num = bits[big_num[i] & 0xF];
        if (num > 0)
            return (num_len - i - 1) * 8 + num; /* 8 */
    }
    return 0;
}

static int32_t rsa_padding_check_pkcs1_pss_hash(const rsa_padding_s *pad,
    const uint8_t *mhash, const rsa_pkcs1_pss_s *pss)
{
    uint32_t mlen;
    uint8_t *ptr_m = NULL;
    uint8_t arr_h[HASH_RESULT_MAX_LEN] = {0};
    int ret;
    uint32_t offset;

    mlen = pss->slen + pad->hlen + PADDING1_SIZE;
    ptr_m = (uint8_t *)(uintptr_t)gsl_malloc(mlen);
    if (ptr_m == NULL)
        return TD_FAILURE;

    /* M' = padding1(zero padding) || mHash || salt */
    if (memset_s(ptr_m, mlen, 0x00, PADDING1_SIZE) != EOK) {
        gsl_free(ptr_m);
        return TD_FAILURE;
    }

    offset = PADDING1_SIZE;
    if (memcpy_s(ptr_m + offset, mlen - offset, mhash, pad->hlen) != EOK) {
        gsl_free(ptr_m);
        return TD_FAILURE;
    }

    offset = PADDING1_SIZE + pad->hlen;
    if (memcpy_s(ptr_m + offset, mlen - offset, pss->salt, pss->slen) != EOK) {
        gsl_free(ptr_m);
        return TD_FAILURE;
    }

    ret = calc_sha((uint32_t)(uintptr_t)ptr_m, mlen, arr_h, sizeof(arr_h));
    if (ret != TD_SUCCESS) {
        gsl_free(ptr_m);
        return TD_FAILURE;
    }

    gsl_free(ptr_m); /* Must free ptr_m befort return */
    ptr_m = NULL;

    if (memcmp(arr_h, pss->masked_seed, pad->hlen) != 0)
        return TD_FAILURE;

    return TD_SUCCESS;
}

static int32_t rsa_pkcs1_mgf1(const uint8_t *seed, uint32_t seed_len, uint8_t *mask,
    uint32_t mask_len)
{
    uint32_t i;
    uint32_t out_len;
	uint8_t ptr_md[HASH_RESULT_MAX_LEN];
	uint8_t *ptr_cnt = NULL;
	uint8_t *ptr_seed = NULL;
	size_t malloc_size = seed_len + MFG1_CNT_LEN;
	int ret;

    ptr_seed = (uint8_t *)gsl_malloc(malloc_size);
	if (ptr_seed == NULL)
		return TD_FAILURE;

    ptr_cnt = ptr_seed + seed_len;

	if (memset_s(ptr_md, HASH_RESULT_MAX_LEN, 0, HASH_RESULT_MAX_LEN) != EOK) {
        gsl_free(ptr_seed);
        return TD_FAILURE;
    }

	if (memset_s(ptr_seed, malloc_size, 0, malloc_size) != EOK) {
        gsl_free(ptr_seed);
        return TD_FAILURE;
    }

	if (memcpy_s(ptr_seed, malloc_size, seed, seed_len) != EOK) {
        gsl_free(ptr_seed);
        return TD_FAILURE;
	}

    /* PKCS#1 V2.1 only use sha1 function, Others allow for future expansion */
    for (i = 0, out_len = 0; out_len < mask_len; i++) {
        uint32_t j;

        ptr_cnt[0] = (uint8_t)((i >> 24) & 0xFF); /* 0 ptr_cnt index, 24 right shift */
        ptr_cnt[1] = (uint8_t)((i >> 16) & 0xFF); /* 1 ptr_cnt index, 16 right shift */
        ptr_cnt[2] = (uint8_t)((i >>  8) & 0xFF); /* 2 ptr_cnt index, 8  right shift */
        ptr_cnt[3] = (uint8_t)(i & 0xFF);         /* 3 ptr_cnt index */

		ret = calc_sha((uint32_t)(uintptr_t)ptr_seed, malloc_size, ptr_md, HASH_RESULT_MAX_LEN);
		if (ret != TD_SUCCESS) {
            gsl_free(ptr_seed);
            return TD_FAILURE;
        }

        for (j = 0; (j < SHA256_LEN) && (out_len < mask_len); j++)
            mask[out_len++] ^= ptr_md[j];
    }

    gsl_free(ptr_seed);

    return TD_SUCCESS;
}

int32_t rsa_padding_check_pkcs1_pss(rsa_padding_s *pad, const uint8_t *mhash)
{
	int32_t ret;
    uint32_t index;
    uint32_t tmp_len;
	rsa_pkcs1_pss_s pss;

    if (pad == NULL || pad->in_data == NULL)
        return TD_FAILURE;

    if (memset_s(&pss, sizeof(rsa_pkcs1_pss_s), 0, sizeof(rsa_pkcs1_pss_s)) != EOK)
        return TD_FAILURE;

    pss.slen = pad->hlen;
    pss.key_len = n_bits_to_n_bytes(pad->em_bit);
    pss.msb_bits = (pad->em_bit - 1) & 0x07;

	if (pss.key_len < (pad->hlen + pss.slen + RSA_PAD_PSS_VALUE_2))
		return TD_FAILURE;

    if (pad->in_data[0] & (0xFF << pss.msb_bits))
        return TD_FAILURE;

    if (pss.msb_bits == 0) {
        pad->in_data++;
        pss.key_len--;
    }

    pss.masked_db = pad->in_data;
    pss.masked_seed = pad->in_data + pss.key_len - pad->hlen - 1;

    if (pad->in_data[pss.key_len - 1] != 0xBC)
        return TD_FAILURE;

    /* formula: maskedDB = DB xor dbMask, DB = PS || 0x01 || salt */
    ret = rsa_pkcs1_mgf1(pss.masked_seed,
                 pad->hlen, pss.masked_db, pss.key_len - pad->hlen - 1);
    if (ret != TD_SUCCESS)
        return TD_FAILURE;

    if (pss.msb_bits) {
		pss.masked_db[0] &= 0xFF >> (8 - pss.msb_bits); /* 8 */
	}

    tmp_len = pss.key_len - pss.slen - pad->hlen - 2; /* 2 */
    if (tmp_len >= CIPHER_MAX_RSA_KEY_LEN - 1)
        return TD_FAILURE;

    for (index = 0; index < tmp_len; index++) {
        if (pss.masked_db[index] != 0x00)
            return TD_FAILURE;
    }

    if (pss.masked_db[index] != 0x01)
        return TD_FAILURE;

    index++;

    if (memcpy_s(pss.salt, sizeof(pss.salt), &pss.masked_db[index], pss.slen) != EOK)
        return TD_FAILURE;

    return rsa_padding_check_pkcs1_pss_hash(pad, mhash, &pss);
}
