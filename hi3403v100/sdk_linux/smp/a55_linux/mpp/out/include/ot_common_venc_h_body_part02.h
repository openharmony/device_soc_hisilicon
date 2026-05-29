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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART02_H_


typedef struct {
    td_u32  intra_trans_mode;
    td_u32  inter_trans_mode;
    td_bool scaling_list_valid;
    td_u8   inter_scaling_list8x8[OT_VENC_SCALING_LIST_SIZE_64];
    td_u8   intra_scaling_list8x8[OT_VENC_SCALING_LIST_SIZE_64];
    td_s32  chroma_qp_index_offset;
} ot_venc_h264_trans;

typedef struct {
    td_u32 entropy_coding_mode_i;
    td_u32 entropy_coding_mode_p;
    td_u32 entropy_coding_mode_b;
    td_u32 cabac_init_idc;
} ot_venc_h264_entropy;

typedef struct {
    td_u32 pic_order_cnt_type;
} ot_venc_h264_poc;

typedef struct {
    td_u32 disable_deblocking_filter_idc;
    td_s32 slice_alpha_c0_offset_div2;
    td_s32 slice_beta_offset_div2;
} ot_venc_h264_dblk;

typedef struct {
    td_u8  timing_info_present_flag;
    td_u8  fixed_frame_rate_flag;
    td_u32 num_units_in_tick;
    td_u32 time_scale;
} ot_venc_h264_vui_time_info;

typedef struct {
    td_u8  aspect_ratio_info_present_flag;
    td_u8  aspect_ratio_idc;
    td_u8  overscan_info_present_flag;
    td_u8  overscan_appropriate_flag;
    td_u16 sar_width;
    td_u16 sar_height;
} ot_venc_vui_aspect_ratio;

typedef struct {
    td_u8 video_signal_type_present_flag;
    td_u8 video_format;
    td_u8 video_full_range_flag;
    td_u8 colour_description_present_flag;
    td_u8 colour_primaries;
    td_u8 transfer_characteristics;
    td_u8 matrix_coefficients;
} ot_venc_vui_video_signal;

typedef struct {
    td_u8 bitstream_restriction_flag;
} ot_venc_vui_bitstream_restric;

typedef struct {
    ot_venc_vui_aspect_ratio      vui_aspect_ratio;
    ot_venc_h264_vui_time_info    vui_time_info;
    ot_venc_vui_video_signal      vui_video_signal;
    ot_venc_vui_bitstream_restric vui_bitstream_restric;
} ot_venc_h264_vui;

typedef struct {
    td_u32 timing_info_present_flag;
    td_u32 num_units_in_tick;
    td_u32 time_scale;
    td_u32 num_ticks_poc_diff_one_minus1;
} ot_venc_h265_vui_time_info;

typedef struct {
    ot_venc_vui_aspect_ratio      vui_aspect_ratio;
    ot_venc_h265_vui_time_info    vui_time_info;
    ot_venc_vui_video_signal      vui_video_signal;
    ot_venc_vui_bitstream_restric vui_bitstream_restric;
} ot_venc_h265_vui;

typedef struct {
    td_u32 qfactor;
    td_u8  y_qt[OT_VENC_JPEG_QT_COEF_NUM];
    td_u8  cb_qt[OT_VENC_JPEG_QT_COEF_NUM];
    td_u8  cr_qt[OT_VENC_JPEG_QT_COEF_NUM];
    td_u32 mcu_per_ecs;
    td_bool ecs_output_en;
} ot_venc_jpeg_param;

typedef struct {
    td_u8  y_qt[OT_VENC_MJPEG_QT_COEF_NUM];
    td_u8  cb_qt[OT_VENC_MJPEG_QT_COEF_NUM];
    td_u8  cr_qt[OT_VENC_MJPEG_QT_COEF_NUM];
    td_u32 mcu_per_ecs;
    td_bool ecs_output_en;
} ot_venc_mjpeg_param;

typedef struct {
    td_u32  idx;
    td_bool enable;
    td_bool is_abs_qp;
    td_s32  qp;
    ot_rect rect;
} ot_venc_roi_attr;

typedef enum {
    OT_VENC_ROI_FOR_I = 0,
    OT_VENC_ROI_FOR_P_B,
    OT_VENC_ROI_FOR_VI,
    OT_VENC_ROI_FOR_BUTT,
} ot_venc_roi_for_frame;

