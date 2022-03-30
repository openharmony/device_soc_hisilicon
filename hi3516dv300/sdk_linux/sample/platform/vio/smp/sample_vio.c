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
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"
#include "sample_vio.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 SAMPLE_VIO_ViOnlineVpssOfflineRoute(HI_U32 u32VoIntfType)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_S32 s32ViCnt = 1;
    VI_DEV ViDev = 0;
    VI_PIPE ViPipe = 0;
    VI_CHN ViChn = 0;
    HI_S32 s32WorkSnsId = 0;
    SAMPLE_VI_CONFIG_S stViConfig;

    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;

    VO_CHN VoChn = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;

    WDR_MODE_E enWDRMode = WDR_MODE_NONE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E enMastPipeMode = VI_ONLINE_VPSS_OFFLINE;

    VPSS_GRP VpssGrp = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN VpssChn = VPSS_CHN0;
    HI_BOOL            abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VPSS_CHN_ATTR_S astVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    VENC_CHN           VencChn[1] = {0};
    PAYLOAD_TYPE_E enType = PT_H265;
    SAMPLE_RC_E enRcMode = SAMPLE_RC_CBR;
    HI_U32 u32Profile = 0;
    HI_BOOL bRcnRefShareBuf = HI_FALSE;
    VENC_GOP_ATTR_S stGopAttr;

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));

    /* config vi */
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = -1; /* index: 1 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode = enCompressMode;

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool cnt: 2 */

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_8,
        COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 10; /* vb block cnt: 10 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 4; /* vb block cnt: 4 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    /* config vpss */
    (hi_void)memset_s(&stVpssGrpAttr, sizeof(VPSS_GRP_ATTR_S), 0, sizeof(VPSS_GRP_ATTR_S));
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat = enPixFormat;
    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;

    astVpssChnAttr[VpssChn].u32Width = stSize.u32Width;
    astVpssChnAttr[VpssChn].u32Height = stSize.u32Height;
    astVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_USER;
    astVpssChnAttr[VpssChn].enCompressMode = enCompressMode;
    astVpssChnAttr[VpssChn].enDynamicRange = enDynamicRange;
    astVpssChnAttr[VpssChn].enVideoFormat = enVideoFormat;
    astVpssChnAttr[VpssChn].enPixelFormat = enPixFormat;
    astVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].u32Depth = 0;
    astVpssChnAttr[VpssChn].bMirror = HI_FALSE;
    astVpssChnAttr[VpssChn].bFlip = HI_FALSE;
    astVpssChnAttr[VpssChn].stAspectRatio.enMode = ASPECT_RATIO_NONE;

    /* start vpss */
    abChnEnable[0] = HI_TRUE;
    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /* vi bind vpss */
    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe, ViChn, VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vi bind vpss failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT2;
    }

    /* config venc */
    stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
    stGopAttr.stNormalP.s32IPQpDelta = 2; /* IPQpDelta: 2 */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[0], enType, enPicSize, enRcMode, u32Profile, bRcnRefShareBuf, &stGopAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start venc failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT3;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(VpssGrp, VpssChn, VencChn[0]);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind venc chn %d failed. s32Ret: 0x%x !n", VpssGrp, VencChn[0], s32Ret);
        goto EXIT4;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT5;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;
    if (u32VoIntfType == 1) {
        stVoConfig.enVoIntfType = VO_INTF_BT1120;
        stVoConfig.enIntfSync = VO_OUTPUT_1080P25;
    } else {
        stVoConfig.enVoIntfType = VO_INTF_HDMI;
    }
    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT5;
    }

    /* vpss bind vo */
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind vo%d failed. s32Ret: 0x%x !\n", VpssGrp, stVoConfig.VoDev, s32Ret);
        goto EXIT6;
    }

    s32Ret = SAMPLE_COMM_VENC_StartGetStream(VencChn, sizeof(VencChn) / sizeof(VENC_CHN));
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get venc stream failed!\n");
        goto EXIT7;
    }

    SAMPLE_VIO_GetChar();

    SAMPLE_COMM_VENC_StopGetStream();

EXIT7:
    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
EXIT6:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT5:
    SAMPLE_COMM_VPSS_UnBind_VENC(VpssGrp, VpssChn, VencChn[0]);
EXIT4:
    SAMPLE_COMM_VENC_Stop(VencChn[0]);
EXIT3:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe, ViChn, VpssGrp);
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

