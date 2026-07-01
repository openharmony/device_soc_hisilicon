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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART03_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART03_H_

    hdmi_audio_bit_depth sample_depth;
} hdmi_audio_config;

typedef enum {
    HDMI_FRL_MODE_TMDS,
    HDMI_FRL_MODE_FRL,
    HDMI_FRL_MODE_BUTT
} hdmi_frl_mode;

typedef struct {
    hdmi_sample_rate sample_rate;
    hdmi_frl_mode hdmi_mode;
    td_u8 frl_rate;
    td_u32 pixel_clk;
} hdmi_audio_ncts;

typedef struct {
    td_bool phy_oe;
    td_bool phy_power_on;
    hdmi_video_bit_depth deep_color;
} hdmi_phy_status;

typedef struct {
    td_bool sw_emi_enable;
    td_bool hw_emi_enable;
    td_bool debug_enable;
} hdmi_emi_status;

typedef struct {
    td_bool video_mute;
    td_bool ycbcr2rgb;
    td_bool rgb2ycbcr;
    td_bool ycbcr444_422;
    td_bool ycbcr422_420;
    td_bool ycbcr420_422;
    td_bool ycbcr422_444;
    td_bool in420_ydemux;
    td_bool out420_ydemux;
    hdmi_video_dither dither;
    td_bool v_sync_pol;
    td_bool h_sync_pol;
    td_bool sync_pol;
    td_bool de_pol;
    td_bool swap_hs_cs;
    hdmi_colorspace in_color_space;
    hdmi_colorspace out_color_space;
    hdmi_video_bit_depth out_bit_depth;
    hdmi_hvsync_polarity hv_sync_pol;
    hdmi_quantization_range out_csc_quantization;
    /* detect timing */
    td_bool sync_sw_enable;
    td_bool vsync_polarity; /* when sync_sw_enable==0,indicates hw;or ,indicates sw */
    td_bool hsync_polarity; /* when sync_sw_enable==0,indicates hw;or ,indicates sw */
    td_bool progressive;
    td_u32  hsync_total;
    td_u32  hactive_cnt;
    td_u32  vsync_total;
    td_u32  vactive_cnt;
} hdmi_video_status;

typedef struct {
    td_bool hdcp14_support;
    td_bool hdcp22_support;
} hdmi_hdcp_capability;

typedef struct {
    td_bool audio_mute;
    td_bool audio_enable;
    td_bool down_sample;
    hdmi_sample_rate sample_fs;
    hdmi_audio_ch layout;
    hdmi_audio_interface sound_intf;
    hdmi_audio_bit_depth sample_depth;
    td_u32 ref_n;
    td_u32 reg_n;
    td_u32 ref_cts;
    td_u32 reg_cts;
} hdmi_audio_status;

typedef struct {
    td_bool hotplug;
    td_bool rsen;
    td_bool avmute;
    hdmi_tmds_mode tmds_mode;
} hdmi_common_status;

typedef struct {
    td_bool source_scramble_on;
    td_bool sink_scramble_on;
    td_u8   tmds_bit_clk_ratio;
    td_bool sink_read_quest;
    /* in unit of ms.for [0,200], force to default 200; or, set the value cfg(>200). */
    td_u32 scramble_timeout;
    /* in unit of ms, range[20,200). for [0,20] or >=200, force to default 20; or, set the value cfg[20,200). */
    td_u32 scramble_interval;
} hdmi_scdc_status;

typedef struct {
    td_bool avi_enable;
    td_bool audio_enable;
    td_bool vsif_enable;
    td_bool spd_enable;
    td_bool mpeg_enable;
    td_bool gbd_enable;
    td_u8   avi[HDMI_INFO_FRAME_MAX_SIZE];
    td_u8   audio[HDMI_INFO_FRAME_MAX_SIZE];
    td_u8   vsif[HDMI_INFO_FRAME_MAX_SIZE];
    td_u8   spd[HDMI_INFO_FRAME_MAX_SIZE];
    td_u8   mpeg[HDMI_INFO_FRAME_MAX_SIZE];
    td_u8   gdb[HDMI_INFO_FRAME_MAX_SIZE];
} hdmi_infoframe_status;

typedef struct {
    td_bool hdcp22_enable;
    td_bool hdcp14_enable;
    td_bool repeater_on;
    td_u8   bksv[HDMI_BKSV_LEN];
    td_u8   aksv[HDMI_AKSV_LEN];
    td_u8   hdcp_status;
} hdmi_hdcp_status;

typedef enum {
    FRL_WORK_MODE_NONE,
    FRL_WORK_MODE_3L3G,
    FRL_WORK_MODE_3L6G,
    FRL_WORK_MODE_4L6G,
    FRL_WORK_MODE_4L8G,
    FRL_WORK_MODE_4L10G,
    FRL_WORK_MODE_4L12G,
    FRL_WORK_MODE_BUTT
} hdmi_work_mode;

typedef struct {
    td_bool frl_start;
    td_bool work_en;
    hdmi_work_mode work_mode;
} hdmi_frl_status;

typedef enum {
    FRL_TXFFE_MODE_0,
    FRL_TXFFE_MODE_1,
    FRL_TXFFE_MODE_2,
    FRL_TXFFE_MODE_3,
    FRL_TXFFE_MODE_BUTT
} hdmi_txfff_mode;

typedef struct {
    hdmi_common_status common_status;
    hdmi_phy_status phy_status;
    hdmi_video_status video_status;
    hdmi_audio_status audio_status;
    hdmi_infoframe_status info_frame_status;
    hdmi_hdcp_status hdcp_status;
    hdmi_hwspec phy_hwspec;
    hdmi_frl_status frl_status;
} hdmi_hardware_status;

typedef struct {
    td_u32  max_tmds_character_rate;
    td_bool scdc_present;
    td_bool rr_capable;
    td_bool lte340_mcsc_scramble;
    td_bool _3d_osd_disparity;
    td_bool dual_view;
    td_bool independent_view;
    td_bool dc30bit420;
    td_bool dc36bit420;
    td_bool dc48bit420;
    td_bool scdc_enable;
} hdmi_scdc_config;

typedef struct {
    td_u32 mute_delay;       /* delay for avmute */
    td_u32 fmt_delay;        /* delay for setformat */
    td_bool force_fmt_delay;  /* force setformat delay mode */
    td_bool force_mute_delay; /* force avmute delay mode */
} hdmi_delay;

typedef enum {
    HDMI_VIDEO_UNKNOWN,
    HDMI_VIDEO_PROGRESSIVE,
    HDMI_VIDEO_INTERLACE,
    HDMI_VIDEO_BUTT
} hdmi_video_format_type;

typedef struct {
    hdmi_video_code_vic video_code;
    td_u32 pixclk;
    td_u32 rate;
    td_u32 hactive;
    td_u32 vactive;
    td_u32 hblank;
    td_u32 vblank;
    td_u32 hfront;
    td_u32 hsync;
    td_u32 hback;
    td_u32 vfront;
    td_u32 vsync;
    td_u32 vback;
    hdmi_picture_aspect aspect_ratio;
    hdmi_video_timing timing;
    hdmi_video_format_type pi_type;
    td_char *fmt_str;
} hdmi_video_def;

typedef struct {
    hdmi_vsif_vic hdmi_vic;
    hdmi_video_code_vic equivalent_video_code;
    td_u32 pixclk;
    td_u32 rate;
    td_u32 hactive;
    td_u32 vactive;
    hdmi_picture_aspect aspect_ratio;
    hdmi_video_timing timing;
    hdmi_video_format_type pi_type;
    td_char *fmt_str;
} hdmi_video_4k_def;

typedef struct {
    td_u32 attach_in_time;
    td_u32 attach_out_time;
    td_u32 de_attach_in_time;
    td_u32 de_attach_out_time;
    td_u32 preformat_in_time;
    td_u32 preformat_out_time;
    td_u32 setformat_in_time;
    td_u32 setformat_out_time;
    td_u32 suspend_in_time;
    td_u32 suspend_out_time;
    td_u32 resume_in_time;
    td_u32 resume_out_time;
    td_u32 event_thread_cycle_time;
} hdmi_intf_status;

