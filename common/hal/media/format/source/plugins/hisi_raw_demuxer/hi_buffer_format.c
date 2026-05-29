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
 * Description: hidemuxer module header file
 */

#include "hi_buffer_format.h"

#include <errno.h>
#include <unistd.h>
#include "securec.h"
#include "demuxer_common_err.h"
#include "demuxer_interface.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MODULE_NAME "DemuxerRaw"
#define HI_FILE_PATH_LEN_MAX  255
#define PROBE_SIZE_MIN  (1 * 1024) /* 2KB */
#define SLEEP_TIME 10 /* 10us */
#define PROBE_WAIT_ENOUGH_DATA_TIME 4000000 /* 4s */
#define PROBE_WAIT_ENOUGH_DATA_CYCLE (PROBE_WAIT_ENOUGH_DATA_TIME / SLEEP_TIME)
#define MAX_READ_LEN_PER_PACKET 4096
#define SAMPLES_PER_BLOCK 1024
#define SCORE_FULL 100
#define RAW_FORMAT_PROPERTY 100
#define BITS_PER_BYTE 8
#define MASK_BITS_2 0x03
#define MASK_BITS_3 0x07
#define MASK_BITS_4 0x0f
#define MASK_BITS_7 0x7f
#define ID3V2_MAGIC 0x00494433
#define ID3V2_INVALID_VERSION 0xff
#define ID3V2_HEADER_LEN 10
#define AAC_FRAME_LEN_MAX 32768 /* 2048 * 2 * 8 */

typedef struct {
    OT_FORMAT_FILE_INFO_S fileInfo;
    FormatMediaParam selectInfo;
    DemuxerBufferStream strem;
    bool eof;
    int32_t readLen;
    unsigned char *probeCacheData;
    int32_t probeCacheDatalen;
    int32_t probeDataReadoffset;
    DataType flags;
    int32_t prepared;
} BufferFormatMember;

#define AAC_ADTS_HEADER_SIZE 7
const int32_t g_mpeg4audioSampleRates[16] = {
    96000, 88200, 64000, 48000, 44100, 32000,
    24000, 22050, 16000, 12000, 11025, 8000, 7350, 0, 0, 0
};

static int32_t FillMediaInfo(OT_FORMAT_FILE_INFO_S *fileInfo, int32_t bitrate, int32_t profile, int32_t sampleRate,
    int32_t channel)
{
    (void)profile;
    fileInfo->s64FileSize = -1;
    fileInfo->s64StartTime = 0;
    fileInfo->s64Duration = -1;
    fileInfo->s32UsedVideoStreamIndex = -1;
    fileInfo->fFrameRate = 0;
    fileInfo->u32Bitrate = bitrate;
    fileInfo->u32AudioChannelCnt = channel;
    fileInfo->u32SampleRate = sampleRate;
    fileInfo->s32UsedAudioStreamIndex = 0;
    fileInfo->enVideoType = OT_FORMAT_VIDEO_TYPE_BUTT;
    fileInfo->enAudioType = OT_FORMAT_AUDIO_TYPE_AAC;

    return MEDIA_HAL_OK;
}


