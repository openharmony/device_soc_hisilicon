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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART04_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART04_H_

        unsigned int mipi_frame_num_vc2 : 16; /* [15..0]  */
        unsigned int mipi_frame_num_vc3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_frame_num1;

/* Define the union u_mipi_crc_intr_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int crc_err_vc0_raw : 1;                /* [0]  */
        unsigned int crc_err_vc1_raw : 1;                /* [1]  */
        unsigned int crc_err_vc2_raw : 1;                /* [2]  */
        unsigned int crc_err_vc3_raw : 1;                /* [3]  */
        unsigned int ecc_err_vc0_raw : 1;                /* [4]  */
        unsigned int ecc_err_vc1_raw : 1;                /* [5]  */
        unsigned int ecc_err_vc2_raw : 1;                /* [6]  */
        unsigned int ecc_err_vc3_raw : 1;                /* [7]  */
        unsigned int ecc_err_mult_raw : 1;               /* [8]  */
        unsigned int reserved_0 : 7;                     /* [15..9]  */
        unsigned int frame_num_err_vc0_raw : 1;          /* [16]  */
        unsigned int frame_num_err_vc1_raw : 1;          /* [17]  */
        unsigned int frame_num_err_vc2_raw : 1;          /* [18]  */
        unsigned int frame_num_err_vc3_raw : 1;          /* [19]  */
        unsigned int frame_s_e_num_mismatch_vc0_raw : 1; /* [20]  */
        unsigned int frame_s_e_num_mismatch_vc1_raw : 1; /* [21]  */
        unsigned int frame_s_e_num_mismatch_vc2_raw : 1; /* [22]  */
        unsigned int frame_s_e_num_mismatch_vc3_raw : 1; /* [23]  */
        unsigned int reserved_1 : 8;                     /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crc_intr_raw;

/* Define the union u_mipi_crc_intr_st */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int crc_err_vc0_st : 1;                /* [0]  */
        unsigned int crc_err_vc1_st : 1;                /* [1]  */
        unsigned int crc_err_vc2_st : 1;                /* [2]  */
        unsigned int crc_err_vc3_st : 1;                /* [3]  */
        unsigned int ecc_err_vc0_st : 1;                /* [4]  */
        unsigned int ecc_err_vc1_st : 1;                /* [5]  */
        unsigned int ecc_err_vc2_st : 1;                /* [6]  */
        unsigned int ecc_err_vc3_st : 1;                /* [7]  */
        unsigned int ecc_err_mult_st : 1;               /* [8]  */
        unsigned int reserved_0 : 7;                    /* [15..9]  */
        unsigned int frame_num_err_vc0_st : 1;          /* [16]  */
        unsigned int frame_num_err_vc1_st : 1;          /* [17]  */
        unsigned int frame_num_err_vc2_st : 1;          /* [18]  */
        unsigned int frame_num_err_vc3_st : 1;          /* [19]  */
        unsigned int frame_s_e_num_mismatch_vc0_st : 1; /* [20]  */
        unsigned int frame_s_e_num_mismatch_vc1_st : 1; /* [21]  */
        unsigned int frame_s_e_num_mismatch_vc2_st : 1; /* [22]  */
        unsigned int frame_s_e_num_mismatch_vc3_st : 1; /* [23]  */
        unsigned int reserved_1 : 8;                    /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crc_intr_st;

/* Define the union u_mipi_crc_intr_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int crc_err_vc0_msk : 1;                /* [0]  */
        unsigned int crc_err_vc1_msk : 1;                /* [1]  */
        unsigned int crc_err_vc2_msk : 1;                /* [2]  */
        unsigned int crc_err_vc3_msk : 1;                /* [3]  */
        unsigned int ecc_err_vc0_msk : 1;                /* [4]  */
        unsigned int ecc_err_vc1_msk : 1;                /* [5]  */
        unsigned int ecc_err_vc2_msk : 1;                /* [6]  */
        unsigned int ecc_err_vc3_msk : 1;                /* [7]  */
        unsigned int ecc_err_mult_msk : 1;               /* [8]  */
        unsigned int reserved_0 : 7;                     /* [15..9]  */
        unsigned int frame_num_err_vc0_msk : 1;          /* [16]  */
        unsigned int frame_num_err_vc1_msk : 1;          /* [17]  */
        unsigned int frame_num_err_vc2_msk : 1;          /* [18]  */
        unsigned int frame_num_err_vc3_msk : 1;          /* [19]  */
        unsigned int frame_s_e_num_mismatch_vc0_msk : 1; /* [20]  */
        unsigned int frame_s_e_num_mismatch_vc1_msk : 1; /* [21]  */
        unsigned int frame_s_e_num_mismatch_vc2_msk : 1; /* [22]  */
        unsigned int frame_s_e_num_mismatch_vc3_msk : 1; /* [23]  */
        unsigned int reserved_1 : 8;                     /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crc_intr_msk;

