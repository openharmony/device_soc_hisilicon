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

#ifndef TRACK_SOURCE_H
#define TRACK_SOURCE_H
#include "ot_mw_type.h"

typedef enum {
    OT_TRACK_SOURCE_TYPE_PRIV = 0,
    OT_TRACK_SOURCE_TYPE_VIDEO,
    OT_TRACK_SOURCE_TYPE_AUDIO,
    OT_TRACK_SOURCE_TYPE_BUTT
} OT_Track_SourceType_E;

typedef enum {
    OT_TRACK_VIDEO_CODEC_H264 = 96,
    OT_TRACK_VIDEO_CODEC_H265 = 98,
    OT_TRACK_VIDEO_CODEC_MJPEG = 102,
    OT_TRACK_VIDEO_CODEC_BUTT
} OT_Track_VideoCodec_E;

typedef enum {
    OT_TRACK_AUDIO_CODEC_G711MU = 0,  /* G.711 Mu */
    OT_TRACK_AUDIO_CODEC_G711A = 8,   /* G.711 A */
    OT_TRACK_AUDIO_CODEC_LPCM = 23,   /* LPCM see ot_common.h file from sdk */
    OT_TRACK_AUDIO_CODEC_G726 = 97,   /* G.726 */
    OT_TRACK_AUDIO_CODEC_AMR = 101,   /* AMR encoder format */
    OT_TRACK_AUDIO_CODEC_ADPCM = 104, /* ADPCM */
    OT_TRACK_AUDIO_CODEC_AAC = 105,
    OT_TRACK_AUDIO_CODEC_WAV = 108, /* WAV encoder */
    OT_TRACK_AUDIO_CODEC_MP3 = 109,
    OT_TRACK_AUDIO_CODEC_BUTT
} OT_Track_AudioCodec_E;

typedef struct {
    OT_Track_VideoCodec_E enCodecType;
    TD_U32 u32Width;
    TD_U32 u32Height;
    TD_U32 u32BitRate;
    TD_FLOAT frameRate;
    TD_U32 u32Gop;
    TD_FLOAT fSpeed;
} OT_Track_VideoSourceInfo_S;

typedef struct {
    OT_Track_AudioCodec_E enCodecType;
    TD_U32 u32ChnCnt;
    TD_U32 u32SampleRate;
    TD_U32 u32AvgBytesPerSec;
    TD_U32 u32SamplesPerFrame;
    TD_U16 u16SampleBitWidth;
} OT_Track_AudioSourceInfo_S;

typedef struct {
    TD_U32 u32PrivateData;
    TD_FLOAT frameRate;
    TD_U32 u32BytesPerSec;
    TD_BOOL bStrictSync;
} OT_Track_PrivateSourceInfo_S;

typedef struct TrackSource OT_Track_Source_S;
typedef OT_Track_Source_S *OT_Track_Source_Handle;

typedef TD_S32 (*SS_Track_Source_Start_FN)(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller);
typedef TD_S32 (*SS_Track_Source_Stop_FN)(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller);
typedef TD_S32 (*SS_Track_Source_RequestKeyFrame_FN)(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller);

struct TrackSource {
    intptr_t s32PrivateHandle;  // venc/aenc/ handle
    SS_Track_Source_Start_FN pfnSourceStart;
    SS_Track_Source_Stop_FN pfnSourceStop;
    SS_Track_Source_RequestKeyFrame_FN pfnRequestKeyFrame;

    OT_Track_SourceType_E enTrackType;
    union {
        OT_Track_VideoSourceInfo_S stVideoInfo;  /** video track info */
        OT_Track_AudioSourceInfo_S stAudioInfo;  /** audio track info */
        OT_Track_PrivateSourceInfo_S stPrivInfo; /** private track info */
    } unTrackSourceAttr;
};

#endif
