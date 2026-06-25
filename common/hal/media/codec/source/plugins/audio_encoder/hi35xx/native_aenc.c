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

#include <limits.h>
#include <unistd.h>
#include <sys/select.h>

#include "ot_osal_user.h"
#include "ot_aacdec.h"
#include "aenc_common.h"
#include "audio_aac_intf.h"
#include "codec_common.h"
#include "ot_common_adec.h"
#include "ot_list.h"
#include "ot_mw_type.h"
#include "media_hal_common.h"
#include "ss_mpi_audio.h"
#include "ss_mpi_sys.h"


#define MODULE_NAME "CODEC_AENC_NATIVE"

#define AENC_CHN_NUM_START (OT_AENC_MAX_CHN_NUM / 2)
#define AENC_CHN_NUM_MAX (OT_AENC_MAX_CHN_NUM - AENC_CHN_NUM_START)

#define NATIVE_OUT_BUFFER_DEFAULT_NUM 5

/* samples per frame for G711 and G726 */
#define G711_G726_SAMPLES_PER_FRAME 480

#define AUDIO_HISI_HEAD_BYTE_LEN 4
#define AENC_GET_FRAME_TIMEOUT_MS 3000
#define AENC_GET_FRAME_BLOCK_TIMEOUT_MS 1000

#define MOD_ID(deviceid) \
            ((unsigned int)(((deviceid) >> 16) & 0xFF))

#define DEVICE_ID(deviceid) \
            ((unsigned int)(((deviceid) >> 8) & 0xFF))

#define CHAN_ID(deviceid) \
            ((unsigned int)((deviceid) & 0xFF))

typedef struct {
    ot_audio_stream nativeBuf;
    bool isUsed;
} NativeOutBuffer;

typedef struct {
    ot_aenc_chn chnID;
    bool used;
} AencChnStatus;

static AencChnStatus g_aencChnStatusList[AENC_CHN_NUM_MAX];
static pthread_mutex_t g_aencListLock;

typedef struct {
    ot_payload_type encType;
    AacType aacType;
    ot_aenc_chn aencChn;
    td_u32 bufSize;
    ot_mpp_chn srcChn;
    ot_mpp_chn destChn;
    bool hardwareMod;
    NativeOutBuffer nativeOutbufs[NATIVE_OUT_BUFFER_DEFAULT_NUM];
} NativeAencContext;

typedef struct {
    ot_payload_type      encType;
    AacType          enAACType;   /* AAC profile type */
    /* AAC bitrate (LC:16~320, OT_EAAC:24~128, OT_EAAC+:16~64, OT_AACLD:16~320, OT_AACELD:32~320) */
    td_u32               enBitRate;
    /* AAC sample rate (LC:8~48, OT_EAAC:16~48, OT_EAAC+:16~48, OT_AACLD:8~48, OT_AACELD:8~48) */
    ot_audio_sample_rate enSmpRate;
    ot_audio_snd_mode  enSoundMode; /* sound mode of inferent audio frame */
    td_u32              ptNumPerFrm;
    td_u32              bufSize;      /* buf size [2~OT_MAX_AUDIO_FRAME_NUM] */
} AvAenAttr;

