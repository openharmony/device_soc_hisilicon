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

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "securec.h"
#include "media_hal_common.h"
#include "list.h"
#include "audio_plugins_manager.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MODULE_NAME "AudioPluginManager"

#ifdef ENABLE_DL_AUDIO_PLUGIN
#define AUDIO_PORT_PLUGIN_FUNCS "GetPortPluginFuncs"

#define PRIMARY_PORT_PLUGIN_LIB_PATH "/lib/libaudio_primary_port.so"
#define PRIMARY_PORT_PLUGIN_USR_LIB_PATH "/usr/lib/libaudio_primary_port.so"

#define HDMI_PORT_PLUGIN_LIB_PATH "/lib/libaudio_hdmi_port.so"
#define HDMI_PORT_PLUGIN_USR_LIB_PATH "/usr/lib/libaudio_hdmi_port.so"

#define USB_PORT_PLUGIN_LIB_PATH "/lib/libaudio_usb_port.so"
#define USB_PORT_PLUGIN_USR_LIB_PATH "/usr/lib/libaudio_usb_port.so"

#define BLUETOOTH_PORT_PLUGIN_LIB_PATH "/lib/libaudio_bluetooth_port.so"
#define BLUETOOTH_PORT_PLUGIN_USR_LIB_PATH "/usr/lib/libaudio_bluetooth_port.so"
#endif

typedef struct {
    AudioPluginType audioPluginType;
    uintptr_t pluginHandle;
#ifdef ENABLE_DL_AUDIO_PLUGIN
    void *pluginDlHandle;
#endif
} AudioPlugin;

typedef struct {
    bool inited;
    pthread_mutex_t mutex;
    List plugins;
} AudioPluginsManager;

static AudioPluginsManager g_audioPluginsManager = {
    .inited = false,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
};

static int32_t CheckPortPlugin(const AudioPortPlugin *plugin)
{
    CHK_NULL_RETURN(plugin->GetPortPluginCapability, MEDIA_HAL_INVALID_PARAM,
        "port plugin GetPortPluginCapability() is null");
    CHK_NULL_RETURN(plugin->Open, MEDIA_HAL_INVALID_PARAM, "port plugin Open() is null");
    CHK_NULL_RETURN(plugin->CreateTrack, MEDIA_HAL_INVALID_PARAM, "port plugin CreateTrack() is null");
    CHK_NULL_RETURN(plugin->DestroyTrack, MEDIA_HAL_INVALID_PARAM, "port plugin DestroyTrack() is null");
    CHK_NULL_RETURN(plugin->Close, MEDIA_HAL_INVALID_PARAM, "port plugin Close() is null");
    return MEDIA_HAL_OK;
}

static int32_t ScanAndLoadPrimaryPortPlugin(AudioPluginsManager *manager)
{
    AudioPlugin primaryPlugin = {};
    primaryPlugin.audioPluginType = AUDIO_PLUGIN_PRIMARY;
    const AudioPortPlugin *primaryPluginImpl = NULL;
#ifdef ENABLE_DL_AUDIO_PLUGIN
    primaryPlugin.pluginDlHandle = MediaHalDLOpen(PRIMARY_PORT_PLUGIN_USR_LIB_PATH);
    if (primaryPlugin.pluginDlHandle == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not found primary port plugin in path: %s", PRIMARY_PORT_PLUGIN_USR_LIB_PATH);
        primaryPlugin.pluginDlHandle = MediaHalDLOpen(PRIMARY_PORT_PLUGIN_LIB_PATH);
        if (primaryPlugin.pluginDlHandle == NULL) {
            MEDIA_HAL_LOGE(MODULE_NAME, "not found primary port plugin in path: %s", PRIMARY_PORT_PLUGIN_LIB_PATH);
            return MEDIA_HAL_ERR;
        }
    }
    GetPortPlugin getPluginFuns = (GetPortPlugin)MediaHalDLSym(primaryPlugin.pluginDlHandle, AUDIO_PORT_PLUGIN_FUNCS);
    if (getPluginFuns == NULL) {
        MediaHalDLClose(primaryPlugin.pluginDlHandle);
        MEDIA_HAL_LOGE(MODULE_NAME, "not found symbol: %s", AUDIO_PORT_PLUGIN_FUNCS);
        return MEDIA_HAL_ERR;
    }
    primaryPluginImpl = getPluginFuns(AUDIO_PLUGIN_NONE);
    if (primaryPluginImpl == NULL ||
        CheckPortPlugin(primaryPluginImpl) != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not found valid primary port plugin");
        MediaHalDLClose(primaryPlugin.pluginDlHandle);
        return MEDIA_HAL_ERR;
    }
#else
    primaryPluginImpl = GetPortPluginFuncs(AUDIO_PLUGIN_PRIMARY);
    if (primaryPluginImpl == NULL ||
        CheckPortPlugin(primaryPluginImpl) != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not found valid primary port plugin");
        return MEDIA_HAL_ERR;
    }
#endif
    primaryPlugin.pluginHandle = (uintptr_t)primaryPluginImpl;
    if (ListPushFront(&manager->plugins, (uintptr_t)&primaryPlugin) != LIST_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "primary port plugin cannot be pushed into the list");
        return MEDIA_HAL_ERR;
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}


