/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sample_comm.h"
#include "sample_comm_ive.h"
#include "sample_store.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define PIC_WIDTH 720
#define PIC_HEIGHT 576

SampleStoreInfo g_sampleStoreInfo = {0};
static VPSS_CHN VpssChn[2] = {0, 1};
static PIC_SIZE_E enSize[2] = {PIC_D1_MIPI, PIC_D1_MIPI};
static VENC_CHN VencChn[2] = {0, 1};
static PAYLOAD_TYPE_E enPayLoad[2] = {PT_H265, PT_H264};
static HI_U32 u32Profile[2] = {0, 0};
static HI_BOOL bRcnRefShareBuf = HI_TRUE;
static HI_BOOL abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {1, 1, 0};
static HI_U32 u32SupplementConfig = HI_FALSE;

HI_VOID SAMPLE_VOU_SYS_Exit(void)
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();
}

VENC_GOP_MODE_E SAMPLE_VENC_GetGopMode(void)
{
    char c;
    HI_S32 s32Ret = HI_SUCCESS;
    VENC_GOP_MODE_E enGopMode = 0;

Begin_Get:

    printf("please input choose gop mode!\n");
    printf("\t 0) NORMALP.\n");
    printf("\t 1) DUALP.\n");
    printf("\t 2) SMARTP.\n");

    while ((c = getchar()) != '\n') {
        switch (c) {
            case '0':
                enGopMode = VENC_GOPMODE_NORMALP;
                break;
            case '1':
                enGopMode = VENC_GOPMODE_DUALP;
                break;
            case '2':
                enGopMode = VENC_GOPMODE_SMARTP;
                break;
            default:
                s32Ret = HI_FAILURE;
                SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, Begin_Get,
                    "input gopmode: %c, is invaild!\n", c);
        }
    }

    return enGopMode;
}

SAMPLE_RC_E SAMPLE_VENC_GetRcMode(void)
{
    char c;
    HI_S32 s32Ret = HI_SUCCESS;
    SAMPLE_RC_E  enRcMode = 0;

Begin_Get:

    printf("please input choose rc mode!\n");
    printf("\t c) cbr.\n");
    printf("\t v) vbr.\n");
    printf("\t a) avbr.\n");
    printf("\t x) cvbr.\n");
    printf("\t q) qvbr.\n");
    printf("\t f) fixQp\n");

    while ((c = getchar()) != '\n') {
        switch (c) {
            case 'c':
                enRcMode = SAMPLE_RC_CBR;
                break;
            case 'v':
                enRcMode = SAMPLE_RC_VBR;
                break;
            case 'a':
                enRcMode = SAMPLE_RC_AVBR;
                break;
            case 'q':
                enRcMode = SAMPLE_RC_QVBR;
                break;
            case 'x':
                enRcMode = SAMPLE_RC_CVBR;
                break;
            case 'f':
                enRcMode = SAMPLE_RC_FIXQP;
                break;
            default:
                s32Ret = HI_FAILURE;
                SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, Begin_Get,
                    "input rcmode: %c, is invaild!\n", c);
        }
    }

    return enRcMode;
}

