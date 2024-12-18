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
 * Description: Header file
 */

#ifndef MSG_DFT_ROM_H
#define MSG_DFT_ROM_H

#include "osal_types.h"

#define DFT_VAP_AGGRE_REPORT_NUM 20
typedef struct {
    osal_u8   mpdu_num[DFT_VAP_AGGRE_REPORT_NUM];
    osal_u8   msdu_num[DFT_VAP_AGGRE_REPORT_NUM];
} wlan_tx_aggregation_stats_stru;

typedef struct {
    osal_u8 enable;
    osal_u8 vap_id;
    osal_u8 rsv[2];
} dft_vap_statis_command_stru;

typedef struct {
    osal_u32 rx_from_hw_mpdu_cnt;            /* DMAC收到hw发送的mpdu报文个数 */
    osal_u32 tx_from_dmac_mpdu_cnt;          /* DMAC挂硬件队列发送mpdu报文个数 */
} dft_device_statis_cnt_stru;
#endif