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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART01_H_


/* define the union reg_ctrl_reset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ctrl_reset : 1;  /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ctrl_reset;

/* define the union reg_crg_cfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int txescclk_div : 8; /* [7..0]  */
        unsigned int toclk_div : 8;    /* [15..8]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_crg_cfg;

/* define the union reg_video_vc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vcid : 2;        /* [1..0]  */
        unsigned int reserved_0 : 2;  /* [3..2]  */
        unsigned int vcid_sel : 1;    /* [4]  */
        unsigned int reserved_1 : 27; /* [31..5]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_video_vc;

/* define the union reg_data_type */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int data_type : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_data_type;

/* define the union reg_lp_cmd_byte */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int invact_lpcmd_byte : 8;  /* [7..0]  */
        unsigned int reserved_0 : 8;         /* [15..8]  */
        unsigned int outvact_lpcmd_byte : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;         /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lp_cmd_byte;

/* define the union reg_pck_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int eotp_tx_en : 1;   /* [0]  */
        unsigned int eotp_rx_en : 1;   /* [1]  */
        unsigned int crc_rx_en : 1;    /* [2]  */
        unsigned int ecc_rx_en : 1;    /* [3]  */
        unsigned int bta_en : 1;       /* [4]  */
        unsigned int tear_fx_en : 1;   /* [5]  */
        unsigned int ack_rqst_en : 1;  /* [6]  */
        unsigned int frame_ack_en : 1; /* [7]  */
        unsigned int reserved_0 : 24;  /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_pck_en;

/* define the union reg_gen_vc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int gen_vcid : 2;    /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gen_vc;

/* define the union reg_mode_cfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cmd_video_mode : 1;     /* [0]  */
        unsigned int reserved_0 : 3;         /* [3..1]  */
        unsigned int cmd_mode_tran_type : 1; /* [4]  */
        unsigned int reserved_1 : 3;         /* [7..5]  */
        unsigned int video_mode_type : 2;    /* [9..8]  */
        unsigned int reserved_2 : 2;         /* [11..10]  */
        unsigned int dualpix_en : 1;         /* [12]  */
        unsigned int reserved_3 : 3;         /* [15..13]  */
        unsigned int scramb_en : 1;          /* [16]  */
        unsigned int reserved_4 : 3;         /* [19..17]  */
        unsigned int cphy_ppi_dw_sel : 1;    /* [20]  */
        unsigned int reserved_5 : 11;        /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mode_cfg;

/* define the union reg_video_lp_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsa_lp_en : 1;   /* [0]  */
        unsigned int vbp_lp_en : 1;   /* [1]  */
        unsigned int vfp_lp_en : 1;   /* [2]  */
        unsigned int vact_lp_en : 1;  /* [3]  */
        unsigned int hbp_lp_en : 1;   /* [4]  */
        unsigned int hfp_lp_en : 1;   /* [5]  */
        unsigned int reserved_0 : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_video_lp_en;

/* define the union reg_videom_pkt_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_pkt_size : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;      /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_pkt_size;

/* define the union reg_videom_num_chunks */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_num_chunks : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;        /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_num_chunks;

/* define the union reg_videom_null_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_null_size : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;       /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_null_size;

/* define the union reg_videom_hsa_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_hsa_size : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_hsa_size;

/* define the union reg_videom_hbp_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_hbp_size : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_hbp_size;

/* define the union reg_videom_hline_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_hline_size : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;       /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_hline_size;

/* define the union reg_videom_vsa_lines */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsa_lines : 10;  /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_vsa_lines;

/* define the union reg_videom_vbp_lines */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbp_lines : 10;  /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_vbp_lines;

/* define the union reg_videom_vfp_lines */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vfp_lines : 10;  /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_vfp_lines;

/* define the union reg_videom_vactive_lines */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int v_active_lines : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;     /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_vactive_lines;

/* define the union reg_command_pkt_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cmd_pkt_size : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_pkt_size;

/* define the union reg_command_tran_mode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_rd_pkt_size_tran : 1; /* [0]  */
        unsigned int gen_sw_0p : 1;            /* [1]  */
        unsigned int gen_sw_1p : 1;            /* [2]  */
        unsigned int gen_sw_2p : 1;            /* [3]  */
        unsigned int gen_sr_0p : 1;            /* [4]  */
        unsigned int gen_sr_1p : 1;            /* [5]  */
        unsigned int gen_sr_2p : 1;            /* [6]  */
        unsigned int gen_lw : 1;               /* [7]  */
        unsigned int dcs_sw_0p : 1;            /* [8]  */
        unsigned int dcs_sw_1p : 1;            /* [9]  */
        unsigned int dcs_sr_0p : 1;            /* [10]  */
        unsigned int dcs_lw : 1;               /* [11]  */
        unsigned int reserved_0 : 20;          /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_tran_mode;

/* define the union reg_command_header */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int command_datatype : 6;       /* [5..0]  */
        unsigned int command_virtualchannel : 2; /* [7..6]  */
        unsigned int command_wordcount_low : 8;  /* [15..8]  */
        unsigned int command_wordcount_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;             /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_header;

/* define the union reg_command_payload */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int command_pld_b1 : 8; /* [7..0]  */
        unsigned int command_pld_b2 : 8; /* [15..8]  */
        unsigned int command_pld_b3 : 8; /* [23..16]  */
        unsigned int command_pld_b4 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_payload;

/* define the union reg_command_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int command_empty : 1;   /* [0]  */
        unsigned int command_full : 1;    /* [1]  */
        unsigned int pld_write_empty : 1; /* [2]  */
        unsigned int pld_write_full : 1;  /* [3]  */
        unsigned int pld_read_empty : 1;  /* [4]  */
        unsigned int pld_read_full : 1;   /* [5]  */
        unsigned int rd_cmd_busy : 1;     /* [6]  */
        unsigned int command_idle : 1;    /* [7]  */
        unsigned int reserved_0 : 24;     /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_status;

/* define the union reg_hs_lp_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lprx_to_cnt : 16; /* [15..0]  */
        unsigned int hstx_to_cnt : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hs_lp_to_set;

/* define the union reg_hsrd_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hsrd_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hsrd_to_set;

/* define the union reg_lprd_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lprd_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lprd_to_set;

/* define the union reg_hswr_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hswr_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 8;   /* [23..16]  */
        unsigned int pre_to_mode : 1;  /* [24]  */
        unsigned int reserved_1 : 7;   /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hswr_to_set;

/* define the union reg_lpwr_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lpwr_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lpwr_to_set;

/* define the union reg_bta_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bta_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_bta_to_set;

/* define the union reg_clklane_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int txrequesthsclk : 1;       /* [0]  */
        unsigned int clklane_continue : 1;     /* [1]  */
        unsigned int cphy_clklane_sel_qst : 1; /* [2]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_clklane_ctrl;

/* define the union reg_clklane_time */
typedef union {
    /* define the struct bits */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART01_H_ */
