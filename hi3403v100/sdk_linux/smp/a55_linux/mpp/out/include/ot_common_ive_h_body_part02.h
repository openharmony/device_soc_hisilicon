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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART02_H_

    td_u16 freq_add_factor;                                         /* Frequency adding factor (default: 0xEF) */
    td_u16 freq_threshold;                                          /* Frequency threshold (default: 12000) */
    td_u16 var_rate;                                                /* Variation update rate (default: 1) */
    td_u9q7 max_var;                                                /* Max variation (default: (16 * 16)<<7) */
    td_u9q7 min_var;                                                /* Min variation (default: ( 8 *  8)<<7) */
    td_u8 global_sns_factor;                                        /* Global sensitivity factor (default: 8) */
    td_u8 model_num;                                                /* Model number (range: 1~5, default: 3) */
} ot_ive_gmm2_ctrl;

typedef struct {
    ot_svp_mem_info mem;
    td_u16 low_threshold;
    td_u16 high_threshold;
    td_s8 mask[OT_IVE_MASK_NUM];
} ot_ive_canny_hys_edge_ctrl;

typedef struct {
    td_u32 stack_size;  /* Stack size for output */
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_TWELVE]; /* For 16 byte align */
} ot_ive_canny_stack_size;

/*
 * LBP compare mode
 */
typedef enum {
    OT_IVE_LBP_COMPARE_MODE_NORMAL = 0x0,
    OT_IVE_LBP_COMPARE_MODE_ABS = 0x1,

    OT_IVE_LBP_COMPARE_MODE_BUTT
} ot_ive_lbp_compare_mode;

typedef struct {
    ot_ive_lbp_compare_mode mode;
    ot_svp_8bit bit_threshold;
} ot_ive_lbp_ctrl;

/*
 * Type of the gradient_filter output format
 */
typedef enum {
    OT_IVE_NORM_GRAD_OUT_CTRL_HOR_AND_VER = 0x0,
    OT_IVE_NORM_GRAD_OUT_CTRL_HOR = 0x1,
    OT_IVE_NORM_GRAD_OUT_CTRL_VER = 0x2,
    OT_IVE_NORM_GRAD_OUT_CTRL_COMBINE = 0x3,

    OT_IVE_NORM_GRAD_OUT_CTRL_BUTT
} ot_ive_norm_grad_out_ctrl;

typedef struct {
    ot_ive_norm_grad_out_ctrl out_ctrl;
    td_s8 mask[OT_IVE_MASK_NUM];
    td_u8 norm;
} ot_ive_norm_grad_ctrl;

/*
 * lk_optical_flow_pyr output mode
 */
typedef enum {
    OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_NONE = 0,   /* Output none */
    OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_STATUS = 1, /* Output status */
    OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH = 2,   /* Output status and err */

    OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BUTT
} ot_ive_lk_optical_flow_pyr_out_mode;

typedef struct {
    ot_ive_lk_optical_flow_pyr_out_mode out_mode;
    td_bool use_init_flow;          /* where to use initial flow */
    td_u16 points_num;              /* Number of the feature points,<=500 */
    td_u8 max_level;                /* max_level should be in [0, 3] */
    td_u0q8 min_eig_val_threshold;  /* Minimum eigenvalue threshold */
    td_u8 iter_cnt;                 /* Maximum iteration times, <=20 */
    td_u0q8 eps;                    /* Used for exit criteria: dx^2 + dy^2 < eps */
} ot_ive_lk_optical_flow_pyr_ctrl;

typedef struct {
    td_u16 max_eig_val;     /* Shi-Tomasi-like second step output MaxEig */
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_FOURTEEN]; /* For 16 byte align */
} ot_ive_st_max_eig_val;

typedef struct {
    ot_svp_mem_info mem;
    td_u0q8 quality_level;
} ot_ive_st_cand_corner_ctrl;

typedef struct {
    td_u16 corner_num;
    ot_svp_point_u16 corner[OT_IVE_ST_MAX_CORNER_NUM];
} ot_ive_st_corner_info;

typedef struct {
    td_u16 max_corner_num;
    td_u16 min_dist;
} ot_ive_st_corner_ctrl;

/*
 * grad_fg mode enum
 */
