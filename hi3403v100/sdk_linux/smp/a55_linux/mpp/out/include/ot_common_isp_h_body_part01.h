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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART01_H_


#include "ot_type.h"
#include "ot_errno.h"
#include "ot_common.h"
#include "ot_isp_debug.h"
#include "ot_common_video.h"
#include "ot_isp_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* MACRO DEFINITION */
#define ot_isp_res_width_max(dev)       (isp_get_max_width(dev))
#define ot_isp_res_height_max(dev)      (isp_get_max_height(dev))
#define OT_ISP_WIDTH_MIN                120
#define OT_ISP_HEIGHT_MIN               88
#define OT_ISP_ALIGN_WIDTH              4
#define OT_ISP_ALIGN_HEIGHT             4
#define OT_ISP_FRAME_RATE_MAX           65535.0

#define OT_ISP_BE_BUF_NUM_MIN           2

#define OT_ISP_VREG_MAX_NUM             16

#define OT_ISP_AE_ZONE_ROW              15
#define OT_ISP_AE_ZONE_COLUMN           17
#define OT_ISP_AE_PLANE_NUM             4
#define OT_ISP_MG_ZONE_ROW              15
#define OT_ISP_MG_ZONE_COLUMN           17
#define OT_ISP_AWB_ZONE_NUM             (OT_ISP_AWB_ZONE_ORIG_ROW * OT_ISP_AWB_ZONE_ORIG_COLUMN)
#define OT_ISP_AWB_ZONE_STITCH_MAX      (OT_ISP_AWB_ZONE_NUM * OT_ISP_MAX_STITCH_NUM)
#define OT_ISP_AF_ZONE_ROW              15
#define OT_ISP_AF_ZONE_COLUMN           17
#define OT_ISP_MAX_AWB_LIB_NUM          (OT_ISP_AWB_LIB_NUM)
#define OT_ISP_MAX_AE_LIB_NUM           (OT_ISP_AE_LIB_NUM)

#define OT_ISP_AE_MIN_WIDTH             256
#define OT_ISP_AE_MIN_HEIGHT            120
#define OT_ISP_AWB_MIN_WIDTH            60
#define OT_ISP_AWB_MIN_HEIGHT           14
#define OT_ISP_AF_MIN_WIDTH             256
#define OT_ISP_AF_MIN_HEIGHT            120
#define OT_ISP_AF_ALIGN_WIDTH           8
#define OT_ISP_AF_ALIGN_HEIGHT          2
#define OT_ISP_AF_PLGS_NUM              2

#define OT_ISP_GAMMA_NODE_NUM           1025
#define OT_ISP_PREGAMMA_NODE_NUM        257
#define OT_ISP_AGAMMA_NODE_NUM          257
#define OT_ISP_SCALE_MIN                204
#define OT_ISP_SCALE_MAX                5120

#define OT_ISP_AUTO_ISO_NUM             16
#define OT_ISP_SHARPEN_LUMA_NUM         32
#define OT_ISP_SHARPEN_GAIN_NUM         32

#define OT_ISP_MAX_SNS_REGS             32

#define OT_ISP_RLSC_POINTS                129
#define OT_ISP_MLSC_X_HALF_GRID_NUM       ((OT_ISP_LSC_GRID_COL - 1) / 2)
#define OT_ISP_MLSC_Y_HALF_GRID_NUM       ((OT_ISP_LSC_GRID_ROW - 1) / 2)
#define OT_ISP_MLSC_GAIN_LUT_NUM          2
#define OT_ISP_ACS_LIGHT_NUM              32
#define OT_ISP_ACS_CHN_NUM                2

#define OT_ISP_WDR_MAX_FRAME_NUM        4

#define OT_ISP_CSC_DC_NUM               3
#define OT_ISP_CSC_COEF_NUM             9

#define OT_ISP_BAYER_CALIBTAION_MAX_NUM 50
#define OT_ISP_AI_MAX_STEP_FNO_NUM      1024

#define OT_ISP_BAYERNR_LUT_LENGTH       33
#define OT_ISP_PRO_MAX_FRAME_NUM        8
#define OT_ISP_RGB_CHN_NUM              3

#define OT_ISP_DEMOSAIC_LUT_LENGTH      17
#define OT_ISP_BAYER_CHN_NUM            4
#define OT_ISP_CHN_R                    0
#define OT_ISP_CHN_GR                   1
#define OT_ISP_CHN_GB                   2
#define OT_ISP_CHN_B                    3

#define OT_ISP_DRC_FLTX_NODE_NUM        3
#define OT_ISP_DRC_MLUT_NODE_NUM        28
#define OT_ISP_DRC_LMIX_NODE_NUM        33
#define OT_ISP_DRC_EXP_COMP_SAMPLE_NUM  8
#define OT_ISP_DRC_SHP_LOG_CONFIG_NUM   16
#define OT_ISP_DRC_RG_BG_CTR_NODE_NUM   12
#define OT_ISP_DRC_RAW_WGT_NODE_NUM     16

#define OT_ISP_DEHAZE_STAT_SIZE         18
#define OT_ISP_DEHAZE_LUT_SIZE          256
#define OT_ISP_WDR_RATIO_NUM            10

#define OT_ISP_LCAC_EXP_RATIO_NUM       16
#define OT_ISP_LCAC_DET_NUM             3
#define OT_ISP_ACAC_THR_NUM             2

#define OT_ISP_BSHP_LUMA_WGT_NUM        32
#define OT_ISP_BSHP_HALF_CURVE_NUM      32

#define OT_ISP_DO_NOT_NEED_SWITCH_IMAGEMODE (-2)

/*  DEFAULT VALUE OF GLOBAL REGISTERS DEFINED HERE */
/*  AE  */
#define OT_ISP_TOP_RGGB_START_R_GR_GB_B           0
#define OT_ISP_TOP_RGGB_START_GR_R_B_GB           1
#define OT_ISP_TOP_RGGB_START_GB_B_R_GR           2
#define OT_ISP_TOP_RGGB_START_B_GB_GR_R           3

/*  AWB */
#define OT_ISP_AWB_OFFSET_COMP_DEF                0
#define OT_ISP_CCM_COLORTONE_EN_DEFAULT           1
#define OT_ISP_CCM_COLORTONE_RGAIN_DEFAULT        256
#define OT_ISP_CCM_COLORTONE_BGAIN_DEFAULT        256
#define OT_ISP_CCM_COLORTONE_GGAIN_DEFAULT        256

#define OT_ISP_CCM_PROT_EN_DEFAULT                0
#define OT_ISP_CCM_RECOVER_EN_DEFAULT             1
#define OT_ISP_CCM_MATRIX_SIZE                    9
#define OT_ISP_CCM_MATRIX_NUM                     7

/* GENERAL STRUCTURES */
/*
 * ISP Error Code
 * 0x40 : ISP_NOT_INIT
 * 0x41 : ISP_MEM_NOT_INIT
 * 0x42 : ISP_ATTR_NOT_CFG
 * 0x43 : ISP_SNS_UNREGISTER
 * 0x44 : ISP_INVALID_ADDR
 * 0x45 : ISP_NOMEM
 * 0x46 : ISP_NO_INT
 */
typedef enum {
    OT_ERR_ISP_NOT_INIT                = 0x40, /* ISP not init */
    OT_ERR_ISP_MEM_NOT_INIT            = 0x41, /* ISP memory not init */
    OT_ERR_ISP_ATTR_NOT_CFG            = 0x42, /* ISP attribute not cfg */
    OT_ERR_ISP_SNS_UNREGISTER          = 0x43, /* ISP sensor unregister */
    OT_ERR_ISP_INVALID_ADDR            = 0x44, /* ISP invalid address */
    OT_ERR_ISP_NOMEM                   = 0x45, /* ISP nomem */
    OT_ERR_ISP_NO_INT                  = 0x46, /* ISP */
    OT_ERR_ISP_ALG_NOT_INIT            = 0x47, /* ISP alg not init */
} ot_isp_err_code;

