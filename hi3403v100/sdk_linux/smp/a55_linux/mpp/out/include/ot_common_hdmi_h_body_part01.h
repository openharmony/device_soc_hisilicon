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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_HDMI_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_HDMI_H_BODY_PART01_H_


#include "ot_type.h"
#include "ot_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OT_HDMI_MAX_AUDIO_CAPBILITY_CNT  16
#define OT_HDMI_MAX_SAMPLE_RATE_NUM      8
#define OT_HDMI_MAX_BIT_DEPTH_NUM        6
#define OT_HDMI_DETAIL_TIMING_MAX        10
#define OT_HDMI_EDID_RAW_DATA_LEN        512
#define OT_HDMI_VENDOR_NAME_LEN          8
#define OT_HDMI_HW_PARAM_NUM             4
#define OT_HDMI_BKSV_LEN                 5
#define OT_HDMI_MANUFACTURE_NAME_LEN     4
/* user data len:31 - 4(pkt header) - 5(4K- the length must be 0x05)  */
#define OT_HDMI_VENDOR_USER_DATA_MAX_LEN 22

typedef enum {
    OT_ERRNO_HDMI_NOT_INIT = 1,
    OT_ERRNO_HDMI_INVALID_PARAM,
    OT_ERRNO_HDMI_NULL_PTR,
    OT_ERRNO_HDMI_DEV_NOT_OPEN,
    OT_ERRNO_HDMI_DEV_NOT_CONNECT,
    OT_ERRNO_HDMI_READ_SINK_FAILED,
    OT_ERRNO_HDMI_INIT_ALREADY,
    OT_ERRNO_HDMI_CALLBACK_ALREADY,
    OT_ERRNO_HDMI_INVALID_CALLBACK,
    OT_ERRNO_HDMI_FEATURE_NO_SUPPORT,
    OT_ERRNO_HDMI_BUS_BUSY,
    OT_ERRNO_HDMI_READ_EVENT_FAILED,
    OT_ERRNO_HDMI_NOT_START,
    OT_ERRNO_HDMI_READ_EDID_FAILED,
    OT_ERRNO_HDMI_INIT_FAILED,
    OT_ERRNO_HDMI_CREATE_TESK_FAILED,
    OT_ERRNO_HDMI_MALLOC_FAILED,
    OT_ERRNO_HDMI_FREE_FAILED,
    OT_ERRNO_HDMI_PTHREAD_CREATE_FAILED,
    OT_ERRNO_HDMI_PTHREAD_JOIN_FAILED,
    OT_ERRNO_HDMI_STRATEGY_FAILED,
    OT_ERRNO_HDMI_SET_ATTR_FAILED,
    OT_ERRNO_HDMI_CALLBACK_NOT_REGISTER,
    OT_ERRNO_HDMI_UNKNOWN_CMD,
    OT_ERRNO_HDMI_MUTEX_LOCK_FAILED,
    ERR_HDMI_BUTT
} ot_hdmi_errno;

typedef enum {
    OT_HDMI_AUDIO_CHN_CNT_STREAM,
    OT_HDMI_AUDIO_CHN_CNT_2,
    OT_HDMI_AUDIO_CHN_CNT_3,
    OT_HDMI_AUDIO_CHN_CNT_4,
    OT_HDMI_AUDIO_CHN_CNT_5,
    OT_HDMI_AUDIO_CHN_CNT_6,
    OT_HDMI_AUDIO_CHN_CNT_7,
    OT_HDMI_AUDIO_CHN_CNT_8,
    OT_HDMI_AUDIO_CHN_CNT_BUTT
} ot_hdmi_audio_chn_cnt;

typedef enum {
    OT_HDMI_AUDIO_SAMPLE_SIZE_STREAM,
    OT_HDMI_AUDIO_SAMPLE_SIZE_16,
    OT_HDMI_AUDIO_SAMPLE_SIZE_20,
    OT_HDMI_AUDIO_SAMPLE_SIZE_24,
    OT_HDMI_AUDIO_SAMPLE_SIZE_BUTT
} ot_hdmi_audio_sample_size;

