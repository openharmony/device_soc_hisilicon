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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART05_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART05_H_

    ot_isp_af_pre_filter_cfg pre_flt_cfg;   /* RW; pre filter cfg */
    td_u16                   high_luma_threshold;   /* RW; Range: [0,0xFF]; high luma threshold. */
} ot_isp_af_cfg;

/* Configures the AF level depend gain module.  */
typedef struct {
    td_bool     en;                       /* RW; Range: [0, 1]; FILTER level depend gain enable. */
    td_u16      threshold_low;            /* RW; range: [0x0, 0xFF]; FILTER level depend th low */
    td_u16      gain_low;                 /* RW; range: [0x0, 0xFF]; FILTER level depend gain low */
    td_u16      slope_low;                /* RW; range: [0x0, 0xF];  FILTER level depend slope low */
    td_u16      threshold_high;           /* RW; range: [0x0, 0xFF]; FILTER level depend th high */
    td_u16      gain_high;                /* RW; range: [0x0, 0xFF]; FILTER level depend gain high */
    td_u16      slope_high;               /* RW; range: [0x0, 0xF];  FILTER level depend slope high */
} ot_isp_af_level_depend;

/* Configures the AF coring module.  */
typedef struct {
    td_u16      threshold;                 /* RW; Range: [0x0, 0x7FF];FILTER coring threshold. */
    td_u16      slope;                     /* RW; Range: [0x0, 0xF];  FILTER Coring Slope */
    td_u16      limit;                     /* RW; Range: [0x0, 0x7FF];FILTER coring limit */
} ot_isp_af_coring;

#define OT_ISP_IIR_EN_NUM                                3
#define OT_ISP_IIR_GAIN_NUM                              7
#define OT_ISP_IIR_SHIFT_NUM                             4
/* Defines the IIR parameter configuration of horizontal filters for AF statistics.  */
typedef struct {
    td_bool          narrow_band_en;             /* RW; Range: [0, 1]; IIR narrow band enable. */
    td_bool          iir_en[OT_ISP_IIR_EN_NUM];  /* RW; Range: [0, 1]; IIR enable. */
    td_u8            iir_shift;                  /* RW; Range: [0, 63]; IIR Shift */
    td_s16           iir_gain[OT_ISP_IIR_GAIN_NUM];  /* RW; Range: [-511, 511]. IIR gain,gain0 range:[0,255]. */
    td_u16           iir_shift_lut[OT_ISP_IIR_SHIFT_NUM]; /* RW; Range: [0x0, 0x7];  IIR shift. */
    ot_isp_af_level_depend  level_depend;                 /* RW; filter level depend. */
    ot_isp_af_coring        coring;                       /* RW; filter coring. */
} ot_isp_af_h_param;

#define OT_ISP_FIR_GAIN_NUM                              5

typedef struct {
    td_s16                 fir_gain[OT_ISP_FIR_GAIN_NUM]; /* RW; Range: [-31, 31];   FIR gain. */
    ot_isp_af_level_depend level_depend;                  /* RW; filter level depend. */
    ot_isp_af_coring       coring;                        /* RW; filter coring.  */
} ot_isp_af_v_param;

#define OT_ISP_ACC_SHIFT_H_NUM                           2
#define OT_ISP_ACC_SHIFT_V_NUM                           2
typedef struct {
    td_u16 acc_shift_y;                          /* RW; Range: [0x0, 0xF]; luminance Y statistic shift. */
    td_u16 acc_shift_h[OT_ISP_ACC_SHIFT_H_NUM];  /* RW; Range: [0x0, 0xF]; IIR statistic shift. */
    td_u16 acc_shift_v[OT_ISP_ACC_SHIFT_V_NUM];  /* RW; Range: [0x0, 0xF]; FIR statistic shift. */
    td_u16 hl_cnt_shift;                         /* RW; Range: [0x0, 0xF]; High luminance counter shift */
} ot_isp_af_fv_param;

typedef struct {
    ot_isp_af_cfg        config;
    ot_isp_af_h_param    h_param_iir0;
    ot_isp_af_h_param    h_param_iir1;
    ot_isp_af_v_param    v_param_fir0;
    ot_isp_af_v_param    v_param_fir1;
    ot_isp_af_fv_param   fv_param;
} ot_isp_focus_stats_cfg;

/*
   the main purpose of stat key was to access individual statistic info separately...
   ...for achieving performance optimization of CPU, because of we acquire stat...
   ... in ISP_DRV ISR for our firmware reference and USER can also use alternative MPI...
   ... to do this job, so bit1AeStat1~bit1AfStat for MPI behavior control, and bit16IsrAccess...
   ... for ISR access control, they were independent. but they have the same bit order, for example...
   ... bit1AeStat1 for MPI AeStat1 access key, and bit16 of u32Key for ISR AeStat1 access key
 */
typedef union {
    td_u64  key;
    struct {
        td_u64  bit1_fe_ae_global_stat         : 1;   /* [0] */
        td_u64  bit1_fe_ae_local_stat          : 1;   /* [1] */
        td_u64  bit1_fe_ae_stitch_global_stat  : 1;   /* [2] */
        td_u64  bit1_fe_ae_stitch_local_stat   : 1;   /* [3] */
        td_u64  bit1_be_ae_global_stat         : 1;   /* [4] */
        td_u64  bit1_be_ae_local_stat          : 1;   /* [5] */
        td_u64  bit1_be_ae_stitch_global_stat  : 1;   /* [6] */
        td_u64  bit1_be_ae_stitch_local_stat   : 1;   /* [7] */
        td_u64  bit1_awb_stat1                 : 1;   /* [8] */
        td_u64  bit1_awb_stat2                 : 1;   /* [9] */
        td_u64  bit2_reserved0                 : 2;   /* [10:11] */
        td_u64  bit1_fe_af_stat                : 1;   /* [12] */
        td_u64  bit1_be_af_stat                : 1;   /* [13] */
        td_u64  bit2_reserved1                 : 2;   /* [14:15] */
        td_u64  bit1_dehaze                    : 1;   /* [16] */
        td_u64  bit1_mg_stat                   : 1;   /* [17] */
        td_u64  bit1_extend_stats              : 1;   /* [18] */
        td_u64  bit13_reserved                 : 13;   /* [19:31] */
        td_u64  bit32_isr_access               : 32;   /* [32:63] */
    };
} ot_isp_stats_ctrl;

/* statistics structs */
#define OT_ISP_HIST_NUM 1024

typedef struct {
    td_u16 grid_y_pos[OT_ISP_AE_ZONE_ROW + 1];   /* R */
    td_u16 grid_x_pos[OT_ISP_AE_ZONE_COLUMN + 1]; /* R */
    td_u8  status;                        /* R;0:not update, 1: update,others:reserved */
} ot_isp_ae_grid_info;

typedef struct {
    td_u16 grid_y_pos[OT_ISP_MG_ZONE_ROW + 1];    /* R */
    td_u16 grid_x_pos[OT_ISP_MG_ZONE_COLUMN + 1]; /* R */
    td_u8  status;                         /* R;0:not update, 1: update,others:reserved */
} ot_isp_mg_grid_info;

typedef struct {
    td_u16 grid_y_pos[OT_ISP_AWB_ZONE_ORIG_ROW  + 1];   /* R */
    td_u16 grid_x_pos[OT_ISP_AWB_ZONE_ORIG_COLUMN + 1]; /* R */
    td_u8  status;                               /* R;0:not update, 1: update,others:reserved */
} ot_isp_awb_grid_info;

