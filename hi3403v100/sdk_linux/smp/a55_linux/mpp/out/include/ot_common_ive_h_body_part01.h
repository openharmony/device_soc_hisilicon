/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART01_H_


#include "ot_common_svp.h"
#include "ot_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OT_IVE_HIST_NUM                     256
#define OT_IVE_MAP_NUM                      256
#define OT_IVE_MAX_RGN_NUM                  254
#define OT_IVE_ST_MAX_CORNER_NUM            500
#define OT_IVE_MASK_NUM                     25
#define OT_IVE_ARR_RESERVED_NUM_TWO         2
#define OT_IVE_ARR_RESERVED_NUM_THREE       3
#define OT_IVE_ARR_RESERVED_NUM_EIGHT       8
#define OT_IVE_ARR_RESERVED_NUM_TWELVE      12
#define OT_IVE_ARR_RESERVED_NUM_FOURTEEN    14
#define OT_IVE_ARR_NUM_THREE                3
#define OT_IVE_ARR_NUM_EIGHT                8

typedef enum {
    OT_IVE_ERR_SYS_TIMEOUT      = 0x40,             /* IVE process timeout */
    OT_IVE_ERR_QUERY_TIMEOUT    = 0x41,             /* IVE query timeout */
    OT_IVE_ERR_BUS_ERR          = 0x42,             /* IVE BUS error */
    OT_IVE_ERR_OPEN_FILE        = 0x43,             /* IVE open file error */
    OT_IVE_ERR_READ_FILE        = 0x44,             /* IVE read file error */

    OT_IVE_ERR_BUTT
} ot_ive_err_code;

/************************************************IVE error code ***********************************/
/* Invalid device ID */
#define OT_ERR_IVE_INVALID_DEV_ID OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_INVALID_DEV_ID)
/* Invalid channel ID */
#define OT_ERR_IVE_INVALID_CHN_ID OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_INVALID_CHN_ID)
/* At least one parameter is illegal. For example, an illegal enumeration value exists. */
#define OT_ERR_IVE_ILLEGAL_PARAM OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_ILLEGAL_PARAM)
/* The channel exists. */
#define OT_ERR_IVE_EXIST OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_EXIST)
/* The UN exists. */
#define OT_ERR_IVE_UNEXIST OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_UNEXIST)
/* A null point is used. */
#define OT_ERR_IVE_NULL_PTR OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_NULL_PTR)
/* Try to enable or initialize the system, device, or channel before configuring attributes. */
#define OT_ERR_IVE_NOT_CFG OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_CFG)
/* The operation is not supported currently. */
#define OT_ERR_IVE_NOT_SURPPORT OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_SUPPORT)
/* The operation, changing static attributes for example, is not permitted. */
#define OT_ERR_IVE_NOT_PERM OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_PERM)
/* A failure caused by the malloc memory occurs. */
#define OT_ERR_IVE_NO_MEM OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_NO_MEM)
/* A failure caused by the malloc buffer occurs. */
#define OT_ERR_IVE_NO_BUF OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_NO_BUF)
/* The buffer is empty. */
#define OT_ERR_IVE_BUF_EMPTY OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_BUF_EMPTY)
/* No buffer is provided for storing new data. */
#define OT_ERR_IVE_BUF_FULL OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_BUF_FULL)
/* The system is not ready because it may be not initialized or loaded.
 * The error code is returned when a device file fails to be opened. */
#define OT_ERR_IVE_NOT_READY OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_READY)
/* The source address or target address is incorrect during the operations such as calling
copy_from_user or copy_to_user. */
#define OT_ERR_IVE_BAD_ADDR OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_BAD_ADDR)
/* The resource is busy during the operations such as destroying a VENC channel
without deregistering it. */
#define OT_ERR_IVE_BUSY OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_ERR_BUSY)
/* IVE process timeout: 0xA01D0000 + 0x8000 + 0x40 */
#define OT_ERR_IVE_SYS_TIMEOUT OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_IVE_ERR_SYS_TIMEOUT)
/* IVE query timeout: 0xA01D0000 + 0x8000 + 0x41 */
#define OT_ERR_IVE_QUERY_TIMEOUT OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_IVE_ERR_QUERY_TIMEOUT)
/* IVE Bus error: 0xA01D0000 + 0x8000 + 0x42 */
#define OT_ERR_IVE_BUS_ERR OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_IVE_ERR_BUS_ERR)
/* IVE open file error: 0xA01D0000 + 0x8000 + 0x43 */
#define OT_ERR_IVE_OPEN_FILE OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_IVE_ERR_OPEN_FILE)
/* IVE read file error: 0xA01D0000 + 0x8000 + 0x44 */
#define OT_ERR_IVE_READ_FILE OT_DEFINE_ERR(OT_ID_IVE, OT_ERR_LEVEL_ERROR, OT_IVE_ERR_READ_FILE)

