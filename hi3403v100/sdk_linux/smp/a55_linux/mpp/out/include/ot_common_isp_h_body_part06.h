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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART06_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART06_H_

    td_u32 max_pwm_duty;   /* RW; Range:[0, 1000]; Format:32.0; which is the max pwm duty for dciris control */
    td_u32 open_pwm_duty;  /* RW; Range:[0, 1000]; Format:32.0; which is the open pwm duty for dciris control */
} ot_isp_dciris_attr;

typedef struct {
    td_bool step_fno_table_change;    /* W; Range:[0, 1]; Format:1.0; Step-F number mapping table change or not */
    td_bool zero_is_max;              /* RW; Range:[0, 1]; Format:1.0; Step 0 corresponds to max aperture or not,
                                         it's related to the specific iris */
    td_u16  total_step;               /* RW; Range:[1, 1024]; Format:16.0; Total steps of  Piris's aperture,
                                         it's related to the specific iris */
    td_u16  step_count;               /* RW; Range:[1, 1024]; Format:16.0; Used steps of Piris's aperture.
                                         when Piris's aperture is too small, the F number precision is not enough,
                                         so not all the steps can be used. It's related to the specific iris */
    td_u16 step_fno_table[OT_ISP_AI_MAX_STEP_FNO_NUM]; /* RW; Range:[1, 1024]; Format:16.0; Step-F number mapping table.
                                                    F1.0 is expressed as 1024, F32 is expressed as 1,
                                                    it's related to the specific iris */
    ot_isp_iris_f_no  max_iris_fno_target;  /* RW; Range:[0, 10]; Format:4.0;
                                               Max F number of Piris's aperture, it's related to the specific iris */
    ot_isp_iris_f_no  min_iris_fno_target;  /* RW; Range:[0, 10]; Format:4.0;
                                               Min F number of Piris's aperture, it's related to the specific iris */

    td_bool fno_ex_valid;           /* RW; Range:[0, 1]; Format:1.0; use equivalent gain to present FNO or not */
    td_u32  max_iris_fno_target_linear;  /* RW; Range:[1, 1024]; Format:16.0;
                                            Max equivalent gain of F number of Piris's aperture,
                                            only used when bFNOExValid is true, it's related to the specific iris */
    td_u32  min_iris_fno_target_linear;  /* RW; Range:[1, 1024]; Format:16.0;
                                            Min equivalent gain of F number of Piris's aperture,
                                            only used when bFNOExValid is true, it's related to the specific iris */
} ot_isp_piris_attr;

/* Defines the MI attribute */
typedef struct {
    td_u32  hold_value;           /* RW; Range:[0, 1000]; Format:32.0; iris hold value for DC-iris */
    ot_isp_iris_f_no  iris_fno;   /* RW; Range:[0, 10]; Format:4.0; the F number of P-iris */
} ot_isp_mi_attr;

/*
  Defines the ISP iris status
  0 = In this mode can set the MI holdvalue
  1 = Open Iris to the max
  2 = Close Iris to the min
 */
typedef enum {
    OT_ISP_IRIS_KEEP  = 0,
    OT_ISP_IRIS_OPEN  = 1,
    OT_ISP_IRIS_CLOSE = 2,
    OT_ISP_IRIS_BUTT
} ot_isp_iris_status;

/* Defines the iris attribute */
typedef struct {
    td_bool enable;                    /* RW;iris enable/disable */
    ot_op_mode         op_type;        /* auto iris or manual iris */
    ot_isp_iris_type   iris_type;      /* DC-iris or P-iris */
    ot_isp_iris_status iris_status;    /* RW; status of Iris */
    ot_isp_mi_attr     mi_attr;
} ot_isp_iris_attr;

/* AE structs */
/*
  Defines the AE mode
  0 = Automatic frame rate reduction mode (slow shutter mode)
  1 = Constant frame rate mode
 */
typedef enum {
    OT_ISP_AE_MODE_SLOW_SHUTTER = 0,
    OT_ISP_AE_MODE_FIX_FRAME_RATE  = 1,
    OT_ISP_AE_MODE_BUTT
} ot_isp_ae_mode;
/*
  Defines the AE exposure policy mode
  0 = Highlight first exposure mode
  1 = Lowlight first exposure mode
 */
