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
 * Description: muxer manager
 */

#include "rec_muxer_manager.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "ot_defs.h"
#include "ss_mp4_format.h"
#include "ss_ts_format.h"
#include "media_hal_common.h"
#include "securec.h"
#define MODULE_NAME "HisiMuxerMng"

#define BYTE_UNIT 1024 // 1024 byte unit
#define TIME_MS_UNIT 1000 // 1000 time unit

#define META_ATOM_DATA_SIZE 4
#define HDLR_ATOM_DATA_SIZE 34

static const td_u32 WRITE_FRAME_TIMEOUT  = 200; // 200 write frame timeout

td_s32 CreateMP4Muxer(const td_char *pszFileName, void *pMuxerConfig, TD_MW_PTR *pMuxerHandle)
{
    CHK_NULL_RETURN(pszFileName, TD_FAILURE, "pszFileName is null");
    CHK_NULL_RETURN(pMuxerHandle, TD_FAILURE, "pMuxerHandle is null");
    CHK_NULL_RETURN(pMuxerConfig, TD_FAILURE, "pMuxerConfig is null");
    OT_MP4_CONFIG_S stMuxerConfig = { 0 };
    td_s32 s32Ret;
    MuxerConfig *pstMuxerConfig = (MuxerConfig*)pMuxerConfig;
    if (strcpy_s(stMuxerConfig.aszFileName, OT_MP4_MAX_FILE_NAME, pszFileName) != EOK) {
        MEDIA_HAL_LOGI(MODULE_NAME, "strcpy_s stMuxerConfig.aszFileName fail\n ");
        return TD_FAILURE;
    }
    stMuxerConfig.enConfigType = OT_MP4_CONFIG_MUXER;
    stMuxerConfig.stMuxerConfig.u32PreAllocUnit = pstMuxerConfig->s32FileAllocSize;
    stMuxerConfig.stMuxerConfig.bCo64Flag = TD_FALSE;
    stMuxerConfig.stMuxerConfig.bConstantFps = TD_FALSE;
    stMuxerConfig.stMuxerConfig.u32VBufSize = BYTE_UNIT * BYTE_UNIT;
    stMuxerConfig.stMuxerConfig.u32BackupUnit = 50 * 1024 * 1024; /* 50 * 1024 * 1024: BackupUnit size in bytes */
    s32Ret = SS_MP4_Create(pMuxerHandle, &stMuxerConfig);
    if (s32Ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_MP4_Create failed:%d", s32Ret);
        return s32Ret;
    }
    pstMuxerConfig->hMuxerHandle = *pMuxerHandle;
    return s32Ret;
}

td_s32 CreateTSMuxer(const td_char *pszFileName, void *pMuxerConfig, TD_MW_PTR *pMuxerHandle)
{
    CHK_NULL_RETURN(pszFileName, TD_FAILURE, "pszFileName is null");
    CHK_NULL_RETURN(pMuxerHandle, TD_FAILURE, "pMuxerHandle is null");
    CHK_NULL_RETURN(pMuxerConfig, TD_FAILURE, "pMuxerConfig is null");
    OT_TS_CONFIG_S stMuxerConfig = { 0 };
    td_s32 s32Ret;
    MuxerConfig *pstMuxerConfig = (MuxerConfig*)pMuxerConfig;
    stMuxerConfig.enConfigType = OT_TS_CONFIG_MUXER;
    if (strncpy_s(stMuxerConfig.aszFileName, OT_TS_MAX_FILE_NAME, pszFileName, OT_TS_MAX_FILE_NAME - 1) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "strncpy_s fileName error");
        return TD_FAILURE;
    }
    stMuxerConfig.stMuxerConfig.u32PreAllocUnit = pstMuxerConfig->s32FileAllocSize;
    stMuxerConfig.stMuxerConfig.u32VBufSize = BYTE_UNIT * BYTE_UNIT;
    s32Ret = SS_TS_Create(&stMuxerConfig, pMuxerHandle);
    if (s32Ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_TS_Create failed:%d", s32Ret);
        return s32Ret;
    }
    pstMuxerConfig->hMuxerHandle = *pMuxerHandle;
    return s32Ret;
}


