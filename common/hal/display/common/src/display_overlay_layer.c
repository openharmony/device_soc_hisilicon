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

#include "display_overlay_layer.h"
#include "display_adapt_video.h"
#include "display_adapt_vo.h"
#include "display_adapt_sys.h"

#include "ot_math.h"
#define ALIGN_DOWN OT_ALIGN_DOWN
#include <pthread.h>
#include <stdarg.h>
#include <securec.h>
#include "vpss_resmng.h"

#define ALIGN_DOWN_NUM 2
#define DISPLAY_LAYER_INVALID_ID (-1)
#define VO_BUFF_LEN 3
#define LAYER_IDX_OFFSET 24
#define VO_DEV_OFFSET 16
#define VO_LAYER_OFFSET 8
#define INVALID_ID (-1U)
#define UNUSED_ATTR __attribute__((unused))
#define DISPLAY_LAYER_MAX_NUM 64

#ifdef ENABLE_MULTI_DISP_CHN
#define DISPLAY_CNT 2  // Two CHNs need to be initialized for dual-camera display
#else
#define DISPLAY_CNT 1
#endif

/* default layer width */
#define DEFAULT_WIDTH_FOR_HDMI 1920
#define DEFAULT_WIDTH_FOR_LCD 480
/* default layer height */
#define DEFAULT_HEIGHT_FOR_HDMI 1080
#define DEFAULT_HEIGHT_FOR_LCD 960
#define DEFAULT_DISPLAY_FPS 60
#define DEFAULT_SET_FPS 30
#define OVERLAY_LAYER_NUM ((OV_LAYER_MAX) - (GRA_LAYER_MAX))
#define OFFSET_8 8

#if ENABLE_VO_TIME_TASK
#define CNT 1000
static pthread_t g_voThread = 0;
static bool g_voRunning = false;
#endif

#if ENABLE_VO_BUSYDROP
static pthread_t g_voDropThread = 0;
static bool g_voDropRunning = false;
#endif

typedef struct {
    VO_CHN voChn;
    VPSS_GRP vpssGrp;
    VPSS_CHN vpssChn;
    bool vpssIsInternalEnable;
} OverlayLayerPrivate;

static bool CheckTypeIsOverlayLayer(uint32_t layerId)
{
    return (layerId & LAYER_ID_VO_LAYER_TYPE_MARK) != 0;
}

static uint8_t GetVoChn(uint32_t layerId)
{
    return (uint8_t)layerId & LAYER_ID_VO_CHN_MASK;
}

static uint8_t GetVoLayer(uint32_t layerId)
{
    return (uint8_t)((layerId & LAYER_ID_VO_LAYER_ID_MASK) >> VO_LAYER_OFFSET);
}

static OverlayLayerPrivate *GetLayerInstance(uint32_t devId, uint8_t index)
{
    static OverlayLayerPrivate layerPriv[DEV_ID_NUM][OVERLAY_LAYER_NUM] = {{
        {
            .voChn = -1,
            .vpssGrp = -1,
            .vpssChn = -1,
        }, {
            .voChn = -1,
            .vpssGrp = -1,
            .vpssChn = -1,
        }, {
            .voChn = -1,
            .vpssGrp = -1,
            .vpssChn = -1,
        }, {
            .voChn = -1,
            .vpssGrp = -1,
            .vpssChn = -1,
        }
    }};
    return &layerPriv[devId][index];
}

static int32_t BindVoVpss(VPSS_GRP vpssGrp, VPSS_CHN vpssChn, VO_LAYER voLayer, VO_CHN voChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.mod_id = HI_ID_VPSS;
    stSrcChn.dev_id = vpssGrp;
    stSrcChn.chn_id = vpssChn;

    stDestChn.mod_id = HI_ID_VO;
    stDestChn.dev_id = voLayer;
    stDestChn.chn_id = voChn;
    int32_t ret = ss_mpi_sys_bind(&stSrcChn, &stDestChn);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("ss_mpi_sys_bind(vpssGrp=%d vpssChn=%d, voLayer=%d voChn=%d) failed, ret=0x%x",
                 vpssGrp, vpssChn, voLayer, voChn, ret);
        return ret;
    }
    HDF_LOGI("bind vo & vpss success, vpssGrp=%d vpssChn=%d voLayer=%d voChn=%d", vpssGrp, vpssChn, voLayer, voChn);
    return DISPLAY_SUCCESS;
}

