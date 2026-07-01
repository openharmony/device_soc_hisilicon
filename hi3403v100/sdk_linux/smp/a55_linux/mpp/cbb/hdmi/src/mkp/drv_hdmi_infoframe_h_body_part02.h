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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INFOFRAME_H_BODY_PART02_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INFOFRAME_H_BODY_PART02_H_

    HDMI_VIDEO_FORMAT_VESA_1280X800_60,
    HDMI_VIDEO_FORMAT_VESA_1280X1024_60,
    HDMI_VIDEO_FORMAT_VESA_1366X768_60,
    HDMI_VIDEO_FORMAT_VESA_1440X900_60,
    HDMI_VIDEO_FORMAT_VESA_1400X1050_60,
    HDMI_VIDEO_FORMAT_VESA_1600X1200_60,
    HDMI_VIDEO_FORMAT_VESA_1680X1050_60,
    HDMI_VIDEO_FORMAT_VESA_1920X1200_60,
    HDMI_VIDEO_FORMAT_2560X1440_30,
    HDMI_VIDEO_FORMAT_2560X1440_60,
    HDMI_VIDEO_FORMAT_2560X1600_60,
    HDMI_VIDEO_FORMAT_1920X2160_30,
    HDMI_VIDEO_FORMAT_3840X2160P_24,
    HDMI_VIDEO_FORMAT_3840X2160P_25,
    HDMI_VIDEO_FORMAT_3840X2160P_30,
    HDMI_VIDEO_FORMAT_3840X2160P_50,
    HDMI_VIDEO_FORMAT_3840X2160P_60,
    HDMI_VIDEO_FORMAT_4096X2160P_24,
    HDMI_VIDEO_FORMAT_4096X2160P_25,
    HDMI_VIDEO_FORMAT_4096X2160P_30,
    HDMI_VIDEO_FORMAT_4096X2160P_50,
    HDMI_VIDEO_FORMAT_4096X2160P_60,
    HDMI_VIDEO_FORMAT_3840X2160P_120,
    HDMI_VIDEO_FORMAT_4096X2160P_120,
    HDMI_VIDEO_FORMAT_7680X4320P_30,
    HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    HDMI_VIDEO_FORMAT_BUTT
} hdmi_disp_format;

struct hdmi_avi_infoframe {
    enum hdmi_infoframe_type    type;
    td_u8                       version;
    td_u8                       length;
    hdmi_colorspace             colorspace;
    td_bool                     active_info_valid;
    td_bool                     horizontal_bar_valid;
    td_bool                     vertical_bar_valid;
    enum hdmi_scan_mode         scan_mode;
    enum hdmi_colorimetry       colorimetry;
    hdmi_picture_aspect         picture_aspect;
    hdmi_active_aspect          active_aspect;
    td_bool                     itc;
    hdmi_extended_colormetry    extended_colorimetry;
    hdmi_quantization_range     quantization_range;
    enum hdmi_nups              nups;
    hdmi_video_timing           video_timing;
    hdmi_disp_format            disp_fmt;
    hdmi_ycc_quantization_range ycc_quantization_range;
    enum hdmi_content_type      content_type;
    td_u8                       pixel_repeat;
    td_u16                      top_bar;
    td_u16                      bottom_bar;
    td_u16                      left_bar;
    td_u16                      right_bar;
};

enum hdmi_audio_sample_size {
    HDMI_AUDIO_SAMPLE_SIZE_STREAM,
    HDMI_AUDIO_SAMPLE_SIZE_16,
    HDMI_AUDIO_SAMPLE_SIZE_20,
    HDMI_AUDIO_SAMPLE_SIZE_24
};

enum hdmi_audio_sample_frequency {
    HDMI_AUDIO_SAMPLE_FREQUENCY_STREAM,
    HDMI_AUDIO_SAMPLE_FREQUENCY_32000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_44100,
    HDMI_AUDIO_SAMPLE_FREQUENCY_48000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_88200,
    HDMI_AUDIO_SAMPLE_FREQUENCY_96000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_176400,
    HDMI_AUDIO_SAMPLE_FREQUENCY_192000
};

enum hdmi_audio_coding_type_ext {
    HDMI_AUDIO_CODING_TYPE_EXT_STREAM,
    HDMI_AUDIO_CODING_TYPE_EXT_HE_AAC,
    HDMI_AUDIO_CODING_TYPE_EXT_HE_AAC_V2,
    HDMI_AUDIO_CODING_TYPE_EXT_MPEG_SURROUND
};

enum hdmi_audio_lfe_playback_level {
    HDMI_AUDIO_LFE_PLAYBACK_NO,
    HDMI_AUDIO_LFE_PLAYBACK_0_DB,
    HDMI_AUDIO_LFE_PLAYBACK_10_DB,
    HDMI_AUDIO_LFE_PLAYBACK_RESERVED
};

