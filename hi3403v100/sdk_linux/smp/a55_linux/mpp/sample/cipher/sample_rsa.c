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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "ot_type.h"
#include "ss_mpi_cipher.h"

#include "sample_utils.h"

#define MAX_DATA_LEN        512
#define TEST_DATA_LEN       128
#define SHA256_SIGN_LEN     32
td_s32 rsa_public_encrypt(ot_cipher_rsa_scheme scheme, ot_cipher_hash_type sha_type)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 encrypt[MAX_DATA_LEN] = {0};
    td_u8 test_data[TEST_DATA_LEN] = {0};
    td_u8 rsa_n[MAX_DATA_LEN] = {0};
    td_u8 rsa_e[MAX_DATA_LEN] = {0};
    ot_cipher_rsa_public_key public_key = {.n = rsa_n, .e = rsa_e};
    ot_cipher_common_data plain_txt = {0};
    ot_cipher_common_data cipher_txt = {0};

    sample_chk_expr_return(get_rsa_key(&public_key, RSA_TYPE_2048), TD_SUCCESS);

    sample_chk_expr_return(get_random_data(test_data, sizeof(test_data)), TD_SUCCESS);

    /* cipher init */
    sample_chk_expr_return(ss_mpi_cipher_init(), TD_SUCCESS);

    plain_txt.data = test_data;
    plain_txt.data_len = sizeof(test_data);

    cipher_txt.data = encrypt;
    cipher_txt.data_len = sizeof(encrypt);

    sample_chk_expr_goto(
        ss_mpi_cipher_rsa_public_encrypt(scheme, sha_type, &public_key, &plain_txt, &cipher_txt),
        TD_SUCCESS, exit__);

exit__:
    ss_mpi_cipher_deinit();
    return ret;
}

td_s32 rsa_public_decrypt(ot_cipher_rsa_scheme scheme, ot_cipher_hash_type sha_type)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 encrypt[MAX_DATA_LEN] = {0};
    td_u8 decrypt[MAX_DATA_LEN] = {0};
    td_u8 rsa_n[MAX_DATA_LEN] = {0};
    td_u8 rsa_e[MAX_DATA_LEN] = {0};
    ot_cipher_rsa_public_key public_key = {.n = rsa_n, .e = rsa_e};
    ot_cipher_common_data plain_txt;
    ot_cipher_common_data cipher_txt;

    sample_chk_expr_return(get_rsa_key(&public_key, RSA_TYPE_2048), TD_SUCCESS);

    sample_chk_expr_return(get_random_data(encrypt, sizeof(encrypt)), TD_SUCCESS);

    /* cipher init */
    sample_chk_expr_return(ss_mpi_cipher_init(), TD_SUCCESS);

    cipher_txt.data = encrypt;
    cipher_txt.data_len = sizeof(encrypt);

    plain_txt.data = decrypt;
    plain_txt.data_len = sizeof(decrypt);

    sample_chk_expr_goto(
        ss_mpi_cipher_rsa_public_decrypt(scheme, sha_type, &public_key, &cipher_txt, &plain_txt),
        TD_SUCCESS, exit__);

exit__:
    ss_mpi_cipher_deinit();
    return ret;
}

td_s32 rsa_sign_verify(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, ot_cipher_sign_type sign_type)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 sign[MAX_DATA_LEN] = {0};
    td_u8 test_data[TEST_DATA_LEN] = {0};
    td_u8 sha256_sum[SHA256_SIGN_LEN] = {0};
    td_u8 rsa_n[MAX_DATA_LEN] = {0};
    td_u8 rsa_e[MAX_DATA_LEN] = {0};
    ot_cipher_rsa_public_key public_key = {.n = rsa_n, .e = rsa_e};
    ot_cipher_sign_in_data rsa_data = {0};
    ot_cipher_common_data sign_data = {0};

    sample_chk_expr_return(get_rsa_key(&public_key, RSA_TYPE_2048), TD_SUCCESS);

    sample_chk_expr_return(ss_mpi_cipher_init(), TD_SUCCESS);

    rsa_data.sign_type = sign_type;
    if (sign_type == OT_CIPHER_SIGN_TYPE_MSG) {
        sample_chk_expr_return(get_random_data(test_data, sizeof(test_data)), TD_SUCCESS);
        rsa_data.input = test_data;
        rsa_data.input_len = sizeof(test_data);
    } else {
        sample_chk_expr_return(get_random_data(sha256_sum, sizeof(sha256_sum)), TD_SUCCESS);
        rsa_data.input = sha256_sum;
        rsa_data.input_len = sizeof(sha256_sum);
    }

    sample_chk_expr_return(get_random_data(sign, sizeof(sign)), TD_SUCCESS);
    sign_data.data = sign;
    sign_data.data_len = sizeof(sign);

    /* verify will fail, cause test_data/sha256_sum/sign are all generated in random. */
    sample_chk_expr_goto(ss_mpi_cipher_rsa_verify(scheme, sha_type, &public_key, &rsa_data, &sign_data),
        TD_SUCCESS, exit__);

