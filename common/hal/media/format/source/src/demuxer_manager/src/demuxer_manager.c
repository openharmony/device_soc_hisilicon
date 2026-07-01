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

#include "demuxer_manager.h"

#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "demuxer_common_err.h"
#include "demuxer_plugins_manager.h"
#include "format_common.h"
#include "format_interface.h"
#include "format_type.h"
#include "media_hal_common.h"
#include "plugins_manager_common.h"
#include "protocol_plugins_manager.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MODULE_NAME "DemuxerManager"
#define SELECTED_AUDIO_TRACK_NUM_MAX 32
#define SELECTED_SUB_TRACK_NUM_MAX 32
#define SELECTED_IMG_TRACK_NUM_MAX 32
#define MAX_PROGRAM_NUM 256
#define MAX_TRACK_NUM_PER_PROGRAM 256
#define MAX_SUB_IMG_NUM_PER_TRACK 1024

typedef struct {
    bool inited;
    pthread_mutex_t mutex;
} DemuxerManager;

typedef struct {
    bool enable;
    bool isRunning;
    FormatBufferSetting bufferSetting;
    pthread_mutex_t mutex;
    pthread_t threadId;
} BufferCacheManager;

typedef struct {
    int16_t programId;
    int16_t videoTrackId;
    int16_t audioTrackId[SELECTED_AUDIO_TRACK_NUM_MAX];
    int16_t subTrackId[SELECTED_SUB_TRACK_NUM_MAX];
    int16_t imgTrackId[SELECTED_IMG_TRACK_NUM_MAX];
} SelectTrack;

typedef struct {
    void *innerCtx;
    const OT_DEMUX_S *demuxer;
    FormatState state;
    pthread_mutex_t mutex;
    DemuxerSource source;
    void *demuxerHdl;
    FileInfo info;
    FormatCallback callback;
    BufferCacheManager cache;
    SelectTrack selectTrack;
} DemuxerContext;

typedef int32_t (*ReadData)(void *handle, uint8_t *data, int32_t size, int32_t timeOutMs, DataType *flags);

DemuxerManager g_demuxerManager = {
    .inited = false,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
};

static int32_t TransToPluginSource(DemuxerSource *demuxerSrc, const FormatSource *formatSrc)
{
    if (formatSrc->type == SOURCE_TYPE_FD) {
        demuxerSrc->type = DEMUXER_SOURCE_TYPE_FD;
        demuxerSrc->fd = formatSrc->fd;
    } else if (formatSrc->type == SOURCE_TYPE_URI) {
        demuxerSrc->type = DEMUXER_SOURCE_TYPE_URI;
        if (memcpy_s(demuxerSrc->url, PLUGIN_URL_LEN, formatSrc->url, URL_LEN) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s failed");
            return -1;
        }
    } else if (formatSrc->type == SOURCE_TYPE_STREAM) {
        CHK_NULL_RETURN(formatSrc->stream, -1, "formatSrc->stream null");
        CHK_NULL_RETURN(formatSrc->stream->ReadData, -1, "formatSrc->stream null");
        CHK_NULL_RETURN(formatSrc->stream->GetReadableSize, -1, "formatSrc->stream null");
        CHK_NULL_RETURN(formatSrc->stream->handle, -1, "formatSrc->stream null");
        demuxerSrc->type = DEMUXER_SOURCE_TYPE_STREAM;
        demuxerSrc->stream.ReadData = (ReadData)formatSrc->stream->ReadData;
        demuxerSrc->stream.GetReadableSize = formatSrc->stream->GetReadableSize;
        demuxerSrc->stream.handle = formatSrc->stream->handle;
    } else {
        demuxerSrc->type = DEMUXER_SOURCE_TYPE_BUT;
    }
    return 0;
}

void DemuxerManagerInit(void)
{
    DemuxerManager *manager = &g_demuxerManager;
    MEDIA_HAL_LOCK(manager->mutex);
    if (manager->inited == true) {
        MEDIA_HAL_UNLOCK(manager->mutex);
        return;
    }
    DemuxerPluginManagerInit();
    ProtocolPluginManagerInit();
    manager->inited = true;
    MEDIA_HAL_UNLOCK(manager->mutex);
}

void DemuxerManagerDeInit(void)
{
    MEDIA_HAL_LOCK(g_demuxerManager.mutex);
    g_demuxerManager.inited = false;
    DemuxerPluginManagerDeInit();
    ProtocolPluginManagerDeInit();
    MEDIA_HAL_UNLOCK(g_demuxerManager.mutex);
    pthread_mutex_destroy(&g_demuxerManager.mutex);
}

int32_t DemuxerCreate(const FormatSource *source, FormatHandle * const handle)
{
    CHK_NULL_RETURN(handle, -1, "input param null");
    CHK_NULL_RETURN(source, -1, "input param null");

    DemuxerContext *ctx = (DemuxerContext*)malloc(sizeof(DemuxerContext));
    CHK_NULL_RETURN(ctx, -1, "malloc buffer failed");
    if (memset_s(ctx, sizeof(DemuxerContext), 0, sizeof(DemuxerContext)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s memory failed");
        goto FREE;
    }
    ctx->demuxer = NULL;
    ctx->state = STATE_IDLE;
    if (TransToPluginSource(&ctx->source, source) != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "format source trans to plugin source failed");
        goto FREE;
    }
    pthread_mutex_init(&ctx->mutex, NULL);
    ctx->selectTrack.programId = -1;
    ctx->selectTrack.videoTrackId = -1;
    int32_t i;
    for (i = 0; i < SELECTED_AUDIO_TRACK_NUM_MAX; i++) {
        ctx->selectTrack.audioTrackId[i] = -1;
    }
    for (i = 0; i < SELECTED_SUB_TRACK_NUM_MAX; i++) {
        ctx->selectTrack.subTrackId[i] = -1;
    }
    for (i = 0; i < SELECTED_IMG_TRACK_NUM_MAX; i++) {
        ctx->selectTrack.imgTrackId[i] = -1;
    }
    ctx->cache.enable = false;
    ctx->innerCtx = ctx;
    *handle = (FormatHandle)ctx;
    return 0;
FREE:
    free(ctx);
    return -1;
}

int32_t DemuxerSetParameter(const FormatHandle handle, int32_t trackId, const ParameterItem *metaData,
    int32_t metaDataCnt)
{
    CHK_NULL_RETURN(metaData, -1, "input param metaData null");
    int32_t ret = 0;
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    if (metaDataCnt == 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "no param to set");
        return -1;
    }

    MEDIA_HAL_LOCK(ctx->mutex);
    if (trackId != FORMAT_INVALID_TRACK_ID && ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "demuxer not inited, not support set track param");
        ret = -1;
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

static int32_t GetKeyTypeExtradata(const DemuxerContext *ctx, ParameterItem *metaData, int32_t trackId)
{
    int32_t i;
    for (i = 0; i < ctx->info.programInfo[0].track[trackId].itemCnt; i++) {
        if (ctx->info.programInfo[0].track[trackId].item[i].key == OPTIONAL_KEY_TYPE_EXTRADATA) {
            metaData->value.pValue = ctx->info.programInfo[0].track[trackId].item[i].value.pValue;
            metaData->size = ctx->info.programInfo[0].track[trackId].item[i].size;
            break;
        }
    }
    if (i >= ctx->info.programInfo[0].track[trackId].itemCnt) {
        return -1;
    }
    return 0;
}

int32_t DemuxerGetParameter(const FormatHandle handle, int32_t trackId, ParameterItem *metaData)
{
    CHK_NULL_RETURN(metaData, -1, "input param metaData null");
    int32_t ret = 0;
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;
    MEDIA_HAL_LOCK(ctx->mutex);
    if ((trackId != FORMAT_INVALID_TRACK_ID) && (ctx->state == STATE_IDLE)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "demuxer not inited, not support get track param");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    if (ctx->info.programInfo == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ctx->info.programInfo is null, not support get track param");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    if (trackId >= (int32_t)ctx->info.programInfo[0].trackNum) {
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    switch (metaData->key) {
        case KEY_TYPE_EXTRADATA:
            ret = GetKeyTypeExtradata(ctx, metaData, trackId);
            break;
        default:
            break;
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t DemuxerSetCallBack(const FormatHandle handle, const FormatCallback *callBack)
{
    CHK_NULL_RETURN(callBack, -1, "input param callBack null");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state >= STATE_START) {
        MEDIA_HAL_LOGE(MODULE_NAME, "demuxer not support set callback after inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }

    ctx->callback = *callBack;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return 0;
}

int32_t DemuxerSetBufferConfig(const FormatHandle handle, const FormatBufferSetting *setting)
{
    CHK_NULL_RETURN(setting, -1, "input param setting null");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state != STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "only support before prepare");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    ctx->cache.bufferSetting = *setting;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return 0;
}

int32_t DemuxerGetBufferConfig(const FormatHandle handle, FormatBufferSetting *setting)
{
    CHK_NULL_RETURN(setting, -1, "input param setting null");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    *setting = ctx->cache.bufferSetting;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return 0;
}


int32_t DemuxerPrepare(const FormatHandle handle)
{
    MEDIA_HAL_LOGD(MODULE_NAME, "func in");
    ProtocolFun protocolFunc;
    ProtocolFun *func = NULL;
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->source.type == DEMUXER_SOURCE_TYPE_URI) {
        if (ProtocolPluginManagerFindPlugin(ctx->source.url, strlen(ctx->source.url) + 1, &protocolFunc) == 0) {
            func = &protocolFunc;
        }
    }

    ctx->demuxer = DemuxerPluginManagerFindPlugin(&ctx->source, func, &ctx->demuxerHdl);
    if (ctx->demuxer == NULL || ctx->demuxerHdl == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "prepare failed");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    ctx->state = STATE_INIT;
    MEDIA_HAL_LOGD(MODULE_NAME, "func out");
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return 0;
}

static void FreeFileInfo(DemuxerContext *ctx)
{
    if (ctx->info.programInfo == NULL) {
        return;
    }
    uint32_t i;
    for (i = 0; i < ctx->info.programNum; i++) {
        if (ctx->info.programInfo[i].track == NULL) {
            continue;
        }
        for (uint32_t j = 0; j < ctx->info.programInfo[i].trackNum; j++) {
            TrackInfo *info = &ctx->info.programInfo[i].track[j];
            if (info->trackType == TRACK_TYPE_IMAGE && info->imgTrack.subImageInfo != NULL) {
                free(info->imgTrack.subImageInfo);
                info->imgTrack.subImageInfo = NULL;
            }
            if (info->item != NULL) {
                free(info->item);
                info->item = NULL;
            }
        }
        free(ctx->info.programInfo[i].track);
        ctx->info.programInfo[i].track = NULL;
    }
    free(ctx->info.programInfo);
    ctx->info.programInfo = NULL;
}

static const CodecFormat g_videoTypeToCodecFormat[OT_FORMAT_VIDEO_TYPE_BUTT + 1] = {
    CODEC_H264, CODEC_H265, CODEC_JPEG, CODEC_JPEG, CODEC_BUT, CODEC_BUT
};

static const CodecFormat g_audioTypeToCodecFormat[OT_FORMAT_AUDIO_TYPE_BUTT + 1] = {
    CODEC_AAC, CODEC_PCM, CODEC_G711A, CODEC_G711U, CODEC_MP3, CODEC_BUT
};

int32_t DemuxerGetFileInfo(const FormatHandle handle, FormatFileInfo *info)
{
    int ret;
    CHK_NULL_RETURN(info, -1, "input param info null");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state == STATE_IDLE || ctx->demuxer == NULL || ctx->demuxerHdl == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not inited, state:%d, or demuxer plugin null", ctx->state);
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    OT_FORMAT_FILE_INFO_S fileInfo = {0};
    ret = ctx->demuxer->stFmtFun.fmt_getinfo(ctx->demuxerHdl, &fileInfo);
    if (ret != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "fmt_getinfo failed, ret:%d", ret);
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return ret;
    }
    FreeFileInfo(ctx);
    MEDIA_HAL_LOGI(MODULE_NAME, "File info: video type=%d, audio type=%d", fileInfo.enVideoType, fileInfo.enAudioType);
    info->formatName = NULL;
    info->s64FileSize = fileInfo.s64FileSize;
    info->s64StartTime = fileInfo.s64StartTime;
    info->s64Duration = fileInfo.s64Duration;
    info->s32UsedVideoStreamIndex = fileInfo.s32UsedVideoStreamIndex;
    info->fFrameRate = fileInfo.fFrameRate;
    info->u32Bitrate = fileInfo.u32Bitrate;
    info->u32AudioChannelCnt = fileInfo.u32AudioChannelCnt;
    info->u32SampleRate = fileInfo.u32SampleRate;
    info->s32UsedAudioStreamIndex = fileInfo.s32UsedAudioStreamIndex;
    info->enVideoType = g_videoTypeToCodecFormat[fileInfo.enVideoType];
    info->enAudioType = g_audioTypeToCodecFormat[fileInfo.enAudioType];

    for (int32_t index = 0; index < FORMAT_DEMUXER_RESOLUTION_CNT; index++) {
        info->stStreamResolution[index].s32VideoStreamIndex = fileInfo.stStreamResolution[index].s32VideoStreamIndex;
        info->stStreamResolution[index].u32Width = fileInfo.stStreamResolution[index].u32Width;
        info->stStreamResolution[index].u32Height = fileInfo.stStreamResolution[index].u32Height;
        info->stStreamResolution[index].enVideoType =
            g_videoTypeToCodecFormat[fileInfo.stStreamResolution[index].enVideoType];
    }

    MEDIA_HAL_UNLOCK(ctx->mutex);
    return 0;
}

int32_t DemuxerSelectTrack(const FormatHandle handle, int32_t programId, int32_t trackId)
{
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;
    int32_t ret = 0;
    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    SelectTrack *selectTrack = &ctx->selectTrack;
    selectTrack->programId = (int16_t)programId;
    selectTrack->videoTrackId = (int16_t)trackId;

    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t DemuxerUnselectTrack(const FormatHandle handle, int32_t programId, int32_t trackId)
{
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not inited");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    (void)programId;
    (void)trackId;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return 0;
}

int32_t DemuxerStart(const FormatHandle handle)
{
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state != STATE_INIT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not inited, state:%d", ctx->state);
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    ctx->state = STATE_START;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return 0;
}

int32_t DemuxerGetSelectedTrack(const FormatHandle handle, int32_t *programId, int32_t trackId[], int32_t *nums)
{
    int32_t i;
    int addTrackCnt = 0;
    CHK_NULL_RETURN(nums, -1, "input param nums null");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state == STATE_IDLE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not call prepare");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    *programId = ctx->selectTrack.programId;
    if (ctx->selectTrack.videoTrackId != -1) {
        trackId[addTrackCnt] = ctx->selectTrack.videoTrackId;
        addTrackCnt++;
    }
    for (i = 0; i < SELECTED_AUDIO_TRACK_NUM_MAX; i++) {
        if (ctx->selectTrack.audioTrackId[i] == -1 || addTrackCnt == *nums) {
            break;
        }
        trackId[addTrackCnt++] = ctx->selectTrack.audioTrackId[i];
    }
    for (i = 0; i < SELECTED_SUB_TRACK_NUM_MAX; i++) {
        if (ctx->selectTrack.subTrackId[i] == -1 || addTrackCnt == *nums) {
            break;
        }
        trackId[addTrackCnt++] = ctx->selectTrack.subTrackId[i];
    }
    *nums = addTrackCnt;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return 0;
}

static int32_t ReadFrame(DemuxerContext *ctx, FormatFrame *frame)
{
    int ret;
    OT_FORMAT_PACKET_S lowerPacket = {0};
    ret = ctx->demuxer->stFmtFun.fmt_read(ctx->demuxerHdl, &lowerPacket);
    FormatPacket upperPacket = {0};
    FormatPacketToUpperPacket(&lowerPacket, &upperPacket);
    if ((ret == TD_SUCCESS) && (upperPacket.data != NULL) && (upperPacket.len == 0)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "fmt_read failed frame len:%d", frame->len);
        return TD_FAILURE;
    }
    if (ret == TD_SUCCESS) {
        FormatPacketToFrame(&upperPacket, frame);
        upperPacket.item = NULL;
        upperPacket.itemCnt = 0;
    }
    return ret;
}

int32_t DemuxerReadFrame(const FormatHandle handle, FormatFrame *frame, int32_t timeOutMs)
{
    MEDIA_HAL_UNUSED(timeOutMs);
    int ret;
    CHK_NULL_RETURN(frame, -1, "input param null");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state != STATE_START) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not started");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }

    ret = ReadFrame(ctx, frame);
    if ((ret == TD_SUCCESS) && (frame->data != NULL) && (frame->len == 0)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "fmt_read failed frame len:%d", frame->len);
        ret = TD_FAILURE;
    }
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t DemuxerFreeFrame(const FormatHandle handle, FormatFrame *frame)
{
    int ret;
    CHK_NULL_RETURN(frame, -1, "input param frame null");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state != STATE_START) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not started");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    FormatPacket packet;
    FormatFrameToPacket(frame, &packet);
    OT_FORMAT_PACKET_S lowerPacket = {0};
    FormatUpperPacketToPacket(&lowerPacket, &packet);

    ret = ctx->demuxer->stFmtFun.fmt_free(ctx->demuxerHdl, &lowerPacket);
    frame->data = NULL;
    frame->len = 0;
    frame->item = NULL;
    frame->itemCnt = 0;

    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}


