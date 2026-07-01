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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_H_BODY_PART02_H_


    td_u32 dis_buf_len;
    ot_dynamic_range dst_dynamic_range;

    /* for channel */
    sample_vo_mode vo_mode;

    /* for user sync */
    ot_vo_sync_info sync_info;
    ot_vo_user_sync_info user_sync;
    td_u32 dev_frame_rate;
} sample_vo_cfg;

#if VO_MIPI_SUPPORT

typedef enum {
    OT_MIPI_TX_OUT_576P50       = OT_VO_OUT_576P50,
    OT_MIPI_TX_OUT_1024X768_60  = OT_VO_OUT_1024x768_60,
    OT_MIPI_TX_OUT_720P50       = OT_VO_OUT_720P50,
    OT_MIPI_TX_OUT_720P60       = OT_VO_OUT_720P60,
    OT_MIPI_TX_OUT_1280X1024_60 = OT_VO_OUT_1280x1024_60,
    OT_MIPI_TX_OUT_1080P24      = OT_VO_OUT_1080P24,
    OT_MIPI_TX_OUT_1080P25      = OT_VO_OUT_1080P25,
    OT_MIPI_TX_OUT_1080P30      = OT_VO_OUT_1080P30,
    OT_MIPI_TX_OUT_1080P50      = OT_VO_OUT_1080P50,
    OT_MIPI_TX_OUT_1080P60      = OT_VO_OUT_1080P60,
    OT_MIPI_TX_OUT_3840X2160_24 = OT_VO_OUT_3840x2160_24,
    OT_MIPI_TX_OUT_3840X2160_25 = OT_VO_OUT_3840x2160_25,
    OT_MIPI_TX_OUT_3840X2160_30 = OT_VO_OUT_3840x2160_30,
    OT_MIPI_TX_OUT_3840X2160_50 = OT_VO_OUT_3840x2160_50,
    OT_MIPI_TX_OUT_3840X2160_60 = OT_VO_OUT_3840x2160_60,

    OT_MIPI_TX_OUT_720X1280_60  = OT_VO_OUT_720x1280_60,
    OT_MIPI_TX_OUT_1080X1920_60 = OT_VO_OUT_1080x1920_60,

    OT_MIPI_TX_OUT_USER = OT_VO_OUT_USER,

    OT_MIPI_TX_OUT_BUTT = OT_VO_OUT_BUTT,
} mipi_tx_intf_sync;

typedef struct {
    cmd_info_t cmd_info;
    td_u32 usleep_value;
}mipi_tx_cmd_info;

typedef struct {
    /* for combo dev config */
    mipi_tx_intf_sync intf_sync;

    /* for screen cmd */
    td_u32 cmd_count;
    mipi_tx_cmd_info *cmd_info;

    /* for user sync */
    combo_dev_cfg_t combo_dev_cfg;
} sample_mipi_tx_config;

typedef struct {
    sample_vo_cfg vo_config;
    sample_mipi_tx_config tx_config;
} sample_vo_mipi_tx_cfg;

#endif

typedef enum {
    THREAD_CTRL_START,
    THREAD_CTRL_PAUSE,
    THREAD_CTRL_STOP,
} thread_contrl;

typedef struct {
    ot_pic_size pic_size;
    ot_vo_intf_sync intf_sync;
    ot_vo_intf_type intf_type;
} vdec_display_cfg;

typedef struct {
    td_s32 chn_id;
    ot_payload_type type;
    td_char c_file_path[FILE_PATH_LEN];
    td_char c_file_name[FILE_NAME_LEN];
    td_s32 stream_mode;
    td_s32 milli_sec;
    td_s32 min_buf_size;
    td_s32 interval_time;
    thread_contrl e_thread_ctrl;
    td_u64 pts_init;
    td_u64 pts_increase;
    td_bool circle_send;
    td_u64 last_time;
    td_u64 time_gap;
    td_u64 fps;
} vdec_thread_param;

typedef struct {
    td_u32 pic_buf_size;
    td_u32 tmv_buf_size;
    td_bool pic_buf_alloc;
    td_bool tmv_buf_alloc;
} sample_vdec_buf;

typedef struct {
    ot_video_dec_mode dec_mode;
    td_u32 ref_frame_num;
    ot_data_bit_width bit_width;
} sample_vdec_video_attr;

typedef struct {
    ot_pixel_format pixel_format;
    td_u32 alpha;
} sample_vdec_pic_attr;

