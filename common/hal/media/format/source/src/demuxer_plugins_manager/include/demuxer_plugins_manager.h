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

#ifndef DEMUXER_PLUGINS_MANAGER_H
#define DEMUXER_PLUGINS_MANAGER_H

#include "plugins_manager_common.h"
#include "demuxer_interface.h"
#include "ss_demuxer.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

void DemuxerPluginManagerInit(void);
void DemuxerPluginManagerDeInit(void);
const OT_DEMUX_S *DemuxerPluginManagerFindPlugin(const DemuxerSource *source, const ProtocolFun *protocol,
    HalHandle *demuxerHdl);
void FormatPacketToUpperPacket(OT_FORMAT_PACKET_S *packet, FormatPacket *upperPacket);
void FormatUpperPacketToPacket(OT_FORMAT_PACKET_S *packet, FormatPacket *upperPacket);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // FORMAT_INTERFACE_H
/** @} */