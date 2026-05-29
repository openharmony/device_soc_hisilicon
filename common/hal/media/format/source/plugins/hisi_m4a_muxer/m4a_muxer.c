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

#include "muxer_interface.h"
#include "ss_mp4_format.h"
#include "dtcf_manager.h"
#include "media_hal_common.h"
#include "securec.h"

#include <malloc.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/prctl.h>
#include <unistd.h>

#define MODULE_NAME "HisiM4aMuxer"

#define BYTE_UNIT 1024
#define M4A_MUXER_PRIORITY 100

#define REC_MSEC_PER_USEC (1000000)
#define REC_SPLIT_TIME_THRESHOLD (0.9)
#define REC_SPLIT_SIZE_THRESHOLD (0.9)

/**<max file size, determined by the file system */
/**(4*1024*1024*1024-1*1024*1024*1024) */
#define REC_MAX_FILE_SIZE (3221225472LL)


typedef struct {
    MuxerOutput muxerOutput;
    MuxerTrackSource trackSource;
    RecMuxerHandle handle;
    RecMuxerHandle hTrackHandle;
    bool muxerStarted;
    int64_t maxFileSize;
    int64_t maxDuration;
    PluginCallback callBack;
    uint64_t streamSize;
    uint64_t baseFramePts;
    bool splitSizeBegin;
    bool splitTimeBegin;
} M4aMuxerContext;

static int32_t M4aMuxerFind(const MuxerOutput *output, const ProtocolFun *protocol)
{
    CHK_NULL_RETURN(output, MEDIA_HAL_INVALID_PARAM, "output is NULL");
    if (output->type == OUT_TYPE_INVALID) {
        MEDIA_HAL_LOGE(MODULE_NAME, "out type invlid");
        return MEDIA_HAL_INVALID_PARAM;
    }
    if (protocol != NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not support extra protocol");
        return MEDIA_HAL_INVALID_PARAM;
    }
    if (output->format != FORMAT_TYPE_M4A) {
        return MEDIA_HAL_ERR;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "M4aMuxer Found");
    return MEDIA_HAL_OK;
}

static int32_t OpenOutTypeUriProcess(const MuxerOutput *output)
{
    int optInit = FileOptInit(output->url);
    if (optInit != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "FileOptInit fail 0x%x", optInit);
        (void)FileOptDeInit();
        return MEDIA_HAL_ERR;
    }
    return MEDIA_HAL_OK;
}

static int32_t CreateM4aMuxer(M4aMuxerContext *ctx)
{
    int32_t ret;
    OT_MP4_CONFIG_S stMuxerConfig = { 0 };
    stMuxerConfig.enConfigType = OT_MP4_CONFIG_MUXER;
    if (ctx->muxerOutput.type == OUT_TYPE_FD) {
        MEDIA_HAL_LOGI(MODULE_NAME, "output fd :%d", ctx->muxerOutput.fd);
    } else if (ctx->muxerOutput.type == OUT_TYPE_URI) {
        MEDIA_HAL_LOGI(MODULE_NAME, "output url format:%d", ctx->muxerOutput.format);
        if (OpenOutTypeUriProcess(&ctx->muxerOutput) != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "OpenOutTypeUriProcess error");
            return MEDIA_HAL_ERR;
        }
        td_char fileNames[2][OT_REC_FILE_NAME_LEN];
        ret = RequestFileNames(NULL, 0, fileNames);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "RequestFileNames error");
            return MEDIA_HAL_ERR;
        }
        MEDIA_HAL_LOGE(MODULE_NAME, "FileNames:%s", fileNames);
        if (strncpy_s(stMuxerConfig.aszFileName, OT_MP4_MAX_FILE_NAME,
            fileNames[0], OT_MP4_MAX_FILE_NAME - 1) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "strncpy_s fileName error");
            return MEDIA_HAL_ERR;
        }
    }
    stMuxerConfig.stMuxerConfig.u32PreAllocUnit = 0;
    stMuxerConfig.stMuxerConfig.bCo64Flag = TD_FALSE;
    stMuxerConfig.stMuxerConfig.bConstantFps = TD_FALSE;
    stMuxerConfig.stMuxerConfig.u32VBufSize = BYTE_UNIT * BYTE_UNIT;
    stMuxerConfig.stMuxerConfig.u32BackupUnit = 5 * 1024 * 1024; /* 5 * 1024 * 1024: BackupUnit size in bytes */

    ret = SS_MP4_Create(&ctx->handle, &stMuxerConfig);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_MP4_Create failed:0x%x", ret);
        return ret;
    }
    return MEDIA_HAL_OK;
}

static int32_t M4aMuxerOpen(const MuxerOutput *output, const ProtocolFun *protocol, RecMuxerHandle *handle)
{
    MEDIA_HAL_UNUSED(protocol);
    CHK_NULL_RETURN(output, MEDIA_HAL_INVALID_PARAM, "output is null");
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle is null");
    
    M4aMuxerContext *ctx = (M4aMuxerContext*)malloc(sizeof(M4aMuxerContext));
    CHK_NULL_RETURN(ctx, MEDIA_HAL_NO_MEM, "M4aMuxerContext is null");
    if (memset_s(ctx, sizeof(M4aMuxerContext), 0x0, sizeof(M4aMuxerContext)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s M4aMuxerContext error");
        goto CREATE_FAILED;
    }
    ctx->muxerOutput = *output;
    int32_t ret = CreateM4aMuxer(ctx);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "CreateM4aMuxer failed:0x%x", ret);
        goto CREATE_FAILED;
    }
    *handle = ctx;
    MEDIA_HAL_LOGI(MODULE_NAME, "M4aMuxerOpen :%d", ret);
    return MEDIA_HAL_OK;
CREATE_FAILED:
    free(ctx);
    return MEDIA_HAL_ERR;
}

static int32_t DestroyM4aMuxer(M4aMuxerContext *ctx)
{
    CHK_NULL_RETURN(ctx->handle, MEDIA_HAL_INVALID_PARAM, "handle");
    td_s32 ret = SS_MP4_DestroyAllTracks(ctx->handle, NULL);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "call SS_MP4_DestroyAllTracks fail:0x%x", ret);
    }
    ret = SS_MP4_Destroy(ctx->handle, NULL);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "call SS_MP4_Destroy fail:0x%x", ret);
    }
    ctx->handle = NULL;
    ctx->hTrackHandle = NULL;
    ctx->muxerStarted = false;
    MEDIA_HAL_LOGI(MODULE_NAME, "DestroyM4aMuxer success");
    return MEDIA_HAL_OK;
}

static int32_t M4aMuxerDestroy(RecMuxerHandle handle)
{
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    CHK_NULL_RETURN(ctx, MEDIA_HAL_INVALID_PARAM, "handle");

    int32_t ret = DestroyM4aMuxer(ctx);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "call DestroyM4aMuxer fail:0x%x", ret);
    }
    (void)FileOptDeInit();
    free(ctx);
    MEDIA_HAL_LOGI(MODULE_NAME, "M4aMuxerDestroy success");
    return MEDIA_HAL_OK;
}

static int32_t AddAudioTrack(M4aMuxerContext *ctx)
{
    CHK_NULL_RETURN(ctx->handle, MEDIA_HAL_INVALID_PARAM, "Mp4Handle is null");
    OT_MP4_TRACK_INFO_S stTrakInfo = { 0 };
    if (ctx->trackSource.type == STREAM_TYPE_AUDIO) {
        stTrakInfo.enTrackType = OT_MP4_STREAM_AUDIO;
        if (snprintf_s(stTrakInfo.aszHdlrName, OT_MP4_MAX_HDLR_NAME,
            OT_MP4_MAX_HDLR_NAME - 1, "%s", "Hisilicon AUDIO") <= EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s stTrakInfo.aszHdlrName Hisilicon AUDIO error");
            return MEDIA_HAL_ERR;
        }
        stTrakInfo.stAudioInfo.u32Channels = ctx->trackSource.info.audioInfo.channelCount;
        stTrakInfo.stAudioInfo.u32SampleRate = ctx->trackSource.info.audioInfo.sampleRate;
        stTrakInfo.stAudioInfo.u16SampleSize = ctx->trackSource.info.audioInfo.sampleBitWidth;
        stTrakInfo.stAudioInfo.u32SamplePerFrame = ctx->trackSource.info.audioInfo.samplesPerFrame;
        stTrakInfo.stAudioInfo.enCodecID = OT_MP4_CODEC_ID_AACLC;
        stTrakInfo.fSpeed = 1;
        stTrakInfo.u32TimeScale = ctx->trackSource.info.audioInfo.sampleRate;
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "Not STREAM_TYPE_AUDIO");
        return MEDIA_HAL_ERR;
    }
    int32_t ret = SS_MP4_CreateTrack(ctx->handle, &ctx->hTrackHandle, &stTrakInfo);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_MP4_CreateTrack failed [0x%x]", ret);
        return ret;
    }
    return ret;
}

static int32_t M4aMuxerAddTrack(RecMuxerHandle handle, const MuxerTrackSource *trackSource)
{
    CHK_NULL_RETURN(trackSource, MEDIA_HAL_INVALID_PARAM, "trackSource is null");
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    CHK_NULL_RETURN(ctx, MEDIA_HAL_INVALID_PARAM, "M4aMuxerContext is null");
    ctx->trackSource = *trackSource;
    int32_t ret = AddAudioTrack(ctx);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "MuxerAddTrack failed [0x%x]", ret);
    }
    return ret;
}

static int32_t M4aMuxerStart(RecMuxerHandle handle)
{
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    CHK_NULL_RETURN(ctx, MEDIA_HAL_INVALID_PARAM, "ctx is null");
    CHK_NULL_RETURN(ctx->handle, MEDIA_HAL_INVALID_PARAM, "ctx handle is null");
    CHK_NULL_RETURN(ctx->hTrackHandle, MEDIA_HAL_INVALID_PARAM, "ctx handle is null");
    CHK_COND_RETURN(ctx->muxerStarted, MEDIA_HAL_OK, "ctx is Started");
    ctx->muxerStarted = true;
    ctx->splitSizeBegin = true;
    ctx->splitTimeBegin = true;
    MEDIA_HAL_LOGI(MODULE_NAME, "M4aMuxerStart success ");
    return MEDIA_HAL_OK;
}

static void ReportInfo(PluginCallback callBack, PluginInfoType infoType, int32_t extra)
{
    CHK_NULL_RETURN_NONE(callBack.OnInfo, "OnInfo invalid pointer");
    callBack.OnInfo(callBack.handle, infoType, extra);
}

static void ReportError(PluginCallback callBack, int32_t errorType, int32_t errorCode)
{
    CHK_NULL_RETURN_NONE(callBack.OnError, "OnError invalid pointer");
    callBack.OnError(callBack.handle, errorType, errorCode);
}

static bool SplitIsAchiveSize(M4aMuxerContext *ctx, const OT_MP4_FRAME_DATA_S *m4aFrameData)
{
    bool result = false;
    ctx->streamSize += m4aFrameData->u32DataLength;
    if (ctx->splitSizeBegin) {
        if (ctx->maxFileSize > 0 && ctx->streamSize >= ctx->maxFileSize * REC_SPLIT_SIZE_THRESHOLD) {
            ReportInfo(ctx->callBack, MAX_FILESIZE_APPROACHING,
                ctx->muxerOutput.type == OUT_TYPE_FD ? ctx->muxerOutput.fd : -1);
            MEDIA_HAL_LOGI(MODULE_NAME, "size %lld Approaching maxFileSize %lld", ctx->streamSize, ctx->maxFileSize);
            ctx->splitSizeBegin = false;
            return result;
        }
    }

    if ((ctx->maxFileSize > 0 && ctx->streamSize >= (uint64_t)ctx->maxFileSize) ||
        ctx->streamSize >= REC_MAX_FILE_SIZE) {
        ReportInfo(ctx->callBack, MAX_FILESIZE_REACHED,
            ctx->muxerOutput.type == OUT_TYPE_FD ? ctx->muxerOutput.fd : -1);
        result = true;
        MEDIA_HAL_LOGI(MODULE_NAME, "size %lld has Approached maxFileSize %lld", ctx->streamSize, ctx->maxFileSize);
        ctx->streamSize = 0;
        ctx->splitSizeBegin = true;
        ctx->baseFramePts = m4aFrameData->u64TimeStamp;
    }
    return result;
}

static bool SplitIsAchiveTime(M4aMuxerContext *ctx, const OT_MP4_FRAME_DATA_S *m4aFrameData)
{
    bool result = false;
    if (ctx->baseFramePts == 0) {
        ctx->baseFramePts = m4aFrameData->u64TimeStamp;
        MEDIA_HAL_LOGI(MODULE_NAME, "set base pts %llu", m4aFrameData->u64TimeStamp);
    }

    uint32_t realRecTimeLenMs = (uint32_t)((m4aFrameData->u64TimeStamp - ctx->baseFramePts) / REC_MSEC_PER_USEC);
    if (ctx->splitTimeBegin) {
        if (ctx->maxDuration > 0 && realRecTimeLenMs >= ctx->maxDuration * REC_SPLIT_TIME_THRESHOLD) {
            ReportInfo(ctx->callBack, MAX_DURATION_APPROACHING,
                ctx->muxerOutput.type == OUT_TYPE_FD ? ctx->muxerOutput.fd : -1);
            MEDIA_HAL_LOGI(MODULE_NAME, "realRecTimeLenMs %u Approaching maxDuration %lld",
                realRecTimeLenMs, ctx->maxDuration);
            ctx->splitTimeBegin = false;
            return result;
        }
    }
    if (ctx->maxDuration > 0 && realRecTimeLenMs >= ctx->maxDuration) {
        // split and reset manual split info
        ReportInfo(ctx->callBack, MAX_DURATION_REACHED,
            ctx->muxerOutput.type == OUT_TYPE_FD ? ctx->muxerOutput.fd : -1);
        result = true;
        ctx->streamSize = 0;
        ctx->splitTimeBegin = true;
        ctx->baseFramePts = m4aFrameData->u64TimeStamp;
    }
    return result;
}

static int32_t ReConfigMuxer(M4aMuxerContext *ctx)
{
    int32_t ret = DestroyM4aMuxer(ctx);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "call DestroyM4aMuxer fail:0x%x", ret);
        return MEDIA_HAL_ERR;
    }
    ret = CreateM4aMuxer(ctx);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "CreateM4aMuxer failed:0x%x", ret);
        return MEDIA_HAL_ERR;
    }
    ret = AddAudioTrack(ctx);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AddAudioTrack failed:0x%x", ret);
        DestroyM4aMuxer(ctx);
        return MEDIA_HAL_ERR;
    }
    ctx->muxerStarted = true;
    MEDIA_HAL_LOGE(MODULE_NAME, "ReConfigMuxer success");
    return MEDIA_HAL_OK;
}

static int32_t M4aMuxerWriteFrame(RecMuxerHandle handle, const FormatPacket *frameData)
{
    CHK_NULL_RETURN(frameData, MEDIA_HAL_INVALID_PARAM, "frameData");
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    CHK_NULL_RETURN(ctx, MEDIA_HAL_INVALID_PARAM, "M4aMuxerContext is null");
    CHK_COND_RETURN(!ctx->muxerStarted, MEDIA_HAL_ERR, "M4aMuxerContext is not Started");

    OT_MP4_FRAME_DATA_S m4aFrameData = {};
    m4aFrameData.u64TimeStamp = frameData->timestampUs;
    m4aFrameData.bKeyFrameFlag = frameData->isKeyFrame;
    m4aFrameData.u32DataLength = frameData->len;
    m4aFrameData.pu8DataBuffer = frameData->data;
    int32_t ret = SS_MP4_WriteFrame(ctx->handle, ctx->hTrackHandle, &m4aFrameData);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_MP4_WriteFrame ret 0x[%x].bKeyFrameFlag[%d] PTS[%llu]",
                       ret, m4aFrameData.bKeyFrameFlag, m4aFrameData.u64TimeStamp);
    }

    if (SplitIsAchiveSize(ctx, &m4aFrameData) || SplitIsAchiveTime(ctx, &m4aFrameData)) {
        ret = ReConfigMuxer(ctx);
        if (ret != MEDIA_HAL_OK) {
            goto RECONFIG_MUXER_FAILED;
        }
    }
    return ret;
