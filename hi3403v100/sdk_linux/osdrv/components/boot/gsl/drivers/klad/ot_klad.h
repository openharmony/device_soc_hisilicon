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

#ifndef __HPP_ROM_H__
#define __HPP_ROM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define KLAD_KEY_LEN      0x10
#define KLAD_KEY0_LEN      0x10
#define KLAD_KEY1_LEN      0x20

typedef enum {
    HPP_ALG_AES          = 0x00,
    HPP_ALG_SM4          = 0x01,
} hpp_alg_sel;

typedef struct {
    uint32_t is_odd;
    uint32_t key_slot_num;
} hpp_klad_cfg;

typedef struct {
    hpp_alg_sel alg;
    uint8_t session_key0[KLAD_KEY0_LEN];
    uint8_t session_key1[KLAD_KEY1_LEN];
    hpp_klad_cfg cfg;
} hpp_klad;

typedef enum {
    KLAD_BOOT               = 0x0,
    KLAD_ATF                   = 0x1,
    KLAD_TEE                   = 0x2,
} hpp_keyladder_sel;

int32_t hpp_klad_process(const hpp_keyladder_sel keyladder, const hpp_klad *klad);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

