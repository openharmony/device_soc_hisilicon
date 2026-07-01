/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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

#ifndef MUXER_INTERFACE_H
#define MUXER_INTERFACE_H

#include "plugin_common.h"
#include "protocol_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef void *RecMuxerHandle;

typedef enum {
    OUT_TYPE_FD = 0, /* File descriptor */
    OUT_TYPE_URI,    /* Local file URI */
    OUT_TYPE_INVALID /* Undefined type */
} OutType;

typedef struct {
    OutType type;        /* Output type. For details, see {@link OutputType}. */
    union {
        int32_t fd;         /* File descriptor */
        char url[PLUGIN_URL_LEN];  /* File path. For details, see {@link URL_LEN}. */
    };
    FormatType format;    /* File format */
} MuxerOutput;

typedef enum {
    STREAM_TYPE_VIDEO = 0, /* Video track */
    STREAM_TYPE_AUDIO, /* Audio track */
    STREAM_TYPE_DATA, /* Data track */
    STREAM_TYPE_INVALID /* Invalid type */
} StreamType;

typedef struct {
    CodecType codecType; /* Video encoding type, for details, see {@link CodecType}. */
    uint32_t width; /* Video width */
    uint32_t height; /* Video height */
    uint32_t bitRate; /* Encoding bit rate, in bit/s */
    uint32_t frameRate; /* Encoding frame rate */
    float speed; /* Video speed */
    uint32_t keyFrameInterval; /* Keyframe interval */
} VideoInfo;

typedef struct {
    CodecType codecType; /* Audio encoding type, for details, see {@link CodecType}. */
    uint32_t sampleRate; /* Sampling rate */
    uint32_t channelCount; /* Number of audio channels */
    AudioSampleFormat sampleBitWidth; /* Bit width */
    uint32_t samplesPerFrame; /* Number of samples per frame */
    uint32_t avgBytesPerSec; /* Average bit rate, in byte/s */
} AudioInfo;

typedef struct {
    uint32_t frameRate; /* Frame rate */
    uint32_t bitRate; /* Bit rate */
} DataInfo;

typedef struct {
    StreamType type; /* Stream source type. For details, see {@link StreamType} */
    union {
        VideoInfo videoInfo; /* Video stream. For details, see {@link VideoInfo}. */
        AudioInfo audioInfo; /* Audio stream. For details, see {@link AudioInfo}. */
        DataInfo dataInfo;   /* Data stream. For details, see {@link DataInfo} */
    } info;
} MuxerTrackSource;

typedef struct {
    PluginDesc desc;

    int (*fmt_find)(const MuxerOutput *output, const ProtocolFun *protocol);
    int (*fmt_open)(const MuxerOutput *output, const ProtocolFun *protocol, RecMuxerHandle *handle);
    int (*fmt_add_track)(RecMuxerHandle handle, const MuxerTrackSource *trackSource);
    int (*fmt_start)(RecMuxerHandle handle); /* 可以 write_frame 触发 */
    int (*fmt_write)(RecMuxerHandle handle, const FormatPacket *frameData);
    int (*fmt_stop)(RecMuxerHandle handle, bool block); /* fmt_write_trail */
    int (*fmt_set_max_size)(RecMuxerHandle handle, int64_t byte);
    int (*fmt_set_max_duration)(RecMuxerHandle handle, int64_t byte);
    int (*fmt_set_next_outputfile)(RecMuxerHandle handle, int fd);
    int (*fmt_invoke)(RecMuxerHandle handle, uint32_t invokeId, void *arg);
    int (*fmt_close)(RecMuxerHandle handle);
} MuxerPluginEntry;

typedef const MuxerPluginEntry *(*GetMuxerFun)(void);
const MuxerPluginEntry *GetMuxer(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // MUXER_INTERFACE_H