static const CodecCapability g_aencCapEntries[] = {
    {
        MEDIA_MIMETYPE_AUDIO_AAC, AUDIO_ENCODER, {ALGIN_LEVEL_INVALID, ALGIN_LEVEL_INVALID}, {0, 0}, {0, 0},
        AUD_BITRATE_LEVEL1, AUD_BITRATE_LEVEL1, {{AAC_MAIN_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{PIX_FORMAT_INVALID}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },
    {
        MEDIA_MIMETYPE_AUDIO_G711A, AUDIO_ENCODER, {ALGIN_LEVEL_INVALID, ALGIN_LEVEL_INVALID}, {0, 0}, {0, 0},
        AUD_BITRATE_LEVEL1, AUD_BITRATE_LEVEL1, {{INVALID_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{PIX_FORMAT_INVALID}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },
    {
        MEDIA_MIMETYPE_AUDIO_G711U, AUDIO_ENCODER, {ALGIN_LEVEL_INVALID, ALGIN_LEVEL_INVALID}, {0, 0}, {0, 0},
        AUD_BITRATE_LEVEL1, AUD_BITRATE_LEVEL1, {{INVALID_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{PIX_FORMAT_INVALID}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },
    {
        MEDIA_MIMETYPE_AUDIO_G726, AUDIO_ENCODER, {ALGIN_LEVEL_INVALID, ALGIN_LEVEL_INVALID}, {0, 0}, {0, 0},
        AUD_BITRATE_LEVEL1, AUD_BITRATE_LEVEL1, {{INVALID_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{PIX_FORMAT_INVALID}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    }
};

typedef struct {
    AvCodecMime mimeUpperLayer;
    ot_payload_type mimeLowerLayer;
} MimePair;

typedef enum {
    AAC_PROFILE_AACLC      = 0,
    AAC_RPOFILE_EAAC       = 1,
    AAC_PROFILE_EAACPLUS   = 2,
    PROFILE_INVALID,
} NativeProfile;

typedef struct {
    ot_payload_type codecType;
    NativeProfile  profile;
    uint32_t numPonitsPerFrame;
    ot_audio_snd_mode soundMode;
} ConstraintTable;

typedef struct {
    AudioBitRate audioBitrate;
    AacBsp aacBitrate;
} AACBpsPairs;

static MimePair g_mimePairs[] = {
    { MEDIA_MIMETYPE_AUDIO_AAC, OT_PT_AAC },
    { MEDIA_MIMETYPE_AUDIO_G711A, OT_PT_G711A },
    { MEDIA_MIMETYPE_AUDIO_G711U, OT_PT_G711U },
    { MEDIA_MIMETYPE_AUDIO_G726, OT_PT_G726 },
};

static const ConstraintTable CONSTRAINTS_TABLE[] = {
    /* OT_AUDIO_SOUND_MODE_BUTT means no constraint in sound mode */
    {OT_PT_AAC, AAC_PROFILE_AACLC, 1024, OT_AUDIO_SOUND_MODE_BUTT},
    /* OT_AUDIO_SOUND_MODE_BUTT means no constraint in sound mode */
    {OT_PT_AAC, AAC_RPOFILE_EAAC, 2048, OT_AUDIO_SOUND_MODE_BUTT},
    /* OT_AUDIO_SOUND_MODE_STEREO is constraint in sound mode */
    {OT_PT_AAC, AAC_PROFILE_EAACPLUS, 2048, OT_AUDIO_SOUND_MODE_STEREO},
};

static AACBpsPairs g_aacBpsPairs[] = {
    { AUD_BITRATE_8K, AAC_BPS_8K },
    { AUD_BITRATE_16K, AAC_BPS_16K },
    { AUD_BITRATE_22K, AAC_BPS_22K },
    { AUD_BITRATE_24K, AAC_BPS_24K },
    { AUD_BITRATE_32K, AAC_BPS_32K },
    { AUD_BITRATE_48K, AAC_BPS_48K },
    { AUD_BITRATE_64K, AAC_BPS_64K },
    { AUD_BITRATE_96K, AAC_BPS_96K },
    { AUD_BITRATE_128K, AAC_BPS_128K },
    { AUD_BITRATE_132K, AAC_BPS_132K },
    { AUD_BITRATE_144K, AAC_BPS_144K },
    { AUD_BITRATE_192K, AAC_BPS_192K },
    { AUD_BITRATE_256K, AAC_BPS_256K },
    { AUD_BITRATE_265K, AAC_BPS_265K },
    { AUD_BITRATE_288K, AAC_BPS_288K },
    { AUD_BITRATE_320K, AAC_BPS_320K },
};

static bool g_aencMngInited = TD_FALSE;

static void AencMngInit()
{
    if (g_aencMngInited == TD_FALSE) {
        for (int i = 0; i < AENC_CHN_NUM_MAX; i++) {
            g_aencChnStatusList[i].used = TD_FALSE;
            g_aencChnStatusList[i].chnID = i;
        }
        (void)pthread_mutex_init(&g_aencListLock, NULL);
        g_aencMngInited = TD_TRUE;
    }
}

static void AencMngDeinit()
{
    if (g_aencMngInited == TD_TRUE) {
        for (int i = 0; i < AENC_CHN_NUM_MAX; i++) {
            if (g_aencChnStatusList[i].used) {
                MEDIA_HAL_LOGE(MODULE_NAME, "aenc channel %d not be freed err", g_aencChnStatusList[i].chnID);
            }
        }
        (void)pthread_mutex_destroy(&g_aencListLock);
        g_aencMngInited = TD_FALSE;
    }
}

static td_s32 AencMngGetFreeChn(ot_aenc_chn *outChn)
{
    MEDIA_HAL_LOCK(g_aencListLock);
    for (int i = 0; i < AENC_CHN_NUM_MAX; i++) {
        if (!g_aencChnStatusList[i].used) {
            g_aencChnStatusList[i].used = TD_TRUE;
            *outChn = g_aencChnStatusList[i].chnID;
            MEDIA_HAL_UNLOCK(g_aencListLock);
            return TD_SUCCESS;
        }
    }
    MEDIA_HAL_UNLOCK(g_aencListLock);
    MEDIA_HAL_LOGE(MODULE_NAME, "could get free aenc channel");
    return TD_FAILURE;
}

static td_s32 AencMngFreeChn(ot_aenc_chn aencChn)
{
    MEDIA_HAL_LOCK(g_aencListLock);

    for (int i = 0; i < AENC_CHN_NUM_MAX; i++) {
        if (g_aencChnStatusList[i].chnID == aencChn) {
            g_aencChnStatusList[i].used = TD_FALSE;
            MEDIA_HAL_UNLOCK(g_aencListLock);
            return TD_SUCCESS;
        }
    }
    MEDIA_HAL_UNLOCK(g_aencListLock);
    MEDIA_HAL_LOGE(MODULE_NAME, "freehandle input adec handle invalid : %d", aencChn);
    return TD_FAILURE;
}

static void InitAencNativeOutBuffers(NativeAencContext *aencCtx)
{
    for (int i = 0; i < NATIVE_OUT_BUFFER_DEFAULT_NUM; i++) {
        aencCtx->nativeOutbufs[i].isUsed = TD_FALSE;
        if (memset_s(&(aencCtx->nativeOutbufs[i].nativeBuf), sizeof(ot_audio_stream),
                     0x00, sizeof(ot_audio_stream)) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memset_s nativeBuf failed");
        }
    }
}

static ot_audio_stream* GetFreeAencNativeOutbuffer(NativeAencContext *aencCtx)
{
    for (int i = 0; i < NATIVE_OUT_BUFFER_DEFAULT_NUM; i++) {
        if (aencCtx->nativeOutbufs[i].isUsed == TD_FALSE) {
            aencCtx->nativeOutbufs[i].isUsed = TD_TRUE;
            return &(aencCtx->nativeOutbufs[i].nativeBuf);
        }
    }
    return NULL;
}

static td_bool IsValidAencNativeOutbuffer(const NativeAencContext *aencCtx,
    const ot_audio_stream *audioFrameInfo)
{
    td_bool isValid = TD_FALSE;

    for (int i = 0; i < NATIVE_OUT_BUFFER_DEFAULT_NUM; i++) {
        if (aencCtx->nativeOutbufs[i].isUsed == TD_TRUE &&
            &(aencCtx->nativeOutbufs[i].nativeBuf) == audioFrameInfo) {
            isValid = TD_TRUE;
            break;
        }
    }

    return isValid;
}

static td_s32 ReleaseAencNativeOutbuffer(NativeAencContext *aencCtx, ot_audio_stream *audioFrameInfo)
{
    if (audioFrameInfo == NULL || aencCtx == NULL) {
        return TD_FAILURE;
    }

    bool isMatch = false;
    for (int i = 0; i < NATIVE_OUT_BUFFER_DEFAULT_NUM; i++) {
        isMatch = aencCtx->nativeOutbufs[i].isUsed == TD_TRUE &&
                  &(aencCtx->nativeOutbufs[i].nativeBuf) == audioFrameInfo;
        if (isMatch) {
            aencCtx->nativeOutbufs[i].isUsed = TD_FALSE;
            if (memset_s(&(aencCtx->nativeOutbufs[i].nativeBuf), sizeof(ot_audio_stream),
                         0x00, sizeof(ot_audio_stream)) != EOK) {
                MEDIA_HAL_LOGE(MODULE_NAME, "memset_s nativeBuf failed");
            }
            return TD_SUCCESS;
        }
    }
    return TD_FAILURE;
}

static ot_payload_type ConvertMime(AvCodecMime mimeFromUpperLayer)
{
    size_t len = sizeof(g_mimePairs) / sizeof(g_mimePairs[0]);
    bool isFound = false;
    ot_payload_type tmpLower = OT_PT_BUTT;
    for (size_t i = 0; i < len; i++) {
        if (mimeFromUpperLayer == g_mimePairs[i].mimeUpperLayer) {
            isFound = TD_TRUE;
            tmpLower = g_mimePairs[i].mimeLowerLayer;
            break;
        }
    }
    if (!isFound) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not found lowermime corespording to uppermime");
        return OT_PT_BUTT;
    }
    return tmpLower;
}

static AacType ConvertAudioProfile(Profile profile)
{
    switch (profile) {
        case AAC_LC_PROFILE:
            return AAC_TYPE_AACLC;
        case AAC_HE_V1_PROFILE:
            return AAC_TYPE_EAAC;
        case AAC_HE_V2_PROFILE:
            return AAC_TYPE_EAACPLUS;
        case AAC_LD_PROFILE:
            return AAC_TYPE_AACLD;
        case AAC_ELD_PROFILE:
            return AAC_TYPE_AACELD;
        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "Invalid profile: 0x%x", profile);
            return AAC_TYPE_AACLC;
    }
}

static ot_audio_sample_rate ConvertSampleRate(uint32_t sampleRate)
{
    switch (sampleRate) {
        case AUD_SAMPLE_RATE_8000:
            return OT_AUDIO_SAMPLE_RATE_8000;
        case AUD_SAMPLE_RATE_11025:
            return OT_AUDIO_SAMPLE_RATE_11025;
        case AUD_SAMPLE_RATE_12000:
            return OT_AUDIO_SAMPLE_RATE_12000;
        case AUD_SAMPLE_RATE_16000:
            return OT_AUDIO_SAMPLE_RATE_16000;
        case AUD_SAMPLE_RATE_22050:
            return OT_AUDIO_SAMPLE_RATE_22050;
        case AUD_SAMPLE_RATE_24000:
            return OT_AUDIO_SAMPLE_RATE_24000;
        case AUD_SAMPLE_RATE_32000:
            return OT_AUDIO_SAMPLE_RATE_32000;
        case AUD_SAMPLE_RATE_44100:
            return OT_AUDIO_SAMPLE_RATE_44100;
        case AUD_SAMPLE_RATE_48000:
            return OT_AUDIO_SAMPLE_RATE_48000;
        case AUD_SAMPLE_RATE_64000:
            return OT_AUDIO_SAMPLE_RATE_64000;
        case AUD_SAMPLE_RATE_96000:
            return OT_AUDIO_SAMPLE_RATE_96000;
        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "Invalid sampleRate: %u", sampleRate);
            return OT_AUDIO_SAMPLE_RATE_48000;
    }
}

static bool ConvertAACBitRate(uint32_t inBitrate, ot_aac_bps *outBitrate)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "ConvertAACBitRate bit_rate: %u", inBitrate);
    size_t len = sizeof(g_aacBpsPairs) / sizeof(g_aacBpsPairs[0]);
    bool isFound = false;
    for (size_t i = 0; i < len; i++) {
        if (inBitrate == g_aacBpsPairs[i].audioBitrate) {
            isFound = TD_TRUE;
            *outBitrate = (ot_aac_bps)g_aacBpsPairs[i].aacBitrate;
            break;
        }
    }
    if (!isFound) {
        printf("Invalid AAC bit_rate: %u\n", inBitrate);
        return TD_FALSE;
    }
    return TD_TRUE;
}

static bool ConvertG726BitRate(uint32_t inBitrate, ot_g726_bps *outBitrate)
{
    MEDIA_HAL_LOGE(MODULE_NAME, "ConvertG726BitRate bit_rate: %u", inBitrate);
    switch (inBitrate) {
        case AUD_BITRATE_16K:
            *outBitrate = OT_G726_16K;
            break;
        case AUD_BITRATE_24K:
            *outBitrate = OT_G726_24K;
            break;
        case AUD_BITRATE_32K:
            *outBitrate = OT_G726_32K;
            break;
        case AUD_BITRATE_40K:
            *outBitrate = OT_G726_40K;
            break;
        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "Invalid G726 bit_rate: %u", inBitrate);
            return TD_FALSE;
    }
    return TD_TRUE;
}

static bool ConvertBitRate(ot_payload_type encType, uint32_t inBitrate, void *outBitrate)
{
    if (outBitrate == NULL) {
        return TD_FALSE;
    }

    if (encType == OT_PT_AAC) {
        return ConvertaacBitrate(inBitrate, (ot_aac_bps *)outBitrate);
    } else if (encType == OT_PT_G726) {
        return ConvertG726BitRate(inBitrate, (ot_g726_bps *)outBitrate);
    }
    return TD_FALSE;
}

static ot_audio_snd_mode ConvertSoundMode(uint32_t soundMode)
{
    switch (soundMode) {
        case AUD_SOUND_MODE_MONO:
            return OT_AUDIO_SOUND_MODE_MONO;
        case AUD_SOUND_MODE_STEREO:
            return OT_AUDIO_SOUND_MODE_STEREO;
        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "Invalid soundMode: %u", soundMode);
            return OT_AUDIO_SOUND_MODE_MONO;
    }
}

static td_s32 CheckAencAttribute(const Param *attrSrc)
{
    if (attrSrc->key == KEY_MIMETYPE) {
            if (attrSrc->size != sizeof(AvCodecMime)) {
                MEDIA_HAL_LOGE(MODULE_NAME, "param kParamIndexMimeType size wrong size is %d", attrSrc->size);
                return TD_FAILURE;
            }
    } else {
        if (attrSrc->key == KEY_AUDIO_PROFILE || attrSrc->key == KEY_SAMPLE_RATE ||
            attrSrc->key == KEY_BITRATE || attrSrc->key == KEY_SOUND_MODE ||
            attrSrc->key == KEY_POINT_NUM_PER_FRAME || attrSrc->key == KEY_BUFFERSIZE) {
                if (attrSrc->size != sizeof(uint32_t)) {
                    MEDIA_HAL_LOGE(MODULE_NAME, "key %d param size wrong, size is %d", attrSrc->key, attrSrc->size);
                    return TD_FAILURE;
                }
            }
    }
    return TD_SUCCESS;
}

static td_s32 FillAencParam(const Param *attrSrc, AvAenAttr *attrDst)
{
    switch (attrSrc->key) {
        case KEY_MIMETYPE: {
            AvCodecMime tmpUpper = *((AvCodecMime *)(attrSrc->val));
            ot_payload_type tmpLower = ConvertMime(tmpUpper);
            if (tmpLower == OT_PT_BUTT) {
                return TD_FAILURE;
            }
            attrDst->encType = tmpLower;
            break;
        }
        case KEY_AUDIO_PROFILE: {
            attrDst->enAACType = ConvertAudioProfile(*((uint32_t *)(attrSrc->val)));
            break;
        }
        case KEY_SAMPLE_RATE: {
            attrDst->enSmpRate = ConvertSampleRate(*((uint32_t *)(attrSrc->val)));
            break;
        }
        case KEY_BITRATE: {
            attrDst->enBitRate = *(uint32_t *)(attrSrc->val);
            break;
        }
        case KEY_SOUND_MODE: {
            attrDst->enSoundMode = ConvertSoundMode(*((uint32_t *)(attrSrc->val)));
            break;
        }
        case KEY_POINT_NUM_PER_FRAME: {
            attrDst->ptNumPerFrm = *((uint32_t *)(attrSrc->val));
            break;
        }
        case KEY_BUFFERSIZE: {
            attrDst->bufSize = *((uint32_t *)(attrSrc->val));
            break;
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "aenc not need param:%d", attrSrc->key);
            break;
        }
    }
    return TD_SUCCESS;
}

static td_s32 FillAencAttribute(const Param *attrSrc, int lenSrc, AvAenAttr *attrDst)
{
    for (int32_t i = 0; i < lenSrc; i++) {
        int32_t ret = CheckAencAttribute(&attrSrc[i]);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "CheckAencAttribute failed ret:%d", ret);
            return ret;
        }
        ret = FillAencParam(&attrSrc[i], attrDst);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    MEDIA_HAL_LOGE(MODULE_NAME, " encType %d enBitRate %u",  attrDst->encType, attrDst->enBitRate);
    return TD_SUCCESS;
}

// only search in default type with count is equal to 1
static bool IsValidAencType(ot_payload_type type)
{
    // sdk inner aenc contains: PCM, G711A, G711U, G726, LPCM, ADPCMA
    if (type != OT_PT_G711A && type != OT_PT_G711U && type != OT_PT_G726 &&
        type != OT_PT_AAC && type != OT_PT_HEAAC) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

static NativeProfile ConvertAACType(AacType aacType)
{
    switch (aacType) {
        case AAC_TYPE_AACLC:
            return AAC_PROFILE_AACLC;
        case AAC_TYPE_EAAC:
            return AAC_RPOFILE_EAAC;
        case AAC_TYPE_EAACPLUS:
            return AAC_PROFILE_EAACPLUS;
        default:
            return PROFILE_INVALID;
    }
}

static td_s32 CheckAencConfig(const AvAenAttr *aencAttr)
{
    size_t num = sizeof(CONSTRAINTS_TABLE) / sizeof(CONSTRAINTS_TABLE[0]);
    bool configBad = false;
    for (size_t i = 0; i < num; i++) {
        bool isMatch = CONSTRAINTS_TABLE[i].codecType == aencAttr->encType &&
            CONSTRAINTS_TABLE[i].profile == ConvertAACType(aencAttr->enAACType);
        if (!isMatch) {
            continue;
        }

        if (aencAttr->enAACType == AAC_TYPE_AACLD || aencAttr->enAACType == AAC_TYPE_AACELD) {
            MEDIA_HAL_LOGE(MODULE_NAME, " not support OT_AACLD or OT_AACELD");
            configBad = true;
        }
        if (CONSTRAINTS_TABLE[i].soundMode != OT_AUDIO_SOUND_MODE_BUTT &&
            CONSTRAINTS_TABLE[i].soundMode != aencAttr->enSoundMode) {
            MEDIA_HAL_LOGE(MODULE_NAME, " [err]:%s now is:%d should be:%d", "invalid sound mode",
                aencAttr->enSoundMode, CONSTRAINTS_TABLE[i].soundMode);
            configBad = true;
            break;
        }
    }
    if (configBad) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static bool ConvertAACAttr(const AvAenAttr *aencAttr, ot_aenc_chn_attr *chnAttr)
{
    ot_aenc_attr_aac *aencAac = (ot_aenc_attr_aac *)malloc(sizeof(ot_aenc_attr_aac));
    if (aencAac == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc aencAac failed");
        return TD_FALSE;
    }
    if (memset_s(aencAac, sizeof(ot_aenc_attr_aac), 0x00, sizeof(ot_aenc_attr_aac)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    aencAac->aac_type = (ot_aac_type)aencAttr->enAACType;   /* AAC profile type */
    /* AAC bitrate (LC:16~320, OT_EAAC:24~128, OT_EAAC+:16~64, OT_AACLD:16~320, OT_AACELD:32~320) */
    if (!ConvertBitRate(aencAttr->encType, aencAttr->enBitRate, &(aencAac->bit_rate))) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AAC ConvertBitRate failed");
        free(aencAac);
        return TD_FALSE;
    }
    /* AAC sample rate (LC:8~48, OT_EAAC:16~48, OT_EAAC+:16~48, OT_AACLD:8~48, OT_AACELD:8~48) */
    aencAac->sample_rate = aencAttr->enSmpRate;
    aencAac->bit_width = OT_AUDIO_BIT_WIDTH_16;  /* AAC bit width (only support 16bit) */
    aencAac->snd_mode = aencAttr->enSoundMode; /* sound mode of inferent audio frame */
    if ((aencAttr->enAACType == AAC_TYPE_AACLD) || (aencAttr->enAACType == AAC_TYPE_AACELD)) {
        aencAac->transport_type = OT_AAC_TRANSPORT_TYPE_LOAS;
        MEDIA_HAL_LOGE(MODULE_NAME, "[Info]:%s", "OT_AACLD or OT_AACELD not support AAC_TRANS_TYPE_ADTS");
    } else {
        aencAac->transport_type = OT_AAC_TRANSPORT_TYPE_ADTS;
    }
    aencAac->band_width = 0; /* targeted audio bandwidth in Hz (0 or 1000~enSmpRate/2), the default is 0 */

    chnAttr->type = aencAttr->encType;
    const uint32_t numPointsTable[] = {
        AACLC_SAMPLES_PER_FRAME, // AAC_TYPE_AACLC
        AACPLUS_SAMPLES_PER_FRAME, // AAC_TYPE_EAAC
        AACPLUS_SAMPLES_PER_FRAME, // AAC_TYPE_EAACPLUS
    };
    size_t size = sizeof(numPointsTable) / sizeof(numPointsTable[0]);
    if ((size_t)aencAttr->enAACType >= size) {
        free(aencAac);
        return TD_FALSE;
    }
    chnAttr->point_num_per_frame = numPointsTable[aencAttr->enAACType];
    chnAttr->buf_size = aencAttr->bufSize;      /* buf size [2~OT_MAX_AUDIO_FRAME_NUM] */
    chnAttr->value = aencAac;

    return TD_TRUE;
}

static bool ConvertG711Attr(const AvAenAttr *aencAttr, ot_aenc_chn_attr *chnAttr)
{
    ot_aenc_attr_g711 *stG711Attr = NULL;
    stG711Attr = (ot_aenc_attr_g711 *)malloc(sizeof(ot_aenc_attr_g711));
    if (stG711Attr == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc ot_aenc_attr_g711 failed");
        return TD_FALSE;
    }
    if (memset_s(stG711Attr, sizeof(ot_aenc_attr_g711), 0x00, sizeof(ot_aenc_attr_g711)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    chnAttr->type = aencAttr->encType;
    chnAttr->point_num_per_frame = G711_G726_SAMPLES_PER_FRAME;
    chnAttr->buf_size = OT_MAX_AUDIO_FRAME_NUM;
    chnAttr->value = stG711Attr;
    return TD_TRUE;
}

static bool ConvertG726Attr(const AvAenAttr *aencAttr, ot_aenc_chn_attr *chnAttr)
{
    ot_aenc_attr_g726 *stG726Attr = (ot_aenc_attr_g726 *)malloc(sizeof(ot_aenc_attr_g726));
    if (stG726Attr == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc ot_aenc_attr_g726 failed");
        return TD_FALSE;
    }
    if (memset_s(stG726Attr, sizeof(ot_aenc_attr_g726), 0x00, sizeof(ot_aenc_attr_g726)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    if (!ConvertBitRate(aencAttr->encType, aencAttr->enBitRate, &(stG726Attr->g726bps))) {
        MEDIA_HAL_LOGE(MODULE_NAME, "G726 ConvertBitRate failed");
        free(stG726Attr);
        return TD_FALSE;
    }
    chnAttr->type = aencAttr->encType;
    chnAttr->point_num_per_frame = G711_G726_SAMPLES_PER_FRAME;
    chnAttr->buf_size = OT_MAX_AUDIO_FRAME_NUM;
    chnAttr->value = stG726Attr;
    return TD_TRUE;
}

static bool ConvertAvAenAttrToAencChnAttr(const AvAenAttr *aencAttr, ot_aenc_chn_attr *chnAttr)
{
    if (aencAttr == NULL || chnAttr == NULL) {
        return TD_FALSE;
    }

    if (aencAttr->encType == OT_PT_AAC) {
        return ConvertAACAttr(aencAttr, chnAttr);
    } else if (aencAttr->encType == OT_PT_G711A || aencAttr->encType == OT_PT_G711U) {
        return ConvertG711Attr(aencAttr, chnAttr);
    } else if (aencAttr->encType == OT_PT_G726) {
        return ConvertG726Attr(aencAttr, chnAttr);
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "aenc not support encType:%d", (int32_t)aencAttr->encType);
        return TD_FALSE;
    }
}

static td_s32 AencCreateChn(const AvAenAttr *aenAttr, const void *priv, ot_aenc_chn aencChn)
{
    MEDIA_HAL_UNUSED(priv);
    ot_aenc_chn_attr stAencAttr;
    if (memset_s(&stAencAttr, sizeof(ot_aenc_chn_attr), 0x00, sizeof(ot_aenc_chn_attr)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    td_s32 ret = TD_SUCCESS;
    if (!ConvertAvAenAttrToAencChnAttr(aenAttr, &stAencAttr)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConvertAvAenAttrToAencChnAttr failed");
        ret = TD_FAILURE;
        goto FREE;
    }

    ret = ss_mpi_aenc_create_chn(aencChn, &stAencAttr);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_aenc_create_chn fail,ret:0x%x", ret);
        goto FREE;
    }
FREE:
    if (stAencAttr.value) {
        free(stAencAttr.value);
        stAencAttr.value = NULL;
    }
    return ret;
}

static td_s32 NATIVE_AENC_Create(void **aencHdl, const Param *attr, int len)
{
    CHK_NULL_RETURN(aencHdl, TD_FAILURE, "aencHdl null");
    CHK_NULL_RETURN(attr, TD_FAILURE, "attr null");

    AvAenAttr aencAttr = { 0 };
    int32_t ret = FillAencAttribute(attr, len, &aencAttr);
    if (ret != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "fill aencAttr fail");
        return ret;
    }
    if (CheckAencConfig(&aencAttr) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (IsValidAencType(aencAttr.encType) == TD_FALSE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not registered any adecoder,create aenc fail");
        return AENC_ERR_NOTREG;
    }

    NativeAencContext *aencCtx = (NativeAencContext*)malloc(sizeof(NativeAencContext));
    if (aencCtx == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc NativeAencContext failed");
        return TD_FAILURE;
    }
    if (memset_s(aencCtx, sizeof(NativeAencContext), 0x00, sizeof(NativeAencContext)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }

    ot_aenc_chn aencChn = 0;
    ret = AencMngGetFreeChn(&aencChn);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AdecMngGetFreeChn failed Ret: %d", ret);
        ret = AENC_ERR_NOFREE_CHANNEL;
        goto FREE;
    }
    aencCtx->aencChn = aencChn;
    ret = AencCreateChn(&aencAttr, NULL, aencChn);
    if (ret != TD_SUCCESS) {
        if (AencMngFreeChn(aencCtx->aencChn) != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AdecMngFreeChn error!!");
        }
        ret = AENC_ERR_AENC_ERR;
        goto FREE;
    }
    InitAencNativeOutBuffers(aencCtx);
    aencCtx->encType = aencAttr.encType;
    aencCtx->hardwareMod = TD_FALSE;
    *aencHdl = aencCtx;
    MEDIA_HAL_LOGI(MODULE_NAME, "NATIVE_AENC_Create succeed");
    return TD_SUCCESS;

FREE:
    free(aencCtx);
    return ret;
}

static td_s32 NativeSetSourceId(NativeAencContext *aencCtx, uint32_t deviceId)
{
    CHK_NULL_RETURN(aencCtx, TD_FAILURE, "aencCtx null");
    aencCtx->srcChn.mod_id = MOD_ID(deviceId);
    aencCtx->srcChn.dev_id = DEVICE_ID(deviceId);
    aencCtx->srcChn.chn_id = CHAN_ID(deviceId);
    aencCtx->destChn.mod_id = OT_ID_AENC;
    aencCtx->destChn.dev_id = 0;
    aencCtx->destChn.chn_id = aencCtx->aencChn;
    MEDIA_HAL_LOGE(MODULE_NAME, "HI_MPI_SYS_BindenModId %d aCapDev %d aiChn %d aEncChn %d",
        aencCtx->srcChn.mod_id, aencCtx->srcChn.dev_id,
        aencCtx->srcChn.chn_id, aencCtx->destChn.chn_id);

    td_s32 ret = ss_mpi_sys_bind(&aencCtx->srcChn, &aencCtx->destChn);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_bind failed ret %#x", ret);
        return ret;
    }
    aencCtx->hardwareMod = TD_TRUE;
    return TD_SUCCESS;
}

static td_s32 NATIVE_AENC_SetParameter(void *aencHdl, const Param *param)
{
    CHK_NULL_RETURN(aencHdl, TD_FAILURE, "aencHdl null");
    CHK_NULL_RETURN(param, TD_FAILURE, "param null");
    CHK_NULL_RETURN(param->val, TD_FAILURE, "param val null");

    NativeAencContext *aencCtx = (NativeAencContext*)aencHdl;
    switch (param->key) {
        case KEY_DEVICE_ID: {
            if (param->size != sizeof(uint32_t)) {
                MEDIA_HAL_LOGE(MODULE_NAME, "param PARAM_INDEX_DEVICE_ID wrong");
                return TD_FAILURE;
            }
            uint32_t deviceId = *((uint32_t *)(param->val));
            td_s32 ret = NativeSetSourceId(aencCtx, deviceId);
            if (ret != TD_SUCCESS) {
                return TD_FAILURE;
            }
            break;
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "SetParameter invalid param key");
            break;
        }
    }
    return TD_SUCCESS;
}

static td_s32 NATIVE_AENC_Destroy(void *aencHdl)
{
    CHK_NULL_RETURN(aencHdl, TD_FAILURE, "aencHdl null");
    NativeAencContext *nativeAencCtx = (NativeAencContext*)aencHdl;
    td_s32 ret = ss_mpi_aenc_destroy_chn(nativeAencCtx->aencChn);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_aenc_destroy_chn error Ret: %d", ret);
    }
    ret = AencMngFreeChn(nativeAencCtx->aencChn);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AdecMngFreeChn %d error.", (td_u32)nativeAencCtx->aencChn);
    }
    free(nativeAencCtx);
    return TD_SUCCESS;
}

static td_s32 NATIVE_AENC_Start(void *aencHdl)
{
    MEDIA_HAL_UNUSED(aencHdl);
    return TD_SUCCESS;
}

static td_s32 NATIVE_AENC_Stop(void *aencHdl)
{
    CHK_NULL_RETURN(aencHdl, TD_FAILURE, "aencHdl null");
    NativeAencContext *nativeAencCtx = (NativeAencContext*)aencHdl;
    if (nativeAencCtx->hardwareMod) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_unbind in");
        td_s32 ret = ss_mpi_sys_unbind(&nativeAencCtx->srcChn, &nativeAencCtx->destChn);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_unbind fail,ret:%x", ret);
        }
        nativeAencCtx->hardwareMod = TD_FALSE;
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_unbind aCapDev %d aiChn %d aEncChn %d",
            nativeAencCtx->srcChn.dev_id, nativeAencCtx->srcChn.chn_id, nativeAencCtx->destChn.chn_id);
    }
    MEDIA_HAL_LOGD(MODULE_NAME, "NATIVE_AENC_Stop out");
    return TD_SUCCESS;
}