typedef enum {
    OT_HDMI_AUDIO_SAMPLE_FREQ_STREAM,
    OT_HDMI_AUDIO_SAMPLE_FREQ_32000,
    OT_HDMI_AUDIO_SAMPLE_FREQ_44100,
    OT_HDMI_AUDIO_SAMPLE_FREQ_48000,
    OT_HDMI_AUDIO_SAMPLE_FREQ_88200,
    OT_HDMI_AUDIO_SAMPLE_FREQ_96000,
    OT_HDMI_AUDIO_SAMPLE_FREQ_176400,
    OT_HDMI_AUDIO_SAMPLE_FREQ_192000,
    OT_HDMI_AUDIO_SAMPLE_FREQ_BUTT
} ot_hdmi_audio_sample_freq;

typedef enum {
    OT_HDMI_LEVEL_SHIFT_VAL_0_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_1_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_2_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_3_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_4_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_5_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_6_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_7_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_8_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_9_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_10_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_11_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_12_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_13_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_14_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_15_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_BUTT
} ot_hdmi_level_shift_val;

typedef enum {
    OT_HDMI_LFE_PLAYBACK_NO,
    OT_HDMI_LFE_PLAYBACK_0_DB,
    OT_HDMI_LFE_PLAYBACK_10_DB,
    OT_HDMI_LFE_PLAYBACK_BUTT
} ot_hdmi_lfe_playback_level;

typedef enum {
    OT_INFOFRAME_TYPE_AVI,
    OT_INFOFRAME_TYPE_AUDIO,
    OT_INFOFRAME_TYPE_VENDORSPEC,
    OT_INFOFRAME_TYPE_BUTT
} ot_hdmi_infoframe_type;

typedef enum {
    OT_HDMI_VIDEO_FORMAT_1080P_60,
    OT_HDMI_VIDEO_FORMAT_1080P_50,
    OT_HDMI_VIDEO_FORMAT_1080P_30,
    OT_HDMI_VIDEO_FORMAT_1080P_25,
    OT_HDMI_VIDEO_FORMAT_1080P_24,
    OT_HDMI_VIDEO_FORMAT_1080i_60,
    OT_HDMI_VIDEO_FORMAT_1080i_50,
    OT_HDMI_VIDEO_FORMAT_720P_60,
    OT_HDMI_VIDEO_FORMAT_720P_50,
    OT_HDMI_VIDEO_FORMAT_576P_50,
    OT_HDMI_VIDEO_FORMAT_480P_60,
    OT_HDMI_VIDEO_FORMAT_PAL,
    OT_HDMI_VIDEO_FORMAT_NTSC,
    OT_HDMI_VIDEO_FORMAT_861D_640X480_60,
    OT_HDMI_VIDEO_FORMAT_VESA_800X600_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1024X768_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1280X800_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1280X1024_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1366X768_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1440X900_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1400X1050_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1600X1200_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1680X1050_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1920X1200_60,
    OT_HDMI_VIDEO_FORMAT_2560x1440_30,
    OT_HDMI_VIDEO_FORMAT_2560x1440_60,
    OT_HDMI_VIDEO_FORMAT_2560x1600_60,
    OT_HDMI_VIDEO_FORMAT_1920x2160_30,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_24,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_25,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_30,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_50,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_60,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_24,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_25,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_30,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_50,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_60,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_120,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_120,
    OT_HDMI_VIDEO_FORMAT_7680X4320P_30,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_BUTT
} ot_hdmi_video_format;

typedef enum {
    OT_HDMI_PIC_ASPECT_RATIO_NO_DATA,
    OT_HDMI_PIC_ASPECT_RATIO_4TO3,
    OT_HDMI_PIC_ASPECT_RATIO_16TO9,
    OT_HDMI_PIC_ASPECT_RATIO_64TO27,
    OT_HDMI_PIC_ASPECT_RATIO_256TO135,
    OT_HDMI_PIC_ASPECT_RATIO_BUTT
} ot_pic_aspect_ratio;