td_s32 DestroyMP4Muxer(TD_MW_PTR hMuxerHandle)
{
    td_s32 s32Ret;
    int64_t time;
    struct timeval begin = { 0, 0 };
    struct timeval end = { 0, 0 };

    gettimeofday(&begin, NULL);
    if (SS_MP4_DestroyAllTracks(hMuxerHandle, NULL) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "call SS_MP4_DestroyAllTracks fail");
    }
    gettimeofday(&end, NULL);
    time = CalcDiffTimeMs(begin, end);
    MEDIA_HAL_LOGE(MODULE_NAME, "call SS_MP4_DestroyAllTracks cost:%lld", time);

    gettimeofday(&begin, NULL);
    s32Ret = SS_MP4_Destroy(hMuxerHandle, NULL);
    gettimeofday(&end, NULL);
    time = CalcDiffTimeMs(begin, end);
    MEDIA_HAL_LOGE(MODULE_NAME, "call SS_MP4_Destroy cost:%lld", time);
    return s32Ret;
}

td_s32 DestroyTSMuxer(TD_MW_PTR hMuxerHandle)
{
    td_s32 s32Ret;
    if (SS_TS_DestroyAllStreams(hMuxerHandle) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "call SS_TS_DestroyAllStreams fail");
    }
    s32Ret = SS_TS_Destroy(hMuxerHandle);
    MEDIA_HAL_LOGI(MODULE_NAME, "SS_TS_DestroyAllStreams:%d", s32Ret);
    return s32Ret;
}


td_s32 CreateSlaveMuxer(const td_char *pszFileName, void *pMuxerConfig, TD_MW_PTR *pMuxerHandle)
{
    MW_UNUSED(pszFileName);
    CHK_NULL_RETURN(pMuxerConfig, TD_FAILURE, "pMuxerConfig is null");
    CHK_NULL_RETURN(pMuxerHandle, TD_FAILURE, "pMuxerHandle is null");
    td_s32 s32Ret = TD_SUCCESS;
    // set muxhandle to first muxer handle.
    MuxerConfig *pstMuxerConfig = (MuxerConfig*)pMuxerConfig;
    *pMuxerHandle = pstMuxerConfig->hMuxerHandle;
    MEDIA_HAL_LOGI(MODULE_NAME, "CreateSlaveMuxer: success");
    return s32Ret;
}

td_s32 DestroySlaveMuxer(TD_MW_PTR hMuxerHandle)
{
    MW_UNUSED(hMuxerHandle);
    return TD_SUCCESS;
}

td_s32 CreateSlaveTrack(TD_MW_PTR hMuxerHandle,
    OT_Track_Source_Handle pstTrackSrcHandle, TD_MW_PTR *phTrackHandle)
{
    CHK_NULL_RETURN(hMuxerHandle, TD_FAILURE, "hMuxerHandle is null");
    CHK_NULL_RETURN(pstTrackSrcHandle, TD_FAILURE, "pstTrackSrcHandle is null");
    MEDIA_HAL_LOGI(MODULE_NAME, "in");
    OT_MP4_TRACK_INFO_S stTrakInfo = { 0 };
    if (pstTrackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_VIDEO) {
        stTrakInfo.enTrackType = OT_MP4_STREAM_DATA;
        stTrakInfo.stDataInfo.u32Width = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Width;
        stTrakInfo.stDataInfo.u32Height = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Height;
        stTrakInfo.stVideoInfo.u32BitRate = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32BitRate;
        stTrakInfo.stVideoInfo.frameRate = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.frameRate;
        if (snprintf_s(stTrakInfo.aszHdlrName, OT_MP4_MAX_HDLR_NAME,
            OT_MP4_MAX_HDLR_NAME - 1, "%s", "Hisilicon MetaData") <= EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s stTrakInfo.aszHdlrName Hisilicon MetaData error");
            return TD_FAILURE;
        }
        stTrakInfo.stDataInfo.enCodecID = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.enCodecType ==
            OT_TRACK_VIDEO_CODEC_H264 ? OT_MP4_CODEC_ID_H264 : OT_MP4_CODEC_ID_H265;
        stTrakInfo.fSpeed = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.fSpeed;
        stTrakInfo.u32TimeScale = 120000; /* 120000: time scale for trak */
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "slave track not video type");
        return TD_FAILURE;
    }
    return SS_MP4_CreateTrack(hMuxerHandle, phTrackHandle, &stTrakInfo);
}

