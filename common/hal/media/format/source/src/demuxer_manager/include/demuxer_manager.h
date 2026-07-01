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

#ifndef DEMUXER_MANAGER_H
#define DEMUXER_MANAGER_H

#include <stdint.h>
#include <format_type.h>

void DemuxerManagerInit(void);
void DemuxerManagerDeInit(void);
int32_t DemuxerCreate(const FormatSource *source, FormatHandle * const handle);
int32_t DemuxerSetParameter(const FormatHandle handle, int32_t trackId, const ParameterItem *metaData,
    int32_t metaDataCnt);
int32_t DemuxerGetParameter(const FormatHandle handle, int32_t trackId, ParameterItem *metaData);
int32_t DemuxerSetCallBack(const FormatHandle handle, const FormatCallback *callBack);
int32_t DemuxerSetBufferConfig(const FormatHandle handle, const FormatBufferSetting *setting);
int32_t DemuxerGetBufferConfig(const FormatHandle handle, FormatBufferSetting *setting);
int32_t DemuxerPrepare(const FormatHandle handle);
int32_t DemuxerGetFileInfo(const FormatHandle handle, FormatFileInfo *info);
int32_t DemuxerSelectTrack(const FormatHandle handle, int32_t programId, int32_t trackId);
int32_t DemuxerUnselectTrack(const FormatHandle handle, int32_t programId, int32_t trackId);
int32_t DemuxerStart(const FormatHandle handle);
int32_t DemuxerGetSelectedTrack(const FormatHandle handle, int32_t *programId, int32_t trackId[], int32_t *nums);
int32_t DemuxerReadFrame(const FormatHandle handle, FormatFrame *frame, int32_t timeOutMs);
int32_t DemuxerFreeFrame(const FormatHandle handle, FormatFrame *frame);
int32_t DemuxerSeek(const FormatHandle handle, int32_t streamIndex, int64_t timeStampUs, FormatSeekMode mode);
int32_t DemuxerStop(const FormatHandle handle);
int32_t DemuxerDestroy(const FormatHandle handle);
#endif  // DEMUXER_MANAGER_H