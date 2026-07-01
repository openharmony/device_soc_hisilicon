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

#ifndef CODEC_BUFFER_CHANNEL_H
#define CODEC_BUFFER_CHANNEL_H

#include <stdint.h>
#include "adec_common.h"
#include "aenc_common.h"
#include "codec_type.h"
#include "venc_common.h"
#include "vdec_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define CHANNEL_ERR_BASE 0X1000
#define CHANNEL_ERR_CHANNEL_NOT_STARTED (CHANNEL_ERR_BASE + 1)
#define CHANNEL_ERR_CHANNEL_CACHED (CHANNEL_ERR_BASE + 2)

typedef struct {
    void *pluginHandle;
    AvCodecMime mime;
    CodecPluginCommon *pluginCommon; // base ptr, actually ponit to derive obj
    bool channelCreated;
    bool channelStarted;
    int32_t frameIndex;
    bool cached;
    bool    isBindedMode;
    pthread_mutex_t channelLock;
} CodecBufferChannel;

int32_t CodecChannelCreate(void **channelCtx, CodecType type, const Param *attr, int len);

int32_t CodecChannelDestroy(CodecBufferChannel *channelCtx, CodecType type);

int32_t CodecChannelStart(CodecBufferChannel *channelCtx, CodecType type);

int32_t CodecChannelStop(CodecBufferChannel *channelCtx, CodecType type);

int32_t CodecChannelFlush(CodecBufferChannel *channelCtx, CodecType type);

int32_t CodecChannelQueueInput(CodecBufferChannel *channelCtx, CodecType type,
    const InputInfo *inputData, uint32_t timeoutMs);

int32_t CodecChannelQueueOutput(CodecBufferChannel *channelCtx, CodecType type,
    OutputInfo *outInfo, uint32_t timeoutMs);

int32_t CodecChannelDequeueOutput(CodecBufferChannel *channelCtx, CodecType type,
    OutputInfo *outInfo, uint32_t timeoutMs);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* CODEC_BUFFER_CHANNEL_H */

