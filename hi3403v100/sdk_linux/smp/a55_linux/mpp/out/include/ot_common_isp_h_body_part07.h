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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART07_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART07_H_

    td_bool green_enhance_en;  /* RW; Range:[0, 1]; Format:1.0;If this is enabled, Green channel will be enhanced
                                  based on the ratio of green plant */
    td_u8   out_shift_limit;   /* RW; Range:[0, 0xFF]; Format:8.0;Max white point zone distance to Planckian Curve */
} ot_isp_awb_in_out_attr;

typedef struct {
    td_bool enable;                         /* RW; Range:[0, 1]; Format:1.0;
                                               If enabled, statistic parameter cr, cb will change according to iso */

    td_u16  cr_max[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xFFF]; au16CrMax[i] >= au16CrMin[i] */
    td_u16  cr_min[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xFFF] */
    td_u16  cb_max[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xFFF]; au16CbMax[i] >= au16CbMin[i] */
    td_u16  cb_min[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xFFF] */
} ot_isp_awb_cbcr_track_attr;

#define OT_ISP_AWB_LUM_HIST_NUM                          6
/* Defines the parameters of the luminance histogram statistics for white balance */
typedef struct {
    td_bool enable;                        /* RW; Range:[0, 1]; Format:1.0;
                                              If enabled, zone weight to awb is combined with zone luma */
    ot_op_mode   op_type;              /* In auto mode, the weight distribution follows Gaussian distribution */
    td_u8   hist_thresh[OT_ISP_AWB_LUM_HIST_NUM]; /* RW; Range:[0x0, 0xFF]; Format:8.0;
                                              In manual mode, user define luma thresh, thresh[0] is 0,
                                              thresh[5] is 0xFF,  thresh[i] greater or equal to  thresh[i-1] */
    td_u16  hist_wt[OT_ISP_AWB_LUM_HIST_NUM];     /* RW; Range:[0x0, 0xFFFF]; Format:16.0;
                                              user can define luma weight in both manual and auto mode. */
} ot_isp_awb_lum_histgram_attr;

/* Defines the information about a separate illuminant */
typedef struct {
    td_u16 white_r_gain;  /* RW;Range:[0x0, 0xFFF]; Format:4.8; G/R of White points at this light source */
    td_u16 white_b_gain;  /* RW;Range:[0x0, 0xFFF]; Format:4.8; G/B of White points at this light source */
    td_u16 exp_quant;     /* RW;shutter time * again * dgain >> 4, Not support Now */
    td_u8  light_status;  /* RW;Range:[0,2]; Format:2.0; idle  1:add light source  2:delete sensitive color */
    td_u8  radius;        /* RW;Range:[0x0, 0xFF];  Format:8.0; Radius of light source, */
} ot_isp_awb_extra_light_source_info;

#define OT_ISP_AWB_LS_NUM                                4
#define OT_ISP_AWB_MULTI_CT_NUM                          8
/* extended AWB attributes */
typedef struct {
    td_u8  tolerance;        /* RW; Range:[0x0, 0xFF]; Format:8.0; AWB adjust tolerance */
    td_u8  zone_radius;      /* RW; Range:[0x0, 0xFF]; Format:8.0; radius of AWB blocks */
    td_u16 curve_l_limit;    /* RW; Range:[0x0, 0x100]; Format:9.0; Left limit of AWB Curve,
                                recommend for indoor 0xE0, outdoor 0xE0 */
    td_u16 curve_r_limit;    /* RW; Range:[0x100, 0xFFF]; Format:12.0; Right Limit of AWB Curve,
                                recommend for indoor 0x130, outdoor 0x120 */

    td_bool  extra_light_en; /* RW; Range:[0, 1]; Format:1.0; Enable special light source function */
    ot_isp_awb_extra_light_source_info  light_info[OT_ISP_AWB_LS_NUM];
    ot_isp_awb_in_out_attr              in_or_out;

    td_bool multi_light_source_en;           /* RW; Range:[0, 1]; Format:1.0; If enabled, awb will do special process
                                                in multi light source environment */
    ot_isp_awb_multi_ls_type multi_ls_type;  /* Saturation or CCM Tuning */
    td_u16  multi_ls_scaler;                 /* RW; Range:[0x0, 0x100]; Format:12.0;
                                                In saturation type, it means the max saturation it can achieve,
                                                in ccm type, it means the strength of multi process. */
    td_u16  multi_ct_bin[OT_ISP_AWB_MULTI_CT_NUM];  /* RW; Range:[0, 0XFFFF]; Format:16.0;
                                                AWB Support divide the color temperature range by 8 bins */
    td_u16  multi_ct_wt[OT_ISP_AWB_MULTI_CT_NUM];   /* RW; Range:[0x0, 0x400];Weight for different color temperature,
                                                same value of 8 means CT weight doesn't work, */

    td_bool fine_tun_en;       /* RW; Range:[0x0, 0x1]; Format:1.0;If enabled, skin color scene will be optimized */
    td_u8   fine_tun_strength; /* RW; Range:[0x0, 0xFF]; Format:8.0;
                                  larger value means better performance of skin color scene,
                                  but will increase error probability in low color temperature scene */
} ot_isp_awb_attr_ex;