static int32_t UnBindVoVpss(VPSS_GRP vpssGrp, VPSS_CHN vpssChn, VO_LAYER voLayer, VO_CHN voChn)
{
    MPP_CHN_S stSrcChn;
    stSrcChn.mod_id = HI_ID_VPSS;
    stSrcChn.dev_id = vpssGrp;
    stSrcChn.chn_id = vpssChn;

    MPP_CHN_S stDestChn;
    stDestChn.mod_id = HI_ID_VO;
    stDestChn.dev_id = voLayer;
    stDestChn.chn_id = voChn;
    int32_t ret = ss_mpi_sys_unbind(&stSrcChn, &stDestChn);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("ss_mpi_sys_unbind(vpssGrp=%d vpssChn=%d, voLayer=%d voChn=%d) failed, ret=0x%x",
                 vpssGrp, vpssChn, voLayer, voChn, ret);
        return ret;
    }
    HDF_LOGI("unbind vo & vpss success, vpssGrp=%d vpssChn=%d voLayer=%d voChn=%d", vpssGrp, vpssChn, voLayer, voChn);

    return DISPLAY_SUCCESS;
}

#if ENABLE_VO_BUSYDROP
static int32_t sample_comm_vdec_clear_vo_busybuf(td_void)
{
    for (int32_t index = 0; index < OVERLAY_LAYER_NUM; index++) {
        int32_t ret;
        ot_vo_layer layer = 0;
        ot_vo_chn_status status;
        td_bool is_clear_all = TD_FALSE;
        ret = ss_mpi_vo_query_chn_status(layer, index, &status);
        if (ret != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if (status.chn_busy_num >= 2) {  // 2:busy_num
            ret = ss_mpi_vo_clear_chn_buf(layer, index, is_clear_all);
            if (ret != TD_SUCCESS) {
                HDF_LOGE("Clear channel %d buf failed with errno %#x!\n", index, ret);
                return TD_FAILURE;
            }
        }
    }

    return TD_SUCCESS;
}

static void *VoDropTask(void *args)
{
    (void)args;
    while (g_voDropThread && g_voDropRunning) {
        sample_comm_vdec_clear_vo_busybuf();
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 1 * 1000 * 1000;  // 1000: ns to ms
        nanosleep(&ts, TD_NULL);
    }
    HDF_LOGI(" VoDropTask exit\n");
    return NULL;
}

static void VoDropTaskBegin()
{
    if (g_voDropThread == 0) {
        g_voDropRunning = true;
        int32_t ret = pthread_create(&g_voDropThread, NULL, VoDropTask, NULL);
        if (ret != 0) {
            g_voDropRunning = false;
            g_voDropThread = 0;
            HDF_LOGI("creat vo drop thread failed %d \n", ret);
            return;
        }
        HDF_LOGI("creat vo drop thread = %d \n", ret);
    }
}

static void VoDropTaskEnd()
{
    if (g_voDropThread != 0) {
        g_voDropRunning = false;
        pthread_join(g_voDropThread, NULL);
        g_voDropThread = 0;
    }
}
#endif

#if ENABLE_VO_TIME_TASK
static void *VoTimeTask(void *args)
{
    (void)args;
    VIDEO_FRAME_INFO_S videoFrame = {0};
    td_u64 curPTS = 0;
    td_u64 dl[CNT] = {0};
    td_u64 lastPts = 0;
    HDF_LOGI(" VoTimeTask enter\n");
    int32_t i = 0;
    while (i < CNT && g_voRunning) {
        int32_t ret = ss_mpi_vo_get_screen_frame(0, &videoFrame, 0x3E8); /* 1000ms */
        if (ret != TD_SUCCESS) {
            continue;
        }
        ret = ss_mpi_sys_get_cur_pts(&curPTS);
        if (ret == TD_SUCCESS) {
            dl[i] = curPTS - videoFrame.video_frame.pts;
            if (lastPts != videoFrame.video_frame.pts) {
                lastPts = videoFrame.video_frame.pts;
                i++;
            }
        }
        ss_mpi_vo_release_screen_frame(0, &videoFrame);
        if (memset_s(&videoFrame, sizeof(videoFrame), 0, sizeof(videoFrame)) != EOK) {
            HDF_LOGI("memset_s videoFrame failed\n");
        }
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 1 * 1000 * 1000;  // 1000: ns to ms
        nanosleep(&ts, TD_NULL);
    }
    HDF_LOGI(" VoTimeTask exit\n");
    if (i < CNT) {
        return NULL;
    }
    td_u64 total = 0;
    for (i = 0; i < CNT; i++) {
        if (i % 0x32 == 0) { /* 50 */
            HDF_LOGI(" vo screen = %llu \n", dl[i]);
        }
        total += dl[i];
    }
    HDF_LOGI(" vo screen avg = %llu\n", total / CNT);
    return NULL;
}

static void VoTimeTaskBegin(void)
{
    if (g_voThread == 0) {
        g_voRunning = true;
        int32_t ret = pthread_create(&g_voThread, NULL, VoTimeTask, NULL);
        if (ret != 0) {
            g_voRunning = false;
            g_voThread = 0;
            HDF_LOGI("creat vo thread failed %d \n", ret);
            return;
        }
        HDF_LOGI("creat vo thread = %d \n", ret);
    }
}

static void VoTimeTaskEnd(void)
{
    if (g_voThread != 0) {
        g_voRunning = false;
        pthread_join(g_voThread, NULL);
        g_voThread = 0;
    }
}
#endif

static int32_t SetVoLayerParam(uint32_t devId, VO_VIDEO_LAYER_ATTR_S *attr)
{
    VO_PUB_ATTR_S stPubAttr = {0};
    int32_t ret = ss_mpi_vo_get_pub_attr(devId > INT32_MAX ? INT32_MAX : devId, &stPubAttr);
    CHECK_CALL_MPI_RET(ret, ss_mpi_vo_get_pub_attr);
    attr->display_rect.x = 0;
    attr->display_rect.y = 0;
    attr->display_rect.width = DEFAULT_WIDTH_FOR_HDMI;
    attr->display_rect.height = DEFAULT_HEIGHT_FOR_HDMI;
    attr->display_frame_rate = DEFAULT_DISPLAY_FPS;
    attr->cluster_mode_en = TD_FALSE;
    attr->double_frame_en = TD_FALSE;
    attr->pixel_format = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    attr->img_size.width = attr->display_rect.width;
    attr->img_size.height = attr->display_rect.height;
    attr->dst_dynamic_range = DYNAMIC_RANGE_SDR8;
    attr->display_buf_len = VIDEO_LAYER_BUFF_LEN;
    return 0;
}

static int32_t EnabelVoChannel(VO_LAYER voLayer, VO_CHN voChn, uint32_t width, uint32_t height)
{
    if (voChn >= OT_VO_MAX_CHN_NUM) {
        return DISPLAY_FAILURE;
    }
    VO_CHN_ATTR_S chnAttr;
    int32_t ret = ss_mpi_vo_get_chn_attr(voLayer, voChn, &chnAttr);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("ss_mpi_vo_get_chn_attr failure, ret = 0x%x", ret);
        return ret;
    }

    chnAttr.rect.x = ALIGN_DOWN(0, BYTE_ALIGN_NUM);
    chnAttr.rect.y = ALIGN_DOWN(0, BYTE_ALIGN_NUM);
    chnAttr.rect.width = ALIGN_DOWN(width, BYTE_ALIGN_NUM);
    chnAttr.rect.height = ALIGN_DOWN(height, BYTE_ALIGN_NUM);
    chnAttr.deflicker_en = TD_FALSE;
    chnAttr.priority = 0;
    ret = ss_mpi_vo_set_chn_attr(voLayer, voChn, &chnAttr);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("ss_mpi_vo_set_chn_attr failure, ret = 0x%x", ret);
        return ret;
    }
    ret = ss_mpi_vo_enable_chn(voLayer, voChn);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("ss_mpi_vo_enable_chn failure, ret = 0x%x", ret);
        return ret;
    }

    return DISPLAY_SUCCESS;
}

