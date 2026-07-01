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
 *
 * Description: muxer manager interface
 */

#ifndef REC_MUXER_MANAGER_H
#define REC_MUXER_MANAGER_H

#include <stdint.h>
#include "ot_mw_type.h"
#include "ot_track_source.h"
#include "ss_recorder_pro.h"

typedef struct {
    td_s32 s32FileAllocSize;
    int32_t fileDuration;
    TD_MW_PTR hMuxerHandle;
} MuxerConfig;

#define CHK_RETURN(condition, ret, printfString) \
    do { \
        if (condition) { \
            printf("[%s: %d], %s \n", __func__, __LINE__, ((printfString) == NULL) ? "" : (printfString)); \
            return ret; \
        } \
    } while (0)

#define CHK_RETURN_VOID(condition, printfString) \
        do { \
            if (condition) { \
                printf("[%s: %d], %s \n", __func__, __LINE__, ((printfString) == NULL) ? "" : (printfString)); \
                return; \
            } \
        } while (0)

td_s32 CreateMP4Muxer(const td_char *pszFileName, void *pMuxerConfig, TD_MW_PTR *pMuxerHandle);
td_s32 DestroyMP4Muxer(TD_MW_PTR hMuxerHandle);
td_s32 CreateMP4Track(TD_MW_PTR hMuxerHandle, OT_Track_Source_Handle pstTrackSrcHandle, TD_MW_PTR *phTrackHandle);
td_s32 WriteMP4Frame(TD_MW_PTR hMuxerHandle, TD_MW_PTR hTrackHandle,
    const OT_REC_FRAME_DATA_S *pstFrameData, TD_U64 *fileSize);
td_s32 WriteMP4Gps(TD_MW_PTR hMuxerHandle, const td_u8 *frameAddr, int len);
td_s32 WriteMP4FrontCover(TD_MW_PTR hMuxerHandle, const td_u8 *frameAddr, int len);


td_s32 CreateTSMuxer(const td_char *pszFileName, void *pMuxerConfig, TD_MW_PTR *pMuxerHandle);
td_s32 DestroyTSMuxer(TD_MW_PTR hMuxerHandle);
td_s32 CreateTSTrack(TD_MW_PTR hMuxerHandle, OT_Track_Source_Handle pstTrackSrcHandle, TD_MW_PTR *phTrackHandle);
td_s32 WriteTSFrame(TD_MW_PTR hMuxerHandle, TD_MW_PTR hTrackHandle,
    const OT_REC_FRAME_DATA_S *pstFrameData, TD_U64 *fileSize);

td_s32 CreateSlaveMuxer(const td_char *pszFileName, void *pMuxerConfig, TD_MW_PTR *pMuxerHandle);
td_s32 DestroySlaveMuxer(TD_MW_PTR hMuxerHandle);
td_s32 CreateSlaveTrack(TD_MW_PTR hMuxerHandle,
    OT_Track_Source_Handle pstTrackSrcHandle, TD_MW_PTR *phTrackHandle);
#endif