#define OT_ERR_ISP_NULL_PTR         OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_NULL_PTR)
#define OT_ERR_ISP_ILLEGAL_PARAM    OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ILLEGAL_PARAM)
#define OT_ERR_ISP_NOT_SUPPORT      OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_SUPPORT)
#define OT_ERR_ISP_TIME_OUT         OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_TIMEOUT)

#define OT_ERR_ISP_NOT_INIT         OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ISP_NOT_INIT)
#define OT_ERR_ISP_MEM_NOT_INIT     OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ISP_MEM_NOT_INIT)
#define OT_ERR_ISP_ATTR_NOT_CFG     OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ISP_ATTR_NOT_CFG)
#define OT_ERR_ISP_SNS_UNREGISTER   OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ISP_SNS_UNREGISTER)
#define OT_ERR_ISP_INVALID_ADDR     OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ISP_INVALID_ADDR)
#define OT_ERR_ISP_NOMEM            OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ISP_NOMEM)
#define OT_ERR_ISP_NO_INT           OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ISP_NO_INT)
#define OT_ERR_ISP_ALG_NOT_INIT     OT_DEFINE_ERR(OT_ID_ISP, OT_ERR_LEVEL_ERROR, OT_ERR_ISP_ALG_NOT_INIT)
/* Defines the structure of ISP module parameters. */
typedef struct {
    td_u32 interrupt_bottom_half; /* RW;Range:[0, 1]; Format:32.0; Indicate ISP interrupt bottom half,
                                     No distinction vipipe */
    td_u32 quick_start;           /* RW;Range:[0, 1]; Format:32.0; Indicate ISP Quick Start No distinction vipipe.
                                     Not Support */
    td_bool long_frame_interrupt_en;  /* when wdr mode enable/disable long frame pipe interrupt.Not Support */
} ot_isp_mod_param;

/* Defines the structure of ISP control parameters. */

typedef enum {
    OT_ISP_UPDATE_OB_STATS_FE_FRAME_END   = 0, /* delay 1 frame */
    OT_ISP_UPDATE_OB_STATS_FE_FRAME_START = 1, /* delay 2 frame */
    OT_ISP_UPDATE_OB_STATS_BUTT,
} ot_isp_ob_stats_update_pos;

typedef enum {
    OT_ISP_ALG_RUN_NORM     = 0,
    OT_ISP_ALG_RUN_FE_ONLY  = 1,
    OT_ISP_ALG_RUN_BUTT,
} ot_isp_alg_run_select;

typedef enum {
    OT_ISP_RUN_WAKEUP_FE_START = 0,
    OT_ISP_RUN_WAKEUP_BE_END = 1,
    OT_ISP_RUN_WAKEUP_BUTT,
} ot_isp_run_wakeup_select;

typedef struct {
    td_u8   be_buf_num;      /* RW;Range:SS928V100 = [2, 20]; offline be cfg buffer number(Only used for offline mode).
                                Not support changed after isp init */
    td_u32  proc_param;      /* RW;Format:32.0; Indicate the update frequency of ISP_PROC information,
                                No distinction vipipe */
    td_u32  stat_interval;   /* RW;Range:(0, 0xffffffff];Format:32.0;
                                Indicate the time interval of ISP statistic information */
    td_u32  update_pos;      /* RW;Range:[0, 1]; Format:32.0; Indicate the location of the configuration register of
                                ISP interrupt */
    td_u32  interrupt_time_out; /* RW;Format:32.0; Indicate the time(unit:ms) of interrupt timeout */
    td_u32  pwm_num;      /* RW;Format:32.0; Indicate PWM number */
    td_u32  port_interrupt_delay;  /* RW;Format:32.0; Port intertupt delay value, unit:clk */
    td_bool ldci_tpr_flt_en; /* RW;Range:[0, 1]; Indicate Enable LDCI temporal filter. Not support changed after
                                isp init */
    ot_isp_ob_stats_update_pos ob_stats_update_pos; /* RW;Range:[0, 1];  Indicate the location of reading ob orea
                                                       statistics. Not support changed after isp init */
    ot_isp_alg_run_select alg_run_select; /* RW;Range:[0, 1];  Indicate the algorithm of isp register and run.
                                             Not support changed after isp init */
    ot_isp_run_wakeup_select isp_run_wakeup_select; /* RW;Range:[0, 1];  Indicate the isp software is drived by which
                                                       interrupt, FE_START or BE_END */
} ot_isp_ctrl_param;

