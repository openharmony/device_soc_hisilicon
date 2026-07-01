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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART03_H_

    td_u8  sfm0_ex_detail_prot[OT_ISP_AUTO_ISO_NUM];  /* RW; Range:[0, 31];Format:5.0; sfm0 detail protect rate under
                                                         ext mode */
    td_u8  sfm0_norm_edge_strength[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 31];Format:5.0; sfm0 edge smooth str under
                                                            normal mode */
    td_u8  sfm1_detail_prot[OT_ISP_AUTO_ISO_NUM];        /* RW; Range:[0, 31];Format:5.0; sfm0 edge smooth str under
                                                            normal mode */
    td_u8  sfm1_coarse_strength[OT_ISP_AUTO_ISO_NUM];  /* RW; Range:[0, 63];Format:10.0; sfm1 filter coarstr strength */
    td_u8  fine_strength[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 128];Format:8.0; denoise rate */
    td_u16 coring_wgt[OT_ISP_AUTO_ISO_NUM]; /* RW;Range:[0, 3200];Format:12.0;Strength of reserving the random noise */
    td_u8  coring_mot_thresh[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0,255];Format:8.0; coring motion thresh */
} ot_isp_nr_snr_auto_attr;

typedef struct {
    td_u8                 sfm_threshold;  /* RW; Range:[0, 255];Format:8.0; spatial filter merge thresh */
    ot_isp_bnr_sfm0_mode  sfm0_mode;    /* RW; Range:[0, 1];Format:1.0; sfm0 filter mode */
    td_u16    sfm0_coarse_strength[OT_ISP_BAYER_CHN_NUM];  /* RW; Range:[0,864];Format:10.0; sfm0 filter mode */
    td_u8     sfm0_ex_strength;    /* RW; Range:[0, 16];Format:5.0; sfm0 filter mode */
    td_u8     sfm0_ex_detail_prot;  /* RW; Range:[0, 31];Format:5.0; sfm0 detail protect rate under ext mode */
    td_u8     sfm0_norm_edge_strength; /* RW; Range:[0, 31];Format:5.0; sfm0 edge smooth str under normal mode */
    td_u8     sfm1_detail_prot;     /* RW; Range:[0, 31];Format:5.0; sfm0 edge smooth str under normal mode */
    td_u8     sfm1_coarse_strength;    /* RW; Range:[0, 63];Format:10.0; sfm1 filter coarstr strength */
    td_u8     fine_strength;       /* RW; Range:[0, 128];Format:8.0; denoise rate */
    td_u16    coring_wgt;     /* RW;Range:[0, 3200];Format:12.0;Strength of reserving the random noise */
    td_u8     coring_mot_thresh;   /* RW; Range:[0, 255];Format:8.0; coring motion thresh */
} ot_isp_nr_snr_manual_attr;

typedef struct {
    ot_isp_nr_snr_auto_attr   snr_auto;
    ot_isp_nr_snr_manual_attr snr_manual;
} ot_isp_nr_snr_attr;

typedef struct {
    td_bool md_mode[OT_ISP_AUTO_ISO_NUM];    /* RW; Range:[0, 1];Format:1.0; motion detection mode */
    td_u8   md_anti_flicker_strength[OT_ISP_AUTO_ISO_NUM];  /* RW; Range:[0, 64];Format:7.0;
                                                               motion detection anti flicker strength */
    td_u8   md_static_ratio[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 64];Format:7.0; motion detection static area ratio */
    td_u8   md_static_fine_strength[OT_ISP_AUTO_ISO_NUM];  /* RW; Range:[0, 255];Format:8.0;
                                                              motion detection background tnr strength  */

    td_u8   tss[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 128];Format:8.0; snr ratio in background  */
    td_u8   tfr[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 255];Format:8.0; tnr strength based on texture strength */
    td_u8   tfs[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 255];Format:8.0; tnr absolute strength  */

    td_bool user_define_md[OT_ISP_AUTO_ISO_NUM];   /* RW; Range:[0, 1];Format:1.0; user define motion detection mode */
    td_s16  user_define_slope[OT_ISP_AUTO_ISO_NUM];        /* RW; Range:[-32768, 32767];Format:8.8; user define motion
                                                              detection strength change with luma */
    td_u16  user_define_dark_thresh[OT_ISP_AUTO_ISO_NUM];  /* RW; Range:[0, 65535];Format:8.8; user define motion
                                                              detection strength in dark area */

    td_u8   user_define_color_thresh[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 64];Format:7.0; user define motion
                                                              detection strength in color area */
    td_u8   sfr_r[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 128];Format:8.0; sfr rate r */
    td_u8   sfr_g[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 128];Format:8.0; sfr rate g */
    td_u8   sfr_b[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0, 128];Format:8.0; sfr rate b */
} ot_isp_nr_tnr_auto_attr;

