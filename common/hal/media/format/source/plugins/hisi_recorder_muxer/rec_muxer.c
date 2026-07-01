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
 * Description: format_muxer.c
 */

#include <malloc.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/prctl.h>
#include <unistd.h>
#include "dtcf_manager.h"
#include "ot_mw_type.h"
#include "ot_track_source.h"
#include "ss_recorder_pro.h"
#include "media_hal_common.h"
#include "muxer_interface.h"
#include "rec_buffer_manager.h"
#include "rec_muxer_manager.h"
#include "securec.h"
#define MODULE_NAME "HisiMuxer"
td_u32 g_streamCnt;

#define FORMAT_ERR_NULL_PTR 0x02       /**<  using a NULL point */
#define FORMAT_ERR_NOFREE_CHANNEL (0x03)
#define FORMAT_ERR_NO_AVAILABLE_STREAM_ID 0x04
#define FORMAT_ERR_NOT_CREATED 0x05
#define FORMAT_ERR_INVALID_PARAM 0x06
#define FORMAT_ERR_NOT_STARTED 0x07
#define FORMAT_ERR_NOT_SUPPORT 0x08
#define INVALID_STREAM_INDEX 0xffffffff

#define HISI_MUXER_PRIORITY 100
#define MUXER_BITRATE_WEIGHTS 1024
#define MUXER_BUFFER_TIME_IN_MSEC (5 * 1000)

#define MUXER_AUDIO_SAMPLE_S8 8
#define MUXER_AUDIO_SAMPLE_S16 16
#define MUXER_AUDIO_SAMPLE_S24 24
#define MUXER_AUDIO_SAMPLE_S32 32

#define TIME_TO_WAIT_S 1
#define US_TO_NSEC 1000
#define SEC_TO_MS 1000
#define TIME_UNIT 1000
#define REC_BUFFER_TIME  3
#define MUXER_PRE_ALLOC_SIZE_100 100


/** bit31:16 reserve, bit15:8 streamIndex, bit7:0 trackIndex */
#define TRACK_ID(streamIndex, trackIndex) \
    ((unsigned int)(((streamIndex) << 8) | (trackIndex)))

#define SREAM_INDEX(trackid) \
    ((unsigned int)(((trackid) >> 8) & 0xFF))

#define TRACK_INDEX(trackid) \
    ((unsigned int)((trackid) & 0xFF))

#define REC_MUXER_HANDLE_MAX_CNT (6)
#define REC_MUXER_SPLIT_TIME_IN_SEC 60

#define PARAM_MAX_LEN 100
typedef struct {
    bool used;
    MuxerOutput muxerOutput;
    MuxerConfig muxerConfig;
    OT_REC_ATTR_S recAttr;
    TD_MW_PTR hRecHandle;
    bool   recStarted;
    bool   recStoping;
    bool   recError;
    int degrees;
    int latitude;
    int longitude;
    int64_t maxFileSize;
    int64_t maxDuration;
    PluginCallback callBack;
    bool firstNewFileBegin;
    struct timeval timeToStop;
    pthread_cond_t contextCond;
    pthread_mutex_t condMutex;
    pthread_mutex_t contextMutex;
} RecMuxerContext;

typedef struct {
    bool inited;
    pthread_mutex_t mutex;
    RecMuxerContext ctx[REC_MUXER_HANDLE_MAX_CNT];
} RecMuxerManager;

static RecMuxerManager g_recMuxerManager = {
    .inited = false,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
};

static int32_t RecMuxerInit(void)
{
    RecMuxerManager *recMgr = &g_recMuxerManager;
    MEDIA_HAL_LOCK(recMgr->mutex);
    if (recMgr->inited) {
        MEDIA_HAL_UNLOCK(recMgr->mutex);
        return MEDIA_HAL_OK;
    }
    if (memset_s(recMgr->ctx, sizeof(RecMuxerContext) * REC_MUXER_HANDLE_MAX_CNT, 0x00,
        sizeof(RecMuxerContext) * REC_MUXER_HANDLE_MAX_CNT) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        MEDIA_HAL_UNLOCK(recMgr->mutex);
        return MEDIA_HAL_ERR;
    }
    recMgr->inited = true;
    MEDIA_HAL_UNLOCK(recMgr->mutex);
    return MEDIA_HAL_OK;
}

static RecMuxerContext *RecMuxerManagerAllocMuxer(void)
{
    uint32_t idx;
    RecMuxerContext *ctx = NULL;
    RecMuxerManager *muxerMgr = &g_recMuxerManager;
    MEDIA_HAL_LOCK(muxerMgr->mutex);
    for (idx = 0; idx < REC_MUXER_HANDLE_MAX_CNT; idx++) {
        if (muxerMgr->ctx[idx].used) {
            continue;
        }

        ctx = &muxerMgr->ctx[idx];
        if (memset_s(ctx, sizeof(RecMuxerContext), 0, sizeof(RecMuxerContext)) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        }
        ctx->used = true;
        break;
    }
    if (idx == REC_MUXER_HANDLE_MAX_CNT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "no idle muxer, max:%d", REC_MUXER_HANDLE_MAX_CNT);
    }
    MEDIA_HAL_UNLOCK(muxerMgr->mutex);
    return ctx;
}

static void RecMuxerManagerFreeMuxer(RecMuxerContext *ctx)
{
    uint32_t idx;
    RecMuxerManager *muxerMgr = &g_recMuxerManager;
    MEDIA_HAL_LOCK(muxerMgr->mutex);
    for (idx = 0; idx < REC_MUXER_HANDLE_MAX_CNT; idx++) {
        if (&muxerMgr->ctx[idx] == ctx) {
            ctx->used = false;
            break;
        }
    }
    MEDIA_HAL_UNLOCK(muxerMgr->mutex);
    return;
}

static RecMuxerContext *RecMuxerMngGetContext(TD_MW_PTR hRecHandle)
{
    uint32_t idx;
    for (idx = 0; idx < REC_MUXER_HANDLE_MAX_CNT; idx++) {
        if (g_recMuxerManager.ctx[idx].hRecHandle == hRecHandle) {
            return &g_recMuxerManager.ctx[idx];
        }
    }
    return NULL;
}

static int32_t VideoSourceStart(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller)
{
    CHK_NULL_RETURN(pTrackSource, MEDIA_HAL_ERR, "pTrackSource NULL");
    CHK_NULL_RETURN(pCaller, MEDIA_HAL_ERR, "pCaller NULL");
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    return MEDIA_HAL_OK;
}

static int32_t VideoSourceStop(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller)
{
    CHK_NULL_RETURN(pTrackSource, MEDIA_HAL_ERR, "pTrackSource NULL");
    CHK_NULL_RETURN(pCaller, MEDIA_HAL_ERR, "pCaller NULL");
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    return MEDIA_HAL_OK;
}

static int32_t AudioSourceStart(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller)
{
    CHK_NULL_RETURN(pTrackSource, MEDIA_HAL_ERR, "pTrackSource NULL");
    CHK_NULL_RETURN(pCaller, MEDIA_HAL_ERR, "pCaller NULL");
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    return MEDIA_HAL_OK;
}

