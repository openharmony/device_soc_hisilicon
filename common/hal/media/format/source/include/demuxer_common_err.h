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
 *
 * Description: hidemuxer module error header file
 */

#ifndef DEMUXER_COMMON_ERR_H
#define DEMUXER_COMMON_ERR_H

#if defined(ENABLE_H9) || defined(ENABLE_H8)
#include "ot_error_def.h"
#include "ot_defs.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* 0x40~0xFF */
typedef enum {
    APP_DEMUXER_ERR_OPEN_FILE = 0x40,      /* open media file failed */
    APP_DEMUXER_ERR_READ_PACKET = 0x41,    /* read one packet from media file failed */
    APP_DEMUXER_ERR_SEEK = 0x42,           /* seek in media file failed */
    APP_DEMUXER_ERR_MEM_MALLOC = 0x43,     /* memery alloc failed */
    APP_DEMUXER_ERR_PROBE = 0x44,          /* probe the media info failed */
    APP_DEMUXER_ERR_ACTION = 0x45,         /* should not exec this action here */
    APP_DEMUXER_ERR_INVALID_HANDLE = 0x46, /* handle is invalid */
    APP_DEMUXER_ERR_FREE_PACKET = 0x47,    /* free the packet failed */
    APP_DEMUXER_ERR_SET_ATTR = 0x48,       /* set demuxer attr failed */
    APP_DEMUXER_ERR_NOT_SUPPORT = 0x49,    /* current action not support */
    APP_DEMUXER_ERR_CLOSE_FILE = 0x50,     /* close media file failed */
    APP_DEMUXER_ERR_SKIP = 0x51, /* close media file failed */
    APP_DEMUXER_BUTT = 0xFF
} AppDemuxerErrCode;

/* general error code */
#define OT_ERR_DEMUXER_NULL_PTR       OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_ERR_NULL_PTR)
#define OT_ERR_DEMUXER_ILLEGAL_PARAM  OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_ERR_ILLEGAL_PARAM)
#define OT_ERR_DEMUXER_OPEN_FILE      OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_OPEN_FILE)
#define OT_ERR_DEMUXER_READ_PACKET    OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_READ_PACKET)
#define OT_ERR_DEMUXER_SEEK           OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_SEEK)
#define OT_ERR_DEMUXER_MEM_MALLOC     OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_MEM_MALLOC)
#define OT_ERR_DEMUXER_PROBE          OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_PROBE)
#define OT_ERR_DEMUXER_ACTION         OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_ACTION)
#define OT_ERR_DEMUXER_INVALID_HANDLE \
    OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_INVALID_HANDLE)
#define OT_ERR_DEMUXER_FREE_PACKET    OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_FREE_PACKET)
#define OT_ERR_DEMUXER_SET_ATTR       OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_SET_ATTR)
#define OT_ERR_DEMUXER_NOT_SUPPORT    OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_NOT_SUPPORT)
#define OT_ERR_DEMUXER_CLOSE_FILE     OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_CLOSE_FILE)
#define OT_ERR_DEMUXER_SKIP_FILE      OT_APP_DEF_ERR(OT_APPID_DEMUXER, APP_ERR_LEVEL_ERROR, APP_DEMUXER_ERR_SKIP)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* HI_DEMUXER_ERR_H */
