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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_HDMI_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_HDMI_H_BODY_PART02_H_

    td_u32  hbb;            /* horizontal back blank */
    td_u32  hact;           /* horizontal active area */
    td_u32  vpw;            /* vertical sync pulse width */
    td_u32  hpw;            /* horizontal sync pulse width */
    td_bool idv;            /* flag of data valid signal is needed flip */
    td_bool ihs;            /* flag of horizontal sync pulse is needed flip */
    td_bool ivs;            /* flag of vertical sync pulse is needed flip */
    td_u32  img_width;      /* image width */
    td_u32  img_height;     /* image height */
    td_u32  aspect_ratio_w; /* aspect ratio width */
    td_u32  aspect_ratio_h; /* aspect ratio height */
    td_bool interlace;      /* flag of interlace */
    td_s32  pixel_clk;      /* pixelc clk for this timing, unit:KHz */
} ot_hdmi_timing_info;

typedef struct {
    td_u32 detail_timing_num;
    ot_hdmi_timing_info detail_timing[OT_HDMI_DETAIL_TIMING_MAX];
} ot_hdmi_detail_timing;

typedef struct {
    td_bool is_connected; /* Whether the devices are connected. */
    /* Whether the HDMI is supported by the device. If the HDMI is not supported by the device, the device is DVI. */
    td_bool support_hdmi;
    td_bool is_sink_power_on; /* Whether the sink device is powered on. */
    ot_hdmi_video_format native_video_format; /* Physical resolution of the display device. */
    /*
     * Video capability set.
     * TD_TRUE: This display format is supported.
     * TD_FALSE: This display format is not supported.
     */
    td_bool support_video_format[OT_HDMI_VIDEO_FORMAT_BUTT];
    /*
     * Whether the YCBCR display is supported.
     * TD_TRUE: The YCBCR display is supported.
     * TD_FALSE: Only red-green-blue (RGB) is supported.
     */
    td_bool support_ycbcr;
    /* Whether the xvYCC601 color format is supported. */
    td_bool support_xvycc601;
    /* Whether the xvYCC709 color format is supported. */
    td_bool support_xvycc709;
    /* Transfer profile supported by xvYCC601. 1: P0; 2: P1; 4: P2. */
    td_u8 md_bit;
    /* Num of audio Info */
    td_u32 audio_info_num;
    /* Audio Info.For details, see Table 37 in EIA-CEA-861-D. */
    ot_hdmi_audio_info audio_info[OT_HDMI_MAX_AUDIO_CAPBILITY_CNT];
    /* Speaker position. For details, see the definition of SpeakerDATABlock in EIA-CEA-861-D. */
    td_bool speaker[OT_HDMI_AUDIO_SPEAKER_BUTT];
    /* Device vendor flag */
    td_u8 manufacture_name[OT_HDMI_MANUFACTURE_NAME_LEN];
    /* Device ID. */
    td_u32 pdt_code;
    /* Device sequence number. */
    td_u32 serial_num;
    /* Device production data (week). */
    td_u32 week_of_manufacture;
    /* Set the production data (year). */
    td_u32 year_of_manufacture;
    /* Device version number */
    td_u8 version;
    /* Device sub version number */
    td_u8 revision;
    /* EDID extended block number */
    td_u8 edid_ex_blk_num;
    /* Valid flag of the consumer electronics control (CEC) physical address */
    td_bool is_phys_addr_valid;
    /* CEC physical address A */
    td_u8 phys_addr_a;
    /* CEC physical address B */
    td_u8 phys_addr_b;
    /* CEC physical address C */
    td_u8 phys_addr_c;
    /* CEC physical address D */
    td_u8 phys_addr_d;
    /* Whether to support the DVI dual-link operation. */
    td_bool support_dvi_dual;
    /* Whether to support the YCBCR 4:4:4 deep-color mode. */
    td_bool support_deepcolor_ycbcr444;
    /* Whether to support the deep-color 30-bit mode. */
    td_bool support_deepcolor_30bit;
    /* Whether to support the deep-color 36-bit mode. */
    td_bool support_deepcolor_36bit;
    /* Whether to support the deep-color 48-bit mode. */
    td_bool support_deepcolor_48bit;
    /* Whether to support the Supports_AI mode. */
    td_bool support_ai;
    /* Maximum TMDS clock. */
    td_u32 max_tmds_clk;
    /* Delay flag bit. */
    td_bool i_latency_fields_present;
    /* whether Video_Latency and Audio_Latency fields are present */
    td_bool latency_fields_present;
    /* Special video format */
    td_bool hdmi_video_present;
    /* Video delay */
    td_u8 video_latency;
    /* Audio delay */
    td_u8 audio_latency;
    /* Video delay in interlaced video mode */
    td_u8 interlaced_video_latency;
    /* Audio delay in interlaced video mode */
    td_u8 interlaced_audio_latency;
    /* Whether to support the YUV420 deep-color 30-bit mode. */
    td_bool support_y420_dc_30bit;
    /* Whether to support the YUV420 deep-color 36-bit mode. */
    td_bool support_y420_dc_36bit;
    /* Whether to support the YUV420 deep-color 48-bit mode. */
    td_bool support_y420_dc_48bit;
    /* Whether to support HDMI2.0. */
    td_bool support_hdmi_2_0;
    /* the format support YUV420 */
    td_bool support_y420_format[OT_HDMI_VIDEO_FORMAT_BUTT];
    /* the format only support YUV420 */
    td_bool only_support_y420_format[OT_HDMI_VIDEO_FORMAT_BUTT];
    /*
     * flag of RGB Quantization Range selectable.
     * when TD_TRUE,sink expect full/limited range is selectable;or sink expect default range.
     */
    td_bool ycc_quant_selectable;
    /*
     * flag of YCC Quantization Range selectable.
     * when TD_TRUE,sink expect full/limited range is selectable;or sink expect default range.
     */
    td_bool rgb_quant_selectable;
    /* HDR support flag.When HDR Static Metadata Data Block is indicated,flag TD_TRUE;or,TD_FALSE. */
    td_bool support_hdr;
    /* HDR capability */
    ot_hdmi_hdr_capability hdr;
    /* Detailed timing information */
    ot_hdmi_detail_timing detailed_timing;
} ot_hdmi_sink_capability;

