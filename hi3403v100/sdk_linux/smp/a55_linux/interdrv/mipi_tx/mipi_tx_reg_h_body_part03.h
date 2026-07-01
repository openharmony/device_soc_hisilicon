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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART03_H_

typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsync_plr : 1;   /* [0]  */
        unsigned int hsync_plr : 1;   /* [1]  */
        unsigned int reserved_0 : 1;  /* [2]  */
        unsigned int data_en_plr : 1; /* [3]  */
        unsigned int reserved_1 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_polarity_ctrl;

/* define the union reg_dual_pixelmode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 5;     /* [4..0]  */
        unsigned int dual_mode_en : 1;   /* [5]  */
        unsigned int reserved_1 : 7;     /* [12..6]  */
        unsigned int dual_mode_ctrl : 1; /* [13]  */
        unsigned int reserved_2 : 18;    /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dual_pixelmode;

/* define the union reg_hrz_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dsi_hsize : 13;  /* [12..0]  */
        unsigned int reserved_0 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hrz_ctrl;

/* define the union reg_vrt_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dsi_vsize : 13;  /* [12..0]  */
        unsigned int reserved_0 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vrt_ctrl;

/* define the union reg_int_pro_msk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int int_pro_msk : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;  /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int_pro_msk;

/* define the union reg_vstate */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vstate : 11;     /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vstate;

/* define the union reg_hstate */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hstate : 6;      /* [5..0]  */
        unsigned int reserved_0 : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hstate;

/* define the union reg_dphytx_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dphytx_ctrl : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dphytx_ctrl;

/* define the union reg_phytx_trstop_flag */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int phytx_trstop_flag : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phytx_trstop_flag;

/* define the union reg_mipi_dsi_mem_control */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tselr : 2;       /* [1..0]  */
        unsigned int tselw : 2;       /* [3..2]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dsi_mem_control;

/* define the union reg_display_control */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dpishutdn : 1;    /* [0]  */
        unsigned int dpicolorm : 1;    /* [1]  */
        unsigned int vsync_f_sub : 1;  /* [2]  */
        unsigned int vsync_b_sub : 1;  /* [3]  */
        unsigned int v1_sel : 3;       /* [6..4]  */
        unsigned int reserved_0 : 1;   /* [7]  */
        unsigned int u1_sel : 3;       /* [10..8]  */
        unsigned int reserved_1 : 1;   /* [11]  */
        unsigned int y1_sel : 3;       /* [14..12]  */
        unsigned int reserved_2 : 1;   /* [15]  */
        unsigned int v2_sel : 3;       /* [18..16]  */
        unsigned int reserved_3 : 1;   /* [19]  */
        unsigned int u2_sel : 3;       /* [22..20]  */
        unsigned int reserved_4 : 1;   /* [23]  */
        unsigned int y2_sel : 3;       /* [26..24]  */
        unsigned int reserved_5 : 1;   /* [27]  */
        unsigned int dataen_f_sub : 1; /* [28]  */
        unsigned int dataen_b_sub : 1; /* [29]  */
        unsigned int hsync_f_sub : 1;  /* [30]  */
        unsigned int hsync_b_sub : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_display_control;

/* define the union reg_operation_mode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 1;          /* [0]  */
        unsigned int mem_ck_en : 1;           /* [1]  */
        unsigned int reserved_1 : 3;          /* [4..2]  */
        unsigned int init_skew_en : 1;        /* [5]  */
        unsigned int period_skew_en : 1;      /* [6]  */
        unsigned int reserved_2 : 1;          /* [7]  */
        unsigned int colorbar_en : 1;         /* [8]  */
        unsigned int colorbar_orien : 1;      /* [9]  */
        unsigned int colorbar_test_en : 1;    /* [10]  */
        unsigned int reserved_3 : 1;          /* [11]  */
        unsigned int colorbar_mode : 4;       /* [15..12]  */
        unsigned int read_empty_vsync_en : 1; /* [16]  */
        unsigned int reserved_4 : 3;          /* [19..17]  */
        unsigned int hss_abnormal_rst : 1;    /* [20]  */
        unsigned int rxesc_buf_en : 1;        /* [21]  */
        unsigned int reserved_5 : 2;          /* [23..22]  */
        unsigned int turndisable : 1;         /* [24]  */
        unsigned int forcerxmode : 1;         /* [25]  */
        unsigned int forcetxstopmode : 1;     /* [26]  */
        unsigned int video_te_en : 1;         /* [27]  */
        unsigned int reserved_6 : 3;          /* [30..28]  */
        unsigned int input_en : 1;            /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_operation_mode;

