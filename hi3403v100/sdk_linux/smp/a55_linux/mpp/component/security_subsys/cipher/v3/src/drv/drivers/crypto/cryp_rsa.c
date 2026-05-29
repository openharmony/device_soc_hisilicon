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

#include "cryp_rsa.h"
#include "drv_osal_lib.h"
#include "drv_pke.h"
#include "cryp_trng.h"

#if (defined(CHIP_RSA_SUPPORT) || defined(SOFT_RSA_SUPPORT))
#include "mbedtls/rsa.h"
#include "mbedtls/bignum.h"

/* Internal Structure Definition */
#define RSA_PKCS1_TYPE_MIN_PAD_LEN               11

/* rsa key len in bits */
#define RSA_BITS_1024                            1024
#define RSA_BITS_2048                            2048
#define RSA_BITS_3072                            3072
#define RSA_BITS_4096                            4096

#define CRYP_TRNG_TIMEOUT  (-1)

typedef enum {
    RSA_BLOCK_YTPE_0 = 0X0,
    RSA_BLOCK_YTPE_1,
    RSA_BLOCK_YTPE_2,
} rsa_pkcs1_padding_type;

typedef struct {
    td_u32 mode;
    td_u32 klen;
    td_u8 *in;
    td_u32 in_len;
    td_u8 *out;
    td_u32 *out_len;
    td_u8 bt;
} rsa_padding_pack;

/* rsa mutex */
static CRYPTO_MUTEX g_rsa_mutex;

/* rsa rsa soft function */
int mbedtls_mpi_exp_mod_sw(mbedtls_mpi *x, const mbedtls_mpi *a,
    const mbedtls_mpi *e, const mbedtls_mpi *n, mbedtls_mpi *_rr);

#define kapi_rsa_lock_err_return()                      \
    do {                                                \
        ret = crypto_mutex_lock(&g_rsa_mutex);          \
        if (ret != TD_SUCCESS) {                        \
            ot_cipher_error("error, rsa lock failed\n");   \
            return ret;                                 \
        }                                               \
    } while (0)

#define kapi_rsa_unlock()   crypto_mutex_unlock(&g_rsa_mutex)

/* API Code for cryp rsa */
static td_void mbedtls_mpi_print(const mbedtls_mpi *x, const char *name)
{
#if (LOG_DEFAULT == CIPHER_LEVEL_DBG)
    int ret;
    size_t n;
    td_u8 buf[RSA_KEY_LEN_4096] = {0};

    n = mbedtls_mpi_size(x);
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(x, buf, n));

cleanup:
    return;
#else
    crypto_unused(x);
    crypto_unused(name);
#endif
}

