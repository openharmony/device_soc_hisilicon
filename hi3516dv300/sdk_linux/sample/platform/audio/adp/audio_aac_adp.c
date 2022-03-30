/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "audio_aac_adp.h"
#include "audio_dl_adp.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <aacdec.h>
#include <aacenc.h>

#define HI_AUDIO_ASSERT(x)  \
    {                       \
        if ((x) != HI_TRUE) \
            return -1;      \
    }

#define aac_bitrate_sel(is_mono, mono_bitrate, stereo_bitrate) ((is_mono) ? (mono_bitrate) : (stereo_bitrate))

#define AAC_ENC_LIB_NAME            "libaacenc.so"
#define AAC_DEC_LIB_NAME            "libaacdec.so"

#define AACENC_BITRATE_16K          16000
#define AACENC_BITRATE_24K          24000
#define AACENC_BITRATE_32K          32000
#define AACENC_BITRATE_44K          44000
#define AACENC_BITRATE_48K          48000
#define AACENC_BITRATE_64K          64000
#define AACENC_BITRATE_96K          96000
#define AACENC_BITRATE_128K         128000
#define AACENC_BITRATE_132K         132000
#define AACENC_BITRATE_144K         144000
#define AACENC_BITRATE_192K         192000
#define AACENC_BITRATE_256K         256000
#define AACENC_BITRATE_265K         265000
#define AACENC_BITRATE_288K         288000
#define AACENC_BITRATE_320K         320000

#define AACENC_CHANNEL_SINGLE       1
#define AACENC_CHANNEL_STEREO       2

#define DUMP_PATH_NAME_MAX_BYTES    256
#define DUMP_MAX_TIMES              10000

// aac enc lib
typedef HI_S32 (*pHI_AACENC_GetVersion_Callback)(AACENC_VERSION_S *pVersion);
typedef HI_S32 (*pAACInitDefaultConfig_Callback)(AACENC_CONFIG *pstConfig);
typedef HI_S32 (*pAACEncoderOpen_Callback)(AAC_ENCODER_S **phAacPlusEnc, const AACENC_CONFIG *pstConfig);
typedef HI_S32 (*pAACEncoderFrame_Callback)(AAC_ENCODER_S *hAacPlusEnc, HI_S16 *ps16PcmBuf, HI_U8 *pu8Outbuf,
    HI_S32 *ps32NumOutBytes);
typedef HI_VOID (*pAACEncoderClose_Callback)(AAC_ENCODER_S *hAacPlusEnc);

// aac dec lib
typedef HI_S32 (*pHI_AACDEC_GetVersion_Callback)(AACDEC_VERSION_S *pVersion);
typedef HAACDecoder (*pAACInitDecoder_Callback)(AACDECTransportType enTranType);
typedef HI_VOID (*pAACFreeDecoder_Callback)(HAACDecoder hAACDecoder);
typedef HI_S32 (*pAACSetRawMode_Callback)(HAACDecoder hAACDecoder, HI_S32 nChans, HI_S32 sampRate);
typedef HI_S32 (*pAACDecodeFindSyncHeader_Callback)(HAACDecoder hAACDecoder, HI_U8 **ppInbufPtr, HI_S32 *pBytesLeft);
typedef HI_S32 (*pAACDecodeFrame_Callback)(HAACDecoder hAACDecoder, HI_U8 **ppInbufPtr, HI_S32 *pBytesLeft,
    HI_S16 *pOutPcm);
typedef HI_S32 (*pAACGetLastFrameInfo_Callback)(HAACDecoder hAACDecoder, AACFrameInfo *aacFrameInfo);
typedef HI_S32 (*pAACDecoderSetEosFlag_Callback)(HAACDecoder hAACDecoder, HI_S32 s32Eosflag);
typedef HI_S32 (*pAACFlushCodec_Callback)(HAACDecoder hAACDecoder);

typedef struct {
    HI_S32 s32OpenCnt;
    HI_VOID *pLibHandle;

    pHI_AACENC_GetVersion_Callback pHI_AACENC_GetVersion;
    pAACInitDefaultConfig_Callback pAACInitDefaultConfig;
    pAACEncoderOpen_Callback pAACEncoderOpen;
    pAACEncoderFrame_Callback pAACEncoderFrame;
    pAACEncoderClose_Callback pAACEncoderClose;
} AACENC_FUN_S;

typedef struct {
    HI_S32 s32OpenCnt;
    HI_VOID *pLibHandle;

    pHI_AACDEC_GetVersion_Callback pHI_AACDEC_GetVersion;
    pAACInitDecoder_Callback pAACInitDecoder;
    pAACFreeDecoder_Callback pAACFreeDecoder;
    pAACSetRawMode_Callback pAACSetRawMode;
    pAACDecodeFindSyncHeader_Callback pAACDecodeFindSyncHeader;
    pAACDecodeFrame_Callback pAACDecodeFrame;
    pAACGetLastFrameInfo_Callback pAACGetLastFrameInfo;
    pAACDecoderSetEosFlag_Callback pAACDecoderSetEosFlag;
    pAACFlushCodec_Callback pAACFlushCodec;
} AACDEC_FUN_S;

#ifdef HI_AAC_USE_STATIC_MODULE_REGISTER
static HI_S32 g_aenc_open_flag = 0;
static HI_S32 g_adec_open_flag = 0;
#endif

static HI_S32 g_AacEncHandle = 0;
static HI_S32 g_AacDecHandle = 0;

static AACENC_FUN_S g_stAacEncFunc = { 0 };
static AACDEC_FUN_S g_stAacDecFunc = { 0 };

#if defined(HI_AAC_USE_STATIC_MODULE_REGISTER) && defined(HI_AAC_HAVE_SBR_LIB)
static hi_s32 aac_init_sbr_enc_lib(hi_void)
{
    hi_s32 ret;
    hi_void *sbr_enc_handle = HI_AAC_SBRENC_GetHandle();
    ret = AACEncoderRegisterModule(sbr_enc_handle);
    if (ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "init SbrEnc lib fail!\n");
        return HI_ERR_AENC_NOT_SUPPORT;
    }
    return ret;
}