/* define the union reg_hfp_hact_in */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hact_in : 16; /* [15..0]  */
        unsigned int hfp_in : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hfp_hact_in;

/* define the union reg_hbp_hsa_in */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hsa_in : 16; /* [15..0]  */
        unsigned int hbp_in : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hbp_hsa_in;

/* define the union reg_vert_det */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vact_det : 16; /* [15..0]  */
        unsigned int vall_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vert_det;

/* define the union reg_hori0_det */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hact_det : 16;  /* [15..0]  */
        unsigned int hline_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hori0_det;

/* define the union reg_hori1_det */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hsa_det : 16; /* [15..0]  */
        unsigned int hbp_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hori1_det;

/* define the union reg_vsa_det */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsa_det : 16;    /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vsa_det;

/* define the union reg_v_h_send */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int h_send : 16; /* [15..0]  */
        unsigned int v_send : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v_h_send;

/* define the union reg_lanereqhs_width */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lanerequesths_width : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;          /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lanereqhs_width;

/* define the union reg_hs_data */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int l0hsdata : 8; /* [7..0]  */
        unsigned int l1hsdata : 8; /* [15..8]  */
        unsigned int l2hsdata : 8; /* [23..16]  */
        unsigned int l3hsdata : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hs_data;

/* define the union reg_datatype0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dt_pixel : 6;   /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int dt_hss : 6;     /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int dt_vse : 6;     /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int dt_vss : 6;     /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_datatype0;

/* define the union reg_datatype1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dt_null : 6;    /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int dt_blank : 6;   /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int dt_etop : 6;    /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int dt_hse : 6;     /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_datatype1;

/* define the union reg_csi_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int csi_en : 1;           /* [0]  */
        unsigned int reserved_0 : 3;       /* [3..1]  */
        unsigned int csi_vse_position : 1; /* [4]  */
        unsigned int reserved_1 : 3;       /* [7..5]  */
        unsigned int frame_num_inc : 1;    /* [8]  */
        unsigned int reserved_2 : 3;       /* [11..9]  */
        unsigned int dt_config : 1;        /* [12]  */
        unsigned int reserved_3 : 3;       /* [15..13]  */
        unsigned int csi_hss_en : 1;       /* [16]  */
        unsigned int reserved_4 : 15;      /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_csi_ctrl;

/* define the union reg_lane_id */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lane0_id : 2;    /* [1..0]  */
        unsigned int reserved_0 : 2;  /* [3..2]  */
        unsigned int lane1_id : 2;    /* [5..4]  */
        unsigned int reserved_1 : 2;  /* [7..6]  */
        unsigned int lane2_id : 2;    /* [9..8]  */
        unsigned int reserved_2 : 2;  /* [11..10]  */
        unsigned int lane3_id : 2;    /* [13..12]  */
        unsigned int reserved_3 : 18; /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lane_id;

/* define the union reg_read_memory_delay_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int delay_regin : 17;   /* [16..0]  */
        unsigned int reserved_0 : 3;     /* [19..17]  */
        unsigned int delay_abnormal : 1; /* [20]  */
        unsigned int delay_from_reg : 1; /* [21]  */
        unsigned int reserved_1 : 10;    /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_read_memory_delay_ctrl;

/* define the union reg_ppi_v_send0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsa_length : 16;     /* [15..0]  */
        unsigned int vsa_vbp_length : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_v_send0;

/* define the union reg_ppi_v_send1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vactive_length : 16; /* [15..0]  */
        unsigned int vtotal_length : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_v_send1;

/* define the union reg_ppi_h_send0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hsa_length : 16;     /* [15..0]  */
        unsigned int hsa_hbp_length : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_h_send0;

/* define the union reg_ppi_h_send1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hactive_length : 16; /* [15..0]  */
        unsigned int htotal_length : 16;  /* [31..16]  */
    } bits;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_TX_MIPI_TX_REG_H_BODY_PART03_H_ */