#ifdef CHIP_RSA_SUPPORT
static td_s32 rsa_get_klen(unsigned long module_len, td_u32 *keylen, rsa_key_width *width)
{
    if (module_len > RSA_KEY_LEN_3072 && module_len <= RSA_KEY_LEN_4096) {
        *keylen = RSA_KEY_LEN_4096;
        *width = RSA_KEY_WIDTH_4096;
    } else if (module_len > RSA_KEY_LEN_2048 && module_len <= RSA_KEY_LEN_3072) {
        *keylen = RSA_KEY_LEN_3072;
        *width = RSA_KEY_WIDTH_3072;
    } else if (module_len > RSA_KEY_LEN_1024 && module_len <= RSA_KEY_LEN_2048) {
        *keylen = RSA_KEY_LEN_2048;
        *width = RSA_KEY_WIDTH_2048;
    } else {
        ot_cipher_error("error, invalid key len %ld\n", module_len);
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 cryp_check_data(const td_u8 *n, const td_u8 *e, const td_u8 *mc, td_u32 len)
{
    td_u32 i;

    /* MC > 0 */
    for (i = 0; i < len; i++) {
        if (mc[i] > 0) {
            break;
        }
    }
    if (i >= len) {
        ot_cipher_error("RSA M/C is zero, error!\n");
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    /* MC < N */
    for (i = 0; i < len; i++) {
        if (mc[i] < n[i]) {
            break;
        }
    }
    if (i >= len) {
        ot_cipher_error("RSA M/C is larger than N, error!\n");
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    /* E >= 1 */
    for (i = 0; i < len; i++) {
        if (e[i] > 0) {
            break;
        }
    }
    if (i >= len) {
        ot_cipher_error("RSA D/E is zero, error!\n");
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    /* N is Odd ? */
    if (((n[len - 1] & 0x01) == 0x00)) {
        ot_cipher_error("RSA N is odd, error!\n");
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

static int cryp_ifep_rsa_exp_mod(mbedtls_mpi *x, const mbedtls_mpi *a,
                                 const mbedtls_mpi *e, const mbedtls_mpi *n)
{
    td_u32 module_len;
    td_u8 *buf = TD_NULL;
    td_u8 *tmp_n = TD_NULL;
    td_u8 *k = TD_NULL;
    td_u8 *in = TD_NULL;
    td_u8 *out = TD_NULL;
    td_u32 keylen = 0;
    rsa_key_width width = 0;
    mbedtls_mpi tmp_a;
    td_s32 ret;

    func_enter();

    /* computes valid bits of N */
    module_len = crypto_max(mbedtls_mpi_size(n), mbedtls_mpi_size(a));

    ret = rsa_get_klen(module_len, &keylen, &width);
    if (ret != TD_SUCCESS) {
        print_func_errno(rsa_get_klen, ret);
        return ret;
    }

    /* mallc buf to store n || k(e or d) || in || out */
    buf = crypto_calloc(MUL_VAL_4, keylen);
    if (buf == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    tmp_n = buf;
    k = tmp_n + keylen;
    in = k + keylen;
    out = in + keylen;

    (td_void)memset_s(&tmp_a, sizeof(mbedtls_mpi), 0, sizeof(mbedtls_mpi));
    mbedtls_mpi_init(&tmp_a);
    chk_func_err_goto(mbedtls_mpi_mod_mpi(&tmp_a, a, n));

    /* read A, E, N */
    chk_func_err_goto(mbedtls_mpi_write_binary(&tmp_a, in, keylen));
    chk_func_err_goto(mbedtls_mpi_write_binary(e, k, keylen));
    chk_func_err_goto(mbedtls_mpi_write_binary(n, tmp_n, keylen));

    /* key and data valid ? */
    chk_func_err_goto(cryp_check_data(tmp_n, k, in, keylen));

    /* out = in ^ k mod n */
    ret = drv_ifep_rsa_exp_mod(tmp_n, k, in, out, width);
    if (ret == TD_SUCCESS) {
        /* write d */
        mbedtls_mpi_read_binary(x, out, keylen);
    }

exit__:
    mbedtls_mpi_free(&tmp_a);
    crypto_free(buf);

    func_exit();

    return ret;
}
#endif

int mbedtls_mpi_exp_mod(mbedtls_mpi *x,
    const mbedtls_mpi *a, const mbedtls_mpi *e, const mbedtls_mpi *n, mbedtls_mpi *rr)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(x);
    chk_ptr_err_return(a);
    chk_ptr_err_return(e);
    chk_ptr_err_return(n);

    mbedtls_mpi_print(a, "M");
    mbedtls_mpi_print(e, "E");
    mbedtls_mpi_print(n, "N");

#if defined(CHIP_RSA_SUPPORT)
    ret = cryp_ifep_rsa_exp_mod(x, a, e, n);
#else
    ret = mbedtls_mpi_exp_mod_sw(x, a, e, n, rr);
#endif
    mbedtls_mpi_print(x, "X");

    if (ret != TD_SUCCESS) {
        ot_cipher_error("rsa mpi_exp_mod failed, ret = 0x%x\n", ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

int cryp_rsa_init(td_void)
{
    func_enter();

    crypto_mutex_init(&g_rsa_mutex);

#ifdef CHIP_PKE_SUPPORT
    {
        td_s32 ret;
        ret = drv_pke_init();
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_pke_init, ret);
            crypto_mutex_destroy(&g_rsa_mutex);
            return ret;
        }
    }
#endif

    func_exit();
    return TD_SUCCESS;
}

void cryp_rsa_deinit(void)
{
#ifdef CHIP_PKE_SUPPORT
    drv_pke_deinit();
#endif

    crypto_mutex_destroy(&g_rsa_mutex);
}

int mbedtls_get_random(td_void *param, td_u8 *randnum, size_t size)
{
    td_s32 ret;
    td_u32 num;
    size_t i;

    crypto_unused(param);

    chk_ptr_err_return(randnum);

    for (i = 0; i < size; i += WORD_WIDTH) {
        ret = cryp_trng_get_random(&num, CRYP_TRNG_TIMEOUT);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_trng_get_random, ret);
            return ret;
        }

        ret = memcpy_s(randnum, size - i,
            (td_u8 *)&num, ((size - i) > WORD_WIDTH) ? WORD_WIDTH : (size - i));
        if (ret != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            return OT_ERR_CIPHER_FAILED_SEC_FUNC;
        }
    }

    /* non-zero random octet string */
    for (i = 0; i < size; i++) {
        if (randnum[i] != 0x00) {
            continue;
        }

        ret = cryp_trng_get_random(&num, CRYP_TRNG_TIMEOUT);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_trng_get_random, ret);
            return ret;
        }
        randnum[i] = (td_u8)(num) & 0xFF;

        if (i > 0) {
            i--;
        } else {
            continue;
        }
    }

    return TD_SUCCESS;
}

static td_s32 cryp_rsa_init_key(const cryp_rsa_key *key, td_u32 *mode, mbedtls_rsa_context *rsa)
{
    td_s32 ret;

    func_enter();

    mbedtls_mpi_init(&rsa->N);
    mbedtls_mpi_init(&rsa->E);
    mbedtls_mpi_init(&rsa->D);
    mbedtls_mpi_init(&rsa->P);
    mbedtls_mpi_init(&rsa->Q);
    mbedtls_mpi_init(&rsa->DP);
    mbedtls_mpi_init(&rsa->DQ);
    mbedtls_mpi_init(&rsa->QP);

    chk_func_err_goto(mbedtls_mpi_read_binary(&rsa->N, key->n, key->klen));
    rsa->len = key->klen;

    if (key->public) {
        chk_func_err_goto(mbedtls_mpi_read_binary(&rsa->E, (td_u8 *)&key->e, WORD_WIDTH));
        *mode = MBEDTLS_RSA_PUBLIC;
    } else {
        if (key->d != TD_NULL) { /* Non CRT */
            chk_func_err_goto(mbedtls_mpi_read_binary(&rsa->D, key->d, key->klen));
            *mode = MBEDTLS_RSA_PRIVATE;
        } else { /* CRT */
            chk_func_err_goto(mbedtls_mpi_read_binary(&rsa->P, key->p, key->klen >> 1));
            chk_func_err_goto(mbedtls_mpi_read_binary(&rsa->Q, key->q, key->klen >> 1));
            chk_func_err_goto(mbedtls_mpi_read_binary(&rsa->DP, key->dp, key->klen >> 1));
            chk_func_err_goto(mbedtls_mpi_read_binary(&rsa->DQ, key->dq, key->klen >> 1));
            chk_func_err_goto(mbedtls_mpi_read_binary(&rsa->QP, key->qp, key->klen >> 1));
            *mode = MBEDTLS_RSA_PRIVATE;
        }
    }

    ot_cipher_dbg("mode %u, e 0x%x\n", *mode, key->e);

    func_exit();
    return TD_SUCCESS;

exit__:
    mbedtls_mpi_free(&rsa->N);
    mbedtls_mpi_free(&rsa->E);
    mbedtls_mpi_free(&rsa->D);
    mbedtls_mpi_free(&rsa->P);
    mbedtls_mpi_free(&rsa->Q);
    mbedtls_mpi_free(&rsa->DP);
    mbedtls_mpi_free(&rsa->DQ);
    mbedtls_mpi_free(&rsa->QP);

    return ret;
}

static td_void cryp_rsa_deinit_key(mbedtls_rsa_context *rsa)
{
    func_enter();

    mbedtls_mpi_free(&rsa->N);
    mbedtls_mpi_free(&rsa->E);
    mbedtls_mpi_free(&rsa->D);
    mbedtls_mpi_free(&rsa->P);
    mbedtls_mpi_free(&rsa->Q);
    mbedtls_mpi_free(&rsa->DP);
    mbedtls_mpi_free(&rsa->DQ);
    mbedtls_mpi_free(&rsa->QP);

    func_exit();
}

static td_s32 cryp_rsa_get_alg(td_u32 scheme, int *padding, int *hash_id, int *hashlen)
{
    switch (scheme) {
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_PKCS1_V1_5:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = 0;
            *hashlen = 0;
            break;
        case OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA256;
            *hashlen = SHA256_RESULT_SIZE;
            break;
        case OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA384;
            *hashlen = SHA384_RESULT_SIZE;
            break;
        case OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA512;
            *hashlen = SHA512_RESULT_SIZE;
            break;
        case OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA256:
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA256;
            *hashlen = SHA256_RESULT_SIZE;
            break;
        case OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA384:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA384:
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA384;
            *hashlen = SHA384_RESULT_SIZE;
            break;
        case OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA512:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA512:
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA512;
            *hashlen = SHA512_RESULT_SIZE;
            break;
        default:
            ot_cipher_error("RSA padding mode error, mode = 0x%x.\n", scheme);
            return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 ext_rsa_encrypt(ot_cipher_rsa_encrypt_scheme scheme,
    mbedtls_rsa_context *rsa, rsa_padding_pack *pad)
{
    td_s32 ret;

    switch (scheme) {
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA256:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA384:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA512:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_PKCS1_V1_5:
            ret = mbedtls_rsa_pkcs1_encrypt(rsa,
                mbedtls_get_random, TD_NULL, pad->mode, pad->in_len, pad->in, pad->out);
            if (ret != TD_SUCCESS) {
                print_func_errno(mbedtls_rsa_pkcs1_encrypt, ret);
                print_errno(OT_ERR_CIPHER_FAILED_ENCRYPT);
                return OT_ERR_CIPHER_FAILED_ENCRYPT;
            }
            break;
        default:
            ot_cipher_error("RSA padding mode error, mode = 0x%x.\n", scheme);
            print_errno(OT_ERR_CIPHER_INVALID_PARAM);
            return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 cryp_rsa_encrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_crypto_data *rsa_data)
{
    td_s32 ret;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    mbedtls_rsa_context rsa;
    rsa_padding_pack pad;

    func_enter();

    chk_ptr_err_return(key);
    chk_ptr_err_return(rsa_data);
    chk_param_err_return(rsa_data->in  == TD_NULL || rsa_data->in_len == 0);
    chk_param_err_return(rsa_data->out == TD_NULL || rsa_data->out_len == 0);
    chk_param_err_return(key->klen > RSA_KEY_LEN_4096);
    chk_param_err_return(rsa_data->in_len > key->klen);

    ret = cryp_rsa_get_alg(scheme, &padding, &hash_id, &hashlen);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_rsa_get_alg, ret);
        return ret;
    }

    kapi_rsa_lock_err_return();

    mbedtls_rsa_init(&rsa, padding, hash_id);
    (td_void)memset_s(&pad, sizeof(pad), 0, sizeof(pad));

    ret = cryp_rsa_init_key(key, &pad.mode, &rsa);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_rsa_init_key, ret);
        kapi_rsa_unlock();
        return ret;
    }

    pad.in = rsa_data->in;
    pad.in_len = rsa_data->in_len;
    pad.out = rsa_data->out;
    pad.out_len = &rsa_data->out_len;
    pad.klen = key->klen;
    ret = ext_rsa_encrypt(scheme, &rsa, &pad);
    if (ret != TD_SUCCESS) {
        print_func_errno(ext_rsa_encrypt, ret);
        cryp_rsa_deinit_key(&rsa);
        kapi_rsa_unlock();
        return ret;
    }

    rsa_data->out_len = key->klen;
    cryp_rsa_deinit_key(&rsa);
    kapi_rsa_unlock();
    func_exit();
    return ret;
}

static td_s32 ext_rsa_decrypt(ot_cipher_rsa_encrypt_scheme scheme,
    mbedtls_rsa_context *rsa, rsa_padding_pack *pad)
{
    td_s32 ret;
    size_t out_size = 0;

    switch (scheme) {
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA256:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA384:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA512:
        case OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_PKCS1_V1_5:
            ret = mbedtls_rsa_pkcs1_decrypt(rsa, mbedtls_get_random, TD_NULL, pad->mode, &out_size,
                pad->in, pad->out, pad->klen);
            *pad->out_len = (td_u32)out_size;
            if (ret != TD_SUCCESS) {
                print_func_errno(mbedtls_rsa_pkcs1_decrypt, ret);
                print_errno(OT_ERR_CIPHER_INVALID_PARAM);
                return OT_ERR_CIPHER_INVALID_PARAM;
            }
            break;
        default:
            ot_cipher_error("RSA padding mode error, mode = 0x%x.\n", scheme);
            return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 cryp_rsa_decrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_crypto_data *rsa_data)
{
    td_s32 ret;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    mbedtls_rsa_context rsa;
    rsa_padding_pack pad;

    func_enter();

    chk_ptr_err_return(key);
    chk_ptr_err_return(rsa_data);
    chk_param_err_return(rsa_data->in  == TD_NULL || rsa_data->in_len == 0);
    chk_param_err_return(rsa_data->out == TD_NULL || rsa_data->out_len == 0);
    chk_param_err_return(key->klen > RSA_KEY_LEN_4096);
    chk_param_err_return(rsa_data->in_len != key->klen);

    ret = cryp_rsa_get_alg(scheme, &padding, &hash_id, &hashlen);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_rsa_get_alg, ret);
        return ret;
    }

    kapi_rsa_lock_err_return();

    mbedtls_rsa_init(&rsa, padding, hash_id);
    (td_void)memset_s(&pad, sizeof(pad), 0, sizeof(pad));

    ret = cryp_rsa_init_key(key, &pad.mode, &rsa);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_rsa_init_key, ret);
        kapi_rsa_unlock();
        return ret;
    }

    pad.in = rsa_data->in;
    pad.in_len = rsa_data->in_len;
    pad.out = rsa_data->out;
    pad.out_len = &rsa_data->out_len;
    pad.klen = key->klen;
    ret = ext_rsa_decrypt(scheme, &rsa, &pad);
    if (ret != TD_SUCCESS) {
        print_func_errno(ext_rsa_decrypt, ret);
        cryp_rsa_deinit_key(&rsa);
        kapi_rsa_unlock();
        return ret;
    }

    rsa_data->out_len = *pad.out_len;
    cryp_rsa_deinit_key(&rsa);
    kapi_rsa_unlock();
    func_exit();
    return TD_SUCCESS;
}

