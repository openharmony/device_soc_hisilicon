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

#include "sample_comm_ive.h"

#define SAMPLE_IVE_LK_MAX_POINT_NUM 500
#define SAMPLE_IVE_LK_MIN_DIST      5
#define SAMPLE_IVE_LK_PYR_MAX_LEVEL 4
#define SAMPLE_IVE_LK_PYR_LEVEL_3   3
#define SAMPLE_IVE_LK_MIN_EIG_THR   100
#define SAMPLE_IVE_LK_ITER_CNT      10
#define SAMPLE_IVE_LK_EPS           2
#define SAMPLE_IVE_LK_QUALITY_LEVEL 25
#define SAMPLE_IVE_LK_FRM_NUM       10
#define SAMPLE_IVE_LK_FIX_POINT_7   7

typedef struct hiSAMPLE_IVE_ST_LK_S {
    IVE_SRC_IMAGE_S astPrevPyr[SAMPLE_IVE_LK_PYR_MAX_LEVEL];
    IVE_SRC_IMAGE_S astNextPyr[SAMPLE_IVE_LK_PYR_MAX_LEVEL];
    IVE_SRC_MEM_INFO_S stPrevPts;
    IVE_MEM_INFO_S stNextPts;
    IVE_DST_MEM_INFO_S stStatus;
    IVE_DST_MEM_INFO_S stErr;
    IVE_LK_OPTICAL_FLOW_PYR_CTRL_S stLkPyrCtrl;

    IVE_SRC_IMAGE_S stStSrc;
    IVE_IMAGE_S stStDst;
    IVE_DST_MEM_INFO_S stCorner;
    IVE_ST_CANDI_CORNER_CTRL_S stStCandiCornerCtrl;
    IVE_ST_CORNER_CTRL_S stStCornerCtrl;

    IVE_IMAGE_S stPyrTmp;
    IVE_IMAGE_S stSrcYuv;
    FILE *pFpSrc;
} SAMPLE_IVE_ST_LK_S;

static SAMPLE_IVE_ST_LK_S s_stStLk;
static HI_BOOL s_bStopSignal = HI_FALSE;

static HI_S32 SAMPLE_IVE_St_Lk_DMA(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,
    IVE_DMA_CTRL_S *pstDmaCtrl, HI_BOOL bInstant)
{
    HI_S32 s32Ret = HI_FAILURE;
    IVE_SRC_DATA_S stDataSrc;
    IVE_DST_DATA_S stDataDst;

    stDataSrc.u64VirAddr = pstSrc->au64VirAddr[0];
    stDataSrc.u64PhyAddr = pstSrc->au64PhyAddr[0];
    stDataSrc.u32Width = pstSrc->u32Width;
    stDataSrc.u32Height = pstSrc->u32Height;
    stDataSrc.u32Stride = pstSrc->au32Stride[0];

    stDataDst.u64VirAddr = pstDst->au64VirAddr[0];
    stDataDst.u64PhyAddr = pstDst->au64PhyAddr[0];
    stDataDst.u32Width = pstDst->u32Width;
    stDataDst.u32Height = pstDst->u32Height;
    stDataDst.u32Stride = pstDst->au32Stride[0];
    s32Ret = HI_MPI_IVE_DMA(pIveHandle, &stDataSrc, &stDataDst, pstDmaCtrl, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_DMA failed!\n", s32Ret);

    return s32Ret;
}

/* function : Copy pyr */
static HI_VOID SAMPLE_IVE_St_Lk_CopyPyr(IVE_SRC_IMAGE_S astPyrSrc[], IVE_DST_IMAGE_S astPyrDst[], HI_U8 u8MaxLevel)
{
    HI_U8 i;
    HI_S32 s32Ret = HI_FAILURE;
    IVE_HANDLE hIveHandle;

    IVE_DMA_CTRL_S stCtrl;
    (HI_VOID)memset_s(&stCtrl, sizeof(stCtrl), 0, sizeof(stCtrl));
    stCtrl.enMode = IVE_DMA_MODE_DIRECT_COPY;

    for (i = 0; i <= u8MaxLevel; i++) {
        s32Ret = SAMPLE_IVE_St_Lk_DMA(&hIveHandle, &astPyrSrc[i], &astPyrDst[i], &stCtrl, HI_FALSE);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("SAMPLE_IVE_St_Lk_DMA fail,Error(%d)\n", s32Ret);
            break;
        }
    }
}