HI_S32 SAMPLE_VIO_WDR_LDC_DIS_SPREAD(HI_U32 u32VoIntfType)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_S32 s32ViCnt = 1;
    VI_DEV ViDev = 0;
    VI_PIPE            ViPipe[2] = {0, 1};
    VI_CHN ViChn = 0;
    HI_S32 s32WorkSnsId = 0;
    SAMPLE_VI_CONFIG_S stViConfig;

    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;

    VO_CHN VoChn = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;

    WDR_MODE_E enWDRMode = WDR_MODE_2To1_LINE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE;

    VPSS_GRP VpssGrp = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN VpssChn = VPSS_CHN0;
    HI_BOOL            abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VPSS_CHN_ATTR_S astVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    VENC_CHN           VencChn[1] = {0};
    PAYLOAD_TYPE_E enType = PT_H265;
    SAMPLE_RC_E enRcMode = SAMPLE_RC_CBR;
    HI_U32 u32Profile = 0;
    HI_BOOL bRcnRefShareBuf = HI_FALSE;
    VENC_GOP_ATTR_S stGopAttr;

    VI_LDC_ATTR_S stLDCAttr = { 0 };
    DIS_CONFIG_S stDISConfig = { 0 };
    DIS_ATTR_S stDISAttr = { 0 };
    SPREAD_ATTR_S stSpreadAttr = { 0 };

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe[0];
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = ViPipe[1]; /* index: 1 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode = enCompressMode;

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool count: 2 */

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_8,
        COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 10; /* vb block count: 10 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 6; /* vb block count: 6 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    s32Ret = HI_MPI_VI_SetPipeRepeatMode(ViPipe[0], VI_PIPE_REPEAT_ONCE);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetPipeRepeatMode failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /* config vpss */
    (hi_void)memset_s(&stVpssGrpAttr, sizeof(VPSS_GRP_ATTR_S), 0, sizeof(VPSS_GRP_ATTR_S));
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat = enPixFormat;
    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;

    astVpssChnAttr[VpssChn].u32Width = stSize.u32Width;
    astVpssChnAttr[VpssChn].u32Height = stSize.u32Height;
    astVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_USER;
    astVpssChnAttr[VpssChn].enCompressMode = enCompressMode;
    astVpssChnAttr[VpssChn].enDynamicRange = enDynamicRange;
    astVpssChnAttr[VpssChn].enVideoFormat = enVideoFormat;
    astVpssChnAttr[VpssChn].enPixelFormat = enPixFormat;
    astVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].u32Depth = 0;
    astVpssChnAttr[VpssChn].bMirror = HI_FALSE;
    astVpssChnAttr[VpssChn].bFlip = HI_FALSE;
    astVpssChnAttr[VpssChn].stAspectRatio.enMode = ASPECT_RATIO_NONE;

    /* start vpss */
    abChnEnable[0] = HI_TRUE;
    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /* vi bind vpss */
    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe[0], ViChn, VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vi bind vpss failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT2;
    }

    /* config venc */
    stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
    stGopAttr.stNormalP.s32IPQpDelta = 2; /* IPQpDelta: 2 */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[0], enType, enPicSize, enRcMode, u32Profile, bRcnRefShareBuf, &stGopAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start venc failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT3;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(VpssGrp, VpssChn, VencChn[0]);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind venc chn %d failed. s32Ret: 0x%x !n", VpssGrp, VencChn[0], s32Ret);
        goto EXIT4;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT5;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;
    if (u32VoIntfType == 1) {
        stVoConfig.enVoIntfType = VO_INTF_BT1120;
        stVoConfig.enIntfSync = VO_OUTPUT_1080P25;
    } else {
        stVoConfig.enVoIntfType = VO_INTF_HDMI;
    }
    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT5;
    }

    /* vpss bind vo */
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind vo%d failed. s32Ret: 0x%x !\n", VpssGrp, stVoConfig.VoDev, s32Ret);
        goto EXIT6;
    }

    s32Ret = SAMPLE_COMM_VENC_StartGetStream(VencChn, sizeof(VencChn) / sizeof(VENC_CHN));
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get venc stream failed!\n");
        goto EXIT7;
    }

    SAMPLE_PRT("Press Enter key to Enable LDC!\n");
    (HI_VOID)getchar();

    stLDCAttr.bEnable = HI_TRUE;
    stLDCAttr.stAttr.bAspect = 0;
    stLDCAttr.stAttr.s32XRatio = 100; /* x ratio: 100 */
    stLDCAttr.stAttr.s32YRatio = 100; /* y ratio: 100 */
    stLDCAttr.stAttr.s32XYRatio = 100; /* z ratio: 100 */
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32DistortionRatio = 500; /* distortion ratio: 500 */

    s32Ret = HI_MPI_VI_SetChnLDCAttr(ViPipe[0], ViChn, &stLDCAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT8;
    }

    SAMPLE_PRT("Press Enter key to Enable DIS!\n");
    (HI_VOID)getchar();

    stDISConfig.enMode = DIS_MODE_6_DOF_GME;
    stDISConfig.enMotionLevel = DIS_MOTION_LEVEL_NORMAL;
    stDISConfig.u32CropRatio = 80; /* crop ratio: 80 */
    stDISConfig.u32BufNum = 5; /* buffer nums: 5 */
    stDISConfig.enPdtType = DIS_PDT_TYPE_IPC;
    stDISConfig.u32GyroOutputRange = 0;
    stDISConfig.u32FrameRate = FPS_30;
    stDISConfig.bScale = HI_TRUE;
    stDISConfig.bCameraSteady = HI_FALSE;

    s32Ret = HI_MPI_VI_SetChnDISConfig(ViPipe[0], ViChn, &stDISConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnDISConfig failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT8;
    }

    stDISAttr.bEnable = HI_TRUE;
    stDISAttr.u32MovingSubjectLevel = 0;
    stDISAttr.s32RollingShutterCoef = 0;
    stDISAttr.u32ViewAngle = 1000; /* view angle: 1000 */
    stDISAttr.bStillCrop = HI_FALSE;
    stDISAttr.u32HorizontalLimit = 512; /* horizontal limit: 512 */
    stDISAttr.u32VerticalLimit = 512; /* ertical limit: 512 */

    s32Ret = HI_MPI_VI_SetChnDISAttr(ViPipe[0], ViChn, &stDISAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnDISAttr failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT8;
    }

    SAMPLE_PRT("Press Enter key to Enable Spread!\n");
    (HI_VOID)getchar();

    stSpreadAttr.bEnable = HI_TRUE;
    stSpreadAttr.u32SpreadCoef = 16; /* spread coef: 16 */
    stSpreadAttr.stDestSize.u32Width = stSize.u32Width;
    stSpreadAttr.stDestSize.u32Height = stSize.u32Height;

    s32Ret = HI_MPI_VI_SetChnSpreadAttr(ViPipe[0], ViChn, &stSpreadAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnSpreadAttr failed witfh %d\n", s32Ret);
        goto EXIT8;
    }

    SAMPLE_VIO_GetChar();

EXIT8:
    SAMPLE_COMM_VENC_StopGetStream();
EXIT7:
    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
EXIT6:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT5:
    SAMPLE_COMM_VPSS_UnBind_VENC(VpssGrp, VpssChn, VencChn[0]);
EXIT4:
    SAMPLE_COMM_VENC_Stop(VencChn[0]);
EXIT3:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe[0], ViChn, VpssGrp);
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