typedef enum {
    OT_ISP_AE_EXP_HIGHLIGHT_PRIOR = 0,
    OT_ISP_AE_EXP_LOWLIGHT_PRIOR  = 1,
    OT_ISP_AE_STRATEGY_MODE_BUTT
} ot_isp_ae_strategy;
/* Defines the maximum exposure time or gain and minimum exposure time or gain */
typedef struct {
    td_u32 max;  /* RW;Range:[0,0xFFFFFFFF];Format:32.0;Maximum value */
    td_u32 min;  /* RW;Range:[0,0xFFFFFFFF];Format:32.0;limited Range:[0,u32Max],Minimum value */
} ot_isp_ae_range;

/* Defines the ISP exposure delay attribute */
typedef struct {
    td_u16 black_delay_frame;    /* RW; Range:[0, 65535]; Format:16.0; AE black delay frame count */
    td_u16 white_delay_frame;    /* RW; Range:[0, 65535]; Format:16.0; AE white delay frame count */
} ot_isp_ae_delay;

/*
  Defines the anti-flicker mode.
  0 = The epxosure time is fixed to be the multiplies of 1/(2*frequency) sec,
    it may lead to over exposure in the high-luminance environments.
  1 = The anti flicker may be closed to avoid over exposure in the high-luminance environments.
 */
typedef enum {
    OT_ISP_ANTIFLICKER_NORMAL_MODE = 0x0,
    OT_ISP_ANTIFLICKER_AUTO_MODE   = 0x1,
    OT_ISP_ANTIFLICKER_MODE_BUTT
} ot_isp_antiflicker_mode;

/* Defines the anti-flicker attribute */
typedef struct {
    td_bool enable;      /* RW; Range:[0, 1]; Format:1.0; */
    td_u8   frequency;   /* RW; Range:[0, 255]; Format:8.0;
                            usually this value is 50 or 60  which is the frequency of the AC power supply */
    ot_isp_antiflicker_mode  mode;
} ot_isp_antiflicker;

typedef struct {
    td_bool enable; /* RW; Range:[0, 1]; Format:1.0; */

    td_u8   luma_diff; /* RW; Range:[0, 100]; Format:8.0; if subflicker mode enable, current luma is less than AE
                          compensation plus LumaDiff, AE will keep min antiflicker shutter time
                          (for example: 1/100s or 1/120s) to avoid flicker. while current luma is larger than AE
                          compensation plus the LumaDiff, AE will reduce shutter time to avoid over-exposure and
                          introduce flicker in the pircture */
} ot_isp_subflicker;

typedef struct {
    /* base parameter */
    ot_isp_ae_range exp_time_range;   /* RW; Range:[0x0, 0xFFFFFFFF]; Format:32.0; sensor exposure time (unit: us),
                                         it's related to the specific sensor */
    ot_isp_ae_range a_gain_range;     /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                         sensor analog gain (unit: times, 10bit precision), it's related to the
                                         specific sensor */
    ot_isp_ae_range d_gain_range;     /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                         sensor digital gain (unit: times, 10bit precision), it's related to the
                                         specific sensor */
    ot_isp_ae_range ispd_gain_range;  /* RW; Range:[0x400, 0x40000]; Format:22.10;
                                         ISP digital gain (unit: times, 10bit precision), it's related to the
                                         ISP digital gain range */
    ot_isp_ae_range sys_gain_range;   /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                         system gain (unit: times, 10bit precision), it's related to the specific sensor
                                         and ISP Dgain range */
    td_u32 gain_threshold;            /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                         Gain threshold for slow shutter mode (unit: times, 10bit precision) */

    td_u8   speed;                   /* RW; Range:[0x0, 0xFF]; Format:8.0;
                                        AE adjust step for dark scene to bright scene switch */
    td_u16  black_speed_bias;        /* RW; Range:[0x0, 0xFFFF]; Format:16.0;
                                        AE adjust step bias for bright scene to dark scene switch */
    td_u8   tolerance;               /* RW; Range:[0x0, 0xFF]; Format:8.0; AE adjust tolerance */
    td_u8   compensation;            /* RW; Range:[0x0, 0xFF]; Format:8.0; AE compensation */
    td_u16  ev_bias;                 /* RW; Range:[0x0, 0xFFFF]; Format:16.0; AE EV bias */
    ot_isp_ae_strategy ae_strategy_mode;  /* RW; Range:[0, 1]; Format:1.0; Support Highlight prior or Lowlight prior */
    td_u16  hist_ratio_slope;       /* RW; Range:[0x0, 0xFFFF]; Format:16.0; AE hist ratio slope */
    td_u8   max_hist_offset;        /* RW; Range:[0x0, 0xFF]; Format:8.0; Max hist offset */

    ot_isp_ae_mode     ae_mode;     /* RW; Range:[0, 1]; Format:1.0; AE mode(slow shutter/fix framerate)(onvif) */
    ot_isp_antiflicker antiflicker;
    ot_isp_subflicker  subflicker;
    ot_isp_ae_delay    ae_delay_attr;

    td_bool manual_exp_value;     /* RW; Range:[0, 1]; Format:1.0; manual exposure value or not */
    td_u32  exp_value;            /* RW; Range:(0x0, 0xFFFFFFFF]; Format:32.0; manual exposure value */

    ot_isp_fswdr_mode fswdr_mode; /* RW; Range:[0, 2]; Format:2.0; FSWDR running mode */
    td_bool wdr_quick;            /* RW; Range:[0, 1]; Format:1.0; WDR use delay strategy or not;
                                     If is true, WDR AE adjust will be faster */

    td_u16  iso_cal_coef;     /* RW; Range:[0x0, 0xFFFF]; Format:8.8;
                                 The coefficient between Standard ISO and origin ISO (unit: times, 8bit precision) */
} ot_isp_ae_attr;

typedef struct {
    ot_op_mode exp_time_op_type;
    ot_op_mode a_gain_op_type;
    ot_op_mode d_gain_op_type;
    ot_op_mode ispd_gain_op_type;

    td_u32 exp_time;   /* RW; Range:[0x0, 0xFFFFFFFF]; Format:32.0;
                          sensor exposure time (unit: us), it's related to the specific sensor */
    td_u32 a_gain;     /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                          sensor analog gain (unit: times, 10bit precision), it's related to the specific sensor */
    td_u32 d_gain;     /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                          sensor digital gain (unit: times, 10bit precision), it's related to the specific sensor */
    td_u32 isp_d_gain; /* RW; Range:[0x400, 0x40000]; Format:22.10;
                          ISP digital gain(unit: times, 10bit precision), it's related to the ISP digital gain range */
} ot_isp_me_attr;

typedef struct {
    td_bool    bypass;      /* RW; Range:[0, 1]; Format:1.0; */
    ot_op_mode op_type;
    td_u8     ae_run_interval;   /* RW; Range:[0x1, 0xFF]; Format:8.0; set the AE run interval */
    td_bool   hist_stat_adjust;  /* RW; Range:[0, 1]; Format:1.0;
                                    TD_TRUE: 256 bin histogram statistic config will adjust when large red or blue
                                    area detected.
                                    TD_FALSE: 256 bin histogram statistic config will not change */
    td_bool   ae_route_ex_valid;   /* RW; Range:[0, 1]; Format:1.0; use extend AE route or not */
    ot_isp_me_attr   manual_attr;
    ot_isp_ae_attr   auto_attr;
    ot_isp_prior_frame prior_frame; /* RW; Range:[0, 2); Format:1.0; AE prior frame */
    td_bool ae_gain_sep_cfg; /* RW; Range:[0, 1]; Format:1.0; long and short frame gain separately configure or not */
    td_bool advance_ae; /* RW; Range:[0, 1]; Format:1.0;  open advance AE or not */
} ot_isp_exposure_attr;