static hi_s32 aac_init_sbr_dec_lib(hi_void)
{
    hi_s32 ret;
    hi_void *sbr_dec_handle = HI_AAC_SBRDEC_GetHandle();
    ret = AACDecoderRegisterModule(sbr_dec_handle);
    if (ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "init SbrDec lib fail!\n");
        return HI_ERR_ADEC_NOT_SUPPORT;
    }
    return ret;
}
#endif

#ifndef HI_AAC_USE_STATIC_MODULE_REGISTER
static hi_s32 aac_enc_lib_dlsym(AACENC_FUN_S *aac_enc_func)
{
    hi_s32 ret;

    ret = Audio_Dlsym((hi_void **)&(aac_enc_func->pHI_AACENC_GetVersion), aac_enc_func->pLibHandle,
        "HI_AACENC_GetVersion");
    if (ret != HI_SUCCESS) {
        return HI_ERR_AENC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_enc_func->pAACInitDefaultConfig), aac_enc_func->pLibHandle,
        "AACInitDefaultConfig");
    if (ret != HI_SUCCESS) {
        return HI_ERR_AENC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_enc_func->pAACEncoderOpen), aac_enc_func->pLibHandle, "AACEncoderOpen");
    if (ret != HI_SUCCESS) {
        return HI_ERR_AENC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_enc_func->pAACEncoderFrame), aac_enc_func->pLibHandle, "AACEncoderFrame");
    if (ret != HI_SUCCESS) {
        return HI_ERR_AENC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_enc_func->pAACEncoderClose), aac_enc_func->pLibHandle, "AACEncoderClose");
    if (ret != HI_SUCCESS) {
        return HI_ERR_AENC_NOT_SUPPORT;
    }

    return HI_SUCCESS;
}
#endif

static HI_S32 InitAacAencLib(void)
{
#ifdef HI_AAC_USE_STATIC_MODULE_REGISTER
    g_stAacEncFunc.pHI_AACENC_GetVersion = HI_AACENC_GetVersion;
    g_stAacEncFunc.pAACInitDefaultConfig = AACInitDefaultConfig;
    g_stAacEncFunc.pAACEncoderOpen = AACEncoderOpen;
    g_stAacEncFunc.pAACEncoderFrame = AACEncoderFrame;
    g_stAacEncFunc.pAACEncoderClose = AACEncoderClose;
#ifdef HI_AAC_HAVE_SBR_LIB
    if (g_aenc_open_flag == 0) {
        if (aac_init_sbr_enc_lib() != HI_SUCCESS) {
            return HI_ERR_AENC_NOT_SUPPORT;
        }
        g_aenc_open_flag = 1;
    }
#endif
#else
    if (g_stAacEncFunc.s32OpenCnt == 0) {
        hi_s32 ret;
        AACENC_FUN_S aac_enc_func = { 0 };

        ret = Audio_Dlopen(&(aac_enc_func.pLibHandle), AAC_ENC_LIB_NAME);
        if (ret != HI_SUCCESS) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "load aenc lib fail!\n");
            return HI_ERR_AENC_NOT_SUPPORT;
        }

        ret = aac_enc_lib_dlsym(&aac_enc_func);
        if (ret != HI_SUCCESS) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "find symbol error!\n");
            Audio_Dlclose(aac_enc_func.pLibHandle);
            aac_enc_func.pLibHandle = HI_NULL;
            return HI_ERR_AENC_NOT_SUPPORT;
        }

        (hi_void)memcpy_s(&g_stAacEncFunc, sizeof(g_stAacEncFunc), &aac_enc_func, sizeof(aac_enc_func));
    }
    g_stAacEncFunc.s32OpenCnt++;
#endif
    return HI_SUCCESS;
}

static HI_VOID DeInitAacAencLib(HI_VOID)
{
    HI_S32 s32Ret;

    if (g_stAacEncFunc.s32OpenCnt != 0) {
        g_stAacEncFunc.s32OpenCnt--;
    }

    if (g_stAacEncFunc.s32OpenCnt == 0) {
#ifndef HI_AAC_USE_STATIC_MODULE_REGISTER
        if (g_stAacEncFunc.pLibHandle != HI_NULL) {
            Audio_Dlclose(g_stAacEncFunc.pLibHandle);
        }
#endif
        s32Ret = memset_s(&g_stAacEncFunc, sizeof(AACENC_FUN_S), 0, sizeof(AACENC_FUN_S));
        if (s32Ret != EOK) {
            printf("%s: memset_s failed, ret:0x%x.\n", __FUNCTION__, s32Ret);
            return;
        }
    }

    return;
}

static HI_S32 AACInitDefaultConfig_Adp(AACENC_CONFIG *pstConfig)
{
    if (g_stAacEncFunc.pAACInitDefaultConfig == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return HI_ERR_AENC_NOT_SUPPORT;
    }
    return g_stAacEncFunc.pAACInitDefaultConfig(pstConfig);
}

static HI_S32 AACEncoderOpen_Adp(AAC_ENCODER_S **phAacPlusEnc, const AACENC_CONFIG *pstConfig)
{
    if (g_stAacEncFunc.pAACEncoderOpen == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return HI_ERR_AENC_NOT_SUPPORT;
    }
    return g_stAacEncFunc.pAACEncoderOpen(phAacPlusEnc, pstConfig);
}

static HI_S32 AACEncoderFrame_Adp(AAC_ENCODER_S *hAacPlusEnc, HI_S16 *ps16PcmBuf,
    HI_U8 *pu8Outbuf, HI_S32 *ps32NumOutBytes)
{
    if (g_stAacEncFunc.pAACEncoderFrame == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return HI_ERR_AENC_NOT_SUPPORT;
    }
    return g_stAacEncFunc.pAACEncoderFrame(hAacPlusEnc, ps16PcmBuf, pu8Outbuf, ps32NumOutBytes);
}

static HI_VOID AACEncoderClose_Adp(AAC_ENCODER_S *hAacPlusEnc)
{
    if (g_stAacEncFunc.pAACEncoderClose == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return;
    }
    return g_stAacEncFunc.pAACEncoderClose(hAacPlusEnc);
}

