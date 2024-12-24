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
 * Description: algorithm txbf
 */

#ifndef ALG_TXBF_ROM_H
#define ALG_TXBF_ROM_H

/*****************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "dmac_tx_bss_comm_rom.h"
#include "wlan_spec_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_TXBF
/*****************************************************************************
  10 函数声明
******************************************************************************/
osal_void alg_txbf_init(osal_void);
typedef osal_u32 (*alg_txbf_init_cb)(void);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
