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

#ifndef CRYP_RSA_H
#define CRYP_RSA_H

#include "drv_osal_lib.h"

/******************************* API Code *****************************/
/*
 * brief          Initialize crypto of rsa *
 */
td_s32 cryp_rsa_init(td_void);

/*
 * brief          Deinitialize crypto of rsa *
 */
td_void cryp_rsa_deinit(td_void);

/*
 * brief RSA encryption a plaintext with a RSA key.
 * param[in] key:         rsa key.
 * param[in] rsa_crypt:   rsa encrypt data.
 * retval ::TD_SUCCESS  Call this API successful.
 * retval ::TD_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
td_s32 cryp_rsa_encrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_crypto_data *rsa_data);

/*
 * brief RSA decryption a plaintext with a RSA key.
 * param[in] key:         rsa key.
 * param[in] rsa_crypt:   rsa decrypt data.
 * retval ::TD_SUCCESS  Call this API successful.
 * retval ::TD_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
td_s32 cryp_rsa_decrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_crypto_data *rsa_data);

/*
 * brief RSA sign a hash value with a RSA private key.
 * param[in] key:         rsa key.
 * param[in] rsa_sign:    rsa sign data.
 * retval ::TD_SUCCESS  Call this API successful.
 * retval ::TD_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
td_s32 cryp_rsa_sign_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_sign_data *rsa_sign);

/*
 * brief RSA verify a hash value with a RSA public key.
 * param[in] key:         rsa key.
 * param[in] rsa_verify:  rsa sign data.
 * retval ::TD_SUCCESS  Call this API successful.
 * retval ::TD_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
td_s32 cryp_rsa_verify_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *key, const cryp_rsa_sign_data *rsa_verify);

/*
 * brief Generate random.
 * N/A
 */
int mbedtls_get_random(td_void *param, td_u8 *rand, size_t size);

/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* CRYP_RSA_H */
