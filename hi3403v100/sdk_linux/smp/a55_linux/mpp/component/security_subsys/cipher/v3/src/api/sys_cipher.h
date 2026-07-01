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

#ifndef SYS_CIPHER_H
#define SYS_CIPHER_H

#include "drv_cipher_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

/* rsa sign and verify struct for sys class. */
typedef struct {
    const td_u8 *hash;
    td_u32 hash_len;
    td_u8 *sign;
    td_u32 sign_len;
} sys_rsa_sign_data;

/* RSA crypto data */
typedef struct {
    const td_u8 *in;                        /* Input data to be cryption. */
    td_u32 in_len;                          /* Length of input data. */
    td_u8 *out;                             /* Output data. */
    td_u32 out_buf_len;                     /* Length of output buffer. */
    td_u32 *out_len;                        /* Length of output valid data. */
} sys_rsa_crypto_data;

/******************************* API Declaration *****************************/

/*
 * brief   mpi Init.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_symc_init(td_void);

/*
 * brief   Kapi Deinit.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_symc_deinit(td_void);

/*
 * brief   Create symc handle.
 * param[in]  id The channel number.
 * param[in]  uuid The user identification.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_symc_create(td_u32 *id, ot_cipher_type type);

/*
 * brief   Destroy symc handle.
 * param[in]  id The channel number.
 * param[in]  uuid The user identification.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_symc_destroy(td_u32 id);

/*
 * brief  set work params.
 * param[in]  ctrl          Sys symc config ctrl.
 * retval     On success,   TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_symc_config(td_u32 id, const ot_cipher_ctrl *ctrl);


/*
 * brief  get work params.
 * param[in]  id The channel number.
 * param[out] ctrl information.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_symc_get_config(td_u32 id, ot_cipher_ctrl *ctrl);

/*
 * brief          SYMC  buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param[in] id   The channel number.
 * param input    buffer holding the input data
 * param output   buffer holding the output data
 * param length   length of the input data
 * param decrypt  decrypt or encrypt
 *
 * return         0 if successful
 */
td_s32 sys_symc_crypto(td_u32 id, const compat_addr in, const compat_addr out, td_u32 len, td_u32 operation);

/*
 * brief          SYMC multiple buffer encryption/decryption.
 * param[in]   id The channel number.
 * param[out]  tag tag data of CCM/GCM
 * param[in]   tag_buf_len tag buf length of CCM/GCM
 * param[in]   taglen tag length of CCM/GCM
 * param uuid uuid The user identification.
 *
 * return         0 if successful
 */
td_s32 sys_aead_get_tag(td_u32 id, td_u8 *tag, td_u32 taglen);

/*
 * brief   Kapi Init.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_hash_init(td_void);

/*
 * brief   Kapi Deinit.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_hash_deinit(td_void);

/*
 * brief          HASH context setup.
 *
 * param[out] id The channel number.
 * param[in] type    Hash type
 * param[in] key     hmac key
 * param[in] keylen  hmac key length
 *
 * return         0 if successful
 */
td_s32 sys_hash_start(td_u32 *id, ot_cipher_hash_type type, const td_u8 *key, td_u32 keylen);

/*
 * brief          HASH process buffer.
 *
 * param[in] id       The channel number.
 * param[in] input    buffer holding the input data
 * param[in] length   length of the input data
 * param[in] src      source of hash message
 *
 * return         0 if successful
 */
td_s32 sys_hash_update(td_u32 id, const td_u8 *input, td_u32 length, hash_chunk_src src);

/*
 * brief          HASH final digest.
 *
 * param[in]  id      The channel number.
 * param[out] hash    buffer holding the hash data
 * param[out] hashlen length of the hash data
 * param[in]  uuid    uuid The user identification.
 *
 * return         0 if successful
 */
td_s32 sys_hash_finish(td_u32 id, td_u8 *hash, td_u32 hashlen);

/*
 * brief   Kapi Init.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_rsa_init(td_void);

/*
 * brief   Kapi Deinitialize.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_rsa_deinit(td_void);

/*
 * brief RSA encryption a plaintext with a RSA private key.
 *
 * param[in] key:     rsa key struct.
 * param[in] scheme:  rsa encrypt/decrypt scheme.
 * param[in/out] rsa_data: input and output data of encryption
 * retval TD_SUCCESS  Call this API successful
 * retval TD_FAILURE  Call this API fails.
 */
