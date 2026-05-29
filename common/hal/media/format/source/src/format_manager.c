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

#ifdef ENABLE_DEMUXER
#include "demuxer_manager.h"
#endif
#ifdef ENABLE_MUXER
#include "muxer_manager.h"
#endif
#include "format_interface.h"
#include "media_hal_common.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MODULE_NAME "FormatManager"

typedef struct {
    void (*DemuxerManagerInit)(void);
    void (*DemuxerManagerDeInit)(void);
    int32_t (*DemuxerCreate)(const FormatSource *source, FormatHandle * const handle);
    int32_t (*DemuxerSetParameter)(const FormatHandle handle, int32_t trackId, const ParameterItem *metaData,
        int32_t metaDataCnt);
    int32_t (*DemuxerGetParameter)(const FormatHandle handle, int32_t trackId, ParameterItem *metaData);
    int32_t (*DemuxerSetCallBack)(const FormatHandle handle, const FormatCallback *callBack);
    int32_t (*DemuxerSetBufferConfig)(const FormatHandle handle, const FormatBufferSetting *setting);
    int32_t (*DemuxerGetBufferConfig)(const FormatHandle handle, FormatBufferSetting *setting);
    int32_t (*DemuxerPrepare)(const FormatHandle handle);
    int32_t (*DemuxerGetFileInfo)(const FormatHandle handle, FormatFileInfo *info);
    int32_t (*DemuxerSelectTrack)(const FormatHandle handle, int32_t programId, int32_t trackId);
    int32_t (*DemuxerUnselectTrack)(const FormatHandle handle, int32_t programId, int32_t trackId);
    int32_t (*DemuxerStart)(const FormatHandle handle);
    int32_t (*DemuxerGetSelectedTrack)(const FormatHandle handle, int32_t *programId, int32_t trackId[],
        int32_t *nums);
    int32_t (*DemuxerReadFrame)(const FormatHandle handle, FormatFrame *frame, int32_t timeOutMs);
    int32_t (*DemuxerFreeFrame)(const FormatHandle handle, FormatFrame *frame);
    int32_t (*DemuxerSeek)(const FormatHandle handle, int32_t streamIndex, int64_t timeStampUs, FormatSeekMode mode);
    int32_t (*DemuxerStop)(const FormatHandle handle);
    int32_t (*DemuxerDestroy)(const FormatHandle handle);
} DemuxerManagerFunc;

typedef struct {
    void (*MuxerManagerInit)(void);
    void (*MuxerManagerDeInit)(void);
    int32_t (*MuxerCreate)(FormatHandle * const handle, const FormatOutputConfig *outputConfig);
    int32_t (*MuxerDestroy)(const FormatHandle handle);
    int32_t (*MuxerAddTrack)(const FormatHandle handle, const TrackSource *trackSource);
    int32_t (*MuxerSetCallBack)(const FormatHandle handle, const FormatCallback *callBack);
    int32_t (*MuxerSetOrientation)(const FormatHandle handle, int degrees);
    int32_t (*MuxerSetLocation)(const FormatHandle handle, int latitude, int longitude);
    int32_t (*MuxerSetMaxFileSize)(const FormatHandle handle, int64_t bytes);
    int32_t (*MuxerSetMaxFileDuration)(const FormatHandle handle, int64_t durationUs);
    int32_t (*MuxerSetFileSplitDuration)(const FormatHandle handle, ManualSplitType type, int64_t timestampUs,
        uint32_t durationUs);
    int32_t (*MuxerStart)(const FormatHandle handle);
    int32_t (*MuxerWriteFrame)(const FormatHandle handle, const FormatFrame *frameData);
    int32_t (*MuxerSetNextOutputFile)(const FormatHandle handle, int32_t fd);
    int32_t (*MuxerStop)(const FormatHandle handle, bool block);
    int32_t (*MuxerSetParameter)(const FormatHandle handle, int32_t trackId, const ParameterItem *item,
        int32_t itemNum);
    int32_t (*MuxerGetParameter)(const FormatHandle handle, int32_t trackId, ParameterItem *item, int32_t itemNum);
} MuxerManagerFunc;