typedef struct {
    td_u32  idx;
    td_bool enable[OT_VENC_ROI_FOR_BUTT];
    td_bool is_abs_qp[OT_VENC_ROI_FOR_BUTT];
    td_s32  qp[OT_VENC_ROI_FOR_BUTT];
    ot_rect rect[OT_VENC_ROI_FOR_BUTT];
} ot_venc_roi_attr_ex;

typedef struct {
    td_u32 idx;
    td_bool enable;
    td_u32 level;
    ot_rect rect;
} ot_venc_jpeg_roi_attr;

typedef struct {
    td_u32 idx;
    td_bool enable;
    td_u32 fg_level;
    td_u32 bg_level;
    ot_rect rect;
} ot_venc_jpeg_roi_adv_attr;

typedef struct {
    td_s32 src_frame_rate;
    td_s32 dst_frame_rate;
} ot_venc_roi_bg_frame_rate;

typedef struct {
    td_u32  base;
    td_u32  enhance;
    td_bool pred_en;
    td_bool base_qp_delta_en;
    td_s32  base_qp_delta;
} ot_venc_ref_param;

typedef enum {
    OT_VENC_JPEG_DERING_LEVEL_0 = 0,                  /* Jpeg channel dering mode is 1 */
    OT_VENC_JPEG_DERING_LEVEL_1 = 1,                  /* Jpeg channel dering mode is adaptive  */
    OT_VENC_JPEG_DERING_LEVEL_2 = 2,                  /* Jpeg channel dering mode is adaptive  */
    OT_VENC_JPEG_DERING_LEVEL_3 = 3,                  /* Jpeg channel dering mode is adaptive  */
    OT_VENC_JPEG_DERING_LEVEL_BUTT,
} ot_venc_jpeg_dering_level;

typedef enum {
    OT_VENC_JPEG_ENC_ALL  = 0,                        /* Jpeg channel snap all the pictures when started. */
    OT_VENC_JPEG_ENC_SNAP = 1,                        /* Jpeg channel snap the flashed pictures when started. */
    OT_VENC_JPEG_ENC_BUTT,
} ot_venc_jpeg_enc_mode;

typedef struct {
    td_phys_addr_t phys_addr[OT_VENC_MAX_TILE_NUM];
    td_void ATTRIBUTE *user_addr[OT_VENC_MAX_TILE_NUM];
    td_u64 ATTRIBUTE buf_size[OT_VENC_MAX_TILE_NUM];
} ot_venc_stream_buf_info;

typedef struct {
    td_u32 constrained_intra_pred_flag;
    td_u32 strong_intra_smoothing_enabled_flag;
} ot_venc_h265_pu;

typedef struct {
    td_s32  cb_qp_offset;
    td_s32  cr_qp_offset;
    td_bool scaling_list_en;
    td_bool scaling_list_tu4_valid;
    td_u8   inter_scaling_list4x4[2][OT_VENC_SCALING_LIST_SIZE_16]; // 2: 0 luma; 1 chroma
    td_u8   intra_scaling_list4x4[2][OT_VENC_SCALING_LIST_SIZE_16]; // 2: 0 luma; 1 chroma
    td_bool scaling_list_tu8_valid;
    td_u8   inter_scaling_list8x8[2][OT_VENC_SCALING_LIST_SIZE_64]; // 2: 0 luma; 1 chroma
    td_u8   intra_scaling_list8x8[2][OT_VENC_SCALING_LIST_SIZE_64]; // 2: 0 luma; 1 chroma
    td_bool scaling_list_tu16_valid;
    td_u8   inter_scaling_list16x16[2][OT_VENC_SCALING_LIST_SIZE_64]; // 2: 0 luma; 1 chroma
    td_u8   intra_scaling_list16x16[2][OT_VENC_SCALING_LIST_SIZE_64]; // 2: 0 luma; 1 chroma
    td_bool scaling_list_tu32_valid;
    td_u8   inter_scaling_list32x32[OT_VENC_SCALING_LIST_SIZE_64];
    td_u8   intra_scaling_list32x32[OT_VENC_SCALING_LIST_SIZE_64];
} ot_venc_h265_trans;

