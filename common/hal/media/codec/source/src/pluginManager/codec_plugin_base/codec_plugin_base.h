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

#ifndef CODEC_PLUGIN_BASE_H
#define CODEC_PLUGIN_BASE_H

#include "codec_type.h"
#include "codec_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX_PLUGIN_NAME_LEN  256
#define MAX_CODEC_CAPBILITY_NUM  20

typedef struct {
    void *module;
    td_char pluginName[MAX_PLUGIN_NAME_LEN];
    CodecCapability capbilites[MAX_CODEC_CAPBILITY_NUM];
    int validCapbilityNum;
    bool isHardwarePlugin;
    td_s32 (*pfnCreate)(void **pluginHandle, const Param *attr, int len);
    td_s32 (*pfnDestroy)(void *pluginHandle);
    td_s32 (*pfnStart)(void *pluginHandle);
    td_s32 (*pfnStop)(void *pluginHandle);
    td_s32 (*pfnReset)(void *pluginHandle);
    td_s32 (*pfnGetParameter)(void *pluginHandle, Param *attr);
    td_s32 (*pfnSetParameter)(void *pluginHandle, const Param *attr);
    td_s32 (*pfnInvoke)(void *pluginHandle, td_u32 cmd, void *arg1, void *arg2);
} CodecPluginCommon;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* CODEC_PLUGIN_BASE_H */

