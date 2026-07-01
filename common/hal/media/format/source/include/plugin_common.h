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


#ifndef PLUGINS_COMMON_H
#define PLUGINS_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#if defined(ENABLE_H9) || defined(ENABLE_H8)
#include "ss_demuxer.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MIN_FILE_SURFIX_LEN  2
#define MAX_FILE_SURFIX_LEN  32
#define PLUGIN_URL_LEN 4096 /* Indicates the URL length. */

typedef struct {
    uint32_t codecId;
    uint8_t *data;
    int32_t size;
} AttachPicInfo;

typedef struct {
    int32_t vidStreamId; /* Video stream ID. */
    int32_t audStreamId; /* Audio stream ID. */
} FormatMediaParam;

typedef enum {
    INVOKE_SET_CALLBACK, /* 设置回调函数 */
    INVOKE_SET_SPIT_CONFIG, /* 设置紧急分片属性 */
    INVOKE_SET_BUFFER_CONFIG, /* 设置网络缓存信息 */
    INVOKE_SET_LOCATION, /* 设置位置信息 */
    INVOKE_SET_DEGREE, /* 设置角度信息 */
    INVOKE_SET_LOG_LEVEL, /* 设置日志级别 */
    INVOKE_SET_REC_PRE_CACHE,
    INVOKE_SET_HEADERS,
    INVOKE_SET_MANNUAL_SPIT,
    INVOKE_SET_HTTP_DOWNLOAD_SIZE_ONCE,
    INVOKE_GET_MSG_EVENT,
    INVOKE_SET_RINGBUFFER_SIZE,
    INVOKE_WRITE_FRONT_COVER,
    INVOKE_WRITE_FRONT_GPS,
    INVOKE_SET_PREALLOC_SIZE
} FormatPluginInvokeId;

typedef enum {
    SPLIT_TYPE_POST = 0, /* Search forwards from the current I-frame and split the file at the closest
                                                               I-frame. */
    SPLIT_TYPE_PRE,      /* Search backwards from the current I-frame and split the file at the closest I-frame. */
    SPLIT_TYPE_NORMAL    /* Normal split */
} SplitType;

typedef struct {
    int32_t latitude;
    int32_t longitude;
} Location;

typedef struct {
    uint8_t *frameAddr;
    int32_t len;
} FrontCover;

typedef struct {
    SplitType type;
    int64_t timestampUs;
    uint32_t durationS;
} MannualSpit;

typedef enum {
    OPTIONAL_KEY_TYPE_PRE_CACHE = 0x70726361,  /* Precapture cache size(prca). The value type is <b>int</b> */
    OPTIONAL_KEY_TYPE_RINGBUFFER_SIZE_IN_MSEC = 0x70726362, /* set ringbuffer size. The value type is uint */
    OPTIONAL_KEY_TYPE_EXTRADATA = 0x65787472
} OptionalKeyType;

/**
 * @brief Defines a structure for configuring and obtaining dynamic parameters.
 */
typedef struct {
    uint32_t key; /* Key type. For details, see {@link OptionalKeyType} */
    int32_t size; /* Data size, which indicates the size of the buffer memory pointed by <b>value</b> */
    /**
     * @brief Defines value types of the parameters.
     */
    union {
        int32_t s32Value;  /* 32-bit integer */
        uint32_t u32Value; /* 32-bit unsigned integer */
        int64_t s64Value;  /* 64-bit integer */
        uint64_t u64Value; /* 64-bit unsigned integer */
        float fValue;      /* Single-precision floating point number */
        double dValue;      /* Double-precision floating point number */
        const void *value; /* Pointer */
    } value;
} OptionalItem;

typedef struct {
    PacketType packetType; /* Data frame type. For details, see {@link PacketType}. */
    uint32_t trackId;    /* Index of the track where the data frame is located */
    bool isKeyFrame;     /* Keyframe flag. <b>false</b>: The data frame is not a keyframe.
                          * <b>true</b>: The data frame is a keyframe.
                          */
    int64_t timestampUs; /* Timestamp of a data frame, in us */
    int64_t durationUs;  /* Data frame duration, in us */
    uint8_t *data;       /* Address of the data frame buffer */
    uint32_t len;        /* Data frame length */
    uint32_t frameIndex; /* Data frame index. For the MPF container, the value indicates
                          * the index of the sub-image frame.
                          */
    int64_t position;    /* Position of the data frame in the file */
    int32_t itemCnt;     /* Number of parameters, which can be used for information such as side data,
                          * PSSH, DRM, and HDR.
                          */
    OptionalItem *item; /* Pointer to the parameter array */
} FormatPacket;

typedef enum {
    CODEC_TYPE_H264 = 0, /* H264 or AVC */
    CODEC_TYPE_H265,     /* H265 or HEVC */
    CODEC_TYPE_JPEG,     /* JPEG */
    CODEC_TYPE_AAC,      /* AAC */
    CODEC_TYPE_G711A,    /* G711A */
    CODEC_TYPE_G711U,    /* G711u */
    CODEC_TYPE_PCM,      /* PCM */
    CODEC_TYPE_MP3,      /* MP3 */
    CODEC_TYPE_G726,     /* G726 */
    CODEC_TYPE_OPUS,     /* OPUS */
    CODEC_TYPE_FLAC,     /* FLAC */
    CODEC_TYPE_VORBIS,   /* VORBIS */
    CODEC_TYPE_APE,      /* APE */
    CODEC_TYPE_BUT,      /* Undefined format */
} CodecType;

