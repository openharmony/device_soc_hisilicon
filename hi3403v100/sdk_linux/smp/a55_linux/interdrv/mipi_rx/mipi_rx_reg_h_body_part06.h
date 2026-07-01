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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART06_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART06_H_


    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize2_statis;

/* Define the union u_lvds_imgsize3_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth3 : 16;  /* [15..0]  */
        unsigned int lvds_imgheight3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize3_statis;

/* Define the union u_lvds_lane_orders0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_lane0_id : 4; /* [3..0]  */
        unsigned int lvds_lane1_id : 4; /* [7..4]  */
        unsigned int lvds_lane2_id : 4; /* [11..8]  */
        unsigned int lvds_lane3_id : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_orders0;

/* Define the union u_lvds_lane_orders1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_lane4_id : 4; /* [3..0]  */
        unsigned int lvds_lane5_id : 4; /* [7..4]  */
        unsigned int lvds_lane6_id : 4; /* [11..8]  */
        unsigned int lvds_lane7_id : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_orders1;

/* Define the union u_lvds_lane_orders2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_lane8_id : 4;  /* [3..0]  */
        unsigned int lvds_lane9_id : 4;  /* [7..4]  */
        unsigned int lvds_lane10_id : 4; /* [11..8]  */
        unsigned int lvds_lane11_id : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_orders2;

/* Define the union u_lvds_lane_orders3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_lane12_id : 4; /* [3..0]  */
        unsigned int lvds_lane13_id : 4; /* [7..4]  */
        unsigned int lvds_lane14_id : 4; /* [11..8]  */
        unsigned int lvds_lane15_id : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_orders3;

/* Define the union u_lvds_output_pix_num */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_double_pix_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;        /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_output_pix_num;

/* Define the union u_lvds_f1_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_f1_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;    /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_f1_en;

/* Define the union u_lvds_ctrl_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_sync_err_raw : 1;  /* [0]  */
        unsigned int lane1_sync_err_raw : 1;  /* [1]  */
        unsigned int lane2_sync_err_raw : 1;  /* [2]  */
        unsigned int lane3_sync_err_raw : 1;  /* [3]  */
        unsigned int lane4_sync_err_raw : 1;  /* [4]  */
        unsigned int lane5_sync_err_raw : 1;  /* [5]  */
        unsigned int lane6_sync_err_raw : 1;  /* [6]  */
        unsigned int lane7_sync_err_raw : 1;  /* [7]  */
        unsigned int lane8_sync_err_raw : 1;  /* [8]  */
        unsigned int lane9_sync_err_raw : 1;  /* [9]  */
        unsigned int lane10_sync_err_raw : 1; /* [10]  */
        unsigned int lane11_sync_err_raw : 1; /* [11]  */
        unsigned int lane12_sync_err_raw : 1; /* [12]  */
        unsigned int lane13_sync_err_raw : 1; /* [13]  */
        unsigned int lane14_sync_err_raw : 1; /* [14]  */
        unsigned int lane15_sync_err_raw : 1; /* [15]  */
        unsigned int link0_wr_err_raw : 1;    /* [16]  */
        unsigned int link1_wr_err_raw : 1;    /* [17]  */
        unsigned int link2_wr_err_raw : 1;    /* [18]  */
        unsigned int link3_wr_err_raw : 1;    /* [19]  */
        unsigned int link0_rd_err_raw : 1;    /* [20]  */
        unsigned int link1_rd_err_raw : 1;    /* [21]  */
        unsigned int link2_rd_err_raw : 1;    /* [22]  */
        unsigned int link3_rd_err_raw : 1;    /* [23]  */
        unsigned int lvds_state_err_raw : 1;  /* [24]  */
        unsigned int reserved_0 : 3;          /* [27..25]  */
        unsigned int lvds_vsync_raw : 1;      /* [28]  */
        unsigned int reserved_1 : 3;          /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_ctrl_int_raw;