typedef struct {
    ot_payload_type type;
    ot_vdec_send_mode mode;
    td_u32 width;
    td_u32 height;
    td_u32 frame_buf_cnt;
    td_u32 display_frame_num;
    union {
        sample_vdec_video_attr sample_vdec_video; /* structure with video (h265/h264) */
        sample_vdec_pic_attr sample_vdec_picture; /* structure with picture (jpeg/mjpeg) */
    };
} sample_vdec_attr;

typedef struct {
    ot_video_format video_format;
    ot_pixel_format pixel_format;
    td_u32 width;
    td_u32 height;
    td_u32 align;
    ot_compress_mode compress_mode;
} sample_vb_base_info;

typedef struct {
    td_u32 vb_size;
    td_u32 head_stride;
    td_u32 head_size;
    td_u32 head_y_size;
    td_u32 main_stride;
    td_u32 main_size;
    td_u32 main_y_size;
    td_u32 ext_stride;
    td_u32 ext_y_size;
} sample_vb_cal_config;

typedef struct {
    td_u32 frame_rate;
    td_u32 stats_time;
    td_u32 gop;
    ot_size venc_size;
    ot_pic_size size;
    td_u32 profile;
    td_bool is_rcn_ref_share_buf;
    ot_venc_gop_attr gop_attr;
    ot_payload_type type;
    sample_rc rc_mode;
} sample_comm_venc_chn_param;

typedef struct {
    ot_vpss_chn *vpss_chn;
    ot_venc_chn *venc_chn;
    td_s32 cnt;
} sample_venc_roimap_chn_info;

typedef struct {
    ot_audio_sample_rate out_sample_rate;
    td_bool resample_en;
    td_void *ai_vqe_attr;
    td_u32 ai_vqe_type;
} sample_comm_ai_vqe_param;

/* function announce */
#ifndef __LITEOS__
td_void sample_sys_signal(void (*func)(int));
#endif

td_s32 sample_comm_sys_get_pic_size(ot_pic_size pic_size, ot_size *size);
ot_pic_size sample_comm_sys_get_pic_enum(const ot_size *size);
td_s32 sample_comm_sys_mem_config(td_void);
td_void sample_comm_sys_exit(td_void);
td_s32 sample_comm_sys_init(const ot_vb_cfg *vb_cfg);
td_s32 sample_comm_sys_init_with_vb_supplement(const ot_vb_cfg *vb_cfg, td_u32 supplement_config);

td_s32 sample_comm_vi_bind_vo(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vo_layer vo_layer, ot_vo_chn vo_chn);
td_s32 sample_comm_vi_un_bind_vo(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vo_layer vo_layer, ot_vo_chn vo_chn);
td_s32 sample_comm_vi_bind_vpss(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn);
td_s32 sample_comm_vi_un_bind_vpss(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn);
td_s32 sample_comm_vi_bind_venc(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_venc_chn venc_chn);
td_s32 sample_comm_vi_un_bind_venc(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_venc_chn venc_chn);
td_s32 sample_comm_avs_bind_venc(ot_avs_grp avs_grp, ot_avs_chn avs_chn, ot_venc_chn venc_chn);
td_s32 sample_comm_avs_un_bind_venc(ot_avs_grp avs_grp, ot_avs_chn avs_chn, ot_venc_chn venc_chn);
td_s32 sample_comm_avs_bind_vo(ot_avs_grp avs_grp, ot_avs_chn avs_chn, ot_vo_layer vo_layer, ot_vo_chn vo_chn);
td_s32 sample_comm_avs_un_bind_vo(ot_avs_grp avs_grp, ot_avs_chn avs_chn, ot_vo_layer vo_layer, ot_vo_chn vo_chn);

td_s32 sample_comm_vi_switch_isp_mode(const sample_vi_cfg *vi_cfg);
td_s32 sample_comm_vi_switch_isp_resolution(const sample_vi_cfg *vi_cfg, const ot_size *size);
td_void sample_comm_vi_mode_switch_stop_vi(const sample_vi_cfg *vi_cfg);
td_s32 sample_comm_vi_mode_switch_start_vi(const sample_vi_cfg *vi_cfg, td_bool chg_resolution, const ot_size *size);

td_s32 sample_comm_vpss_bind_vo(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn, ot_vo_layer vo_layer, ot_vo_chn vo_chn);
td_s32 sample_comm_vpss_un_bind_vo(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn, ot_vo_layer vo_layer, ot_vo_chn vo_chn);
td_s32 sample_comm_vpss_bind_avs(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn, ot_avs_grp avs_grp, ot_avs_pipe avs_pipe);
td_s32 sample_comm_vpss_un_bind_avs(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn,
                                    ot_avs_grp avs_grp, ot_avs_pipe avs_pipe);
