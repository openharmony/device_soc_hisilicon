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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART05_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART05_H_

    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_dolscd_hblk;

/* Define the union u_lvds_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_sync_mode : 1;       /* [0]  */
        unsigned int reserved_0 : 3;           /* [3..1]  */
        unsigned int lvds_raw_type : 3;        /* [6..4]  */
        unsigned int reserved_1 : 1;           /* [7]  */
        unsigned int lvds_pix_big_endian : 1;  /* [8]  */
        unsigned int lvds_code_big_endian : 1; /* [9]  */
        unsigned int reserved_2 : 2;           /* [11..10]  */
        unsigned int lvds_crop_en : 1;         /* [12]  */
        unsigned int reserved_3 : 3;           /* [15..13]  */
        unsigned int lvds_split_mode : 3;      /* [18..16]  */
        unsigned int reserved_4 : 13;          /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_ctrl;

/* Define the union u_lvds_imgsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth_lane : 16; /* [15..0]  */
        unsigned int lvds_imgheight : 16;     /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize;

/* Define the union u_lvds_crop_start0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_start_x0_lane : 16; /* [15..0]  */
        unsigned int lvds_start_y0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_crop_start0;

/* Define the union u_lvds_crop_start1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_start_x1_lane : 16; /* [15..0]  */
        unsigned int lvds_start_y1 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_crop_start1;

/* Define the union u_lvds_crop_start2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_start_x2_lane : 16; /* [15..0]  */
        unsigned int lvds_start_y2 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_crop_start2;

/* Define the union u_lvds_crop_start3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_start_x3_lane : 16; /* [15..0]  */
        unsigned int lvds_start_y3 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_crop_start3;

/* Define the union u_lvds_lane_sof_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_sof_0 : 16; /* [15..0]  */
        unsigned int lane_sof_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_sof_01;

/* Define the union u_lvds_lane_sof_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_sof_2 : 16; /* [15..0]  */
        unsigned int lane_sof_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_sof_23;

/* Define the union u_lvds_lane_eof_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_eof_0 : 16; /* [15..0]  */
        unsigned int lane_eof_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_eof_01;

/* Define the union u_lvds_lane_eof_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_eof_2 : 16; /* [15..0]  */
        unsigned int lane_eof_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_eof_23;

/* Define the union u_lvds_lane_sol_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_sol_0 : 16; /* [15..0]  */
        unsigned int lane_sol_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_sol_01;

/* Define the union u_lvds_lane_sol_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_sol_2 : 16; /* [15..0]  */
        unsigned int lane_sol_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_sol_23;

/* Define the union u_lvds_lane_eol_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_eol_0 : 16; /* [15..0]  */
        unsigned int lane_eol_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_eol_01;

/* Define the union u_lvds_lane_eol_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_eol_2 : 16; /* [15..0]  */
        unsigned int lane_eol_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_eol_23;

/* Define the union u_lvds_lane_nxt_sof_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_sof_0 : 16; /* [15..0]  */
        unsigned int lane_nxt_sof_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_sof_01;

/* Define the union u_lvds_lane_nxt_sof_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_sof_2 : 16; /* [15..0]  */
        unsigned int lane_nxt_sof_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_sof_23;

/* Define the union u_lvds_lane_nxt_eof_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_eof_0 : 16; /* [15..0]  */
        unsigned int lane_nxt_eof_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_eof_01;

/* Define the union u_lvds_lane_nxt_eof_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_eof_2 : 16; /* [15..0]  */
        unsigned int lane_nxt_eof_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_eof_23;

/* Define the union u_lvds_lane_nxt_sol_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_sol_0 : 16; /* [15..0]  */
        unsigned int lane_nxt_sol_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_sol_01;

/* Define the union u_lvds_lane_nxt_sol_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_sol_2 : 16; /* [15..0]  */
        unsigned int lane_nxt_sol_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_sol_23;

/* Define the union u_lvds_lane_nxt_eol_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_eol_0 : 16; /* [15..0]  */
        unsigned int lane_nxt_eol_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_eol_01;

/* Define the union u_lvds_lane_nxt_eol_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_eol_2 : 16; /* [15..0]  */
        unsigned int lane_nxt_eol_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_eol_23;

/* Define the union u_lvds_li_word0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int li_word0_0 : 16; /* [15..0]  */
        unsigned int li_word0_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_li_word0;

/* Define the union u_lvds_li_word1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int li_word1_0 : 16; /* [15..0]  */
        unsigned int li_word1_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_li_word1;

/* Define the union u_lvds_li_word2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int li_word2_0 : 16; /* [15..0]  */
        unsigned int li_word2_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_li_word2;

/* Define the union u_lvds_li_word3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int li_word3_0 : 16; /* [15..0]  */
        unsigned int li_word3_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_li_word3;

/* Define the union u_lvds_sync_bord0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_bord0_lane0 : 4; /* [3..0]  */
        unsigned int sync_bord0_lane1 : 4; /* [7..4]  */
        unsigned int sync_bord0_lane2 : 4; /* [11..8]  */
        unsigned int sync_bord0_lane3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_sync_bord0;

/* Define the union u_lvds_sync_bord1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_bord1_lane0 : 4; /* [3..0]  */
        unsigned int sync_bord1_lane1 : 4; /* [7..4]  */
        unsigned int sync_bord1_lane2 : 4; /* [11..8]  */
        unsigned int sync_bord1_lane3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_sync_bord1;

/* Define the union u_lvds_sync_bord2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_bord2_lane0 : 4; /* [3..0]  */
        unsigned int sync_bord2_lane1 : 4; /* [7..4]  */
        unsigned int sync_bord2_lane2 : 4; /* [11..8]  */
        unsigned int sync_bord2_lane3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_sync_bord2;

/* Define the union u_lvds_sync_bord3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_bord3_lane0 : 4; /* [3..0]  */
        unsigned int sync_bord3_lane1 : 4; /* [7..4]  */
        unsigned int sync_bord3_lane2 : 4; /* [11..8]  */
        unsigned int sync_bord3_lane3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_sync_bord3;

/* Define the union u_lvds_lane_imgsize_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_imgwidth : 16;  /* [15..0]  */
        unsigned int lane_imgheight : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_imgsize_statis;

/* Define the union u_lvds_imgsize0_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth0 : 16;  /* [15..0]  */
        unsigned int lvds_imgheight0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize0_statis;

/* Define the union u_lvds_imgsize1_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth1 : 16;  /* [15..0]  */
        unsigned int lvds_imgheight1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize1_statis;

/* Define the union u_lvds_imgsize2_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth2 : 16;  /* [15..0]  */
        unsigned int lvds_imgheight2 : 16; /* [31..16]  */
    } bits;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART05_H_ */