HI_S32 SAMPLE_VENC_SYS_Init(HI_U32 u32SupplementConfig, SAMPLE_SNS_TYPE_E enSnsType)
{
    HI_S32 s32Ret;
    HI_U64 u64BlkSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enSnsSize;
    SIZE_S stSnsSize;

    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    u64BlkSize = COMMON_GetPicBufferSize(stSnsSize.u32Width, stSnsSize.u32Height,
        PIXEL_FORMAT_YVU_SEMIPLANAR_422, DATA_BITWIDTH_8, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u64BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 10; // 10: Number of cache blocks per cache pool

    u64BlkSize = COMMON_GetPicBufferSize(PIC_WIDTH, PIC_HEIGHT, PIXEL_FORMAT_YVU_SEMIPLANAR_422,
        DATA_BITWIDTH_8, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u64BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 10; // 10: Number of cache blocks per cache pool

    stVbConf.u32MaxPoolCnt = 2; // 2: The number of buffer pools that can be accommodated in the entire system

    if (u32SupplementConfig == 0) {
        s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    } else {
        s32Ret = SAMPLE_COMM_SYS_InitWithVbSupplement(&stVbConf, u32SupplementConfig);
    }
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VENC_VI_Init(SAMPLE_VI_CONFIG_S *pstViConfig, HI_BOOL bLowDelay, HI_U32 u32SupplementConfig)
{
    HI_S32  s32Ret;
    SAMPLE_SNS_TYPE_E enSnsType;
    ISP_CTRL_PARAM_S stIspCtrlParam;
    HI_U32 u32FrameRate;

    enSnsType = pstViConfig->astViInfo[0].stSnsInfo.enSnsType;

    pstViConfig->as32WorkingViId[0] = 0;
    pstViConfig->astViInfo[0].stSnsInfo.MipiDev =
        SAMPLE_COMM_VI_GetComboDevBySensor(pstViConfig->astViInfo[0].stSnsInfo.enSnsType, 0);
    pstViConfig->astViInfo[0].stSnsInfo.s32BusId = 0;
    pstViConfig->astViInfo[0].stDevInfo.enWDRMode = WDR_MODE_NONE;

    if (HI_TRUE == bLowDelay) {
        pstViConfig->astViInfo[0].stPipeInfo.enMastPipeMode = VI_ONLINE_VPSS_ONLINE;
    } else {
        pstViConfig->astViInfo[0].stPipeInfo.enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE;
    }
    s32Ret = SAMPLE_VENC_SYS_Init(u32SupplementConfig, enSnsType);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Init SYS err for %#x!\n", s32Ret);
        return s32Ret;
    }

    pstViConfig->astViInfo[0].stPipeInfo.aPipe[1] = -1;  // -1: the value of aPipe[1]
    pstViConfig->astViInfo[0].stPipeInfo.aPipe[2] = -1;  // -1: the value of aPipe[2]
    pstViConfig->astViInfo[0].stPipeInfo.aPipe[3]  = -1;  // -1: the value of aPipe[3]
    pstViConfig->astViInfo[0].stChnInfo.enVideoFormat = VIDEO_FORMAT_LINEAR;
    pstViConfig->astViInfo[0].stChnInfo.enCompressMode = COMPRESS_MODE_SEG; // COMPRESS_MODE_SEG

    s32Ret = SAMPLE_COMM_VI_SetParam(pstViConfig);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    SAMPLE_COMM_VI_GetFrameRateBySensor(enSnsType, &u32FrameRate);

    s32Ret = HI_MPI_ISP_GetCtrlParam(pstViConfig->astViInfo[0].stPipeInfo.aPipe[0], &stIspCtrlParam);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("HI_MPI_ISP_GetCtrlParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    // 30: ISP statistics update frequency is equal to (FrameRate / 30)
    stIspCtrlParam.u32StatIntvl  = u32FrameRate / 30;

    s32Ret = HI_MPI_ISP_SetCtrlParam(pstViConfig->astViInfo[0].stPipeInfo.aPipe[0], &stIspCtrlParam);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("HI_MPI_ISP_SetCtrlParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_COMM_SYS_Exit();
        SAMPLE_PRT("SAMPLE_COMM_VI_StartVi failed with %d!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VENC_ModifyResolution(SAMPLE_SNS_TYPE_E enSnsType, PIC_SIZE_E *penSize, SIZE_S *pstSize)
{
    HI_S32 s32Ret;
    SIZE_S stSnsSize;
    PIC_SIZE_E enSnsSize;

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    *penSize = enSnsSize;
    pstSize->u32Width  = stSnsSize.u32Width;
    pstSize->u32Height = stSnsSize.u32Height;

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VENC_CheckSensor(SAMPLE_SNS_TYPE_E enSnsType, SIZE_S stSize)
{
    HI_S32 s32Ret;
    SIZE_S          stSnsSize;
    PIC_SIZE_E      enSnsSize;

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    if ((stSnsSize.u32Width < stSize.u32Width) || (stSnsSize.u32Height < stSize.u32Height)) {
        SAMPLE_PRT("Sensor size is (%d,%d), but encode chnl is (%d,%d) !\n",
            stSnsSize.u32Width, stSnsSize.u32Height, stSize.u32Width, stSize.u32Height);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VENC_VPSS_Init(VPSS_GRP VpssGrp, HI_BOOL* pabChnEnable, DYNAMIC_RANGE_E enDynamicRange,
    PIXEL_FORMAT_E enPixelFormat, SIZE_S stSize[], SAMPLE_SNS_TYPE_E enSnsType)
{
    HI_S32 i;
    HI_S32 s32Ret;
    PIC_SIZE_E      enSnsSize;
    SIZE_S          stSnsSize;
    VPSS_GRP_ATTR_S stVpssGrpAttr = {0};
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    stVpssGrpAttr.enDynamicRange = enDynamicRange;
    stVpssGrpAttr.enPixelFormat  = enPixelFormat;
    stVpssGrpAttr.u32MaxW        = stSnsSize.u32Width;
    stVpssGrpAttr.u32MaxH        = stSnsSize.u32Height;
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1; // -1: The source frame rate, no frame rate control
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1; // -1: The destination frame rate, no frame rate control
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enNrType = VPSS_NR_TYPE_VIDEO;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;

    for (i = 0; i < VPSS_MAX_PHY_CHN_NUM; i++) {
        if (HI_TRUE == pabChnEnable[i]) {
            stVpssChnAttr[i].u32Width                     = stSize[i].u32Width;
            stVpssChnAttr[i].u32Height                    = stSize[i].u32Height;
            stVpssChnAttr[i].enChnMode                    = VPSS_CHN_MODE_USER;
            stVpssChnAttr[i].enCompressMode               = COMPRESS_MODE_NONE; // COMPRESS_MODE_SEG
            stVpssChnAttr[i].enDynamicRange               = enDynamicRange;
            stVpssChnAttr[i].enPixelFormat                = enPixelFormat;
            stVpssChnAttr[i].stFrameRate.s32SrcFrameRate  = -1; // -1: The source frame rate, no frame rate control
            stVpssChnAttr[i].stFrameRate.s32DstFrameRate  = -1; // -1: The destination frame rate, no frame rate control
            stVpssChnAttr[i].u32Depth                     = 0;
            stVpssChnAttr[i].bMirror                      = HI_FALSE;
            stVpssChnAttr[i].bFlip                        = HI_FALSE;
            stVpssChnAttr[i].enVideoFormat                = VIDEO_FORMAT_LINEAR;
            stVpssChnAttr[i].stAspectRatio.enMode         = ASPECT_RATIO_NONE;
        }
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, pabChnEnable, &stVpssGrpAttr, stVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start VPSS fail for %#x!\n", s32Ret);
    }

    return s32Ret;
}

/* init ViCfg */
HI_S32 ViCfgInit(ViCfg *self)
{
    HI_S32 s32Ret;
    HI_ASSERT(self);
    if (memset_s(self, sizeof(*self), 0, sizeof(*self)) != EOK) {
        HI_ASSERT(0);
    }

    SAMPLE_COMM_VI_GetSensorInfo(self);
    if (SAMPLE_SNS_TYPE_BUTT == self->astViInfo[0].stSnsInfo.enSnsType) {
        SAMPLE_PRT("Not set SENSOR%d_TYPE !\n", 0);
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_VENC_CheckSensor(self->astViInfo[0].stSnsInfo.enSnsType, g_sampleStoreInfo.stSize[0]);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = SAMPLE_VENC_ModifyResolution(self->astViInfo[0].stSnsInfo.enSnsType,
            &enSize[0], &g_sampleStoreInfo.stSize[0]);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "SAMPLE_VENC_ModifyResolution failed!\n", s32Ret);
    }

    return HI_SUCCESS;
}

HI_VOID ViCfgSetDevPipeChnInfo(ViCfg *self)
{
    HI_ASSERT(self);
    self->s32WorkingViNum = 1;
    self->astViInfo[0].stDevInfo.ViDev = 0;
    self->astViInfo[0].stPipeInfo.aPipe[0] = 0;
    self->astViInfo[0].stChnInfo.ViChn = 0;
    self->astViInfo[0].stChnInfo.enDynamicRange = DYNAMIC_RANGE_SDR8;
    self->astViInfo[0].stChnInfo.enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
}

static HI_VOID Pause(HI_VOID)
{
    printf("---------------press Enter key to exit!---------------\n");
    (void)getchar();
}

static HI_VOID EnRcModeGopModeCfg(SampleStoreInfo *storeInfo)
{
    storeInfo->enRcMode = SAMPLE_VENC_GetRcMode();
    storeInfo->enGopMode = SAMPLE_VENC_GetGopMode();
}

int SAMPLE_VENC_H265_H264(void)
{
    HI_S32 s32Ret;

    for (int i = 0; i < 2; i++) { // 2: channel number
        s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSize[i], &g_sampleStoreInfo.stSize[i]);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "SAMPLE_COMM_SYS_GetPicSize failed!\n", s32Ret);
    }

    s32Ret = ViCfgInit(&g_sampleStoreInfo.viCfg);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "ViCfgInit failed!\n", s32Ret);
    ViCfgSetDevPipeChnInfo(&g_sampleStoreInfo.viCfg);

    s32Ret = SAMPLE_VENC_VI_Init(&g_sampleStoreInfo.viCfg, HI_FALSE, u32SupplementConfig);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Init VI err for %#x!\n", s32Ret);

    s32Ret = SAMPLE_VENC_VPSS_Init(0, abChnEnable, DYNAMIC_RANGE_SDR8, PIXEL_FORMAT_YVU_SEMIPLANAR_420,
        g_sampleStoreInfo.stSize, g_sampleStoreInfo.viCfg.astViInfo[0].stSnsInfo.enSnsType);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VI_STOP, "Init VPSS err for %#x!\n", s32Ret);

    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(0, 0, 0);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VPSS_STOP, "VI Bind VPSS err for %#x!\n", s32Ret);

    /* start stream venc */
    EnRcModeGopModeCfg(&g_sampleStoreInfo);
    s32Ret = SAMPLE_COMM_VENC_GetGopAttr(g_sampleStoreInfo.enGopMode, &g_sampleStoreInfo.stGopAttr);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VI_VPSS_UNBIND, "Venc Get GopAttr for %#x!\n", s32Ret);

    /* encode h.265 */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[0], enPayLoad[0], enSize[0],
        g_sampleStoreInfo.enRcMode, u32Profile[0], bRcnRefShareBuf, &g_sampleStoreInfo.stGopAttr);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VI_VPSS_UNBIND, "Venc Start failed for %#x!\n", s32Ret);

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(0, VpssChn[0], VencChn[0]);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VENC_H265_STOP, "Venc Get GopAttr failed for %#x!\n", s32Ret);

    /* encode h.264 */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[1], enPayLoad[1], enSize[1],
        g_sampleStoreInfo.enRcMode, u32Profile[1], bRcnRefShareBuf, &g_sampleStoreInfo.stGopAttr);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VENC_H265_UnBind, "Venc Start failed for %#x!\n", s32Ret);

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(0, VpssChn[1], VencChn[1]);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VENC_H264_STOP, "Venc bind Vpss failed for %#x!\n", s32Ret);

    /* stream save process */
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(VencChn, 2); // 2: channel number
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VENC_H264_UnBind, "Start Venc failed!\n", s32Ret);

    Pause();

    /*  exit process */
    SAMPLE_COMM_VENC_StopGetStream();

EXIT_VENC_H264_UnBind:
    SAMPLE_COMM_VPSS_UnBind_VENC(0, VpssChn[1], VencChn[1]);
EXIT_VENC_H264_STOP:
    SAMPLE_COMM_VENC_Stop(VencChn[1]);
EXIT_VENC_H265_UnBind:
    SAMPLE_COMM_VPSS_UnBind_VENC(0, VpssChn[0], VencChn[0]);
EXIT_VENC_H265_STOP:
    SAMPLE_COMM_VENC_Stop(VencChn[0]);
EXIT_VI_VPSS_UNBIND:
    SAMPLE_COMM_VI_UnBind_VPSS(0, 0, 0);
EXIT_VPSS_STOP:
    SAMPLE_COMM_VPSS_Stop(0, abChnEnable);
EXIT_VI_STOP:
    SAMPLE_COMM_VI_StopVi(&g_sampleStoreInfo.viCfg);
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