static int32_t DisabelVoChannel(VO_LAYER voLayer, VO_CHN voChn)
{
    int32_t ret = ss_mpi_vo_disable_chn(voLayer, voChn);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("Camera video output disable channel failed.(ret=0x%x)\n", ret);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

int32_t OverlayLayerFlush(uint32_t devId, uint32_t layerId, LayerBuffer *buffer)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    int32_t ret = DISPLAY_FAILURE;
    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return ret;
    }

    uint8_t vochn = GetVoChn(layerId);
    if (vochn >= OVERLAY_LAYER_NUM) {
        HDF_LOGE("%s: layerId is out of range", __func__);
        return ret;
    }

    OverlayLayerPrivate *priv = GetLayerInstance(devId, vochn);
    if (buffer != NULL && buffer->data.virAddr != NULL) {
        ret = VpssSendFrame(priv->vpssGrp, priv->vpssChn, (ot_video_frame_info *)buffer->data.virAddr, 0);
        if (ret != TD_SUCCESS) {
            HDF_LOGE("Flush ret =0x%x\n", ret);
        }
        return ret;
    }
    return ret;
}

int32_t EnabelVideoLayer(uint32_t devId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);

    const VO_LAYER voLayer = OT_VO_LAYER_V0;
    VO_VIDEO_LAYER_ATTR_S layerAttr = {0};
    int32_t ret = ss_mpi_vo_get_video_layer_attr(voLayer, &layerAttr);
    if (ret != 0) {
        HDF_LOGI("Camera video output get layer failed.(ret=0x%x)\n", ret);
    }

    // If the configuration is initialized first
    if (layerAttr.display_rect.width > 0 && layerAttr.display_rect.height > 0) {
        DisableVideoLayer(devId);
    }

    SetVoLayerParam(devId, &layerAttr);
    ret = ss_mpi_vo_set_video_layer_attr(voLayer, &layerAttr);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("ss_mpi_vo_set_video_layer_attr failure, ret = 0x%x", ret);
        return DISPLAY_FAILURE;
    }

    ret = ss_mpi_vo_enable_video_layer(voLayer);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("ss_mpi_vo_enable_video_layer failure, ret = 0x%x", ret);
        return DISPLAY_FAILURE;
    }

    VO_CSC_S stVideoCSC = {0};
    ret = ss_mpi_vo_get_layer_csc(voLayer, &stVideoCSC);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("ss_mpi_vo_get_layer_csc.(ret=0x%x)\n", ret);
        return DISPLAY_FAILURE;
    }

    stVideoCSC.csc_matrix = VO_CSC_MATRIX_IDENTITY;
    ret = ss_mpi_vo_set_layer_csc(voLayer, &stVideoCSC);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("ss_mpi_vo_set_layer_csc.(ret=0x%x)\n", ret);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

