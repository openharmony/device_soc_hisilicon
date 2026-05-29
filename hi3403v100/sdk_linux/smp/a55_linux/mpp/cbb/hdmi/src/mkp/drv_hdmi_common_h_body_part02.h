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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART02_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART02_H_

#define hdmi_unequal_eok_return(ret, err_code)             \
    do {                                                   \
        if ((ret) != EOK) {                                \
            hdmi_err("secure function error:%d\n", (ret)); \
            return (err_code);                             \
        }                                                  \
    } while (0)

#define hdmi_unlock_unequal_eok_return(ret, mutex, err_code) \
    do {                                                     \
        if ((ret) != EOK) {                                  \
            hdmi_err("secure function error:%d\n", (ret));   \
            hdmi_mutex_unlock((mutex));                      \
            return (err_code);                               \
        }                                                    \
    } while (0)

#define hdmi_unequal_eok_return_void(ret)                  \
    do {                                                   \
        if ((ret) != EOK) {                                \
            hdmi_err("secure function error:%d\n", (ret)); \
            return;                                        \
        }                                                  \
    } while (0)

#define hdmi_check_open_return(state)           \
    do {                                        \
        if (!((state) & HDMI_RUN_STATE_OPEN)) { \
            hdmi_warn("device not open\n");     \
            return OT_ERR_HDMI_DEV_NOT_OPEN;    \
        }                                       \
    } while (0)

typedef enum {
    HDMI_DEVICE_ID0,
    HDMI_DEVICE_ID1,
    HDMI_DEVICE_ID_BUTT
} hdmi_device_id;

#ifdef HDMI_SUPPORT_DUAL_CHANNEL
#define HDMI_ID_MAX HDMI_DEVICE_ID_BUTT
#else
#define HDMI_ID_MAX HDMI_DEVICE_ID1
#endif

typedef enum {
    HDMI_THREAD_STATE_IDLE,
    HDMI_THREAD_STATE_RUN,
    HDMI_THREAD_STATE_STOP
} hdmi_thread_state;

typedef enum {
    HDMI_EVENT_HOTPLUG = 0x10,
    HDMI_EVENT_HOTUNPLUG,
    HDMI_EVENT_EDID_FAIL,
    HDMI_EVENT_RSEN_CONNECT,
    HDMI_EVENT_RSEN_DISCONNECT,
    HDMI_EVENT_SCRAMBLE_FAIL,
    HDMI_EVENT_SCRAMBLE_SUCCESS,
    HDMI_EVENT_ZERO_DRMIF_TIMEOUT,
    HDMI_EVENT_SWITCH_TO_HDRMODE_TIMEOUT,
    HDMI_EVENT_BUTT
} hdmi_event;

typedef enum {
    HDMI_DEBUG_BASE_OSD = 8,
    HDMI_DEBUG_BASE_DEC = 10,
    HDMI_DEBUG_BASE_HEX = 16
} hdmi_debug_base;

typedef td_s32 (*hdmi_callback)(td_void *, hdmi_event);

typedef struct {
    hdmi_device_id hdmi_id;
    td_char *argv[DEBUG_MAX_ARGV_NUM];
    td_u32 argc;
    td_u32 remain_len;
} hdmi_debug_cmd_arg;

typedef td_s32 (*cmd_func)(const hdmi_debug_cmd_arg *cmd_arg);

typedef struct {
    td_char *name;
    td_char *short_name;
    cmd_func fn_cmd_func;
    td_char *comment_help;
} hdmi_debug_cmd_info;

typedef struct {
    td_u32 ddc_reg_cfg;
    td_u32 approximate_value;
    td_char *read_value;
} hdmi_ddc_freq;

typedef struct {
    td_bool data_valid;
    td_s32 len;
    td_u8 data[HDMI_EDID_TOTAL_SIZE];
} hdmi_debug_edid;

typedef struct {
    td_void *event_data;
    hdmi_callback event_callback;
    td_u32 hdmi_dev_id;
    td_char *base_addr;
    td_char *phy_addr;
} hdmi_hal_init;

typedef struct {
    td_u32 cmd;
    td_s32 (*hdmi_ioctrl_func)(td_void *arg, td_bool user);
} hdmi_ioctrl_func;