#define OT_ISP_AWB_CURVE_PARA_NUM                        6
typedef struct {
    td_bool enable;               /* RW; Range:[0x0, 0x1]; Format:1.0;If AWB is disabled, static wb gain will be used,
                                     otherwise auto wb gain will be used */

    td_u16 ref_color_temp;        /* RW; Range:[0x0, 0xFFFF]; Format:16.0; Calibration Information */
    td_u16 static_wb[OT_ISP_BAYER_CHN_NUM];   /* RW; Range:[0x0, 0xFFF]; Format:12.0;Calibration Information */
    td_s32 curve_para[OT_ISP_AWB_CURVE_PARA_NUM]; /* RW; Format:32.0;Calibration Information,
                                              limited Range:as32CurvePara[3] != 0, as32CurvePara[4]==128 */

    ot_isp_awb_alg_type       alg_type;

    td_u8  rg_strength;     /* RW; Range: [0x0, 0xFF]; Format:8.0; AWB Strength of R Channel */
    td_u8  bg_strength;     /* RW; Range: [0x0, 0xFF]; Format:8.0; AWB Strength of B Channel */
    td_u16 speed;           /* RW; Range: [0x0, 0xFFF]; Format:12.0; Convergence speed of AWB  */
    td_u16 zone_sel;        /* RW; Range: [0, 255]; Format:8.0; A value of 0 or 0xFF means global AWB,
                               A value between 0 and 0xFF means zoned AWB */
    td_u16 high_color_temp; /* RW; Range: [0, 65535]; Format:16.0; AWB max temperature, Recommended: [8500, 10000] */
    td_u16 low_color_temp;  /* RW; Range: [0, 65535]; Format:16.0; Limited Range:[0, u16HighColorTemp),
                               AWB min temperature, Recommended: [2000, 2500] */
    ot_isp_awb_ct_limit_attr ct_limit;
    td_bool shift_limit_en;  /* RW; Range: [0, 1]; Format:1.0; If enabled, when the statistic information is out of
                                range, it should be project back */
    td_u8  shift_limit;      /* RW; Range: [0x0, 0xFF];  Format:8.0; planckian curve range, Recommended: [0x30, 0x50] */
    td_bool gain_norm_en;    /* RW; Range: [0, 1]; Format:1.0; if enabled, the min of RGB gain is fixed. */
    td_bool natural_cast_en; /* RW, Range: [0, 1]; Format:1.0; if enabled, the color performance will be natural in
                                lowlight and low color temperature */

    ot_isp_awb_cbcr_track_attr cb_cr_track;
    ot_isp_awb_lum_histgram_attr luma_hist;
    td_bool awb_zone_wt_en;      /* RW, Range: [0, 1]; Format:1.0; if enabled, user can set weight for each zones */
    td_u8   zone_wt[OT_ISP_AWB_ZONE_NUM];   /* RW; Range: [0, 255]; Format:8.0;Zone Wt Table */
} ot_isp_awb_attr;

