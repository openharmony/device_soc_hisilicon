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

#ifndef HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART01_H_
#define HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART01_H_


#include "ot_type.h"

typedef union {
    struct {
        unsigned int tmds_pack_mode       : 2;  /* [1:0] */
        unsigned int reg_fifo_auto_rst_en : 1;  /* [2] */
        unsigned int reg_fifo_manu_rst    : 1;  /* [3] */
        unsigned int reg_clock_det_en     : 1;  /* [4] */
        unsigned int reg_ext_tmds_para    : 1;  /* [5] */
        unsigned int rsv_0                : 2;  /* [7:6] */
        unsigned int reg_fifo_delay_cnt   : 8;  /* [15:8] */
        unsigned int rsv_1                : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} tx_pack_fifo_ctrl;

typedef union {
    struct {
        unsigned int pclk2tclk_stable : 1;  /* [0] */
        unsigned int rsv_2            : 31; /* [31:1] */
    } bits;
    unsigned int u32;
} tx_pack_fifo_st;

typedef union {
    struct {
        unsigned int reg_pclk_refer_cnt : 18; /* [17:0] */
        unsigned int rsv_3              : 14; /* [31:18] */
    } bits;
    unsigned int u32;
} pclk_refer_cnt;

typedef union {
    struct {
        unsigned int reg_tcnt_lower_threshold : 18; /* [17:0] */
        unsigned int rsv_4                    : 14; /* [31:18] */
    } bits;
    unsigned int u32;
} tclk_lower_threshold;

typedef union {
    struct {
        unsigned int reg_tcnt_upper_threshold : 18; /* [17:0] */
        unsigned int rsv_5                    : 14; /* [31:18] */
    } bits;
    unsigned int u32;
} tclk_upper_threshold;

typedef union {
    struct {
        unsigned int avi_pkt_hb0 : 8; /* [7:0] */
        unsigned int avi_pkt_hb1 : 8; /* [15:8] */
        unsigned int avi_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_6       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_pkt_header;

typedef union {
    struct {
        unsigned int avi_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int avi_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int avi_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int avi_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_sub_pkt0_low;

typedef union {
    struct {
        unsigned int avi_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int avi_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int avi_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_7            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_sub_pkt0_high;

typedef union {
    struct {
        unsigned int avi_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int avi_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int avi_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int avi_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_sub_pkt1_low;

typedef union {
    struct {
        unsigned int avi_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int avi_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int avi_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_8            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_sub_pkt1_high;

typedef union {
    struct {
        unsigned int avi_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int avi_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int avi_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int avi_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_sub_pkt2_low;

typedef union {
    struct {
        unsigned int avi_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int avi_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int avi_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_9            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_sub_pkt2_high;

typedef union {
    struct {
        unsigned int avi_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int avi_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int avi_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int avi_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_sub_pkt3_low;

typedef union {
    struct {
        unsigned int avi_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int avi_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int avi_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_10           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} avi_sub_pkt3_high;

typedef union {
    struct {
        unsigned int aif_pkt_hb0 : 8; /* [7:0] */
        unsigned int aif_pkt_hb1 : 8; /* [15:8] */
        unsigned int aif_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_11      : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_pkt_header;

typedef union {
    struct {
        unsigned int aif_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int aif_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int aif_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int aif_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_sub_pkt0_low;

typedef union {
    struct {
        unsigned int aif_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int aif_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int aif_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_12           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_sub_pkt0_high;

typedef union {
    struct {
        unsigned int aif_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int aif_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int aif_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int aif_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_sub_pkt1_low;

typedef union {
    struct {
        unsigned int aif_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int aif_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int aif_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_13           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_sub_pkt1_high;

typedef union {
    struct {
        unsigned int aif_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int aif_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int aif_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int aif_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_sub_pkt2_low;

typedef union {
    struct {
        unsigned int aif_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int aif_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int aif_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_14           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_sub_pkt2_high;

typedef union {
    struct {
        unsigned int aif_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int aif_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int aif_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int aif_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_sub_pkt3_low;

typedef union {
    struct {
        unsigned int aif_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int aif_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int aif_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_15           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} aif_sub_pkt3_high;

typedef union {
    struct {
        unsigned int spd_pkt_hb0 : 8; /* [7:0] */
        unsigned int spd_pkt_hb1 : 8; /* [15:8] */
        unsigned int spd_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_16      : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_pkt_header;

typedef union {
    struct {
        unsigned int spd_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int spd_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int spd_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int spd_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_sub_pkt0_low;

typedef union {
    struct {
        unsigned int spd_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int spd_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int spd_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_17           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_sub_pkt0_high;

typedef union {
    struct {
        unsigned int spd_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int spd_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int spd_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int spd_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_sub_pkt1_low;

typedef union {
    struct {
        unsigned int spd_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int spd_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int spd_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_18           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_sub_pkt1_high;

typedef union {
    struct {
        unsigned int spd_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int spd_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int spd_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int spd_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_sub_pkt2_low;

typedef union {
    struct {
        unsigned int spd_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int spd_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int spd_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_19           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_sub_pkt2_high;

typedef union {
    struct {
        unsigned int spd_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int spd_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int spd_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int spd_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_sub_pkt3_low;

typedef union {
    struct {
        unsigned int spd_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int spd_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int spd_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_20           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} spif_sub_pkt3_high;

typedef union {
    struct {
        unsigned int mpeg_pkt_hb0 : 8; /* [7:0] */
        unsigned int mpeg_pkt_hb1 : 8; /* [15:8] */
        unsigned int mpeg_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_21       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} speg_pkt_header;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int mpeg_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} speg_sub_pkt0_low;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_22            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} speg_sub_pkt0_high;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int mpeg_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} speg_sub_pkt1_low;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_23            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} mpeg_sub_pkt1_high;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int mpeg_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} mpeg_sub_pkt2_low;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_24            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} mpeg_sub_pkt2_high;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int mpeg_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} mpeg_sub_pkt3_low;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_25            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} mpeg_sub_pkt3_high;

typedef union {
    struct {
        unsigned int gen_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_26      : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_pkt_header;

typedef union {
    struct {
        unsigned int gen_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_sub_pkt0_low;

typedef union {


#endif /* HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART01_H_ */
