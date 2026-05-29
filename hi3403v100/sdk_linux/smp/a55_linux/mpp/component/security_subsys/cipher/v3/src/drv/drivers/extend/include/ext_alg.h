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

#ifndef EXT_ALG_H
#define EXT_ALG_H

#include "drv_osal_lib.h"
#include "drv_hash.h"
#include "drv_symc.h"

/******************************* API Declaration *****************************/
/** \addtogroup      hash */
/** @{ */ /** <!--[hash] */

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      cipher drivers */
/** @{ */ /** <!-- [cipher] */

/**
* \brief          AES CMAC init
* \param id       hard key or not
* \param hkey     input data
* \param key      key buffer
* \param klen     key length
 */
td_s32 ext_aes_cmac_init(td_u32 id, td_u32 hkey, td_u8 *key, ot_cipher_key_len klen);

/**
* \brief          AES CMAC update data
* \param id       kapi symc channel ID
* \param input    input data
* \param inlen    input data length, must align at aes block size
 */
td_s32 ext_aes_cmac_update(td_u32 id, td_u8 *input, td_u32 inlen, td_u32 from_user);
/**
* \brief          AES CMAC Finish
* \param id       kapi symc channel ID
* \param input    input data
* \param inlen    input data length, must align at aes block size
* \param mac      cmac output data
 */
td_s32 ext_aes_cmac_finish(td_u32 id, td_u8 *input, td_u32 inlen, td_u32 from_user, td_u8 *mac);

/**
 * \brief          Create symc handle
 *
 * \param handle   symc handle to be initialized
 * \param hard_key symc use hard key or not
 */
td_void *ext_mbedtls_aead_create(td_u32 hard_key);

/**
 * \brief          Clear symc context
 *
 * \param handle      symc handle to be destroy
 */
td_s32 ext_mbedtls_aead_destroy(td_void *ctx);

/**
 * \brief          symc iv schedule
 *
 * \param handle   symc handle
 * \param IV       encryption key
 * \param ivlen    length of iv
 *
 * \return         0 if successful.
 */
td_s32 ext_mbedtls_aead_setiv(td_void *ctx, const td_u8 *iv, td_u32 ivlen, td_u32 usage);

/**
 * \brief          symc key schedule
 *
 * \param ctx      SYMC handle
 * \param key      SYMC key
 * \param keylen   SYMC key length
 *
 * \return         0 if successful, or OT_SYMC_ERR_KEY_LEN_INVALID
 */
td_s32 ext_mbedtls_aead_setkey(td_void *ctx, const td_u8 *fkey, const td_u8 *skey, td_u32 *ot_klen);

/**
 * \brief          SYMC alg and mode
 *
 * \param ctx      SYMC handle
 * \param aad      Associated Data
 * \param alen     Associated Data Length
 * \param tlen     Tag length
 *
 * \return         0 if successful.
 */
td_s32 ext_mbedtls_aead_set_aad(td_void *ctx, compat_addr aad, td_u32 alen, td_u32 tlen);

/**
\brief  get ccm/gcm tag.
\param[in]   chn_num The channel number.
\param[out]  tag The tag value.
\param[out]  taglen tag length
\retval     On received interception, TD_TRUE is returned  otherwise TD_FALSE is returned.
 */
td_s32 ext_mbedtls_aead_get_tag(td_void *ctx, td_u32 tag[AEAD_TAG_SIZE_IN_WORD], td_u32 *taglen);

/**
 * \brief          aead ccm buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param handle   symc handle
 * \param decrypt  decrypt or encrypt
 * \param mode     mode
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param usage_list usage of buffer
 * \param pkg_num  numbers of buffer
 *
 * \return         0 if successful
 */
td_s32 ext_mbedtls_aead_ccm_crypto(td_void *ctx,
                                   td_u32 operation,
                                   compat_addr input[],
                                   compat_addr output[],
                                   td_u32 length[],
                                   td_u32 usage_list[],
                                   td_u32 pkg_num, td_u32 last);

/**
 * \brief          aead gcm buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param handle   symc handle
 * \param decrypt  decrypt or encrypt
 * \param mode     mode
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param usage_list usage of buffer
 * \param pkg_num  numbers of buffer
 *
 * \return         0 if successful
 */
td_s32 ext_mbedtls_aead_gcm_crypto(td_void *ctx,
                                   td_u32 operation,
                                   compat_addr input[],
                                   compat_addr output[],
                                   td_u32 length[],
                                   td_u32 usage_list[],
                                   td_u32 pkg_num, td_u32 last);

/**
 * \brief          Create symc handle
 *
 * \param handle   symc handle to be initialized
 * \param hard_key symc use hard key or not
 */
td_void *ext_mbedtls_symc_create(td_u32 hard_key);

/**
 * \brief          Clear symc context
 *
 * \param handle      symc handle to be destroy
 */
td_s32 ext_mbedtls_symc_destroy(td_void *ctx);

/**
 * \brief          SYMC alg and mode
 *
 * \param handle   SYMC handle
 * \param alg      Symmetric cipher alg
 * \param mode     Symmetric cipher mode
 *
 * \return         0 if successful.
 */