#define OT_ISP_AE_ROUTE_MAX_NODES                    16
typedef struct {
    td_u32  int_time;         /* RW; Range:(0x0, 0xFFFFFFFF]; Format:32.0;
                                 sensor exposure time (unit: us), it's related to the specific sensor */
    td_u32  sys_gain;         /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                 system gain (unit: times, 10bit precision), it's related to the specific sensor and
                                 ISP Dgain range */
    ot_isp_iris_f_no iris_fno;  /* RW; Range:[0, 10]; Format:4.0;
                                   the F number of the iris's aperture, only support for Piris */
    td_u32  iris_fno_lin;       /* RW; Range:[0x1, 0x400]; Format:32.0;
                                   the equivalent gain of F number of the iris's aperture, only support for Piris */
} ot_isp_ae_route_node;

typedef struct {
    td_u32 total_num;     /* RW; Range:[0, 0x10]; Format:8.0; total node number of AE route */
    ot_isp_ae_route_node route_node[OT_ISP_AE_ROUTE_MAX_NODES];
} ot_isp_ae_route;

#define OT_ISP_AE_ROUTE_EX_MAX_NODES                 16
typedef struct {
    td_u32  int_time;     /* RW; Range:(0x0, 0xFFFFFFFF]; Format:32.0;
                             sensor exposure time (unit: us), it's related to the specific sensor */
    td_u32  a_gain;       /* RW; Range:[0x400, 0x3FFFFF]; Format:22.10;
                             sensor analog gain (unit: times, 10bit precision), it's related to the specific sensor */
    td_u32  d_gain;       /* RW; Range:[0x400, 0x3FFFFF]; Format:22.10;
                             sensor digital gain (unit: times, 10bit precision), it's related to the specific sensor */
    td_u32  isp_d_gain;   /* RW; Range:[0x400, 0x40000]; Format:22.10;
                             ISP digital gain (unit: times, 10bit precision) */
    ot_isp_iris_f_no  iris_fno;  /* RW; Range:[0, 10]; Format:4.0;
                                    the F number of the iris's aperture, only support for Piris */
    td_u32  iris_fno_lin;  /* RW; Range:[0x1, 0x400]; Format:32.0;
                              the equivalent gain of F number of the iris's aperture, only support for Piris */
} ot_isp_ae_route_ex_node;

typedef struct {
    td_u32 total_num;       /* RW; Range:[0, 0x10]; Format:8.0; total node number of extend AE route */
    ot_isp_ae_route_ex_node route_ex_node[OT_ISP_AE_ROUTE_EX_MAX_NODES];
} ot_isp_ae_route_ex;

typedef struct {
    td_u32  exp_time;                   /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    td_u32  short_exp_time;             /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    td_u32  median_exp_time;            /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    td_u32  long_exp_time;              /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    td_u32  a_gain;                     /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    td_u32  d_gain;                     /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    td_u32  a_gain_sf;                  /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    td_u32  d_gain_sf;                  /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    td_u32  isp_d_gain;                 /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    td_u32  exposure;                   /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    td_bool exposure_is_max;            /* R; Range:[0, 1]; Format:1.0; */
    td_s16  hist_error;                 /* R; Range:[-32768, 32767]; Format:16.0; */
    td_u32  ae_hist1024_value[OT_ISP_HIST_NUM]; /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0;
                                            1024 bins histogram for channel 1 */

    td_u8  ave_lum;                 /* R; Range:[0x0, 0xFF]; Format:8.0; */
    td_u32 lines_per500ms;          /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; exposure lines per 500ms */
    td_u32 piris_fno;               /* R; Range:[0x0, 0x400]; Format:32.0; */
    td_u32 fps;                     /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; actual fps */
    td_u32 iso;                     /* R; Range:[0x64, 0xFFFFFFFF]; Format:32.0; */
    td_u32 isosf;                   /* R; Range:[0x64, 0xFFFFFFFF]; Format:32.0; */
    td_u32 iso_calibrate;           /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    td_u32 ref_exp_ratio;           /* R; Range:[0x40, 0x4000]; Format:26.6; */
    td_u16 wdr_exp_coef;            /* R; Range:[0x0, 0x400]; Format:6.10; 0x400 means 1 times */
    td_u32 first_stable_time;       /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; AE first stable time for quick start */
    td_u32 quick_star_iso;          /* R; Range:[0x64, 0xFFFFFFFF]; Format:32.0; */
    ot_isp_ae_route ae_route;       /* R; Actual AE route */
    ot_isp_ae_route_ex ae_route_ex; /* R; Actual AE route_ex */
    ot_isp_ae_route ae_route_sf;    /* R; Actual AE route_sf */
    ot_isp_ae_route_ex ae_route_sf_ex; /* R; Actual AE route_sf_ex */
} ot_isp_exp_info;