/*************************************************************************************************
* | sync header |id | layer  | protection_absent |profile_objecttype | sample_frequency_index|
*   12                  1      2                  1                           2                            4
* __________________________________________________________________________________________________
* |private_bit | channel_configuration | original/copy | home | copyright_identification_bit |
*           1                3                                 1                1                  1
* _________________________________________________________________________________________________
* | copyright_identification_start | aac_frame_length | adts_buffer_fullness | number_of_raw_data_blocks_in_frame |
*            1                                       13                             11                            2
***************************************************************************************************/
static int32_t AacParseHeader(const uint8_t *data, int32_t dataLen, OT_FORMAT_FILE_INFO_S *fileInfo)
{
    MEDIA_HAL_UNUSED(dataLen);
    int32_t rdb;
    int32_t sr;
    int32_t aot;

    if (((data[0] << 0x8) | (data[1] & 0xf0)) != 0xfff0) { /* sync header */
        return -1;
    }
    aot = ((data[0x2] >> 0x6) & MASK_BITS_2);  /* profile_objecttype */
    sr = ((data[0x2] >> 0x2) & MASK_BITS_4);  /* sample_frequency_index */

    if (g_mpeg4audioSampleRates[sr] == 0) {
        return -1;
    }

    int32_t ch = (((data[0x2] & 0x1) << 0x2) | ((data[0x3] >> 0x6) & MASK_BITS_2)); /* channel_configuration */
    /* aac_frame_length */
    int32_t size = (((data[0x3] & MASK_BITS_2) << 0x11) | (data[0x4] << 0x3) | ((data[0x5] >> 0x5) & MASK_BITS_3));
    if (size < AAC_ADTS_HEADER_SIZE || size > AAC_FRAME_LEN_MAX) {
        return -1;
    }
    if (ch <= 0) {
        return -1;
    }
    rdb = (data[0x6] & MASK_BITS_2);      /* number_of_raw_data_blocks_in_frame */

    int32_t blockNum = rdb + 1;
    int32_t sampleRate = g_mpeg4audioSampleRates[sr];
    int32_t samples = blockNum * SAMPLES_PER_BLOCK;
    float bitRate = ((float)size * BITS_PER_BYTE * sampleRate) / (float)samples;
    int32_t tmp = bitRate;
    if (FillMediaInfo(fileInfo, tmp, aot + 1, sampleRate, ch) != 0) {
        return -1;
    }

    return size;
}

static void ParseId3v2TagAndSkip(BufferFormatMember *member, const uint8_t *data, int32_t dataLen)
{
    if (dataLen < ID3V2_HEADER_LEN) {
        MEDIA_HAL_LOGI(MODULE_NAME, "dataLen:%d", dataLen);
        return;
    }
    uint32_t identifier = ((data[0] << 0x10) | (data[0x1] << 0x8) | data[0x2]); /* ID3 tree bytes */
    if (identifier != ID3V2_MAGIC) {
        return;
    }
    /* the version and sub-version can not be 0xff */
    if (data[0x3] == ID3V2_INVALID_VERSION || data[0x4] == ID3V2_INVALID_VERSION) {
        return;
    }
    /* flag 1 byte, skip */
    /* header length, 4 bytes. only use low 7bits per byte */
    if ((data[0x6] & 0x80) != 0 || (data[0x7] & 0x80) != 0 || (data[0x8] & 0x80) != 0 || (data[0x9] & 0x80) != 0) {
        return;
    }
    uint32_t len = (((data[0x6] & MASK_BITS_7) << 0x15) | ((data[0x7] & MASK_BITS_7) << 0x0e) |
        ((data[0x8] & MASK_BITS_7) << 0x07) | (data[0x9] & MASK_BITS_7));

    member->probeDataReadoffset = len + ID3V2_HEADER_LEN;
    member->readLen = len + ID3V2_HEADER_LEN;
    MEDIA_HAL_LOGI(MODULE_NAME, "have id3v2 header, skip len:%x", len + ID3V2_HEADER_LEN);
}

static int32_t AacAdtsParse(BufferFormatMember *member, const uint8_t *data, int32_t datalen)
{
    int32_t ret = -1;
    int32_t reserveLen = datalen;
    const uint8_t *start = data;
    CHK_NULL_RETURN(member, -1, "input param hanle null");

    ParseId3v2TagAndSkip(member, data, datalen);
    start += member->probeDataReadoffset; /* offset the id3v2 info, if need */
    reserveLen -= member->readLen;
    while ((start != NULL) && reserveLen >= AAC_ADTS_HEADER_SIZE) {
        if (AacParseHeader(start, reserveLen, &member->fileInfo) > 0) {
            ret = 0;
            break;
        }
        start += 1;
        reserveLen -= 1;
    };
    return ret;
}