typedef struct {
    td_u16 grid_y_pos[OT_ISP_AF_ZONE_ROW + 1];    /* R */
    td_u16 grid_x_pos[OT_ISP_AF_ZONE_COLUMN + 1]; /* R */
    td_u8  status;                         /* R;0:not update, 1: update,others:reserved */
} ot_isp_focus_grid_info;

typedef struct {
    td_u32 fe_hist1024_value[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_HIST_NUM];  /* R; Range: [0x0, 0xFFFFFFFF]; Format:32.0;
                                                             Global 1024 bins histogram of FE */
    td_u16 fe_global_avg[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_BAYER_CHN_NUM]; /* R; Range: [0x0, 0xFFFF]; Format:16.0;
                                                                 Global average value of FE */
    td_u16 fe_zone_avg[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_AE_ZONE_ROW][OT_ISP_AE_ZONE_COLUMN][OT_ISP_BAYER_CHN_NUM]; /* R;
                              Range: [0x0, 0xFFFF]; Format:16.0; Zone average value of FE */
    td_u32 be_estimate_hist1024_value[OT_ISP_HIST_NUM]; /* R; Range: [0x0, 0xFFFFFFFF]; Format:32.0;
                                                          Estimated Global 1024 bins histogram of BE */
    td_u32 be_hist1024_value[OT_ISP_HIST_NUM];         /* R; Range: [0x0, 0xFFFFFFFF]; Format:32.0;
                                                          Global 1024 bins histogram of BE */
    td_u16 be_global_avg[OT_ISP_BAYER_CHN_NUM];        /* R; Range: [0x0, 0xFFFF]; Format:16.0;
                                                          Global average value of BE */
    td_u16 be_zone_avg[OT_ISP_AE_ZONE_ROW][OT_ISP_AE_ZONE_COLUMN][OT_ISP_BAYER_CHN_NUM]; /* R; Range: [0x0, 0xFFFF];
                                                                               Format:16.0; Zone average value of BE */
    ot_isp_ae_grid_info fe_grid_info;
    ot_isp_ae_grid_info be_grid_info;
    td_u64 pts;
} ot_isp_ae_stats;

typedef struct {
    td_u32 fe_hist1024_value[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_HIST_NUM];   /* R; Range: [0x0, 0xFFFFFFFF]; Format:32.0;
                                                                 Global 1024 bins histogram of FE */
    td_u16 fe_global_avg[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_BAYER_CHN_NUM]; /* R; Range: [0x0, 0xFFFF]; Format:16.0;
                                                                 Global average value of FE. */
    td_u16 fe_zone_avg[OT_ISP_MAX_STITCH_NUM][OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_AE_ZONE_ROW][OT_ISP_AE_ZONE_COLUMN]\
    [OT_ISP_BAYER_CHN_NUM]; /* R; Range: [0x0, 0xFFFF]; Format:16.0; Zone average value of FE. */
    td_u32 be_hist1024_value[OT_ISP_HIST_NUM];          /* R; Range: [0x0, 0xFFFFFFFF]; Format:32.0;
                                                    Global 1024 bins histogram of BE. */
    td_u16 be_global_avg[OT_ISP_BAYER_CHN_NUM];  /* R; Range: [0x0, 0xFFFF]; Format:16.0; Global average value of BE */
    td_u16 be_zone_avg[OT_ISP_MAX_STITCH_NUM][OT_ISP_AE_ZONE_ROW][OT_ISP_AE_ZONE_COLUMN][OT_ISP_BAYER_CHN_NUM]; /* R;
                                                        Range: [0x0, 0xFFFF]; Format:16.0; Zone average value of BE */
    td_u64 pts;
} ot_isp_ae_stitch_stats;

typedef struct {
    td_u16 zone_avg[OT_ISP_MG_ZONE_ROW][OT_ISP_MG_ZONE_COLUMN][OT_ISP_BAYER_CHN_NUM]; /* R; Range: [0x0, 0xFF];
                                                                                      Format:8.0; Zone average value */
    ot_isp_mg_grid_info grid_info;
} ot_isp_mg_stats;

