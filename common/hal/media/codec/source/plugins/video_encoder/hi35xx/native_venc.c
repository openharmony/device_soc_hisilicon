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
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include "codec_common.h"
#include "ot_osal_user.h"
#include "ot_common_vpss.h"
#include "ot_common_rc.h"
#include "ot_math.h"
#include "media_hal_common.h"
#include "ss_mpi_sys.h"
#include "ss_mpi_vpss.h"
#include "ss_mpi_venc.h"
#include "venc_common.h"

#define MODULE_NAME "CODEC_VENC_NATIVE"

#define MOD_ID(deviceid) \
            ((unsigned int)(((deviceid) >> 16) & 0xFF))

#define DEVICE_ID(deviceid) \
            ((unsigned int)(((deviceid) >> 8) & 0xFF))

#define CHAN_ID(deviceid) \
            ((unsigned int)((deviceid) & 0xFF))

#define NATIVE_OUT_BUFFER_DEFAULT_NUM 5
#define FRAME_BUF_RATIO_MIN 70

#define PARAM_UPPER_LIMIT 500

#define BITRATE_1M 1024  /* kbps */
#define BITRATE_2M 2048  /* kbps */
#define BITRATE_3M 3072  /* kbps */
#define BITRATE_4M 4096  /* kbps */
#define BITRATE_5M 5120  /* kbps */
#define BITRATE_8M 8192  /* kbps */
#define BITRATE_10M 10240  /* kbps */
#define BITRATE_12M 12288  /* kbps */
#define BITRATE_15M 15360  /* kbps */
#define BITRATE_20M 20480  /* kbps */
#define BITRATE_24M 24576  /* kbps */

#define DEFAULT_FPS 60
#define BIT_LEN 16

#define PACK_CNT_MAX 6

#define JPEG_ENC_WIDTH_MAX 8192
#define JPEG_ENC_HEIGHT_MAX 8192
#define JPEG_ENC_RESOLUITION_MAX (JPEG_ENC_WIDTH_MAX * JPEG_ENC_HEIGHT_MAX)

#define JPEG_ENC_WIDTH_MIN 32
#define JPEG_ENC_HEIGHT_MIN 32

#ifdef __HI3516DV300__
#define AVC_HEVC_ENC_WIDTH_MAX 3072
#define AVC_HEVC_ENC_HEIGHT_MAX 3072
#define AVC_HEVC_ENC_RESOLUITION_MAX (3072 * 1728)
#else
#define AVC_HEVC_ENC_WIDTH_MAX 3840
#define AVC_HEVC_ENC_HEIGHT_MAX 3840
#define AVC_HEVC_ENC_RESOLUITION_MAX (3840 * 2160)
#endif
#define AVC_HEVC_ENC_WIDTH_MIN 114
#define AVC_HEVC_ENC_HEIGHT_MIN 114

#define DEFAULT_INTRA_REFREASH_NUM 6

typedef struct {
    AvCodecMime mimeUpperLayer;
    ot_payload_type mimeLowerLayer;
} MimePair;

static MimePair g_mimePairs[] = {
    { MEDIA_MIMETYPE_IMAGE_JPEG, OT_PT_JPEG },
    { MEDIA_MIMETYPE_VIDEO_AVC, OT_PT_H264 },
    { MEDIA_MIMETYPE_VIDEO_HEVC, OT_PT_H265 },
    { MEDIA_MIMETYPE_AUDIO_AAC, OT_PT_AAC },
};

typedef struct {
    ot_venc_stream nativeBuf;
    td_bool isUsed;
} NativeOutBuffer;

typedef struct {
    AvCodecMime mime;
    ot_payload_type encType; /* encode type */
    VenCodeRcMode rcMode; /* encode rc mode */
    VenCodeGopMode gopMode; /* encode gop type */
    PicSize picSize; /* encode picsize */
    td_u32 profile; /* encode level */
    td_u32 frameRate;
    td_u32 bitRate; /* kbps */
} AvVencAttr;

typedef struct {
    ot_payload_type encType;
    ot_vdec_chn vencChn;
    td_u32 bufSize;
    td_bool hardwareMod;
    ot_mpp_chn srcChn;
    ot_mpp_chn destChn;
    NativeOutBuffer nativeOutbufs[NATIVE_OUT_BUFFER_DEFAULT_NUM];
} NativeVencContext;