typedef struct {
    hdmi_colorimetry colorimetry;
    hdmi_quant_range quantization;
    hdmi_pixel_encoding pixel_encoding;
} hdmi_csc_attr;

typedef struct {
    td_u8  edid_valid;
    td_u32 edid_len;
    td_u8  edid[HDMI_EDID_SIZE];
} hdmi_edid_raw_data;

typedef enum {
    HDMI_HDCP_VERSION_NONE,
    HDMI_HDCP_VERSION_1_4,
    HDMI_HDCP_VERSION_2_2,
    HDMI_HDCP_VERSION_BUTT
} hdmi_hdcp_version;

typedef struct {
    td_bool connected;
    td_bool sink_power_on;
    td_bool authed;
    td_u8 bksv[HDMI_BKSV_LEN];
    hdmi_hdcp_version hdcp_version;
} hdmi_status;

typedef struct {
    td_u32 i_de_main_clk;
    td_u32 i_de_main_data;
    td_u32 i_main_clk;
    td_u32 i_main_data;
    td_u32 ft_cap_clk;
    td_u32 ft_cap_data;
} hdmi_hw_param;

typedef struct {
    hdmi_hw_param hw_param[HDMI_HW_PARAM_LEN];
} hdmi_hw_spec;

typedef struct {
    hdmi_hw_spec hwspec_user;
    hdmi_hw_spec hwspec_def;
    hdmi_hw_param hwparam_cur;
} hdmi_hwspec;

typedef enum {
    HDMI_DEEP_COLOR_24BIT,
    HDMI_DEEP_COLOR_30BIT,
    HDMI_DEEP_COLOR_36BIT,
    HDMI_DEEP_COLOR_48BIT,
    HDMI_DEEP_COLOR_OFF = 0xff,
    HDMI_DEEP_COLOR_BUTT
} hdmi_deep_color;

typedef enum {
    HDMI_VIDEO_BITDEPTH_8,
    HDMI_VIDEO_BITDEPTH_10,
    HDMI_VIDEO_BITDEPTH_12,
    HDMI_VIDEO_BITDEPTH_16,
    HDMI_VIDEO_BITDEPTH_OFF,
    HDMI_VIDEO_BITDEPTH_BUTT
} hdmi_video_bit_depth;

typedef enum {
    HDMI_HV_SYNC_POL_HPVP,
    HDMI_HV_SYNC_POL_HPVN,
    HDMI_HV_SYNC_POL_HNVP,
    HDMI_HV_SYNC_POL_HNVN,
    HDMI_HV_SYNC_POL_BUTT
} hdmi_hvsync_polarity;

typedef enum {
    HDMI_PICTURE_NON_UNIFORM_SCALING,
    HDMI_PICTURE_SCALING_H,
    HDMI_PICTURE_SCALING_V,
    HDMI_PICTURE_SCALING_HV
} hdmi_picture_scaling;

typedef struct {
    td_u32 clk_fs; /* VDP setting(in) */
    td_u32 tmds_clk;
    td_u32 hdmi_adapt_pix_clk; /* HDMI adapt setting(out) */
    td_u32 pixel_repeat;
    td_bool v_sync_pol;
    td_bool h_sync_pol;
    td_bool de_pol;
    hdmi_video_timing video_timing;
    hdmi_3d_mode stereo_mode;
    hdmi_colorspace in_color_space;
    hdmi_colormetry colorimetry;
    hdmi_extended_colormetry extended_colorimetry;
    hdmi_quantization_range rgb_quantization;
    hdmi_ycc_quantization_range ycc_quantization;
    hdmi_picture_aspect picture_aspect;
    hdmi_active_aspect active_aspect;
    hdmi_picture_scaling picture_scaling;
    hdmi_video_bit_depth in_bit_depth;
    hdmi_disp_format disp_fmt;
} hdmi_vo_attr;

typedef enum {
    HDMI_AUDIO_FORMAT_2CH = 0x2,
    HDMI_AUDIO_FORMAT_3CH,
    HDMI_AUDIO_FORMAT_4CH,
    HDMI_AUDIO_FORMAT_5CH,
    HDMI_AUDIO_FORMAT_6CH,
    HDMI_AUDIO_FORMAT_7CH,
    HDMI_AUDIO_FORMAT_8CH,
    HDMI_AUDIO_FORMAT_BUTT
} hdmi_audio_ch;