static td_s32 NATIVE_AENC_SendFrame(void *aencHdl, const InputInfo *inputInfo, uint32_t frameIndex, bool block)
{
    MEDIA_HAL_UNUSED(aencHdl);
    MEDIA_HAL_UNUSED(inputInfo);
    MEDIA_HAL_UNUSED(frameIndex);
    MEDIA_HAL_UNUSED(block);
    return TD_SUCCESS;
}

static td_s32 FillAencOutputInfo(ot_payload_type encType, const ot_audio_stream *src, OutputInfo *dst)
{
    if (src == NULL || dst == NULL) {
        return TD_FAILURE;
    }
    dst->type = AUDIO_ENCODER;
    dst->timeStamp = (int64_t)src->time_stamp;
    dst->sequence = src->seq;
    dst->flag |= ~STREAM_FLAG_KEYFRAME;

    dst->buffers = (CodecBufferInfo*)malloc(sizeof(CodecBufferInfo));
    if (dst->buffers == NULL) {
        return TD_FAILURE;
    }
    dst->buffers[0].type = BUFFER_TYPE_VIRTUAL;
    dst->buffers[0].addr = (uint8_t*)src->stream;
    dst->buffers[0].length = src->len;
    dst->vendorPrivate = (void*)src;

    // G711/G726 need to delete 4 bytes frame header information adec will auto add this info.
    if (encType == OT_PT_G711A || encType == OT_PT_G711U || encType == OT_PT_G726) {
        dst->buffers[0].addr = ((uint8_t*)src->stream) + AUDIO_HISI_HEAD_BYTE_LEN;
        dst->buffers[0].length = src->len - AUDIO_HISI_HEAD_BYTE_LEN;
    }

    return TD_SUCCESS;
}

