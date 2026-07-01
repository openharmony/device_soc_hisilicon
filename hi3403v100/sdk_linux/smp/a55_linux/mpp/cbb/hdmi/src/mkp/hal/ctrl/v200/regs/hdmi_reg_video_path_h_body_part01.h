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

#ifndef HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_VIDEO_PATH_H_BODY_PART01_H_
#define HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_VIDEO_PATH_H_BODY_PART01_H_


#include "ot_type.h"

typedef union {
    struct {
        unsigned int reg_timing_gen_en : 1;  /* [0] */
        unsigned int reg_extmode       : 1;  /* [1] */
        unsigned int reg_timing_sel    : 6;  /* [7:2] */
        unsigned int reg_sync_polarity : 2;  /* [9:8] */
        unsigned int rsv_0             : 22; /* [31:10] */
    } bits;
    unsigned int u32;
} timing_gen_ctrl;

typedef union {
    struct {
        unsigned int hsync_total_count       : 13; /* [12:0] */
        unsigned int hsync_high_to_low_count : 13; /* [25:13] */
        unsigned int rsv_1                   : 6;  /* [31:26] */
    } bits;
    unsigned int u32;
} hsync_timing_config0;

typedef union {
    struct {
        unsigned int hsync_de_start_count : 13; /* [12:0] */
        unsigned int hsync_de_end_count   : 13; /* [25:13] */
        unsigned int rsv_2                : 6;  /* [31:26] */
    } bits;
    unsigned int u32;
} hsync_timing_config1;

typedef union {
    struct {
        unsigned int hsync_low_to_high_count : 13; /* [12:0] */
        unsigned int rsv_3                   : 19; /* [31:13] */
    } bits;
    unsigned int u32;
} hsync_timing_config2;

