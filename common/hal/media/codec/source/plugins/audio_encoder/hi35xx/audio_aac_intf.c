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

#include "audio_aac_intf.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ot_aacdec.h>
#include <ot_aacenc.h>
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MODULE_NAME "AAC_ALG"

#define HI_AUDIO_ASSERT(x) {if (TD_TRUE != (x))return -1;}

#define MW_AAC_USE_STATIC_MODULE_REGISTER
#define AAC_ENC_LIB_NAME "libaac_enc.so"
#define AAC_DEC_LIB_NAME "libaac_dec.so"

#define BITNUM_ONE_BYTE 8
#define AAC_ENC_TRANSPORT_TYPE_NUM 5
#define AAC_ENC_SUPPORT_SAMPLERATE_NUM 7
#define AAC_ENC_SUPPORT_BITRATE_NUM 7

/* # aac enc lib */
typedef td_s32 (*pHI_AACENC_GetVersion_Callback)(ot_aacenc_version* pVersion);
typedef td_s32 (*pAACInitDefaultConfig_Callback)(ot_aacenc_config* pstConfig);
typedef td_s32 (*pAACEncoderOpen_Callback)(ot_aac_encoder** phAacPlusEnc, ot_aacenc_config* pstConfig);
typedef td_s32 (*pAACEncoderFrame_Callback)(ot_aac_encoder* hAacPlusEnc,
    td_s16* ps16PcmBuf, td_u8* pu8Outbuf, td_s32* ps32NumOutBytes);
typedef void (*pAACEncoderClose_Callback)(ot_aac_encoder* hAacPlusEnc);

/* # aac dec lib */
typedef td_s32 (*pHI_OT_AACDEC_GetVersion_Callback)(ot_aacdec_version* pVersion);
typedef ot_aac_decoder (*pAACInitDecoder_Callback)(ot_aacdec_transport_type enTranType);
typedef void (*pAACFreeDecoder_Callback)(ot_aac_decoder hAACDecoder);
typedef td_s32 (*pAACSetRawMode_Callback)(ot_aac_decoder hAACDecoder, td_s32 nChans, td_s32 sampRate);
typedef td_s32 (*pAACDecodeFindSyncHeader_Callback)(ot_aac_decoder hAACDecoder, td_u8** ppInbufPtr, td_s32* pBytesLeft);
typedef td_s32 (*pAACDecodeFrame_Callback)(ot_aac_decoder hAACDecoder,
    td_u8** ppInbufPtr, td_s32* pBytesLeft, td_s16* pOutPcm);
typedef td_s32 (*pAACGetLastFrameInfo_Callback)(ot_aac_decoder hAACDecoder, ot_aacdec_frame_info* aacFrameInfo);
typedef td_s32 (*pAACDecoderSetEosFlag_Callback)(ot_aac_decoder hAACDecoder, td_s32 s32Eosflag);
typedef td_s32 (*pAACFlushCodec_Callback)(ot_aac_decoder hAACDecoder);

typedef struct {
    td_s32 s32OpenCnt;
    void *pLibHandle;

    pHI_AACENC_GetVersion_Callback pHI_AACENC_GetVersion;
    pAACInitDefaultConfig_Callback pAACInitDefaultConfig;
    pAACEncoderOpen_Callback pAACEncoderOpen;
    pAACEncoderFrame_Callback pAACEncoderFrame;
    pAACEncoderClose_Callback pAACEncoderClose;
}AacEncFun;

static td_s32 g_AacEncHandle = 0;

static AacEncFun g_stAacEncFunc = { 0 };

typedef struct {
    ot_aacenc_format coder_format;
    td_s32 supportSampleRate[AAC_ENC_SUPPORT_SAMPLERATE_NUM];
    td_s32 minBitRateMonoChannle[AAC_ENC_SUPPORT_BITRATE_NUM];
    td_s32 minBitRateStereoChannle[AAC_ENC_SUPPORT_BITRATE_NUM];
    td_s32 maxBitRateMonoChannle[AAC_ENC_SUPPORT_BITRATE_NUM];
    td_s32 maxBitRateStereoChannle[AAC_ENC_SUPPORT_BITRATE_NUM];
    td_s32 RecommendBitRateMonoChannle[AAC_ENC_SUPPORT_BITRATE_NUM];
    td_s32 RecommendBitRateStereoChannle[AAC_ENC_SUPPORT_BITRATE_NUM];
}AacConfig;