typedef struct {
    bool inited;
    pthread_mutex_t mutex;
    DemuxerManagerFunc demuxerMgrFunc;
    MuxerManagerFunc muxerMgrFunc;
} FormatManager;

FormatManager g_formatManager = {
    .inited = false,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
};

#ifdef ENABLE_DEMUXER
static void DemuxerManagerFuncInit(DemuxerManagerFunc *demuxerMgrFunc)
{
    demuxerMgrFunc->DemuxerManagerInit = DemuxerManagerInit;
    demuxerMgrFunc->DemuxerManagerDeInit = DemuxerManagerDeInit;
    demuxerMgrFunc->DemuxerCreate = DemuxerCreate;
    demuxerMgrFunc->DemuxerSetParameter = DemuxerSetParameter;
    demuxerMgrFunc->DemuxerGetParameter = DemuxerGetParameter;
    demuxerMgrFunc->DemuxerSetCallBack = DemuxerSetCallBack;
    demuxerMgrFunc->DemuxerSetBufferConfig = DemuxerSetBufferConfig;
    demuxerMgrFunc->DemuxerGetBufferConfig = DemuxerGetBufferConfig;
    demuxerMgrFunc->DemuxerPrepare = DemuxerPrepare;
    demuxerMgrFunc->DemuxerGetFileInfo = DemuxerGetFileInfo;
    demuxerMgrFunc->DemuxerSelectTrack = DemuxerSelectTrack;
    demuxerMgrFunc->DemuxerUnselectTrack = DemuxerUnselectTrack;
    demuxerMgrFunc->DemuxerStart = DemuxerStart;
    demuxerMgrFunc->DemuxerGetSelectedTrack = DemuxerGetSelectedTrack;
    demuxerMgrFunc->DemuxerReadFrame = DemuxerReadFrame;
    demuxerMgrFunc->DemuxerFreeFrame = DemuxerFreeFrame;
    demuxerMgrFunc->DemuxerSeek = DemuxerSeek;
    demuxerMgrFunc->DemuxerStop = DemuxerStop;
    demuxerMgrFunc->DemuxerDestroy = DemuxerDestroy;
    demuxerMgrFunc->DemuxerManagerInit();
}
#endif

#ifdef ENABLE_MUXER
static void MuxerManagerFuncInit(MuxerManagerFunc *muxerMgrFunc)
{
    muxerMgrFunc->MuxerManagerInit = MuxerManagerInit;
    muxerMgrFunc->MuxerManagerDeInit = MuxerManagerDeInit;
    muxerMgrFunc->MuxerCreate = MuxerCreate;
    muxerMgrFunc->MuxerDestroy = MuxerDestroy;
    muxerMgrFunc->MuxerAddTrack = MuxerAddTrack;
    muxerMgrFunc->MuxerSetCallBack = MuxerSetCallBack;
    muxerMgrFunc->MuxerSetOrientation = MuxerSetOrientation;
    muxerMgrFunc->MuxerSetLocation = MuxerSetLocation;
    muxerMgrFunc->MuxerSetMaxFileSize = MuxerSetMaxFileSize;
    muxerMgrFunc->MuxerSetMaxFileDuration = MuxerSetMaxFileDuration;
    muxerMgrFunc->MuxerSetFileSplitDuration = MuxerSetFileSplitDuration;
    muxerMgrFunc->MuxerStart = MuxerStart;
    muxerMgrFunc->MuxerWriteFrame = MuxerWriteFrame;
    muxerMgrFunc->MuxerSetNextOutputFile = MuxerSetNextOutputFile;
    muxerMgrFunc->MuxerStop = MuxerStop;
    muxerMgrFunc->MuxerSetParameter = MuxerSetParameter;
    muxerMgrFunc->MuxerGetParameter = MuxerGetParameter;
    muxerMgrFunc->MuxerManagerInit();
}
#endif

