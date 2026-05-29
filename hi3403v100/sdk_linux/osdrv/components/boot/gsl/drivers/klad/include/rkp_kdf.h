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
#ifndef __RKP_KDF_H__
#define __RKP_KDF_H__

#include "rkp_com.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    rkp_slot_choose_e rootkey_slot; /* delete in hpp rom but hrf hsl */
    uint32_t cas_kdf_static; /* delete in hpp rom , fixed set xx in hrf hsl */
    uint32_t unique_type; /* fixed set 0 */
    uint32_t sw_reg; /* sw reg */
    uint32_t remap; /* remap */
} rkp_kdf;

typedef struct {
    rkp_klad_sel_e klad_sel;
    rkp_klad_type_sel_e klad_type_sel;
} klad_sel;

typedef struct {
    uint8_t sck_crc;
    uint8_t sckv_crc;
    uint8_t seedv_crc;
    uint8_t modv_crc;
    uint8_t eff_rk_crc;
} rkp_kdf_rst;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __drv_rkp_h__ */