td_s32 sample_comm_vpss_bind_venc(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn, ot_venc_chn venc_chn);
td_s32 sample_comm_vpss_un_bind_venc(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn, ot_venc_chn venc_chn);

td_s32 sample_comm_vdec_bind_vpss(ot_vdec_chn vdec_chn, ot_vpss_grp vpss_grp);
td_s32 sample_comm_vdec_un_bind_vpss(ot_vdec_chn vdec_chn, ot_vpss_grp vpss_grp);
td_s32 sample_comm_vo_bind_vo(ot_vo_layer src_vo_layer, ot_vo_chn src_vo_chn,
                              ot_vo_layer dst_vo_layer, ot_vo_chn dst_vo_chn);
td_s32 sample_comm_vo_un_bind_vo(ot_vo_layer dst_vo_layer, ot_vo_chn dst_vo_chn);

td_s32 sample_comm_isp_sensor_regiter_callback(ot_isp_dev isp_dev, sample_sns_type sns_type);
td_s32 sample_comm_isp_sensor_unregiter_callback(ot_isp_dev isp_dev);
td_s32 sample_comm_isp_bind_sns(ot_isp_dev isp_dev, sample_sns_type sns_type, td_s8 sns_dev);
td_s32 sample_comm_isp_ae_lib_callback(ot_isp_dev isp_dev);
td_s32 sample_comm_isp_ae_lib_uncallback(ot_isp_dev isp_dev);
td_s32 sample_comm_isp_awb_lib_callback(ot_isp_dev isp_dev);
td_s32 sample_comm_isp_awb_lib_uncallback(ot_isp_dev isp_dev);
td_s32 sample_comm_isp_run(ot_isp_dev isp_dev);
td_void sample_comm_isp_stop(ot_isp_dev isp_dev);
td_void sample_comm_all_isp_stop(td_void);
td_s32 sample_comm_isp_get_pub_attr_by_sns(sample_sns_type sns_type, ot_isp_pub_attr *pub_attr);
ot_isp_sns_type sample_comm_get_sns_bus_type(sample_sns_type sns_type);
td_s32 sample_comm_isp_sensor_founction_cfg(ot_vi_pipe vi_pipe, sample_sns_type sns_type);

td_void sample_comm_vi_get_size_by_sns_type(sample_sns_type sns_type, ot_size *size);
td_void sample_comm_vi_get_default_vi_cfg(sample_sns_type sns_type, sample_vi_cfg *vi_cfg);
td_void sample_comm_vi_init_vi_cfg(sample_sns_type sns_type, ot_size *size, sample_vi_cfg *vi_cfg);

td_s32 sample_comm_vi_set_vi_vpss_mode(ot_vi_vpss_mode_type mode_type, ot_vi_video_mode video_mode);
td_s32 sample_comm_vi_get_isp_run_state(td_bool *isp_states, td_u32 size);
td_s32 sample_comm_vi_start_vi(const sample_vi_cfg *vi_cfg);
td_void sample_comm_vi_stop_vi(const sample_vi_cfg *vi_cfg);
td_void sample_comm_vi_stop_four_vi(const sample_vi_cfg *vi_cfg, td_s32 route_num);
td_s32 sample_comm_vi_fpn_calibrate(ot_vi_pipe vi_pipe, sample_vi_fpn_calibration_cfg *calibration_cfg);
td_s32 sample_comm_vi_enable_fpn_correction(ot_vi_pipe vi_pipe, sample_vi_fpn_correction_cfg *correction_cfg);
td_s32 sample_comm_vi_disable_fpn_correction(ot_vi_pipe vi_pipe, sample_vi_fpn_correction_cfg *correction_cfg);

td_s32 sample_comm_vi_enable_fpn_correction_for_scene(ot_vi_pipe vi_pipe, sample_vi_fpn_correction_cfg *correction_cfg,
    td_u32 iso, sample_scene_fpn_offset_cfg *scene_fpn_offset_cfg, const td_char *dir_name);

td_s32 sample_comm_vi_start_virt_pipe(const sample_vi_cfg *vi_cfg);
td_void sample_comm_vi_stop_virt_pipe(const sample_vi_cfg *vi_cfg);
td_s32 sample_common_vi_load_user_pic(ot_vi_pipe vi_pipe, sample_vi_user_pic_type user_pic_type,
    sample_vi_user_frame_info *user_frame_info);
