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
 * Description: dbac algorithm
 */

#ifndef ALG_DBAC_TXRX_ROM_H
#define ALG_DBAC_TXRX_ROM_H

#ifdef _PRE_WLAN_FEATURE_DBAC
/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "oal_types_device_rom.h"
#include "alg_main_rom.h"
#include "frw_ext_if_rom.h"
#include "alg_dbac_rom.h"
#include "mac_device_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    struct osal_list_head                  header;             /* 发送描述符队列头结点 */
    osal_u8                               element_cnt;         /* 描述符队列中元素的个数 */
} alg_dbac_txrx_queue_stru;


typedef struct {
    alg_dbac_txrx_queue_stru resv_dscr_queue[CFG_DBAC_VAP_IDX_BUTT];
} alg_dbac_txrx_stru;

alg_dbac_txrx_stru *alg_dbac_txrx_get_stru(osal_void);
osal_bool alg_dbac_dmac_is_vap_stop(osal_u8 hal_vap_id);
osal_void alg_dbac_tx_netbuf_resverve(alg_dbac_vap_idx_enum_uint8 dbac_vap_type);
osal_void alg_dbac_txrx_stru_init(osal_void);
osal_void alg_dbac_tx_netbuf_release_all(osal_void);
osal_void alg_dbac_tx_netbuf_resverve_for_vap(osal_u8 dmac_vap_id);
osal_void alg_dbac_tx_netbuf_release_for_another_vap(osal_u8 dmac_vap_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _PRE_WLAN_FEATURE_DBAC */
#endif
