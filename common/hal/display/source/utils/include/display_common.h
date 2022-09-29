/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef DISP_COMMON_H
#define DISP_COMMON_H
#include <string.h>
#include <stdint.h>
#include "hilog/log.h"
#include "stdio.h"
#ifdef HDF_LOG_TAG
#undef HDF_LOG_TAG
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#undef LOG_TAG
#undef LOG_DOMAIN
#define LOG_TAG "DISP"
#define LOG_DOMAIN 0xD001400

#define __DISP_FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

#ifndef DISPLAY_DEBUG_ENABLE
#define DISPLAY_DEBUG_ENABLE 0
#endif

#ifndef DISPLAY_LOGD
#define DISPLAY_LOGD(format, ...)                                                   \
    do {                                                                            \
        if (DISPLAY_DEBUG_ENABLE) {                                                 \
            HILOG_INFO(LOG_CORE, "[%{public}s@%{public}s:%{public}d] " format "\n", \
                __FUNCTION__, __DISP_FILENAME__, __LINE__,                          \
                ##__VA_ARGS__);                                                     \
        }                                                                           \
    } while (0)
#endif

#ifndef DISPLAY_LOGI
#define DISPLAY_LOGI(format, ...)                                               \
    do {                                                                        \
        HILOG_INFO(LOG_CORE, "[%{public}s@%{public}s:%{public}d] " format "\n", \
            __FUNCTION__, __DISP_FILENAME__, __LINE__, ##__VA_ARGS__);          \
    } while (0)
#endif

#ifndef DISPLAY_LOGW
#define DISPLAY_LOGW(format, ...)                                               \
    do {                                                                        \
        HILOG_WARN(LOG_CORE, "[%{public}s@%{public}s:%{public}d] " format "\n", \
            __FUNCTION__, __DISP_FILENAME__, __LINE__,                          \
            ##__VA_ARGS__);                                                     \
    } while (0)
#endif

#ifndef DISPLAY_LOGE
#define DISPLAY_LOGE(format, ...)                                      \
    do {                                                               \
        HILOG_ERROR(LOG_CORE,                                          \
            "\033[0;32;31m"                                            \
            "[%{public}s@%{public}s:%{public}d] " format "\033[m"      \
            "\n",                                                      \
            __FUNCTION__, __DISP_FILENAME__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#endif

#ifndef CHECK_NULLPOINTER_RETURN_VALUE
#define CHECK_NULLPOINTER_RETURN_VALUE(pointer, ret)          \
    do {                                                      \
        if ((pointer) == NULL) {                              \
            DISPLAY_LOGE("pointer is null and return ret\n"); \
            return (ret);                                     \
        }                                                     \
    } while (0)
#endif

#ifndef CHECK_NULLPOINTER_RETURN
#define CHECK_NULLPOINTER_RETURN(pointer)                 \
    do {                                                  \
        if ((pointer) == NULL) {                          \
            DISPLAY_LOGE("pointer is null and return\n"); \
            return;                                       \
        }                                                 \
    } while (0)
#endif

#ifndef DISPLAY_CHK_RETURN
#define DISPLAY_CHK_RETURN(val, ret, ...) \
    do {                                  \
        if (val) {                        \
            __VA_ARGS__;                  \
            return (ret);                 \
        }                                 \
    } while (0)
#endif

#ifndef DISPLAY_CHK_RETURN_NOT_VALUE
#define DISPLAY_CHK_RETURN_NOT_VALUE(val, ...) \
    do {                                       \
        if (val) {                             \
            __VA_ARGS__;                       \
            return;                            \
        }                                      \
    } while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* DISP_COMMON_H */
