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

#include "dh_log.h"

#include "securec.h"

#include "constants.h"

#ifdef HI_LOG_ENABLE
#include "hilog/log.h"
#else
#include <cstdio>
#endif

namespace OHOS {
namespace DistributedHardware {
static void DHLogOut(DHLogLevel logLevel, const char *logBuf)
{
#ifdef HI_LOG_ENABLE
    LogLevel hiLogLevel = LOG_INFO;
    switch (logLevel) {
        case DH_LOG_DEBUG:
            hiLogLevel = LOG_DEBUG;
            break;
        case DH_LOG_INFO:
            hiLogLevel = LOG_INFO;
            break;
        case DH_LOG_WARN:
            hiLogLevel = LOG_WARN;
            break;
        case DH_LOG_ERROR:
            hiLogLevel = LOG_ERROR;
            break;
        default:
            break;
    }
    (void)HiLogPrint(LOG_CORE, hiLogLevel, LOG_DOMAIN, DC_LOG_TITLE_TAG.c_str(), "%{public}s", logBuf);
#else
    switch (logLevel) {
        case DH_LOG_DEBUG:
            printf("[D]%s\n", logBuf);
            break;
        case DH_LOG_INFO:
            printf("[I]%s\n", logBuf);
            break;
        case DH_LOG_WARN:
            printf("[W]%s\n", logBuf);
            break;
        case DH_LOG_ERROR:
            printf("[E]%s\n", logBuf);
            break;
        default:
            break;
    }
#endif
}

void DH_HAL_Log(DHLogLevel logLevel, const char *fmt, ...)
{
    if (logLevel < DH_LOG_INFO) {
        return;
    }
    char logBuf[LOG_MAX_LEN] = {0};
    va_list arg;

    (void)memset_s(&arg, sizeof(va_list), 0, sizeof(va_list));
    va_start(arg, fmt);
    int32_t ret = vsprintf_s(logBuf, sizeof(logBuf), fmt, arg);
    va_end(arg);
    if (ret < 0) {
        DHLogOut(logLevel, "DH log length error.");
        return;
    }
    DHLogOut(logLevel, logBuf);
}
} // namespace DistributedHardware
} // namespace OHOS
