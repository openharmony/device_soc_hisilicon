/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef HI_DEBUG_DEFINE_H
#define HI_DEBUG_DEFINE_H

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*
 * @brief  log level type.
 */
typedef enum hiLOG_LEVEL_E {
    HI_LOG_LEVEL_VERBOSE = -1,  // assigned to verbose -1 because need to be compatible with previous version
    HI_LOG_LEVEL_DEBUG = 0,
    HI_LOG_LEVEL_INFO,
    HI_LOG_LEVEL_WARN,
    HI_LOG_LEVEL_ERR,
    HI_LOG_LEVEL_FATAL,
    HI_LOG_LEVEL_BUTT
} HI_LOG_LEVEL_E;

typedef HI_VOID (*HI_OUTPUT_FUNC)(const HI_CHAR *fmt, ...);

/**
*   @brief  set enabled log level, logs with equal or higher level than enabled will be output
*   @param[in] enLevel : HI_LOG_LEVEL_E: enabled level
*   @retval  0 success,others failed
*/
HI_S32 HI_LOG_SetEnabledLevel(HI_LOG_LEVEL_E enLevel);

/**
*   @brief  use this API to set log output function instead of 'printf'
*   @param[in] pFunc : HI_OUTPUT_FUNC: output function implements by user
*   @retval  0 success,others failed
*/
HI_S32 HI_LOG_SetOutputFunc(HI_OUTPUT_FUNC pFunc);

/**
*   @brief  output log
*   @param[in] pszModName : HI_CHAR*: module name
*   @param[in] enLevel : HI_LOG_LEVEL_E: log level
*   @param[in] pszFmt : HI_CHAR*: log content, accept multi-parameters
*   @retval  0 success,others failed
*/
#ifdef ENABLE_LOG
HI_S32 HI_LOG_Printf(const HI_CHAR *pszModName, HI_LOG_LEVEL_E enLevel, const HI_CHAR *pszFmt, ...)
    __attribute__((format(printf, 3, 4)));
#else
#define HI_LOG_Printf(pszModName, enLevel, pszFmt, ...)
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* HI_DEBUG_DEFINE_H */