/* fileName is the buffer address */
static int32_t BufferRawOpen(void **fmtHandle, const DemuxerSource *source)
{
    CHK_NULL_RETURN(fmtHandle, OT_ERR_DEMUXER_NULL_PTR, "input param hanle null");
    CHK_NULL_RETURN(source, OT_ERR_DEMUXER_NULL_PTR, "input param hanle null");
    CHK_FAILED_RETURN(source->type, DEMUXER_SOURCE_TYPE_STREAM, -1, "only support stream type");

    if (source->stream.ReadData == NULL || source->stream.GetReadableSize == NULL || source->stream.handle == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "stream param error");
        return MEDIA_HAL_ERR;
    }

    BufferFormatMember *member = (BufferFormatMember *)malloc(sizeof(BufferFormatMember));
    CHK_NULL_RETURN(member, OT_ERR_DEMUXER_MEM_MALLOC, "malloc BufferFormatMember failed");
    if (memset_s(member, sizeof(BufferFormatMember), 0, sizeof(BufferFormatMember)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        goto FREE;
    }
    member->strem = source->stream;
    member->eof = false;
    member->readLen = 0;
    member->probeCacheData = NULL;
    member->probeCacheDatalen = 0;
    member->probeDataReadoffset = 0;
    member->selectInfo.vidStreamId = HI_DEMUXER_NO_MEDIA_STREAM;
    member->selectInfo.audStreamId = HI_DEMUXER_NO_MEDIA_STREAM;
    member->flags = DATA_TYPE_NONE;
    member->prepared = false;
    *fmtHandle = (void*)member;

    MEDIA_HAL_LOGE(MODULE_NAME, "buffer Demuxer open");
    return MEDIA_HAL_OK;
FREE:
    free(member);
    member = NULL;
    return MEDIA_HAL_ERR;
}

static int32_t ReadFromCache(BufferFormatMember *member, OT_FORMAT_PACKET_S *fmtFrame)
{
    int32_t readLen = MIN(MAX_READ_LEN_PER_PACKET, member->probeCacheDatalen - member->probeDataReadoffset);
    if (readLen <= 0) {
        return MEDIA_HAL_ERR;
    }

    int32_t bufferLen = readLen;
    /* free at BufferRawFree */
    fmtFrame->pu8Data = (uint8_t*)malloc(bufferLen);
    if (fmtFrame->pu8Data == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc buffer failed");
        return MEDIA_HAL_ERR;
    }

    if (memcpy_s(fmtFrame->pu8Data, bufferLen, member->probeCacheData + member->probeDataReadoffset, readLen) != EOK) {
        free(fmtFrame->pu8Data);
        fmtFrame->pu8Data = NULL;
        return MEDIA_HAL_ERR;
    }

    member->probeDataReadoffset += readLen;
    if (member->flags == DATA_TYPE_EOS && member->probeDataReadoffset == member->probeCacheDatalen) {
        member->eof = true;
    }

    fmtFrame->u32Size = readLen;
    fmtFrame->s64Pos = member->readLen;
    member->readLen += readLen;
    return MEDIA_HAL_OK;
}

static int32_t ReadFromSource(BufferFormatMember *member, OT_FORMAT_PACKET_S *fmtFrame)
{
    int32_t ret = MEDIA_HAL_OK;
    DataType flags = DATA_TYPE_NONE;
    DemuxerBufferStream *strem = &member->strem;
    int32_t len = strem->GetReadableSize(strem->handle);
    if (len <= 0) {
        return HI_RET_NODATA;
    }

    len = MIN(MAX_READ_LEN_PER_PACKET, len);
    /* free at BufferRawFree */
    fmtFrame->pu8Data = (uint8_t*)malloc(len);
    if (fmtFrame->pu8Data == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc buffer failed");
        return MEDIA_HAL_NO_MEM;
    }

    int32_t readLen = strem->ReadData(strem->handle, fmtFrame->pu8Data, len, 0, &flags);
    if (readLen == 0 && flags != DATA_TYPE_EOS) {
        ret = MEDIA_HAL_ERR;
        goto END;
    }

    if (flags == DATA_TYPE_EOS) {
        member->eof = true;
    }
    if ((readLen == 0) && flags == DATA_TYPE_EOS) {
        ret = HI_RET_FILE_EOF;
    }

END:
    if (readLen == 0) {
        free(fmtFrame->pu8Data);
        fmtFrame->pu8Data = NULL;
    }
    fmtFrame->u32Size = readLen;
    fmtFrame->s64Pos = member->readLen;
    member->readLen += readLen;
    return ret;
}