int32_t DisableVideoLayer(uint32_t devId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);

    int32_t ret = ss_mpi_vo_disable_video_layer((VO_LAYER)OT_VO_LAYER_V0);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("%s: ss_mpi_vo_disable_video_layer failure, ret 0x%x", __func__, ret);
    }
    return DISPLAY_SUCCESS;
}

int32_t CreateOverlayLayer(uint32_t devId, const LayerInfo *layerInfo, uint32_t *layerId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_NULLPOINTER_RETURN_VALUE(layerInfo, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(layerId, DISPLAY_NULL_PTR);

    const VO_LAYER voLayer = OT_VO_LAYER_V0;
    for (int32_t index = 0; index < OVERLAY_LAYER_NUM; index++) {
        OverlayLayerPrivate *priv = GetLayerInstance(devId, index > UINT8_MAX ? UINT8_MAX : index);
        if (priv->voChn != -1) {
            continue;
        }

        VO_CHN voChn = index;
        int32_t ret = EnabelVoChannel(voLayer, voChn, layerInfo->width < 0 ? 0 : layerInfo->width,
                                      layerInfo->height < 0 ? 0 : layerInfo->height);
        if (ret != DISPLAY_SUCCESS) {
            HDF_LOGE("ss_mpi_vo_set_chn_attr failure, ret = 0x%x", ret);
        }

        if (layerInfo != NULL) {
            ret = OpenVpssRes(layerInfo, &priv->vpssGrp, &priv->vpssChn);
            if (ret == DISPLAY_SUCCESS) {
                BindVoVpss(priv->vpssGrp, priv->vpssChn, voLayer, voChn);
                priv->vpssIsInternalEnable = true;
            }
        }
#if ENABLE_VO_TIME_TASK
        VoTimeTaskBegin();
#endif

#if ENABLE_VO_BUSYDROP
        VoDropTaskBegin();
#endif
        priv->voChn = voChn;
        uint32_t voLayerUint32 = voLayer < 0 ? 0 : (uint32_t)voLayer;
        uint32_t voChnUint32 = voChn < 0 ? 0 : (uint32_t)voChn;
        *layerId = LAYER_ID_VO_LAYER_TYPE_MARK | (voLayerUint32 << VO_LAYER_OFFSET) | voChnUint32;
        return DISPLAY_SUCCESS;
    }
    HDF_LOGE("%s: no layer to open", __func__);
    return DISPLAY_FAILURE;
}

int32_t CloseOverlayLayer(uint32_t devId, uint32_t layerId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return DISPLAY_FAILURE;
    }
    uint8_t vochn = GetVoChn(layerId);
    if (vochn >= OVERLAY_LAYER_NUM) {
        HDF_LOGE("%s: layerId is out of range", __func__);
        return DISPLAY_FAILURE;
    }

#if ENABLE_VO_BUSYDROP
    VoDropTaskEnd();
#endif

#if ENABLE_VO_TIME_TASK
    VoTimeTaskEnd();
#endif

    OverlayLayerPrivate *priv = GetLayerInstance(devId, vochn);
    if (priv->voChn == -1) {
        HDF_LOGE("%s: layerId error, layerId not create", __func__);
        return DISPLAY_FAILURE;
    }

    const VO_LAYER voLayer = OT_VO_LAYER_V0;
    if (priv->vpssGrp != -1 || priv->vpssChn != -1) {
        UnBindVoVpss(priv->vpssGrp, priv->vpssChn, voLayer, priv->voChn);
        if (priv->vpssIsInternalEnable) {
            CloseVpssRes(priv->vpssGrp, priv->vpssChn);
            priv->vpssGrp = -1;
            priv->vpssChn = -1;
        }
    }

    DisabelVoChannel(OT_VO_LAYER_V0, priv->voChn);
    priv->voChn = -1;
    return DISPLAY_SUCCESS;
}