/*
 * DMA mode
 */
typedef enum {
    OT_IVE_DMA_MODE_DIRECT_COPY = 0x0,
    OT_IVE_DMA_MODE_INTERVAL_COPY = 0x1,
    OT_IVE_DMA_MODE_SET_3BYTE = 0x2,
    OT_IVE_DMA_MODE_SET_8BYTE = 0x3,
    OT_IVE_DMA_MODE_BUTT
} ot_ive_dma_mode;

typedef struct {
    ot_ive_dma_mode mode;
    td_u64 val;             /* Used in memory-set mode */
    td_u8  hor_seg_size;    /* Used in interval-copy mode, every row was segmented by hor_seg_size bytes,
                            restricted in values of 2,3,4,8,16 */
    td_u8 elem_size;        /* Used in interval-copy mode, the valid bytes copied in front of every segment
                            in a valid row, which 0<elem_size<hor_seg_size */
    td_u8 ver_seg_rows;     /* Used in interval-copy mode, copy one row in every ver_seg_rows */
} ot_ive_dma_ctrl;

typedef struct {
    td_s8 mask[OT_IVE_MASK_NUM];     /* Template parameter filter coefficient */
    td_u8 norm;         /* Normalization parameter, by right shift */
} ot_ive_filter_ctrl;

/*
 * CSC working mode.
 */
typedef enum {
    OT_IVE_CSC_MODE_VIDEO_BT601_YUV_TO_RGB = 0x0,  /* CSC: YUV2RGB, video transfer mode, RGB value range [16, 235] */
    OT_IVE_CSC_MODE_VIDEO_BT709_YUV_TO_RGB = 0x1,  /* CSC: YUV2RGB, video transfer mode, RGB value range [16, 235] */
    OT_IVE_CSC_MODE_PIC_BT601_YUV_TO_RGB = 0x2,    /* CSC: YUV2RGB, picture transfer mode, RGB value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT709_YUV_TO_RGB = 0x3,    /* CSC: YUV2RGB, picture transfer mode, RGB value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT601_YUV_TO_HSV = 0x4,    /* CSC: YUV2HSV, picture transfer mode, HSV value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT709_YUV_TO_HSV = 0x5,    /* CSC: YUV2HSV, picture transfer mode, HSV value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT601_YUV_TO_LAB = 0x6,    /* CSC: YUV2LAB, picture transfer mode, Lab value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT709_YUV_TO_LAB = 0x7,    /* CSC: YUV2LAB, picture transfer mode, Lab value range [0, 255] */
    OT_IVE_CSC_MODE_VIDEO_BT601_RGB_TO_YUV = 0x8,  /* CSC: RGB2YUV, video transfer mode, YUV value range [0, 255] */
    OT_IVE_CSC_MODE_VIDEO_BT709_RGB_TO_YUV = 0x9,  /* CSC: RGB2YUV, video transfer mode, YUV value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT601_RGB_TO_YUV = 0xa,    /* CSC: RGB2YUV, picture transfer mode, Y:[16, 235],U\V:[16, 240] */
    OT_IVE_CSC_MODE_PIC_BT709_RGB_TO_YUV = 0xb,    /* CSC: RGB2YUV, picture transfer mode, Y:[16, 235],U\V:[16, 240] */

    OT_IVE_CSC_MODE_BUTT
} ot_ive_csc_mode;

typedef struct {
    ot_ive_csc_mode mode; /* Working mode */
} ot_ive_csc_ctrl;

typedef struct {
    ot_ive_csc_mode mode;   /* CSC working mode */
    td_s8 mask[OT_IVE_MASK_NUM];         /* Template parameter filter coefficient */
    td_u8 norm;             /* Normalization parameter, by right shift */
} ot_ive_filter_and_csc_ctrl;

