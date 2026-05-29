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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART36_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART36_H_

        unsigned int fifo_det_mode : 4;  /* [7..4]  */
        unsigned int reserved_1 : 24;    /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_debug_ctrl;

/* define the union reg_mac_debug_clr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int axi_det_clr : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_debug_clr;

/* define the union reg_mac_axi_press0_ctrl0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int awvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int awvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 14;        /* [30..17]  */
        unsigned int bypass_flag : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press0_ctrl0;

/* define the union reg_mac_axi_press0_ctrl1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int arvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int arvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 15;        /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press0_ctrl1;

/* define the union reg_mac_axi_press0_ctrl2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int wvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 15;       /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press0_ctrl2;

/* define the union reg_mac_axi_press0_ctrl3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int rvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 15;       /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press0_ctrl3;

/* define the union reg_mac_axi_press0_ctrl4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int bvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 15;       /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press0_ctrl4;

/* define the union reg_mac_axi_press0_ctrl5 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int axi_press_st : 1; /* [0]  */
        unsigned int reserved_0 : 31;  /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press0_ctrl5;

/* define the union reg_mac_axi_press1_ctrl0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int awvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int awvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 14;        /* [30..17]  */
        unsigned int bypass_flag : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press1_ctrl0;

/* define the union reg_mac_axi_press1_ctrl1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int arvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int arvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 15;        /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press1_ctrl1;

/* define the union reg_mac_axi_press1_ctrl2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int wvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 15;       /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press1_ctrl2;

/* define the union reg_mac_axi_press1_ctrl3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int rvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 15;       /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press1_ctrl3;

/* define the union reg_mac_axi_press1_ctrl4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bvalid_delay_cfg : 16; /* [15..0]  */
        unsigned int bvalid_delay_mode : 1; /* [16]  */
        unsigned int reserved_0 : 15;       /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press1_ctrl4;

/* define the union reg_mac_axi_press1_ctrl5 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int axi_press_st : 1; /* [0]  */
        unsigned int reserved_0 : 31;  /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_axi_press1_ctrl5;

/* define the union reg_vid_read_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int chm_rmode : 3;     /* [2..0]  */
        unsigned int reserved_0 : 1;    /* [3]  */
        unsigned int lm_rmode : 3;      /* [6..4]  */
        unsigned int reserved_1 : 1;    /* [7]  */
        unsigned int chm_draw_mode : 2; /* [9..8]  */
        unsigned int lm_draw_mode : 2;  /* [11..10]  */
        unsigned int flip_en : 1;       /* [12]  */
        unsigned int chm_copy_en : 1;   /* [13]  */
        unsigned int reserved_2 : 2;    /* [15..14]  */
        unsigned int mute_en : 1;       /* [16]  */
        unsigned int mute_req_en : 1;   /* [17]  */
        unsigned int vicap_mute_en : 1; /* [18]  */
        unsigned int mrg_enable : 1;    /* [19]  */
        unsigned int mrg_mute_mode : 1; /* [20]  */
        unsigned int fdr_ck_gt_en : 1;  /* [21]  */
        unsigned int reserved_3 : 10;   /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_read_ctrl;

/* define the union reg_vid_mac_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_ctrl : 2;    /* [1..0]  */
        unsigned int req_len : 2;     /* [3..2]  */
        unsigned int reserved_0 : 4;  /* [7..4]  */
        unsigned int ofl_master : 1;  /* [8]  */
        unsigned int reserved_1 : 22; /* [30..9]  */
        unsigned int pre_rd_en : 1;   /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_mac_ctrl;

/* define the union reg_vid_out_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int draw_pixel_mode : 3;  /* [2..0]  */
        unsigned int draw_pixel_en : 1;    /* [3]  */
        unsigned int uv_order_en : 1;      /* [4]  */
        unsigned int single_port_mode : 1; /* [5]  */
        unsigned int testpattern_en : 1;   /* [6]  */
        unsigned int reserved_0 : 25;      /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_out_ctrl;

/* define the union reg_vid_mute_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_alpha : 8;  /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_mute_alpha;

/* define the union reg_vid_mute_bk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_cr : 10;   /* [9..0]  */
        unsigned int mute_cb : 10;   /* [19..10]  */
        unsigned int mute_y : 10;    /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_mute_bk;

/* define the union reg_vid_src_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int data_type : 3;        /* [2..0]  */
        unsigned int data_fmt : 2;         /* [4..3]  */
        unsigned int reserved_0 : 3;       /* [7..5]  */
        unsigned int data_width : 2;       /* [9..8]  */
        unsigned int reserved_1 : 2;       /* [11..10]  */
        unsigned int field_type : 1;       /* [12]  */
        unsigned int reserved_2 : 3;       /* [15..13]  */
        unsigned int disp_mode : 4;        /* [19..16]  */
        unsigned int dcmp_en : 2;          /* [21..20]  */
        unsigned int compact_en : 1;       /* [22]  */
        unsigned int compact_req_mode : 1; /* [23]  */
        unsigned int reserved_3 : 8;       /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_src_info;

/* define the union reg_vid_src_reso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int src_w : 16; /* [15..0]  */
        unsigned int src_h : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_src_reso;

/* define the union reg_vid_src_crop */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int src_crop_x : 16; /* [15..0]  */
        unsigned int src_crop_y : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_src_crop;

/* define the union reg_vid_in_reso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ireso_w : 16; /* [15..0]  */
        unsigned int ireso_h : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_in_reso;

/* define the union reg_vid_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lm_stride : 16;  /* [15..0]  */
        unsigned int chm_stride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_stride;

/* define the union reg_vid_2bit_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lm_tile_stride : 16;  /* [15..0]  */
        unsigned int chm_tile_stride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_2bit_stride;

/* define the union reg_vid_head_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lm_head_stride : 16;  /* [15..0]  */
        unsigned int chm_head_stride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_head_stride;

/* define the union reg_vid_smmu_bypass */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lm_bypass_2d : 1;  /* [0]  */
        unsigned int chm_bypass_2d : 1; /* [1]  */
        unsigned int lm_bypass_3d : 1;  /* [2]  */
        unsigned int chm_bypass_3d : 1; /* [3]  */
        unsigned int reserved_0 : 28;   /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_smmu_bypass;

/* define the union reg_vid_testpat_cfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tp_speed : 10;     /* [9..0]  */
        unsigned int reserved_0 : 2;    /* [11..10]  */
        unsigned int tp_line_w : 1;     /* [12]  */
        unsigned int tp_color_mode : 1; /* [13]  */
        unsigned int reserved_1 : 2;    /* [15..14]  */
        unsigned int tp_mode : 2;       /* [17..16]  */
        unsigned int reserved_2 : 14;   /* [31..18]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_testpat_cfg;

/* define the union reg_vid_testpat_seed */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tp_seed : 30;   /* [29..0]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_testpat_seed;

/* define the union reg_vid_tunl_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tunl_interval : 8; /* [7..0]  */
        unsigned int tunl_thd : 16;     /* [23..8]  */
        unsigned int reserved_0 : 6;    /* [29..24]  */
        unsigned int tunl_uf : 1;       /* [30]  */
        unsigned int rtunl_en : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_tunl_ctrl;

/* define the union reg_vid_tunl_crop */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tunl_crop_line : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_tunl_crop;

/* define the union reg_vid_tunl_errsta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int l_tunl_err : 1;  /* [0]  */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART36_H_ */
