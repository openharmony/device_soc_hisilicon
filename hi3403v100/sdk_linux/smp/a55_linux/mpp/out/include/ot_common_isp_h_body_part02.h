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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART02_H_

    ot_op_mode op_type;
    ot_isp_fswdr_manual_attr manual_attr;
    ot_isp_fswdr_auto_attr   auto_attr;
} ot_isp_fswdr_mdt_attr;

typedef struct {
    td_bool motion_comp;        /* RW;Range:[0, 0x1];Format:1.0; enable/disable motion compensation */
    td_bool forcelong_coarse;           /* RW;Range:[0x0, 0x1];Format:1.0;
                                TD_TRUE: enable background information; TD_FALSE: background information */
    td_u16  short_threshold;          /* RW;Range:[0, 0xFFF];Format:12.0; data above this threshold will be taken from
                                      short exposure only. */
    td_u16  long_threshold;           /* RW;Range:[0, 0xFFF];Format:12.0; limited range:[0x0, short_threshold],
                                      data below this threshold will be taken from long exposure only. */
    td_bool force_long;         /* RW;Range:[0, 1];Format:1.0; enable/disable force long */
    td_u16  force_long_low_threshold; /* RW;Range:[0, 0xFFF];Format:12.0; data above this threshold will force to choose
                                   long frame only */
    td_u16  force_long_hig_threshold; /* RW;Range:[0, 0xFFF];Format:12.0; data below this threshold will force to choose
                                   long frame only */
    ot_isp_fswdr_mdt_attr wdr_mdt;

    td_u16  forcelong_coarse_low_threshold; /* RW;Range:[0x0, 0x3FFF];Format:14.0;
                                    The low threshold of the background information */
    td_u16  forcelong_coarse_high_threshold;/* RW;Range:[0x1, 0x3FFF];Format:14.0;
                                    The high threshold of  the background information */
} ot_isp_wdr_combine_attr;

typedef struct {
    td_u16  fusion_threshold[OT_ISP_WDR_MAX_FRAME_NUM]; /* RW;Range:[0x0, 0x3FFF];Format:14.0;
                                                           the threshold of the 4 frame */
} ot_isp_fusion_attr;

typedef struct {
    ot_isp_wdr_merge_mode   wdr_merge_mode;
    ot_isp_wdr_combine_attr wdr_combine;
    ot_isp_fusion_attr      fusion_attr;
} ot_isp_wdr_fs_attr;

typedef struct {
    td_u8 asymmetry;    /* RW; Range:[  1,  30]; Parameter0 of DRC asymmetry tone mapping curve */
    td_u8 second_pole;  /* RW; Range:[150, 210]; Parameter1 of DRC asymmetry tone mapping curve */
    td_u8 stretch;      /* RW; Range:[ 30,  60]; Parameter2 of DRC asymmetry tone mapping curve */
    td_u8 compress;     /* RW; Range:[100, 200]; Parameter3 of DRC asymmetry tone mapping curve */
} ot_isp_drc_asymmetry_curve_attr;

typedef struct {
    td_u16  strength;  /* RW; Range:[0x0, 0x3FF]; DRC manual strength */
} ot_isp_drc_manual_attr;

typedef struct {
    td_u16  strength;      /* RW; Range:[0x0, 0x3FF]; Base strength for strength adaptation in auto mode.
                              The actual DRC strength is calculated automatically by firmware:
                              1) linear mode, auto strength = f1(strength, histogram)
                              2) sensor built-in WDR mode: auto strength = f2(strength, histogram)
                              3) FS WDR mode: auto strength = f3(strength, exp_ratio) */
    td_u16  strength_max;  /* RW; Range:[0x0, 0x3FF]; uppper limit of firmware calculated strength */
    td_u16  strength_min;  /* RW; Range:[0x0, 0x3FF]; lower limit of firmware calculated strength */
} ot_isp_drc_auto_attr;