void FormatInit(void)
{
    FormatManager *manager = &g_formatManager;
    MEDIA_HAL_LOCK(manager->mutex);
    if (manager->inited == true) {
        goto UNLOCK;
    }

    if (memset_s(&manager->demuxerMgrFunc, sizeof(DemuxerManagerFunc), 0, sizeof(DemuxerManagerFunc)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        goto UNLOCK;
    }
    if (memset_s(&manager->muxerMgrFunc, sizeof(MuxerManagerFunc), 0, sizeof(MuxerManagerFunc)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        goto UNLOCK;
    }
#ifdef ENABLE_DEMUXER
    DemuxerManagerFuncInit(&manager->demuxerMgrFunc);
#endif
#ifdef ENABLE_MUXER
    MuxerManagerFuncInit(&manager->muxerMgrFunc);
#endif
    manager->inited = true;
UNLOCK:
    MEDIA_HAL_UNLOCK(manager->mutex);
}

void FormatDeInit(void)
{
    FormatManager *manager = &g_formatManager;
    MEDIA_HAL_LOCK(manager->mutex);
    if (manager->inited == false) {
        goto UNLOCK;
    }

    if (manager->demuxerMgrFunc.DemuxerManagerDeInit != NULL) {
        manager->demuxerMgrFunc.DemuxerManagerDeInit();
    }
    if (manager->muxerMgrFunc.MuxerManagerDeInit != NULL) {
        manager->muxerMgrFunc.MuxerManagerDeInit();
    }
    if (memset_s(&manager->demuxerMgrFunc, sizeof(DemuxerManagerFunc), 0, sizeof(DemuxerManagerFunc)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        goto UNLOCK;
    }
    if (memset_s(&manager->muxerMgrFunc, sizeof(MuxerManagerFunc), 0, sizeof(MuxerManagerFunc)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        goto UNLOCK;
    }
    manager->inited = false;
UNLOCK:
    MEDIA_HAL_UNLOCK(manager->mutex);
}

int32_t FormatDemuxerCreate(const FormatSource *source, FormatHandle * const handle)
{
    FormatInit();
    CHK_FAILED_RETURN(g_formatManager.inited, true, -1, "not inited");
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerCreate, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerCreate(source, handle);
}

int32_t FormatDemuxerSetParameter(const FormatHandle handle, int32_t trackId, const ParameterItem *metaData,
    int32_t metaDataCnt)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerSetParameter, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerSetParameter(handle, trackId, metaData, metaDataCnt);
}

int32_t FormatDemuxerGetParameter(const FormatHandle handle, int32_t trackId, ParameterItem *metaData)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerGetParameter, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerGetParameter(handle, trackId, metaData);
}

int32_t FormatDemuxerSetCallBack(const FormatHandle handle, const FormatCallback *callBack)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerSetCallBack, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerSetCallBack(handle, callBack);
}

int32_t FormatDemuxerSetBufferConfig(const FormatHandle handle, const FormatBufferSetting *setting)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerSetBufferConfig, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerSetBufferConfig(handle, setting);
}

int32_t FormatDemuxerGetBufferConfig(const FormatHandle handle, FormatBufferSetting *setting)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerGetBufferConfig, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerGetBufferConfig(handle, setting);
}

int32_t FormatDemuxerPrepare(const FormatHandle handle)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerPrepare, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerPrepare(handle);
}

int32_t FormatDemuxerGetFileInfo(const FormatHandle handle, FormatFileInfo *info)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerGetFileInfo, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerGetFileInfo(handle, info);
}

int32_t FormatDemuxerSelectTrack(const FormatHandle handle, int32_t programId, int32_t trackId)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerSelectTrack, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerSelectTrack(handle, programId, trackId);
}

int32_t FormatDemuxerUnselectTrack(const FormatHandle handle, int32_t programId, int32_t trackId)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerUnselectTrack, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerUnselectTrack(handle, programId, trackId);
}

int32_t FormatDemuxerStart(const FormatHandle handle)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerStart, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerStart(handle);
}

int32_t FormatDemuxerGetSelectedTrack(const FormatHandle handle, int32_t *programId, int32_t trackId[], int32_t *nums)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerGetSelectedTrack, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerGetSelectedTrack(handle, programId, trackId, nums);
}

