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

#include <stdbool.h>
#include "ss_mpi_sys.h"
#include "ss_mpi_audio.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
#define MODULE_NAME "MediaHalCommonSys"

static bool g_systemInited = false;
static pthread_mutex_t g_systemInitMutex = PTHREAD_MUTEX_INITIALIZER;

int32_t MediaSystemInit(void)
{
    MEDIA_HAL_LOCK(g_systemInitMutex);
    if (g_systemInited) {
        MEDIA_HAL_LOGI(MODULE_NAME, "Media System has already inited");
        MEDIA_HAL_UNLOCK(g_systemInitMutex);
        return MEDIA_HAL_OK;
    }
    int32_t ret = ss_mpi_sys_init();
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_UNLOCK(g_systemInitMutex);
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_init failed. ret:0x%x !", ret);
        return ret;
    }
    ret = ss_mpi_adec_aac_init();
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_UNLOCK(g_systemInitMutex);
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_adec_aac_init fail,ret:%x", ret);
        return ret;
    }
    ret = ss_mpi_aenc_aac_init();
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_UNLOCK(g_systemInitMutex);
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_aenc_aac_init fail,ret:%x", ret);
        return ret;
    }
    g_systemInited = true;
    MEDIA_HAL_UNLOCK(g_systemInitMutex);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

int32_t MediaSystemDeinit(void)
{
    MEDIA_HAL_LOCK(g_systemInitMutex);
    if (!g_systemInited) {
        MEDIA_HAL_UNLOCK(g_systemInitMutex);
        return MEDIA_HAL_OK;
    }
    int32_t ret = ss_mpi_aenc_aac_deinit();
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_aenc_aac_deinit fail,ret:%x", ret);
    }
    ret = ss_mpi_adec_aac_deinit();
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_adec_aac_deinit fail,ret:%x", ret);
    }
    ret = ss_mpi_sys_exit();
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_exit failed. ret:0x%x !", ret);
    }
    g_systemInited = false;
    MEDIA_HAL_UNLOCK(g_systemInitMutex);
    MEDIA_HAL_LOGE(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