static const CodecCapability g_vencCapEntries[] = {
    {
        MEDIA_MIMETYPE_VIDEO_HEVC, VIDEO_ENCODER, {ALGIN_LEVEL_2, ALGIN_LEVEL_2},
        {AVC_HEVC_ENC_WIDTH_MIN, AVC_HEVC_ENC_HEIGHT_MIN}, {AVC_HEVC_ENC_WIDTH_MAX, AVC_HEVC_ENC_HEIGHT_MAX},
        VID_BITRATE_LEVEL1, VID_BITRATE_LEVEL2, {{HEVC_MAIN_PROFILE}, 1}, {{HEVC_LEVEL_MAIN_2}, 1},
        {{YVU_SEMIPLANAR_420}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },
    {
        MEDIA_MIMETYPE_IMAGE_JPEG, VIDEO_ENCODER, {ALGIN_LEVEL_2, ALGIN_LEVEL_2},
        {AVC_HEVC_ENC_WIDTH_MIN, AVC_HEVC_ENC_HEIGHT_MIN}, {AVC_HEVC_ENC_WIDTH_MAX, AVC_HEVC_ENC_HEIGHT_MAX},
        VID_BITRATE_LEVEL1, VID_BITRATE_LEVEL2, {{INVALID_PROFILE}, 1}, {{INVALID_LEVEL}, 1},
        {{YVU_SEMIPLANAR_420}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },
    {
        MEDIA_MIMETYPE_VIDEO_AVC, VIDEO_ENCODER, {ALGIN_LEVEL_2, ALGIN_LEVEL_2},
        {JPEG_ENC_WIDTH_MIN, JPEG_ENC_HEIGHT_MAX}, {JPEG_ENC_WIDTH_MAX, JPEG_ENC_HEIGHT_MAX},
        VID_BITRATE_LEVEL1, VID_BITRATE_LEVEL2, {{AVC_MAIN_PROFILE}, 1}, {{AVC_LEVEL_1}, 1},
        {{YVU_SEMIPLANAR_420}, 1}, 1, 1, ALLOCATE_INPUT_BUFFER_USER | ALLOCATE_OUTPUT_BUFFER_CODEC, 0
    },
};

typedef struct {
    ot_venc_chn chnID;
    td_bool used;
} VencChnStatus;

#define BREATHE_INIT_PARAM_PATH "/storage/data/codec_config.ini"
static ot_venc_debreath_effect g_breatheEffect;
#define PATH_MAX_LEN 128

static td_bool g_vencMngInited = TD_FALSE;
static pthread_mutex_t g_vencListLock;

ot_venc_chn g_vencChn = 0;

#define VENC_CHN_NUM_MAX 3
static VencChnStatus g_vencChnStatusList[VENC_CHN_NUM_MAX];

typedef struct {
    PicSize Resolution;
    uint32_t width;
    uint32_t height;
} ResolutionInfo;

static const ResolutionInfo g_resolution[RESOLUTION_INVALID] = {
    {RESOLUTION_CIF, 352, 288},         {RESOLUTION_360P, 640, 360},        {RESOLUTION_D1_PAL, 720, 576},
    {RESOLUTION_D1_NTSC, 720, 480},     {RESOLUTION_720P, 1280, 720},       {RESOLUTION_1080P, 1920, 1080},
    {RESOLUTION_2560X1440, 2560, 1440}, {RESOLUTION_2592X1520, 2592, 1520}, {RESOLUTION_2592X1536, 2592, 1536},
    {RESOLUTION_2592X1944, 2592, 1944}, {RESOLUTION_2688X1536, 2688, 1536}, {RESOLUTION_2716X1524, 2716, 1524},
    {RESOLUTION_3840X2160, 3840, 2160}, {RESOLUTION_4096X2160, 4096, 2160}, {RESOLUTION_3000X3000, 3000, 3000},
    {RESOLUTION_4000X3000, 4000, 3000}, {RESOLUTION_7680X4320, 7680, 4320}, {RESOLUTION_3840X8640, 3840, 8640}
};

int16_t g_fixedBitRateNormal[RESOLUTION_INVALID] = {
    BITRATE_1M,  BITRATE_1M,  BITRATE_2M,  BITRATE_2M,
    BITRATE_2M,  BITRATE_2M,  BITRATE_3M,  BITRATE_3M,
    BITRATE_3M,  BITRATE_3M,  BITRATE_3M,  BITRATE_3M,
    BITRATE_5M,  BITRATE_5M,  BITRATE_8M, BITRATE_10M,
    BITRATE_20M, BITRATE_20M
};

int16_t g_variableBitRateFactor[RESOLUTION_INVALID] = {
    BITRATE_1M,  BITRATE_1M,  BITRATE_1M,  BITRATE_1M,
    BITRATE_1M,  BITRATE_2M,  BITRATE_3M,  BITRATE_3M,
    BITRATE_3M,  BITRATE_3M,  BITRATE_3M,  BITRATE_3M,
    BITRATE_5M,  BITRATE_5M,  BITRATE_5M, BITRATE_5M,
    BITRATE_5M, BITRATE_5M
};

int16_t g_fixedBitRateForH264Cbr[RESOLUTION_INVALID] = {
    BITRATE_1M,  BITRATE_1M,  BITRATE_2M,  BITRATE_2M,
    BITRATE_3M,  BITRATE_2M,  BITRATE_4M,  BITRATE_4M,
    BITRATE_4M,  BITRATE_4M,  BITRATE_4M,  BITRATE_4M,
    BITRATE_8M,  BITRATE_8M,  BITRATE_10M, BITRATE_12M,
    BITRATE_24M, BITRATE_24M
};

static void GetBreatheEffectConfig(void)
{
    char path[PATH_MAX_LEN] = { 0 };
    if (snprintf_s(path, PATH_MAX_LEN, PATH_MAX_LEN - 1, "%s", BREATHE_INIT_PARAM_PATH) < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s error!\n");
        return;
    }
    g_breatheEffect.enable = 0;
}

static void VencMngInit()
{
    if (g_vencMngInited == TD_FALSE) {
        for (int i = 0; i < VENC_CHN_NUM_MAX; i++) {
            g_vencChnStatusList[i].used = TD_FALSE;
            g_vencChnStatusList[i].chnID = i;
        }
        (void)pthread_mutex_init(&g_vencListLock, NULL);
        GetBreatheEffectConfig();
        g_vencMngInited = TD_TRUE;
    }
}

static void VencMngDeinit()
{
    if (g_vencMngInited == TD_TRUE) {
        for (int i = 0; i < VENC_CHN_NUM_MAX; i++) {
            if (g_vencChnStatusList[i].used) {
                MEDIA_HAL_LOGE(MODULE_NAME, "venc channel %d not be freed err", g_vencChnStatusList[i].chnID);
            }
        }
        (void)pthread_mutex_destroy(&g_vencListLock);
        if (memset_s(&g_breatheEffect, sizeof(ot_venc_debreath_effect), 0, sizeof(ot_venc_debreath_effect)) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        }
        g_vencMngInited = TD_FALSE;
    }
}

static td_s32 VencMngGetFreeChn(ot_venc_chn *outChn)
{
    CHK_NULL_RETURN(outChn, TD_FAILURE, "outChn null");
    MEDIA_HAL_LOCK(g_vencListLock);
    for (int i = 0; i < VENC_CHN_NUM_MAX; i++) {
        if (!g_vencChnStatusList[i].used) {
            g_vencChnStatusList[i].used = TD_TRUE;
            *outChn = g_vencChnStatusList[i].chnID;
            MEDIA_HAL_UNLOCK(g_vencListLock);
            return TD_SUCCESS;
        }
    }
    MEDIA_HAL_UNLOCK(g_vencListLock);
    MEDIA_HAL_LOGE(MODULE_NAME, "could get free aenc channel");
    return TD_FAILURE;
}

static td_s32 VencMngFreeChn(ot_venc_chn vencChn)
{
    MEDIA_HAL_LOCK(g_vencListLock);

    for (int i = 0; i < VENC_CHN_NUM_MAX; i++) {
        if (g_vencChnStatusList[i].chnID == vencChn) {
            g_vencChnStatusList[i].used = TD_FALSE;
            MEDIA_HAL_UNLOCK(g_vencListLock);
            return TD_SUCCESS;
        }
    }
    MEDIA_HAL_UNLOCK(g_vencListLock);
    MEDIA_HAL_LOGE(MODULE_NAME, "freehandle input vdec handle invalid : %d", vencChn);
    return TD_FAILURE;
}

static void InitVencNativeOutBuffers(NativeVencContext *vencCtx)
{
    for (int i = 0; i < NATIVE_OUT_BUFFER_DEFAULT_NUM; i++) {
        vencCtx->nativeOutbufs[i].isUsed = TD_FALSE;
        if (memset_s(&(vencCtx->nativeOutbufs[i].nativeBuf), sizeof(ot_venc_stream),
            0x00, sizeof(ot_venc_stream)) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        }
    }
}

static ot_venc_stream* GetFreeVencNativeOutBuffer(NativeVencContext *vencCtx)
{
    for (int i = 0; i < NATIVE_OUT_BUFFER_DEFAULT_NUM; i++) {
        if (vencCtx->nativeOutbufs[i].isUsed == TD_FALSE) {
            vencCtx->nativeOutbufs[i].isUsed = TD_TRUE;
            return &(vencCtx->nativeOutbufs[i].nativeBuf);
        }
    }
    return NULL;
}

static td_bool IsValidVencNativeOutbuffer(const NativeVencContext *vencCtx, const ot_venc_stream *videoStreamInfo)
{
    td_bool isValid = TD_FALSE;

    for (int i = 0; i < NATIVE_OUT_BUFFER_DEFAULT_NUM; i++) {
        if (vencCtx->nativeOutbufs[i].isUsed == TD_TRUE &&
            &(vencCtx->nativeOutbufs[i].nativeBuf) == videoStreamInfo) {
            isValid = TD_TRUE;
            break;
        }
    }

    return isValid;
}

static td_s32 ReleaseVencNativeOutbuffer(NativeVencContext *vencCtx, ot_venc_stream *videoStreamInfo)
{
    if (videoStreamInfo == NULL) {
        return TD_FAILURE;
    }

    td_bool isMatch = TD_FALSE;
    for (int i = 0; i < NATIVE_OUT_BUFFER_DEFAULT_NUM; i++) {
        isMatch = vencCtx->nativeOutbufs[i].isUsed == TD_TRUE &&
                  &(vencCtx->nativeOutbufs[i].nativeBuf) == videoStreamInfo;
        if (isMatch) {
            vencCtx->nativeOutbufs[i].isUsed = TD_FALSE;
            if (memset_s(&(vencCtx->nativeOutbufs[i].nativeBuf), sizeof(ot_venc_stream),
                0x00, sizeof(ot_venc_stream)) != EOK) {
                MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
            }
            return TD_SUCCESS;
        }
    }
    return TD_FAILURE;
}

static ot_payload_type ConvertMime(AvCodecMime mimeFromUpperLayer)
{
    size_t len = sizeof(g_mimePairs) / sizeof(g_mimePairs[0]);
    td_bool isFound = TD_FALSE;
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
static td_s32 FillVencEncType(const Param *attrSrc, int size, const AvCodecMime *avCodecMime, AvVencAttr *attrDst)
{
    MEDIA_HAL_UNUSED(attrSrc);
    AvCodecMime tmpUpper = MEDIA_MIMETYPE_INVALID;
    if (size != sizeof(AvCodecMime)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param kParamIndexMimeType size wrong");
        return TD_FAILURE;
    }
    tmpUpper = *avCodecMime;
    ot_payload_type tmpLower = ConvertMime(tmpUpper);
    if (tmpLower == OT_PT_BUTT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "convert fail");
        return TD_FAILURE;
    }
    MEDIA_HAL_LOGD(MODULE_NAME, "param kParamIndexMimeType tmpLower:%d", tmpLower);
    attrDst->encType = tmpLower;
    return TD_SUCCESS;
}

static int32_t GetRcMode(const Param *attr, AvVencAttr *attrDst)
{
    if (attr->size != sizeof(VenCodeRcMode)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param venc rc mode size wrong");
        return MEDIA_HAL_ERR;
    }
    attrDst->rcMode = *((VenCodeRcMode *)(attr->val));
    return MEDIA_HAL_OK;
}

static int32_t GetGopMode(const Param *attr, AvVencAttr *attrDst)
{
    if (attr->size != sizeof(VenCodeGopMode)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param venc gop mode size wrong");
        return MEDIA_HAL_ERR;
    }
    attrDst->gopMode = *((VenCodeGopMode *)(attr->val));
    return MEDIA_HAL_OK;
}

static int32_t GetPictureSize(const Param *attr, AvVencAttr *attrDst)
{
    if (attr->size != sizeof(PicSize)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param pic size wrong");
        return MEDIA_HAL_ERR;
    }
    attrDst->picSize = *((PicSize *)(attr->val));
    return MEDIA_HAL_OK;
}

static int32_t GetVideoProfile(const Param *attr, AvVencAttr *attrDst)
{
    if (attr->size != sizeof(uint32_t)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param profile wrong");
        return MEDIA_HAL_ERR;
    }
    attrDst->profile = *((uint32_t *)(attr->val));
    return MEDIA_HAL_OK;
}

static int32_t GetFrameRate(const Param *attr, AvVencAttr *attrDst)
{
    if (attr->size != sizeof(uint32_t)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param frame rate wrong");
        return MEDIA_HAL_ERR;
    }
    attrDst->frameRate = *((uint32_t *)(attr->val));
    return MEDIA_HAL_OK;
}

static int32_t GetBitRate(const Param *attr, AvVencAttr *attrDst)
{
    if (attr->size != sizeof(uint32_t)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param frame rate wrong");
        return MEDIA_HAL_ERR;
    }
    attrDst->bitRate = *((uint32_t *)(attr->val));
    return MEDIA_HAL_OK;
}

static td_s32 FillVencAttribute(const Param *attrSrc, int lenSrc, AvVencAttr *attrDst)
{
    MEDIA_HAL_LOGD(MODULE_NAME, "in");
    int32_t ret = MEDIA_HAL_OK;

    for (int i = 0; i < lenSrc; i++) {
        ret = TD_SUCCESS;
        switch (attrSrc[i].key) {
            case KEY_MIMETYPE:
                ret = FillVencEncType(attrSrc, attrSrc[i].size, (AvCodecMime *)(attrSrc[i].val), attrDst);
                break;
            case KEY_VIDEO_RC_MODE:
                ret = GetRcMode(&attrSrc[i], attrDst);
                break;
            case KEY_VIDEO_GOP_MODE:
                ret = GetGopMode(&attrSrc[i], attrDst);
                break;
            case KEY_VIDEO_PIC_SIZE:
                ret = GetPictureSize(&attrSrc[i], attrDst);
                break;
            case KEY_VIDEO_PROFILE:
                ret = GetVideoProfile(&attrSrc[i], attrDst);
                break;
            case KEY_VIDEO_FRAME_RATE:
                ret = GetFrameRate(&attrSrc[i], attrDst);
                break;
            case KEY_BITRATE:
                ret = GetBitRate(&attrSrc[i], attrDst);
                break;
            default:
                MEDIA_HAL_LOGE(MODULE_NAME, "venc not need param:%d", attrSrc[i].key);
                break;
        }

        if (ret != MEDIA_HAL_OK) {
            break;
        }
    }
    return ret;
}

static td_s32 GetPicWidthHeight(PicSize enPicSize, ot_size *pstSize)
{
    if ((int32_t)enPicSize >= RESOLUTION_INVALID || enPicSize < 0) {
        return TD_FAILURE;
    }

    const ResolutionInfo *info = &g_resolution[enPicSize];
    pstSize->width = info->width;
    pstSize->height = info->height;

    return TD_SUCCESS;
}

static td_s32 CodecVencSnapCreate(ot_venc_chn vencChn, const AvVencAttr *vencParam, td_bool dcf_en)
{
    td_s32 ret;
    ot_venc_chn_attr stVencChnAttr;
    ot_size stPicSize;
    if (memset_s(&stVencChnAttr, sizeof(ot_venc_chn_attr), 0, sizeof(ot_venc_chn_attr)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    ret = GetPicWidthHeight(vencParam->picSize, &stPicSize);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "PIC size [%d] is invalid", vencParam->picSize);
        return ret;
    }
    /* *****************************************
     jpeg Create Venc Channel
    ***************************************** */
    stVencChnAttr.venc_attr.type = OT_PT_JPEG;
    stVencChnAttr.venc_attr.max_pic_width = stPicSize.width;
    stVencChnAttr.venc_attr.max_pic_height = stPicSize.height;
    stVencChnAttr.venc_attr.pic_width = stPicSize.width;
    stVencChnAttr.venc_attr.pic_height = stPicSize.height;
    stVencChnAttr.venc_attr.buf_size =
        ALIGN_UP(stPicSize.width, BIT_LEN) * ALIGN_UP(stPicSize.height, BIT_LEN);
    stVencChnAttr.venc_attr.is_by_frame = TD_TRUE; /* get stream mode is field mode  or frame mode */
    stVencChnAttr.venc_attr.jpeg_attr.dcf_en = dcf_en;
    stVencChnAttr.venc_attr.jpeg_attr.mpf_cfg.large_thumbnail_num = 0;
    stVencChnAttr.venc_attr.jpeg_attr.recv_mode = OT_VENC_PIC_RECV_SINGLE;
    stVencChnAttr.venc_attr.profile = 0; // jpeg baseline
    ret = ss_mpi_venc_create_chn(vencChn, &stVencChnAttr);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_create_chn [%d] faild with %#x!", vencChn, ret);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 CodecVencGetGopAttr(ot_venc_gop_mode gopMode, ot_venc_gop_attr *pstGopAttr)
{
    switch (gopMode) {
        case OT_VENC_GOP_MODE_NORMAL_P:
            pstGopAttr->gop_mode  = OT_VENC_GOP_MODE_NORMAL_P;
            pstGopAttr->normal_p.ip_qp_delta = 0x2; /* 2: QP variance between P frame and I frame */
            break;
        case OT_VENC_GOP_MODE_SMART_P:
            pstGopAttr->gop_mode  = OT_VENC_GOP_MODE_SMART_P;
            pstGopAttr->smart_p.bg_qp_delta  = 0x4; /* 4: QP variance between P frame and Bg frame */
            pstGopAttr->smart_p.vi_qp_delta  = 0x2; /* 2: QP variance between P frame and virtual I frame */
            pstGopAttr->smart_p.bg_interval =  0x5a; /* 90: Interval of the long-term reference frame */
            break;

        case OT_VENC_GOP_MODE_DUAL_P:
            pstGopAttr->gop_mode  = OT_VENC_GOP_MODE_DUAL_P;
            pstGopAttr->dual_p.ip_qp_delta  = 0x4; /* 4: QP variance between P frame and I frame */
            pstGopAttr->dual_p.sp_qp_delta  = 0x2; /* 2: QP variance between P frame and special P frame */
            pstGopAttr->dual_p.sp_interval = 0x3; /* 3: Interval of the special P frames,  1 is not supported */
            break;

        case OT_VENC_GOP_MODE_BIPRED_B:
            pstGopAttr->gop_mode  = OT_VENC_GOP_MODE_BIPRED_B;
            pstGopAttr->bipred_b.b_qp_delta  = -2; /* -2: QP variance between P frame and B frame */
            pstGopAttr->bipred_b.ip_qp_delta = 0x3; /* 3: QP variance between P frame and I frame */
            pstGopAttr->bipred_b.b_frame_num   = 0x2;  /* 2: Number of B frames */
            break;

        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "not support the gop mode !");
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static uint32_t GetVencBitRateByParamNormal(PicSize enSize, td_u32 frameRate)
{
    int16_t fixedBitRateNormal = g_fixedBitRateNormal[enSize];
    int32_t bitRate = (int32_t)fixedBitRateNormal + g_variableBitRateFactor[enSize] * (int32_t)frameRate / DEFAULT_FPS;

    return (uint32_t)bitRate;
}

static uint32_t GetVencBitRateByParamForH264Cbr(PicSize enSize, td_u32 frameRate)
{
    int16_t fixedBitRateNormal = g_fixedBitRateForH264Cbr[enSize];
    int32_t bitRate = fixedBitRateNormal + g_variableBitRateFactor[enSize] * (int32_t)frameRate / DEFAULT_FPS;

    return (uint32_t)bitRate;
}

static td_s32 VencCorrectRcParam(ot_venc_chn vencChn)
{
    ot_venc_rc_param stRcParam = { 0 };
    ot_venc_chn_attr stChnAttr = { 0 };

    td_s32 ret = ss_mpi_venc_get_chn_attr(vencChn, &stChnAttr);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_get_chn_attr failed!");
        return TD_FAILURE;
    }

    ret = ss_mpi_venc_get_rc_param(vencChn, &stRcParam);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_get_rc_param failed!");
        return TD_FAILURE;
    }

    if (stChnAttr.rc_attr.rc_mode == OT_VENC_RC_MODE_H264_CBR) {
        stRcParam.h264_cbr_param.max_reencode_times = 0;
    } else if (stChnAttr.rc_attr.rc_mode == OT_VENC_RC_MODE_H264_VBR) {
        stRcParam.h264_vbr_param.max_reencode_times = 0;
    } else if (stChnAttr.rc_attr.rc_mode == OT_VENC_RC_MODE_H265_CBR) {
        stRcParam.h264_cbr_param.max_reencode_times = 0;
    } else if (stChnAttr.rc_attr.rc_mode == OT_VENC_RC_MODE_H265_VBR) {
        stRcParam.h264_vbr_param.max_reencode_times = 0;
    } else {
        return TD_SUCCESS;
    }
    ret = ss_mpi_venc_set_rc_param(vencChn, &stRcParam);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SetRcParam failed!");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 ConstructVencH265RcCBRAttribute(const AvVencAttr *vencParam,
    td_u32 statsTime, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h265_cbr stH265CbrAttr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H265_CBR;
    stH265CbrAttr.gop = DEFAULT_FPS;
    stH265CbrAttr.stats_time = statsTime;       /* stream rate statics time(s) */
    stH265CbrAttr.src_frame_rate = vencParam->frameRate;  /* input (vi) frame rate */
    stH265CbrAttr.dst_frame_rate = vencParam->frameRate; /* target frame rate */
    stH265CbrAttr.bit_rate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    td_s32 ret = memcpy_s(&(rcAttr->h265_cbr), sizeof(ot_venc_h265_cbr), &stH265CbrAttr,
        sizeof(ot_venc_h265_cbr));
    return ret;
}


static td_s32 ConstructVencH265RcFIXQPAttribute(const AvVencAttr *vencParam, ot_venc_rc_attr *rcAttr)
{
    rcAttr->rc_mode = OT_VENC_RC_MODE_H265_FIXQP;
    ot_venc_h265_fixqp stH265FixQpAttr;
    stH265FixQpAttr.gop = DEFAULT_FPS; /* idr frame gop */
    stH265FixQpAttr.src_frame_rate = vencParam->frameRate;
    stH265FixQpAttr.dst_frame_rate = vencParam->frameRate;
    stH265FixQpAttr.i_qp = 25; /* i frame Qp value is 25 */
    stH265FixQpAttr.p_qp = 30; /* p frame Qp value is 30 */
    stH265FixQpAttr.b_qp = 32; /* b frame Qp value is 32 */
    td_s32 ret = memcpy_s(&(rcAttr->h265_fixqp), sizeof(ot_venc_h265_fixqp), &stH265FixQpAttr,
        sizeof(ot_venc_h265_fixqp));
    return ret;
}

static td_s32 ConstructVencH265RcVBRAttribute(const AvVencAttr *vencParam,
    td_u32 stats_time, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h265_vbr *h265VbrAttr = &rcAttr->h265_vbr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H265_VBR;
    h265VbrAttr->gop = DEFAULT_FPS;
    h265VbrAttr->stats_time = stats_time;
    h265VbrAttr->src_frame_rate = vencParam->frameRate;
    h265VbrAttr->dst_frame_rate = vencParam->frameRate;
    uint32_t bitRate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    h265VbrAttr->max_bit_rate = bitRate;
    return 0;
}

static td_s32 ConstructVencH265RcAVBRAttribute(const AvVencAttr *vencParam,
    td_u32 stats_time, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h265_avbr *h265AVbrAttr = &rcAttr->h265_avbr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H265_AVBR;
    h265AVbrAttr->gop = DEFAULT_FPS;
    h265AVbrAttr->stats_time = stats_time;
    h265AVbrAttr->src_frame_rate = vencParam->frameRate;
    h265AVbrAttr->dst_frame_rate = vencParam->frameRate;
    uint32_t bitRate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    h265AVbrAttr->max_bit_rate = bitRate;
    return 0;
}

static td_s32 ConstructVencH265RcQVBRAttribute(const AvVencAttr *vencParam,
    td_u32 stats_time, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h265_qvbr *h265QVbrAttr = &rcAttr->h265_qvbr;
    rcAttr->rc_mode = DEFAULT_FPS;
    h265QVbrAttr->gop = DEFAULT_FPS;
    h265QVbrAttr->stats_time = stats_time;
    h265QVbrAttr->src_frame_rate = vencParam->frameRate;
    h265QVbrAttr->dst_frame_rate = vencParam->frameRate;
    uint32_t bitRate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    h265QVbrAttr->target_bit_rate = bitRate;

    return 0;
}

static td_s32 ConstructVencH265RcCVBRAttribute(const AvVencAttr *vencParam,
    td_u32 stats_time, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h265_cvbr *h265CVbrAttr = &rcAttr->h265_cvbr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H265_CVBR;
    h265CVbrAttr->gop = DEFAULT_FPS;
    h265CVbrAttr->stats_time = stats_time;
    h265CVbrAttr->src_frame_rate = vencParam->frameRate;
    h265CVbrAttr->dst_frame_rate = vencParam->frameRate;
    h265CVbrAttr->long_term_stats_time = 1;
    h265CVbrAttr->short_term_stats_time = stats_time;

    uint32_t bitRate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    h265CVbrAttr->long_term_max_bit_rate = bitRate;
    h265CVbrAttr->max_bit_rate = bitRate;
    h265CVbrAttr->long_term_min_bit_rate = (uint32_t)g_fixedBitRateNormal[vencParam->picSize] / 0x2;
    return 0;
}

static td_s32 ConstructVencH265RcQPMAPAttribute(const AvVencAttr *vencParam,
    td_u32 stats_time, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h265_qpmap stH265QpMapAttr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H265_QPMAP;
    stH265QpMapAttr.gop = DEFAULT_FPS;
    stH265QpMapAttr.stats_time = stats_time;
    stH265QpMapAttr.src_frame_rate = vencParam->frameRate;
    stH265QpMapAttr.dst_frame_rate = vencParam->frameRate;
    stH265QpMapAttr.qpmap_mode = OT_VENC_RC_QPMAP_MODE_MEAN_QP;
    td_s32 ret = memcpy_s(&(rcAttr->h265_qpmap), sizeof(ot_venc_h265_qpmap), &stH265QpMapAttr,
        sizeof(ot_venc_h265_qpmap));
    return ret;
}


static td_s32 ConstructVencH265RcAttribute(const AvVencAttr *vencParam, td_u32 statsTime,
    ot_venc_chn_attr *stVencChnAttr)
{
    CHK_NULL_RETURN(vencParam, TD_FAILURE, "vencParam null");
    CHK_NULL_RETURN(stVencChnAttr, TD_FAILURE, "stVencChnAttr null");

    td_s32 ret = TD_FAILURE;
    if (vencParam->rcMode == VENCOD_RC_CBR) {
        ret = ConstructVencH265RcCBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_FIXQP) {
        ret = ConstructVencH265RcFIXQPAttribute(vencParam, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_VBR) {
        ret = ConstructVencH265RcVBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_AVBR) {
        ret = ConstructVencH265RcAVBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_QVBR) {
        ret = ConstructVencH265RcQVBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_CVBR) {
        ret = ConstructVencH265RcCVBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_QPMAP) {
        ret = ConstructVencH265RcQPMAPAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "rcMode(%d) not support", vencParam->rcMode);
        return TD_FAILURE;
    }
    if (ret != EOK) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 ConstructVencH264RcCBRAttribute(const AvVencAttr *vencParam,
    td_u32 statsTime, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h264_cbr *h264Cbr = &rcAttr->h264_cbr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H264_CBR;
    h264Cbr->gop = DEFAULT_FPS;                 /* the interval of IFrame */
    h264Cbr->stats_time = statsTime;       /* stream rate statics time(s) */
    h264Cbr->src_frame_rate = vencParam->frameRate;  /* input (vi) frame rate */
    h264Cbr->dst_frame_rate = vencParam->frameRate; /* target frame rate */
    h264Cbr->bit_rate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamForH264Cbr(vencParam->picSize, vencParam->frameRate);
    return 0;
}

static td_s32 ConstructVencH264RcFIXQPAttribute(const AvVencAttr *vencParam,
    td_u32 statsTime, ot_venc_rc_attr *rcAttr)
{
    MEDIA_HAL_UNUSED(statsTime);
    ot_venc_h264_fixqp h264_fixqp;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H264_FIXQP;
    h264_fixqp.gop = DEFAULT_FPS;
    h264_fixqp.src_frame_rate = vencParam->frameRate;
    h264_fixqp.dst_frame_rate = vencParam->frameRate;
    h264_fixqp.i_qp = 25; /* i frame Qp value is 25 */
    h264_fixqp.p_qp = 30; /* p frame Qp value is 30 */
    h264_fixqp.b_qp = 32; /* b frame Qp value is 32 */
    td_s32 ret = memcpy_s(&rcAttr->h264_fixqp, sizeof(ot_venc_h264_fixqp), &h264_fixqp,
        sizeof(ot_venc_h264_fixqp));
    return ret;
}

static td_s32 ConstructVencH264RcVBRAttribute(const AvVencAttr *vencParam,
    td_u32 statsTime, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h264_vbr h264_vbr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H264_VBR;
    h264_vbr.gop = DEFAULT_FPS;
    h264_vbr.stats_time = statsTime;
    h264_vbr.src_frame_rate = vencParam->frameRate;
    h264_vbr.dst_frame_rate = vencParam->frameRate;
    h264_vbr.max_bit_rate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    td_s32 ret = memcpy_s(&rcAttr->h264_vbr, sizeof(ot_venc_h264_vbr), &h264_vbr,
        sizeof(ot_venc_h264_vbr));
    return ret;
}

static td_s32 ConstructVencH264RcAVBRAttribute(const AvVencAttr *vencParam,
    td_u32 statsTime, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h264_vbr h264_avbr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H264_AVBR;
    h264_avbr.gop = DEFAULT_FPS;
    h264_avbr.stats_time = statsTime;
    h264_avbr.src_frame_rate = vencParam->frameRate;
    h264_avbr.dst_frame_rate = vencParam->frameRate;
    h264_avbr.max_bit_rate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    td_s32 ret = memcpy_s(&rcAttr->h264_avbr, sizeof(ot_venc_h264_avbr), &h264_avbr,
        sizeof(ot_venc_h264_avbr));
    return ret;
}

static td_s32 ConstructVencH264RcQVBRAttribute(const AvVencAttr *vencParam,
    td_u32 statsTime, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h264_qvbr h264_qvbr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H264_QVBR;
    h264_qvbr.gop = DEFAULT_FPS;
    h264_qvbr.stats_time = statsTime;
    h264_qvbr.src_frame_rate = vencParam->frameRate;
    h264_qvbr.dst_frame_rate = vencParam->frameRate;
    h264_qvbr.target_bit_rate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    td_s32 ret = memcpy_s(&rcAttr->h264_qvbr, sizeof(ot_venc_h264_qvbr), &h264_qvbr,
        sizeof(ot_venc_h264_qvbr));
    return ret;
}

static td_s32 ConstructVencH264RcCVBRAttribute(const AvVencAttr *vencParam,
    td_u32 statsTime, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h264_cvbr *h264CVbr = &rcAttr->h264_cvbr;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H264_CVBR;
    h264CVbr->gop = DEFAULT_FPS;
    h264CVbr->stats_time = statsTime;
    h264CVbr->src_frame_rate = vencParam->frameRate;
    h264CVbr->dst_frame_rate = vencParam->frameRate;
    h264CVbr->long_term_stats_time = 1;
    h264CVbr->short_term_stats_time = statsTime;
    uint32_t bitRate = (vencParam->bitRate != 0) ? vencParam->bitRate :
        GetVencBitRateByParamNormal(vencParam->picSize, vencParam->frameRate);
    h264CVbr->long_term_max_bit_rate = bitRate;
    h264CVbr->max_bit_rate = bitRate;
    h264CVbr->long_term_min_bit_rate = (uint32_t)g_fixedBitRateNormal[vencParam->picSize] / 0x2;
    return 0;
}

static td_s32 ConstructVencH264RcQPMAPAttribute(const AvVencAttr *vencParam,
    td_u32 statsTime, ot_venc_rc_attr *rcAttr)
{
    ot_venc_h264_qpmap h264_qpmap;
    rcAttr->rc_mode = OT_VENC_RC_MODE_H264_QPMAP;
    h264_qpmap.gop = DEFAULT_FPS;
    h264_qpmap.stats_time = statsTime;
    h264_qpmap.src_frame_rate = vencParam->frameRate;
    h264_qpmap.dst_frame_rate = vencParam->frameRate;
    td_s32 ret = memcpy_s(&rcAttr->h264_qpmap, sizeof(ot_venc_h264_qpmap), &h264_qpmap,
        sizeof(ot_venc_h264_qpmap));
    return ret;
}

static td_s32 ConstructVencH264RcAttribute(const AvVencAttr *vencParam, td_u32 statsTime,
    ot_venc_chn_attr *stVencChnAttr)
{
    CHK_NULL_RETURN(vencParam, TD_FAILURE, "vencParam null");
    CHK_NULL_RETURN(stVencChnAttr, TD_FAILURE, "stVencChnAttr null");

    td_s32 ret = TD_FAILURE;
    if (vencParam->rcMode == VENCOD_RC_CBR) {
        ret = ConstructVencH264RcCBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_FIXQP) {
        ret = ConstructVencH264RcFIXQPAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_VBR) {
        ret = ConstructVencH264RcVBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_AVBR) {
        ret = ConstructVencH264RcAVBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_QVBR) {
        ret = ConstructVencH264RcQVBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_CVBR) {
        ret = ConstructVencH264RcCVBRAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else if (vencParam->rcMode == VENCOD_RC_QPMAP) {
        ret = ConstructVencH264RcQPMAPAttribute(vencParam, statsTime, &(stVencChnAttr->rc_attr));
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "rcMode(%d) not support", vencParam->rcMode);
        return TD_FAILURE;
    }
    if (ret != EOK) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 ConstructVencRcAttribute(const AvVencAttr *vencParam, td_u32 statsTime,
    ot_venc_chn_attr *stVencChnAttr)
{
    CHK_NULL_RETURN(vencParam, TD_FAILURE, "vencParam null");
    CHK_NULL_RETURN(stVencChnAttr, TD_FAILURE, "stVencChnAttr null");

    td_s32 ret = TD_FAILURE;
    switch (vencParam->encType) {
        case OT_PT_H265: {
            ret = ConstructVencH265RcAttribute(vencParam, statsTime, stVencChnAttr);
            stVencChnAttr->venc_attr.h265_attr. rcn_ref_share_buf_en = TD_FALSE;
            break;
        }
        case OT_PT_H264: {
            ret = ConstructVencH264RcAttribute(vencParam, statsTime, stVencChnAttr);
            stVencChnAttr->venc_attr.h264_attr. rcn_ref_share_buf_en = TD_FALSE;
            break;
        }
        default:
            MEDIA_HAL_LOGE(MODULE_NAME, "cann't support this type (%d) in this version!", vencParam->encType);
            return OT_ERR_VENC_NOT_SUPPORT;
    }
    if (ret != EOK) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static void ConstructVencBaseAttribute(const AvVencAttr *vencParam, const ot_size *stPicSize,
    ot_venc_chn_attr *stVencChnAttr)
{
    stVencChnAttr->venc_attr.type = vencParam->encType;
    stVencChnAttr->venc_attr.max_pic_width = stPicSize->width;
    stVencChnAttr->venc_attr.max_pic_height = stPicSize->height;
    stVencChnAttr->venc_attr.pic_width = stPicSize->width;
    stVencChnAttr->venc_attr.pic_height = stPicSize->height;
    stVencChnAttr->venc_attr.buf_size = stPicSize->width * stPicSize->height * 0x4 / 0x3;
    stVencChnAttr->venc_attr.profile = 0;
    stVencChnAttr->venc_attr.is_by_frame = TD_TRUE;
}

static td_s32 ConstructVencJpegAttribute(ot_venc_chn_attr *stVencChnAttr)
{
    ot_venc_jpeg_attr stJpegAttr;

    stVencChnAttr->gop_attr.gop_mode = OT_VENC_GOP_MODE_NORMAL_P;
    stVencChnAttr->gop_attr.normal_p.ip_qp_delta = 0;
    stJpegAttr.dcf_en = TD_FALSE;
    stJpegAttr.mpf_cfg.large_thumbnail_num = 0;
    stJpegAttr.recv_mode = OT_VENC_PIC_RECV_SINGLE;
    if (memcpy_s(&(stVencChnAttr->venc_attr.jpeg_attr), sizeof(ot_venc_jpeg_attr), &stJpegAttr,
        sizeof(ot_venc_jpeg_attr)) != EOK) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 ConstructVencGopAttribute(const AvVencAttr *vencParam, const ot_venc_gop_attr *gopAttr,
    ot_venc_chn_attr *stVencChnAttr)
{
    if (memcpy_s(&(stVencChnAttr->gop_attr), sizeof(ot_venc_gop_attr), gopAttr, sizeof(ot_venc_gop_attr)) != EOK) {
        return TD_FAILURE;
    }
    if ((gopAttr->gop_mode == OT_VENC_GOP_MODE_BIPRED_B) && (vencParam->encType == OT_PT_H264)) {
        if (stVencChnAttr->venc_attr.profile == 0) {
            stVencChnAttr->venc_attr.profile = 1;
            MEDIA_HAL_LOGE(MODULE_NAME, "H.264 base profile not support BIPREDB, change to main profile!");
        }
    }
    if ((stVencChnAttr->rc_attr.rc_mode == OT_VENC_RC_MODE_H264_QPMAP) ||
        (stVencChnAttr->rc_attr.rc_mode == OT_VENC_RC_MODE_H265_QPMAP)) {
        if (gopAttr->gop_mode == OT_VENC_GOP_MODE_ADV_SMART_P) {
            stVencChnAttr->gop_attr.gop_mode = OT_VENC_GOP_MODE_SMART_P;
            MEDIA_HAL_LOGE(MODULE_NAME, "advsmartp not support QPMAP, so change gopmode to smartp!");
        }
    }
    return TD_SUCCESS;
}

static td_s32 ConstructVencChnAttribute(const AvVencAttr *vencParam, ot_venc_gop_attr *gopAttr, ot_size *stPicSize,
    ot_venc_chn_attr *stVencChnAttr)
{
    ConstructVencBaseAttribute(vencParam, stPicSize, stVencChnAttr);
    td_bool isGopSmartP = gopAttr->gop_mode == OT_VENC_GOP_MODE_SMART_P;
    td_u32 stats_time = isGopSmartP ? gopAttr->smart_p.bg_interval / DEFAULT_FPS : 1;

    if (ConstructVencRcAttribute(vencParam, stats_time, stVencChnAttr) != EOK) {
        return TD_FAILURE;
    }

    if (vencParam->encType == OT_PT_JPEG) {
        if (ConstructVencJpegAttribute(stVencChnAttr) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    } else {
        if (ConstructVencGopAttribute(vencParam, gopAttr, stVencChnAttr) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    }
    if (vencParam->encType == OT_PT_H264) {
        stVencChnAttr->venc_attr.h264_attr.frame_buf_ratio = FRAME_BUF_RATIO_MIN;
    } else if (vencParam->encType == OT_PT_H265) {
        stVencChnAttr->venc_attr.h265_attr.frame_buf_ratio = FRAME_BUF_RATIO_MIN;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "encType=%d", vencParam->encType);
    return TD_SUCCESS;
}

static td_s32 CodecVencCreate(ot_venc_chn vencChn, const AvVencAttr *vencParam)
{
    ot_size stPicSize = { 0 };
    ot_venc_chn_attr stVencChnAttr;
    ot_venc_gop_attr gopAttr;
    if (memset_s(&stVencChnAttr, sizeof(ot_venc_chn_attr), 0, sizeof(ot_venc_chn_attr)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    if (memset_s(&gopAttr, sizeof(ot_venc_gop_attr), 0, sizeof(ot_venc_gop_attr)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }

    gopAttr.gop_mode = (ot_venc_gop_mode)vencParam->gopMode;
    if (CodecVencGetGopAttr(gopAttr.gop_mode, &gopAttr) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "CodecVencGetGopAttr failed!");
        return TD_FAILURE;
    }

    if (GetPicWidthHeight(vencParam->picSize, &stPicSize) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Get picture size failed!");
        return TD_FAILURE;
    }

    /* *****************************************
     step 1:  Create Venc Channel
    ***************************************** */
    td_s32 ret = ConstructVencChnAttribute(vencParam, &gopAttr, &stPicSize, &stVencChnAttr);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConstructVencChnAttribute failed!");
        return TD_FAILURE;
    }
    ret = ss_mpi_venc_create_chn(vencChn, &stVencChnAttr);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_create_chn [%d] failed with %#x! ===", vencChn, ret);
        return ret;
    }

#if defined(ENABLE_DISTRIBUTED_CAMERA_LOW_DELAY) && (ENABLE_DISTRIBUTED_CAMERA_LOW_DELAY != 0)
    VEncoderEnableLowDelay();
#endif

    ret = VencCorrectRcParam(vencChn);
    if (ret != TD_SUCCESS) {
        ss_mpi_venc_destroy_chn(vencChn);
        return ret;
    }
    return TD_SUCCESS;
}

// only search in default type with count is equal to 1
static td_bool IsValidVencType(ot_payload_type type)
{
    td_bool ret = TD_TRUE;
    if ((type != OT_PT_JPEG) && (type != OT_PT_H264) && (type != OT_PT_H265)) {
        ret = TD_FALSE;
    }
    return ret;
}

static bool CheckVencWhetherSupportSolution(AvVencAttr *attr)
{
    if ((int32_t)attr->picSize >= RESOLUTION_INVALID || (int32_t)attr->picSize < 0) {
        return false;
    }

    const CodecCapability *capability = NULL;
    size_t validCapNum = sizeof(g_vencCapEntries) / sizeof(g_vencCapEntries[0]);

    for (size_t i = 0; i < validCapNum; i++) {
        if (g_vencCapEntries[i].mime == attr->mime) {
            capability = &g_vencCapEntries[i];
            break;
        }
    }
    if (capability == NULL) {
        return false;
    }

    const ResolutionInfo *info = &g_resolution[attr->picSize];

    if ((int32_t)info->width > capability->maxSize.width || (int32_t)info->height > capability->maxSize.height ||
        (int32_t)info->width < capability->minSize.width || (int32_t)info->height < capability->minSize.height) {
        return false;
    }

    if ((attr->mime == MEDIA_MIMETYPE_VIDEO_AVC || attr->mime == MEDIA_MIMETYPE_VIDEO_HEVC) &&
        ((int32_t)info->width * (int32_t)info->height > AVC_HEVC_ENC_RESOLUITION_MAX)) {
        return false;
    }
    return true;
}

static int32_t VencCreateProcess(NativeVencContext *vencCtx, const AvVencAttr *vencAttr)
{
    ot_venc_chn vencChn = 0;
    int32_t ret = VencMngGetFreeChn(&vencChn);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "VencMngGetFreeChn failed Ret: %d", ret);
        return VENC_ERR_NOFREE_CHANNEL;
    }
    vencCtx->vencChn = vencChn;
    vencCtx->encType = vencAttr->encType;
    MEDIA_HAL_LOGI(MODULE_NAME, "vencChn=%d\n", vencChn);
    g_vencChn = vencChn;
    if (vencAttr->encType == OT_PT_JPEG) {
        ret = CodecVencSnapCreate(vencChn, vencAttr, TD_TRUE);
    } else {
        ret = CodecVencCreate(vencChn, vencAttr);
    }
    if (ret != TD_SUCCESS) {
        if (VencMngFreeChn(vencCtx->vencChn) != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "VencMngFreeChn error!!");
        }
        return VENC_ERR_VENC_ERR;
    }
    return TD_SUCCESS;
}

static td_s32 NativeVencCreate(void **vencHdl, const Param *attr, int len)
{
    CHK_NULL_RETURN(vencHdl, TD_FAILURE, "vencHdl null");
    CHK_NULL_RETURN(attr, TD_FAILURE, "attr null");

    MEDIA_HAL_LOGD(MODULE_NAME, "in\n");
    AvVencAttr vencAttr = { 0 };
    if (len > PARAM_UPPER_LIMIT) {
        MEDIA_HAL_LOGE(MODULE_NAME, "length error, length is %d, it should not beyond %d", len, PARAM_UPPER_LIMIT);
        return TD_FAILURE;
    }
    int32_t ret = FillVencAttribute(attr, len, &vencAttr);
    if (ret != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "fill aencAttr fail");
        return ret;
    }

    if (IsValidVencType(vencAttr.encType) == TD_FALSE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not valid video encoder");
        return VENC_ERR_NOTREG;
    }
    if (!CheckVencWhetherSupportSolution(&vencAttr)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "not support resolution: %d", vencAttr.picSize);
        return VENC_ERR_NOTREG;
    }

    NativeVencContext *vencCtx = (NativeVencContext*)malloc(sizeof(NativeVencContext));
    if (vencCtx == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc NativeVencContext failed");
        return TD_FAILURE;
    }
    if (memset_s(vencCtx, sizeof(NativeVencContext), 0x00, sizeof(NativeVencContext)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    if (VencCreateProcess(vencCtx, &vencAttr) != TD_SUCCESS) {
        goto FREE;
    }

    InitVencNativeOutBuffers(vencCtx);
    *vencHdl = vencCtx;
    return TD_SUCCESS;

FREE:
    free(vencCtx);
    return ret;
}

static td_s32 NativeVencStart(void *vencHdl)
{
    CHK_NULL_RETURN(vencHdl, TD_FAILURE, "vencHdl null");
    NativeVencContext *vencCtx = (NativeVencContext*)vencHdl;

    ot_venc_start_param stRecvParam;
    stRecvParam.recv_pic_num = -1;
    td_s32 ret = ss_mpi_venc_start_chn(vencCtx->vencChn, &stRecvParam);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_start_chn faild with%#x!", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 NativeSetSourceId(NativeVencContext *vencCtx, uint32_t deviceId)
{
    CHK_NULL_RETURN(vencCtx, TD_FAILURE, "vencCtx null");
    vencCtx->srcChn.mod_id = MOD_ID(deviceId);
    vencCtx->srcChn.dev_id = DEVICE_ID(deviceId);
    vencCtx->srcChn.chn_id = CHAN_ID(deviceId);

    vencCtx->destChn.mod_id = OT_ID_VENC;
    vencCtx->destChn.dev_id = 0;
    vencCtx->destChn.chn_id = vencCtx->vencChn;
    MEDIA_HAL_LOGD(MODULE_NAME, "HI_MPI_SYS_BindenModId %d vpssCapDev %d vpssChn %d vEncChn %d",
        vencCtx->srcChn.mod_id, vencCtx->srcChn.dev_id,
        vencCtx->srcChn.chn_id, vencCtx->destChn.chn_id);
    td_s32 ret = ss_mpi_sys_bind(&vencCtx->srcChn, &vencCtx->destChn);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_bind failed ret %#x", ret);
        return ret;
    }
    vencCtx->hardwareMod = TD_TRUE;
    return TD_SUCCESS;
}

static td_s32 NativeSetDeBreatheEffect(const NativeVencContext *vencCtx, const Param *param)
{
    if (param->size != sizeof(uint32_t) || param->val == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "param KEY_DEBREATHE_EFFECT wrong");
        return TD_FAILURE;
    }
    ot_venc_debreath_effect breatheEffect;
    td_s32 ret = ss_mpi_venc_get_debreath_effect(vencCtx->vencChn, &breatheEffect);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_get_debreath_effect fail ret:%d", ret);
    }
    breatheEffect.enable = *((uint32_t *)param->val);
    breatheEffect.strength0 = g_breatheEffect.strength0;
    breatheEffect.strength1 = g_breatheEffect.strength1;
    MEDIA_HAL_LOGI(MODULE_NAME, "ss_mpi_venc_set_debreath_effect Enable: %d, strength0: %d, strength1: %d",
        breatheEffect.enable, breatheEffect.strength0, breatheEffect.strength1);
    ret = ss_mpi_venc_set_debreath_effect(vencCtx->vencChn, &breatheEffect);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_set_debreath_effect fail ret:%d", ret);
    }
    return TD_SUCCESS;
}

static td_s32 NativeVencSetParameter(void *vencHdl, const Param *param)
{
    CHK_NULL_RETURN(vencHdl, TD_FAILURE, "vencHdl null");
    CHK_NULL_RETURN(param, TD_FAILURE, "param null");
    NativeVencContext *vencCtx = (NativeVencContext*)vencHdl;
    switch (param->key) {
        case KEY_DEVICE_ID: {
            if (param->size != sizeof(uint32_t) || param->val == NULL) {
                MEDIA_HAL_LOGE(MODULE_NAME, "param PARAM_INDEX_DEVICE_ID wrong");
                return TD_FAILURE;
            }
            uint32_t deviceId = *((uint32_t *)(param->val));
            return NativeSetSourceId(vencCtx, deviceId);
        }
        case KEY_IMAGE_Q_FACTOR: {
            if (param->size != sizeof(uint32_t) || param->val == NULL) {
                MEDIA_HAL_LOGE(MODULE_NAME, "param KEY_JPEG_Q_FACTOR wrong");
                return TD_FAILURE;
            }
            if (vencCtx->encType == OT_PT_JPEG) {
                ot_venc_jpeg_param stParamJpeg = {};
                td_s32 ret = ss_mpi_venc_get_jpeg_param(vencCtx->vencChn, &stParamJpeg);
                if (ret != TD_SUCCESS) {
                    MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_get_jpeg_param fail ret:%d", ret);
                    return TD_FAILURE;
                }
                stParamJpeg.qfactor = *((uint32_t *)param->val);
                ret = ss_mpi_venc_set_jpeg_param(vencCtx->vencChn, &stParamJpeg);
                if (ret != TD_SUCCESS) {
                    MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_set_jpeg_param fail");
                    return TD_FAILURE;
                }
            }
            break;
        }
        case KEY_DEBREATHE_EFFECT: {
            td_s32 ret = NativeSetDeBreatheEffect(vencCtx, param);
            if (ret != TD_SUCCESS) {
                MEDIA_HAL_LOGE(MODULE_NAME, "NativeSetDeBreatheEffect fail ret:%d", ret);
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

static td_s32 NativeVencDestroy(void *vencHdl)
{
    CHK_NULL_RETURN(vencHdl, TD_FAILURE, "vencHdl null");
    NativeVencContext *vencCtx = (NativeVencContext*)vencHdl;
    td_s32 ret;

    if (vencCtx->hardwareMod) {
        ret = ss_mpi_sys_unbind(&vencCtx->srcChn, &vencCtx->destChn);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_unbind failed ret %#x", ret);
        }
    }
    ret = ss_mpi_venc_destroy_chn(vencCtx->vencChn);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_destroy_chn error ret:%x", ret);
    }

    ret = VencMngFreeChn(vencCtx->vencChn);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "VencMngFreeChn %u error.", (td_u32)vencCtx->vencChn);
    }
    free(vencCtx);
    return TD_SUCCESS;
}

static td_s32 NativeVencStop(void *vencHdl)
{
    CHK_NULL_RETURN(vencHdl, TD_FAILURE, "vencHdl null");
    NativeVencContext *nativeVencCtx = (NativeVencContext*)vencHdl;

    if (nativeVencCtx->hardwareMod) {
        MEDIA_HAL_LOGD(MODULE_NAME, "ss_mpi_sys_unbind in");
        td_s32 ret = ss_mpi_sys_unbind(&nativeVencCtx->srcChn, &nativeVencCtx->destChn);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_sys_unbind fail,ret:%x", ret);
        }
        MEDIA_HAL_LOGD(MODULE_NAME, "ss_mpi_sys_unbind srcDev %d srcChn %d dstChn %d",
            nativeVencCtx->srcChn.dev_id, nativeVencCtx->srcChn.chn_id, nativeVencCtx->destChn.chn_id);

        ret = ss_mpi_venc_stop_chn(nativeVencCtx->vencChn);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_stop_chn fail,ret:%x", ret);
        }
        nativeVencCtx->hardwareMod = TD_FALSE;
    }
    return TD_SUCCESS;
}