typedef union {
    struct {
        unsigned int vsync_total_count       : 12; /* [11:0] */
        unsigned int vsync_high_to_low_count : 12; /* [23:12] */
        unsigned int rsv_4                   : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} vsync_timing_config0;

typedef union {
    struct {
        unsigned int vsync_de_start_count : 12; /* [11:0] */
        unsigned int vsync_de_end_count   : 12; /* [23:12] */
        unsigned int rsv_5                : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} vsync_timing_config1;

typedef union {
    struct {
        unsigned int vsync_low_to_high_count : 12; /* [11:0] */
        unsigned int rsv_6                   : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} vsync_timing_config2;

typedef union {
    struct {
        unsigned int reg_video_blank_en : 1;  /* [0] */
        unsigned int rsv_6              : 31; /* [31:1] */
    } bits;
    unsigned int u32;
} video_path_ctrl;

typedef union {
    struct {
        unsigned int tpg_enable          : 1;  /* [0] */
        unsigned int video_format        : 2;  /* [2:1] */
        unsigned int solid_pattern_en    : 1;  /* [3] */
        unsigned int colorbar_en         : 1;  /* [4] */
        unsigned int square_pattern_en   : 1;  /* [5] */
        unsigned int mask_pattern_en     : 3;  /* [8:6] */
        unsigned int replace_pattern_en  : 3;  /* [11:9] */
        unsigned int bar_pattern_extmode : 1;  /* [12] */
        unsigned int cbar_pattern_sel    : 2;  /* [14:13] */
        unsigned int mix_color_en        : 1;  /* [15] */
        unsigned int increase_en         : 1;  /* [16] */
        unsigned int rsv_7               : 15; /* [31:17] */
    } bits;
    unsigned int u32;
} pattern_gen_ctrll;

typedef union {
    struct {
        unsigned int solid_pattern_cb : 10; /* [9:0] */
        unsigned int solid_pattern_y  : 10; /* [19:10] */
        unsigned int solid_pattern_cr : 10; /* [29:20] */
        unsigned int rsv_8            : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} solid_pattern_config;

typedef union {
    struct {
        unsigned int mask_pattern_cb : 10; /* [9:0] */
        unsigned int mask_pattern_y  : 10; /* [19:10] */
        unsigned int mask_pattern_cr : 10; /* [29:20] */
        unsigned int rsv_9           : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} mask_pattern_config;

typedef union {
    struct {
        unsigned int colorbar_width : 12; /* [11:0] */
        unsigned int square_height  : 12; /* [23:12] */
        unsigned int rsv_10         : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} bar_ext_config;

typedef union {
    struct {
        unsigned int bar_pattern_a0 : 30; /* [29:0] */
        unsigned int rsv_11         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_a0;

typedef union {
    struct {
        unsigned int bar_pattern_a1 : 30; /* [29:0] */
        unsigned int rsv_12         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_a1;

typedef union {
    struct {
        unsigned int bar_pattern_a2 : 30; /* [29:0] */
        unsigned int rsv_13         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_a2;

typedef union {
    struct {
        unsigned int bar_pattern_a3 : 30; /* [29:0] */
        unsigned int rsv_14         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_a3;

typedef union {
    struct {
        unsigned int bar_pattern_a4 : 30; /* [29:0] */
        unsigned int rsv_15         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_a4;

typedef union {
    struct {
        unsigned int bar_pattern_a5 : 30; /* [29:0] */
        unsigned int rsv_16         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_a5;

typedef union {
    struct {
        unsigned int bar_pattern_a6 : 30; /* [29:0] */
        unsigned int rsv_17         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_a6;

typedef union {
    struct {
        unsigned int bar_pattern_a7 : 30; /* [29:0] */
        unsigned int rsv_18         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_a7;

typedef union {
    struct {
        unsigned int bar_pattern_b0 : 30; /* [29:0] */
        unsigned int rsv_19         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_b0;

typedef union {
    struct {
        unsigned int bar_pattern_b1 : 30; /* [29:0] */
        unsigned int rsv_20         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_b1;

typedef union {
    struct {
        unsigned int bar_pattern_b2 : 30; /* [29:0] */
        unsigned int rsv_21         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_b2;

typedef union {
    struct {
        unsigned int bar_pattern_b3 : 30; /* [29:0] */
        unsigned int rsv_22         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_b3;

typedef union {
    struct {
        unsigned int bar_pattern_b4 : 30; /* [29:0] */
        unsigned int rsv_23         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_b4;

typedef union {
    struct {
        unsigned int bar_pattern_b5 : 30; /* [29:0] */
        unsigned int rsv_24         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_b5;

typedef union {
    struct {
        unsigned int bar_pattern_b6 : 30; /* [29:0] */
        unsigned int rsv_25         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_b6;

typedef union {
    struct {
        unsigned int bar_pattern_b7 : 30; /* [29:0] */
        unsigned int rsv_26         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} cbar_pattern_b7;

typedef union {
    struct {
        unsigned int sync_polarity_force  : 1;  /* [0] */
        unsigned int hsync_polarity_value : 1;  /* [1] */
        unsigned int vsync_polarity_value : 1;  /* [2] */
        unsigned int fdt_status_clear     : 1;  /* [3] */
        unsigned int pixel_cnt_threhold   : 4;  /* [7:4] */
        unsigned int rsv_27               : 24; /* [31:8] */
    } bits;
    unsigned int u32;
} format_det_config;

typedef union {
    struct {
        unsigned int interlaced     : 1;  /* [0] */
        unsigned int hsync_polarity : 1;  /* [1] */
        unsigned int vsync_polarity : 1;  /* [2] */
        unsigned int rsv_28         : 29; /* [31:3] */
    } bits;
    unsigned int u32;
} fdet_status;

typedef union {
    struct {
        unsigned int hsync_active_cnt : 13; /* [12:0] */
        unsigned int hsync_total_cnt  : 13; /* [25:13] */
        unsigned int rsv_29           : 6;  /* [31:26] */
    } bits;
    unsigned int u32;
} fdet_hori_res;

typedef union {
    struct {
        unsigned int vsync_active_cnt : 13; /* [12:0] */
        unsigned int vsync_total_cnt  : 13; /* [25:13] */
        unsigned int rsv_30           : 6;  /* [31:26] */
    } bits;
    unsigned int u32;
} fdet_hori_vert_res;

typedef union {
    struct {
        unsigned int dither_rnd_byp      : 1;  /* [0] */
        unsigned int dither_mode         : 2;  /* [2:1] */
        unsigned int dither_rnd_en       : 1;  /* [3] */
        unsigned int dither_spatial_en   : 1;  /* [4] */
        unsigned int dither_spatial_dual : 1;  /* [5] */
        unsigned int rsv_31              : 26; /* [31:6] */
    } bits;
    unsigned int u32;
} dither_config;

typedef union {
    struct {
        unsigned int range_clip_byp : 1;  /* [0] */
        unsigned int clip_rgb_mode  : 1;  /* [1] */
        unsigned int rsv_32         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} clip_ctrl;

typedef union {
    struct {
        unsigned int clip_y_min : 12; /* [11:0] */
        unsigned int clip_y_max : 12; /* [23:12] */
        unsigned int rsv_33     : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} clip_y_config;

typedef union {
    struct {
        unsigned int clip_c_min : 12; /* [11:0] */
        unsigned int clip_c_max : 12; /* [23:12] */
        unsigned int rsv_34     : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} clip_c_config;

typedef union {
    struct {
        unsigned int auto_trigger_en : 1;  /* [0] */
        unsigned int soft_trigger_en : 1;  /* [1] */
        unsigned int show_point_en   : 1;  /* [2] */
        unsigned int rsv_35          : 1;  /* [3] */
        unsigned int cap_stat_done   : 1;  /* [4] */
        unsigned int cap_stat_busy   : 1;  /* [5] */
        unsigned int cap_stat_error  : 1;  /* [6] */
        unsigned int rsv_36          : 25; /* [31:7] */
    } bits;
    unsigned int u32;
} pxl_cap_ctrl;

typedef union {
    struct {
        unsigned int cap_pixel_position : 13; /* [12:0] */
        unsigned int cap_line_position  : 13; /* [25:13] */
        unsigned int rsv_37             : 6;  /* [31:26] */
    } bits;
    unsigned int u32;
} pxl_cap_position;

typedef union {
    struct {
        unsigned int capture_y_value : 12; /* [11:0] */
        unsigned int rsv_38          : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} cap_y_value;

typedef union {
    struct {
        unsigned int capture_cb_value : 12; /* [11:0] */
        unsigned int rsv_39           : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} cap_cb_value;

typedef union {
    struct {
        unsigned int capture_cr_value : 12; /* [11:0] */
        unsigned int rsv_40           : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} cap_cr_value;

typedef union {
    struct {
        unsigned int reg_csc_en          : 1;  /* [0] */
        unsigned int reg_csc_coef_ext    : 1;  /* [1] */
        unsigned int reg_csc_dither_en   : 1;  /* [2] */
        unsigned int reg_csc_saturate_en : 1;  /* [3] */
        unsigned int reg_csc_mode        : 8;  /* [11:4] */
        unsigned int rsv_41              : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} multi_csc_ctrl;

typedef union {
    struct {
        unsigned int reg_csc_r1c1 : 16; /* [15:0] */
        unsigned int rsv_42       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff11;

typedef union {
    struct {
        unsigned int reg_csc_r1c2 : 16; /* [15:0] */
        unsigned int rsv_43       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff12;

typedef union {
    struct {
        unsigned int reg_csc_r1c3 : 16; /* [15:0] */
        unsigned int rsv_44       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff13;

typedef union {
    struct {
        unsigned int reg_csc_r2c1 : 16; /* [15:0] */
        unsigned int rsv_45       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff21;

typedef union {
    struct {
        unsigned int reg_csc_r2c2 : 16; /* [15:0] */
        unsigned int rsv_46       : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} multi_csc_coeff22;

typedef union {
    struct {
        unsigned int reg_csc_r2c3 : 16; /* [15:0] */


#endif /* HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_VIDEO_PATH_H_BODY_PART01_H_ */