static td_s32 NATIVE_AENC_GetStream(void *aencHdl, OutputInfo *outInfo, bool block)
{
    CHK_NULL_RETURN(aencHdl, TD_FAILURE, "aencHdl null");
    CHK_NULL_RETURN(outInfo, TD_FAILURE, "outInfo null");
    NativeAencContext *nativeAencCtx = (NativeAencContext*)aencHdl;

    td_s32 ret;
    osal_fd_set readFds;
    td_s32 aencFd = ss_mpi_aenc_get_fd(nativeAencCtx->aencChn);
    if (aencFd < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_aenc_get_fd failed ret %#x", aencFd);
        return AENC_ERR_AENC_ERR;
    }

    ot_audio_stream *audioStream = GetFreeAencNativeOutbuffer(nativeAencCtx);
    if (audioStream == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "no free native out buffer for aenc");
        return OT_ERR_AAC_UNKNOWN;
    }

    OSAL_FD_ZERO(&readFds);
    OSAL_FD_SET(aencFd, &readFds);
    struct timeval timeoutVal;

    timeoutVal.tv_sec = block ? 30 : 1; /* 30: time */
    timeoutVal.tv_usec = 0;
    ret = OSAL_SELECT(aencFd + 1, &readFds, NULL, NULL, &timeoutVal);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "select failed!");
        ReleaseAencNativeOutbuffer(nativeAencCtx, audioStream);
        return OT_ERR_AAC_UNKNOWN;
    } else if (ret == 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "chn [%d] get aenc stream time out", nativeAencCtx->aencChn);
        ReleaseAencNativeOutbuffer(nativeAencCtx, audioStream);
        return MEDIA_HAL_TIMEOUT;
    } else {
        if (OSAL_FD_ISSET(aencFd, &readFds)) {
            ret = ss_mpi_aenc_get_stream(nativeAencCtx->aencChn, audioStream, TD_FALSE);
            if (ret != TD_SUCCESS) {
                MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_aenc_get_stream failed with %#x !", ret);
                ReleaseAencNativeOutbuffer(nativeAencCtx, audioStream);
                return ret;
            }
        }
    }

    ret = FillAencOutputInfo(nativeAencCtx->encType, audioStream, outInfo);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 NATIVE_AENC_ReleaseStream(void *aencHdl, OutputInfo *outInfo)
{
    CHK_NULL_RETURN(aencHdl, TD_FAILURE, "aencHdl null");
    CHK_NULL_RETURN(outInfo, TD_FAILURE, "outInfo null");
    CHK_NULL_RETURN(outInfo->vendorPrivate, TD_FAILURE, "outInfo vendorPrivate null");
    NativeAencContext *nativeAencCtx = (NativeAencContext*)aencHdl;
    if (outInfo->buffers != NULL) {
        free(outInfo->buffers);
        outInfo->buffers = NULL;
    }
    ot_audio_stream *audioStream = (ot_audio_stream *)outInfo->vendorPrivate;
    CHK_COND_RETURN(!IsValidAencNativeOutbuffer(nativeAencCtx, audioStream), MEDIA_HAL_INVALID_PARAM,
        "audio stream invalid");
    td_s32 ret = ss_mpi_aenc_release_stream(nativeAencCtx->aencChn, audioStream);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_aenc_release_stream error.ret=0x%x", ret);
    }
    ret = ReleaseAencNativeOutbuffer(nativeAencCtx, audioStream);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ReleaseAencNativeOutbuffer error.ret=0x%x", ret);
        return TD_FAILURE;
    }
    return ret;
}