/*
 * Defines the prior frame of ISP
 * 0 = long frame
 * 1 = short frame
 */
typedef enum {
    OT_ISP_LONG_FRAME      = 0,
    OT_ISP_SHORT_FRAME     = 1,
    OT_ISP_PRIOR_FRAME_BUTT
} ot_isp_prior_frame;

/*
 * Defines the ISP correction or detection status
 * 0 = initial status, no calibration
 * 1 = The static defect pixel calibration ends normally
 * 2 = The static defect pixel calibration ends due to timeout.
 */
typedef enum {
    OT_ISP_STATE_INIT     = 0,
    OT_ISP_STATE_SUCCESS  = 1,
    OT_ISP_STATE_TIMEOUT  = 2,
    OT_ISP_STATE_BUTT
} ot_isp_status;

typedef enum {
    OT_ISP_PIPE_DIFF_CALIBRATION_MODE = 0,
    OT_ISP_PIPE_DIFF_USER_MODE        = 1,
    OT_ISP_PIPE_DIFF_MODE_BUTT
} ot_isp_pipe_diff_mode;

typedef struct {
    td_s32 offset[OT_ISP_BAYER_CHN_NUM];   /* RW;Range:[-0x3FFF, 0x3FFF];Format:15.0;
                                              Black level differences between the offset value */
    td_u32 gain[OT_ISP_BAYER_CHN_NUM];     /* RW;Range:[0x80, 0x400];Format:4.8; The gain difference ratio */
    td_u16 color_matrix[OT_ISP_CCM_MATRIX_SIZE]; /* RW;Range:[0x0, 0xFFFF];Format:8.8;Color correction matrix
                                                    variance ratio */
} ot_isp_pipe_diff_param;

typedef struct {
    ot_isp_pipe_diff_mode  mode;
    ot_isp_pipe_diff_param param;
} ot_isp_pipe_diff_attr;

/* Defines the format of the input Bayer image */
typedef enum {
    OT_ISP_BAYER_RGGB    = 0,
    OT_ISP_BAYER_GRBG    = 1,
    OT_ISP_BAYER_GBRG    = 2,
    OT_ISP_BAYER_BGGR    = 3,
    OT_ISP_BAYER_BUTT
} ot_isp_bayer_format;

/* Defines the bitwidth of the input Bayer image, used for lsc online calibration */
typedef enum {
    OT_ISP_BAYER_RAW_BIT_8BIT    = 8,
    OT_ISP_BAYER_RAW_BIT_10BIT   = 10,
    OT_ISP_BAYER_RAW_BIT_12BIT   = 12,
    OT_ISP_BAYER_RAW_BIT_14BIT   = 14,
    OT_ISP_BAYER_RAW_BIT_16BIT   = 16,
    OT_ISP_BAYER_RAW_BIT_BUTT
} ot_isp_bayer_raw_bit;

/* MIPI attribute, contains the mipi crop attribute */
typedef struct {
    td_bool           mipi_crop_en;      /* RW;Range:[0, 0x1];Format:1.0; enable/disable mipi crop */
    ot_rect           mipi_crop_offset;  /* RW; start position of the mipi crop window, image width and height */
}ot_mipi_crop_attr;