typedef struct {
    td_u16 r_gain;            /* RW; Range: [0x0, 0xFFF]; Format:4.8; Multiplier for R  color channel  */
    td_u16 gr_gain;           /* RW; Range: [0x0, 0xFFF]; Format:4.8; Multiplier for Gr color channel */
    td_u16 gb_gain;           /* RW; Range: [0x0, 0xFFF]; Format:4.8; Multiplier for Gb color channel */
    td_u16 b_gain;            /* RW; Range: [0x0, 0xFFF]; Format:4.8; Multiplier for B  color channel */
} ot_isp_mwb_attr;

typedef enum {
    OT_ISP_ALG_AWB_GW      = 0,
    OT_ISP_ALG_AWB_SPEC    = 1,
    OT_ISP_ALG_BUTT
} ot_isp_awb_alg;

typedef struct {
    td_bool bypass;              /* RW; Range: [0, 1];  Format:1.0; If enabled, awb will be bypassed */
    td_u8   awb_run_interval;    /* RW; Range: [0x1, 0xFF];  Format:8.0; set the AWB run interval */
    ot_op_mode   op_type;
    ot_isp_mwb_attr  manual_attr;
    ot_isp_awb_attr  auto_attr;
    ot_isp_awb_alg   alg_type;
} ot_isp_wb_attr;

typedef struct {
    td_bool sat_en;               /* RW; Range: [0, 1]; Format:1.0;
                                     If bSatEn=1, the active CCM = SatMatrix * ManualMatrix,
                                     else the active CCM =  ManualMatrix */
    td_u16  ccm[OT_ISP_CCM_MATRIX_SIZE]; /* RW; Range: [0x0, 0xFFFF]; Format:8.8; Manul CCM matrix, */
} ot_isp_color_matrix_manual;

typedef struct {
    td_u16 color_temp;           /* RW; Range: [500, 30000]; Format:16.0; the current color temperature */
    td_u16 ccm[OT_ISP_CCM_MATRIX_SIZE]; /* RW; Range: [0x0, 0xFFFF]; Format:8.8;CCM matrixes for different
                                           color temperature */
} ot_isp_color_matrix_param;

typedef struct {
    td_bool iso_act_en;    /* RW; Range: [0, 1]; Format:1.0; if enabled, CCM will bypass in low light */
    td_bool temp_act_en;   /* RW; Range: [0, 1]; Format:1.0; if enabled, CCM will bypass when color temperature is
                              larger than 8000K or less than 2500K */
    td_u16  ccm_tab_num;   /* RW; Range: [0x3, 0x7]; Format:16.0; The number of CCM matrixes */
    ot_isp_color_matrix_param ccm_tab[OT_ISP_CCM_MATRIX_NUM];
} ot_isp_color_matrix_auto;

typedef struct {
    ot_op_mode op_type;
    ot_isp_color_matrix_manual manual_attr;
    ot_isp_color_matrix_auto   auto_attr;
} ot_isp_color_matrix_attr;

typedef struct {
    td_u8 saturation;           /* RW; Range: [0, 0xFF]; Format:8.0; set the manual saturation of CCM */
} ot_isp_saturation_manual;

typedef struct {
    td_u8 sat[OT_ISP_AUTO_ISO_NUM];           /* RW; Range: [0, 0xFF]; should be decreased with increasing ISO */
} ot_isp_saturation_auto;

typedef struct {
    ot_op_mode op_type;
    ot_isp_saturation_manual manual_attr;
    ot_isp_saturation_auto   auto_attr;
} ot_isp_saturation_attr;

typedef struct {
    td_u16 red_cast_gain;        /* RW; Range: [0x100, 0x180]; Format:4.8; R channel gain after CCM */
    td_u16 green_cast_gain;      /* RW; Range: [0x100, 0x180]; Format:4.8; G channel gain after CCM */
    td_u16 blue_cast_gain;       /* RW; Range: [0x100, 0x180]; Format:4.8; B channel gain after CCM */
} ot_isp_color_tone_attr;