typedef enum {
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_TOP = 2,
    OT_HDMI_ACTIVE_ASPECT_RATIO_14TO9_TOP,
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_BOX_CENTER,
    OT_HDMI_ACTIVE_ASPECT_RATIO_SAME_PIC = 8,
    OT_HDMI_ACTIVE_ASPECT_RATIO_4TO3_CENTER,
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_CENTER,
    OT_HDMI_ACTIVE_ASPECT_RATIO_14TO9_CENTER,
    OT_HDMI_ACTIVE_ASPECT_RATIO_4TO3_14_9 = 13,
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_14_9,
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_4_3,
    OT_HDMI_ACTIVE_ASPECT_RATIO_BUTT
} ot_hdmi_active_aspect_ratio;

typedef enum {
    OT_HDMI_SAMPLE_RATE_UNKNOWN, /* unknown sample rate */
    OT_HDMI_SAMPLE_RATE_8K,      /* 8K sample rate */
    OT_HDMI_SAMPLE_RATE_11K,     /* 11.025K sample rate */
    OT_HDMI_SAMPLE_RATE_12K,     /* 12K sample rate */
    OT_HDMI_SAMPLE_RATE_16K,     /* 16K sample rate */
    OT_HDMI_SAMPLE_RATE_22K,     /* 22.050K sample rate */
    OT_HDMI_SAMPLE_RATE_24K,     /* 24K sample rate */
    OT_HDMI_SAMPLE_RATE_32K,     /* 32K sample rate */
    OT_HDMI_SAMPLE_RATE_44K,     /* 44.1K sample rate */
    OT_HDMI_SAMPLE_RATE_48K,     /* 48K sample rate */
    OT_HDMI_SAMPLE_RATE_88K,     /* 88.2K sample rate */
    OT_HDMI_SAMPLE_RATE_96K,     /* 96K sample rate */
    OT_HDMI_SAMPLE_RATE_176K,    /* 176K sample rate */
    OT_HDMI_SAMPLE_RATE_192K,    /* 192K sample rate */
    OT_HDMI_SAMPLE_RATE_768K,    /* 768K sample rate */
    OT_HDMI_SAMPLE_RATE_BUTT
} ot_hdmi_sample_rate;

typedef enum {
    OT_HDMI_AUDIO_FORMAT_CODE_RESERVED, /* Audio coding type, refer stream,default type */
    OT_HDMI_AUDIO_FORMAT_CODE_PCM,      /* Audio coding PCM type */
    OT_HDMI_AUDIO_FORMAT_CODE_AC3,      /* Audio coding AC3 type */
    OT_HDMI_AUDIO_FORMAT_CODE_MPEG1,    /* Audio coding MPEG1 type */
    OT_HDMI_AUDIO_FORMAT_CODE_MP3,      /* Audio coding MP3 type */
    OT_HDMI_AUDIO_FORMAT_CODE_MPEG2,    /* Audio coding MPEG2 type */
    OT_HDMI_AUDIO_FORMAT_CODE_AAC,      /* Audio coding AAC type */
    OT_HDMI_AUDIO_FORMAT_CODE_DTS,      /* Audio coding DTS type */
    OT_HDMI_AUDIO_FORMAT_CODE_ATRAC,    /* Audio coding ATRAC type */
    OT_HDMI_AUDIO_FORMAT_CODE_ONE_BIT,  /* Audio coding ONE BIT AUDIO type */
    OT_HDMI_AUDIO_FORMAT_CODE_DDP,      /* Audio coding DDPLUS type */
    OT_HDMI_AUDIO_FORMAT_CODE_DTS_HD,   /* Audio coding DTS HD type */
    OT_HDMI_AUDIO_FORMAT_CODE_MAT,      /* Audio coding MAT type */
    OT_HDMI_AUDIO_FORMAT_CODE_DST,      /* Audio coding DST type */
    OT_HDMI_AUDIO_FORMAT_CODE_WMA_PRO,  /* Audio coding WMA PRO type */
    OT_HDMI_AUDIO_FORMAT_CODE_BUTT
} ot_hdmi_audio_format_code;

