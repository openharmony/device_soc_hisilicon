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

#ifndef DISTRIBUTED_CONSTANTS_H
#define DISTRIBUTED_CONSTANTS_H

#include <string>
#include <vector>

namespace OHOS {
namespace DistributedHardware {
const uint32_t YUV_WIDTH_RATIO = 3;
const uint32_t YUV_HEIGHT_RATIO = 2;

const uint32_t DEVID_MAX_LENGTH = 256;
const uint32_t DHID_MAX_LENGTH =  256;
const uint32_t CONTAINER_CAPACITY_MAX_SIZE = 10 * 1024;
const uint32_t METADATA_CAPACITY_MAX_SIZE = 10 * 1024 * 1024;
const int32_t STREAM_HEIGHT_MAX_SIZE = 10000;
const int32_t STREAM_WIDTH_MAX_SIZE = 10000;

constexpr size_t DEFAULT_ENTRY_CAPACITY = 100;
constexpr size_t DEFAULT_DATA_CAPACITY = 2000;

const uint32_t SIZE_FMT_LEN = 2;
const uint32_t MAX_SUPPORT_WIDTH = 3840;
const uint32_t MAX_SUPPORT_HEIGHT = 2160;
const uint32_t MAX_SUPPORT_PHOTO_WIDTH = 4096;
const uint32_t MAX_SUPPORT_PHOTO_HEIGHT = 3072;
const std::string STAR_SEPARATOR = "*";

const uint32_t MIN_SUPPORT_DEFAULT_FPS = 15;
const uint32_t MAX_SUPPORT_DEFAULT_FPS = 30;

const int64_t MAX_FRAME_DURATION = 1000000000LL / 10;

const uint32_t BUFFER_QUEUE_SIZE = 2;

const uint32_t DEGREE_180 = 180;
const uint32_t DEGREE_240 = 240;

const uint32_t INGNORE_STR_LEN = 2;

const uint32_t WAIT_OPEN_TIMEOUT_SEC = 5;
const uint32_t BUFFER_SYNC_FENCE_TIMEOUT = 100;

const std::string ENCODE_TYPE_STR_H264 = "video/avc";
const std::string ENCODE_TYPE_STR_H265 = "video/hevc";
const std::string ENCODE_TYPE_STR_JPEG = "jpeg";
const std::string ENCODE_TYPE_STR_MPEG4_ES = "video/mp4v-es";
const std::string DC_LOG_TITLE_TAG = "DCAMERA";
constexpr int32_t LOG_MAX_LEN = 4096;
constexpr uint64_t SEC_TO_NSEC_TIMES = 1000000000;

typedef enum {
    OHOS_CAMERA_FORMAT_INVALID = 0,
    OHOS_CAMERA_FORMAT_RGBA_8888,
    OHOS_CAMERA_FORMAT_YCBCR_420_888,
    OHOS_CAMERA_FORMAT_YCRCB_420_SP,
    OHOS_CAMERA_FORMAT_JPEG,
} DCameraFormat;

typedef enum {
    DCAMERA_MESSAGE = 0,
    DCAMERA_OPERATION = 1,
} DCameraEventType;

typedef enum {
    DCAMERA_EVENT_CHANNEL_DISCONNECTED = 0,
    DCAMERA_EVENT_CHANNEL_CONNECTED = 1,
    DCAMERA_EVENT_CAMERA_SUCCESS = 2,

    DCAMERA_EVENT_CAMERA_ERROR = -1,
    DCAMERA_EVENT_OPEN_CHANNEL_ERROR = -2,
    DCAMERA_EVENT_CLOSE_CHANNEL_ERROR = -3,
    DCAMERA_EVENT_CONFIG_STREAMS_ERROR = -4,
    DCAMERA_EVENT_RELEASE_STREAMS_ERROR = -5,
    DCAMERA_EVENT_START_CAPTURE_ERROR = -6,
    DCAMERA_EVENT_STOP_CAPTURE_ERROR = -7,
    DCAMERA_EVENT_UPDATE_SETTINGS_ERROR = -8,
    DCAMERA_EVENT_DEVICE_ERROR = -9,
    DCAMERA_EVENT_DEVICE_PREEMPT = -10,
    DCAMERA_EVENT_DEVICE_IN_USE = -11,
    DCAMERA_EVENT_NO_PERMISSION = -12,
} DCameraEventResult;

enum DCameraBufferUsage : uint64_t {
    CAMERA_USAGE_SW_READ_OFTEN = (1 << 0),
    CAMERA_USAGE_SW_WRITE_OFTEN = (1 << 1),
    CAMERA_USAGE_MEM_DMA = (1 << 2),
};

using DCSceneType = enum _DCSceneType : int32_t {
    PREVIEW = 0,
    VIDEO = 1,
    PHOTO = 2
};

using RetCode = uint32_t;
enum Ret : uint32_t {
    RC_OK = 0,
    RC_ERROR,
};

struct DCResolution {
    int32_t width_;
    int32_t height_;

    DCResolution() : width_(0), height_(0) {}

    DCResolution(int32_t width, int32_t height) : width_(width), height_(height) {}

    bool operator ==(const DCResolution others) const
    {
        return (this->width_ == others.width_) && (this->height_ == others.height_);
    }

    bool operator <(const DCResolution others) const
    {
        return this->width_ < others.width_ ||
            (this->width_ == others.width_ && this->height_ < others.height_);
    }
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // DISTRIBUTED_CONSTANTS_H