typedef enum {
    HDMI_AUDIO_INTF_I2S,
    HDMI_AUDIO_INTF_SPDIF,
    HDMI_AUDIO_INTF_HBRA,
    HDMI_AUDIO_INTF_BUTT
} hdmi_audio_interface;

typedef enum {
    HDMI_AUDIO_BIT_DEPTH_UNKNOWN,
    HDMI_AUDIO_BIT_DEPTH_8  = 8,
    HDMI_AUDIO_BIT_DEPTH_16 = 16,
    HDMI_AUDIO_BIT_DEPTH_18 = 18,
    HDMI_AUDIO_BIT_DEPTH_20 = 20,
    HDMI_AUDIO_BIT_DEPTH_24 = 24,
    HDMI_AUDIO_BIT_DEPTH_32 = 32,
    HDMI_AUDIO_BIT_DEPTH_BUTT
} hdmi_audio_bit_depth;

typedef enum {
    HDMI_SAMPLE_RATE_UNKNOWN,
    HDMI_SAMPLE_RATE_8K   = 8000,
    HDMI_SAMPLE_RATE_11K  = 11025,
    HDMI_SAMPLE_RATE_12K  = 12000,
    HDMI_SAMPLE_RATE_16K  = 16000,
    HDMI_SAMPLE_RATE_22K  = 22050,
    HDMI_SAMPLE_RATE_24K  = 24000,
    HDMI_SAMPLE_RATE_32K  = 32000,
    HDMI_SAMPLE_RATE_44K  = 44100,
    HDMI_SAMPLE_RATE_48K  = 48000,
    HDMI_SAMPLE_RATE_88K  = 88200,
    HDMI_SAMPLE_RATE_96K  = 96000,
    HDMI_SAMPLE_RATE_176K = 176400,
    HDMI_SAMPLE_RATE_192K = 192000,
    HDMI_SAMPLE_RATE_768K = 768000,
    HDMI_SAMPLE_RATE_BUTT
} hdmi_sample_rate;

typedef struct {
    td_bool down_sample;
    hdmi_sample_rate sample_fs;
    hdmi_audio_ch channels;
    hdmi_audio_interface sound_intf;
    hdmi_audio_bit_depth sample_depth;
    hdmi_audio_format_code audio_code;
} hdmi_ao_attr;

typedef enum {
    HDMI_TMDS_MODE_NONE,
    HDMI_TMDS_MODE_DVI,
    HDMI_TMDS_MODE_HDMI_1_4,
    HDMI_TMDS_MODE_HDMI_2_0,
    HDMI_TMDS_MODE_AUTO,
    HDMI_TMDS_MODE_HDMI_2_1,
    HDMI_TMDS_MODE_BUTT
} hdmi_tmds_mode;

typedef enum {
    HDMI_HDCP_MODE_AUTO,
    HDMI_HDCP_MODE_1_4,
    HDMI_HDCP_MODE_2_2,
    HDMI_HDCP_MODE_BUTT
} hdmi_hdcp_mode;

typedef enum {
    HDMI_DEFAULT_ACTION_NULL,
    HDMI_DEFAULT_ACTION_HDMI,
    HDMI_DEFAULT_ACTION_DVI,
    HDMI_DEFAULT_ACTION_BUTT
} hdmi_default_action;

typedef enum {
    HDMI_VIDEO_DITHER_12_10,
    HDMI_VIDEO_DITHER_12_8,
    HDMI_VIDEO_DITHER_10_8,
    HDMI_VIDEO_DITHER_DISALBE
} hdmi_video_dither;

typedef struct {
    td_bool enable_hdmi;
    td_bool enable_video;
    td_bool enable_audio;
    hdmi_colorspace out_color_space;
    hdmi_quantization_range out_csc_quantization;
    hdmi_deep_color deep_color_mode;
    td_bool xvycc_mode;
    td_bool enable_avi_infoframe;
    td_bool enable_spd_infoframe;
    td_bool enable_mpeg_infoframe;
    td_bool enable_aud_infoframe;
    td_u32  debug_flag;
    td_bool hdcp_enable;
    hdmi_default_action hdmi_action;
    td_bool enable_clr_space_adapt;
    td_bool enable_deep_clr_adapt;
    td_bool auth_mode;
    hdmi_hdcp_mode hdcp_mode;
} hdmi_app_attr;