td_s32 CreateMP4Track(TD_MW_PTR hMuxerHandle, OT_Track_Source_Handle pstTrackSrcHandle, TD_MW_PTR *phTrackHandle)
{
    CHK_NULL_RETURN(pstTrackSrcHandle, TD_FAILURE, "pstTrackSrcHandle is null");
    CHK_NULL_RETURN(phTrackHandle, TD_FAILURE, "phTrackHandle is null");
    OT_MP4_TRACK_INFO_S stTrakInfo = { 0 };
    if (pstTrackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_VIDEO) {
        stTrakInfo.enTrackType = OT_MP4_STREAM_VIDEO;
        if (snprintf_s(stTrakInfo.aszHdlrName, OT_MP4_MAX_HDLR_NAME,
            OT_MP4_MAX_HDLR_NAME - 1, "%s", "Hisilicon VIDEO") <= EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s stTrakInfo.aszHdlrName Hisilicon VIDEO error");
            return TD_FAILURE;
        }
        stTrakInfo.stVideoInfo.u32Width = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Width;
        stTrakInfo.stVideoInfo.u32Height = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32Height;
        stTrakInfo.stVideoInfo.u32BitRate = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.u32BitRate;
        stTrakInfo.stVideoInfo.frameRate = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.frameRate;
        stTrakInfo.stVideoInfo.enCodecID = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.enCodecType ==
            OT_TRACK_VIDEO_CODEC_H264 ? OT_MP4_CODEC_ID_H264 : OT_MP4_CODEC_ID_H265;
        stTrakInfo.fSpeed = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.fSpeed;
        stTrakInfo.u32TimeScale = 120000; /* 120000: time scale for trak */
    } else if (pstTrackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_AUDIO) {
        stTrakInfo.enTrackType = OT_MP4_STREAM_AUDIO;
        if (snprintf_s(stTrakInfo.aszHdlrName, OT_MP4_MAX_HDLR_NAME,
            OT_MP4_MAX_HDLR_NAME - 1, "%s", "Hisilicon AUDIO") <= EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s stTrakInfo.aszHdlrName Hisilicon AUDIO error");
            return TD_FAILURE;
        }
        stTrakInfo.stAudioInfo.u32Channels = pstTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32ChnCnt;
        stTrakInfo.stAudioInfo.u32SampleRate = pstTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32SampleRate;
        stTrakInfo.stAudioInfo.u16SampleSize = pstTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u16SampleBitWidth;
        stTrakInfo.stAudioInfo.u32SamplePerFrame = pstTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32SamplesPerFrame;
        stTrakInfo.stAudioInfo.enCodecID = OT_MP4_CODEC_ID_AACLC;
        stTrakInfo.fSpeed = 1;
        stTrakInfo.u32TimeScale = pstTrackSrcHandle->unTrackSourceAttr.stAudioInfo.u32SampleRate;
    } else if (pstTrackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_PRIV) {
        stTrakInfo.enTrackType = OT_MP4_STREAM_DATA;

        if (snprintf_s(stTrakInfo.aszHdlrName, OT_MP4_MAX_HDLR_NAME,
            OT_MP4_MAX_HDLR_NAME - 1, "%s", "Hisilicon DATA") <= EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s stTrakInfo.aszHdlrName(Hisilicon DATA) error");
            return TD_FAILURE;
        }

        stTrakInfo.stDataInfo.frameRate = pstTrackSrcHandle->unTrackSourceAttr.stPrivInfo.frameRate;
        stTrakInfo.fSpeed = 1;
        stTrakInfo.u32TimeScale = 120000; /* 120000 : default mp4 time scale. */
        stTrakInfo.stDataInfo.u32Width = 0;
        stTrakInfo.stDataInfo.u32Height = 0;
        stTrakInfo.stDataInfo.enCodecID = OT_MP4_CODEC_ID_BUTT;
    }
    return SS_MP4_CreateTrack(hMuxerHandle, phTrackHandle, &stTrakInfo);
}

