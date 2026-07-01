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

#include "dcamera.h"
#include <chrono>
#include "constants.h"

namespace OHOS {
namespace DistributedHardware {
const uint32_t OFFSET2 = 2;
const uint32_t OFFSET4 = 4;
const uint32_t OFFSET6 = 6;
const uint8_t PARAM_FC = 0xfc;
const uint8_t PARAM_03 = 0x03;
const uint8_t PARAM_F0 = 0xf0;
const uint8_t PARAM_0F = 0x0f;
const uint8_t PARAM_C0 = 0xc0;
const uint8_t PARAM_3F = 0x3f;
const int INDEX_FIRST = 0;
const int INDEX_SECOND = 1;
const int INDEX_THIRD = 2;
const int INDEX_FORTH = 3;

uint64_t GetCurrentLocalTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp =
        std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    return static_cast<uint64_t>(tmp.count());
}

void SplitString(const std::string &str, std::vector<std::string> &tokens, const std::string &delimiters)
{
    std::string::size_type lastPos = 0;
    std::string::size_type pos = str.find(delimiters);
    while (std::string::npos != pos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiters.size();
        pos = str.find(delimiters, lastPos);
    }
    if (lastPos != str.length()) {
        tokens.push_back(str.substr(lastPos));
    }
}

std::string Base64Encode(const unsigned char *toEncode, unsigned int len)
{
    std::string ret;
    uint32_t i = 0;
    unsigned char charArray3[3];
    unsigned char charArray4[4];

    while (len--) {
        charArray3[i++] = *(toEncode++);
        if (i == sizeof(charArray3)) {
            charArray4[INDEX_FIRST] = (charArray3[INDEX_FIRST] & PARAM_FC) >> OFFSET2;
            charArray4[INDEX_SECOND] = ((charArray3[INDEX_FIRST] & PARAM_03) << OFFSET4) +
                ((charArray3[INDEX_SECOND] & PARAM_F0) >> OFFSET4);
            charArray4[INDEX_THIRD] = ((charArray3[INDEX_SECOND] & PARAM_0F) << OFFSET2) +
                ((charArray3[INDEX_THIRD] & PARAM_C0) >> OFFSET6);
            charArray4[INDEX_FORTH] = charArray3[INDEX_THIRD] & PARAM_3F;
            for (i = 0; i < sizeof(charArray4); i++) {
                ret += BASE_64_CHARS[charArray4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        uint32_t j = 0;
        for (j = i; j < sizeof(charArray3); j++) {
            charArray3[j] = '\0';
        }
        charArray4[INDEX_FIRST] = (charArray3[INDEX_FIRST] & PARAM_FC) >> OFFSET2;
        charArray4[INDEX_SECOND] = ((charArray3[INDEX_FIRST] & PARAM_03) << OFFSET4) +
            ((charArray3[INDEX_SECOND] & PARAM_F0) >> OFFSET4);
        charArray4[INDEX_THIRD] = ((charArray3[INDEX_SECOND] & PARAM_0F) << OFFSET2) +
            ((charArray3[INDEX_THIRD] & PARAM_C0) >> OFFSET6);
        charArray4[INDEX_FORTH] = charArray3[INDEX_THIRD] & PARAM_3F;
        for (j = 0; j < i + 1; j++) {
            ret += BASE_64_CHARS[charArray4[j]];
        }
        while (i++ < sizeof(charArray3)) {
            ret += '=';
        }
    }
    return ret;
}

std::string Base64Decode(const std::string& basicString)
{
    std::string ret;
    uint32_t i = 0;
    int index = 0;
    int len = static_cast<int>(basicString.size());
    unsigned char charArray3[3];
    unsigned char charArray4[4];

    while (len-- && (basicString[index] != '=') && IsBase64(basicString[index])) {
        charArray4[i++] = basicString[index];
        index++;
        if (i == sizeof(charArray4)) {
            for (i = 0; i < sizeof(charArray4); i++) {
                charArray4[i] = BASE_64_CHARS.find(charArray4[i]);
            }
            charArray3[INDEX_FIRST] = (charArray4[INDEX_FIRST] << OFFSET2) +
                ((charArray4[INDEX_SECOND] & 0x30) >> OFFSET4);
            charArray3[INDEX_SECOND] = ((charArray4[INDEX_SECOND] & 0xf) << OFFSET4) +
                ((charArray4[INDEX_THIRD] & 0x3c) >> OFFSET2);
            charArray3[INDEX_THIRD] = ((charArray4[INDEX_THIRD] & 0x3) << OFFSET6) + charArray4[INDEX_FORTH];
            for (i = 0; i < sizeof(charArray3); i++) {
                ret += charArray3[i];
            }
            i = 0;
        }
    }

    if (i) {
        uint32_t j = 0;
        for (j = i; j < sizeof(charArray4); j++) {
            charArray4[j] = 0;
        }
        for (j = 0; j < sizeof(charArray4); j++) {
            charArray4[j] = BASE_64_CHARS.find(charArray4[j]);
        }
        charArray3[INDEX_FIRST] = (charArray4[INDEX_FIRST] << OFFSET2) +
            ((charArray4[INDEX_SECOND] & 0x30) >> OFFSET4);
        charArray3[INDEX_SECOND] = ((charArray4[INDEX_SECOND] & 0xf) << OFFSET4) +
            ((charArray4[INDEX_THIRD] & 0x3c) >> OFFSET2);
        charArray3[INDEX_THIRD] = ((charArray4[INDEX_THIRD] & 0x3) << OFFSET6) + charArray4[INDEX_FORTH];
        for (j = 0; j < i - 1; j++) {
            ret += charArray3[j];
        }
    }
    return ret;
}

bool IsBase64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

bool IsDhBaseInfoInvalid(const DHBase& dhBase)
{
    return dhBase.deviceId_.empty() || (dhBase.deviceId_.length() > DEVID_MAX_LENGTH) ||
        dhBase.dhId_.empty() || (dhBase.dhId_.length() > DHID_MAX_LENGTH);
}
} // namespace DistributedHardware
} // namespace OHOS