td_s32 sys_rsa_encrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, sys_rsa_crypto_data *rsa_data);

/*
 * brief RSA decryption a ciphertext with a RSA public key.
 *
 * param[in] key:     rsa key struct.
 * param[in] scheme:  rsa encrypt/decrypt scheme.
 * param[in/out] rsa_data: input and output data of decryption
 * retval TD_SUCCESS  Call this API successful
 * retval TD_FAILURE  Call this API fails.
 */
td_s32 sys_rsa_decrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, sys_rsa_crypto_data *rsa_data);

/*
 * brief RSA signature a context with appendix, where a signer's RSA private key is used.
 *
 * param[in] rsa_sign:rsa sign struct info.
 * param[out]sign:    rsa signature.
 * param[out] sign_len:length of rsa signature
 * retval TD_SUCCESS  Call this API successful
 * retval TD_FAILURE  Call this API fails.
 */
td_s32 sys_rsa_sign_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *rsa_key, sys_rsa_sign_data *sign_data);

/*
 * brief RSA verify a ciphertext with a RSA public key.
 *
 * param[in]  rsa_verify:rsa verify struct info.
 * param[in]  sign:   signature info
 * param[in]  sign_len:  length of rsa signature
 * retval TD_SUCCESS  Call this API successful
 * retval TD_FAILURE  Call this API fails.
 */
td_s32 sys_rsa_verify_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *rsa_key, const sys_rsa_sign_data *sign_data);

/*
 * brief get rand number.
 * param[out]  randnum rand number.
 * param[in]   timeout time out.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 sys_trng_get_random(td_u32 *randnum, td_u32 timeout);

/**
\brief SM2 signature a context with appendix, where a signers SM2 private key is used.
\param d[in]     sm2 private key
\param px[in]    sm2 x public key
\param py[in]    sm2 y public key
\param id[in]    sm2 user id
\param idlen[in] length of sm2 user id
\param msg[in]   message to be sign
\param msglen[in] length of message to be sign
\param src[in]    source of hash message
\param r[out]      sm2 sign result of r
\param s[out]      sm2 sign result of s
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 sys_sm2_sign(const ot_cipher_sm2_sign *sm2_sign,
    const ot_cipher_common_data *msg, ot_cipher_sm2_sign_data *sign_data);

/**
\brief SM2 signature verification a context with appendix, where a signers SM2 public key is used.
\param px[in]    sm2 x public key
\param py[in]    sm2 y public key
\param id[in]    sm2 user id
\param idlen[in] length of sm2 user id
\param msg[in]   message to be sign
\param msglen[in] length of message to be sign
\param src[in]    source of hash message
\param r[in]      sm2 sign result of r
\param s[in]      sm2 sign result of s
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 sys_sm2_verify(const ot_cipher_sm2_verify *sm2_verify,
    const ot_cipher_common_data *msg, const ot_cipher_sm2_sign_data *sign_data);

/**
\brief SM2 encryption a plaintext with a RSA public key.
\param px[in]    sm2 x public key
\param py[in]    sm2 y public key
\param msg[in]   message to be encryption
\param msglen[in] length of message to be encryption
\param enc[out]    encryption message
\param enclen[out] length of encryption message
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 sys_sm2_encrypt(const ot_cipher_sm2_public_key *sm2_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt);
/**
\brief SM2 decryption a plaintext with a RSA public key.
\param d[in]     sm2 private key
\param enc[out]    message to be decryption
\param enclen[out] length of message to be decryption
\param msg[in]     decryption message
\param msglen[in]  length of decryption message
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 sys_sm2_decrypt(const ot_cipher_sm2_private_key *sm2_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt);

td_s32 sys_keyslot_create(const ot_keyslot_attr *attr, td_handle *keyslot);

td_s32 sys_keyslot_destroy(td_handle keyslot);

td_s32 sys_cipher_attach(td_handle cipher, td_handle keyslot);

td_s32 sys_cipher_detach(td_handle cipher, td_handle keyslot);

/** @} */  /** <!-- ==== API Code end ==== */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SYS_CIPHER_H */
