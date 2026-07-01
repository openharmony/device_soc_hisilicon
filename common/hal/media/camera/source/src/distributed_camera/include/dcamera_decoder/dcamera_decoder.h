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

#ifndef DCAMERA_DECODER_H
#define DCAMERA_DECODER_H
#include <string>
#include "codec_type.h"

typedef struct {
    CodecType type;
    AvCodecMime mime;
    uint32_t maxWidth;
    uint32_t maxHeight;
    uint32_t bufSize;
    uint32_t frameBufCnt;
    void *priv;
} VdecAttr;

class DCameraDecoder {
public:
    DCameraDecoder();

    virtual ~DCameraDecoder();

    int32_t Init();

    int32_t DeInit();

    bool HandleCreated();

    bool Started();

    static int32_t GetCapbilityByMime(AvCodecMime mime, CodecType type, uint32_t flags, CodecCapability &cap);

    int32_t CreateHandle(const std::string &name, VdecAttr &attr);

    int32_t DestroyHandle();

    int32_t SetPortBufferMode(DirectionType type, BufferMode mode);

    int32_t SetMetadata(const Param *params, int paramCnt);

    int32_t GetMetadata(Param *param, int paramCnt);

    int32_t StartDec();

    int32_t StopDec();

    int32_t FlushDec();

    int32_t QueueInputBuffer(InputInfo &inputData, uint32_t timeoutMs);

    int32_t DequeInputBuffer(InputInfo &inputData, uint32_t timeoutMs);

    int32_t QueueOutputBuffer(OutputInfo &outInfo, uint32_t timeoutMs);

    int32_t DequeueOutputBuffer(OutputInfo &outInfo, uint32_t timeoutMs);

    int32_t SetCallback(CodecCallback &cb, UINTPTR instance);

private:
    bool isInited_ = false;
    bool isStarted_ = false;
    CODEC_HANDLETYPE codecHandle_;
};

#endif
