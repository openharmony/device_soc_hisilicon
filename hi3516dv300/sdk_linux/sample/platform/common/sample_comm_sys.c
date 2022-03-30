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
#include <sys/mman.h>
#include "sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static HI_S32 s_s32SampleMemDev = -1;
#define SAMPLE_MEM_DEV_OPEN                                    \
    if (s_s32SampleMemDev <= 0) {                              \
        s_s32SampleMemDev = open("/dev/mem", O_RDWR | O_SYNC); \
        if (s_s32SampleMemDev < 0) {                           \
            perror("Open dev/mem error");                      \
            return NULL;                                       \
        }                                                      \
    }

HI_VOID SAMPLE_SYS_SIGNAL(void (*func)(int))
{
#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
    struct sigaction sa = { 0 };

    sa.sa_handler = func;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, HI_NULL);
    sigaction(SIGTERM, &sa, HI_NULL);
#endif
}

HI_VOID *SAMPLE_SYS_IOMmap(HI_U64 u64PhyAddr, HI_U32 u32Size)
{
    HI_U32 u32Diff;
    HI_U64 u64PagePhy;
    HI_U8 *pPageAddr = HI_NULL;
    HI_U32 u32PageSize;

    SAMPLE_MEM_DEV_OPEN;

    if (u32Size == 0) {
        printf("Func: %s u32Size can't be 0.\n", __FUNCTION__);
        return NULL;
    }

    /* The mmap address should align with page */
    u64PagePhy = u64PhyAddr & 0xfffffffffffff000ULL;
    u32Diff = u64PhyAddr - u64PagePhy;

    /* The mmap size should be multiples of 1024 */
    u32PageSize = ((u32Size + u32Diff - 1) & 0xfffff000UL) + 0x1000;

    pPageAddr = mmap((void *)0, u32PageSize, PROT_READ | PROT_WRITE, MAP_SHARED, s_s32SampleMemDev, u64PagePhy);
    if (MAP_FAILED == pPageAddr) {
        perror("mmap error");
        return NULL;
    }
    return (HI_VOID *)(pPageAddr + u32Diff);
}

HI_S32 SAMPLE_SYS_Munmap(HI_VOID *pVirAddr, HI_U32 u32Size)
{
    HI_U64 u64PageAddr;
    HI_U32 u32PageSize;
    HI_U32 u32Diff;

    u64PageAddr = (((HI_U64)(HI_UINTPTR_T)pVirAddr) & 0xfffffffffffff000ULL);
    u32Diff = (HI_U32)(HI_UINTPTR_T)pVirAddr - u64PageAddr;
    u32PageSize = ((u32Size + u32Diff - 1) & 0xfffff000UL) + 0x1000;

    return munmap((HI_VOID *)(HI_UINTPTR_T)u64PageAddr, u32PageSize);
}


HI_S32 SAMPLE_SYS_SetReg(HI_U64 u64Addr, HI_U32 u32Value)
{
    HI_U32 *pu32RegAddr = NULL;
    HI_U32 u32MapLen = sizeof(u32Value);

    pu32RegAddr = (HI_U32 *)SAMPLE_SYS_IOMmap(u64Addr, u32MapLen);
    if (pu32RegAddr == NULL) {
        return HI_FAILURE;
    }

    *pu32RegAddr = u32Value;

    return SAMPLE_SYS_Munmap(pu32RegAddr, u32MapLen);
}

HI_S32 SAMPLE_SYS_GetReg(HI_U64 u64Addr, HI_U32 *pu32Value)
{
    HI_U32 *pu32RegAddr = NULL;
    HI_U32 u32MapLen;

    if (pu32Value == NULL) {
        return HI_ERR_SYS_NULL_PTR;
    }

    u32MapLen = sizeof(*pu32Value);
    pu32RegAddr = (HI_U32 *)SAMPLE_SYS_IOMmap(u64Addr, u32MapLen);
    if (pu32RegAddr == NULL) {
        return HI_FAILURE;
    }

    *pu32Value = *pu32RegAddr;

    return SAMPLE_SYS_Munmap(pu32RegAddr, u32MapLen);
}