/* ISP public attribute, contains the public image attribute */
typedef struct {
    ot_rect             wnd_rect;        /* RW; start position of the cropping window, image width, and image height */
    ot_size             sns_size;          /* RW; width and height of the image output from the sensor */
    td_float            frame_rate;        /* RW; Range: [0, 0xFFFF]; for frame rate */
    ot_isp_bayer_format bayer_format;      /* RW; Range:[0,3] ;Format:2.0;the format of the input bayer image */
    ot_wdr_mode         wdr_mode;          /* RW; WDR mode select */
    td_u8               sns_mode;          /* RW; Range: [0, 0xFF]; Format: 8.0; for special sensor mode switches */
    td_bool             sensor_flip_en;    /* RW;Range:[0, 0x1];Format:1.0; enable/disable sensor flip */
    td_bool             sensor_mirror_en;  /* RW;Range:[0, 0x1];Format:1.0; enable/disable sensor mirror */
    ot_mipi_crop_attr   mipi_crop_attr;    /* RW; mipi crop window attr, enable/disable mipi crop */
} ot_isp_pub_attr;

/* Slave mode sensor sync signal generate module */
typedef struct {
    union {
        struct {
            td_u32  bit16_reserved      :  16;
            td_u32  bit_h_inv           :  1;
            td_u32  bit_v_inv           :  1;
            td_u32  bit12_reserved      :  12;
            td_u32  bit_h_enable        :  1;
            td_u32  bit_v_enable        :  1;
        } bits;
        td_u32 bytes;
    } cfg;

    td_u32  vs_time;
    td_u32  hs_time;
    td_u32  vs_cyc;
    td_u32  hs_cyc;
    td_u32  hs_dly_cyc;
    td_u32  slave_mode_time;
} ot_isp_slave_sns_sync;

/* Defines the ISP stitch attribute. */
/*
 * Defines the ISP firmware status
 * 0 = Running status
 * 1 = Frozen status
 */
typedef enum {
    OT_ISP_FMW_STATE_RUN = 0,
    OT_ISP_FMW_STATE_FREEZE,
    OT_ISP_FMW_STATE_BUTT
} ot_isp_fmw_state;

/* Defines the WDR mode of the ISP */
typedef struct {
    ot_wdr_mode  wdr_mode;
} ot_isp_wdr_mode;

typedef union {
    td_u64  key;
    struct {
        td_u64  bit_bypass_isp_d_gain        : 1;   /* RW;[0] */
        td_u64  bit_bypass_anti_false_color  : 1;   /* RW;[1] */
        td_u64  bit_bypass_crosstalk_removal : 1;   /* RW;[2] */
        td_u64  bit_bypass_dpc            : 1;   /* RW;[3] */
        td_u64  bit_bypass_nr             : 1;   /* RW;[4] */
        td_u64  bit_bypass_dehaze         : 1;   /* RW;[5] */
        td_u64  bit_bypass_wb_gain        : 1;   /* RW;[6] */
        td_u64  bit_bypass_mesh_shading   : 1;   /* RW;[7] */
        td_u64  bit_bypass_drc            : 1;   /* RW;[8] */
        td_u64  bit_bypass_demosaic       : 1;   /* RW;[9] */
        td_u64  bit_bypass_color_matrix   : 1;   /* RW;[10] */
        td_u64  bit_bypass_gamma          : 1;   /* RW;[11] */
        td_u64  bit_bypass_fswdr          : 1;   /* RW;[12] */
        td_u64  bit_bypass_ca             : 1;   /* RW;[13] */
        td_u64  bit_bypass_csc            : 1;   /* RW;[14] */
        td_u64  bit_bypass_radial_crop    : 1;   /* RW;[15] */
        td_u64  bit_bypass_sharpen        : 1;   /* RW;[16] */
        td_u64  bit_bypass_local_cac      : 1;   /* RW;[17] */
        td_u64  bit_bypass_acac           : 1;   /* RW;[18]; */
        td_u64  bit2_chn_select           : 2;   /* RW;[19:20] */
        td_u64  bit_bypass_ldci           : 1;   /* RW;[21] */
        td_u64  bit_bypass_pregamma       : 1;   /* RW;[22] */
        td_u64  bit_bypass_ae_stat_fe     : 1;   /* RW;[23] */
        td_u64  bit_bypass_ae_stat_be     : 1;   /* RW;[24] */
        td_u64  bit_bypass_mg_stat        : 1;   /* RW;[25] */
        td_u64  bit_bypass_af_stat_fe     : 1;   /* RW;[26] */
        td_u64  bit_bypass_af_stat_be     : 1;   /* RW;[27] */
        td_u64  bit_bypass_awb_stat       : 1;   /* RW;[28] */
        td_u64  bit_bypass_clut           : 1;   /* RW;[29] */
        td_u64  bit_bypass_rgbir          : 1;   /* RW;[30]  */
        td_u64  bit_bypass_agamma         : 1;   /* RW;[31]  */
        td_u64  bit_bypass_adgamma        : 1;   /* RW;[32]  */
        td_u64  bit_bypass_crb            : 1;   /* RW [33]  */
        td_u64  bit_reserved30            : 30;  /* H; [34:63] */
    };
} ot_isp_module_ctrl;