td_s32 cryp_rsa_sign_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_sign_data *rsa_sign)
{
    td_s32 ret;
    td_u32 mode = 0;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    mbedtls_rsa_context rsa;

    func_enter();

    chk_ptr_err_return(key);
    chk_ptr_err_return(rsa_sign);
    chk_param_err_return(rsa_sign->in  == TD_NULL || rsa_sign->in_len == 0);
    chk_param_err_return(rsa_sign->out == TD_NULL || rsa_sign->out_len == 0);
    chk_param_err_return(key->klen > RSA_KEY_LEN_4096);
    chk_param_err_return(rsa_sign->in_len > key->klen);

    ret = cryp_rsa_get_alg(scheme, &padding, &hash_id, &hashlen);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_rsa_get_alg, ret);
        return ret;
    }
    chk_param_err_return(rsa_sign->in_len < (td_u32)hashlen);

    kapi_rsa_lock_err_return();

    mbedtls_rsa_init(&rsa, padding, hash_id);

    ret = cryp_rsa_init_key(key, &mode, &rsa);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_rsa_init_key, ret);
        kapi_rsa_unlock();
        return ret;
    }

    /* rsa_sign->in is input hash data, rsa_sign->out is output sign data. */
    ret = mbedtls_rsa_pkcs1_sign(&rsa, mbedtls_get_random, TD_NULL,
                                 mode, hash_id, hashlen, rsa_sign->in, rsa_sign->out);
    if (ret != TD_SUCCESS) {
        print_func_errno(mbedtls_rsa_pkcs1_sign, ret);
        print_errno(OT_ERR_CIPHER_RSA_SIGN);
        cryp_rsa_deinit_key(&rsa);
        kapi_rsa_unlock();
        return OT_ERR_CIPHER_RSA_SIGN;
    }

    rsa_sign->out_len = key->klen;
    cryp_rsa_deinit_key(&rsa);

    kapi_rsa_unlock();
    func_exit();
    return TD_SUCCESS;
}

