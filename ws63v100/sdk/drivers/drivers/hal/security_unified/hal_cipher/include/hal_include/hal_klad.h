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
 * Description: hal klad header. \n
 *
 * History: \n
 * 2023-03-22, Create file. \n
 */
#ifndef HAL_KLAD_H
#define HAL_KLAD_H

#include "td_type.h"

#include "hal_rkp.h"
#include "crypto_km_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 hal_klad_lock(td_void);

td_s32 hal_klad_unlock(td_void);

td_s32 hal_klad_set_key_crypto_cfg(td_bool encrypt_support, td_bool decrypt_support, crypto_klad_engine engine);

td_s32 hal_klad_set_key_dest_cfg(crypto_klad_dest dest, crypto_klad_flash_key_type flash_key_type);

td_s32 hal_klad_set_key_secure_cfg(const crypto_klad_key_secure_config *secure_cfg);

td_s32 hal_klad_set_key_addr(crypto_klad_dest klad_dest, td_u32 keyslot_chn);

td_s32 hal_klad_set_data(const td_u8 *data, td_u32 data_length);

td_s32 hal_klad_start_clr_route(crypto_klad_dest klad_dest, const crypto_klad_clear_key *clear_key);

td_s32 hal_klad_start_com_route(crypto_kdf_hard_key_type rk_type, const crypto_klad_effective_key *content_key,
    crypto_klad_dest klad_dest);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif