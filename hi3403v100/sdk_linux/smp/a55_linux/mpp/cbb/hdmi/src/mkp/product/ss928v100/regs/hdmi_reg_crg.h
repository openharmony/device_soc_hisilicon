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

#ifndef HDMI_REG_CRG_H
#define HDMI_REG_CRG_H

#include "ot_type.h"

typedef union {
    struct {
        unsigned int rsv_0                    : 2;  /* [1..0] */
        unsigned int hdmitx_ctrl_osc_24m_cken : 1;  /* [2] */
        unsigned int hdmitx_ctrl_cec_cken     : 1;  /* [3] */
        unsigned int hdmitx_ctrl_os_cken      : 1;  /* [4] */
        unsigned int hdmitx_ctrl_as_cken      : 1;  /* [5] */
        unsigned int hdmitx_ctrl_bus_srst_req : 1;  /* [6] */
        unsigned int hdmitx_ctrl_srst_req     : 1;  /* [7] */
        unsigned int hdmitx_ctrl_cec_srst_req : 1;  /* [8] */
        unsigned int rsv_1                    : 23; /* [31..9] */
    } bits;
    unsigned int u32;
} peri_crg8144;

typedef union {
    struct {
        unsigned int hdmitx_phy_tmds_cken    : 1;  /* [0] */
        unsigned int hdmitx_phy_modclk_cken  : 1;  /* [1] */
        unsigned int ac_ctrl_modclk_cken     : 1;  /* [2] */
        unsigned int rsv_0                   : 1;  /* [3] */
        unsigned int hdmitx_phy_srst_req     : 1;  /* [4] */
        unsigned int hdmitx_phy_bus_srst_req : 1;  /* [5] */
        unsigned int ac_ctrl_srst_req        : 1;  /* [6] */
        unsigned int ac_ctrl_bus_srst_req    : 1;  /* [7] */
        unsigned int hdmitx_phy_clk_pctrl    : 1;  /* [8] */
        unsigned int rsv_1                   : 23; /* [31..9] */
    } bits;
    unsigned int u32;
} peri_crg8152;

typedef struct {
    volatile peri_crg8144 crg8144; /* 0x7F40 */
    volatile unsigned int rsv[7];  /* 0x7F44~0x0x7F5C */
    volatile peri_crg8152 crg8152; /* 0x7F60 */
} hdmi_reg_crg;

int hdmi_reg_crg_init(td_void);
int hdmi_reg_crg_deinit(td_void);
td_void hdmi_reg_ctrl_osc_24m_cken_set(td_u8 hdmitx_ctrl_osc_24m_cken);
td_void hdmi_reg_ctrl_cec_cken_set(td_u8 hdmitx_ctrl_cec_cken);
td_void hdmi_reg_ctrl_os_cken_set(td_u8 hdmitx_ctrl_os_cken);
td_void hdmi_reg_ctrl_as_cken_set(td_u8 hdmitx_ctrl_as_cken);
td_void hdmi_reg_ctrl_bus_srst_req_set(td_u8 hdmitx_ctrl_bus_srst_req);
td_void hdmi_reg_ctrl_srst_req_set(td_u8 hdmitx_ctrl_srst_req);
td_void hdmi_reg_ctrl_cec_srst_req_set(td_u8 hdmitx_ctrl_cec_srst_req);
td_void hdmi_reg_hdmitx_phy_tmds_cken_set(td_u8 phy_tmds_cken);
td_void hdmi_reg_hdmitx_phy_modclk_cken_set(td_u8 phy_modclk_cken);
td_void hdmi_reg_ac_ctrl_modclk_cken_set(td_u8 ac_ctrl_modclk_cken);
td_void hdmi_reg_phy_srst_req_set(td_u8 hdmitx_phy_srst_req);
td_u8 hdmi_reg_phy_srst_req_get(td_void);
td_void hdmi_reg_phy_bus_srst_req_set(td_u8 hdmitx_phy_bus_srst_req);
td_void hdmi_reg_ac_ctrl_srst_req_set(td_u8 ac_ctrl_srst_req);
td_void hdmi_reg_ac_ctrl_bus_srst_req_set(td_u8 ac_ctrl_bus_srst_req);
td_void hdmi_reg_phy_clk_pctrl_set(td_u8 hdmitx_phy_clk_pctrl);
#endif