static td_s32 NATIVE_AENC_Reset(void *aencHdl)
{
    MEDIA_HAL_UNUSED(aencHdl);
    return TD_SUCCESS;
}

static td_s32 NATIVE_AENC_Invoke(void *aencHdl, td_u32 cmd, void *arg1, void *arg2)
{
    MEDIA_HAL_UNUSED(aencHdl);
    MEDIA_HAL_UNUSED(cmd);
    MEDIA_HAL_UNUSED(arg1);
    MEDIA_HAL_UNUSED(arg2);
    return TD_SUCCESS;
}

void *AEncoderGetImpl(void)
{
    AvAudioEncoder *encoder = (AvAudioEncoder*)malloc(sizeof(AvAudioEncoder));
    if (encoder == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc fail");
        return NULL;
    }
    if (memset_s(encoder, sizeof(AvAudioEncoder), 0x00, sizeof(AvAudioEncoder)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }

    encoder->pluginCommon.pfnCreate = NATIVE_AENC_Create;
    encoder->pluginCommon.isHardwarePlugin = true;
    encoder->pluginCommon.pfnDestroy = NATIVE_AENC_Destroy;
    encoder->pluginCommon.pfnStart = NATIVE_AENC_Start;
    encoder->pluginCommon.pfnStop = NATIVE_AENC_Stop;
    encoder->pfnQueueInput = NATIVE_AENC_SendFrame;
    encoder->pfnDequeueOutput = NATIVE_AENC_GetStream;
    encoder->pfnQueueOutput = NATIVE_AENC_ReleaseStream;
    encoder->pluginCommon.pfnReset = NATIVE_AENC_Reset;
    encoder->pluginCommon.pfnInvoke = NATIVE_AENC_Invoke;
    encoder->pluginCommon.pfnSetParameter = NATIVE_AENC_SetParameter;

    size_t validCapNum = sizeof(g_aencCapEntries) / sizeof(g_aencCapEntries[0]);
    encoder->pluginCommon.validCapbilityNum = (int)validCapNum;
    for (size_t i = 0; i < validCapNum; i++) {
        encoder->pluginCommon.capbilites[i] = g_aencCapEntries[i];
    }

    AencMngInit();

    return encoder;
}

td_s32 AEncoderImplUnRegister()
{
    td_s32 ret = HiMpiAencAacDeInit();
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "HiMpiAencAacDeInit error Ret: %x", ret);
    }
    AencMngDeinit();
    return ret;
}