RECONFIG_MUXER_FAILED:
    ReportError(ctx->callBack, ERROR_TYPE_INTERNAL_OPERATION_FAIL, ERROR_TYPE_INTERNAL_OPERATION_FAIL);
    return MEDIA_HAL_ERR;
}

static int32_t M4aMuxerStop(RecMuxerHandle handle, bool block)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    CHK_NULL_RETURN(ctx, MEDIA_HAL_INVALID_PARAM, "M4aMuxerContext");
    CHK_COND_RETURN(!ctx->muxerStarted, MEDIA_HAL_ERR, "M4aMuxerContext is not Started");
    MEDIA_HAL_LOGI(MODULE_NAME, " block %d", block);
    ctx->muxerStarted = false;
    return MEDIA_HAL_OK;
}

static int32_t M4aMuxerSetMaxFileSize(RecMuxerHandle handle, int64_t bytes)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    MEDIA_HAL_LOGI(MODULE_NAME, "bytes:%lld ", bytes);
    ctx->maxFileSize = bytes;
    return MEDIA_HAL_OK;
}

static int32_t M4aMuxerSetMaxFileDuration(RecMuxerHandle handle, int64_t durationUs)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "handle");
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    MEDIA_HAL_LOGI(MODULE_NAME, "durationUs:%lld ", durationUs);
    ctx->maxDuration = durationUs;
    return MEDIA_HAL_OK;
}

static int32_t M4aMuxerSetNextOutputFile(RecMuxerHandle handle, int32_t fd)
{
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    CHK_NULL_RETURN(ctx, MEDIA_HAL_INVALID_PARAM, "handle is null");
    MEDIA_HAL_LOGI(MODULE_NAME, "fd:%d", fd);
    if (ctx->muxerOutput.type == OUT_TYPE_URI) {
        return MEDIA_HAL_ERR;
    }
    ctx->muxerOutput.fd = fd;
    return MEDIA_HAL_OK;
}

static int32_t MuxerSetCallBack(M4aMuxerContext *ctx, const PluginCallback *callBack)
{
    CHK_NULL_RETURN(callBack, MEDIA_HAL_INVALID_PARAM, "callBack");
    ctx->callBack = *callBack;
    return MEDIA_HAL_OK;
}

static int32_t M4aMuxerInvoke(RecMuxerHandle handle, uint32_t invokeId, VoidPtr arg)
{
    MEDIA_HAL_UNUSED(arg);
    int32_t ret = MEDIA_HAL_ERR;
    M4aMuxerContext *ctx = (M4aMuxerContext *)handle;
    CHK_NULL_RETURN(ctx, MEDIA_HAL_INVALID_PARAM, "muxerContext");
    MEDIA_HAL_LOGI(MODULE_NAME, "invokeId:%d ", invokeId);

    switch (invokeId) {
        case INVOKE_SET_CALLBACK:
            ret = MuxerSetCallBack(ctx, (const PluginCallback *)arg);
            break;
        default:
            break;
    }
    return ret;
}

static MuxerPluginEntry g_hisiM4aMuxerEntry = {
    .desc = {
        .libName = "libplugin_muxer_m4a.so",
        .name = "m4a_muxer",
        .version = "version 1.0.0",
        .description = "m4a_demuxer",
        .supportCapability = "m4a",
        .priority = M4A_MUXER_PRIORITY,
    },
    .fmt_find = M4aMuxerFind,
    .fmt_open = M4aMuxerOpen,
    .fmt_add_track = M4aMuxerAddTrack,
    .fmt_start = M4aMuxerStart,
    .fmt_write = M4aMuxerWriteFrame,
    .fmt_stop = M4aMuxerStop,
    .fmt_set_max_size = M4aMuxerSetMaxFileSize,
    .fmt_set_max_duration = M4aMuxerSetMaxFileDuration,
    .fmt_set_next_outputfile = M4aMuxerSetNextOutputFile,
    .fmt_invoke = M4aMuxerInvoke,
    .fmt_close = M4aMuxerDestroy,
};

const MuxerPluginEntry *GetMuxer(void)
{
    return &g_hisiM4aMuxerEntry;
}
