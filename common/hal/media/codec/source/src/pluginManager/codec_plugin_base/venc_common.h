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

#ifndef AV_VENC_COMMON_H
#define AV_VENC_COMMON_H

#include "codec_plugin_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VENC_ERR_BASE 0x1000

#define VENC_ERR_INVALID_OP (VENC_ERR_BASE + 1)
#define VENC_ERR_NOTREG (VENC_ERR_BASE + 2)
#define VENC_ERR_NOMEM (VENC_ERR_BASE + 3)
#define VENC_ERR_NOFREE_CHANNEL (VENC_ERR_BASE + 4)
#define VENC_ERR_CHANNEL_NOT_CREATED (VENC_ERR_BASE + 5)
#define VENC_ERR_CHANNEL_NOT_STARTED (VENC_ERR_BASE + 6)
#define VENC_ERR_VENC_ERR (VENC_ERR_BASE + 7)

#define VENC_ERR_STREAM_BUF_FULL (VENC_ERR_BASE + 8)

#define VENC_ERR_FRAME_BUF_EMPTY (VENC_ERR_BASE + 9)
#define VENC_RECEIVE_EOS         (VENC_ERR_BASE + 10)
#define VENC_CACHED_STREAM_WAIT (VENC_ERR_BASE + 11)
#define VENC_ERR_INVALID_STATE (VENC_ERR_BASE + 12)

#define VENC_ERR_UNKNOWN (VENC_ERR_BASE + 100)

#define MAX_FF_LIB_NAME_LEN  1024

#define MAX_PLUGIN_NAME_LEN  256

#define MAX_VENC_CAPBILITY_NUM  20

typedef struct {
    CodecPluginCommon pluginCommon;
    td_s32 (*pfnQueueInput)(void *vencHdl, const InputInfo *inputInfo, uint32_t frameIndex, td_bool block);
    td_s32 (*pfnDequeueOutput)(void *vencHdl, OutputInfo *outInfo, td_bool block);
    td_s32 (*pfnQueueOutput)(void *vencHdl, OutputInfo *outInfo);
} AvVideoEncoder;

void *VEncoderGetImpl(void);

td_s32 VEncoderImplUnRegister();

td_s32 VEncoderEnableLowDelay(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* AV_VENC_COMMON_H */

