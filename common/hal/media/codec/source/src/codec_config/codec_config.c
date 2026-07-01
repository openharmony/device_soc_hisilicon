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

#include "codec_config.h"
#include "codec_plugin_manager.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MODULE_NAME "CODEC_CONFIG"

int32_t SetDevice(CodecBufferChannel *channelCtx, CodecType type, DirectionType dirType,
    const Param *params)
{
    MEDIA_HAL_UNUSED(dirType);
    MEDIA_HAL_UNUSED(type);
    CHK_NULL_RETURN(params, MEDIA_HAL_INVALID_PARAM, "Param is null");
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "channelCtx is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon, MEDIA_HAL_INVALID_PARAM, "pluginCommon is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon->pfnSetParameter, MEDIA_HAL_INVALID_PARAM, "pfnSetParameter is null");
    channelCtx->isBindedMode = true;

    int32_t ret = channelCtx->pluginCommon->pfnSetParameter(channelCtx->pluginHandle, params);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "videoEncoder.pfnSetParameter failed Ret:0x%x", ret);
        return ret;
    }
    channelCtx->channelStarted = true;
    return TD_SUCCESS;
}

int32_t CodecVencSetParameter(const CodecBufferChannel *channelCtx, CodecType type,
    const Param *params)
{
    CHK_NULL_RETURN(params, MEDIA_HAL_INVALID_PARAM, "Param arg is null");
    CHK_NULL_RETURN(channelCtx, MEDIA_HAL_INVALID_PARAM, "channelCtx is null");
    CHK_NULL_RETURN(channelCtx->pluginHandle, MEDIA_HAL_INVALID_PARAM, "pluginHandle is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon, MEDIA_HAL_INVALID_PARAM, "pluginCommon is null");
    CHK_NULL_RETURN(channelCtx->pluginCommon->pfnSetParameter, MEDIA_HAL_INVALID_PARAM, "pfnSetParameter is null");

    if (type != VIDEO_ENCODER) {
        MEDIA_HAL_LOGE(MODULE_NAME, "only venc support setting device");
        return TD_FAILURE;
    }

    int32_t ret = channelCtx->pluginCommon->pfnSetParameter(channelCtx->pluginHandle, params);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "videoEncoder.pfnSetParameter failed Ret:Ox%x", ret);
        return ret;
    }
    return TD_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
