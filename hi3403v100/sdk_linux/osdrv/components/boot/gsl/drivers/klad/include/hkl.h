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
#ifndef __HKL_H__
#define __HKL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HKL_LEVEL 3
#define HKL_KEY_LEN 16
#define HKL_KEY0_LEN 16
#define HKL_KEY1_LEN 32
#define HKL_KEY_MAX_LEN 32
typedef struct {
    uint32_t key_dec_support;
    uint32_t key_enc_support;
    unsigned int  port_sel;
    unsigned int dsc_mode;
} hkl_target_cfg;

typedef struct {
    uint32_t hpp_only;
    uint32_t dest_sec;
    uint32_t dest_nsec;
    uint32_t src_sec;
    uint32_t src_nsec;
    uint32_t key_sec;
} hkl_target_sec_cfg;

typedef enum {
    KLAD_ALG_TDES         = 0x00,
    KLAD_ALG_AES          = 0x01,
    KLAD_ALG_SM4          = 0x02,
} hkl_alg_sel_e;

typedef struct {
    uint32_t is_odd;
    unsigned int key_slot_num;
    hkl_target_cfg tgt_cfg;
    hkl_target_sec_cfg tgt_sec_cfg;
} hkl_cfg;

typedef struct {
    hkl_alg_sel_e klad_alg;
    hkl_alg_sel_e reserved; /* for 8 Align */
    uint8_t session_key[HKL_LEVEL][HKL_KEY_MAX_LEN];
    unsigned int key_size;
    hkl_cfg cfg;
} common_hkl;

int32_t hkl_com_lock();
int32_t hkl_com_unlock();
int32_t hkl_content_start(const common_hkl *com_klad, const uint32_t level,
    const uint32_t check_wd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