/* DRC curve type: 0 = Asymmetry curve, 1 = User-defined curve */
typedef enum {
    OT_ISP_DRC_CURVE_ASYMMETRY = 0x0,
    OT_ISP_DRC_CURVE_USER,
    OT_ISP_DRC_CURVE_BUTT
} ot_isp_drc_curve_select;

typedef struct {
    td_bool enable;
    ot_isp_drc_curve_select curve_select; /* RW; Range:[0x0, 0x1]; Tone mapping curve selection */
    td_u8  purple_reduction_strength; /* RW; Range:[0x0, 0x80]; Purple detection and reduction strength */
    td_u8  bright_gain_limit; /* RW; Range:[0x0, 0xF]; Bright area gain limit */
    td_u8  bright_gain_limit_step; /* RW; Range:[0x0, 0xF]; Bright area gain high limit step */
    td_u8  dark_gain_limit_luma; /* RW; Range:[0x0, 0x85]; Dark area luma gain limit */
    td_u8  dark_gain_limit_chroma; /* RW; Range:[0x0, 0x85]; Dark area chroma gain limit */
    td_u8  contrast_ctrl; /* RW; Range:[0x0, 0xF]; Format:4.0; Contrast control, smaller value yields better contrast */
    td_u8  rim_reduction_strength;  /* RW; Range:[0x0, 0x40]; Gradient reversal (rim) reduction strength */
    td_u8  rim_reduction_threshold; /* RW; Range:[0x0, 0x80]; Gradient reversal (rim) reduction threshold */
    td_u16 color_correction_lut[OT_ISP_DRC_CC_NODE_NUM]; /* RW; Range:[0x0, 0x400];
                                                           LUT of color correction coefficients */
    td_u16 tone_mapping_value[OT_ISP_DRC_TM_NODE_NUM]; /* RW; Range:[0x0, 0xffff]; LUT of user-defined curve */
    td_u8  spatial_filter_coef; /* RW; Range:[0x0, 0x5]; Spatial filter coefficients */
    td_u8  range_filter_coef; /* RW; Range:[0x0, 0xA]; Range filter coefficients */
    td_u8  detail_adjust_coef; /* RW; Range:[0x0, 0xF]; Detail adjust coefficient */
    td_u8  local_mixing_bright[OT_ISP_DRC_LMIX_NODE_NUM]; /* RW; Range:[0x0, 0x80];
                                                             LUT of enhancement coefficients for positive details */
    td_u8  local_mixing_dark[OT_ISP_DRC_LMIX_NODE_NUM]; /* RW; Range:[0x0, 0x80];
                                                           LUT of enhancement coefficients for negative details */
    td_u8  filter_coef_x[OT_ISP_DRC_FLTX_NODE_NUM]; /* RW; Range:[0x0, 0xF]; X filter coefficients,
                                                           larger value yields stronger details */
    td_u8  filter_low_threshold_x[OT_ISP_DRC_FLTX_NODE_NUM]; /* RW; Range:[0x0, 0xF]; X filter low thresholds,
                                                                larger value yields less noise */
    td_u8  filter_high_threshold_x[OT_ISP_DRC_FLTX_NODE_NUM]; /* RW; Range:[0x0, 0xF]; X filter high thresholds,
                                                                 larger value yields stronger details */
    td_u8  detail_adjust_coef_x; /* RW; Range:[0x0, 0xF]; X filter detail adjust coefficient */
    td_u8  local_mixing_bright_x[OT_ISP_DRC_LMIX_NODE_NUM]; /* RW; Range:[0x0, 0x80];
                                                               LUT of enhancement coefficients for positive details */
    td_u8  local_mixing_dark_x[OT_ISP_DRC_LMIX_NODE_NUM]; /* RW; Range:[0x0, 0x80];
                                                             LUT of enhancement coefficients for positive details */
    td_u8  blend_luma_max; /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */
    td_u8  blend_luma_bright_min; /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */
    td_u8  blend_luma_bright_threshold; /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */
    td_u8  blend_luma_bright_slope; /* RW; Range:[0x0, 0xF]; Luma-based filter blending weight control */
    td_u8  blend_luma_dark_min; /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */
    td_u8  blend_luma_dark_threshold; /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */
    td_u8  blend_luma_dark_slope; /* RW; Range:[0x0, 0xF]; Luma-based filter blending weight control */
    td_u8  blend_detail_max; /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */
    td_u8  blend_detail_bright_min; /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */
    td_u8  blend_detail_bright_threshold; /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */
    td_u8  blend_detail_bright_slope; /* RW; Range:[0x0, 0xF]; Detail-based filter blending weight control */
    td_u8  blend_detail_dark_min; /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */
    td_u8  blend_detail_dark_threshold; /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */
    td_u8  blend_detail_dark_slope; /* RW; Range:[0x0, 0xF]; Detail-based filter blending weight control */
    td_u8  detail_adjust_coef_blend; /* RW; Range:[0x0, 0xF]; Extra detail gain of X filter details */
    td_u8  low_saturation_color_ctrl; /* RW; Range:[0x0, 0xF]; Low saturation color control */
    td_u8  high_saturation_color_ctrl; /* RW; Range:[0x0, 0xF]; High saturation color control */
    td_bool color_correction_ctrl; /* RW; Range:[0x0, 0x1]; Color correction mode control */

    ot_op_mode op_type;
    ot_isp_drc_manual_attr manual_attr;
    ot_isp_drc_auto_attr   auto_attr;
    ot_isp_drc_asymmetry_curve_attr asymmetry_curve;
} ot_isp_drc_attr;

