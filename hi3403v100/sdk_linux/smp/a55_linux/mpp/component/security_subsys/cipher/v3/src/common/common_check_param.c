/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "common_check_param.h"
#include "drv_cipher_debug.h"

/* Constants for reported magic-number values. */
#define CIPHER_CHECK_VALUE_11                  11

td_s32 mpi_rsa_chk_private_key(const ot_cipher_rsa_private_key *rsa_key)
{
    chk_ptr_err_return(rsa_key);
    chk_ptr_err_return(rsa_key->n);
    chk_param_err_return(rsa_key->n_len < RSA_MIN_KEY_LEN);
    chk_param_err_return(rsa_key->n_len > RSA_MAX_KEY_LEN);

    if (rsa_key->d == TD_NULL) {
        chk_ptr_err_return(rsa_key->p);
        chk_ptr_err_return(rsa_key->q);
        chk_ptr_err_return(rsa_key->dp);
        chk_ptr_err_return(rsa_key->dq);
        chk_ptr_err_return(rsa_key->qp);
        chk_param_err_return((rsa_key->n_len >> 1) != rsa_key->p_len);
        chk_param_err_return((rsa_key->n_len >> 1) != rsa_key->q_len);
        chk_param_err_return((rsa_key->n_len >> 1) != rsa_key->dp_len);
        chk_param_err_return((rsa_key->n_len >> 1) != rsa_key->dq_len);
        chk_param_err_return((rsa_key->n_len >> 1) != rsa_key->qp_len);
    } else {
        chk_param_err_return(rsa_key->n_len != rsa_key->d_len);
    }

    return TD_SUCCESS;
}