typedef struct {
    td_u16 r_gain;               /* R; Range: [0x0, 0xFFF]; Format:8.8;AWB result of R color channel */
    td_u16 gr_gain;              /* R; Range: [0x0, 0xFFF]; Format:8.8; AWB result of Gr color channel */
    td_u16 gb_gain;              /* R; Range: [0x0, 0xFFF]; Format:8.8; AWB result of Gb color channel */
    td_u16 b_gain;               /* R; Range: [0x0, 0xFFF]; Format:8.8; AWB result of B color channel */
    td_u16 saturation;           /* R; Range: [0x0, 0xFF];Format:8.0;Current saturation */
    td_u16 color_temp;           /* R; Range: [0x0, 0xFFFF];Format:16.0;Detect color temperature, maybe out of color
                                    cemeprature range */
    td_u16 ccm[OT_ISP_CCM_MATRIX_SIZE]; /* R; Range: [0x0, 0xFFFF];Format:16.0;Current color correction matrix */

    td_u16 ls0_ct;               /* R; Range: [0x0, 0xFFFF];Format:16.0;color tempearture of primary light source */
    td_u16 ls1_ct;               /* R; Range: [0x0, 0xFFFF];Format:16.0;color tempearture of secondary light source */
    td_u16 ls0_area;             /* R; Range: [0x0, 0xFF];Format:8.0;area of primary light source */
    td_u16 ls1_area;             /* R; Range: [0x0, 0xFF];Format:8.0;area of secondary light source */
    td_u8  multi_degree;         /* R; Range: [0x0, 0xFF];0 means uniform light source, larger value means multi
                                    light source */
    td_u16 active_shift;         /* R; Range;[0x0,0xFF] */
    td_u32 first_stable_time;    /* R, Range: [0x0, 0xFFFFFFFF];Format:32.0;AWB first stable frame number */
    ot_isp_awb_indoor_outdoor_status in_out_status;   /* R; indoor or outdoor status */
    td_s16 bv;                                   /* R; Range;[-32768, 32767]; Bv value */
} ot_isp_wb_info;

typedef struct {
    td_bool is_ir_mode;
    td_u32 ae_comp;
    td_u32 exp_time;
    td_float int_time_accu;
    td_u32 a_gain;
    td_float again_accu;
    td_u32 d_gain;
    td_float dgain_accu;
    td_u32 ispd_gain;
    td_u32 exposure;
    td_u32 init_iso;
    td_u32 lines_per500ms;
    td_u32 piris_fno;
    td_u16 wb_r_gain;
    td_u16 wb_g_gain;
    td_u16 wb_b_gain;
    td_u16 sample_r_gain;
    td_u16 sample_b_gain;
    td_u16 init_ccm[OT_ISP_CCM_MATRIX_SIZE];
    td_bool ae_route_ex_valid;
    td_bool quick_start_en;
    ot_isp_ae_route ae_route;
    ot_isp_ae_route_ex ae_route_ex;
    ot_isp_ae_route ae_route_sf;
    ot_isp_ae_route_ex ae_route_sf_ex;
    td_u32 ae_stat_pos;
} ot_isp_init_attr;

typedef struct {
    td_s32 distance_max; /* RW;the focus range */
    td_s32 distance_min;
    td_u8  wgt[OT_ISP_AF_ZONE_ROW][OT_ISP_AF_ZONE_COLUMN]; /* RW;weighting table */
} ot_isp_af_attr;

typedef struct {
    td_s32 default_speed;     /* RW;1,default speed(unit:m/s).(onvif) */
} ot_isp_mf_attr;

typedef struct {
    ot_op_mode  op_type;
    ot_isp_mf_attr  manual_attr;
    ot_isp_af_attr  auto_attr;
} ot_isp_focus_attr;

/*
  DNG cfalayout type
  1 = Rectangular (or square) layout
  2 = Staggered layout A: even columns are offset down by 1/2 row
  3 = Staggered layout B: even columns are offset up by 1/2 row
  4 = Staggered layout C: even rows are offset right by 1/2 column
  5 = Staggered layout D: even rows are offset left by 1/2 column
  6 = Staggered layout E: even rows are offset up by 1/2 row, even columns are offset left by 1/2 column
  7 = Staggered layout F: even rows are offset up by 1/2 row, even columns are offset right by 1/2 column
  8 = Staggered layout G: even rows are offset down by 1/2 row, even columns are offset left by 1/2 column
  9 = Staggered layout H: even rows are offset down by 1/2 row, even columns are offset right by 1/2 column
 */