static td_s32 NativeVencSendFrame(void *vencHdl, const InputInfo *inputInfo, uint32_t frameIndex, td_bool block)
{
    MEDIA_HAL_UNUSED(vencHdl);
    MEDIA_HAL_UNUSED(inputInfo);
    MEDIA_HAL_UNUSED(frameIndex);
    MEDIA_HAL_UNUSED(block);
    return TD_SUCCESS;
}

static td_s32 GetVencStream(ot_venc_chn VencChn, ot_venc_chn_status *stStat, ot_venc_stream *videoStream)
{
    td_s32 ret = ss_mpi_venc_query_status(VencChn, stStat);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_query_status failed with %#x!", ret);
        return TD_FAILURE;
    }
    if (stStat->cur_packs == 0) {
        MEDIA_HAL_LOGD(MODULE_NAME, "NOTE: Current  frame is NULL!");
        return TD_SUCCESS;
    }
    if (stStat->cur_packs > PACK_CNT_MAX) {
        MEDIA_HAL_LOGE(MODULE_NAME, "cur_packs invalid %u", stStat->cur_packs);
        return TD_FAILURE;
    }
    videoStream->pack = (ot_venc_pack *)malloc(sizeof(ot_venc_pack) * stStat->cur_packs);
    if (videoStream->pack == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc memory failed!");
        return TD_FAILURE;
    }
    videoStream->pack_cnt = stStat->cur_packs;
    ret = ss_mpi_venc_get_stream(VencChn, videoStream, -1);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_get_stream failed with %#x!", ret);
        free(videoStream->pack);
        videoStream->pack = NULL;
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 MpiGetBindbyDest(ot_venc_chn VencChn)
{
    ot_mpp_chn stDestChn;
    ot_mpp_chn stSrcChn;
    ot_vpss_chn_attr stVpssChnAttr;
    /* *****************************************
    Start Recv Venc Pictures
    ***************************************** */
    stDestChn.mod_id = OT_ID_VENC;
    stDestChn.dev_id = 0;
    stDestChn.chn_id = VencChn;
    td_s32 ret = ss_mpi_sys_get_bind_by_dst(&stDestChn, &stSrcChn);
    if (ret == TD_SUCCESS && stSrcChn.mod_id == OT_ID_VPSS) {
        ret = ss_mpi_vpss_get_chn_attr(stSrcChn.dev_id, stSrcChn.chn_id, &stVpssChnAttr);
    }
    return TD_SUCCESS;
}