static const AacConfig g_aacConfigTable[] = {
    {
        OT_AACLC,
        {32000, 44100, 48000, 16000, 8000, 24000, 22050},
        {32000, 48000, 48000, 24000, 16000, 32000, 32000},
        {32000, 48000, 48000, 24000, 16000, 32000, 32000},
        {192000, 265000, 288000, 96000, 48000, 144000, 132000},
        {320000, 320000, 320000, 192000, 96000, 288000, 265000},
        {48000, 64000, 64000, 48000, 24000, 48000, 64000},
        {128000, 128000, 128000, 48000, 32000, 48000, 48000}
    },
    {
        OT_EAAC,
        {32000, 44100, 48000, 16000, 22050, 24000, 0},
        {32000, 32000, 32000, 24000, 32000, 32000, 0},
        {32000, 32000, 32000, 24000, 32000, 32000, 0},
        {64000, 64000, 64000, 48000, 64000, 64000, 0},
        {128000, 128000, 128000, 96000, 128000, 128000, 0},
        {48000, 48000, 48000, 32000, 48000, 48000, 0},
        {64000, 64000, 64000, 32000, 64000, 64000, 0}
    },
    {
        OT_EAACPLUS,
        {32000, 44100, 48000, 16000, 22050, 24000, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {16000, 16000, 16000, 16000, 16000, 16000, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {64000, 64000, 64000, 48000, 64000, 64000, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {32000, 48000, 48000, 32000, 32000, 32000, 0}
    },
    {
        OT_AACLD,
        {32000, 44100, 48000, 16000, 8000, 24000, 22050},
        {48000, 64000, 64000, 24000, 16000, 32000, 32000},
        {64000, 44000, 64000, 32000, 16000, 48000, 48000},
        {320000, 320000, 320000, 192000, 96000, 256000, 256000},
        {320000, 320000, 320000, 320000, 192000, 320000, 320000},
        {64000, 128000, 128000, 48000, 24000, 64000, 48000},
        {128000, 256000, 256000, 96000, 48000, 128000, 96000}
    },
    {
        OT_AACELD,
        {32000, 44100, 48000, 16000, 8000, 24000, 22050},
        {48000, 64000, 64000, 24000, 16000, 32000, 32000},
        {64000, 44000, 64000, 32000, 16000, 48000, 48000},
        {320000, 320000, 320000, 192000, 96000, 256000, 256000},
        {320000, 320000, 320000, 320000, 192000, 320000, 320000},
        {64000, 128000, 128000, 48000, 24000, 64000, 48000},
        {128000, 256000, 256000, 96000, 48000, 128000, 96000}
    }
};

#ifndef MW_AAC_USE_STATIC_MODULE_REGISTER
static td_s32 AacAencLibInitByDynamicSo(AacEncFun *stAacEncFunc)
{
    stAacEncFunc->pLibHandle = MediaHalDLOpen(AAC_ENC_LIB_NAME);
    if (stAacEncFunc->pLibHandle != NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "load aenc lib fail!");
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    stAacEncFunc->pHI_AACENC_GetVersion = (pHI_AACENC_GetVersion_Callback)MediaHalDLSym(
        stAacEncFunc->pLibHandle, "ot_aacenc_get_version");
    if (stAacEncFunc->pHI_AACENC_GetVersion != NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "find symbol error!");
        MediaHalDLClose(stAacEncFunc->pLibHandle);
        stAacEncFunc->pLibHandle = NULL;
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    stAacEncFunc->pAACInitDefaultConfig = (pAACInitDefaultConfig_Callback)MediaHalDLSym(
        stAacEncFunc->pLibHandle, "ot_aacenc_init_default_config");
    if (stAacEncFunc->pAACInitDefaultConfig != NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "find symbol error!");
        MediaHalDLClose(stAacEncFunc->pLibHandle);
        stAacEncFunc->pLibHandle = NULL;
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    stAacEncFunc->pAACEncoderOpen = (pAACEncoderOpen_Callback)MediaHalDLSym(
        stAacEncFunc->pLibHandle, "ot_aacenc_open");
    if (stAacEncFunc->pAACEncoderOpen != NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "find symbol error!");
        MediaHalDLClose(stAacEncFunc->pLibHandle);
        stAacEncFunc->pLibHandle = NULL;
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    stAacEncFunc->pAACEncoderFrame = (pAACEncoderFrame_Callback)MediaHalDLSym(
        stAacEncFunc->pLibHandle, "ot_aacenc_frame");
    if (stAacEncFunc->pAACEncoderFrame != NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "find symbol error!");
        MediaHalDLClose(stAacEncFunc->pLibHandle);
        stAacEncFunc->pLibHandle = NULL;
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    stAacEncFunc->pAACEncoderClose = (pAACEncoderClose_Callback)MediaHalDLSym(
        stAacEncFunc->pLibHandle, "ot_aacenc_close");
    if (stAacEncFunc->pAACEncoderClose != NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, " [Info]:%s", "find symbol error!");
        MediaHalDLClose(stAacEncFunc->pLibHandle);
        stAacEncFunc->pLibHandle = NULL;
        return OT_ERR_AENC_NOT_SUPPORT;
    }
    return TD_SUCCESS;
}
#endif

static td_s32 InitAacAencLib(void)
{
    if (g_stAacEncFunc.s32OpenCnt == 0) {
        AacEncFun stAacEncFunc;
        if (memset_s(&stAacEncFunc, sizeof(AacEncFun), 0, sizeof(AacEncFun)) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        }
#ifdef MW_AAC_USE_STATIC_MODULE_REGISTER
        stAacEncFunc.pHI_AACENC_GetVersion = ot_aacenc_get_version;
        stAacEncFunc.pAACInitDefaultConfig = ot_aacenc_init_default_config;
        stAacEncFunc.pAACEncoderOpen = (pAACEncoderOpen_Callback)ot_aacenc_open;
        stAacEncFunc.pAACEncoderFrame = ot_aacenc_frame;
        stAacEncFunc.pAACEncoderClose = ot_aacenc_close;
#else /* when load so go this case */
        td_s32 ret = AacAencLibInitByDynamicSo(&stAacEncFunc);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "AacAencLibInitByDynamicSo error!");
            return ret;
        }
#endif
        if (memcpy_s(&g_stAacEncFunc, sizeof(AacEncFun),
            &stAacEncFunc, sizeof(AacEncFun)) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "copy fail");
            return TD_FAILURE;
        }
    }
    g_stAacEncFunc.s32OpenCnt++;
    return TD_SUCCESS;
}

