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

#ifndef __SS_MPI_CIPHER_H__
#define __SS_MPI_CIPHER_H__

#include "ot_common_cipher.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_cipher_init(td_void);

td_s32 ss_mpi_cipher_deinit(td_void);

td_s32 ss_mpi_cipher_create(td_handle *handle, const ot_cipher_attr *cipher_attr);

td_s32 ss_mpi_cipher_destroy(td_handle handle);

td_s32 ss_mpi_cipher_set_cfg(td_handle handle, const ot_cipher_ctrl *ctrl);

td_s32 ss_mpi_cipher_get_cfg(td_handle handle, ot_cipher_ctrl *ctrl);

td_s32 ss_mpi_cipher_encrypt(td_handle handle,
    td_phys_addr_t src_phys_addr, td_phys_addr_t dst_phys_addr, td_u32 byte_len);

td_s32 ss_mpi_cipher_decrypt(td_handle handle,
    td_phys_addr_t src_phys_addr, td_phys_addr_t dst_phys_addr, td_u32 byte_len);

td_s32 ss_mpi_cipher_encrypt_virt(td_handle handle, const td_u8 *src_data, td_u8 *dst_data, td_u32 byte_len);

td_s32 ss_mpi_cipher_decrypt_virt(td_handle handle, const td_u8 *src_data, td_u8 *dst_data, td_u32 byte_len);

td_s32 ss_mpi_cipher_encrypt_multi_pack(td_handle handle, const ot_cipher_data *data_pack, td_u32 data_pack_num);

td_s32 ss_mpi_cipher_decrypt_multi_pack(td_handle handle, const ot_cipher_data *data_pack, td_u32 data_pack_num);

td_s32 ss_mpi_cipher_get_tag(td_handle handle, td_u8 *tag, td_u32 tag_len);

td_s32 ss_mpi_cipher_hash_init(const ot_cipher_hash_attr *hash_attr, td_handle *handle);

td_s32 ss_mpi_cipher_hash_update(td_handle handle, const td_u8 *in_data, td_u32 in_data_len);

td_s32 ss_mpi_cipher_hash_final(td_handle handle, td_u8 *out_hash, td_u32 out_hash_len);

td_s32 ss_mpi_cipher_get_random_num(td_u32 *random_num);

td_s32 ss_mpi_cipher_rsa_public_encrypt(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt);

td_s32 ss_mpi_cipher_rsa_private_decrypt(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_private_key *rsa_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt);

td_s32 ss_mpi_cipher_rsa_private_encrypt(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_private_key *rsa_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt);

td_s32 ss_mpi_cipher_rsa_public_decrypt(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt);

td_s32 ss_mpi_cipher_rsa_sign(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_private_key *rsa_key,
    const ot_cipher_sign_in_data *rsa_data, ot_cipher_common_data *sign_data);

td_s32 ss_mpi_cipher_rsa_verify(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_sign_in_data *rsa_data, const ot_cipher_common_data *sign_data);

td_s32 ss_mpi_cipher_sm2_encrypt(const ot_cipher_sm2_public_key *sm2_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt);

td_s32 ss_mpi_cipher_sm2_decrypt(const ot_cipher_sm2_private_key *sm2_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt);

td_s32 ss_mpi_cipher_sm2_sign(const ot_cipher_sm2_sign *sm2_sign,
    const ot_cipher_sign_in_data *sm2_data, ot_cipher_sm2_sign_data *sign_data);

td_s32 ss_mpi_cipher_sm2_verify(const ot_cipher_sm2_verify *sm2_verify,
    const ot_cipher_sign_in_data *sm2_data, const ot_cipher_sm2_sign_data *sign_data);

td_s32 ss_mpi_keyslot_create(const ot_keyslot_attr *attr, td_handle *keyslot);

td_s32 ss_mpi_keyslot_destroy(td_handle keyslot);

td_s32 ss_mpi_cipher_attach(td_handle cipher, td_handle keyslot);

td_s32 ss_mpi_cipher_detach(td_handle cipher, td_handle keyslot);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_CIPHER_H__ */
