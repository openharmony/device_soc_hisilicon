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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART03_H_

    td_u32 roi_id;
} ot_ive_roi_info;

/* Kcf ctrl param */
typedef struct {
    ot_ive_csc_mode csc_mode;   /* Only support:
                                OT_IVE_CSC_MODE_VIDEO_BT601_YUV_TO_RGB  CSC: YUV2RGB, video transfer mode,
                                RGB value range [16, 235]
                                OT_IVE_CSC_MODE_VIDEO_BT709_YUV_TO_RGB  CSC: YUV2RGB, video transfer mode,
                                RGB value range [16, 235]
                                OT_IVE_CSC_MODE_PIC_BT601_YUV_TO_RGB  CSC: YUV2RGB, picture transfer mode,
                                RGB value range [0, 255]
                                OT_IVE_CSC_MODE_PIC_BT709_YUV_TO_RGB  CSC: YUV2RGB, picture transfer mode,
                                RGB value range [0, 255]
                                */
    ot_svp_mem_info tmp_buf;
    td_u1q15 interp_factor; /* Blend coefficient. [0, 32768] */
    td_u0q16 lamda;        /* The regularization coefficient. [0, 65535] */
    td_u4q12 norm_trunc_alfa;   /* The normalization truncation threshold. [0, 4095] */
    td_u0q8 sigma;         /* The gaussian kernel bandwidth. [0, 255]  */
    td_u8 response_threshold;
    ot_ive_kcf_core_id core_id;
} ot_ive_kcf_proc_ctrl;

typedef struct {
    ot_ive_roi_info roi_info;
    ot_svp_mem_info cos_win_x;
    ot_svp_mem_info cos_win_y;
    ot_svp_mem_info gauss_peak;
    ot_svp_mem_info hog_feature;
    ot_svp_mem_info alpha;
    ot_svp_mem_info dst;
    td_u3q5 padding; /* [48, 160]  */
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_THREE];
} ot_ive_kcf_obj;

typedef struct tag_ot_ive_list_head {
    struct tag_ot_ive_list_head *next, *prev;
} ot_ive_list_head;

typedef struct {
    ot_ive_list_head list;
    ot_ive_kcf_obj kcf_obj;
} ot_ive_kcf_obj_node;

typedef enum {
    OT_IVE_KCF_LIST_STATE_CREATE = 0x1,
    OT_IVE_KCF_LIST_STATE_DESTORY = 0x2,
    OT_IVE_KCF_LIST_STATE_BUTT
} ot_ive_kcf_list_state;

typedef struct {
    ot_ive_kcf_obj_node *obj_node_buf;  /* The object list node address */
    ot_ive_list_head free_obj_list;     /* The free list of object list */
    ot_ive_list_head train_obj_list;    /* The training list of object list */
    ot_ive_list_head track_obj_list;    /* The tracking list of object list */

    td_u32 free_obj_num;                /* The numbers of free list */
    td_u32 train_obj_num;               /* The numbers of training list */
    td_u32 track_obj_num;               /* The numbers of tracking list */
    td_u32 max_obj_num;                 /* The maximum numbers of object list */
    ot_ive_kcf_list_state list_state;   /* The object list state */
    td_u8 *tmp_buf;                     /* Assist buffer */
    td_u32 width;                       /* image width */
    td_u32 height;                      /* image height */
} ot_ive_kcf_obj_list;

typedef struct {
    ot_ive_kcf_obj_node *node;
    td_s32 response; /* Bbox Response value. */

    ot_ive_roi_info roi_info;
    td_bool is_track_ok;
    td_bool is_roi_refresh;
} ot_ive_kcf_bbox;

typedef struct {
    td_u32 max_bbox_num;        /* The member numbers of Bbox Array. */
    td_s32 response_threshold;  /* Select Bbox when Bbox'Response value is greater than or equal to RespThr. */
} ot_ive_kcf_bbox_ctrl;

typedef enum {
    OT_IVE_HOG_MODE_VER_TANGENT_PLANE = 0x1,
    OT_IVE_HOG_MODE_HOR_TANGENT_PLANE = 0x2,
    OT_IVE_HOG_MODE_BUTT
} ot_ive_hog_mode;

typedef struct {
    ot_ive_csc_mode csc_mode;   /* Only support:
                                 * OT_IVE_CSC_MODE_VIDEO_BT601_YUV_TO_RGB  CSC: YUV2RGB, video transfer mode,
                                 * RGB value range [16, 235]
                                 * OT_IVE_CSC_MODE_VIDEO_BT709_YUV_TO_RGB  CSC: YUV2RGB, video transfer mode,
                                 * RGB value range [16, 235]
                                 * OT_IVE_CSC_MODE_PIC_BT601_YUV_TO_RGB  CSC: YUV2RGB, picture transfer mode,
                                 * RGB value range [0, 255]
                                 * OT_IVE_CSC_MODE_PIC_BT709_YUV_TO_RGB  CSC: YUV2RGB, picture transfer mode,
                                 * RGB value range [0, 255] */
    ot_ive_hog_mode hog_mode;   /* Hog mode */
    td_u32 roi_num;             /* Roi number. [1, 64] */
    td_u4q12 norm_trunc_alfa;   /* The normalization truncation threshold. [0, 4095] */

    ot_ive_kcf_core_id core_id;
} ot_ive_hog_ctrl;
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_IVE_H_BODY_PART03_H_ */