/* function : St lk uninit */
static HI_VOID SAMPLE_IVE_St_Lk_Uninit(SAMPLE_IVE_ST_LK_S *pstStLk)
{
    HI_U16 i;
    for (i = 0; i <= pstStLk->stLkPyrCtrl.u8MaxLevel; i++) {
        IVE_MMZ_FREE(pstStLk->astPrevPyr[i].au64PhyAddr[0], pstStLk->astPrevPyr[i].au64VirAddr[0]);
        IVE_MMZ_FREE(pstStLk->astNextPyr[i].au64PhyAddr[0], pstStLk->astNextPyr[i].au64VirAddr[0]);
    }

    IVE_MMZ_FREE(pstStLk->stPrevPts.u64PhyAddr, pstStLk->stPrevPts.u64VirAddr);
    IVE_MMZ_FREE(pstStLk->stNextPts.u64PhyAddr, pstStLk->stNextPts.u64VirAddr);
    IVE_MMZ_FREE(pstStLk->stStatus.u64PhyAddr, pstStLk->stStatus.u64VirAddr);
    IVE_MMZ_FREE(pstStLk->stErr.u64PhyAddr, pstStLk->stErr.u64VirAddr);

    IVE_MMZ_FREE(pstStLk->stStSrc.au64PhyAddr[0], pstStLk->stStSrc.au64VirAddr[0]);
    IVE_MMZ_FREE(pstStLk->stStDst.au64PhyAddr[0], pstStLk->stStDst.au64VirAddr[0]);
    IVE_MMZ_FREE(pstStLk->stCorner.u64PhyAddr, pstStLk->stCorner.u64VirAddr);

    IVE_MMZ_FREE(pstStLk->stStCandiCornerCtrl.stMem.u64PhyAddr, pstStLk->stStCandiCornerCtrl.stMem.u64VirAddr);

    IVE_MMZ_FREE(pstStLk->stPyrTmp.au64PhyAddr[0], pstStLk->stPyrTmp.au64VirAddr[0]);
    IVE_MMZ_FREE(pstStLk->stSrcYuv.au64PhyAddr[0], pstStLk->stSrcYuv.au64VirAddr[0]);

    IVE_CLOSE_FILE(pstStLk->pFpSrc);
}