#define OT_ISP_EXPANDER_POINT_NUM_MAX (OT_ISP_EXPANDER_NODE_NUM - 1)

typedef struct {
    td_bool  en;
    td_u8    bit_depth_in;        /* RW;Range:[0xC,0x14];Format:5.0;The Bit depth of input.
                                     bit_depth_in must be an even number */
    td_u8    bit_depth_out;       /* RW;Range:[0xC,0x14];Format:5.0;The Bit depth of output.
                                     bit_depth_out must be an even number */
    td_u16   knee_point_num;      /* Range:[1, 256]; Format:9.0; the number of knee point */
    ot_point knee_point_coord[OT_ISP_EXPANDER_POINT_NUM_MAX]; /* RW; Range: x:[0x0, 0x100],y:[0x0, 0x100000];
                                                                 The coordinate of knee point.
                                                                 The coordinate must be monotonically increasing */
} ot_isp_expander_attr;

typedef struct {
    td_u8 wgt;   /* RW;Range:SS928V100 = [0x0, 0xFF];
                    Format:1.7;Weight of Gaussian distribution */
    td_u8 sigma; /* RW;Range: [0x1, 0xFF];Format:0.8;Sigma of Gaussian distribution */
    td_u8 mean;  /* RW;Range: [0x0, 0xFF];Format:0.8;Mean of Gaussian distribution */
} ot_isp_ldci_gauss_coef_attr;

typedef struct {
    ot_isp_ldci_gauss_coef_attr  he_pos_wgt;
    ot_isp_ldci_gauss_coef_attr  he_neg_wgt;
} ot_isp_ldci_he_wgt_attr;

typedef struct {
    ot_isp_ldci_he_wgt_attr  he_wgt;
    td_u16                   blc_ctrl;  /* RW;Range: [0x0, 0x1FF];Format:9.0;Restrain dark region */
} ot_isp_ldci_manual_attr;

typedef struct {
    ot_isp_ldci_he_wgt_attr  he_wgt[OT_ISP_AUTO_ISO_NUM];
    td_u16                   blc_ctrl[OT_ISP_AUTO_ISO_NUM]; /* RW;Range: [0x0, 0x1FF];Format:9.0;auto mode,
                                                               Restrain dark region */
} ot_isp_ldci_auto_attr;