td_void sample_common_vi_unload_user_pic(sample_vi_user_frame_info *user_frame_info);
td_s32 sample_comm_vi_send_wdr_frame(ot_vi_bind_pipe *bind_pipe);

td_void sample_comm_vpss_get_default_grp_attr(ot_vpss_grp_attr *grp_attr);
td_void sample_comm_vpss_get_default_chn_attr(ot_vpss_chn_attr *chn_attr);
td_s32 sample_common_vpss_start(ot_vpss_grp grp, const td_bool *chn_enable,
    const ot_vpss_grp_attr *grp_attr, const ot_vpss_chn_attr *chn_attr, td_u32 chn_array_size);
td_s32 sample_common_vpss_stop(ot_vpss_grp grp, const td_bool *chn_enable, td_u32 chn_array_size);

td_s32 sample_comm_vo_get_width_height(ot_vo_intf_sync intf_sync, td_u32 *width, td_u32 *height,
    td_u32 *frame_rate);
td_s32 sample_comm_vo_mem_config(ot_vo_dev vo_dev, td_char *pc_mmz_name);
td_s32 sample_comm_vo_start_dev(ot_vo_dev vo_dev,
    const ot_vo_pub_attr *pub_attr,
    const ot_vo_user_sync_info *sync_info,
    td_u32 dev_frame_rate);

td_s32 sample_comm_vo_stop_dev(ot_vo_dev vo_dev);
td_s32 sample_comm_vo_start_layer(ot_vo_layer vo_layer, const ot_vo_video_layer_attr *layer_attr);
td_s32 sample_comm_vo_stop_layer(ot_vo_layer vo_layer);
td_s32 sample_comm_vo_get_wnd_info(sample_vo_mode mode, sample_vo_wnd_info *wnd_info);
td_s32 sample_comm_vo_get_chn_attr(sample_vo_wnd_info *wnd_info, ot_vo_video_layer_attr *layer_attr,
    td_s32 chn, ot_vo_chn_attr *chn_attr);
td_s32 sample_comm_vo_start_chn(ot_vo_layer vo_layer, sample_vo_mode mode);
td_s32 sample_comm_vo_stop_chn(ot_vo_layer vo_layer, sample_vo_mode mode);
td_s32 sample_comm_vo_start_wbc(sample_vo_wbc_cfg *wbc_config);
td_s32 sample_comm_vo_stop_wbc(sample_vo_wbc_cfg *wbc_config);
td_s32 sample_comm_vo_get_def_wbc_config(sample_vo_wbc_cfg *wbc_config);
td_s32 sample_comm_vo_bind_vi(ot_vo_layer vo_layer, ot_vo_chn vo_chn, ot_vi_chn vi_chn);
td_s32 sample_comm_vo_un_bind_vi(ot_vo_layer vo_layer, ot_vo_chn vo_chn);
td_s32 sample_comm_vo_bt1120_start(ot_vo_dev vo_dev, ot_vo_pub_attr *pub_attr);
td_s32 sample_comm_vo_hdmi_start(ot_vo_intf_sync intf_sync);
td_s32 sample_comm_vo_hdmi_stop(td_void);
td_s32 sample_comm_start_mipi_tx(const sample_mipi_tx_config *tx_config);
td_void sample_comm_stop_mipi_tx(ot_vo_intf_type intf_type);
td_s32 sample_comm_vo_get_def_config(sample_vo_cfg *vo_config);
td_s32 sample_comm_vo_stop_vo(const sample_vo_cfg *vo_config);
td_void sample_comm_vo_set_hdmi_rgb_mode(td_bool enable);
td_s32 sample_comm_vo_start_vo(const sample_vo_cfg *vo_config);
td_s32 sample_comm_vo_stop_pip(const sample_vo_cfg *vo_config);
td_s32 sample_comm_vo_start_pip(sample_vo_cfg *vo_config);
td_s32 sample_comm_vo_get_def_layer_config(sample_comm_vo_layer_cfg *vo_layer_config);
td_s32 sample_comm_vo_start_layer_chn(sample_comm_vo_layer_cfg *vo_layer_config);
td_s32 sample_comm_vo_stop_layer_chn(sample_comm_vo_layer_cfg *vo_layer_config);

