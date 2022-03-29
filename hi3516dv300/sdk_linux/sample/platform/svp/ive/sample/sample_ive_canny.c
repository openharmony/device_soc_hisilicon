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
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>

#include "sample_comm_ive.h"

#define SAMPLE_IVE_CANNY_HYS_EDGE_HIGH_THR 150
#define SAMPLE_IVE_CANNY_HYS_EDGE_LOW_THR  50
#define SAMPLE_IVE_CANNY_THR_U16_HIGH_THR  100
#define SAMPLE_IVE_CANNY_THR_U16_LOW_THR   100
#define SAMPLE_IVE_CANNY_THR_U16_MAX_VAL   255
#define SAMPLE_IVE_CANNY_THR_U16_MID_VAL   0
#define SAMPLE_IVE_CANNY_THR_U16_MIN_VAL   0

typedef struct hiSAMPLE_IVE_CANNY_INFO_S {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stEdge;
    IVE_DST_IMAGE_S stMag;
    IVE_MEM_INFO_S stStack;
    IVE_CANNY_HYS_EDGE_CTRL_S stCannyHysEdgeCtrl;
    IVE_MAG_AND_ANG_CTRL_S stMagAndAngCtrl;
    IVE_THRESH_U16_CTRL_S stThrU16Ctrl;
    FILE *pFpSrc;
    FILE *pFpDst;
} SAMPLE_IVE_CANNY_INFO_S;

static SAMPLE_IVE_CANNY_INFO_S s_stCannyInfo;
static HI_BOOL s_bStopSignal = HI_FALSE;

static HI_VOID SAMPLE_IVE_Canny_Uninit(SAMPLE_IVE_CANNY_INFO_S *pstCannyInfo)
{
    IVE_MMZ_FREE(pstCannyInfo->stSrc.au64PhyAddr[0], pstCannyInfo->stSrc.au64VirAddr[0]);
    IVE_MMZ_FREE(pstCannyInfo->stEdge.au64PhyAddr[0], pstCannyInfo->stEdge.au64VirAddr[0]);
    IVE_MMZ_FREE(pstCannyInfo->stMag.au64PhyAddr[0], pstCannyInfo->stMag.au64VirAddr[0]);
    IVE_MMZ_FREE(pstCannyInfo->stStack.u64PhyAddr, pstCannyInfo->stStack.u64VirAddr);
    IVE_MMZ_FREE(pstCannyInfo->stCannyHysEdgeCtrl.stMem.u64PhyAddr, pstCannyInfo->stCannyHysEdgeCtrl.stMem.u64VirAddr);

    IVE_CLOSE_FILE(pstCannyInfo->pFpSrc);
    IVE_CLOSE_FILE(pstCannyInfo->pFpDst);
}