typedef enum {
    OT_HDMI_BIT_DEPTH_UNKNOWN, /* unknown bit width */
    OT_HDMI_BIT_DEPTH_8,       /* 8 bits width */
    OT_HDMI_BIT_DEPTH_16,      /* 16 bits width */
    OT_HDMI_BIT_DEPTH_18,      /* 18 bits width */
    OT_HDMI_BIT_DEPTH_20,      /* 20 bits width */
    OT_HDMI_BIT_DEPTH_24,      /* 24 bits width */
    OT_HDMI_BIT_DEPTH_32,      /* 32 bits width */
    OT_HDMI_BIT_DEPTH_BUTT
} ot_hdmi_bit_depth;

typedef enum {
    OT_HDMI_ID_0,
    OT_HDMI_ID_1,
    OT_HDMI_ID_BUTT
} ot_hdmi_id;

typedef enum {
    OT_HDMI_EVENT_HOTPLUG = 0x10, /* HDMI hot-plug event */
    OT_HDMI_EVENT_NO_PLUG,        /* HDMI cable disconnection event */
    OT_HDMI_EVENT_EDID_FAIL,      /* HDMI EDID read failure event */
    OT_HDMI_EVENT_BUTT
} ot_hdmi_event_type;

typedef enum {
    OT_HDMI_VIDEO_MODE_RGB444,
    OT_HDMI_VIDEO_MODE_YCBCR422,
    OT_HDMI_VIDEO_MODE_YCBCR444,
    OT_HDMI_VIDEO_MODE_YCBCR420,
    OT_HDMI_VIDEO_MODE_BUTT
} ot_hdmi_video_mode;

/* Color Component Sample format and chroma sampling format enum,see EIA-CEA-861-D/F */
typedef enum {
    OT_HDMI_COLOR_SPACE_RGB444,
    OT_HDMI_COLOR_SPACE_YCBCR422,
    OT_HDMI_COLOR_SPACE_YCBCR444,
    /* following is new featrue of CEA-861-F */
    OT_HDMI_COLOR_SPACE_YCBCR420,
    OT_HDMI_COLOR_SPACE_BUTT
} ot_hdmi_color_space;

typedef enum {
    OT_HDMI_DEEP_COLOR_24BIT, /* HDMI Deep Color 24bit mode */
    OT_HDMI_DEEP_COLOR_30BIT, /* HDMI Deep Color 30bit mode */
    OT_HDMI_DEEP_COLOR_36BIT, /* HDMI Deep Color 36bit mode */
    OT_HDMI_DEEP_COLOR_BUTT
} ot_hdmi_deep_color;

typedef enum {
    OT_HDMI_BAR_INFO_NOT_VALID, /* Bar Data not valid */
    OT_HDMI_BAR_INFO_V,         /* Vertical bar data valid */
    OT_HDMI_BAR_INFO_H,         /* Horizontal bar data valid */
    OT_HDMI_BAR_INFO_VH,        /* Horizontal and Vertical bar data valid */
    OT_HDMI_BAR_INFO_BUTT
} ot_hdmi_bar_info;

typedef enum {
    OT_HDMI_SCAN_INFO_NO_DATA,      /* No Scan information */
    OT_HDMI_SCAN_INFO_OVERSCANNED,  /* Scan information, Overscanned (for television) */
    OT_HDMI_SCAN_INFO_UNDERSCANNED, /* Scan information, Underscanned (for computer) */
    OT_HDMI_SCAN_INFO_BUTT
} ot_hdmi_scan_info;

typedef enum {
    OT_HDMI_PIC_NON_UNIFORM_SCALING, /* No Known, non-uniform picture scaling */
    OT_HDMI_PIC_SCALING_H,           /* Picture has been scaled horizontally */
    OT_HDMI_PIC_SCALING_V,           /* Picture has been scaled Vertically */
    OT_HDMI_PIC_SCALING_HV,          /* Picture has been scaled horizontally and Vertically */
    OT_HDMI_PIC_SCALING_BUTT
} ot_hdmi_pic_scaline;

typedef enum {
    OT_HDMI_COMMON_COLORIMETRY_NO_DATA, /* Colorimetry No Data option */
    OT_HDMI_COMMON_COLORIMETRY_ITU601,  /* Colorimetry ITU601 option */
    OT_HDMI_COMMON_COLORIMETRY_ITU709,  /* Colorimetry ITU709 option */
    OT_HDMI_COMMON_COLORIMETRY_BUTT     /* Colorimetry extended option */
} ot_hdmi_colorimetry;

