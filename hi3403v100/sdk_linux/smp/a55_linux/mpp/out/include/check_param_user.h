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

#ifndef CHECK_PARAM_H
#define CHECK_PARAM_H

#include "ot_common_ive.h"
#include "ot_common.h"
#include "ot_common_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define IVE_EVEN_USER           2
#define IVE_ADDR_NUM_2_USER     2
#define IVE_ADDR_NUM_3_USER     3
#define IVE_IDX_2               2

typedef struct {
    ot_ive_handle *handle;
    const ot_svp_img *src;
    const ot_svp_dst_img *fg;
    const ot_svp_dst_img *bg;
    const ot_svp_mem_info *model;
    const ot_ive_gmm_ctrl *ctrl;
} ive_gmm_arg_enca;

typedef struct {
    ot_ive_handle *handle;
    const ot_svp_src_img *src;
    const ot_svp_dst_img *dst_h;
    const ot_svp_dst_img *dst_v;
    const ot_svp_dst_img *dst_hv;
    const ot_ive_norm_grad_ctrl *ctrl;
} ive_norm_grad_arg_enca;

typedef struct {
    ot_ive_handle *handle;
    const ot_svp_src_img *bg_diff_fg;
    const ot_svp_src_img *cur_grad;
    const ot_svp_src_img *bg_grad;
    const ot_svp_dst_img *grad_fg;
    const ot_ive_grad_fg_ctrl *ctrl;
} ive_grad_fg_arg_enca;

typedef struct {
    ot_ive_handle *handle;
    const ot_svp_src_img *cur_img;
    const ot_svp_data *bg_model;
    const ot_svp_img *fg_flag;
    const ot_svp_dst_img *bg_diff_fg;
    const ot_svp_dst_img *frm_diff_fg;
    const ot_svp_dst_mem_info *stat_data;
    const ot_ive_match_bg_model_ctrl *ctrl;
} ive_match_bg_model_arg_enca;

typedef struct {
    ot_ive_handle *handle;
    const ot_svp_data *bg_model;
    const ot_svp_img *fg_flag;
    const ot_svp_dst_img *bg_img;
    const ot_svp_dst_img *chg_sta_img;
    const ot_svp_dst_img *chg_sta_fg;
    const ot_svp_dst_img *chg_sta_life;
    const ot_svp_dst_mem_info *stat_data;
    const ot_ive_update_bg_model_ctrl *ctrl;
} ive_update_bg_model_arg_enca;

typedef struct {
    ot_ive_handle *handle;
    const ot_svp_src_img *src1;
    const ot_svp_src_img *src2;
    const ot_svp_dst_img *sad;
    const ot_svp_dst_img *thr;
    const ot_ive_sad_ctrl *ctrl;
} ive_sad_arg_enca;

typedef struct {
    ot_ive_handle *handle;
    const ot_svp_src_img *src;
    const ot_svp_src_img *factor;
    const ot_svp_dst_img *fg;
    const ot_svp_dst_img *bg;
    const ot_svp_dst_img *match_model_info;
    const ot_svp_mem_info  *model;
    const ot_ive_gmm2_ctrl *gmm2_ctrl;
} ive_gmm2_arg_enca;

typedef struct {
    td_u64 min_dist;
    td_void *cells;
    ot_svp_point_u16 *ptr_corner;
    td_u32 stride;
    td_u32 grid_width;
    td_u32 grid_height;
    td_u32 cell_size;
    td_u16 max_corner_num;
} ive_st_collect_per_corner_arg_enca;

typedef struct {
    ot_ive_handle *handle;
    ot_svp_src_img *src_prev_pyr;
    ot_svp_src_img *src_next_pyr;
    ot_svp_src_mem_info *prev_pts;
    ot_svp_mem_info *next_pts;
    ot_svp_dst_mem_info *status;
    ot_svp_dst_mem_info *err;
    ot_ive_lk_optical_flow_pyr_ctrl *ctrl;
} ive_lk_optical_flow_pyr_arg_enca;

#ifdef CONFIG_OT_SVP_IVE_PERSP_TRANS
typedef struct {
    ot_ive_handle *handle;
    const ot_svp_src_img *src;
    const ot_svp_rect_u32 *roi;
    const ot_svp_src_mem_info *point_pair;
    const ot_svp_dst_img *dst;
    const ot_ive_persp_trans_ctrl *ctrl;
} ive_persp_trans_arg_enca;
#endif