int32_t SetOverlayLayerSize(uint32_t devId, uint32_t layerId, const IRect *rect)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_NULLPOINTER_RETURN_VALUE(rect, DISPLAY_NULL_PTR);
    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return DISPLAY_FAILURE;
    }
    uint8_t vochn = GetVoChn(layerId);
    if (vochn >= OVERLAY_LAYER_NUM) {
        HDF_LOGE("%s: layerId is out of range", __func__);
        return DISPLAY_FAILURE;
    }
    const VO_LAYER voLayer = GetVoLayer(layerId);

    VO_CHN_ATTR_S chnAttr = {0};
    int32_t ret = ss_mpi_vo_get_chn_attr(voLayer, vochn, &chnAttr);
    CHECK_CALL_MPI_RET(ret, ss_mpi_vo_get_chn_attr);

    IRect displayRect = {0};
    displayRect.x = ALIGN_DOWN(rect->x, ALIGN_DOWN_NUM);
    displayRect.y = ALIGN_DOWN(rect->y, ALIGN_DOWN_NUM);
    displayRect.w = ALIGN_DOWN(rect->w, ALIGN_DOWN_NUM);
    displayRect.h = ALIGN_DOWN(rect->h, ALIGN_DOWN_NUM);
    if (displayRect.x != chnAttr.rect.x || displayRect.y != chnAttr.rect.y ||
        displayRect.w != (int32_t)chnAttr.rect.width || displayRect.h != (int32_t)chnAttr.rect.height) {
        chnAttr.rect.x = displayRect.x;
        chnAttr.rect.y = displayRect.y;
        chnAttr.rect.width = displayRect.w < 0 ? 0 : (td_u32)displayRect.w;
        chnAttr.rect.height = displayRect.h < 0 ? 0 : (td_u32)displayRect.h;
        HDF_LOGI("set display rect (%d, %d, %u, %u)", chnAttr.rect.x, chnAttr.rect.y, chnAttr.rect.width,
                 chnAttr.rect.height);
        ret = ss_mpi_vo_set_chn_attr(voLayer, vochn, &chnAttr);
        CHECK_CALL_MPI_RET(ret, ss_mpi_vo_set_chn_attr);
    }

    return DISPLAY_SUCCESS;
}

