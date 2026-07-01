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

#include "muxer_manager.h"

#include <malloc.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include "format_common.h"
#include "format_interface.h"
#include "format_type.h"
#include "media_hal_common.h"
#include "muxer_interface.h"
#include "muxer_plugins_manager.h"
#include "protocol_plugins_manager.h"
#include "securec.h"

#define MODULE_NAME "MuxerManager"
#define FORMAT_ERR_NULL_PTR 0x02       /**<  using a NULL point */
#define FORMAT_ERR_NOFREE_CHANNEL 0x03
#define FORMAT_ERR_NO_AVAILABLE_STREAM_ID 0x04
#define FORMAT_ERR_NOT_CREATED 0x05
#define FORMAT_ERR_INVALID_PARAM 0x06
#define FORMAT_ERR_NOT_STARTED 0x07
#define FORMAT_ERR_NOT_SUPPORT 0x08
#define FORMAT_ERR_NO_MEMORY 0x09

#define FORMAT_MUXER_HANDLE_MAX_CNT 8
#define PARAM_MAX_LEN 32

typedef struct {
    void *innerCtx;
    MuxerOutput cfgParam;
    const MuxerPluginEntry *plugin;
    void *muxer;
    FormatState state;
    bool needInterleave;
    int64_t maxFileSize;
    int64_t maxDuration;
    FormatCallback callBack;
    pthread_mutex_t mutex;
} MuxerContext;

typedef struct {
    bool inited;
    pthread_mutex_t mutex;
} MuxerManager;

static MuxerManager g_muxerManager = {
    .inited = false,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
};

void MuxerManagerInit(void)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "func In");
    MuxerManager *muxerMgr = &g_muxerManager;
    MEDIA_HAL_LOCK(muxerMgr->mutex);
    if (muxerMgr->inited) {
        MEDIA_HAL_UNLOCK(muxerMgr->mutex);
        return;
    }
    MuxerPluginManagerInit();
    ProtocolPluginManagerInit();
    muxerMgr->inited = true;
    MEDIA_HAL_UNLOCK(muxerMgr->mutex);
}

void MuxerManagerDeInit(void)
{
    MuxerManager *muxerMgr = &g_muxerManager;
    MEDIA_HAL_LOCK(muxerMgr->mutex);
    muxerMgr->inited = false;
    MEDIA_HAL_UNLOCK(muxerMgr->mutex);
}

static MuxerContext *AllocMuxer(void)
{
    MuxerContext *ctx = (MuxerContext *)malloc(sizeof(MuxerContext));
    CHK_NULL_RETURN(ctx, NULL, "malloc failed");
    ctx->innerCtx = ctx;
    return ctx;
}

static void FreeMuxer(MuxerContext **ctx)
{
    if (ctx == NULL || *ctx == NULL) {
        return;
    }
    (*ctx)->innerCtx = NULL;
    free(*ctx);
    *ctx = NULL;
}

int32_t MuxerCreate(FormatHandle * const handle, const FormatOutputConfig *outputConfig)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "func In");
    CHK_NULL_RETURN(handle, FORMAT_ERR_NULL_PTR, "handle null");
    CHK_NULL_RETURN(outputConfig, FORMAT_ERR_INVALID_PARAM, "outputConfig null");
    MuxerContext *ctx = AllocMuxer();
    CHK_NULL_RETURN(ctx, FORMAT_ERR_NOFREE_CHANNEL, "alloc muxer failed");
    MEDIA_HAL_LOGI(MODULE_NAME, "output config type=%d, format=%d", outputConfig->type, outputConfig->format);
    ctx->cfgParam.type = (OutType)outputConfig->type;
    ctx->cfgParam.format = (outputConfig->format != OUTPUT_FORMAT_INVALID) ? (FormatType)outputConfig->format :
        FORMAT_TYPE_OGG;
    if (outputConfig->type == OUTPUT_TYPE_FD) {
        ctx->cfgParam.fd = outputConfig->fd;
    } else if (outputConfig->type == OUTPUT_TYPE_URI) {
        if (memcpy_s(ctx->cfgParam.url, PLUGIN_URL_LEN, outputConfig->url, URL_LEN) != EOK) {
            FreeMuxer(&ctx);
            MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s failed");
            return MEDIA_HAL_ERR;
        }
    }
    ctx->state = STATE_IDLE;
    ProtocolFun protocolFunc;
    ProtocolFun *func = NULL;
    if (ctx->cfgParam.type == OUT_TYPE_URI) {
        if (ProtocolPluginManagerFindPlugin(ctx->cfgParam.url, strlen(ctx->cfgParam.url) + 1, &protocolFunc) == 0) {
            func = &protocolFunc;
        }
    }
    ctx->plugin = MuxerPluginManagerFindPlugin(&ctx->cfgParam, func, &ctx->muxer);
    if (ctx->plugin == NULL) {
        FreeMuxer(&ctx);
        MEDIA_HAL_LOGE(MODULE_NAME, "find plugin failed");
        return FORMAT_ERR_NOT_SUPPORT;
    }
    pthread_mutex_init(&ctx->mutex, NULL);
    ctx->state = STATE_INIT;
    ctx->needInterleave = false;
    ctx->maxFileSize = -1;
    ctx->maxDuration = -1;
    ctx->callBack.privateDataHandle = NULL;
    ctx->callBack.OnError = NULL;
    ctx->callBack.OnInfo = NULL;
    *handle = ctx;
    MEDIA_HAL_LOGI(MODULE_NAME, "func out");
    return MEDIA_HAL_OK;
}

int32_t MuxerDestroy(const FormatHandle handle)
{
    MEDIA_HAL_LOGD(MODULE_NAME, "func In");
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    if (ctx->state == STATE_START) {
        ctx->plugin->fmt_stop(ctx->muxer, false);
        ctx->state = STATE_STOP;
    }
    ctx->plugin->fmt_close(ctx->muxer);
    ctx->muxer = NULL;
    ctx->plugin = NULL;
    ctx->state = STATE_IDLE;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    pthread_mutex_destroy(&ctx->mutex);
    FreeMuxer(&ctx);
    MEDIA_HAL_LOGD(MODULE_NAME, "func out");
    return MEDIA_HAL_OK;
}

int32_t MuxerAddTrack(const FormatHandle handle, const TrackSource *trackSource)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "func In");
    CHK_NULL_RETURN(trackSource, FORMAT_ERR_INVALID_PARAM, "trackSource null");
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    MuxerTrackSource mediaTrackSource = {0};
    mediaTrackSource.type = (StreamType)trackSource->trackSourceType;
    if (mediaTrackSource.type == STREAM_TYPE_VIDEO) {
        VideoInfo *videoInfo = &mediaTrackSource.info.videoInfo;
        videoInfo->codecType = CodecFormatToCodecType(trackSource->trackSourceInfo.videoInfo.codecType);
        videoInfo->width = trackSource->trackSourceInfo.videoInfo.width;
        videoInfo->height = trackSource->trackSourceInfo.videoInfo.height;
        videoInfo->bitRate = trackSource->trackSourceInfo.videoInfo.bitRate;
        videoInfo->frameRate = trackSource->trackSourceInfo.videoInfo.frameRate;
        videoInfo->speed = trackSource->trackSourceInfo.videoInfo.speed;
        videoInfo->keyFrameInterval = trackSource->trackSourceInfo.videoInfo.keyFrameInterval;
    } else if (mediaTrackSource.type == STREAM_TYPE_AUDIO) {
        AudioInfo *audioInfo = &mediaTrackSource.info.audioInfo;
        audioInfo->codecType = CodecFormatToCodecType(trackSource->trackSourceInfo.audioInfo.codecType);
        audioInfo->sampleRate = trackSource->trackSourceInfo.audioInfo.sampleRate;
        audioInfo->channelCount = trackSource->trackSourceInfo.audioInfo.channelCount;
        audioInfo->sampleBitWidth = (AudioSampleFormat)trackSource->trackSourceInfo.audioInfo.sampleBitWidth;
        audioInfo->samplesPerFrame = trackSource->trackSourceInfo.audioInfo.samplesPerFrame;
        audioInfo->avgBytesPerSec = trackSource->trackSourceInfo.audioInfo.avgBytesPerSec;
    } else if (mediaTrackSource.type == STREAM_TYPE_DATA) {
        DataInfo *dataInfo = &mediaTrackSource.info.dataInfo;
        dataInfo->frameRate = trackSource->trackSourceInfo.dataInfo.frameRate;
        dataInfo->bitRate = trackSource->trackSourceInfo.dataInfo.bitRate;
    }
    int32_t trackId = ctx->plugin->fmt_add_track(ctx->muxer, &mediaTrackSource);
    MEDIA_HAL_LOGI(MODULE_NAME, "func out");
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return trackId;
}

static int32_t OnError(CallbackHandle handle, int32_t errorType, int32_t errorCode)
{
    int32_t ret = MEDIA_HAL_ERR;
    CHK_NULL_RETURN(handle, FORMAT_ERR_NULL_PTR, "handle null");
    MuxerContext *ctx = (MuxerContext *)handle;
    if (ctx->callBack.OnError != NULL && ctx->callBack.privateDataHandle != NULL) {
        ret = ctx->callBack.OnError(ctx->callBack.privateDataHandle, errorType, errorCode);
    }
    return ret;
}

static int32_t OnInfo(CallbackHandle handle, int32_t type, int32_t extra)
{
    int32_t ret = MEDIA_HAL_ERR;
    CHK_NULL_RETURN(handle, FORMAT_ERR_NULL_PTR, "handle null");
    MuxerContext *ctx = (MuxerContext *)handle;
    if (ctx->callBack.OnInfo != NULL && ctx->callBack.privateDataHandle != NULL) {
        ret = ctx->callBack.OnInfo(ctx->callBack.privateDataHandle, type, extra);
    }
    return ret;
}

int32_t MuxerSetCallBack(const FormatHandle handle, const FormatCallback *callBack)
{
    CHK_NULL_RETURN(callBack, FORMAT_ERR_INVALID_PARAM, "callBack null");
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "func In");
    if (ctx->state != STATE_INIT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer not support set callback on non-init state");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    ctx->callBack = *callBack;
    if (ctx->plugin->fmt_invoke != NULL && ctx->muxer != NULL) {
        PluginCallback pluginCallback;
        pluginCallback.handle = handle;
        pluginCallback.OnError = OnError;
        pluginCallback.OnInfo = OnInfo;
        ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_SET_CALLBACK, &pluginCallback);
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "func out");
    return MEDIA_HAL_OK;
}


int32_t MuxerSetOrientation(const FormatHandle handle, int degrees)
{
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "degrees:%d", degrees);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, " Muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    if (ctx->plugin->fmt_invoke != NULL) {
        int32_t tmp = degrees;
        ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_SET_DEGREE, &tmp);
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return MEDIA_HAL_OK;
}


int32_t MuxerSetLocation(const FormatHandle handle, int latitude, int longitude)
{
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "func in");
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    if (ctx->plugin->fmt_invoke != NULL) {
        Location tmp;
        tmp.latitude = latitude;
        tmp.longitude = longitude;
        ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_SET_LOCATION, &tmp);
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return MEDIA_HAL_OK;
}

int32_t MuxerSetMaxFileSize(const FormatHandle handle, int64_t bytes)
{
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "bytes:%lld", bytes);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    ctx->maxFileSize = bytes;
    if (ctx->plugin->fmt_set_max_size != NULL && ctx->muxer != NULL) {
        ctx->plugin->fmt_set_max_size(ctx->muxer, bytes);
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return MEDIA_HAL_OK;
}

int32_t MuxerSetMaxFileDuration(const FormatHandle handle, int64_t durationUs)
{
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "durationUs:%lld", durationUs);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    ctx->maxDuration = durationUs;
    if (ctx->plugin->fmt_set_max_duration != NULL) {
        ctx->plugin->fmt_set_max_duration(ctx->muxer, durationUs);
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return MEDIA_HAL_OK;
}

int32_t MuxerSetFileSplitDuration(const FormatHandle handle, ManualSplitType type, int64_t timestampUs,
    uint32_t durationS)
{
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "type:%d, timestampUs:%lld, durationUs:%d", type, timestampUs, durationS);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    if (ctx->plugin->fmt_invoke != NULL) {
        MannualSpit tmp;
        tmp.type = (SplitType)type;
        tmp.timestampUs = timestampUs;
        tmp.durationS = durationS;
        ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_SET_MANNUAL_SPIT, &tmp);
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return MEDIA_HAL_OK;
}

