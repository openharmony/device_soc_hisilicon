/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */
#include "hdmi_reg_tx.h"
#include "hdmi_product_define.h"

volatile tx_hdmi_reg_regs_type *g_tx_hdmi_regs[HDMI_ID_MAX] = {TD_NULL};

int hdmi_reg_tx_hdmi_regs_init(td_u32 id, const td_char *addr)
{
    g_tx_hdmi_regs[id] = (volatile tx_hdmi_reg_regs_type *)(addr + HDMI_TX_BASE_ADDR_HDMITX);
    return TD_SUCCESS;
}

int hdmi_reg_tx_hdmi_regs_deinit(td_u32 id)
{
    if (g_tx_hdmi_regs[id] != TD_NULL) {
        g_tx_hdmi_regs[id] = TD_NULL;
    }
    return TD_SUCCESS;
}

td_void hdmi_reg_tmds_pack_mode_set(td_u32 id, td_u8 tmds_pack_mode)
{
    td_u32 *reg_addr = NULL;
    tx_pack_fifo_ctrl tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->pack_fifo_ctrl.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.tmds_pack_mode = tmds_pack_mode;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt_header_hb_set(td_u32 id, td_u8 hb0, td_u8 hb1, td_u8 hb2)
{
    td_u32 *reg_addr = NULL;
    avi_pkt_header tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_head.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_pkt_hb2 = hb2;
    tmp.bits.avi_pkt_hb1 = hb1;
    tmp.bits.avi_pkt_hb0 = hb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt0_low_set(td_u32 id, td_u8 avi_pkt0_pb0,
    td_u8 avi_pkt0_pb1, td_u8 avi_pkt0_pb2, td_u8 avi_pkt0_pb3)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt0_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt0l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_sub_pkt0_pb3 = avi_pkt0_pb3;
    tmp.bits.avi_sub_pkt0_pb2 = avi_pkt0_pb2;
    tmp.bits.avi_sub_pkt0_pb1 = avi_pkt0_pb1;
    tmp.bits.avi_sub_pkt0_pb0 = avi_pkt0_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt0_high_set(td_u32 id, td_u8 avi_pkt0_pb4, td_u8 avi_pkt0_pb5, td_u8 avi_pkt0_pb6)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt0_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt0h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_sub_pkt0_pb6 = avi_pkt0_pb6;
    tmp.bits.avi_sub_pkt0_pb5 = avi_pkt0_pb5;
    tmp.bits.avi_sub_pkt0_pb4 = avi_pkt0_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt1_low_set(td_u32 id, td_u8 avi_pkt1_pb0,
    td_u8 avi_pkt1_pb1, td_u8 avi_pkt1_pb2, td_u8 avi_pkt1_pb3)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt1_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt1l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_sub_pkt1_pb3 = avi_pkt1_pb3;
    tmp.bits.avi_sub_pkt1_pb2 = avi_pkt1_pb2;
    tmp.bits.avi_sub_pkt1_pb1 = avi_pkt1_pb1;
    tmp.bits.avi_sub_pkt1_pb0 = avi_pkt1_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt1_high_set(td_u32 id, td_u8 avi_pkt1_pb4, td_u8 avi_pkt1_pb5, td_u8 avi_pkt1_pb6)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt1_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt1h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_sub_pkt1_pb6 = avi_pkt1_pb6;
    tmp.bits.avi_sub_pkt1_pb5 = avi_pkt1_pb5;
    tmp.bits.avi_sub_pkt1_pb4 = avi_pkt1_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt2_low_set(td_u32 id, td_u8 avi_pkt2_pb0,
    td_u8 avi_pkt2_pb1, td_u8 avi_pkt2_pb2, td_u8 avi_pkt2_pb3)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt2_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt2l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_sub_pkt2_pb3 = avi_pkt2_pb3;
    tmp.bits.avi_sub_pkt2_pb2 = avi_pkt2_pb2;
    tmp.bits.avi_sub_pkt2_pb1 = avi_pkt2_pb1;
    tmp.bits.avi_sub_pkt2_pb0 = avi_pkt2_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt2_high_set(td_u32 id, td_u8 avi_pkt2_pb4, td_u8 avi_pkt2_pb5, td_u8 avi_pkt2_pb6)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt2_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt2h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_sub_pkt2_pb6 = avi_pkt2_pb6;
    tmp.bits.avi_sub_pkt2_pb5 = avi_pkt2_pb5;
    tmp.bits.avi_sub_pkt2_pb4 = avi_pkt2_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt3_low_set(td_u32 id, td_u8 avi_pkt3_pb0, td_u8 avi_pkt3_pb1,
    td_u8 avi_pkt3_pb2, td_u8 avi_pkt3_pb3)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt3_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt3l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_sub_pkt3_pb3 = avi_pkt3_pb3;
    tmp.bits.avi_sub_pkt3_pb2 = avi_pkt3_pb2;
    tmp.bits.avi_sub_pkt3_pb1 = avi_pkt3_pb1;
    tmp.bits.avi_sub_pkt3_pb0 = avi_pkt3_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt3_high_set(td_u32 id, td_u8 avi_pkt3_pb4, td_u8 avi_pkt3_pb5, td_u8 avi_pkt3_pb6)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt3_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt3h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.avi_sub_pkt3_pb6 = avi_pkt3_pb6;
    tmp.bits.avi_sub_pkt3_pb5 = avi_pkt3_pb5;
    tmp.bits.avi_sub_pkt3_pb4 = avi_pkt3_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_avi_pkt_header_hb_get(td_u32 id, avi_pkt_header *avi_header)
{
    td_u32 *reg_addr = NULL;
    avi_pkt_header tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_head.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_header->bits.avi_pkt_hb2 = tmp.bits.avi_pkt_hb2;
    avi_header->bits.avi_pkt_hb1 = tmp.bits.avi_pkt_hb1;
    avi_header->bits.avi_pkt_hb0 = tmp.bits.avi_pkt_hb0;

    return;
}

td_void hdmi_reg_avi_pkt0_low_get(td_u32 id, avi_sub_pkt0_low *avi_pkt0_low)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt0_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt0l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_pkt0_low->bits.avi_sub_pkt0_pb3 = tmp.bits.avi_sub_pkt0_pb3;
    avi_pkt0_low->bits.avi_sub_pkt0_pb2 = tmp.bits.avi_sub_pkt0_pb2;
    avi_pkt0_low->bits.avi_sub_pkt0_pb1 = tmp.bits.avi_sub_pkt0_pb1;
    avi_pkt0_low->bits.avi_sub_pkt0_pb0 = tmp.bits.avi_sub_pkt0_pb0;

    return;
}

td_void hdmi_reg_avi_pkt0_high_get(td_u32 id, avi_sub_pkt0_high *avi_pkt0_high)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt0_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt0h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_pkt0_high->bits.avi_sub_pkt0_pb6 = tmp.bits.avi_sub_pkt0_pb6;
    avi_pkt0_high->bits.avi_sub_pkt0_pb5 = tmp.bits.avi_sub_pkt0_pb5;
    avi_pkt0_high->bits.avi_sub_pkt0_pb4 = tmp.bits.avi_sub_pkt0_pb4;

    return;
}

td_void hdmi_reg_avi_pkt1_low_get(td_u32 id, avi_sub_pkt1_low *avi_pkt1_low)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt1_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt1l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_pkt1_low->bits.avi_sub_pkt1_pb3 = tmp.bits.avi_sub_pkt1_pb3;
    avi_pkt1_low->bits.avi_sub_pkt1_pb2 = tmp.bits.avi_sub_pkt1_pb2;
    avi_pkt1_low->bits.avi_sub_pkt1_pb1 = tmp.bits.avi_sub_pkt1_pb1;
    avi_pkt1_low->bits.avi_sub_pkt1_pb0 = tmp.bits.avi_sub_pkt1_pb0;

    return;
}

td_void hdmi_reg_avi_pkt1_high_get(td_u32 id, avi_sub_pkt1_high *avi_pkt1_h)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt1_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt1h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_pkt1_h->bits.avi_sub_pkt1_pb6 = tmp.bits.avi_sub_pkt1_pb6;
    avi_pkt1_h->bits.avi_sub_pkt1_pb5 = tmp.bits.avi_sub_pkt1_pb5;
    avi_pkt1_h->bits.avi_sub_pkt1_pb4 = tmp.bits.avi_sub_pkt1_pb4;

    return;
}

td_void hdmi_reg_avi_pkt2_low_get(td_u32 id, avi_sub_pkt2_low *avi_pkt2_l)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt2_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt2l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_pkt2_l->bits.avi_sub_pkt2_pb3 = tmp.bits.avi_sub_pkt2_pb3;
    avi_pkt2_l->bits.avi_sub_pkt2_pb2 = tmp.bits.avi_sub_pkt2_pb2;
    avi_pkt2_l->bits.avi_sub_pkt2_pb1 = tmp.bits.avi_sub_pkt2_pb1;
    avi_pkt2_l->bits.avi_sub_pkt2_pb0 = tmp.bits.avi_sub_pkt2_pb0;

    return;
}

void hdmi_reg_avi_pkt2_high_get(td_u32 id, avi_sub_pkt2_high *avi_pkt2_h)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt2_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt2h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_pkt2_h->bits.avi_sub_pkt2_pb6 = tmp.bits.avi_sub_pkt2_pb6;
    avi_pkt2_h->bits.avi_sub_pkt2_pb5 = tmp.bits.avi_sub_pkt2_pb5;
    avi_pkt2_h->bits.avi_sub_pkt2_pb4 = tmp.bits.avi_sub_pkt2_pb4;

    return;
}

void hdmi_reg_avi_pkt3_low_get(td_u32 id, avi_sub_pkt3_low *avi_pkt3_low)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt3_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt3l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_pkt3_low->bits.avi_sub_pkt3_pb3 = tmp.bits.avi_sub_pkt3_pb3;
    avi_pkt3_low->bits.avi_sub_pkt3_pb2 = tmp.bits.avi_sub_pkt3_pb2;
    avi_pkt3_low->bits.avi_sub_pkt3_pb1 = tmp.bits.avi_sub_pkt3_pb1;
    avi_pkt3_low->bits.avi_sub_pkt3_pb0 = tmp.bits.avi_sub_pkt3_pb0;

    return;
}

void hdmi_reg_avi_pkt3_high_get(td_u32 id, avi_sub_pkt3_high *avi_pkt3_high)
{
    td_u32 *reg_addr = NULL;
    avi_sub_pkt3_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_pkt3h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    avi_pkt3_high->bits.avi_sub_pkt3_pb6 = tmp.bits.avi_sub_pkt3_pb6;
    avi_pkt3_high->bits.avi_sub_pkt3_pb5 = tmp.bits.avi_sub_pkt3_pb5;
    avi_pkt3_high->bits.avi_sub_pkt3_pb4 = tmp.bits.avi_sub_pkt3_pb4;

    return;
}

td_void hdmi_reg_audio_pkt_header_set(td_u32 id, td_u8 hb0, td_u8 hb1, td_u8 hb2)
{
    td_u32 *reg_addr = NULL;
    aif_pkt_header tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt_head.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_pkt_hb2 = hb2;
    tmp.bits.aif_pkt_hb1 = hb1;
    tmp.bits.aif_pkt_hb0 = hb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_audio_pkt0_low_set(td_u32 id, td_u8 audio_pkt0_pb0,
    td_u8 audio_pkt0_pb1, td_u8 audio_pkt0_pb2, td_u8 audio_pkt0_pb3)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt0_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt0l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_sub_pkt0_pb3 = audio_pkt0_pb3;
    tmp.bits.aif_sub_pkt0_pb2 = audio_pkt0_pb2;
    tmp.bits.aif_sub_pkt0_pb1 = audio_pkt0_pb1;
    tmp.bits.aif_sub_pkt0_pb0 = audio_pkt0_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_audio_pkt0_high_set(td_u32 id, td_u8 audio_pkt0_pb4,
    td_u8 audio_pkt0_pb5, td_u8 audio_pkt0_pb6)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt0_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt0h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_sub_pkt0_pb6 = audio_pkt0_pb6;
    tmp.bits.aif_sub_pkt0_pb5 = audio_pkt0_pb5;
    tmp.bits.aif_sub_pkt0_pb4 = audio_pkt0_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_audio_pkt1_low_set(td_u32 id, td_u8 audio_pkt1_pb0,
    td_u8 audio_pkt1_pb1, td_u8 audio_pkt1_pb2, td_u8 audio_pkt1_pb3)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt1_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt1l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_sub_pkt1_pb3 = audio_pkt1_pb3;
    tmp.bits.aif_sub_pkt1_pb2 = audio_pkt1_pb2;
    tmp.bits.aif_sub_pkt1_pb1 = audio_pkt1_pb1;
    tmp.bits.aif_sub_pkt1_pb0 = audio_pkt1_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_audio_pkt1_high_set(td_u32 id, td_u8 audio_pkt1_pb4,
    td_u8 audio_pkt1_pb5, td_u8 audio_pkt1_pb6)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt1_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt1h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_sub_pkt1_pb6 = audio_pkt1_pb6;
    tmp.bits.aif_sub_pkt1_pb5 = audio_pkt1_pb5;
    tmp.bits.aif_sub_pkt1_pb4 = audio_pkt1_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_audio_pkt2_low_set(td_u32 id, td_u8 audio_pkt2_pb0,
    td_u8 audio_pkt2_pb1, td_u8 audio_pkt2_pb2, td_u8 audio_pkt2_pb3)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt2_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt2l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_sub_pkt2_pb3 = audio_pkt2_pb3;
    tmp.bits.aif_sub_pkt2_pb2 = audio_pkt2_pb2;
    tmp.bits.aif_sub_pkt2_pb1 = audio_pkt2_pb1;
    tmp.bits.aif_sub_pkt2_pb0 = audio_pkt2_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_audio_pkt2_high_set(td_u32 id, td_u8 audio_pkt2_pb4,
    td_u8 audio_pkt2_pb5, td_u8 audio_pkt2_pb6)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt2_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt2h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_sub_pkt2_pb6 = audio_pkt2_pb6;
    tmp.bits.aif_sub_pkt2_pb5 = audio_pkt2_pb5;
    tmp.bits.aif_sub_pkt2_pb4 = audio_pkt2_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_audio_pkt3_low_set(td_u32 id, td_u8 audio_pkt3_pb0,
    td_u8 audio_pkt3_pb1, td_u8 audio_pkt3_pb2, td_u8 audio_pkt3_pb3)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt3_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt3l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_sub_pkt3_pb3 = audio_pkt3_pb3;
    tmp.bits.aif_sub_pkt3_pb2 = audio_pkt3_pb2;
    tmp.bits.aif_sub_pkt3_pb1 = audio_pkt3_pb1;
    tmp.bits.aif_sub_pkt3_pb0 = audio_pkt3_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_audio_pkt3_high_set(td_u32 id, td_u8 audio_pkt3_pb4,
    td_u8 audio_pkt3_pb5, td_u8 audio_pkt3_pb6)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt3_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt3h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.aif_sub_pkt3_pb6 = audio_pkt3_pb6;
    tmp.bits.aif_sub_pkt3_pb5 = audio_pkt3_pb5;
    tmp.bits.aif_sub_pkt3_pb4 = audio_pkt3_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_aif_pkt_header_get(td_u32 id, aif_pkt_header *aif_header)
{
    td_u32 *reg_addr = NULL;
    aif_pkt_header tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt_head.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_header->bits.aif_pkt_hb2 = tmp.bits.aif_pkt_hb2;
    aif_header->bits.aif_pkt_hb1 = tmp.bits.aif_pkt_hb1;
    aif_header->bits.aif_pkt_hb0 = tmp.bits.aif_pkt_hb0;

    return;
}

td_void hdmi_reg_aif_pkt0_low_get(td_u32 id, aif_sub_pkt0_low *aif_pkt0_low)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt0_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt0l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_pkt0_low->bits.aif_sub_pkt0_pb3 = tmp.bits.aif_sub_pkt0_pb3;
    aif_pkt0_low->bits.aif_sub_pkt0_pb2 = tmp.bits.aif_sub_pkt0_pb2;
    aif_pkt0_low->bits.aif_sub_pkt0_pb1 = tmp.bits.aif_sub_pkt0_pb1;
    aif_pkt0_low->bits.aif_sub_pkt0_pb0 = tmp.bits.aif_sub_pkt0_pb0;

    return;
}

td_void hdmi_reg_aif_pkt0_high_get(td_u32 id, aif_sub_pkt0_high *aif_pkt0_high)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt0_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt0h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_pkt0_high->bits.aif_sub_pkt0_pb6 = tmp.bits.aif_sub_pkt0_pb6;
    aif_pkt0_high->bits.aif_sub_pkt0_pb5 = tmp.bits.aif_sub_pkt0_pb5;
    aif_pkt0_high->bits.aif_sub_pkt0_pb4 = tmp.bits.aif_sub_pkt0_pb4;

    return;
}

td_void hdmi_reg_aif_pkt1_low_get(td_u32 id, aif_sub_pkt1_low *aif_pkt1_low)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt1_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt1l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_pkt1_low->bits.aif_sub_pkt1_pb3 = tmp.bits.aif_sub_pkt1_pb3;
    aif_pkt1_low->bits.aif_sub_pkt1_pb2 = tmp.bits.aif_sub_pkt1_pb2;
    aif_pkt1_low->bits.aif_sub_pkt1_pb1 = tmp.bits.aif_sub_pkt1_pb1;
    aif_pkt1_low->bits.aif_sub_pkt1_pb0 = tmp.bits.aif_sub_pkt1_pb0;

    return;
}

td_void hdmi_reg_aif_pkt1_high_get(td_u32 id, aif_sub_pkt1_high *aif_pkt1_high)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt1_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt1h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_pkt1_high->bits.aif_sub_pkt1_pb6 = tmp.bits.aif_sub_pkt1_pb6;
    aif_pkt1_high->bits.aif_sub_pkt1_pb5 = tmp.bits.aif_sub_pkt1_pb5;
    aif_pkt1_high->bits.aif_sub_pkt1_pb4 = tmp.bits.aif_sub_pkt1_pb4;

    return;
}

td_void hdmi_reg_aif_pkt2_low_get(td_u32 id, aif_sub_pkt2_low *aif_pkt2_low)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt2_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt2l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_pkt2_low->bits.aif_sub_pkt2_pb3 = tmp.bits.aif_sub_pkt2_pb3;
    aif_pkt2_low->bits.aif_sub_pkt2_pb2 = tmp.bits.aif_sub_pkt2_pb2;
    aif_pkt2_low->bits.aif_sub_pkt2_pb1 = tmp.bits.aif_sub_pkt2_pb1;
    aif_pkt2_low->bits.aif_sub_pkt2_pb0 = tmp.bits.aif_sub_pkt2_pb0;

    return;
}

td_void hdmi_reg_aif_pkt2_high_get(td_u32 id, aif_sub_pkt2_high *aif_pkt2_high)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt2_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt2h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_pkt2_high->bits.aif_sub_pkt2_pb6 = tmp.bits.aif_sub_pkt2_pb6;
    aif_pkt2_high->bits.aif_sub_pkt2_pb5 = tmp.bits.aif_sub_pkt2_pb5;
    aif_pkt2_high->bits.aif_sub_pkt2_pb4 = tmp.bits.aif_sub_pkt2_pb4;

    return;
}

td_void hdmi_reg_aif_pkt3_low_get(td_u32 id, aif_sub_pkt3_low *aif_pkt3_low)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt3_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt3l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_pkt3_low->bits.aif_sub_pkt3_pb3 = tmp.bits.aif_sub_pkt3_pb3;
    aif_pkt3_low->bits.aif_sub_pkt3_pb2 = tmp.bits.aif_sub_pkt3_pb2;
    aif_pkt3_low->bits.aif_sub_pkt3_pb1 = tmp.bits.aif_sub_pkt3_pb1;
    aif_pkt3_low->bits.aif_sub_pkt3_pb0 = tmp.bits.aif_sub_pkt3_pb0;

    return;
}

td_void hdmi_reg_aif_pkt3_high_get(td_u32 id, aif_sub_pkt3_high *aif_pkt3_high)
{
    td_u32 *reg_addr = NULL;
    aif_sub_pkt3_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aif_pkt3h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    aif_pkt3_high->bits.aif_sub_pkt3_pb6 = tmp.bits.aif_sub_pkt3_pb6;
    aif_pkt3_high->bits.aif_sub_pkt3_pb5 = tmp.bits.aif_sub_pkt3_pb5;
    aif_pkt3_high->bits.aif_sub_pkt3_pb4 = tmp.bits.aif_sub_pkt3_pb4;

    return;
}

td_void hdmi_reg_gamut_pkt_header_get(td_u32 id, gamut_pkt_header *gamut_header)
{
    td_u32 *reg_addr = NULL;
    gamut_pkt_header tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt_head.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_header->bits.gamut_pkt_hb2 = tmp.bits.gamut_pkt_hb2;
    gamut_header->bits.gamut_pkt_hb1 = tmp.bits.gamut_pkt_hb1;
    gamut_header->bits.gamut_pkt_hb0 = tmp.bits.gamut_pkt_hb0;

    return;
}

td_void hdmi_reg_gamut_pkt0_low_get(td_u32 id, gamut_sub_pkt0_low *gamut_pkt0_low)
{
    td_u32 *reg_addr = NULL;
    gamut_sub_pkt0_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt0l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_pkt0_low->bits.gamut_sub_pkt0_pb3 = tmp.bits.gamut_sub_pkt0_pb3;
    gamut_pkt0_low->bits.gamut_sub_pkt0_pb2 = tmp.bits.gamut_sub_pkt0_pb2;
    gamut_pkt0_low->bits.gamut_sub_pkt0_pb1 = tmp.bits.gamut_sub_pkt0_pb1;
    gamut_pkt0_low->bits.gamut_sub_pkt0_pb0 = tmp.bits.gamut_sub_pkt0_pb0;

    return;
}

td_void hdmi_reg_gamut_pkt0_high_get(td_u32 id, gamut_sub_pkt0_high *gamut_pkt0_high)
{
    td_u32 *reg_addr = NULL;
    gamut_sub_pkt0_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt0h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_pkt0_high->bits.gamut_sub_pkt0_pb6 = tmp.bits.gamut_sub_pkt0_pb6;
    gamut_pkt0_high->bits.gamut_sub_pkt0_pb5 = tmp.bits.gamut_sub_pkt0_pb5;
    gamut_pkt0_high->bits.gamut_sub_pkt0_pb4 = tmp.bits.gamut_sub_pkt0_pb4;

    return;
}

td_void hdmi_reg_gamut_pkt1_low_get(td_u32 id, gamut_sub_pkt1_low *gamut_pkt1_low)
{
    td_u32 *reg_addr = NULL;
    gamut_sub_pkt1_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt1l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_pkt1_low->bits.gamut_sub_pkt1_pb3 = tmp.bits.gamut_sub_pkt1_pb3;
    gamut_pkt1_low->bits.gamut_sub_pkt1_pb2 = tmp.bits.gamut_sub_pkt1_pb2;
    gamut_pkt1_low->bits.gamut_sub_pkt1_pb1 = tmp.bits.gamut_sub_pkt1_pb1;
    gamut_pkt1_low->bits.gamut_sub_pkt1_pb0 = tmp.bits.gamut_sub_pkt1_pb0;

    return;
}

td_void hdmi_reg_gamut_pkt1_high_get(td_u32 id, gamut_sub_pkt1_high *gamut_pkt1_high)
{
    td_u32 *reg_addr = NULL;
    gamut_sub_pkt1_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt1h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_pkt1_high->bits.gamut_sub_pkt1_pb6 = tmp.bits.gamut_sub_pkt1_pb6;
    gamut_pkt1_high->bits.gamut_sub_pkt1_pb5 = tmp.bits.gamut_sub_pkt1_pb5;
    gamut_pkt1_high->bits.gamut_sub_pkt1_pb4 = tmp.bits.gamut_sub_pkt1_pb4;

    return;
}

td_void hdmi_reg_gamut_pkt2_low_get(td_u32 id, gamut_sub_pkt2_low *gamut_pkt2_low)
{
    td_u32 *reg_addr = NULL;
    gamut_sub_pkt2_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt2l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_pkt2_low->bits.gamut_sub_pkt2_pb3 = tmp.bits.gamut_sub_pkt2_pb3;
    gamut_pkt2_low->bits.gamut_sub_pkt2_pb2 = tmp.bits.gamut_sub_pkt2_pb2;
    gamut_pkt2_low->bits.gamut_sub_pkt2_pb1 = tmp.bits.gamut_sub_pkt2_pb1;
    gamut_pkt2_low->bits.gamut_sub_pkt2_pb0 = tmp.bits.gamut_sub_pkt2_pb0;

    return;
}

td_void hdmi_reg_gamut_pkt2_high_get(td_u32 id, gamut_sub_pkt2_high *gamut_pkt2_high)
{
    td_u32 *reg_addr = NULL;
    gamut_sub_pkt2_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt2h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_pkt2_high->bits.gamut_sub_pkt2_pb6 = tmp.bits.gamut_sub_pkt2_pb6;
    gamut_pkt2_high->bits.gamut_sub_pkt2_pb5 = tmp.bits.gamut_sub_pkt2_pb5;
    gamut_pkt2_high->bits.gamut_sub_pkt2_pb4 = tmp.bits.gamut_sub_pkt2_pb4;

    return;
}

td_void hdmi_reg_gamut_pkt3_low_get(td_u32 id, gamut_sub_pkt3_low *gamut_pkt3_low)
{
    td_u32 *reg_addr = NULL;
    gamut_sub_pkt3_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt3l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_pkt3_low->bits.gamut_sub_pkt3_pb3 = tmp.bits.gamut_sub_pkt3_pb3;
    gamut_pkt3_low->bits.gamut_sub_pkt3_pb2 = tmp.bits.gamut_sub_pkt3_pb2;
    gamut_pkt3_low->bits.gamut_sub_pkt3_pb1 = tmp.bits.gamut_sub_pkt3_pb1;
    gamut_pkt3_low->bits.gamut_sub_pkt3_pb0 = tmp.bits.gamut_sub_pkt3_pb0;

    return;
}

td_void hdmi_reg_gamut_pkt3_high_get(td_u32 id, gamut_sub_pkt3_high *gamut_pkt3_high)
{
    td_u32 *reg_addr = NULL;
    gamut_sub_pkt3_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_pkt3h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    gamut_pkt3_high->bits.gamut_sub_pkt3_pb6 = tmp.bits.gamut_sub_pkt3_pb6;
    gamut_pkt3_high->bits.gamut_sub_pkt3_pb5 = tmp.bits.gamut_sub_pkt3_pb5;
    gamut_pkt3_high->bits.gamut_sub_pkt3_pb4 = tmp.bits.gamut_sub_pkt3_pb4;

    return;
}

td_void hdmi_reg_vsif_header_set(td_u32 id, td_u8 hb0, td_u8 hb1, td_u8 hb2)
{
    td_u32 *reg_addr = NULL;
    vsif_pkt_header tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt_head.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_pkt_hb2 = hb2;
    tmp.bits.vsif_pkt_hb1 = hb1;
    tmp.bits.vsif_pkt_hb0 = hb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt0_low_set(td_u32 id, td_u8 vsif_pkt0_pb0,
    td_u8 vsif_pkt0_pb1, td_u8 vsif_pkt0_pb2, td_u8 vsif_pkt0_pb3)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt0_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt0l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_sub_pkt0_pb3 = vsif_pkt0_pb3;
    tmp.bits.vsif_sub_pkt0_pb2 = vsif_pkt0_pb2;
    tmp.bits.vsif_sub_pkt0_pb1 = vsif_pkt0_pb1;
    tmp.bits.vsif_sub_pkt0_pb0 = vsif_pkt0_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt0_high_set(td_u32 id, td_u8 vsif_pkt0_pb4, td_u8 vsif_pkt0_pb5, td_u8 vsif_pkt0_pb6)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt0_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt0h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_sub_pkt0_pb6 = vsif_pkt0_pb6;
    tmp.bits.vsif_sub_pkt0_pb5 = vsif_pkt0_pb5;
    tmp.bits.vsif_sub_pkt0_pb4 = vsif_pkt0_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt1_low_set(td_u32 id, td_u8 vsif_pkt1_pb0,
    td_u8 vsif_pkt1_pb1, td_u8 vsif_pkt1_pb2, td_u8 vsif_pkt1_pb3)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt1_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt1l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_sub_pkt1_pb3 = vsif_pkt1_pb3;
    tmp.bits.vsif_sub_pkt1_pb2 = vsif_pkt1_pb2;
    tmp.bits.vsif_sub_pkt1_pb1 = vsif_pkt1_pb1;
    tmp.bits.vsif_sub_pkt1_pb0 = vsif_pkt1_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt1_high_set(td_u32 id, td_u8 vsif_pkt1_pb4, td_u8 vsif_pkt1_pb5, td_u8 vsif_pkt1_pb6)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt1_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt1h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_sub_pkt1_pb6 = vsif_pkt1_pb6;
    tmp.bits.vsif_sub_pkt1_pb5 = vsif_pkt1_pb5;
    tmp.bits.vsif_sub_pkt1_pb4 = vsif_pkt1_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt2_low_set(td_u32 id, td_u8 vsif_pkt2_pb0,
    td_u8 vsif_pkt2_pb1, td_u8 vsif_pkt2_pb2, td_u8 vsif_pkt2_pb3)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt2_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt2l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_sub_pkt2_pb3 = vsif_pkt2_pb3;
    tmp.bits.vsif_sub_pkt2_pb2 = vsif_pkt2_pb2;
    tmp.bits.vsif_sub_pkt2_pb1 = vsif_pkt2_pb1;
    tmp.bits.vsif_sub_pkt2_pb0 = vsif_pkt2_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt2_high_set(td_u32 id, td_u8 vsif_pkt2_pb4, td_u8 vsif_pkt2_pb5, td_u8 vsif_pkt2_pb6)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt2_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt2h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_sub_pkt2_pb6 = vsif_pkt2_pb6;
    tmp.bits.vsif_sub_pkt2_pb5 = vsif_pkt2_pb5;
    tmp.bits.vsif_sub_pkt2_pb4 = vsif_pkt2_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt3_low_set(td_u32 id, td_u8 vsif_pkt3_pb0,
    td_u8 vsif_pkt3_pb1, td_u8 vsif_pkt3_pb2, td_u8 vsif_pkt3_pb3)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt3_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt3l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_sub_pkt3_pb3 = vsif_pkt3_pb3;
    tmp.bits.vsif_sub_pkt3_pb2 = vsif_pkt3_pb2;
    tmp.bits.vsif_sub_pkt3_pb1 = vsif_pkt3_pb1;
    tmp.bits.vsif_sub_pkt3_pb0 = vsif_pkt3_pb0;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt3_high_set(td_u32 id, td_u8 vsif_pkt3_pb4, td_u8 vsif_pkt3_pb5, td_u8 vsif_pkt3_pb6)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt3_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt3h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.vsif_sub_pkt3_pb6 = vsif_pkt3_pb6;
    tmp.bits.vsif_sub_pkt3_pb5 = vsif_pkt3_pb5;
    tmp.bits.vsif_sub_pkt3_pb4 = vsif_pkt3_pb4;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_avi_rpt_en_set(td_u32 id, td_u8 cea_avi_rpt_en)
{
    td_u32 *reg_addr = NULL;
    cea_avi_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_avi_rpt_en = cea_avi_rpt_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_avi_en_set(td_u32 id, td_u8 cea_avi_en)
{
    td_u32 *reg_addr = NULL;
    cea_avi_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_avi_en = cea_avi_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_vsif_pkt_header_get(td_u32 id, vsif_pkt_header *vsif_header)
{
    td_u32 *reg_addr = NULL;
    vsif_pkt_header tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt_head.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_header->bits.vsif_pkt_hb2 = tmp.bits.vsif_pkt_hb2;
    vsif_header->bits.vsif_pkt_hb1 = tmp.bits.vsif_pkt_hb1;
    vsif_header->bits.vsif_pkt_hb0 = tmp.bits.vsif_pkt_hb0;

    return;
}

td_void hdmi_reg_vsif_pkt0_low_get(td_u32 id, vsif_sub_pkt0_low *vsif_pkt0_low)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt0_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt0l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_pkt0_low->bits.vsif_sub_pkt0_pb3 = tmp.bits.vsif_sub_pkt0_pb3;
    vsif_pkt0_low->bits.vsif_sub_pkt0_pb2 = tmp.bits.vsif_sub_pkt0_pb2;
    vsif_pkt0_low->bits.vsif_sub_pkt0_pb1 = tmp.bits.vsif_sub_pkt0_pb1;
    vsif_pkt0_low->bits.vsif_sub_pkt0_pb0 = tmp.bits.vsif_sub_pkt0_pb0;

    return;
}

td_void hdmi_reg_vsif_pkt0_high_get(td_u32 id, vsif_sub_pkt0_high *vsif_pkt0_high)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt0_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt0h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_pkt0_high->bits.vsif_sub_pkt0_pb6 = tmp.bits.vsif_sub_pkt0_pb6;
    vsif_pkt0_high->bits.vsif_sub_pkt0_pb5 = tmp.bits.vsif_sub_pkt0_pb5;
    vsif_pkt0_high->bits.vsif_sub_pkt0_pb4 = tmp.bits.vsif_sub_pkt0_pb4;

    return;
}

td_void hdmi_reg_vsif_pkt1_low_get(td_u32 id, vsif_sub_pkt1_low *vsif_pkt1_low)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt1_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt1l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_pkt1_low->bits.vsif_sub_pkt1_pb3 = tmp.bits.vsif_sub_pkt1_pb3;
    vsif_pkt1_low->bits.vsif_sub_pkt1_pb2 = tmp.bits.vsif_sub_pkt1_pb2;
    vsif_pkt1_low->bits.vsif_sub_pkt1_pb1 = tmp.bits.vsif_sub_pkt1_pb1;
    vsif_pkt1_low->bits.vsif_sub_pkt1_pb0 = tmp.bits.vsif_sub_pkt1_pb0;

    return;
}

td_void hdmi_reg_vsif_pkt1_high_get(td_u32 id, vsif_sub_pkt1_high *vsif_pkt1_high)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt1_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt1h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_pkt1_high->bits.vsif_sub_pkt1_pb6 = tmp.bits.vsif_sub_pkt1_pb6;
    vsif_pkt1_high->bits.vsif_sub_pkt1_pb5 = tmp.bits.vsif_sub_pkt1_pb5;
    vsif_pkt1_high->bits.vsif_sub_pkt1_pb4 = tmp.bits.vsif_sub_pkt1_pb4;

    return;
}

td_void hdmi_reg_vsif_pkt2_low_get(td_u32 id, vsif_sub_pkt2_low *vsif_pkt2_low)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt2_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt2l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_pkt2_low->bits.vsif_sub_pkt2_pb3 = tmp.bits.vsif_sub_pkt2_pb3;
    vsif_pkt2_low->bits.vsif_sub_pkt2_pb2 = tmp.bits.vsif_sub_pkt2_pb2;
    vsif_pkt2_low->bits.vsif_sub_pkt2_pb1 = tmp.bits.vsif_sub_pkt2_pb1;
    vsif_pkt2_low->bits.vsif_sub_pkt2_pb0 = tmp.bits.vsif_sub_pkt2_pb0;

    return;
}

td_void hdmi_reg_vsif_pkt2_high_get(td_u32 id, vsif_sub_pkt2_high *vsif_pkt2_high)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt2_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt2h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_pkt2_high->bits.vsif_sub_pkt2_pb6 = tmp.bits.vsif_sub_pkt2_pb6;
    vsif_pkt2_high->bits.vsif_sub_pkt2_pb5 = tmp.bits.vsif_sub_pkt2_pb5;
    vsif_pkt2_high->bits.vsif_sub_pkt2_pb4 = tmp.bits.vsif_sub_pkt2_pb4;

    return;
}

td_void hdmi_reg_vsif_pkt3_low_get(td_u32 id, vsif_sub_pkt3_low *vsif_pkt3_low)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt3_low tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt3l.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_pkt3_low->bits.vsif_sub_pkt3_pb3 = tmp.bits.vsif_sub_pkt3_pb3;
    vsif_pkt3_low->bits.vsif_sub_pkt3_pb2 = tmp.bits.vsif_sub_pkt3_pb2;
    vsif_pkt3_low->bits.vsif_sub_pkt3_pb1 = tmp.bits.vsif_sub_pkt3_pb1;
    vsif_pkt3_low->bits.vsif_sub_pkt3_pb0 = tmp.bits.vsif_sub_pkt3_pb0;

    return;
}

td_void hdmi_reg_vsif_pkt3_high_get(td_u32 id, vsif_sub_pkt3_high *vsif_pkt3_high)
{
    td_u32 *reg_addr = NULL;
    vsif_sub_pkt3_high tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_pkt3h.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    vsif_pkt3_high->bits.vsif_sub_pkt3_pb6 = tmp.bits.vsif_sub_pkt3_pb6;
    vsif_pkt3_high->bits.vsif_sub_pkt3_pb5 = tmp.bits.vsif_sub_pkt3_pb5;
    vsif_pkt3_high->bits.vsif_sub_pkt3_pb4 = tmp.bits.vsif_sub_pkt3_pb4;

    return;
}