/*
 * Defines the RAW stream Position from the ISP
 * 0 = the data is writing out after all isp module.
 * 1 = the raw data is writing out after the WDR  module.
 */
typedef enum {
    OT_ISP_DUMP_FRAME_POS_NORMAL    = 0,
    OT_ISP_DUMP_FRAME_POS_AFTER_WDR = 1,
    OT_ISP_DUMP_FRAME_POS_BUTT
} ot_isp_dump_frame_pos;

/*
 * Defines the dump frame Position from the ISP.
 */
typedef struct {
    ot_isp_dump_frame_pos frame_pos;
} ot_isp_be_frame_attr;

/*
 * Defines the ISP WDR merge mode.
 * 0 = WDR mode is used
 * 1 = Fusion mode is used
 */
typedef enum {
    OT_ISP_MERGE_WDR_MODE      = 0,
    OT_ISP_MERGE_FUSION_MODE   = 1,
    OT_ISP_MERGE_BUTT
} ot_isp_wdr_merge_mode;

typedef enum {
    OT_ISP_BNR_EX = 0,
    OT_ISP_BNR_NORM = 1,
} ot_isp_bnr_sfm0_mode;

typedef enum {
    OT_ISP_WDR_WBGAIN_IN_DG1         = 0,
    OT_ISP_WDR_WBGAIN_IN_WB          = 1,
    OT_ISP_WDR_WBGAIN_POS_BUTT
} ot_isp_wdr_wb_gain_pos;

typedef struct {
    td_u8 md_thr_low_gain;
    td_u8 md_thr_hig_gain;
} ot_isp_fswdr_manual_attr;

typedef struct {
    td_u8 md_thr_low_gain[OT_ISP_WDR_RATIO_NUM][OT_ISP_AUTO_ISO_NUM];
    td_u8 md_thr_hig_gain[OT_ISP_WDR_RATIO_NUM][OT_ISP_AUTO_ISO_NUM];
} ot_isp_fswdr_auto_attr;

typedef struct {
    td_bool short_expo_chk;     /* RW;Range:[0x0,0x1];Format:1.0; */
    td_u16  short_check_threshold;   /* RW;Range:[0x0,0xFFF];Format:12.0 */
    td_bool md_ref_flicker;
    td_u8   mdt_still_threshold;     /* RW;Range:[0x0,0xFE];Format:8.0 */
    td_u8   mdt_full_threshold;      /* RW;Range:[0x0,0xFE];Format:8.0 */
    td_u8   mdt_long_blend;    /* RW;Range:[0x0,0xFE] */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_ISP_H_BODY_PART01_H_ */