td_s32 sample_comm_venc_mem_config(td_void);
td_s32 sample_comm_venc_create(ot_venc_chn venc_chn, sample_comm_venc_chn_param *chn_param);
td_s32 sample_comm_venc_start(ot_venc_chn venc_chn, sample_comm_venc_chn_param *chn_param);
td_s32 sample_comm_venc_stop(ot_venc_chn venc_chn);
td_s32 sample_comm_venc_snap_start(ot_venc_chn venc_chn, ot_size *size, td_bool support_dcf);
td_s32 sample_comm_venc_photo_start(ot_venc_chn venc_chn, ot_size *size, td_bool support_dcf);
td_s32 sample_comm_venc_snap_process(ot_venc_chn venc_chn, td_u32 snap_cnt, td_bool save_jpg, td_bool save_thm);
td_s32 sample_comm_venc_save_jpeg(ot_venc_chn venc_chn, td_u32 snap_cnt);
td_s32 sample_comm_venc_snap_stop(ot_venc_chn venc_chn);
td_s32 sample_comm_venc_start_get_stream(ot_venc_chn ve_chn[], td_s32 cnt);
td_s32 sample_comm_venc_stop_get_stream(td_s32 chn_num);
td_s32 sample_comm_venc_start_get_stream_svc_t(td_s32 cnt);
td_s32 sample_comm_venc_get_gop_attr(ot_venc_gop_mode gop_mode, ot_venc_gop_attr *gop_attr);
td_s32 sample_comm_venc_qpmap_send_frame(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn[],
                                         ot_venc_chn venc_chn[], td_s32 cnt, ot_size size[]);
td_s32 sample_comm_venc_stop_send_qpmap_frame(td_void);
td_s32 sample_comm_venc_rateauto_start(ot_venc_chn ve_chn[], td_s32 cnt, ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn[]);
td_s32 sample_comm_venc_stop_rateauto(ot_venc_chn ve_chn[], td_s32 cnt);

td_s32 sample_comm_venc_send_roimap_frame(ot_vpss_grp vpss_grp, sample_venc_roimap_chn_info roimap_chn_info,
    ot_size size[], ot_venc_jpeg_roi_attr roi_attr[]);
td_s32 sample_comm_venc_stop_send_roimap_frame(td_void);
td_s32 sample_comm_venc_save_stream(FILE *fd, ot_venc_stream *stream);

td_s32 sample_comm_region_create(td_s32 handle_num, ot_rgn_type type);
td_s32 sample_comm_region_destroy(td_s32 handle_num, ot_rgn_type type);
td_s32 sample_comm_region_attach_to_chn(td_s32 handle_num, ot_rgn_type type, ot_mpp_chn *mpp_chn);
td_s32 sample_comm_check_min(td_s32 min_handle);
td_s32 sample_comm_region_detach_frm_chn(td_s32 handle_num, ot_rgn_type type, ot_mpp_chn *mpp_chn);
td_s32 sample_comm_region_set_bit_map(ot_rgn_handle handle, const td_char *bmp_path);
td_s32 sample_comm_region_get_up_canvas(ot_rgn_handle handle, const td_char *bmp_path);
td_s32 sample_comm_region_get_min_handle(ot_rgn_type type);

td_s32 sample_comm_audio_creat_trd_ai_ao(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn);
td_s32 sample_comm_audio_creat_trd_ai_aenc(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_aenc_chn ae_chn);
td_s32 sample_comm_audio_creat_trd_aenc_adec(ot_aenc_chn ae_chn, ot_adec_chn ad_chn, FILE *aenc_fd);
td_s32 sample_comm_audio_creat_trd_file_adec(ot_adec_chn ad_chn, FILE *adec_fd);
td_s32 sample_comm_audio_creat_trd_ao_vol_ctrl(ot_audio_dev ao_dev);
td_s32 sample_comm_audio_destory_trd_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn);
td_s32 sample_comm_audio_destory_trd_aenc_adec(ot_aenc_chn ae_chn);
td_s32 sample_comm_audio_destory_trd_file_adec(ot_adec_chn ad_chn);
td_s32 sample_comm_audio_destory_trd_ao_vol_ctrl(ot_audio_dev ao_dev);
td_s32 sample_comm_audio_destory_all_trd(td_void);
td_s32 sample_comm_audio_ao_bind_adec(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_adec_chn ad_chn);
td_s32 sample_comm_audio_ao_unbind_adec(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_adec_chn ad_chn);
td_s32 sample_comm_audio_ao_bind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn);
td_s32 sample_comm_audio_ao_unbind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn);
td_s32 sample_comm_audio_aenc_bind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_aenc_chn ae_chn);
td_s32 sample_comm_audio_aenc_unbind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_aenc_chn ae_chn);
td_s32 sample_comm_audio_start_ai(ot_audio_dev ai_dev_id, td_u32 ai_chn_cnt, ot_aio_attr *aio_attr,
    const sample_comm_ai_vqe_param *ai_vqe_param, ot_audio_dev ao_dev_id);
