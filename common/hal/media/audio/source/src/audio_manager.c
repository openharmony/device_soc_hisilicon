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

#include <stdlib.h>
#include <errno.h>
#include "securec.h"
#include "media_hal_common.h"
#include "audio_internal.h"
#include "audio_plugins_manager.h"
#include "audio_manager.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

struct AudioHwAdapterInternal {
    uint32_t count;
    bool loaded;
    struct AudioHwAdapter hwAdapter;
    pthread_mutex_t adapterLock;
};

static struct AudioPort g_audioPrimaryPort[] = {
    { PORT_IN, 0, "PrimaryPortIn"},
    { PORT_OUT, 1, "PrimaryPortOut" }
};

static struct AudioPort g_audioUsbPort[] = {
    { PORT_IN, 0, "UsbPortIn"},
    { PORT_OUT, 1, "UsbPortOut" }
};

static struct AudioAdapterDescriptor g_audioAdapterDescs[] = {
    {
        .adapterName = NULL,
        .portNum = 0,
        .ports = NULL,
    },
    {
        .adapterName = NULL,
        .portNum = 0,
        .ports = NULL,
    },
};

static struct AudioHwAdapterInternal g_audioAdapters[] = {
    {
        .count = 0,
        .loaded = false,
        .hwAdapter = {},
        .adapterLock = PTHREAD_MUTEX_INITIALIZER,
    },
    {
        .count = 0,
        .loaded = false,
        .hwAdapter = {},
        .adapterLock = PTHREAD_MUTEX_INITIALIZER,
    },
};

static bool g_allAdapterLoaded = false;
static pthread_mutex_t g_adapterMutex = PTHREAD_MUTEX_INITIALIZER;

static int32_t GetAdapterCount(void)
{
    int32_t adapterCnt = 0;
    for (uint32_t adapterIndex = 0; adapterIndex < ARRAY_SIZE(g_audioAdapterDescs); adapterIndex++) {
        if (g_audioAdapterDescs[adapterIndex].adapterName != NULL) {
            adapterCnt++;
        }
    }
    MEDIA_HAL_LOGD(MODULE_NAME, "adapter count: %d", adapterCnt);
    return adapterCnt;
}

bool AudioManagerIsValidAdapter(const struct AudioAdapter *adapter)
{
    size_t adapterNum = ARRAY_SIZE(g_audioAdapters);
    CHK_NULL_RETURN(adapter, false, "input param adapter is null");
    for (size_t adapterIndex = 0; adapterIndex < adapterNum; adapterIndex++) {
        MEDIA_HAL_LOCK(g_audioAdapters[adapterIndex].adapterLock);
        if ((&g_audioAdapters[adapterIndex].hwAdapter.common) == adapter) {
            MEDIA_HAL_UNLOCK(g_audioAdapters[adapterIndex].adapterLock);
            return true;
        }
        MEDIA_HAL_UNLOCK(g_audioAdapters[adapterIndex].adapterLock);
    }
    return false;
}

static int32_t AudioManagerGetAllAdapters(struct AudioManager *manager, struct AudioAdapterDescriptor **descs,
    int32_t *size)
{
    CHK_NULL_RETURN(manager, MEDIA_HAL_INVALID_PARAM, "Input param manager is null");
    CHK_NULL_RETURN(descs, MEDIA_HAL_INVALID_PARAM, "Input param descs is null");
    CHK_NULL_RETURN(size, MEDIA_HAL_INVALID_PARAM, "Input param size is null");

    MEDIA_HAL_LOCK(g_adapterMutex);
    if (g_allAdapterLoaded) {
        *descs = g_audioAdapterDescs;
        *size = GetAdapterCount();
        MEDIA_HAL_UNLOCK(g_adapterMutex);
        return MEDIA_HAL_OK;
    }

    AudioPluginManagerInit();

    uint32_t adapterIndex = 0;
    /* init primary adapter port plugin */
    const AudioPortPlugin *primaryPlugin = AudioPluginManagerGetPlugin(AUDIO_PLUGIN_PRIMARY);
    if (primaryPlugin != NULL) {
        g_audioAdapters[adapterIndex].hwAdapter.portPlugin = primaryPlugin;
        g_audioAdapters[adapterIndex].hwAdapter.portDirection = primaryPlugin->audioPortDir;
        g_audioAdapterDescs[adapterIndex].adapterName = "Primary";
        g_audioAdapterDescs[adapterIndex].portNum = (size_t)ARRAY_SIZE(g_audioPrimaryPort);
        g_audioAdapterDescs[adapterIndex].ports = g_audioPrimaryPort;
        adapterIndex++;
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPluginManagerGetPlugin(Primary) failed");
    }

    const AudioPortPlugin *usbPlugin = AudioPluginManagerGetPlugin(AUDIO_PLUGIN_USB);
    if (usbPlugin != NULL) {
        g_audioAdapters[adapterIndex].hwAdapter.portPlugin = usbPlugin;
        g_audioAdapters[adapterIndex].hwAdapter.portDirection = usbPlugin->audioPortDir;
        g_audioAdapterDescs[adapterIndex].adapterName = "USB";
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPluginManagerGetPlugin(USB) success, adapterIndex = %d, adapterName = %s",
            adapterIndex, g_audioAdapterDescs[adapterIndex].adapterName);
        g_audioAdapterDescs[adapterIndex].portNum = (size_t)ARRAY_SIZE(g_audioUsbPort);
        g_audioAdapterDescs[adapterIndex].ports = g_audioUsbPort;

        adapterIndex++;
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPluginManagerGetPlugin(USB) failed");
    }

    if (adapterIndex == 0) {
        MEDIA_HAL_UNLOCK(g_adapterMutex);
        return MEDIA_HAL_ERR;
    } else {
        *descs = g_audioAdapterDescs;
        *size = (int32_t)ARRAY_SIZE(g_audioAdapters);
        g_allAdapterLoaded = true;
        MEDIA_HAL_UNLOCK(g_adapterMutex);
        return MEDIA_HAL_OK;
    }
}

