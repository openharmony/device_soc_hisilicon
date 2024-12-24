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
 * Description: NV Async Store Used Hash
 */

#ifndef NV_ASYNC_STORE_H
#define NV_ASYNC_STORE_H

#include "nv.h"
#include "nv_store.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct nv_key_map {
    uint16_t key_id;                /* Key ID */
    uint16_t key_len    : 13;       /* Key length */
    uint16_t permanent  : 1;        /* 是否是永久Key */
    uint16_t encrypted  : 1;        /* 是否是加密Key */
    uint16_t upgrade    : 1;        /* 是否可以升级 */
    uint8_t  kvalue[0];             /* key value */
} nv_key_map_t;

typedef struct {
    uint16_t key_id;
    uintptr_t key_ram_location;
} nv_queue_msg_handle_t;

errcode_t nv_async_read_with_attr(uint16_t key, uint16_t kvalue_max_length,
    uint16_t *kvalue_length, uint8_t *kvalue, nv_key_attr_t *attr);

errcode_t nv_async_write_with_attr(uint16_t key, const uint8_t *kvalue,
    uint16_t kvalue_length, nv_key_attr_t *attr, nv_storage_completed_callback func);

errcode_t nv_async_init(kv_store_t store, uint8_t num_pages);
errcode_t nv_async_flush(void);

errcode_t nv_key_map_init(kv_store_t store, uint8_t num_pages);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* NV_ASYNC_STORE_H */