/* Define the union u_mipi_userdef_dt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int user_def0_dt : 3; /* [2..0]  */
        unsigned int reserved_0 : 1;   /* [3]  */
        unsigned int user_def1_dt : 3; /* [6..4]  */
        unsigned int reserved_1 : 1;   /* [7]  */
        unsigned int user_def2_dt : 3; /* [10..8]  */
        unsigned int reserved_2 : 1;   /* [11]  */
        unsigned int user_def3_dt : 3; /* [14..12]  */
        unsigned int reserved_3 : 17;  /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_userdef_dt;

/* Define the union u_mipi_user_def */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int user_def0 : 6;  /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int user_def1 : 6;  /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int user_def2 : 6;  /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int user_def3 : 6;  /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_user_def;

/* Define the union u_mipi_ctrl_mode_hs */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hdr_mode : 1;       /* [0]  */
        unsigned int reserved_0 : 3;     /* [3..1]  */
        unsigned int vc_mode : 1;        /* [4]  */
        unsigned int reserved_1 : 3;     /* [7..5]  */
        unsigned int user_def_en : 1;    /* [8]  */
        unsigned int reserved_2 : 3;     /* [11..9]  */
        unsigned int lane0_location : 3; /* [14..12]  */
        unsigned int reserved_3 : 17;    /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_mode_hs;

/* Define the union u_mipi_vhend_delay */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vend_delay : 16; /* [15..0]  */
        unsigned int hend_delay : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_vhend_delay;

/* Define the union u_mipi_dol_id_code0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int id_code_reg0 : 16; /* [15..0]  */
        unsigned int id_code_reg1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_dol_id_code0;

/* Define the union u_mipi_dol_id_code1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int id_code_reg2 : 16; /* [15..0]  */
        unsigned int id_code_reg3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_dol_id_code1;

/* Define the union u_mipi_dol_id_code2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int id_code_reg4 : 16; /* [15..0]  */
        unsigned int id_code_reg5 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_dol_id_code2;

/* Define the union u_mipi_crop_start_chn0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_start_x_chn0 : 16; /* [15..0]  */
        unsigned int mipi_start_y_chn0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crop_start_chn0;

/* Define the union u_mipi_crop_start_chn1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_start_x_chn1 : 16; /* [15..0]  */
        unsigned int mipi_start_y_chn1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crop_start_chn1;

/* Define the union u_mipi_crop_start_chn2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_start_x_chn2 : 16; /* [15..0]  */
        unsigned int mipi_start_y_chn2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crop_start_chn2;

/* Define the union u_mipi_crop_start_chn3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_start_x_chn3 : 16; /* [15..0]  */
        unsigned int mipi_start_y_chn3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crop_start_chn3;

/* Define the union u_mipi_imgsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_imgwidth : 16;  /* [15..0]  */
        unsigned int mipi_imgheight : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize;

/* Define the union u_mipi_ctrl_mode_pixel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int crop_en : 1;                  /* [0]  */
        unsigned int mipi_wdr_num : 2;             /* [2..1]  */
        unsigned int memory_hold_en : 1;           /* [3]  */
        unsigned int mipi_dol_mode : 1;            /* [4]  */
        unsigned int reserved_0 : 3;               /* [7..5]  */
        unsigned int rx2mode : 1;                  /* [8]  */
        unsigned int data_merge_mode : 2;          /* [10..9]  */
        unsigned int sync_clear_en : 1;            /* [11]  */
        unsigned int stagger_hdr_mode : 1;         /* [12]  */
        unsigned int stagger_frm_num : 2;          /* [14..13]  */
        unsigned int dummy_line_detect : 1;        /* [15]  */
        unsigned int mipi_double_pix_en : 1;       /* [16]  */
        unsigned int mipi_double_yuv_en : 1;       /* [17]  */
        unsigned int mipi_yuv_422_en : 1;          /* [18]  */
        unsigned int mipi_yuv_420_nolegacy_en : 1; /* [19]  */
        unsigned int mipi_yuv_420_legacy_en : 1;   /* [20]  */
        unsigned int reserved_1 : 11;              /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_mode_pixel;