HI_S32 SAMPLE_VIO_ViDoublePipeRoute(HI_U32 u32VoIntfType)
{
    HI_S32 s32Ret;

    HI_S32 s32ViCnt = 1;
    VI_DEV ViDev = 0;
    VI_PIPE ViPipe[2] = {0, 2}; /* pipe0 pipe2 */
    VI_CHN ViChn = 0;
    HI_S32 s32WorkSnsId = 0;
    SAMPLE_VI_CONFIG_S stViConfig;

    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;

    VO_CHN             VoChn[2] = {0, 1};
    SAMPLE_VO_CONFIG_S stVoConfig;

    WDR_MODE_E enWDRMode = WDR_MODE_NONE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E enMastPipeMode = VI_OFFLINE_VPSS_ONLINE;

    VPSS_GRP           VpssGrp[2] = {0, 2}; /* vpss grp0 grp2 */
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN VpssChn = VPSS_CHN0;
    HI_BOOL            abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VPSS_CHN_ATTR_S astVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    VENC_CHN           VencChn[1] = {0};
    PAYLOAD_TYPE_E enType = PT_H265;
    SAMPLE_RC_E enRcMode = SAMPLE_RC_CBR;
    HI_U32 u32Profile = 0;
    HI_BOOL bRcnRefShareBuf = HI_FALSE;
    VENC_GOP_ATTR_S stGopAttr;

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));
    /* config vi */
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe[0];
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = ViPipe[1]; /* index: 1 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode = enCompressMode;

    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.bMultiPipe = HI_TRUE;

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool count: 2 */

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_8,
        COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 20; /* vb block count: 20 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 4; /* vb block count: 4 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    /* config vpss */
    (hi_void)memset_s(&stVpssGrpAttr, sizeof(VPSS_GRP_ATTR_S), 0, sizeof(VPSS_GRP_ATTR_S));
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat = enPixFormat;
    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;

    astVpssChnAttr[VpssChn].u32Width = stSize.u32Width;
    astVpssChnAttr[VpssChn].u32Height = stSize.u32Height;
    astVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_USER;
    astVpssChnAttr[VpssChn].enCompressMode = enCompressMode;
    astVpssChnAttr[VpssChn].enDynamicRange = enDynamicRange;
    astVpssChnAttr[VpssChn].enVideoFormat = enVideoFormat;
    astVpssChnAttr[VpssChn].enPixelFormat = enPixFormat;
    astVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = -1;
    astVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = -1;
    astVpssChnAttr[VpssChn].u32Depth = 0;
    astVpssChnAttr[VpssChn].bMirror = HI_FALSE;
    astVpssChnAttr[VpssChn].bFlip = HI_FALSE;
    astVpssChnAttr[VpssChn].stAspectRatio.enMode = ASPECT_RATIO_NONE;

    /* start vpss */
    abChnEnable[0] = HI_TRUE;
    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp[0], abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp[1], abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT2;
    }

    /* config venc */
    stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
    stGopAttr.stNormalP.s32IPQpDelta = 2; /* IPQpDelta: 2 */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[0], enType, enPicSize, enRcMode, u32Profile, bRcnRefShareBuf, &stGopAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start venc failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT3;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(VpssGrp[1], VpssChn, VencChn[0]);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind venc chn %d failed. s32Ret: 0x%x !n", VpssGrp[1], VencChn[0], s32Ret);
        goto EXIT4;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT5;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;
    if (u32VoIntfType == 1) {
        stVoConfig.enVoIntfType = VO_INTF_BT1120;
        stVoConfig.enIntfSync = VO_OUTPUT_1080P25;
    } else {
        stVoConfig.enVoIntfType = VO_INTF_HDMI;
    }
    stVoConfig.enVoMode = VO_MODE_2MUX;
    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT5;
    }

    /* vpss bind vo */
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp[0], VpssChn, stVoConfig.VoDev, VoChn[0]);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind vo%d failed. s32Ret: 0x%x !\n", VpssGrp[0], stVoConfig.VoDev, s32Ret);
        goto EXIT6;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp[1], VpssChn, stVoConfig.VoDev, VoChn[1]);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind vo%d failed. s32Ret: 0x%x !\n", VpssGrp[1], stVoConfig.VoDev, s32Ret);
        goto EXIT7;
    }

    s32Ret = SAMPLE_COMM_VENC_StartGetStream(VencChn, sizeof(VencChn) / sizeof(VENC_CHN));
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get venc stream failed!\n");
        goto EXIT8;
    }

    SAMPLE_VIO_GetChar();

    SAMPLE_COMM_VENC_StopGetStream();

EXIT8:
    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp[1], VpssChn, stVoConfig.VoDev, VoChn[1]);
EXIT7:
    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp[0], VpssChn, stVoConfig.VoDev, VoChn[0]);
EXIT6:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT5:
    SAMPLE_COMM_VPSS_UnBind_VENC(VpssGrp[1], VpssChn, VencChn[0]);
EXIT4:
    SAMPLE_COMM_VENC_Stop(VencChn[0]);
EXIT3:
    SAMPLE_COMM_VPSS_Stop(VpssGrp[1], abChnEnable);
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp[0], abChnEnable);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}