typedef enum {
    OT_ISP_AWB_AFTER_DG = 0,
    OT_ISP_AWB_AFTER_EXPANDER, /* Not Support */
    OT_ISP_AWB_AFTER_DRC,
    OT_ISP_AWB_SWITCH_BUTT
} ot_isp_awb_switch;

typedef enum {
    OT_ISP_AWB_GAIN_IN_ISP = 0,
    OT_ISP_AWB_GAIN_IN_SENSOR,
    OT_ISP_AWB_GAIN_IN_BUTT
} ot_isp_awb_gain_switch;

/* Crops the input image of the AWB module */
typedef struct {
    td_bool en;  /* RW; Range: [0,1];  Format:1.0;AWB crop enable */
    td_u16  x;    /* RW; Range: SS928V100 = [0, 8192 - 60];
                     Format:13.0;AWB image crop start x, limited range:[0, ImageWidth - u16ZoneCol * 60] */
    td_u16  y;    /* RW; Range: SS928V100 = [0, 8192 - 14];
                     Format:13.0;AWB image crop start y,limited range:[0, ImageHeight - u16ZoneRow * 14] */
    td_u16  width;    /* RW; Range: SS928V100 = [60, 8192]; Format:14.0;
                     AWB image crop width, limited range:[u16ZoneCol * 60, ImageWidth] */
    td_u16  height;    /* RW; Range: SS928V100 = [14, 8192]; Format:14.0;
                     AWB image crop height, limited range:[u16ZoneRow * 14, ImageHeight] */
} ot_isp_awb_crop;

/* Defines the AWB statistics configuration */
typedef struct {
    ot_isp_awb_switch awb_switch; /* RW; Range: [0x0, 0x2]; Position of AWB statistics in pipeline */
    td_u16 zone_row;           /* RW; Range: [0x1, 0x20]; Vertical Blocks,
                                  limited range:[1, min(32, ImageHeight /OT_ISP_AWB_MIN_HEIGHT)] */
    td_u16 zone_col;           /* RW; Range: [0x1, 0x20]; Horizontal Blocks,
                                  limited range:[BlkNum, min(32, Width /OT_ISP_AWB_MIN_WIDTH)] */
    td_u16 white_level;        /* RW; Range: [0x0, 0xFFFF];Upper limit of valid data for white region,
                                  for Bayer statistics, [0x0, 0x3FF] for RGB statistics */
    td_u16 black_level;        /* RW; Range: [0x0, 0xFFFF];limited range: [0x0, u16WhiteLevel],
                                  Lower limit of valid data for white region .
                                  for Bayer statistics, bitwidth is 12, for RGB statistics, bitwidth is 10 */
    td_u16 cb_max;             /* RW; Range: [0x0, 0xFFF];Maximum value of B/G for white region */
    td_u16 cb_min;             /* RW; Range: [0x0, 0xFFF];
                                  limited range: [0x0, u16CbMax]Minimum value of B/G for white region */
    td_u16 cr_max;             /* RW; Range: [0x0, 0xFFF];Maximum value of R/G for white region */
    td_u16 cr_min;             /* RW; Range: [0x0, 0xFFF];
                                  limited range: [0x0, u16CrMax],Minimum value of R/G for white region */
    ot_isp_awb_crop crop;
} ot_isp_wb_stats_cfg;

typedef struct {
    td_u16  zone_row;                            /* R; Range: [0x0, 0x20];
                                                    effective horizontal block number for AWB statistic stitch window */
    td_u16  zone_col;                            /* R; Range: [0x0, 0x80];
                                                    effective vetical block number for AWB statistic stitch window */
    td_u16  zone_avg_r[OT_ISP_AWB_ZONE_STITCH_MAX];     /* R; Range: [0x0, 0xFFFF];Zone Average R  for Stitich mode */
    td_u16  zone_avg_g[OT_ISP_AWB_ZONE_STITCH_MAX];     /* R; Range: [0x0, 0xFFFF];Zone Average G  for Stitich mode */
    td_u16  zone_avg_b[OT_ISP_AWB_ZONE_STITCH_MAX];     /* R; Range: [0x0, 0xFFFF];Zone Average B  for Stitich mode */
    td_u16  zone_count_all[OT_ISP_AWB_ZONE_STITCH_MAX]; /* R; Range: [0x0, 0xFFFF];
                                                    normalized number of Gray points  for Stitich mode */
    td_u64 pts;
} ot_isp_wb_stitch_stats;

typedef struct {
    td_u16 global_r;          /* R; Range: [0x0, 0xFFFF];Global WB output Average R */
    td_u16 global_g;          /* R; Range: [0x0, 0xFFFF];Global WB output Average G */
    td_u16 global_b;          /* R; Range: [0x0, 0xFFFF];Global WB output Average B */
    td_u16 count_all;         /* R; Range: [0x0, 0xFFFF];normalized number of Gray points */

    td_u16 zone_avg_r[OT_ISP_AWB_ZONE_NUM];            /* R; Range: [0x0, 0xFFFF];Zone Average R */
    td_u16 zone_avg_g[OT_ISP_AWB_ZONE_NUM];            /* R; Range: [0x0, 0xFFFF];Zone Average G */
    td_u16 zone_avg_b[OT_ISP_AWB_ZONE_NUM];            /* R; Range: [0x0, 0xFFFF];Zone Average B */
    td_u16 zone_count_all[OT_ISP_AWB_ZONE_NUM];        /* R; Range: [0x0, 0xFFFF];normalized number of Gray points */
    ot_isp_awb_grid_info grid_info;
    td_u64 pts;
} ot_isp_wb_stats;

typedef struct {
    td_u16  v1;            /* R; Range: [0x0, 0xFFFF];vertical fir block1 Statistics */
    td_u16  h1;            /* R; Range: [0x0, 0xFFFF];horizontal iir block1 Statistics */
    td_u16  v2;            /* R; Range: [0x0, 0xFFFF];vertical fir block2 Statistics */
    td_u16  h2;            /* R; Range: [0x0, 0xFFFF];horizontal iir block2 Statistics */
    td_u16  y;             /* R; Range: [0x0, 0xFFFF];Y Statistics */
    td_u16  hl_cnt;        /* R; Range: [0x0, 0xFFFF];HlCnt Statistics */
} ot_isp_focus_zone;

typedef struct {
    ot_isp_focus_zone zone_metrics[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_AF_ZONE_ROW][OT_ISP_AF_ZONE_COLUMN]; /* R;
                                                                                       The zoned measure of contrast */
} ot_isp_fe_focus_stats;

typedef struct {
    ot_isp_focus_zone zone_metrics[OT_ISP_AF_ZONE_ROW][OT_ISP_AF_ZONE_COLUMN]; /* R; The zoned measure of contrast */
} ot_isp_be_focus_stats;

typedef struct {
    ot_isp_fe_focus_stats  fe_af_stat;
    ot_isp_be_focus_stats  be_af_stat;
    ot_isp_focus_grid_info fe_af_grid_info;
    ot_isp_focus_grid_info be_af_grid_info;
    td_u64 pts;
} ot_isp_af_stats;

typedef struct {
    ot_isp_stats_ctrl      key;
    ot_isp_ae_stats_cfg    ae_cfg;
    ot_isp_wb_stats_cfg    wb_cfg;
    ot_isp_focus_stats_cfg focus_cfg;
} ot_isp_stats_cfg;

/* ISP debug information */
typedef struct {
    td_bool debug_en;     /* RW; 1:enable debug, 0:disable debug */
    td_u64  phys_addr;     /* RW; physics address of debug info */
    td_u32  depth;        /* RW; depth of debug info */
} ot_isp_debug_info;