typedef enum {
    OT_IVE_GRAD_FG_MODE_USE_CUR_GRAD = 0x0,
    OT_IVE_GRAD_FG_MODE_FIND_MIN_GRAD = 0x1,

    OT_IVE_GRAD_FG_MODE_BUTT
} ot_ive_grad_fg_mode;

typedef struct {
    ot_ive_grad_fg_mode mode;  /* Calculation mode */
    td_u16 edw_factor;         /* Edge width adjustment factor (range: 500 to 2000; default: 1000) */
    td_u8 crl_coef_threshold;  /* Gradient vector correlation coefficient threshold
                                (ranges: 50 to 100; default: 80) */
    td_u8 mag_crl_threshold;   /* Gradient amplitude threshold (range: 0 to 20; default: 4) */
    td_u8 min_mag_diff;        /* Gradient magnitude difference threshold (range: 2 to 8; default: 2) */
    td_u8 noise_val;           /* Gradient amplitude noise threshold (range: 1 to 8; default: 1) */
    td_u8 edw_dark;            /* Black pixels enable flag (range: 0 (no), 1 (yes); default: 1) */
} ot_ive_grad_fg_ctrl;

typedef struct {
    td_u8q4f4 mean;         /* Candidate background grays value */
    td_u16 start_time;      /* Candidate Background start time */
    td_u16 sum_access_time; /* Candidate Background cumulative access time */
    td_u16 short_keep_time; /* Candidate background short hold time */
    td_u8 chg_condition;    /* Time condition for candidate background into the changing state */
    td_u8 poten_bg_life;    /* Potential background cumulative access time */
} ot_ive_cand_bg_pixel;

typedef struct {
    td_u8q4f4 mean;                         /* 0# background grays value */
    td_u16 access_time;                     /* Background cumulative access time */
    td_u8 prev_gray;                        /* Gray value of last pixel */
    td_u5q3 diff_threshold;                 /* Differential threshold */
    td_u8 access_flag;                      /* Background access flag */
    td_u8 bg_gray[OT_IVE_ARR_NUM_THREE];    /* 1# ~ 3# background grays value */
} ot_ive_wrok_bg_pixel;

typedef struct {
    td_u8 work_bg_life[OT_IVE_ARR_NUM_THREE];   /* 1# ~ 3# background vitality */
    td_u8 candi_bg_life;                        /* Candidate background vitality */
} ot_ive_bg_life;

typedef struct {
    ot_ive_wrok_bg_pixel work_bg_pixel;     /* Working background */
    ot_ive_cand_bg_pixel cand_pixel;        /* Candidate background */
    ot_ive_bg_life bg_life;                 /* Background vitality */
} ot_ive_bg_model_pixel;

typedef struct {
    td_u32 pixel_num;
    td_u32 sum_luma;
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_EIGHT];
} ot_ive_fg_status_data;

typedef struct {
    td_u32 pixel_num;
    td_u32 sum_luma;
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_EIGHT];
} ot_ive_bg_status_data;

typedef struct {
    td_u32 cur_frame_num;         /* Current frame timestamp, in frame units */
    td_u32 prev_frame_num;         /* Previous frame timestamp, in frame units */
    td_u16 time_threshold;            /* Potential background replacement time threshold
                                (range: 2 to 100 frames; default: 20) */

    td_u8 diff_threshold_crl_coef;    /* Correlation coefficients between differential threshold and gray value
                                (range: 0 to 5; default: 0) */
    td_u8 diff_max_threshold;         /* Maximum of background differential threshold (range: 3 to 15; default: 6) */
    td_u8 diff_min_threshold;         /* Minimum of background differential threshold (range: 3 to 15; default: 4) */
    td_u8 diff_threshold_inc;         /* Dynamic Background differential threshold increment
                                (range: 0 to 6; default: 0) */
    td_u8 fast_learn_rate;      /* Quick background learning rate (range: 0 to 4; default: 2) */
    td_u8 detected_chg_rgn;       /* Whether to detect change region (range: 0 (no), 1 (yes); default: 0) */
} ot_ive_match_bg_model_ctrl;