typedef enum {
    OT_HDMI_COMMON_COLORIMETRY_XVYCC_601,               /* Colorimetry xvYCC601 extended option */
    OT_HDMI_COMMON_COLORIMETRY_XVYCC_709,               /* Colorimetry xvYCC709 extended option */
    OT_HDMI_COMMON_COLORIMETRY_S_YCC_601,               /* Colorimetry S YCC 601 extended option */
    OT_HDMI_COMMON_COLORIMETRY_ADOBE_YCC_601,           /* Colorimetry ADOBE YCC 601 extended option */
    OT_HDMI_COMMON_COLORIMETRY_ADOBE_RGB,               /* Colorimetry ADOBE RGB extended option */
    OT_HDMI_COMMON_COLORIMETRY_2020_CONST_LUMINOUS,     /* Colorimetry ITU2020 extended option */
    OT_HDMI_COMMON_COLORIMETRY_2020_NON_CONST_LUMINOUS, /* Colorimetry ITU2020 extended option */
    OT_HDMI_COMMON_COLORIMETRY_EXT_BUTT
} ot_hdmi_ex_colorimetry;

typedef enum {
    OT_HDMI_RGB_QUANT_DEFAULT_RANGE, /* Default range, it depends on the video format */
    OT_HDMI_RGB_QUANT_LIMITED_RANGE, /* Limited quantization range of 220 levels when receiving a CE video format */
    OT_HDMI_RGB_QUANT_FULL_RANGE,    /* Full quantization range of 256 levels when receiving an IT video format */
    OT_HDMI_RGB_QUANT_FULL_BUTT
} ot_hdmi_rgb_quant_range;

typedef enum {
    OT_HDMI_YCC_QUANT_LIMITED_RANGE, /* Limited quantization range of 220 levels when receiving a CE video format */
    OT_HDMI_YCC_QUANT_FULL_RANGE,    /* Full quantization range of 256 levels when receiving an IT video format */
    OT_HDMI_YCC_QUANT_BUTT
} ot_hdmi_ycc_quant_range;

typedef enum {
    OT_HDMI_PIXEL_REPET_NO,
    OT_HDMI_PIXEL_REPET_2_TIMES,
    OT_HDMI_PIXEL_REPET_3_TIMES,
    OT_HDMI_PIXEL_REPET_4_TIMES,
    OT_HDMI_PIXEL_REPET_5_TIMES,
    OT_HDMI_PIXEL_REPET_6_TIMES,
    OT_HDMI_PIXEL_REPET_7_TIMES,
    OT_HDMI_PIXEL_REPET_8_TIMES,
    OT_HDMI_PIXEL_REPET_9_TIMES,
    OT_HDMI_PIXEL_REPET_10_TIMES,
    OT_HDMI_PIXEL_REPET_BUTT
} ot_hdmi_pixel_repetition;

typedef enum {
    OT_HDMI_CONTNET_GRAPHIC,
    OT_HDMI_CONTNET_PHOTO,
    OT_HDMI_CONTNET_CINEMA,
    OT_HDMI_CONTNET_GAME,
    OT_HDMI_CONTNET_BUTT
} ot_hdmi_content_type;

typedef enum {
    OT_HDMI_AUDIO_CODING_REFER_STREAM_HEAD,
    OT_HDMI_AUDIO_CODING_PCM,
    OT_HDMI_AUDIO_CODING_AC3,
    OT_HDMI_AUDIO_CODING_MPEG1,
    OT_HDMI_AUDIO_CODING_MP3,
    OT_HDMI_AUDIO_CODING_MPEG2,
    OT_HDMI_AUDIO_CODING_AACLC,
    OT_HDMI_AUDIO_CODING_DTS,
    OT_HDMI_AUDIO_CODING_ATRAC,
    OT_HDMI_AUDIO_CODIND_ONE_BIT_AUDIO,
    OT_HDMI_AUDIO_CODING_ENAHNCED_AC3,
    OT_HDMI_AUDIO_CODING_DTS_HD,
    OT_HDMI_AUDIO_CODING_MAT,
    OT_HDMI_AUDIO_CODING_DST,
    OT_HDMI_AUDIO_CODING_WMA_PRO,
    OT_HDMI_AUDIO_CODING_BUTT
} ot_hdmi_coding_type;