static HI_S32 SAMPLE_IVE_Canny_Init(SAMPLE_IVE_CANNY_INFO_S *pstCannyInfo, HI_CHAR *pchSrcFileName,
    HI_CHAR *pchDstFileName, HI_U32 u32Width, HI_U32 u32Height)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Size;
    /* canny filter mask 5x5 */
    HI_S8 as8Mask[IVE_MASK_NUM_25] = {
        0, 0, 0, 0, 0,
        0, -1, 0, 1, 0,
        0, -2, 0, 2, 0,
        0, -1, 0, 1, 0,
        0, 0, 0, 0, 0};

    (HI_VOID)memset_s(pstCannyInfo, sizeof(SAMPLE_IVE_CANNY_INFO_S), 0, sizeof(SAMPLE_IVE_CANNY_INFO_S));
    (HI_VOID)memcpy_s(pstCannyInfo->stCannyHysEdgeCtrl.as8Mask, IVE_MASK_NUM_25, as8Mask, IVE_MASK_NUM_25);
    (HI_VOID)memcpy_s(pstCannyInfo->stMagAndAngCtrl.as8Mask, IVE_MASK_NUM_25, as8Mask, IVE_MASK_NUM_25);
    pstCannyInfo->stCannyHysEdgeCtrl.u16HighThr = SAMPLE_IVE_CANNY_HYS_EDGE_HIGH_THR;
    pstCannyInfo->stCannyHysEdgeCtrl.u16LowThr = SAMPLE_IVE_CANNY_HYS_EDGE_LOW_THR;
    pstCannyInfo->stMagAndAngCtrl.enOutCtrl = IVE_MAG_AND_ANG_OUT_CTRL_MAG;
    pstCannyInfo->stMagAndAngCtrl.u16Thr = 0;
    pstCannyInfo->stThrU16Ctrl.enMode = IVE_THRESH_U16_MODE_U16_TO_U8_MIN_MID_MAX;
    pstCannyInfo->stThrU16Ctrl.u16HighThr = SAMPLE_IVE_CANNY_THR_U16_HIGH_THR;
    pstCannyInfo->stThrU16Ctrl.u16LowThr = SAMPLE_IVE_CANNY_THR_U16_LOW_THR;
    pstCannyInfo->stThrU16Ctrl.u8MaxVal = SAMPLE_IVE_CANNY_THR_U16_MAX_VAL;
    pstCannyInfo->stThrU16Ctrl.u8MidVal = SAMPLE_IVE_CANNY_THR_U16_MID_VAL;
    pstCannyInfo->stThrU16Ctrl.u8MinVal = SAMPLE_IVE_CANNY_THR_U16_MIN_VAL;

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstCannyInfo->stSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, CANNY_INIT_FAIL, "Error(%#x),Create Src Image failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstCannyInfo->stEdge, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, CANNY_INIT_FAIL, "Error(%#x),Create edge Image failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstCannyInfo->stMag, IVE_IMAGE_TYPE_U16C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, CANNY_INIT_FAIL, "Error(%#x),Create Mag Image failed!\n", s32Ret);
    u32Size = (HI_U32)sizeof(HI_U16) + (HI_U32)sizeof(HI_U16);
    u32Size = pstCannyInfo->stSrc.au32Stride[0] * pstCannyInfo->stSrc.u32Height * u32Size +
        sizeof(IVE_CANNY_STACK_SIZE_S);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&pstCannyInfo->stStack, u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, CANNY_INIT_FAIL, "Error(%#x),Create Stack Mem info failed!\n", s32Ret);
    u32Size = (HI_U32)sizeof(HI_U16) + (HI_U32)sizeof(HI_U8);
    u32Size = pstCannyInfo->stSrc.au32Stride[0] * pstCannyInfo->stSrc.u32Height * u32Size;
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&pstCannyInfo->stCannyHysEdgeCtrl.stMem, u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, CANNY_INIT_FAIL,
        "Error(%#x),Create CannyHysEdgeCtrl.stMem Mem info failed!\n", s32Ret);

    s32Ret = HI_FAILURE;
    pstCannyInfo->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(pstCannyInfo->pFpSrc == HI_NULL, CANNY_INIT_FAIL, "Error,Open file %s failed!\n",
        pchSrcFileName);
    pstCannyInfo->pFpDst = fopen(pchDstFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstCannyInfo->pFpDst == HI_NULL, CANNY_INIT_FAIL, "Error,Open file %s failed!\n",
        pchDstFileName);

    s32Ret = HI_SUCCESS;

CANNY_INIT_FAIL:
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_IVE_Canny_Uninit(pstCannyInfo);
    }
    return s32Ret;
}

static HI_S32 SAMPLE_IVE_Complete_Canny(SAMPLE_IVE_CANNY_INFO_S *pstCannyInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bInstant = HI_TRUE;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;
    IVE_HANDLE IveHandle;

    s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstCannyInfo->stSrc), pstCannyInfo->pFpSrc);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_CannyHysEdge(&IveHandle, &pstCannyInfo->stSrc, &pstCannyInfo->stEdge, &pstCannyInfo->stStack,
        &pstCannyInfo->stCannyHysEdgeCtrl, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_CannyHysEdge failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
        usleep(IVE_QUERY_SLEEP_TIME);
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);
    s32Ret = HI_MPI_IVE_CannyEdge(&pstCannyInfo->stEdge, &pstCannyInfo->stStack);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_CannyEdge failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstCannyInfo->stEdge, pstCannyInfo->pFpDst);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Write edge file failed!\n", s32Ret);

    return s32Ret;
}

