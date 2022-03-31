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
#include "higv_mw_media.h"
#include "hi_common.h"
#include "hi_buffer.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include "hi_comm_isp.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_venc.h"
#include "hi_comm_vdec.h"
#include "hi_comm_vpss.h"
#include "hi_comm_region.h"
#include "hi_comm_adec.h"
#include "hi_comm_aenc.h"
#include "hi_comm_ai.h"
#include "hi_comm_ao.h"
#include "hi_comm_aio.h"
#include "hi_defines.h"
#include "hi_comm_hdmi.h"
#include "hi_mipi.h"
#include "hi_comm_vgs.h"
#include "mpi_sys.h"
#include "mpi_vb.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"
#include "mpi_vdec.h"
#include "mpi_vpss.h"
#include "mpi_region.h"
#include "mpi_audio.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "hi_math.h"
#include "hi_sns_ctrl.h"
#include "mpi_hdmi.h"
#include "mpi_vgs.h"

HI_S32 SAMPLE_COMM_SYS_Init(const VB_CONFIG_S *pstVbConfig)
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();

    if (pstVbConfig == NULL) {
        return HI_FAILURE;
    }

    HI_S32 ret = HI_MPI_VB_SetConfig(pstVbConfig);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    ret = HI_MPI_VB_Init();
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    ret = HI_MPI_SYS_Init();
    if (ret != HI_SUCCESS) {
        HI_MPI_VB_Exit();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 HI_MW_Sys_Init(HI_VOID)
{
    VB_CONFIG_S vbConf = { 0 };
    vbConf.u32MaxPoolCnt = 1;
    vbConf.astCommPool[0].u64BlkSize = 1920 * 1080 * 2; /* 1920/1080/2: calculate block buf size */
    vbConf.astCommPool[0].u32BlkCnt = 4; /* 4: block cnt */
    HI_S32 ret = SAMPLE_COMM_SYS_Init(&vbConf);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VO_StartDev(VO_DEV voDev, const VO_PUB_ATTR_S *pstPubAttr)
{
    HI_S32 ret;

    ret = HI_MPI_VO_SetPubAttr(voDev, pstPubAttr);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    ret = HI_MPI_VO_Enable(voDev);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return ret;
}

HI_VOID SAMPLE_COMM_VO_HdmiConvertSync(VO_INTF_SYNC_E enIntfSync, HI_HDMI_VIDEO_FMT_E *penVideoFmt)
{
    if (penVideoFmt == NULL) {
        return;
    }
    switch (enIntfSync) {
        case VO_OUTPUT_PAL:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_PAL;
            break;
        case VO_OUTPUT_NTSC:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_NTSC;
            break;
        case VO_OUTPUT_1080P24:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_24;
            break;
        case VO_OUTPUT_1080P25:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_25;
            break;
        case VO_OUTPUT_1080P30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_30;
            break;
        case VO_OUTPUT_720P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_720P_50;
            break;
        case VO_OUTPUT_720P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_720P_60;
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
        case VO_OUTPUT_576P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_576P_50;
            break;
        case VO_OUTPUT_480P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_480P_60;
            break;
        case VO_OUTPUT_800x600_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_800X600_60;
            break;
        case VO_OUTPUT_1024x768_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1024X768_60;
            break;
        case VO_OUTPUT_1280x1024_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1280X1024_60;
            break;
        case VO_OUTPUT_1366x768_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1366X768_60;
            break;
        case VO_OUTPUT_1440x900_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1440X900_60;
            break;
        case VO_OUTPUT_1280x800_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1280X800_60;
            break;
        case VO_OUTPUT_1920x2160_30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1920x2160_30;
            break;
        case VO_OUTPUT_1600x1200_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1600X1200_60;
            break;
        case VO_OUTPUT_1920x1200_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1920X1200_60;
            break;
        case VO_OUTPUT_2560x1440_30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_2560x1440_30;
            break;
        case VO_OUTPUT_2560x1600_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_2560x1600_60;
            break;
        case VO_OUTPUT_3840x2160_30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_3840X2160P_30;
            break;
        case VO_OUTPUT_3840x2160_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_3840X2160P_60;
            break;
        default:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_60;
            break;
    }

    return;
}

HI_S32 SAMPLE_COMM_VO_HdmiStart(VO_INTF_SYNC_E enIntfSync)
{
    HI_HDMI_ATTR_S stAttr;
    HI_HDMI_VIDEO_FMT_E enVideoFmt;
    HI_HDMI_ID_E enHdmiId = HI_HDMI_ID_0;

    SAMPLE_COMM_VO_HdmiConvertSync(enIntfSync, &enVideoFmt);

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

HI_S32 HI_MW_DISP_Open(HI_VOID)
{
    HI_S32 ret;
    VO_PUB_ATTR_S pubAttr;
    const VO_DEV voDev = 0;

    ret = HI_MW_Sys_Init();
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    pubAttr.u32BgColor = 0x000000ff; /* 0x000000ff: bg color value */
    pubAttr.enIntfType = VO_INTF_HDMI;
    pubAttr.enIntfSync = VO_OUTPUT_1080P30;

    if (SAMPLE_COMM_VO_StartDev(voDev, &pubAttr) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    ret = SAMPLE_COMM_VO_HdmiStart(pubAttr.enIntfSync);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return ret;
}

HI_VOID SAMPLE_COMM_VO_Exit(void)
{
    HI_S32 i, j;
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

HI_VOID SAMPLE_COMM_SYS_Exit(void)
{
    SAMPLE_COMM_VO_Exit();
    HI_MPI_SYS_Exit();
    HI_MPI_VB_ExitModCommPool(VB_UID_VDEC);
    HI_MPI_VB_Exit();
    return;
}

static HI_S32 HI_MW_Sys_Deinit(HI_VOID)
{
    SAMPLE_COMM_SYS_Exit();
    return HI_SUCCESS;
}

HI_S32 HI_MW_DISP_Close(HI_VOID)
{
    HI_S32 ret = HI_MW_Sys_Deinit();
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return ret;
}

HI_S32 HI_MW_DISP_SetDisplayGraphicCSC()
{
    return HI_SUCCESS;
}
