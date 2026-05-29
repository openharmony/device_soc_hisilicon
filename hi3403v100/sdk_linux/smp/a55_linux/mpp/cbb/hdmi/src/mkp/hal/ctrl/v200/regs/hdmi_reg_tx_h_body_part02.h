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

#ifndef HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART02_H_
#define HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART02_H_

    struct {
        unsigned int gen_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_27           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_sub_pkt0_high;

typedef union {
    struct {
        unsigned int gen_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_sub_pkt1_low;

typedef union {
    struct {
        unsigned int gen_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_28           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_sub_pkt1_high;

typedef union {
    struct {
        unsigned int gen_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_sub_pkt2_low;

typedef union {
    struct {
        unsigned int gen_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_29           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_sub_pkt2_high;

typedef union {
    struct {
        unsigned int gen_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_sub_pkt3_low;

typedef union {
    struct {
        unsigned int gen_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_30           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen_sub_pkt3_high;

typedef union {
    struct {
        unsigned int gen2_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen2_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen2_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_31       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_pkt_header;

typedef union {
    struct {
        unsigned int gen2_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen2_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_sub_pkt0_low;

typedef union {
    struct {
        unsigned int gen2_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_32            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_sub_pkt0_high;

typedef union {
    struct {
        unsigned int gen2_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen2_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_sub_pkt1_low;

typedef union {
    struct {
        unsigned int gen2_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_33            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_sub_pkt1_high;

typedef union {
    struct {
        unsigned int gen2_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen2_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_sub_pkt2_low;

typedef union {
    struct {
        unsigned int gen2_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_34            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_sub_pkt2_high;

typedef union {
    struct {
        unsigned int gen2_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen2_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_sub_pkt3_low;

typedef union {
    struct {
        unsigned int gen2_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_35            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen2_sub_pkt3_high;

typedef union {
    struct {
        unsigned int gen3_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen3_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen3_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_36       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_pkt_header;

typedef union {
    struct {
        unsigned int gen3_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen3_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_sub_pkt0_low;

typedef union {
    struct {
        unsigned int gen3_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_37            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_sub_pkt0_high;

typedef union {
    struct {
        unsigned int gen3_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen3_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_sub_pkt1_low;

typedef union {
    struct {
        unsigned int gen3_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_38            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_sub_pkt1_high;

typedef union {
    struct {
        unsigned int gen3_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen3_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_sub_pkt2_low;

typedef union {
    struct {
        unsigned int gen3_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_39            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_sub_pkt2_high;

typedef union {
    struct {
        unsigned int gen3_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen3_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_sub_pkt3_low;

typedef union {
    struct {
        unsigned int gen3_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_40            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen3_sub_pkt3_high;

typedef union {
    struct {
        unsigned int gen4_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen4_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen4_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_41       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_pkt_header;

typedef union {
    struct {
        unsigned int gen4_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen4_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_sub_pkt0_low;

typedef union {
    struct {
        unsigned int gen4_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_42            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_sub_pkt0_high;

typedef union {
    struct {
        unsigned int gen4_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen4_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_sub_pkt1_low;

typedef union {
    struct {
        unsigned int gen4_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_43            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_sub_pkt1_high;

typedef union {
    struct {
        unsigned int gen4_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen4_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_sub_pkt2_low;

typedef union {
    struct {
        unsigned int gen4_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_44            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_sub_pkt2_high;

typedef union {
    struct {
        unsigned int gen4_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen4_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_sub_pkt3_low;

typedef union {
    struct {
        unsigned int gen4_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_45            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen4_sub_pkt3_high;

typedef union {
    struct {
        unsigned int gen5_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen5_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen5_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_46       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_pkt_header;

typedef union {
    struct {
        unsigned int gen5_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen5_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_sub_pkt0_low;

typedef union {
    struct {
        unsigned int gen5_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_47            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_sub_pkt0_high;

typedef union {
    struct {
        unsigned int gen5_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen5_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_sub_pkt1_low;

typedef union {
    struct {
        unsigned int gen5_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_48            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_sub_pkt1_high;

typedef union {
    struct {
        unsigned int gen5_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen5_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_sub_pkt2_low;

typedef union {
    struct {
        unsigned int gen5_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_49            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_sub_pkt2_high;

typedef union {
    struct {
        unsigned int gen5_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen5_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_sub_pkt3_low;

typedef union {
    struct {
        unsigned int gen5_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_50            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gen5_sub_pkt3_high;

typedef union {


#endif /* HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART02_H_ */