HI_S32 SAMPLE_VIO_ViWdrSwitch(HI_U32 u32VoIntfType)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_S32 s32ViCnt = 1;
    VI_DEV ViDev = 0;
    VI_PIPE ViPipe[2] = {0, 1};
    VI_CHN ViChn = 0;
    HI_S32 s32WorkSnsId = 0;
    SAMPLE_VI_CONFIG_S stViConfig;

    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;

    VO_CHN VoChn = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;

    WDR_MODE_E enWDRMode = WDR_MODE_NONE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E enMastPipeMode = VI_ONLINE_VPSS_OFFLINE;

    VPSS_GRP VpssGrp = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN VpssChn = VPSS_CHN0;
    HI_BOOL            abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VPSS_CHN_ATTR_S astVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    VENC_CHN           VencChn[1] = {0};
    PAYLOAD_TYPE_E enType = PT_H265;
    SAMPLE_RC_E enRcMode = SAMPLE_RC_CBR;
    HI_U32 u32Profile = 0;
    HI_BOOL bRcnRefShareBuf = HI_FALSE;
    VENC_GOP_ATTR_S stGopAttr;

    HI_BOOL bBypassAWB;
    ISP_MODULE_CTRL_U unModCtrl;
    HI_S32 s32MilliSec = 80; /* 80ms timeout */
    ISP_VD_TYPE_E enIspVDType = ISP_VD_FE_START;
    HI_U8 i = 0;

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));
    /* config vi */
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe[0];
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = -1; /* index: 1 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode = enCompressMode;

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool count: 2 */

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_8,
        COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 10; /* vb block count: 10 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 4; /* vb block count: 4 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    /* config vpss */
    (hi_void)memset_s(&stVpssGrpAttr, sizeof(VPSS_GRP_ATTR_S), 0, sizeof(VPSS_GRP_ATTR_S));
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat = enPixFormat;
    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;
    stVpssGrpAttr.bNrEn = HI_FALSE;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;

    astVpssChnAttr[VpssChn].u32Width = stSize.u32Width;
    astVpssChnAttr[VpssChn].u32Height = stSize.u32Height;
    astVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_USER;
    astVpssChnAttr[VpssChn].enCompressMode = enCompressMode;
    astVpssChnAttr[VpssChn].enDynamicRange = enDynamicRange;
    astVpssChnAttr[VpssChn].enVideoFormat = enVideoFormat;
    astVpssChnAttr[VpssChn].enPixelFormat = enPixFormat;
    astVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].u32Depth = 0;
    astVpssChnAttr[VpssChn].bMirror = HI_FALSE;
    astVpssChnAttr[VpssChn].bFlip = HI_FALSE;
    astVpssChnAttr[VpssChn].stAspectRatio.enMode = ASPECT_RATIO_NONE;

    /* start vpss */
    abChnEnable[0] = HI_TRUE;
    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /* vi bind vpss */
    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe[0], ViChn, VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vi bind vpss failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT2;
    }

    /* config venc */
    stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
    stGopAttr.stNormalP.s32IPQpDelta = 2; /* IPQpDelta: 2 */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[0], enType, enPicSize, enRcMode, u32Profile, bRcnRefShareBuf, &stGopAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start venc failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT3;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(VpssGrp, VpssChn, VencChn[0]);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind venc chn %d failed. s32Ret: 0x%x !n", VpssGrp, VencChn[0], s32Ret);
        goto EXIT4;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT5;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;
    if (u32VoIntfType == 1) {
        stVoConfig.enVoIntfType = VO_INTF_BT1120;
        stVoConfig.enIntfSync = VO_OUTPUT_1080P25;
    } else {
        stVoConfig.enVoIntfType = VO_INTF_HDMI;
    }
    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT5;
    }

    /* vpss bind vo */
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind vo%d failed. s32Ret: 0x%x !\n", VpssGrp, stVoConfig.VoDev, s32Ret);
        goto EXIT6;
    }

    s32Ret = SAMPLE_COMM_VENC_StartGetStream(VencChn, sizeof(VencChn) / sizeof(VENC_CHN));
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get venc stream failed!\n");
        goto EXIT7;
    }
    SAMPLE_PRT("switch to wdr mode========\n");
    (HI_VOID)getchar();

    bBypassAWB = ((enMastPipeMode == VI_ONLINE_VPSS_OFFLINE) || (enMastPipeMode == VI_ONLINE_VPSS_ONLINE));

    if (bBypassAWB == HI_TRUE) {
        s32Ret = HI_MPI_ISP_GetModuleControl(ViPipe[0], &unModCtrl);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_ISP_GetModuleControl failed!\n");
            goto EXIT8;
        }
        unModCtrl.bitBypassAWBStat = HI_TRUE;
        s32Ret = HI_MPI_ISP_SetModuleControl(ViPipe[0], &unModCtrl);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_ISP_SetModuleControl failed!\n");
            goto EXIT8;
        }
        for (i = 0; i < 2; i++) { /* loop 2 times */
            s32Ret = HI_MPI_ISP_GetVDTimeOut(ViPipe[0], enIspVDType, s32MilliSec);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("HI_MPI_ISP_GetVDTimeOut failed with %#x\n", s32Ret);
                goto EXIT8;
            }
        }
    }

    s32Ret = SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss grp %d stop failed. s32Ret: 0x%x!\n", VpssGrp, s32Ret);
        goto EXIT8;
    }

    s32Ret = SAMPLE_COMM_VI_SwitchMode_StopVI(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SwitchMode_StopVI failed with %#x\n", s32Ret);
        goto EXIT8;
    }

    if (stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType == SONY_IMX335_MIPI_5M_30FPS_12BIT) {
        stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType = SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1;
    } else if (stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType == SONY_IMX335_MIPI_4M_30FPS_12BIT) {
        stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType = SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1;
    } else {
        SAMPLE_PRT("sensor:%d, not support switch to wdr mode.\n",
            stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType);
    }

    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = WDR_MODE_2To1_LINE;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe[0];
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = ViPipe[1];
    s32Ret = SAMPLE_COMM_VI_SwitchMode(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SwitchMode failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT8;
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT8;
    }

    if (bBypassAWB == HI_TRUE) {
        s32Ret = HI_MPI_ISP_GetModuleControl(ViPipe[0], &unModCtrl);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_ISP_GetModuleControl failed. s32Ret: 0x%x!\n", s32Ret);
            goto EXIT8;
        }
        unModCtrl.bitBypassAWBStat = HI_FALSE;
        s32Ret = HI_MPI_ISP_SetModuleControl(ViPipe[0], &unModCtrl);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_ISP_SetModuleControl failed. s32Ret: 0x%x!\n", s32Ret);
            goto EXIT8;
        }
    }

    SAMPLE_PRT("switch to linear mode========\n");
    (HI_VOID)getchar();

    if (bBypassAWB == HI_TRUE) {
        s32Ret = HI_MPI_ISP_GetModuleControl(ViPipe[0], &unModCtrl);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_ISP_GetModuleControl failed. s32Ret: 0x%x!\n", s32Ret);
            goto EXIT8;
        }
        unModCtrl.bitBypassAWBStat = HI_TRUE;
        s32Ret = HI_MPI_ISP_SetModuleControl(ViPipe[0], &unModCtrl);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_ISP_SetModuleControl failed. s32Ret: 0x%x!\n", s32Ret);
            goto EXIT8;
        }
        for (i = 0; i < 2; i++) { /* loop 2 times */
            s32Ret = HI_MPI_ISP_GetVDTimeOut(ViPipe[0], enIspVDType, s32MilliSec);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("HI_MPI_ISP_GetVDTimeOut failed with %#x!\n", s32Ret);
                goto EXIT8;
            }
        }
    }

    s32Ret = SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss grp %d stop failed. s32Ret: 0x%x !\n", VpssGrp, s32Ret);
        goto EXIT8;
    }
    s32Ret = SAMPLE_COMM_VI_SwitchMode_StopVI(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SwitchMode_StopVI failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT8;
    }

    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType = SENSOR0_TYPE;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = WDR_MODE_NONE;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe[0];
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = -1;

    s32Ret = SAMPLE_COMM_VI_SwitchMode(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SwitchMode failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT8;
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT8;
    }

    if (bBypassAWB == HI_TRUE) {
        s32Ret = HI_MPI_ISP_GetModuleControl(ViPipe[0], &unModCtrl);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_ISP_GetModuleControl failed. s32Ret: 0x%x!\n", s32Ret);
            goto EXIT8;
        }
        unModCtrl.bitBypassAWBStat = HI_FALSE;
        s32Ret = HI_MPI_ISP_SetModuleControl(ViPipe[0], &unModCtrl);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_ISP_SetModuleControl failed. s32Ret: 0x%x!\n", s32Ret);
            goto EXIT8;
        }
    }

    SAMPLE_VIO_GetChar();

