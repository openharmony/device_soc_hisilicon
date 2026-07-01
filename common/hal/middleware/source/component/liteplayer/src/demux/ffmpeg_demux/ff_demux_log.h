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

#ifndef FFDEMUX_LOG_H
#define FFDEMUX_LOG_H

#include "ot_mw_type.h"

#define MODULE_NAME_DEMUX "ffmpegDemux"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*
 * @brief  log level type.
 */
typedef enum {
    FFDEMUX_LOG_LEVEL_DEBUG = 0,
    FFDEMUX_LOG_LEVEL_INFO,
    FFDEMUX_LOG_LEVEL_WARN,
    FFDEMUX_LOG_LEVEL_ERR,
    FFDEMUX_LOG_LEVEL_FATAL,
    FFDEMUX_LOG_LEVEL_BUTT
}FfDemuxLogLevel;

typedef TD_VOID (*FfDemuxOutputFunc)(const TD_CHAR *fmt, ...);

/**
*   @brief  set enabled log level, logs with equal or higher level than enabled will be output
*   @param[in] enLevel : FfDemuxLogLevel: enabled level
*   @retval  0 success,others failed
 */
TD_S32 FfDemuxSetEnabledLevel(FfDemuxLogLevel level);

/**
*   @brief  use this API to set log output function instread of 'printf'
*   @param[in] pFunc : FfDemuxOutputFunc: output function implements by user
*   @retval  0 success,others failed
 */
TD_S32 FfDemuxSetOutputFunc(FfDemuxOutputFunc func);

/**
*   @brief  output log
*   @param[in] modName : TD_CHAR*: module name
*   @param[in] enLevel : FfDemuxLogLevel: log level
*   @param[in] fmt : TD_CHAR*: log content, accept multi-parameters
*   @retval  0 success,others failed
 */
#ifdef ENABLE_FF_LOG
TD_VOID FFDEMUX_PRINTF(const TD_CHAR modName[], FfDemuxLogLevel level, const TD_CHAR fmt[],
    ...) __attribute__((format(printf, 3, 4)));
#else
#define FFDEMUX_PRINTF(fmt, ...)
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* FFDEMUX_LOG_H */