typedef struct {
    td_bool black_enable;
    td_u8 in_color_space;
    td_u8 in_bit_depth;
    td_u8 in_quantization;
} hdmi_black_frame_info;

typedef struct {
    td_u32 stop_delay;
    hdmi_intf_status intf_status;
} hdmi_debug;

typedef enum {
    HDMI_TIMER_ZERO_DRMIF,
    HDMI_TIMER_SDR_TO_HDR10,
    HDMI_TIMER_TYPE_BUTT
} hdmi_timer_type;

typedef struct {
    td_bool mute_pkg_en;
    td_bool mute_set;
    td_bool mute_clr;
    td_bool mute_rpt_en;
    td_u32  rpt_cnt;
} hdmi_avmute_cfg;

typedef enum {
    HDMI_FRL_SCDC_TYPE_SINK_VERSION,
    HDMI_FRL_SCDC_TYPE_SOURCE_VERSION,
    HDMI_FRL_SCDC_TYPE_UPDATE_FLAGS,
    HDMI_FRL_SCDC_TYPE_STATUS_FLAGS,
    HDMI_FRL_SCDC_TYPE_CONFIG,
    HDMI_FRL_SCDC_TYPE_TXFFE_REQ,
    HDMI_FRL_FLAGS_TYPE_BUTT
} hdmi_frl_scdc_type;

typedef enum {
    HDMI_FRL_TRAIN_PATTERN_NONE,
    HDMI_FRL_TRAIN_PATTERN_LP1,
    HDMI_FRL_TRAIN_PATTERN_LP2,
    HDMI_FRL_TRAIN_PATTERN_LP3,
    HDMI_FRL_TRAIN_PATTERN_LP4,
    HDMI_FRL_TRAIN_PATTERN_LP5,
    HDMI_FRL_TRAIN_PATTERN_LP6,
    HDMI_FRL_TRAIN_PATTERN_LP7,
    HDMI_FRL_TRAIN_PATTERN_LP8,
    HDMI_FRL_TRAIN_PATTERN_RESERVED,
    HDMI_FRL_TRAIN_PATTERN_0E = 0xE,
    HDMI_FRL_TRAIN_PATTERN_0F = 0xF,
    HDMI_FRL_TRAIN_PATTERN_BUTT
} hdmi_frl_train_pattern;

typedef enum {
    HDMI_FRL_TRAIN_NONE,
    HDMI_FRL_TRAIN_FAIL,
    HDMI_FRL_TRAIN_SUCCESS,
    HDMI_FRL_TRAIN_BUSY,
    HDMI_FRL_TRAIN_BUTT
} hdmi_frl_train_status;

typedef enum {
    HDMI_FRL_TRAIN_FAIL_NORMAL,
    HDMI_FRL_TRAIN_FAIL_FLTTIMEOUT,
    HDMI_FRL_TRAIN_FAIL_FLTSTEPTIMEOUT,
    HDMI_FRL_TRAIN_FAIL_RATECHANGE,
    HDMI_FRL_TRAIN_FAIL_FFECHANGE,
    HDMI_FRL_TRAIN_FAIL_BUTT
} hdmi_frl_train_fail_code;

typedef enum {
    HDMI_FRL_TRAIN_SEL_SW,
    HDMI_FRL_TRAIN_SEL_HW,
    HDMI_FRL_TRAIN_SEL_BUTT
} hdmi_frl_train_sel;

typedef enum {
    FRL_DEBUG_RATE,
    FRL_DEBUG_SW_TRAIN_SEL,
    FRL_DEBUG_LTP_PATTERN,
    FRL_DEBUG_SELECT_CHANNEL,
    FRL_DEBUG_LTS3_INTERVAL,
    FRL_DEBUG_LTS3_TIMEOUT,
    FRL_DEBUG_TRAINING_BREAK,
    FRL_DEBUG_LM_TABLE_GET,
    FRL_DEBUG_CTRL_TYPE_CONFIG,
    FRL_DEBUG_BUTT
} frl_debug_cmd;