td_s32 cryp_rsa_verify_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *key, const cryp_rsa_sign_data *rsa_verify)
{
    td_s32 ret;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    td_u32 mode = 0;
    mbedtls_rsa_context rsa;

    func_enter();

    chk_ptr_err_return(key);
    chk_ptr_err_return(rsa_verify);
    chk_param_err_return(rsa_verify->in  == TD_NULL || rsa_verify->in_len == 0);
    chk_param_err_return(rsa_verify->out == TD_NULL || rsa_verify->out_len == 0);
    chk_param_err_return(key->klen > RSA_KEY_LEN_4096);
    chk_param_err_return(rsa_verify->in_len != key->klen);
    chk_param_err_return(rsa_verify->out_len > key->klen);

    ret = cryp_rsa_get_alg(scheme, &padding, &hash_id, &hashlen);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_rsa_get_alg, ret);
        return ret;
    }
    chk_param_err_return(rsa_verify->in_len < (td_u32)hashlen);

    kapi_rsa_lock_err_return();

    mbedtls_rsa_init(&rsa, padding, hash_id);

    ret = cryp_rsa_init_key(key, &mode, &rsa);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_rsa_init_key, ret);
        kapi_rsa_unlock();
        return ret;
    }

    /* rsa_verify->out is input hash data, rsa_verify->in is input sign data. */
    ret = mbedtls_rsa_pkcs1_verify(&rsa, mbedtls_get_random,
        TD_NULL, mode, hash_id, hashlen, rsa_verify->out, rsa_verify->in);
    if (ret != TD_SUCCESS) {
        print_func_errno(mbedtls_rsa_pkcs1_verify, ret);
        print_errno(OT_ERR_CIPHER_RSA_VERIFY);
        cryp_rsa_deinit_key(&rsa);
        kapi_rsa_unlock();
        return OT_ERR_CIPHER_RSA_VERIFY;
    }

    cryp_rsa_deinit_key(&rsa);
    kapi_rsa_unlock();
    func_exit();
    return TD_SUCCESS;
}
#endif

/** @} */ /** <!-- ==== API Code end ==== */