static td_s32 GetVencStreamCapture(ot_venc_chn VencChn, ot_venc_stream *videoStream, td_u32 SnapCnt)
{
    ot_venc_chn_status stStat = { 0 };
    td_s32 ret = MpiGetBindbyDest(VencChn);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /* *****************************************
     recv picture
    ***************************************** */
    const td_u32 MAX_RETRY_TIMES = 30;
    const td_u32 SLEEP_TIME_US = 100000;
    for (td_u32 i = 0; i < MAX_RETRY_TIMES; i++) {
        ret = GetVencStream(VencChn, &stStat, videoStream);
        if (ret == TD_SUCCESS && stStat.cur_packs != 0) {
            return TD_SUCCESS;
        } else {
            MEDIA_HAL_LOGE(MODULE_NAME, "GetVencStream failed! retry times:%u", i);
            usleep(SLEEP_TIME_US);
        }
    }

    return TD_FAILURE;
}

static td_s32 GetVencStreamNonCapture(const NativeVencContext *nativeVencCtx,
    ot_venc_stream *videoStream, td_bool block)
{
    td_s32 vencFd = ss_mpi_venc_get_fd(nativeVencCtx->vencChn);
    if (vencFd < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_get_fd failed ret %x", vencFd);
        return VENC_ERR_UNKNOWN;
    }

    ot_venc_chn_status stStat = { 0 };
    osal_fd_set readFds;
    OSAL_FD_ZERO(&readFds);
    OSAL_FD_SET(vencFd, &readFds);
    struct timeval timeoutVal = { 0 };
    timeoutVal.tv_sec = block ? 0x5 : 2; /* 5: time , 2:2s timeout. */
    td_s32 ret = OSAL_SELECT(vencFd + 1, &readFds, NULL, NULL, &timeoutVal);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "select failed!");
        return VENC_ERR_UNKNOWN;
    } else if (ret == 0) {
        MEDIA_HAL_LOGD(MODULE_NAME, "get venc stream time out");
        return MEDIA_HAL_TIMEOUT;
    } else {
        if (OSAL_FD_ISSET(vencFd, &readFds)) {
            /*******************************************************
             query how many packs in one-frame stream.
            *******************************************************/
            if (memset_s(videoStream, sizeof(ot_venc_stream), 0, sizeof(ot_venc_stream)) != EOK) {
                MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
            }
            ret = ss_mpi_venc_query_status(nativeVencCtx->vencChn, &stStat);
            if (ret != TD_SUCCESS) {
                MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_query_status chan:%d fail:%d", nativeVencCtx->vencChn, ret);
                return ret;
            }
            if (stStat.cur_packs == 0) {
                MEDIA_HAL_LOGE(MODULE_NAME, "NOTE: Current frame is NULL");
                return VENC_ERR_UNKNOWN;
            }
            /*******************************************************
             malloc corresponding number of pack nodes.
            *******************************************************/
            videoStream->pack = (ot_venc_pack *)(malloc(sizeof(ot_venc_pack) * stStat.cur_packs));
            if (videoStream->pack == NULL) {
                MEDIA_HAL_LOGE(MODULE_NAME, "malloc stream pack failed!");
                return VENC_ERR_NOMEM;
            }

            /*******************************************************
             call mpi to get stream encoded by one-frame
            *******************************************************/
            videoStream->pack_cnt = stStat.cur_packs;
            ret = ss_mpi_venc_get_stream(nativeVencCtx->vencChn, videoStream, TD_TRUE);
            if (ret != TD_SUCCESS) {
                free(videoStream->pack);
                videoStream->pack = NULL;
                MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_get_stream failed:%d", ret);
                return ret;
            }
        }
    }
    return TD_SUCCESS;
}