typedef struct {
    td_bool                 en;          /* RW;Range:[0x0,0x1];Format:1.0; */
    td_u8                   gauss_lpf_sigma; /* RW;Range: [0x1, 0xFF];Format:0.8;
                                                Coefficient of Gaussian low-pass filter */
    ot_op_mode              op_type;
    ot_isp_ldci_manual_attr manual_attr;
    ot_isp_ldci_auto_attr   auto_attr;
    td_u16                  tpr_incr_coef;   /* RW;Range: [0x0, 0x100];Format:0.9;
                                                Increase Coefficient of temporal filter */
    td_u16                  tpr_decr_coef;   /* RW;Range: [0x0, 0x100];Format:0.9;
                                                Decrease Coefficient of temporal filter */
} ot_isp_ldci_attr;

/*
  defines CA type
  0 = enable Ca module
  1 = enable Cp module
 */
typedef enum {
    OT_ISP_CA_ENABLE = 0x0,
    OT_ISP_CP_ENABLE,
    OT_ISP_CA_BUTT
} ot_isp_ca_type;

typedef struct {
    td_u32  y_ratio_lut[OT_ISP_CA_YRATIO_LUT_LENGTH];  /* RW;Range:[0,2047];Format:1.11 */
    td_s32  iso_ratio[OT_ISP_AUTO_ISO_NUM];            /* RW;Range:[0,2047];Format:1.10 */
} ot_isp_ca_lut;

typedef struct {
    td_u8   cp_lut_y[OT_ISP_CA_YRATIO_LUT_LENGTH]; /* RW;Range:[0,255];Format:8.0; */
    td_u8   cp_lut_u[OT_ISP_CA_YRATIO_LUT_LENGTH]; /* RW;Range:[0,255];Format:8.0; */
    td_u8   cp_lut_v[OT_ISP_CA_YRATIO_LUT_LENGTH]; /* RW;Range:[0,255];Format:8.0; */
} ot_isp_cp_lut;

typedef struct {
    td_bool en;             /* RW;Range:[0x0,0x1];Format:1.0 */
    ot_isp_ca_type ca_cp_en;
    ot_isp_ca_lut  ca;
    ot_isp_cp_lut  cp;
} ot_isp_ca_attr;

typedef struct {
    td_u16 r_gain_limit; /* RW; Range: [0x1FF, 0x7FF]; Format:1.10; gain of R color channel 1024 to RgainLimit */
    td_u16 b_gain_limit; /* RW; Range: [0x1FF, 0x7FF]; Format:1.10; gain of B color channel 1024 to BgainLimit */
} ot_isp_crb_manual_attr;

typedef struct {
    td_u16 r_gain_limit[OT_ISP_WDR_RATIO_NUM]; /* RW; Range: [0x1FF, 0x7FF]; Format:1.10; R gain 1024 to gainLimit */
    td_u16 b_gain_limit[OT_ISP_WDR_RATIO_NUM]; /* RW; Range: [0x1FF, 0x7FF]; Format:1.10; B gain 1024 to gainLimit */
} ot_isp_crb_auto_attr;

typedef struct {
    td_bool                  en;
    ot_op_mode               op_type;
    ot_isp_crb_auto_attr     auto_attr;
    ot_isp_crb_manual_attr   manual_attr;
} ot_isp_crb_attr;

/* CSC */
typedef struct {
    td_s16 csc_in_dc[OT_ISP_CSC_DC_NUM]; /* RW; Range:[-1024, 1023]; Format: 11.0;
                                            Input dc component for csc matrix */
    td_s16 csc_out_dc[OT_ISP_CSC_DC_NUM]; /* RW; Range:[-1024, 1023]; Format: 11.0;
                                             Output dc component for csc matrix */
    td_s16 csc_coef[OT_ISP_CSC_COEF_NUM];  /* RW; Range:[-4096, 4095]; Format: 5.10; 3x3 coefficients for csc matrix */
} ot_isp_csc_matrx;

