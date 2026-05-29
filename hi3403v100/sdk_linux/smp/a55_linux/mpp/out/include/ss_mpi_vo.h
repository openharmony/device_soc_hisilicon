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

#ifndef __SS_MPI_VO_H__
#define __SS_MPI_VO_H__

#include "ot_common_vo.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_vo_enable(ot_vo_dev dev);
td_s32 ss_mpi_vo_disable(ot_vo_dev dev);
td_s32 ss_mpi_vo_set_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr);
td_s32 ss_mpi_vo_get_pub_attr(ot_vo_dev dev, ot_vo_pub_attr *pub_attr);
td_s32 ss_mpi_vo_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param);
td_s32 ss_mpi_vo_get_dev_param(ot_vo_dev dev, ot_vo_dev_param *dev_param);
td_s32 ss_mpi_vo_close_fd(td_void);
td_s32 ss_mpi_vo_set_dev_frame_rate(ot_vo_dev dev, td_u32 frame_rate);
td_s32 ss_mpi_vo_get_dev_frame_rate(ot_vo_dev dev, td_u32 *frame_rate);
td_s32 ss_mpi_vo_set_user_sync_info(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info);
td_s32 ss_mpi_vo_get_user_sync_info(ot_vo_dev dev, ot_vo_user_sync_info *sync_info);
td_s32 ss_mpi_vo_set_mod_param(const ot_vo_mod_param *mod_param);
td_s32 ss_mpi_vo_get_mod_param(ot_vo_mod_param *mod_param);
td_s32 ss_mpi_vo_set_vtth(ot_vo_dev dev, td_u32 vtth);
td_s32 ss_mpi_vo_get_vtth(ot_vo_dev dev, td_u32 *vtth);
td_s32 ss_mpi_vo_set_less_buf_attr(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr);
td_s32 ss_mpi_vo_get_less_buf_attr(ot_vo_dev dev, ot_vo_less_buf_attr *less_buf_attr);
td_s32 ss_mpi_vo_set_user_notify_attr(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr);
td_s32 ss_mpi_vo_get_user_notify_attr(ot_vo_dev dev, ot_vo_user_notify_attr *user_notify_attr);

td_s32 ss_mpi_vo_query_intf_status(ot_vo_intf_type intf_type, ot_vo_intf_status *status);
td_s32 ss_mpi_vo_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param);
td_s32 ss_mpi_vo_get_vga_param(ot_vo_dev dev, ot_vo_vga_param *vga_param);
td_s32 ss_mpi_vo_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param);
td_s32 ss_mpi_vo_get_hdmi_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi_param);
td_s32 ss_mpi_vo_set_hdmi1_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi1_param);
td_s32 ss_mpi_vo_get_hdmi1_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi1_param);
td_s32 ss_mpi_vo_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param);
td_s32 ss_mpi_vo_get_rgb_param(ot_vo_dev dev, ot_vo_rgb_param *rgb_param);
td_s32 ss_mpi_vo_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param);
td_s32 ss_mpi_vo_get_bt_param(ot_vo_dev dev, ot_vo_bt_param *bt_param);
td_s32 ss_mpi_vo_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param);
td_s32 ss_mpi_vo_get_mipi_param(ot_vo_dev dev, ot_vo_mipi_param *mipi_param);