static td_s32 FillVencOutputInfo(const ot_venc_stream *src, OutputInfo *dst)
{
    if (src == NULL || dst == NULL) {
        return TD_FAILURE;
    }

    dst->type = VIDEO_ENCODER;
    dst->bufferCnt = src->pack_cnt;
    dst->vendorPrivate = (void*)src;
    if (src->pack_cnt == 0) {
        return TD_SUCCESS;
    }

    dst->timeStamp = (int64_t)src->pack[0].pts;
    dst->sequence = src->seq;
    if (src->pack[0].data_type.h265_type == OT_VENC_H265_NALU_P_SLICE ||
        src->pack[0].data_type.h265_type == OT_VENC_H265_NALU_B_SLICE ||
        src->pack[0].data_type.h264_type == OT_VENC_H264_NALU_P_SLICE ||
        src->pack[0].data_type.h264_type == OT_VENC_H264_NALU_B_SLICE) {
        dst->flag |= ~STREAM_FLAG_KEYFRAME;
    } else {
        dst->flag |= STREAM_FLAG_KEYFRAME;
    }

    if (src->pack_cnt > PACK_CNT_MAX) {
        return TD_FAILURE;
    }
    dst->buffers = (CodecBufferInfo*)malloc(src->pack_cnt * sizeof(CodecBufferInfo));
    if (dst->buffers == NULL) {
        return TD_FAILURE;
    }
    for (td_u32 i = 0; i < src->pack_cnt; i++) {
        dst->buffers[i].type = BUFFER_TYPE_VIRTUAL;
        dst->buffers[i].addr = (uint8_t*)src->pack[i].addr;
        dst->buffers[i].length = src->pack[i].len;
        dst->buffers[i].offset = src->pack[i].offset;
    }
    return TD_SUCCESS;
}

