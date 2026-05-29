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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART04_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART04_H_


    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_h_send1;

/* define the union reg_ppi_h_send2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int pix_pkt_length : 16;   /* [15..0]  */
        unsigned int pix_pkt_distance : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_h_send2;

/* define the union reg_ppi_h_send3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int line_length_send_diff : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_h_send3;

/* define the union reg_dft_control */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int code_length : 17;    /* [16..0]  */
        unsigned int reserved_0 : 3;      /* [19..17]  */
        unsigned int pattern_type : 2;    /* [21..20]  */
        unsigned int reserved_1 : 2;      /* [23..22]  */
        unsigned int hs_lp_mode : 2;      /* [25..24]  */
        unsigned int txskewcalhs_dft : 1; /* [26]  */
        unsigned int bist_mode : 1;       /* [27]  */
        unsigned int compare_done : 1;    /* [28]  */
        unsigned int compare : 1;         /* [29]  */
        unsigned int compare_result : 1;  /* [30]  */
        unsigned int reserved_2 : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dft_control;

/* define the union reg_dft_prbs_init */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int prbs_init : 7;   /* [6..0]  */
        unsigned int reserved_0 : 25; /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dft_prbs_init;

// ==============================================================================
/* define the global struct */
typedef struct {
    volatile reg_ctrl_reset ctrl_reset;                         /* 0x0 */
    volatile unsigned int reserved_0;                           /* 0x4 */
    volatile reg_crg_cfg crg_cfg;                               /* 0x8 */
    volatile reg_video_vc video_vc;                             /* 0xc */
    volatile reg_data_type data_type;                           /* 0x10 */
    volatile unsigned int reserved_1;                           /* 0x14 */
    volatile reg_lp_cmd_byte lp_cmd_byte;                       /* 0x18 */
    volatile unsigned int reserved_2[4];                        /* 0x1c~0x28 */
    volatile reg_pck_en pck_en;                                 /* 0x2c */
    volatile reg_gen_vc gen_vc;                                 /* 0x30 */
    volatile reg_mode_cfg mode_cfg;                             /* 0x34 */
    volatile reg_video_lp_en video_lp_en;                       /* 0x38 */
    volatile reg_videom_pkt_size videom_pkt_size;               /* 0x3c */
    volatile reg_videom_num_chunks videom_num_chunks;           /* 0x40 */
    volatile reg_videom_null_size videom_null_size;             /* 0x44 */
    volatile reg_videom_hsa_size videom_hsa_size;               /* 0x48 */
    volatile reg_videom_hbp_size videom_hbp_size;               /* 0x4c */
    volatile reg_videom_hline_size videom_hline_size;           /* 0x50 */
    volatile reg_videom_vsa_lines videom_vsa_lines;             /* 0x54 */
    volatile reg_videom_vbp_lines videom_vbp_lines;             /* 0x58 */
    volatile reg_videom_vfp_lines videom_vfp_lines;             /* 0x5c */
    volatile reg_videom_vactive_lines videom_vactive_lines;     /* 0x60 */
    volatile reg_command_pkt_size command_pkt_size;             /* 0x64 */
    volatile reg_command_tran_mode command_tran_mode;           /* 0x68 */
    volatile reg_command_header command_header;                 /* 0x6c */
    volatile reg_command_payload command_payload;               /* 0x70 */
    volatile reg_command_status command_status;                 /* 0x74 */
    volatile reg_hs_lp_to_set hs_lp_to_set;                     /* 0x78 */
    volatile reg_hsrd_to_set hsrd_to_set;                       /* 0x7c */
    volatile reg_lprd_to_set lprd_to_set;                       /* 0x80 */
    volatile reg_hswr_to_set hswr_to_set;                       /* 0x84 */
    volatile reg_lpwr_to_set lpwr_to_set;                       /* 0x88 */
    volatile reg_bta_to_set bta_to_set;                         /* 0x8c */
    volatile unsigned int reserved_3;                           /* 0x90 */
    volatile reg_clklane_ctrl clklane_ctrl;                     /* 0x94 */
    volatile reg_clklane_time clklane_time;                     /* 0x98 */
    volatile reg_datalane_time datalane_time;                   /* 0x9c */
    volatile reg_phy_ctrl phy_ctrl;                             /* 0xa0 */
    volatile reg_lane_num lane_num;                             /* 0xa4 */
    volatile reg_ulps_ctrl ulps_ctrl;                           /* 0xa8 */
    volatile reg_tx_triggers tx_triggers;                       /* 0xac */
    volatile reg_ppi_status ppi_status;                         /* 0xb0 */
    volatile reg_phy_reg_cfg0 phy_reg_cfg0;                     /* 0xb4 */
    volatile reg_phy_reg_cfg1 phy_reg_cfg1;                     /* 0xb8 */
    volatile reg_int0_status int0_status;                       /* 0xbc */
    volatile reg_int1_status int1_status;                       /* 0xc0 */
    volatile reg_int0_mask int0_mask;                           /* 0xc4 */
    volatile reg_int1_mask int1_mask;                           /* 0xc8 */
    volatile reg_phy_cal phy_cal;                               /* 0xcc */
    volatile unsigned int reserved_4[2];                        /* 0xd0~0xd4 */
    volatile unsigned int int_force0;                           /* 0xd8 */
    volatile unsigned int int_force1;                           /* 0xdc */
    volatile reg_auto_ulps_mode auto_ulps_mode;                 /* 0xe0 */
    volatile unsigned int auto_ulps_entry_delay;                /* 0xe4 */
    volatile reg_auto_ulps_wakeup_time auto_ulps_wakeup_time;   /* 0xe8 */
    volatile unsigned int reserved_5;                           /* 0xec */
    volatile reg_dsc_config dsc_config;                         /* 0xf0 */
    volatile reg_read_cmd_time read_cmd_time;                   /* 0xf4 */
    volatile reg_auto_ulps_min_time auto_ulps_min_time;         /* 0xf8 */
    volatile reg_phy_mode phy_mode;                             /* 0xfc */
    volatile reg_vid_shadow_ctrl vid_shadow_ctrl;               /* 0x100 */
    volatile unsigned int reserved_6[38];                       /* 0x104~0x198 */
    volatile reg_dsi_dbg0 dsi_dbg0;                             /* 0x19c */
    volatile reg_phy_pll_start_time phy_pll_start_time;         /* 0x1a0 */
    volatile reg_dbg_crc_ctrl dbg_crc_ctrl;                     /* 0x1a4 */
    volatile unsigned int dbg_crc_val;                          /* 0x1a8 */
    volatile reg_secu_cfg_en secu_cfg_en;                       /* 0x1ac */
    volatile reg_polarity_ctrl polarity_ctrl;                   /* 0x1b0 */
    volatile unsigned int reserved_7;                           /* 0x1b4 */
    volatile reg_dual_pixelmode dual_pixelmode;                 /* 0x1b8 */
    volatile unsigned int reserved_8[7];                        /* 0x1bc~0x1d4 */
    volatile unsigned int frm_valid_dbg;                        /* offset 0x1d0 + 0x08 */
    volatile unsigned int reserved_9[4];                        /* 0x1dc~0x1e8 */
    volatile reg_hrz_ctrl hrz_ctrl;                             /* 0x1ec */
    volatile reg_vrt_ctrl vrt_ctrl;                             /* 0x1f0 */
    volatile unsigned int reserved_10;                          /* 0x1f4 */
    volatile reg_int_pro_msk int_pro_msk;                       /* 0x1f8 */
    volatile reg_vstate vstate;                                 /* 0x1fc */
    volatile reg_hstate hstate;                                 /* 0x200 */
    volatile unsigned int reserved_11[8];                       /* 0x204~0x220 */
    volatile unsigned int phytx_stopsnt;                        /* 0x224 */
    volatile reg_dphytx_ctrl dphytx_ctrl;                       /* 0x228 */
    volatile reg_phytx_trstop_flag phytx_trstop_flag;           /* 0x22c */
    volatile unsigned int phytx_status;                         /* 0x230 */
    volatile unsigned int reserved_12[6];                       /* 0x234~0x248 */
    volatile unsigned int gint_msk;                             /* 0x24c */
    volatile unsigned int reserved_13[44];                      /* 0x250~0x2fc */
    volatile reg_mipi_dsi_mem_control mipi_dsi_mem_control;     /* 0x300 */
    volatile reg_display_control display_control;               /* 0x304 */
    volatile reg_operation_mode operation_mode;                 /* 0x308 */
    volatile reg_hfp_hact_in hfp_hact_in;                       /* 0x30c */
    volatile reg_hbp_hsa_in hbp_hsa_in;                         /* 0x310 */
    volatile reg_vert_det vert_det;                             /* 0x314 */
    volatile reg_hori0_det hori0_det;                           /* 0x318 */
    volatile reg_hori1_det hori1_det;                           /* 0x31c */
    volatile reg_vsa_det vsa_det;                               /* 0x320 */
    volatile reg_v_h_send v_h_send;                             /* 0x324 */
    volatile reg_lanereqhs_width lanereqhs_width;               /* 0x328 */
    volatile reg_hs_data hs_data;                               /* 0x32c */
    volatile reg_datatype0 datatype0;                           /* 0x330 */
    volatile reg_datatype1 datatype1;                           /* 0x334 */
    volatile reg_csi_ctrl csi_ctrl;                             /* 0x338 */
    volatile unsigned int skew_begin;                           /* 0x33c */
    volatile unsigned int skew_end;                             /* 0x340 */
    volatile unsigned int reserved_14[3];                       /* 0x344~0x34c */
    volatile reg_lane_id lane_id;                               /* 0x350 */
    volatile reg_read_memory_delay_ctrl read_memory_delay_ctrl; /* 0x354 */
    volatile reg_ppi_v_send0 ppi_v_send0;                       /* 0x358 */
    volatile reg_ppi_v_send1 ppi_v_send1;                       /* 0x35c */
    volatile reg_ppi_h_send0 ppi_h_send0;                       /* 0x360 */
    volatile reg_ppi_h_send1 ppi_h_send1;                       /* 0x364 */
    volatile reg_ppi_h_send2 ppi_h_send2;                       /* 0x368 */
    volatile reg_ppi_h_send3 ppi_h_send3;                       /* 0x36c */
    volatile unsigned int reserved_15[12];                      /* 0x370~0x39c */
    volatile unsigned int te_length;                            /* 0x3a0 */
    volatile unsigned int reserved_16[15];                      /* 0x3a4~0x3dc */
    volatile reg_dft_control dft_control;                       /* 0x3e0 */
    volatile unsigned int dft_skew_period;                      /* 0x3e4 */
    volatile reg_dft_prbs_init dft_prbs_init;                   /* 0x3e8 */
} mipi_tx_regs_type_t;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART04_H_ */