typedef struct {
    /* Whether to forcibly output the video over the HDMI. */
    td_bool hdmi_en;
    /* Video norm. This value of the video norm must be consistent with the norm of the video output. */
    ot_hdmi_video_format video_format;
    /* DeepColor output mode.It is OT_HDMI_DEEP_COLOR_24BIT by default. */
    ot_hdmi_deep_color deep_color_mode;
    /* Whether to enable the audio. */
    td_bool audio_en;
    /* Audio sampling rate. This parameter needs to be consistent with that of the VO. */
    ot_hdmi_sample_rate sample_rate;
    /* Audio bit width. It is 16 by default. This parameter needs to be consistent with that of the VO. */
    ot_hdmi_bit_depth bit_depth;
    /* Whether to enable auth mode. 0: disabled 1: enabled */
    td_bool auth_mode_en;
    /*
     * Enable flag of deep color mode adapting case of user setting incorrect,
     * default: TD_FALSE.When user have no any adapting strategy,suggestion TD_TRUE
     */
    td_bool deep_color_adapt_en;
    /*
     * Pixclk of enVideoFmt(unit is kHz).
     * (This param is valid only when enVideoFmt is OT_HDMI_VIDEO_FMT_VESA_CUSTOMER_DEFINE)
     */
    td_u32 pix_clk;
} ot_hdmi_attr;

typedef struct {
    td_bool edid_valid;
    td_u32  edid_len;
    td_u8   edid[OT_HDMI_EDID_RAW_DATA_LEN];
} ot_hdmi_edid;

typedef struct {
    ot_hdmi_audio_chn_cnt      chn_cnt;
    ot_hdmi_coding_type        coding_type;
    ot_hdmi_audio_sample_size  sample_size;
    ot_hdmi_audio_sample_freq  sampling_freq;
    td_u8                      chn_alloc; /* Channel/Speaker Allocation.Range [0,255] */
    ot_hdmi_level_shift_val    level_shift;
    ot_hdmi_lfe_playback_level lfe_playback_level;
    td_bool                    down_mix_inhibit;
} ot_hdmi_audio_infoframe;

typedef struct {
    ot_hdmi_video_format        timing_mode;
    ot_hdmi_color_space         color_space;
    td_bool                     active_info_present;
    ot_hdmi_bar_info            bar_info;
    ot_hdmi_scan_info           scan_info;
    ot_hdmi_colorimetry         colorimetry;
    ot_hdmi_ex_colorimetry      ex_colorimetry;
    ot_pic_aspect_ratio         aspect_ratio;
    ot_hdmi_active_aspect_ratio active_aspect_ratio;
    ot_hdmi_pic_scaline         pic_scaling;
    ot_hdmi_rgb_quant_range     rgb_quant;
    td_bool                     is_it_content;
    ot_hdmi_pixel_repetition    pixel_repetition;
    ot_hdmi_content_type        content_type;
    ot_hdmi_ycc_quant_range     ycc_quant;
    td_u16                      line_n_end_of_top_bar;
    td_u16                      line_n_start_of_bot_bar;
    td_u16                      pixel_n_end_of_left_bar;
    td_u16                      pixel_n_start_of_right_bar;
} ot_hdmi_avi_infoframe;

typedef struct {
    td_u8 data_len;
    td_u8 user_data[OT_HDMI_VENDOR_USER_DATA_MAX_LEN];
} ot_hdmi_vendorspec_infoframe;

typedef union {
    ot_hdmi_avi_infoframe avi_infoframe;     /* AUTO:ot_hdmi_infoframe_type:OT_INFOFRAME_TYPE_AVI; */
    ot_hdmi_audio_infoframe audio_infoframe; /* AUTO:ot_hdmi_infoframe_type:OT_INFOFRAME_TYPE_AUDIO; */
    ot_hdmi_vendorspec_infoframe vendor_spec_infoframe; /* AUTO:ot_hdmi_infoframe_type:OT_INFOFRAME_TYPE_VENDORSPEC; */
} ot_hdmi_infoframe_unit;