static HI_S32 SAMPLE_IVE_Part_Canny(SAMPLE_IVE_CANNY_INFO_S *pstCannyInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bInstant = HI_TRUE;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;
    IVE_HANDLE IveHandle;

    s32Ret = SAMPLE_COMM_IVE_ReadFile(&pstCannyInfo->stSrc, pstCannyInfo->pFpSrc);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);

    bInstant = HI_FALSE;
    s32Ret = HI_MPI_IVE_MagAndAng(&IveHandle, &pstCannyInfo->stSrc, &pstCannyInfo->stMag, HI_NULL,
        &pstCannyInfo->stMagAndAngCtrl, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_MagAndAng failed!\n", s32Ret);

    bInstant = HI_TRUE;
    s32Ret = HI_MPI_IVE_Thresh_U16(&IveHandle, &pstCannyInfo->stMag, &pstCannyInfo->stEdge, &pstCannyInfo->stThrU16Ctrl,
        bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Thresh_U16 failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
        usleep(IVE_QUERY_SLEEP_TIME);
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstCannyInfo->stEdge, pstCannyInfo->pFpDst);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Write edge file failed!\n", s32Ret);

    return s32Ret;
}

static HI_VOID SAMPLE_IVE_CannyStop(HI_VOID)
{
    SAMPLE_IVE_Canny_Uninit(&s_stCannyInfo);
    (HI_VOID)memset_s(&s_stCannyInfo, sizeof(s_stCannyInfo), 0, sizeof(s_stCannyInfo));
    SAMPLE_COMM_IVE_IveMpiExit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

HI_VOID SAMPLE_IVE_Canny(HI_CHAR chComplete)
{
    HI_U16 u32Width = IVE_D1_WIDTH;
    HI_U16 u32Height = IVE_D1_HEIGHT;
    HI_CHAR *pchSrcFileName = "./data/input/canny/canny.yuv";
    HI_CHAR *pchDstFileName = "./data/output/canny";

    HI_CHAR achSrcFileName[PATH_MAX] = {0};
    HI_CHAR achDstFileName[PATH_MAX] = {0};
    HI_CHAR tmpFile[PATH_MAX] = {0};
    HI_S32 s32Ret;
    s_bStopSignal = HI_FALSE;

    SAMPLE_CHECK_EXPR_RET_VOID((strlen(pchSrcFileName) > PATH_MAX) ||
        (realpath(pchSrcFileName, achSrcFileName) == NULL),
        "invalid input file\n");

    SAMPLE_CHECK_EXPR_RET_VOID((strlen(pchDstFileName) > PATH_MAX) ||
        (realpath(pchDstFileName, achDstFileName) == NULL),
        "invalid input file\n");

    s32Ret = snprintf_s(tmpFile, sizeof(tmpFile), sizeof(tmpFile) - 1, "/cannyout_complete_%c.yuv", chComplete);
    SAMPLE_CHECK_EXPR_RET_VOID(s32Ret < 0, "invalid param!\n");

    s32Ret = strcat_s(achDstFileName, PATH_MAX, tmpFile);
    SAMPLE_CHECK_EXPR_RET_VOID(s32Ret < 0, "invalid param!\n");

    (HI_VOID)memset_s(&s_stCannyInfo, sizeof(s_stCannyInfo), 0, sizeof(s_stCannyInfo));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    s32Ret = SAMPLE_IVE_Canny_Init(&s_stCannyInfo, achSrcFileName, achDstFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, CANNY_FAIL, "Error(%#x),SAMPLE_IVE_Canny_Init failed!\n", s32Ret);
    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_CannyStop();
        return;
    }

    if (chComplete == '0') {
        s32Ret = SAMPLE_IVE_Part_Canny(&s_stCannyInfo);
    } else {
        s32Ret = SAMPLE_IVE_Complete_Canny(&s_stCannyInfo);
    }

    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_CannyStop();
        return;
    }

    if (s32Ret == HI_SUCCESS) {
        SAMPLE_PRT("Process success!\n");
    }

    s_bStopSignal = HI_TRUE;
    SAMPLE_IVE_Canny_Uninit(&s_stCannyInfo);
    (HI_VOID)memset_s(&s_stCannyInfo, sizeof(s_stCannyInfo), 0, sizeof(s_stCannyInfo));

CANNY_FAIL:
    s_bStopSignal = HI_TRUE;
    SAMPLE_COMM_IVE_IveMpiExit();
}

HI_VOID SAMPLE_IVE_Canny_HandleSig(HI_VOID)
{
    s_bStopSignal = HI_TRUE;
}
