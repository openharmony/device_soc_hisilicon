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
 *
 * Description: hidemuxer module header file
 */

#ifndef DEMUXER_COMMON_H
#define DEMUXER_COMMON_H

#include "plugin_common.h"

/** \addtogroup     PLAYER */
/** @{ */ /** <!-- [PLAYER] */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_DEMUXER_RESOLUTION_CNT     5
#define HI_DEMUXER_NO_MEDIA_STREAM    (-1)

#define HI_DEMUXER_EMTRY_NAME "g_stFormatEntry"

#define HI_RET_FILE_EOF  2
#define HI_RET_NODATA  3


typedef struct {
    int32_t s32VideoStreamIndex; /* the index of the video stream */
    uint32_t u32Width;            /* The height of the media file's resolution */
    uint32_t u32Height;           /* The width of the media file's resolution */
    CodecType enVideoType;
} StreamResolution;

typedef struct {
    char *formatName;  /**< File format name, in the unit of byte. */
    int64_t s64FileSize;  /* File size, in the unit of byte. */
    int64_t s64StartTime; /* the media file begin time */
    int64_t s64Duration;  /* Total duration of a file, in the unit of ms. */
    StreamResolution stStreamResolution[HI_DEMUXER_RESOLUTION_CNT];
    int32_t s32UsedVideoStreamIndex; /* <default used video index> */
    float fFrameRate;            /* the frame rate of the stream */
    uint32_t u32Bitrate;              /* File bit rate, in the unit of bit/s. */
    uint32_t u32AudioChannelCnt;
    uint32_t u32SampleRate;           /* the sample rate of the audio stream */
    int32_t s32UsedAudioStreamIndex; /* the index of the audio stream. one file may have many audio streams */
    uint32_t u32Width;            /* The height of the media file's resolution */
    uint32_t u32Height;           /* The width of the media file's resolution */
    CodecType enVideoType;
    CodecType enAudioType;
} FormatFileInfo;

/** @} */ /** <!-- ==== PLAYER End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
