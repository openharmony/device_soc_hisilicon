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

#ifndef OT_DEMUXER_H
#define OT_DEMUXER_H

#include "ot_mw_type.h"

/** \addtogroup     PLAYER */
/** @{ */ /** <!-- [PLAYER] */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define OT_DEMUXER_NAME_LEN           20
#define OT_DEMUXER_SUPPORT_NAMES_LEN  56
#define OT_DEMUXER_RESOLUTION_CNT     5
#define OT_DEMUXER_NO_MEDIA_STREAM    (-1)

#define OT_DEMUXER_EMTRY_NAME "g_stFormatEntry"

#define OT_DEMUXER_SEEK_FORWARD   0
#define OT_DEMUXER_SEEK_BACKWARD  1
#define OT_DEMUXER_SEEK_BYTE      2

#define OT_RET_FILE_EOF  2

#define PLUGIN_URL_LEN 4096 /* Indicates the URL length. */

typedef enum {
    DATA_TYPE_NONE = 0, /* None */
    DATA_TYPE_SYNCFRAME = 1, /* Synchronization frame */
    DATA_TYPE_CODECCONFIG = 2, /* Codec configuration information */
    DATA_TYPE_EOS = 4, /* End of a single stream */
    DATA_TYPE_PARTIAL_FRAME = 8, /* Partial synchronization frame. This flag is used for multiple
                                  * segments (except for the last one) into which a single frame is divided.
                                  */
    DATA_TYPE_ENDOFFRAME = 16, /* End of a frame. This flag is used in pair with <b>DATA_FLAG_PARTIAL_FRAME</b> for
                                * the last segment of the frame.
                                */
    DATA_TYPE_MUXER_DATA = 32, /* Encapsulated data with its container */
} DataType;

typedef struct {
    int32_t (*ReadData)(void *handle, uint8_t *data, int32_t size, int32_t timeOutMs, DataType *flags);
    int32_t (*GetReadableSize)(const void *handle);
    void *handle; /* Context handle */
} DemuxerBufferStream;

typedef enum {
    DEMUXER_SOURCE_TYPE_FD = 0, /* File descriptor */
    DEMUXER_SOURCE_TYPE_URI, /* URI, which can be a network address or a local file path */
    DEMUXER_SOURCE_TYPE_STREAM, /* Streams */
    DEMUXER_SOURCE_TYPE_BUT /* Undefined source */
} DemuxerSourceType;

typedef struct {
    DemuxerSourceType type; /* Data source type. For details, see {@link DemuxerSourceType}. */
    union {
        int fd; /* Local file descriptor */
        char url[PLUGIN_URL_LEN]; /* Network URI or local file path. The maximum length is
                                   * specified by <b>URL_LEN</b>. */
        DemuxerBufferStream stream; /* Pointer to the function for reading buffer stream data */
    };
} DemuxerSource;

typedef enum {
    PACKET_TYPE_NONE,  /* Unknown type, which can be used for transport stream (TS) packets */
    PACKET_TYPE_AUDIO, /* Audio frame */
    PACKET_TYPE_VIDEO, /* Video frame */
    PACKET_TYPE_IMAGE, /* Image frame */
    PACKET_TYPE_SUB,   /* Subtitle frame */
    PACKET_TYPE_DATA,  /* Data */
    PACKET_TYPE_BUT,   /* Undefined type */
} PacketType;

typedef struct FORMAT_PACKET_S {
    PacketType packetType; /* Data frame type. For details, see {@link PacketType}. */
    TD_S64 s64Pts;         /* the time at which the decompressed packet will be presented to the user. */
    TD_S64 s64Dts;         /* the time at which the packet is decompressed.
                            * Can be AV_NOPTS_VALUE if it is not stored in the file
                            */
    TD_U8 *pu8Data;        /* the address of the packet. */
    TD_U32 u32Size;        /* the size of the packet. */
    TD_U8 *pu8Header;      /* Frame data header address */
    TD_U32 u32HeaderLen;   /* Frame data header length */
    TD_U32 u32StreamIndex; /* the index of the stream. */
    TD_BOOL bKeyFrame;     /* whether is the key frame. */
    TD_S64 s64Duration;    /* Duration of this packet in AVStream->time_base units, 0 if unknown.
                            * Equals next_pts - this_pts in presentation order.
                            */
    TD_S64 s64Pos;         /* byte position in stream, -1 if unknown. */
} OT_FORMAT_PACKET_S;

typedef struct FORMAT_MEDIA_ATTR_S {
    TD_S32 s32VidStreamId; /* Video stream ID. */
    TD_S32 s32AudStreamId; /* Audio stream ID. */
} OT_FORMAT_MEDIA_ATTR_S;