typedef enum {
    FORMAT_SUB_TYPE_ASS = 0x0, /* ASS */
    FORMAT_SUB_TYPE_LRC,       /* LRC */
    FORMAT_SUB_TYPE_SRT,       /* SRT */
    FORMAT_SUB_TYPE_SMI,       /* SMI */
    FORMAT_SUB_TYPE_SUB,       /* SUB */
    FORMAT_SUB_TYPE_TXT,       /* RAW UTF-8 */
    FORMAT_SUB_TYPE_HDMV_PGS,  /* HDMV PGS */
    FORMAT_SUB_TYPE_DVB_SUB,   /* DVB */
    FORMAT_SUB_TYPE_DVD_SUB,   /* DVD */
    FORMAT_SUB_TYPE_TTML,      /* TTML */
    FORMAT_SUB_TYPE_WEBVTT,    /* WebVTT */
    FORMAT_SUB_TYPE_BUTT       /* Undefined format */
} SubtitleFormatType;

typedef enum {
    AUDIO_SAMPLE_FORMAT_S8 = 0, /* 8-bit integer for a single sample */
    AUDIO_SAMPLE_FORMAT_S16,    /* 16-bit integer for a single sample */
    AUDIO_SAMPLE_FORMAT_S24,    /* 24-bit integer for a single sample */
    AUDIO_SAMPLE_FORMAT_S32,    /* 32-bit integer for a single sample */
    AUDIO_SAMPLE_FORMAT_FLOAT,  /* Single-precision floating point number for a single sample */
} AudioSampleFormat;

typedef enum {
    FORMAT_TYPE_MPEG_4 = 0, /* MP4 */
    FORMAT_TYPE_TS = 1, /* TS */
    FORMAT_TYPE_THREE_GPP = 2, /* 3GPP */
    FORMAT_TYPE_HEIF = 3, /* HEIF */
    FORMAT_TYPE_OGG = 4, /* Ogg */
    FORMAT_TYPE_M4A = 5, /* M4A */
    FORMAT_TYPE_INVALID /* Invalid format */
} FormatType;

typedef enum {
    ERROR_TYPE_CREATE_FILE_FAIL = 0,      /* create file failed */
    ERROR_TYPE_WRITE_FILE_FAIL,           /* write file failed */
    ERROR_TYPE_CLOSE_FILE_FAIL,           /* close file failed */
    ERROR_TYPE_READ_DATA_ERROR,           /* rec internal operation fail, must stop rec */
    ERROR_TYPE_INTERNAL_OPERATION_FAIL,   /* rec internal operation fail, must stop rec */
    ERROR_TYPE_UNKNOWN                    /* Unknown error */
} PluginErrorType;

typedef enum {
    MAX_DURATION_APPROACHING = 0, /* The capturing duration is reaching the threshold specified by
                                              * {@link FormatMuxerSetMaxFileDuration}. This information is reported
                                              * when only one second or 10% is left to reach the allowed duration.
                                              */
    MAX_FILESIZE_APPROACHING,     /* The captured file size is reaching the threshold specified by
                                              * {@link FormatMuxerSetMaxFileSize}. This information is reported when
                                              * only 100 KB or 10% is left to reach the allowed size.
                                              */
    MAX_DURATION_REACHED,         /* The capturing duration reached the threshold specified by
                                              * {@link FormatMuxerSetMaxFileDuration}, and the capturing is ended.
                                              * If the file is set by a file descriptor, the caller needs to close
                                              * the file.
                                              */
    MAX_FILESIZE_REACHED,         /* The captured file size reached the threshold specified by
                                              * {@link FormatMuxerSetMaxFileSize}, and the capturing is ended.
                                              * If the file is set by a file descriptor, the caller needs to
                                              * close the file.
                                              */
    NEXT_OUTPUT_FILE_STARTED,     /* The capturing started for the next output file. */
    FILE_SPLIT_FINISHED,          /* Manual file split is completed. */
    FILE_START_TIME_MS,           /* Start time of the captured file */
    NEXT_FILE_FD_NOT_SET,         /* this event will be reported when next fd is needed but not set. */
    NO_FRAME_DATA,                /** this event will be reported when no frame data is sent to recorder. */

    DEMUXER_PREPARED = 10000,           /* The prepare function is asynchronously executed. This information
                                              * is reported after the execution is complete.
                                              */
    DEMUXER_SEEK_COMPLETE,              /* The seek function is asynchronously executed. This information is
                                              * reported after the execution is complete.
                                              */
    DEMUXER_NETWORK_DISCONNECTED,       /* The network is disconnected during network playback. */
    DEMUXER_NETWORK_RECONNECTED,        /* The network is automatically reconnected during network playback. */
} PluginInfoType;


typedef void *CallbackHandle;
typedef struct {
    CallbackHandle handle;
    int32_t (*OnError)(CallbackHandle handle, int32_t errorType, int32_t errorCode);
    int32_t (*OnInfo)(CallbackHandle handle, int32_t type, int32_t extra);
} PluginCallback;

typedef struct {
    char *libName; /* name, for demuxer, it should be libplugin_demuxer_xxx.so.
                                      for muxer, it should be libplugin_muxer_xxx.so
                                      for protocol, it should be libplugin_protocol_xxx.so */
    char *name;
    char *version;
    char *description;
    char *supportCapability;
    int priority;
} PluginDesc;

typedef struct {
    int (*protocol_find)(const char *url);
    int (*protocol_open)(const char *url, int flags, void **handle);
    int (*protocol_seek)(void *handle, int64_t pos, int whence);
    int (*protocol_seek_stream)(void *handle, int32_t streamId, int64_t pts, uint32_t flags);
    int (*protocol_read)(void *handle, unsigned char *buf, int size);
    int (*protocol_write)(void *handle, const unsigned char *buf, int size);
    int (*protocol_invoke)(void *handle, uint32_t invokeId, void *arg);
    int (*protocol_close)(void *handle);
} ProtocolFun;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // PLUGINS_COMMON_H
/** @} */
