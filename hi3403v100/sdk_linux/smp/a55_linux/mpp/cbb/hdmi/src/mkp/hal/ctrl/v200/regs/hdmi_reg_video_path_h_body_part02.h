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

#ifndef HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_VIDEO_PATH_H_BODY_PART02_H_
#define HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_VIDEO_PATH_H_BODY_PART02_H_

        unsigned int rsv_47       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff23;

typedef union {
    struct {
        unsigned int reg_csc_r3c1 : 16; /* [15:0] */
        unsigned int rsv_48       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff31;

typedef union {
    struct {
        unsigned int reg_csc_r3c2 : 16; /* [15:0] */
        unsigned int rsv_49       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff32;

typedef union {
    struct {
        unsigned int reg_csc_r3c3 : 16; /* [15:0] */
        unsigned int rsv_50       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff33;

typedef union {
    struct {
        unsigned int reg_csc_inoffset_y : 13; /* [12:0] */
        unsigned int rsv_51             : 19; /* [31:13] */
    } bits;
    unsigned int u32;
} multi_csc_inoffset_y;

typedef union {
    struct {
        unsigned int reg_csc_inoffset_cb : 13; /* [12:0] */
        unsigned int rsv_52              : 19; /* [31:13] */
    } bits;
    unsigned int u32;
} multi_csc_inoffset_cb;

typedef union {
    struct {
        unsigned int reg_csc_inoffset_cr : 13; /* [12:0] */
        unsigned int rsv_53              : 19; /* [31:13] */
    } bits;
    unsigned int u32;
} multi_csc_inoffset_cr;

typedef union {
    struct {
        unsigned int reg_csc_outoffset_y : 13; /* [12:0] */
        unsigned int rsv_54              : 19; /* [31:13] */
    } bits;
    unsigned int u32;
} multi_csc_outoffset_y;

typedef union {
    struct {
        unsigned int reg_csc_outoffset_cb : 13; /* [12:0] */
        unsigned int rsv_55               : 19; /* [31:13] */
    } bits;
    unsigned int u32;
} multi_csc_outoffset_cb;

typedef union {
    struct {
        unsigned int reg_csc_outoffset_cr : 13; /* [12:0] */
        unsigned int rsv_56               : 19; /* [31:13] */
    } bits;
    unsigned int u32;
} multi_csc_outoffset_cr;

typedef union {
    struct {
        unsigned int reg_dwsm_hori_en   : 1;  /* [0] */
        unsigned int reg_hori_filter_en : 1;  /* [1] */
        unsigned int reg_dwsm_vert_en   : 1;  /* [2] */
        unsigned int reg_dwsm_vert_byp  : 1;  /* [3] */
        unsigned int reg_vert_cbcr_sel  : 1;  /* [4] */
        unsigned int rsv_57             : 27; /* [31:5] */
    } bits;
    unsigned int u32;
} video_dwsm_ctrl;

typedef union {
    struct {
        unsigned int reg_demux_420_en     : 1;  /* [0] */
        unsigned int reg_ddr_en           : 1;  /* [1] */
        unsigned int reg_yc_mux_en        : 1;  /* [2] */
        unsigned int reg_blank_replace_en : 1;  /* [3] */
        unsigned int reg_pixel_rate       : 2;  /* [5:4] */
        unsigned int reg_ddr_polarity     : 1;  /* [6] */
        unsigned int reg_yc_mux_polarity  : 1;  /* [7] */
        unsigned int reg_cbcr_order       : 1;  /* [8] */
        unsigned int reg_demux_cb_or_cr   : 1;  /* [9] */
        unsigned int reg_pxl_div_en       : 1;  /* [10] */
        unsigned int rsv_58               : 21; /* [31:11] */
    } bits;
    unsigned int u32;
} data_align_ctrl;

typedef union {
    struct {
        unsigned int reg_blank_y : 12; /* [11:0] */
        unsigned int rsv_59      : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} blank_data_y;

typedef union {
    struct {
        unsigned int reg_blank_cb : 12; /* [11:0] */
        unsigned int rsv_60       : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} blank_data_cb;

typedef union {
    struct {
        unsigned int reg_blank_cr : 12; /* [11:0] */
        unsigned int rsv_61       : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} blank_data_cr;

typedef union {
    struct {
        unsigned int reg_vmux_y_sel  : 3; /* [2:0] */
        unsigned int reg_vmux_cb_sel : 3; /* [5:3] */
        unsigned int reg_vmux_cr_sel : 3; /* [8:6] */
        unsigned int reg_bitmask_y   : 2; /* [10:9] */
        unsigned int reg_bitmask_cb  : 2; /* [12:11] */
        unsigned int reg_bitmask_cr  : 2; /* [14:13] */
        unsigned int reg_bitrev_en   : 3; /* [17:15] */
        unsigned int reg_datamask_en : 3; /* [20:18] */
        unsigned int reg_syncmask_en : 4; /* [24:21] */
        unsigned int reg_inver_sync  : 4; /* [28:25] */
        unsigned int rsv_62          : 3; /* [31:29] */
    } bits;
    unsigned int u32;
} video_dmux_ctrl;

typedef struct {
    volatile timing_gen_ctrl        tim_gen_ctrl;           /* 800 */
    volatile hsync_timing_config0   hsync_timing_cfg0;      /* 804 */
    volatile hsync_timing_config1   hsync_timing_cfg1;      /* 808 */
    volatile hsync_timing_config2   hsync_timing_cfg2;      /* 80C */
    volatile vsync_timing_config0   vsync_timing_cfg0;      /* 810 */
    volatile vsync_timing_config1   vsync_timing_cfg1;      /* 814 */
    volatile vsync_timing_config2   vsync_timing_cfg2;      /* 818 */
    unsigned int                    reserved_0[5];          /* 81C-82C */
    volatile video_path_ctrl        vid_path_ctrl;          /* 830 */
    unsigned int                    reserved_1[3];          /* 834-83c */
    volatile pattern_gen_ctrll      ptn_gen_ctrl;           /* 840 */
    volatile solid_pattern_config   solid_ptn_ctrl;         /* 844 */
    volatile mask_pattern_config    mask_ptn_ctrl;          /* 848 */
    volatile bar_ext_config         bar_ext_cfg;            /* 84C */
    volatile cbar_pattern_a0        cbar_a0;                /* 850 */
    volatile cbar_pattern_a1        cbar_a1;                /* 854 */
    volatile cbar_pattern_a2        cbar_a2;                /* 858 */
    volatile cbar_pattern_a3        cbar_a3;                /* 85C */
    volatile cbar_pattern_a4        cbar_a4;                /* 860 */
    volatile cbar_pattern_a5        cbar_a5;                /* 864 */
    volatile cbar_pattern_a6        cbar_a6;                /* 868 */
    volatile cbar_pattern_a7        cbar_a7;                /* 86C */
    volatile cbar_pattern_b0        cbar_b0;                /* 870 */
    volatile cbar_pattern_b1        cbar_b1;                /* 874 */
    volatile cbar_pattern_b2        cbar_b2;                /* 878 */
    volatile cbar_pattern_b3        cbar_b3;                /* 87C */
    volatile cbar_pattern_b4        cbar_b4;                /* 880 */
    volatile cbar_pattern_b5        cbar_b5;                /* 884 */
    volatile cbar_pattern_b6        cbar_b6;                /* 888 */
    volatile cbar_pattern_b7        cbar_b7;                /* 88C */
    unsigned int                    reserved_2[10];         /* 890-8B4 */
    volatile format_det_config      fmt_det_cfg;            /* 8B8 */
    volatile fdet_status            fmt_dect_status;        /* 8BC */
    volatile fdet_hori_res          fmt_dect_hori_res;      /* 8C0 */
    volatile fdet_hori_vert_res     fmt_dect_hori_vert_res; /* 8C4 */
    unsigned int                    reserved_3[9];          /* 8C8-8E8 */
    volatile dither_config          dither_cfg;             /* 8EC */
    unsigned int                    reserved_4[2];          /* 8F0-8F4 */
    volatile clip_ctrl              clip_ctrl;              /* 8F8 */
    volatile clip_y_config          clip_y_cfg;             /* 8FC */
    volatile clip_c_config          clip_c_cfg;             /* 900 */
    unsigned int                    reserved_5[2];          /* 904-908 */
    volatile pxl_cap_ctrl           capture_ctrl;           /* 90C */
    volatile pxl_cap_position       capture_cfg;            /* 910 */
    volatile cap_y_value            pxl_y_capture;          /* 914 */
    volatile cap_cb_value           pxl_cb_capture;         /* 918 */
    volatile cap_cr_value           pxl_cr_capture;         /* 91C */
    unsigned int                    reserved_6[5];          /* 920-930 */
    volatile multi_csc_ctrl         csc_ctrl;               /* 934 */
    volatile multi_csc_coeff11      csc_coeff11;            /* 938 */
    volatile multi_csc_coeff12      csc_coeff12;            /* 93C */
    volatile multi_csc_coeff13      csc_coeff13;            /* 940 */
    volatile multi_csc_coeff21      csc_coeff21;            /* 944 */
    volatile multi_csc_coeff22      csc_coeff22;            /* 948 */
    volatile multi_csc_coeff23      csc_coeff23;            /* 94C */
    volatile multi_csc_coeff31      csc_coeff31;            /* 950 */
    volatile multi_csc_coeff32      csc_coeff32;            /* 954 */
    volatile multi_csc_coeff33      csc_coeff33;            /* 958 */
    volatile multi_csc_inoffset_y   y_in_offset;            /* 95C */
    volatile multi_csc_inoffset_cb  cb_in_offset;           /* 960 */
    volatile multi_csc_inoffset_cr  cr_in_offset;           /* 964 */
    volatile multi_csc_outoffset_y  y_out_offset;           /* 968 */
    volatile multi_csc_outoffset_cb cb_out_offset;          /* 96C */
    volatile multi_csc_outoffset_cr cr_out_offset;          /* 970 */
    unsigned int                    reserved_7[3];          /* 974-97c */
    volatile video_dwsm_ctrl        dwsm_ctrl;              /* 980 */
    unsigned int                    reserved_8[2];          /* 984-988 */
    volatile data_align_ctrl        align_ctrl;             /* 98C */
    volatile blank_data_y           blk_data_y;             /* 990 */
    volatile blank_data_cb          blk_data_cb;            /* 994 */
    volatile blank_data_cr          blk_data_cr;            /* 998 */
    unsigned int                    reserved_9[3];         /* 99C-9A4 */
    volatile video_dmux_ctrl        dmux_ctrl;              /* 9A8 */
} video_path_reg_regs_type;

int hdmi_reg_video_path_regs_init(td_u32 id, const td_char *addr);
int hdmi_reg_video_path_regs_deinit(td_u32 id);
td_void hdmi_reg_video_blank_en_set(td_u32 id, td_u8 reg_video_blank_en);
td_u8 hdmi_reg_video_blank_en_get(td_u32 id);
td_void hdmi_reg_solid_pattern_en_set(td_u32 id, td_u8 solid_pattern_en);
td_u8 hdmi_reg_solid_pattern_en_get(td_u32 id);
td_void hdmi_reg_solid_pattern_cb_set(td_u32 id, td_u16 solid_pattern_cb);
td_void hdmi_reg_solid_pattern_y_set(td_u32 id, td_u16 solid_pattern_y);
td_void hdmi_reg_solid_pattern_cr_set(td_u32 id, td_u16 solid_pattern_cr);
td_void hdmi_reg_sync_polarity_force_set(td_u32 id, td_u8 sync_polarity_force);
td_void hdmi_reg_fdt_status_clear_set(td_u32 id, td_u8 fdt_status_clear);
td_u8 hdmi_reg_interlaced_get(td_u32 id);
td_u8 hdmi_reg_hsync_polarity_get(td_u32 id);
td_u8 hdmi_reg_vsync_polarity_get(td_u32 id);
td_u16 hdmi_reg_hsync_active_cnt_get(td_u32 id);
td_u16 hdmi_reg_hsync_total_cnt_get(td_u32 id);
td_u16 hdmi_reg_vsync_active_cnt_get(td_u32 id);
td_u16 hdmi_reg_vsync_total_cnt_get(td_u32 id);
td_void hdmi_reg_dither_rnd_bypass_set(td_u32 id, td_u8 dither_rnd_byp);
td_u8 hdmi_reg_dither_rnd_bypass_get(td_u32 id);
td_void hdmi_reg_dither_mode_set(td_u32 id, td_u8 dither_mode);
td_u8 hdmi_reg_dither_mode_get(td_u32 id);
td_void hdmi_reg_csc_en_set(td_u32 id, td_u8 reg_csc_en);
td_u8 hdmi_reg_csc_en_get(td_u32 id);
td_void hdmi_reg_csc_saturate_en_set(td_u32 id, td_u8 reg_csc_saturate_en);
td_void hdmi_reg_csc_mode_set(td_u32 id, td_u8 reg_csc_mode);
td_u8 hdmi_reg_csc_mode_get(td_u32 id);
td_void hdmi_reg_dwsm_hori_en_set(td_u32 id, td_u8 reg_dwsm_hori_en);
td_u8 hdmi_reg_dwsm_hori_en_get(td_u32 id);
td_void hdmi_reg_hori_filter_en_set(td_u32 id, td_u8 reg_hori_filter_en);
td_u8 hdmi_reg_hori_filter_en_get(td_u32 id);
td_void hdmi_reg_dwsm_vert_en_set(td_u32 id, td_u8 reg_dwsm_vert_en);
td_u8 hdmi_reg_dwsm_vert_en_get(td_u32 id);
td_void hdmi_reg_dwsm_vert_bypass_set(td_u32 id, td_u8 reg_dwsm_vert_byp);
td_u8 hdmi_reg_dwsm_vert_bypass_get(td_u32 id);
td_void hdmi_reg_demux_420_en_set(td_u32 id, td_u8 reg_demux_420_en);
td_u8 hdmi_reg_demux_420_en_get(td_u32 id);
td_void hdmi_reg_pxl_div_en_set(td_u32 id, td_u8 reg_pxl_div_en);
td_u8 hdmi_reg_pxl_div_en_get(td_u32 id);
td_void hdmi_reg_vmux_y_sel_set(td_u32 id, td_u8 reg_vmux_y_sel);
td_u8 hdmi_reg_vmux_y_sel_get(td_u32 id);
td_void hdmi_reg_vmux_cb_sel_set(td_u32 id, td_u8 reg_vmux_cb_sel);
td_u8 hdmi_reg_vmux_cb_sel_get(td_u32 id);
td_void hdmi_reg_vmux_cr_sel_set(td_u32 id, td_u8 reg_vmux_cr_sel);
td_u8 hdmi_reg_vmux_cr_sel_get(td_u32 id);
td_void hdmi_reg_syncmask_en_set(td_u32 id, td_u8 reg_syncmask_en);
td_void hdmi_reg_inver_sync_set(td_u32 id, td_u8 reg_inver_sync);
td_u8 hdmi_reg_inver_sync_get(td_u32 id);


#endif /* HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_VIDEO_PATH_H_BODY_PART02_H_ */