static int32_t HwAdapterInit(const struct AudioAdapterDescriptor *desc, struct AudioHwAdapter *hwAdapter)
{
    if (memcpy_s(&(hwAdapter->adapterDescriptor),
        sizeof(struct AudioAdapterDescriptor), desc, sizeof(struct AudioAdapterDescriptor)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s hwAdapter->adapterDescriptor failed");
        return MEDIA_HAL_ERR;
    }
    MEDIA_HAL_LOGD(MODULE_NAME, "adapter descriptor port num: %d", hwAdapter->adapterDescriptor.portNum);

    size_t capSize = sizeof(struct AudioPortCapability) * hwAdapter->adapterDescriptor.portNum;
    hwAdapter->capability = (struct AudioPortCapability *)malloc(capSize);
    CHK_NULL_RETURN(hwAdapter->capability, MEDIA_HAL_NO_MEM, "malloc AudioPortCapability failed");
    if (memset_s(hwAdapter->capability, capSize, 0, capSize) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s hwAdapter->capability failed");
        free(hwAdapter->capability);
        hwAdapter->capability = NULL;
        return MEDIA_HAL_ERR;
    }
    hwAdapter->common.InitAllPorts = AudioAdapterInitAllPorts;
    hwAdapter->common.CreateRender = AudioAdapterCreateRender;
    hwAdapter->common.DestroyRender = AudioAdapterDestroyRender;
    hwAdapter->common.CreateCapture = AudioAdapterCreateCapture;
    hwAdapter->common.DestroyCapture = AudioAdapterDestroyCapture;
    hwAdapter->common.GetPortCapability = AudioAdapterGetPortCapability;
    hwAdapter->common.SetPassthroughMode = AudioAdapterSetPassthroughMode;
    hwAdapter->common.GetPassthroughMode = AudioAdapterGetPassthroughMode;
    pthread_mutex_init(&hwAdapter->mutex, NULL);
    return MEDIA_HAL_OK;
}

static int32_t AudioManagerLoadAdapter(struct AudioManager *manager, const struct AudioAdapterDescriptor *desc,
    struct AudioAdapter **adapter)
{
    CHK_NULL_RETURN(manager, MEDIA_HAL_INVALID_PARAM, "Input param manager is null");
    CHK_NULL_RETURN(desc, MEDIA_HAL_INVALID_PARAM, "Input param desc is null");
    CHK_NULL_RETURN(desc->adapterName, MEDIA_HAL_INVALID_PARAM, "Input param adapterName is null");
    CHK_NULL_RETURN(desc->ports, MEDIA_HAL_INVALID_PARAM, "Input param ports is null");
    CHK_NULL_RETURN(adapter, MEDIA_HAL_INVALID_PARAM, "Input param adapter is null");

    MEDIA_HAL_LOGD(MODULE_NAME, "adapter name: %s", desc->adapterName);
    int32_t ret = MediaSystemInit();
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "MediaSystemInit failed ret:%d", ret);
    }
    int32_t adapterNum = GetAdapterCount();
    int32_t adapterIndex;
    for (adapterIndex = 0; adapterIndex < adapterNum; adapterIndex++) {
        if (strcmp(desc->adapterName, g_audioAdapterDescs[adapterIndex].adapterName) == 0 &&
            desc->portNum == g_audioAdapterDescs[adapterIndex].portNum) {
            MEDIA_HAL_LOGD(MODULE_NAME, "supported this adapter: %s", desc->adapterName);
            break;
        }
    }
    if (adapterIndex >= adapterNum) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not supported this adapter: %s", desc->adapterName);
        return MEDIA_HAL_INVALID_PARAM;
    }

    MEDIA_HAL_LOCK(g_audioAdapters[adapterIndex].adapterLock);
    if (g_audioAdapters[adapterIndex].loaded) {
        *adapter = &(g_audioAdapters[adapterIndex].hwAdapter.common);
        g_audioAdapters[adapterIndex].count++;
        MEDIA_HAL_LOGI(MODULE_NAME, "adapter %s has loaded count: %d",
            desc->adapterName, g_audioAdapters[adapterIndex].count);
        MEDIA_HAL_UNLOCK(g_audioAdapters[adapterIndex].adapterLock);
        return MEDIA_HAL_OK;
    }

    struct AudioHwAdapter *hwAdapter = &(g_audioAdapters[adapterIndex].hwAdapter);
    ret = HwAdapterInit(desc, hwAdapter);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_UNLOCK(g_audioAdapters[adapterIndex].adapterLock);
        MEDIA_HAL_LOGE(MODULE_NAME, "HwAdapterInit failed:%d", ret);
        return ret;
    }
    *adapter = &(hwAdapter->common);
    g_audioAdapters[adapterIndex].loaded = true;
    g_audioAdapters[adapterIndex].count++;
    MEDIA_HAL_LOGD(MODULE_NAME, "adapter %s load count: %d",
        desc->adapterName, g_audioAdapters[adapterIndex].count);
    MEDIA_HAL_UNLOCK(g_audioAdapters[adapterIndex].adapterLock);
    return MEDIA_HAL_OK;
}