typedef struct {
    td_u32  reserved;         /* H;need to add member */
} ot_isp_debug_attr;

typedef struct {
    td_u32  frame_num_begain;
    td_u16  black_level_actual[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_BAYER_CHN_NUM];    /* R; BLC actual value  */
    td_u16  black_level_original[OT_ISP_WDR_MAX_FRAME_NUM][OT_ISP_BAYER_CHN_NUM];  /* R; BLC logic value  */
    td_u32  frame_num_end;
} ot_isp_debug_status;

/*
0 = Communication between the sensor and the ISP over the I2C interface
1 = Communication between the sensor and the ISP over the SSP interface
*/
typedef enum {
    OT_ISP_SNS_I2C_TYPE = 0,
    OT_ISP_SNS_SSP_TYPE,

    OT_ISP_SNS_TYPE_BUTT,
} ot_isp_sns_type;

/* sensor communication bus */
typedef union {
    td_s8   i2c_dev;
    struct {
        td_s8  bit4_ssp_dev       : 4;
        td_s8  bit4_ssp_cs        : 4;
    } ssp_dev;
} ot_isp_sns_commbus;

typedef struct {
    td_bool update;        /* RW; Range: [0x0, 0x1]; Format:1.0;
                              TD_TRUE: The sensor registers are written,
                              TD_FALSE: The sensor registers are not written */
    td_u8   delay_frame_num; /* RW; Number of delayed frames for the sensor register */
    td_u8   interrupt_pos;       /* RW;Position where the configuration of the sensor register takes effect */
                                /* 0x0,very short frame start interrupt, 0x1:very short frame end interrupt,
                                   0x10,short frame start interrupt, 0x11:short frame end interrupt,
                                   0x20,middle frame start interrupt, 0x21:middle frame end interrupt,
                                   0x30,long frame start interrupt, 0x31:long frame end interrupt            */
    td_u8   dev_addr;      /* RW;Sensor device address */
    td_u32  reg_addr;      /* RW;Sensor register address */
    td_u32  addr_byte_num; /* RW;Bit width of the sensor register address */
    td_u32  data;          /* RW;Sensor register data */
    td_u32  data_byte_num; /* RW;Bit width of sensor register data */
} ot_isp_i2c_data;

typedef struct {
    td_bool update;             /* RW; Range: [0x0, 0x1]; Format:1.0;
                                   TD_TRUE: The sensor registers are written,
                                   TD_FALSE: The sensor registers are not written */
    td_u8   delay_frame_num;      /* RW; Number of delayed frames for the sensor register */
    td_u8   interrupt_pos;      /* RW;Position where the configuration of the sensor register takes effect
                                 0x0,very short frame start interrupt, 0x1:very short frame end interrupt
                                 0x10,short frame start interrupt, 0x11:short frame end interrupt
                                 0x20,middle frame start interrupt, 0x21:middle frame end interrupt
                                 0x30,long frame start interrupt, 0x31:long frame end interrupt            */

    td_u32  dev_addr;           /* RW;Sensor device address */
    td_u32  dev_addr_byte_num;  /* RW;Bit width of the sensor device address */
    td_u32  reg_addr;           /* RW;Sensor register address */
    td_u32  reg_addr_byte_num;  /* RW;Bit width of the sensor register address */
    td_u32  data;               /* RW;Sensor register data */
    td_u32  data_byte_num;      /* RW;Bit width of sensor register data */
} ot_isp_ssp_data;