static int32_t AudioSourceStop(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller)
{
    CHK_NULL_RETURN(pTrackSource, MEDIA_HAL_ERR, "pTrackSource NULL");
    CHK_NULL_RETURN(pCaller, MEDIA_HAL_ERR, "pCaller NULL");
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    return MEDIA_HAL_OK;
}

static int32_t DataSourceStart(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller)
{
    CHK_NULL_RETURN(pTrackSource, MEDIA_HAL_ERR, "pTrackSource NULL");
    CHK_NULL_RETURN(pCaller, MEDIA_HAL_ERR, "pCaller NULL");
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    return MEDIA_HAL_OK;
}

static int32_t DataSourceStop(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller)
{
    CHK_NULL_RETURN(pTrackSource, MEDIA_HAL_ERR, "pTrackSource NULL");
    CHK_NULL_RETURN(pCaller, MEDIA_HAL_ERR, "pCaller NULL");
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    return MEDIA_HAL_OK;
}

static int32_t RequestKeyFrame(OT_Track_Source_Handle pTrackSource, TD_MW_PTR pCaller)
{
    CHK_NULL_RETURN(pTrackSource, MEDIA_HAL_ERR, "pTrackSource NULL");
    CHK_NULL_RETURN(pCaller, MEDIA_HAL_ERR, "pCaller NULL");
    MEDIA_HAL_LOGI(MODULE_NAME, "in");
    return MEDIA_HAL_OK;
}

inline static int32_t RecMuxerOpenParamCheck(const MuxerOutput *output)
{
    int32_t ret = MEDIA_HAL_OK;

    if (output->type >= OUT_TYPE_INVALID || (output->type == OUT_TYPE_FD && output->fd < 0)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "output type:%d or fd error", output->type);
        ret = MEDIA_HAL_ERR;
    }

    return ret;
}

static int32_t RecMuxerOpenOutTypeUriProcess(RecMuxerContext *ctx, const MuxerOutput *output)
{
    int optInit = FileOptInit(output->url);
    if (optInit != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "FileOptInit fail 0x%x", optInit);
        (void)FileOptDeInit();
        return MEDIA_HAL_ERR;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "output->format=%d.\n", output->format);
    if (output->format == FORMAT_TYPE_TS) {
        ctx->recAttr.stRecCallbacks.pfnRequestFileNames = RequestDualTSFileNames;
    } else {
        ctx->recAttr.stRecCallbacks.pfnRequestFileNames = RequestDualFileNames;
    }

    return MEDIA_HAL_OK;
}

static int32_t RecMuxerOpen(const MuxerOutput *output, const ProtocolFun *protocol, RecMuxerHandle *handle)
{
    MEDIA_HAL_UNUSED(protocol);
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle failed");
    CHK_NULL_RETURN(output, MEDIA_HAL_INVALID_PARAM, "outputConfig failed");

    if (RecMuxerOpenParamCheck(output) != MEDIA_HAL_OK) {
        return MEDIA_HAL_ERR;
    }

    if (RecMuxerInit() != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGD(MODULE_NAME, "RecMuxerInit failed");
    }
    RecMuxerContext *ctx = RecMuxerManagerAllocMuxer();
    CHK_NULL_RETURN(ctx, MEDIA_HAL_INVALID_PARAM, "RecMuxerManagerAllocMuxer failed");

    pthread_cond_init(&ctx->contextCond, NULL);
    pthread_mutex_init(&ctx->condMutex, NULL);
    pthread_mutex_init(&ctx->contextMutex, NULL);

    MEDIA_HAL_LOCK(ctx->contextMutex);
    if (output->type == OUT_TYPE_FD) {
        MEDIA_HAL_LOGI(MODULE_NAME, "output fd :%d", output->fd);
        ctx->recAttr.stRecCallbacks.pfnRequestFileNames = NULL;
    } else if (output->type == OUT_TYPE_URI) {
        MEDIA_HAL_LOGI(MODULE_NAME, "output url format:%d", output->format);
        if (RecMuxerOpenOutTypeUriProcess(ctx, output) != MEDIA_HAL_OK) {
            goto PARAM_INVALID;
        }
    }

    ctx->maxDuration = REC_MUXER_SPLIT_TIME_IN_SEC;
    ctx->maxFileSize = -1;
    ctx->recStarted = false;
    ctx->recStoping = false;
    ctx->recError = false;
    ctx->firstNewFileBegin = false;
    ctx->muxerConfig.s32FileAllocSize = 0;
    ctx->muxerConfig.fileDuration = -1;
    ctx->muxerOutput = *output;
    *handle = ctx;
    MEDIA_HAL_UNLOCK(ctx->contextMutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;

PARAM_INVALID:
    MEDIA_HAL_UNLOCK(ctx->contextMutex);
    pthread_cond_destroy(&ctx->contextCond);
    pthread_mutex_destroy(&ctx->condMutex);
    pthread_mutex_destroy(&ctx->contextMutex);
    RecMuxerManagerFreeMuxer(ctx);
    return FORMAT_ERR_INVALID_PARAM;
}

static int32_t RecMuxerDestroy(RecMuxerHandle handle)
{
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    CHK_NULL_RETURN(muxerContext, MEDIA_HAL_INVALID_PARAM, "handle is null");
    MEDIA_HAL_LOCK(muxerContext->contextMutex);
    int32_t ret;
    MEDIA_HAL_LOGI(MODULE_NAME, "in");
    if (muxerContext->hRecHandle != NULL) {
        MEDIA_HAL_LOGD(MODULE_NAME, "SS_REC_Destroy in");
        ret = SS_REC_Destroy(muxerContext->hRecHandle);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "SS_REC_Destroy failed ! ret: 0x%x", ret);
        }
        MEDIA_HAL_LOGD(MODULE_NAME, "SS_REC_Destroy out");
        muxerContext->hRecHandle  = NULL;
    }
    for (uint32_t streamIdx = 0; streamIdx < muxerContext->recAttr.u32StreamCnt && streamIdx < OT_REC_STREAM_MAX_CNT;
         streamIdx++) {
        OT_REC_STREAM_ATTR_S *streamAttr = &muxerContext->recAttr.astStreamAttr[streamIdx];
        for (uint32_t index = 0; index < streamAttr->u32TrackCnt && index < OT_REC_TRACK_MAX_CNT; index++) {
            OT_Track_Source_Handle trackHandle = streamAttr->aHTrackSrcHandle[index];
            if (trackHandle != NULL) {
                free(trackHandle);
                streamAttr->aHTrackSrcHandle[index] = NULL;
            }
        }
    }
    MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
    pthread_cond_destroy(&muxerContext->contextCond);
    pthread_mutex_destroy(&muxerContext->condMutex);
    pthread_mutex_destroy(&muxerContext->contextMutex);
    RecMuxerManagerFreeMuxer(muxerContext);
    muxerContext = NULL;
    MEDIA_HAL_LOGI(MODULE_NAME, "RecMuxerDestroy ok");
    return MEDIA_HAL_OK;
}

static uint32_t GetFreeVideoTrackIndex(const RecMuxerContext *muxerContext, uint32_t *trackIndex)
{
    CHK_NULL_RETURN(muxerContext, MEDIA_HAL_ERR, "input param muxerContext null");
    CHK_NULL_RETURN(trackIndex, MEDIA_HAL_ERR, "input param trackIndex null");
    if (muxerContext->recAttr.u32StreamCnt >= OT_REC_STREAM_MAX_CNT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Recorder only support %d stream", OT_REC_STREAM_MAX_CNT);
        return INVALID_STREAM_INDEX;
    }
    for (uint32_t streamIndex = 0; streamIndex < OT_REC_STREAM_MAX_CNT; streamIndex++) {
        uint32_t tmpTrackIndex = UINT32_MAX;
        bool hasVideo = false;
        for (uint32_t index = 0; index < OT_REC_TRACK_MAX_CNT; index++) {
            OT_Track_Source_Handle trackSrcHandle =
                muxerContext->recAttr.astStreamAttr[streamIndex].aHTrackSrcHandle[index];
            if (trackSrcHandle == NULL && tmpTrackIndex == UINT32_MAX) {
                tmpTrackIndex = index;
            } else if (trackSrcHandle != NULL && trackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_VIDEO) {
                hasVideo = true;
                break;
            }
        }
        if (!hasVideo && tmpTrackIndex != UINT32_MAX) {
            *trackIndex = tmpTrackIndex;
            MEDIA_HAL_LOGI(MODULE_NAME, " Get Free streamIndex :%u VideoTrackIndex:%u", streamIndex, tmpTrackIndex);
            return streamIndex;
        }
    }
    MEDIA_HAL_LOGE(MODULE_NAME, "no FreeVideoTrackIndex");
    return INVALID_STREAM_INDEX;
}

static int32_t GetMuxerOps(const RecMuxerContext *muxerContext, OT_REC_MUXER_OPERATE_FN_S *muxerOps,
    uint32_t streamIndex)
{
    CHK_NULL_RETURN(muxerContext, MEDIA_HAL_ERR, "muxerConfig is NULL");
    CHK_NULL_RETURN(muxerOps, MEDIA_HAL_ERR, "muxerOps is NULL");
    MEDIA_HAL_LOGE(MODULE_NAME, "muxerOutput.format :%d streamIndex:%d", muxerContext->muxerOutput.format,
                   streamIndex);
    if (muxerContext->muxerOutput.format == FORMAT_TYPE_TS) {
        muxerOps->pfnCreateMuxer = (OT_REC_CREATE_MUXER_FN)CreateTSMuxer;
        muxerOps->pfnDestroyMuxer = DestroyTSMuxer;
        muxerOps->pfnCreateTrack = CreateTSTrack;
        muxerOps->pfnWriteFrame = (OT_REC_WRITE_FRAME_FN)WriteTSFrame;
    } else {
        muxerOps->pfnCreateMuxer = (OT_REC_CREATE_MUXER_FN)CreateMP4Muxer;
        muxerOps->pfnDestroyMuxer = DestroyMP4Muxer;
        muxerOps->pfnCreateTrack = CreateMP4Track;
        muxerOps->pfnWriteFrame = (OT_REC_WRITE_FRAME_FN)WriteMP4Frame;
    }
    muxerOps->pMuxerCfg = (void *)&muxerContext->muxerConfig;
    return MEDIA_HAL_OK;
}

static void GetMuxerCtxRecAttr(RecMuxerContext *muxerContext, uint32_t streamIndex,
    uint32_t trackIndex, OT_Track_Source_Handle videoTrackSourceHandle)
{
    muxerContext->recAttr.astStreamAttr[streamIndex].stBufOps.pfnBufAlloc = (OT_REC_BUF_ALLOC_FN)RecAllocBuf;
    muxerContext->recAttr.astStreamAttr[streamIndex].stBufOps.pfnBufFree = (OT_REC_BUF_FREE_FN)RecFreeBuf;
    muxerContext->recAttr.astStreamAttr[streamIndex].stBufOps.pBufArg = TD_NULL;
    muxerContext->recAttr.astStreamAttr[streamIndex].u32TrackCnt++;
    muxerContext->recAttr.u32StreamCnt++;
    muxerContext->recAttr.astStreamAttr[streamIndex].aHTrackSrcHandle[trackIndex] = videoTrackSourceHandle;
}

static int32_t AddVideoTrack(RecMuxerContext *muxerContext, const MuxerTrackSource *trackSource)
{
    td_u32 multiRet = 0;
    uint32_t trackIndex;
    uint32_t streamIndex = GetFreeVideoTrackIndex(muxerContext, &trackIndex);
    if (streamIndex == INVALID_STREAM_INDEX || trackIndex >= OT_REC_TRACK_MAX_CNT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Recorder only support %d stream", OT_REC_STREAM_MAX_CNT);
        return -1;
    }
    /* free at RecMuxerDestroy */
    OT_Track_Source_Handle videoTrackSourceHandle = (OT_Track_Source_Handle)malloc(sizeof(OT_Track_Source_S));
    CHK_NULL_RETURN(videoTrackSourceHandle, MEDIA_HAL_ERR, "video OT_Track_Source_Handle malloc failed");
    videoTrackSourceHandle->enTrackType = OT_TRACK_SOURCE_TYPE_VIDEO;
    videoTrackSourceHandle->s32PrivateHandle = (intptr_t)muxerContext; /* array of track source cnt */
    videoTrackSourceHandle->pfnSourceStart = VideoSourceStart;
    videoTrackSourceHandle->pfnSourceStop = VideoSourceStop;
    videoTrackSourceHandle->pfnRequestKeyFrame = RequestKeyFrame;

    switch (trackSource->info.videoInfo.codecType) {
        case CODEC_TYPE_H264:
            MEDIA_HAL_LOGI(MODULE_NAME, "codecType is CODEC_H264 ");
            videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.enCodecType = OT_TRACK_VIDEO_CODEC_H264;
            break;
        case CODEC_TYPE_H265:
            MEDIA_HAL_LOGI(MODULE_NAME, "codecType is CODEC_H265 ");
            videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.enCodecType = OT_TRACK_VIDEO_CODEC_H265;
            break;
        default:
            MEDIA_HAL_LOGI(MODULE_NAME, "codecType default CODEC_H265");
            videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.enCodecType = OT_TRACK_VIDEO_CODEC_H265;
            break;
    }
    videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.u32Width = trackSource->info.videoInfo.width;
    videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.u32Height = trackSource->info.videoInfo.height;
    if (CheckUnIntMultip(trackSource->info.videoInfo.bitRate, MUXER_BITRATE_WEIGHTS, &multiRet)) {
        videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.u32BitRate = multiRet;
    }
    videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.frameRate = trackSource->info.videoInfo.frameRate;
    videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.u32Gop = trackSource->info.videoInfo.keyFrameInterval;
    videoTrackSourceHandle->unTrackSourceAttr.stVideoInfo.fSpeed = trackSource->info.videoInfo.speed;

    if (GetMuxerOps(muxerContext, &muxerContext->recAttr.astStreamAttr[streamIndex].stMuxerOps,
        streamIndex) != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "GetMuxerOps failed");
        free(videoTrackSourceHandle);
        videoTrackSourceHandle = NULL;
        return -1;
    }
    GetMuxerCtxRecAttr(muxerContext, streamIndex, trackIndex, videoTrackSourceHandle);
    return TRACK_ID(streamIndex, trackIndex);
}

static uint32_t GetFreeAudioTrackIndex(const RecMuxerContext *muxerContext, uint32_t *trackIndex)
{
    CHK_NULL_RETURN(muxerContext, INVALID_STREAM_INDEX, "muxerContext null");
    CHK_NULL_RETURN(trackIndex, INVALID_STREAM_INDEX, "trackIndex null");
    for (uint32_t streamIndex = 0; streamIndex < OT_REC_STREAM_MAX_CNT; streamIndex++) {
        uint32_t tmpTrackIndex = UINT32_MAX;
        bool hasAudio = false;
        for (uint32_t index = 0; index < OT_REC_TRACK_MAX_CNT; index++) {
            OT_Track_Source_Handle trackSrcHandle =
            muxerContext->recAttr.astStreamAttr[streamIndex].aHTrackSrcHandle[index];
            if (trackSrcHandle == NULL && tmpTrackIndex == UINT32_MAX) {
                tmpTrackIndex = index;
            } else if (trackSrcHandle != NULL &&
                       trackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_AUDIO) {
                hasAudio = true;
                break;
            }
        }
        if (!hasAudio && tmpTrackIndex != UINT32_MAX) {
            *trackIndex =  tmpTrackIndex;
            MEDIA_HAL_LOGI(MODULE_NAME, "Get Free streamIndex :%u AudioTrackIndex:%u", streamIndex, tmpTrackIndex);
            return streamIndex;
        }
    }
    MEDIA_HAL_LOGE(MODULE_NAME, "no FreeAudioTrackIndex");
    return MEDIA_HAL_ERR;
}

static int32_t AddAudioTrack(RecMuxerContext *muxerContext, const MuxerTrackSource *trackSource)
{
    uint32_t trackIndex;
    uint32_t streamIndex = GetFreeAudioTrackIndex(muxerContext, &trackIndex);
    if (streamIndex == INVALID_STREAM_INDEX || trackIndex >= OT_REC_TRACK_MAX_CNT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Recorder only support %d stream ", OT_REC_STREAM_MAX_CNT);
        return -1;
    }
    /* free at RecMuxerDestroy */
    OT_Track_Source_Handle audioTrackSourceHandle = (OT_Track_Source_Handle)malloc(sizeof(OT_Track_Source_S));
    CHK_NULL_RETURN(audioTrackSourceHandle, MEDIA_HAL_ERR, "audio OT_Track_Source_Handle malloc failed");
    audioTrackSourceHandle->enTrackType = OT_TRACK_SOURCE_TYPE_AUDIO;
    audioTrackSourceHandle->s32PrivateHandle = (intptr_t)muxerContext; /* array of track source cnt */
    audioTrackSourceHandle->pfnSourceStart = AudioSourceStart;
    audioTrackSourceHandle->pfnSourceStop = AudioSourceStop;
    audioTrackSourceHandle->pfnRequestKeyFrame = RequestKeyFrame;

    audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.enCodecType = OT_TRACK_AUDIO_CODEC_AAC;
    audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u32ChnCnt = trackSource->info.audioInfo.channelCount;
    audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u32SampleRate = trackSource->info.audioInfo.sampleRate;
    audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u32AvgBytesPerSec = \
    trackSource->info.audioInfo.avgBytesPerSec;
    audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u32SamplesPerFrame = \
    trackSource->info.audioInfo.samplesPerFrame;
    switch (trackSource->info.audioInfo.sampleBitWidth) {
        case AUDIO_SAMPLE_FORMAT_S8:
            audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u16SampleBitWidth = MUXER_AUDIO_SAMPLE_S8;
            break;
        case AUDIO_SAMPLE_FORMAT_S16:
            audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u16SampleBitWidth = MUXER_AUDIO_SAMPLE_S16;
            break;
        case AUDIO_SAMPLE_FORMAT_S24:
            audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u16SampleBitWidth = MUXER_AUDIO_SAMPLE_S24;
            break;
        case AUDIO_SAMPLE_FORMAT_S32:
            audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u16SampleBitWidth = MUXER_AUDIO_SAMPLE_S32;
            break;
        default:
            audioTrackSourceHandle->unTrackSourceAttr.stAudioInfo.u16SampleBitWidth = MUXER_AUDIO_SAMPLE_S16;
            break;
    }
    muxerContext->recAttr.astStreamAttr[streamIndex].aHTrackSrcHandle[trackIndex] = audioTrackSourceHandle;
    muxerContext->recAttr.astStreamAttr[streamIndex].u32TrackCnt++;
    MEDIA_HAL_LOGI(MODULE_NAME, "AddAudioTrack streamIndex :%u AudioTrackIndex:%u", streamIndex, trackIndex);
    return TRACK_ID(streamIndex, trackIndex);
}

static uint32_t GetFreeDataTrackIndex(const RecMuxerContext *muxerContext, uint32_t *trackIndex)
{
    CHK_NULL_RETURN(muxerContext, INVALID_STREAM_INDEX, "muxerContext null");
    CHK_NULL_RETURN(trackIndex, INVALID_STREAM_INDEX, "trackIndex null");
    for (uint32_t streamIndex = 0; streamIndex < OT_REC_STREAM_MAX_CNT; streamIndex++) {
        uint32_t tmpTrackIndex = UINT32_MAX;
        bool hasData = false;
        for (uint32_t index = 0; index < OT_REC_TRACK_MAX_CNT; index++) {
            OT_Track_Source_Handle trackSrcHandle =
            muxerContext->recAttr.astStreamAttr[streamIndex].aHTrackSrcHandle[index];
            if (trackSrcHandle == NULL && tmpTrackIndex == UINT32_MAX) {
                tmpTrackIndex = index;
            } else if (trackSrcHandle != NULL && trackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_PRIV) {
                hasData = true;
                break;
            }
        }
        if (!hasData && tmpTrackIndex != UINT32_MAX) {
            *trackIndex =  tmpTrackIndex;
            MEDIA_HAL_LOGI(MODULE_NAME, "Get Free streamIndex :%u DataTrackIndex:%u", streamIndex, tmpTrackIndex);
            return streamIndex;
        }
    }
    MEDIA_HAL_LOGE(MODULE_NAME, "no FreeDataTrackIndex");
    return MEDIA_HAL_ERR;
}

static int32_t AddDataTrack(RecMuxerContext *muxerContext, const MuxerTrackSource *trackSource)
{
    uint32_t trackIndex;
    uint32_t streamIndex = GetFreeDataTrackIndex(muxerContext, &trackIndex);
    if (streamIndex == INVALID_STREAM_INDEX || trackIndex >= OT_REC_TRACK_MAX_CNT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Recorder only support %d stream ", OT_REC_STREAM_MAX_CNT);
        return -1;
    }
    /* free at RecMuxerDestroy */
    OT_Track_Source_Handle dataTrackSourceHandle = (OT_Track_Source_Handle)malloc(sizeof(OT_Track_Source_S));
    CHK_NULL_RETURN(dataTrackSourceHandle, MEDIA_HAL_ERR, "data OT_Track_Source_Handle malloc failed");
    dataTrackSourceHandle->enTrackType = OT_TRACK_SOURCE_TYPE_PRIV;
    dataTrackSourceHandle->s32PrivateHandle = (intptr_t)muxerContext; /* array of track source cnt */
    dataTrackSourceHandle->pfnSourceStart = DataSourceStart;
    dataTrackSourceHandle->pfnSourceStop = DataSourceStop;
    dataTrackSourceHandle->pfnRequestKeyFrame = RequestKeyFrame;

    dataTrackSourceHandle->unTrackSourceAttr.stPrivInfo.frameRate = trackSource->info.dataInfo.frameRate;
    dataTrackSourceHandle->unTrackSourceAttr.stPrivInfo.u32BytesPerSec = trackSource->info.dataInfo.bitRate;
    dataTrackSourceHandle->unTrackSourceAttr.stPrivInfo.bStrictSync = false;

    muxerContext->recAttr.astStreamAttr[streamIndex].aHTrackSrcHandle[trackIndex] = dataTrackSourceHandle;
    muxerContext->recAttr.astStreamAttr[streamIndex].u32TrackCnt++;
    MEDIA_HAL_LOGI(MODULE_NAME, "AddDataTrack streamIndex :%u DataTrackIndex:%u", streamIndex, trackIndex);
    return TRACK_ID(streamIndex, trackIndex);
}

static int32_t RecMuxerAddTrack(RecMuxerHandle handle, const MuxerTrackSource *trackSource)
{
    CHK_NULL_RETURN(trackSource, MEDIA_HAL_INVALID_PARAM, "input param trackSource null");
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    CHK_NULL_RETURN(muxerContext, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    if (trackSource->type == STREAM_TYPE_VIDEO) {
        return AddVideoTrack(muxerContext, trackSource);
    } else if (trackSource->type == STREAM_TYPE_AUDIO) {
        return AddAudioTrack(muxerContext, trackSource);
    } else if (trackSource->type == STREAM_TYPE_DATA) {
        return AddDataTrack(muxerContext, trackSource);
    }
    return MEDIA_HAL_ERR;
}

static void RecEventCallbackError(RecMuxerContext *context, const OT_REC_EVENT_INFO_S *pstEventInfo)
{
    int32_t error = ERROR_TYPE_UNKNOWN;
    switch (pstEventInfo->enEventCode) {
        case OT_REC_EVENT_ERR_CREATE_FILE_FAIL:
            error = ERROR_TYPE_CREATE_FILE_FAIL;
            break;
        case OT_REC_EVENT_ERR_WRITE_FILE_FAIL:
            error = ERROR_TYPE_WRITE_FILE_FAIL;
            break;
        case OT_REC_EVENT_ERR_CLOSE_FILE_FAIL:
            error = ERROR_TYPE_CLOSE_FILE_FAIL;
            break;
        case OT_REC_EVENT_ERR_READ_DATA_ERROR:
            error = ERROR_TYPE_READ_DATA_ERROR;
            break;
        case OT_REC_EVENT_ERR_INTERNAL_OPERATION_FAIL:
            error = ERROR_TYPE_INTERNAL_OPERATION_FAIL;
            break;
        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "REC_EVENT_ERR unknown event %d", pstEventInfo->enEventCode);
            break;
    }
    MEDIA_HAL_LOGE(MODULE_NAME, "REC_EVENT_ERR %d", pstEventInfo->enEventCode);
    MEDIA_HAL_LOCK(context->condMutex);
    context->recError = true;
    pthread_cond_signal(&context->contextCond);
    MEDIA_HAL_UNLOCK(context->condMutex);
    CHK_NULL_RETURN_NONE(context->callBack.OnError, "OnError invalid pointer");
    context->callBack.OnError(context->callBack.handle, error, pstEventInfo->unEventInfo.stErrorInfo.s32ErrorCode);
}

static void RecEventCallbackNewFile(RecMuxerContext *context, const OT_REC_EVENT_INFO_S *pstEventInfo)
{
    const OT_REC_FILE_EVENT_INFO_S *fileInfo = &pstEventInfo->unEventInfo.stFileInfo;
    switch (pstEventInfo->enEventCode) {
        case OT_REC_EVENT_NEW_FILE_BEGIN:
            if (!context->firstNewFileBegin) {
                context->firstNewFileBegin = true;
            } else {
                CHK_NULL_RETURN_NONE(context->callBack.OnInfo, "OnInfo invalid pointer");
            }
            break;
        case OT_REC_EVENT_NEW_FILE_END:
            AddNorm(fileInfo->aszFileName);
            break;
        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "pass event %d", pstEventInfo->enEventCode);
            break;
    }
}

static void RecEventStopProcess(RecMuxerContext *context)
{
    MEDIA_HAL_LOCK(context->condMutex);
    struct timeval timeToEnd;
    gettimeofday(&timeToEnd, NULL);
    td_s64 time = CalcDiffTimeMs(context->timeToStop, timeToEnd);
    MEDIA_HAL_LOGI(MODULE_NAME, "REC_EVENT_STOP cost :%lld ms", time);
    context->recStoping = TD_FALSE;
    pthread_cond_signal(&context->contextCond);
    MEDIA_HAL_UNLOCK(context->condMutex);
}

static void ReportInfo(PluginCallback callBack, PluginInfoType infoType, int32_t extra)
{
    CHK_NULL_RETURN_NONE(callBack.OnInfo, "OnInfo invalid pointer");
    callBack.OnInfo(callBack.handle, infoType, extra);
}

static void RecEventCallbackInfo(RecMuxerContext *context, const OT_REC_EVENT_INFO_S *pstEventInfo)
{
    const OT_REC_FILE_EVENT_INFO_S *fileInfo = &pstEventInfo->unEventInfo.stFileInfo;

    switch (pstEventInfo->enEventCode) {
        case OT_REC_EVENT_START:
            MEDIA_HAL_LOGI(MODULE_NAME, "REC_EVENT_START");
            break;
        case OT_REC_EVENT_STOP:
            RecEventStopProcess(context);
            break;
        case OT_REC_EVENT_NEW_FILE_BEGIN:
        case OT_REC_EVENT_NEW_FILE_END:
            RecEventCallbackNewFile(context, pstEventInfo);
            break;
        case OT_REC_EVENT_NEW_MANUAL_SPLIT_FILE_END:
            MEDIA_HAL_LOGI(MODULE_NAME, "REC_EVENT_NEW_MANUAL_SPLIT_FILE_END end");
            MoveEmr(fileInfo->aszFileName);
            ReportInfo(context->callBack, FILE_SPLIT_FINISHED, 0);
            break;
        default:
            MEDIA_HAL_LOGI(MODULE_NAME, "pass event %d", pstEventInfo->enEventCode);
            break;
    }
}