static void DeInitAacAencLib(void)
{
    if (g_stAacEncFunc.s32OpenCnt != 0) {
        g_stAacEncFunc.s32OpenCnt--;
    }

    if (g_stAacEncFunc.s32OpenCnt == 0) {
#ifndef MW_AAC_USE_STATIC_MODULE_REGISTER
        if (g_stAacEncFunc.pLibHandle != TD_NULL) {
            Audio_Dlclose(g_stAacEncFunc.pLibHandle);
        }
#endif
        if (memset_s(&g_stAacEncFunc, sizeof(AacEncFun), 0, sizeof(AacEncFun)) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        }
    }

    return;
}

static td_s32 AACInitDefaultConfigAdp(ot_aacenc_config* pstConfig)
{
    if (g_stAacEncFunc.pAACInitDefaultConfig == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "call aac function fail!\n");
        return OT_ERR_AENC_NOT_SUPPORT;
    }
    return g_stAacEncFunc.pAACInitDefaultConfig(pstConfig);
}

static td_s32 AACEncoderOpenAdp(ot_aac_encoder** phAacPlusEnc, ot_aacenc_config* pstConfig)
{
    if (g_stAacEncFunc.pAACEncoderOpen == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "call aac function fail!");
        return OT_ERR_AENC_NOT_SUPPORT;
    }
    return g_stAacEncFunc.pAACEncoderOpen(phAacPlusEnc, pstConfig);
}

static td_s32 AACEncoderFrameAdp(ot_aac_encoder* hAacPlusEnc, td_s16* ps16PcmBuf, td_u32 dataLen,
    td_u8* pu8Outbuf, td_s32* ps32NumOutBytes)
{
    MEDIA_HAL_UNUSED(dataLen);
    if (g_stAacEncFunc.pAACEncoderFrame == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "call aac function fail!");
        return OT_ERR_AENC_NOT_SUPPORT;
    }
    return g_stAacEncFunc.pAACEncoderFrame(hAacPlusEnc, ps16PcmBuf, pu8Outbuf, ps32NumOutBytes);
}

static void AACEncoderCloseAdp(ot_aac_encoder* hAacPlusEnc)
{
    if (g_stAacEncFunc.pAACEncoderClose == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "call aac function fail!");
        return;
    }
    return g_stAacEncFunc.pAACEncoderClose(hAacPlusEnc);
}

static td_s32 AencCheckAACAttr(const AencAacAttr *pstAACAttr)
{
    if (pstAACAttr == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "pstAACAttr is null");
        return OT_ERR_AENC_NULL_PTR;
    }
    if (pstAACAttr->enBitWidth != OT_AUDIO_BIT_WIDTH_16) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "invalid bitwidth for AAC encoder");
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pstAACAttr->enSoundMode >= OT_AUDIO_SOUND_MODE_BUTT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "invalid sound mode for AAC encoder");
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if ((pstAACAttr->enAACType == AAC_TYPE_EAACPLUS) && (pstAACAttr->enSoundMode != OT_AUDIO_SOUND_MODE_STEREO)) {
        MEDIA_HAL_LOGE(MODULE_NAME, " [Info]:%s", "invalid sound mode for AAC encoder");
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pstAACAttr->enTransType == AAC_TRANS_TYPE_ADTS) {
        if ((pstAACAttr->enAACType == AAC_TYPE_AACLD) ||
            (pstAACAttr->enAACType == AAC_TYPE_AACELD)) {
            MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "OT_AACLD or OT_AACELD not support AAC_TRANS_TYPE_ADTS");
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    }

    return TD_SUCCESS;
}

static td_s32 AencAACCheckCommonConfig(const ot_aacenc_config *pconfig)
{
    if (pconfig->coder_format != OT_AACLC &&
        pconfig->coder_format != OT_EAAC &&
        pconfig->coder_format != OT_EAACPLUS &&
        pconfig->coder_format != OT_AACLD &&
        pconfig->coder_format != OT_AACELD) {
        MEDIA_HAL_LOGE(MODULE_NAME, "aacenc coder_format(%d) invalid", pconfig->coder_format);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pconfig->quality != OT_AACENC_QUALITY_EXCELLENT &&
        pconfig->quality != OT_AACENC_QUALITY_HIGH &&
        pconfig->quality != OT_AACENC_QUALITY_MEDIUM &&
        pconfig->quality != OT_AACENC_QUALITY_LOW) {
        MEDIA_HAL_LOGE(MODULE_NAME, "aacenc quality(%d) invalid", pconfig->quality);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pconfig->bits_per_sample != 16) { /* 16: aacenc bits_per_sample */
        MEDIA_HAL_LOGE(MODULE_NAME, "aacenc bits_per_sample(%d) should be 16", pconfig->bits_per_sample);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pconfig->trans_type < 0 || pconfig->trans_type > 2) { /* 2: Defines AACENC container type */
        MEDIA_HAL_LOGE(MODULE_NAME, "invalid trans_type(%d), not in [0, 2]", pconfig->trans_type);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }
    /* 1000: Maximum Target Audio Bandwidth (Hz), 2: number of audio file sampling times */
    if (pconfig->band_width != 0 && (pconfig->band_width < 1000 || pconfig->band_width > pconfig->sample_rate / 2)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AAC band_width(%d) should be 0, or 1000 ~ %d",
            pconfig->band_width, pconfig->sample_rate / 2); /* 2: number of audio file sampling times */
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pconfig->num_channels_out != pconfig->num_channels_in) {
        MEDIA_HAL_LOGE(MODULE_NAME, "coder_format(%d) num_channels_out(%d) in not equal to num_channels_in(%d)",
            pconfig->coder_format, pconfig->num_channels_out, pconfig->num_channels_in);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pconfig->coder_format == OT_EAACPLUS) {
        if (pconfig->num_channels_out != 2 || pconfig->num_channels_in != 2) { /* 2: number of channels on in and out */
            MEDIA_HAL_LOGE(MODULE_NAME, "OT_EAACPLUS num_channels_out(%d) and num_channels_in(%d) should be 2",
                pconfig->num_channels_out, pconfig->num_channels_in);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    }

    return TD_SUCCESS;
}


