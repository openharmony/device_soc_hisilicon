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

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "buffer_channel.h"
#include "codec_interface.h"
#include "codec_config.h"
#include "codec_plugin_manager.h"
#include "media_hal_common.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#define PARAM_UPPER_LIMIT 500
#define SLEEP_US 30000 // 30ms
#define TIME_OUT_SECONDS 3
#define US_TO_NSEC 1000
#define PATH_MAX_LEN 1025

#define MODULE_NAME "CODEC_INTERFACE"
#define MAX_HANDLE_INSTANCE_NUM 10

typedef struct {
    void *codecHandle;
    CodecType type;
    int instanceNum;
    AvCodecMime mime;
    bool isStarted;
    uint32_t frameIndex;
    bool isBindedMode;
    CodecCallback callback;
    UINTPTR callbackInstance;
    pthread_t callbackThread;
    bool asyncInputMode;
    bool asyncOutputMode;
    bool isOutputThreadRunning;
    pthread_mutex_t ctxMutex;
    pthread_cond_t ctxCond;
} CodecCtx;

typedef struct {
    CODEC_HANDLETYPE handle;
    bool isValid;
} HandleRecord;

typedef struct {
    char *dirPath;
    char *getImplSymbol;
    CodecType type;
} PluginConfig;

static const PluginConfig g_plugCfgs[] = {
    {"/usr/lib/", "VDecoderGetImpl", VIDEO_DECODER},
    {"/usr/lib/", "ADecoderGetImpl", AUDIO_DECODER},
    {"/usr/lib/", "AEncoderGetImpl", AUDIO_ENCODER},
    {"/usr/lib/", "VEncoderGetImpl", VIDEO_ENCODER},
};

#define NO_WAIT  0

static const CodecCapability capEntries[] = {
    {
        MEDIA_MIMETYPE_VIDEO_AVC, VIDEO_DECODER, {ALGIN_LEVEL_2, ALGIN_LEVEL_2}, {128, 128}, {1920, 1080},
        VID_BITRATE_LEVEL1, VID_BITRATE_LEVEL2, {{AAC_MAIN_PROFILE}, 1}, {{AVC_LEVEL_1}, 1},
        {{YVU_SEMIPLANAR_420}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },

    {
        MEDIA_MIMETYPE_VIDEO_HEVC, VIDEO_DECODER, {ALGIN_LEVEL_2, ALGIN_LEVEL_2}, {128, 128}, {1920, 1080},
        VID_BITRATE_LEVEL1, VID_BITRATE_LEVEL2, {{HEVC_MAIN_PROFILE}, 1}, {{HEVC_LEVEL_MAIN_2}, 1},
        {{YVU_SEMIPLANAR_420}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },

    {
        MEDIA_MIMETYPE_IMAGE_JPEG, VIDEO_DECODER, {ALGIN_LEVEL_2, ALGIN_LEVEL_2}, {128, 128}, {1920, 1080},
        VID_BITRATE_LEVEL1, VID_BITRATE_LEVEL2, {{INVALID_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{YVU_SEMIPLANAR_420}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },

    {
        MEDIA_MIMETYPE_AUDIO_AAC, AUDIO_DECODER, {ALGIN_LEVEL_INVALID, ALGIN_LEVEL_INVALID}, {0, 0}, {0, 0},
        VID_BITRATE_LEVEL1, AUD_BITRATE_LEVEL1, {{AAC_MAIN_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{YVU_SEMIPLANAR_420}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },

    {
        MEDIA_MIMETYPE_AUDIO_G711A, AUDIO_DECODER, {ALGIN_LEVEL_INVALID, ALGIN_LEVEL_INVALID}, {0, 0}, {0, 0},
        VID_BITRATE_LEVEL1, AUD_BITRATE_LEVEL1, {{INVALID_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{PIX_FORMAT_INVALID}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },

    {
        MEDIA_MIMETYPE_AUDIO_G711U, AUDIO_DECODER, {ALGIN_LEVEL_INVALID, ALGIN_LEVEL_INVALID}, {0, 0}, {0, 0},
        VID_BITRATE_LEVEL1, AUD_BITRATE_LEVEL1, {{INVALID_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{PIX_FORMAT_INVALID}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },

    {
        MEDIA_MIMETYPE_AUDIO_G726, AUDIO_DECODER, {ALGIN_LEVEL_INVALID, ALGIN_LEVEL_INVALID}, {0, 0}, {0, 0},
        VID_BITRATE_LEVEL1, AUD_BITRATE_LEVEL1, {{INVALID_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{PIX_FORMAT_INVALID}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    }
};

static bool g_inited = false;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static HandleRecord g_handlesRecord[MAX_HANDLE_INSTANCE_NUM];

static struct timeval g_timeInitStart = { };
static struct timeval g_timeInitEnd = { };
static struct timeval g_timeDeinitStart = { };
static struct timeval g_timeDeinitEnd = { };

static void InitHandleRecord(void)
{
    pthread_mutex_lock(&g_mutex);
    for (int i = 0; i < MAX_HANDLE_INSTANCE_NUM; i++) {
        g_handlesRecord[i].handle = NULL;
        g_handlesRecord[i].isValid = false;
    }
    pthread_mutex_unlock(&g_mutex);
}

static bool RecordThisHandle(CODEC_HANDLETYPE handle)
{
    bool hasSpace = false;
    pthread_mutex_lock(&g_mutex);
    for (int i = 0; i < MAX_HANDLE_INSTANCE_NUM; i++) {
        if (!g_handlesRecord[i].isValid) {
            hasSpace = true;
            g_handlesRecord[i].handle = handle;
            g_handlesRecord[i].isValid = true;
            break;
        }
    }
    pthread_mutex_unlock(&g_mutex);
    if (!hasSpace) {
        MEDIA_HAL_LOGW(MODULE_NAME, "no space to record handle");
        return false;
    }
    return true;
}

static bool CheckValidHandle(CODEC_HANDLETYPE handle)
{
    if (handle == NULL) {
        return false;
    }
    pthread_mutex_lock(&g_mutex);
    for (int i = 0; i < MAX_HANDLE_INSTANCE_NUM; i++) {
        if (g_handlesRecord[i].isValid && g_handlesRecord[i].handle == handle) {
            pthread_mutex_unlock(&g_mutex);
            return true;
        }
    }
    pthread_mutex_unlock(&g_mutex);
    return false;
}

static void DeleteHandle(CODEC_HANDLETYPE handle)
{
    pthread_mutex_lock(&g_mutex);
    for (int i = 0; i < MAX_HANDLE_INSTANCE_NUM; i++) {
        if (g_handlesRecord[i].isValid && g_handlesRecord[i].handle == handle) {
            g_handlesRecord[i].isValid = false;
            g_handlesRecord[i].handle = NULL;
        }
    }
    pthread_mutex_unlock(&g_mutex);
}

int32_t CodecInit(void)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "in");
    pthread_mutex_lock(&g_mutex);
    if (g_inited) {
        MEDIA_HAL_LOGI(MODULE_NAME, "Codec is inited");
        pthread_mutex_unlock(&g_mutex);
        return TD_SUCCESS;
    }
    int ret = MediaSystemInit();
    if (ret != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "MediaSystemInit failed ret:%d", ret);
    }
    gettimeofday(&g_timeInitStart, NULL);
    size_t cfgSize = sizeof(g_plugCfgs) / sizeof(g_plugCfgs[0]);
    for (size_t i = 0; i < cfgSize; i++) {
        ret = SearchLoadValidPlugin(g_plugCfgs[i].dirPath, g_plugCfgs[i].getImplSymbol, g_plugCfgs[i].type);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "search and load plugin type[%d] failed", g_plugCfgs[i].type);
            pthread_mutex_unlock(&g_mutex);
            return TD_FAILURE;
        }
    }
    gettimeofday(&g_timeInitEnd, NULL);
    CalDiffTimeBetween(g_timeInitStart, g_timeInitEnd, MODULE_NAME, "Codec init", true);
    g_inited = TD_TRUE;
    pthread_mutex_unlock(&g_mutex);
    InitHandleRecord();
    MEDIA_HAL_LOGI(MODULE_NAME, "out");
    return TD_SUCCESS;
}

int32_t CodecDeinit(void)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "in");
    pthread_mutex_lock(&g_mutex);
    gettimeofday(&g_timeDeinitStart, NULL);
    if (g_inited) {
        size_t cfgSize = sizeof(g_plugCfgs) / sizeof(g_plugCfgs[0]);
        for (size_t i = 0; i < cfgSize; i++) {
            int ret = UnLoadPlugin(g_plugCfgs[i].type);
            if (ret != TD_SUCCESS) {
                MEDIA_HAL_LOGE(MODULE_NAME, "unload plugin type[%d] failed", g_plugCfgs[i].type);
                pthread_mutex_unlock(&g_mutex);
                return TD_FAILURE;
            }
        }
        g_inited = TD_FALSE;
    }
    gettimeofday(&g_timeDeinitEnd, NULL);
    CalDiffTimeBetween(g_timeDeinitStart, g_timeDeinitEnd, MODULE_NAME, "Codec deinit", true);
    pthread_mutex_unlock(&g_mutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "out");
    return TD_SUCCESS;
}

int32_t CodecEnumerateCapbility(uint32_t index, CodecCapability *cap)
{
    MEDIA_HAL_UNUSED(index);
    MEDIA_HAL_UNUSED(cap);

    MEDIA_HAL_LOGE(MODULE_NAME, "not support now");
    return TD_FAILURE;
}

int32_t CodecGetCapability(AvCodecMime mime, CodecType type, uint32_t flags, CodecCapability *cap)
{
    MEDIA_HAL_UNUSED(flags);
    if (cap == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "cap ptr is null");
        return TD_FAILURE;
    }

    size_t size = sizeof(capEntries) / sizeof(capEntries[0]);
    for (size_t i = 0; i < size; i++) {
        bool match = capEntries[i].mime == mime && capEntries[i].type == type;
        if (match) {
            *cap = capEntries[i];
            return TD_SUCCESS;
        }
    }
    return TD_FAILURE;
}

static td_s32 FindDomainKind(const Param *attr, int len, CodecType *type)
{
    td_bool isFound = TD_FALSE;
    for (int i = 0; i < len; i++) {
        if (attr[i].key == KEY_CODEC_TYPE) {
            if (attr[i].size != sizeof(CodecType)) {
                MEDIA_HAL_LOGE(MODULE_NAME, "param kParamIndexDomainKind size wrong");
                return TD_FAILURE;
            }
            *type = *((CodecType *)(attr[i].val));
            isFound = TD_TRUE;
            break;
        }
    }
    if (!isFound) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 WhichType(const Param *attr, int len, CodecType *type)
{
    td_s32 ret = FindDomainKind(attr, len, type);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param not find domian or kind");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 CreateHandler(const Param *attr, int len, CodecType type, CodecCtx *ctx)
{
    void *handle = NULL;
    MEDIA_HAL_LOGI(MODULE_NAME, "codec type[%s]\n", GetCodecTypeName(type));
    int32_t ret = CodecChannelCreate(&handle, type, attr, len);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "type[%d] channel create failed ret:0X%X\n", type, ret);
        return TD_FAILURE;
    }
    ctx->codecHandle = handle;
    ctx->type = type;
    ctx->instanceNum = 0;
    ctx->isBindedMode = false;
    ctx->isStarted = false;
    if (memset_s(&(ctx->callback), sizeof(CodecCallback), 0x00, sizeof(CodecCallback)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    ctx->asyncInputMode = false;
    ctx->asyncOutputMode = false;
    pthread_mutex_init(&ctx->ctxMutex, NULL);
    pthread_cond_init(&ctx->ctxCond, NULL);
    return TD_SUCCESS;
}

int32_t CodecCreate(const char* name, const Param *attr, int len, CODEC_HANDLETYPE *handle)
{
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    if (attr == NULL || handle == NULL || name == NULL || len > PARAM_UPPER_LIMIT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param is invalid");
        return TD_FAILURE;
    }

    CodecType type = INVALID_TYPE;
    td_s32 ret = WhichType(attr, len, &type);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "codec type invalid");
        return TD_FAILURE;
    }

    CodecCtx *codecCtx = (CodecCtx *)malloc(sizeof(CodecCtx));
    if (codecCtx == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc context error");
        return TD_FAILURE;
    }
    if (!RecordThisHandle(codecCtx)) {
        free(codecCtx);
        return TD_FAILURE;
    }
    if (memset_s(codecCtx, sizeof(CodecCtx), 0x00, sizeof(CodecCtx)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    pthread_mutex_lock(&g_mutex);
    ret = CreateHandler(attr, len, type, codecCtx);
    pthread_mutex_unlock(&g_mutex);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "CreateHandler fail");
        DeleteHandle(codecCtx);
        free(codecCtx);
        return TD_FAILURE;
    }

    *handle = codecCtx;
    MEDIA_HAL_LOGD(MODULE_NAME, "out");
    return TD_SUCCESS;
}

static void DestoryHandler(CodecCtx *ctx)
{
    CodecBufferChannel *channelCtx = (CodecBufferChannel *)ctx->codecHandle;
    td_s32 ret = CodecChannelDestroy(channelCtx, ctx->type);
    ctx->codecHandle = NULL;
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "channel destroy failed");
    }
}

int32_t CodecDestroy(CODEC_HANDLETYPE handle)
{
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }
    pthread_mutex_lock(&g_mutex);
    CodecCtx *codecCtx = (CodecCtx *)handle;
    DestoryHandler(codecCtx);
    pthread_mutex_unlock(&g_mutex);
    DeleteHandle(handle);
    pthread_mutex_destroy(&codecCtx->ctxMutex);
    pthread_cond_destroy(&codecCtx->ctxCond);
    free(codecCtx);
    return TD_SUCCESS;
}

int32_t CodecSetPortMode(CODEC_HANDLETYPE handle, DirectionType type, BufferMode mode)
{
    MEDIA_HAL_UNUSED(type);
    MEDIA_HAL_UNUSED(mode);
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

int32_t CodecSetParameter(CODEC_HANDLETYPE handle, const Param *params, int paramCnt)
{
    if (!CheckValidHandle(handle) || params == NULL || paramCnt > PARAM_UPPER_LIMIT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle or params is not valid");
        return TD_FAILURE;
    }

    CodecCtx *ctx = (CodecCtx *)handle;
    pthread_mutex_lock(&ctx->ctxMutex);
    CodecBufferChannel *channelCtx = (CodecBufferChannel *)ctx->codecHandle;
    if (channelCtx == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "CodecBufferChannel NULL");
        pthread_mutex_unlock(&ctx->ctxMutex);
        return MEDIA_HAL_INVALID_PARAM;
    }

    if (channelCtx->channelCreated != true) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Channel not created yet");
        pthread_mutex_unlock(&ctx->ctxMutex);
        return TD_FAILURE;
    }

    int32_t ret = TD_SUCCESS;
    for (int i = 0; i < paramCnt; i++) {
        switch (params[i].key) {
            case KEY_DEVICE_ID: {
                ret = SetDevice(channelCtx, ctx->type, INPUT_TYPE, &params[i]);
                break;
            }
            case KEY_IMAGE_Q_FACTOR: {
                ret = CodecVencSetParameter(channelCtx, ctx->type, &params[i]);
                break;
            }
            case KEY_DEBREATHE_EFFECT: {
                ret = CodecVencSetParameter(channelCtx, ctx->type, &params[i]);
                break;
            }
            default: {
                MEDIA_HAL_LOGE(MODULE_NAME, "CodecSetParameter invalid param key");
                break;
            }
        }
    }
    pthread_mutex_unlock(&ctx->ctxMutex);
    return ret;
}

int32_t CodecGetParameter(CODEC_HANDLETYPE handle, Param *params, int paramCnt)
{
    MEDIA_HAL_UNUSED(paramCnt);
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }
    if (params == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param is NULL");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static void* AsyncOutputProcess(VoidPtr arg)
{
    if (arg == NULL) {
        return NULL;
    }
    CodecCtx *codecCtx = (CodecCtx *)arg;
    prctl(PR_SET_NAME, "AsyncOutputProcess", 0, 0, 0);
    while (codecCtx->isOutputThreadRunning) {
        OutputInfo outInfo = {};
        int32_t ret = CodecChannelDequeueOutput(codecCtx->codecHandle, codecCtx->type, &outInfo, 0);
        if (ret != TD_SUCCESS) {
            if (ret == CHANNEL_ERR_CHANNEL_CACHED) {
                MEDIA_HAL_LOGE(MODULE_NAME, "previous frame not released, wait a while");
                usleep(SLEEP_US);
            } else {
                MEDIA_HAL_LOGE(MODULE_NAME, "async process deq out fail");
            }
            continue;
        }

        if (codecCtx->callback.OutputBufferAvailable != NULL) {
            codecCtx->callback.OutputBufferAvailable((UINTPTR)codecCtx, codecCtx->callbackInstance, &outInfo);
        }
    }
    pthread_cond_signal(&codecCtx->ctxCond);
    return NULL;
}

static td_s32 StartCodec(CodecCtx *ctx)
{
    CodecBufferChannel *channelCtx = (CodecBufferChannel *)ctx->codecHandle;
    td_s32 ret = CodecChannelStart(channelCtx, ctx->type);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (ctx->asyncOutputMode) {
        ctx->isOutputThreadRunning = true;
        ret = pthread_create(&ctx->callbackThread, NULL, AsyncOutputProcess, ctx);
        if (ret != 0) {
            MEDIA_HAL_LOGE(MODULE_NAME, "pthread_create failed %d", ret);
            ctx->isOutputThreadRunning = false;
            return TD_FAILURE;
        }
    }
    ctx->isStarted = true;
    ctx->frameIndex = 0;
    return TD_SUCCESS;
}

int32_t CodecStart(CODEC_HANDLETYPE handle)
{
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }
    CodecCtx *codecCtx = (CodecCtx *)handle;
    pthread_mutex_lock(&codecCtx->ctxMutex);
    if (codecCtx->isStarted) {
        pthread_mutex_unlock(&codecCtx->ctxMutex);
        return TD_SUCCESS;
    }
    int32_t ret = StartCodec(codecCtx);
    pthread_mutex_unlock(&codecCtx->ctxMutex);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "CodecStart success");
    return TD_SUCCESS;
}

static td_s32 StopCodec(CodecCtx *ctx)
{
    CodecBufferChannel *channelCtx = (CodecBufferChannel *)ctx->codecHandle;
    td_s32 ret = CodecChannelStop(channelCtx, ctx->type);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    ctx->isStarted = false;
    return TD_SUCCESS;
}

int32_t CodecStop(CODEC_HANDLETYPE handle)
{
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }

    CodecCtx *codecCtx = (CodecCtx *)handle;
    pthread_mutex_lock(&codecCtx->ctxMutex);
    int32_t ret;
    if (codecCtx->asyncOutputMode && codecCtx->isOutputThreadRunning) {
        codecCtx->isOutputThreadRunning = false;
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        struct timespec timeOut = { now.tv_sec + TIME_OUT_SECONDS,  now.tv_nsec};
        ret = pthread_cond_timedwait(&codecCtx->ctxCond, &codecCtx->ctxMutex, &timeOut);
        if (ret == ETIMEDOUT) {
            MEDIA_HAL_LOGE(MODULE_NAME, "something wrong to timeout, check it");
        }
        pthread_join(codecCtx->callbackThread, NULL);
    }
    ret = StopCodec(codecCtx);
    pthread_mutex_unlock(&codecCtx->ctxMutex);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

int32_t CodecFlush(CODEC_HANDLETYPE handle, DirectionType directType)
{
    MEDIA_HAL_UNUSED(directType);
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }

    CodecCtx *codecCtx = (CodecCtx *)handle;
    pthread_mutex_lock(&codecCtx->ctxMutex);
    CodecBufferChannel *channelCtx = (CodecBufferChannel *)codecCtx->codecHandle;
    int32_t ret = CodecChannelFlush(channelCtx, codecCtx->type);
    pthread_mutex_unlock(&codecCtx->ctxMutex);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

int32_t CodecQueueInput(CODEC_HANDLETYPE handle, const InputInfo *inputData, uint32_t timeoutMs)
{
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }

    CodecCtx *codecCtx = (CodecCtx *)handle;
    pthread_mutex_lock(&codecCtx->ctxMutex);
    if (!codecCtx->isStarted) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not start");
        pthread_mutex_unlock(&codecCtx->ctxMutex);
        return TD_FAILURE;
    }

    CodecBufferChannel *channelCtx = (CodecBufferChannel *)codecCtx->codecHandle;
    int32_t ret = CodecChannelQueueInput(channelCtx, codecCtx->type, inputData, timeoutMs);
    pthread_mutex_unlock(&codecCtx->ctxMutex);
    return ret;
}

int32_t CodecDequeInput(CODEC_HANDLETYPE handle, uint32_t timeoutMs, InputInfo *inputData)
{
    MEDIA_HAL_UNUSED(timeoutMs);
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }
    if (inputData == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param is NULL");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

int32_t CodecQueueOutput(CODEC_HANDLETYPE handle, OutputInfo *outInfo, uint32_t timeoutMs, int releaseFenceFd)
{
    MEDIA_HAL_UNUSED(releaseFenceFd);
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }

    CodecCtx *codecCtx = (CodecCtx *)handle;
    pthread_mutex_lock(&codecCtx->ctxMutex);
    CodecBufferChannel *channelCtx = (CodecBufferChannel *)codecCtx->codecHandle;
    int32_t ret = CodecChannelQueueOutput(channelCtx, codecCtx->type, outInfo, timeoutMs);
    pthread_mutex_unlock(&codecCtx->ctxMutex);
    return ret;
}

int32_t CodecDequeueOutput(CODEC_HANDLETYPE handle, uint32_t timeoutMs, int *acquireFd, OutputInfo *outInfo)
{
    MEDIA_HAL_UNUSED(acquireFd);
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }
    CodecCtx *codecCtx = (CodecCtx *)handle;
    pthread_mutex_lock(&codecCtx->ctxMutex);
    CodecBufferChannel *channelCtx = (CodecBufferChannel *)codecCtx->codecHandle;
    int32_t ret = CodecChannelDequeueOutput(channelCtx, codecCtx->type, outInfo, timeoutMs);
    pthread_mutex_unlock(&codecCtx->ctxMutex);
    return ret;
}

int32_t CodecSetCallback(CODEC_HANDLETYPE handle, const CodecCallback *cb, UINTPTR instance)
{
    if (!CheckValidHandle(handle)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "handle is not valid");
        return TD_FAILURE;
    }
    if (cb == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "callback is NULL");
        return TD_FAILURE;
    }
    CodecCtx *codecCtx = (CodecCtx *)handle;
    pthread_mutex_lock(&codecCtx->ctxMutex);
    codecCtx->callback = *cb;
    codecCtx->callbackInstance = instance;
    codecCtx->asyncInputMode = (codecCtx->callback.InputBufferAvailable != NULL) ? true : false;
    codecCtx->asyncOutputMode = (codecCtx->callback.OutputBufferAvailable != NULL) ? true : false;
    pthread_mutex_unlock(&codecCtx->ctxMutex);
    return TD_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
