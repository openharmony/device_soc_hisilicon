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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include "sample_comm.h"
#include "hi_mipi_tx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define SAMPLE_VO_DEF_VALUE (-1)

combo_dev_cfg_t MIPI_TX_1920X1080_60_CONFIG = {
    .devno = 0,
    .lane_id = {0, 1, 2, 3},
    .output_mode = OUTPUT_MODE_DSI_VIDEO,
    .output_format = OUT_FORMAT_RGB_24_BIT,
    .video_mode =  BURST_MODE,
    .sync_info = {
        .vid_pkt_size = IMG_2M_WIDTH,
        .vid_hsa_pixels = 44,
        .vid_hbp_pixels = 148,
        .vid_hline_pixels = 2200,
        .vid_vsa_lines = 5,
        .vid_vbp_lines = 36,
        .vid_vfp_lines = 4,
        .vid_active_lines = IMG_2M_HEIGHT,
        .edpi_cmd_size = 0,
    },
    .phy_data_rate = 945,
    .pixel_clk = 148500,
};

combo_dev_cfg_t MIPI_TX_1080X1920_60_CONFIG = {
    .devno = 0,
    .lane_id = {0, 1, 2, 3},
    .output_mode = OUTPUT_MODE_DSI_VIDEO,
    .output_format = OUT_FORMAT_RGB_24_BIT,
    .video_mode =  BURST_MODE,
    .sync_info = {
        .vid_pkt_size     = IMG_2M_HEIGHT,
        .vid_hsa_pixels   = 8,
        .vid_hbp_pixels   = 20,
        .vid_hline_pixels = 1238,
        .vid_vsa_lines    = 10,
        .vid_vbp_lines    = 26,
        .vid_vfp_lines    = 16,
        .vid_active_lines = IMG_2M_WIDTH,
        .edpi_cmd_size = 0,
    },
    .phy_data_rate = 945,
    .pixel_clk = 148500,
};