typedef struct {
    td_bool md_mode;   /* RW; Range:[0, 1];Format:1.0; motion detection mode */
    td_u8   md_anti_flicker_strength;  /* RW; Range:[0, 64];Format:7.0; motion detection anti flicker strength */
    td_u8   md_static_ratio;      /* RW; Range:[0, 64];Format:7.0; motion detection static area ratio */
    td_u8   md_static_fine_strength;    /* RW; Range:[0, 255];Format:8.0; motion detection background tnr strength  */

    td_u8   tss;    /* RW; Range:[0, 128];Format:8.0; snr ratio in background  */
    td_u8   tfr;    /* RW; Range:[0, 255];Format:8.0; tnr strength based on texture strength  */
    td_u8   tfs;    /* RW; Range:[0, 255];Format:8.0; tnr absolute strength  */

    td_bool user_define_md;           /* RW; Range:[0, 1];Format:1.0; user define motion detection mode */
    td_s16  user_define_slope;        /* RW; Range:[-32768, 32767];Format:8.8; user define motion detection strength
                                         change with luma */
    td_u16  user_define_dark_thresh;  /* RW; Range:[0, 65535];Format:8.8; user define motion detection strength in
                                         dark area */
    td_u8   user_define_color_thresh; /* RW; Range:[0, 64];Format:7.0; user define motion detection strength in
                                         color area */

    td_u8   sfr_r;  /* RW; Range:[0, 128];Format:8.0; sfr rate r */
    td_u8   sfr_g;  /* RW; Range:[0, 128];Format:8.0; sfr rate g */
    td_u8   sfr_b;  /* RW; Range:[0, 128];Format:8.0; sfr rate b */
} ot_isp_nr_tnr_manual_attr;

typedef struct {
    ot_isp_nr_tnr_auto_attr   tnr_auto;
    ot_isp_nr_tnr_manual_attr tnr_manual;
} ot_isp_nr_tnr_attr;

typedef struct {
    td_u8 snr_sfm0_wdr_strength[OT_ISP_WDR_MAX_FRAME_NUM];    /* RW; Range:[0, 64];Format:7.0; motion detection mode */
    td_u8 snr_sfm0_fusion_strength[OT_ISP_WDR_MAX_FRAME_NUM]; /* RW; Range:[0, 64];Format:7.0; motion detection mode */
    td_u8 snr_sfm1_wdr_strength;                             /* RW; Range:[0, 255];Format:8.0; motion detection mode */
    td_u8 snr_sfm1_fusion_strength;                          /* RW; Range:[0, 255];Format:8.0; motion detection mode */
    td_u8 md_wdr_strength[OT_ISP_WDR_MAX_FRAME_NUM];     /* RW; Range:[0, 64];Format:7.0; motion detection mode */
    td_u8 md_fusion_strength[OT_ISP_WDR_MAX_FRAME_NUM];  /* RW; Range:[0, 64];Format:7.0; motion detection mode */
} ot_isp_nr_wdr_attr;

typedef struct {
    td_bool    en;      /* RW; Range:[0, 1];Format:1.0; bayernr enable  */
    ot_op_mode op_type;     /* RW; Range:[0, 1];Format:1.0; bayernr operation mode  */
    td_bool    tnr_en;  /* RW; Range:[0, 1];Format:1.0; bayernr tnr enable */
    td_bool    lsc_nr_en;  /* RW; Range:[0, 1];Format:1.0; lsc nr enable */

    td_u8      lsc_ratio1;  /* RW; Range:[0, 15];Format:4.0; lsc nr ratio1 */
    td_u8      lsc_ratio2;  /* RW; Range:[0, 255];Format:8.0; lsc nr ratio2 */

    td_u16     coring_ratio[OT_ISP_BAYERNR_LUT_LENGTH];  /* RW;Range:[0x0, 0x3ff];Format:10.0;
                                                            Strength of reserving the random noise according to luma */
    ot_isp_nr_snr_attr   snr_cfg;
    ot_isp_nr_tnr_attr   tnr_cfg;
    ot_isp_nr_wdr_attr   wdr_cfg;
} ot_isp_nr_attr;

typedef enum {
    OT_ISP_IRBAYER_GRGBI = 0,
    OT_ISP_IRBAYER_RGBGI = 1,
    OT_ISP_IRBAYER_GBGRI = 2,
    OT_ISP_IRBAYER_BGRGI = 3,
    OT_ISP_IRBAYER_IGRGB = 4,
    OT_ISP_IRBAYER_IRGBG = 5,
    OT_ISP_IRBAYER_IBGRG = 6,
    OT_ISP_IRBAYER_IGBGR = 7,
    OT_ISP_IRBAYER_BUTT
} ot_isp_ir_bayer_format;

typedef enum {
    OT_ISP_IR_CVTMAT_MODE_NORMAL = 0,
    OT_ISP_IR_CVTMAT_MODE_MONO,
    OT_ISP_IR_CVTMAT_MODE_USER,
    OT_ISP_IR_CVTMAT_MODE_BUTT
} ot_isp_ir_cvtmat_mode;

typedef enum {
    OT_ISP_RGBIR_MODE_RGBIR4X4 = 0,
    OT_ISP_RGBIR_MODE_NORMAL,
    OT_ISP_RGBIR_MODE_BUTT
} ot_isp_rgbir_mode;

typedef struct {
    ot_isp_rgbir_mode mode; /* RW; Range:[0x0, 0x1]; 0: rgbir mode; 1: ir remove mode */
    ot_isp_bayer_format   out_pattern; /* RW;Range:[0x0, 0x3];Format:2.0; Bayer pattern of the output */
    ot_isp_ir_bayer_format in_rgbir_pattern; /* RW;Range:[0x0, 0x7];Format:3.0; IR pattern of the input */
    ot_isp_bayer_format in_bayer_pattern; /* RW;Range:[0x0, 0x3]; Format:2.0;
                                                 Bayer pattern of the input signal for ir remove mode */
    td_bool is_ir_upscale; /* RW;Range:[0x0, 0x1]; Format:1.0;
                                is upscale for ir image input of IR remove mode */
} ot_isp_rgbir_cfg;

typedef struct {
    td_bool rgbir_en; /* RW;Range:[0x0, 0x1];Format:1.0; Enable/Disable RGBIR module */
    ot_isp_rgbir_cfg rgbir_cfg;
    td_u32  ir_sum_info; /* R;Range:[0, 0xffffffff]; Format:32.0; ir_sum_info value, only read */
    td_bool auto_gain_en; /* RW;Range:[0x0,0x1];Format:1.0;  Enable/Disable auto gain */
    td_u16  auto_gain; /* RW;Range:[0, 1023]; Format:10.0; auto gain value, 256 for 1x */
    td_bool smooth_en; /* RW;Range:[0x0, 0x1];Format:1.0; Enable/Disable Smooth Function */
    td_u16  exp_ctrl[OT_ISP_RGBIR_CTRL_NUM]; /* RW;Range:[0, 2047];Format:11.0; Over expose control */
    td_u16  exp_gain[OT_ISP_RGBIR_CTRL_NUM]; /* RW;Range:[0, 511];Format:9.0; Over expose control */
    td_bool ir_rm_en; /* RW;Range:[0x0,0x1]; Format:1.0; Enable/Disable ir remove Function before interplattion */
    td_u16  ir_rm_ratio[OT_ISP_RGBIR_CROSSTALK_NUM]; /* RW;Range:[0, 1000]; Format:10.0; crosstalk, 1000 for 1x */
    ot_isp_ir_cvtmat_mode ir_cvtmat_mode;
    td_s16  cvt_matrix[OT_ISP_RGBIR_CVTMAT_NUM]; /* RW;Range:[-16384, 16383]; Format:s14.0;
                                                      RGBIR to Bayer image Convert matrix (need calibration) */
    td_u16  wb_ctrl_strength[OT_ISP_RGBIR_CTRL_NUM]; /* RW;Range:[0,1024]; Format:11.0; wb control of R/B */
} ot_isp_rgbir_attr;