#if defined(CONFIG_OT_SVP_IVE_KCF) || defined(CONFIG_OT_SVP_IVE_MULTI_KCF)
typedef struct {
    td_u3q5 padding;
    const ot_ive_roi_info *roi_info;
    td_u32 obj_num;
    const ot_svp_mem_info *cos_win_x;
    const ot_svp_mem_info *cos_win_y;
    const ot_svp_mem_info *gauss_peak;
    ot_ive_kcf_obj_list *obj_list;
} ive_kcf_get_train_obj_arg_enca;
#endif

td_s32 ive_check_stride_user(td_u32 stride, td_u32 width, td_s32 align, td_u32 max_stride);

td_s32 ive_check_w_and_h_user(td_u32 width, td_u32 height);

td_s32 ive_check_dma_param_user(const ot_ive_handle *handle, const ot_svp_data *src, const ot_svp_data *dst,
    const ot_ive_dma_ctrl *ctrl);

td_s32 ive_check_filter_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_filter_ctrl *ctrl);

td_s32 ive_check_csc_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_csc_ctrl *ctrl);

td_s32 ive_check_filter_and_csc_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_filter_and_csc_ctrl *ctrl);

td_s32 ive_check_sobel_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst_h, const ot_svp_dst_img *dst_v, const ot_ive_sobel_ctrl *ctrl);

td_s32 ive_check_mag_and_ang_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst_mag, const ot_svp_dst_img *dst_ang, const ot_ive_mag_and_ang_ctrl *ctrl);

td_s32 ive_check_dilate_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_dilate_ctrl *ctrl);

td_s32 ive_check_erode_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_erode_ctrl *ctrl);

td_s32 ive_check_threshold_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_threshold_ctrl *ctrl);

td_s32 ive_check_and_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src1,
    const ot_svp_src_img *src2, const ot_svp_dst_img *dst);

td_s32 ive_check_sub_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src1,
    const ot_svp_src_img *src2, const ot_svp_dst_img *dst, const ot_ive_sub_ctrl *ctrl);

td_s32 ive_check_or_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src1,
    const ot_svp_src_img *src2, const ot_svp_dst_img *dst);

td_s32 ive_check_integ_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_integ_ctrl *ctrl);

td_s32 ive_check_hist_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_mem_info *dst);

td_s32 ive_check_threshold_s16_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_threshold_s16_ctrl *ctrl);

td_s32 ive_check_threshold_u16_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_threshold_u16_ctrl *ctrl);

td_s32 ive_check_16bit_to_8bit_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_16bit_to_8bit_ctrl *ctrl);

td_s32 ive_check_ord_stat_filter_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_order_stats_filter_ctrl *ctrl);

td_s32 ive_check_equalize_hist_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_equalize_hist_ctrl *ctrl);

td_s32 ive_check_add_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src1,
    const ot_svp_src_img *src2, const ot_svp_dst_img *dst, const ot_ive_add_ctrl *ctrl);

td_s32 ive_check_xor_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src1,
    const ot_svp_src_img *src2, const ot_svp_dst_img *dst);

td_s32 ive_check_ncc_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src1,
    const ot_svp_src_img *src2, const ot_svp_dst_mem_info *dst);

td_s32 ive_check_ccl_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src_dst,
    const ot_svp_dst_mem_info *blob, const ot_ive_ccl_ctrl *ctrl);

td_s32 ive_check_gmm_param_user(const ive_gmm_arg_enca *gmm_arg_enca);

td_s32 ive_check_canny_hys_edge_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *edge, const ot_svp_dst_mem_info *stack, const ot_ive_canny_hys_edge_ctrl *ctrl);

td_s32 ive_check_canny_edge_param_user(const ot_svp_img *edge, const ot_svp_mem_info *stack);

td_s32 ive_check_lbp_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *dst, const ot_ive_lbp_ctrl *ctrl);

td_s32 ive_check_norm_grad_param_user(const ive_norm_grad_arg_enca *norm_grad_arg_enca);