typedef struct {
    td_u32 cur_frame_num;         /* Current frame timestamp, in frame units */
    td_u32 prev_check_time;        /* The last time when background status is checked */
    td_u32 frame_check_period;      /* Background status checking period (range: 0 to 2000 frames; default: 50) */

    td_u32 init_min_time;       /* Background initialization shortest time
                                (range: 20 to 6000 frames; default: 100) */
    td_u32 steady_bg_min_blend_time; /* Steady background integration shortest time
                                (range: 20 to 6000 frames; default: 200) */
    td_u32 steady_bg_max_blend_time; /* Steady background integration longest time
                                (range: 20 to 40000 frames; default: 1500) */
    td_u32 dynamic_bg_min_blend_time; /* Dynamic background integration shortest time
                                (range: 0 to 6000 frames; default: 0) */
    td_u32 static_detection_min_time;  /* Still detection shortest time (range: 20 to 6000 frames; default: 80) */
    td_u16 fg_max_fade_time;     /* Foreground disappearing longest time
                                (range: 1 to 255 seconds; default: 15) */
    td_u16 bg_max_fade_time;     /* Background disappearing longest time
                                (range: 1 to 255  seconds ; default: 60) */

    td_u8 steay_bg_access_time_rate_threshold; /* Steady background access time ratio threshold
                                (range: 10 to 100; default: 80) */
    td_u8 chg_bg_access_time_rate_threshold; /* Change background access time ratio threshold
                                (range: 10 to 100; default: 60) */
    td_u8 dynamic_bg_access_time_threshold;     /* Dynamic background access time ratio threshold
                                (range: 0 to 50; default: 0) */
    td_u8 dynamic_bg_depth;          /* Dynamic background depth (range: 0 to 3; default: 3) */
    td_u8 bg_eff_state_rate_threshold;     /* Background state time ratio threshold when initializing
                                (range: 90 to 100; default: 90) */

    td_u8 acc_bg_learn;  /* Whether to accelerate background learning (range: 0 (no), 1 (yes); default: 0) */
    td_u8 detected_chg_rgn; /* Whether to detect change region (range: 0 (no), 1 (yes); default: 0) */
} ot_ive_update_bg_model_ctrl;

typedef enum {
    OT_IVE_ANN_MLP_ACTV_FUNC_IDENTITY = 0x0,
    OT_IVE_ANN_MLP_ACTV_FUNC_SIGMOID_SYM = 0x1,
    OT_IVE_ANN_MLP_ACTV_FUNC_GAUSSIAN = 0x2,

    OT_IVE_ANN_MLP_ACTV_FUNC_BUTT
} ot_ive_ann_mlp_actv_func;

typedef enum {
    OT_IVE_ANN_MLP_ACCURATE_SRC16_WGT16 = 0x0, /* input decimals' accurate 16 bit, wgt 16bit */
    OT_IVE_ANN_MLP_ACCURATE_SRC14_WGT20 = 0x1, /* input decimals' accurate 14 bit, wgt 20bit */

    OT_IVE_ANN_MLP_ACCURATE_BUTT
} ot_ive_ann_mlp_accurate;

typedef struct {
    ot_ive_ann_mlp_actv_func actv_func;
    ot_ive_ann_mlp_accurate accurate;
    ot_svp_mem_info wgt;
    td_u32 total_wgt_size;

    td_u16 layer_cnt[OT_IVE_ARR_NUM_EIGHT];      /* 8 layers, including input and output layer */
    td_u16 max_count;           /* MaxCount should be less than and equal 1024 */
    td_u8 layer_num;            /* layerNum should in (2,8] */
    td_u8 reserved;
} ot_ive_ann_mlp_model;

typedef enum {
    OT_IVE_SVM_TYPE_C_SVC = 0x0,
    OT_IVE_SVM_TYPE_NU_SVC = 0x1,

    OT_IVE_SVM_TYPE_BUTT
} ot_ive_svm_type;

typedef enum {
    OT_IVE_SVM_KERNEL_TYPE_LINEAR = 0x0,
    OT_IVE_SVM_KERNEL_TYPE_POLY = 0x1,
    OT_IVE_SVM_KERNEL_TYPE_RBF = 0x2,
    OT_IVE_SVM_KERNEL_TYPE_SIGMOID = 0x3,

    OT_IVE_SVM_KERNEL_TYPE_BUTT
} ot_ive_svm_kernel_type;