td_s32 AencAACCheckSpecificConfig(const ot_aacenc_config *pconfig)
{
    td_s32 s32MinBitRate = 0;
    td_s32 s32MaxBitRate = 0;
    td_s32 s32RecommendBitRate = 0;
    bool isMatchFormat = false;
    bool isMatchSampleRate = false;

    for (int i = 0; i < AAC_ENC_TRANSPORT_TYPE_NUM; i++) {
        if (pconfig->coder_format != g_aacConfigTable[i].coder_format) {
            continue;
        }
        isMatchFormat = TD_TRUE;
        for (int j = 0; j < AAC_ENC_SUPPORT_SAMPLERATE_NUM; j++) {
            if (pconfig->sample_rate != g_aacConfigTable[i].supportSampleRate[j]) {
                continue;
            }
            isMatchSampleRate = TD_TRUE;
            if (pconfig->coder_format == OT_EAACPLUS) {
                s32MinBitRate = g_aacConfigTable[i].minBitRateStereoChannle[j];
                s32MaxBitRate = g_aacConfigTable[i].maxBitRateStereoChannle[j];
                s32RecommendBitRate = g_aacConfigTable[i].RecommendBitRateStereoChannle[j];
                break;
            }
            if (pconfig->num_channels_in == 1) {
                s32MinBitRate = g_aacConfigTable[i].minBitRateMonoChannle[j];
                s32MaxBitRate = g_aacConfigTable[i].maxBitRateMonoChannle[j];
                s32RecommendBitRate = g_aacConfigTable[i].RecommendBitRateMonoChannle[j];
            } else {
                s32MinBitRate = g_aacConfigTable[i].minBitRateStereoChannle[j];
                s32MaxBitRate = g_aacConfigTable[i].maxBitRateStereoChannle[j];
                s32RecommendBitRate = g_aacConfigTable[i].RecommendBitRateStereoChannle[j];
            }
            break;
        }
        break;
    }

    if (isMatchFormat != TD_TRUE || isMatchSampleRate != TD_TRUE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "format(%d) sample_rate(%d) not match, format match result %d, samplerate match "\
            "result %d", pconfig->coder_format, pconfig->sample_rate, isMatchFormat, isMatchSampleRate);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pconfig->bit_rate < s32MinBitRate ||  pconfig->bit_rate > s32MaxBitRate) {
        MEDIA_HAL_LOGE(MODULE_NAME, "coder_format(%d) sample_rate(%d) bit_rate(%d) should be %d ~ %d, recommend %d",
            pconfig->coder_format, pconfig->sample_rate, pconfig->bit_rate,
            s32MinBitRate, s32MaxBitRate, s32RecommendBitRate);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }
    return TD_SUCCESS;
}

