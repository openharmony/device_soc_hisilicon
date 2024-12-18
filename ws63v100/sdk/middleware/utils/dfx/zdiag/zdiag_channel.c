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
 * Description: diag channel
 * This file should be changed only infrequently and with great care.
 */
#include "diag_channel_item.h"
#include "securec.h"
#include "zdiag_mem.h"
#include "diag_config.h"
#include "diag_adapt_layer.h"
#include "errcode.h"

typedef struct {
    diag_channel_item_t item[DIAG_SUPPORT_CHANNEL_CNT];
} diag_channel_ctrl_t;

diag_channel_ctrl_t g_diag_channel_ctrl = {
    .item[0].init = 0,
};

STATIC diag_channel_ctrl_t *diag_get_channel_ctrl(void)
{
    return &g_diag_channel_ctrl;
}

diag_channel_item_t *diag_chan_idx_2_item(diag_channel_id_t id)
{
    diag_channel_ctrl_t *chan_ctrl = diag_get_channel_ctrl();
    return &chan_ctrl->item[id];
}

errcode_t soc_diag_channel_set_notify_hook(diag_channel_id_t id, diag_channel_notify_hook hook)
{
    diag_channel_item_t *item = diag_chan_idx_2_item(id);
    if (item == NULL) {
        return ERRCODE_FAIL;
    }

    item->notify_hook = hook;
    return ERRCODE_SUCC;
}

errcode_t soc_diag_channel_set_tx_hook(diag_channel_id_t id, diag_channel_tx_hook hook)
{
    diag_channel_item_t *item = diag_chan_idx_2_item(id);
    if (item == NULL) {
        return ERRCODE_FAIL;
    }

    item->tx_hook = hook;
    return ERRCODE_SUCC;
}

errcode_t soc_diag_channel_set_connect_hso_addr(diag_channel_id_t id, uint8_t hso_addr)
{
    diag_channel_item_t *item = diag_chan_idx_2_item(id);
    if (item == NULL) {
        return ERRCODE_FAIL;
    }
    item->hso_addr = hso_addr;
    return ERRCODE_SUCC;
}

diag_channel_item_t *zdiag_dst_2_chan(uint8_t addr)
{
    diag_channel_id_t channel_id = diag_adapt_dst_2_channel_id(addr);
    diag_channel_item_t *item = diag_chan_idx_2_item(channel_id);
    return item;
}

errcode_t soc_diag_channel_init(diag_channel_id_t id, uint32_t attribute)
{
    diag_channel_item_t *item = diag_chan_idx_2_item(id);
    if (item == NULL || item->init == true) {
        return ERRCODE_FAIL;
    }

    if ((attribute & SOC_DIAG_CHANNEL_ATTR_NEED_RX_BUF) != 0) {
        item->rx_buf_len = CONFIG_DIAG_RX_BUF_SIZE;
        item->rx_buf_pos = 0;
        item->rx_buf_is_using = false;
        item->rx_buf = dfx_malloc(0, item->rx_buf_len);
        if (item->rx_buf == NULL) {
            return ERRCODE_FAIL;
        }
    }
    return ERRCODE_SUCC;
}