/* Define the union u_lvds_ctrl_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_sync_err_st : 1;  /* [0]  */
        unsigned int lane1_sync_err_st : 1;  /* [1]  */
        unsigned int lane2_sync_err_st : 1;  /* [2]  */
        unsigned int lane3_sync_err_st : 1;  /* [3]  */
        unsigned int lane4_sync_err_st : 1;  /* [4]  */
        unsigned int lane5_sync_err_st : 1;  /* [5]  */
        unsigned int lane6_sync_err_st : 1;  /* [6]  */
        unsigned int lane7_sync_err_st : 1;  /* [7]  */
        unsigned int lane8_sync_err_st : 1;  /* [8]  */
        unsigned int lane9_sync_err_st : 1;  /* [9]  */
        unsigned int lane10_sync_err_st : 1; /* [10]  */
        unsigned int lane11_sync_err_st : 1; /* [11]  */
        unsigned int lane12_sync_err_st : 1; /* [12]  */
        unsigned int lane13_sync_err_st : 1; /* [13]  */
        unsigned int lane14_sync_err_st : 1; /* [14]  */
        unsigned int lane15_sync_err_st : 1; /* [15]  */
        unsigned int link0_wr_err_st : 1;    /* [16]  */
        unsigned int link1_wr_err_st : 1;    /* [17]  */
        unsigned int link2_wr_err_st : 1;    /* [18]  */
        unsigned int link3_wr_err_st : 1;    /* [19]  */
        unsigned int link0_rd_err_st : 1;    /* [20]  */
        unsigned int link1_rd_err_st : 1;    /* [21]  */
        unsigned int link2_rd_err_st : 1;    /* [22]  */
        unsigned int link3_rd_err_st : 1;    /* [23]  */
        unsigned int lvds_state_err_st : 1;  /* [24]  */
        unsigned int reserved_0 : 3;         /* [27..25]  */
        unsigned int lvds_vsync_st : 1;      /* [28]  */
        unsigned int reserved_1 : 3;         /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_ctrl_int;

/* Define the union u_lvds_ctrl_int_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_sync_err_msk : 1;  /* [0]  */
        unsigned int lane1_sync_err_msk : 1;  /* [1]  */
        unsigned int lane2_sync_err_msk : 1;  /* [2]  */
        unsigned int lane3_sync_err_msk : 1;  /* [3]  */
        unsigned int lane4_sync_err_msk : 1;  /* [4]  */
        unsigned int lane5_sync_err_msk : 1;  /* [5]  */
        unsigned int lane6_sync_err_msk : 1;  /* [6]  */
        unsigned int lane7_sync_err_msk : 1;  /* [7]  */
        unsigned int lane8_sync_err_msk : 1;  /* [8]  */
        unsigned int lane9_sync_err_msk : 1;  /* [9]  */
        unsigned int lane10_sync_err_msk : 1; /* [10]  */
        unsigned int lane11_sync_err_msk : 1; /* [11]  */
        unsigned int lane12_sync_err_msk : 1; /* [12]  */
        unsigned int lane13_sync_err_msk : 1; /* [13]  */
        unsigned int lane14_sync_err_msk : 1; /* [14]  */
        unsigned int lane15_sync_err_msk : 1; /* [15]  */
        unsigned int link0_wr_err_msk : 1;    /* [16]  */
        unsigned int link1_wr_err_msk : 1;    /* [17]  */
        unsigned int link2_wr_err_msk : 1;    /* [18]  */
        unsigned int link3_wr_err_msk : 1;    /* [19]  */
        unsigned int link0_rd_err_msk : 1;    /* [20]  */
        unsigned int link1_rd_err_msk : 1;    /* [21]  */
        unsigned int link2_rd_err_msk : 1;    /* [22]  */
        unsigned int link3_rd_err_msk : 1;    /* [23]  */
        unsigned int lvds_state_err_msk : 1;  /* [24]  */
        unsigned int reserved_0 : 3;          /* [27..25]  */
        unsigned int lvds_vsync_msk : 1;      /* [28]  */
        unsigned int reserved_1 : 3;          /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_ctrl_int_msk;

