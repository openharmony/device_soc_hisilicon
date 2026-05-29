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

#ifndef OHOS_DISTRIBUTED_CAMERA_HAL_LOG_H
#define OHOS_DISTRIBUTED_CAMERA_HAL_LOG_H

#include <string>

#include "dh_log.h"

namespace OHOS {
namespace DistributedHardware {
#define FILENAME (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define SL(x) #x
#define STR_LINE(x) SL(x)
#define DH_STR_LINE STR_LINE(__LINE__)

#define DHLOGD(fmt, ...) DH_HAL_Log(DH_LOG_DEBUG, \
    (std::string("[") + DH_LOG_TAG + "][" + FILENAME + "][" + __FUNCTION__ + ":" + DH_STR_LINE + "]:" + \
    fmt).c_str(), ##__VA_ARGS__)

#define DHLOGI(fmt, ...) DH_HAL_Log(DH_LOG_INFO, \
    (std::string("[") + DH_LOG_TAG + "][" + FILENAME + "][" + __FUNCTION__ + ":" + DH_STR_LINE + "]:" + \
    fmt).c_str(), ##__VA_ARGS__)

#define DHLOGW(fmt, ...) DH_HAL_Log(DH_LOG_WARN, \
    (std::string("[") + DH_LOG_TAG + "][" + FILENAME + "][" + __FUNCTION__ + ":" + DH_STR_LINE + "]:" + \
    fmt).c_str(), ##__VA_ARGS__)

#define DHLOGE(fmt, ...) DH_HAL_Log(DH_LOG_ERROR, \
    (std::string("[") + DH_LOG_TAG + "][" + FILENAME + "][" + __FUNCTION__ + ":" + DH_STR_LINE + "]:" + \
    fmt).c_str(), ##__VA_ARGS__)

} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DISTRIBUTED_CAMERA_LOG_H
