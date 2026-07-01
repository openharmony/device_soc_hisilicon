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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART03_H_

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phycil0_cfg_en : 1; /* [0]  */
        unsigned int phycil1_cfg_en : 1; /* [1]  */
        unsigned int phycil2_cfg_en : 1; /* [2]  */
        unsigned int phycil3_cfg_en : 1; /* [3]  */
        unsigned int reserved_0 : 28;    /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phycfg_en;

/* Define the union u_chn0_mem_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn0_mem_ck_gt : 1;  /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int chn0_mem_rma : 4;    /* [7..4]  */
        unsigned int chn0_mem_test1a : 1; /* [8]  */
        unsigned int chn0_mem_rmea : 1;   /* [9]  */
        unsigned int reserved_1 : 2;      /* [11..10]  */
        unsigned int chn0_mem_rmb : 4;    /* [15..12]  */
        unsigned int chn0_mem_test1b : 1; /* [16]  */
        unsigned int chn0_mem_rmeb : 1;   /* [17]  */
        unsigned int reserved_2 : 14;     /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn0_mem_ctrl;

/* Define the union u_chn0_clr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn0_clr_en_lvds : 1;  /* [0]  */
        unsigned int chn0_clr_en_align : 1; /* [1]  */
        unsigned int reserved_0 : 30;       /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn0_clr_en;

/* Define the union u_chn1_mem_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn1_mem_ck_gt : 1;  /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int chn1_mem_rma : 4;    /* [7..4]  */
        unsigned int chn1_mem_test1a : 1; /* [8]  */
        unsigned int chn1_mem_rmea : 1;   /* [9]  */
        unsigned int reserved_1 : 2;      /* [11..10]  */
        unsigned int chn1_mem_rmb : 4;    /* [15..12]  */
        unsigned int chn1_mem_test1b : 1; /* [16]  */
        unsigned int chn1_mem_rmeb : 1;   /* [17]  */
        unsigned int reserved_2 : 14;     /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn1_mem_ctrl;

/* Define the union u_chn1_clr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn1_clr_en_lvds : 1;  /* [0]  */
        unsigned int chn1_clr_en_align : 1; /* [1]  */
        unsigned int reserved_0 : 30;       /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn1_clr_en;

/* Define the union u_chn2_mem_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn2_mem_ck_gt : 1;  /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int chn2_mem_rma : 4;    /* [7..4]  */
        unsigned int chn2_mem_test1a : 1; /* [8]  */
        unsigned int chn2_mem_rmea : 1;   /* [9]  */
        unsigned int reserved_1 : 2;      /* [11..10]  */
        unsigned int chn2_mem_rmb : 4;    /* [15..12]  */
        unsigned int chn2_mem_test1b : 1; /* [16]  */
        unsigned int chn2_mem_rmeb : 1;   /* [17]  */
        unsigned int reserved_2 : 14;     /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn2_mem_ctrl;

/* Define the union u_chn2_clr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn2_clr_en_lvds : 1;  /* [0]  */
        unsigned int chn2_clr_en_align : 1; /* [1]  */
        unsigned int reserved_0 : 30;       /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn2_clr_en;

/* Define the union u_chn3_mem_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn3_mem_ck_gt : 1;  /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int chn3_mem_rma : 4;    /* [7..4]  */
        unsigned int chn3_mem_test1a : 1; /* [8]  */
        unsigned int chn3_mem_rmea : 1;   /* [9]  */
        unsigned int reserved_1 : 2;      /* [11..10]  */
        unsigned int chn3_mem_rmb : 4;    /* [15..12]  */
        unsigned int chn3_mem_test1b : 1; /* [16]  */
        unsigned int chn3_mem_rmeb : 1;   /* [17]  */
        unsigned int reserved_2 : 14;     /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn3_mem_ctrl;

/* Define the union u_chn3_clr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn3_clr_en_lvds : 1;  /* [0]  */
        unsigned int chn3_clr_en_align : 1; /* [1]  */
        unsigned int reserved_0 : 30;       /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn3_clr_en;

/* Define the union u_mipi_test_phy */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy_en : 1;       /* [0]  */
        unsigned int reserved_0 : 3;        /* [3..1]  */
        unsigned int test_phy_srst_req : 1; /* [4]  */
        unsigned int reserved_1 : 27;       /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_test_phy;

/* Define the union u_colorbar_ctrl_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_en : 1;        /* [0]  */
        unsigned int colorbar_mode : 1;      /* [1]  */
        unsigned int colorbar_pattern : 1;   /* [2]  */
        unsigned int colorbar_line_init : 1; /* [3]  */
        unsigned int colorbar_width : 10;    /* [13..4]  */
        unsigned int reserved_0 : 18;        /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_ctrl_link0;

/* Define the union u_colorbar_init_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_data_init : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;          /* [15..12]  */
        unsigned int colorbar_data_inc : 12;  /* [27..16]  */
        unsigned int reserved_1 : 4;          /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_init_link0;