static int32_t ScanAndLoadUSBPortPlugin(AudioPluginsManager *manager)
{
    AudioPlugin usbPlugin = {};
    usbPlugin.audioPluginType = AUDIO_PLUGIN_USB;
    const AudioPortPlugin *usbPluginImpl = NULL;
#ifdef ENABLE_DL_AUDIO_PLUGIN
    usbPlugin.pluginDlHandle = MediaHalDLOpen(USB_PORT_PLUGIN_USR_LIB_PATH);
    if (usbPlugin.pluginDlHandle == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not found usb port plugin in path: %s", USB_PORT_PLUGIN_USR_LIB_PATH);
        usbPlugin.pluginDlHandle = MediaHalDLOpen(USB_PORT_PLUGIN_LIB_PATH);
        if (usbPlugin.pluginDlHandle == NULL) {
            MEDIA_HAL_LOGE(MODULE_NAME, "not found usb port plugin in path: %s", USB_PORT_PLUGIN_LIB_PATH);
            return MEDIA_HAL_ERR;
        }
    }
    GetPortPlugin getPluginFuns = (GetPortPlugin)MediaHalDLSym(usbPlugin.pluginDlHandle, AUDIO_PORT_PLUGIN_FUNCS);
    if (getPluginFuns == NULL) {
        MediaHalDLClose(usbPlugin.pluginDlHandle);
        MEDIA_HAL_LOGE(MODULE_NAME, "not found symbol: %s", AUDIO_PORT_PLUGIN_FUNCS);
        return MEDIA_HAL_ERR;
    }
    usbPluginImpl = getPluginFuns(AUDIO_PLUGIN_NONE);
    if (usbPluginImpl == NULL ||
        CheckPortPlugin(usbPluginImpl) != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not found valid hdmi port plugin");
        MediaHalDLClose(usbPlugin.pluginDlHandle);
        return MEDIA_HAL_ERR;
    }
#else
    usbPluginImpl = GetPortPluginFuncs(AUDIO_PLUGIN_USB);
    if (usbPluginImpl == NULL ||
        CheckPortPlugin(usbPluginImpl) != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not found valid hdmi port plugin");
        return MEDIA_HAL_ERR;
    }
#endif
    usbPlugin.pluginHandle = (uintptr_t)usbPluginImpl;
    if (ListPushFront(&manager->plugins, (uintptr_t)&usbPlugin) != LIST_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "hdmi port plugin cannot be pushed into the list");
        return MEDIA_HAL_ERR;
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static uintptr_t DupPlugin(const AudioPlugin *audioPlugin)
{
    CHK_NULL_RETURN(audioPlugin, MEDIA_HAL_INVALID_PARAM, "input param audioPlugin null");
    AudioPortPlugin *plugin = (AudioPortPlugin *)malloc(sizeof(AudioPortPlugin));
    if (plugin == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc failed");
        return 0;
    }
    *plugin = *(AudioPortPlugin *)audioPlugin->pluginHandle;
    return (uintptr_t)plugin;
}

static uintptr_t PluginListDup(uintptr_t ptr)
{
    if (ptr == 0) {
        return 0;
    }
    AudioPlugin *audioPluginSrc = (AudioPlugin *)ptr;
    AudioPlugin *audioPluginDst = (AudioPlugin *)malloc(sizeof(AudioPlugin));
    if (audioPluginDst == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc failed");
        return 0;
    }
    if (memcpy_s(audioPluginDst, sizeof(AudioPlugin), audioPluginSrc,
                 sizeof(AudioPlugin)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s audioPluginDst failed");
        free(audioPluginDst);
        return 0;
    }
    audioPluginDst->pluginHandle = DupPlugin(audioPluginSrc);
    if (audioPluginDst->pluginHandle == 0) {
        free(audioPluginDst);
        return 0;
    }
    return (uintptr_t)audioPluginDst;
}

static void FreePlugin(AudioPlugin *audioPlugin)
{
    AudioPortPlugin *plugin = (AudioPortPlugin *)audioPlugin->pluginHandle;
    if (plugin != NULL) {
        free(plugin);
        audioPlugin->pluginHandle = 0;
    }
}

static void PluginListFree(uintptr_t ptr)
{
    if (ptr == 0) {
        return;
    }
    AudioPlugin *audioPlugin = (AudioPlugin *)ptr;
#ifdef ENABLE_DL_AUDIO_PLUGIN
    if (audioPlugin->pluginDlHandle != NULL) {
        MediaHalDLClose(audioPlugin->pluginDlHandle);
    }
#endif
    FreePlugin(audioPlugin);
    free(audioPlugin);
}

void AudioPluginManagerInit(void)
{
    AudioPluginsManager *manager = &g_audioPluginsManager;
    MEDIA_HAL_LOCK(manager->mutex);
    if (manager->inited) {
        MEDIA_HAL_UNLOCK(manager->mutex);
        return;
    }
    DupFreeFuncPair dataFunc;
    dataFunc.dupFunc = PluginListDup;
    dataFunc.freeFunc = PluginListFree;
    ListInit(&manager->plugins, &dataFunc);

    if (ScanAndLoadPrimaryPortPlugin(manager) != MEDIA_HAL_OK) {
        goto LOAD_FAILED;
    }
    if (ScanAndLoadUSBPortPlugin(manager) != MEDIA_HAL_OK) {
        goto LOAD_FAILED;
    }

    manager->inited = true;
    MEDIA_HAL_UNLOCK(manager->mutex);
    return;
LOAD_FAILED:
    ListDeinit(&manager->plugins);
    MEDIA_HAL_UNLOCK(manager->mutex);
    return;
}

const void *AudioPluginManagerGetPlugin(AudioPluginType pluginType)
{
    AudioPluginsManager *manager = &g_audioPluginsManager;
    MEDIA_HAL_LOCK(manager->mutex);
    if (!manager->inited) {
        MEDIA_HAL_UNLOCK(manager->mutex);
        return NULL;
    }
    List *pluginListHead = &manager->plugins;
    MEDIA_HAL_LOGI(MODULE_NAME, "get audio plugin type: %d", pluginType);
    for (ListIterator it = ListIterBegin(pluginListHead); it != ListIterEnd(pluginListHead);
        it = ListIterNext(pluginListHead, it)) {
        AudioPlugin *audioPlugin = (AudioPlugin *)ListIterData(it);
        if (audioPlugin != NULL && audioPlugin->audioPluginType == pluginType) {
            MEDIA_HAL_UNLOCK(manager->mutex);
            return (const void *)audioPlugin->pluginHandle;
        }
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "get audio plugin failed: %d", pluginType);
    MEDIA_HAL_UNLOCK(manager->mutex);
    return NULL;
}

void AudioPluginManagerDeInit(void)
{
    AudioPluginsManager *manager = &g_audioPluginsManager;
    MEDIA_HAL_LOCK(manager->mutex);
    if (!manager->inited) {
        MEDIA_HAL_UNLOCK(manager->mutex);
        return;
    }
    ListDeinit(&manager->plugins);
    manager->inited = false;
    MEDIA_HAL_UNLOCK(manager->mutex);
    pthread_mutex_destroy(&manager->mutex);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