typedef enum {
    FRL_SW_TRAIN_DELAY,
    FRL_SW_TRAIN_TIMER,
    FRL_SW_TRAIN_BUTT
} frl_sw_train_mode;

typedef enum {
    FRL_CHL_SEL_NORMAL,
    FRL_CHL_SEL_RX_TMDS,
    FRL_CHL_SEL_BUTT
} frl_channel_sel;

typedef struct {
    hdmi_frl_train_status frl_train_status;
    hdmi_frl_train_pattern train_pattern[HDMI_FRL_LANE_MAX_NUM];
    hdmi_frl_train_fail_code train_fail_res;
} hdmi_frl_train;

typedef enum {
    HDMI_FRL_MACH_MODE_STEP,
    HDMI_FRL_MACH_MODE_TIMEOUT,
    HDMI_FRL_MACH_MODE_BUTT
} hdmi_frl_mach_mode;

typedef struct {
    td_bool frl_no_timeout;
    td_u8 frl_rate;
    td_u8 ffe_levels;
    td_u32 train_timeout;
    hdmi_frl_mach_mode mach_mode;
    frl_sw_train_mode sw_train_mode;
    td_u8 ctl_type_config;
} hdmi_frl_train_config;

typedef struct {
    frl_debug_cmd debug_cmd;
    td_u8 rate;
    td_u8 ltp;
    td_u8 lane_idx;
    td_u8 training_break;
    frl_sw_train_mode sw_train_mode;
    frl_channel_sel channel_sel;
    td_u32 lts3_interval;
    td_u32 lts3_timeout;
    td_u8 crtl_type_config;
    td_u8 avi_send_by_gen5;
} frl_debug;

typedef enum {
    SCDC_CMD_SET_SOURCE_VER,
    SCDC_CMD_GET_SOURCE_VER,
    SCDC_CMD_GET_SINK_VER,
    SCDC_CMD_SET_FLT_UPDATE,
    SCDC_CMD_GET_FLT_UPDATE,
    SCDC_CMD_SET_FLT_UPDATE_TRIM,
    SCDC_CMD_GET_FLT_UPDATE_TRIM,
    SCDC_CMD_SET_FRL_START,
    SCDC_CMD_GET_FRL_START,
    SCDC_CMD_SET_CONFIG1,
    SCDC_CMD_GET_CONFIG1,
    SCDC_CMD_GET_TEST_CONFIG,
    SCDC_CMD_GET_FLT_READY,
    SCDC_CMD_GET_LTP_REQ,
    SCDC_CMD_BUTT
} scdc_cmd;

typedef struct {
    td_u8 frl_rate;
    td_u8 ffe_levels;
} scdc_config1;

typedef struct {
    td_bool pre_shoot_only;
    td_bool de_emphasis_only;
    td_bool no_ffe;
    td_bool flt_no_timeout;
    td_bool dsc_frl_max;
    td_bool frl_max;
} scdc_test_config;

typedef struct {
    td_u8 ln0_ltp;
    td_u8 ln1_ltp;
    td_u8 ln2_ltp;
    td_u8 ln3_ltp;
} scdc_ltp_req;

typedef enum {
    HDMI_PHY_MODE_CFG_TMDS,
    HDMI_PHY_MODE_CFG_FRL,
    HDMI_PHY_MODE_CFG_TXFFE
} hdmi_phy_mode_cfg;

typedef enum {
    HDMI_TRACE_LEN_0, /* 1.0 inch */
    HDMI_TRACE_LEN_1, /* 1.5 inch */
    HDMI_TRACE_LEN_2, /* 2.0 inch */
    HDMI_TRACE_LEN_3, /* 2.5 inch */
    HDMI_TRACE_LEN_4, /* 3.0 inch */
    HDMI_TRACE_LEN_5, /* 3.5 inch */
    HDMI_TRACE_LEN_6, /* 4.0 inch */
    HDMI_TRACE_LEN_7, /* 4.5 inch */
    HDMI_TRACE_LEN_8, /* 5.0 inch */
    HDMI_TRACE_DEFAULT, /* default config */
    HDMI_TRACE_BUTT
} hdmi_trace_len;

typedef struct {


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART03_H_ */