static int32_t OverlayLayerCmdVoBindVpss(uint32_t devId, uint8_t index, VPSS_GRP vpssGrp, VPSS_CHN vpssChn)
{
    OverlayLayerPrivate *priv = GetLayerInstance(devId, index);
    const VO_LAYER voLayer = OT_VO_LAYER_V0;
    if (priv->vpssGrp != -1 && priv->vpssChn != -1) {
        UnBindVoVpss(priv->vpssGrp, priv->vpssChn, voLayer, priv->voChn);
        if (priv->vpssIsInternalEnable) {
            CloseVpssRes(priv->vpssGrp, priv->vpssChn);
            priv->vpssIsInternalEnable = false;
        }
    }
    int32_t ret = BindVoVpss(vpssGrp, vpssChn, voLayer, index);
    if (ret == DISPLAY_SUCCESS) {
        priv->vpssGrp = vpssGrp;
        priv->vpssChn = vpssChn;
    }
    return ret;
}

static int32_t OverlayLayerCmdVoUnBindVpss(uint32_t devId, uint8_t index, VPSS_GRP vpssGrp, VPSS_CHN vpssChn)
{
    OverlayLayerPrivate *priv = GetLayerInstance(devId, index);
    const VO_LAYER voLayer = OT_VO_LAYER_V0;
    if (priv->vpssGrp == vpssGrp && priv->vpssChn == vpssChn) {
        UnBindVoVpss(priv->vpssGrp, priv->vpssChn, voLayer, priv->voChn);
        if (priv->vpssIsInternalEnable) {
            CloseVpssRes(priv->vpssGrp, priv->vpssChn);
            priv->vpssIsInternalEnable = false;
        }
        priv->vpssGrp = -1;
        priv->vpssChn = -1;
    } else {
        UnBindVoVpss(vpssGrp, vpssChn, voLayer, priv->voChn);
    }
    return DISPLAY_SUCCESS;
}

static int32_t OverlayLayerCmdVdecVpss(uint32_t devId, uint8_t index, ot_vdec_chn vdecChn, bool isBind)
{
    OverlayLayerPrivate *priv = GetLayerInstance(devId, index);
    ot_mpp_chn srcChn;
    ot_mpp_chn destChn;

    srcChn.mod_id = OT_ID_VDEC;
    srcChn.dev_id = 0;
    srcChn.chn_id = vdecChn;

    destChn.mod_id = OT_ID_VPSS;
    destChn.dev_id = priv->vpssGrp;
    destChn.chn_id = priv->vpssChn;

    int32_t ret = DISPLAY_FAILURE;
    if (isBind) {
        ret = ss_mpi_sys_bind(&srcChn, &destChn);
        CHECK_CALL_MPI_RET(ret, ss_mpi_sys_bind);
        HDF_LOGI("bind vdec&vpss success, vpssGrp=%d vpssChn=%d vdecChn=%d", priv->vpssGrp, priv->vpssChn, vdecChn);
    } else {
        ret = ss_mpi_sys_unbind(&srcChn, &destChn);
        CHECK_CALL_MPI_RET(ret, ss_mpi_sys_unbind);
        HDF_LOGI("unbind vdec&vpss success, vpssGrp=%d vpssChn=%d vdecChn=%d", priv->vpssGrp, priv->vpssChn, vdecChn);
    }
    return ret;
}