typedef enum FORMAT_VIDEO_TYPE_E {
    OT_FORMAT_VIDEO_TYPE_H264,
    OT_FORMAT_VIDEO_TYPE_H265,
    OT_FORMAT_VIDEO_TYPE_JPEG,
    OT_FORMAT_VIDEO_TYPE_MJPEG,
    OT_FORMAT_VIDEO_TYPE_WRAPPED_AVFRAME,  /* this type used for datastream */
    OT_FORMAT_VIDEO_TYPE_BUTT
} OT_FORMAT_VIDEO_TYPE_E;

typedef enum FORMAT_AUDIO_TYPE_E {
    OT_FORMAT_AUDIO_TYPE_AAC,
    OT_FORMAT_AUDIO_TYPE_PCM,
    OT_FORMAT_AUDIO_TYPE_G711A,
    OT_FORMAT_AUDIO_TYPE_G711U,
    OT_FORMAT_AUDIO_TYPE_MP3,
    OT_FORMAT_AUDIO_TYPE_BUTT
} OT_FORMAT_AUDIO_TYPE_E;

typedef enum {
    OT_DEMUXER_PROBE_MODE_NORMAL         = 0x0000,
    OT_DEMUXER_PROBE_MODE_GET_DATASTREAM = 0x0001  /* used to get data like thumbnail in.ts file */
} OT_DEMUXER_PROBE_MODE_TYPE_E;

typedef struct FORMAT_STREAM_RESOLUTION_S {
    TD_S32 s32VideoStreamIndex; /* the index of the video stream */
    TD_U32 u32Width;            /* The height of the media file's resolution */
    TD_U32 u32Height;           /* The width of the media file's resolution */
    TD_U32 codecTag;            /* The codectag of the stream */
    OT_FORMAT_VIDEO_TYPE_E enVideoType;
} OT_FORMAT_STREAM_RESOLUTION_S;

typedef struct FORMAT_FILE_INFO_S {
    TD_S64 s64FileSize;  /* File size, in the unit of byte. */
    TD_S64 s64StartTime; /* the media file begin time */
    TD_S64 s64Duration;  /* Total duration of a file, in the unit of ms. */
    TD_S64 audioDuration;  /* Total duration of a audiostream, in the unit of ms. */
    TD_S64 videoDuration;  /* Total duration of a videostream, in the unit of ms. */
    OT_FORMAT_STREAM_RESOLUTION_S stStreamResolution[OT_DEMUXER_RESOLUTION_CNT];
    TD_S32 s32UsedVideoStreamIndex; /* <default used video index> */
    TD_FLOAT fFrameRate;            /* the frame rate of the stream */
    TD_U32 u32Bitrate;              /* File bit rate, in the unit of bit/s. */
    TD_U32 u32AudioChannelCnt;
    TD_U32 u32SampleRate;           /* the sample rate of the audio stream */
    TD_S32 s32UsedAudioStreamIndex; /* the index of the audio stream. one file may have many audio streams */
    OT_FORMAT_VIDEO_TYPE_E enVideoType;
    OT_FORMAT_AUDIO_TYPE_E enAudioType;
} OT_FORMAT_FILE_INFO_S;

typedef struct FORMAT_FUN_S {
    TD_S32 (*fmt_find)(const DemuxerSource *source);
    TD_S32 (*fmt_open)(TD_VOID **ppFmt, const DemuxerSource *source);
    TD_S32 (*fmt_close)(const TD_VOID *pFmt);
    TD_S32 (*fmt_read)(const TD_VOID *pFmt, OT_FORMAT_PACKET_S *pstFmtFrame);
    TD_S32 (*fmt_free)(const TD_VOID *pFmt, const OT_FORMAT_PACKET_S *pstFmtFrame);
    TD_S32 (*fmt_seek)(const TD_VOID *pFmt, TD_S32 s32StreamIndex, TD_S64 s64MSec, TD_S32 s32Flag);
    TD_S32 (*fmt_probe)(const TD_VOID *pFmt, OT_DEMUXER_PROBE_MODE_TYPE_E demuxProbeMode);
    TD_S32 (*fmt_getinfo)(const TD_VOID *pFmt, OT_FORMAT_FILE_INFO_S *pstFmtInfo);
    TD_S32 (*fmt_setattr)(const TD_VOID *pFmt, OT_FORMAT_MEDIA_ATTR_S *pstFmtInfo);
} OT_FORMAT_FUN_S;

typedef struct DEMUX_S {
    TD_CHAR aszDemuxerName[OT_DEMUXER_NAME_LEN];            /* the demuxer's name */
    TD_CHAR aszSupportFormat[OT_DEMUXER_SUPPORT_NAMES_LEN]; /* File formats supported by the DEMUX.
                                                             * The file formats are separated by commas (,).
                                                             */
    OT_FORMAT_FUN_S stFmtFun;
    TD_U32 u32Priority; /* the priority of the demuxers. the lower value, the higher priority. range from 0-10 */
} OT_DEMUX_S;

/* PLAYER End */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