HI_S32 SAMPLE_COMM_SYS_GetPicSize(PIC_SIZE_E enPicSize, SIZE_S *pstSize)
{
    if (pstSize == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    switch (enPicSize) {
        case PIC_D1_MIPI:
            pstSize->u32Width  = 800; /* PIC_D1_MIPI width is 800 */
            pstSize->u32Height = 480; /* PIC_D1_MIPI width is 480 */
            break;

        case PIC_CIF:
            pstSize->u32Width  = 352; /* PIC_CIF width is 352 */
            pstSize->u32Height = 288; /* PIC_CIF height is 288 */
            break;

        case PIC_360P:
            pstSize->u32Width  = 640; /* PIC_360P width is 640 */
            pstSize->u32Height = 360; /* PIC_360P height is 360 */
            break;

        case PIC_D1_PAL:
            pstSize->u32Width  = 720; /* PIC_D1_PAL width is 720 */
            pstSize->u32Height = 576; /* PIC_D1_PAL height is 576 */
            break;

        case PIC_720P:
            pstSize->u32Width  = 1280; /* PIC_720P width is 1280 */
            pstSize->u32Height = 720; /* PIC_720P height is 720 */
            break;

        case PIC_1080P:
            pstSize->u32Width = IMG_2M_WIDTH;
            pstSize->u32Height = IMG_2M_HEIGHT;
            break;

        case PIC_2592x1944:
            pstSize->u32Width = IMG_5M_WIDTH;
            pstSize->u32Height = IMG_5M_HEIGHT;
            break;

        case PIC_2592x1536:
            pstSize->u32Width = IMG_4M_WIDTH;
            pstSize->u32Height = IMG_4M_HEIGHT;
            break;

        default:
            pstSize->u32Width = IMG_4M_WIDTH;
            pstSize->u32Height = IMG_4M_HEIGHT;
            break;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_SYS_MemConfig(HI_VOID)
{
    HI_S32 i, j;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR *pcMmzName = NULL;
    MPP_CHN_S stMppChn;

    /* config memory for vi */
    for (i = 0; i < VI_MAX_PIPE_NUM; i++) {
        for (j = 0; j < VI_MAX_CHN_NUM; j++) {
            stMppChn.enModId = HI_ID_VI;
            stMppChn.s32DevId = i;
            stMppChn.s32ChnId = j;
            s32Ret = HI_MPI_SYS_SetMemConfig(&stMppChn, pcMmzName);
            if (s32Ret) {
                SAMPLE_PRT("HI_MPI_SYS_SetMemConfig ERR !\n");
                return HI_FAILURE;
            }
        }
    }

    /* config memory for vpss */
    for (i = 0; i < VPSS_MAX_GRP_NUM; i++) {
        stMppChn.enModId = HI_ID_VPSS;
        stMppChn.s32DevId = i;
        stMppChn.s32ChnId = 0;
        s32Ret = HI_MPI_SYS_SetMemConfig(&stMppChn, pcMmzName);
        if (s32Ret) {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConfig ERR !\n");
            return HI_FAILURE;
        }
    }

    /* config memory for venc */
    for (i = 0; i < VENC_MAX_CHN_NUM; i++) {
        stMppChn.enModId = HI_ID_VENC;
        stMppChn.s32DevId = 0;
        stMppChn.s32ChnId = i;
        s32Ret = HI_MPI_SYS_SetMemConfig(&stMppChn, pcMmzName);
        if (s32Ret) {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
            return HI_FAILURE;
        }
    }

    /* config memory for vo */
    for (i = 0; i < VO_MAX_LAYER_NUM; i++) {
        for (j = 0; j < VO_MAX_CHN_NUM; j++) {
            stMppChn.enModId = HI_ID_VO;
            stMppChn.s32DevId = i;
            stMppChn.s32ChnId = j;
            s32Ret = HI_MPI_SYS_SetMemConfig(&stMppChn, pcMmzName);
            if (s32Ret) {
                SAMPLE_PRT("HI_MPI_SYS_SetMemConfig ERR !\n");
                return HI_FAILURE;
            }
        }
    }

    /* config memory for vdec */
    for (i = 0; i < VDEC_MAX_CHN_NUM; i++) {
        stMppChn.enModId = HI_ID_VDEC;
        stMppChn.s32DevId = 0;
        stMppChn.s32ChnId = i;
        s32Ret = HI_MPI_SYS_SetMemConfig(&stMppChn, pcMmzName);
        if (s32Ret) {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
            return HI_FAILURE;
        }
    }

    return s32Ret;
}

/* function : vb init & MPI system init */
HI_S32 SAMPLE_COMM_SYS_Init(VB_CONFIG_S *pstVbConfig)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();

    if (pstVbConfig == NULL) {
        SAMPLE_PRT("input parameter is null, it is invalid!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_SetConfig(pstVbConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VB_SetConf failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_Init();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VB_Init failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_SYS_Init();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_SYS_Init failed!\n");
        HI_MPI_VB_Exit();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* function : vb init with VbSupplement & MPI system init */
HI_S32 SAMPLE_COMM_SYS_InitWithVbSupplement(VB_CONFIG_S *pstVbConf, HI_U32 u32SupplementConfig)
{
    VB_SUPPLEMENT_CONFIG_S stSupplementConf = { 0 };
    HI_S32 s32Ret = HI_FAILURE;

    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();

    if (pstVbConf == NULL) {
        SAMPLE_PRT("input parameter is null, it is invalid!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_SetConfig(pstVbConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VB_SetConf failed!\n");
        return HI_FAILURE;
    }

    stSupplementConf.u32SupplementConfig = u32SupplementConfig;

    s32Ret = HI_MPI_VB_SetSupplementConfig(&stSupplementConf);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VB_SetSupplementConf failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_Init();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VB_Init failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_SYS_Init();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_SYS_Init failed!\n");
        HI_MPI_VB_Exit();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* function : vb exit & MPI system exit */
HI_VOID SAMPLE_COMM_SYS_Exit(void)
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_ExitModCommPool(VB_UID_VDEC);
    HI_MPI_VB_Exit();
    return;
}

HI_S32 SAMPLE_COMM_VI_Bind_VO(VI_PIPE ViPipe, VI_CHN ViChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VI;
    stSrcChn.s32DevId = ViPipe;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = HI_ID_VO;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    CHECK_RET(HI_MPI_SYS_Bind(&stSrcChn, &stDestChn), "HI_MPI_SYS_Bind(VI-VO)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_UnBind_VO(VI_PIPE ViPipe, VI_CHN ViChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VI;
    stSrcChn.s32DevId = ViPipe;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = HI_ID_VO;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    CHECK_RET(HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn), "HI_MPI_SYS_UnBind(VI-VO)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_Bind_VPSS(VI_PIPE ViPipe, VI_CHN ViChn, VPSS_GRP VpssGrp)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VI;
    stSrcChn.s32DevId = ViPipe;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = 0;

    CHECK_RET(HI_MPI_SYS_Bind(&stSrcChn, &stDestChn), "HI_MPI_SYS_Bind(VI-VPSS)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_UnBind_VPSS(VI_PIPE ViPipe, VI_CHN ViChn, VPSS_GRP VpssGrp)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VI;
    stSrcChn.s32DevId = ViPipe;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = 0;

    CHECK_RET(HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn), "HI_MPI_SYS_UnBind(VI-VPSS)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_Bind_VENC(VI_PIPE ViPipe, VI_CHN ViChn, VENC_CHN VencChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VI;
    stSrcChn.s32DevId = ViPipe;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VencChn;

    CHECK_RET(HI_MPI_SYS_Bind(&stSrcChn, &stDestChn), "HI_MPI_SYS_Bind(VI-VENC)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_UnBind_VENC(VI_PIPE ViPipe, VI_CHN ViChn, VENC_CHN VencChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VI;
    stSrcChn.s32DevId = ViPipe;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VencChn;

    CHECK_RET(HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn), "HI_MPI_SYS_UnBind(VI-VENC)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VPSS_Bind_VO(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;
    stDestChn.enModId = HI_ID_VO;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    CHECK_RET(HI_MPI_SYS_Bind(&stSrcChn, &stDestChn), "HI_MPI_SYS_Bind(VPSS-VO)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VPSS_UnBind_VO(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;
    stDestChn.enModId = HI_ID_VO;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    CHECK_RET(HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn), "HI_MPI_SYS_UnBind(VPSS-VO)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VPSS_Bind_VENC(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VENC_CHN VencChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VencChn;

    CHECK_RET(HI_MPI_SYS_Bind(&stSrcChn, &stDestChn), "HI_MPI_SYS_Bind(VPSS-VENC)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VPSS_UnBind_VENC(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, VENC_CHN VencChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VencChn;

    CHECK_RET(HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn), "HI_MPI_SYS_UnBind(VPSS-VENC)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VDEC_Bind_VPSS(VDEC_CHN VdecChn, VPSS_GRP VpssGrp)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdecChn;
    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = 0;

    CHECK_RET(HI_MPI_SYS_Bind(&stSrcChn, &stDestChn), "HI_MPI_SYS_Bind(VDEC-VPSS)");

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VDEC_UnBind_VPSS(VDEC_CHN VdecChn, VPSS_GRP VpssGrp)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdecChn;
    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = 0;

    CHECK_RET(HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn), "HI_MPI_SYS_UnBind(VDEC-VPSS)");

    return HI_SUCCESS;
}
HI_S32 SAMPLE_COMM_VO_Bind_VO(VO_LAYER SrcVoLayer, VO_CHN SrcVoChn, VO_LAYER DstVoLayer, VO_CHN DstVoChn)
{
    MPP_CHN_S stSrcChn, stDestChn;
    stSrcChn.enModId = HI_ID_VO;
    stSrcChn.s32DevId = SrcVoLayer;
    stSrcChn.s32ChnId = SrcVoChn;
    stDestChn.enModId = HI_ID_VO;
    stDestChn.s32DevId = DstVoLayer;
    stDestChn.s32ChnId = DstVoChn;

    return HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
}

HI_S32 SAMPLE_COMM_VO_UnBind_VO(VO_LAYER DstVoLayer, VO_CHN DstVoChn)
{
    MPP_CHN_S stDestChn;
    stDestChn.enModId = HI_ID_VO;
    stDestChn.s32DevId = DstVoLayer;
    stDestChn.s32ChnId = DstVoChn;

    return HI_MPI_SYS_UnBind(NULL, &stDestChn);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
