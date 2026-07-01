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
#ifndef __DRV_RKP_H__
#define __DRV_RKP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    ALG_AES          = 0x00,
    ALG_SM4          = 0x01,
    ALG_TWEAKED_AES  = 0x02,
    ALG_TDES         = 0x03,
    ALG_MAX
} alg_sel_e;

typedef enum {
    RKP_COMMAND_TYPE_INTE                 = 0x0,
    RKP_COMMAND_TYPE_DEOB_INTE            = 0x1,
    RKP_COMMAND_TYPE_CRC                  = 0x2,
    RKP_COMMAND_TYPE_EFF_RK_GEN           = 0x3,
    RKP_COMMAND_TYPE_FIXED_RK_KDF         = 0x4,
    RKP_COMMAND_TYPE_DEOB                 = 0x5, /* only for PANDA */
    RKP_COMMAND_TYPE_MAX
} rkp_command_type_e;

typedef enum { /* only for panda */
    RKP_DEOB_TYPE_DOK1                    = 0x0,
    RKP_DEOB_TYPE_ECSUK2                  = 0x1,
    RKP_DEOB_TYPE_CSGK2                   = 0x2,
    RKP_DEOB_TYPE_VKL                     = 0x3,
    RKP_DEOB_TYPE_MAX
} rkp_deob_type_e;

/* Note: unchangerable order */
typedef enum {
    RKP_SLOT_CHOOSE_OEM0                 = 0x0,
    RKP_SLOT_CHOOSE_OEM1                 = 0x1,
    RKP_SLOT_CHOOSE_OEM2                 = 0x2,
    RKP_SLOT_CHOOSE_OEM3                 = 0x3,
    RKP_SLOT_CHOOSE_VENDOR             = 0x4,
    RKP_SLOT_CHOOSE_MAX
} rkp_slot_choose_e;

typedef enum {
    RKP_KLAD_SEL_SKL_KLAD0                 = 0x0,
    RKP_KLAD_SEL_SKL_KLAD1                 = 0x1,
    RKP_KLAD_SEL_HKL_KLAD0                 = 0x0,
    RKP_KLAD_SEL_HKL_KLAD1                 = 0x1,
    RKP_KLAD_SEL_HKL_KLAD2                 = 0x2,
    RKP_KLAD_SEL_HKL_KLAD3                 = 0x3,
    RKP_KLAD_SEL_HKL_KLAD4                 = 0x4,
    RKP_KLAD_SEL_HKL_KLAD5                 = 0x5,
    RKP_KLAD_SEL_HKL_KLAD6                 = 0x6,
    RKP_KLAD_SEL_HKL_KLAD7                 = 0x7,
    RKP_KLAD_SEL_HKL_TA                    = 0x8,
    RKP_KLAD_SEL_MAX
} rkp_klad_sel_e;

typedef enum {
    RKP_KLAD_TYPE_SEL_HKL                  = 0x0,
    RKP_KLAD_TYPE_SEL_SKL                  = 0x1,
    RKP_KLAD_TYPE_SEL_MAX
} rkp_klad_type_sel_e;

typedef enum {
    RKP_CAS_KDF_STATIC_REE              = 0x0,
    RKP_CAS_KDF_STATIC_TEE                 = 0x1,
    RKP_CAS_KDF_STATIC_MAX
} rkp_cas_kdf_static_sel_e;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __drv_rkp_h__ */