EXIT8:
    SAMPLE_COMM_VENC_StopGetStream();
EXIT7:
    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
EXIT6:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT5:
    SAMPLE_COMM_VPSS_UnBind_VENC(VpssGrp, VpssChn, VencChn[0]);
EXIT4:
    SAMPLE_COMM_VENC_Stop(VencChn[0]);
EXIT3:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe[0], ViChn, VpssGrp);
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

HI_S32 SAMPLE_VIO_ViVpssLowDelay(HI_U32 u32VoIntfType)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32ViCnt = 1;
    VI_DEV ViDev = 0;
    VI_PIPE ViPipe = 0;
    VI_CHN ViChn = 0;
    HI_S32 s32WorkSnsId = 0;
    SAMPLE_VI_CONFIG_S stViConfig;
    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;
    VO_CHN VoChn = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;
    WDR_MODE_E enWDRMode = WDR_MODE_NONE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE;
    VPSS_GRP VpssGrp = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN VpssChn = VPSS_CHN0;
    HI_BOOL            abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VPSS_CHN_ATTR_S astVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];
    VENC_CHN           VencChn[1] = {0};
    PAYLOAD_TYPE_E enType = PT_H265;
    SAMPLE_RC_E enRcMode = SAMPLE_RC_CBR;
    HI_U32 u32Profile = 0;
    HI_BOOL bRcnRefShareBuf = HI_FALSE;
    VENC_GOP_ATTR_S stGopAttr;
    VI_LDC_ATTR_S stLDCAttr = { 0 };
    VI_LOW_DELAY_INFO_S stLowDelayInfo;

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));
    /* config vi */
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = -1; /* index: 1 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode = enCompressMode;

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool count: 2 */

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_8,
        COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 8; /* vb block count: 8 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 4; /* vb block count: 4 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    /* config vpss */
    (hi_void)memset_s(&stVpssGrpAttr, sizeof(VPSS_GRP_ATTR_S), 0, sizeof(VPSS_GRP_ATTR_S));
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat = enPixFormat;
    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;

    astVpssChnAttr[VpssChn].u32Width = stSize.u32Width;
    astVpssChnAttr[VpssChn].u32Height = stSize.u32Height;
    astVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_USER;
    astVpssChnAttr[VpssChn].enCompressMode = enCompressMode;
    astVpssChnAttr[VpssChn].enDynamicRange = enDynamicRange;
    astVpssChnAttr[VpssChn].enVideoFormat = enVideoFormat;
    astVpssChnAttr[VpssChn].enPixelFormat = enPixFormat;
    astVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].u32Depth = 0;
    astVpssChnAttr[VpssChn].bMirror = HI_FALSE;
    astVpssChnAttr[VpssChn].bFlip = HI_FALSE;
    astVpssChnAttr[VpssChn].stAspectRatio.enMode = ASPECT_RATIO_NONE;

    /* start vpss */
    abChnEnable[0] = HI_TRUE;
    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /* vi bind vpss */
    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe, ViChn, VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vi bind vpss failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT2;
    }

    /* config venc */
    stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
    stGopAttr.stNormalP.s32IPQpDelta = 2; /* IPQpDelta: 2 */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[0], enType, enPicSize, enRcMode, u32Profile, bRcnRefShareBuf, &stGopAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start venc failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT3;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(VpssGrp, VpssChn, VencChn[0]);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind venc chn %d failed. s32Ret: 0x%x !n", VpssGrp, VencChn[0], s32Ret);
        goto EXIT4;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT5;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;
    if (u32VoIntfType == 1) {
        stVoConfig.enVoIntfType = VO_INTF_BT1120;
        stVoConfig.enIntfSync = VO_OUTPUT_1080P25;
    } else {
        stVoConfig.enVoIntfType = VO_INTF_HDMI;
    }
    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT5;
    }

    /* vpss bind vo */
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind vo%d failed. s32Ret: 0x%x !\n", VpssGrp, stVoConfig.VoDev, s32Ret);
        goto EXIT6;
    }

    s32Ret = SAMPLE_COMM_VENC_StartGetStream(VencChn, sizeof(VencChn) / sizeof(VENC_CHN));
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get venc stream failed!\n");
        goto EXIT7;
    }

    stLDCAttr.bEnable = HI_TRUE;
    stLDCAttr.stAttr.bAspect = 0;
    stLDCAttr.stAttr.s32XRatio = 100; /* x ratio: 100 */
    stLDCAttr.stAttr.s32YRatio = 100; /* y ratio: 100 */
    stLDCAttr.stAttr.s32XYRatio = 100; /* z ratio: 100 */
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32DistortionRatio = 500; /* distortion ratio: 500 */
    s32Ret = HI_MPI_VI_SetChnLDCAttr(ViPipe, ViChn, &stLDCAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT8;
    }

    SAMPLE_PRT("Press Enter key to Enable LowDelay!\n");
    (HI_VOID)getchar();

    stLowDelayInfo.bEnable = HI_TRUE;
    stLowDelayInfo.u32LineCnt = stSize.u32Height / 2; /* line count: 1/2 of height */
    HI_MPI_VI_SetChnLowDelayAttr(ViPipe, ViChn, &stLowDelayInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnLowDelayAttr failed witfh %d\n", s32Ret);
        goto EXIT8;
    }

    SAMPLE_VIO_GetChar();