/* Define the union u_colorbar_v_blk_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_vertical_blk : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;            /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_v_blk_link0;

/* Define the union u_colorbar_h_blk_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_line_blk : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_h_blk_link0;

/* Define the union u_colorbar_img_size_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_imgwidth : 16;  /* [15..0]  */
        unsigned int colorbar_imgheight : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_img_size_link0;

/* Define the union u_colorbar_sync_code0_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_sof : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;    /* [15..12]  */
        unsigned int colorbar_eof : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;    /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_sync_code0_link0;

/* Define the union u_colorbar_sync_code1_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_sol : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;    /* [15..12]  */
        unsigned int colorbar_eol : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;    /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_sync_code1_link0;

/* Define the union u_phy0_test_out */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy0_resault : 4; /* [3..0]  */
        unsigned int test_phy0_compare : 4; /* [7..4]  */
        unsigned int test_phy0_done : 4;    /* [11..8]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy0_test_out;

/* Define the union u_phy1_test_out */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy1_resault : 4; /* [3..0]  */
        unsigned int test_phy1_compare : 4; /* [7..4]  */
        unsigned int test_phy1_done : 4;    /* [11..8]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy1_test_out;

/* Define the union u_phy2_test_out */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy2_resault : 4; /* [3..0]  */
        unsigned int test_phy2_compare : 4; /* [7..4]  */
        unsigned int test_phy2_done : 4;    /* [11..8]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy2_test_out;

/* Define the union u_phy3_test_out */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy3_resault : 4; /* [3..0]  */
        unsigned int test_phy3_compare : 4; /* [7..4]  */
        unsigned int test_phy3_done : 4;    /* [11..8]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy3_test_out;

/* Define the union u_lpdt_ph_addr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int lpdt_packet_header : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lpdt_ph_addr;
/* Define the union u_lpdt_check_res_addr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lpdt_crc_err : 1; /* [0]  */
        unsigned int lpdt_ecc_err : 1; /* [1]  */
        unsigned int reserved_0 : 30;  /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lpdt_check_res_addr;

/* Define the union u_mipi_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_phycil0_raw : 1; /* [0]  */
        unsigned int int_phycil1_raw : 1; /* [1]  */
        unsigned int int_phycil2_raw : 1; /* [2]  */
        unsigned int int_phycil3_raw : 1; /* [3]  */
        unsigned int int_chn0_raw : 1;    /* [4]  */
        unsigned int int_chn1_raw : 1;    /* [5]  */
        unsigned int int_chn2_raw : 1;    /* [6]  */
        unsigned int int_chn3_raw : 1;    /* [7]  */
        unsigned int int_chn4_raw : 1;    /* [8]  */
        unsigned int int_chn5_raw : 1;    /* [9]  */
        unsigned int int_chn6_raw : 1;    /* [10]  */
        unsigned int int_chn7_raw : 1;    /* [11]  */
        unsigned int reserved_0 : 20;     /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_int_raw;

/* Define the union u_mipi_int_st */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_phycil0_st : 1; /* [0]  */
        unsigned int int_phycil1_st : 1; /* [1]  */
        unsigned int int_phycil2_st : 1; /* [2]  */
        unsigned int int_phycil3_st : 1; /* [3]  */
        unsigned int int_chn0_st : 1;    /* [4]  */
        unsigned int int_chn1_st : 1;    /* [5]  */
        unsigned int int_chn2_st : 1;    /* [6]  */
        unsigned int int_chn3_st : 1;    /* [7]  */
        unsigned int int_chn4_st : 1;    /* [8]  */
        unsigned int int_chn5_st : 1;    /* [9]  */
        unsigned int int_chn6_st : 1;    /* [10]  */
        unsigned int int_chn7_st : 1;    /* [11]  */
        unsigned int reserved_0 : 20;    /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_int_st;

/* Define the union u_mipi_int_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_phycil0_mask : 1; /* [0]  */
        unsigned int int_phycil1_mask : 1; /* [1]  */
        unsigned int int_phycil2_mask : 1; /* [2]  */
        unsigned int int_phycil3_mask : 1; /* [3]  */
        unsigned int int_chn0_mask : 1;    /* [4]  */
        unsigned int int_chn1_mask : 1;    /* [5]  */
        unsigned int int_chn2_mask : 1;    /* [6]  */
        unsigned int int_chn3_mask : 1;    /* [7]  */
        unsigned int int_chn4_mask : 1;    /* [8]  */
        unsigned int int_chn5_mask : 1;    /* [9]  */
        unsigned int int_chn6_mask : 1;    /* [10]  */
        unsigned int int_chn7_mask : 1;    /* [11]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_int_msk;

/* Define the union u_mipi_interlaced */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_csi_interlaced : 1; /* [0]  */
        unsigned int reserved_0 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_interlaced;

/* Define the union u_mipi_frame_num0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_frame_num_vc0 : 16; /* [15..0]  */
        unsigned int mipi_frame_num_vc1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_frame_num0;

/* Define the union u_mipi_frame_num1 */
typedef union {
    /* Define the struct bits */
    struct {


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART03_H_ */