typedef struct {
    td_bool en;                 /* RW; Range:[0, 1];Format:1.0; Enable/Disable CSC Function */
    ot_color_gamut color_gamut; /* RW; Range: [0, 3]; Color gamut type; Not Support COLOR_GAMUT_BT2020 */
    td_u8   hue;              /* RW; Range:[0, 100];Format:8.0; Csc Hue Value */
    td_u8   luma;             /* RW; Range:[0, 100];Format:8.0; Csc Luma Value */
    td_u8   contr;            /* RW; Range:[0, 100];Format:8.0; Csc Contrast Value */
    td_u8   satu;             /* RW; Range:[0, 100];Format:8.0; Csc Saturation Value */
    td_bool limited_range_en; /* RW; Range: [0x0, 0x1];
                                 Enable/Disable: Enable Limited range output mode(default full range output) */
    td_bool ext_csc_en;       /* RW; Range: [0x0, 0x1]; Enable/Disable: Enable extended luma range */
    td_bool ct_mode_en;       /* RW; Range: [0x0, 0x1]; Enable/Disable: Enable ct mode */
    ot_isp_csc_matrx csc_magtrx;     /* RW; Color Space Conversion matrix */
} ot_isp_csc_attr;

/* CLUT */
typedef struct {
    td_bool en; /* RW; Range:[0, 1];Format:1.0; Enable/Disable CLUT Function */
    td_u32  gain_r; /* RW; Range:[0, 4095];Format:12.0 */
    td_u32  gain_g; /* RW; Range:[0, 4095];Format:12.0 */
    td_u32  gain_b; /* RW; Range:[0, 4095];Format:12.0 */
} ot_isp_clut_attr;

/* CLUT LUT */
typedef struct {
    td_u32 lut[OT_ISP_CLUT_LUT_LENGTH];  /* RW; Range: SS928V100 = [0, 1073741823] */
} ot_isp_clut_lut;

/*
 * Defines the type of static defect pixel calibration
 * 0 = bright defect pixel calibration
 * 1 = dark defect pixel calibration
 */
typedef enum {
    OT_ISP_STATIC_DP_BRIGHT = 0x0,
    OT_ISP_STATIC_DP_DARK,
    OT_ISP_STATIC_DP_BUTT
} ot_isp_static_dp_type;

typedef struct {
    td_bool enable_detect;                /* RW; Range: [0, 1];Format 1.0;Set 'TD_TRUE'to start static defect-pixel
                                             calibration, and firmware will set 'TD_FALSE' when finished */
    ot_isp_static_dp_type static_dp_type; /* RW; Range: [0, 1];Format 1.0;Select static bright/dark defect-pixel
                                             calibration */
    td_u8  start_thresh;  /* RW; Range: [1, 255]; Format 8.0;Start threshold for static defect-pixel calibraiton */
    td_u16 count_max;     /* RW; Range:SS928V100 = [0, 6144];
                           Format 14.0; limited Range: [0, OT_ISP_STATIC_DP_COUNT_NORMAL*BlkNum],
                           Limit of max number of static defect-pixel calibraiton */
    td_u16 count_min;     /* RW; Range:SS928V100 = [0, 6144];Format 14.0; limited Range: [0, u16CountMax],
                             Limit of min number of static defect-pixel calibraiton */
    td_u16 time_limit;    /* RW; Range: [0x0, 1600];Format 11.0;Time limit for static defect-pixel calibraiton */

    td_u32 table[OT_ISP_STATIC_DP_COUNT_MAX];  /* R; Range: [0, 0x1FFF1FFF];Format 29.0;
                                           Static defect-pixel calibraiton table,
                                           0~12 bits represents the X coordinate of the defect pixel,
                                           16~28 bits represent the Y coordinate of the defect pixel */
    td_u8  finish_thresh;   /* R; Range: [0, 255];Format 8.0; Finish threshold for static defect-pixel calibraiton */
    td_u16 count;          /* R; Range:SS928V100 = [0, 6144];Format 14.0;
                              Finish number for static defect-pixel calibraiton */
    ot_isp_status status;  /* R; Range: [0, 2];Format 2.0;Status of static defect-pixel calibraiton */
} ot_isp_dp_static_calibrate;