static td_s32 NativeVencGetStream(void *vencHdl, OutputInfo *outInfo, td_bool block)
{
    CHK_NULL_RETURN(vencHdl, TD_FAILURE, "vencHdl null");
    CHK_NULL_RETURN(outInfo, TD_FAILURE, "outInfo null");
    NativeVencContext *nativeVencCtx = (NativeVencContext*)vencHdl;

    ot_venc_stream *videoStream = GetFreeVencNativeOutBuffer(nativeVencCtx);
    if (videoStream == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "no free native out buffer for venc");
        return TD_FAILURE;
    }
    td_s32 ret;
    if (nativeVencCtx->encType == OT_PT_JPEG) {
        ret = GetVencStreamCapture(nativeVencCtx->vencChn, videoStream, 1);
    } else {
        ret = GetVencStreamNonCapture(nativeVencCtx, videoStream, block);
    }
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "venc get stream fail ret:%d", ret);
        ReleaseVencNativeOutbuffer(nativeVencCtx, videoStream);
        return ret;
    }

    ret = FillVencOutputInfo(videoStream, outInfo);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 NativeVencReleaseStream(void *vencHdl, OutputInfo *outInfo)
{
    CHK_NULL_RETURN(vencHdl, TD_FAILURE, "vencHdl null");
    CHK_NULL_RETURN(outInfo, TD_FAILURE, "outInfo null");
    CHK_NULL_RETURN(outInfo->vendorPrivate, TD_FAILURE, "outInfo vendorPrivate null");
    NativeVencContext *nativeVencCtx = (NativeVencContext*)vencHdl;
    if (outInfo->buffers != NULL) {
        free(outInfo->buffers);
        outInfo->buffers = NULL;
    }

    ot_venc_stream *videoStream = (ot_venc_stream *)outInfo->vendorPrivate;
    CHK_COND_RETURN(!IsValidVencNativeOutbuffer(nativeVencCtx, videoStream), MEDIA_HAL_INVALID_PARAM,
        "venc stream invalid");
    td_s32 ret = ss_mpi_venc_release_stream(nativeVencCtx->vencChn, videoStream);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_release_stream error.ret=0x%x", ret);
    }
    if (videoStream->pack != NULL) {
        free(videoStream->pack);
        videoStream->pack = NULL;
    }
    ReleaseVencNativeOutbuffer(nativeVencCtx, videoStream);
    return ret;
}

