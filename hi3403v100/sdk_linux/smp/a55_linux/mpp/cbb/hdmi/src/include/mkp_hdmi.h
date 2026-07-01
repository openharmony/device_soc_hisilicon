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
#ifndef MKP_HDMI_H
#define MKP_HDMI_H

#include "ot_common.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_common.h"
#include "drv_hdmi_infoframe.h"
#include "ot_common_hdmi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define hdmi_err_trace(fmt, ...)          \
            OT_ERR_TRACE(OT_ID_HDMI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define hdmi_warn_trace(fmt, ...)         \
            OT_WARN_TRACE(OT_ID_HDMI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define hdmi_info_trace(fmt, ...)         \
            OT_INFO_TRACE(OT_ID_HDMI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define hdmi_fatal_trace(fmt, ...)         \
            OT_EMERG_TRACE(OT_ID_HDMI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#ifdef HDMI_SUPPORT_DUAL_CHANNEL
#define HDMI_MPI_ID_MAX OT_HDMI_ID_BUTT
#else
#define HDMI_MPI_ID_MAX OT_HDMI_ID_1
#endif

typedef enum {
    HDMI_FORCE_NULL, /* Standard mode */
    HDMI_FORCE_HDMI, /* Forcibly output video in HDMI mode */
    HDMI_FORCE_DVI,  /* Forcibly output video in DVI mode */
    HDMI_FORCE_BUTT
} hdmi_force_active;

typedef struct {
    /* force to HDMI or DVI,the value must set before hdmi_start or behind hdmi_stop */
    td_bool enable_hdmi;
    td_bool enable_video; /* parameter must set TD_TRUE,or the HDMI diver will force to set TD_TRUE */
    /* HDMI output video mode VIDEO_MODE_YCBCR,VIDEO_MODE_YCBCR444,VIDEO_MODE_YCBCR422,VIDEO_MODE_RGB444 */
    ot_hdmi_video_mode vid_out_mode;
    ot_hdmi_deep_color deep_color_mode; /* deep color output mode,default: HDMI_DEEP_COLOR_24BIT */
    td_bool xvycc_mode;                 /* the xv_ycc output mode,default:TD_FALSE */
    td_bool enable_audio;               /* enable flag of audio */
    td_bool enable_avi_infoframe;       /* enable flag of AVI infoframe, suggestion:enable */
    td_bool enable_aud_infoframe;       /* enable flag of audio infoframe, suggestion:enable */
    td_bool enable_spd_infoframe;       /* enable flag of SPD infoframe, suggestion:disable */
    td_bool enable_mpeg_infoframe;      /* enable flag of MPEG infoframe, suggestion:disable */
    hdmi_force_active default_mode;  /* HDMI priority judgments strategy enum */
} hdmi_base_attr;

/* hdmi audio interface */
typedef enum {
    HDMI_AUDIO_INTERFACE_I2S,
    HDMI_AUDIO_INTERFACE_SPDIF,
    HDMI_AUDIO_INTERFACE_HBR,
    HDMI_AUDIO_INTERFACE_BUTT
} hdmi_audiointerface;

typedef struct {
    /* the origin of sound,suggestion set HDMI_AUDIO_INTERFACE_I2S,the parameter need consistent with ao input */
    hdmi_audiointerface sound_intf;
    td_bool is_multi_channel;        /* set muti_channel or stereo;0:stereo,1:mutichannel fixup 8 channel */
    td_u32 channels;
    ot_hdmi_sample_rate sample_rate; /* the samplerate of audio,this parameter consistent with AO config */
    td_u8 down_sample_parm;          /* PCM parameter of dowmsample,default 0 */
    ot_hdmi_bit_depth bit_depth;     /* the audio bit depth,default 16,this parameter consistent with AO config */
    td_u8 i2_s_ctl_vbit;             /* reserve:config 0,I2S control(0x7A:0x1D) */
    ot_hdmi_coding_type audio_code;
} hdmi_audio_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* MKP_HDMI_H */