static void AudioManagerUnloadAdapter(struct AudioManager *manager, struct AudioAdapter *adapter)
{
    CHK_NULL_RETURN_NONE(manager, "input param manager null");
    CHK_NULL_RETURN_NONE(adapter, "input param adapter null");
    MEDIA_HAL_LOGI(MODULE_NAME, "AudioManagerUnloadAdapter: Enter");
    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;
    uint32_t adapterNum = (uint32_t)ARRAY_SIZE(g_audioAdapters);
    uint32_t adapterIndex;
    struct AudioAdapterDescriptor *adapterDescriptor = NULL;
    for (adapterIndex = 0; adapterIndex < adapterNum; adapterIndex++) {
        if (&(g_audioAdapters[adapterIndex].hwAdapter) == hwAdapter) {
            adapterDescriptor = &(g_audioAdapters[adapterIndex].hwAdapter.adapterDescriptor);
            MEDIA_HAL_LOGD(MODULE_NAME, "found this adapter");
            break;
        }
    }
    if (adapterIndex >= adapterNum) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not found this adapter");
        return;
    }
    CHK_NULL_RETURN_NONE(adapterDescriptor, "adapterDescriptor is null");
    MEDIA_HAL_LOCK(g_audioAdapters[adapterIndex].adapterLock);
    if (!(g_audioAdapters[adapterIndex].loaded)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "adapter have unloaded");
        MEDIA_HAL_UNLOCK(g_audioAdapters[adapterIndex].adapterLock);
        return;
    }

    g_audioAdapters[adapterIndex].count--;
    MEDIA_HAL_LOGD(MODULE_NAME, "adapter %s unload count: %d",
        hwAdapter->adapterDescriptor.adapterName, g_audioAdapters[adapterIndex].count);
    if (g_audioAdapters[adapterIndex].count > 0) {
        MEDIA_HAL_UNLOCK(g_audioAdapters[adapterIndex].adapterLock);
        return;
    }

    (void)AudioAdapterClosePort(adapter);

    if (hwAdapter->capability != NULL) {
        free(hwAdapter->capability);
        hwAdapter->capability = NULL;
    }
    pthread_mutex_destroy(&hwAdapter->mutex);
    g_audioAdapters[adapterIndex].loaded = false;
    MEDIA_HAL_UNLOCK(g_audioAdapters[adapterIndex].adapterLock);

    return;
}

static struct AudioManager g_audioManagerFuncs = {
    .GetAllAdapters = AudioManagerGetAllAdapters,
    .LoadAdapter = AudioManagerLoadAdapter,
    .UnloadAdapter = AudioManagerUnloadAdapter,
};

struct AudioManager *GetAudioManagerFuncs(void)
{
    return &g_audioManagerFuncs;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