int32_t MuxerStart(const FormatHandle handle)
{
    int32_t ret = MEDIA_HAL_OK;
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "func in");
    if (ctx->state == STATE_START) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer have started");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_OK;
    }
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    if (ctx->plugin->fmt_start != NULL) {
        ret = ctx->plugin->fmt_start(ctx->muxer);
    }
    if (ret != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "fmt_start falied");
    } else {
        ctx->state = STATE_START;
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t MuxerWriteFrame(const FormatHandle handle, const FormatFrame *frameData)
{
    int32_t ret;
    CHK_NULL_RETURN_NO_LOG(frameData, FORMAT_ERR_INVALID_PARAM);
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state != STATE_START) {
        MEDIA_HAL_LOGE(MODULE_NAME, "muxer not started");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }

    FormatPacket packet;
    FormatFrameToPacket(frameData, &packet);
    ret = ctx->plugin->fmt_write(ctx->muxer, &packet);

    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t MuxerSetNextOutputFile(const FormatHandle handle, int32_t fd)
{
    int32_t ret = MEDIA_HAL_ERR;
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "fd:%d", fd);
    if (ctx->plugin->fmt_set_next_outputfile != NULL) {
        ret = ctx->plugin->fmt_set_next_outputfile(ctx->muxer, fd);
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t MuxerStop(const FormatHandle handle, bool block)
{
    int32_t ret = MEDIA_HAL_ERR;
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGD(MODULE_NAME, "MuxerStop block :%d", block);
    if (ctx->state != STATE_START) {
        MEDIA_HAL_LOGE(MODULE_NAME, "muxer not started");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    if (ctx->plugin->fmt_stop != NULL) {
        ret = ctx->plugin->fmt_stop(ctx->muxer, block);
    }
    ctx->state = STATE_STOP;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t MuxerSetParameter(const FormatHandle handle, int32_t trackId, const ParameterItem *item, int32_t itemNum)
{
    MEDIA_HAL_UNUSED(trackId);
    int32_t ret = 0;
    CHK_NULL_RETURN(item, FORMAT_ERR_INVALID_PARAM, "item null");
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "muxer not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    if (itemNum > PARAM_MAX_LEN) {
        MEDIA_HAL_LOGE(MODULE_NAME, "itemNum[%d] is too big, max value:%d", itemNum, PARAM_MAX_LEN);
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    if (ctx->plugin->fmt_invoke == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not support");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return MEDIA_HAL_ERR;
    }
    for (int32_t index = 0; index < itemNum; index++) {
        if (item[index].key == KEY_TYPE_PRE_CACHE) {
            uint32_t value = item[index].value.s32Value;
            ret = ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_SET_REC_PRE_CACHE, &value);
        } else if (item[index].key == KEY_TYPE_SET_SYNC_BUFFER_MSEC) {
            uint32_t value = item[index].value.s32Value;
            ret = ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_SET_RINGBUFFER_SIZE, &value);
        } else if (item[index].key == KEY_TYPE_WRITE_FRONT_GPS) {
            FrontCover location = {0};
            location.frameAddr = (uint8_t*)item[index].value.pValue;
            index++;
            location.len = item[index].value.s32Value;
            ret = ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_WRITE_FRONT_GPS, &location);
        } else if (item[index].key == KEY_TYPE_WRITE_FRONT_COVER) {
            FrontCover location = {0};
            location.frameAddr = (uint8_t*)item[index].value.pValue;
            index++;
            location.len = item[index].value.s32Value;
            ret = ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_WRITE_FRONT_COVER, &location);
        } else if (item[index].key == KEY_TYPE_PRE_ALLOC_SIZE) {
            int32_t value = item[index].value.s32Value;
            ret = ctx->plugin->fmt_invoke(ctx->muxer, INVOKE_SET_PREALLOC_SIZE, &value);
        }
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t MuxerGetParameter(const FormatHandle handle, int32_t trackId, ParameterItem *item, int32_t itemNum)
{
    MEDIA_HAL_UNUSED(trackId);
    CHK_NULL_RETURN(item, FORMAT_ERR_INVALID_PARAM, "item null");
    const MuxerContext *cur = (const MuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    MuxerContext *ctx = (MuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "func in, itemNum:%d", itemNum);
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return MEDIA_HAL_OK;
}