td_s32 CreateTSTrack(TD_MW_PTR hMuxerHandle, OT_Track_Source_Handle pstTrackSrcHandle, TD_MW_PTR *phTrackHandle)
{
    CHK_NULL_RETURN(pstTrackSrcHandle, TD_FAILURE, "pstTrackSrcHandle is null");
    CHK_NULL_RETURN(phTrackHandle, TD_FAILURE, "phTrackHandle is null");
    OT_TS_STREAM_INFO_S stTrakInfo = { 0 };
    if (pstTrackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_VIDEO) {
        stTrakInfo.enCodecID = pstTrackSrcHandle->unTrackSourceAttr.stVideoInfo.enCodecType ==
            OT_TRACK_VIDEO_CODEC_H264 ? OT_TS_CODEC_ID_H264 : OT_TS_CODEC_ID_H265;
    } else if (pstTrackSrcHandle->enTrackType == OT_TRACK_SOURCE_TYPE_AUDIO) {
        stTrakInfo.enCodecID = OT_TS_CODEC_ID_AAC;
    }
    return SS_TS_CreateStream(hMuxerHandle, phTrackHandle, &stTrakInfo);
}


td_s32 WriteMP4Frame(TD_MW_PTR hMuxerHandle, TD_MW_PTR hTrackHandle,
    const OT_REC_FRAME_DATA_S *pstFrameData, TD_U64 *fileSize)
{
    MEDIA_HAL_UNUSED(fileSize);
    CHK_NULL_RETURN(pstFrameData, TD_FAILURE, "pstFrameData is null");
    struct timeval start;
    struct timeval end;
    long long diff;
    td_s32 s32Ret;
    OT_MP4_FRAME_DATA_S stFrameData;

    stFrameData.u64TimeStamp = pstFrameData->u64TimeStamp;
    stFrameData.pu8DataBuffer = pstFrameData->apu8SliceAddr[0];
    stFrameData.u32DataLength = pstFrameData->au32SliceLen[0];
    stFrameData.bKeyFrameFlag = pstFrameData->bKeyFrameFlag;
    (void)gettimeofday(&start, NULL);
    s32Ret = SS_MP4_WriteFrame(hMuxerHandle, hTrackHandle, &stFrameData);
    (void)gettimeofday(&end, NULL);

    diff = CalcDiffTimeMs(start, end);
    if (diff > WRITE_FRAME_TIMEOUT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "WriteFrame len %u cost %lld ms", stFrameData.u32DataLength, diff);
    }
    return s32Ret;
}

td_s32 WriteTSFrame(TD_MW_PTR hMuxerHandle, TD_MW_PTR hTrackHandle,
    const OT_REC_FRAME_DATA_S *pstFrameData, TD_U64 *fileSize)
{
    MEDIA_HAL_UNUSED(fileSize);
    CHK_NULL_RETURN(pstFrameData, TD_FAILURE, "pstFrameData is null");
    struct timeval start;
    struct timeval end;
    long long diff;
    td_s32 s32Ret;
    OT_TS_FRAME_DATA_S stFrameData;

    stFrameData.u64TimeStamp = pstFrameData->u64TimeStamp;
    stFrameData.pu8DataBuffer = pstFrameData->apu8SliceAddr[0];
    stFrameData.u32DataLength = pstFrameData->au32SliceLen[0];
    stFrameData.bKeyFrameFlag = pstFrameData->bKeyFrameFlag;
    (void)gettimeofday(&start, NULL);
    s32Ret = SS_TS_WriteFrame(hMuxerHandle, hTrackHandle, &stFrameData);
    (void)gettimeofday(&end, NULL);

    diff = CalcDiffTimeMs(start, end);
    if (diff > WRITE_FRAME_TIMEOUT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "WriteFrame len %u cost %lld ms", stFrameData.u32DataLength, diff);
    }
    return s32Ret;
}

