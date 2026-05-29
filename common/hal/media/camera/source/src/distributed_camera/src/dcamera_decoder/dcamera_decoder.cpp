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

#include "dcamera_decoder.h"
#include <cstdio>
#include "securec.h"
#include "dcamera_utils.h"
#include "codec_interface.h"
#include "distributed_hardware_log.h"

const int CODEC_FAILURE = -1;
const int PARAM_MAX_NUM = 30;
DCameraDecoder::DCameraDecoder()
    : codecHandle_(nullptr)
{
}

DCameraDecoder::~DCameraDecoder()
{
}

int32_t DCameraDecoder::Init()
{
    if (isInited_) {
        return CODEC_SUCCESS;
    }
    int32_t ret = CodecInit();
    if (ret != CODEC_SUCCESS) {
        DHLOGE("Codec module init failed.(ret=%d)", ret);
        return CODEC_FAILURE;
    }
    isInited_ = true;
    DHLOGE("Codec module init succeed.");
}

int32_t DCameraDecoder::DeInit()
{
    if (!isInited_) {
        return HAL_SUCCESS;
    }
    int32_t ret = CodecDeinit();
    if (ret != CODEC_SUCCESS) {
        DHLOGE("CodecDeinitfailed.(ret=%d)", ret);
    }
    isInited_ = false;
    DHLOGI("CodecDeinit succeed.");
}

bool DCameraDecoder::HandleCreated()
{
    return codecHandle_ != nullptr;
}

bool DCameraDecoder::Started()
{
    return isStarted_;
}

int32_t DCameraDecoder::GetCapbilityByMime(AvCodecMime mime, CodecType type, uint32_t flags, CodecCapability &cap)
{
    int32_t ret = CodecGetCapbility(mime, type, flags, &cap);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

static bool ConvertVdecAttributToParams(VdecAttr &attr, Param *param,
                                        int maxLen, int &actualLen)
{
    int index = 0;
    param[index].key = KEY_MIMETYPE;
    param[index].val = (void*)&(attr.mime);
    param[index].size = sizeof(attr.mime);
    index++;
    param[index].key = KEY_WIDTH;
    param[index].val = (void*)&(attr.maxWidth);
    param[index].size = sizeof(attr.maxWidth);
    index++;
    param[index].key = KEY_HEIGHT;
    param[index].val = (void*)&(attr.maxHeight);
    param[index].size = sizeof(attr.maxHeight);
    index++;
    param[index].key = KEY_BUFFERSIZE;
    param[index].val = (void*)&(attr.bufSize);
    param[index].size = sizeof(attr.bufSize);
    index++;
    param[index].key = KEY_CODEC_TYPE;
    param[index].val = (void*)&(attr.type);
    param[index].size = sizeof(attr.type);
    index++;
    actualLen = index;
    if (actualLen > maxLen) {
        DHLOGE("anylsis vdec param too much");
        return false;
    }
    return true;
}

int32_t DCameraDecoder::CreateHandle(const std::string &name, VdecAttr &attr)
{
    int actualLen = 0;
    Param param[PARAM_MAX_NUM];
    memset_s(param, PARAM_MAX_NUM * sizeof(Param), 0x00, PARAM_MAX_NUM * sizeof(Param));
    if (ConvertVdecAttributToParams(attr, param, PARAM_MAX_NUM, actualLen) == false) {
        DHLOGE("convert fail");
    }

    int32_t ret = CodecCreate(name.c_str(), param, actualLen, &codecHandle_);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::DestroyHandle()
{
    int32_t ret = CodecDestroy(codecHandle_);
    codecHandle_ = nullptr;
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::SetPortBufferMode(DirectionType type, BufferMode mode)
{
    int32_t ret = CodecSetPortMode(codecHandle_, type, mode);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::SetMetadata(const Param *params, int paramCnt)
{
    int32_t ret = CodecSetParameter(codecHandle_, params, paramCnt);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::GetMetadata(Param *params, int paramCnt)
{
    int32_t ret = CodecGetParameter(codecHandle_, params, paramCnt);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::StartDec()
{
    if (isStarted_) {
        return CODEC_SUCCESS;
    }
    int32_t ret = CodecStart(codecHandle_);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    isStarted_ = true;
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::StopDec()
{
    if (!isStarted_) {
        return CODEC_FAILURE;
    }
    int32_t ret = CodecStop(codecHandle_);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    isStarted_ = false;
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::FlushDec()
{
    int32_t ret = CodecFlush(codecHandle_, ALL_TYPE);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::QueueInputBuffer(InputInfo &inputData, uint32_t timeoutMs)
{
    return CodecQueueInput(codecHandle_, &inputData, timeoutMs);
}

int32_t DCameraDecoder::DequeInputBuffer(InputInfo &inputData, uint32_t timeoutMs)
{
    int32_t ret = CodecDequeInput(codecHandle_, timeoutMs, &inputData);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::QueueOutputBuffer(OutputInfo &outInfo, uint32_t timeoutMs)
{
    int32_t ret = CodecQueueOutput(codecHandle_, &outInfo, timeoutMs, -1);
    if (ret != CODEC_SUCCESS) {
        return CODEC_FAILURE;
    }
    return CODEC_SUCCESS;
}

int32_t DCameraDecoder::DequeueOutputBuffer(OutputInfo &outInfo, uint32_t timeoutMs)
{
    return CodecDequeueOutput(codecHandle_, timeoutMs, nullptr, &outInfo);
}

int32_t DCameraDecoder::SetCallback(CodecCallback &cb, UINTPTR instance)
{
    return CodecSetCallback(codecHandle_, &cb, instance);
}