enum hdmi_audio_code {
    HDMI_AUDIO_CODING_TYPE_STREAM,
    HDMI_AUDIO_CODING_TYPE_PCM,
    HDMI_AUDIO_CODING_TYPE_AC3,
    HDMI_AUDIO_CODING_TYPE_MPEG1,
    HDMI_AUDIO_CODING_TYPE_MP3,
    HDMI_AUDIO_CODING_TYPE_MPEG2,
    HDMI_AUDIO_CODING_TYPE_AAC_LC,
    HDMI_AUDIO_CODING_TYPE_DTS,
    HDMI_AUDIO_CODING_TYPE_ATRAC,
    HDMI_AUDIO_CODING_TYPE_DSD,
    HDMI_AUDIO_CODING_TYPE_EAC3,
    HDMI_AUDIO_CODING_TYPE_DTS_HD,
    HDMI_AUDIO_CODING_TYPE_MLP,
    HDMI_AUDIO_CODING_TYPE_DST,
    HDMI_AUDIO_CODING_TYPE_WMA_PRO,
    HDMI_AUDIO_CODING_TYPE_RESERVED,
    HDMI_AUDIO_CODING_TYPE_BUTT
};

struct hdmi_audio_infoframe {
    enum hdmi_infoframe_type           type;
    td_u8                              version;
    td_u8                              length;
    td_u8                              channels;
    enum hdmi_audio_code               coding_type;
    enum hdmi_audio_sample_size        sample_size;
    enum hdmi_audio_sample_frequency   sample_frequency;
    enum hdmi_audio_coding_type_ext    coding_type_ext;
    td_u8                              channel_allocation;
    enum hdmi_audio_lfe_playback_level lfe_playback_level;
    td_u8                              level_shift_value;
    td_bool                            downmix_inhibit;
};

enum hdmi_ieee_oui {
    HDMI14_IEEE_OUI = 0x00000c03, // HDMI1.4
    HDMI20_IEEE_OUI = 0x00c45dd8  // HDMI2.0
};

enum hdmi_video_format {
    HDMI_VIDEO_FORMAT_NONE,
    HDMI_VIDEO_FORMAT_4K,
    HDMI_VIDEO_FORMAT_3D
};

enum hdmi_vic {
    HDMI_VIC_4K_3840X2160_30 = 0x1,
    HDMI_VIC_4K_3840X2160_25,
    HDMI_VIC_4K_3840X2160_24,
    HDMI_VIC_4K_4096X2160_24
};

enum hdmi_3d_structure {
    HDMI_3D_FRAME_PACKING,
    HDMI_3D_FIELD_ALTERNATIVE,
    HDMI_3D_LINE_ALTERNATIVE,
    HDMI_3D_SIDE_BY_SIDE_FULL,
    HDMI_3D_L_DEPTH,
    HDMI_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH,
    HDMI_3D_TOP_AND_BOTTOM,
    HDMI_3D_SIDE_BY_SIDE_HALF = 0x08,
    HDMI_3D_BUTT
};

enum hdmi_3d_ext_data {
    HDMI_3D_PICTURE_ODD_LEFT_ODD_RIGHT = 0x4,
    HDMI_3D_PICTURE_ODD_LEFT_EVEN_RIGHT,
    HDMI_3D_PICTURE_EVEN_LEFT_ODD_RIGHT,
    HDMI_3D_PICTURE_EVEN_LEFT_EVEN_RIGHT
};

typedef struct {
    enum hdmi_ieee_oui     ieee;
    enum hdmi_video_format format;
    enum hdmi_vic          vic;
    enum hdmi_3d_structure _3d_structure;
    td_u8                  _3d_meta_present;
    enum hdmi_3d_ext_data  _3d_ext_data;
    td_u8                  _3d_metadata_type;
    td_u8                  _3d_metadata_length;
    td_u8                  _3d_metadata[20];
} hdmi_14_vsif_content;

typedef struct {
    enum hdmi_ieee_oui     ieee;
    enum hdmi_video_format format;
    enum hdmi_vic          vic;
    enum hdmi_3d_structure _3d_structure;
    td_u8                  _3d_meta_present;
    td_u8                  len;
    td_u8                  data[HDMI_VENDOR_USER_DATA_MAX_LEN];
} hdmi_user_vsif_content;

typedef struct {
    enum hdmi_ieee_oui ieee;
    td_u8 version;
    td_u8 _3d_valid;
    td_u8 _3d_f_structure;
    td_u8 _3d_additional_info_present;
    td_u8 _3d_disparity_data_present;
    td_u8 _3d_meta_present;
    td_u8 _3d_f_ext_data;
    td_u8 _3d_dual_view;
    td_u8 _3d_view_dependency;
    td_u8 _3d_preferred_2d_view;
    td_u8 _3d_disparity_data_version;
    td_u8 _3d_disparity_data_length;
    td_u8 _3d_disparity_data[10];
    td_u8 _3d_ext_data;
    td_u8 _3d_metadata_type;
    td_u8 _3d_metadata_length;
    td_u8 _3d_metadata[10];
} hdmi_forum_vsif_content;