/*
  Defines the type of the ISP gamma curve
  0 = Default curve
  1 = Default BT.709 curve (Gamma Curve)
  2 = Default SMPTE.2084 curve (PQ Curve), Not Support
  3 = User defined Gamma curve, LUT must be correct
*/
typedef enum {
    OT_ISP_GAMMA_CURVE_DEFAULT = 0x0,
    OT_ISP_GAMMA_CURVE_SRGB,
    OT_ISP_GAMMA_CURVE_HDR,    /* Not Support */
    OT_ISP_GAMMA_CURVE_USER_DEFINE,
    OT_ISP_GAMMA_CURVE_BUTT
} ot_isp_gamma_curve_type;
typedef struct {
    td_bool   enable;                     /* RW; Range:[0, 1]; Format:1.0;Enable/Disable Gamma Function */
    td_u16    table[OT_ISP_GAMMA_NODE_NUM];    /* RW; Range:[0, 4095]; Format:12.0;Gamma LUT nodes value */

    ot_isp_gamma_curve_type curve_type;    /* RW; Range:[0, 3]; Format:2.0;Gamma curve type */
} ot_isp_gamma_attr;

typedef struct {
    td_bool enable;                   /* RW; Range:[0, 1]; Format:1.0;Enable/Disable PreGamma Function */
    td_u32  table[OT_ISP_PREGAMMA_NODE_NUM]; /* RW; Range:SS928V100 = [0, 0xFFFFF]; Format:20.0;
                                         PreGamma LUT nodes value */
} ot_isp_pregamma_attr;

typedef struct {
    td_bool enable;                   /* RW; Range:[0, 1]; Format:1.0;Enable/Disable AGamma Function */
    td_u32  scale;                   /* RW; Range:SS928V100 = [0xC8, 0x1400]; Format:3.10;
                                         AGamma LUT strength */
} ot_isp_agamma_attr;