typedef struct {
    ot_isp_sns_type sns_type;      /* RW; Range:[0, 1]; Format: 1.0;
                                      OT_ISP_SNS_I2C_TYPE: I2C interface type between the sensor and ISP,
                                      OT_ISP_SNS_SSP_TYPE: SPI interface type between the sensor and ISP */
    td_u32  reg_num;               /* RW;Number of registers required when exposure results are written to the sensor.
                                      The member value cannot be dynamically changed */
    td_u8   cfg2_valid_delay_max;  /* RW;Maximum number of delayed frames from the time when all sensor registers are
                                      configured to the time when configurations take effect, which is used to ensure
                                      the synchronization between sensor registers and ISP registers */
    struct {
        td_u32  exp_distance[OT_ISP_WDR_MAX_FRAME_NUM - 1]; /* RW; wdr sensor exposure distance. */
        td_u8   delay_frame_num;
    } distance_attr;

    ot_isp_sns_commbus  com_bus;
    union {
        ot_isp_i2c_data i2c_data[OT_ISP_MAX_SNS_REGS];  /* AUTO: ot_isp_sns_type:OT_ISP_SNS_I2C_TYPE */
        ot_isp_ssp_data ssp_data[OT_ISP_MAX_SNS_REGS];  /* AUTO: ot_isp_sns_type:OT_ISP_SNS_SSP_TYPE */
    };

    struct {
        td_bool update;
        td_u8   delay_frame_num;
        td_u32  slave_vs_time;      /* RW;time of vsync. Unit: inck clock cycle */
        td_u32  slave_bind_dev;
    } slv_sync;

    td_bool config;
} ot_isp_sns_regs_info;

typedef enum {
    OT_ISP_VD_FE_START   = 0,
    OT_ISP_VD_FE_END,
    OT_ISP_VD_BE_END,

    OT_ISP_VD_BUTT
} ot_isp_vd_type;

/* Defines the attributes of the virtual addresses for the registers of ISP submodules */
typedef struct {
    td_void *isp_ext_reg_addr;        /* R;Start virtual address for the ISP external virtual registers */
    td_u32  isp_ext_reg_size;         /* R;Size of the ISP external virtual registers */
    td_void *ae_ext_reg_addr;         /* R;Start virtual address for the AE library module */
    td_u32  ae_ext_reg_size;          /* R;Size of the AE library module */
    td_void *awb_ext_reg_addr;        /* R;Start virtual address for the AWB library module */
    td_u32  awb_ext_reg_size;         /* R;Size of the AWB library module */
} ot_isp_reg_attr;

/* AI structs */
/*
  Defines the ISP iris type
  0 = DC iris
  1 = P iris
 */
typedef enum {
    OT_ISP_IRIS_DC_TYPE = 0,
    OT_ISP_IRIS_P_TYPE,

    OT_ISP_IRIS_TYPE_BUTT,
} ot_isp_iris_type;

/* Defines the F value of the ISP iris */
typedef enum {
    OT_ISP_IRIS_F_NO_32_0 = 0,
    OT_ISP_IRIS_F_NO_22_0,
    OT_ISP_IRIS_F_NO_16_0,
    OT_ISP_IRIS_F_NO_11_0,
    OT_ISP_IRIS_F_NO_8_0,
    OT_ISP_IRIS_F_NO_5_6,
    OT_ISP_IRIS_F_NO_4_0,
    OT_ISP_IRIS_F_NO_2_8,
    OT_ISP_IRIS_F_NO_2_0,
    OT_ISP_IRIS_F_NO_1_4,
    OT_ISP_IRIS_F_NO_1_0,

    OT_ISP_IRIS_F_NO_BUTT,
} ot_isp_iris_f_no;

typedef struct {
    td_s32 kp;             /* RW; Range:[0, 100000]; Format:32.0; the proportional gain of PID algorithm,
                              default value is 7000 */
    td_s32 ki;             /* RW; Range:[0, 1000]; Format:32.0; the integral gain of PID algorithm,
                              default value is 100 */
    td_s32 kd;             /* RW; Range:[0, 100000]; Format:32.0; the derivative gain of PID algorithm,
                              default value is 3000 */
    td_u32 min_pwm_duty;   /* RW; Range:[0, 1000]; Format:32.0; which is the min pwm duty for dciris control */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART05_H_ */