int32_t FormatDemuxerReadFrame(const FormatHandle handle, FormatFrame *frame, int32_t timeOutMs)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerReadFrame, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerReadFrame(handle, frame, timeOutMs);
}

int32_t FormatDemuxerFreeFrame(const FormatHandle handle, FormatFrame *frame)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerFreeFrame, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerFreeFrame(handle, frame);
}

int32_t FormatDemuxerSeek(const FormatHandle handle, int32_t streamIndex, int64_t timeStampUs, FormatSeekMode mode)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerSeek, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerSeek(handle, streamIndex, timeStampUs, mode);
}

int32_t FormatDemuxerStop(const FormatHandle handle)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerStop, -1, "not support demuxer");

    return g_formatManager.demuxerMgrFunc.DemuxerStop(handle);
}

int32_t FormatDemuxerDestroy(const FormatHandle handle)
{
    CHK_NULL_RETURN(g_formatManager.demuxerMgrFunc.DemuxerDestroy, -1, "not support demuxer");

    (void)g_formatManager.demuxerMgrFunc.DemuxerDestroy(handle);
    return MEDIA_HAL_OK;
}

int32_t FormatMuxerCreate(FormatHandle * const handle, const FormatOutputConfig *outputConfig)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerCreate, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerCreate(handle, outputConfig);
}

int32_t FormatMuxerDestroy(const FormatHandle handle)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerDestroy, -1, "not support muxer");

    (void)g_formatManager.muxerMgrFunc.MuxerDestroy(handle);
    return MEDIA_HAL_OK;
}

int32_t FormatMuxerAddTrack(const FormatHandle handle, const TrackSource *trackSource)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerAddTrack, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerAddTrack(handle, trackSource);
}

int32_t FormatMuxerSetCallBack(const FormatHandle handle, const FormatCallback *callBack)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerSetCallBack, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerSetCallBack(handle, callBack);
}

int32_t FormatMuxerSetOrientation(const FormatHandle handle, int degrees)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerSetOrientation, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerSetOrientation(handle, degrees);
}

int32_t FormatMuxerSetLocation(const FormatHandle handle, int latitude, int longitude)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerSetLocation, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerSetLocation(handle, latitude, longitude);
}

int32_t FormatMuxerSetMaxFileSize(const FormatHandle handle, int64_t bytes)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerSetMaxFileSize, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerSetMaxFileSize(handle, bytes);
}

int32_t FormatMuxerSetMaxFileDuration(const FormatHandle handle, int64_t durationUs)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerSetMaxFileDuration, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerSetMaxFileDuration(handle, durationUs);
}

int32_t FormatMuxerSetFileSplitDuration(const FormatHandle handle, ManualSplitType type, int64_t timestampUs,
    uint32_t durationUs)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerSetFileSplitDuration, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerSetFileSplitDuration(handle, type, timestampUs, durationUs);
}

int32_t FormatMuxerStart(const FormatHandle handle)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerStart, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerStart(handle);
}

int32_t FormatMuxerWriteFrame(const FormatHandle handle, const FormatFrame *frameData)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerWriteFrame, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerWriteFrame(handle, frameData);
}

int32_t FormatMuxerSetNextOutputFile(const FormatHandle handle, int32_t fd)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerSetNextOutputFile, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerSetNextOutputFile(handle, fd);
}

int32_t FormatMuxerStop(const FormatHandle handle, bool block)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerStop, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerStop(handle, block);
}

int32_t FormatMuxerSetParameter(const FormatHandle handle, int32_t trackId, const ParameterItem *item, int32_t itemNum)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerSetParameter, -1, "not support muxer");

    return  g_formatManager.muxerMgrFunc.MuxerSetParameter(handle, trackId, item, itemNum);
}

int32_t FormatMuxerGetParameter(const FormatHandle handle, int32_t trackId, ParameterItem *item, int32_t itemNum)
{
    CHK_NULL_RETURN(g_formatManager.muxerMgrFunc.MuxerGetParameter, -1, "not support muxer");

    return g_formatManager.muxerMgrFunc.MuxerGetParameter(handle, trackId, item, itemNum);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