/*
 * sobel output ctrl
 */
typedef enum {
    OT_IVE_SOBEL_OUT_CTRL_BOTH = 0x0, /* Output horizontal and vertical */
    OT_IVE_SOBEL_OUT_CTRL_HOR = 0x1,  /* Output horizontal */
    OT_IVE_SOBEL_OUT_CTRL_VER = 0x2,  /* Output vertical */
    OT_IVE_SOBEL_OUT_CTRL_BUTT
} ot_ive_sobel_out_ctrl;


typedef struct {
    ot_ive_sobel_out_ctrl out_ctrl;                 /* Output format */
    td_s8 mask[OT_IVE_MASK_NUM];                    /* Template parameter */
} ot_ive_sobel_ctrl;

/*
 * Type of the magnitude and angle output results
 */
typedef enum {
    OT_IVE_MAG_AND_ANG_OUT_CTRL_MAG = 0x0,         /* Only the magnitude is output. */
    OT_IVE_MAG_AND_ANG_OUT_CTRL_MAG_AND_ANG = 0x1, /* The magnitude and angle are output. */
    OT_IVE_MAG_AND_ANG_OUT_CTRL_BUTT
} ot_ive_mag_and_ang_out_ctrl;

typedef struct {
    ot_ive_mag_and_ang_out_ctrl out_ctrl;
    td_u16 threshold;
    td_s8 mask[OT_IVE_MASK_NUM]; /* Template parameter. */
} ot_ive_mag_and_ang_ctrl;

typedef struct {
    td_u8 mask[OT_IVE_MASK_NUM]; /* The template parameter value must be 0 or 255. */
} ot_ive_dilate_ctrl;

typedef struct {
    td_u8 mask[OT_IVE_MASK_NUM]; /* The template parameter value must be 0 or 255. */
} ot_ive_erode_ctrl;

/*
 * Type of the threshold mode.
 */
typedef enum {
    OT_IVE_THRESHOLD_MODE_BINARY = 0x0,
    OT_IVE_THRESHOLD_MODE_TRUNC = 0x1,
    OT_IVE_THRESHOLD_MODE_TO_MIN_VAL = 0x2,
    OT_IVE_THRESHOLD_MODE_MIN_MID_MAX = 0x3,
    OT_IVE_THRESHOLD_MODE_ORIG_MID_MAX = 0x4,
    OT_IVE_THRESHOLD_MODE_MIN_MID_ORI = 0x5,
    OT_IVE_THRESHOLD_MODE_MIN_ORIG_MAX = 0x6,
    OT_IVE_THRESHOLD_MODE_ORI_MID_ORIG = 0x7,

    OT_IVE_THRESHOLD_MODE_BUTT
} ot_ive_threshold_mode;

typedef struct {
    ot_ive_threshold_mode mode;
    td_u8 low_threshold;  /* user-defined threshold,  0<=low_threshold<=255 */
    td_u8 high_threshold; /* user-defined threshold, if mode<OT_IVE_THRESHOLD_MODE_MIN_MID_MAX, high_thresh is not used,
                      else 0<=low_threshold<=high_threshold<=255; */
    td_u8 min_val;  /* Minimum value when tri-level thresholding */
    td_u8 mid_val;  /* Middle value when tri-level thresholding, if mode<2, mid_val is not used; */
    td_u8 max_val;  /* Maxmum value when tri-level thresholding */
} ot_ive_threshold_ctrl;

/*
 * Type of the sub output results
 */
typedef enum {
    OT_IVE_SUB_MODE_ABS = 0x0,      /* Absolute value of the difference */
    OT_IVE_SUB_MODE_SHIFT = 0x1,    /* The output result is obtained by shifting the result one digit right
                                    to reserve the signed bit. */
    OT_IVE_SUB_MODE_BUTT
} ot_ive_sub_mode;

typedef struct {
    ot_ive_sub_mode mode;
} ot_ive_sub_ctrl;

/*
 * Type of the integ output results
 */