EXIT8:
    SAMPLE_COMM_VENC_StopGetStream();
EXIT7:
    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
EXIT6:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT5:
    SAMPLE_COMM_VPSS_UnBind_VENC(VpssGrp, VpssChn, VencChn[0]);
EXIT4:
    SAMPLE_COMM_VENC_Stop(VencChn[0]);
EXIT3:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe, ViChn, VpssGrp);
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

HI_S32 SAMPLE_VIO_Rotate(HI_U32 u32VoIntfType)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32ViCnt = 1;
    VI_DEV ViDev = 0;
    VI_PIPE ViPipe = 0;
    VI_CHN ViChn = 0;
    HI_S32 s32WorkSnsId = 0;
    SAMPLE_VI_CONFIG_S stViConfig;
    VI_ROTATION_EX_ATTR_S stViRotationExAttr;
    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;
    VO_CHN VoChn = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;

    WDR_MODE_E enWDRMode = WDR_MODE_NONE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE;

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));
    /* config vi */
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = -1; /* index: 1 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode = enCompressMode;

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool count: 2 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 10; /* vb block count: 10 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 4; /* vb block count: 4 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT1;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;
    if (u32VoIntfType == 1) {
        stVoConfig.enVoIntfType = VO_INTF_BT1120;
        stVoConfig.enIntfSync = VO_OUTPUT_1080P25;
    } else {
        stVoConfig.enVoIntfType = VO_INTF_HDMI;
    }
    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /* vi bind vo */
    s32Ret = SAMPLE_COMM_VI_Bind_VO(ViPipe, ViChn, stVoConfig.VoDev, VoChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vi pipe %d bind vo%d failed. s32Ret: 0x%x !\n", ViPipe, stVoConfig.VoDev, s32Ret);
        goto EXIT2;
    }

    SAMPLE_PRT("Press Enter key to switch Rotation 90!\n");
    (HI_VOID)getchar();

    s32Ret = HI_MPI_VI_SetChnRotation(ViPipe, ViChn, ROTATION_90);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotation failed with %d\n", s32Ret);
        goto EXIT3;
    }

    SAMPLE_PRT("Press Enter key to switch Rotation 180!\n");
    (HI_VOID)getchar();

    s32Ret = HI_MPI_VI_SetChnRotation(ViPipe, ViChn, ROTATION_180);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotation failed with %d\n", s32Ret);
        goto EXIT3;
    }

    SAMPLE_PRT("Press Enter key to switch Rotation 270!\n");
    (HI_VOID)getchar();

    s32Ret = HI_MPI_VI_SetChnRotation(ViPipe, ViChn, ROTATION_270);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotation failed witfh %d\n", s32Ret);
        goto EXIT3;
    }

    SAMPLE_PRT("Press Enter key to switch Rotation 0!\n");
    (HI_VOID)getchar();

    s32Ret = HI_MPI_VI_SetChnRotation(ViPipe, ViChn, ROTATION_0);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotation failed with %d\n", s32Ret);
        goto EXIT3;
    }

    SAMPLE_PRT("Press Enter key to switch RotateEx 60!\n");
    (HI_VOID)getchar();
    stViRotationExAttr.bEnable = HI_TRUE;
    stViRotationExAttr.stRotationEx.enViewType = ROTATION_VIEW_TYPE_ALL;
    stViRotationExAttr.stRotationEx.u32Angle = 60; /* angle: 60 */
    stViRotationExAttr.stRotationEx.s32CenterXOffset = 0;
    stViRotationExAttr.stRotationEx.s32CenterYOffset = 0;
    stViRotationExAttr.stRotationEx.stDestSize.u32Width = stSize.u32Width;
    stViRotationExAttr.stRotationEx.stDestSize.u32Height = stSize.u32Height;
    s32Ret = HI_MPI_VI_SetChnRotationEx(ViPipe, ViChn, &stViRotationExAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotationEx failed with %d\n", s32Ret);
        goto EXIT3;
    }

    SAMPLE_PRT("Enter any key to Disable RotateEx!!\n");
    (HI_VOID)getchar();
    stViRotationExAttr.bEnable = HI_FALSE;
    s32Ret = HI_MPI_VI_SetChnRotationEx(ViPipe, ViChn, &stViRotationExAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotationEx failed with %d\n", s32Ret);
        goto EXIT3;
    }

    SAMPLE_VIO_GetChar();

EXIT3:
    SAMPLE_COMM_VI_UnBind_VO(ViPipe, ViChn, stVoConfig.VoDev, VoChn);
EXIT2:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