static td_s32 AencAACCheckConfig(const ot_aacenc_config *pconfig)
{
    if (pconfig == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "pconfig is null");
        return OT_ERR_AENC_NULL_PTR;
    }

    td_s32 ret = AencAACCheckCommonConfig(pconfig);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AencAACCheckCommonConfig failed s32Ret = %d", ret);
        return ret;
    }

    ret = AencAACCheckSpecificConfig(pconfig);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AencAACCheckSpecificConfig failed ret = %d", ret);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 AencAACConfig(const AencAacAttr *pstAttr, ot_aacenc_config* pstConfig)
{
    /* set default config to encoder */
    td_s32 ret = AACInitDefaultConfigAdp(pstConfig);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "s32Ret:0x%x", ret);
        return ret;
    }

    pstConfig->coder_format   = (ot_aacenc_format)pstAttr->enAACType;
    pstConfig->bit_rate       = pstAttr->enBitRate;
    pstConfig->bits_per_sample = (td_s16)(BITNUM_ONE_BYTE * (1 + pstAttr->enBitWidth));
    pstConfig->sample_rate    = pstAttr->enSmpRate;
    pstConfig->band_width     = pstAttr->s16BandWidth; /* config.sample_rate/2; */
    pstConfig->trans_type     = (ot_aacenc_transport_type)pstAttr->enTransType;

    if (pstAttr->enSoundMode == OT_AUDIO_SOUND_MODE_MONO && pstAttr->enAACType != AAC_TYPE_EAACPLUS) {
        pstConfig->num_channels_in   = 1;
        pstConfig->num_channels_out  = 1;
    } else {
        pstConfig->num_channels_in   = 2; /* 2: number of channels on input */
        pstConfig->num_channels_out  = 2; /* 2: number of channels on output */
    }

    pstConfig->quality = OT_AACENC_QUALITY_HIGH;

    ret = AencAACCheckConfig(pstConfig);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ret:%d", ret);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 OpenAACEncoder(void *pEncoderAttr, void **ppEncoder)
{
    HI_AUDIO_ASSERT(pEncoderAttr != NULL);
    HI_AUDIO_ASSERT(ppEncoder != NULL);

    AencAacEncoder *pstEncoder = NULL;
    const AencAacAttr *pstAttr = NULL;
    ot_aacenc_config config = { 0 };

    /* check attribute of encoder */
    pstAttr = (const AencAacAttr *)pEncoderAttr;
    td_s32 ret = AencCheckAACAttr(pstAttr);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "s32Ret:0x%x", ret);
        return ret;
    }

    /* allocate memory for encoder */
    pstEncoder = (AencAacEncoder *)malloc(sizeof(AencAacEncoder));
    if (pstEncoder == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "no memory");
        return OT_ERR_AENC_NO_MEM;
    }

    if (memset_s(pstEncoder, sizeof(AencAacEncoder), 0, sizeof(AencAacEncoder)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }

    ret = AencAACConfig(pstAttr, &config);
    if (ret != TD_SUCCESS) {
        free(pstEncoder);
        MEDIA_HAL_LOGE(MODULE_NAME, "s32Ret:0x%x", ret);
        return ret;
    }

    /* create encoder */
    ret = AACEncoderOpenAdp(&pstEncoder->pstAACState, &config);
    if (ret != TD_SUCCESS) {
        free(pstEncoder);
        MEDIA_HAL_LOGE(MODULE_NAME, "s32Ret:0x%x", ret);
        return ret;
    }

    ret = memcpy_s(&pstEncoder->stAACAttr, sizeof(AencAacAttr),
        pstAttr, sizeof(AencAacAttr));
    if (ret != EOK) {
        free(pstEncoder);
        MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s aenc attr fail");
        return TD_FAILURE;
    }

    *ppEncoder = (void *)pstEncoder;
    return TD_SUCCESS;
}

static td_s32 AencCheckAttrAndGetWaterLine(AencAacAttr *stAACAttr, const ot_audio_frame *pstData, td_u32 *u32WaterLine)
{
    if (pstData == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "pstData is null");
        return OT_ERR_AENC_NULL_PTR;
    }
    if (stAACAttr->enSoundMode == OT_AUDIO_SOUND_MODE_STEREO) {
        /* whether the sound mode of frame and channel is match  */
        if (pstData->snd_mode != OT_AUDIO_SOUND_MODE_STEREO) {
            MEDIA_HAL_LOGE(MODULE_NAME, " [Info]:%s", "AAC encode receive a frame which not match its Soundmode");
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    }

    /* WaterLine, equals to the frame sample frame of protocol */
    if (stAACAttr->enAACType == AAC_TYPE_AACLC) {
        *u32WaterLine = AACLC_SAMPLES_PER_FRAME;
    } else if (stAACAttr->enAACType == AAC_TYPE_EAAC ||
        stAACAttr->enAACType == AAC_TYPE_EAACPLUS) {
        *u32WaterLine = AACPLUS_SAMPLES_PER_FRAME;
    } else if (stAACAttr->enAACType == AAC_TYPE_AACLD ||
        stAACAttr->enAACType == AAC_TYPE_AACELD) {
        *u32WaterLine = AACLD_SAMPLES_PER_FRAME;
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "invalid AAC coder type");
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }
    /* calculate point number */
    td_u32 u32PtNums = pstData->len / (pstData->bit_width + 1);

    /* if frame sample larger than protocol sample, reject to receive, or buffer will be overflow */
    if (u32PtNums != *u32WaterLine) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:invalid u32PtNums%d for AACType:%d",
            u32PtNums, stAACAttr->enAACType);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }
    return TD_SUCCESS;
}