void ext_mbedtls_symc_setmode(td_void *ctx, symc_alg alg, symc_mode mode, symc_width width);

/**
 * \brief          symc iv schedule
 *
 * \param handle   symc handle
 * \param IV       encryption key
 * \param ivlen    length of iv
 *
 * \return         0 if successful.
 */
td_s32 ext_mbedtls_symc_setiv(td_void *ctx, const td_u8 *iv, td_u32 ivlen, td_u32 usage);

/**
 * \brief          Symc iv schedule
 *
 * \param handle   symc handle
 * \param IV       Symc IV
 * \param ivlen    must be 128, 192 or 256
 *
 * \return         0 if successful.
 */
void ext_mbedtls_symc_getiv(td_void *ctx, td_u8 *iv, td_u32 iv_buf_len, td_u32 *ivlen);

/**
 * \brief          symc key schedule
 *
 * \param ctx      SYMC handle
 * \param key      SYMC key
 * \param keylen   SYMC key length
 *
 * \return         0 if successful, or OT_SYMC_ERR_KEY_LEN_INVALID
 */
td_s32 ext_mbedtls_symc_setkey(td_void *ctx, const td_u8 *fkey, const td_u8 *skey, td_u32 *ot_klen);

/**
 * \brief          symc buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param handle   symc handle
 * \param decrypt  decrypt or encrypt
 * \param mode     mode
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param usage_list usage of buffer
 * \param pkg_num  numbers of buffer
 *
 * \return         0 if successful
 */
td_s32 ext_mbedtls_symc_crypto(td_void *ctx,
                               td_u32 operation,
                               compat_addr input[],
                               compat_addr output[],
                               td_u32 length[],
                               symc_node_usage usage_list[],
                               td_u32 pkg_num, td_u32 last);

/**
 * \brief          Create sm4 handle
 *
 * \param handle   sm4 handle to be initialized
 * \param hard_key sm4 use hard key or not
 */
td_void *ext_sm4_create(td_u32 hard_key);

/**
 * \brief          Clear sm4 context
 *
 * \param handle      sm4 handle to be destroy
 */
td_s32 ext_sm4_destroy(const td_void *ctx);

/**
 * \brief          sm4 alg and mode
 *
 * \param handle   sm4 handle
 * \param alg      Symmetric cipher alg
 * \param mode     Symmetric cipher mode
 *
 * \return         NA.
 */
void ext_sm4_setmode(td_void *ctx, symc_alg alg, symc_mode mode, symc_width width);

/**
 * \brief          sm4 iv schedule
 *
 * \param handle   sm4 handle
 * \param IV       encryption key
 * \param ivlen    length of iv
 *
 * \return         0 if successful.
 */
td_s32 ext_sm4_setiv(td_void *ctx, const td_u8 *iv, td_u32 ivlen, td_u32 usage);

/**
 * \brief          Symc iv schedule
 *
 * \param handle   symc handle
 * \param IV       Symc IV
 * \param ivlen    must be 128, 192 or 256
 *
 * \return         NA.
 */
td_s32 ext_sm4_getiv(td_void *ctx, td_u8 *iv, td_u32 iv_buf_len, td_u32 *ivlen);

/**
 * \brief          sm4 key schedule
 *
 * \param ctx      sm4 handle
 * \param key      sm4 key
 * \param keylen   sm4 key length
 *
 * \return         0 if successful, or OT_SYMC_ERR_KEY_LEN_INVALID
 */
td_s32 ext_sm4_setkey(td_void *ctx, const td_u8 *fkey, const td_u8 *skey, td_u32 *ot_klen);

/**
 * \brief          sm4 buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param handle   sm4 handle
 * \param decrypt  decrypt or encrypt
 * \param mode     mode
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param usage_list usage of buffer
 * \param pkg_num  numbers of buffer
 *
 * \return         0 if successful
 */
td_s32 ext_sm4_crypto(td_void *ctx,
                      td_u32 operation,
                      compat_addr input[],
                      compat_addr output[],
                      td_u32 length[],
                      symc_node_usage usage_list[],
                      td_u32 pkg_num, td_u32 last);

/**
 * \brief          Clear hash context
 *
 * \param ctx      symc handle to be destroy
 */
td_void *mbedtls_hash_create(hash_mode mode);

/**
 * \brief          Hash message chunk calculation
 *
 * Note: the message must be write to the buffer
 * which get from cryp_hash_get_cpu_addr, and the length of message chunk
 * can't large than the length which get from cryp_hash_get_cpu_addr.
 *
 * \param ctx      hash handle to be destroy
 * \param chunk    hash message to update
 * \param length   length of hash message
 * \param src      source of hash message
 */
td_s32 mbedtls_hash_update(td_void *ctx, const td_u8 *chunk, td_u32 chunkLen, hash_chunk_src src);

/**
 * \brief          HASH final digest
 *
 * \param ctx      Hash handle
 * \param hash     HASH checksum result
 * \param hashlen  Length of HASH checksum result
 */