typedef struct {
    td_u32 cabac_init_flag;
} ot_venc_h265_entropy;

typedef struct {
    td_u32 slice_deblocking_filter_disabled_flag;
    td_s32 slice_beta_offset_div2;
    td_s32 slice_tc_offset_div2;
    td_u32 loop_filter_across_tiles_enabled_flag;
    td_u32 loop_filter_across_slices_enabled_flag;
} ot_venc_h265_dblk;

typedef struct {
    td_u32 slice_sao_luma_flag;
    td_u32 slice_sao_chroma_flag;
} ot_venc_h265_sao;

typedef enum {
    OT_VENC_INTRA_REFRESH_ROW = 0,
    OT_VENC_INTRA_REFRESH_COLUMN,
    OT_VENC_INTRA_REFRESH_BUTT
} ot_venc_intra_refresh_mode;

typedef struct {
    td_bool enable;
    ot_venc_intra_refresh_mode mode;
    td_u32 refresh_num;
    td_u32 request_i_qp;
} ot_venc_intra_refresh;

typedef enum {
    OT_VENC_MOD_VENC = 1,
    OT_VENC_MOD_H264,
    OT_VENC_MOD_H265,
    OT_VENC_MOD_JPEG,
    OT_VENC_MOD_RC,
    OT_VENC_MOD_BUTT
} ot_venc_mod_type;

typedef struct {
    td_u32 buf_cache;
    td_u32 frame_buf_recycle;
} ot_venc_venc_mod_param;

typedef struct {
    td_u32    one_stream_buf;
    td_u32    mini_buf_mode;
    td_u32    low_power_mode;
    ot_vb_src vb_src;
    td_bool   qp_hist_en;
    td_u32    max_user_data_len; /* RW; Range:[0, 65536]; one user data buffer len */
} ot_venc_h264_mod_param;

typedef struct {
    td_u32    one_stream_buf;
    td_u32    mini_buf_mode;
    td_u32    low_power_mode;
    ot_vb_src vb_src;
    td_bool   qp_hist_en;
    td_u32    max_user_data_len; /* RW; Range:[0, 65536]; one user data buffer len */
} ot_venc_h265_mod_param;

typedef struct {
    td_u32 one_stream_buf;
    td_u32 mini_buf_mode;
    td_u32 clear_stream_buf;
    td_u32 dering_mode;
} ot_venc_jpeg_mod_param;

typedef struct {
    ot_venc_mod_type mod_type;
    union {
        ot_venc_venc_mod_param venc_mod_param; /* AUTO:ot_venc_mod_type:OT_VENC_MOD_VENC; */
        ot_venc_h264_mod_param h264_mod_param; /* AUTO:ot_venc_mod_type:OT_VENC_MOD_H264; */
        ot_venc_h265_mod_param h265_mod_param; /* AUTO:ot_venc_mod_type:OT_VENC_MOD_H265; */
        ot_venc_jpeg_mod_param jpeg_mod_param; /* AUTO:ot_venc_mod_type:OT_VENC_MOD_JPEG; */
    };
} ot_venc_mod_param;

typedef enum {
    OT_VENC_FRAME_TYPE_NONE = 1,
    OT_VENC_FRAME_TYPE_IDR,
    OT_VENC_FRAME_TYPE_P_SKIP,
    OT_VENC_FRAME_TYPE_BUTT
} ot_venc_frame_type;

typedef struct {
    td_bool qpmap_valid;
    td_bool skip_weight_valid;
    td_u32  blk_start_qp;
    td_phys_addr_t qpmap_phys_addr;
    td_phys_addr_t skip_weight_phys_addr;
    ot_venc_frame_type frame_type;
} ot_venc_user_rc_info;

typedef struct {
    td_bool valid;
    td_phys_addr_t phys_addr;
} ot_venc_user_roimap;

typedef struct {
    ot_video_frame_info user_frame;
    ot_venc_user_rc_info user_rc_info;
    ot_venc_user_roimap user_roimap;
} ot_venc_user_frame_info;

typedef struct {
    td_u8 data_y;
    td_u8 data_u;
    td_u8 data_v;
} ot_venc_mosaic_pixel_yuv;

typedef struct {
    td_u32 rect_num;
    ot_rect rect[OT_VENC_MAX_MOSAIC_RECT_NUM];
} ot_venc_mosaic_rect;