typedef struct {
    ot_hdmi_infoframe_type infoframe_type; /* InfoFrame type */
    ot_hdmi_infoframe_unit infoframe_unit; /* InfoFrame date */
} ot_hdmi_infoframe;

typedef struct {
    ot_hdmi_callback hdmi_event_callback; /* Event handling callback function */
    td_void *private_data;                /* Private data of the callback functions and parameters */
} ot_hdmi_callback_func;

typedef struct {
    td_u32 i_de_main_clk;
    td_u32 i_de_main_data;
    td_u32 i_main_clk;
    td_u32 i_main_data;
    td_u32 ft_cap_clk;
    td_u32 ft_cap_data;
} ot_hdmi_hw_param;

typedef struct {
    ot_hdmi_hw_param hw_param[OT_HDMI_HW_PARAM_NUM];
} ot_hdmi_hw_spec;

typedef struct {
    td_bool is_connected;     /* The Device is connected or disconnected */
    td_bool is_sink_power_on; /* The sink is PowerOn or not */
} ot_hdmi_status;

typedef enum {
    OT_HDMI_TRACE_LEN_0,
    OT_HDMI_TRACE_LEN_1,
    OT_HDMI_TRACE_LEN_2,
    OT_HDMI_TRACE_LEN_3,
    OT_HDMI_TRACE_LEN_4,
    OT_HDMI_TRACE_LEN_5,
    OT_HDMI_TRACE_LEN_6,
    OT_HDMI_TRACE_LEN_7,
    OT_HDMI_TRACE_LEN_8,
    OT_HDMI_TRACE_DEFAULT,
    OT_HDMI_TRACE_BUTT
} ot_hdmi_trace_len;

typedef struct {
    td_bool emi_en; /* emi enable/disable */
    ot_hdmi_trace_len trace_len; /* Phy parameters selection. */
} ot_hdmi_mod_param;

#define OT_ERR_HDMI_NOT_INIT              OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_NOT_INIT)
#define OT_ERR_HDMI_INVALID_PARA          OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_INVALID_PARAM)
#define OT_ERR_HDMI_NULL_PTR              OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_NULL_PTR)
#define OT_ERR_HDMI_DEV_NOT_OPEN          OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_DEV_NOT_OPEN)
#define OT_ERR_HDMI_DEV_NOT_CONNECT       OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_DEV_NOT_CONNECT)
#define OT_ERR_HDMI_READ_SINK_FAILED      OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_READ_SINK_FAILED)
#define OT_ERR_HDMI_INIT_ALREADY          OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_INIT_ALREADY)
#define OT_ERR_HDMI_CALLBACK_ALREADY      OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_CALLBACK_ALREADY)
#define OT_ERR_HDMI_INVALID_CALLBACK      OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_INVALID_CALLBACK)
#define OT_ERR_HDMI_FEATURE_NO_SUPPORT \
    OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_FEATURE_NO_SUPPORT)
#define OT_ERR_HDMI_BUS_BUSY              OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_BUS_BUSY)
#define OT_ERR_HDMI_READ_EVENT_FAILED     OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_READ_EVENT_FAILED)
#define OT_ERR_HDMI_NOT_START             OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_NOT_START)
#define OT_ERR_HDMI_READ_EDID_FAILED      OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_READ_EDID_FAILED)
#define OT_ERR_HDMI_INIT_FAILED           OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_INIT_FAILED)
#define OT_ERR_HDMI_CREATE_TESK_FAILED \
    OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_CREATE_TESK_FAILED)
#define OT_ERR_HDMI_MALLOC_FAILED         OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_MALLOC_FAILED)
#define OT_ERR_HDMI_FREE_FAILED           OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_FREE_FAILED)
#define OT_ERR_HDMI_PTHREAD_CREATE_FAILED \
    OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_PTHREAD_CREATE_FAILED)
#define OT_ERR_HDMI_PTHREAD_JOIN_FAILED \
    OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_PTHREAD_JOIN_FAILED)
#define OT_ERR_HDMI_STRATEGY_FAILED       OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_STRATEGY_FAILED)
#define OT_ERR_HDMI_SET_ATTR_FAILED       OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_SET_ATTR_FAILED)
#define OT_ERR_HDMI_CALLBACK_NOT_REGISTER \
    OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_CALLBACK_NOT_REGISTER)
#define OT_ERR_HDMI_UNKNOWN_COMMAND       OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_UNKNOWN_CMD)
#define OT_ERR_HDMI_MUTEX_LOCK_FAILED     OT_DEFINE_ERR(OT_ID_HDMI, OT_ERR_LEVEL_ERROR, OT_ERRNO_HDMI_MUTEX_LOCK_FAILED)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_HDMI_H_BODY_PART02_H_ */
