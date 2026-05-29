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

#ifndef COMMON_CHECK_PARAM_H
#define COMMON_CHECK_PARAM_H

#include "ot_type.h"
#include "ot_common_cipher.h"
#include "drv_cipher_ioctl.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_rsa_chk_private_key(const ot_cipher_rsa_private_key *rsa_key);

td_s32 mpi_rsa_get_crypto_attr(ot_cipher_rsa_encrypt_scheme *rsa_scheme,
    ot_cipher_rsa_scheme scheme, ot_cipher_hash_type sha_type);

td_s32 mpi_rsa_get_sign_attr(ot_cipher_rsa_sign_scheme *rsa_scheme,
    td_u32 *hash_len, ot_cipher_rsa_scheme scheme, ot_cipher_hash_type sha_type);

td_s32 mpi_rsa_chk_verify_param(const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_sign_in_data *rsa_data, const ot_cipher_common_data *sign_data);

td_s32 mpi_rsa_chk_sign_param(const ot_cipher_rsa_private_key *rsa_key,
    const ot_cipher_sign_in_data *rsa_data, const ot_cipher_common_data *sign_data);

td_s32 cipher_rsa_public_encrypt_check(const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt);

td_s32 cipher_rsa_public_decrypt_check(const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt);

td_void cipher_get_pub_exponent(cryp_rsa_key *key, const ot_cipher_rsa_public_key *rsa_key);

td_s32 cipher_rsa_pubkey_len_check(ot_cipher_rsa_scheme scheme, const ot_cipher_rsa_public_key *public_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_hash_type sha_type);

td_s32 cipher_rsa_prikey_len_check(ot_cipher_rsa_scheme scheme, const ot_cipher_rsa_private_key *private_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_hash_type sha_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* COMMON_CHECK_PARAM_H */