exit__:
    ss_mpi_cipher_deinit();
    return ret;
}


td_s32 sample_rsa_crypto(td_void)
{
    sample_log("************ test rsa_crypto ************\n");
    td_s32 ret;

    ret = rsa_public_encrypt(OT_CIPHER_RSA_SCHEME_PKCS1_V15, OT_CIPHER_HASH_TYPE_SHA256);
    if (ret != TD_SUCCESS) {
        sample_err("rsa_public_encrypt failed\n");
        return TD_FAILURE;
    }

    ret = rsa_public_encrypt(OT_CIPHER_RSA_SCHEME_PKCS1_V21, OT_CIPHER_HASH_TYPE_SHA256);
    if (ret != TD_SUCCESS) {
        sample_err("rsa_public_encrypt failed\n");
        return TD_FAILURE;
    }

    ret = rsa_public_decrypt(OT_CIPHER_RSA_SCHEME_PKCS1_V15, OT_CIPHER_HASH_TYPE_SHA256);
    if (ret != TD_SUCCESS) {
        sample_err("rsa_public_decrypt failed\n");
        return TD_FAILURE;
    }

    ret = rsa_public_decrypt(OT_CIPHER_RSA_SCHEME_PKCS1_V21, OT_CIPHER_HASH_TYPE_SHA256);
    if (ret != TD_SUCCESS) {
        sample_err("rsa_public_decrypt failed\n");
        return TD_FAILURE;
    }
    sample_log("************ test rsa_crypto success ************\n");
    return TD_SUCCESS;
}

td_s32 sample_rsa_sign_verify(td_void)
{
    sample_log("************ test rsa_sign_verify ************\n");
    td_s32 ret;

    ret = rsa_sign_verify(OT_CIPHER_RSA_SCHEME_PKCS1_V15,
        OT_CIPHER_HASH_TYPE_SHA256, OT_CIPHER_SIGN_TYPE_MSG);
    if (ret != TD_SUCCESS) {
        sample_err("rsa_sign_verify failed\n");
        return TD_FAILURE;
    }

    ret = rsa_sign_verify(OT_CIPHER_RSA_SCHEME_PKCS1_V15,
        OT_CIPHER_HASH_TYPE_SHA256, OT_CIPHER_SIGN_TYPE_HASH);
    if (ret != TD_SUCCESS) {
        sample_err("rsa_sign_verify failed\n");
        return TD_FAILURE;
    }

    ret = rsa_sign_verify(OT_CIPHER_RSA_SCHEME_PKCS1_V21,
        OT_CIPHER_HASH_TYPE_SHA256, OT_CIPHER_SIGN_TYPE_HASH);
    if (ret != TD_SUCCESS) {
        sample_err("rsa_sign_verify failed\n");
        return TD_FAILURE;
    }

    ret = rsa_sign_verify(OT_CIPHER_RSA_SCHEME_PKCS1_V21,
        OT_CIPHER_HASH_TYPE_SHA256, OT_CIPHER_SIGN_TYPE_MSG);
    if (ret != TD_SUCCESS) {
        sample_err("rsa_sign_verify failed\n");
        return TD_FAILURE;
    }
    sample_log("************ test rsa_crypto success ************\n");
    return TD_SUCCESS;
}