int32_t InvokeOverlayLayerCmd(uint32_t devId, uint32_t layerId, uint32_t cmd, ...)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    if (cmd > OVERLAYER_CMD_MAX) {
        HDF_LOGE("%s: not support cmd 0x%x\n", __func__, cmd);
        return DISPLAY_FAILURE;
    }
    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return DISPLAY_FAILURE;
    }
    uint8_t vochn = GetVoChn(layerId);
    if (vochn >= OVERLAY_LAYER_NUM) {
        HDF_LOGE("%s: layerId is out of range", __func__);
        return DISPLAY_FAILURE;
    }

    va_list vl;
    va_start(vl, cmd);
    int32_t ret = DISPLAY_FAILURE;
    VPSS_CHN vpssChn;
    VPSS_GRP vpssGroup;
    ot_vdec_chn vdecChn;
    switch (cmd) {
        case OVERLAYER_CMD_VO_BIND_VPSS:
            vpssChn = va_arg(vl, int32_t);
            vpssGroup = va_arg(vl, int32_t);
            ret = OverlayLayerCmdVoBindVpss(devId, vochn, vpssGroup, vpssChn);
            break;
        case OVERLAYER_CMD_VO_UNBIND_VPSS:
            vpssChn = va_arg(vl, int32_t);
            vpssGroup = va_arg(vl, int32_t);
            ret = OverlayLayerCmdVoUnBindVpss(devId, vochn, vpssGroup, vpssChn);
            break;
        case OVERLAYER_CMD_VPSS_BING_VDEC:
            vdecChn = va_arg(vl, int32_t);
            ret = OverlayLayerCmdVdecVpss(devId, vochn, vdecChn, true);
            break;
        case OVERLAYER_CMD_VPSS_UNBING_VDEC:
            vdecChn = va_arg(vl, int32_t);
            ret = OverlayLayerCmdVdecVpss(devId, vochn, vdecChn, false);
            break;
        default:
            break;
    }
    va_end(vl);
    return ret;
}

int32_t GetOverlayLayerDeviceId(uint32_t devId, uint32_t layerId, uint32_t *deviceId)
{
    if (deviceId == NULL) {
        return DISPLAY_FAILURE;
    }
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return DISPLAY_FAILURE;
    }

    uint8_t vochn = GetVoChn(layerId);
    if (vochn >= OVERLAY_LAYER_NUM) {
        HDF_LOGE("%s: layerId is out of range", __func__);
        return DISPLAY_FAILURE;
    }

    OverlayLayerPrivate *priv = GetLayerInstance(devId, vochn);
    uint32_t vpssGrpUint32 = priv->vpssGrp < 0 ? 0 : (uint32_t)priv->vpssGrp;
    uint32_t vpssChnUint32 = priv->vpssChn < 0 ? 0 : (uint32_t)priv->vpssChn;
    *deviceId = (((OT_ID_VPSS << 0x10) & 0xff0000) + ((vpssGrpUint32 << OFFSET_8) & 0xff00) +
                 (vpssChnUint32 & 0xff));  // 8: offset
    HDF_LOGI("GetOverlayLayerDeviceId (%u)", *deviceId);
    return DISPLAY_SUCCESS;
}

int32_t SetOverlayLayerPriority(uint32_t priority)
{
    const VO_LAYER voLayer = OT_VO_LAYER_V0;
    HDF_LOGI("SetOverlayLayerPriority priority = %d", priority);
    ss_mpi_vo_set_layer_priority(voLayer, priority);
    return DISPLAY_SUCCESS;
}