td_s32 RecEventCallback(TD_MW_PTR hRecHandle, const OT_REC_EVENT_INFO_S *pstEventInfo)
{
    CHK_NULL_RETURN(pstEventInfo, MEDIA_HAL_INVALID_PARAM, "invalide param pstEventInfo");
    RecMuxerContext *context = RecMuxerMngGetContext(hRecHandle);
    CHK_NULL_RETURN(context, MEDIA_HAL_INVALID_PARAM, "input param hRecHandle null");
    if (pstEventInfo->enEventCode >= OT_REC_EVENT_START &&
        pstEventInfo->enEventCode <= OT_REC_EVENT_NEW_MANUAL_SPLIT_FILE_END) {
        RecEventCallbackInfo(context, pstEventInfo);
    } else {
        RecEventCallbackError(context, pstEventInfo);
    }
    return MEDIA_HAL_OK;
}

static int32_t RecMuxerSetCallBack(RecMuxerContext *muxerContext, const PluginCallback *callBack)
{
    CHK_NULL_RETURN(callBack, MEDIA_HAL_INVALID_PARAM, "callBack");
    muxerContext->callBack = *callBack;
    return MEDIA_HAL_OK;
}

static int32_t RecMuxerSetOrientation(RecMuxerContext *muxerContext, const int *degrees)
{
    CHK_NULL_RETURN(degrees, MEDIA_HAL_INVALID_PARAM, "degrees");
    muxerContext->degrees = *degrees;
    return MEDIA_HAL_OK;
}

static int32_t RecMuxerSetPreAllocSize(RecMuxerContext *muxerContext, const int *size)
{
    CHK_NULL_RETURN(size, MEDIA_HAL_INVALID_PARAM, "size");
    int32_t value = *size;
    if (value < 0 || value > MUXER_PRE_ALLOC_SIZE_100) {
        MEDIA_HAL_LOGE(MODULE_NAME, "invalid parameter:%d ", value);
        return MEDIA_HAL_ERR;
    }
    muxerContext->muxerConfig.s32FileAllocSize = value * 1024 * 1024; /* 1024 * 1024 = 1M */
    return MEDIA_HAL_OK;
}

static int32_t RecMuxerSetLocation(RecMuxerContext *muxerContext, const Location *location)
{
    CHK_NULL_RETURN(location, MEDIA_HAL_INVALID_PARAM, "location");
    muxerContext->latitude = location->latitude;
    muxerContext->longitude = location->longitude;
    return MEDIA_HAL_OK;
}

int32_t RecMuxerSetMaxFileSize(RecMuxerHandle handle, int64_t bytes)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    MEDIA_HAL_LOGI(MODULE_NAME, "bytes:%lld ", bytes);
    muxerContext->maxFileSize = bytes;
    return MEDIA_HAL_OK;
}

int32_t RecMuxerSetMaxFileDuration(RecMuxerHandle handle, int64_t durationUs)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    MEDIA_HAL_LOGI(MODULE_NAME, "durationUs:%lld ", durationUs);
    muxerContext->maxDuration = durationUs;
    muxerContext->muxerConfig.fileDuration = durationUs;
    return MEDIA_HAL_OK;
}

static int32_t RecMuxerSetFileSplitDuration(const RecMuxerContext *muxerContext, const MannualSpit *split)
{
    CHK_NULL_RETURN(split, MEDIA_HAL_INVALID_PARAM, "split");
    OT_REC_MANUAL_SPLIT_ATTR_S manualSplitAttr = {};
    switch (split->type) {
        case SPLIT_TYPE_POST:
            manualSplitAttr.enManualType = OT_REC_POST_MANUAL_SPLIT;
            manualSplitAttr.stPostSplitAttr.u32AfterSec = split->durationS;
            MEDIA_HAL_LOGI(MODULE_NAME, "SS_REC_ManualSplit SPLIT_TYPE_POST u32AfterSec:%u",
                manualSplitAttr.stPostSplitAttr.u32AfterSec);
            break;
        case SPLIT_TYPE_PRE:
            manualSplitAttr.enManualType = OT_REC_PRE_MANUAL_SPLIT;
            manualSplitAttr.stPreSplitAttr.u32DurationSec = split->durationS;
            MEDIA_HAL_LOGI(MODULE_NAME, "SS_REC_ManualSplit SPLIT_TYPE_PRE u32DurationSec:%u",
                manualSplitAttr.stPreSplitAttr.u32DurationSec);
            break;
        case SPLIT_TYPE_NORMAL:
            manualSplitAttr.enManualType = OT_REC_NORMAL_MANUAL_SPLIT;
            MEDIA_HAL_LOGI(MODULE_NAME, "SS_REC_ManualSplit SPLIT_TYPE_NORMAL");
            break;
        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "Invalid FileSplitType: %d", split->type);
            return FORMAT_ERR_INVALID_PARAM;
    }
    int32_t ret = MEDIA_HAL_OK;
    CHK_NULL_RETURN(muxerContext->hRecHandle, MEDIA_HAL_INVALID_PARAM, "handle");
    if ((ret = SS_REC_ManualSplit(muxerContext->hRecHandle, &manualSplitAttr)) != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_REC_ManualSplit failed ! ret: 0x%x", ret);
        return ret;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "SS_REC_ManualSplit OK");
    return ret;
}

static int32_t RecMuxerSetPreRecordTime(RecMuxerContext *muxerContext, const int *preRecordTimeSec)
{
    CHK_NULL_RETURN(preRecordTimeSec, MEDIA_HAL_INVALID_PARAM, "callBack");
    muxerContext->recAttr.u32PreRecTimeSec = *preRecordTimeSec;
    return MEDIA_HAL_OK;
}

static TD_S32 RecMuxerRequestFileNames(TD_MW_PTR pRecorder, TD_U32 u32FileCnt,
                                       TD_CHAR (*paszFilename)[OT_REC_FILE_NAME_LEN])
{
    (void)pRecorder;
    static const int usecToMsec = 1000;
    for (td_u32 i = 0; i < u32FileCnt; i++) {
        struct timeval tv;
        (void)gettimeofday(&tv, NULL);
        struct tm *tm_now = localtime(&tv.tv_sec);
        (void)snprintf_s(paszFilename[i], OT_REC_FILE_NAME_LEN, OT_REC_FILE_NAME_LEN - 1,
            "/userdata/photo/record%02d-%02d-%02d-%03ld.mp4",
            tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, tv.tv_usec / usecToMsec);
    }
    return TD_SUCCESS;
}