typedef enum {
    OT_IVE_INTEG_OUT_CTRL_COMBINE = 0x0,
    OT_IVE_INTEG_OUT_CTRL_SUM = 0x1,
    OT_IVE_INTEG_OUT_CTRL_SQRT_SUM = 0x2,
    OT_IVE_INTEG_OUT_CTRL_BUTT
} ot_ive_integ_out_ctrl;

typedef struct {
    ot_ive_integ_out_ctrl out_ctrl;
} ot_ive_integ_ctrl;

/*
 * Type of the thresh_s16 mode
 */
typedef enum {
    OT_IVE_THRESHOLD_S16_MODE_S16_TO_S8_MIN_MID_MAX = 0x0,
    OT_IVE_THRESHOLD_S16_MODE_S16_TO_S8_MIN_ORIG_MAX = 0x1,
    OT_IVE_THRESHOLD_S16_MODE_S16_TO_U8_MIN_MID_MAX = 0x2,
    OT_IVE_THRESHOLD_S16_MODE_S16_TO_U8_MIN_ORIG_MAX = 0x3,

    OT_IVE_THRESHOLD_S16_MODE_BUTT
} ot_ive_threshold_s16_mode;

typedef struct {
    ot_ive_threshold_s16_mode mode;
    td_s16 low_threshold;           /* User-defined threshold */
    td_s16 high_threshold;          /* User-defined threshold */
    ot_svp_8bit min_val;            /* Minimum value when tri-level thresholding */
    ot_svp_8bit mid_val;            /* Middle value when tri-level thresholding */
    ot_svp_8bit max_val;            /* Maxmum value when tri-level thresholding */
} ot_ive_threshold_s16_ctrl;

/*
 * Type of the thresh_u16 mode
 */
typedef enum {
    OT_IVE_THRESHOLD_U16_MODE_U16_TO_U8_MIN_MID_MAX = 0x0,
    OT_IVE_THRESHOLD_U16_MODE_U16_TO_U8_MIN_ORIG_MAX = 0x1,

    OT_IVE_THRESHOLD_U16_MODE_BUTT
} ot_ive_threshold_u16_mode;

typedef struct {
    ot_ive_threshold_u16_mode mode;
    td_u16 low_threshold;
    td_u16 high_threshold;
    td_u8 min_val;
    td_u8 mid_val;
    td_u8 max_val;
} ot_ive_threshold_u16_ctrl;

/*
 * Mode of 16bit_to_8bit
 */
typedef enum {
    OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_S8 = 0x0,
    OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_ABS = 0x1,
    OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_BIAS = 0x2,
    OT_IVE_16BIT_TO_8BIT_MODE_U16_TO_U8 = 0x3,

    OT_IVE_16BIT_TO_8BIT_MODE_BUTT
} ot_ive_16bit_to_8bit_mode;

typedef struct {
    ot_ive_16bit_to_8bit_mode mode;
    td_u16 denominator;
    td_u8 num;
    td_s8 bias;
} ot_ive_16bit_to_8bit_ctrl;

/*
 * Type of the ord_stat_filter
 */
typedef enum {
    OT_IVE_ORDER_STATS_FILTER_MODE_MEDIAN = 0x0,
    OT_IVE_ORDER_STATS_FILTER_MODE_MAX = 0x1,
    OT_IVE_ORDER_STATS_FILTER_MODE_MIN = 0x2,

    OT_IVE_ORD_STAT_FILTER_MODE_BUTT
} ot_ive_order_stats_filter_mode;

typedef struct {
    ot_ive_order_stats_filter_mode mode;
} ot_ive_order_stats_filter_ctrl;

/*
 * Type of the map
 */
typedef enum {
    OT_IVE_MAP_MODE_U8 = 0x0,
    OT_IVE_MAP_MODE_S16 = 0x1,
    OT_IVE_MAP_MODE_U16 = 0x2,

    OT_IVE_MAP_MODE_BUTT
} ot_ive_map_mode;

typedef struct {
    ot_ive_map_mode mode;
} ot_ive_map_ctrl;

typedef struct {
    td_u8 map[OT_IVE_MAP_NUM];
} ot_ive_map_u8bit_lut_mem;

typedef struct {
    td_u16 map[OT_IVE_MAP_NUM];
} ot_ive_map_u16bit_lut_mem;

typedef struct {
    td_s16 map[OT_IVE_MAP_NUM];
} ot_ive_map_s16bit_lut_mem;