static td_s32 NativeVencReset(void *pluginHandle)
{
    MEDIA_HAL_UNUSED(pluginHandle);
    return TD_SUCCESS;
}

static td_s32 NativeVencInvoke(void *pluginHandle, td_u32 cmd, void *arg1, void *arg2)
{
    MEDIA_HAL_UNUSED(pluginHandle);
    MEDIA_HAL_UNUSED(cmd);
    MEDIA_HAL_UNUSED(arg1);
    MEDIA_HAL_UNUSED(arg2);
    return TD_SUCCESS;
}

td_s32 VEncoderEnableLowDelay(void)
{
    ot_venc_intra_refresh intraRefresh = {0};
    td_s32 ret = ss_mpi_venc_get_intra_refresh(g_vencChn, &intraRefresh);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_get_intra_refresh failed! vencChn=%d, ret: %#x!", g_vencChn, ret);
        return ret;
    }
    intraRefresh.enable = 1;
    intraRefresh.mode = OT_VENC_INTRA_REFRESH_ROW;
    intraRefresh.refresh_num = DEFAULT_INTRA_REFREASH_NUM;
    ret = ss_mpi_venc_set_intra_refresh(g_vencChn, &intraRefresh);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_venc_set_intra_refresh failed! vencChn=%d, ret: %#x!", g_vencChn, ret);
    }
    return ret;
}