/* Define the union u_mipi_dummy_pix_reg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dummy_pix_reg : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_dummy_pix_reg;

/* Define the union u_mipi_imgsize0_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int imgwidth_statis_vc0 : 16;  /* [15..0]  */
        unsigned int imgheight_statis_vc0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize0_statis;

/* Define the union u_mipi_imgsize1_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int imgwidth_statis_vc1 : 16;  /* [15..0]  */
        unsigned int imgheight_statis_vc1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize1_statis;

/* Define the union u_mipi_imgsize2_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int imgwidth_statis_vc2 : 16;  /* [15..0]  */
        unsigned int imgheight_statis_vc2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize2_statis;

/* Define the union u_mipi_imgsize3_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int imgwidth_statis_vc3 : 16;  /* [15..0]  */
        unsigned int imgheight_statis_vc3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize3_statis;

/* Define the union u_mipi_ctrl_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_cfifo_wrerr_raw : 1;      /* [0]  */
        unsigned int int_dfifo_wrerr_raw : 1;      /* [1]  */
        unsigned int reserved_0 : 2;               /* [3..2]  */
        unsigned int int_vsync_raw : 1;            /* [4]  */
        unsigned int reserved_1 : 3;               /* [7..5]  */
        unsigned int memory_noempty_write_raw : 1; /* [8]  */
        unsigned int cmdfifo_noempty_raw : 1;      /* [9]  */
        unsigned int reserved_2 : 6;               /* [15..10]  */
        unsigned int int_cfifo_rderr_raw : 1;      /* [16]  */
        unsigned int int_dfifo_rderr_raw : 1;      /* [17]  */
        unsigned int reserved_3 : 14;              /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_int_raw;

/* Define the union u_mipi_ctrl_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_cfifo_wrerr_st : 1;      /* [0]  */
        unsigned int int_dfifo_wrerr_st : 1;      /* [1]  */
        unsigned int reserved_0 : 2;              /* [3..2]  */
        unsigned int int_vsync_st : 1;            /* [4]  */
        unsigned int reserved_1 : 3;              /* [7..5]  */
        unsigned int memory_noempty_write_st : 1; /* [8]  */
        unsigned int cmdfifo_noempty_st : 1;      /* [9]  */
        unsigned int reserved_2 : 6;              /* [15..10]  */
        unsigned int int_cfifo_rderr_st : 1;      /* [16]  */
        unsigned int int_dfifo_rderr_st : 1;      /* [17]  */
        unsigned int reserved_3 : 14;             /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_int;

/* Define the union u_mipi_ctrl_int_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_cfifo_wrerr_msk : 1;      /* [0]  */
        unsigned int int_dfifo_wrerr_msk : 1;      /* [1]  */
        unsigned int reserved_0 : 2;               /* [3..2]  */
        unsigned int int_vsync_msk : 1;            /* [4]  */
        unsigned int reserved_1 : 3;               /* [7..5]  */
        unsigned int memory_noempty_write_msk : 1; /* [8]  */
        unsigned int cmdfifo_noempty_msk : 1;      /* [9]  */
        unsigned int reserved_2 : 6;               /* [15..10]  */
        unsigned int int_cfifo_rderr_msk : 1;      /* [16]  */
        unsigned int int_dfifo_rderr_msk : 1;      /* [17]  */
        unsigned int reserved_3 : 14;              /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_int_msk;

/* Define the union u_lvds_wdr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_wdr_en : 1;       /* [0]  */
        unsigned int reserved_0 : 3;        /* [3..1]  */
        unsigned int lvds_wdr_num : 2;      /* [5..4]  */
        unsigned int reserved_1 : 2;        /* [7..6]  */
        unsigned int lvds_wdr_mode : 4;     /* [11..8]  */
        unsigned int lvds_wdr_id_shift : 4; /* [15..12]  */
        unsigned int reserved_2 : 16;       /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_wdr;

/* Define the union u_lvds_dolscd_hblk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dol_hblank1 : 16; /* [15..0]  */
        unsigned int dol_hblank2 : 16; /* [31..16]  */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART04_H_ */