struct hdmi_vendor_specific_infoframe {
    enum hdmi_infoframe_type type;
    td_u8 version;
    td_u8 length;
    union {
        hdmi_user_vsif_content  cea861_vsif;
        hdmi_14_vsif_content    h14_vsif_content;
        hdmi_forum_vsif_content hf_vsif_content;
    } vsif_content;
};

typedef union {
    struct {
        td_u8 affected_gamut_seq_num : 4; // [0..3]
        td_u8 gbd_profile            : 3; // [4..6]
        td_u8 next_field             : 1; // [7]
    } bits;
    td_u8 u8;
} hdmi_gumat_hb1;

typedef union {
    struct {
        td_u8 current_gamut_seq_num : 4; // [0..3]
        td_u8 packet_seq            : 2; // [4..5]
        td_u8 reserved              : 1; // [6]
        td_u8 no_crnt_gbd           : 1; // [7]
    } bits;
    td_u8 u8;
} hdmi_gumat_hb2;

typedef union {
    struct {
        td_u8 gbd_color_space     : 3; // [0..2]
        td_u8 gbd_color_precision : 2; // [3..4]
        td_u8 reseved0            : 1; // [5]
        td_u8 reseved1            : 1; // [6]
        td_u8 format_flag         : 1; // [7]
    } bits;
    td_u8 u8;
} hdmi_gumat_range_gdb0;

typedef struct {
    td_u8 hb0;
    hdmi_gumat_hb1 hb1;
    hdmi_gumat_hb2 hb2;
    hdmi_gumat_range_gdb0 gdb0;
    td_u8 min_red_data_h;
    td_u8 mid_red_data_m;   // max_red_data_l||max_red_data_h
    td_u8 max_red_data_l;
    td_u8 min_green_data_h;
    td_u8 mid_green_data_m; // max_green_data_l||max_green_data_h
    td_u8 max_green_data_l;
    td_u8 min_blue_data_h;
    td_u8 mid_blue_data_m;  // max_blue_data_l||max_blue_data_h
    td_u8 max_blue_data_l;
} hdmi_gdb_pack;

typedef enum   hdmi_3d_structure              hdmi_3d_mode;
typedef enum   hdmi_colorimetry               hdmi_colormetry;
typedef enum   hdmi_audio_code                hdmi_audio_format_code;
typedef enum   hdmi_vic                       hdmi_vsif_vic;
typedef enum   hdmi_infoframe_type            hdmi_infoframe_id;
typedef struct hdmi_avi_infoframe             hdmi_avi_infoframe;
typedef struct hdmi_audio_infoframe           hdmi_audio_infoframe;
typedef struct hdmi_vendor_specific_infoframe hdmi_vendor_infoframe;

typedef struct {
    hdmi_avi_infoframe    avi_infoframe;
    hdmi_audio_infoframe  audio_infoframe;
    hdmi_vendor_infoframe vendor_infoframe;
    hdmi_gdb_pack         gdb_pack;
} hdmi_infoframe_type;

typedef union {
    hdmi_avi_infoframe    avi_infoframe;
    hdmi_audio_infoframe  audio_infoframe;
    hdmi_vendor_infoframe vendor_infoframe;
    hdmi_gdb_pack         gdb_pack;
} hdmi_infoframe;

td_s32 drv_hdmi_avi_infoframe_send(hdmi_infoframe_type *info_frame, td_bool enable);

td_s32 drv_hdmi_vendor_infoframe_send(hdmi_infoframe_type *info_frame, td_bool enable);

td_s32 drv_hdmi_audio_infoframe_send(hdmi_infoframe_type *info_frame, td_bool enable);

td_void drv_hdmi_infoframe_send(const hdmi_infoframe_type *info_frame,
    hdmi_infoframe_id infoframe_id, hdmi_infoframe *infoframe);

td_void drv_hdmi_infoframe_enable_set(const hdmi_infoframe_type *info_frame,
    hdmi_infoframe_id infoframe_id, td_bool enable);

hdmi_video_timing drv_hdmi_video_timing_lookup(hdmi_video_code_vic vic, hdmi_picture_aspect aspect);

hdmi_video_timing drv_hdmi_vsif_video_timing_lookup(hdmi_vsif_vic vic);

td_s32 drv_hdmi_drm_infoframe_send(hdmi_infoframe_type *info_frame, td_bool enable);


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INFOFRAME_H_BODY_PART02_H_ */
