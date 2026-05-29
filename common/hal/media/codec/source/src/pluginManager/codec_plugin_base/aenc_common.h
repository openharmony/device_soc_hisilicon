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

#ifndef AV_AENC_COMMON_H
#define AV_AENC_COMMON_H

#include "codec_plugin_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define AENC_ERR_BASE 0x1000

#define AENC_ERR_INVALID_OP (AENC_ERR_BASE + 1)
#define AENC_ERR_NOTREG (AENC_ERR_BASE + 2)
#define AENC_ERR_NOMEM (AENC_ERR_BASE + 3)
#define AENC_ERR_NOFREE_CHANNEL (AENC_ERR_BASE + 4)
#define AENC_ERR_CHANNEL_NOT_CREATED (AENC_ERR_BASE + 5)
#define AENC_ERR_CHANNEL_NOT_STARTED (AENC_ERR_BASE + 6)
#define AENC_ERR_AENC_ERR (AENC_ERR_BASE + 7)
#define AENC_ERR_STREAM_BUF_FULL (AENC_ERR_BASE + 8)
#define AENC_ERR_FRAME_BUF_EMPTY (AENC_ERR_BASE + 9)
#define AENC_RECEIVE_EOS         (AENC_ERR_BASE + 10)
#define AENC_ERR_INVALID_STATE   (AENC_ERR_BASE + 11)
#define AENC_ERR_UNKNOWN (AENC_ERR_BASE + 100)

#define MAX_FF_LIB_NAME_LEN  1024

#define MAX_PLUGIN_NAME_LEN  256

#define MAX_AENC_CAPBILITY_NUM  20

typedef struct {
    CodecPluginCommon pluginCommon;
    td_s32 (*pfnQueueInput)(void *aencHdl, const InputInfo *inputInfo, uint32_t frameIndex, bool block);
    td_s32 (*pfnDequeueOutput)(void *aencHdl, OutputInfo *outInfo, bool block);
    td_s32 (*pfnQueueOutput)(void *aencHdl, OutputInfo *outInfo);
} AvAudioEncoder;

void *AEncoderGetImpl(void);

td_s32 AEncoderImplUnRegister();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* AV_AENC_COMMON_H */

