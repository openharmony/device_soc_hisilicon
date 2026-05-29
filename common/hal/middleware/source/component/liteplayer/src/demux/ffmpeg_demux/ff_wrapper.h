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

#ifndef FF_WRAPPER_H
#define FF_WRAPPER_H

#include <pthread.h>
#include "libavformat/avformat.h"
#include "ot_mw_type.h"
#include "ss_demuxer.h"
#include "libavcodec/bsf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define UNUSED_ATTR __attribute__((unused))

/* 30720 = 30 * 1024 = 30k */
#define FF_AAC_BUFFER_LEN 30720

typedef enum {
    OT_FFDEMUX_FORMAT_MP4 = 0,
    OT_FFDEMUX_FORMAT_TS,
    OT_FFDEMUX_FORMAT_WAV,
    OT_FFDEMUX_FORMAT_M4A,
    OT_FFDEMUX_FORMAT_BUTT
} FfDemuxFormat;

typedef struct {
    pthread_mutex_t mutex;
    TD_S64 fileSize;

    AVFormatContext *formatContext;
    AVPacket *pkt;
    TD_S64 lastReadPts; /* unit AV_TIME_BASE */
    FfDemuxFormat formatType;
    AVBSFContext *h26XBsfCtx;
    int64_t timeoutStart;

    AVStream *videoStream;
    enum AVCodecID vidCodecId;
    TD_S32 videoStreamIndex;
    TD_U32 width;
    TD_U32 height;
    TD_S32 videoStreamNum;
    OT_FORMAT_STREAM_RESOLUTION_S videoResolution[OT_DEMUXER_RESOLUTION_CNT];
    TD_BOOL foundFirstIdrFrame;

    AVStream *audioStream;
    enum AVCodecID audioCodeId;
    TD_S32 audioStreamIndex;
    TD_U32 sampleRate;
    TD_U32 chnnum;
    TD_U8 aacBuf[FF_AAC_BUFFER_LEN]; /* store aac contains adts header */
} FfDemuxContext;

TD_S32 FfOpen(FfDemuxContext *demux, const TD_CHAR* fileName, TD_U32 fileNameLen);
TD_VOID FfClose(FfDemuxContext *demux);
TD_S32 FfReadPacket(FfDemuxContext *demux, AVPacket *pkt);
TD_S32 FfFreePacket(FfDemuxContext *demux, AVPacket *pkt);
TD_VOID FfCheckPacketToBeFree(FfDemuxContext *demux, const OT_FORMAT_PACKET_S *fmtFrame);
TD_S32 FfSeek(FfDemuxContext *demux, TD_S32 streamIndex, TD_S64 toMs, TD_S32 flag);
TD_S32 FfProbe(FfDemuxContext *demux, OT_DEMUXER_PROBE_MODE_TYPE_E probeMode);
TD_S32 FfProbeVideo(FfDemuxContext *demux, TD_BOOL *hasVideo);
TD_S32 FfProbeAudio(FfDemuxContext *demux, TD_BOOL *hasAudio);
TD_S32 FfGetinfo(const FfDemuxContext *demux, OT_FORMAT_FILE_INFO_S *fmtInfo);
TD_VOID FfDicardStream(AVStream *stream);
TD_S32 FfSetUsedAudioStrem(FfDemuxContext *demux, TD_S32 id, TD_BOOL *isStreamChanged);
TD_S32 FfSetUsedVideoStrem(FfDemuxContext *demux, TD_S32 id, TD_BOOL *isStreamChanged);
TD_S32 FfGetFileSize(const FfDemuxContext *demux, TD_S64 *outFileSize);
TD_S32 FfGetFormatTypByName(const TD_CHAR *formatName, TD_U32 nameLen, FfDemuxFormat *type);
TD_S32 FfVideoCodecIdToType(enum AVCodecID avCodec, OT_FORMAT_VIDEO_TYPE_E *videoType);
TD_S32 FfAudioCodecIdToType(enum AVCodecID avCodec, OT_FORMAT_AUDIO_TYPE_E *audioType);
TD_S32 FfAvBsfInit(AVBSFContext **outBsfCtx, const AVStream *stream, const TD_CHAR *bsfName);
TD_VOID FfAvBsfDeinit(AVBSFContext **bsfCtx);
TD_S32 FfFilterPacket(AVPacket *pkt, AVBSFContext *avBsfCtx);
TD_VOID FfGenFrameByAvPacket(FfDemuxContext *demux, OT_FORMAT_PACKET_S *fmtFrame);
TD_BOOL FfHasVideo(const FfDemuxContext *demux);
TD_BOOL FfHasAudio(const FfDemuxContext *demux);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* FF_WRAPPER_H */