td_s32 sample_comm_audio_stop_ai(ot_audio_dev ai_dev_id, td_u32 ai_chn_cnt, td_bool resample_en, td_bool vqe_en);
td_s32 sample_comm_audio_start_ao(ot_audio_dev ao_dev_id, td_u32 ao_chn_cnt,
                                  ot_aio_attr *aio_attr, ot_audio_sample_rate in_sample_rate, td_bool resample_en);
td_s32 sample_comm_audio_stop_ao(ot_audio_dev ao_dev_id, td_u32 ao_chn_cnt, td_bool resample_en);
td_s32 sample_comm_audio_start_aenc(td_u32 aenc_chn_cnt, const ot_aio_attr *aio_attr, ot_payload_type type);
td_s32 sample_comm_audio_stop_aenc(td_u32 aenc_chn_cnt);
td_s32 sample_comm_audio_start_adec(td_u32 adec_chn_cnt, ot_payload_type type);
td_s32 sample_comm_audio_stop_adec(ot_adec_chn ad_chn);
td_s32 sample_comm_audio_cfg_acodec(const ot_aio_attr *aio_attr);

td_s32 sample_comm_vdec_init_vb_pool(td_u32 chn_num, sample_vdec_attr *past_sample_vdec, td_u32 arr_len);
td_void sample_comm_vdec_exit_vb_pool(td_void);
td_void sample_comm_vdec_cmd_ctrl(td_s32 chn_num, vdec_thread_param *vdec_send, pthread_t *vdec_thread,
    td_u32 send_arr_len, td_u32 thread_arr_len);
td_void sample_comm_vdec_start_send_stream(td_s32 chn_num, vdec_thread_param *vdec_send, pthread_t *vdec_thread,
    td_u32 send_arr_len, td_u32 thread_arr_len);
td_void sample_comm_vdec_stop_send_stream(td_s32 chn_num, vdec_thread_param *vdec_send, pthread_t *vdec_thread,
    td_u32 send_arr_len, td_u32 thread_arr_len);
td_void *sample_comm_vdec_send_stream(td_void *args);
td_s32 sample_comm_vdec_start(td_s32 chn_num, sample_vdec_attr *past_sample_vdec, td_u32 arr_len);
td_s32 sample_comm_vdec_stop(td_s32 chn_num);
td_void sample_comm_vdec_cmd_not_circle_send(td_u32 chn_num, vdec_thread_param *vdec_send, pthread_t *vdec_thread,
    td_u32 send_arr_len, td_u32 thread_arr_len);
td_void sample_comm_vdec_print_chn_status(td_s32 chn, ot_vdec_chn_status status);
td_bool sample_comm_vdec_get_lowdelay_en(td_void);
td_void sample_comm_vdec_set_lowdelay_en(td_bool enable);

td_s32 sample_comm_venc_get_file_postfix(ot_payload_type payload, td_char *file_postfix, td_u8 len);
td_void sample_comm_vi_get_default_sns_info(sample_sns_type sns_type, sample_sns_info *sns_info);
td_void sample_comm_vi_get_default_pipe_info(sample_sns_type sns_type, ot_vi_bind_pipe *bind_pipe,
                                             sample_vi_pipe_info pipe_info[]);
td_void sample_comm_vi_init_pipe_info(sample_sns_type sns_type, const ot_size *size, ot_vi_bind_pipe *bind_pipe,
                                      sample_vi_pipe_info pipe_info[]);

td_void sample_comm_vi_get_default_mipi_info(sample_sns_type sns_type, sample_mipi_info *mipi_info);
td_void sample_comm_vi_get_default_dev_info(sample_sns_type sns_type, sample_vi_dev_info *dev_info);
td_void sample_comm_vi_get_mipi_info_by_dev_id(sample_sns_type sns_type, ot_vi_dev vi_dev,
                                               sample_mipi_info *mipi_info);
td_void sample_comm_venc_set_save_heif(td_bool save_heif);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_H_BODY_PART02_H_ */
