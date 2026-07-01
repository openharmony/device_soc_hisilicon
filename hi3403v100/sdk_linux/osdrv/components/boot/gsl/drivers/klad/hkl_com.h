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

#ifndef __HKL_COM_H__
#define __HKL_COM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HKL_COM_MAX_LEVEL_SEL      8
#define HKL_COM_MAX_LEVEL          5

#define HKL_FP_MAX_LEVEL_SEL       2
#define HKL_FP_MAX_LEVEL           2

#define HKL_NONCE_MAX_LEVEL_SEL    1
#define HKL_NONCE_MAX_LEVEL        2

#define HKL_TA_MAX_LEVEL_SEL       4
#define HKL_TA_MAX_LEVEL           3

#define HKL_CSGK2_MAX_LEVEL_SEL    1
#define HKL_CSGK2_MAX_LEVEL        1

#define HKL_CLR_MAX_LEVEL_SEL      1
#define HKL_CLR_MAX_LEVEL          1

#define HKL_BASE                   0x10110000

#define HKL_DATA_IN_0              0x000
#define HKL_DATA_IN_1              0x004
#define HKL_DATA_IN_2              0x008
#define HKL_DATA_IN_3              0x00C
#define HKL_KEY_ADDR               0x010
#define HKL_KEY_CFG                0x014
#define HKL_KEY_SEC_CFG            0x018
#define HKL_STATE                  0x030
#define HKL_CRC                    0x034
#define HKL_ERROR                  0x038
#define HKL_KC_ERROR               0x03C
#define HKL_INT_EN                 0x040
#define HKL_INT_RAW                0x044
#define HKL_INT                    0x048
#define HKL_POWER_EN               0x0050
#define HKL_POWER_EN_LOCK          0x0054
#define HKL_CMD_STAT               0x0058
#define HKL_LOCK_CTRL              0x074
#define HKL_UNLOCK_CTRL            0x078
#define HKL_COM_LOCK_INFO          0x07C
#define HKL_COM_LOCK_STAT          0x80
#define HKL_COM_CTRL               0x84
#define HKL_CLR_CTRL               0x438
#define HKL_ALARM_INFO             0x600

typedef enum {
    HKL_STATE_BUSY               = 0b01,
    HKL_STATE_IDLE               = 0b10,
    HKL_STATE_MAX
} hkl_state_e;

typedef enum {
    HKL_LOCK_KLAD_COM            = 0,
    HKL_LOCK_KLAD_NONCE          = 1,
    HKL_LOCK_KLAD_CLR            = 2,
    HKL_LOCK_KLAD_FP             = 3,
    HKL_LOCK_KLAD_TA             = 4,
    HKL_LOCK_KLAD_CSGK2          = 5,
    HKL_LOCK_KLAD_MAX
} hkl_lock_klad_e;

typedef enum {
    HKL_LOCK_STATE_TEE           = 0xa5,
    HKL_LOCK_STATE_REE           = 0xaa,
    HKL_LOCK_STATE_MAX
} hkl_lock_state_e;

typedef enum {
    HKL_KLAD_SEL_COM             = 0x0,
    HKL_KLAD_SEL_COM0            = 0x0,
    HKL_KLAD_SEL_COM1            = 0x1,
    HKL_KLAD_SEL_COM2            = 0x2,
    HKL_KLAD_SEL_COM3            = 0x3,
    HKL_KLAD_SEL_COM4            = 0x4,
    HKL_KLAD_SEL_COM5            = 0x5,
    HKL_KLAD_SEL_COM6            = 0x6,
    HKL_KLAD_SEL_COM7            = 0x7,
    HKL_KLAD_SEL_NONCE           = 0x8,
    HKL_KLAD_SEL_CLR             = 0x9,
    HKL_KLAD_SEL_FP              = 0xa,
    HKL_KLAD_SEL_TA              = 0xb,
    HKL_KLAD_SEL_CSGK2           = 0xc,
    HKL_KLAD_SEL_MAX
} hkl_klad_sel_e;

typedef enum {
    HKL_ALG_SEL_TDES             = 0b00,
    HKL_ALG_SEL_AES              = 0b01,
    HKL_ALG_SEL_SM4              = 0b10,
    HKL_ALG_SEL_MAX
} td_hkl_alg_sel_e;

typedef struct {
    uint32_t com_kl_num;
    uint32_t com_start;
    uint32_t com_level_sel;
    td_hkl_alg_sel_e e_com_alg_sel;
    uint32_t com_ta_sel;
    uint32_t com_key_abort;
    uint32_t attr;
} hkl_com_start_attr_s;

typedef struct {
    uint32_t nonce_start;
    td_hkl_alg_sel_e e_nonce_alg_sel;
    uint32_t nonce_rk_sel;
    uint32_t attr;
} hkl_nonce_start_attr_s;

typedef struct {
    uint32_t clr_start;
    uint32_t clr_iv_sel;
    uint32_t clr_key_size;
    uint32_t attr;
} hkl_clr_start_attr_s;

typedef struct {
    uint32_t fp_start;
    uint32_t fp_level_sel;
    uint32_t fp_dec_sel;
    uint32_t fp_rk_sel;
    uint32_t attr;
} hkl_fp_start_attr_s;

typedef struct {
    uint32_t ta_start;
    uint32_t ta_level_sel;
    uint32_t last_time;
    uint32_t lut_alg_sel;
    uint32_t attr;
} hkl_ta_start_attr_s;

typedef struct {
    uint32_t csgk2start;
    uint32_t attr;
} hkl_csgk2_start_attr_s;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

