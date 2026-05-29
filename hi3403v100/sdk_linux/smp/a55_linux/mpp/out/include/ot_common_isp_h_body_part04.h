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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART04_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART04_H_

/*
  Defines the FPN frame output mode
  0: the pixel value algin with the least significant bit (right-most bit)
  1: the pixel value algin with the most significant bit (left-most bit)
 */
typedef enum {
    OT_ISP_FPN_OUT_MODE_NORM = 0,
    OT_ISP_FPN_OUT_MODE_HIGH,
    OT_ISP_FPN_OUT_MODE_BUTT
} ot_isp_fpn_out_mode;

/* Defines the information about calibrated black frames */
typedef struct {
    td_u32              iso;             /* RW;Range:[0x64,0xFFFFFFFF];Format:32.0;FPN CALIBRATE ISO */
    td_u32              offset[OT_ISP_STRIPING_MAX_NUM]; /* RW;Range:[0, 0x3FFF];Format:14.0;
                                                          FPN frame u32Offset (agv pixel value),
                                                          only higher 12bits valid  */
    td_u32              frm_size;         /* RW;FPN frame size (exactly frm size or compress len) */
    ot_video_frame_info fpn_frame;         /* FPN frame info, 8bpp,10bpp,12bpp,16bpp. Compression or not */
} ot_isp_fpn_frame_info;

/* Defines the calibration attribute of the FPN removal module */
typedef struct {
    td_u32                 threshold;  /* RW;Range:[1,0xFFF];Format:12.0;pix value > threshold means defective pixel */
    td_u32                 frame_num;  /* RW;Range:[1, 16];Format:5.0;Number of frames to be calibrated.
                                        The value range is {1, 2, 4, 8, 16},that is, the integer exponentiation of 2 */
    ot_isp_fpn_type        fpn_type;   /* frame mode or line mode */
    ot_isp_fpn_frame_info  fpn_cali_frame;
    ot_isp_fpn_out_mode    fpn_mode;   /* FPN frame output mode, for normal use, set to 0 */
} ot_isp_fpn_calibrate_attr;

/* Defines the manual correction attribute of the FPN removal module */
typedef struct {
    td_u32  strength;         /* RW;Range:[0,1023];Format:10.0;Manual correction strength */
} ot_isp_fpn_manual_attr;

/* Defines the automatic correction attribute of the FPN removal module */
typedef struct {
    td_u32 strength;          /* RW;Range:[0,1023];Format:10.0;Auto correction strength */
} ot_isp_fpn_auto_attr;

/* Defines the correction attribute of the FPN removal module */
typedef struct {
    td_bool                enable;   /* RW;Range:[0,1];Format:1.0; */
    ot_op_mode            op_type;  /* manual or auto mode */
    ot_isp_fpn_type        fpn_type;
    ot_isp_fpn_frame_info  fpn_frm_info;
    ot_isp_fpn_manual_attr manual_attr;
    ot_isp_fpn_auto_attr   auto_attr;
} ot_isp_fpn_attr;

typedef struct {
    td_u16 stat[OT_ISP_DEHAZE_STAT_SIZE]; /* RW;Range:[0,0xFF]; */
} ot_isp_dehaze_stat;

/* Defines the manual dehaze attribute */
typedef struct {
    td_u8 strength;  /* RW;Range:[0,0xFF];Format:8.0;Manual dehaze strength */
} ot_isp_dehaze_manual_attr;

/* Defines the automatic dehaze attribute */
typedef struct {
    td_u8 strength; /* RW;Range:[0,0xFF];Format:8.0;Weighted coefficient for automatic dehaze strength. */
} ot_isp_dehaze_auto_attr;

/* Defines the ISP dehaze attribute */
typedef struct {
    td_bool  en;           /* RW;Range:[0,1];Format:1.0; */
    td_bool  user_lut_en;    /* RW;Range:[0,1];0:Auto Lut 1:User Lut */
    td_u8    dehaze_lut[OT_ISP_DEHAZE_LUT_SIZE];
    ot_op_mode                op_type;
    ot_isp_dehaze_manual_attr manual_attr;
    ot_isp_dehaze_auto_attr   auto_attr;
    td_u16       tmprflt_incr_coef; /* RW, Range: [0x0, 0x80].filter increase coefficient. */
    td_u16       tmprflt_decr_coef; /* RW, Range: [0x0, 0x80].filter decrease coefficient. */
} ot_isp_dehaze_attr;