td_s32 ss_mpi_vo_enable_video_layer(ot_vo_layer layer);
td_s32 ss_mpi_vo_disable_video_layer(ot_vo_layer layer);
td_s32 ss_mpi_vo_set_video_layer_attr(ot_vo_layer layer, const ot_vo_video_layer_attr *layer_attr);
td_s32 ss_mpi_vo_get_video_layer_attr(ot_vo_layer layer, ot_vo_video_layer_attr *layer_attr);
td_s32 ss_mpi_vo_set_video_layer_param(ot_vo_layer layer, const ot_vo_layer_param *layer_param);
td_s32 ss_mpi_vo_get_video_layer_param(ot_vo_layer layer, ot_vo_layer_param *layer_param);
td_s32 ss_mpi_vo_set_video_layer_crop(ot_vo_layer layer, const ot_crop_info *crop_info);
td_s32 ss_mpi_vo_get_video_layer_crop(ot_vo_layer layer, ot_crop_info *crop_info);
td_s32 ss_mpi_vo_set_video_layer_early_display(ot_vo_layer layer, td_bool early_display_en);
td_s32 ss_mpi_vo_get_video_layer_early_display(ot_vo_layer layer, td_bool *early_display_en);
td_s32 ss_mpi_vo_set_play_toleration(ot_vo_layer layer, td_u32 toleration);
td_s32 ss_mpi_vo_get_play_toleration(ot_vo_layer layer, td_u32 *toleration);
td_s32 ss_mpi_vo_batch_begin(ot_vo_layer layer);
td_s32 ss_mpi_vo_batch_end(ot_vo_layer layer);
td_s32 ss_mpi_vo_get_screen_frame(ot_vo_layer layer, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vo_release_screen_frame(ot_vo_layer layer, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vo_set_layer_priority(ot_vo_layer layer, td_u32 priority);
td_s32 ss_mpi_vo_get_layer_priority(ot_vo_layer layer, td_u32 *priority);
td_s32 ss_mpi_vo_set_layer_csc(ot_vo_layer layer, const ot_vo_csc *csc);
td_s32 ss_mpi_vo_get_layer_csc(ot_vo_layer layer, ot_vo_csc *csc);
td_s32 ss_mpi_vo_bind_layer(ot_vo_layer layer, ot_vo_dev dev);
td_s32 ss_mpi_vo_unbind_layer(ot_vo_layer layer, ot_vo_dev dev);

td_s32 ss_mpi_vo_enable_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_disable_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_set_chn_attr(ot_vo_layer layer, ot_vo_chn chn, const ot_vo_chn_attr *chn_attr);
td_s32 ss_mpi_vo_get_chn_attr(ot_vo_layer layer, ot_vo_chn chn, ot_vo_chn_attr *chn_attr);
td_s32 ss_mpi_vo_set_chn_param(ot_vo_layer layer, ot_vo_chn chn, const ot_vo_chn_param *chn_param);
td_s32 ss_mpi_vo_get_chn_param(ot_vo_layer layer, ot_vo_chn chn, ot_vo_chn_param *chn_param);
td_s32 ss_mpi_vo_set_chn_display_pos(ot_vo_layer layer, ot_vo_chn chn, const ot_point *display_pos);
td_s32 ss_mpi_vo_get_chn_display_pos(ot_vo_layer layer, ot_vo_chn chn, ot_point *display_pos);
td_s32 ss_mpi_vo_set_chn_frame_rate(ot_vo_layer layer, ot_vo_chn chn, td_s32 frame_rate);
td_s32 ss_mpi_vo_get_chn_frame_rate(ot_vo_layer layer, ot_vo_chn chn, td_s32 *frame_rate);
td_s32 ss_mpi_vo_get_chn_frame(ot_vo_layer layer, ot_vo_chn chn, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vo_release_chn_frame(ot_vo_layer layer, ot_vo_chn chn, const ot_video_frame_info *frame_info);
td_s32 ss_mpi_vo_pause_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_resume_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_step_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_refresh_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_show_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_hide_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_set_zoom_in_window(ot_vo_layer layer, ot_vo_chn chn, const ot_vo_zoom_attr *zoom_attr);
td_s32 ss_mpi_vo_get_zoom_in_window(ot_vo_layer layer, ot_vo_chn chn, ot_vo_zoom_attr *zoom_attr);
td_s32 ss_mpi_vo_get_chn_pts(ot_vo_layer layer, ot_vo_chn chn, td_u64 *chn_pts);
td_s32 ss_mpi_vo_query_chn_status(ot_vo_layer layer, ot_vo_chn chn, ot_vo_chn_status *status);
td_s32 ss_mpi_vo_send_frame(ot_vo_layer layer, ot_vo_chn chn, const ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vo_clear_chn_buf(ot_vo_layer layer, ot_vo_chn chn, td_bool is_clear_all);
td_s32 ss_mpi_vo_set_chn_border(ot_vo_layer layer, ot_vo_chn chn, const ot_vo_border *border);
td_s32 ss_mpi_vo_get_chn_border(ot_vo_layer layer, ot_vo_chn chn, ot_vo_border *border);
td_s32 ss_mpi_vo_set_chn_recv_threshold(ot_vo_layer layer, ot_vo_chn chn, td_u32 threshold);
td_s32 ss_mpi_vo_get_chn_recv_threshold(ot_vo_layer layer, ot_vo_chn chn, td_u32 *threshold);
td_s32 ss_mpi_vo_set_chn_rotation(ot_vo_layer layer, ot_vo_chn chn, ot_rotation rotation);
td_s32 ss_mpi_vo_get_chn_rotation(ot_vo_layer layer, ot_vo_chn chn, ot_rotation *rotation);
td_s32 ss_mpi_vo_set_chn_mirror(ot_vo_layer layer, ot_vo_chn chn, ot_vo_mirror_mode mirror_mode);
td_s32 ss_mpi_vo_get_chn_mirror(ot_vo_layer layer, ot_vo_chn chn, ot_vo_mirror_mode *mirror_mode);
td_s32 ss_mpi_vo_get_chn_rgn_luma(ot_vo_layer layer, ot_vo_chn chn, td_u32 num, const ot_rect *rgn, td_u64 *luma_data,
    td_s32 milli_sec);

td_s32 ss_mpi_vo_set_wbc_src(ot_vo_wbc wbc, const ot_vo_wbc_src *wbc_src);
td_s32 ss_mpi_vo_get_wbc_src(ot_vo_wbc wbc, ot_vo_wbc_src *wbc_src);
td_s32 ss_mpi_vo_enable_wbc(ot_vo_wbc wbc);
td_s32 ss_mpi_vo_disable_wbc(ot_vo_wbc wbc);
td_s32 ss_mpi_vo_set_wbc_attr(ot_vo_wbc wbc, const ot_vo_wbc_attr *wbc_attr);
td_s32 ss_mpi_vo_get_wbc_attr(ot_vo_wbc wbc, ot_vo_wbc_attr *wbc_attr);
td_s32 ss_mpi_vo_set_wbc_mode(ot_vo_wbc wbc, ot_vo_wbc_mode wbc_mode);
td_s32 ss_mpi_vo_get_wbc_mode(ot_vo_wbc wbc, ot_vo_wbc_mode *wbc_mode);
td_s32 ss_mpi_vo_set_wbc_depth(ot_vo_wbc wbc, td_u32 depth);
td_s32 ss_mpi_vo_get_wbc_depth(ot_vo_wbc wbc, td_u32 *depth);
td_s32 ss_mpi_vo_get_wbc_frame(ot_vo_wbc wbc, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vo_release_wbc_frame(ot_vo_wbc wbc, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vo_set_cas_attr(const ot_vo_cas_attr *cas_attr);
td_s32 ss_mpi_vo_get_cas_attr(ot_vo_cas_attr *cas_attr);
td_s32 ss_mpi_vo_enable_cas_dev(ot_vo_dev cas_dev);
td_s32 ss_mpi_vo_disable_cas_dev(ot_vo_dev cas_dev);
td_s32 ss_mpi_vo_set_cas_pattern(ot_vo_dev cas_dev, td_u32 pattern);
td_s32 ss_mpi_vo_get_cas_pattern(ot_vo_dev cas_dev, td_u32 *pattern);
td_s32 ss_mpi_vo_bind_cas_chn(td_u32 pos, ot_vo_dev cas_dev, ot_vo_chn chn);
td_s32 ss_mpi_vo_unbind_cas_chn(td_u32 pos, ot_vo_dev cas_dev, ot_vo_chn chn);
td_s32 ss_mpi_vo_enable_cas(td_void);
td_s32 ss_mpi_vo_disable_cas(td_void);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_VO_H__ */
