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
#ifndef SAMPLE_COMM_SVP_H
#define SAMPLE_COMM_SVP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "hi_common.h"
#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum hiSAMPLE_SVP_ERR_LEVEL_E {
    SAMPLE_SVP_ERR_LEVEL_DEBUG,   /* debug-level */
    SAMPLE_SVP_ERR_LEVEL_INFO,    /* informational */
    SAMPLE_SVP_ERR_LEVEL_NOTICE,  /* normal but significant condition */
    SAMPLE_SVP_ERR_LEVEL_WARNING, /* warning conditions */
    SAMPLE_SVP_ERR_LEVEL_ERROR,   /* error conditions */
    SAMPLE_SVP_ERR_LEVEL_CRIT,    /* critical conditions */
    SAMPLE_SVP_ERR_LEVEL_ALERT,   /* action must be taken immediately */
    SAMPLE_SVP_ERR_LEVEL_FATAL,   /* just for compatibility with previous version */
    SAMPLE_SVP_ERR_LEVEL_BUTT
} SAMPLE_SVP_ERR_LEVEL_E;

#define SAMPLE_SVP_PRINTF(LevelStr, Msg, ...)                                                           \
    do {                                                                                                \
        fprintf(stderr, "[Level]:%s,[Func]:%s [Line]:%d [Info]:" Msg, LevelStr, __FUNCTION__, __LINE__, \
            ##__VA_ARGS__);                                                                             \
    } while (0)
#define SAMPLE_SVP_PRINTF_RED(LevelStr, Msg, ...)                                                         \
    do {                                                                                                  \
        fprintf(stderr, "\033[0;31m [Level]:%s,[Func]:%s [Line]:%d [Info]:" Msg "\033[0;39m\n", LevelStr, \
            __FUNCTION__, __LINE__, ##__VA_ARGS__);                                                       \
    } while (0)

#define SAMPLE_SVP_TRACE_FATAL(Msg, ...)    SAMPLE_SVP_PRINTF_RED("Fatal", Msg, ##__VA_ARGS__)
#define SAMPLE_SVP_TRACE_ALERT(Msg, ...)    SAMPLE_SVP_PRINTF_RED("Alert", Msg, ##__VA_ARGS__)
#define SAMPLE_SVP_TRACE_CRIT(Msg, ...)     SAMPLE_SVP_PRINTF_RED("Critical", Msg, ##__VA_ARGS__)
#define SAMPLE_SVP_TRACE_ERR(Msg, ...)      SAMPLE_SVP_PRINTF_RED("Error", Msg, ##__VA_ARGS__)
#define SAMPLE_SVP_TRACE_WARN(Msg, ...)     SAMPLE_SVP_PRINTF("Warning", Msg, ##__VA_ARGS__)
#define SAMPLE_SVP_TRACE_NOTICE(Msg, ...)   SAMPLE_SVP_PRINTF("Notice", Msg, ##__VA_ARGS__)
#define SAMPLE_SVP_TRACE_INFO(Msg, ...)     SAMPLE_SVP_PRINTF("Info", Msg, ##__VA_ARGS__)
#define SAMPLE_SVP_TRACE_DEBUG(Msg, ...)    SAMPLE_SVP_PRINTF("Debug", Msg, ##__VA_ARGS__)

#define SAMPLE_SVP_TRACE(Level, Msg, ...)                    \
    do {                                                     \
        switch (Level) {                                     \
            case SAMPLE_SVP_ERR_LEVEL_DEBUG:                 \
                SAMPLE_SVP_TRACE_DEBUG(Msg, ##__VA_ARGS__);  \
                break;                                       \
            case SAMPLE_SVP_ERR_LEVEL_INFO:                  \
                SAMPLE_SVP_TRACE_INFO(Msg, ##__VA_ARGS__);   \
                break;                                       \
            case SAMPLE_SVP_ERR_LEVEL_NOTICE:                \
                SAMPLE_SVP_TRACE_NOTICE(Msg, ##__VA_ARGS__); \
                break;                                       \
            case SAMPLE_SVP_ERR_LEVEL_WARNING:               \
                SAMPLE_SVP_TRACE_WARN(Msg, ##__VA_ARGS__);   \
                break;                                       \
            case SAMPLE_SVP_ERR_LEVEL_ERROR:                 \
                SAMPLE_SVP_TRACE_ERR(Msg, ##__VA_ARGS__);    \
                break;                                       \
            case SAMPLE_SVP_ERR_LEVEL_CRIT:                  \
                SAMPLE_SVP_TRACE_CRIT(Msg, ##__VA_ARGS__);   \
                break;                                       \
            case SAMPLE_SVP_ERR_LEVEL_ALERT:                 \
                SAMPLE_SVP_TRACE_ALERT(Msg, ##__VA_ARGS__);  \
                break;                                       \
            case SAMPLE_SVP_ERR_LEVEL_FATAL:                 \
                SAMPLE_SVP_TRACE_FATAL(Msg, ##__VA_ARGS__);  \
                break;                                       \
            default:                                         \
                break;                                       \
        }                                                    \
    } while (0)

#define SAMPLE_SVP_CHECK_EXPR_GOTO(Expr, Label, Level, Msg, ...) \
    do {                                                         \
        if (Expr) {                                              \
            SAMPLE_SVP_TRACE(Level, Msg, ##__VA_ARGS__);         \
            goto Label;                                          \
        }                                                        \
    } while (0)

#define SAMPLE_SVP_CHECK_EXPR_RET_VOID(Expr, Level, Msg, ...) \
    do {                                                      \
        if (Expr) {                                           \
            SAMPLE_SVP_TRACE(Level, Msg, ##__VA_ARGS__);      \
            return;                                           \
        }                                                     \
    } while (0)

#define SAMPLE_SVP_CHECK_EXPR_RET(Expr, Ret, Level, Msg, ...) \
    do {                                                      \
        if (Expr) {                                           \
            SAMPLE_SVP_TRACE(Level, Msg, ##__VA_ARGS__);      \
            return Ret;                                       \
        }                                                     \
    } while (0)

#define SAMPLE_SVP_CHECK_EXPR_TRACE(Expr, Level, Msg, ...) \
    do {                                                   \
        if (Expr) {                                        \
            SAMPLE_SVP_TRACE(Level, Msg, ##__VA_ARGS__);   \
        }                                                  \
    } while (0)

#define SAMPLE_SVP_ALIGN_16         16
#define SAMPLE_SVP_ALIGN_32         32
#define SAMPLE_SVP_D1_PAL_HEIGHT    576
#define SAMPLE_SVP_D1_PAL_WIDTH     704

#define SAMPLE_SVP_MMZ_FREE(phy, vir)                                  \
    do {                                                               \
        if (((phy) != 0) && ((vir) != 0)) {                            \
            HI_MPI_SYS_MmzFree((phy), (HI_VOID *)(HI_UINTPTR_T)(vir)); \
            (phy) = 0;                                                 \
            (vir) = 0;                                                 \
        }                                                              \
    } while (0)


#define SAMPLE_SVP_CLOSE_FILE(fp) \
    do {                          \
        if ((fp) != NULL) {       \
            (void)fclose((fp));         \
            (fp) = NULL;          \
        }                         \
    } while (0)

HI_S32 SAMPLE_COMM_SVP_CheckSysInit(void);

HI_VOID SAMPLE_COMM_SVP_CheckSysExit(void);

HI_U32 SAMPLE_COMM_SVP_Align(HI_U32 u32Size, HI_U16 u16Align);

HI_S32 SAMPLE_COMM_SVP_CreateImage(SVP_IMAGE_S *pstImg, SVP_IMAGE_TYPE_E enType, HI_U32 u32Width, HI_U32 u32Height,
    HI_U32 u32AddrOffset);

HI_VOID SAMPLE_COMM_SVP_DestroyImage(SVP_IMAGE_S *pstImg, HI_U32 u32AddrOffset);

HI_S32 SAMPLE_COMM_SVP_CreateMemInfo(SVP_MEM_INFO_S *pstMemInfo, HI_U32 u32Size, HI_U32 u32AddrOffset);

HI_VOID SAMPLE_COMM_SVP_DestroyMemInfo(SVP_MEM_INFO_S *pstMemInfo, HI_U32 u32AddrOffset);

HI_S32 SAMPLE_COMM_SVP_MallocMem(HI_CHAR *pszMmb, HI_CHAR *pszZone, HI_U64 *pu64PhyAddr, HI_VOID **ppvVirAddr,
    HI_U32 u32Size);

HI_S32 SAMPLE_COMM_SVP_MallocCached(HI_CHAR *pszMmb, HI_CHAR *pszZone, HI_U64 *pu64PhyAddr, HI_VOID **ppvVirAddr,
    HI_U32 u32Size);

HI_S32 SAMPLE_COMM_SVP_FlushCache(HI_U64 u64PhyAddr, HI_VOID *pvVirAddr, HI_U32 u32Size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* SAMPLE_COMM_SVP_H */