typedef struct {
    td_bool enable_hdmi;
    td_bool enable_video;
    hdmi_disp_format disp_fmt;
    hdmi_video_timing video_timing;
    td_u32 pix_clk;
    hdmi_colorspace in_color_space;
    hdmi_colorspace out_color_space;
    hdmi_deep_color deep_color_mode;
    hdmi_quantization_range out_csc_quantization;
    td_bool enable_audio;
    hdmi_sample_rate sample_rate;
    hdmi_audio_bit_depth bit_depth;
    td_bool enable_avi_infoframe;
    td_bool enable_aud_infoframe;
    hdmi_default_action hdmi_action;
    td_bool enable_vid_mode_adapt;
    td_bool enable_deep_clr_adapt;
    td_bool auth_mode;
} hdmi_property;

typedef struct {
    hdmi_ao_attr ao_attr;
    hdmi_vo_attr vo_attr;
    hdmi_app_attr app_attr;
} hdmi_attr;

typedef enum {
    HDMI_TRANSITION_NONE,
    HDMI_TRANSITION_BOOT_MCE,
    HDMI_TRANSITION_MCE_APP,
    HDMI_TRANSITION_BOOT_APP = 0x4
} hdmi_transition_state;

typedef enum {
    HDMI_RUN_STATE_NONE,
    HDMI_RUN_STATE_INIT,
    HDMI_RUN_STATE_OPEN,
    HDMI_RUN_STATE_START = 0x4,
    HDMI_RUN_STATE_STOP = 0x8,
    HDMI_RUN_STATE_CLOSE = 0x10,
    HDMI_RUN_STATE_DEINIT = 0x20
} hdmi_run_state;

typedef struct {
    td_u16 length;
    td_u8 *list;
    td_u8 *list_start;
} hdmi_hdcp_ksv_list;

typedef struct {
    td_bool tx_hdmi_14;
    td_bool tx_hdmi_20;
    td_bool tx_hdmi_21;
    td_bool tx_hdcp_14;
    td_bool tx_hdcp_22;
    td_bool tx_rgb444;
    td_bool tx_ycbcr444;
    td_bool tx_ycbcr422;
    td_bool tx_ycbcr420;
    td_bool tx_deep_clr10_bit;
    td_bool tx_deep_clr12_bit;
    td_bool tx_deep_clr16_bit;
    td_bool tx_rgb_ycbcr444;
    td_bool tx_ycbcr444_422;
    td_bool tx_ycbcr422_420;
    td_bool tx_ycbcr420_422;
    td_bool tx_ycbcr422_444;
    td_bool tx_ycbcr444_rgb;
    td_bool tx_scdc;
    td_u32  tx_max_tmds_clk;
    td_u32  tx_max_frl_rate;
} hdmi_tx_capability_data;

typedef enum {
    HDMI_CONV_STD_BT_709,
    HDMI_CONV_STD_BT_601,
    HDMI_CONV_STD_BT_2020_NON_CONST_LUMINOUS,
    HDMI_CONV_STD_BT_2020_CONST_LUMINOUS,
    HDMI_CONV_STD_BUTT
} hdmi_conversion_stb;

typedef struct {
    hdmi_video_timing timing;
    td_u32 pixel_clk;
    td_u32 tmds_clk;
    td_bool v_sync_pol;
    td_bool h_sync_pol;
    td_bool de_pol;
    hdmi_conversion_stb conv_std;
    hdmi_quantization_range quantization;
    hdmi_colorspace in_color_space;
    hdmi_colorspace out_color_space;
    hdmi_deep_color deep_color;
    hdmi_video_bit_depth in_bit_depth;
    hdmi_quantization_range out_csc_quantization;
    td_bool emi_enable;
} hdmi_video_config;

typedef struct {
    td_bool enable_audio;
    td_bool down_sample;
    td_u32 tmds_clk;
    td_u32 pixel_repeat;
    hdmi_sample_rate sample_fs;
    hdmi_audio_ch layout;
    hdmi_audio_interface sound_intf;


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART02_H_ */
