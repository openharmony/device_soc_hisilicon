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

#include "protocol_plugins_manager.h"

#include <dlfcn.h>
#include <dirent.h>
#include "plugins_manager_common.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define BASE_PATH "usr/lib/"
#define DNAME_TYPE_FILE 8
#define PATH_LEN 1024
#define MODULE_NAME "ProtocolManager"

typedef struct {
    bool inited;
    pthread_mutex_t mutex;
    int pluginsNum;
    PluginEntry *list;
} ProtocolPluginsManager;

ProtocolPluginsManager g_protocolPluginsManager = {
    .inited = false,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .pluginsNum = 0,
    .list = NULL,
};

static PluginEntry *GetProtocolPluginEntry(const char *name, int32_t len)
{
    MEDIA_HAL_UNUSED(len);
    if (name == NULL || strlen(name) < strlen("libplugin_protocol") || strstr(name, "libplugin_protocol") == NULL) {
        return NULL;
    }
    void *module = MediaHalDLOpen(name);
    CHK_NULL_RETURN_NO_LOG(module, NULL);
    GetProtocolFun func = MediaHalDLSym(module, "GetProtocol");
    if (func == NULL) {
        MediaHalDLClose(module);
        return NULL;
    }
    const ProtocolPluginEntry *entry = func();
    MEDIA_HAL_LOGI(MODULE_NAME, "add lib :%s!", entry->desc.libName);
    PluginEntry *node = PluginFillNode((const PluginDesc *)entry, module);
    if (node == NULL) {
        MediaHalDLClose(module);
    }
    return node;
}

void ProtocolPluginManagerInit(void)
{
    ProtocolPluginsManager *manager = &g_protocolPluginsManager;
    if (manager->inited == true) {
        return;
    }
    PluginScanPathAndAddNode(&manager->list, &manager->pluginsNum, GetProtocolPluginEntry);
    if (manager->list == NULL) {
        MEDIA_HAL_LOGI(MODULE_NAME, "Warnning!!! No protocol registered");
    }
    manager->inited = true;
    return;
}

void ProtocolPluginManagerDeInit(void)
{
    ProtocolPluginsManager *manager = &g_protocolPluginsManager;
    if (manager->inited == false) {
        return;
    }
    PluginRemoveAllNodes(manager->list);
    manager->list = NULL;
    manager->pluginsNum = 0;
    manager->inited = false;
    return;
}

int32_t ProtocolPluginManagerFindPlugin(const char *url, int32_t len, ProtocolFun *func)
{
    MEDIA_HAL_UNUSED(len);
    int32_t ret;
    int32_t protocolMerit = 0;
    ProtocolPluginsManager *manager = &g_protocolPluginsManager;
    PluginEntry *p = manager->list;
    const ProtocolPluginEntry *selectProtocol = NULL;
    CHK_NULL_RETURN(func, -1, "func NULL");
    CHK_NULL_RETURN(url, -1, "url NULL");
    MEDIA_HAL_LOCK(manager->mutex);
    if (manager->inited == false) {
        MEDIA_HAL_UNLOCK(manager->mutex);
        return -1;
    }
    while (p != NULL) {
        ProtocolPluginEntry *plugin = (ProtocolPluginEntry *)p->plugin;
        if (plugin != NULL && plugin->protocol_find != NULL) {
            ret = plugin->protocol_find(url);
            if ((ret == 0) && (plugin->desc.priority > protocolMerit)) {
                selectProtocol = (const ProtocolPluginEntry *)p->plugin;
                protocolMerit = plugin->desc.priority;
            }
        }
        p = (PluginEntry *)p->list.next;
    }
    if (selectProtocol != NULL) {
        func->protocol_find = selectProtocol->protocol_find;
        func->protocol_open = selectProtocol->protocol_open;
        func->protocol_seek = selectProtocol->protocol_seek;
        func->protocol_seek_stream = selectProtocol->protocol_seek_stream;
        func->protocol_read = selectProtocol->protocol_read;
        func->protocol_write = selectProtocol->protocol_write;
        func->protocol_invoke = selectProtocol->protocol_invoke;
        func->protocol_close = selectProtocol->protocol_close;
        ret = 0;
    } else {
        ret = -1;
    }
    MEDIA_HAL_UNLOCK(manager->mutex);
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