HI_S32 SAMPLE_COMM_VO_GetWH(VO_INTF_SYNC_E enIntfSync, HI_U32 *pu32W, HI_U32 *pu32H, HI_U32 *pu32Frm)
{
    CHECK_NULL_PTR(pu32W);
    CHECK_NULL_PTR(pu32H);
    CHECK_NULL_PTR(pu32Frm);

    switch (enIntfSync) {
        case VO_OUTPUT_1080P25:
            *pu32W = IMG_2M_WIDTH;
            *pu32H = IMG_2M_HEIGHT;
            *pu32Frm = FPS_25;
            break;
        case VO_OUTPUT_1080P30:
            *pu32W = IMG_2M_WIDTH;
            *pu32H = IMG_2M_HEIGHT;
            *pu32Frm = FPS_30;
            break;
        case VO_OUTPUT_1080I50:
            *pu32W = IMG_2M_WIDTH;
            *pu32H = IMG_2M_HEIGHT;
            *pu32Frm = FPS_50;
            break;
        case VO_OUTPUT_1080I60:
            *pu32W = IMG_2M_WIDTH;
            *pu32H = IMG_2M_HEIGHT;
            *pu32Frm = FPS_60;
            break;
        case VO_OUTPUT_1080P50:
            *pu32W = IMG_2M_WIDTH;
            *pu32H = IMG_2M_HEIGHT;
            *pu32Frm = FPS_50;
            break;
        case VO_OUTPUT_1080P60:
            *pu32W = IMG_2M_WIDTH;
            *pu32H = IMG_2M_HEIGHT;
            *pu32Frm = FPS_60;
            break;
        case VO_OUTPUT_800x600_60:
            *pu32W = 800; /* width 800 */
            *pu32H = 600; /* height 600 */
            *pu32Frm = FPS_60;
            break;
        case VO_OUTPUT_800x600_50:
            *pu32W = 800; /* width 800 */
            *pu32H = 600; /* height 600 */
            *pu32Frm = FPS_50;
            break;
        case VO_OUTPUT_1080x1920_60:
            *pu32W = IMG_2M_HEIGHT;
            *pu32H = IMG_2M_WIDTH;
            *pu32Frm = FPS_60;
        case VO_OUTPUT_USER:
            *pu32W = 720; /* width 720 */
            *pu32H = 576; /* height 576 */
            *pu32Frm = FPS_25;
            break;
        default:
            SAMPLE_PRT("vo enIntfSync %d not support!\n", enIntfSync);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VO_StartDev(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr)
{
    HI_S32 s32Ret;

    CHECK_NULL_PTR(pstPubAttr);

    s32Ret = HI_MPI_VO_SetPubAttr(VoDev, pstPubAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VO_Enable(VoDev);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VO_StopDev(VO_DEV VoDev)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VO_Disable(VoDev);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VO_StartLayer(VO_LAYER VoLayer, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
{
    HI_S32 s32Ret;

    CHECK_NULL_PTR(pstLayerAttr);

    s32Ret = HI_MPI_VO_SetVideoLayerAttr(VoLayer, pstLayerAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VO_EnableVideoLayer(VoLayer);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VO_StopLayer(VO_LAYER VoLayer)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VO_DisableVideoLayer(VoLayer);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VO_StartChn(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode)
{
    HI_U32 i;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32WndNum = 0;
    HI_U32 u32Square = 0;
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    VO_CHN_ATTR_S stChnAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    switch (enMode) {
        case VO_MODE_1MUX:
            u32WndNum = 1; /* wndnum: 1 */
            u32Square = 1; /* square: 1 */
            break;
        case VO_MODE_2MUX:
            u32WndNum = 2; /* wndnum: 2 */
            u32Square = 2; /* square: 2 */
            break;
        case VO_MODE_4MUX:
            u32WndNum = 4; /* wndnum: 4 */
            u32Square = 2; /* square: 2 */
            break;
        default:
            SAMPLE_PRT("enMode is invalid!\n");
            return s32Ret;
    }

    s32Ret = HI_MPI_VO_GetVideoLayerAttr(VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return s32Ret;
    }
    u32Width = stLayerAttr.stImageSize.u32Width;
    u32Height = stLayerAttr.stImageSize.u32Height;
    SAMPLE_PRT("u32Width:%d, u32Height:%d, u32Square:%d\n", u32Width, u32Height, u32Square);
    for (i = 0; i < u32WndNum; i++) {
        stChnAttr.stRect.s32X = HI_ALIGN_DOWN((u32Width / u32Square) * (i % u32Square), 2); /* 2 align */
        stChnAttr.stRect.s32Y = HI_ALIGN_DOWN((u32Height / u32Square) * (i / u32Square), 2); /* 2 align */
        stChnAttr.stRect.u32Width = HI_ALIGN_DOWN(u32Width / u32Square, 2); /* 2 align */
        stChnAttr.stRect.u32Height = HI_ALIGN_DOWN(u32Height / u32Square, 2); /* 2 align */
        stChnAttr.u32Priority = 0;
        stChnAttr.bDeflicker = HI_FALSE;
        s32Ret = HI_MPI_VO_SetChnAttr(VoLayer, i, &stChnAttr);
        if (s32Ret != HI_SUCCESS) {
            printf("%s(%d):failed with %#x!\n", __FUNCTION__, __LINE__, s32Ret);
            return s32Ret;
        }

        s32Ret = HI_MPI_VO_EnableChn(VoLayer, i);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VO_StopChn(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode)
{
    HI_U32 i;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32WndNum = 0;

    switch (enMode) {
        case VO_MODE_1MUX: {
            u32WndNum = 1; /* wndnum: 1 */
            break;
        }
        case VO_MODE_2MUX: {
            u32WndNum = 2; /* wndnum: 2 */
            break;
        }
        case VO_MODE_4MUX: {
            u32WndNum = 4; /* wndnum: 4 */
            break;
        }
        default:
            SAMPLE_PRT("enMode is invalid!\n");
            return s32Ret;
    }

    for (i = 0; i < u32WndNum; i++) {
        s32Ret = HI_MPI_VO_DisableChn(VoLayer, i);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    return s32Ret;
}

#ifdef CONFIG_HI_HDMI_SUPPORT
static HI_S32 SAMPLE_COMM_VO_HdmiConvertSync(VO_INTF_SYNC_E enIntfSync, HI_HDMI_VIDEO_FMT_E *penVideoFmt)
{
    CHECK_NULL_PTR(penVideoFmt);

    switch (enIntfSync) {
        case VO_OUTPUT_1080P25:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_25;
            break;
        case VO_OUTPUT_1080P30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_30;
            break;
        case VO_OUTPUT_1080I50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080i_50;
            break;
        case VO_OUTPUT_1080I60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080i_60;
            break;
        case VO_OUTPUT_1080P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_50;
            break;
        case VO_OUTPUT_1080P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_60;
            break;
        default:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_60;
            break;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VO_HdmiStart(VO_INTF_SYNC_E enIntfSync)
{
    HI_S32 s32Ret;
    HI_HDMI_ATTR_S stAttr;
    HI_HDMI_VIDEO_FMT_E enVideoFmt;
    HI_HDMI_ID_E enHdmiId = HI_HDMI_ID_0;

    s32Ret = SAMPLE_COMM_VO_HdmiConvertSync(enIntfSync, &enVideoFmt);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    CHECK_RET(HI_MPI_HDMI_Init(), "HI_MPI_HDMI_Init");
    CHECK_RET(HI_MPI_HDMI_Open(enHdmiId), "HI_MPI_HDMI_Open");
    CHECK_RET(HI_MPI_HDMI_GetAttr(enHdmiId, &stAttr), "HI_MPI_HDMI_GetAttr");
    stAttr.bEnableHdmi = HI_TRUE;
    stAttr.bEnableVideo = HI_TRUE;
    stAttr.enVideoFmt = enVideoFmt;
    stAttr.enVidOutMode = HI_HDMI_VIDEO_MODE_YCBCR444;
    stAttr.enDeepColorMode = HI_HDMI_DEEP_COLOR_24BIT;
    stAttr.bxvYCCMode = HI_FALSE;
    stAttr.enOutCscQuantization = HDMI_QUANTIZATION_LIMITED_RANGE;

    stAttr.bEnableAudio = HI_FALSE;
    stAttr.enSoundIntf = HI_HDMI_SND_INTERFACE_I2S;
    stAttr.bIsMultiChannel = HI_FALSE;

    stAttr.enBitDepth = HI_HDMI_BIT_DEPTH_16;

    stAttr.bEnableAviInfoFrame = HI_TRUE;
    stAttr.bEnableAudInfoFrame = HI_TRUE;
    stAttr.bEnableSpdInfoFrame = HI_FALSE;
    stAttr.bEnableMpegInfoFrame = HI_FALSE;

    stAttr.bDebugFlag = HI_FALSE;
    stAttr.bHDCPEnable = HI_FALSE;

    stAttr.b3DEnable = HI_FALSE;
    stAttr.enDefaultMode = HI_HDMI_FORCE_HDMI;

    CHECK_RET(HI_MPI_HDMI_SetAttr(enHdmiId, &stAttr), "HI_MPI_HDMI_SetAttr");
    CHECK_RET(HI_MPI_HDMI_Start(enHdmiId), "HI_MPI_HDMI_Start");

    return HI_SUCCESS;
}

/*
 * Name : SAMPLE_COMM_VO_HdmiStartByDyRg
 * Desc : Another function to start hdmi, according to video's dynamic range.
 */
HI_S32 SAMPLE_COMM_VO_HdmiStartByDyRg(VO_INTF_SYNC_E enIntfSync, DYNAMIC_RANGE_E enDyRg)
{
    HI_S32 s32Ret;
    HI_HDMI_ATTR_S stAttr;
    HI_HDMI_VIDEO_FMT_E enVideoFmt;
    HI_HDMI_ID_E enHdmiId = HI_HDMI_ID_0;

    s32Ret = SAMPLE_COMM_VO_HdmiConvertSync(enIntfSync, &enVideoFmt);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    CHECK_RET(HI_MPI_HDMI_Init(), "HI_MPI_HDMI_Init");
    CHECK_RET(HI_MPI_HDMI_Open(enHdmiId), "HI_MPI_HDMI_Open");
    CHECK_RET(HI_MPI_HDMI_GetAttr(enHdmiId, &stAttr), "HI_MPI_HDMI_GetAttr");
    stAttr.bEnableHdmi = HI_TRUE;
    stAttr.bEnableVideo = HI_TRUE;
    stAttr.enVideoFmt = enVideoFmt;
    stAttr.enVidOutMode = HI_HDMI_VIDEO_MODE_YCBCR444;
    switch (enDyRg) {
        case DYNAMIC_RANGE_SDR8:
            stAttr.enDeepColorMode = HI_HDMI_DEEP_COLOR_24BIT;
            break;
        case DYNAMIC_RANGE_HDR10:
            stAttr.enVidOutMode = HI_HDMI_VIDEO_MODE_YCBCR422;
            break;
        default:
            stAttr.enDeepColorMode = HI_HDMI_DEEP_COLOR_24BIT;
            break;
    }
    stAttr.bxvYCCMode = HI_FALSE;
    stAttr.enOutCscQuantization = HDMI_QUANTIZATION_LIMITED_RANGE;

    stAttr.bEnableAudio = HI_TRUE;
    stAttr.enSoundIntf = HI_HDMI_SND_INTERFACE_I2S;
    stAttr.bIsMultiChannel = HI_FALSE;

    stAttr.enBitDepth = HI_HDMI_BIT_DEPTH_16;

    stAttr.bEnableAviInfoFrame = HI_TRUE;
    stAttr.bEnableAudInfoFrame = HI_TRUE;
    stAttr.bEnableSpdInfoFrame = HI_FALSE;
    stAttr.bEnableMpegInfoFrame = HI_FALSE;

    stAttr.bDebugFlag = HI_FALSE;
    stAttr.bHDCPEnable = HI_FALSE;

    stAttr.b3DEnable = HI_FALSE;
    stAttr.enDefaultMode = HI_HDMI_FORCE_HDMI;

    CHECK_RET(HI_MPI_HDMI_SetAttr(enHdmiId, &stAttr), "HI_MPI_HDMI_SetAttr");
    CHECK_RET(HI_MPI_HDMI_Start(enHdmiId), "HI_MPI_HDMI_Start");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VO_HdmiStop(HI_VOID)
{
    HI_HDMI_ID_E enHdmiId = HI_HDMI_ID_0;

    HI_MPI_HDMI_Stop(enHdmiId);
    HI_MPI_HDMI_Close(enHdmiId);
    HI_MPI_HDMI_DeInit();

    return HI_SUCCESS;
}
#endif

static void SAMPLE_COMM_VO_StartMipiTx(VO_INTF_SYNC_E enVoIntfSync);

/*
 * Name : SAMPLE_COMM_VO_GetDefConfig
 * Desc : An instance of SAMPLE_VO_CONFIG_S, which allows you to use vo immediately.
 */
HI_S32 SAMPLE_COMM_VO_GetDefConfig(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    RECT_S stDefDispRect = { 0, 0, IMG_2M_WIDTH, IMG_2M_HEIGHT };
    SIZE_S stDefImageSize = { IMG_2M_WIDTH, IMG_2M_HEIGHT };
    HI_U32 u32ChipId = 0;

    CHECK_NULL_PTR(pstVoConfig);

    pstVoConfig->VoDev = SAMPLE_VO_DEV_UHD;

    HI_MPI_SYS_GetChipId(&u32ChipId);

    if (HI3516C_V500 == u32ChipId) {
        pstVoConfig->enVoIntfType = VO_INTF_BT1120;
    } else {
#ifdef CONFIG_HI_HDMI_SUPPORT
        pstVoConfig->enVoIntfType = VO_INTF_HDMI;
        pstVoConfig->enIntfSync = VO_OUTPUT_1080P30;
#else
        pstVoConfig->enVoIntfType = VO_INTF_LCD_6BIT;
        pstVoConfig->enIntfSync = VO_OUTPUT_320x240_50;
#endif
    }
    pstVoConfig->u32BgColor = COLOR_RGB_BLUE;
    pstVoConfig->enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    pstVoConfig->stDispRect = stDefDispRect;
    pstVoConfig->stImageSize = stDefImageSize;
    pstVoConfig->enVoPartMode = VO_PART_MODE_SINGLE;
    pstVoConfig->u32DisBufLen = 3; /* dis buffler len: 3 */
    pstVoConfig->enDstDynamicRange = DYNAMIC_RANGE_SDR8;
    pstVoConfig->enVoMode = VO_MODE_1MUX;

    return HI_SUCCESS;
}
HI_S32 SAMPLE_COMM_VO_StartVO(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    RECT_S stDefDispRect = { 0, 0, IMG_2M_WIDTH, IMG_2M_HEIGHT };
    SIZE_S stDefImageSize = { IMG_2M_WIDTH, IMG_2M_HEIGHT };
    VO_DEV VoDev = 0;
    VO_LAYER VoLayer = 0;
    SAMPLE_VO_MODE_E enVoMode = 0;
    VO_INTF_TYPE_E enVoIntfType = VO_INTF_HDMI;
    VO_INTF_SYNC_E enIntfSync = VO_OUTPUT_1080P30;
    DYNAMIC_RANGE_E enDstDyRg = DYNAMIC_RANGE_SDR8;
    VO_PART_MODE_E enVoPartMode = VO_PART_MODE_SINGLE;
    VO_PUB_ATTR_S stVoPubAttr = { 0 };
    VO_VIDEO_LAYER_ATTR_S stLayerAttr = { 0 };
    VO_CSC_S stVideoCSC = { 0 };
    HI_S32 s32Ret;

    CHECK_NULL_PTR(pstVoConfig);

    VoDev = pstVoConfig->VoDev;
    VoLayer = pstVoConfig->VoDev;
    enVoMode = pstVoConfig->enVoMode;
    enVoIntfType = pstVoConfig->enVoIntfType;
    enIntfSync = pstVoConfig->enIntfSync;
    enDstDyRg = pstVoConfig->enDstDynamicRange;
    enVoPartMode = pstVoConfig->enVoPartMode;

    /* Set and start VO device VoDev. */
#ifdef CONFIG_HI_HDMI_SUPPORT
    stVoPubAttr.enIntfType = enVoIntfType;
    stVoPubAttr.enIntfSync = enIntfSync;
#else
    stVoPubAttr.enIntfType = VO_INTF_LCD_6BIT;
    stVoPubAttr.enIntfSync = VO_OUTPUT_240x320_50;
#endif
    stVoPubAttr.u32BgColor = pstVoConfig->u32BgColor;
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        return s32Ret;
    }

    /* Set and start layer VoDev */
    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync, &stLayerAttr.stDispRect.u32Width,
        &stLayerAttr.stDispRect.u32Height, &stLayerAttr.u32DispFrmRt);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        SAMPLE_COMM_VO_StopDev(VoDev);
        return s32Ret;
    }
    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = pstVoConfig->enPixFormat;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    if (memcmp(&pstVoConfig->stDispRect, &stDefDispRect, sizeof(RECT_S)) != 0) {
        (HI_VOID)memcpy_s(&stLayerAttr.stDispRect, sizeof(RECT_S), &pstVoConfig->stDispRect, sizeof(RECT_S));
    }
    if (memcmp(&pstVoConfig->stImageSize, &stDefImageSize, sizeof(SIZE_S)) != 0) {
        (HI_VOID)memcpy_s(&stLayerAttr.stImageSize, sizeof(SIZE_S), &pstVoConfig->stImageSize, sizeof(SIZE_S));
    }
#ifdef CONFIG_HI_HDMI_SUPPORT
    stLayerAttr.stImageSize.u32Width = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;
#else
    stLayerAttr.stImageSize.u32Width = stLayerAttr.stDispRect.u32Width = 240; /* width: 240 */
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height = 320; /* height: 320 */
#endif
    stLayerAttr.enDstDynamicRange = pstVoConfig->enDstDynamicRange;
    if (pstVoConfig->u32DisBufLen) {
        s32Ret = HI_MPI_VO_SetDisplayBufLen(VoLayer, pstVoConfig->u32DisBufLen);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_VO_SetDisplayBufLen failed with %#x!\n", s32Ret);
            SAMPLE_COMM_VO_StopDev(VoDev);
            return s32Ret;
        }
    }
    if (enVoPartMode == VO_PART_MODE_MULTI) {
        s32Ret = HI_MPI_VO_SetVideoLayerPartitionMode(VoLayer, enVoPartMode);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_VO_SetVideoLayerPartitionMode failed!\n");
            SAMPLE_COMM_VO_StopDev(VoDev);
            return s32Ret;
        }
    }

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VO_Start video layer failed!\n");
        SAMPLE_COMM_VO_StopDev(VoDev);
        return s32Ret;
    }

    if (VO_INTF_MIPI == enVoIntfType) {
        s32Ret = HI_MPI_VO_GetVideoLayerCSC(VoLayer, &stVideoCSC);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_VO_GetVideoLayerCSC failed!\n");
            SAMPLE_COMM_VO_StopDev(VoDev);
            return s32Ret;
        }
        stVideoCSC.enCscMatrix = VO_CSC_MATRIX_BT709_TO_RGB_PC;
        s32Ret = HI_MPI_VO_SetVideoLayerCSC(VoLayer, &stVideoCSC);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_VO_SetVideoLayerCSC failed!\n");
            SAMPLE_COMM_VO_StopDev(VoDev);
            return s32Ret;
        }
    }
    s32Ret = SAMPLE_COMM_VO_StartChn(VoLayer, enVoMode);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        SAMPLE_COMM_VO_StopLayer(VoLayer);
        SAMPLE_COMM_VO_StopDev(VoDev);
        return s32Ret;
    }

#ifdef CONFIG_HI_HDMI_SUPPORT
    if (VO_INTF_HDMI & enVoIntfType) {
        SAMPLE_COMM_VO_HdmiStartByDyRg(enIntfSync, enDstDyRg);
    }
#endif
    if (VO_INTF_MIPI & enVoIntfType) {
        SAMPLE_COMM_VO_StartMipiTx(enIntfSync);
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VO_StopVO(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    VO_DEV VoDev = 0;
    VO_LAYER VoLayer = 0;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_BUTT;

    CHECK_NULL_PTR(pstVoConfig);

    VoDev = pstVoConfig->VoDev;
    VoLayer = pstVoConfig->VoDev;
    enVoMode = pstVoConfig->enVoMode;

#ifdef CONFIG_HI_HDMI_SUPPORT
    if (VO_INTF_HDMI & pstVoConfig->enVoIntfType) {
        SAMPLE_COMM_VO_HdmiStop();
    }
#endif

    SAMPLE_COMM_VO_StopChn(VoLayer, enVoMode);
    SAMPLE_COMM_VO_StopLayer(VoLayer);
    SAMPLE_COMM_VO_StopDev(VoDev);

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VO_StartLayerChn(SAMPLE_COMM_VO_LAYER_CONFIG_S *pstVoLayerConfig)
{
    RECT_S stDefDispRect = { 0, 0, IMG_2M_WIDTH, IMG_2M_HEIGHT };
    SIZE_S stDefImageSize = { IMG_2M_WIDTH, IMG_2M_HEIGHT };
    VO_LAYER VoLayer = 0;
    VO_INTF_SYNC_E enIntfSync = VO_OUTPUT_1080P30;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_BUTT;
    HI_S32 s32Ret;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    HI_U32 u32Frmt, u32Width, u32Height;

    CHECK_NULL_PTR(pstVoLayerConfig);

    VoLayer = pstVoLayerConfig->VoLayer;
    enIntfSync = pstVoLayerConfig->enIntfSync;
    enVoMode = pstVoLayerConfig->enVoMode;

    /* start vo layer. */
    s32Ret = SAMPLE_COMM_VO_GetWH(enIntfSync, &u32Width, &u32Height, &u32Frmt);
    if (s32Ret != HI_SUCCESS) {
        printf("Can not get synchronization information!\n");
        return HI_FAILURE;
    }

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    stLayerAttr.stDispRect.u32Width = u32Width;
    stLayerAttr.stDispRect.u32Height = u32Height;
    stLayerAttr.stImageSize.u32Width = u32Width;
    stLayerAttr.stImageSize.u32Height = u32Height;
    stLayerAttr.u32DispFrmRt = u32Frmt;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.enDstDynamicRange = DYNAMIC_RANGE_SDR8;
    stLayerAttr.enPixFormat = pstVoLayerConfig->enPixFormat;
    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    if (memcmp(&pstVoLayerConfig->stDispRect, &stDefDispRect, sizeof(RECT_S)) != 0) {
        stLayerAttr.stDispRect = pstVoLayerConfig->stDispRect;
    }
    stLayerAttr.stImageSize.u32Width = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;
    stLayerAttr.stImageSize.u32Width = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;
    if (memcmp(&pstVoLayerConfig->stImageSize, &stDefImageSize, sizeof(SIZE_S)) != 0) {
        stLayerAttr.stImageSize = pstVoLayerConfig->stImageSize;
    }
    stLayerAttr.enDstDynamicRange = pstVoLayerConfig->enDstDynamicRange;
    if (pstVoLayerConfig->u32DisBufLen) {
        s32Ret = HI_MPI_VO_SetDisplayBufLen(VoLayer, pstVoLayerConfig->u32DisBufLen);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_VO_SetDisplayBufLen failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VO_Start video layer failed!\n");
        return s32Ret;
    }

    /* start vo channels. */
    s32Ret = SAMPLE_COMM_VO_StartChn(VoLayer, enVoMode);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        SAMPLE_COMM_VO_StopLayer(VoLayer);
        return s32Ret;
    }
    return s32Ret;
}

HI_S32 SAMPLE_COMM_VO_StopLayerChn(SAMPLE_COMM_VO_LAYER_CONFIG_S *pstVoLayerConfig)
{
    VO_LAYER VoLayer = 0;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_BUTT;

    CHECK_NULL_PTR(pstVoLayerConfig);

    VoLayer = pstVoLayerConfig->VoLayer;
    enVoMode = pstVoLayerConfig->enVoMode;

    SAMPLE_COMM_VO_StopChn(VoLayer, enVoMode);
    SAMPLE_COMM_VO_StopLayer(VoLayer);

    return HI_SUCCESS;
}

HI_VOID SAMPLE_COMM_VO_Exit(void)
{
    HI_S32 i = 0, j = 0;

    for (i = 0; i < VO_MAX_LAYER_NUM; i++) {
        for (j = 0; j < VO_MAX_CHN_NUM; j++) {
            HI_MPI_VO_DisableChn(i, j);
        }
    }

    for (i = 0; i < VO_MAX_LAYER_NUM; i++) {
        HI_MPI_VO_DisableVideoLayer(i);
    }

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        HI_MPI_VO_Disable(i);
    }
}

static HI_VOID SAMPLE_PRIVATE_VO_InitScreen1080x1920(HI_S32 fd)
{
    HI_S32 s32Ret;
    cmd_info_t cmd_info;

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0xeeff; /* cmd size: 0xeeff */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(1000); /* sleep 1000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x4018; /* cmd size: 0x4018 */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(10000); /* sleep 10000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x18; /* cmd size: 0x18 */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(20000); /* sleep 20000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0xff; /* cmd size: 0xff */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(10000); /* sleep 10000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x1fb; /* cmd size: 0x1fb */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(10000); /* sleep 10000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x135; /* cmd size: 0x135 */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(10000); /* sleep 10000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0xff51; /* cmd size: 0xff51 */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(1000); /* sleep 1000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x2c53; /* cmd size: 0x2c53 */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(1000); /* sleep 1000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x155; /* cmd size: 0x155 */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(1000); /* sleep 1000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x24d3; /* cmd size: 0x24d3 */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(10000); /* sleep 10000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x10d4; /* cmd size: 0x10d4 */
    cmd_info.data_type = 0x23; /* data type: 0x23 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(10000); /* sleep 10000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x11; /* cmd size: 0x11 */
    cmd_info.data_type = 0x05; /* data type: 0x05 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }

    usleep(200000); /* sleep 200000 us */

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x29; /* cmd size: 0x29 */
    cmd_info.data_type = 0x05; /* data type: 0x05 */
    cmd_info.cmd = NULL;
    s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET CMD failed\n");
        close(fd);
        return;
    }
}

static void SAMPLE_PRIVATE_VO_InitMipiTxScreen(VO_INTF_SYNC_E enVoIntfSync, HI_S32 fd)
{
    SAMPLE_PRT("%s,%d,Init 1080p screen.\n", __FUNCTION__, __LINE__);
    SAMPLE_PRIVATE_VO_InitScreen1080x1920(fd);
}

void SAMPLE_COMM_VO_StartMipiTx(VO_INTF_SYNC_E enVoIntfSync)
{
    HI_S32 fd;
    HI_S32 s32Ret;
    combo_dev_cfg_t *pstMipiTxConfig = HI_NULL;

    fd = open("/dev/hi_mipi_tx", O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi_tx dev failed\n");
        return;
    }

    switch (enVoIntfSync) {
        case VO_OUTPUT_1080P60:
            pstMipiTxConfig = &MIPI_TX_1920X1080_60_CONFIG;
            break;
        case VO_OUTPUT_1080x1920_60:
            pstMipiTxConfig = &MIPI_TX_1080X1920_60_CONFIG;
            break;
        default:
            pstMipiTxConfig = &MIPI_TX_1080X1920_60_CONFIG;
            break;
    }

    s32Ret = ioctl(fd, HI_MIPI_TX_SET_DEV_CFG, pstMipiTxConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX SET_DEV_CONFIG failed\n");
        close(fd);
        return;
    }

    SAMPLE_PRIVATE_VO_InitMipiTxScreen(enVoIntfSync, fd);

    usleep(10000); /* sleep 10000 us to enable mipitx */
    s32Ret = ioctl(fd, HI_MIPI_TX_ENABLE);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("MIPI_TX enable failed\n");
    }

    close(fd);
    fd = HI_INVALID_VALUE;
    return;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
