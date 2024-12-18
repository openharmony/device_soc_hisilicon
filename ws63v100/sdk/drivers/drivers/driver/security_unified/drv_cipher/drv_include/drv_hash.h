/**
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 *
 * Description: cipher driver hash header. \n
 *
 * History: \n
 * 2023-03-22, Create file. \n
 */
#ifndef DRV_HASH_H
#define DRV_HASH_H

#include "crypto_type.h"
#include "crypto_hash_struct.h"
#include "crypto_kdf_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_cipher_hash_init(td_void);

td_s32 drv_cipher_hash_deinit(td_void);

const td_u32 *drv_hash_get_state_iv(crypto_hash_type hash_type, td_u32 *iv_size);

td_s32 drv_cipher_hash_start(td_handle *drv_hash_handle, const crypto_hash_attr *hash_attr);

td_s32 drv_cipher_hash_update(td_handle drv_hash_handle,  const crypto_buf_attr *src_buf, const td_u32 len);

td_s32 drv_cipher_hash_finish(td_handle drv_hash_handle, td_u8 *out, td_u32 *out_len);

td_s32 drv_cipher_hash_finish_data(td_handle drv_hash_handle, td_u8 *out, td_u32 *out_len);

td_s32 drv_cipher_hash_get(td_handle drv_hash_handle, crypto_hash_clone_ctx *hash_ctx);

td_s32 drv_cipher_hash_set(td_handle drv_hash_handle, const crypto_hash_clone_ctx *hash_ctx);

td_s32 drv_cipher_hash_destroy(td_handle drv_hash_handle);

td_void drv_cipher_hash_debug_handle(td_handle drv_hash_handle);

td_s32 drv_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif