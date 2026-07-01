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

#include "buffer_channel.h"
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include "codec_plugin_manager.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MODULE_NAME "CODEC_BUFFER_CHANNEL"

int32_t CodecChannelCreate(void **channelCtx, CodecType type, const Param *attr, int len)
{
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "create buffer channel context is invalid");

    AvCodecMime mime = GetCodecMime(attr, len);
    if (mime == MEDIA_MIMETYPE_INVALID) {
        return TD_FAILURE;
    }

    void *pluginEntry = FindSpecificPlugin(mime, true, type);
    if (pluginEntry == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "could not find correspond plugin for type: %d\n", mime);
        return TD_FAILURE;
    }

    CodecBufferChannel *chanCtx = (CodecBufferChannel *)malloc(sizeof(CodecBufferChannel));
    if (chanCtx == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc CodecBufferChannel failed\n");
        return TD_FAILURE;
    }
    if (memset_s(chanCtx, sizeof(CodecBufferChannel), 0x00, sizeof(CodecBufferChannel)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    chanCtx->mime = mime;

    (void)pthread_mutex_init(&chanCtx->channelLock, NULL);
    chanCtx->pluginCommon = (CodecPluginCommon *)pluginEntry;
    if (chanCtx->pluginCommon->pfnCreate == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "create fun ptr is NULL\n");
        pthread_mutex_destroy(&chanCtx->channelLock);
        free(chanCtx);
        return MEDIA_HAL_INVALID_PARAM;
    }
    int32_t ret = chanCtx->pluginCommon->pfnCreate(&chanCtx->pluginHandle, attr, len);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "channel create failed ret:0X%X", ret);
        pthread_mutex_destroy(&chanCtx->channelLock);
        free(chanCtx);
        return ret;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "channel create succeed");

    chanCtx->channelCreated = true;
    chanCtx->channelStarted = false;
    chanCtx->cached = false;
    chanCtx->isBindedMode = false;
    *channelCtx = (void *)chanCtx;
    return TD_SUCCESS;
}

int32_t CodecChannelDestroy(CodecBufferChannel *channelCtx, CodecType type)
{
    MEDIA_HAL_UNUSED(type);
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "CodecBufferChannel context is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon, MEDIA_HAL_INVALID_PARAM, "pluginCommon is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon->pfnDestroy, MEDIA_HAL_INVALID_PARAM,
        "destroy fun ptr is NULL");
    int32_t ret = channelCtx->pluginCommon->pfnDestroy(channelCtx->pluginHandle);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "plugin destroy failed Ret:0X%X", ret);
    }
    (void)pthread_mutex_destroy(&channelCtx->channelLock);
    free(channelCtx);
    return ret;
}

int32_t CodecChannelStart(CodecBufferChannel *channelCtx, CodecType type)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "start in type[%d]", type);
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "CodecBufferChannel context is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon, MEDIA_HAL_INVALID_PARAM, "pluginCommon is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon->pfnStart, MEDIA_HAL_INVALID_PARAM, "start fun ptr is NULL");

    MEDIA_HAL_LOCK(channelCtx->channelLock);
    int32_t ret = channelCtx->pluginCommon->pfnStart(channelCtx->pluginHandle);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "plugin start failed ret:0X%X type[%d]", ret, type);
        MEDIA_HAL_UNLOCK(channelCtx->channelLock);
        return ret;
    }
    channelCtx->channelStarted = true;
    MEDIA_HAL_UNLOCK(channelCtx->channelLock);
    MEDIA_HAL_LOGI(MODULE_NAME, "start out type[%d] common", type);
    return TD_SUCCESS;
}

int32_t CodecChannelStop(CodecBufferChannel *channelCtx, CodecType type)
{
    MEDIA_HAL_LOGD(MODULE_NAME, "stop in type[%d]", type);
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "CodecBufferChannel context is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon, MEDIA_HAL_INVALID_PARAM, "pluginCommon is NULL");

    if (channelCtx->channelCreated == false) {
        MEDIA_HAL_LOGE(MODULE_NAME, "channel not created, type[%d]", type);
        return TD_FAILURE;
    }
    if (channelCtx->channelStarted == false) {
        MEDIA_HAL_LOGE(MODULE_NAME, "channel has stopped, just return type[%d]", type);
        return TD_SUCCESS;
    }

    CHK_NULL_RETURN(channelCtx->pluginCommon->pfnStop, MEDIA_HAL_INVALID_PARAM, "stop fun ptr is NULL");
    MEDIA_HAL_LOCK(channelCtx->channelLock);
    int32_t ret = channelCtx->pluginCommon->pfnStop(channelCtx->pluginHandle);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "stop failed ret:0X%X type[%d]", ret, type);
        MEDIA_HAL_UNLOCK(channelCtx->channelLock);
        return ret;
    }
    channelCtx->channelStarted = false;
    MEDIA_HAL_UNLOCK(channelCtx->channelLock);
    MEDIA_HAL_LOGD(MODULE_NAME, "stop out type[%d]", type);
    return TD_SUCCESS;
}

int32_t CodecChannelFlush(CodecBufferChannel *channelCtx, CodecType type)
{
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "CodecBufferChannel context is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon, MEDIA_HAL_INVALID_PARAM, "pluginCommon is NULL");
    CHK_NULL_RETURN(channelCtx->pluginCommon->pfnReset, MEDIA_HAL_INVALID_PARAM, "reset fun ptr is NULL");

    MEDIA_HAL_LOCK(channelCtx->channelLock);
    int32_t ret = channelCtx->pluginCommon->pfnReset(channelCtx->pluginHandle);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "flush failed ret[0X%X] type[%d]", ret, type);
        MEDIA_HAL_UNLOCK(channelCtx->channelLock);
        return ret;
    }
    MEDIA_HAL_UNLOCK(channelCtx->channelLock);
    return TD_SUCCESS;
}

static int32_t CodecChannelQueueVideoInput(CodecBufferChannel *channelCtx, const InputInfo *inputData,
    uint32_t timeoutMs)
{
    AvVideoDecoder *vdecPluginEntry = (AvVideoDecoder *)channelCtx->pluginCommon;
    int32_t ret = vdecPluginEntry->pfnQueueInput(channelCtx->pluginHandle, inputData, timeoutMs);
    if (ret == VDEC_ERR_STREAM_BUF_FULL) {
        return CODEC_ERR_STREAM_BUF_FULL;
    } else if (ret != TD_SUCCESS) {
        return CODEC_ERR_INVALID_OP;
    }
    return TD_SUCCESS;
}

static int32_t CodecChannelQueueAudioInput(CodecBufferChannel *channelCtx, const InputInfo *inputData)
{
    AvAudioDecoder *adecPluginEntry = (AvAudioDecoder *)channelCtx->pluginCommon;
    int32_t ret = adecPluginEntry->pfnQueueInput(channelCtx->pluginHandle, inputData,
        channelCtx->frameIndex++, TD_FALSE);
    if (ret == ADEC_ERR_STREAM_BUF_FULL) {
        return CODEC_ERR_STREAM_BUF_FULL;
    } else if (ret != TD_SUCCESS) {
        return CODEC_ERR_INVALID_OP;
    }
    return TD_SUCCESS;
}

int32_t CodecChannelQueueInput(CodecBufferChannel *channelCtx, CodecType type,
    const InputInfo *inputData, uint32_t timeoutMs)
{
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "CodecBufferChannel context is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon, MEDIA_HAL_INVALID_PARAM, "vdecPluginEntry is null");

    MEDIA_HAL_LOCK(channelCtx->channelLock);
    int32_t ret;
    switch (type) {
        case VIDEO_DECODER:
            ret = CodecChannelQueueVideoInput(channelCtx, inputData, timeoutMs);
            if (ret != TD_SUCCESS) {
                goto FAILED;
            }
            break;
        case AUDIO_DECODER:
            ret = CodecChannelQueueAudioInput(channelCtx, inputData);
            if (ret != TD_SUCCESS) {
                goto FAILED;
            }
            break;
        case VIDEO_ENCODER:
        case AUDIO_ENCODER: {
            break;
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "invalid type");
            break;
        }
    }
    MEDIA_HAL_UNLOCK(channelCtx->channelLock);
    return TD_SUCCESS;

FAILED:
    if (ret == CODEC_ERR_STREAM_BUF_FULL) {
        MEDIA_HAL_LOGD(MODULE_NAME, "send input but full wait a while, type[%d]", type);
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "send input failed ret[0X%X] type[%d]", ret, type);
    }
    MEDIA_HAL_UNLOCK(channelCtx->channelLock);
    return ret;
}