static int32_t PrepareFormatMuxerInternal(RecMuxerContext *muxerContext)
{
    CHK_NULL_RETURN(muxerContext, MEDIA_HAL_INVALID_PARAM, "muxerContext");
    td_u32 streamCnt = muxerContext->recAttr.u32StreamCnt;
    if (streamCnt == 0) {
        td_u32 trackCnt = muxerContext->recAttr.astStreamAttr[streamCnt].u32TrackCnt;
        if (trackCnt == 0) {
            MEDIA_HAL_LOGE(MODULE_NAME, "NO audio or video track add  failed ");
            return FORMAT_ERR_NOT_CREATED;
        }
    }
    g_streamCnt = muxerContext->recAttr.u32StreamCnt;
    MEDIA_HAL_LOGI(MODULE_NAME, "g_streamCnt=%u\n", g_streamCnt);
    muxerContext->recAttr.enRecType = OT_REC_TYPE_NORMAL;
    muxerContext->recAttr.stRecCallbacks.pfnRequestFileNames = RecMuxerRequestFileNames;
    muxerContext->recAttr.unRecAttr.stNormalRecAttr.u32Rsv = 0;
    // defalt
    muxerContext->recAttr.stSplitAttr.enSplitType = OT_REC_SPLIT_TYPE_TIME;
    if (muxerContext->maxDuration > UINT32_MAX || muxerContext->maxDuration < -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "maxDuration error ");
        return TD_FAILURE;
    }
    muxerContext->recAttr.stSplitAttr.u32SplitTimeLenSec = (uint32_t)muxerContext->maxDuration;
    if (muxerContext->maxDuration == -1 &&
        muxerContext->maxFileSize != -1) {
        muxerContext->recAttr.stSplitAttr.maxFileSizeGB = muxerContext->maxFileSize;
        muxerContext->recAttr.stSplitAttr.u32SplitTimeLenSec = 0;
    } else if (muxerContext->maxDuration != -1 &&
               muxerContext->maxFileSize != -1) {
        muxerContext->recAttr.stSplitAttr.maxFileSizeGB = muxerContext->maxFileSize;
    }
    muxerContext->recAttr.stSplitAttr.maxFileSizeGB = 4;  // 4: record max file size GB
    MEDIA_HAL_LOGI(MODULE_NAME, "u32SplitTimeLenSec:%d\n", muxerContext->recAttr.stSplitAttr.u32SplitTimeLenSec);
    if (muxerContext->recAttr.u32PreRecTimeSec > (INT32_MAX / SEC_TO_MS)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "u32PreRecTimeSec:%d error", muxerContext->recAttr.u32PreRecTimeSec);
        return MEDIA_HAL_ERR;
    }

    muxerContext->recAttr.u32PreRecTimeSec = 1;
    muxerContext->recAttr.astStreamAttr[0].bufferTimeMSec = REC_BUFFER_TIME * TIME_UNIT;

    return MEDIA_HAL_OK;
}

static int32_t RecMuxerStart(RecMuxerHandle handle)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    MEDIA_HAL_LOCK(muxerContext->contextMutex);
    if (muxerContext->recStarted) {
        MEDIA_HAL_LOGI(MODULE_NAME, "Format Muxer Started");
        MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
        return MEDIA_HAL_OK;
    }
    int32_t ret = MEDIA_HAL_OK;
    if (muxerContext->hRecHandle != NULL && muxerContext->recError) {
        MEDIA_HAL_LOGI(MODULE_NAME, "SS_REC_Destroy in");
        ret = SS_REC_Destroy(muxerContext->hRecHandle);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "SS_REC_Destroy failed ! ret: 0x%x", ret);
        }
        MEDIA_HAL_LOGI(MODULE_NAME, "SS_REC_Destroy out");
        muxerContext->hRecHandle  = NULL;
    }
    ret = PrepareFormatMuxerInternal(muxerContext);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "PrepareFormatMuxerInternal failed ret:0x%x", ret);
        MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
        return ret;
    }
    if (muxerContext->hRecHandle == NULL) {
        ret = SS_REC_Create(&muxerContext->recAttr, &muxerContext->hRecHandle);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "SS_REC_Create failed ret:0x%x", ret);
            MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
            return ret;
        }
        MEDIA_HAL_LOGI(MODULE_NAME, "SS_REC_Create success");
        ret = SS_REC_RegisterEventCallback(muxerContext->hRecHandle, RecEventCallback);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "SS_REC_RegisterEventCallback failed ret:0x%x", ret);
        }
    }
    ret = SS_REC_Start(muxerContext->hRecHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_REC_Start failed ret:0x%x", ret);
        MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
        return ret;
    }
    muxerContext->recStarted = true;
    MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
    return MEDIA_HAL_OK;
}

static int32_t RecMuxerWriteFrame(RecMuxerHandle handle, const FormatPacket *frameData)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    CHK_NULL_RETURN(frameData, MEDIA_HAL_INVALID_PARAM, "frameData");
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    td_u32 streamCnt = SREAM_INDEX(frameData->trackId);
    td_u32 trackCnt = TRACK_INDEX(frameData->trackId);
    if (streamCnt >= OT_REC_STREAM_MAX_CNT || streamCnt >= muxerContext->recAttr.u32StreamCnt) {
        MEDIA_HAL_LOGE(MODULE_NAME, "trackId %d invalid ", frameData->trackId);
        return MEDIA_HAL_ERR;
    }
    if (trackCnt >= OT_REC_TRACK_MAX_CNT || trackCnt >= muxerContext->recAttr.astStreamAttr[streamCnt].u32TrackCnt) {
        MEDIA_HAL_LOGE(MODULE_NAME, "trackId %d invalid", frameData->trackId);
        return MEDIA_HAL_ERR;
    }
    CHK_NULL_RETURN(muxerContext->hRecHandle, FORMAT_ERR_NOT_CREATED, "operate must after hRecHandle created");

    MEDIA_HAL_LOCK(muxerContext->contextMutex);
    OT_Track_Source_Handle trackSrcHandle = muxerContext->recAttr.astStreamAttr[streamCnt].aHTrackSrcHandle[trackCnt];
    OT_REC_FRAME_DATA_S recFrameData = {};
    recFrameData.u64TimeStamp = frameData->timestampUs;
    recFrameData.bKeyFrameFlag = frameData->isKeyFrame;
    recFrameData.apu8SliceAddr[0] = frameData->data;
    recFrameData.au32SliceLen[0] = frameData->len;
    recFrameData.u32SliceCnt = 1;
    int32_t ret = SS_REC_WriteData(muxerContext->hRecHandle, trackSrcHandle, &recFrameData);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[%s] HI_REC_WriteData ret \
            0x[%x].bKeyFrameFlag[%d] PTS[%llu] SliceLen[0][%u] u32SliceCnt[%u]",
            trackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_VIDEO ? "video" : "audio",
            ret, recFrameData.bKeyFrameFlag, recFrameData.u64TimeStamp, recFrameData.au32SliceLen[0], \
            recFrameData.u32SliceCnt);
    }
    MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
    return ret;
}

static int32_t RecMuxerSetNextOutputFile(RecMuxerHandle handle, int32_t fd)
{
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    CHK_NULL_RETURN(muxerContext, MEDIA_HAL_INVALID_PARAM, "handle is null");
    MEDIA_HAL_LOGI(MODULE_NAME, "fd:%d", fd);
    CHK_NULL_RETURN(muxerContext->hRecHandle, FORMAT_ERR_NOT_CREATED,
                    "setNextOutputFile must after hRecHandle created");
    return MEDIA_HAL_OK;
}

