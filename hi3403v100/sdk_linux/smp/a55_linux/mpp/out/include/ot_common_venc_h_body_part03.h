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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART03_H_

    SVC_RECT_TYPE2,
    SVC_RECT_TYPE3,
    SVC_RECT_TYPE4,
    SVC_RECT_TYPE_BUTT
} ot_venc_svc_rect_type;

typedef struct {
    td_u8 qpmap_value_i;   /* RW;Range: [0, 255] */
    td_u8 qpmap_value_p;   /* RW;Range: [0, 255] */
    td_u8 skipmap_value;  /* RW;Range: [0, 255] */
} ot_venc_svc_map_param;

typedef struct {
    td_u32 rect_num;
    ot_size base_resolution;
    ot_rect rect_attr[OT_VENC_MAX_SVC_RECT_NUM];
    ot_venc_svc_rect_type detect_type[OT_VENC_MAX_SVC_RECT_NUM];
    td_u64 pts;
} ot_venc_svc_rect_info;

typedef struct {
    ot_venc_svc_map_param  fg_region[SVC_RECT_TYPE_BUTT];
    ot_venc_svc_map_param  motion_region;
    ot_venc_svc_map_param  bg_region;
    td_bool fg_protect_adaptive_en; /* RW;Range: [0, 1] */
} ot_venc_svc_param;

typedef struct {
    td_bool deblur_en;
    td_bool deblur_adaptive_en;
} ot_venc_deblur_param;

typedef struct {
    td_u8 gain;
    td_u8 offset;
    td_u8 qp_delta;
    td_u8 min_qp;
    td_u8 max_qp;
} ot_venc_deblur_near_bg;

typedef struct {
    td_u8 qp_delta;
    td_u8 min_qp;
    td_u8 max_qp;
} ot_venc_deblur_far_bg;

typedef struct {
    td_bool near_bg_en;
    td_bool far_bg_en;
    ot_venc_deblur_near_bg near_bg;
    ot_venc_deblur_far_bg far_bg;
} ot_venc_adv_deblur;
typedef enum {
    OT_VENC_MD_SAD_MODE_ZERO = 0,
    OT_VENC_MD_SAD_MODE_SEARCH = 1,
    OT_VENC_MD_SAD_MODE_BUTT
} ot_venc_md_sad_mode;

typedef struct {
    td_bool online_en;
    td_bool sad_stats_en;
    td_bool level_stats_en;
    ot_venc_md_sad_mode sad_mode;
    td_s32 threshold_gain[OT_VENC_MD_THRESHOLD_NUM];
    td_s32 threshold_offset[OT_VENC_MD_THRESHOLD_NUM];
    td_s32 qp_delta[OT_VENC_MD_LEVEL_NUM];
    td_s32 md_skip_weight[OT_VENC_MD_LEVEL_NUM];
} ot_venc_md_param;

/* the information of the user frame */
typedef struct {
    td_bool rc_info_valid;
    td_u32 target_bits;
    td_u32 max_qp;
    td_u32 min_qp;
    td_u32 start_qp;

    ot_venc_frame_type frame_type;
} ot_omx_user_rc_info;

typedef struct {
    ot_video_frame_info user_frame;
    ot_omx_user_rc_info user_rc_info;
} ot_omx_user_frame_info;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART03_H_ */
