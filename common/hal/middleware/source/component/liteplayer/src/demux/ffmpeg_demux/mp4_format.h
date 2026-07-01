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

#ifndef MP4_FORMAT_H
#define MP4_FORMAT_H

#include "ff_wrapper.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

TD_S32 Mp4Read(FfDemuxContext *demux, OT_FORMAT_PACKET_S *fmtFrame);
TD_S32 Mp4Free(FfDemuxContext *demux, const OT_FORMAT_PACKET_S *fmtFrame);
TD_S32 Mp4Probe(FfDemuxContext *demux);
TD_S32 Mp4Setattr(FfDemuxContext *demux, OT_FORMAT_MEDIA_ATTR_S *fmtInfo);
TD_VOID Mp4Close(FfDemuxContext *demux);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MP4_FORMAT_H */