typedef struct {
    td_bool enable;       /* RW; Range: [0, 1];Format 1.0;Enable/disable the static defect-pixel module */
    td_u16  bright_count; /* RW; Range:SS928V100 = [0, 6144];Format 14.0;
                             limited Range: [0, OT_ISP_STATIC_DP_COUNT_NORMAL*BlkNum],
                             When used as input(W), indicate the number of static bright defect pixels;
                             As output(R),indicate the number of static bright and dark defect pixels */
    td_u16  dark_count;   /* RW; Range:SS928V100 = [0, 6144];Format 14.0;
                             limited Range: [0, OT_ISP_STATIC_DP_COUNT_NORMAL*BlkNum],
                             When used as input(W), indicate the number of static dark defect pixels;
                             As output(R), invalid value 0. */
    td_u32  bright_table[OT_ISP_STATIC_DP_COUNT_MAX]; /* RW; Range: [0x0, 0x1FFF1FFF];Format 29.0;
                                                  0~12 bits represents the X coordinate of the defect pixel,
                                                  16~28 bits represent the Y coordinate of the defect pixel
                                                  Notice: When used as input(W), indicate static bright defect pixels
                                                  table; As output(R), indicate static bright and dark defect pixels
                                                  table. */

    td_u32  dark_table[OT_ISP_STATIC_DP_COUNT_MAX]; /* RW; Range: [0x0, 0x1FFF1FFF];Format 29.0;
                                                0~12 bits represents the X coordinate of the defect pixel,
                                                16~28 bits represent the Y coordinate of the defect pixel
                                                Notice: When used as input(W), indicate static dark defect pixels table;
                                                As output(R), invalid value */
    td_bool show;                  /* RW; Range: [0, 1];Format 1.0;RW;highlight static defect pixel */
} ot_isp_dp_static_attr;

typedef struct {
    td_u16  strength;           /* RW; Range: [0, 255];Format:8.0;Dynamic DPC strength. */
    td_u16  blend_ratio;        /* RW; Range: [0, 128];Format:9.0;Blending ratio required for DPC */
} ot_isp_dp_dynamic_manual_attr;

typedef struct {
    td_u16  strength[OT_ISP_AUTO_ISO_NUM];  /* RW; Range: [0, 255];Format:8.0;Dynamic DPC strength. */
    td_u16  blend_ratio[OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 128];Format:9.0;Blending ratio required for DPC */
} ot_isp_dp_dynamic_auto_attr;

typedef struct {
    td_bool enable;             /* RW; Range: [0, 1];Format 1.0;Enable/disable the dynamic defect-pixel module */
    td_bool sup_twinkle_en;     /* RW; Range: [0, 1];Format 1.0;Enable/disable the twinkle suppression  module */
    td_s8   soft_thr;           /* RW; Range: [0, 127];Format s8.0;twinkle suppression threshold */
    td_u8   soft_slope;         /* RW; Range: [0, 255];Format 8.0;Correction controlling parameter of the pixels whose
                                   deviation is smaller than s8SoftThr. */
    ot_op_mode op_type;         /* RW; Range: [0, 1];Format 1.0;Working mode of dynamic DPC */
    ot_isp_dp_dynamic_manual_attr manual_attr;
    ot_isp_dp_dynamic_auto_attr   auto_attr;
    td_u8 bright_strength;      /* RW; Range: [0, 127];Format 8.0, local DPC strength for bright area */
    td_u8 dark_strength;        /* RW; Range: [0, 127];Format 8.0, local DPC strength for dark area */
} ot_isp_dp_dynamic_attr;

