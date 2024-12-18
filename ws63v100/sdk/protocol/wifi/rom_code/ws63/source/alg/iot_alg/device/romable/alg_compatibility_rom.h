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
 * Description: compatibility algorithm rom.
 */

#ifndef ALG_COMPATIBILITY_ROM_H
#define ALG_COMPATIBILITY_ROM_H

/* 1 其他头文件包含 */
#include "alg_main_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_COMPATIBILITY

#define ALG_COMPATIBILITY_LDPC_TX_TH 256 /* ldpc探测最大惩罚门限 */

/* 解决LDPC兼容性问题: AR算法按照协议能力协商, 此处默认设计为不支持; 如果LDPC编码发送成功一次则此处设计为True */
typedef struct {
    oal_bool_enum_uint8 ldpc_detect; /* 该用户是否需要ldpc探测 */
    oal_bool_enum_uint8 ldpc_disable;
    oal_bool_enum_uint8 en_supp_ldpc; /* 该用户是否支持LDPC */
    osal_u8 resv;

    osal_u16 cur_tx_total;  /* 当前累计发帧数 */
    osal_u16 detect_tx_th;  /* 探测切换发帧门限 */
} alg_ldpc_support_stru;

typedef struct {
    alg_ldpc_support_stru ldpc_det;

    osal_u16 lut_index;  /* 对应的USER lut index，避免回调指针 */
    osal_u16 resv;
} alg_compatibility_user_stru;

osal_void alg_compatibility_init(osal_void);
osal_void alg_compatibility_exit(osal_void);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