typedef struct {
    td_u32 reg_addr;
    td_u32 reg_value;
} ot_isp_exp_param_reg;

typedef struct {
    td_u32 tar_fps;
    td_u32 tar_isp_dgain; /* 10 bit */
    ot_isp_exp_param_reg time_reg[10]; /* default 10 */
    ot_isp_exp_param_reg again_reg[10]; /* default 10 */
    ot_isp_exp_param_reg dgain_reg[10]; /*  default 10 */
} ot_isp_exp_conv_param;

typedef struct {
    td_bool enable;           /* RW; Range:[0, 1]; Format:1.0; */
    td_u8 face_tolerance;     /* RW; Range:[0, 0xFF];  Format:8.0   */
    td_u8 face_comp;          /* RW; Range:[0, 0xFF];  Format:8.0   */
    td_u8 stat_delay_num;    /* RW; Range:[0, 10];    Format:4.0   */
    td_u8 speed;              /* RW; Range:[0, 0x64];  Format:7.0   */
    td_u16 face_delay_num;    /* RW; Range:[0, 0x400]; Format:10.0  */
} ot_isp_fast_face_ae_attr;

#define OT_ISP_EXP_RATIO_NUM                             3
typedef struct {
    ot_op_mode     exp_ratio_type;   /* RW; Range:[0, 1]; Format:1.0;
                                        OT_OP_MODE_AUTO: The ExpRatio used in ISP is generated by firmware;
                                        OT_OP_MODE_MANUAL: The ExpRatio used in ISP is set by u32ExpRatio */
    td_u32 exp_ratio[OT_ISP_EXP_RATIO_NUM]; /* RW; Range:[0x40, 0x4000]; Format:26.6; 0x40 means 1 times.
                                        When enExpRatioType is OT_OP_MODE_AUTO, u32ExpRatio is invalid.
                                        When enExpRatioType is OT_OP_MODE_MANUAL, u32ExpRatio is quotient of
                                        long exposure time / short exposure time. */
    td_u32 exp_ratio_max;           /* RW; Range:[0x40, 0x4000]; Format:26.6; 0x40 means 1 times.
                                       When enExpRatioType is OT_OP_MODE_AUTO, u32ExpRatioMax is max(upper limit) of
                                       ExpRatio generated by firmware.
                                       When enExpRatioType is OT_OP_MODE_MANUAL, u32ExpRatioMax is invalid. */
    td_u32 exp_ratio_min;          /* RW; Range:[0x40, 0x4000]; Format:26.6; limited Range:[0x40, u32ExpRatioMax],
                                      0x40 means 1 times.
                                      When enExpRatioType is OT_OP_MODE_AUTO, u32ExpRatioMin is min(lower limit) of
                                      ExpRatio generated by firmware.
                                      When enExpRatioType is OT_OP_MODE_MANUAL, u32ExpRatioMin is invalid. */
    td_u16 tolerance;              /* RW; Range:[0x0, 0xFF]; Format:8.0;et the dynamic range tolerance.
                                      Format: unsigned 6.2-bit fixed-point. 0x4 means 1dB. */
    td_u16 speed;                  /* RW; Range:[0x0, 0xFF]; Format:8.0; exposure ratio adjust speed */
    td_u16 ratio_bias;             /* RW; Range:[0x0, 0xFFFF]; Format:16.0; exposure ratio bias */
    td_u16 high_light_target;      /* RW; Range:[0x0, 0x400]; Format:10.0; the high light target. */
    td_u16 exp_coef_min;       /* RW; Range:[0x0, 0x400]; Format:6.10; 0x400 means 1 times; min exposure coefficient */
} ot_isp_wdr_exposure_attr;

