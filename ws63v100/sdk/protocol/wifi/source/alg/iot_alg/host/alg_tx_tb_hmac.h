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
 * Description: algorithm tx tb
 */

#ifndef __ALG_TX_TB_HMAC_H__
#define __ALG_TX_TB_HMAC_H__

/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "frw_ext_if.h"
#include "hmac_alg_if.h"
#include "alg_transplant.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define ALG_TX_TB_IN_TH 2
#define ALG_TX_TB_OUT_TH 0
#define ALG_TX_TB_IN_MS_TH 20
#define ALG_TX_TB_OUT_MS_TH 100
osal_void alg_tx_tb_hmac_init(osal_void);
osal_void alg_tx_tb_hmac_exit(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