typedef enum {
    OT_HDMI_AUDIO_SPEAKER_FL_FR,
    OT_HDMI_AUDIO_SPEAKER_LFE,
    OT_HDMI_AUDIO_SPEAKER_FC,
    OT_HDMI_AUDIO_SPEAKER_RL_RR,
    OT_HDMI_AUDIO_SPEAKER_RC,
    OT_HDMI_AUDIO_SPEAKER_FLC_FRC,
    OT_HDMI_AUDIO_SPEAKER_RLC_RRC,
    OT_HDMI_AUDIO_SPEAKER_FLW_FRW,
    OT_HDMI_AUDIO_SPEAKER_FLH_FRH,
    OT_HDMI_AUDIO_SPEAKER_TC,
    OT_HDMI_AUDIO_SPEAKER_FCH,
    OT_HDMI_AUDIO_SPEAKER_BUTT
} ot_hdmi_audio_speaker;

typedef void (*ot_hdmi_callback)(ot_hdmi_event_type event, td_void *private_data);

typedef struct {
    ot_hdmi_audio_format_code audio_format_code; /* Audio coding type */
    ot_hdmi_sample_rate support_sample_rate[OT_HDMI_MAX_SAMPLE_RATE_NUM]; /* Audio sample rate */
    td_u8 audio_chn; /* Aud Channel of the coding type */
    /* sample bit depth,when audio format code is OT_UNF_EDID_AUDIO_FORMAT_CODE_PCM */
    ot_hdmi_bit_depth support_bit_depth[OT_HDMI_MAX_BIT_DEPTH_NUM];
    /* sample bit depth Num,when audio format code is OT_UNF_EDID_AUDIO_FORMAT_CODE_PCM */
    td_u32 support_bit_depth_num;
    /*
     * enter max bit rate,when audio format code is
     * OT_UNF_EDID_AUDIO_FORMAT_CODE_AC3 - OT_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC
     */
    td_u32 max_bit_rate;
} ot_hdmi_audio_info;

typedef struct {
    td_bool eotf_sdr;          /* Traditional gamma - SDR Luminance Range. */
    td_bool eotf_hdr;          /* Traditional gamma - HDR Luminance Range. */
    td_bool eotf_smpte_st2084; /* SMPTE ST 2084 */
    td_bool eotf_hlg;          /* Hybrid Log-Gamma(HLG) based on Recommendation ITU-R BT.2100-0 */
    td_bool eotf_future;       /* Future EOTF */
} ot_hdmi_eotf;

/* EDID Supported Static Metadata Descriptor */
typedef struct {
    td_bool descriptor_type1; /* Support Static Metadata Type 1 or not. */
} ot_hdmi_hdr_metadata_type;

typedef struct {
    ot_hdmi_eotf eotf; /* EOTF support. */
    ot_hdmi_hdr_metadata_type metadata; /* static metadata descriptor type(ID) support. */
    td_u8 max_luma_cv; /* Desired Content Max Luminance Data. Real value(in 1cd/m^2) = 50.2^(CV/32) */
    td_u8 avg_luma_cv; /* Desired Content Max Frame-average Luminance. Real value(in 1cd/m^2) = 50.2^(CV/32) */
    /*
     * Desired Content Min Luminance Data.
     * Real value(in 1cd/m^2) = u8MaxLuminance_CV * (u8MinLuminance_CV/255)^2 / 100
     */
    td_u8 min_luma_cv;
} ot_hdmi_hdr_capability;

typedef struct {
    td_u32  vfb;            /* vertical front blank */
    td_u32  vbb;            /* vertical back blank */
    td_u32  vact;           /* vertical active area */
    td_u32  hfb;            /* horizontal front blank */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_HDMI_H_BODY_PART01_H_ */
