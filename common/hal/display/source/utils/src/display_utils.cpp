/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include "display_utils.h"
#include "display_common.h"
#include "display_type.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
bool DisplayUtils::IsYuv(PixelFormat format)
{
    switch (format) {
        case PIXEL_FMT_YCBCR_420_P:
        case PIXEL_FMT_YUV_422_I:
        case PIXEL_FMT_YCBCR_422_SP:
        case PIXEL_FMT_YCRCB_422_SP:
        case PIXEL_FMT_YCBCR_420_SP:
        case PIXEL_FMT_YCRCB_420_SP:
        case PIXEL_FMT_YCBCR_422_P:
        case PIXEL_FMT_YCRCB_422_P:
        case PIXEL_FMT_YCRCB_420_P:
        case PIXEL_FMT_YUYV_422_PKG:
        case PIXEL_FMT_UYVY_422_PKG:
        case PIXEL_FMT_YVYU_422_PKG:
        case PIXEL_FMT_VYUY_422_PKG:
            return true;
        default:
            return false;
    }
}
}
}
}