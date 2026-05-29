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
#ifndef __RKP_DEOB_H__
#define __RKP_DEOB_H__

#include "rkp_com.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    rkp_slot_choose_e rootkey_slot; /* delte in hpp rom but hrf hsl */
} rkp_deob;

typedef struct {
    uint8_t vmask_crc;
    uint8_t clear_rk_crc;
} rkp_deob_rst;
int rkp_deob_start(rkp_deob *deob, rkp_deob_rst *deob_rst, rkp_deob_type_e deob_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __drv_rkp_h__ */