/* Define the union u_lane_id0_chn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_id : 4;    /* [3..0]  */
        unsigned int lane1_id : 4;    /* [7..4]  */
        unsigned int lane2_id : 4;    /* [11..8]  */
        unsigned int lane3_id : 4;    /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_id0_chn;

/* Define the union u_lane_id1_chn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane4_id : 4;    /* [3..0]  */
        unsigned int lane5_id : 4;    /* [7..4]  */
        unsigned int lane6_id : 4;    /* [11..8]  */
        unsigned int lane7_id : 4;    /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_id1_chn;

/* Define the union u_lane_id2_chn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane8_id : 4;    /* [3..0]  */
        unsigned int lane9_id : 4;    /* [7..4]  */
        unsigned int lane10_id : 4;   /* [11..8]  */
        unsigned int lane11_id : 4;   /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_id2_chn;

/* Define the union u_lane_id3_chn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane12_id : 4;   /* [3..0]  */
        unsigned int lane13_id : 4;   /* [7..4]  */
        unsigned int lane14_id : 4;   /* [11..8]  */
        unsigned int lane15_id : 4;   /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_id3_chn;

/* Define the union u_align_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_full_raw : 1;   /* [0]  */
        unsigned int err_lane0_raw : 1;  /* [1]  */
        unsigned int err_lane1_raw : 1;  /* [2]  */
        unsigned int err_lane2_raw : 1;  /* [3]  */
        unsigned int err_lane3_raw : 1;  /* [4]  */
        unsigned int err_lane4_raw : 1;  /* [5]  */
        unsigned int err_lane5_raw : 1;  /* [6]  */
        unsigned int err_lane6_raw : 1;  /* [7]  */
        unsigned int err_lane7_raw : 1;  /* [8]  */
        unsigned int err_lane8_raw : 1;  /* [9]  */
        unsigned int err_lane9_raw : 1;  /* [10]  */
        unsigned int err_lane10_raw : 1; /* [11]  */
        unsigned int err_lane11_raw : 1; /* [12]  */
        unsigned int err_lane12_raw : 1; /* [13]  */
        unsigned int err_lane13_raw : 1; /* [14]  */
        unsigned int err_lane14_raw : 1; /* [15]  */
        unsigned int err_lane15_raw : 1; /* [16]  */
        unsigned int reserved_0 : 15;    /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_align_int_raw;

/* Define the union u_align_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_full_st : 1;   /* [0]  */
        unsigned int err_lane0_st : 1;  /* [1]  */
        unsigned int err_lane1_st : 1;  /* [2]  */
        unsigned int err_lane2_st : 1;  /* [3]  */
        unsigned int err_lane3_st : 1;  /* [4]  */
        unsigned int err_lane4_st : 1;  /* [5]  */
        unsigned int err_lane5_st : 1;  /* [6]  */
        unsigned int err_lane6_st : 1;  /* [7]  */
        unsigned int err_lane7_st : 1;  /* [8]  */
        unsigned int err_lane8_st : 1;  /* [9]  */
        unsigned int err_lane9_st : 1;  /* [10]  */
        unsigned int err_lane10_st : 1; /* [11]  */
        unsigned int err_lane11_st : 1; /* [12]  */
        unsigned int err_lane12_st : 1; /* [13]  */
        unsigned int err_lane13_st : 1; /* [14]  */
        unsigned int err_lane14_st : 1; /* [15]  */
        unsigned int err_lane15_st : 1; /* [16]  */
        unsigned int reserved_0 : 15;   /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_align_int;

/* Define the union u_align_int_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_full_mask : 1;   /* [0]  */
        unsigned int err_lane0_mask : 1;  /* [1]  */
        unsigned int err_lane1_mask : 1;  /* [2]  */
        unsigned int err_lane2_mask : 1;  /* [3]  */
        unsigned int err_lane3_mask : 1;  /* [4]  */
        unsigned int err_lane4_mask : 1;  /* [5]  */
        unsigned int err_lane5_mask : 1;  /* [6]  */
        unsigned int err_lane6_mask : 1;  /* [7]  */
        unsigned int err_lane7_mask : 1;  /* [8]  */
        unsigned int err_lane8_mask : 1;  /* [9]  */
        unsigned int err_lane9_mask : 1;  /* [10]  */
        unsigned int err_lane10_mask : 1; /* [11]  */
        unsigned int err_lane11_mask : 1; /* [12]  */
        unsigned int err_lane12_mask : 1; /* [13]  */
        unsigned int err_lane13_mask : 1; /* [14]  */
        unsigned int err_lane14_mask : 1; /* [15]  */
        unsigned int err_lane15_mask : 1; /* [16]  */
        unsigned int reserved_0 : 15;     /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_align_int_msk;

/* Define the union u_chn_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_lvds_ctrl_raw : 1;  /* [0]  */
        unsigned int int_mipi_csi_raw : 1;   /* [1]  */
        unsigned int int_mipi_ctrl_raw : 1;  /* [2]  */
        unsigned int int_data_align_raw : 1; /* [3]  */
        unsigned int reserved_0 : 28;        /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn_int_raw;

/* Define the union u_chn_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_lvds_ctrl_st : 1;  /* [0]  */
        unsigned int int_mipi_csi_st : 1;   /* [1]  */
        unsigned int int_mipi_ctrl_st : 1;  /* [2]  */
        unsigned int int_data_align_st : 1; /* [3]  */
        unsigned int reserved_0 : 28;       /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn_int;

/* Define the union u_chn_int_mask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_lvds_ctrl_mask : 1;  /* [0]  */
        unsigned int int_mipi_csi_mask : 1;   /* [1]  */
        unsigned int int_mipi_ctrl_mask : 1;  /* [2]  */
        unsigned int int_data_align_mask : 1; /* [3]  */
        unsigned int reserved_0 : 28;         /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn_int_mask;

// ==============================================================================
/* Define the global struct */
typedef struct {
    u_phy_mode_link phy_mode_link;                        /* 0x0 */
    u_phy_skew_link phy_skew_link;                        /* 0x4 */
    u_phy_lane_si_link phy_lane_si_link;                  /* 0x8 */
    u_phy_en_link phy_en_link;                            /* 0xc */
    u_phy_eq_link phy_eq_link;                            /* 0x10 */
    u_phy_cfg_link phy_cfg_link;                          /* 0x14 */
    u_phy_data_link phy_data_link;                        /* 0x18 */
    u_phy_ph_mipi_link phy_ph_mipi_link;                  /* 0x1c */
    u_phy_data_mipi_link phy_data_mipi_link;              /* 0x20 */
    u_phy_sync_dct_link phy_sync_dct_link;                /* 0x24 */
    u_phy_cmos_dir phy_cmos_dir;                          /* 0x28 */
    unsigned int reserved_0;                              /* 0x2c */
    u_phy_sync_sof0_link phy_sync_sof0_link;              /* 0x30 */
    u_phy_sync_sof1_link phy_sync_sof1_link;              /* 0x34 */
    u_phy_sync_sof2_link phy_sync_sof2_link;              /* 0x38 */
    u_phy_sync_sof3_link phy_sync_sof3_link;              /* 0x3c */
    u_phy_out_valid_link phy_out_valid_link;              /* 0x40 */
    u_phy_data_lvds_link phy_data_lvds_link;              /* 0x44 */
    unsigned int reserved_1[2];                           /* 0x48~0x4c */
    u_phy_deskew_cal_link phy_deskew_cal_link;            /* 0x50 */
    u_phy_pix_pum_link phy_pix_pum_link;                  /* 0x54 */
    unsigned int reserved_2[2];                           /* 0x58~0x5c */
    u_phy_sync_sol0_link phy_sync_sol0_link;              /* 0x60 */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART06_H_ */
