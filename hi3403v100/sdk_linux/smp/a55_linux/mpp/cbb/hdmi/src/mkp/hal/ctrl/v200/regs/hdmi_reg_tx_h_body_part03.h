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

#ifndef HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART03_H_
#define HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART03_H_

    struct {
        unsigned int gamut_pkt_hb0 : 8; /* [7:0] */
        unsigned int gamut_pkt_hb1 : 8; /* [15:8] */
        unsigned int gamut_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_51        : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_pkt_header;

typedef union {
    struct {
        unsigned int gamut_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gamut_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_sub_pkt0_low;

typedef union {
    struct {
        unsigned int gamut_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_52             : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_sub_pkt0_high;

typedef union {
    struct {
        unsigned int gamut_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gamut_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_sub_pkt1_low;

typedef union {
    struct {
        unsigned int gamut_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_53             : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_sub_pkt1_high;

typedef union {
    struct {
        unsigned int gamut_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gamut_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_sub_pkt2_low;

typedef union {
    struct {
        unsigned int gamut_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_54             : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_sub_pkt2_high;

typedef union {
    struct {
        unsigned int gamut_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gamut_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_sub_pkt3_low;

typedef union {
    struct {
        unsigned int gamut_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_55             : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} gamut_sub_pkt3_high;

typedef union {
    struct {
        unsigned int vsif_pkt_hb0 : 8; /* [7:0] */
        unsigned int vsif_pkt_hb1 : 8; /* [15:8] */
        unsigned int vsif_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_56       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_pkt_header;

typedef union {
    struct {
        unsigned int vsif_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int vsif_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_sub_pkt0_low;

typedef union {
    struct {
        unsigned int vsif_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_57            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_sub_pkt0_high;

typedef union {
    struct {
        unsigned int vsif_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int vsif_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_sub_pkt1_low;

typedef union {
    struct {
        unsigned int vsif_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_58            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_sub_pkt1_high;

typedef union {
    struct {
        unsigned int vsif_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int vsif_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_sub_pkt2_low;

typedef union {
    struct {
        unsigned int vsif_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_59            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_sub_pkt2_high;

typedef union {
    struct {
        unsigned int vsif_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int vsif_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_sub_pkt3_low;

typedef union {
    struct {
        unsigned int vsif_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_60            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} vsif_sub_pkt3_high;

typedef union {
    struct {
        unsigned int cea_avi_en     : 1;  /* [0] */
        unsigned int cea_avi_rpt_en : 1;  /* [1] */
        unsigned int rsv_61         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_avi_cfg;

typedef union {
    struct {
        unsigned int cea_spf_en     : 1;  /* [0] */
        unsigned int cea_spf_rpt_en : 1;  /* [1] */
        unsigned int rsv_62         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_spf_cfg;

typedef union {
    struct {
        unsigned int cea_aud_en     : 1;  /* [0] */
        unsigned int cea_aud_rpt_en : 1;  /* [1] */
        unsigned int rsv_63         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_aud_cfg;

typedef union {
    struct {
        unsigned int cea_mpeg_en     : 1;  /* [0] */
        unsigned int cea_mpeg_rpt_en : 1;  /* [1] */
        unsigned int rsv_64          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_mpeg_cfg;

typedef union {
    struct {
        unsigned int cea_gen_en     : 1;  /* [0] */
        unsigned int cea_gen_rpt_en : 1;  /* [1] */
        unsigned int rsv_65         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_gen_cfg;

typedef union {
    struct {
        unsigned int cea_cp_en      : 1;  /* [0] */
        unsigned int cea_cp_rpt_en  : 1;  /* [1] */
        unsigned int cea_cp_rpt_cnt : 8;  /* [2:7] */
        unsigned int rsv_66         : 22; /* [31:8] */
    } bits;
    unsigned int u32;
} cea_cp_cfg;

typedef union {
    struct {
        unsigned int cea_gen2_en     : 1;  /* [0] */
        unsigned int cea_gen2_rpt_en : 1;  /* [1] */
        unsigned int rsv_67          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_gen2_cfg;

typedef union {
    struct {
        unsigned int cea_gen3_en     : 1;  /* [0] */
        unsigned int cea_gen3_rpt_en : 1;  /* [1] */
        unsigned int rsv_68          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_gen3_cfg;

typedef union {
    struct {
        unsigned int cea_gen4_en     : 1;  /* [0] */
        unsigned int cea_gen4_rpt_en : 1;  /* [1] */
        unsigned int rsv_69          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_gen4_cfg;

typedef union {
    struct {
        unsigned int cea_gen5_en     : 1;  /* [0] */
        unsigned int cea_gen5_rpt_en : 1;  /* [1] */
        unsigned int rsv_70          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_gen5_cfg;

typedef union {
    struct {
        unsigned int cea_gamut_en     : 1;  /* [0] */
        unsigned int cea_gamut_rpt_en : 1;  /* [1] */
        unsigned int rsv_71           : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_gamut_cfg;

typedef union {
    struct {
        unsigned int cea_vsif_en     : 1;  /* [0] */
        unsigned int cea_vsif_rpt_en : 1;  /* [1] */
        unsigned int rsv_72          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cea_vsif_cfg;

typedef union {
    struct {
        unsigned int cea_avi_state   : 1;  /* [0] */
        unsigned int cea_aud_state   : 1;  /* [1] */
        unsigned int cea_cp_state    : 1;  /* [2] */
        unsigned int cea_gen_state   : 1;  /* [3] */
        unsigned int cea_gen2_state  : 1;  /* [4] */
        unsigned int cea_gen3_state  : 1;  /* [5] */
        unsigned int cea_gen4_state  : 1;  /* [6] */
        unsigned int cea_gen5_state  : 1;  /* [7] */
        unsigned int cea_spd_state   : 1;  /* [8] */
        unsigned int cea_mpeg_state  : 1;  /* [9] */
        unsigned int cea_gamut_state : 1;  /* [10] */
        unsigned int cea_vsif_state  : 1;  /* [11] */
        unsigned int rsv_73          : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} cea_pktf_state;

typedef union {
    struct {
        unsigned int hdmi_mode           : 1;  /* [0] */
        unsigned int dc_pkt_en           : 1;  /* [1] */
        unsigned int null_pkt_en         : 1;  /* [2] */
        unsigned int null_pkt_en_vs_high : 1;  /* [3] */
        unsigned int intr_encryption     : 1;  /* [4] */
        unsigned int ovr_dc_pkt_en       : 1;  /* [5] */
        unsigned int priotity_ctl        : 1;  /* [6] */
        unsigned int pkt_bypass_mode     : 1;  /* [7] */
        unsigned int avmute_in_phase     : 1;  /* [8] */
        unsigned int hdmi_dvi_sel        : 1;  /* [9] */
        unsigned int eess_mode_en        : 1;  /* [10] */
        unsigned int rsv_74              : 21; /* [31:11] */
    } bits;
    unsigned int u32;
} cea_avmixer_config;

typedef union {
    struct {
        unsigned int cp_set_avmute : 1;  /* [0] */
        unsigned int cp_clr_avmute : 1;  /* [1] */
        unsigned int rsv_75        : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} cp_pkt_avmute;

typedef union {
    struct {
        unsigned int video_blank : 24; /* [23:0] */
        unsigned int rsv_76      : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} video_blank_cfg;

typedef union {
    struct {
        unsigned int reg_tbist_en         : 1;  /* [0] */
        unsigned int reg_tbist_syn_pol    : 2;  /* [2:1] */
        unsigned int reg_tbist_timing_sel : 6;  /* [8:3] */
        unsigned int reg_tbist_patt_sel   : 5;  /* [13:9] */
        unsigned int rsv_77               : 18; /* [31:14] */
    } bits;
    unsigned int u32;
} tmds_bist_ctrl;

typedef union {
    struct {
        unsigned int enc_hdmi2_on   : 1;  /* [0] */
        unsigned int enc_scr_on     : 1;  /* [1] */
        unsigned int enc_scr_md     : 1;  /* [2] */
        unsigned int enc_hdmi_val   : 1;  /* [3] */
        unsigned int enc_hdmi_ovr   : 1;  /* [4] */
        unsigned int enc_bypass     : 1;  /* [5] */
        unsigned int enc_ck_div_sel : 2;  /* [7:6] */
        unsigned int rsv_78         : 24; /* [31:8] */
    } bits;
    unsigned int u32;
} hdmi_enc_ctrl;

typedef union {
    struct {
        unsigned int enc_ck_sharp0 : 10; /* [9:0] */
        unsigned int enc_ck_sharp1 : 10; /* [19:10] */
        unsigned int enc_ck_sharp2 : 10; /* [29:20] */
        unsigned int rsv_79        : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} enc_ck_sharp;

typedef struct {
    volatile tx_pack_fifo_ctrl    pack_fifo_ctrl;   /* 1800 */
    volatile tx_pack_fifo_st      pack_fifo_status; /* 1804 */
    volatile pclk_refer_cnt       pclk_ref_cnt;     /* 1808 */
    volatile tclk_lower_threshold lower_threshold;  /* 180C */
    volatile tclk_upper_threshold upper_threshold;  /* 1810 */
    unsigned int                  reserved_0[1];    /* 1814 */
    volatile avi_pkt_header       avi_head;         /* 1818 */
    volatile avi_sub_pkt0_low     avi_pkt0l;        /* 181C */
    volatile avi_sub_pkt0_high    avi_pkt0h;        /* 1820 */
    volatile avi_sub_pkt1_low     avi_pkt1l;        /* 1824 */
    volatile avi_sub_pkt1_high    avi_pkt1h;        /* 1828 */
    volatile avi_sub_pkt1_low     avi_pkt2l;        /* 182C */
    volatile avi_sub_pkt2_high    avi_pkt2h;        /* 1830 */
    volatile avi_sub_pkt3_low     avi_pkt3l;        /* 1834 */
    volatile avi_sub_pkt3_high    avi_pkt3h;        /* 1838 */
    volatile aif_pkt_header       aif_pkt_head;     /* 183C */
    volatile aif_sub_pkt0_low     aif_pkt0l;        /* 1840 */
    volatile aif_sub_pkt0_high    aif_pkt0h;        /* 1844 */
    volatile aif_sub_pkt1_low     aif_pkt1l;        /* 1848 */
    volatile aif_sub_pkt1_high    aif_pkt1h;        /* 184C */
    volatile aif_sub_pkt2_low     aif_pkt2l;        /* 1850 */
    volatile aif_sub_pkt2_high    aif_pkt2h;        /* 1854 */
    volatile aif_sub_pkt3_low     aif_pkt3l;        /* 1858 */
    volatile aif_sub_pkt3_high    aif_pkt3h;        /* 185C */
    volatile spif_pkt_header      spif_pkt_head;    /* 1860 */
    volatile spif_sub_pkt0_low    spif_pkt0l;       /* 1864 */
    volatile spif_sub_pkt0_high   spif_pkt0h;       /* 1868 */
    volatile spif_sub_pkt1_low    spif_pkt1l;       /* 186C */
    volatile spif_sub_pkt1_high   spif_pkt1h;       /* 1870 */
    volatile spif_sub_pkt2_low    spif_pkt2l;       /* 1874 */
    volatile spif_sub_pkt2_high   spif_pkt2h;       /* 1878 */
    volatile spif_sub_pkt3_low    spif_pkt3l;       /* 187C */
    volatile spif_sub_pkt3_high   spif_pkt3h;       /* 1880 */
    volatile speg_pkt_header      mpeg_pkt_head;    /* 1884 */
    volatile speg_sub_pkt0_low    mpeg_pkt0l;       /* 1888 */
    volatile speg_sub_pkt0_high   mpeg_pkt0h;       /* 188C */
    volatile speg_sub_pkt1_low    mpeg_pkt1l;       /* 1890 */
    volatile mpeg_sub_pkt1_high   mpeg_pkt1h;       /* 1894 */
    volatile mpeg_sub_pkt2_low    mpeg_pkt2l;       /* 1898 */
    volatile mpeg_sub_pkt2_high   mpeg_pkt2h;       /* 189C */
    volatile mpeg_sub_pkt3_low    mpeg_pkt3l;       /* 18A0 */
    volatile mpeg_sub_pkt3_high   mpeg_pkt3h;       /* 18A4 */
    volatile gen_pkt_header       gen_pkt_head;     /* 18A8 */
    volatile gen_sub_pkt0_low     gen_pkt0l;        /* 18AC */
    volatile gen_sub_pkt0_high    gen_pkt0h;        /* 18B0 */
    volatile gen_sub_pkt1_low     gen_pkt1l;        /* 18B4 */
    volatile gen_sub_pkt1_high    gen_pkt1h;        /* 18B8 */
    volatile gen_sub_pkt2_low     gen_pkt2l;        /* 18BC */
    volatile gen_sub_pkt2_high    gen_pkt2h;        /* 18C0 */
    volatile gen_sub_pkt3_low     gen_pkt3l;        /* 18C4 */
    volatile gen_sub_pkt3_high    gen_pkt3h;        /* 18C8 */
    volatile gen2_pkt_header      gen2_pkt_head;    /* 18CC */


#endif /* HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART03_H_ */
