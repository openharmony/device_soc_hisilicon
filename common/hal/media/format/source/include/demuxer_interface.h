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

#ifndef DEMUXER_INTERFACE_H
#define DEMUXER_INTERFACE_H

#include "protocol_interface.h"
#include "plugin_common.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/* define demuxer */
#define DEMUXER_MP4 "mp4"
#define DEMUXER_FMP4 "fmp4"
#define DEMUXER_FMP4 "fmp4"
#define DEMUXER_AC3 "ac3"
#define DEMUXER_AAC "aac"

#define LANGUAGE_NUM_MAX 4    /* Indicates the number of languages in the subtitle file. */
#define LANGUAGE_LEN_MAX 4        /* Indicates the number of characters contained in the language description. */

typedef enum {
    DEMUXER_SEEK_MODE_FORWARD_KEY,  /* Seeks forwards for the keyframe closest to specified position. */
    DEMUXER_SEEK_MODE_BACKWARD_KEY, /* Seeks backwards for the keyframe closest to specified position. */
    DEMUXER_SEEK_MODE_CLOSEST_KEY,  /* Seeks for the keyframe closest to specified position. */
    DEMUXER_SEEK_MODE_BUT                  /* Undefined mode */
} DemuxerSeekMode;

typedef enum {
    DEMUXER_INVOKE_GET_PICTURE,
    DEMUXER_INVOKE_GET_GPSINFO,
    DEMUXER_INVOKE_BUT
} DemuxerInvokeType;

typedef struct {
    CodecType format; /* Audio encoding format. For details, see {@link CodecType}. */
    uint32_t profile; /* Audio encoding profile, for example, <b>0x160(WMAV1)</b> and <b>0x161 (WMAV2)</b>. */
    uint32_t sampleRate; /* Audio sampling rate, for example, <b>8000</b>, <b>16000</b>, <b>24000</b>, <b>32000</b>,
                          * <b>11025</b>, <b>22050</b>, <b>441000</b>, and <b>48000</b>
                          */
    uint16_t sampleFmt; /* Data storage format of one sample in an audio channel.
                         * For details, see {@link AudioSampleFmt}.
                         */
    uint16_t channels; /* Number of audio channels */
    int32_t subStreamID; /* ID of the subsidiary audio stream, which is used when the encoding or decoding format
                          * of the primary stream is not supported for audio rendering.
                          */
    uint32_t bitrate; /* Audio and video bit rate */
    char language[LANGUAGE_NUM_MAX]; /* Audio track language */
    int64_t durationMs; /* Stream duration, in milliseconds */
} AudioTrack;

typedef struct {
    CodecType format; /* Video encoding format. For details, see {@link CodecType}. */
    uint16_t profile; /* Profile */
    uint16_t width; /* Width, in pixels */
    uint16_t height; /* Height, in pixels */
    uint16_t fpsNum; /* Numerator of the frame rate */
    uint16_t fpsDen; /* Denominator of the frame rate */
    uint32_t bitrate; /* <Video bit rate, in bit/s */
    uint32_t rotate; /* Video rotation angle. The value can be <b>90</b>, <b>180</b>, or <b>270</b>.
                      * The default value is <b>0</b>.
                      */
    int64_t durationMs; /* Track duration, in milliseconds */
}VideoTrack;

typedef struct {
    CodecType format; /* Image encoding format. For details, see {@link CodecFormat}. */
    uint16_t width; /* Width, in pixels */
    uint16_t height; /* Height, in pixels */
    uint32_t rotate; /* Image rotation angle. The value can be <b>90</b>, <b>180</b>, or <b>270</b>.
                      * The default value is <b>0</b>.
                      */
    uint32_t dataLength; /* Image size */
    bool thumbnail; /* Thumbnail flag */
} SubImage;

typedef struct {
    int32_t subImageNum;        /* Number of sub-images */
    SubImage *subImageInfo; /* Detailed information about a sub-image */
} ImageTrack;

typedef struct {
    SubtitleFormatType format; /* Subtitle encoding and decoding format. For details, see {@link SubtitleFormat} */
    uint32_t charSet; /* Character encoding format */
    char language[LANGUAGE_NUM_MAX][LANGUAGE_LEN_MAX]; /* Subtitle language */
    uint16_t originalFrameWidth; /* Width of the original image. This variable is valid for the image subtitle. */
    uint16_t originalFrameHeight; /* Height of the original image. This variable is valid for the image subtitle. */
} SubtitleTrack;

typedef enum {
    MEDIA_TRACK_TYPE_VIDEO, /* Video track */
    MEDIA_TRACK_TYPE_AUDIO, /* Audio track */
    MEDIA_TRACK_TYPE_IMAGE, /* Image track */
    MEDIA_TRACK_TYPE_SUB, /* Subtitle track */
    MEDIA_TRACK_TYPE_DATA, /* Data track */
    MEDIA_TRACK_TYPE_BUT, /* Undefined track */
} MediaTrackType;

typedef struct {
    MediaTrackType trackType; /* Track type. For details, see {@link MediaTrackType} */
    int32_t trackId; /* Track index */
    union {
        VideoTrack vidTrack; /* Video track information */
        AudioTrack audTrack; /* Audio track information */
        SubtitleTrack subTrack; /* Subtitle track information */
        ImageTrack imgTrack; /* Image track information */
    };
    int32_t itemCnt; /* Number of extra track information entries */
    OptionalItem *item; /* Pointer to the array of extra track information, including HDR, DRM, and CAS */
} MediaTrackInfo;

typedef struct {
    int32_t programId;  /* Program index */
    int64_t durationMs; /* Program duration */
    uint32_t trackNum;  /* Number of tracks of the current program */
    MediaTrackInfo *track;   /* Pointer to the array of current program information */
} MediaProgramInfo;

typedef struct {
    char *formatName; /* Pointer to the name of the movie file container format */
    uint32_t bitrate; /* Bit rate of the movie file */
    uint32_t programNum; /* Number of programs */
    MediaProgramInfo *programInfo; /* Pointer to the array of information about the movie file program */
} MediaFileInfo;

typedef struct {
    PluginDesc desc;
    /* function */
    int (*fmt_find)(const DemuxerSource *source, const ProtocolFun *protocol);
    int (*fmt_open)(const DemuxerSource *source, const ProtocolFun *protocol, void **handle, int *score);
    int (*fmt_find_stream)(void *handle, void *arg);
    int (*fmt_read)(void *handle, FormatPacket *frame);
    int (*fmt_free)(void *handle, FormatPacket *frame);
    int (*fmt_seek_pts)(void *handle, int trackId, int64_t pts, uint32_t flag);
    int (*fmt_seek_pos)(void *handle, int64_t pos, uint32_t whence);
    int (*fmt_getinfo)(void *handle, MediaFileInfo *info);
    int (*fmt_invoke)(void *handle, uint32_t invokeId, void *arg);
    int (*fmt_close)(void *handle);
    int (*fmt_select_track)(void *handle, int32_t programId, int32_t trackId);
} DemuxerPluginEntry;

typedef const DemuxerPluginEntry *(*GetDemuxerFun)(void);
const DemuxerPluginEntry *GetDemuxer(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // DEMUXER_INTERFACE_H