int32_t DemuxerSeek(const FormatHandle handle, int32_t streamIndex, int64_t timeStampUs, FormatSeekMode mode)
{
    MEDIA_HAL_LOGD(MODULE_NAME, "func in");
    int ret;
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state != STATE_START) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not started");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }
    ret = ctx->demuxer->stFmtFun.fmt_seek(ctx->demuxerHdl, streamIndex, timeStampUs, mode);
    MEDIA_HAL_UNLOCK(ctx->mutex);
    return ret;
}

int32_t DemuxerStop(const FormatHandle handle)
{
    MEDIA_HAL_LOGD(MODULE_NAME, "func in");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state != STATE_START) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not started");
        MEDIA_HAL_UNLOCK(ctx->mutex);
        return -1;
    }

    ctx->state = STATE_STOP;
    MEDIA_HAL_UNLOCK(ctx->mutex);
    MEDIA_HAL_LOGD(MODULE_NAME, "func out");
    return 0;
}

int32_t DemuxerDestroy(const FormatHandle handle)
{
    MEDIA_HAL_LOGD(MODULE_NAME, "func in");
    const DemuxerContext *cur = (const DemuxerContext*)handle;
    CHK_HANDLE_VALID(cur);
    DemuxerContext *ctx = (DemuxerContext *)cur->innerCtx;

    MEDIA_HAL_LOCK(ctx->mutex);
    if (ctx->state != STATE_IDLE) {
        int ret = ctx->demuxer->stFmtFun.fmt_close(ctx->demuxerHdl);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "fmt_close failed, ret:%d", ret);
        }
    }
    ctx->demuxer = NULL;
    ctx->demuxerHdl = NULL;
    FreeFileInfo(ctx);
    MEDIA_HAL_UNLOCK(ctx->mutex);
    pthread_mutex_destroy(&ctx->mutex);
    ctx->innerCtx = NULL;
    free(ctx);
    MEDIA_HAL_LOGD(MODULE_NAME, "func out");
    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
