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
#ifndef __OT_LOG_H__
#define __OT_LOG_H__
#include <stdio.h>
#include "ot_mw_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*
 * @brief  log level type.
 */
typedef enum OT_LOG_LEVEL_E {
    OT_LOG_LEVEL_VERBOSE = -1,  // assigned to verbose -1 because need to be compatible with previous version
    OT_LOG_LEVEL_DEBUG = 0,
    OT_LOG_LEVEL_INFO,
    OT_LOG_LEVEL_WARN,
    OT_LOG_LEVEL_ERR,
    OT_LOG_LEVEL_FATAL,
    OT_LOG_LEVEL_BUTT
} OT_LOG_LEVEL_E;

typedef TD_VOID (*SS_OUTPUT_FUNC)(const TD_CHAR *fmt, ...);

TD_S32 SS_LOG_OpenSaveFile(const TD_CHAR *filePath, size_t filePathLen, TD_U32 fileSize);

TD_S32 SS_LOG_CloseSaveFile(TD_VOID);

/**
*   @brief  set enabled log level, logs with equal or higher level than enabled will be output
*   @param[in] enLevel : OT_LOG_LEVEL_E: enabled level
*   @retval  0 success,others failed
*/
TD_S32 SS_LOG_SetEnabledLevel(OT_LOG_LEVEL_E enLevel);

OT_LOG_LEVEL_E SS_LOG_GetLevel(TD_VOID);

#ifdef ENABLE_LOG
TD_S32 SS_LOG_PrintfInfo(const TD_CHAR *pszModName, OT_LOG_LEVEL_E enLevel, const char* func,
    TD_S32 line, const TD_CHAR *pszFmt, ...);

/**
*   @brief  output log
*   @param[in] pszModName : TD_CHAR*: module name
*   @param[in] enLevel : OT_LOG_LEVEL_E: log level
*   @param[in] pszFmt : TD_CHAR*: log content, accept multi-parameters
*   @retval  0 success,others failed
*/
TD_S32 SS_LOG_Printf(const TD_CHAR *pszModName, OT_LOG_LEVEL_E enLevel, const TD_CHAR *pszFmt, ...)
    __attribute__((format(printf, 3, 4)));

#define OT_LOG_FATAL(mode, ...)  \
    do { \
        ((void)SS_LOG_PrintfInfo((const TD_CHAR *)mode, OT_LOG_LEVEL_FATAL, __FUNCTION__, __LINE__, __VA_ARGS__)); \
    } while (0)

#define OT_LOG_ERR(mode, ...)  \
    do { \
        ((void)SS_LOG_PrintfInfo((const TD_CHAR *)mode, OT_LOG_LEVEL_ERR, __FUNCTION__, __LINE__, __VA_ARGS__)); \
    } while (0)

#define OT_LOG_WARN(mode, ...)  \
    do { \
        ((void)SS_LOG_PrintfInfo((const TD_CHAR *)mode, OT_LOG_LEVEL_WARN, __FUNCTION__, __LINE__, __VA_ARGS__)); \
    } while (0)

#define OT_LOG_INFO(mode, ...)  \
    do { \
            ((void)SS_LOG_PrintfInfo((const TD_CHAR *)mode, OT_LOG_LEVEL_INFO, __FUNCTION__, __LINE__, __VA_ARGS__)); \
    } while (0)

#define OT_LOG_DEBUG(mode, ...)  \
    do { \
        ((void)SS_LOG_PrintfInfo((const TD_CHAR *)mode, OT_LOG_LEVEL_DEBUG, __FUNCTION__, __LINE__, __VA_ARGS__)); \
    } while (0)

#define OT_LOG_RETURN_IF_FAIL(mode, ret, errCode)                  \
    do {                                                           \
        if ((ret) != TD_SUCCESS) {                                 \
            OT_LOG_ERR((mode), "Error Code: [0x%08X]\n\n", (ret)); \
            return (errCode);                                      \
        }                                                          \
    } while (0)

#define OT_LOG_RETURN_IF_EXPR_FALSE(mode, expr, errCode)   \
    do {                                                   \
        if (!(expr)) {                                     \
            OT_LOG_ERR((mode), "expr[%s] false\n", #expr); \
            return (errCode);                              \
        }                                                  \
    } while (0)

#define OT_LOG_LOG_AND_RETURN_IF_FAIL(mode, ret, errCode, errString)         \
    do {                                                                     \
        if ((ret) != TD_SUCCESS) {                                           \
            OT_LOG_ERR((mode), "[%s] failed[0x%08X]\n", (errString), (ret)); \
            return (errCode);                                                \
        }                                                                    \
    } while (0)

#define OT_LOG_LOG_AND_RETURN_IF_EXPR_FALSE(mode, expr, errCode, errString) \
    do {                                                                    \
        if (!(expr)) {                                                      \
            OT_LOG_ERR((mode), "[%s] failed\n", (errString));               \
            return (errCode);                                               \
        }                                                                   \
    } while (0)

#define OT_LOG_RETURN_IF_PTR_NULL(mode, p, errCode)          \
    do {                                                     \
        if ((p) == TD_NULL) {                                \
            OT_LOG_ERR((mode), "pointer[%s] is null\n", #p); \
            return (errCode);                                \
        }                                                    \
    } while (0)

#define OT_LOG_LOG_IF_EXPR_FALSE(mode, expr, errString)       \
    do {                                                      \
        if (!(expr)) {                                        \
            OT_LOG_ERR((mode), "[%s] failed\n", (errString)); \
        }                                                     \
    } while (0)

#define OT_LOG_LOG_AND_GOTO_IF_EXPR_FALSE(mode, expr, label, errString) \
    do {                                                                \
        if (!(expr)) {                                                  \
            OT_LOG_ERR((mode), "[%s] failed\n", (errString));           \
            goto label;                                                 \
        }                                                               \
    } while (0)

#define OT_LOG_LOG_IF_FAIL(mode, ret, errCode, errString)                    \
    do {                                                                     \
        if ((ret) != TD_SUCCESS) {                                           \
            OT_LOG_ERR((mode), "[%s] failed[0x%08X]\n", (errString), (ret)); \
        }                                                                    \
    } while (0)


#else
#define SS_LOG_PrintfInfo(pszModName, enLevel, fnuc, line, pszFmt, ...)
#define SS_LOG_Printf(pszModName, enLevel, pszFmt, ...)
#define OT_LOG_FATAL(mode, ...)
#define OT_LOG_ERR(mode, ...)
#define OT_LOG_WARN(mode, ...)
#define OT_LOG_INFO(mode, ...)
#define OT_LOG_DEBUG(mode, ...)
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __OT_LOG_H__ */
