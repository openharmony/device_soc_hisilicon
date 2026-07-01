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

#ifndef CODEC_PLUGIN_MANAGER_H
#define CODEC_PLUGIN_MANAGER_H

#include "adec_common.h"
#include "aenc_common.h"
#include "ot_list.h"
#include "vdec_common.h"
#include "venc_common.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    OtListHead listPtr;
    union {
        AvVideoDecoder videoDecoder;
        AvAudioDecoder audioDecoder;
        AvVideoEncoder videoEncoder;
        AvAudioEncoder audioEncoder;
    };
} CodecPluginNode;

int SearchLoadValidPlugin(const char *dirPath, const char *getImplSymbol, CodecType type);

void* FindSpecificPlugin(AvCodecMime mime, bool hardwareFirst, CodecType type);

int UnLoadPlugin(CodecType type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* CODEC_PLUGIN_MANAGER_H */