#ifndef HI_AAC_USE_STATIC_MODULE_REGISTER
static hi_s32 aac_dec_lib_dlsym(AACDEC_FUN_S *aac_dec_func)
{
    hi_s32 ret;

    ret = Audio_Dlsym((hi_void **)&(aac_dec_func->pHI_AACDEC_GetVersion), aac_dec_func->pLibHandle,
        "HI_AACDEC_GetVersion");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_dec_func->pAACInitDecoder), aac_dec_func->pLibHandle, "AACInitDecoder");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_dec_func->pAACFreeDecoder), aac_dec_func->pLibHandle, "AACFreeDecoder");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_dec_func->pAACSetRawMode), aac_dec_func->pLibHandle, "AACSetRawMode");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_dec_func->pAACDecodeFindSyncHeader), aac_dec_func->pLibHandle,
        "AACDecodeFindSyncHeader");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_dec_func->pAACDecodeFrame), aac_dec_func->pLibHandle, "AACDecodeFrame");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    ret =
        Audio_Dlsym((hi_void **)&(aac_dec_func->pAACGetLastFrameInfo), aac_dec_func->pLibHandle, "AACGetLastFrameInfo");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_dec_func->pAACDecoderSetEosFlag), aac_dec_func->pLibHandle,
        "AACDecoderSetEosFlag");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    ret = Audio_Dlsym((hi_void **)&(aac_dec_func->pAACFlushCodec), aac_dec_func->pLibHandle, "AACFlushCodec");
    if (ret != HI_SUCCESS) {
        return HI_ERR_ADEC_NOT_SUPPORT;
    }

    return HI_SUCCESS;
}
#endif

static HI_S32 InitAacAdecLib(void)
{
#ifdef HI_AAC_USE_STATIC_MODULE_REGISTER
    g_stAacDecFunc.pHI_AACDEC_GetVersion = HI_AACDEC_GetVersion;
    g_stAacDecFunc.pAACInitDecoder = AACInitDecoder;
    g_stAacDecFunc.pAACFreeDecoder = AACFreeDecoder;
    g_stAacDecFunc.pAACSetRawMode = AACSetRawMode;
    g_stAacDecFunc.pAACDecodeFindSyncHeader = AACDecodeFindSyncHeader;
    g_stAacDecFunc.pAACDecodeFrame = AACDecodeFrame;
    g_stAacDecFunc.pAACGetLastFrameInfo = AACGetLastFrameInfo;
    g_stAacDecFunc.pAACDecoderSetEosFlag = AACDecoderSetEosFlag;
    g_stAacDecFunc.pAACFlushCodec = AACFlushCodec;
#ifdef HI_AAC_HAVE_SBR_LIB
    if (g_adec_open_flag == 0) {
        if (aac_init_sbr_dec_lib() != HI_SUCCESS) {
            return HI_ERR_ADEC_NOT_SUPPORT;
        }
        g_adec_open_flag = 1;
    }
#endif
#else

    if (g_stAacDecFunc.s32OpenCnt == 0) {
        hi_s32 ret;
        AACDEC_FUN_S aac_dec_func = { 0 };

        ret = Audio_Dlopen(&(aac_dec_func.pLibHandle), AAC_DEC_LIB_NAME);
        if (ret != HI_SUCCESS) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "load aenc lib fail!\n");
            return HI_ERR_ADEC_NOT_SUPPORT;
        }

        ret = aac_dec_lib_dlsym(&aac_dec_func);
        if (ret != HI_SUCCESS) {
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "find symbol error!\n");
            Audio_Dlclose(aac_dec_func.pLibHandle);
            aac_dec_func.pLibHandle = HI_NULL;
            return HI_ERR_ADEC_NOT_SUPPORT;
        }

        (hi_void)memcpy_s(&g_stAacDecFunc, sizeof(g_stAacDecFunc), &aac_dec_func, sizeof(aac_dec_func));
    }
    g_stAacDecFunc.s32OpenCnt++;
#endif

    return HI_SUCCESS;
}

static HI_VOID DeInitAacAdecLib(HI_VOID)
{
    HI_S32 s32Ret;

    if (g_stAacDecFunc.s32OpenCnt != 0) {
        g_stAacDecFunc.s32OpenCnt--;
    }

    if (g_stAacDecFunc.s32OpenCnt == 0) {
#ifdef HI_AAC_USE_STATIC_MODULE_REGISTER
        if (g_stAacDecFunc.pLibHandle != HI_NULL) {
            Audio_Dlclose(g_stAacDecFunc.pLibHandle);
        }
#endif
        s32Ret = memset_s(&g_stAacDecFunc, sizeof(AACDEC_FUN_S), 0, sizeof(AACDEC_FUN_S));
        if (s32Ret != EOK) {
            printf("%s: memset_s failed, ret:0x%x.\n", __FUNCTION__, s32Ret);
            return;
        }
    }

    return;
}

static HAACDecoder AACInitDecoder_Adp(AACDECTransportType enTranType)
{
    if (g_stAacDecFunc.pAACInitDecoder == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return HI_NULL;
    }
    return g_stAacDecFunc.pAACInitDecoder(enTranType);
}

static HI_VOID AACFreeDecoder_Adp(HAACDecoder hAACDecoder)
{
    if (g_stAacDecFunc.pAACFreeDecoder == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return;
    }
    return g_stAacDecFunc.pAACFreeDecoder(hAACDecoder);
}

static HI_S32 AACDecodeFindSyncHeader_Adp(HAACDecoder hAACDecoder, HI_U8 **ppInbufPtr, HI_S32 *pBytesLeft)
{
    if (g_stAacDecFunc.pAACDecodeFindSyncHeader == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return HI_ERR_ADEC_NOT_SUPPORT;
    }
    return g_stAacDecFunc.pAACDecodeFindSyncHeader(hAACDecoder, ppInbufPtr, pBytesLeft);
}

