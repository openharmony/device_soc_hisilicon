/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
#include <cstdarg>
#include <cerrno>
#include "hifb_adapter.h"
#include "display_adapter_interface.h"
#include "display_common.h"
static int32_t DisplayOpenDevice(const char *path, int flags, mode_t mode)
{
    DISPLAY_LOGI();
    return OSAL_OPEN(path, flags, mode);
}

static int32_t DisplayCloseDevice(int32_t devFd)
{
    return OSAL_CLOSE(devFd);
}

static int32_t DisplayIoctl(int32_t devFd, uint32_t cmd, void *args)
{
    return OSAL_IOCTL(devFd, cmd, args);
}

static int32_t DisplayFbGetDmaBuffer(int32_t devFd)
{
    struct hifb_membuf_info fbMemBuf;
    fbMemBuf.fd = -1;
    int32_t ret = OSAL_IOCTL(devFd, HIFB_GET_MEMBUF, &fbMemBuf);
    if (ret < 0) {
        return -1;
    }
    return fbMemBuf.fd;
}

static int32_t DisplayFbFresh(int32_t devFd, DisplayFrameInfo *frame)
{
    if (frame == nullptr) {
        return DISPLAY_NULL_PTR;
    }

    struct hifb_frame_info fbFrameInfo;
    fbFrameInfo.rect.x = frame->rect.x;
    fbFrameInfo.rect.y = frame->rect.y;
    fbFrameInfo.rect.w = frame->rect.w;
    fbFrameInfo.rect.h = frame->rect.h;
    fbFrameInfo.in_fence = frame->inFence;
    fbFrameInfo.stride = frame->stride;
    fbFrameInfo.bufaddr = frame->bufaddr;
    fbFrameInfo.format = frame->format;
    DISPLAY_LOGI("bufaddr %{public}lx", fbFrameInfo.bufaddr);
    if (OSAL_IOCTL(devFd, HIFB_REFRESH_FRAMEINFO, &fbFrameInfo) != 0) {
        return DISPLAY_FAILURE;
    }
    frame->outFence = fbFrameInfo.out_fence;
    return DISPLAY_SUCCESS;
}

extern "C"
{
int32_t DisplayAdapaterInitialize(DisplayAdapterFuncs **funcs)
{
    DisplayAdapterFuncs *adapter = (DisplayAdapterFuncs *)calloc(1, sizeof(DisplayAdapterFuncs));
    if (adapter == nullptr) {
        return DISPLAY_FAILURE;
    }
    adapter->OpenDevice = DisplayOpenDevice;
    adapter->CloseDevice = DisplayCloseDevice;
    adapter->Ioctl = DisplayIoctl;
    adapter->FbGetDmaBuffer = DisplayFbGetDmaBuffer;
    adapter->FbFresh = DisplayFbFresh;
    *funcs = adapter;
    return DISPLAY_SUCCESS;
}

int32_t DisplayAdapaterUninitialize(DisplayAdapterFuncs *funcs)
{
    if (funcs != nullptr) {
        free(funcs);
    }
    return DISPLAY_SUCCESS;
}
}