td_s32 mbedtls_hash_finish(td_void *ctx, td_void *hash, td_u32 hash_buf_len, td_u32 *hashlen);

/**
 * \brief          Clear hash context
 *
 * \param ctx      symc handle to be destroy
 */
td_s32 mbedtls_hash_destroy(td_void *ctx);

/* sm3 */
td_void *ext_sm3_create(hash_mode mode);

td_s32 ext_sm3_update(td_void *ctx, const td_u8 *chunk, td_u32 chunkLen, hash_chunk_src src);

td_s32 ext_sm3_finish(td_void *ctx, td_void *hash, td_u32 hash_buf_len, td_u32 *hashlen);

td_s32 ext_sm3_destroy(td_void *ctx);

/* sm2 */
td_s32 mbedtls_sm2_sign(td_u32 e[SM2_LEN_IN_WROD], td_u32 d[SM2_LEN_IN_WROD],
                        td_u32 r[SM2_LEN_IN_WROD], td_u32 s[SM2_LEN_IN_WROD], td_u32 length);

td_s32 mbedtls_sm2_verify(td_u32 e[SM2_LEN_IN_WROD],
                          td_u32 px[SM2_LEN_IN_WROD], td_u32 py[SM2_LEN_IN_WROD],
                          td_u32 r[SM2_LEN_IN_WROD], td_u32 s[SM2_LEN_IN_WROD], td_u32 length);

td_s32 mbedtls_sm2_encrypt(td_u32 px[SM2_LEN_IN_WROD], td_u32 py[SM2_LEN_IN_WROD],
                           td_u32 c1x[SM2_LEN_IN_WROD], td_u32 c1y[SM2_LEN_IN_WROD],
                           td_u32 x2[SM2_LEN_IN_WROD], td_u32 y2[SM2_LEN_IN_WROD], td_u32 length);

td_s32 mbedtls_sm2_decrypt(td_u32 d[SM2_LEN_IN_WROD],
                           td_u32 c1x[SM2_LEN_IN_WROD], td_u32 c1y[SM2_LEN_IN_WROD],
                           td_u32 x2[SM2_LEN_IN_WROD], td_u32 y2[SM2_LEN_IN_WROD], td_u32 length);

td_s32 mbedtls_sm2_gen_key(td_u32 d[SM2_LEN_IN_WROD],
                           td_u32 px[SM2_LEN_IN_WROD], td_u32 py[SM2_LEN_IN_WROD], td_u32 length);

/**
* \ brief Computes CRT parameters dP, dQ and qInv from the two primes p and q and the public exponent e.
* \param key      rsa key
* \return         0 if successful
 */
td_s32 ext_rsa_compute_crt(cryp_rsa_key *key);

/**
* \brief Compute ECDH shared secret key. This key corresponds to the X coordinates of the computed P point.
* \param d         ecc private key
* \param px        ecc x public key
* \param py        ecc y public key
* \param sharekey  ecdh share key
* \retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 ext_ecdh_compute_key(ecc_param_t *ecc, td_u8 *d, td_u8 *px, td_u8 *py, td_u8 *sharekey);

/**
* \brief Generate a ECC key pair.
* \param inkey    input private key
* \param outkey   input private key
* \param px       ECC x public key
* \param py       ECC y public key
* \retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 ext_ecc_gen_key(ecc_param_t *ecc, td_u8 *inkey, td_u8 *outkey, td_u8 *px, td_u8 *py);

/**
* \brief ECDSA signature a context with appendix, where a signers ECC private key is used.
* \param d         ECC private key
* \param hash      hash to be sign
* \param hlen      length of hash to be sign
* \param r         ECC sign result of r
* \param s         ECC sign result of s
* \retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 ext_ecdsa_sign_hash(ecc_param_t *ecc, td_u8 *d, td_u8 *hash, td_u32 hlen, td_u8 *r, td_u8 *s);

/**
* \brief ECDSA signature verification a context with appendix, where a signers ECC public key is used.
* \param px       ECC x public key
* \param py       ECC y public key
* \param hash     hash to be verification
* \param hlen     length of hash to be verification
* \param r        ECDSA sign result of r
* \param s        ECDSA sign result of s
* \retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 ext_ecdsa_verify_hash(ecc_param_t *ecc, td_u8 *px, td_u8 *py, td_u8 *hash, td_u32 hlen, td_u8 *r, td_u8 *s);

/**
* \brief Generate a RSA private key.
*
* \param[in]  key:   rsa key struct.
* \retval TD_SUCCESS  Call this API successful
* \retval TD_FAILURE  Call this API fails.
 */
td_s32 mpi_rsa_gen_key(cryp_rsa_key *key);

/**
* \brief Compute Diffie-Hellman shared secret as otherPubKey^privKey mod p.
*
* \param[in]  key:   rsa key struct.
* \retval TD_SUCCESS  Call this API successful
* \retval TD_FAILURE  Call this API fails.
 */
td_s32 mpi_rsa_compute_ctr(cryp_rsa_key *key);

/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* EXT_ALG_H */