// this for out buffer allocated by codec
int32_t CodecChannelQueueOutput(CodecBufferChannel *channelCtx, CodecType type,
    OutputInfo *outInfo, uint32_t timeoutMs)
{
    MEDIA_HAL_UNUSED(timeoutMs);
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "CodecBufferChannel context is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon, MEDIA_HAL_INVALID_PARAM, "pluginCommon is null");
    CHK_NULL_RETURN(outInfo, MEDIA_HAL_INVALID_PARAM, "OutputInfo is null");

    if (!channelCtx->cached) {
        return TD_SUCCESS;
    }

    MEDIA_HAL_LOCK(channelCtx->channelLock);
    int32_t ret;
    switch (type) {
        case VIDEO_DECODER: {
            AvVideoDecoder *vdecPluginEntry = (AvVideoDecoder *)channelCtx->pluginCommon;
            ret = vdecPluginEntry->pfnQueueOutput(channelCtx->pluginHandle, outInfo, -1);
            break;
        }
        case AUDIO_DECODER: {
            AvAudioDecoder *adecPluginEntry = (AvAudioDecoder *)channelCtx->pluginCommon;
            ret = adecPluginEntry->pfnQueueOutput(channelCtx->pluginHandle, outInfo);
            break;
        }
        case VIDEO_ENCODER: {
            AvVideoEncoder *vencPluginEntry = (AvVideoEncoder *)channelCtx->pluginCommon;
            ret = vencPluginEntry->pfnQueueOutput(channelCtx->pluginHandle, outInfo);
            break;
        }
        case AUDIO_ENCODER: {
            AvAudioEncoder *aencPluginEntry = (AvAudioEncoder *)channelCtx->pluginCommon;
            ret = aencPluginEntry->pfnQueueOutput(channelCtx->pluginHandle, outInfo);
            break;
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "invalid type");
            ret = TD_FAILURE;
            break;
        }
    }
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "plugin queueout failed[0X%X] type[%d]", ret, type);
    }
    channelCtx->cached = false;
    MEDIA_HAL_UNLOCK(channelCtx->channelLock);
    return ret;
}

static int32_t ChannelDequeueOutInternal(CodecBufferChannel *channelCtx, CodecType type,
    OutputInfo *outInfo, uint32_t timeoutMs)
{
    int32_t ret;
    switch (type) {
        case VIDEO_DECODER: {
            AvVideoDecoder *vdecPluginEntry = (AvVideoDecoder *)channelCtx->pluginCommon;
            ret = vdecPluginEntry->pfnDequeueOutput(channelCtx->pluginHandle, outInfo, timeoutMs, NULL);
            if (ret == VDEC_ERR_FRAME_BUF_EMPTY) {
                ret = CODEC_ERR_FRAME_BUF_EMPTY;
            } else if (ret == VDEC_FRAME_BUF_EOS) {
                ret =  CODEC_RECEIVE_EOS;
            }
            break;
        }
        case AUDIO_DECODER: {
            AvAudioDecoder *adecPluginEntry = (AvAudioDecoder *)channelCtx->pluginCommon;
            ret = adecPluginEntry->pfnDequeueOutput(channelCtx->pluginHandle, outInfo, TD_FALSE);
            if (ret == ADEC_ERR_FRAME_BUF_EMPTY) {
                ret = CODEC_ERR_FRAME_BUF_EMPTY;
            } else if (ret == ADEC_RECEIVE_EOS) {
                ret =  CODEC_RECEIVE_EOS;
            }
            break;
        }
        case VIDEO_ENCODER: {
            AvVideoEncoder *vencPluginEntry = (AvVideoEncoder *)channelCtx->pluginCommon;
            ret = vencPluginEntry->pfnDequeueOutput(channelCtx->pluginHandle, outInfo, TD_FALSE);
            break;
        }
        case AUDIO_ENCODER: {
            AvAudioEncoder *aencPluginEntry = (AvAudioEncoder *)channelCtx->pluginCommon;
            ret = aencPluginEntry->pfnDequeueOutput(channelCtx->pluginHandle, outInfo, TD_FALSE);
            break;
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "invalid type");
            ret = TD_FAILURE;
            break;
        }
    }
    return ret;
}

int32_t CodecChannelDequeueOutput(CodecBufferChannel *channelCtx, CodecType type,
    OutputInfo *outInfo, uint32_t timeoutMs)
{
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "CodecBufferChannel context is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(outInfo, MEDIA_HAL_INVALID_PARAM, "OutputInfo is null");

    if (channelCtx->cached) {
        return CHANNEL_ERR_CHANNEL_CACHED;
    }

    if (channelCtx->channelStarted == false) {
        MEDIA_HAL_LOGE(MODULE_NAME, "channel not started type[%d]", type);
        return CHANNEL_ERR_CHANNEL_NOT_STARTED;
    }

    MEDIA_HAL_LOCK(channelCtx->channelLock);
    int32_t ret = ChannelDequeueOutInternal(channelCtx, type, outInfo, timeoutMs);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGD(MODULE_NAME, "plugin dequeue out failed[OX%X] type[%d]", ret, type);
        MEDIA_HAL_UNLOCK(channelCtx->channelLock);
        return ret;
    }
    channelCtx->cached = true;
    MEDIA_HAL_UNLOCK(channelCtx->channelLock);
    return TD_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