typedef struct {
    ot_op_mode  exp_hdr_lv_type;   /* RW; Range:[0, 1]; Format:1.0;
                                         OT_OP_MODE_AUTO: The ExpHDRLv used in ISP is generated by firmware;
                                         OT_OP_MODE_MANUAL: The ExpHDRLv used in ISP is set by u32ExpHDRLv */
    td_u32 exp_hdr_lv;             /* RW; Range:[0x40, 0x400]; Format:26.6; 0x40 means 1 times.
                                      When enExpHDRLvType is OT_OP_MODE_AUTO, u32ExpHDRLv is invalid.
                                      When enExpHDRLvType is OT_OP_MODE_MANUAL, u32ExpHDRLv is the quotient of
                                      exposure / linear exposure. */
    td_u32 exp_hdr_lv_max;         /* RW; Range:[0x40, 0x400]; Format:26.6; 0x40 means 1 times.
                                      When enExpHDRLvType is OT_OP_MODE_AUTO, u32ExpHDRLvMax is max(upper limit) of
                                      ExpHDRLv generated by firmware.
                                      When enExpHDRLvType is OT_OP_MODE_MANUAL, u32ExpHDRLvMax is invalid. */
    td_u32 exp_hdr_lv_min;         /* RW; Range:[0x40, 0x400]; Format:26.6; limited range:[0x40, u32ExpHDRLvMax],
                                      0x40 means 1 times.
                                      When enExpHDRLvType is OT_OP_MODE_AUTO, u32ExpHDRLvMin is min(lower limit) of
                                      ExpHDRLv generated by firmware.
                                      When enExpHDRLvType is OT_OP_MODE_MANUAL, u32ExpHDRLvMin is invalid. */
    td_u32 exp_hdr_lv_weight;      /* RW; Range:[0x0, 0x400]; Format:16.0; exposure HDR level weight. */
} ot_isp_hdr_exposure_attr;

typedef struct {
    td_bool enable;               /* RW; Range:[0, 1]; Format:1.0; smart ae enable or not */
    td_bool ir_mode;              /* RW; Range:[0, 1]; Format:1.0; smart ae IR mode or not */
    ot_op_mode   smart_exp_type;  /* RW; Range:[0, 1]; Format:1.0;
                                       OT_OP_MODE_AUTO: The ExpCoef used in ISP is  generated by firmware;
                                       OT_OP_MODE_MANUAL: The ExpCoef used in ISP is set by u32ExpCoef */
    td_u16  exp_coef;          /* RW; Range:[0x0, 0xFFFF]; Format:6.10; 0x400 means 1 times.
                                  When enExpHDRLvType is OT_OP_MODE_AUTO, u32ExpCoef is invalid.
                                  When enExpHDRLvType is OT_OP_MODE_MANUAL, u32ExpCoef is the quotient of exposure. */
    td_u8   luma_target;       /* RW; Range:[0x0, 0xFF]; Format:8.0; luma target of smart ae. */
    td_u16  exp_coef_max;      /* RW; Range:[0x0, 0xFFFF]; Format:6.10; 0x400 means 1 times.
                                  When enExpHDRLvType is OT_OP_MODE_AUTO, u32ExpCoefvMax is max(upper limit) of ExpCoef
                                  generated by firmware.
                                  When enExpHDRLvType is OT_OP_MODE_MANUAL, u32ExpCoefMax is invalid. */
    td_u16  exp_coef_min;      /* RW; Range:[0x0, 0xFFFF]; Format:6.10; 0x400 means 1 times.
                                  When enExpHDRLvType is OT_OP_MODE_AUTO, u32ExpCoefMax is min(lower limit) of ExpCoef
                                  generated by firmware.
                                  When enExpHDRLvType is OT_OP_MODE_MANUAL, u32ExpCoefMin is invalid. */
    td_u8   smart_interval;       /* RW; Range:[0x1, 0xFF]; Format:8.0; smart ae run interval */
    td_u8   smart_speed;          /* RW; Range:[0x0, 0xFF]; Format:8.0; smart ae adjust step */
    td_u16  smart_delay_num;      /* RW; Range:[0x0, 0x400]; Format:16.0; smart ae adjust delay frame num */
} ot_isp_smart_exposure_attr;