HI_S32 SAMPLE_VIO_FPN(HI_U32 u32VoIntfType)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32ViCnt = 1;
    VI_DEV ViDev = 0;
    VI_PIPE ViPipe = 0;
    VI_CHN ViChn = 0;
    HI_S32 s32WorkSnsId = 0;
    SAMPLE_VI_CONFIG_S stViConfig;
    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;
    VO_CHN VoChn = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;
    WDR_MODE_E enWDRMode = WDR_MODE_NONE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E enMastPipeMode = VI_ONLINE_VPSS_OFFLINE;
    SAMPLE_VI_FPN_CALIBRATE_INFO_S stViFpnCalibrateInfo;
    SAMPLE_VI_FPN_CORRECTION_INFO_S stViFpnCorrectionInfo;

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));

    /* config vi */
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = -1; /* index: 1 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode = enCompressMode;

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool count: 2 */

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_8,
        COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 10; /* vb block count: 10 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 4; /* vb block count: 4 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT1;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;
    if (u32VoIntfType == 1) {
        stVoConfig.enVoIntfType = VO_INTF_BT1120;
        stVoConfig.enIntfSync = VO_OUTPUT_1080P25;
    } else {
        stVoConfig.enVoIntfType = VO_INTF_HDMI;
    }
    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /* vi bind vo */
    s32Ret = SAMPLE_COMM_VI_Bind_VO(ViPipe, ViChn, stVoConfig.VoDev, VoChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vi pipe %d bind vo%d failed. s32Ret: 0x%x !\n", ViPipe, stVoConfig.VoDev, s32Ret);
        goto EXIT2;
    }

    stViFpnCalibrateInfo.u32Threshold = 4095; /* threshold : 4095 */
    stViFpnCalibrateInfo.u32FrameNum = 16; /* frame num: 16 */
    stViFpnCalibrateInfo.enFpnType = ISP_FPN_TYPE_FRAME;
    stViFpnCalibrateInfo.enPixelFormat = PIXEL_FORMAT_RGB_BAYER_16BPP;
    stViFpnCalibrateInfo.enCompressMode = COMPRESS_MODE_NONE;

    s32Ret = SAMPLE_COMM_VI_FpnCalibrateConfig(ViPipe, &stViFpnCalibrateInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("ViPipe%d FPN Calibrate failed. s32Ret: 0x%x !\n", ViPipe, s32Ret);
        goto EXIT3;
    }

    SAMPLE_PRT("Press Enter key to FPN correction!\n");
    (HI_VOID)getchar();

    stViFpnCorrectionInfo.enOpType = OP_TYPE_AUTO;
    stViFpnCorrectionInfo.enFpnType = stViFpnCalibrateInfo.enFpnType;
    stViFpnCorrectionInfo.u32Strength = 0;
    stViFpnCorrectionInfo.enPixelFormat = stViFpnCalibrateInfo.enPixelFormat;
    stViFpnCorrectionInfo.enCompressMode = stViFpnCalibrateInfo.enCompressMode;

    s32Ret = SAMPLE_COMM_VI_FpnCorrectionConfig(ViPipe, &stViFpnCorrectionInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("ViPipe%d FPN Correct failed. s32Ret: 0x%x !\n", ViPipe, s32Ret);
        goto EXIT3;
    }

    SAMPLE_VIO_GetChar();

    SAMPLE_COMM_VI_DisableFpnCorrection(ViPipe, &stViFpnCorrectionInfo);
EXIT3:
    SAMPLE_COMM_VI_UnBind_VO(ViPipe, ViChn, stVoConfig.VoDev, VoChn);
EXIT2:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

HI_S32 SAMPLE_VIO_ViDoubleWdrPipe(HI_U32 u32VoIntfType)
{
    HI_S32 s32Ret;

    HI_S32 s32ViCnt = 2;
    VI_DEV             ViDev[2] = {0, 1};
    VI_PIPE            ViPipe[4] = {0, 1, 2, 3};
    VI_CHN ViChn = 0;
    SAMPLE_VI_CONFIG_S stViConfig;

    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;

    VO_CHN VoChn = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;

    WDR_MODE_E enWDRMode = WDR_MODE_2To1_LINE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;

    VPSS_GRP           VpssGrp[2] = {0, 1};
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN VpssChn = VPSS_CHN0;
    HI_BOOL            abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VPSS_CHN_ATTR_S astVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));

    /* config vi */
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.as32WorkingViId[1] = 1;
    stViConfig.astViInfo[0].stSnsInfo.MipiDev = ViDev[0];
    stViConfig.astViInfo[0].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[0].stDevInfo.ViDev = ViDev[0];
    stViConfig.astViInfo[0].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[0].stPipeInfo.enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[0] = ViPipe[0];
    stViConfig.astViInfo[0].stPipeInfo.aPipe[1] = ViPipe[1]; /* index: 1 */
    stViConfig.astViInfo[0].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[0].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[0].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[0].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[0].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[0].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[0].stChnInfo.enCompressMode = enCompressMode;

    stViConfig.astViInfo[1].stSnsInfo.MipiDev = ViDev[1];
    stViConfig.astViInfo[1].stSnsInfo.s32BusId = 1;
    stViConfig.astViInfo[1].stDevInfo.ViDev = ViDev[1];
    stViConfig.astViInfo[1].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[1].stPipeInfo.enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE;
    stViConfig.astViInfo[1].stPipeInfo.aPipe[0] = ViPipe[2]; /* vi pipe index: 2 */
    stViConfig.astViInfo[1].stPipeInfo.aPipe[1] = ViPipe[3]; /* vi pipe index: 3 */
    stViConfig.astViInfo[1].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[1].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[1].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[1].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[1].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[1].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[1].stChnInfo.enCompressMode = enCompressMode;

    if ((stViConfig.astViInfo[0].stSnsInfo.enSnsType != SONY_IMX327_2L_MIPI_2M_30FPS_12BIT_WDR2TO1) &&
        (stViConfig.astViInfo[0].stSnsInfo.enSnsType != SONY_IMX307_2L_MIPI_2M_30FPS_12BIT_WDR2TO1) &&
        (stViConfig.astViInfo[0].stSnsInfo.enSnsType != OMNIVISION_OV2775_2L_MIPI_2M_30FPS_12BIT_WDR2TO1)) {
        SAMPLE_PRT("Not Support!\n");
        return HI_SUCCESS;
    }

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool count: 2 */

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_8,
        COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 20; /* vb block count: 20 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 4; /* vb block count: 4 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    /* config vpss */
    (hi_void)memset_s(&stVpssGrpAttr, sizeof(VPSS_GRP_ATTR_S), 0, sizeof(VPSS_GRP_ATTR_S));
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat = enPixFormat;
    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;

    astVpssChnAttr[VpssChn].u32Width = stSize.u32Width;
    astVpssChnAttr[VpssChn].u32Height = stSize.u32Height;
    astVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_USER;
    astVpssChnAttr[VpssChn].enCompressMode = enCompressMode;
    astVpssChnAttr[VpssChn].enDynamicRange = enDynamicRange;
    astVpssChnAttr[VpssChn].enVideoFormat = enVideoFormat;
    astVpssChnAttr[VpssChn].enPixelFormat = enPixFormat;
    astVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].u32Depth = 0;
    astVpssChnAttr[VpssChn].bMirror = HI_FALSE;
    astVpssChnAttr[VpssChn].bFlip = HI_FALSE;
    astVpssChnAttr[VpssChn].stAspectRatio.enMode = ASPECT_RATIO_NONE;

    /* start vpss */
    abChnEnable[0] = HI_TRUE;
    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp[0], abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp[1], abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT2;
    }

    /* vi bind vpss */
    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe[0], ViChn, VpssGrp[0]);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vi bind vpss failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT3;
    }

    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe[2], ViChn, VpssGrp[1]); /* vi pipe index: 2 */
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vi bind vpss failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT4;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT5;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;
    if (u32VoIntfType == 1) {
        stVoConfig.enVoIntfType = VO_INTF_BT1120;
    } else {
        stVoConfig.enVoIntfType = VO_INTF_HDMI;
    }
    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT5;
    }

    /* vpss bind vo */
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp[0], VpssChn, stVoConfig.VoDev, VoChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind vo%d failed. s32Ret: 0x%x !\n", VpssGrp[0], stVoConfig.VoDev, s32Ret);
        goto EXIT6;
    }

    SAMPLE_VIO_GetChar();

    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp[0], VpssChn, stVoConfig.VoDev, VoChn);