td_s32 mpi_rsa_get_crypto_attr(ot_cipher_rsa_encrypt_scheme *rsa_scheme, ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type hash_type)
{
    if (scheme == OT_CIPHER_RSA_SCHEME_PKCS1_V15) {
        *rsa_scheme = OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_PKCS1_V1_5;
    } else if (scheme == OT_CIPHER_RSA_SCHEME_PKCS1_V21) {
        switch (hash_type) {
            case OT_CIPHER_HASH_TYPE_SHA256:
                *rsa_scheme = OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA256;
                break;
            case OT_CIPHER_HASH_TYPE_SHA384:
                *rsa_scheme = OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA384;
                break;
            case OT_CIPHER_HASH_TYPE_SHA512:
                *rsa_scheme = OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA512;
                break;
            default:
                ot_cipher_error("Invalid sha type %d! should be in [%d, %d]!\n", hash_type,
                    OT_CIPHER_HASH_TYPE_SHA256, OT_CIPHER_HASH_TYPE_SHA512);
                return OT_ERR_CIPHER_INVALID_PARAM;
        }
    } else {
        ot_cipher_error("Invalid scheme %d! should be in [%d, %d]!\n", scheme,
            OT_CIPHER_RSA_SCHEME_PKCS1_V15, OT_CIPHER_RSA_SCHEME_PKCS1_V21);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 mpi_rsa_get_sign_attr(ot_cipher_rsa_sign_scheme *rsa_scheme,
    td_u32 *hash_len, ot_cipher_rsa_scheme scheme, ot_cipher_hash_type hash_sha_type)
{
    if (scheme == OT_CIPHER_RSA_SCHEME_PKCS1_V15) {
        switch (hash_sha_type) {
            case OT_CIPHER_HASH_TYPE_SHA256:
                *rsa_scheme = OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256;
                *hash_len = SHA256_RESULT_SIZE;
                break;
            case OT_CIPHER_HASH_TYPE_SHA384:
                *rsa_scheme = OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384;
                *hash_len = SHA384_RESULT_SIZE;
                break;
            case OT_CIPHER_HASH_TYPE_SHA512:
                *rsa_scheme = OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512;
                *hash_len = SHA512_RESULT_SIZE;
                break;
            default:
                ot_cipher_error("Invalid sha type %d! should be in [%d, %d]!\n", hash_sha_type,
                    OT_CIPHER_HASH_TYPE_SHA256, OT_CIPHER_HASH_TYPE_SHA512);
                return OT_ERR_CIPHER_INVALID_PARAM;
        }
    } else if (scheme == OT_CIPHER_RSA_SCHEME_PKCS1_V21) {
        switch (hash_sha_type) {
            case OT_CIPHER_HASH_TYPE_SHA256:
                *rsa_scheme = OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256;
                *hash_len = SHA256_RESULT_SIZE;
                break;
            case OT_CIPHER_HASH_TYPE_SHA384:
                *rsa_scheme = OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA384;
                *hash_len = SHA384_RESULT_SIZE;
                break;
            case OT_CIPHER_HASH_TYPE_SHA512:
                *rsa_scheme = OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA512;
                *hash_len = SHA512_RESULT_SIZE;
                break;
            default:
                ot_cipher_error("Invalid sha type %d! should be in [%d, %d]!\n", hash_sha_type,
                    OT_CIPHER_HASH_TYPE_SHA256, OT_CIPHER_HASH_TYPE_SHA512);
                return OT_ERR_CIPHER_INVALID_PARAM;
        }
    } else {
        ot_cipher_error("Invalid scheme %d! should be in [%d, %d]!\n", scheme,
            OT_CIPHER_RSA_SCHEME_PKCS1_V15, OT_CIPHER_RSA_SCHEME_PKCS1_V21);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 mpi_rsa_chk_verify_param(const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_sign_in_data *rsa_data, const ot_cipher_common_data *sign_data)
{
    /* check public key */
    chk_ptr_err_return(rsa_key);
    chk_ptr_err_return(rsa_key->n);
    chk_ptr_err_return(rsa_key->e);
    chk_param_err_return((rsa_key->n_len < RSA_MIN_KEY_LEN) || (rsa_key->n_len > RSA_MAX_KEY_LEN));
    chk_param_err_return(rsa_key->n_len < rsa_key->e_len);

    /* check verify data */
    chk_ptr_err_return(rsa_data);
    chk_ptr_err_return(rsa_data->input);
    chk_param_err_return(rsa_data->sign_type >= OT_CIPHER_SIGN_TYPE_BUTT);
    chk_param_err_return(rsa_data->input_len == 0);

    chk_ptr_err_return(sign_data);
    chk_ptr_err_return(sign_data->data);
    chk_param_err_return((sign_data->data_len == 0) || (sign_data->data_len != rsa_key->n_len));

    return TD_SUCCESS;
}

td_s32 mpi_rsa_chk_sign_param(const ot_cipher_rsa_private_key *rsa_key,
    const ot_cipher_sign_in_data *rsa_data, const ot_cipher_common_data *sign_data)
{
    td_s32 ret;
    ret = mpi_rsa_chk_private_key(rsa_key);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_chk_private_key, ret);
        return ret;
    }

    /* check sign data */
    chk_ptr_err_return(rsa_data);
    chk_ptr_err_return(rsa_data->input);
    chk_param_err_return(rsa_data->sign_type >= OT_CIPHER_SIGN_TYPE_BUTT);
    chk_param_err_return(rsa_data->input_len == 0);

    chk_ptr_err_return(sign_data);
    chk_ptr_err_return(sign_data->data);
    chk_param_err_return((sign_data->data_len == 0) || (sign_data->data_len < rsa_key->n_len));

    return TD_SUCCESS;
}

static td_s32 cipher_rsa_public_check(const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *plain_txt, const ot_cipher_common_data *cipher_txt)
{
    chk_ptr_err_return(rsa_key);
    chk_ptr_err_return(rsa_key->n);
    chk_ptr_err_return(rsa_key->e);
    chk_param_err_return((rsa_key->n_len < RSA_MIN_KEY_LEN) || (rsa_key->n_len > RSA_MAX_KEY_LEN));
    chk_param_err_return(rsa_key->n_len < rsa_key->e_len);

    chk_ptr_err_return(plain_txt);
    chk_ptr_err_return(plain_txt->data);
    chk_param_err_return(plain_txt->data_len == 0);

    chk_ptr_err_return(cipher_txt);
    chk_ptr_err_return(cipher_txt->data);
    chk_param_err_return(cipher_txt->data_len == 0);
    return TD_SUCCESS;
}

td_s32 cipher_rsa_public_encrypt_check(const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt)
{
    td_s32 ret;
    ret = cipher_rsa_public_check(rsa_key, plain_txt, cipher_txt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    chk_param_err_return(cipher_txt->data_len < rsa_key->n_len);
    return TD_SUCCESS;
}

td_s32 cipher_rsa_public_decrypt_check(const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt)
{
    td_s32 ret;
    ret = cipher_rsa_public_check(rsa_key, plain_txt, cipher_txt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    chk_param_err_return(cipher_txt->data_len != rsa_key->n_len);
    return TD_SUCCESS;
}

/*
* Read E in public key from array to U32,
* so only use last byte0~byte3, others are zero
*/
td_void cipher_get_pub_exponent(cryp_rsa_key *key, const ot_cipher_rsa_public_key *rsa_key)
{
    td_u32 i;
    td_u8 *buf;
    td_u8 *pub;
    td_u32 len;
    buf = rsa_key->e;
    pub = (td_u8 *)(&(key->e));
    len = rsa_key->e_len;
    for (i = 0; i < crypto_min(WORD_WIDTH, len); i++) {
        pub[WORD_WIDTH - i - 1] = buf[len - i - 1];
    }
}

static td_void cipher_rsa_get_hashlen(td_u32 *hash_len, ot_cipher_hash_type sha_type)
{
    switch (sha_type) {
        case OT_CIPHER_HASH_TYPE_SHA256:
            *hash_len = SHA256_RESULT_SIZE;
            break;
        case OT_CIPHER_HASH_TYPE_SHA384:
            *hash_len = SHA384_RESULT_SIZE;
            break;
        case OT_CIPHER_HASH_TYPE_SHA512:
            *hash_len = SHA512_RESULT_SIZE;
            break;
        default:
            ot_cipher_error("Invalid sha type %d\n", sha_type);
    }
}

td_s32 cipher_rsa_pubkey_len_check(ot_cipher_rsa_scheme scheme, const ot_cipher_rsa_public_key *public_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_hash_type sha_type)
{
    if (scheme == OT_CIPHER_RSA_SCHEME_PKCS1_V15) {
        chk_param_err_return(plain_txt->data_len > public_key->n_len - CIPHER_CHECK_VALUE_11);
    } else if (scheme == OT_CIPHER_RSA_SCHEME_PKCS1_V21) {
        td_u32 hash_len = 0;
        cipher_rsa_get_hashlen(&hash_len, sha_type);
        /* the max len of plain txt is key_len - 2 * hash_len - 1 */
        chk_param_err_return(plain_txt->data_len > public_key->n_len - 2 * hash_len - 1);
    }
    return TD_SUCCESS;
}

td_s32 cipher_rsa_prikey_len_check(ot_cipher_rsa_scheme scheme, const ot_cipher_rsa_private_key *private_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_hash_type sha_type)
{
    if (scheme == OT_CIPHER_RSA_SCHEME_PKCS1_V15) {
        chk_param_err_return(plain_txt->data_len > private_key->n_len - CIPHER_CHECK_VALUE_11);
    } else if (scheme == OT_CIPHER_RSA_SCHEME_PKCS1_V21) {
        td_u32 hash_len = 0;
        cipher_rsa_get_hashlen(&hash_len, sha_type);

        /* the max len of plain txt is key_len - 2 * hash_len - 1 */
        chk_param_err_return(plain_txt->data_len > private_key->n_len - 2 * hash_len - 1);
    }
    return TD_SUCCESS;
}
