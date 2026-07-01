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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART02_H_

    struct {
        unsigned int clklane_lp2hs_time : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int clklane_hs2lp_time : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_clklane_time;

/* define the union reg_datalane_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int datalane_lp2hs_time : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;           /* [15..10]  */
        unsigned int datalane_hs2lp_time : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;           /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_datalane_time;

/* define the union reg_phy_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int shutdownz : 1;   /* [0]  */
        unsigned int rstz : 1;        /* [1]  */
        unsigned int enableclk : 1;   /* [2]  */
        unsigned int forcepll : 1;    /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_ctrl;

/* define the union reg_lane_num */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lane_num : 2;       /* [1..0]  */
        unsigned int reserved_0 : 6;     /* [7..2]  */
        unsigned int stopstate_time : 8; /* [15..8]  */
        unsigned int reserved_1 : 16;    /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lane_num;

/* define the union reg_ulps_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int txulpsclklane : 1;      /* [0]  */
        unsigned int txulpsexitclklane : 1;  /* [1]  */
        unsigned int txulpsdatalane : 1;     /* [2]  */
        unsigned int txulpsexitdatalane : 1; /* [3]  */
        unsigned int reserved_0 : 28;        /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ulps_ctrl;

/* define the union reg_tx_triggers */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tx_triggers : 4; /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_tx_triggers;

/* define the union reg_ppi_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int phy_pll_lock : 1;       /* [0]  */
        unsigned int phy_tran_direction : 1; /* [1]  */
        unsigned int stopstateclklane : 1;   /* [2]  */
        unsigned int ulpsactivenotclk : 1;   /* [3]  */
        unsigned int stopstate0lane : 1;     /* [4]  */
        unsigned int ulpsactivenot0lane : 1; /* [5]  */
        unsigned int rxulpsesc0lane : 1;     /* [6]  */
        unsigned int stopstate1lane : 1;     /* [7]  */
        unsigned int ulpsactivenot1lane : 1; /* [8]  */
        unsigned int stopstate2lane : 1;     /* [9]  */
        unsigned int ulpsactivenot2lane : 1; /* [10]  */
        unsigned int stopstate3lane : 1;     /* [11]  */
        unsigned int ulpsactivenot3lane : 1; /* [12]  */
        unsigned int reserved_0 : 19;        /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_status;

/* define the union reg_phy_reg_cfg0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int testclr : 1;     /* [0]  */
        unsigned int testclk : 1;     /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_reg_cfg0;

/* define the union reg_phy_reg_cfg1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int testdin : 8;     /* [7..0]  */
        unsigned int testdout : 8;    /* [15..8]  */
        unsigned int testen : 1;      /* [16]  */
        unsigned int reserved_0 : 15; /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_reg_cfg1;

/* define the union reg_int0_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int error_report_0 : 1;   /* [0]  */
        unsigned int error_report_1 : 1;   /* [1]  */
        unsigned int error_report_2 : 1;   /* [2]  */
        unsigned int error_report_3 : 1;   /* [3]  */
        unsigned int error_report_4 : 1;   /* [4]  */
        unsigned int error_report_5 : 1;   /* [5]  */
        unsigned int error_report_6 : 1;   /* [6]  */
        unsigned int error_report_7 : 1;   /* [7]  */
        unsigned int error_report_8 : 1;   /* [8]  */
        unsigned int error_report_9 : 1;   /* [9]  */
        unsigned int error_report_10 : 1;  /* [10]  */
        unsigned int error_report_11 : 1;  /* [11]  */
        unsigned int error_report_12 : 1;  /* [12]  */
        unsigned int error_report_13 : 1;  /* [13]  */
        unsigned int error_report_14 : 1;  /* [14]  */
        unsigned int error_report_15 : 1;  /* [15]  */
        unsigned int errescentry : 1;      /* [16]  */
        unsigned int errsyncesc : 1;       /* [17]  */
        unsigned int errcontrol : 1;       /* [18]  */
        unsigned int errcontentionlp0 : 1; /* [19]  */
        unsigned int errcontentionlp1 : 1; /* [20]  */
        unsigned int reserved_0 : 11;      /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int0_status;

/* define the union reg_int1_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int eotp_rx_err : 1;            /* [0]  */
        unsigned int rxpkt_size_err : 1;         /* [1]  */
        unsigned int rxcrc_err : 1;              /* [2]  */
        unsigned int rxecc_single_err : 1;       /* [3]  */
        unsigned int rxecc_multi_err : 1;        /* [4]  */
        unsigned int pld_wr_err : 1;             /* [5]  */
        unsigned int pld_rd_err : 1;             /* [6]  */
        unsigned int cmd_wr_err : 1;             /* [7]  */
        unsigned int cmd_pld_wr_err : 1;         /* [8]  */
        unsigned int cmd_pld_rd_err : 1;         /* [9]  */
        unsigned int rxpld_rd_err : 1;           /* [10]  */
        unsigned int rxpld_wr_err : 1;           /* [11]  */
        unsigned int to_lp_rx : 1;               /* [12]  */
        unsigned int to_hs_tx : 1;               /* [13]  */
        unsigned int phy_pll_lock_err : 1;       /* [14]  */
        unsigned int cmd_tran_end : 1;           /* [15]  */
        unsigned int line_length_change : 1;     /* [16]  */
        unsigned int hss_abnormal : 1;           /* [17]  */
        unsigned int fifo_nempty_when_vsync : 1; /* [18]  */
        unsigned int reserved_0 : 1;             /* [19]  */
        unsigned int vss : 1;                    /* [20]  */
        unsigned int reserved_1 : 3;             /* [23..21]  */
        unsigned int rxtrigger0 : 1;             /* [24]  */
        unsigned int rxtrigger1 : 1;             /* [25]  */
        unsigned int rxtrigger2 : 1;             /* [26]  */
        unsigned int rxtrigger3 : 1;             /* [27]  */
        unsigned int reserved_2 : 4;             /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int1_status;

/* define the union reg_int0_mask */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mask_error_report_0 : 1;   /* [0]  */
        unsigned int mask_error_report_1 : 1;   /* [1]  */
        unsigned int mask_error_report_2 : 1;   /* [2]  */
        unsigned int mask_error_report_3 : 1;   /* [3]  */
        unsigned int mask_error_report_4 : 1;   /* [4]  */
        unsigned int mask_error_report_5 : 1;   /* [5]  */
        unsigned int mask_error_report_6 : 1;   /* [6]  */
        unsigned int mask_error_report_7 : 1;   /* [7]  */
        unsigned int mask_error_report_8 : 1;   /* [8]  */
        unsigned int mask_error_report_9 : 1;   /* [9]  */
        unsigned int mask_error_report_10 : 1;  /* [10]  */
        unsigned int mask_error_report_11 : 1;  /* [11]  */
        unsigned int mask_error_report_12 : 1;  /* [12]  */
        unsigned int mask_error_report_13 : 1;  /* [13]  */
        unsigned int mask_error_report_14 : 1;  /* [14]  */
        unsigned int mask_error_report_15 : 1;  /* [15]  */
        unsigned int mask_errescentry : 1;      /* [16]  */
        unsigned int mask_errsyncesc : 1;       /* [17]  */
        unsigned int mask_errcontrol : 1;       /* [18]  */
        unsigned int mask_errcontentionlp0 : 1; /* [19]  */
        unsigned int mask_errcontentionlp1 : 1; /* [20]  */
        unsigned int reserved_0 : 11;           /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int0_mask;

/* define the union reg_int1_mask */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mask_eotp_rx_err : 1;            /* [0]  */
        unsigned int mask_rxpkt_size_err : 1;         /* [1]  */
        unsigned int mask_rxcrc_err : 1;              /* [2]  */
        unsigned int mask_rxecc_single_err : 1;       /* [3]  */
        unsigned int mask_rxecc_multi_err : 1;        /* [4]  */
        unsigned int mask_pld_wr_err : 1;             /* [5]  */
        unsigned int mask_pld_rd_err : 1;             /* [6]  */
        unsigned int mask_cmd_wr_err : 1;             /* [7]  */
        unsigned int mask_cmd_pld_wr_err : 1;         /* [8]  */
        unsigned int mask_cmd_pld_rd_err : 1;         /* [9]  */
        unsigned int mask_rxpld_rd_err : 1;           /* [10]  */
        unsigned int mask_rxpld_wr_err : 1;           /* [11]  */
        unsigned int mask_to_lp_rx : 1;               /* [12]  */
        unsigned int mask_to_hs_tx : 1;               /* [13]  */
        unsigned int mask_phy_pll_lock_err : 1;       /* [14]  */
        unsigned int mask_cmd_tran_end : 1;           /* [15]  */
        unsigned int mask_line_length_change : 1;     /* [16]  */
        unsigned int mask_hss_abnormal : 1;           /* [17]  */
        unsigned int mask_fifo_nempty_when_vsync : 1; /* [18]  */
        unsigned int reserved_0 : 1;                  /* [19]  */
        unsigned int mask_vss : 1;                    /* [20]  */
        unsigned int reserved_1 : 3;                  /* [23..21]  */
        unsigned int mask_rxtrigger0 : 1;             /* [24]  */
        unsigned int mask_rxtrigger1 : 1;             /* [25]  */
        unsigned int mask_rxtrigger2 : 1;             /* [26]  */
        unsigned int mask_rxtrigger3 : 1;             /* [27]  */
        unsigned int reserved_2 : 4;                  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int1_mask;

/* define the union reg_phy_cal */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int txskewcalhs : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_cal;

/* define the union reg_auto_ulps_mode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int auto_ulps : 1;    /* [0]  */
        unsigned int reserved_0 : 15;  /* [15..1]  */
        unsigned int pll_off_ulps : 1; /* [16]  */
        unsigned int reserved_1 : 15;  /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_auto_ulps_mode;

/* define the union reg_auto_ulps_wakeup_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int twakeup_clk_div : 16; /* [15..0]  */
        unsigned int twakeup_cnt : 16;     /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_auto_ulps_wakeup_time;

/* define the union reg_dsc_config */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int compression_mode : 1; /* [0]  */
        unsigned int reserved_0 : 7;       /* [7..1]  */
        unsigned int algorithm_mode : 2;   /* [9..8]  */
        unsigned int reserved_1 : 6;       /* [15..10]  */
        unsigned int pps : 2;              /* [17..16]  */
        unsigned int reserved_2 : 6;       /* [23..18]  */
        unsigned int video_lpcmd : 1;      /* [24]  */
        unsigned int reserved_3 : 7;       /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dsc_config;

/* define the union reg_read_cmd_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int read_cmd_time : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;    /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_read_cmd_time;

/* define the union reg_auto_ulps_min_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int auto_ulps_min_time : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;         /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_auto_ulps_min_time;

/* define the union reg_phy_mode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int phy_mode : 1;    /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_mode;

/* define the union reg_vid_shadow_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vid_shadow_en : 1;  /* [0]  */
        unsigned int reserved_0 : 7;     /* [7..1]  */
        unsigned int vid_shadow_req : 1; /* [8]  */
        unsigned int reserved_1 : 23;    /* [31..9]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_shadow_ctrl;

/* define the union reg_dsi_dbg0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vid_delay : 17; /* [16..0]  */
        unsigned int phyfsm_st : 3;  /* [19..17]  */
        unsigned int dbips_st : 4;   /* [23..20]  */
        unsigned int vidregion : 3;  /* [26..24]  */
        unsigned int dpips_st : 5;   /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dsi_dbg0;

/* define the union reg_phy_pll_start_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int phy_pll_start_time_qst : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;             /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_pll_start_time;

/* define the union reg_dbg_crc_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dbg_crc_init : 1;     /* [0]  */
        unsigned int dbg_crc_lane_sel : 2; /* [2..1]  */
        unsigned int dbg_crc_en : 1;       /* [3]  */
        unsigned int reserved_0 : 28;      /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dbg_crc_ctrl;

/* define the union reg_secu_cfg_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int secu_cfg_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_secu_cfg_en;

/* define the union reg_polarity_ctrl */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART02_H_ */