td_void hdmi_reg_cea_aud_rpt_en_set(td_u32 id, td_u8 cea_aud_rpt_en)
{
    td_u32 *reg_addr = NULL;
    cea_aud_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aud_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_aud_rpt_en = cea_aud_rpt_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_aud_en_set(td_u32 id, td_u8 cea_aud_en)
{
    td_u32 *reg_addr = NULL;
    cea_aud_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aud_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_aud_en = cea_aud_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_cp_rpt_cnt_set(td_u32 id, td_u8 cea_cp_rpt_cnt)
{
    td_u32 *reg_addr = NULL;
    cea_cp_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->cp_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_cp_rpt_cnt = cea_cp_rpt_cnt;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_cp_rpt_en_set(td_u32 id, td_u8 cea_cp_rpt_en)
{
    td_u32 *reg_addr = NULL;
    cea_cp_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->cp_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_cp_rpt_en = cea_cp_rpt_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_cp_en_set(td_u32 id, td_u8 cea_cp_en)
{
    td_u32 *reg_addr = NULL;
    cea_cp_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->cp_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_cp_en = cea_cp_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_gamut_rpt_en_set(td_u32 id, td_u8 cea_gamut_rpt_en)
{
    td_u32 *reg_addr = NULL;
    cea_gamut_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_gamut_rpt_en = cea_gamut_rpt_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_gamut_en_set(td_u32 id, td_u8 cea_gamut_en)
{
    td_u32 *reg_addr = NULL;
    cea_gamut_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_gamut_en = cea_gamut_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_vsif_rpt_en_set(td_u32 id, td_u8 cea_vsif_rpt_en)
{
    td_u32 *reg_addr = NULL;
    cea_vsif_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_vsif_rpt_en = cea_vsif_rpt_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cea_vsif_en_set(td_u32 id, td_u8 cea_vsif_en)
{
    td_u32 *reg_addr = NULL;
    cea_vsif_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cea_vsif_en = cea_vsif_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_eess_mode_en_set(td_u32 id, td_u8 eess_mode_en)
{
    td_u32 *reg_addr = NULL;
    cea_avmixer_config tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avmixer_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.eess_mode_en = eess_mode_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_hdmi_dvi_sel_set(td_u32 id, td_u8 hdmi_dvi_sel)
{
    td_u32 *reg_addr = NULL;
    cea_avmixer_config tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avmixer_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.hdmi_dvi_sel = hdmi_dvi_sel;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_dc_pkt_en_set(td_u32 id, td_u8 dc_pkt_en)
{
    td_u32 *reg_addr = NULL;
    cea_avmixer_config tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avmixer_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.dc_pkt_en = dc_pkt_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_null_pkt_en_set(td_u32 id, td_u8 null_pkt_en)
{
    td_u32 *reg_addr = NULL;
    cea_avmixer_config tmp;
    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avmixer_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.null_pkt_en = null_pkt_en;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_u8 hdmi_reg_null_pkt_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cea_avmixer_config tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avmixer_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);

    return tmp.bits.null_pkt_en;
}

td_void hdmi_reg_hdmi_mode_set(td_u32 id, td_u8 hdmi_mode)
{
    td_u32 *reg_addr = NULL;
    cea_avmixer_config tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avmixer_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.hdmi_mode = hdmi_mode;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cp_clr_avmute_set(td_u32 id, td_u8 cp_clr_avmute)
{
    td_u32 *reg_addr = NULL;
    cp_pkt_avmute tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->pkt_avmute.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cp_clr_avmute = cp_clr_avmute;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_cp_set_avmute_set(td_u32 id, td_u8 cp_set_avmute)
{
    td_u32 *reg_addr = NULL;
    cp_pkt_avmute tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->pkt_avmute.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.cp_set_avmute = cp_set_avmute;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_enc_bypass_set(td_u32 id, td_u8 enc_bypass)
{
    td_u32 *reg_addr = NULL;
    hdmi_enc_ctrl tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->enc_ctrl.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.enc_bypass = enc_bypass;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_enc_scr_on_set(td_u32 id, td_u8 enc_scr_on)
{
    td_u32 *reg_addr = NULL;
    hdmi_enc_ctrl tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->enc_ctrl.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.enc_scr_on = enc_scr_on;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_enc_hdmi2_on_set(td_u32 id, td_u8 enc_hdmi2_on)
{
    td_u32 *reg_addr = NULL;
    hdmi_enc_ctrl tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->enc_ctrl.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    tmp.bits.enc_hdmi2_on = enc_hdmi2_on;
    hdmi_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_u8 hdmi_reg_tmds_pack_mode_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    tx_pack_fifo_ctrl tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->pack_fifo_ctrl.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.tmds_pack_mode;
}

td_u8 hdmi_reg_pclk2tclk_stable_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    tx_pack_fifo_st tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->pack_fifo_status.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.pclk2tclk_stable;
}

td_u8 hdmi_reg_cea_avi_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cea_avi_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avi_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.cea_avi_en;
}

td_u8 hdmi_reg_cea_aud_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cea_aud_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->aud_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.cea_aud_en;
}

td_u8 hdmi_reg_cea_cp_rpt_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cea_cp_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->cp_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.cea_cp_rpt_en;
}

td_u8 hdmi_reg_cea_gamut_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cea_gamut_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->gamut_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.cea_gamut_en;
}

td_u8 hdmi_reg_cea_vsif_rpt_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cea_vsif_cfg tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->vsif_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.cea_vsif_rpt_en;
}

td_u8 hdmi_reg_dc_pkt_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cea_avmixer_config tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avmixer_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.dc_pkt_en;
}

td_u8 hdmi_reg_hdmi_mode_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cea_avmixer_config tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->avmixer_cfg.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.hdmi_mode;
}

td_u8 hdmi_reg_cp_set_avmute_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    cp_pkt_avmute tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->pkt_avmute.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.cp_set_avmute;
}

td_u8 hdmi_reg_enc_scr_on_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    hdmi_enc_ctrl tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->enc_ctrl.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.enc_scr_on;
}

td_u8 hdmi_reg_enc_hdmi2_on_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    hdmi_enc_ctrl tmp;

    reg_addr = (td_u32 *)&(g_tx_hdmi_regs[id]->enc_ctrl.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.enc_hdmi2_on;
}

