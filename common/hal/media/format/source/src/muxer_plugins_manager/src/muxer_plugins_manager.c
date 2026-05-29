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

#include "muxer_plugins_manager.h"
#include "plugins_manager_common.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MODULE_NAME "MuxerPluginManager"

typedef struct {
    const MuxerPluginEntry *plugin;
    const ProtocolFun *protocol;
    const MuxerOutput *cfgParam;
    HalHandle muxerHdl;
} MuxerProbe;

typedef struct {
    bool inited;
    pthread_mutex_t mutex;
    int pluginsNum;
    PluginEntry *list;
} MuxerPluginsManager;

static MuxerPluginsManager g_muxerPluginsManager = {
    .inited = false,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .pluginsNum = 0,
    .list = NULL,
};

static PluginEntry *GetMuxerPluginEntry(const char *name, int32_t len)
{
    MEDIA_HAL_UNUSED(len);
    if (name == NULL || strstr(name, "libplugin_muxer") == NULL) {
        return NULL;
    }
    void *module = MediaHalDLOpen(name);
    CHK_NULL_RETURN_NO_LOG(module, NULL);
    GetMuxerFun func = MediaHalDLSym(module, "GetMuxer");
    if (func == NULL) {
        MediaHalDLClose(module);
        return NULL;
    }
    const MuxerPluginEntry *entry = func();

    MEDIA_HAL_LOGI(MODULE_NAME, "add lib :%s!", entry->desc.libName);
    PluginEntry *node = PluginFillNode((const PluginDesc *)&entry->desc, module);
    if (node == NULL || node->plugin == NULL) {
        MediaHalDLClose(module);
        return NULL;
    }
    if (node->plugin->libName != NULL) {
        MEDIA_HAL_LOGI(MODULE_NAME, "add lib :%s succeed!", node->plugin->libName);
    }
    return node;
}

void MuxerPluginManagerInit(void)
{
    MuxerPluginsManager *manager = &g_muxerPluginsManager;
    if (manager->inited) {
        return;
    }
    PluginScanPathAndAddNode(&manager->list, &manager->pluginsNum, GetMuxerPluginEntry);
    if (manager->list == NULL) {
        MEDIA_HAL_LOGW(MODULE_NAME, "Warnning!!! No muxer registered");
    }
    manager->inited = true;
    MEDIA_HAL_LOGI(MODULE_NAME, "MuxerPluginManagerInit success");
    return;
}

void MuxerPluginManagerDeInit(void)
{
    MuxerPluginsManager *manager = &g_muxerPluginsManager;
    if (manager->inited == false) {
        return;
    }
    PluginRemoveAllNodes(manager->list);
    manager->list = NULL;
    manager->pluginsNum = 0;
    manager->inited = false;
    MEDIA_HAL_LOGI(MODULE_NAME, "MuxerPluginManagerDeInit success");
    return;
}

static const MuxerPluginEntry *FindNextMuxerPlugin(const MuxerPluginsManager *manager, const MuxerOutput *cfgParam,
    const MuxerPluginEntry *cur, const ProtocolFun *protocol)
{
    PluginEntry *p = manager->list;
    const MuxerPluginEntry *selectMuxer = NULL;
    while (p != NULL) {
        if (cur == NULL) {
            break;
        }
        if ((const MuxerPluginEntry *)p->plugin == cur) {
            p = (PluginEntry *)p->list.next;
            break;
        }
        p = (PluginEntry *)p->list.next;
    }
    while (p != NULL) {
        const MuxerPluginEntry *plugin = (const MuxerPluginEntry *)p->plugin;
        if (plugin != NULL && plugin->fmt_find != NULL) {
            if (plugin->fmt_find(cfgParam, protocol) == 0) {
                selectMuxer = plugin;
                break;
            }
        }
        p = (PluginEntry *)p->list.next;
    }
    return selectMuxer;
}

static int32_t ProbeMuxerPlugins(const MuxerPluginsManager *manager, MuxerProbe *ctx, const ProtocolFun *protocol)
{
    int32_t ret = -1;
    const MuxerPluginEntry *entry = NULL;
    void *muxerHdl = NULL;
    entry = FindNextMuxerPlugin(manager, ctx->cfgParam, NULL, protocol);
    while (entry != NULL && ret != 0) {
        ret = entry->fmt_open((const MuxerOutput *)ctx->cfgParam, protocol, &muxerHdl);
        if (ret == 0) {
            ctx->plugin = entry;
            ctx->muxerHdl = muxerHdl;
            ctx->protocol = protocol;
            break;
        }
        ctx->muxerHdl = NULL;
        entry = FindNextMuxerPlugin(manager, ctx->cfgParam, entry, protocol);
    }
    return (ret == 0 && ctx->plugin != NULL) ? 0 : -1;
}

const MuxerPluginEntry *MuxerPluginManagerFindPlugin(const MuxerOutput *cfgParam, const ProtocolFun *protocol,
    HalHandle *muxerHdl)
{
    int32_t ret;
    MuxerProbe ctx;
    const MuxerPluginEntry *plugin = NULL;
    MuxerPluginsManager *manager = &g_muxerPluginsManager;
    CHK_NULL_RETURN(cfgParam, NULL, "cfgParam null");
    CHK_NULL_RETURN(muxerHdl, NULL, "muxerHdl null");
    MEDIA_HAL_LOCK(manager->mutex);
    if (manager->inited == false) {
        MEDIA_HAL_UNLOCK(manager->mutex);
        return NULL;
    }
    ctx.plugin = NULL;
    ctx.protocol = NULL;
    ctx.muxerHdl = NULL;
    ctx.cfgParam = cfgParam;
    ret = ProbeMuxerPlugins(manager, &ctx, NULL);
    if (ret == 0 && ctx.plugin != NULL) {
        *muxerHdl = ctx.muxerHdl;
        MEDIA_HAL_UNLOCK(manager->mutex);
        return ctx.plugin;
    }
    if (protocol == NULL) {
        MEDIA_HAL_UNLOCK(manager->mutex);
        return NULL;
    }
    if (ProbeMuxerPlugins(manager, &ctx, protocol) == 0) {
        *muxerHdl = ctx.muxerHdl;
        plugin = ctx.plugin;
    }
    MEDIA_HAL_UNLOCK(manager->mutex);
    return plugin;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
