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

#include "demuxer_plugins_manager.h"
#include "plugins_manager_common.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MODULE_NAME "DemuxerPluginManager"
#define PLUGIN_PROBE_SCORE_FULL 100
#define PLUGIN_PROBE_SCORE_HIGH 90
#define PLUGIN_PROBE_SCORE_LOW 50

typedef struct {
    const OT_DEMUX_S *plugin;
    const ProtocolFun *protocol;
    int score;
    const DemuxerSource *source;
    HalHandle demuxerHdl;
} DemuxerProbe;

typedef struct {
    bool inited;
    pthread_mutex_t mutex;
    int pluginsNum;
    PluginEntry *list;
} DemuxerPluginsManager;

static DemuxerPluginsManager g_demuxerPluginsManager = {
    .inited = false,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .pluginsNum = 0,
    .list = NULL,
};

PluginEntry *GetDemuxerPluginEntry(const char *name, int32_t len)
{
    MEDIA_HAL_UNUSED(len);
    if (name == NULL || (strstr(name, "libplugin_demuxer") == NULL && strstr(name, "libmpf_demuxer") == NULL)) {
        return NULL;
    }
    void *module = MediaHalDLOpen(name);
    CHK_NULL_RETURN_NO_LOG(module, NULL);

    OT_DEMUX_S *demuxEntry = MediaHalDLSym(module, OT_DEMUXER_EMTRY_NAME);
    if (demuxEntry == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "dlsym g_stFormatEntry error");
        MediaHalDLClose(module);
        return NULL;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "add lib :%s!", demuxEntry->aszDemuxerName);
    PluginEntry *node = PluginFillNode((const PluginDesc *)demuxEntry, module);
    if (node == NULL) {
        MediaHalDLClose(module);
    }
    return node;
}

void DemuxerPluginManagerInit(void)
{
    DemuxerPluginsManager *manager = &g_demuxerPluginsManager;
    if (manager->inited == true) {
        return;
    }
    PluginScanPathAndAddNode(&manager->list, &manager->pluginsNum, GetDemuxerPluginEntry);
    if (manager->list == NULL) {
        MEDIA_HAL_LOGW(MODULE_NAME, "Warnning!!! No demuxer registered");
    }
    manager->inited = true;
    return;
}

void DemuxerPluginManagerDeInit(void)
{
    DemuxerPluginsManager *manager = &g_demuxerPluginsManager;
    if (manager->inited == false) {
        return;
    }
    PluginRemoveAllNodes(manager->list);
    manager->list = NULL;
    manager->pluginsNum = 0;
    manager->inited = false;
    return;
}

static const OT_DEMUX_S *FindNextDemuxerPlugin(const DemuxerPluginsManager *manager,
    const DemuxerSource *source, const OT_DEMUX_S *cur, const ProtocolFun *protocol)
{
    PluginEntry *p = manager->list;
    const OT_DEMUX_S *selectDemuxer = NULL;
    while (p != NULL) {
        if (cur == NULL) {
            break;
        }
        if ((const OT_DEMUX_S *)p->plugin == cur) {
            p = (PluginEntry *)p->list.next;
            break;
        }
        p = (PluginEntry *)p->list.next;
    }
    while (p != NULL) {
        const OT_DEMUX_S *plugin = (const OT_DEMUX_S *)p->plugin;
        if (plugin != NULL && plugin->stFmtFun.fmt_find != NULL && protocol == NULL) {
            if (plugin->stFmtFun.fmt_find(source) == 0) {
                selectDemuxer = plugin;
                break;
            }
        }
        p = (PluginEntry *)p->list.next;
    }
    return selectDemuxer;
}

static int32_t ProbeInDemuxerPlugin(const DemuxerProbe *ctx, const OT_DEMUX_S *entry, HalHandle *demuxerHdl,
    int32_t *score)
{
    int32_t ret = 0;
    if (entry->stFmtFun.fmt_open != NULL) {
        ret = entry->stFmtFun.fmt_open(demuxerHdl, ctx->source);
        if (ret == 0 && score != NULL) {
            *score = PLUGIN_PROBE_SCORE_FULL;
        }
    }

    if (ret == 0 && entry->stFmtFun.fmt_probe != NULL) {
        ret = entry->stFmtFun.fmt_probe(*demuxerHdl, OT_DEMUXER_PROBE_MODE_NORMAL);
    }

    if (ret != 0) {
        (void)entry->stFmtFun.fmt_close(*demuxerHdl);
        *demuxerHdl = NULL;
        *score = 0;
    }
    return ret;
}

static int32_t ProbeDemuxerPlugins(const DemuxerPluginsManager *manager, DemuxerProbe *ctx,
    const ProtocolFun *protocol)
{
    int32_t ret;
    int32_t score;
    const OT_DEMUX_S *entry = NULL;
    void *demuxerHdl = NULL;

    entry = FindNextDemuxerPlugin(manager, ctx->source, NULL, protocol);
    do {
        ret = -1;
        score = 0;
        if (entry == NULL) {
            break;
        }

        ret = ProbeInDemuxerPlugin(ctx, entry, &demuxerHdl, &score);
        if (ret != 0) {
            entry = FindNextDemuxerPlugin(manager, ctx->source, entry, protocol);
            continue;
        }
        if (score > ctx->score && demuxerHdl != NULL && entry != NULL) {
            ctx->score = score;
            if (ctx->plugin != NULL && ctx->demuxerHdl != NULL) {
                (void)ctx->plugin->stFmtFun.fmt_close(ctx->demuxerHdl);
            }
            ctx->plugin = entry;
            ctx->protocol = protocol;
            ctx->demuxerHdl = demuxerHdl;
        }
        if (score == PLUGIN_PROBE_SCORE_FULL) {
            break;
        }
    } while (ret != 0 && entry != NULL);

    return ret;
}

static const OT_DEMUX_S *AnalyseProbeResult(DemuxerProbe *ctx, HalHandle *demuxerHdl)
{
    if (ctx->score >= PLUGIN_PROBE_SCORE_LOW) {
        *demuxerHdl = ctx->demuxerHdl;
        return ctx->plugin;
    } else {
        if (ctx->plugin != NULL && ctx->demuxerHdl != NULL) {
            (void)ctx->plugin->stFmtFun.fmt_close(ctx->demuxerHdl);
            ctx->demuxerHdl = NULL;
        }
        return NULL;
    }
}

const OT_DEMUX_S *DemuxerPluginManagerFindPlugin(const DemuxerSource *source, const ProtocolFun *protocol,
    HalHandle *demuxerHdl)
{
    int32_t ret;
    DemuxerProbe ctx;
    const OT_DEMUX_S *plugin = NULL;
    DemuxerPluginsManager *manager = &g_demuxerPluginsManager;
    if (source == NULL || demuxerHdl == NULL) {
        return NULL;
    }

    MEDIA_HAL_LOCK(manager->mutex);
    if (manager->inited == false) {
        MEDIA_HAL_UNLOCK(manager->mutex);
        return NULL;
    }
    ctx.plugin = NULL;
    ctx.protocol = NULL;
    ctx.score = 0;
    ctx.demuxerHdl = NULL;
    ctx.source = source;
    ret = ProbeDemuxerPlugins(manager, &ctx, NULL);
    if (ret == 0 && ctx.score >= PLUGIN_PROBE_SCORE_HIGH) {
        *demuxerHdl = ctx.demuxerHdl;
        MEDIA_HAL_UNLOCK(manager->mutex);
        return ctx.plugin;
    }

    if (protocol == NULL) {
        plugin = AnalyseProbeResult(&ctx, demuxerHdl);
        MEDIA_HAL_UNLOCK(manager->mutex);
        return plugin;
    }

    (void)ProbeDemuxerPlugins(manager, &ctx, protocol);
    plugin = AnalyseProbeResult(&ctx, demuxerHdl);
    MEDIA_HAL_UNLOCK(manager->mutex);
    return plugin;
}

void FormatPacketToUpperPacket(OT_FORMAT_PACKET_S *packet, FormatPacket *upperPacket)
{
    if (packet == NULL || upperPacket == NULL || packet->packetType > PACKET_TYPE_BUT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "FormatPacketToUpperPacket failed\n");
        return;
    }

    upperPacket->packetType = packet->packetType;
    upperPacket->trackId = packet->u32StreamIndex;
    upperPacket->isKeyFrame = packet->bKeyFrame;
    upperPacket->timestampUs = packet->s64Pts;
    upperPacket->durationUs = packet->s64Duration;
    upperPacket->data = packet->pu8Data;
    upperPacket->len = packet->u32Size;
    upperPacket->frameIndex = 0;
    upperPacket->position = packet->s64Pos;
    upperPacket->itemCnt = 0;
    upperPacket->item = NULL;
}

void FormatUpperPacketToPacket(OT_FORMAT_PACKET_S *packet, FormatPacket *upperPacket)
{
    if (packet == NULL || upperPacket == NULL || upperPacket->packetType > PACKET_TYPE_BUT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "FormatUpperPacketToPacket failed\n");
        return;
    }

    packet->packetType = upperPacket->packetType;
    packet->u32StreamIndex = upperPacket->trackId;
    packet->bKeyFrame = upperPacket->isKeyFrame;
    packet->s64Pts = upperPacket->timestampUs;
    packet->s64Duration = upperPacket->durationUs;
    packet->pu8Data = upperPacket->data;
    packet->u32Size = upperPacket->len;
    packet->s64Pos = upperPacket->position;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