static int32_t RecMuxerStop(RecMuxerHandle handle, bool block)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    CHK_NULL_RETURN(muxerContext->hRecHandle, MEDIA_HAL_INVALID_PARAM, "handle");
    MEDIA_HAL_LOCK(muxerContext->contextMutex);
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    int64_t time;
    struct timeval begin = { 0, 0 };
    struct timeval end = { 0, 0 };
    gettimeofday(&begin, NULL);
    td_bool bQuickMode = block ? TD_FALSE : TD_TRUE;
    int32_t ret = SS_REC_Stop(muxerContext->hRecHandle, bQuickMode);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_REC_Stop failed ! ret: 0x%x", ret);
        MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
        return ret;
    }
    gettimeofday(&end, NULL);
    time = CalcDiffTimeMs(begin, end);
    MEDIA_HAL_LOGI(MODULE_NAME, "SS_REC_Stop bQuickMode :%d cost %llu ms", bQuickMode, time);
    MEDIA_HAL_LOCK(muxerContext->condMutex);
    gettimeofday(&muxerContext->timeToStop, NULL);
    muxerContext->recStoping = TD_TRUE;
    while (muxerContext->recStoping &&
           muxerContext->recError == TD_FALSE) {
        struct timeval now;
        struct timespec timeToWait;
        gettimeofday(&now, NULL);
        timeToWait.tv_sec = now.tv_sec + TIME_TO_WAIT_S;
        timeToWait.tv_nsec = now.tv_usec * US_TO_NSEC;
        MEDIA_HAL_LOGI(MODULE_NAME, "FormatMuxerStop timedwait REC_EVENT_STOP");
        (void)pthread_cond_timedwait(&muxerContext->contextCond,
                                     &muxerContext->condMutex, &timeToWait);
    }
    MEDIA_HAL_UNLOCK(muxerContext->condMutex);
    muxerContext->recStarted = false;
    MEDIA_HAL_UNLOCK(muxerContext->contextMutex);
    return MEDIA_HAL_OK;
}

static int32_t RecMuxerSetParameter(RecMuxerHandle handle, int32_t trackId, const OptionalItem *item, int32_t itemNum)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    CHK_NULL_RETURN(item, MEDIA_HAL_INVALID_PARAM, "item");
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    MEDIA_HAL_LOGI(MODULE_NAME, "trackId:%d", trackId);
    if (itemNum > PARAM_MAX_LEN) {
        MEDIA_HAL_LOGE(MODULE_NAME, "item num is too big");
        return MEDIA_HAL_ERR;
    }
    for (int32_t index = 0; index < itemNum; index++) {
        if (item[index].key == OPTIONAL_KEY_TYPE_PRE_CACHE) {
            muxerContext->recAttr.u32PreRecTimeSec = item[index].value.s32Value;
            MEDIA_HAL_LOGE(MODULE_NAME, "KEY_TYPE_PRE_CACHE  : %d ", item[index].value.s32Value);
            break;
        } else if (item[index].key == OPTIONAL_KEY_TYPE_RINGBUFFER_SIZE_IN_MSEC) {
            muxerContext->recAttr.astStreamAttr[0].bufferTimeMSec = item[index].value.u32Value;
            MEDIA_HAL_LOGI(MODULE_NAME, "KEY_TYPE_RINGBUFFER_SIZE_IN_MSEC  : %u ", item[index].value.u32Value);
            break;
        }
    }
    return MEDIA_HAL_OK;
}

static int32_t SetRingBufferSize(RecMuxerContext *muxerContext, const uint32_t *size)
{
    CHK_NULL_RETURN(size, MEDIA_HAL_INVALID_PARAM, "size");
    OptionalItem item;
    item.key = OPTIONAL_KEY_TYPE_RINGBUFFER_SIZE_IN_MSEC;
    item.size = 0;
    item.value.u32Value = *size;

    return RecMuxerSetParameter(muxerContext, 0, &item, 1);
}

static int32_t RecMuxerInvoke(RecMuxerHandle handle, uint32_t invokeId, VoidPtr arg)
{
    int32_t ret = MEDIA_HAL_ERR;
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    CHK_NULL_RETURN(arg, MEDIA_HAL_INVALID_PARAM, "arg");
    RecMuxerContext *muxerContext = (RecMuxerContext *)handle;
    MEDIA_HAL_LOGI(MODULE_NAME, "invokeId:%d ", invokeId);

    switch (invokeId) {
        case INVOKE_SET_CALLBACK:
            ret = RecMuxerSetCallBack(muxerContext, (const PluginCallback *)arg);
            break;
        case INVOKE_SET_DEGREE:
            ret = RecMuxerSetOrientation(muxerContext, (const int32_t *)arg);
            break;
        case INVOKE_SET_LOCATION:
            ret = RecMuxerSetLocation(muxerContext, (const Location *)arg);
            break;
        case INVOKE_SET_MANNUAL_SPIT:
            ret = RecMuxerSetFileSplitDuration(muxerContext, (const MannualSpit *)arg);
            break;
        case INVOKE_SET_REC_PRE_CACHE:
            ret = RecMuxerSetPreRecordTime(muxerContext, (const int32_t *)arg);
            break;
        case INVOKE_SET_RINGBUFFER_SIZE:
            ret = SetRingBufferSize(muxerContext, (const uint32_t *)arg);
            break;
        case INVOKE_WRITE_FRONT_GPS:
            ret = WriteMP4Gps(muxerContext->muxerConfig.hMuxerHandle, ((FrontCover *)arg)->frameAddr,
                ((FrontCover *)arg)->len);
            break;
        case INVOKE_WRITE_FRONT_COVER:
            ret = WriteMP4FrontCover(muxerContext->muxerConfig.hMuxerHandle, ((FrontCover *)arg)->frameAddr,
                ((FrontCover *)arg)->len);
            break;
        case INVOKE_SET_PREALLOC_SIZE:
            ret = RecMuxerSetPreAllocSize(muxerContext, (const int32_t *)arg);
            break;

        default:
            break;
    }
    return ret;
}

static td_s32 RecMuxerFind(const MuxerOutput *output, const ProtocolFun *protocol)
{
    CHK_NULL_RETURN(output, MEDIA_HAL_INVALID_PARAM, "output is NULL");
    if (output->type == OUT_TYPE_INVALID) {
        MEDIA_HAL_LOGE(MODULE_NAME, "out type invlid");
        return FORMAT_ERR_NOT_SUPPORT;
    }
    if (protocol != NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not support extra protocol");
        return FORMAT_ERR_NOT_SUPPORT;
    }
    if (output->format != FORMAT_TYPE_MPEG_4 && output->format != FORMAT_TYPE_TS) {
        return MEDIA_HAL_ERR;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "RecMuxer Found");
    return MEDIA_HAL_OK;
}

static MuxerPluginEntry g_hisiRecMuxerEntry = {
    .desc = {
        .libName = "libplugin_muxer_recorder.so",
        .name = "recorder_muxer",
        .version = "version 1.0.0",
        .description = "recorder_demuxer",
        .supportCapability = "mp4",
        .priority = HISI_MUXER_PRIORITY,
    },
    .fmt_find = RecMuxerFind,
    .fmt_open = RecMuxerOpen,
    .fmt_add_track = RecMuxerAddTrack,
    .fmt_start = RecMuxerStart,
    .fmt_write = RecMuxerWriteFrame,
    .fmt_stop = RecMuxerStop,
    .fmt_set_max_size = RecMuxerSetMaxFileSize,
    .fmt_set_max_duration = RecMuxerSetMaxFileDuration,
    .fmt_set_next_outputfile = RecMuxerSetNextOutputFile,
    .fmt_invoke = RecMuxerInvoke,
    .fmt_close = RecMuxerDestroy,
};

const MuxerPluginEntry *GetMuxer(void)
{
    return &g_hisiRecMuxerEntry;
}

