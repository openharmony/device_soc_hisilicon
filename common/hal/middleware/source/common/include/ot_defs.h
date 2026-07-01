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

#ifndef __DEFS_H__
#define __DEFS_H__

#include <sys/param.h>
#include <time.h>
#include <stdbool.h>
#include "ot_mw_type.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef __GNUC__
#define OT_ASM    asm
#define OT_INLINE inline
#endif

#define DO_NOTHING

#if OS_TYPE == OS_WIN32
#ifndef INLINE
#define INLINE inline
#endif
#elif OS_TYPE == OS_LINUX
#define INLINE inline
#endif

/* use for parameter INPUT, *DO NOT Modify the value **/
#define IN
/* use for parameter OUTPUT, the value maybe change when return from the function
 * the init value is ingore in the function. */
#define OUT
/* use for parameter INPUT and OUTPUT */
#define IO

/* -------------------------------- */
#ifndef EXTERN
#define EXTERN extern
#endif

#define STATIC static

#define LOCALVAR  static
#define GLOBALVAR extern

/** for declaring global variable */
#define DECLARE_GLOBALVAR

/** for using global variable */
#define USE_GLOBALVAR extern

#define LOCALFUNC  static
#define EXTERNFUNC extern

/** conculate the aligned start address,according to base address and align byte counts
 * eg. 4 byte aligned,0x80001232 ALIGN_START(0x80001232,4) = 0x80001230
 */
#define ALIGN_START(v, a) (((v)) & (~((a) - 1)))

/** conculate the aligned end address,according to base address and align byte counts
 * eg. 4 byte aligned,0x80001232 ALIGN_START(0x80001232,4) = 0x80001230
 */
#define ALIGN_END(v, a) (((v) & ~((a) - 1)) + ((a) - 1))

/** conculate the aligned next address,according to base address and align byte counts
 * eg. 4 byte aligned,0x80001232 ALIGN_START(0x80001232,4) = 0x80001230
 */
#define ALIGN_NEXT(v, a) ((((v) + ((a) - 1)) & (~((a) - 1))))

#define ALIGN_LENGTH(l, a) ALIGN_NEXT(l, a)

#define ALIGNTYPE_1BYTE 1
/* zsp */
#define ALIGNTYPE_2BYTE 2
/* x86... default */
#define ALIGNTYPE_4BYTE 4

#define ALIGNTYPE_8BYTE 8
/* 1 Page */
#define ALIGNTYPE_4K 4096

#define ALIGNTYPE_ZSP ALIGNTYPE_2BYTE

#define ALIGNTYPE_VIDEO ALIGNTYPE_8BYTE

#define PACK_ONE_BYTE __attribute__((__packed__))

#define MW_UNUSED(x) (void)(x)

#define SEC_TO_USEC 1000000
#define SEC_UNIT 1000

/** middleware module id */
typedef enum APPMOD_ID_E {
    OT_APPID_REC = 0x0B,         /** < recoder */
    OT_APPID_SNAP = 0x0C,        /** < snap */
    OT_APPID_FILEMGR = 0x16,     /** < file manager */
    OT_APPID_STORAGE = 0x18,     /** < storage */
    OT_APPID_LOG = 0x1F,         /** < log */
    OT_APPID_LOCALPLAYER = 0x20, /** < local player */
    OT_APPID_RTSPSERVER = 0x22,  /** <rtsp server */
    OT_APPID_HTTPSERVER = 0x24,  /** <http server */
    OT_APPID_MBUF = 0x25,        /** < mbuffer manager */
    OT_APPID_LIVESTREAM = 0x26,  /** < livestream */
    OT_APPID_RTSPCLIENT = 0x27,  /** < rtsp client */
    OT_APPID_DEMUXER = 0x28,     /** < demuxer */
    OT_APPID_MP4 = 0x29,         /** < demuxer */
    OT_APPID_DTCF = 0x2A,        /** < dtcf */
    OT_APPID_TIMER = 0x2C,       /** < ottimer */
    OT_APPID_XMP = 0x2B,         /** < xmp */
    OT_APPID_DCF = 0x2D,         /** <dcf */
    OT_APPID_DCF_WRAPPER = 0x2E, /** <dcf_wrapper */
    OT_APPID_TS = 0x2F,          /** < ts */
    OT_APPID_MSGHDL = 0x30,      /** < message handler */
    OT_APPID_HFSM = 0x31,        /** < Hierarchical state machine */
    OT_APPID_EVTHUB = 0x32,      /** < event hub */
    OT_APPID_TAG_EDITOR = 0x36,  /** < tag editro */
    OT_APPID_PICDEMUXER = 0x37,  /** < pic demuxer */
    OT_APPID_DNG = 0x38,         /** < dng module */
    OT_APPID_OSCSERVER = 0X3F,   /** < osc server */
    OT_APPID_NETATE = 0x40,      /** netate */
    OT_APPID_CONFACCESS = 0x41,  /** confaccess */
    OT_APPID_TSDEMUXER = 0x42,   /** ts demuxer */
    OT_APPID_BUTT = 0xFF
} APPMOD_ID_E;

#define ot_mw_usleep(usec)                       \
    do {                                         \
        struct timespec req;                     \
        req.tv_sec = (long)(usec) / SEC_TO_USEC;           \
        req.tv_nsec = (long)((usec) % SEC_TO_USEC) * SEC_UNIT; \
        nanosleep(&req, NULL);                   \
    } while (0)

static __inline TD_U64 GetTickCount(TD_VOID)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((TD_U64)ts.tv_sec * 1000L + (TD_U64)ts.tv_nsec / SEC_TO_USEC);
}

static __inline TD_VOID GetWaitTimespec(struct timespec *pOuttime, TD_U32 uTimeMs)
{
    if (pOuttime == NULL) {
        return;
    }

    clock_gettime(CLOCK_MONOTONIC, pOuttime);
    pOuttime->tv_sec += (long)uTimeMs / SEC_UNIT;
    pOuttime->tv_nsec += ((long)uTimeMs % SEC_UNIT) * (SEC_TO_USEC);
    if (pOuttime->tv_nsec >= (SEC_UNIT * SEC_UNIT * SEC_UNIT)) {
        pOuttime->tv_sec += 1;
        pOuttime->tv_nsec = pOuttime->tv_nsec % (SEC_UNIT * SEC_UNIT * SEC_UNIT);
    }
}

static inline bool CheckIntMultip(TD_S32 multiplier, TD_S32 val, TD_S32 *result)
{
    TD_S64 temp = (TD_S64)(multiplier * val);
    if (result != NULL) {
        *result = (TD_S32)temp;
    }
    return (temp < INT_MAX && temp > INT_MIN);
}

static inline bool CheckUnIntMultip(TD_U32 multiplier, TD_U32 val, TD_U32 *result)
{
    TD_S64 temp = (TD_S64)(multiplier * val);
    if (result != NULL) {
        *result = (TD_U32)temp;
    }
    return (temp < UINT_MAX);
}

/**
 * @brief Enable proc and log
 * @param[in] TD_BOOL bDebug: enable proc and log if TD_TRUE, otherwise disable the two module.
 *          Proc is debug by default. Log isn't debug(only show error log) by default.
 * @return TD_SUCCESS Enable/Disable debug success.
 * @return TD_FAILURE Enable/Disable debug fail.
 */
TD_S32 SS_MW_EnableDebug(TD_BOOL bDebug);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __DEFS_H__ */