typedef enum {
    OT_ISP_CFALAYOUT_TYPE_RECTANGULAR = 1,
    OT_ISP_CFALAYOUT_TYPE_A,        /* a,b,c... not support */
    OT_ISP_CFALAYOUT_TYPE_B,
    OT_ISP_CFALAYOUT_TYPE_C,
    OT_ISP_CFALAYOUT_TYPE_D,
    OT_ISP_CFALAYOUT_TYPE_E,
    OT_ISP_CFALAYOUT_TYPE_F,
    OT_ISP_CFALAYOUT_TYPE_G,
    OT_ISP_CFALAYOUT_TYPE_H,
    OT_ISP_CFALAYOUT_TYPE_BUTT
} ot_isp_dng_cfa_layout_type;

typedef struct {
    td_s32 numerator;   /* represents the numerator of a fraction, */
    td_s32 denominator; /* the denominator. */
} ot_isp_dng_srational;

typedef struct {
    td_u16 repeat_row;
    td_u16 repeat_col;
} ot_isp_dng_black_level_repeat_dim;

typedef struct {
    ot_isp_dng_srational default_scale_hor;
    ot_isp_dng_srational default_scale_ver;
} ot_isp_dng_default_scale;

typedef struct {
    td_u16 repeat_pattern_dim_row;
    td_u16 repeat_pattern_dim_col;
} ot_isp_dng_repeat_pattern_dim;

/* Defines the structure of dng raw format. */
typedef struct {
    td_u8 bits_per_sample;                      /* RO;Format:8.0; Indicate the bit numbers of raw data */
    td_u8 cfa_plane_color[OT_CFACOLORPLANE];       /* RO;Format:8.0; Indicate the planer numbers of raw data;
                                                   0:red 1:green 2: blue */
    ot_isp_dng_cfa_layout_type cfa_layout;      /* RO;Range:[1,9]; Describes the spatial layout of the CFA */
    ot_isp_dng_black_level_repeat_dim black_level_repeat_dim;   /* Specifies repeat pattern size for the BlackLevel */
    td_u32 white_level;                         /* RO;Format:32.0; Indicate the WhiteLevel of the raw data */
    ot_isp_dng_default_scale default_scale;     /* Specifies the default scale factors for each direction to convert
                                                   the image to square pixels */
    ot_isp_dng_repeat_pattern_dim cfa_repeat_pattern_dim; /* Specifies the pixel number of repeat color planer in
                                                             each direction */
    td_u8 cfa_pattern[OT_ISP_BAYER_CHN_NUM];        /* RO;Format:8.0; Indicate the bayer start order;
                                                       0:red 1:green 2: blue */
} ot_isp_dng_raw_format;

/* Defines the structure of dng image static information. read only */
typedef struct {
    ot_isp_dng_raw_format dng_raw_format;
    ot_isp_dng_srational color_matrix1[OT_ISP_CCM_MATRIX_SIZE];  /* defines a transformation matrix that converts XYZ
                                                                  values to reference camera native color space values,
                                                                  under the first calibration illuminant. */
    ot_isp_dng_srational color_matrix2[OT_ISP_CCM_MATRIX_SIZE];  /* defines a transformation matrix that converts XYZ
                                                                  values to reference camera native color space values,
                                                                  under the second calibration illuminant. */
    ot_isp_dng_srational camera_calibration1[OT_ISP_CCM_MATRIX_SIZE]; /* defines a calibration matrix that transforms
                                                                         reference camera native space values to
                                                                         individual camera native space values under
                                                                         the first calibration illuminant */
    ot_isp_dng_srational camera_calibration2[OT_ISP_CCM_MATRIX_SIZE]; /* defines a calibration matrix that transforms
                                                                         reference camera native space values to
                                                                         individual camera native space values under
                                                                         the second calibration illuminant */
    ot_isp_dng_srational forwad_matrix1[OT_ISP_CCM_MATRIX_SIZE];  /* defines a matrix that maps white balanced camera
                                                                  colors to XYZ D50 colors */
    ot_isp_dng_srational forwad_matrix2[OT_ISP_CCM_MATRIX_SIZE];  /* defines a matrix that maps white balanced camera
                                                                  colors to XYZ D50 colors */

    td_u8  calibration_illuminant1;      /* RO;Format:8.0;Light source, actually this means white balance setting.
                                            '0' means unknown, '1' daylight, '2' fluorescent, '3' tungsten, '10' flash,
                                            '17' standard light A, '18' standard light B, '19' standard light C,
                                            '20' D55, '21' D65, '22' D75, '255' other */
    td_u8  calibration_illuminant2;      /* RO;Format:8.0;Light source, actually this means white balance setting.
                                            '0' means unknown, '1' daylight, '2' fluorescent, '3' tungsten, '10' flash,
                                            '17' standard light A, '18' standard light B, '19' standard light C,
                                            '20' D55, '21' D65, '22' D75, '255' other */
} ot_isp_dng_image_static_info;