/* function : St lk init */
static HI_S32 SAMPLE_IVE_St_Lk_Init(SAMPLE_IVE_ST_LK_S *pstStLk, HI_CHAR *pchSrcFileName, HI_U32 u32Width,
    HI_U32 u32Height, HI_U32 u32PyrWidth, HI_U32 u32PyrHeight, HI_U8 u8MaxLevel)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Size = 0;
    HI_U16 i;

    (HI_VOID)memset_s(pstStLk, sizeof(SAMPLE_IVE_ST_LK_S), 0, sizeof(SAMPLE_IVE_ST_LK_S));

    pstStLk->stLkPyrCtrl.enOutMode = IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH;
    pstStLk->stLkPyrCtrl.bUseInitFlow = HI_TRUE;
    pstStLk->stLkPyrCtrl.u16PtsNum = SAMPLE_IVE_LK_MAX_POINT_NUM;
    pstStLk->stLkPyrCtrl.u8MaxLevel = u8MaxLevel;
    pstStLk->stLkPyrCtrl.u0q8MinEigThr = SAMPLE_IVE_LK_MIN_EIG_THR; /* Minimum eigenvalue threshold: 100 */
    pstStLk->stLkPyrCtrl.u8IterCnt = SAMPLE_IVE_LK_ITER_CNT; /* Maximum iteration times: 10 */
    pstStLk->stLkPyrCtrl.u0q8Eps = SAMPLE_IVE_LK_EPS; /* Threshold of iteration: 2 */
    // Init Pyr
    for (i = 0; i <= u8MaxLevel; i++) {
        s32Ret =
            SAMPLE_COMM_IVE_CreateImage(&pstStLk->astPrevPyr[i], IVE_IMAGE_TYPE_U8C1, u32Width >> i, u32Height >> i);
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create prevPyr[%d] image failed!\n",
            s32Ret, i);

        s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstStLk->astNextPyr[i], IVE_IMAGE_TYPE_U8C1,
            pstStLk->astPrevPyr[i].u32Width, pstStLk->astPrevPyr[i].u32Height);
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create nextPyr[%d] image failed!\n",
            s32Ret, i);
    }
    // Init prev pts
    u32Size = sizeof(IVE_POINT_S25Q7_S) * SAMPLE_IVE_LK_MAX_POINT_NUM;
    u32Size = SAMPLE_COMM_IVE_CalcStride(u32Size, IVE_ALIGN);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stPrevPts), u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create prevPts mem info failed!\n",
        s32Ret);
    // Init next pts
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stNextPts), u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create nextPts mem info failed!\n",
        s32Ret);

    // Init status
    u32Size = sizeof(HI_U8) * SAMPLE_IVE_LK_MAX_POINT_NUM;
    u32Size = SAMPLE_COMM_IVE_CalcStride(u32Size, IVE_ALIGN);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stStatus), u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create status mem info failed!\n",
        s32Ret);

    // Init err
    u32Size = sizeof(HI_U9Q7) * SAMPLE_IVE_LK_MAX_POINT_NUM;
    u32Size = SAMPLE_COMM_IVE_CalcStride(u32Size, IVE_ALIGN);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stErr), u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create err mem info failed!\n", s32Ret);

    // Init St
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstStLk->stStSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create src image failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstStLk->stStDst, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create dst image failed!\n", s32Ret);

    pstStLk->stStCandiCornerCtrl.u0q8QualityLevel = SAMPLE_IVE_LK_QUALITY_LEVEL; /* quality level: 25 */
    u32Size = 4 * SAMPLE_COMM_IVE_CalcStride(u32Width, IVE_ALIGN) * u32Height + sizeof(IVE_ST_MAX_EIG_S); /* 4 */
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stStCandiCornerCtrl.stMem), u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL,
        "Error(%#x),Create CandiCornerCtrl.stMem mem info failed!\n", s32Ret);

    u32Size = sizeof(IVE_ST_CORNER_INFO_S);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stCorner), u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create corner mem info failed!\n",
        s32Ret);

    pstStLk->stStCornerCtrl.u16MaxCornerNum = SAMPLE_IVE_LK_MAX_POINT_NUM;
    pstStLk->stStCornerCtrl.u16MinDist = SAMPLE_IVE_LK_MIN_DIST;

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstStLk->stPyrTmp, IVE_IMAGE_TYPE_U8C1, u32PyrWidth, u32PyrHeight);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create pyrTmp image failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstStLk->stSrcYuv, IVE_IMAGE_TYPE_YUV420SP, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_INIT_FAIL, "Error(%#x),Create srcYuv image failed!\n", s32Ret);

    s32Ret = HI_FAILURE;
    pstStLk->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(pstStLk->pFpSrc == HI_NULL, ST_LK_INIT_FAIL, "Error,Open file %s failed!\n", pchSrcFileName);

    s32Ret = HI_SUCCESS;

ST_LK_INIT_FAIL:
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_IVE_St_Lk_Uninit(pstStLk);
    }
    return s32Ret;
}

/* function : Pyr down */
static HI_S32 SAMPLE_IVE_St_Lk_PyrDown(SAMPLE_IVE_ST_LK_S *pstStLk, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst)
{
    HI_S32 s32Ret = HI_SUCCESS;
    IVE_HANDLE hIveHandle;
    IVE_DMA_CTRL_S stDmaCtrl = { IVE_DMA_MODE_INTERVAL_COPY, 0, 2, 1, 2 };
    IVE_FILTER_CTRL_S stFltCtrl = { { 1, 2, 3, 2, 1, 2, 5, 6, 5, 2, 3, 6, 8, 6, 3, 2, 5, 6, 5, 2, 1, 2, 3, 2, 1 }, 7 };

    pstStLk->stPyrTmp.u32Width = pstSrc->u32Width;
    pstStLk->stPyrTmp.u32Height = pstSrc->u32Height;

    s32Ret = HI_MPI_IVE_Filter(&hIveHandle, pstSrc, &pstStLk->stPyrTmp, &stFltCtrl, HI_FALSE);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Filter failed!\n", s32Ret);

    s32Ret = SAMPLE_IVE_St_Lk_DMA(&hIveHandle, &pstStLk->stPyrTmp, pstDst, &stDmaCtrl, HI_FALSE);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),SAMPLE_IVE_St_Lk_DMA failed!\n", s32Ret);

    return s32Ret;
}