static HI_S32 AACDecodeFrame_Adp(HAACDecoder hAACDecoder, HI_U8 **ppInbufPtr, HI_S32 *pBytesLeft, HI_S16 *pOutPcm)
{
    if (g_stAacDecFunc.pAACDecodeFrame == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return HI_ERR_ADEC_NOT_SUPPORT;
    }
    return g_stAacDecFunc.pAACDecodeFrame(hAACDecoder, ppInbufPtr, pBytesLeft, pOutPcm);
}

static HI_S32 AACGetLastFrameInfo_Adp(HAACDecoder hAACDecoder, AACFrameInfo *aacFrameInfo)
{
    if (g_stAacDecFunc.pAACGetLastFrameInfo == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "call aac function fail!\n");
        return HI_ERR_ADEC_NOT_SUPPORT;
    }
    return g_stAacDecFunc.pAACGetLastFrameInfo(hAACDecoder, aacFrameInfo);
}

static HI_S32 AencCheckAACAttr(const AENC_ATTR_AAC_S *pstAACAttr)
{
    if (pstAACAttr->enBitWidth != AUDIO_BIT_WIDTH_16) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "invalid bitwidth for AAC encoder");
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pstAACAttr->enSoundMode >= AUDIO_SOUND_MODE_BUTT) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "invalid sound mode for AAC encoder");
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    if ((pstAACAttr->enAACType == AAC_TYPE_EAACPLUS) && (pstAACAttr->enSoundMode != AUDIO_SOUND_MODE_STEREO)) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "invalid sound mode for AAC encoder");
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    if (pstAACAttr->enTransType == AAC_TRANS_TYPE_ADTS) {
        if ((pstAACAttr->enAACType == AAC_TYPE_AACLD) || (pstAACAttr->enAACType == AAC_TYPE_AACELD)) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__,
                "AACLD or AACELD not support AAC_TRANS_TYPE_ADTS");
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_get_aaclc_bitrate(const AACENC_CONFIG *config, hi_s32 *min_rate,
                                     hi_s32 *max_rate, hi_s32 *recommemd_rate)
{
    hi_bool chn_single = (config->nChannelsIn == AACENC_CHANNEL_SINGLE) ? HI_TRUE : HI_FALSE;

    if (config->sampleRate == AUDIO_SAMPLE_RATE_32000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_192K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_128K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_44100) {
        *min_rate = AACENC_BITRATE_48K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_265K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_48000) {
        *min_rate = AACENC_BITRATE_48K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_288K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_16000) {
        *min_rate = AACENC_BITRATE_24K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *recommemd_rate = AACENC_BITRATE_48K;
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_8000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_24K, AACENC_BITRATE_32K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_24000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_144K, AACENC_BITRATE_288K);
        *recommemd_rate = AACENC_BITRATE_48K;
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_22050) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_132K, AACENC_BITRATE_265K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_48K);
    } else {
        printf("AACLC invalid samplerate(%d)\n", config->sampleRate);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_check_aaclc_config(const AACENC_CONFIG *config)
{
    hi_s32 min_bit_rate = 0;
    hi_s32 max_bit_rate = 0;
    hi_s32 recommemd_rate = 0;
    hi_s32 ret;

    if (config->coderFormat == AACLC) {
        if (config->nChannelsOut != config->nChannelsIn) {
            printf("AACLC nChannelsOut(%d) in not equal to nChannelsIn(%d)\n",
                config->nChannelsOut, config->nChannelsIn);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_aaclc_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        if ((config->bitRate < min_bit_rate) || (config->bitRate > max_bit_rate)) {
            printf("AACLC %d Hz bitRate(%d) should be %d ~ %d, recommend %d\n",
                config->sampleRate, config->bitRate, min_bit_rate, max_bit_rate, recommemd_rate);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_get_eaac_bitrate(const AACENC_CONFIG *config, hi_s32 *min_rate,
                                    hi_s32 *max_rate, hi_s32 *recommemd_rate)
{
    hi_bool chn_single = (config->nChannelsIn == AACENC_CHANNEL_SINGLE) ? HI_TRUE : HI_FALSE;

    if (config->sampleRate == AUDIO_SAMPLE_RATE_32000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_44100) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_48000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_16000) {
        *min_rate = AACENC_BITRATE_24K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
        *recommemd_rate = AACENC_BITRATE_32K;
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_22050) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_24000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else {
        printf("EAAC invalid samplerate(%d)\n", config->sampleRate);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_check_eaac_config(const AACENC_CONFIG *config)
{
    hi_s32 min_bit_rate = 0;
    hi_s32 max_bit_rate = 0;
    hi_s32 recommemd_rate = 0;
    hi_s32 ret;

    if (config->coderFormat == EAAC) {
        if (config->nChannelsOut != config->nChannelsIn) {
            printf("EAAC nChannelsOut(%d) is not equal to nChannelsIn(%d)\n",
                config->nChannelsOut, config->nChannelsIn);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_eaac_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        if ((config->bitRate < min_bit_rate) || (config->bitRate > max_bit_rate)) {
            printf("EAAC %d Hz bitRate(%d) should be %d ~ %d, recommend %d\n",
                config->sampleRate, config->bitRate, min_bit_rate, max_bit_rate, recommemd_rate);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_get_eaacplus_bitrate(const AACENC_CONFIG *config, hi_s32 *min_rate,
                                        hi_s32 *max_rate, hi_s32 *recommemd_rate)
{
    if (config->sampleRate == AUDIO_SAMPLE_RATE_32000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_32K;
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_44100) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_48K;
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_48000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_48K;
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_16000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_48K;
        *recommemd_rate = AACENC_BITRATE_32K;
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_22050) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_32K;
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_24000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_32K;
    } else {
        printf("EAACPLUS invalid samplerate(%d)\n", config->sampleRate);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_check_eaacplus_config(const AACENC_CONFIG *config)
{
    hi_s32 min_bit_rate = 0;
    hi_s32 max_bit_rate = 0;
    hi_s32 recommemd_rate = 0;
    hi_s32 ret;

    if (config->coderFormat == EAACPLUS) {
        if ((config->nChannelsOut != AACENC_CHANNEL_STEREO) ||
            (config->nChannelsIn != AACENC_CHANNEL_STEREO)) {
            printf("EAACPLUS nChannelsOut(%d) and nChannelsIn(%d) should be 2\n",
                config->nChannelsOut, config->nChannelsIn);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_eaacplus_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        if ((config->bitRate < min_bit_rate) || (config->bitRate > max_bit_rate)) {
            printf("EAACPLUS %d Hz bitRate(%d) should be %d ~ %d, recommend %d\n",
                config->sampleRate, config->bitRate, min_bit_rate, max_bit_rate, recommemd_rate);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_get_aacld_bitrate(const AACENC_CONFIG *config, hi_s32 *min_rate,
                                     hi_s32 *max_rate, hi_s32 *recommemd_rate)
{
    hi_bool chn_single = (config->nChannelsIn == AACENC_CHANNEL_SINGLE) ? HI_TRUE : HI_FALSE;

    if (config->sampleRate == AUDIO_SAMPLE_RATE_32000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_44100) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_44K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_128K, AACENC_BITRATE_256K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_48000) {
        *min_rate = AACENC_BITRATE_64K;
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_128K, AACENC_BITRATE_256K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_16000) {
        *min_rate = (chn_single) ? AACENC_BITRATE_24K : AACENC_BITRATE_32K;
        *max_rate = (chn_single) ? AACENC_BITRATE_192K : AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_8000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_24K, AACENC_BITRATE_48K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_24000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_48K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_22050) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_48K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
    } else {
        printf("AACLD invalid samplerate(%d)\n", config->sampleRate);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_check_aacld_config(const AACENC_CONFIG *config)
{
    hi_s32 min_bit_rate = 0;
    hi_s32 max_bit_rate = 0;
    hi_s32 recommemd_rate = 0;
    hi_s32 ret;

    if (config->coderFormat == AACLD) {
        if (config->nChannelsOut != config->nChannelsIn) {
            printf("AACLD nChannelsOut(%d) in not equal to nChannelsIn(%d)\n",
                config->nChannelsOut, config->nChannelsIn);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_aacld_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        if ((config->bitRate < min_bit_rate) || (config->bitRate > max_bit_rate)) {
            printf("AACLD %d Hz bitRate(%d) should be %d ~ %d, recommend %d\n",
                config->sampleRate, config->bitRate, min_bit_rate, max_bit_rate, recommemd_rate);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_get_aaceld_bitrate(const AACENC_CONFIG *config, hi_s32 *min_rate,
                                      hi_s32 *max_rate, hi_s32 *recommemd_rate)
{
    hi_bool chn_single = (config->nChannelsIn == AACENC_CHANNEL_SINGLE) ? HI_TRUE : HI_FALSE;

    if (config->sampleRate == AUDIO_SAMPLE_RATE_32000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_64K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_44100) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_128K, AACENC_BITRATE_256K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_48000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_128K, AACENC_BITRATE_256K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_16000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_16K, AACENC_BITRATE_32K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_8000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_64K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_64K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_24000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_24K, AACENC_BITRATE_32K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sampleRate == AUDIO_SAMPLE_RATE_22050) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_24K, AACENC_BITRATE_32K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
    } else {
        printf("AACELD invalid samplerate(%d)\n", config->sampleRate);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_check_aaceld_config(const AACENC_CONFIG *config)
{
    hi_s32 min_bit_rate = 0;
    hi_s32 max_bit_rate = 0;
    hi_s32 recommemd_rate = 0;
    hi_s32 ret;

    if (config->coderFormat == AACELD) {
        if (config->nChannelsOut != config->nChannelsIn) {
            printf("AACELD nChannelsOut(%d) in not equal to nChannelsIn(%d)\n",
                config->nChannelsOut, config->nChannelsIn);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_aaceld_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        if ((config->bitRate < min_bit_rate) || (config->bitRate > max_bit_rate)) {
            printf("AACELD %d Hz bitRate(%d) should be %d ~ %d, recommend %d\n",
                config->sampleRate, config->bitRate, min_bit_rate, max_bit_rate, recommemd_rate);
            return HI_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_aac_check_coder_format(AuEncoderFormat coderFormat)
{
    if (coderFormat != AACLC && coderFormat != EAAC &&
        coderFormat != EAACPLUS && coderFormat != AACLD &&
        coderFormat != AACELD) {
        printf("aacenc coderFormat(%d) invalid\n", coderFormat);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 aenc_aac_check_config(const AACENC_CONFIG *config)
{
    hi_s32 ret;

    if (config == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "config is null");
        return HI_ERR_AENC_NULL_PTR;
    }

    if (aenc_aac_check_coder_format(config->coderFormat) != HI_SUCCESS) {
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    if (config->quality != AU_QualityExcellent && config->quality != AU_QualityHigh &&
        config->quality != AU_QualityMedium && config->quality != AU_QualityLow) {
        printf("aacenc quality(%d) invalid\n", config->quality);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    if (config->bitsPerSample != 16) { /* 16: 16bit */
        printf("aacenc bitsPerSample(%d) should be 16\n", config->bitsPerSample);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    if ((config->transtype < 0) || (config->transtype > AACENC_LATM_MCP1)) {
        printf("invalid transtype(%d), not in [0, 2]\n", config->transtype);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    if ((config->bandWidth != 0) && ((config->bandWidth < 1000) || /* 1000: band width */
        (config->bandWidth > config->sampleRate / 2))) { /* 2: half */
        printf("AAC bandWidth(%d) should be 0, or 1000 ~ %d\n",
            config->bandWidth, config->sampleRate / 2); /* 2: half */
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    if (config->coderFormat == AACLC) {
        ret = aenc_check_aaclc_config(config);
    } else if (config->coderFormat == EAAC) {
        ret = aenc_check_eaac_config(config);
    } else if (config->coderFormat == EAACPLUS) {
        ret = aenc_check_eaacplus_config(config);
    } else if (config->coderFormat == AACLD) {
        ret = aenc_check_aacld_config(config);
    } else if (config->coderFormat == AACELD) {
        ret = aenc_check_aaceld_config(config);
    } else {
        ret = HI_FAILURE;
    }

    return ret;
}

static hi_s32 aenc_init_aac_config(AACENC_CONFIG *config, const AENC_ATTR_AAC_S *attr)
{
    hi_s32 ret;

    /* set default config to encoder */
    ret = AACInitDefaultConfig_Adp(config);
    if (ret) {
        printf("[Func]:%s [Line]:%d ret:0x%x.#########\n", __FUNCTION__, __LINE__, ret);
        return ret;
    }

    config->coderFormat = (AuEncoderFormat)attr->enAACType;
    config->bitRate = attr->enBitRate;
    config->bitsPerSample = 8 * (1 << (HI_U32)(attr->enBitWidth)); /* 8: 1byte = 8bits */
    config->sampleRate = attr->enSmpRate;
    config->bandWidth = attr->s16BandWidth;
    config->transtype = (AACENCTransportType)attr->enTransType;

    if (attr->enSoundMode == AUDIO_SOUND_MODE_MONO && attr->enAACType != AAC_TYPE_EAACPLUS) {
        config->nChannelsIn = AACENC_CHANNEL_SINGLE;
        config->nChannelsOut = AACENC_CHANNEL_SINGLE;
    } else {
        config->nChannelsIn = AACENC_CHANNEL_STEREO;
        config->nChannelsOut = AACENC_CHANNEL_STEREO;
    }

    config->quality = AU_QualityHigh;

    ret = aenc_aac_check_config(config);
    if (ret) {
        printf("[Func]:%s [Line]:%d #########\n", __FUNCTION__, __LINE__);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static HI_S32 OpenAACEncoder(HI_VOID *pEncoderAttr, HI_VOID **ppEncoder)
{
    AENC_AAC_ENCODER_S *pstEncoder = NULL;
    AENC_ATTR_AAC_S *pstAttr = NULL;
    HI_S32 s32Ret;
    AACENC_CONFIG config = {0};

    HI_AUDIO_ASSERT(pEncoderAttr != NULL);
    HI_AUDIO_ASSERT(ppEncoder != NULL);

    /* check attribute of encoder */
    pstAttr = (AENC_ATTR_AAC_S *)pEncoderAttr;
    s32Ret = AencCheckAACAttr(pstAttr);
    if (s32Ret) {
        printf("[Func]:%s [Line]:%d s32Ret:0x%x.#########\n", __FUNCTION__, __LINE__, s32Ret);
        return s32Ret;
    }

    /* allocate memory for encoder */
    pstEncoder = (AENC_AAC_ENCODER_S *)malloc(sizeof(AENC_AAC_ENCODER_S));
    if (pstEncoder == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "no memory");
        return HI_ERR_AENC_NOMEM;
    }
    (hi_void)memset_s(pstEncoder, sizeof(AENC_AAC_ENCODER_S), 0, sizeof(AENC_AAC_ENCODER_S));

    *ppEncoder = (HI_VOID *)pstEncoder;

    s32Ret = aenc_init_aac_config(&config, pstAttr);
    if (s32Ret) {
        free(pstEncoder);
        pstEncoder = NULL;
        printf("[Func]:%s [Line]:%d s32Ret:0x%x.#########\n", __FUNCTION__, __LINE__, s32Ret);
        return s32Ret;
    }

    /* create encoder */
    s32Ret = AACEncoderOpen_Adp(&pstEncoder->pstAACState, &config);
    if (s32Ret) {
        free(pstEncoder);
        pstEncoder = NULL;
        printf("[Func]:%s [Line]:%d s32Ret:0x%x.#########\n", __FUNCTION__, __LINE__, s32Ret);
        return s32Ret;
    }

    s32Ret = memcpy_s(&pstEncoder->stAACAttr, sizeof(pstEncoder->stAACAttr), pstAttr, sizeof(AENC_ATTR_AAC_S));
    if (s32Ret != EOK) {
        free(pstEncoder);
        pstEncoder = NULL;
        printf("%s: memcpy_s failed, ret:0x%x.\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static hi_s32 encoder_get_water_line(const AENC_AAC_ENCODER_S *encoder, hi_u32 *water_line)
{
    if (encoder->stAACAttr.enAACType == AAC_TYPE_AACLC) {
        *water_line = AACLC_SAMPLES_PER_FRAME;
    } else if (encoder->stAACAttr.enAACType == AAC_TYPE_EAAC ||
        encoder->stAACAttr.enAACType == AAC_TYPE_EAACPLUS) {
        *water_line = AACPLUS_SAMPLES_PER_FRAME;
    } else if (encoder->stAACAttr.enAACType == AAC_TYPE_AACLD ||
        encoder->stAACAttr.enAACType == AAC_TYPE_AACELD) {
        *water_line = AACLD_SAMPLES_PER_FRAME;
    } else {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "invalid AAC coder type");
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static HI_S32 EncodeAACFrm(HI_VOID *pEncoder, const AUDIO_FRAME_S *pstData, HI_U8 *pu8Outbuf, HI_U32 *pu32OutLen)
{
    HI_S32 s32Ret, i;
    AENC_AAC_ENCODER_S *pstEncoder = NULL;
    HI_U32 u32PtNums, u32WaterLine;
    HI_S16 aData[AACENC_BLOCKSIZE * 2 * MAX_CHANNELS]; /* 2: stereo */
    HI_S16 s16Len;

    HI_AUDIO_ASSERT(pEncoder != NULL);
    HI_AUDIO_ASSERT(pstData != NULL);
    HI_AUDIO_ASSERT(pu8Outbuf != NULL);
    HI_AUDIO_ASSERT(pu32OutLen != NULL);

    pstEncoder = (AENC_AAC_ENCODER_S *)pEncoder;

    if (pstEncoder->stAACAttr.enSoundMode == AUDIO_SOUND_MODE_STEREO &&
        pstData->enSoundmode != AUDIO_SOUND_MODE_STEREO) {
        /* whether the sound mode of frame and channel is match  */
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__,
            "AAC encode receive a frame which not match its Soundmode");
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    /* WaterLine, equals to the frame sample frame of protocol */
    s32Ret = encoder_get_water_line(pstEncoder, &u32WaterLine);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    /* calculate point number */
    u32PtNums = pstData->u32Len / (pstData->enBitwidth + 1);

    /* if frame sample larger than protocol sample, reject to receive, or buffer will be overflow */
    if (u32PtNums != u32WaterLine) {
        printf("[Func]:%s [Line]:%d [Info]:invalid u32PtNums%d for AACType:%d\n", __FUNCTION__, __LINE__, u32PtNums,
            pstEncoder->stAACAttr.enAACType);
        return HI_ERR_AENC_ILLEGAL_PARAM;
    }

    /* AAC encoder need interleaved data,here change LLLRRR to LRLRLR.
       AACLC will encode 1024*2 point, and AACplus encode 2048*2 point */
    if (AUDIO_SOUND_MODE_STEREO == pstEncoder->stAACAttr.enSoundMode) {
        s16Len = u32WaterLine;
        for (i = s16Len - 1; i >= 0; i--) {
            aData[AACENC_CHANNEL_STEREO * i] = *((HI_S16 *)pstData->u64VirAddr[0] + i);
            aData[AACENC_CHANNEL_STEREO * i + 1] = *((HI_S16 *)pstData->u64VirAddr[1] + i);
        }
    } else { /* if inbuf is momo, copy left to right */
        HI_S16 *temp = (HI_S16 *)pstData->u64VirAddr[0];

        s16Len = u32WaterLine;
        for (i = s16Len - 1; i >= 0; i--) {
            aData[i] = *(temp + i);
        }
    }

    s32Ret = AACEncoderFrame_Adp(pstEncoder->pstAACState, aData, pu8Outbuf, (HI_S32 *)pu32OutLen);
    if (s32Ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "AAC encode failed");
    }

    return s32Ret;
}

static HI_S32 CloseAACEncoder(HI_VOID *pEncoder)
{
    AENC_AAC_ENCODER_S *pstEncoder = NULL;

    HI_AUDIO_ASSERT(pEncoder != NULL);

    pstEncoder = (AENC_AAC_ENCODER_S *)pEncoder;
    AACEncoderClose_Adp(pstEncoder->pstAACState);

    free(pstEncoder);

    return HI_SUCCESS;
}

static HI_S32 OpenAACDecoder(HI_VOID *pDecoderAttr, HI_VOID **ppDecoder)
{
    ADEC_AAC_DECODER_S *pstDecoder = NULL;
    ADEC_ATTR_AAC_S *pstAttr = NULL;
    HI_S32 s32Ret;

    HI_AUDIO_ASSERT(pDecoderAttr != NULL);
    HI_AUDIO_ASSERT(ppDecoder != NULL);

    /* allocate memory for decoder */
    pstDecoder = (ADEC_AAC_DECODER_S *)malloc(sizeof(ADEC_AAC_DECODER_S));
    if (pstDecoder == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "no memory");
        return HI_ERR_ADEC_NOMEM;
    }
    s32Ret = memset_s(pstDecoder, sizeof(ADEC_AAC_DECODER_S), 0, sizeof(ADEC_AAC_DECODER_S));
    if (s32Ret != EOK) {
        free(pstDecoder);
        pstDecoder = NULL;
        printf("%s: memset_s failed, ret:0x%x.\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }
    *ppDecoder = (HI_VOID *)pstDecoder;

    pstAttr = (ADEC_ATTR_AAC_S *)pDecoderAttr;
    /* create decoder */
    pstDecoder->pstAACState = AACInitDecoder_Adp((AACDECTransportType)pstAttr->enTransType);
    if (!pstDecoder->pstAACState) {
        free(pstDecoder);
        pstDecoder = NULL;
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "AACInitDecoder failed");
        return HI_ERR_ADEC_DECODER_ERR;
    }

    s32Ret = memcpy_s(&pstDecoder->stAACAttr, sizeof(pstDecoder->stAACAttr), pstAttr, sizeof(ADEC_ATTR_AAC_S));
    if (s32Ret != EOK) {
        free(pstDecoder);
        pstDecoder = NULL;
        printf("%s: memcpy_s failed, ret:0x%x.\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 DecodeAACFrm(HI_VOID *pDecoder, HI_U8 **pu8Inbuf, HI_S32 *ps32LeftByte,
    HI_U16 *pu16Outbuf, HI_U32 *pu32OutLen, HI_U32 *pu32Chns)
{
    HI_S32 s32Ret;
    ADEC_AAC_DECODER_S *pstDecoder = NULL;
    HI_S32 s32Samples, s32FrmLen, s32SampleBytes;
    AACFrameInfo aacFrameInfo;

    HI_AUDIO_ASSERT(pDecoder != NULL);
    HI_AUDIO_ASSERT(pu8Inbuf != NULL);
    HI_AUDIO_ASSERT(ps32LeftByte != NULL);
    HI_AUDIO_ASSERT(pu16Outbuf != NULL);
    HI_AUDIO_ASSERT(pu32OutLen != NULL);
    HI_AUDIO_ASSERT(pu32Chns != NULL);

    *pu32Chns = 1; /* voice encoder only one channel */

    pstDecoder = (ADEC_AAC_DECODER_S *)pDecoder;

    s32FrmLen = AACDecodeFindSyncHeader_Adp(pstDecoder->pstAACState, pu8Inbuf, ps32LeftByte);
    if (s32FrmLen < 0) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "AAC decoder can't find sync header");
        return HI_ERR_ADEC_BUF_LACK;
    }

    /* Notes: pInbuf will updated */
    s32Ret = AACDecodeFrame_Adp(pstDecoder->pstAACState, pu8Inbuf, ps32LeftByte, (HI_S16 *)(HI_UINTPTR_T)pu16Outbuf);
    if (s32Ret) {
        printf("aac decoder failed!, s32Ret:0x%x\n", s32Ret);
        return s32Ret;
    }

    AACGetLastFrameInfo_Adp(pstDecoder->pstAACState, &aacFrameInfo);
    aacFrameInfo.nChans = ((aacFrameInfo.nChans != 0) ? aacFrameInfo.nChans : 1);
    /* samples per frame of one sound track */
    s32Samples = aacFrameInfo.outputSamps / aacFrameInfo.nChans;

    if ((s32Samples != AACLC_SAMPLES_PER_FRAME) && (s32Samples != AACPLUS_SAMPLES_PER_FRAME) &&
        (s32Samples != AACLD_SAMPLES_PER_FRAME)) {
        printf("aac decoder failed!\n");
        return HI_ERR_ADEC_DECODER_ERR;
    }

    s32SampleBytes = s32Samples * sizeof(HI_U16);
    *pu32Chns = aacFrameInfo.nChans;
    *pu32OutLen = s32SampleBytes;

    return s32Ret;
}

static HI_S32 GetAACFrmInfo(HI_VOID *pDecoder, HI_VOID *pInfo)
{
    ADEC_AAC_DECODER_S *pstDecoder = NULL;
    AACFrameInfo aacFrameInfo;
    AAC_FRAME_INFO_S *pstAacFrm = NULL;

    HI_AUDIO_ASSERT(pDecoder != NULL);
    HI_AUDIO_ASSERT(pInfo != NULL);

    pstDecoder = (ADEC_AAC_DECODER_S *)pDecoder;
    AACGetLastFrameInfo_Adp(pstDecoder->pstAACState, &aacFrameInfo);

    pstAacFrm = (AAC_FRAME_INFO_S *)pInfo;
    pstAacFrm->s32Samplerate = aacFrameInfo.sampRateOut;
    pstAacFrm->s32BitRate = aacFrameInfo.bitRate;
    pstAacFrm->s32Profile = aacFrameInfo.profile;
    pstAacFrm->s32TnsUsed = aacFrameInfo.tnsUsed;
    pstAacFrm->s32PnsUsed = aacFrameInfo.pnsUsed;
    pstAacFrm->s32ChnNum = aacFrameInfo.nChans;

    return HI_SUCCESS;
}


static HI_S32 CloseAACDecoder(HI_VOID *pDecoder)
{
    ADEC_AAC_DECODER_S *pstDecoder = NULL;

    HI_AUDIO_ASSERT(pDecoder != NULL);
    pstDecoder = (ADEC_AAC_DECODER_S *)pDecoder;
    AACFreeDecoder_Adp(pstDecoder->pstAACState);
    free(pstDecoder);

    return HI_SUCCESS;
}

static HI_S32 ResetAACDecoder(HI_VOID *pDecoder)
{
    ADEC_AAC_DECODER_S *pstDecoder = NULL;

    HI_AUDIO_ASSERT(pDecoder != NULL);
    pstDecoder = (ADEC_AAC_DECODER_S *)pDecoder;
    AACFreeDecoder_Adp(pstDecoder->pstAACState);

    /* create decoder */
    pstDecoder->pstAACState = AACInitDecoder_Adp((AACDECTransportType)pstDecoder->stAACAttr.enTransType);
    if (pstDecoder->pstAACState == NULL) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "AACResetDecoder failed");
        return HI_ERR_ADEC_DECODER_ERR;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AENC_AacInit(HI_VOID)
{
    HI_S32 s32Ret;
    HI_S32 s32Handle = 0;
    AENC_ENCODER_S stAac;

    s32Ret = InitAacAencLib();
    if (s32Ret) {
        return s32Ret;
    }

    stAac.enType = PT_AAC;
    s32Ret = snprintf_s(stAac.aszName, sizeof(stAac.aszName), sizeof(stAac.aszName) - 1, "Aac");
    if (s32Ret <= EOK) {
        printf("\n snprintf_s fail! ret = 0x%x", s32Ret);
        return s32Ret;
    }
    stAac.u32MaxFrmLen = MAX_AAC_MAINBUF_SIZE;
    stAac.pfnOpenEncoder = OpenAACEncoder;
    stAac.pfnEncodeFrm = EncodeAACFrm;
    stAac.pfnCloseEncoder = CloseAACEncoder;
    s32Ret = HI_MPI_AENC_RegisterEncoder(&s32Handle, &stAac);
    if (s32Ret) {
        return s32Ret;
    }
    g_AacEncHandle = s32Handle;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AENC_AacDeInit(HI_VOID)
{
    HI_S32 s32Ret;
    s32Ret = HI_MPI_AENC_UnRegisterEncoder(g_AacEncHandle);
    if (s32Ret) {
        return s32Ret;
    }

    DeInitAacAencLib();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ADEC_AacInit(HI_VOID)
{
    HI_S32 s32Ret;
    HI_S32 s32Handle = 0;
    ADEC_DECODER_S stAac;

    s32Ret = InitAacAdecLib();
    if (s32Ret) {
        return s32Ret;
    }

    stAac.enType = PT_AAC;
    s32Ret = snprintf_s(stAac.aszName, sizeof(stAac.aszName), sizeof(stAac.aszName) - 1, "Aac");
    if (s32Ret <= EOK) {
        printf("\n snprintf_s fail! ret = 0x%x", s32Ret);
        return s32Ret;
    }
    stAac.pfnOpenDecoder = OpenAACDecoder;
    stAac.pfnDecodeFrm = DecodeAACFrm;
    stAac.pfnGetFrmInfo = GetAACFrmInfo;
    stAac.pfnCloseDecoder = CloseAACDecoder;
    stAac.pfnResetDecoder = ResetAACDecoder;
    s32Ret = HI_MPI_ADEC_RegisterDecoder(&s32Handle, &stAac);
    if (s32Ret) {
        return s32Ret;
    }
    g_AacDecHandle = s32Handle;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ADEC_AacDeInit(HI_VOID)
{
    HI_S32 s32Ret;
    s32Ret = HI_MPI_ADEC_UnRegisterDecoder(g_AacDecHandle);
    if (s32Ret) {
        return s32Ret;
    }

    DeInitAacAdecLib();

    return HI_SUCCESS;
}
