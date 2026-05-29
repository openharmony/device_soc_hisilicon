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
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <assert.h>

#include "ot_type.h"
#include "ss_mpi_cipher.h"
#include "ot_common_cipher.h"
#include "sample_utils.h"

#define MAX_HASH_HANDLE 4
#define MAX_HMAC_HANDLE 3
#define DATA_SIZE 32
#define SHA256_HASH_LEN 32
#define SHA384_HASH_LEN 48
#define SHA512_HASH_LEN 64
#define MAX_HASH_LEN    64

#define HMAC_KEY_LEN    16

td_s32 sample_sha(ot_cipher_hash_type hash_type)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 hash[MAX_HASH_LEN] = {0};
    td_u32 i;
    td_handle handle[MAX_HASH_HANDLE] = {0};
    ot_cipher_hash_attr hash_attr = {0};
    td_u8 data_buffer[DATA_SIZE] = {0};
    td_u32 hash_len = 0;
    switch (hash_type) {
        case OT_CIPHER_HASH_TYPE_SHA256:
            hash_len = SHA256_HASH_LEN;
            break;
        case OT_CIPHER_HASH_TYPE_SHA384:
            hash_len = SHA384_HASH_LEN;
            break;
        case OT_CIPHER_HASH_TYPE_SHA512:
            hash_len = SHA512_HASH_LEN;
            break;
        default:
            sample_err("invalid hash_type, only support sha256, sha384, sha512\n");
            return TD_FAILURE;
    }

    /* cipher init */
    sample_chk_expr_return(ss_mpi_cipher_init(), TD_SUCCESS);

    for (i = 0; i < MAX_HASH_HANDLE; i++) {
        hash_attr.sha_type = hash_type;

        /* hash init */
        sample_chk_expr_goto(ss_mpi_cipher_hash_init(&hash_attr, &handle[i]), TD_SUCCESS, __CIPHER_DEINIT__);
        hash_attr.sha_type = hash_type;
    }
    for (i = 0; i < MAX_HASH_HANDLE; i++) {
        /* generate data */
        sample_chk_expr_goto(get_random_data(data_buffer, sizeof(data_buffer)), TD_SUCCESS, __CIPHER_DEINIT__);

        /* hash update */
        sample_chk_expr_goto(ss_mpi_cipher_hash_update(handle[i], data_buffer, sizeof(data_buffer)),
            TD_SUCCESS, __CIPHER_DEINIT__);
    }

    for (i = 0; i < MAX_HASH_HANDLE; i++) {
        /* get hash */
        sample_chk_expr_goto(ss_mpi_cipher_hash_final(handle[i], hash, hash_len), TD_SUCCESS, __CIPHER_DEINIT__);
    }

__CIPHER_DEINIT__:
    (td_void)ss_mpi_cipher_deinit();
    return ret;
}

td_s32 sample_hmac(ot_cipher_hash_type hash_type)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 hash[MAX_HASH_LEN];
    td_u32 i;
    td_handle handle[MAX_HMAC_HANDLE];
    ot_cipher_hash_attr hash_attr = {0};
    td_u8 hmac_key[HMAC_KEY_LEN] = {0};
    td_u8 data_buffer[DATA_SIZE];

    td_u32 hash_len = 0;
    switch (hash_type) {
        case OT_CIPHER_HASH_TYPE_HMAC_SHA256:
            hash_len = SHA256_HASH_LEN;
            break;
        case OT_CIPHER_HASH_TYPE_HMAC_SHA384:
            hash_len = SHA384_HASH_LEN;
            break;
        case OT_CIPHER_HASH_TYPE_HMAC_SHA512:
            hash_len = SHA512_HASH_LEN;
            break;
        default:
            sample_err("invalid hash_type, only support sha256-hmac, sha384-hmac, sha512-hmac\n");
            return TD_FAILURE;
    }
    /* cipher init */
    sample_chk_expr_return(ss_mpi_cipher_init(), TD_SUCCESS);

    hash_attr.hmac_key = hmac_key;
    hash_attr.hmac_key_len = HMAC_KEY_LEN;
    for (i = 0; i < MAX_HMAC_HANDLE; i++) {
        /* generate hmac key */
        hash_attr.sha_type = hash_type;
        sample_chk_expr_goto(get_random_data(hash_attr.hmac_key, hash_attr.hmac_key_len),
            TD_SUCCESS, __CIPHER_DEINIT__);
        /* hash init */
        sample_chk_expr_goto(ss_mpi_cipher_hash_init(&hash_attr, &handle[i]), TD_SUCCESS, __CIPHER_DEINIT__);
    }
    for (i = 0; i < MAX_HMAC_HANDLE; i++) {
        /* generate data */
        sample_chk_expr_goto(get_random_data(data_buffer, sizeof(data_buffer)), TD_SUCCESS, __CIPHER_DEINIT__);

        /* hash update */
        sample_chk_expr_goto(ss_mpi_cipher_hash_update(handle[i], data_buffer, sizeof(data_buffer)),
            TD_SUCCESS, __CIPHER_DEINIT__);
    }
    for (i = 0; i < MAX_HMAC_HANDLE; i++) {
        /* get hash */
        sample_chk_expr_goto(ss_mpi_cipher_hash_final(handle[i], hash, hash_len), TD_SUCCESS, __CIPHER_DEINIT__);
    }

__CIPHER_DEINIT__:
    (td_void)ss_mpi_cipher_deinit();
    return ret;
}

td_s32 sample_hash(td_void)
{
    sample_log("************ test hash ************\n");
    /* 1. sha256 */
    sample_chk_expr_return(sample_sha(OT_CIPHER_HASH_TYPE_SHA256), TD_SUCCESS);
    sample_log("************ test sha256 success ************\n");
    /* 2. sha384 */
    sample_chk_expr_return(sample_sha(OT_CIPHER_HASH_TYPE_SHA384), TD_SUCCESS);
    sample_log("************ test sha384 success ************\n");
    /* 3. sha512 */
    sample_chk_expr_return(sample_sha(OT_CIPHER_HASH_TYPE_SHA512), TD_SUCCESS);
    sample_log("************ test sha512 success ************\n");
    /* 4. hmac-sha256 */
    sample_chk_expr_return(sample_hmac(OT_CIPHER_HASH_TYPE_HMAC_SHA256), TD_SUCCESS);
    sample_log("************ test hmac sha256 success ************\n");
    /* 5. hmac-sha384 */
    sample_chk_expr_return(sample_hmac(OT_CIPHER_HASH_TYPE_HMAC_SHA384), TD_SUCCESS);
    sample_log("************ test hmac sha384 success  ************\n");
    /* 6. hmac-sha512 */
    sample_chk_expr_return(sample_hmac(OT_CIPHER_HASH_TYPE_HMAC_SHA512), TD_SUCCESS);
    sample_log("************ test hmac sha512 success ************\n");
    sample_log("************ test hash success ************\n");
    return TD_SUCCESS;
}
