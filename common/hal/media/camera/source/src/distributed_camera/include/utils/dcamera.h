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

#ifndef DISTRIBUTED_CAMERA_H
#define DISTRIBUTED_CAMERA_H

#include "v1_0/dcamera_types.h"

namespace OHOS {
namespace DistributedHardware {
using namespace OHOS::HDI::DistributedCamera::V1_0;

using RetCode = uint32_t;
using MetaType = int32_t;
const std::string BASE_64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

uint64_t GetCurrentLocalTimeStamp();

void SplitString(const std::string &str, std::vector<std::string> &tokens, const std::string &delimiters);

std::string Base64Encode(const unsigned char *toEncode, unsigned int len);

std::string Base64Decode(const std::string& basicString);

bool IsBase64(unsigned char c);

bool IsDhBaseInfoInvalid(const DHBase& dhBase);
} // namespace DistributedHardware
} // namespace OHOS
#endif // DISTRIBUTED_CAMERA_H