typedef struct {
    ot_ive_svm_type type;
    ot_ive_svm_kernel_type kernel_type;

    ot_svp_mem_info sv;                 /* SV memory */
    ot_svp_mem_info decision_func;      /* Decision functions memory */
    td_u32 total_decision_func_size;    /* All decision functions coef size in byte */

    td_u16 feature_dim;
    td_u16 sv_total;
    td_u8 class_cnt;
} ot_ive_svm_model;

/*
 * Sad mode
 */
typedef enum {
    OT_IVE_SAD_MODE_MB_4X4 = 0x0,   /* 4x4 */
    OT_IVE_SAD_MODE_MB_8X8 = 0x1,   /* 8x8 */
    OT_IVE_SAD_MODE_MB_16X16 = 0x2, /* 16x16 */

    OT_IVE_SAD_MODE_BUTT
} ot_ive_sad_mode;
/*
 * Sad output ctrl
 */
typedef enum {
    OT_IVE_SAD_OUT_CTRL_16BIT_BOTH = 0x0, /* Output 16 bit sad and threshold */
    OT_IVE_SAD_OUT_CTRL_8BIT_BOTH = 0x1,  /* Output 8 bit sad and threshold */
    OT_IVE_SAD_OUT_CTRL_16BIT_SAD = 0x2,  /* Output 16 bit sad */
    OT_IVE_SAD_OUT_CTRL_8BIT_SAD = 0x3,   /* Output 8 bit sad */
    OT_IVE_SAD_OUT_CTRL_THRESHOLD = 0x4,  /* Output threshold,16 bits sad */

    OT_IVE_SAD_OUT_CTRL_BUTT
} ot_ive_sad_out_ctrl;

typedef struct {
    ot_ive_sad_mode mode;
    ot_ive_sad_out_ctrl out_ctrl;
    td_u16 threshold;
    td_u8 min_val;              /* Min value */
    td_u8 max_val;              /* Max value */
} ot_ive_sad_ctrl;

/*
 * Resize zoom mode
 */
typedef enum {
    OT_IVE_RESIZE_MODE_LINEAR = 0x0, /* Bilinear interpolation */
    OT_IVE_RESIZE_MODE_AREA = 0x1,

    OT_IVE_RESIZE_MODE_BUTT
} ot_ive_resize_mode;

typedef struct {
    ot_ive_resize_mode mode;
    ot_svp_mem_info mem;
    td_u16 num;
} ot_ive_resize_ctrl;

/*
 * CNN active function mode
 */
typedef enum {
    OT_IVE_CNN_ACTV_FUNC_NONE = 0x0,    /* 1. Do not taking a activation, equivalent f(x)=x */
    OT_IVE_CNN_ACTV_FUNC_RELU = 0x1,    /* 2. f(x)=max(0, x) */
    OT_IVE_CNN_ACTV_FUNC_SIGMOID = 0x2, /* 3. f(x)=1/(1+exp(-x)), not support */

    OT_IVE_CNN_ACTV_FUNC_BUTT
} ot_ive_cnn_actv_func;

/*
 * CNN pooling mode
 */
typedef enum {
    OT_IVE_CNN_POOLING_NONE = 0x0, /* Do not taking a pooling action */
    OT_IVE_CNN_POOLING_MAX = 0x1,  /* Using max value of every pooling area */
    OT_IVE_CNN_POOLING_AVG = 0x2,  /* Using average value of every pooling area */

    OT_IVE_CNN_POOLING_BUTT
} ot_ive_cnn_pooling;

typedef struct {
    ot_ive_cnn_actv_func actv_func;   /* Type of activation function */
    ot_ive_cnn_pooling pooling;       /* Mode of pooling method */

    td_u8 feature_map_num;            /* Number of feature maps */
    td_u8 kernel_size;                /* Kernel size, only support 3 currently */
    td_u8 conv_step;                  /* Convolution step, only support 1 currently */

    td_u8 pool_size;                  /* Pooling size, only support 2 currently */
    td_u8 pool_step;                  /* Pooling step, only support 2 currently */
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_THREE];
} ot_ive_cnn_conv_pooling;