/* Defines purple fringing correction manual attribute */
typedef struct {
    td_u8 de_purple_cr_strength;   /* RW;Range: [0,8];Format:4.0;Correction strength of the R channel */
    td_u8 de_purple_cb_strength;   /* RW;Range: [0,8];Format:4.0;Correction strength of the B channel */
} ot_isp_depurplestr_manual_attr;

/* Defines purple fringing correction automatic attribute */
typedef struct {
    td_u8 de_purple_cr_strength[OT_ISP_LCAC_EXP_RATIO_NUM]; /* RW;Range: [0, 8];Format:4.0;
                                                         Correction strength of the R channel */
    td_u8 de_purple_cb_strength[OT_ISP_LCAC_EXP_RATIO_NUM]; /* RW;Range: [0, 8];Format:4.0;
                                                         Correction strength of the B channel */
} ot_isp_depurplestr_auto_attr;

/* Purple fringing detection and correction attribute */
typedef struct {
    td_bool en;               /* RW; Range: [0, 1];Format:1.0; en */
    td_u16  purple_detect_range;     /* RW;Range: [0, 410];Format:9.0;Purple fringing detection scope */
    td_u16  var_threshold;              /* RW;Range: [0, 4095];Format:12.0;Edge detection threshold */
    td_u16  r_detect_threshold[OT_ISP_LCAC_DET_NUM]; /* RW;Range: [0,4095];Format:12.0;Component R detection thd */
    td_u16  g_detect_threshold[OT_ISP_LCAC_DET_NUM]; /* RW;Range: [0,4095];Format:12.0;Component G detection thd */
    td_u16  b_detect_threshold[OT_ISP_LCAC_DET_NUM]; /* RW;Range: [0,4095];Format:12.0;Component B detection thd */
    td_u16  l_detect_threshold[OT_ISP_LCAC_DET_NUM]; /* RW;Range: [0,4095];Format:12.0;Component Luma detection thd */
    td_s16  cb_cr_ratio[OT_ISP_LCAC_DET_NUM];  /* RW;Range: [-2048,2047];Format:S12.0; Cb/Cr ratio threshold */
    ot_op_mode      op_type;  /* RW;Range: [0, 1];Format:1.0;Purple fringing correction working mode */
    ot_isp_depurplestr_manual_attr manual_attr;
    ot_isp_depurplestr_auto_attr   auto_attr;
} ot_isp_local_cac_attr;