/* AWB structs */
/*
 * Defines the AWB online calibration type
 * u16AvgRgain: the avg value of Rgain after online calibration
 * u16AvgBgain: the avg value of Bgain after online calibration
 */
typedef struct {
    td_u16 avg_r_gain;
    td_u16 avg_b_gain;
} ot_isp_awb_calibration_gain;

/*
  Defines the AWB algorithm type
  0 = Improved gray world algorithm.
  1 = AWB algorithm that classifies the statistics and re-filters the white blocks
  2 = AWB algorithm that opt outdoor performance for dv
 */
typedef enum {
    OT_ISP_AWB_ALG_LOWCOST = 0,
    OT_ISP_AWB_ALG_ADVANCE = 1,
    OT_ISP_AWB_ALG_NATURA  = 2,
    OT_ISP_AWB_ALG_BUTT
} ot_isp_awb_alg_type;

/*
  Defines the AWB policy in the multi-illuminant scenario
  0 = Automatic saturation adjustment in the multi-illuminant scenario
  1 = Automatic ccm adjustment in the multi-illuminant scenario
 */
typedef enum {
    OT_ISP_AWB_MULTI_LS_SAT = 0,
    OT_ISP_AWB_MULTI_LS_CCM = 1,
    OT_ISP_AWB_MULTI_LS_BUTT
} ot_isp_awb_multi_ls_type;

typedef enum {
    OT_ISP_AWB_INDOOR_MODE = 0,
    OT_ISP_AWB_OUTDOOR_MODE = 1,
    OT_ISP_AWB_INDOOR_OUTDOOR_BUTT
} ot_isp_awb_indoor_outdoor_status;

/* Defines the AWB gain range */
typedef struct {
    td_bool enable;
    ot_op_mode   op_type;

    td_u16 high_rg_limit; /* RW; Range:[0x0, 0xFFF]; Format:4.8;
                             in Manual Mode, user define the Max Rgain of High Color Temperature */
    td_u16 high_bg_limit; /* RW; Range:[0x0, 0xFFF]; Format:4.8;
                             in Manual Mode, user define the Min Bgain of High Color Temperature */
    td_u16 low_rg_limit;  /* RW; Range:[0x0, 0xFFF]; Format:4.8; limited range:[0x0, u16HighRgLimit),
                             in Manual Mode, user define the Min Rgain of Low Color Temperature */
    td_u16 low_bg_limit;  /* RW; Range:[0, 0xFFF]; Format:4.8; limited Range:(u16HighBgLimit, 0xFFF],
                             in Manual Mode, user define the Max Bgain of Low Color Temperature */
} ot_isp_awb_ct_limit_attr;

typedef struct {
    td_bool enable;                 /* RW; Range:[0, 1]; Format:1.0; Outdoor/Indoor scenario determination enable */
    ot_op_mode   op_type;
    ot_isp_awb_indoor_outdoor_status outdoor_status; /* RW;User should config indoor or outdoor status in Manual Mode */
    td_u32 out_thresh;         /* RW;Format:32.0;shutter time(in us) to judge indoor or outdoor */
    td_u16 low_start;          /* RW;Format:16.0;5000K is recommend */
    td_u16 low_stop;           /* RW;Format:16.0;limited range:(0, u16LowStart),
                                  4500K is recommend, should be smaller than u8LowStart */
    td_u16 high_start;         /* RW;Format:16.0;limited range:(u16LowStart, 0xFFFF],
                                  6500K is recommend, should be larger than u8LowStart */
    td_u16 high_stop;          /* RW;Format:16.0;limited range:(u16HighStart, 0xFFFF],
                                  8000K is recommend, should be larger than u8HighStart */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART06_H_ */