typedef struct {
    td_u16 layer_cnt[OT_IVE_ARR_NUM_EIGHT];    /* Neuron number of every fully connected layers */
    td_u16 max_cnt;         /* Max neuron number in all fully connected layers */
    td_u8 layer_num;        /* Number of fully connected layer */
    td_u8 reserved;
} ot_ive_cnn_fc_info;

typedef struct {
    ot_ive_cnn_conv_pooling conv_pool[OT_IVE_ARR_NUM_EIGHT];  /* Conv-ReLU-Pooling layers info */
    ot_ive_cnn_fc_info fc_info;  /* Fully connected layers info */

    ot_svp_mem_info conv_kernel_bias;      /* Conv-ReLU-Pooling layers' kernels and bias */
    td_u32 conv_kernel_bias_size;          /* Size of Conv-ReLU-Pooling layer' kernels and bias */

    ot_svp_mem_info fc_wgt_bias;          /* Fully Connection Layers' weights and bias */
    td_u32 fc_wgt_bias_size;              /* Size of fully connection layers weights and bias */

    td_u32 total_mem_size;                 /* Total memory size of all kernels, weights, bias */

    ot_svp_img_type type;                /* Image type used for the CNN model */
    td_u32 width;                          /* Image width used for the model */
    td_u32 height;                         /* Image height used for the model */

    td_u16 class_cnt;                    /* Number of classes */
    td_u8 conv_pool_layer_num;             /* Number of Conv-ReLU-Pooling layers */
    td_u8 reserved;
} ot_ive_cnn_model;

typedef struct {
    ot_svp_mem_info mem;    /* Assist memory */
    td_u32 num;             /* Input image number */
} ot_ive_cnn_ctrl;

typedef struct {
    td_s32 class_idx;       /* The most possible index of the classification */
    td_s32 conf;            /* The conf of the classification */
} ot_ive_cnn_result;

/*
 * Perspective transform algorithm mode
 */
typedef enum {
    OT_IVE_PERSP_TRANS_ALG_MODE_NR_SIM = 0x0, /* Non-reflective similarity transform mode */
    OT_IVE_PERSP_TRANS_ALG_MODE_SIM = 0x1,    /* Reflective similarity transform mode */
    OT_IVE_PERSP_TRANS_ALG_MODE_AFFINE = 0x2, /* Affine transform mode */

    OT_IVE_PERSP_TRANS_ALG_MODE_BUTT
} ot_ive_persp_trans_alg_mode;

typedef struct {
    ot_svp_point_u14q2 src_point; /* Source point */
    ot_svp_point_u14q2 dst_point; /* Destination point */
} ot_ive_persp_trans_point_pair;

/*
 * Perspective transform csc mode
 */
typedef enum {
    OT_IVE_PERSP_TRANS_CSC_MODE_NONE = 0x0,                /* No do csc */
    OT_IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT601_YUV_TO_RGB = 0x1, /* CSC: YUV2RGB, video transfer mode,
                                                      RGB value range [16, 235] */
    OT_IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT709_YUV_TO_RGB = 0x2, /* CSC: YUV2RGB, video transfer mode,
                                                      RGB value range [16, 235] */
    OT_IVE_PERSP_TRANS_CSC_MODE_PIC_BT601_YUV_TO_RGB = 0x3,   /* CSC: YUV2RGB, picture transfer mode,
                                                      RGB value range [0, 255] */
    OT_IVE_PERSP_TRANS_CSC_MODE_PIC_BT709_YUV_TO_RGB = 0x4,   /* CSC: YUV2RGB, picture transfer mode,
                                                      RGB value range [0, 255] */

    OT_IVE_PERSP_TRANS_CSC_MODE_BUTT
} ot_ive_persp_trans_csc_mode;

typedef struct {
    ot_ive_persp_trans_alg_mode alg_mode;   /* Alg mode */
    ot_ive_persp_trans_csc_mode csc_mode;   /* CSC mode */
    td_u16 roi_num;                         /* Roi number */
    td_u16 point_pair_num;                  /* Point pair number  */
} ot_ive_persp_trans_ctrl;

typedef enum {
    OT_IVE_KCF_CORE0 = 0x0,

    OT_IVE_KCF_CORE_BUTT
} ot_ive_kcf_core_id;

typedef struct {
    ot_svp_rect_s24q8 roi;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART02_H_ */
