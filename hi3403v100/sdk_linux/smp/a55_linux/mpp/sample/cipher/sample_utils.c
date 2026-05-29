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
#include "sample_utils.h"
#include "sample_rsa_data.h"

#define BYTES_IN_WORD   4

td_s32 get_random_data(td_u8 *buffer, td_u32 size)
{
    if (size % BYTES_IN_WORD != 0) {
        sample_err("invalid size!\n");
        return TD_FAILURE;
    }
    td_s32 ret;
    ret = ss_mpi_cipher_init();
    if (ret != TD_SUCCESS) {
        sample_err("ss_mpi_cipher_init error!\n");
        return ret;
    }
    td_u32 random_num = 0;
    td_u32 i;
    td_u32 j;
    td_u32 times = size / BYTES_IN_WORD;
    for (i = 0; i < times; i++) {
        ret = ss_mpi_cipher_get_random_num(&random_num);
        if (ret != TD_SUCCESS) {
            sample_err("ss_mpi_cipher_get_random_num error!\n");
            return ret;
        }
        for (j = 0; j < BYTES_IN_WORD; j++) {
            buffer[i * BYTES_IN_WORD + j] = ((td_u8 *)(&random_num))[j];
        }
    }
    (td_void)ss_mpi_cipher_deinit();
    return ret;
}

#ifdef SAMPLE_RSA_DEBUG
/**
* Note: This is only for debug scenario!!!
* Do not use such code in production scenario!!!
*/
static td_bool _is_all_zero(const td_u8 *data, td_u32 size)
{
    td_u32 i;
    for (i = 0; i < size; i++) {
        if (data[i] != 0) {
            return TD_FALSE;
        }
    }
    return TD_TRUE;
}

static td_s32 _check_rsa_data(rsa_type_enum rsa_type)
{
    td_bool is_zero = TD_TRUE;
    if (rsa_type == RSA_TYPE_2048) {
        /* check sample_rsa_2048_n */
        is_zero = _is_all_zero(sample_rsa_2048_n, sizeof(sample_rsa_2048_n));
        if (is_zero == TD_TRUE) {
            sample_err("Error: must set the meaningful value for sample_rsa_2048_n first!!!\n");
            return TD_FAILURE;
        }
        /* check sample_rsa_2048_e */
        is_zero = _is_all_zero(sample_rsa_2048_e, sizeof(sample_rsa_2048_e));
        if (is_zero == TD_TRUE) {
            sample_err("Error: must set the meaningful value for sample_rsa_2048_e first!!!\n");
            return TD_FAILURE;
        }
        return TD_SUCCESS;
    }
    if (rsa_type == RSA_TYPE_4096) {
        /* check sample_rsa_4096_n */
        is_zero = _is_all_zero(sample_rsa_4096_n, sizeof(sample_rsa_4096_n));
        if (is_zero == TD_TRUE) {
            sample_err("Error: must set the meaningful value for sample_rsa_4096_n first!!!\n");
            return TD_FAILURE;
        }
        /* check sample_rsa_4096_e */
        is_zero = _is_all_zero(sample_rsa_4096_e, sizeof(sample_rsa_4096_e));
        if (is_zero == TD_TRUE) {
            sample_err("Error: must set the meaningful value for sample_rsa_4096_e first!!!\n");
            return TD_FAILURE;
        }
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 get_rsa_key(ot_cipher_rsa_public_key *public_key, rsa_type_enum rsa_type)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *rsa_n = TD_NULL;
    td_u8 *rsa_e = TD_NULL;
    td_u32 key_len;
    switch (rsa_type) {
        case RSA_TYPE_2048:
            rsa_n = sample_rsa_2048_n;
            rsa_e = sample_rsa_2048_e;
            key_len = RSA_2048_KEY_LEN;
            break;
        case RSA_TYPE_4096:
            rsa_n = sample_rsa_4096_n;
            rsa_e = sample_rsa_4096_e;
            key_len = RSA_4096_KEY_LEN;
            break;
    }
    /* 0. check if the data has been filled by user. */
    ret = _check_rsa_data(rsa_type);
    if (ret != TD_SUCCESS) {
        sample_err("Error: must set the meaningful (n, e) first!!!\n");
        return TD_FAILURE;
    }
    /* 1. get rsa public key */
    sample_chk_expr_with_return(memcpy_s(public_key->n, MAX_DATA_LEN, rsa_n, key_len), 0, TD_FAILURE);
    sample_chk_expr_with_return(memcpy_s(public_key->e, MAX_DATA_LEN, rsa_e, key_len), 0, TD_FAILURE);
    public_key->n_len = key_len;
    public_key->e_len = key_len;
    return ret;
}
#else
/**
* Note: This is for production scenario!!!
*/
/* Tip: You can get rsa public key by calling the interface from mbedtls or read from public key pem file. */
td_s32 get_rsa2048_pubkey(td_u8 *rsa_n, td_u8 *rsa_e, td_u32 len)
{
    sample_err("you must implement this function to get rsa2048 pubkey!!!\n");
    return TD_FAILURE;
}
td_s32 get_rsa4096_pubkey(td_u8 *rsa_n, td_u8 *rsa_e, td_u32 len)
{
    sample_err("you must implement this function to get rsa4096 pubkey!!!\n");
    return TD_FAILURE;
}

td_s32 get_rsa_key(ot_cipher_rsa_public_key *public_key, rsa_type_enum rsa_type)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 key_len = RSA_2048_KEY_LEN;
    switch (rsa_type) {
        case RSA_TYPE_2048:
            key_len = RSA_2048_KEY_LEN;
            sample_chk_expr_return(get_rsa2048_pubkey(public_key->n, public_key->e, key_len), TD_SUCCESS);
            break;
        case RSA_TYPE_4096:
            key_len = RSA_4096_KEY_LEN;
            sample_chk_expr_return(get_rsa4096_pubkey(public_key->n, public_key->e, key_len), TD_SUCCESS);
            break;
    }
    public_key->n_len = key_len;
    public_key->e_len = key_len;
    return ret;
}
#endif