typedef struct {
    td_u8  luma_wgt[OT_ISP_SHARPEN_LUMA_NUM]; /* RW; Range: SS928V100 =[0, 31];
                                               Format:0.5;Adjust the sharpen strength according to luma.
                                               Sharpen strength will be weaker when it decrease. */
    td_u16 texture_strength[OT_ISP_SHARPEN_GAIN_NUM]; /* RW; Range: [0, 4095]; Format:7.5;Undirectional sharpen
                                                         strength for texture and detail enhancement */
    td_u16 edge_strength[OT_ISP_SHARPEN_GAIN_NUM];    /* RW; Range: [0, 4095]; Format:7.5;
                                           Directional sharpen strength for edge enhancement */
    td_u16 texture_freq;       /* RW; Range: [0, 4095];Format:6.6; Texture frequency adjustment.
                                  Texture and detail will be finer when it increase */
    td_u16 edge_freq;          /* RW; Range: [0, 4095];Format:6.6; Edge frequency adjustment.
                                  Edge will be narrower and thiner when it increase */
    td_u8  over_shoot;         /* RW; Range: [0, 127]; Format:7.0;u8OvershootAmt */
    td_u8  under_shoot;        /* RW; Range: [0, 127]; Format:7.0;u8UndershootAmt */

    td_u16 motion_texture_strength[OT_ISP_SHARPEN_GAIN_NUM]; /* RW; Range: [0, 4095];Format:7.5 */
    td_u16 motion_edge_strength[OT_ISP_SHARPEN_GAIN_NUM]; /* RW; Range: [0, 4095]; Format:7.5 */
    td_u16 motion_texture_freq; /* RW; Range: [0, 4095]; Format:6.6 */
    td_u16 motion_edge_freq;   /* RW; Range: [0, 4095]; Format:6.6 */
    td_u8  motion_over_shoot;  /* RW; Range: [0, 127];  Format:7.0 */
    td_u8  motion_under_shoot; /* RW; Range: [0, 127];  Format:7.0 */

    td_u8  shoot_sup_strength; /* RW; Range: [0, 255]; Format:8.0;overshoot and undershoot suppression strength, the
                                  amplitude and width of shoot will be decrease when shootSupSt increase */
    td_u8  shoot_sup_adj;      /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot suppression adjusting,
                                  adjust the edge shoot suppression strength */
    td_u8  detail_ctrl;        /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for detail and edge.
                                  When it is bigger than 128, detail sharpen strength will be stronger than edge */
    td_u8  detail_ctrl_threshold; /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl, it is used to
                                  distinguish detail and edge. */
    td_u8  edge_filt_strength;      /* RW; Range: [0, 63]; Format:6.0;The strength of edge filtering. */
    td_u8  edge_filt_max_cap;  /* RW; Range: [0, 47]; Format:6.0;The max capacity of edge filtering. */
    td_u8  r_gain;             /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Red Area */
    td_u8  g_gain;             /* RW; Range: [0, 255]; Format:8.0; Sharpen Gain for Green Area */
    td_u8  b_gain;             /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Blue Area */
    td_u8  skin_gain;          /* RW; Range: [0, 31]; Format:5.0;Sharpen Gain for Skin Area */
    td_u16 max_sharp_gain;     /* RW; Range: [0, 0x7FF]; Format:8.3; Maximum sharpen gain */
} ot_isp_sharpen_manual_attr;