typedef struct {
    td_u32 hist[OT_IVE_HIST_NUM];
    td_u8 map[OT_IVE_MAP_NUM];
} ot_ive_equalize_hist_ctrl_mem;

typedef struct {
    ot_svp_mem_info mem;
} ot_ive_equalize_hist_ctrl;

typedef struct {
    td_u0q16 x; /* x of "xA+yB" */
    td_u0q16 y; /* y of "xA+yB" */
} ot_ive_add_ctrl;

typedef struct {
    td_u64 num;
    td_u64 quad_sum1;
    td_u64 quad_sum2;
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_EIGHT];
} ot_ive_ncc_dst_mem;

typedef struct {
    td_u32 area;   /* Represented by the pixel number */
    td_u16 left;   /* Circumscribed rectangle left border */
    td_u16 right;  /* Circumscribed rectangle right border */
    td_u16 top;    /* Circumscribed rectangle top border */
    td_u16 bottom; /* Circumscribed rectangle bottom border */
} ot_ive_rgn;

typedef union {
    struct {
        td_u32 cur_area_threshold : 23;  /* Threshold of the result regions' area */
        td_u32 label_status : 1;         /* 1: Labeled failed ; 0: Labeled successfully */
        td_u32 rgn_num : 8;              /* Number of valid region, non-continuous stored */
    } bits;
    td_u32 u32;
} ot_ive_ccblob_info;

typedef struct {
    ot_ive_ccblob_info info;
    ot_ive_rgn rgn[OT_IVE_MAX_RGN_NUM];  /* Valid regions with 'cur_area_threshold>0' and 'label = array_index+1' */
} ot_ive_ccblob;

/*
 * Type of the ccl
 */
typedef enum {
    OT_IVE_CCL_MODE_4C = 0x0, /* 4-connected */
    OT_IVE_CCL_MODE_8C = 0x1, /* 8-connected */

    OT_IVE_CCL_MODE_BUTT
} ot_ive_ccl_mode;

typedef struct {
    ot_ive_ccl_mode mode;           /* Mode */
    td_u16 init_area_threshold;     /* Init threshold of region area */
    td_u16 step;                    /* Increase area step for once */
} ot_ive_ccl_ctrl;

/*
 * GMM control struct
 */
typedef struct {
    td_u22q10 noise_var;        /* Initial noise Variance */
    td_u22q10 max_var;          /* Max  Variance */
    td_u22q10 min_var;          /* Min  Variance */
    td_u0q16 learn_rate;        /* Learning rate */
    td_u0q16 bg_ratio;          /* Background ratio */
    td_u8q8 var_threshold;      /* Variance Threshold */
    td_u0q16 init_wgt;          /* Initial Weight */
    td_u8 model_num;            /* Model number: 3 or 5 */
} ot_ive_gmm_ctrl;

/*
 * Type of the GMM2 sensitivity factor mode
 */
typedef enum {
    OT_IVE_GMM2_SNS_FACTOR_MODE_GLOBAL = 0x0,   /* Global sensitivity factor mode */
    OT_IVE_GMM2_SNS_FACTOR_MODE_PIXEL = 0x1,    /* Pixel sensitivity factor mode */

    OT_IVE_GMM2_SNS_FACTOR_MODE_BUTT
} ot_ive_gmm2_sns_factor_mode;

/*
 * Type of the GMM2 life update factor mode
 */
typedef enum {
    OT_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_GLOBAL  = 0x0,      /* Global life update factor mode */
    OT_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_PIXEL     = 0x1,    /* Pixel life update factor mode */

    OT_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_BUTT
} ot_ive_gmm2_life_update_factor_mode;

typedef struct {
    ot_ive_gmm2_sns_factor_mode sns_factor_mode;                    /* Sensitivity factor mode */
    ot_ive_gmm2_life_update_factor_mode life_update_factor_mode;    /* Life update factor mode */
    td_u16 global_life_update_factor;                               /* Global life update factor (default: 4) */
    td_u16 life_threshold;                                          /* Life threshold (default: 5000) */
    td_u16 freq_init_val;                                           /* Initial frequency (default: 20000) */
    td_u16 freq_reduce_factor;                                      /* Frequency reduction factor (default: 0xFF00) */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART01_H_ */