typedef struct {
    td_bool valid;
    td_phys_addr_t phys_addr;
    td_bool specified_yuv_en;
    ot_venc_mosaic_pixel_yuv pixel_yuv;
} ot_venc_mosaic_map;

/* the information of the mosaic */
typedef struct {
    ot_venc_mosaic_mode mode;
    ot_mosaic_blk_size blk_size;

    union {
        ot_venc_mosaic_rect rect_param; /* AUTO:ot_venc_mosaic_mode:OT_VENC_MOSAIC_MODE_RECT; */
        ot_venc_mosaic_map map_param; /* AUTO:ot_venc_mosaic_mode:OT_VENC_MOSAIC_MODE_MAP; */
    };
} ot_venc_mosaic_info;

/* the information of the multi frame */
typedef struct {
    td_u32 frame_num;
    ot_video_frame_info frame[OT_VENC_MAX_FRAME_NUM];
    ot_venc_mosaic_info mosaic_info;
} ot_venc_multi_frame_info;

typedef struct {
    td_bool mosaic_en;
    td_bool composite_enc_en;
    td_u32 quality_level; /* 0: lower quality with less ddr consumption; 1: higher quality with more ddr consumption. */
} ot_venc_chn_config;

typedef struct {
    td_u32  idx;
    td_bool enable;
    ot_rect rect;
} ot_venc_sse_rgn;

typedef struct {
    td_bool color_to_grey_en;
    td_u32  priority;
    td_u32  max_stream_cnt;
    td_u32  poll_wake_up_frame_cnt;
    ot_crop_info crop_info;
    ot_frame_rate_ctrl frame_rate;
} ot_venc_chn_param;

typedef struct {
    td_bool enable;
    td_u32  direction;
    td_u32  gain;
    td_u32  offset;
    td_u32  threshold_p[OT_VENC_TEXTURE_THRESHOLD_SIZE];
    td_u32  threshold_b[OT_VENC_TEXTURE_THRESHOLD_SIZE];
} ot_venc_fg_protect;

typedef enum {
    OT_VENC_SCENE_0 = 0,        /* RW; A scene in which the camera does not move or periodically moves continuously */
    OT_VENC_SCENE_1 = 1,        /* RW; Motion scene at high bit rate */
    OT_VENC_SCENE_2 = 2,        /* RW; It has regular continuous motion at medium bit rate and
                                   the encoding pressure is relatively large */
    OT_VENC_SCENE_BUTT
} ot_venc_scene_mode;

typedef struct {
    td_bool enable;
    td_s32 strength0;
    td_s32 strength1;
} ot_venc_debreath_effect;

typedef struct {
    ot_op_mode pred_mode;
    td_u32 intra32_cost;
    td_u32 intra16_cost;
    td_u32 intra8_cost;
    td_u32 intra4_cost;
    td_u32 inter64_cost;
    td_u32 inter32_cost;
    td_u32 inter16_cost;
    td_u32 inter8_cost;
} ot_venc_cu_pred;

typedef struct {
    td_bool enable;
    td_u32  gain;
    td_u32  offset;
    td_u32  bg_cost;
    td_u32  fg_cost;
} ot_venc_skip_bias;

typedef struct {
    td_bool enable;
    td_s32  qp_delta[OT_VENC_MAX_HIERARCHY_NUM];
    td_s32  frame_num[OT_VENC_MAX_HIERARCHY_NUM];
} ot_venc_hierarchical_qp;

typedef struct {
    ot_vb_pool pic_vb_pool;
    ot_vb_pool pic_info_vb_pool;
} ot_venc_chn_pool;

typedef struct {
    td_u32 clear_stats_after_set_attr;
} ot_venc_rc_adv_param;

typedef struct {
    td_bool enable;
    td_u32 split_mode;
    td_u32 split_size;
    td_bool slice_output_en;
} ot_venc_slice_split;

typedef struct {
    td_u32 param_set_id; /* Range: for H264: [0, 30]; for H265: [0, 15]; */
} ot_venc_param_set_id;

typedef struct {
    ot_op_mode mode;
    td_u32 hor;
    td_u32 ver;
} ot_venc_search_window;

typedef enum {
    SVC_RECT_TYPE0 = 0,
    SVC_RECT_TYPE1,


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART02_H_ */
