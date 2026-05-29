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

#ifndef AV_VDEC_COMMON_H
#define AV_VDEC_COMMON_H

#include "codec_plugin_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX_FF_LIB_NAME_LEN  1024

typedef struct {
    td_u32 len;          /* W; stream len */
    int64_t pts;          /* W; time stamp */
    td_bool endOfFrame;  /* W; is the end of a frame */
    td_bool endOfStream; /* W; is the end of all stream */
    td_bool display;     /* W; is the current frame displayed. only valid by OT_VDEC_SEND_MODE_FRAME */
    td_u8 *addr;         /* W; stream address */
} AvVdecStream;

typedef struct {
    td_u32 maxWidth;
    td_u32 maxHeight;
    td_u32 errRecover;
    td_u32 bufSize;
    td_u32 frameBufCnt;
    void *priv;
} AvVdecAttr;

#define VDEC_ERR_BASE  0x1000

#define VDEC_ERR_STREAM_BUF_FULL (VDEC_ERR_BASE + 1)

#define VDEC_ERR_FRAME_BUF_EMPTY (VDEC_ERR_BASE + 2)
#define VDEC_FRAME_BUF_EOS (VDEC_ERR_BASE + 3)

typedef struct {
    td_u32 streamBufsizes;     /* R; stream buf size */
    td_u32 leftStreamBytes;    /* R; left stream bytes waiting for decode */
    td_u32 leftStreamFrames;   /* R; left frames waiting for decode,only valid for H264D_MODE_FRAME */
    td_u32 leftPics;           /* R; pics waiting for output */
    td_bool startRecvStream;   /* R; had started recv stream? */
    td_u32 recvStreamFrames;   /* R; how many frames of stream has been received. valid when send by frame. */
    td_u32 decodeStreamFrames; /* R; how many frames of stream has been decoded. valid when send by frame. */
} AvVdecStatus;

typedef enum {
    AV_VDEC_CMD_BUTT,
} AvVdecCmd;

typedef struct {
    CodecPluginCommon pluginCommon;
    td_s32 (*pfnQueueInput)(void *vdecHdl, const InputInfo *vdecStream, td_u32 timeout);
    td_s32 (*pfnDequeueOutput)(void *vdecHdl, OutputInfo *outInfo, td_u32 timeout, int *acquireFd);
    td_s32 (*pfnQueueOutput)(void *vdecHdl, OutputInfo *outInfo, int releaseFenceFd);
    td_s32 (*pfnGetStatus)(void *vdecHdl, AvVdecStatus *vdecStatus);
} AvVideoDecoder;

void* VDecoderGetImpl(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* AV_VDEC_COMMON_H */