void *VEncoderGetImpl(void)
{
    AvVideoEncoder *encoder = (AvVideoEncoder*)malloc(sizeof(AvVideoEncoder));
    if (encoder == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc fail");
        return NULL;
    }
    if (memset_s(encoder, sizeof(AvVideoEncoder), 0x00, sizeof(AvVideoEncoder)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }

    encoder->pluginCommon.pfnCreate = NativeVencCreate;
    encoder->pluginCommon.isHardwarePlugin = true;
    encoder->pluginCommon.pfnDestroy = NativeVencDestroy;
    encoder->pluginCommon.pfnStart = NativeVencStart;
    encoder->pluginCommon.pfnStop = NativeVencStop;
    encoder->pfnQueueInput = NativeVencSendFrame;
    encoder->pfnDequeueOutput = NativeVencGetStream;
    encoder->pfnQueueOutput = NativeVencReleaseStream;
    encoder->pluginCommon.pfnReset = NativeVencReset;
    encoder->pluginCommon.pfnInvoke = NativeVencInvoke;
    encoder->pluginCommon.pfnSetParameter = NativeVencSetParameter;

    size_t validCapNum = sizeof(g_vencCapEntries) / sizeof(g_vencCapEntries[0]);
    encoder->pluginCommon.validCapbilityNum = (int)validCapNum;
    for (size_t i = 0; i < validCapNum; i++) {
        encoder->pluginCommon.capbilites[i] = g_vencCapEntries[i];
    }

    VencMngInit();
    return encoder;
}

td_s32 VEncoderImplUnRegister()
{
    VencMngDeinit();
    return TD_SUCCESS;
}

