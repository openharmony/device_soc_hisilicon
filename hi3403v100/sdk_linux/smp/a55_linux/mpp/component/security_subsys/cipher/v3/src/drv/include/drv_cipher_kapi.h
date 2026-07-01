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

#ifndef DRV_CIPHER_KAPI_H
#define DRV_CIPHER_KAPI_H

#include "drv_cipher_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

/******************************* API Code *****************************/
/*
 * brief   Kapi Init.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_symc_init(td_void);

/*
 * brief   Kapi Deinit.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_symc_deinit(td_void);

/*
 * brief   Kapi release.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_symc_release(td_void);

/*
 * brief   Create symc handle.
 * param[in]  id The channel number.
 * param[in]  uuid The user identification.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_symc_create(td_u32 *id, ot_cipher_type type);

/*
 * brief   Destroy symc handle.
 * param[in]  id The channel number.
 * param[in]  uuid The user identification.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_symc_destroy(td_u32 id);

/*
 * brief  set work params.
 * param[in]  cfg config information.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_symc_cfg(const symc_cfg_t *cfg);

/*
 * brief  get work params.
 * param[in]  id   The channel number.
 * param[out] ctrl information.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_symc_get_cfg(td_u32 id, ot_cipher_ctrl *ctrl);

/*
 * brief          SYMC  buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param[in] crypt  The symc info data.
 *
 * return           0 if successful
 */
td_s32 kapi_symc_crypto(symc_encrypt_t *crypt, td_bool is_from_virt);

/*
 * brief            SYMC  via buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param[in] crypt  The symc info data.
 * param[in] is_from_user input and output virtual address is from user or kapi.
 *
 * return           0 if successful
 */
td_s32 kapi_symc_crypto_via(symc_encrypt_t *crypt, td_u32 is_from_user);

/*
 * brief      SYMC multiple buffer encryption/decryption.
 * param[in]  id  The channel number.
 * param[in]  tag tag data of CCM/GCM
 *
 * return     0 if successful
 */
td_s32 kapi_aead_get_tag(td_u32 id, td_u32 tag[AEAD_TAG_SIZE_IN_WORD], td_u32 *taglen);

/*
 * brief   Kapi Init.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_hash_init(td_void);

/*
 * brief   Kapi Deinit.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_hash_deinit(td_void);

/*
 * brief          HASH context setup.
 *
 *
 * param[out] id      The channel number.
 * param[in]  type    Hash type
 * param[in]  key     hmac key
 * param[in]  keylen  hmac key length
 *
 * return         0 if successful
 */
td_s32 kapi_hash_start(td_u32 *id, ot_cipher_hash_type type, const td_u8 *key, td_u32 keylen);

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
td_s32 kapi_hash_update(td_u32 id, const td_u8 *input, td_u32 length, hash_chunk_src src);

/*
 * brief          HASH final digest.
 *
 * param[in]  id      The channel number.
 * param[out] hash    buffer holding the hash data_type_t
 * param[in] hash_buf_len    buffer length of holding the hash data
 * param[out] hashlen length of the hash data
 *
 * return         0 if successful
 */
td_s32 kapi_hash_finish(td_u32 id, td_u8 *hash, td_u32 hash_buf_len, td_u32 *hashlen);

/*
 * brief   hash release.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_hash_release(td_void);

td_s32 kapi_hash_destroy(td_u32 id);

td_s32 kapi_hash_destroy_handle(td_handle handle);

td_s32 kapi_hash_get(td_handle handle, drv_hash_context *clone_ctx);

td_s32 kapi_hash_set(td_handle handle, const drv_hash_context *clone_ctx);

/*
 * brief   Kapi Init.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_rsa_init(td_void);

/*
 * brief   Kapi Deinitialize.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_rsa_deinit(td_void);

/*
 * brief RSA encryption a plaintext with a RSA private key.
 *
 * param[in] key:       rsa key struct.
 * param[in/out] rsa:   rsa encrypt/decrypt data.
 * retval ::TD_SUCCESS  Call this API successful
 * retval ::TD_FAILURE  Call this API fails.
 */
td_s32 kapi_rsa_encrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_crypto_data *rsa_data);

/*
 * brief RSA decryption a ciphertext with a RSA public key.
 *
 * param[in] key:       rsa key struct.
 * param[in/out] rsa:   rsa encrypt/decrypt data.
 * retval ::TD_SUCCESS  Call this API successful
 * retval ::TD_FAILURE  Call this API fails.
 */
td_s32 kapi_rsa_decrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_crypto_data *rsa_data);

/*
 * brief RSA signature a context with appendix, where a signer's RSA private key is used.
 *
 * param[in] key:       rsa key struct.
 * param[in/out] rsa:   rsa signature data.
 * retval ::TD_SUCCESS  Call this API successful
 * retval ::TD_FAILURE  Call this API fails.
 */
td_s32 kapi_rsa_sign_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *key, cryp_rsa_sign_data *rsa_data);

/*
 * brief RSA verify a ciphertext with a RSA public key.
 *
 * param[in]  key: rsa key struct.
 * param[in/out] rsa:   rsa verify data.
 * retval ::TD_SUCCESS  Call this API successful
 * retval ::TD_FAILURE  Call this API fails.
 */
td_s32 kapi_rsa_verify_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *key, const cryp_rsa_sign_data *rsa_data);

/**
\brief   Kapi Init.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 kapi_trng_init(td_void);

/**
\brief   Kapi Deinitialize.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 kapi_trng_deinit(td_void);

/*
 * brief get rand number.
 * param[out]  randnum rand number.
 * param[in]   timeout time out.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_trng_get_random(td_u32 *randnum, td_u32 timeout);

/**
\brief get rand bytes.
\param[out]  randnum rand bytes.
\param[in]   size size of rand byte.
\param[in]   timeout time out.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 kapi_trng_get_rand_byte(td_u8 *randbyte, td_u32 size, td_u32 timeout);

/**
\brief   Kapi Init.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 kapi_sm2_init(td_void);

/**
\brief   Kapi Deinitialize.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
td_s32 kapi_sm2_deinit(td_void);

#ifdef CHIP_SM2_SUPPORT
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
td_s32 kapi_sm2_sign(sm2_sign_t *sign, const td_u8 *id, td_u16 idlen, hash_chunk_src src);

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
td_s32 kapi_sm2_verify(sm2_verify_t *verify, const td_u8 *id, td_u16 idlen, hash_chunk_src src);

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
td_s32 kapi_sm2_encrypt(const ot_cipher_sm2_public_key *sm2_key,
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
td_s32 kapi_sm2_decrypt(const ot_cipher_sm2_private_key *sm2_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt);
#endif

td_s32 kapi_keyslot_create(const ot_keyslot_attr *attr, td_handle *keyslot);

td_s32 kapi_keyslot_destroy(td_handle keyslot);

td_s32 kapi_symc_attach(td_handle cipher, td_handle keyslot);

td_s32 kapi_symc_detach(td_handle cipher, td_handle keyslot);

/** <!-- ==== API Code end ==== */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_CIPHER_KAPI_H */
