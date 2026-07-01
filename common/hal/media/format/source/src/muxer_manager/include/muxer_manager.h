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

#ifndef MUXER_MANAGER_H
#define MUXER_MANAGER_H

#include "format_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

void MuxerManagerInit(void);
void MuxerManagerDeInit(void);
int32_t MuxerCreate(FormatHandle * const handle, const FormatOutputConfig *outputConfig);
int32_t MuxerDestroy(const FormatHandle handle);
int32_t MuxerAddTrack(const FormatHandle handle, const TrackSource *trackSource);
int32_t MuxerSetCallBack(const FormatHandle handle, const FormatCallback *callBack);
int32_t MuxerSetOrientation(const FormatHandle handle, int degrees);
int32_t MuxerSetLocation(const FormatHandle handle, int latitude, int longitude);
int32_t MuxerSetMaxFileSize(const FormatHandle handle, int64_t bytes);
int32_t MuxerSetMaxFileDuration(const FormatHandle handle, int64_t durationUs);
int32_t MuxerSetFileSplitDuration(const FormatHandle handle, ManualSplitType type, int64_t timestampUs,
    uint32_t durationS);
int32_t MuxerStart(const FormatHandle handle);
int32_t MuxerWriteFrame(const FormatHandle handle, const FormatFrame *frameData);
int32_t MuxerSetNextOutputFile(const FormatHandle handle, int32_t fd);
int32_t MuxerStop(const FormatHandle handle, bool block);
int32_t MuxerSetParameter(const FormatHandle handle, int32_t trackId, const ParameterItem *item, int32_t itemNum);
int32_t MuxerGetParameter(const FormatHandle handle, int32_t trackId, ParameterItem *item, int32_t itemNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // MUXER_MANAGER_H