static int32_t BufferRawRead(const void *fmtHandle, OT_FORMAT_PACKET_S *fmtFrame)
{
    BufferFormatMember *member = (BufferFormatMember *)fmtHandle;
    CHK_NULL_RETURN(member, OT_ERR_DEMUXER_INVALID_HANDLE, "input param hanle null");
    CHK_NULL_RETURN(fmtFrame, OT_ERR_DEMUXER_ILLEGAL_PARAM, "input param fmtFrame null");
    CHK_FAILED_RETURN(member->prepared, true, -1, "not prepared");

    if (member->eof == true) {
        fmtFrame->pu8Data = NULL;
        fmtFrame->u32Size = 0;
        return HI_RET_FILE_EOF;
    }

    if (member->probeDataReadoffset < member->probeCacheDatalen) {
        if (ReadFromCache(member, fmtFrame) != MEDIA_HAL_OK) {
            fmtFrame->u32Size = 0;
            return MEDIA_HAL_ERR;
        }
    } else {
        int32_t ret = ReadFromSource(member, fmtFrame);
        CHK_FAILED_RETURN_NO_LOG(ret, 0, ret);
    }

    fmtFrame->packetType = PACKET_TYPE_AUDIO;
    fmtFrame->u32StreamIndex = 0;
    fmtFrame->bKeyFrame = true;
    fmtFrame->s64Pts = -1;
    fmtFrame->s64Duration = 0;

    return MEDIA_HAL_OK;
}

static int32_t BufferRawClose(const void *fmtHandle)
{
    CHK_NULL_RETURN(fmtHandle, OT_ERR_DEMUXER_INVALID_HANDLE, "input param hanle null");
    BufferFormatMember *member = (BufferFormatMember *)fmtHandle;

    if (member->probeCacheData != NULL) {
        free(member->probeCacheData);
        member->probeCacheData = NULL;
    }

    free(member);
    member = NULL;
    return MEDIA_HAL_OK;
}

static int32_t BufferRawFree(const void *fmtHandle, const OT_FORMAT_PACKET_S *fmtFrame)
{
    BufferFormatMember *member = (BufferFormatMember *)fmtHandle;
    CHK_NULL_RETURN(member, OT_ERR_DEMUXER_INVALID_HANDLE, "input param hanle null");
    CHK_NULL_RETURN(fmtFrame, OT_ERR_DEMUXER_ILLEGAL_PARAM, "input param fmtFrame null");
    CHK_FAILED_RETURN(member->prepared, true, -1, "not prepared");

    if (fmtFrame->pu8Data != NULL) {
        free(fmtFrame->pu8Data);
    }

    return MEDIA_HAL_OK;
}

static int32_t BufferRawSeek(__attribute__((unused)) const TD_VOID *pFmt, __attribute__((unused)) TD_S32 s32StreamIndex,
    __attribute__((unused)) TD_S64 s64MSec, __attribute__((unused)) TD_S32 s32Flag)
{
    MEDIA_HAL_LOGE(MODULE_NAME, "not support");
    return MEDIA_HAL_ERR;
}

