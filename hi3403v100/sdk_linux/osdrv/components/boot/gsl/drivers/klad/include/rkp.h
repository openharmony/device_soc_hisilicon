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
#ifndef __RKP_DEOB_EFF_RK_H__
#define __RKP_DEOB_EFF_RK_H__

#include "rkp_com.h"
#include "rkp_deob.h"
#include "rkp_kdf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    rkp_deob deob;
    rkp_kdf  kdf;
    klad_sel klad;
} rkp_deob_kdf;
int32_t rkp_get_klad_level(const int32_t module_id_0, const uint32_t check_wd);
int32_t rkp_eff_rk_start(const rkp_deob_kdf *deob_kdf, const uint32_t check_wd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __drv_rkp_h__ */