static td_s32 WriteMP4GpsData(TD_MW_PTR hMuxerHandle, const td_u8 *frameAddr, int len)
{
    OT_MP4_ATOM_INFO_S gpsAtomInfo = {0};
    CHK_FAILED_RETURN(strcpy_s(gpsAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "gps "), EOK, TD_FAILURE, "strcpy failed.");
    CHK_FAILED_RETURN(SS_MP4_AddAtom(hMuxerHandle, "/moov/udta/meta/ilst", &gpsAtomInfo), TD_SUCCESS, TD_FAILURE,
        "add covr atom failed.");

    OT_MP4_ATOM_INFO_S dataAtomInfo = {0};
    CHK_FAILED_RETURN(strcpy_s(dataAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "data"), EOK, TD_FAILURE, "strcpy failed.");

    td_u32 dataAtomDataLen = len;
    td_u8* dataAtomData = (td_u8*)malloc(dataAtomDataLen);
    CHK_NULL_RETURN(dataAtomData, TD_FAILURE, "malloc covr atom buffer failed");
    if (memset_s(dataAtomData, dataAtomDataLen, 0, dataAtomDataLen) != EOK) {
        free(dataAtomData);
        return TD_FAILURE;
    }
    if (memcpy_s(dataAtomData, dataAtomDataLen, frameAddr, len) != EOK) {
        free(dataAtomData);
        return TD_FAILURE;
    }
    dataAtomInfo.pu8DataBuf = dataAtomData;
    dataAtomInfo.u32DataLen = len;
    return SS_MP4_AddAtom(hMuxerHandle, "/moov/udta/meta/ilst/gps ", &dataAtomInfo);
}

static td_s32 CreatUdtaBox(TD_MW_PTR hMuxerHandle)
{
    OT_MP4_ATOM_INFO_S metaAtomInfo = {0};
    CHK_FAILED_RETURN(strcpy_s(metaAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "meta"), EOK, TD_FAILURE, "strcpy failed.");
    td_char metaData[META_ATOM_DATA_SIZE] = {0};
    metaAtomInfo.pu8DataBuf = (td_u8 *)metaData;
    metaAtomInfo.u32DataLen = META_ATOM_DATA_SIZE;
    CHK_FAILED_RETURN(SS_MP4_AddAtom(hMuxerHandle, "/moov/udta", &metaAtomInfo), 0, TD_FAILURE, "add meta failed.");

    OT_MP4_ATOM_INFO_S hdlrAtomInfo = {0};
    CHK_FAILED_RETURN(strcpy_s(hdlrAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "hdlr"), EOK, TD_FAILURE, "strcpy failed.");
    td_char hdlrData[HDLR_ATOM_DATA_SIZE] = {0};
    hdlrAtomInfo.pu8DataBuf = (td_u8 *)hdlrData;
    hdlrAtomInfo.u32DataLen = HDLR_ATOM_DATA_SIZE;
    CHK_FAILED_RETURN(SS_MP4_AddAtom(hMuxerHandle, "/moov/udta/meta", &hdlrAtomInfo), TD_SUCCESS, TD_FAILURE,
        "add hdlr atom failed.");

    OT_MP4_ATOM_INFO_S ilstAtomInfo = {0};
    CHK_FAILED_RETURN(strcpy_s(ilstAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "ilst"), EOK, TD_FAILURE, "strcpy failed.");
    CHK_FAILED_RETURN(SS_MP4_AddAtom(hMuxerHandle, "/moov/udta/meta", &ilstAtomInfo), TD_SUCCESS, TD_FAILURE,
        "add ilst atom failed.");
    return TD_SUCCESS;
}