/* Defines the structure of DNG WB gain used for calculate DNG colormatrix. */
typedef struct {
    td_u16 r_gain;            /* RW;Range: [0x0, 0xFFF]; Multiplier for R  color channel */
    td_u16 g_gain;            /* RW;Range: [0x0, 0xFFF]; Multiplier for G  color channel */
    td_u16 b_gain;            /* RW;Range: [0x0, 0xFFF]; Multiplier for B  color channel */
} ot_isp_dng_wb_gain;

/* Defines the structure of DNG color parameters. */
typedef struct {
    ot_isp_dng_wb_gain wb_gain1; /* the calibration White balance gain of colorcheker in low colortemper light source */
    ot_isp_dng_wb_gain wb_gain2; /* the calibration White balance gain of colorcheker in high colortemper
                                    light source */
} ot_isp_dng_color_param;

typedef enum {
    OT_ISP_IR_STATUS_NORMAL = 0,
    OT_ISP_IR_STATUS_IR     = 1,
    OT_ISP_IR_BUTT
} ot_isp_ir_status;

typedef enum {
    OT_ISP_IR_SWITCH_NONE      = 0,
    OT_ISP_IR_SWITCH_TO_NORMAL = 1,
    OT_ISP_IR_SWITCH_TO_IR     = 2,
    OT_ISP_IR_SWITCH_BUTT
} ot_isp_ir_switch_status;

typedef struct {
    td_bool en;            /* RW, TD_TRUE: enable IR_auto function;  TD_TRUE: disable IR_auto function. */
    td_u32  normal_to_ir_iso_threshold; /* RW, Range: [0, 0xFFFFFFFF].
                                           ISO threshold of switching from normal to IR mode. */
    td_u32  ir_to_normal_iso_threshold; /* RW, Range: [0, 0xFFFFFFFF].
                                           ISO threshold of switching from IR to normal mode. */
    td_u32  rg_max;           /* RW, Range: [0x0, 0xFFF].    Maximum value of R/G in IR scene, 4.8-bit fix-point. */
    td_u32  rg_min;           /* RW, Range: [0x0, u32RGMax]. Minimum value of R/G in IR scene, 4.8-bit fix-point. */
    td_u32  bg_max;           /* RW, Range: [0x0, 0xFFF].    Maximum value of B/G in IR scene, 4.8-bit fix-point. */
    td_u32  bg_min;           /* RW, Range: [0x0, u32BGMax]. Minimum value of B/G in IR scene, 4.8-bit fix-point. */

    ot_isp_ir_status ir_status; /* RW. Current IR status. */
    ot_isp_ir_switch_status ir_switch; /* RO, IR switch status. */
} ot_isp_ir_auto_attr;

typedef enum {
    OT_ISP_FLICKER_TYPE_NONE = 0,
    OT_ISP_FLICKER_TYPE_UNKNOW,
    OT_ISP_FLICKER_TYPE_50HZ,
    OT_ISP_FLICKER_TYPE_60HZ,
    OT_ISP_FLICKER_TYPE_BUTT,
} ot_isp_calc_flicker_type;

typedef struct  {
    td_u32 lines_per_second;      /* The total line number of 1 second */
} ot_isp_calc_flicker_input;

typedef struct {
    ot_isp_calc_flicker_type flicker_type;  /* The calculate result of flicker type */
} ot_isp_calc_flicker_output;

#define OT_BAYER_CALIBRATION_PARA_NUM_NEW 16

typedef struct {
    td_double calibration_coef[OT_BAYER_CALIBRATION_PARA_NUM_NEW];
} ot_isp_noise_calibration;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART07_H_ */