typedef struct {
    td_bool en;      /* RW; Range:[0, 1];Format:1.0; TD_TRUE: enable lsc; TD_FALSE: disable lsc */
    td_u16  mesh_strength;    /* RW; Range:SS928V100=[0, 65535];
                            The strength of the mesh shading correction */
    td_u16  blend_ratio; /* RW; Range:[0, 256];Format:9.0; the blendratio of the two mesh gain lookup-table */
    td_bool bnr_lsc_auto_en; /* RW; Range:[0, 1];Format:1.0; TD_TRUE: enable bnr_lsc auto mode;
                            TD_FALSE: disable bnr_lsc auto mode */
} ot_isp_shading_attr;

typedef struct {
    td_u16  r_gain[OT_ISP_LSC_GRID_POINTS];  /* RW; Range:[0, 1023];Member used to store the calibration data of the
                                                R channel required for LSC. */
    td_u16  gr_gain[OT_ISP_LSC_GRID_POINTS]; /* RW; Range:[0, 1023];Member used to store the calibration data of the
                                                Gr channel required for LSC. */
    td_u16  gb_gain[OT_ISP_LSC_GRID_POINTS]; /* RW; Range:[0, 1023];Member used to store the calibration data of the
                                                Gb channel required for LSC. */
    td_u16  b_gain[OT_ISP_LSC_GRID_POINTS];  /* RW; Range:[0, 1023];Member used to store the calibration data of the
                                                B channel required for LSC. */
} ot_isp_shading_gain_lut;

typedef struct {
    td_u16  r_gain[OT_ISP_RLSC_POINTS];  /* RW; Range:[0, 65535];Member used to store the calibration data of the
                                            R channel required for BNR_LSC. */
    td_u16  gr_gain[OT_ISP_RLSC_POINTS]; /* RW; Range:[0, 65535];Member used to store the calibration data of the
                                            Gr channel required for BNR_LSC. */
    td_u16  gb_gain[OT_ISP_RLSC_POINTS]; /* RW; Range:[0, 65535];Member used to store the calibration data of the
                                            Gb channel required for BNR_LSC. */
    td_u16  b_gain[OT_ISP_RLSC_POINTS];  /* RW; Range:[0, 65535];Member used to store the calibration data of the
                                            B channel required for BNR_LSC. */
} ot_isp_bnr_lsc_gain_lut;

typedef struct {
    td_u8   mesh_scale;        /* RW; Range:[0, 7];Format:3.0;Three bit value that selects the scale and precision for
                                  the 10 bit gain coefficients stored in mesh tables */
    td_u16  x_grid_width[OT_ISP_MLSC_X_HALF_GRID_NUM]; /* RW; Range:SS928V100=[4, 1988];
                                                          limited Range:[4, u32Width/4 - 60],
                                                          Member used to store the width data of each GRID partition */
    td_u16  y_grid_width[OT_ISP_MLSC_Y_HALF_GRID_NUM]; /* RW; Range:SS928V100=[4, 1988];
                                                          limited Range:[4, u32Height/4 - 60];
                                                        Member used to store the height data of each GRID partition */
    ot_isp_shading_gain_lut lsc_gain_lut[OT_ISP_MLSC_GAIN_LUT_NUM];
    ot_isp_bnr_lsc_gain_lut bnr_lsc_gain_lut;
} ot_isp_shading_lut_attr;

