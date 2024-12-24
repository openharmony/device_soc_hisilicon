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
 * Description: diag mem
 * This file should be changed only infrequently and with great care.
 */

#include "zdiag_mem.h"
#include "diag_adapt_layer.h"
#include "diag_dfx.h"
#include "diag_pkt.h"
#include "zdiag_debug.h"
#include "errcode.h"

errcode_t diag_pkt_need_cross_task(diag_pkt_handle_t *pkt)
{
    uint8_t *buf = NULL;

    if (pkt->single_task == false) {
        return ERRCODE_SUCC;
    }

    uint32_t copyed_size = 0;
    uint32_t total_size = diag_pkt_handle_get_total_size(pkt);
    buf = dfx_malloc(0, total_size);
    if (buf == NULL) {
        return ERRCODE_FAIL;
    }

    diag_dfx_alloc_pkt(0, total_size);

    for (unsigned i = 0; i < pkt->data_cnt; i++) {
        if (memcpy_s(buf + copyed_size, total_size - copyed_size, pkt->data[i], pkt->data_len[i]) != EOK) {
            dfx_log_debug("memcpy fail\r\n");
        }
        copyed_size += pkt->data_len[i];
    }

    diag_pkt_handle_init(pkt, 1);
    diag_pkt_handle_set_data(pkt, DIAG_PKT_DATA_ID_DATA_0, buf, (uint16_t)total_size, DIAG_PKT_DATA_DFX_MALLOC);
    return ERRCODE_SUCC;
}

void diag_pkt_free(diag_pkt_handle_t *pkt)
{
    if ((pkt->need_free) != 0) {
        dfx_assert(pkt->data_cnt == 1);
        dfx_free(0, pkt->data[0]);
        diag_dfx_free_pkt(0, pkt->data_len[0]);
        diag_pkt_handle_clean(pkt);
        return;
    }
    return;
}