typedef struct {
    td_u8  luma_wgt[OT_ISP_SHARPEN_LUMA_NUM][OT_ISP_AUTO_ISO_NUM];  /* RW; Range:SS928V100 =[0, 31]; Format:0.5;
                                                                       Adjust the sharpen strength according to luma.
                                                                    Sharpen strength will be weaker when it decrease */
    td_u16 texture_strength[OT_ISP_SHARPEN_GAIN_NUM][OT_ISP_AUTO_ISO_NUM];  /* RW; Range: [0, 4095]; Format:7.5;
                                                                          Undirectional sharpen strength for texture
                                                                          and detail enhancement */
    td_u16 edge_strength[OT_ISP_SHARPEN_GAIN_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:7.5;
                                                                   Directional sharpen strength for edge enhancement */
    td_u16 texture_freq[OT_ISP_AUTO_ISO_NUM];   /* RW; Range: [0, 4095]; Format:6.6;Texture frequency adjustment.
                                                   Texture and detail will be finer when it increase */
    td_u16 edge_freq[OT_ISP_AUTO_ISO_NUM];      /* RW; Range: [0, 4095]; Format:6.6;Edge frequency adjustment.
                                                   Edge will be narrower and thiner when it increase */
    td_u8  over_shoot[OT_ISP_AUTO_ISO_NUM];     /* RW; Range: [0, 127];  Format:7.0 */
    td_u8  under_shoot[OT_ISP_AUTO_ISO_NUM];    /* RW; Range: [0, 127];  Format:7.0 */

    td_u16 motion_texture_strength[OT_ISP_SHARPEN_GAIN_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 4095];
                                                                                    Format:7.5 */
    td_u16 motion_edge_strength[OT_ISP_SHARPEN_GAIN_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:7.5 */
    td_u16 motion_texture_freq[OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:6.6 */
    td_u16 motion_edge_freq[OT_ISP_AUTO_ISO_NUM];   /* RW; Range: [0, 4095]; Format:6.6 */
    td_u8  motion_over_shoot[OT_ISP_AUTO_ISO_NUM];  /* RW; Range: [0, 127];  Format:7.0 */
    td_u8  motion_under_shoot[OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 127];  Format:7.0 */

    td_u8  shoot_sup_strength[OT_ISP_AUTO_ISO_NUM];  /* RW; Range: [0, 255]; Format:8.0;overshoot and undershoot
                                                   suppression strength, the amplitude and width of shoot will be
                                                   decrease when shootSupSt increase */
    td_u8  shoot_sup_adj[OT_ISP_AUTO_ISO_NUM];  /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot
                                                   suppression adjusting, adjust the edge shoot suppression strength */
    td_u8  detail_ctrl[OT_ISP_AUTO_ISO_NUM];    /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for
                                                   detail and edge. When it is bigger than 128, detail sharpen
                                                   strength will be stronger than edge. */
    td_u8  detail_ctrl_threshold[OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl,
                                                    it is used to distinguish detail and edge. */
    td_u8  edge_filt_strength[OT_ISP_AUTO_ISO_NUM];   /* RW; Range: [0, 63]; Format:6.0;
                                                    The strength of edge filtering. */
    td_u8  edge_filt_max_cap[OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 47]; Format:6.0;
                                                      The max capacity of edge filtering. */
    td_u8  r_gain[OT_ISP_AUTO_ISO_NUM];        /* RW; Range: [0, 31]; Format:5.0; Sharpen Gain for Red Area */
    td_u8  g_gain[OT_ISP_AUTO_ISO_NUM];        /* RW; Range: [0, 255]; Format:8.0; Sharpen Gain for Green Area */
    td_u8  b_gain[OT_ISP_AUTO_ISO_NUM];        /* RW; Range: [0, 31]; Format:5.0; Sharpen Gain for Blue Area */
    td_u8  skin_gain[OT_ISP_AUTO_ISO_NUM];      /* RW; Range: [0, 31]; Format:5.0;Sharpen Gain for Skin Area */
    td_u16 max_sharp_gain[OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0, 0x7FF]; Format:8.3; Maximum sharpen gain */
} ot_isp_sharpen_auto_attr;

typedef enum {
    OT_ISP_SHARPEN_NORMAL = 0,
    OT_ISP_SHARPEN_DETAIL,
    OT_ISP_SHARPEN_BUTT
} ot_isp_sharpen_detail_map;

typedef struct {
    td_bool en;    /* RW; Range:[0, 1]; Format:1.0;Enable/Disable sharpen module */
    td_bool motion_en;  /* RW; Range:[0, 1]; Format:1.0;Enable/Disable motion sharpen module */
    td_u8   motion_threshold0; /* RW; Range: [0, 15]; Format:8.0 */
    td_u8   motion_threshold1; /* RW; Range: [0, 15]; Format:8.0 */
    td_u16  motion_gain0;      /* RW; Range: [0, 256]; Format:16.0 */
    td_u16  motion_gain1;      /* RW; Range: [0, 256]; Format:16.0 */
    td_u8   skin_umin;   /* RW; Range: [0, 255];  Format:8.0; U min value of the range of skin area */
    td_u8   skin_vmin;   /* RW; Range: [0, 255];  Format:8.0; V min value of the range of skin area */
    td_u8   skin_umax;   /* RW; Range: [0, 255];  Format:8.0; U max value of the range of skin area */
    td_u8   skin_vmax;   /* RW; Range: [0, 255];  Format:8.0; V max value of the range of skin area */
    ot_op_mode op_type;
    ot_isp_sharpen_detail_map  detail_map;
    ot_isp_sharpen_manual_attr manual_attr;
    ot_isp_sharpen_auto_attr   auto_attr;
} ot_isp_sharpen_attr;

/* Crosstalk Removal */
typedef struct {
    td_bool  en;          /* RW; Range: [0, 1];Format 1.0;Enable/disable the crosstalk removal module */
    td_u8    slope;       /* RW; Range: SS928V100 = [0, 12]; Crosstalk slope value. */
    td_u8    sensi_slope; /* RW; Range: SS928V100 = [0, 12]; Crosstalk sensitivity. */
    td_u16   sensi_threshold; /* RW; Range: SS928V100 = [0, 4095]; Crosstalk sensitivity threshold. */
    td_u16   strength[OT_ISP_AUTO_ISO_NUM];   /* RW; Range: [0,256];Crosstalk strength value. */
    td_u16   threshold[OT_ISP_AUTO_ISO_NUM];  /* RW; Range: SS928V100 = [0, 4095]; Crosstalk threshold. */
    td_u16   np_offset[OT_ISP_AUTO_ISO_NUM];   /* RW; Range: SS928V100 = [512, 4095];
                                                  Set Noise profile value. */
} ot_isp_cr_attr;

typedef struct {
    td_u8    threshold;     /* RW;Range:[0x0,0x20];Format:6.0;Threshold for antifalsecolor */
    td_u8    strength;      /* RW;Range:[0x0,0x1F];Format:5.0;Strength of antifalsecolor */
} ot_isp_anti_false_color_manual_attr;

typedef struct {
    td_u8  threshold[OT_ISP_AUTO_ISO_NUM];    /* RW;Range:[0x0,0x20];Format:6.0;Threshold for antifalsecolor */
    td_u8  strength[OT_ISP_AUTO_ISO_NUM];     /* RW;Range:[0x0,0x1F];Format:5.0;Strength of antifalsecolor */
} ot_isp_anti_false_color_auto_attr;

typedef struct {
    td_bool  en;                               /* RW;Range:[0x0,0x1];Format:1.0; AntiFalseColor Enable */
    ot_op_mode op_type;
    ot_isp_anti_false_color_manual_attr manual_attr;
    ot_isp_anti_false_color_auto_attr   auto_attr;
} ot_isp_anti_false_color_attr;

typedef struct {
    td_u8   nddm_strength;            /* RW; Range:[0x0, 0xFF]; Format:4.4; Non-direction strength */
    td_u8   nddm_mf_detail_strength; /* RW; Range:SS928V100 = [0x0, 0x7f];
                                          Format:3.4; Non-direction medium frequent detail enhance  strength */
    td_u8   nddm_hf_detail_strength; /* RW; Range:[0x0, 0x10]; Format:2.2;
                                          Non-direction high frequent detail enhance strength */
    td_u8   detail_smooth_range;    /* RW; Range:SS928V100 = [0x1, 0x7];
                                       Format:4.0; Detail smooth range */
    td_u8   color_noise_f_threshold; /* RW;Range:[0x0,0xff]; Format:8.0; Threshold of color noise cancel */
    td_u8   color_noise_f_strength; /* RW;Range:[0x0, 0x8]; Format:4.0; Strength of color noise cancel */
    td_u8   color_noise_y_threshold; /* RW;Range:SS928V100 = [0x0, 0xF];
                                      Format:8.0; Range of color denoise luma, related to luminance and saturation */
    td_u8   color_noise_y_strength; /* RW;Range:SS928V100 = [0x0, 0x3F];
                                      Format:8.0;Format:8.0; Strength of color denoise luma */
} ot_isp_demosaic_manual_attr;

typedef struct {
    td_u8   nddm_strength[OT_ISP_AUTO_ISO_NUM];     /* RW; Range:[0x0, 0xFF]; Format:4.4; Non-direction strength */
    td_u8   nddm_mf_detail_strength[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:SS928V100 = [0x0, 0x7f];
                                                               Format:3.4; Non-direction medium frequent detail
                                                               enhance strength */
    td_u8   nddm_hf_detail_strength[OT_ISP_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0x10]; Format:2.2; Non-direction
                                                               high frequent detail enhance strength */
    td_u8   detail_smooth_range[OT_ISP_AUTO_ISO_NUM];    /* RW; Range:SS928V100 = [0x1, 0x7];
                                                            Format:4.0; Detail smooth range */
    td_u8   color_noise_f_threshold[OT_ISP_AUTO_ISO_NUM]; /* RW;Range:[0x0, 0xff]; Format:8.0;
                                                           Threshold of color noise cancel */
    td_u8   color_noise_f_strength[OT_ISP_AUTO_ISO_NUM]; /* RW;Range:[0x0, 0x8]; Format:4.0; Strength of color
                                                           noise cancel */
    td_u8   color_noise_y_threshold[OT_ISP_AUTO_ISO_NUM];  /* RW;Range:SS928V100 = [0x0, 0xF]; Format:8.0;
                                                    Range of color denoise luma, related to luminance and saturation */
    td_u8   color_noise_y_strength[OT_ISP_AUTO_ISO_NUM];  /* RW;Range:SS928V100 = [0x0, 0x3F]; Format:8.0;Format:8.0;
                                                            Strength of color denoise luma */
} ot_isp_demosaic_auto_attr;

typedef struct {
    td_bool en;        /* RW; Range:[0, 1]; Format:1.0;Enable/Disable demosaic module */
    ot_op_mode op_type;
    ot_isp_demosaic_manual_attr manual_attr;
    ot_isp_demosaic_auto_attr   auto_attr;
} ot_isp_demosaic_attr;

/* Defines the attributes of the ISP black level */
typedef enum {
    OT_ISP_BLACK_LEVEL_MODE_AUTO    = 0,
    OT_ISP_BLACK_LEVEL_MODE_MANUAL  = 1,
    OT_ISP_BLACK_LEVEL_MODE_DYNAMIC = 2,
    OT_ISP_BLACK_LEVEL_MODE_BUTT
} ot_isp_black_level_mode;

typedef struct {
    td_u16 black_level[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_BAYER_CHN_NUM]; /* RW; Range: [0x0, 0x3FFF];Format:14.0;
                                                 sensor Black level values that correspond to the black levels of the
                                                 R,Gr, Gb, and B components respectively. */
} ot_isp_black_level_manual_attr;

typedef enum {
    OT_ISP_DYNAMIC_PATTERN_RGB  = 0,
    OT_ISP_DYNAMIC_PATTERN_RGBIR,
    OT_ISP_DYNAMIC_PATTERN_BUTT
} ot_isp_black_level_dynamic_pattern;

typedef struct {
    ot_isp_black_level_dynamic_pattern pattern;  /* RW; Range: [0, 1]; The pattern of raw. */
    ot_rect ob_area;        /* RW; xRange: [0, width of the ob_area]; yRange: [0, height of the ob_area];
                            obWidthRange: [0, width of the ob_area - x]; obHeightRange: [0, height of ob_area - y];
                            The size of OB area. */
    td_u16  low_threshold;   /* RW; Range: [0x0, 0xFFF]; Format:14.0;Lower limit of normal pixel value; */
    td_u16  high_threshold;  /* RW; Range: [0x0, 0x3FFF]; Format:14.0;Higher limit of normal pixel value; */
    td_s16  offset[OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [-0x3FFF, 0x3FFF]; Format:15.0;Offset limit of dynamic blc; */
    td_u16  tolerance;       /* RW; Range: [0x0, 0x3FFF]; Format:14.0;Tolerance limit of dynamic blc; */
    td_u8   filter_strength; /* RW; Range: [0, 64];Format:6.0; ob black level minus light black level */
    td_bool separate_en;     /* RW; Range: [0, 1]; set rggb black level separately */
    td_u16  calibration_black_level[OT_ISP_AUTO_ISO_NUM]; /* RW; Range: [0x0, 0x3FFF]; Format:14.0;
                                                             dynamic blc of again; */
    td_u16  filter_thr;      /* RW; Range: [0, 0x3FFF]; Format:14.0; black level IIR filter threshold; */
} ot_isp_black_level_dynamic_attr;

typedef struct {
    td_bool  user_black_level_en;
    td_u16   user_black_level[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_BAYER_CHN_NUM];  /* RW; Range: [0x0, 0x3FFF];Format:14.0;
                                                 user define Black level values that correspond to the black levels of
                                                 the R,Gr, Gb, and B components respectively. */
    ot_isp_black_level_mode         black_level_mode;
    ot_isp_black_level_manual_attr  manual_attr;
    ot_isp_black_level_dynamic_attr dynamic_attr;
} ot_isp_black_level_attr;

/*
  Defines the FPN removal type
  0 = Frame mode correction
  1 = Line mode correction
 */
typedef enum {
    OT_ISP_FPN_TYPE_FRAME = 0,
    OT_ISP_FPN_TYPE_LINE = 1,
    OT_ISP_FPN_TYPE_BUTT
} ot_isp_fpn_type;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART03_H_ */