EXIT6:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT5:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe[2], ViChn, VpssGrp[1]); /* vi pipe index: 2 */
EXIT4:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe[0], ViChn, VpssGrp[0]);
EXIT3:
    SAMPLE_COMM_VPSS_Stop(VpssGrp[1], abChnEnable);
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp[0], abChnEnable);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

HI_S32 SAMPLE_VIO_VPSS_VO_MIPI_TX(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32ViCnt = 1;
    VI_DEV ViDev = 0;
    VI_PIPE ViPipe = 0;
    VI_CHN ViChn = 0;
    HI_S32 s32WorkSnsId = 0;
    SAMPLE_VI_CONFIG_S stViConfig;
    SIZE_S stSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enPicSize;
    HI_U32 u32BlkSize;
    VO_CHN VoChn = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;
    WDR_MODE_E enWDRMode = WDR_MODE_NONE;
    DYNAMIC_RANGE_E enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E enVideoFormat = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E enMastPipeMode = VI_ONLINE_VPSS_OFFLINE;
    VPSS_GRP VpssGrp = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN VpssChn = VPSS_CHN0;
    HI_BOOL            abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VPSS_CHN_ATTR_S astVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    (hi_void)memset_s(&stViConfig, sizeof(stViConfig), 0, sizeof(stViConfig));
    /* config vi */
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);

    stViConfig.s32WorkingViNum = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId = 0;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode = enWDRMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0] = ViPipe;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1] = -1; /* index: 1 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2] = -1; /* index: 2 */
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3] = -1; /* index: 3 */
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = enDynamicRange;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode = enCompressMode;

    /* get picture size */
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /* config vb */
    (hi_void)memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt = 2; /* max pool count: 2 */

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_8,
        COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 10; /* vb block count: 10 */

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP,
        COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 4; /* vb block count: 4 */

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /* start vi */
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        goto EXIT;
    }

    /* config vpss */
    (hi_void)memset_s(&stVpssGrpAttr, sizeof(VPSS_GRP_ATTR_S), 0, sizeof(VPSS_GRP_ATTR_S));
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat = enPixFormat;
    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;

    astVpssChnAttr[VpssChn].u32Width = stSize.u32Width;
    astVpssChnAttr[VpssChn].u32Height = stSize.u32Height;
    astVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_USER;
    astVpssChnAttr[VpssChn].enCompressMode = enCompressMode;
    astVpssChnAttr[VpssChn].enDynamicRange = enDynamicRange;
    astVpssChnAttr[VpssChn].enVideoFormat = enVideoFormat;
    astVpssChnAttr[VpssChn].enPixelFormat = enPixFormat;
    astVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = FPS_30;
    astVpssChnAttr[VpssChn].u32Depth = 0;
    astVpssChnAttr[VpssChn].bMirror = HI_FALSE;
    astVpssChnAttr[VpssChn].bFlip = HI_FALSE;
    astVpssChnAttr[VpssChn].stAspectRatio.enMode = ASPECT_RATIO_NONE;

    /* start vpss */
    abChnEnable[0] = HI_TRUE;
    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, abChnEnable, &stVpssGrpAttr, astVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /* vpss bind vo */
    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe, ViChn, VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss bind vi failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT2;
    }

    /* config vo */
    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VO default config failed. s32Ret: 0x%x!n", s32Ret);
        goto EXIT3;
    }
    stVoConfig.enDstDynamicRange = enDynamicRange;

    stVoConfig.enVoIntfType = VO_INTF_MIPI;
    stVoConfig.enIntfSync = VO_OUTPUT_1080x1920_60;

    stVoConfig.enPicSize = enPicSize;

    /* start vo */
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT3;
    }

    /* vpss bind vo */
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("vpss group %d bind vo%d failed. s32Ret: 0x%x !\n", VpssGrp, stVoConfig.VoDev, s32Ret);
        goto EXIT4;
    }

    SAMPLE_VIO_GetChar();

    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);

EXIT4:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT3:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe, ViChn, VpssGrp);
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