static td_s32 EncodeAACFrm(void *pEncoder, const ot_audio_frame *pstData, td_u8 *outBuf, td_u32 *outLen)
{
    HI_AUDIO_ASSERT(pEncoder != NULL);
    HI_AUDIO_ASSERT(pstData != NULL);

    HI_AUDIO_ASSERT(outBuf != NULL);
    HI_AUDIO_ASSERT(outLen != NULL);
    AencAacEncoder *pstEncoder = NULL;
    td_s32 i;
    td_s16 aData[OT_AACENC_BLOCK_SIZE * 0x2 * OT_AACENC_MAX_CHANNELS];
    td_s16 s16Len;
    td_u32 u32WaterLine = 0;

    pstEncoder = (AencAacEncoder *)pEncoder;

    td_s32 ret = AencCheckAttrAndGetWaterLine(&(pstEncoder->stAACAttr), pstData, &u32WaterLine);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "check attr failed");
        return ret;
    }

    /* AAC encoder need interleaved data,here change LLLRRR to LRLRLR.
       OT_AACLC will encode 1024*2 point, and AACplus encode 2048*2 point */
    if (pstEncoder->stAACAttr.enSoundMode == OT_AUDIO_SOUND_MODE_STEREO) {
        s16Len = u32WaterLine;
        for (i = s16Len - 1; i >= 0; i--) {
            aData[0x2 * i] = *((td_s16 *)pstData->virt_addr[0] + i);
            aData[0x2 * i + 1] = *((td_s16 *)pstData->virt_addr[1] + i);
        }
    } else { /* if inbuf is momo, copy left to right */
        td_s16 *temp = (td_s16 *)pstData->virt_addr[0];
        s16Len = u32WaterLine;
        for (i = s16Len - 1; i >= 0; i--) {
            aData[i] = *(temp + i);
        }
    }

    td_u32 dataCnt = u32WaterLine * 0x2;
    CHK_NULL_RETURN(pstEncoder->pstAACState, TD_FAILURE, "pstAACState null");

    ret = AACEncoderFrameAdp(pstEncoder->pstAACState, aData,
        dataCnt, outBuf, (td_s32*)outLen);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "AAC encode failed");
    }

    return ret;
}

static td_s32 CloseAACEncoder(void *pEncoder)
{
    HI_AUDIO_ASSERT(pEncoder != NULL);
    AencAacEncoder *pstEncoder = (AencAacEncoder *)pEncoder;

    if (pstEncoder->pstAACState == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "pstAACState null");
    } else {
        AACEncoderCloseAdp(pstEncoder->pstAACState);
    }

    free(pstEncoder);
    return TD_SUCCESS;
}

td_s32 HiMpiAencAacInit(void)
{
    ot_aenc_encoder stAac;
    td_s32 ret = InitAacAencLib();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    stAac.type = OT_PT_AAC;
    if (snprintf_s(stAac.name, sizeof(stAac.name), sizeof(stAac.name) - 1, "Aac") < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s stAac.name error");
        return TD_FAILURE;
    }
    stAac.max_frame_len = MAX_AAC_MAINBUF_SIZE;
    stAac.func_open_encoder = OpenAACEncoder;
    stAac.func_enc_frame = EncodeAACFrm;
    stAac.func_close_encoder = CloseAACEncoder;
    td_s32 handle = 0;
    ret = ss_mpi_aenc_register_encoder(&handle, &stAac);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    g_AacEncHandle = handle;

    return TD_SUCCESS;
}

td_s32 HiMpiAencAacDeInit(void)
{
    td_s32 ret = ss_mpi_aenc_unregister_encoder(g_AacEncHandle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    g_AacEncHandle = 0;
    DeInitAacAencLib();

    return TD_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