td_s32 WriteMP4Gps(TD_MW_PTR hMuxerHandle, const td_u8 *frameAddr, int len)
{
    OT_MP4_ATOM_INFO_S udtaAtomInfo = {0};
    if (SS_MP4_GetAtom(hMuxerHandle, "/moov/udta/meta/ilst/gps ", &udtaAtomInfo) == OT_ERR_MP4_ATOM_LEN) {
        MEDIA_HAL_LOGI(MODULE_NAME, "This file already has a front cover!");
        return 0;
    }
    if (SS_MP4_GetAtom(hMuxerHandle, "/moov/udta/meta/ilst", &udtaAtomInfo) == OT_ERR_MP4_ATOM_LEN) {
        MEDIA_HAL_LOGI(MODULE_NAME, "This file already has a ilst Atom!");
        return WriteMP4GpsData(hMuxerHandle, frameAddr, len);
    }

    CHK_FAILED_RETURN(strcpy_s(udtaAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "udta"), EOK, TD_FAILURE, "strcpy failed.");
    CHK_FAILED_RETURN(SS_MP4_AddAtom(hMuxerHandle, "/moov", &udtaAtomInfo), TD_SUCCESS, TD_FAILURE, "add udta failed.");
    td_s32 ret = CreatUdtaBox(hMuxerHandle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return WriteMP4GpsData(hMuxerHandle, frameAddr, len);
}

static td_s32 WriteMP4CoverData(TD_MW_PTR hMuxerHandle, const td_u8 *frameAddr, int len)
{
    OT_MP4_ATOM_INFO_S covrAtomInfo = {0};
    CHK_FAILED_RETURN(strcpy_s(covrAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "covr"), EOK, TD_FAILURE, "strcpy failed.");
    CHK_FAILED_RETURN(SS_MP4_AddAtom(hMuxerHandle, "/moov/udta/meta/ilst", &covrAtomInfo), TD_SUCCESS, TD_FAILURE,
        "add covr atom failed.");

/*
 * data atom in covr:
 * 4 bytes : len of covr atom data. (4 bytes of len + 4 bytes "data" + 1 bytes of version + 3 bytes of flags
 *            + 4 bytes reserved2 + len for picture-data.)
 * 4 bytes : "data".
 * 1 bytes : version.
 * 3 bytes : flags. 00000D:mjpg, 00000E:PNG, 00001B:BMP.
 * 4 bytes : reserved.
 * x bytes : picture-data.
 */
    OT_MP4_ATOM_INFO_S dataAtomInfo = {0};
    CHK_FAILED_RETURN(strcpy_s(dataAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "data"), EOK, TD_FAILURE, "strcpy failed.");

    td_u32 dataAtomDataLen = len + 1 + 3 + 4;
    td_u8* dataAtomData = (td_u8*)malloc(dataAtomDataLen);
    CHK_NULL_RETURN(dataAtomData, TD_FAILURE, "malloc covr atom buffer failed.");
    if (memset_s(dataAtomData, dataAtomDataLen, 0, dataAtomDataLen) != EOK) {
        goto FAILED_COVR;
    }
    dataAtomData[3] = 0x0D; // 03 : set flag to 00000D.
    // 8 : offset to write picture data.
    if (memcpy_s(dataAtomData + 8, dataAtomDataLen - 8, frameAddr, len) != EOK) {
        goto FAILED_COVR;
    }
    dataAtomInfo.pu8DataBuf = dataAtomData;
    dataAtomInfo.u32DataLen = dataAtomDataLen;

    td_s32 ret = SS_MP4_AddAtom(hMuxerHandle, "/moov/udta/meta/ilst/covr", &dataAtomInfo);
    free(dataAtomData);
    return ret;

FAILED_COVR:
    free(dataAtomData);
    return TD_FAILURE;
}

td_s32 WriteMP4FrontCover(TD_MW_PTR hMuxerHandle, const td_u8 *frameAddr, int len)
{
    OT_MP4_ATOM_INFO_S udtaAtomInfo = {0};
    if (SS_MP4_GetAtom(hMuxerHandle, "/moov/udta/meta/ilst/covr", &udtaAtomInfo) == OT_ERR_MP4_ATOM_LEN) {
        MEDIA_HAL_LOGI(MODULE_NAME, "This file already has a front cover!");
        return 0;
    }
    if (SS_MP4_GetAtom(hMuxerHandle, "/moov/udta/meta/ilst", &udtaAtomInfo) == OT_ERR_MP4_ATOM_LEN) {
        MEDIA_HAL_LOGI(MODULE_NAME, "This file already has a ilst Atom!");
        return WriteMP4CoverData(hMuxerHandle, frameAddr, len);
    }

    CHK_FAILED_RETURN(strcpy_s(udtaAtomInfo.aszType, OT_MP4_TYPE_SIZE + 1, "udta"), EOK, TD_FAILURE, "strcpy failed.");
    CHK_FAILED_RETURN(SS_MP4_AddAtom(hMuxerHandle, "/moov", &udtaAtomInfo), TD_SUCCESS, TD_FAILURE, "add udta failed.");
    td_s32 ret = CreatUdtaBox(hMuxerHandle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return WriteMP4CoverData(hMuxerHandle, frameAddr, len);
}