/* defines purple fringing correction automatic attribute */
typedef struct {
    td_u16  edge_threshold[OT_ISP_ACAC_THR_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; range: [0, 4095];
                                                                           format:12.0; cac edge threshold */
    td_u16  edge_gain[OT_ISP_AUTO_ISO_NUM];          /* RW; range: [0, 1023]; format:10.0; acac strength */
    td_s16  purple_upper_limit[OT_ISP_AUTO_ISO_NUM]; /* RW; range: [-511, 511]; format:10.0;
                                                             acac purple high limitation */
    td_s16  purple_lower_limit[OT_ISP_AUTO_ISO_NUM];   /* RW; range: [-511, 511]; format:10.0;
                                                               acac purple  low limitation */
    td_u16  purple_sat_threshold[OT_ISP_AUTO_ISO_NUM];  /* RW; range: [0, 2047]; format:11.0; acac purple sat */
    td_u16  purple_alpha[OT_ISP_AUTO_ISO_NUM];         /* RW; range: [0, 63]; format:6.0; acac purple alpha */
    td_u16  edge_alpha[OT_ISP_AUTO_ISO_NUM];           /* RW; range: [0, 63]; format:6.0; acac edge  alpha */
    td_u16  fcc_y_strength[OT_ISP_AUTO_ISO_NUM];       /* RW; range: [0, 4095]; format:12.0; FCC on Y */
    td_u16  fcc_rb_strength[OT_ISP_AUTO_ISO_NUM];      /* RW; range: [0, 511]; format:10.0; FCC on rb */
} isp_acac_auto_attr;

typedef struct {
    td_u16  edge_threshold[OT_ISP_ACAC_THR_NUM];  /* RW; range: [0, 4095]; format:12.0; cac edge threshold */
    td_u16  edge_gain;              /* RW; range: [0, 1023]; format:10.0; acac edge gain */
    td_s16  purple_upper_limit;     /* RW; range: [-511, 511]; format:10.0; acac purple high limitation */
    td_s16  purple_lower_limit;     /* RW; range: [-511, 511]; format:10.0; acac purple low limitation */
    td_u16  purple_sat_threshold;    /* RW; range: [0, 2047]; format:11.0; acac purple sat */
    td_u16  purple_alpha;        /* RW; range: [0, 63]; format:6.0; acac purple alpha */
    td_u16  edge_alpha;          /* RW; range: [0, 63]; format:6.0; acac edge  alpha */
    td_u16  fcc_y_strength;      /* RW; range: [0, 4095]; format:12.0; FCC on Y */
    td_u16  fcc_rb_strength;     /* RW; range: [0, 511]; format:10.0; FCC on rb */
} isp_acac_manual_attr;

typedef struct {
    td_bool en;                  /* RW; range: [0, 1]; format:1.0; acac en */
    td_u8   detect_mode;         /* RW; range: [0, 1];   format:1.0; acac edge detect mode */
    ot_op_mode      op_type;     /* RW; range: [0,1];format:1.0;purple fringing correction working mode */
    isp_acac_auto_attr     auto_attr;
    isp_acac_manual_attr   manual_attr;
} ot_isp_acac_attr;

typedef struct {
    td_u8  luma_wgt[OT_ISP_BSHP_LUMA_WGT_NUM]; /* RW; range:[0, 31]; format:4.0; luma weight, parameter */
    td_u16 edge_mf_strength[OT_ISP_BSHP_HALF_CURVE_NUM]; /* RW; range:[0, 1023]; edge_course_str, parameter */
    td_u16 texture_mf_strength[OT_ISP_BSHP_HALF_CURVE_NUM]; /* RW; range:[0, 1023];texture_mf_strength parameter */
    td_u16 edge_hf_strength[OT_ISP_BSHP_HALF_CURVE_NUM];   /* RW; range:[0, 1023] edge_hf_strength, parameter */
    td_u16 texture_hf_strength[OT_ISP_BSHP_HALF_CURVE_NUM]; /* RW; range:[0, 1023]  texture_hf_strength, parameter */
    td_u8  edge_filt_strength;     /* RW; range:[0, 127]; format:7.0;the strength of edge filtering */
    td_u16 texture_max_gain;       /* RW; range:[0, 1023]; format10.0; max value gain of detail sharpen */
    td_u16 edge_max_gain;          /* RW; range:[0, 1023]; format10.0; max value gain of edge sharpen */
    td_u16 overshoot;              /* RW; range:[0, 127]; format7.0; overshoot */
    td_u16 undershoot;             /* RW; range:[0, 127]; format7.0; undershoot */
    td_u16 g_chn_gain;             /* RW; range:[0, 127]; format:7.0; sharpen on G inter param */
} ot_isp_bayershp_manual_attr;

typedef struct {
    td_u8  luma_wgt[OT_ISP_BSHP_LUMA_WGT_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; range:[0, 31];
                                                                    format:4.0; luma weight, control parameter */
    td_u16 edge_mf_strength[OT_ISP_BSHP_HALF_CURVE_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; range:[0, 1023];
                                                                              edge_course_str, control parameter */
    td_u16 texture_mf_strength[OT_ISP_BSHP_HALF_CURVE_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; range:[0, 1023];
                                                                        texture_mf_strength , control parameter */
    td_u16 edge_hf_strength[OT_ISP_BSHP_HALF_CURVE_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; range:[0, 1023];
                                                                              edge_hf_strength, control parameter */
    td_u16 texture_hf_strength[OT_ISP_BSHP_HALF_CURVE_NUM][OT_ISP_AUTO_ISO_NUM]; /* RW; range:[0, 1023];
                                                                              texture_fine_str, control parameter */
    td_u8  edge_filt_strength[OT_ISP_AUTO_ISO_NUM]; /* RW; range: [0, 127];format:7.0;the strength of edge filtering. */
    td_u16 texture_max_gain[OT_ISP_AUTO_ISO_NUM];       /* RW; range:[0, 1023];format10.0; max value gain of sharpen */
    td_u16 edge_max_gain[OT_ISP_AUTO_ISO_NUM];          /* RW; range:[0, 1023];format10.0; max value gain of sharpen */
    td_u16 overshoot[OT_ISP_AUTO_ISO_NUM];              /* RW; range:[0, 127];format10.0; overshoot */
    td_u16 undershoot[OT_ISP_AUTO_ISO_NUM];             /* RW; range:[0, 127];format10.0; undershoot */
    td_u16 g_chn_gain[OT_ISP_AUTO_ISO_NUM];             /* RW; range:[0, 127];format:7.0; sharpen on G */
} ot_isp_bayershp_auto_attr;

typedef struct {
    td_bool en;         /* RW; range:[0, 1]; format:1.0;en/disable demosaic module */
    ot_op_mode op_type;
    ot_isp_bayershp_manual_attr manual_attr;
    ot_isp_bayershp_auto_attr   auto_attr;
} ot_isp_bayershp_attr;

/* Defines the Radial Crop attribute */
typedef struct {
    td_bool  en;           /* RW;Range:[0, 1];Format:1.0; enable/disable ridial crop */
    ot_point center_coord; /* RW;the coordinate of central pixel */
    td_u32   radius;       /* RW;Range:[0, 11586];Format:14.0; when the distance to central pixel is greater than
                              u32Radius, the pixel value becomes 0. */
} ot_isp_rc_attr;

typedef struct {
    td_u16 texture_strength[OT_ISP_SHARPEN_GAIN_NUM]; /* RW; range: [0, 4095]; Format:7.5;Undirectional sharpen
                                                         strength for texture and detail enhancement */
    td_u16 edge_strength[OT_ISP_SHARPEN_GAIN_NUM];    /* RW; range: [0, 4095]; Format:7.5;Directional sharpen strength
                                                         for edge enhancement */
    td_u16 texture_freq;         /* RW; range: [0, 4095];Format:6.6; Texture frequency adjustment.
                                    Texture and detail will be finer when it increase */
    td_u16 edge_freq;            /* RW; range: [0, 4095];Format:6.6; Edge frequency adjustment.
                                    Edge will be narrower and thiner when it increase */
    td_u8  over_shoot;           /* RW; range: [0, 127]; Format:7.0;u8OvershootAmt */
    td_u8  under_shoot;          /* RW; range: [0, 127]; Format:7.0;u8UndershootAmt */
    td_u8  shoot_sup_strength;   /* RW; range: [0, 255]; Format:8.0;overshoot and undershoot suppression strength,
                                    the amplitude and width of shoot will be decrease when shootSupSt increase */

    td_u8   nr_lsc_ratio;                     /* RW;Range:[0x0, 0xff];Format:8.0; Strength of reserving the random
                                                 noise according to luma */
    td_u16  coarse_strength[OT_ISP_BAYER_CHN_NUM];    /* RW;Range:[0x0, 0x3ff];Format:10.0;
                                                 Coarse Strength of noise reduction */
    td_u8   wdr_frame_strength[OT_ISP_WDR_MAX_FRAME_NUM]; /* RW;Range:[0x0, 0x50];Format:7.0;
                                                 Coarse strength of each frame in wdr mode */
    td_u8   fine_strength;          /* RW;Range:[0x0,0x80];Format:8.0;Strength of Luma noise reduction */
    td_u16  coring_wgt;             /* RW;Range:[0x0,0xC80];Format:12.0;Strength of reserving the random noise */

    td_u16 dehaze_strength_actual; /* RW;Range:[0,0xFF];Format:8.0;actual dehaze strength */
    td_u16 drc_strength_actual;    /* RW;Range: SS928V100 = [0x0, 0x3FF];
                                    Strength of dynamic range compression.
                                    Higher values lead to higher differential gain between shadows and highlights. */
    td_u32 wdr_exp_ratio_actual[OT_ISP_WDR_MAX_FRAME_NUM - 1]; /* RW; Range:[0x40, 0x4000]; Format:26.6;
                                                         0x40 means 1 times.
                                                         When enExpRatioType is OT_OP_MODE_AUTO, u32ExpRatio is invalid.
                                                         When enExpRatioType is OT_OP_MODE_MANUAL, u32ExpRatio is
                                                         quotient of long exposure time / short exposure time. */
    td_bool wdr_switch_finish;       /* RW; Range:[0, 1];Format:1.0;TD_TRUE: WDR switch is finished */
    td_bool res_switch_finish;       /* RW; Range:[0, 1];Format:1.0;TD_TRUE: Resolution switch is finished */
    td_u16  black_level_actual[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_BAYER_CHN_NUM]; /* RW; Range: [0x0, 0x3FFF];Format:14.0;
                                             Actual Black level values that correspond to the black levels of
                                             the R,Gr, Gb, and B components respectively. */

    td_u16  sns_black_level[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_BAYER_CHN_NUM]; /* RW; Range: [0x0, 0x3FFF];Format:14.0;
                                                      sesnor Black level values that correspond to the black levels of
                                                      the R,Gr, Gb, and B components respectively. */
} ot_isp_inner_state_info;

/*
  The position of AE histogram in ISP BE pipeline
  0 = After DG;
  1 = After static WB;
  2 = After DRC;
 */
typedef enum {
    OT_ISP_AE_AFTER_DG = 0,
    OT_ISP_AE_AFTER_WB,
    OT_ISP_AE_AFTER_DRC,
    OT_ISP_AE_SWITCH_BUTT
} ot_isp_ae_switch;

/*
  FourPlaneMode enable
  0 : Disabled;
  1 : Enabled
*/
typedef enum {
    OT_ISP_AE_FOUR_PLANE_MODE_DISABLE = 0,
    OT_ISP_AE_FOUR_PLANE_MODE_ENABLE,
    OT_ISP_AE_FOUR_PLANE_MODE_BUTT
} ot_isp_ae_four_plane_mode;

typedef enum {
    OT_ISP_AE_HIST_SKIP_EVERY_PIXEL = 0,
    OT_ISP_AE_HIST_SKIP_EVERY_2ND_PIXEL,
    OT_ISP_AE_HIST_SKIP_EVERY_3RD_PIXEL,
    OT_ISP_AE_HIST_SKIP_EVERY_4TH_PIXEL,
    OT_ISP_AE_HIST_SKIP_EVERY_5TH_PIXEL,
    OT_ISP_AE_HIST_SKIP_EVERY_8TH_PIXEL,
    OT_ISP_AE_HIST_SKIP_EVERY_9TH_PIXEL,
    OT_ISP_AE_HIST_SKIP_BUTT
} ot_isp_ae_hist_skip;

typedef enum {
    OT_ISP_AE_HIST_START_FORM_FIRST_COLUMN = 0,
    OT_ISP_AE_HIST_START_FORM_SECOND_COLUMN,
    OT_ISP_AE_HIST_OFFSET_X_BUTT
} ot_isp_ae_hist_offset_x;

typedef enum {
    OT_ISP_AE_HIST_START_FORM_FIRST_ROW = 0,
    OT_ISP_AE_HIST_START_FORM_SECOND_ROW,
    OT_ISP_AE_HIST_OFFSET_Y_BUTT
} ot_isp_ae_hist_offset_y;

/* Defines the mode configuration for the sampling points during global histogram statistics. */
typedef struct {
    ot_isp_ae_hist_skip hist_skip_x;  /* RW; Range:[0, 6]; Format:4.0;
                                         Histogram decimation in horizontal direction: 0=every pixel;
                                         1=every 2nd pixel; 2=every 3rd pixel; 3=every 4th pixel; 4=every 5th pixel;
                                         5=every 8th pixel ; 6+=every 9th pixel */
    ot_isp_ae_hist_skip hist_skip_y;  /* RW; Range:[0, 6]; Format:4.0;
                                         Histogram decimation in vertical direction: 0=every pixel; 1=every 2nd pixel;
                                         2=every 3rd pixel; 3=every 4th pixel; 4=every 5th pixel; 5=every 8th pixel;
                                         6+=every 9th pixel */
    ot_isp_ae_hist_offset_x hist_offset_x;  /* RW; Range:[0, 1]; Format:1.0;
                                               0= start from the first column; 1=start from second column */
    ot_isp_ae_hist_offset_y hist_offset_y;  /* RW; Range:[0, 1]; Format:1.0;
                                               0= start from the first row; 1= start from second row */
} ot_isp_ae_hist_config;

typedef enum {
    OT_ISP_AE_NORMAL = 0,
    OT_ISP_AE_ROOT,
    OT_ISP_AE_STAT_MODE_BUTT
} ot_isp_ae_stat_mode;

/* Crops the input image of the AE module */
typedef struct {
    td_bool en;  /* RW; Range: [0,1];  Format:1.0;AE crop enable. */
    td_u16  x;   /* RW; Range: SS928V100 = [0, 8192 - 256];
                    Format:13.0;AE image crop start x, limited range:[0, ImageWidth - 256] */
    td_u16  y;   /* RW; Range: SS928V100 = [0, 8192 - 120];
                    Format:13.0;AEimage crop start y, limited range:[0, ImageHeight - 120]  */
    td_u16  width;   /* RW; Range: SS928V100 = [256, 8192];
                    Format:14.0;AE image crop width,  limited range:[256, ImageWidth] */
    td_u16  height;   /* RW; Range: SS928V100 = [120, 8192];
                    Format:14.0;AE image crop height  limited range:[120, ImageHeight] */
} ot_isp_ae_crop;

/* config of statistics structs */
#define OT_ISP_HIST_THRESH_NUM                           4
typedef struct {
    ot_isp_ae_switch          ae_switch;          /* RW; Range:[0, 2]; Format:2.0;
                                                     The position of AE histogram in ISP BE pipeline */
    ot_isp_ae_hist_config     hist_config;
    ot_isp_ae_four_plane_mode four_plane_mode;    /* RW; Range:[0, 1]; Format:2.0;Four Plane Mode Enable */
    ot_isp_ae_stat_mode       hist_mode;          /* RW; Range:[0, 1]; Format:2.0;AE Hist Rooting Mode */
    ot_isp_ae_stat_mode       aver_mode;          /* RW; Range:[0, 1]; Format:2.0;AE Aver Rooting Mode */
    ot_isp_ae_stat_mode       max_gain_mode;      /* RW; Range:[0, 1]; Format:2.0;Max Gain Rooting Mode */
    ot_isp_ae_crop            crop;               /* RW; AE input image crop for  module */
    ot_isp_ae_crop            fe_crop;            /* RW; AE input image crop for FE module */
    td_u8 weight[OT_ISP_AE_ZONE_ROW][OT_ISP_AE_ZONE_COLUMN]; /* RW; Range:[0x0, 0xF]; Format:4.0; AE weighting table */
} ot_isp_ae_stats_cfg;

/* Smart Info */
#define OT_ISP_PEOPLE_CLASS_MAX   2
typedef struct {
    td_bool  enable;
    td_bool  available;
    td_u8    luma;
} ot_isp_people_roi;

typedef enum {
    OT_ISP_FACE_INDEX = 0,
    OT_ISP_PEOPLE_INDEX = 1,
    OT_ISP_PEOPLE_BUTT
} ot_isp_people_type;

#define OT_ISP_TUNNEL_CLASS_MAX   2
typedef struct {
    td_bool  enable;
    td_bool  available;
    td_u32   tunnel_area_ratio;
    td_u32   tunnel_exp_perf;
} ot_isp_tunnel_roi;

typedef enum {
    OT_ISP_TUNNEL_IN_INDEX = 0,
    OT_ISP_TUNNEL_OUT_INDEX = 1,
    OT_ISP_TUNNEL_BUTT
} ot_isp_tunnel_type;

#define OT_ISP_FACE_NUM 5
typedef struct {
    td_bool  enable;
    td_bool  available;
    td_u64   frame_pts;
    ot_rect face_rect[OT_ISP_FACE_NUM];
} ot_isp_face_roi;

typedef struct {
    ot_isp_people_roi  people_roi[OT_ISP_PEOPLE_CLASS_MAX];
    ot_isp_tunnel_roi  tunnel_roi[OT_ISP_TUNNEL_CLASS_MAX];
    ot_isp_face_roi face_roi;
} ot_isp_smart_info;

/* fines whether the peak value of the zoned IIR statistics is calculated.  */
typedef enum {
    OT_ISP_AF_STA_NORM         = 0,    /* use every value of the block statistic */
    OT_ISP_AF_STA_PEAK,                /* use the maximum value in one line of the block statistic */
    OT_ISP_AF_STA_BUTT
} ot_isp_af_peak_mode;

/* Defines whether the zoned statistics are squared in square mode.  */
typedef enum {
    OT_ISP_AF_STA_SUM_NORM     = 0,    /* statistic value accumulate */
    OT_ISP_AF_STA_SUM_SQUARE,          /* statistic value square then accumulate */
    OT_ISP_AF_STA_SUM_BUTT
} ot_isp_af_square_mode;
/* Crops the input image of the AF module.  */
typedef struct {
    td_bool en;   /* RW; Range: [0,1];  Format:1.0; AF crop enable */
    td_u16  x;    /* RW; Range: SS928V100 = [0, 7936];
                     Format:13.0;AF image crop start x, limited range:[0, ImageWidth-256] */
    td_u16  y;    /* RW; Range: SS928V100 = [0, 8072];
                     Format:13.0;AF image crop start y, limited range:[0, ImageHeight-120] */
    td_u16  width;    /* RW; Range: SS928V100 = [256, 8192];
                     Format:14.0;AF image crop width, limited range:[256, ImageWidth] */
    td_u16  height;    /* RW; Range: SS928V100 = [120, 8192];
                     Format:14.0;AF image crop height, limited range:[120, ImageHeight] */
} ot_isp_af_crop;

/* Defines the position of AF module statistics.  */
typedef enum {
    OT_ISP_AF_STATS_AFTER_DGAIN = 0, /* The AF module is placed in the raw field for statistics,AF after DGain */
    OT_ISP_AF_STATS_AFTER_DRC,       /* The AF module is placed in the raw field for statistics,AF after DRC */
    OT_ISP_AF_STATS_AFTER_CSC,       /* The AF module is placed in the YUV field for statistics,AF after CSC */
    OT_ISP_AF_STATS_BUTT
} ot_isp_af_stats_pos;

/* Configures the Bayer field of the AF module.  */
typedef struct {
    td_u8               gamma_gain_limit; /* RW; Range: [0x0, 0x5]; Format:3.0 */
    td_u8               gamma_value;      /* RW; Range: SS928V100 = [0x0, 0x6]; */
    ot_isp_bayer_format bayer_format;     /* RW; Range: [0x0, 0x3]; Format:2.0;raw domain pattern */
} ot_isp_af_raw_cfg;

/* Configures the pre-filter of the AF module.  */
typedef struct {
    td_bool             en;       /* RW; Range: [0,1]; Format:1.0; pre filter enable  .          */
    td_u16              strength; /* RW; Range: [0x0, 0xFFFF]; Format:16.0;pre filter strength    */
} ot_isp_af_pre_filter_cfg;

/* Defines AF statistics configuration parameters.  */
typedef struct {
    td_bool                  af_en;         /* RW; Range: [0,1];   AF enable. */
    td_u16                   zone_col;      /* RW; Range: [1, 17]; AF statistic window horizontal block.  */
    td_u16                   zone_row;      /* RW; Range: [1, 15]; AF statistic window vertical block.  */
    ot_isp_af_peak_mode      peak_mode;     /* RW; Range: [0,1]; AF peak value statistic mode. */
    ot_isp_af_square_mode    square_mode;   /* RW; Range: [0,1]; AF statistic square accumulate. */
    ot_isp_af_crop           crop;          /* RW; AF input image crop */
    ot_isp_af_crop           fe_crop;       /* RW; AF input image crop for FE module */
    ot_isp_af_stats_pos      stats_pos;     /* RW; Range: [0,2]; AF statistic position, it can be set to yuv or raw */
    ot_isp_af_raw_cfg        raw_cfg;       /* RW; When AF locate at RAW domain, these para should be cfg. */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART04_H_ */