/* function : St lk proc */
static HI_S32 SAMPLE_IVE_St_LkProc(SAMPLE_IVE_ST_LK_S *pstStLk)
{
    HI_U32 u32FrameNum = SAMPLE_IVE_LK_FRM_NUM;
    HI_U32 i, k;
    HI_U16 u16RectNum;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;
    IVE_HANDLE hIveHandle;
    IVE_DMA_CTRL_S stDmaCtrl;
    IVE_ST_CORNER_INFO_S *pstCornerInfo =
        SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(IVE_ST_CORNER_INFO_S, pstStLk->stCorner.u64VirAddr);
    IVE_POINT_S25Q7_S *psts25q7NextPts =
        SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(IVE_POINT_S25Q7_S, pstStLk->stNextPts.u64VirAddr);

    (HI_VOID)memset_s(&stDmaCtrl, sizeof(stDmaCtrl), 0, sizeof(stDmaCtrl));
    stDmaCtrl.enMode = IVE_DMA_MODE_DIRECT_COPY;

    for (i = 0; i < u32FrameNum && s_bStopSignal == HI_FALSE; i++) {
        SAMPLE_PRT("Proc frame %d\n", i);
        s32Ret = SAMPLE_COMM_IVE_ReadFile(&pstStLk->stSrcYuv, pstStLk->pFpSrc);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);

        s32Ret = SAMPLE_IVE_St_Lk_DMA(&hIveHandle, &pstStLk->stSrcYuv, &pstStLk->astNextPyr[0], &stDmaCtrl, HI_FALSE);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),SAMPLE_IVE_St_Lk_DMA failed!\n", s32Ret);

        for (k = 1; k <= pstStLk->stLkPyrCtrl.u8MaxLevel; k++) {
            s32Ret = SAMPLE_IVE_St_Lk_PyrDown(pstStLk, &pstStLk->astNextPyr[k - 1], &pstStLk->astNextPyr[k]);
            SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret,
                "Error(%#x),SAMPLE_IVE_St_Lk_PyrDown level %d failed!\n", s32Ret, k);
        }

        if (i == 0) {
            s32Ret = HI_MPI_IVE_STCandiCorner(&hIveHandle, &pstStLk->astNextPyr[0], &pstStLk->stStDst,
                &pstStLk->stStCandiCornerCtrl, HI_TRUE);
            SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_STCandiCorner failed!\n",
                s32Ret);

            s32Ret = HI_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
            while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
                usleep(IVE_QUERY_SLEEP_TIME);
                s32Ret = HI_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
            }
            SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);

            s32Ret = HI_MPI_IVE_STCorner(&pstStLk->stStDst, &pstStLk->stCorner, &pstStLk->stStCornerCtrl);
            SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_STCorner failed!\n", s32Ret);

            pstStLk->stLkPyrCtrl.u16PtsNum = pstCornerInfo->u16CornerNum;
            for (k = 0; k < pstStLk->stLkPyrCtrl.u16PtsNum; k++) {
                psts25q7NextPts[k].s25q7X = (HI_S32)(pstCornerInfo->astCorner[k].u16X << SAMPLE_IVE_LK_FIX_POINT_7);
                psts25q7NextPts[k].s25q7Y = (HI_S32)(pstCornerInfo->astCorner[k].u16Y << SAMPLE_IVE_LK_FIX_POINT_7);
            }
        } else {
            s32Ret =
                HI_MPI_IVE_LKOpticalFlowPyr(&hIveHandle, pstStLk->astPrevPyr, pstStLk->astNextPyr, &pstStLk->stPrevPts,
                &pstStLk->stNextPts, &pstStLk->stStatus, &pstStLk->stErr, &pstStLk->stLkPyrCtrl, HI_TRUE);
            SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_LKOpticalFlowPyr failed!\n",
                s32Ret);

            s32Ret = HI_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
            while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
                usleep(IVE_QUERY_SLEEP_TIME);
                s32Ret = HI_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
            }
            SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);

            u16RectNum = 0;
            for (k = 0; k < pstStLk->stLkPyrCtrl.u16PtsNum; k++) {
                if (!(SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstStLk->stStatus.u64VirAddr))[k]) {
                    continue;
                }
                psts25q7NextPts[u16RectNum].s25q7X = psts25q7NextPts[k].s25q7X;
                psts25q7NextPts[u16RectNum].s25q7Y = psts25q7NextPts[k].s25q7Y;
                u16RectNum++;
            }

            pstStLk->stLkPyrCtrl.u16PtsNum = u16RectNum;
        }
        /* pre pts address and next pts address can not be overlap */
        (HI_VOID)memcpy_s(SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_VOID, pstStLk->stPrevPts.u64VirAddr),
            sizeof(IVE_POINT_S25Q7_S) * pstStLk->stLkPyrCtrl.u16PtsNum,
            SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_VOID, pstStLk->stNextPts.u64VirAddr),
            sizeof(IVE_POINT_S25Q7_S) * pstStLk->stLkPyrCtrl.u16PtsNum);

        SAMPLE_IVE_St_Lk_CopyPyr(pstStLk->astNextPyr, pstStLk->astPrevPyr, pstStLk->stLkPyrCtrl.u8MaxLevel);
    }

    return s32Ret;
}

