/*
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
 * Description: UPG verify header file.
 */

#ifndef UPG_VERIFY_H
#define UPG_VERIFY_H

#include "upg.h"
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct upg_auth_data {
    uint32_t length;
    uint8_t *data;
} upg_auth_data_t;

errcode_t verify_hash_cmp(const uint8_t *hash, const uint8_t *hash_res, uint32_t hash_len);

errcode_t secure_authenticate(const uint8_t *key, const upg_auth_data_t *data, uint8_t *sign_buff);

errcode_t calc_hash(uint32_t src_addr, uint32_t src_len, uint8_t *data_sha, uint32_t data_sha_len);

errcode_t uapi_upg_check_head_integrity(const upg_package_header_t *pkg_header);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* UPG_VERIFY_H */