typedef struct {
    ot_isp_bayer_format bayer;    /* RW; Range: [0, 3];Format ENUM;Shows bayer pattern */
    ot_isp_bayer_raw_bit raw_bit;  /* RW; Range: {8,10,12,14,16};Format ENUM;Shows input raw bitwidth */

    td_u16  img_height; /* RW; Range: [0, 65535];Format 16.0;Input raw image height */
    td_u16  img_width;  /* RW; Range: [0, 65535];Format 16.0;Input raw image width */

    td_u16  dst_img_height; /* RW; Range: [0, 65535];Format 16.0;limited Range:[0, u16ImgHeight], Image height that
                               crop from input raw image, set to ImgHeight if don't need to crop */
    td_u16  dst_img_width;  /* RW; Range: [0, 65535];Format 16.0;limited Range:[0, u16ImgWidth], Image width that
                               crop from input raw image, set to ImgWidth if don't need to crop */
    td_u16  offset_x;       /* RW; Range: [0, 65535];Format 16.0;limited Range:[0, u16ImgWidth - u16DstImgWidth],
                               Horizontal offset that crop from input raw image, set to 0 if don't need to crop */
    td_u16  offset_y;       /* RW; Range: [0, 65535];Format 16.0;limited Range:[0, u16ImgHeight - u16DstImgHeight],
                               Vertical offset that crop from input raw image, set to 0 if don't need to crop */

    td_u32  mesh_scale; /* RW; Range: [0, 7];Format 3.0; Shows Mesh Scale value */

    td_u16  blc_offset_r; /* RW; Range: [0, 4095];Format 12.0;BLC value for R channel */
    td_u16  blc_offset_gr; /* RW; Range: [0, 4095];Format 12.0;BLC value for Gr channel */
    td_u16  blc_offset_gb; /* RW; Range: [0, 4095];Format 12.0;BLC value for Gb channel */
    td_u16  blc_offset_b; /* RW; Range: [0, 4095];Format 12.0;BLC value for B channel */
} ot_isp_mlsc_calibration_cfg;

typedef struct {
    td_u8   mesh_scale;       /* RW; Range:[0, 7];Format:3.0;Three bit value that selects the scale and precision for
                                 the 10 bit gain coefficients stored in mesh tables */
    td_u16  x_grid_width[OT_ISP_MLSC_X_HALF_GRID_NUM]; /* RW; Range:SS928V100=[4, 16323];
                                                          limited Range:[4, u16ImgWidth /4 - 60],
                                                     Member used to store the width data of each GRID partition */
    td_u16  y_grid_width[OT_ISP_MLSC_Y_HALF_GRID_NUM]; /* RW; SS928V100=[4, 16323];
                                                          limited Range:[4, u16ImgHeight /4 - 60],
                                                     Member used to store the height data of each GRID partition */
    ot_isp_shading_gain_lut lsc_gain_lut;
    ot_isp_bnr_lsc_gain_lut bnr_lsc_gain_lut;
} ot_isp_mesh_shading_table;

typedef struct {
    td_bool en;           /* RW; Range:[0,1];Format:1.0;Acs Enable */
    td_u16  y_strength;   /* RW; Range:[0,256];Format:8.0;Acs correction strength for y shading */
    td_u16  run_interval; /* RW; Range:[1,255];Format:16.0;Acs Run Interval */
    td_bool lock_en;      /* RW; Range:[0,1];Format:1.0;Lock Gain Lut Enable */
} ot_isp_acs_attr;

typedef struct {
    td_u8  sfm_threshold[OT_ISP_AUTO_ISO_NUM];  /* RW; Range:[0,255];Format:8.0; spatial filter merge thresh */
    ot_isp_bnr_sfm0_mode  sfm0_mode[OT_ISP_AUTO_ISO_NUM];    /* RW; Range:[0,1];Format:1.0; sfm0 filter mode */
    td_u16 sfm0_coarse_strength[OT_ISP_BAYER_CHN_NUM][OT_ISP_AUTO_ISO_NUM];  /* RW; Range:[0, 864];Format:10.0;
                                                                                sfm0 filter mode */
    td_u8  sfm0_ex_strength[OT_ISP_AUTO_ISO_NUM];     /* RW; Range:[0, 16];Format:5.0; sfm0 filter mode */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART02_H_ */