static HI_VOID SAMPLE_IVE_St_LkPause(HI_VOID)
{
    SAMPLE_IVE_St_Lk_Uninit(&s_stStLk);
    (HI_VOID)memset_s(&s_stStLk, sizeof(s_stStLk), 0, sizeof(s_stStLk));
    SAMPLE_COMM_IVE_IveMpiExit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

HI_VOID SAMPLE_IVE_St_Lk(HI_VOID)
{
    HI_CHAR *pchSrcFileName = "./data/input/stlk/st_lk_720x576_420sp.yuv";
    HI_CHAR achSrcFileName[PATH_MAX] = {0};
    HI_U16 u16Width = IVE_D1_WIDTH;
    HI_U16 u16Height = IVE_D1_HEIGHT;
    HI_U16 u16PyrWidth = IVE_D1_WIDTH;
    HI_U16 u16PyrHeight = IVE_D1_HEIGHT;

    HI_S32 s32Ret;
    HI_U8 u8MaxLevel = SAMPLE_IVE_LK_PYR_LEVEL_3;
    s_bStopSignal = HI_FALSE;

    SAMPLE_CHECK_EXPR_RET_VOID(
        (strlen(pchSrcFileName) > PATH_MAX) || (realpath(pchSrcFileName, achSrcFileName) == NULL), "invalid path!\n");
    (HI_VOID)memset_s(&s_stStLk, sizeof(s_stStLk), 0, sizeof(s_stStLk));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    s32Ret =
        SAMPLE_IVE_St_Lk_Init(&s_stStLk, achSrcFileName, u16Width, u16Height, u16PyrWidth, u16PyrHeight, u8MaxLevel);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, ST_LK_FAIL, "Error(%#x),SAMPLE_IVE_St_Lk_Init failed!\n", s32Ret);
    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_St_LkPause();
        return;
    }
    s32Ret = SAMPLE_IVE_St_LkProc(&s_stStLk);
    if (s32Ret == HI_SUCCESS) {
        SAMPLE_PRT("Process success!\n");
    }

    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_St_LkPause();
        return;
    }
    s_bStopSignal = HI_TRUE;
    SAMPLE_IVE_St_Lk_Uninit(&s_stStLk);
    (HI_VOID)memset_s(&s_stStLk, sizeof(s_stStLk), 0, sizeof(s_stStLk));

ST_LK_FAIL:
    s_bStopSignal = HI_TRUE;
    SAMPLE_COMM_IVE_IveMpiExit();
}

/*  function : St_Lk sample signal handle */
HI_VOID SAMPLE_IVE_St_Lk_HandleSig(HI_VOID)
{
    s_bStopSignal = HI_TRUE;
}