static int32_t BufferRawProbe(const void *fmtHandle, OT_DEMUXER_PROBE_MODE_TYPE_E arg)
{
    MEDIA_HAL_UNUSED(arg);
    BufferFormatMember *member = (BufferFormatMember *)fmtHandle;
    CHK_NULL_RETURN(member, OT_ERR_DEMUXER_INVALID_HANDLE, "input param hanle null");
    CHK_NULL_RETURN(member->strem.handle, -1, "input param hanle null");
    CHK_FAILED_RETURN(member->prepared, false, 0, "have prepared");
    /* get data first */
    int32_t bufferLen;
    unsigned char *data = NULL;
    int32_t readLen;
    int32_t totalReadLen = 0;
    int32_t waitCnt = 0;
    int32_t ret = -1;
    DataType flags;

    /* check have enough data to probe */
    bufferLen = member->strem.GetReadableSize(member->strem.handle);
    while (bufferLen < PROBE_SIZE_MIN) {
        if (waitCnt > PROBE_WAIT_ENOUGH_DATA_CYCLE) {
            MEDIA_HAL_LOGE(MODULE_NAME, "GetEnough data failed");
            break;
        }
        usleep(SLEEP_TIME);
        waitCnt++;
        bufferLen = member->strem.GetReadableSize(member->strem.handle);
    }
    if (bufferLen == 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Get data timeout, probe failed");
        return MEDIA_HAL_ERR;
    }
    /* free at BufferRawClose */
    data = (unsigned char*)malloc(PROBE_SIZE_MIN + 1);
    CHK_NULL_RETURN(data, -1, "malloc buffer failed");
    /* read enough data to probe process */
    while (totalReadLen < PROBE_SIZE_MIN) {
        bufferLen = PROBE_SIZE_MIN - totalReadLen;
        readLen = member->strem.ReadData(member->strem.handle, data + totalReadLen, bufferLen, 0, &flags);
        if (readLen <= 0) {
            break;
        }
        totalReadLen += readLen;
    }

    if (AacAdtsParse(member, data, totalReadLen) == 0) {
        member->prepared = true;
        ret = 0;
    } else {
        free(data);
        data = NULL;
    }
    member->probeCacheData = data;
    member->probeCacheDatalen = totalReadLen;
    MEDIA_HAL_LOGI(MODULE_NAME, "get out fmt_probe, ret:%d", ret);
    return ret;
}

static int32_t BufferRawGetInfo(const void *fmtHandle, OT_FORMAT_FILE_INFO_S *fmtInfo)
{
    BufferFormatMember *member = (BufferFormatMember *)fmtHandle;

    CHK_NULL_RETURN(member, OT_ERR_DEMUXER_INVALID_HANDLE, "input param hanle null");
    CHK_NULL_RETURN(fmtInfo, OT_ERR_DEMUXER_ILLEGAL_PARAM, "input param fmtInfo null");
    CHK_FAILED_RETURN(member->prepared, true, -1, "not prepared");

    *fmtInfo = member->fileInfo;
    return MEDIA_HAL_OK;
}

int32_t BufferRawSetAttr(const void *fmtHandle, OT_FORMAT_MEDIA_ATTR_S *fmtMediaInfo)
{
    CHK_NULL_RETURN(fmtHandle, OT_ERR_DEMUXER_INVALID_HANDLE, "input param hanle null");
    CHK_NULL_RETURN(fmtMediaInfo, OT_ERR_DEMUXER_ILLEGAL_PARAM, "input param fmtMediaInfo null");
    BufferFormatMember *member = (BufferFormatMember *)fmtHandle;
    if (member->selectInfo.vidStreamId == HI_DEMUXER_NO_MEDIA_STREAM) {
        MEDIA_HAL_LOGE(MODULE_NAME, "no video stream");
        return MEDIA_HAL_OK;
    }

    if (fmtMediaInfo->s32VidStreamId >= HI_DEMUXER_RESOLUTION_CNT || fmtMediaInfo->s32VidStreamId < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "HI_MPF_SetAttr streamid invalid %d without [0,%d)",
            fmtMediaInfo->s32VidStreamId, HI_DEMUXER_RESOLUTION_CNT);
        return MEDIA_HAL_ERR;
    }

    member->selectInfo.vidStreamId = fmtMediaInfo->s32VidStreamId;
    return MEDIA_HAL_OK;
}

static int32_t BufferRawFind(const DemuxerSource *source)
{
    if (source == NULL) {
        return OT_ERR_DEMUXER_ILLEGAL_PARAM;
    }
    if (source->type != DEMUXER_SOURCE_TYPE_STREAM) {
        MEDIA_HAL_LOGI(MODULE_NAME, "only support stream source type");
        return OT_ERR_DEMUXER_NOT_SUPPORT;
    }

    return MEDIA_HAL_OK;
}

__attribute__((visibility("default"))) OT_DEMUX_S g_stFormatEntry = { "raw_demuxer", "aac",
    {
        BufferRawFind, BufferRawOpen, BufferRawClose, BufferRawRead, BufferRawFree, BufferRawSeek,
        BufferRawProbe, BufferRawGetInfo, BufferRawSetAttr
    },
    4
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
