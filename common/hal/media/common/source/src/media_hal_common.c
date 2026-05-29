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
 */

#include "media_hal_common.h"

#include <sys/time.h>
#ifndef __HuaweiLite__
#include <dlfcn.h>
#endif
#include <limits.h>
#ifdef __HuaweiLite__
#include <los_ld_elflib.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
#define MODULE_NAME "MediaHalCommon"
#define SS2US  1000000
#define US2MS  1000
#define US2MS_HALF  500
#define S2MS  1000

static MEDIA_HAL_LOG_LEVEL g_enabledLevel = MEDIA_HAL_LOG_LEVEL_INFO;
static bool g_systemInited = false;
static pthread_mutex_t g_systemInitMutex = PTHREAD_MUTEX_INITIALIZER;
static void LogTime(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm tm;
    localtime_r(&tv.tv_sec, &tm);
    const int64_t USEC_TO_MSEC = 1000;
    printf("[%02d:%02d:%02d:%03ld]", tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec / USEC_TO_MSEC);
}

int32_t MediaHalLogPrintf(const char *modName, MEDIA_HAL_LOG_LEVEL level, const char *fmt, ...)
{
    if (level < g_enabledLevel) {
        return MEDIA_HAL_OK;
    }
    if (modName == NULL || fmt == NULL) {
        return MEDIA_HAL_ERR;
    }
    char *levelStr = NULL;
    switch (level) {
        case MEDIA_HAL_LOG_LEVEL_VERBOSE:
            levelStr = "V";
            break;
        case MEDIA_HAL_LOG_LEVEL_DEBUG:
            levelStr = "D";
            break;
        case MEDIA_HAL_LOG_LEVEL_INFO:
            levelStr = "I";
            break;
        case MEDIA_HAL_LOG_LEVEL_WARN:
            levelStr = "W";
            break;
        case MEDIA_HAL_LOG_LEVEL_ERR:
            levelStr = "E";
            break;
        default:
            levelStr = "E";
            break;
    }
    LogTime();
    va_list args;
    printf("[%s:%s]", modName, levelStr);
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    return MEDIA_HAL_OK;
}

void *MediaHalDLOpen(const char *file)
{
    void *soModule = NULL;
    if (file == NULL) {
        return NULL;
    }
    char resolvedPath[PATH_MAX] = { 0 };
    if (strlen(file) >= PATH_MAX || realpath(file, resolvedPath) == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "realpath file error");
        return NULL;
    }
#ifndef __HuaweiLite__
    soModule = dlopen(resolvedPath, RTLD_LAZY);
#else
    soModule = LOS_SoLoad(file);
#endif
    if (soModule == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "MediaHalDLOpen file:%s error:%s", file, dlerror());
        return soModule;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "MediaHalDLOpen,file:%s ok", file);
    return soModule;
}

void *MediaHalDLSym(VoidPtr module, const char *symbol)
{
    void *moduleAddr = NULL;
    if (module == NULL || symbol == NULL) {
        return NULL;
    }
#ifndef __HuaweiLite__
    moduleAddr = dlsym(module, symbol);
#else
    moduleAddr = LOS_FindSymByName(module, symbol);
#endif
    return moduleAddr;
}

void MediaHalDLClose(HalHandle handle)
{
    if (handle == NULL) {
        return;
    }
#ifndef __HuaweiLite__
    dlclose(handle);
#else
    LOS_ModuleUnload(handle);
#endif
}

int64_t CalcDiffTimeMs(struct timeval begin, struct timeval end)
{
    int64_t diffSec = (int64_t)(end.tv_sec - begin.tv_sec);
    int64_t diffMsec = ((int64_t)(end.tv_usec - begin.tv_usec) + US2MS_HALF) / US2MS;
    const int64_t diffSecMax = INT64_MAX / S2MS;
    diffSec = (diffSec > diffSecMax) ? diffSecMax : diffSec;
    return diffSec * S2MS + diffMsec;
}

int64_t CalcDiffTimeUs(struct timeval begin, struct timeval end)
{
    int64_t diffSec = (int64_t)(end.tv_sec - begin.tv_sec);
    int64_t diffUsec = (int64_t)(end.tv_usec - begin.tv_usec);
    const int64_t diffSecMax = INT64_MAX / SS2US;
    diffSec = (diffSec > diffSecMax) ? diffSecMax : diffSec;
    return diffSec * SS2US + diffUsec;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
