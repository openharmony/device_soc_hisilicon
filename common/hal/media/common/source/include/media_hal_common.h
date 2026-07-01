/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 * Description: media hal common
 */

#ifndef MEDIA_HAL_COMMON_H
#define MEDIA_HAL_COMMON_H

#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include "securec.h"
#include "audio_aac_adp.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef void *HalHandle;
typedef void *VoidPtr;

#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a)         (((x) / (a)) * (a))
#endif

#ifndef ALIGN_UP
#define ALIGN_UP(x, a)           ((((x) + ((a) - 1)) / (a)) * (a))
#endif

#define MEDIA_HAL_OK 0
#define MEDIA_HAL_ERR (-1)
#define MEDIA_HAL_NO_MEM (-2)
#define MEDIA_HAL_INIT_FAIL (-3)
#define MEDIA_HAL_INVALID_PARAM (-4)
#define MEDIA_HAL_PERMISSION_DENIED (-5)
#define MEDIA_HAL_TIMEOUT (-6)
#define MEDIA_HAL_ERR_AGAIN (-7)

#define MEDIA_HAL_INVALID_CHN_ID (-1)

#define MEDIA_HAL_LOG_ENABLE  1
#define MEDIA_HAL_UNUSED(x) (void)(x)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

/*
 * @brief  log level type.
 */
typedef enum {
    MEDIA_HAL_LOG_LEVEL_VERBOSE = 0,
    MEDIA_HAL_LOG_LEVEL_DEBUG,
    MEDIA_HAL_LOG_LEVEL_INFO,
    MEDIA_HAL_LOG_LEVEL_WARN,
    MEDIA_HAL_LOG_LEVEL_ERR
} MEDIA_HAL_LOG_LEVEL;

int32_t MediaHalLogPrintf(const char *modName, MEDIA_HAL_LOG_LEVEL level, const char *fmt, ...);

#define DECORATOR_LOG(op, modName, level, fmt, args...)                         \
    do {                                                                        \
        op(modName, level, "{%s():%d} " fmt, __FUNCTION__, __LINE__, ##args);   \
    } while (0)

#ifdef MEDIA_HAL_LOG_ENABLE
#undef MEDIA_HAL_LOGV
#define MEDIA_HAL_LOGV(modName, fmt, ...) DECORATOR_LOG(MediaHalLogPrintf, modName, MEDIA_HAL_LOG_LEVEL_VERBOSE, \
                                                        fmt, ##__VA_ARGS__)

#undef MEDIA_HAL_LOGD
#define MEDIA_HAL_LOGD(modName, fmt, ...) DECORATOR_LOG(MediaHalLogPrintf, modName, MEDIA_HAL_LOG_LEVEL_DEBUG,  \
                                                        fmt, ##__VA_ARGS__)

#undef MEDIA_HAL_LOGI
#define MEDIA_HAL_LOGI(modName, fmt, ...) DECORATOR_LOG(MediaHalLogPrintf, modName, MEDIA_HAL_LOG_LEVEL_INFO,   \
                                                        fmt, ##__VA_ARGS__)

#undef MEDIA_HAL_LOGW
#define MEDIA_HAL_LOGW(modName, fmt, ...) DECORATOR_LOG(MediaHalLogPrintf, modName, MEDIA_HAL_LOG_LEVEL_WARN,   \
                                                        fmt, ##__VA_ARGS__)

#undef MEDIA_HAL_LOGE
#define MEDIA_HAL_LOGE(modName, fmt, ...) DECORATOR_LOG(MediaHalLogPrintf, modName, MEDIA_HAL_LOG_LEVEL_ERR, \
                                                        fmt, ##__VA_ARGS__)
#else
#undef MEDIA_HAL_LOGV
#define MEDIA_HAL_LOGV(...)

#undef MEDIA_HAL_LOGD
#define MEDIA_HAL_LOGD(...)

#undef MEDIA_HAL_LOGI
#define MEDIA_HAL_LOGI(...)

#undef MEDIA_HAL_LOGW
#define MEDIA_HAL_LOGW(...)

#undef MEDIA_HAL_LOGE
#define MEDIA_HAL_LOGE(...)
#endif // MEDIA_HAL_LOG_ENABLE

#define CHK_NULL_RETURN(ptr, ret, printfString) \
do { \
    if ((ptr) == NULL) { \
        MEDIA_HAL_LOGE(MODULE_NAME, "%s :ret:%d", ((printfString) == NULL) ? "" : (printfString), ret); \
        return (ret); \
    } \
} while (0)

#define CHK_NULL_RETURN_NONE(ptr, printfString) \
do { \
    if ((ptr) == NULL) { \
        MEDIA_HAL_LOGE(MODULE_NAME, "%s ", ((printfString) == NULL) ? "" : (printfString)); \
        return; \
    } \
} while (0)

#define CHK_NULL_RETURN_NO_LOG(ptr, ret) \
    do { \
        if ((ptr) == NULL) { \
            return (ret); \
        } \
    } while (0)

#define CHK_FAILED_RETURN(value, target, ret, print) \
do { \
    if ((value) != (target)) { \
        MEDIA_HAL_LOGE(MODULE_NAME, "%s, %#x != %d ", ((print) == NULL) ? "" : (print), (value), (target)); \
        return (ret); \
    } \
} while (0)

#define CHK_FAILED_NO_RETURN(value, target, print) \
do { \
    if ((value) != (target)) { \
        MEDIA_HAL_LOGE(MODULE_NAME, "%s, %#x != %d ", ((print) == NULL) ? "" : (print), (value), (target)); \
    } \
} while (0)

#define CHK_FAILED_RETURN_NO_LOG(value, target, ret) \
do { \
    if ((value) != (target)) { \
        return (ret); \
    } \
} while (0)

#define CHK_COND_RETURN(condition, ret, print) \
do { \
    if (condition) { \
        MEDIA_HAL_LOGE(MODULE_NAME, "%s ", ((print) == NULL) ? "" : (print)); \
        return ret; \
    } \
} while (0)


#define MEDIA_HAL_LOCK(mutex)               \
    do {                                    \
        (void)pthread_mutex_lock(&(mutex)); \
    } while (0)

#define MEDIA_HAL_UNLOCK(mutex)               \
    do {                                      \
        (void)pthread_mutex_unlock(&(mutex)); \
    } while (0)

void *MediaHalDLOpen(const char *file);

void *MediaHalDLSym(VoidPtr module, const char *symbol);

void MediaHalDLClose(VoidPtr handle);

int64_t CalcDiffTimeMs(struct timeval begin, struct timeval end);

int64_t CalcDiffTimeUs(struct timeval begin, struct timeval end);

int32_t MediaSystemInit(void);

int32_t MediaSystemDeinit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // MEDIA_HAL_COMMON_H