td_s32 ive_check_st_candi_corner_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_dst_img *cand_corner, const ot_ive_st_cand_corner_ctrl *ctrl);

td_s32 ive_check_st_corner_param_user(const ot_svp_src_img *cand_corner, const ot_svp_dst_mem_info *corner,
    const ot_ive_st_corner_ctrl *ctrl);

td_s32 ive_check_grad_fg_param_user(const ive_grad_fg_arg_enca *grad_fg_arg_enca);

td_s32 ive_check_match_bg_model_param_user(const ive_match_bg_model_arg_enca *match_bg_model_arg_enca);

td_s32 ive_check_update_bg_model_param_user(const ive_update_bg_model_arg_enca *update_bg_mdoel_arg_enca);

td_s32 ive_check_sad_param_user(const ive_sad_arg_enca *sad_arg_enca);

td_s32 ive_check_gmm2_param_user(const ive_gmm2_arg_enca *gmm2_arg_enca);

td_s32 ive_check_lk_optial_flow_pyr_in_param_user(const ot_svp_src_img src_prev_pyr[],
    const ot_svp_src_img src_next_pyr[], const ot_svp_src_mem_info *prev_points, const ot_svp_mem_info *next_points,
    const ot_ive_lk_optical_flow_pyr_ctrl *ctrl);

td_s32 ive_check_lk_optial_flow_pyr_out_param_user(const ot_ive_handle *handle,
    const ot_svp_dst_mem_info *status, const ot_svp_dst_mem_info *err, const ot_ive_lk_optical_flow_pyr_ctrl *ctrl);

td_s32 ive_check_map_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_src_mem_info *map, const ot_svp_dst_img *dst, const ot_ive_map_ctrl *ctrl);

td_s32 ive_check_resize_param_user(const ot_ive_handle *handle, const ot_svp_src_img src[],
    const ot_svp_dst_img dst[], const ot_ive_resize_ctrl *ctrl);

#ifdef CONFIG_OT_SVP_IVE_PERSP_TRANS
td_s32 ive_check_persp_trans_param_user(const ive_persp_trans_arg_enca *persp_trans_arg_enca);
#endif

#if defined(CONFIG_OT_SVP_IVE_KCF) || defined(CONFIG_OT_SVP_IVE_MULTI_KCF)
td_void ive_get_hog_feature_rect(const ot_ive_roi_info *roi_info, td_u3q5 padding, td_u32 *width, td_u32 *height);

td_s32 ive_check_kcf_gauss_peak_param_user(td_u3q5 padding, const ot_svp_mem_info *gauss_peak);

td_s32 ive_check_kcf_cos_win_param_user(const ot_svp_mem_info *cos_win_x, const ot_svp_mem_info *cos_win_y);

td_s32 ive_check_kcf_obj_list_param_user(const ot_svp_mem_info *mem, td_u32 max_obj_num,
    const ot_ive_kcf_obj_list *obj_list);

td_s32 ive_check_kcf_get_train_obj_param_user(const ive_kcf_get_train_obj_arg_enca *kcf_get_train_obj_arg_enca);

td_s32 ive_check_kcf_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_ive_kcf_obj_list *obj_list, const ot_ive_kcf_proc_ctrl *ctrl);

td_s32 ive_check_kcf_get_obj_param_user(const ot_ive_kcf_obj_list *obj_list, const ot_ive_kcf_bbox bbox[],
    const td_u32 *bbox_obj_num, const ot_ive_kcf_bbox_ctrl *ctrl);

td_s32 ive_check_kcf_judge_obj_bbox_param_user(const ot_ive_roi_info *roi_info, const ot_ive_kcf_bbox *bbox,
    const td_bool *track_ok);

td_s32 ive_check_kcf_obj_update_param_user(const ot_ive_kcf_obj_list *obj_list, const ot_ive_kcf_bbox bbox[],
    td_u32 bbox_obj_num);
#endif

#if defined(CONFIG_OT_SVP_IVE_HOG) || defined(CONFIG_OT_SVP_IVE_MULTI_KCF)
td_s32 ive_check_hog_param_user(const ot_ive_handle *handle, const ot_svp_src_img *src,
    const ot_svp_rect_u32 roi[], const ot_svp_dst_blob dst[], const ot_ive_hog_ctrl *ctrl);
#endif

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif
