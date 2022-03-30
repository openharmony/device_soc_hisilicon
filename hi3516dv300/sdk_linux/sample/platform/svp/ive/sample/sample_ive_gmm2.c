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

#define SAMPLE_IVE_GMM2_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SAMPLE_IVE_GMM2_VAR_RATE            1
#define SAMPLE_IVE_GMM2_MODEL_NUM_3         3
#define SAMPLE_IVE_GMM2_MAX_VAR             ((16 * 16) << 7)
#define SAMPLE_IVE_GMM2_MIN_VAR             ((8 * 8) << 7)
#define SAMPLE_IVE_GMM2_GLB_SNS_FACTOR      8
#define SAMPLE_IVE_GMM2_FREQ_THR            12000
#define SAMPLE_IVE_GMM2_FREQ_INIT_VAL       20000
#define SAMPLE_IVE_GMM2_FREQ_ADD_FACTOR     0xEF
#define SAMPLE_IVE_GMM2_FREQ_REDU_FACTOR    0xFF00
#define SAMPLE_IVE_GMM2_LIFE_THR            5000
#define SAMPLE_IVE_GMM2_FACTOR_8            8
#define SAMPLE_IVE_GMM2_FACTOR_4            4
#define SAMPLE_IVE_GMM2_FACTOR_OFFSET       2
#define SAMPLE_IVE_GMM2_SRC_NUM             2
#define SAMPLE_IVE_GMM2_U8C1_MODLE_SIZE_UNIT 8
#define SAMPLE_IVE_GMM2_GEN_FG_MASK_MIN_VAL  0
#define SAMPLE_IVE_GMM2_GEN_FG_MASK_MAX_VAL  1
#define SAMPLE_IVE_GMM2_GEN_FG_MASK_LOW_THR  5
#define SAMPLE_IVE_GMM2_NUM_2                2
#define SAMPLE_IVE_GMM2_ABS_THR_10           10
#define SAMPLE_IVE_GMM2_FACTOR_16            16
#define SAMPLE_IVE_GMM2_FACTOR_20            20
#define SAMPLE_IVE_GMM2_FACTOR_200           200
#define SAMPLE_IVE_GMM2_TIMES_2              2
#define SAMPLE_IVE_GMM2_TIMES_3              3
#define SAMPLE_IVE_GMM2_TIMES_100           100
#define SAMPLE_IVE_GMM2_CCL_STEP             2
#define SAMPLE_IVE_GMM2_CCL_INIT_AREA_THR    4
#define SAMPLE_IVE_GMM2_16_TO_8_NUMERATOR    255
#define SAMPLE_IVE_GMM2_16_TO_8_DENOMINATOR  (255 * 4)
#define SAMPLE_IVE_GMM2_MAX_AREA             60
#define SAMPLE_IVE_GMM2_MIN_GRAD_SUM         10
#define SAMPLE_IVE_GMM2_TOTAL_FRM            700
#define SAMPLE_IVE_GMM2_FRM_500              500
#define SAMPLE_IVE_GMM2_SMALL_FREQ_REDU_FACTOR   0xFC00
#define SAMPLE_IVE_GMM2_BIG_FREQ_REDU_FACTOR     0xFFA0

typedef struct hiSAMPLE_IVE_GMM2_S {
    IVE_SRC_IMAGE_S astSrc[SAMPLE_IVE_GMM2_SRC_NUM];
    IVE_DST_IMAGE_S stFg;
    IVE_DST_IMAGE_S stBg;
    IVE_SRC_IMAGE_S stFactor;
    IVE_DST_IMAGE_S stMatchModelInfo;
    IVE_SRC_IMAGE_S stFgMask;
    IVE_SRC_IMAGE_S stLastImg;
    IVE_DST_IMAGE_S stDiffImg;
    IVE_DST_IMAGE_S stMagImg;
    IVE_DST_IMAGE_S stCurNormMag;
    IVE_DST_IMAGE_S stLastNormMag;
    IVE_DST_MEM_INFO_S stModel;
    IVE_DST_MEM_INFO_S stBlob;
    IVE_GMM2_CTRL_S stGmm2Ctrl;
    FILE *pFpSrc;
    FILE *pFpFg;
    FILE *pFpBg;
} SAMPLE_IVE_GMM2_S;

static SAMPLE_IVE_GMM2_S s_stGmm2;
static HI_BOOL s_bStopSignal = HI_FALSE;

static HI_VOID SAMPLE_IVE_Gmm2_Uninit(SAMPLE_IVE_GMM2_S *pstGmm2)
{
    HI_U16 i;
    for (i = 0; i < SAMPLE_IVE_GMM2_SRC_NUM; i++) { /* max pstGmm2->astSrc index: 2 */
        IVE_MMZ_FREE(pstGmm2->astSrc[i].au64PhyAddr[0], pstGmm2->astSrc[i].au64VirAddr[0]);
    }
    IVE_MMZ_FREE(pstGmm2->stFg.au64PhyAddr[0], pstGmm2->stFg.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stBg.au64PhyAddr[0], pstGmm2->stBg.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stFactor.au64PhyAddr[0], pstGmm2->stFactor.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stMatchModelInfo.au64PhyAddr[0], pstGmm2->stMatchModelInfo.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stFgMask.au64PhyAddr[0], pstGmm2->stFgMask.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stDiffImg.au64PhyAddr[0], pstGmm2->stDiffImg.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stMagImg.au64PhyAddr[0], pstGmm2->stMagImg.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stCurNormMag.au64PhyAddr[0], pstGmm2->stCurNormMag.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stLastNormMag.au64PhyAddr[0], pstGmm2->stLastNormMag.au64VirAddr[0]);
    IVE_MMZ_FREE(pstGmm2->stModel.u64PhyAddr, pstGmm2->stModel.u64VirAddr);
    IVE_MMZ_FREE(pstGmm2->stBlob.u64PhyAddr, pstGmm2->stBlob.u64VirAddr);

    IVE_CLOSE_FILE(pstGmm2->pFpSrc);
    IVE_CLOSE_FILE(pstGmm2->pFpFg);
    IVE_CLOSE_FILE(pstGmm2->pFpBg);
}

static HI_S32 SAMPLE_IVE_Gmm2_Init(SAMPLE_IVE_GMM2_S *pstGmm2, HI_U32 u32Width, HI_U32 u32Height,
    HI_CHAR *pchSrcFileName, HI_CHAR *pchFgFileName, HI_CHAR *pchBgFileName)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Size;
    HI_U16 i, j;
    HI_U8 *pu8Tmp = NULL;
    (HI_VOID)memset_s(pstGmm2, sizeof(SAMPLE_IVE_GMM2_S), 0, sizeof(SAMPLE_IVE_GMM2_S));

    for (i = 0; i < SAMPLE_IVE_GMM2_SRC_NUM; i++) { /* max pstGmm2->astSrc index: 2 */
        s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->astSrc[i]), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create src[%d] image failed!\n",
            s32Ret, i);
    }

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stFg), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create fg image failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stBg), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create bg image failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stFactor), IVE_IMAGE_TYPE_U16C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create factor image failed!\n", s32Ret);
    pu8Tmp = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstGmm2->stFactor.au64VirAddr[0]);
    for (i = 0; i < pstGmm2->stFactor.u32Height; i++) {
        for (j = 0; j < pstGmm2->stFactor.u32Width; j++) {
            pu8Tmp[SAMPLE_IVE_GMM2_FACTOR_OFFSET * j] = SAMPLE_IVE_GMM2_FACTOR_8; /* index: 2*j, value: 8 */
            pu8Tmp[SAMPLE_IVE_GMM2_FACTOR_OFFSET * j + 1] = SAMPLE_IVE_GMM2_FACTOR_4; /* index: 2*j+1, value: 4 */
        }
        pu8Tmp += pstGmm2->stFactor.au32Stride[0] * sizeof(HI_U16);
    }
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stMatchModelInfo), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create matchModelInfo image failed!\n",
        s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stFgMask), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create fgMask image failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stDiffImg), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create diffImg image failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stMagImg), IVE_IMAGE_TYPE_U16C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create magImg image failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stCurNormMag), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create curNormImg image failed!\n",
        s32Ret);
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstGmm2->stLastNormMag), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create lastNormImg image failed!\n",
        s32Ret);

    pstGmm2->stGmm2Ctrl.u16VarRate = SAMPLE_IVE_GMM2_VAR_RATE;
    pstGmm2->stGmm2Ctrl.u8ModelNum = SAMPLE_IVE_GMM2_MODEL_NUM_3; /* model num: 3 */
    pstGmm2->stGmm2Ctrl.u9q7MaxVar = SAMPLE_IVE_GMM2_MAX_VAR; /* max var: (16 * 16) << 7 */
    pstGmm2->stGmm2Ctrl.u9q7MinVar = SAMPLE_IVE_GMM2_MIN_VAR; /* min var: (8 * 8) << 7 */
    pstGmm2->stGmm2Ctrl.u8GlbSnsFactor = SAMPLE_IVE_GMM2_GLB_SNS_FACTOR; /* sensor factor: 8 */
    pstGmm2->stGmm2Ctrl.enSnsFactorMode = IVE_GMM2_SNS_FACTOR_MODE_PIX;
    pstGmm2->stGmm2Ctrl.u16FreqThr = SAMPLE_IVE_GMM2_FREQ_THR; /* frequency threshold: 12000 */
    pstGmm2->stGmm2Ctrl.u16FreqInitVal = SAMPLE_IVE_GMM2_FREQ_INIT_VAL; /* frequency init value: 20000 */
    pstGmm2->stGmm2Ctrl.u16FreqAddFactor = SAMPLE_IVE_GMM2_FREQ_ADD_FACTOR; /* frequency add factor: 0xEF */
    pstGmm2->stGmm2Ctrl.u16FreqReduFactor = SAMPLE_IVE_GMM2_FREQ_REDU_FACTOR; /* frequency redu factor: 0xFF00 */
    pstGmm2->stGmm2Ctrl.u16LifeThr = SAMPLE_IVE_GMM2_LIFE_THR; /* life threshold: 5000 */
    pstGmm2->stGmm2Ctrl.enLifeUpdateFactorMode = IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_GLB;

    u32Size = pstGmm2->stGmm2Ctrl.u8ModelNum * SAMPLE_IVE_GMM2_U8C1_MODLE_SIZE_UNIT *
        pstGmm2->astSrc[0].u32Width * pstGmm2->astSrc[0].u32Height;
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&pstGmm2->stModel, u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create model mem info failed!\n", s32Ret);
    (HI_VOID)memset_s(SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_VOID, pstGmm2->stModel.u64VirAddr),
        pstGmm2->stModel.u32Size, 0, pstGmm2->stModel.u32Size);
    u32Size = sizeof(IVE_CCBLOB_S);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&pstGmm2->stBlob, u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_INIT_FAIL, "Error(%#x),Create blob mem info failed!\n", s32Ret);
    s32Ret = HI_FAILURE;
    pstGmm2->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(pstGmm2->pFpSrc == NULL, GMM2_INIT_FAIL, "Error,Open file %s failed!\n", pchSrcFileName);
    pstGmm2->pFpFg = fopen(pchFgFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstGmm2->pFpFg == NULL, GMM2_INIT_FAIL, "Error,Open file %s failed!\n", pchFgFileName);
    pstGmm2->pFpBg = fopen(pchBgFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstGmm2->pFpBg == NULL, GMM2_INIT_FAIL, "Error,Open file %s failed!\n", pchBgFileName);
    s32Ret = HI_SUCCESS;

GMM2_INIT_FAIL:
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_IVE_Gmm2_Uninit(pstGmm2);
    }
    return s32Ret;
}

static HI_S32 SAMPLE_IVE_GenFgMask(IVE_SRC_IMAGE_S *pstFg, IVE_DST_IMAGE_S *pstFgMask)
{
    HI_S32 s32Ret;
    IVE_HANDLE IveHandle;
    IVE_THRESH_CTRL_S stCtrl;

    stCtrl.enMode = IVE_THRESH_MODE_BINARY;
    stCtrl.u8MinVal = SAMPLE_IVE_GMM2_GEN_FG_MASK_MIN_VAL;
    stCtrl.u8MaxVal = SAMPLE_IVE_GMM2_GEN_FG_MASK_MAX_VAL;
    stCtrl.u8LowThr = SAMPLE_IVE_GMM2_GEN_FG_MASK_LOW_THR; /* low threshold: 5 */
    s32Ret = HI_MPI_IVE_Thresh(&IveHandle, pstFg, pstFgMask, &stCtrl, HI_FALSE);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Thresh failed!\n", s32Ret);

    return s32Ret;
}

/* Reduce the factor gradually to the default value */
static HI_VOID SAMPLE_IVE_ReduFactor(IVE_IMAGE_S *pstFactor)
{
    HI_U16 i, j;
    HI_U8 *pu8Tmp = NULL;

    pu8Tmp = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstFactor->au64VirAddr[0]);
    for (i = 0; i < pstFactor->u32Height; i++) {
        for (j = 0; j < pstFactor->u32Width; j++) {
            pu8Tmp[j << 1] = SAMPLE_IVE_GMM2_MAX(SAMPLE_IVE_GMM2_FACTOR_8, pu8Tmp[i << 1] - 1);
            pu8Tmp[(i << 1) + 1] = SAMPLE_IVE_GMM2_MAX(SAMPLE_IVE_GMM2_FACTOR_8,
                pu8Tmp[(i << 1) + 1] - SAMPLE_IVE_GMM2_NUM_2);
        }
        pu8Tmp += pstFactor->au32Stride[0] * sizeof(HI_U16);
    }
}

/* Change factor by difference frame */
static HI_BOOL SAMPLE_IVE_ChgFactorByDiffFrm(IVE_SRC_IMAGE_S *pstSrc, IVE_SRC_IMAGE_S *pstLastImg,
    IVE_DST_IMAGE_S *pstDiffImg, IVE_DST_IMAGE_S *pstFactor)
{
    HI_S32 s32Ret;
    IVE_HANDLE IveHandle;
    HI_BOOL bFinish = HI_FALSE;
    HI_BOOL bBlock = HI_TRUE;
    HI_U16 i, j;
    HI_S32 s32AbsTh = SAMPLE_IVE_GMM2_ABS_THR_10;
    HI_U32 u32PointSum = 0;
    IVE_SUB_CTRL_S stSubCtrl;
    HI_U8 *pu8Tmp = NULL;
    HI_U8 *pu8Factor = NULL;

    stSubCtrl.enMode = IVE_SUB_MODE_ABS;
    s32Ret = HI_MPI_IVE_Sub(&IveHandle, pstSrc, pstLastImg, pstDiffImg, &stSubCtrl, HI_TRUE);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, HI_FALSE, "Error(%#x),HI_MPI_IVE_Sub failed!\n", s32Ret);

    // Wait task finish
    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
        usleep(IVE_QUERY_SLEEP_TIME); /* sleep 100 us */
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }

    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, HI_FALSE, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);

    pu8Tmp = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstDiffImg->au64VirAddr[0]);
    for (i = 0; i < pstSrc->u32Height; i++) {
        for (j = 0; j < pstSrc->u32Width; j++) {
            u32PointSum += pu8Tmp[j] > s32AbsTh;
        }
        pu8Tmp += pstDiffImg->au32Stride[0];
    }

    if ((u32PointSum * SAMPLE_IVE_GMM2_TIMES_3) > (pstSrc->u32Width * pstSrc->u32Height * SAMPLE_IVE_GMM2_TIMES_2)) {
        pu8Tmp = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstDiffImg->au64VirAddr[0]);
        pu8Factor = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstFactor->au64VirAddr[0]);
        for (i = 0; i < pstSrc->u32Height; i++) {
            for (j = 0; j < pstSrc->u32Width; j++) {
                if (pu8Tmp[j] > s32AbsTh) {
                    pu8Factor[j << 1] = SAMPLE_IVE_GMM2_FACTOR_20; /* set 20 */
                    pu8Factor[(j << 1) + 1] = SAMPLE_IVE_GMM2_FACTOR_200; /* set 200 */
                }
            }
            pu8Tmp += pstDiffImg->au32Stride[0];
            pu8Factor += pstFactor->au32Stride[0] * sizeof(HI_U16);
        }

        return HI_TRUE;
    }

    return HI_FALSE;
}

/* Change factor by gradient */
static HI_S32 SAMPLE_IVE_ChgFactorByGrad(IVE_SRC_IMAGE_S *pstFgMask, IVE_SRC_IMAGE_S *pstSrc,
    IVE_SRC_IMAGE_S *pstLastImg, IVE_SRC_IMAGE_S *pstLastNormMag, IVE_DST_IMAGE_S *pstMagImg,
    IVE_DST_IMAGE_S *pstCurNormMag, IVE_DST_MEM_INFO_S *pstBlob, IVE_DST_IMAGE_S *pstDiffImg,
    IVE_DST_IMAGE_S *pstFactor)
{
    HI_S32 s32Ret;
    IVE_HANDLE IveHandle;
    HI_BOOL bFinish = HI_FALSE;
    HI_BOOL bBlock = HI_TRUE;
    HI_U16 i, j, k;
    HI_U16 u16Top;
    HI_U16 u16Left;
    HI_U16 u16Right;
    HI_U16 u16Bottom;
    HI_U8 *pu8FgCur = NULL;
    HI_U8 *pu8GradDiff = NULL;
    HI_U8 *pu8Factor = NULL;
    HI_S32 s32FgSum = 0;
    HI_S32 s32GradSum = 0;
    /* mag filter mask */
    HI_S8 au8Mask[IVE_MASK_NUM_25] = {
        0, 0, 0, 0, 0,
        0, -1, 0, 1, 0,
        0, -2, 0, 2, 0,
        0, -1, 0, 1, 0,
        0, 0, 0, 0, 0};
    IVE_CCL_CTRL_S stCclCtrl;
    IVE_CCBLOB_S *pstCclBlob = NULL;
    IVE_MAG_AND_ANG_CTRL_S stMagAndAngCtrl;
    IVE_16BIT_TO_8BIT_CTRL_S st16To8Ctrl;
    IVE_SUB_CTRL_S stSubCtrl;

    stCclCtrl.enMode = IVE_CCL_MODE_8C;
    stCclCtrl.u16Step = SAMPLE_IVE_GMM2_CCL_STEP; /* set 2 */
    stCclCtrl.u16InitAreaThr = SAMPLE_IVE_GMM2_CCL_INIT_AREA_THR; /* set 4 */
    s32Ret = HI_MPI_IVE_CCL(&IveHandle, pstFgMask, pstBlob, &stCclCtrl, HI_TRUE);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_CCL failed!\n", s32Ret);

    // Wait task finish
    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
        usleep(IVE_QUERY_SLEEP_TIME); /* sleep 100 us */
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);

    pstCclBlob = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(IVE_CCBLOB_S, pstBlob->u64VirAddr);
    if (pstCclBlob->u8RegionNum > 0) {
        // Calc the gradient difference of the current image and the last image
        (HI_VOID)memcpy_s(stMagAndAngCtrl.as8Mask, sizeof(au8Mask), au8Mask, sizeof(au8Mask));
        stMagAndAngCtrl.u16Thr = 0;
        stMagAndAngCtrl.enOutCtrl = IVE_MAG_AND_ANG_OUT_CTRL_MAG;
        s32Ret = HI_MPI_IVE_MagAndAng(&IveHandle, pstSrc, pstMagImg, HI_NULL, &stMagAndAngCtrl, HI_FALSE);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_MagAndAng failed!\n", s32Ret);

        st16To8Ctrl.s8Bias = 0;
        st16To8Ctrl.enMode = IVE_16BIT_TO_8BIT_MODE_U16_TO_U8;
        st16To8Ctrl.u8Numerator = SAMPLE_IVE_GMM2_16_TO_8_NUMERATOR; /* numerator: 255 */
        st16To8Ctrl.u16Denominator = SAMPLE_IVE_GMM2_16_TO_8_DENOMINATOR; /* Denominator: 255*4 */
        s32Ret = HI_MPI_IVE_16BitTo8Bit(&IveHandle, pstMagImg, pstCurNormMag, &st16To8Ctrl, HI_FALSE);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_16BitTo8Bit failed!\n", s32Ret);
        s32Ret = HI_MPI_IVE_MagAndAng(&IveHandle, pstLastImg, pstMagImg, HI_NULL, &stMagAndAngCtrl, HI_FALSE);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_MagAndAng failed!\n", s32Ret);
        s32Ret = HI_MPI_IVE_16BitTo8Bit(&IveHandle, pstMagImg, pstLastNormMag, &st16To8Ctrl, HI_FALSE);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_16BitTo8Bit failed!\n", s32Ret);
        stSubCtrl.enMode = IVE_SUB_MODE_ABS;
        s32Ret = HI_MPI_IVE_Sub(&IveHandle, pstCurNormMag, pstLastNormMag, pstDiffImg, &stSubCtrl, HI_TRUE);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Sub failed!\n", s32Ret);
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
        while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
            usleep(IVE_QUERY_SLEEP_TIME); /* sleep 100 us */
            s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
        }

        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);
    }

    // for each blob, analyze the gradient change
    for (k = 0; k < IVE_MAX_REGION_NUM; k++) {
        if (pstCclBlob->astRegion[k].u32Area == 0) {
            continue;
        }

        u16Top = pstCclBlob->astRegion[k].u16Top;
        u16Left = pstCclBlob->astRegion[k].u16Left;
        u16Right = pstCclBlob->astRegion[k].u16Right;
        u16Bottom = pstCclBlob->astRegion[k].u16Bottom;
        if ((u16Right - u16Left) * (u16Bottom - u16Top) < SAMPLE_IVE_GMM2_MAX_AREA) { /* max area: 60 */
            continue;
        }

        s32FgSum = 0;
        s32GradSum = 0;
        pu8FgCur =
            (SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstFgMask->au64VirAddr[0])) + u16Top * pstFgMask->au32Stride[0];
        pu8GradDiff = (SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstDiffImg->au64VirAddr[0])) +
            u16Top * pstDiffImg->au32Stride[0];
        for (i = u16Top; i < u16Bottom; i++) {
            for (j = u16Left; j < u16Right; j++) {
                if (pu8FgCur[j]) {
                    s32FgSum++;
                    if (pu8GradDiff[j]) {
                        s32GradSum++;
                    }
                }
            }

            pu8FgCur += pstFgMask->au32Stride[0];
            pu8GradDiff += pstDiffImg->au32Stride[0];
        }
        /* s32GradSum: 100, s32GradSum * 100: s32FgSum */
        if ((s32GradSum < SAMPLE_IVE_GMM2_MIN_GRAD_SUM) || (s32GradSum * SAMPLE_IVE_GMM2_TIMES_100 < s32FgSum)) {
            pu8Factor = (SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstFactor->au64VirAddr[0])) +
                SAMPLE_IVE_GMM2_FACTOR_OFFSET * u16Top * pstFactor->au32Stride[0];
            pu8FgCur = (SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8, pstFgMask->au64VirAddr[0])) +
                u16Top * pstFgMask->au32Stride[0];
            for (i = u16Top; i < u16Bottom; i++) {
                for (j = u16Left; j < u16Right; j++) {
                    if (pu8FgCur[j]) {
                        pu8Factor[j << 1] = SAMPLE_IVE_GMM2_FACTOR_16; /* fator[j<<1] value: 16 */
                        pu8Factor[(j << 1) + 1] = SAMPLE_IVE_GMM2_FACTOR_200; /* fator[(j << 1) + 1] value: 200 */
                    }
                }
                pu8FgCur += pstFgMask->au32Stride[0];
                pu8Factor += pstFactor->au32Stride[0] * sizeof(HI_U16);
            }
        }
    }

    return HI_SUCCESS;
}

static HI_VOID SAMPLE_IVE_AdjustmentFactor(IVE_SRC_IMAGE_S *pstFactor, IVE_SRC_IMAGE_S *pstFgMask,
    IVE_SRC_IMAGE_S *pstSrc, IVE_SRC_IMAGE_S *pstLastImg, IVE_DST_IMAGE_S *pstDiffImg, IVE_DST_IMAGE_S *pstMagImg,
    IVE_DST_IMAGE_S *pstCurNormMag, IVE_DST_IMAGE_S *pstLastNormMag, IVE_DST_MEM_INFO_S *pstBlob)
{
    HI_BOOL bChg = HI_FALSE;
    // First, reduce the factor gradually to the default value------------------------------
    SAMPLE_IVE_ReduFactor(pstFactor);

    // Second, analyze the frame difference-------------------------------------------------
    // When the number of changed pixels is more than the threshold, there maybe a light switch.
    // And then, we should set a big factor to adapt to it quickly.
    bChg = SAMPLE_IVE_ChgFactorByDiffFrm(pstSrc, pstLastImg, pstDiffImg, pstFactor);
    if (HI_TRUE == bChg) {
        return;
    }

    // Third, analyze the gradient for foreground blobs------------------------------------
    // When gradient change of a foreground blob is very small, it maybe a local illumination change,
    // a ghost, or a static object.
    // Here we try to reduce the influence by a local illumination change or a ghost only.
    (HI_VOID)SAMPLE_IVE_ChgFactorByGrad(pstFgMask, pstSrc, pstLastImg, pstLastNormMag, pstMagImg, pstCurNormMag,
        pstBlob, pstDiffImg, pstFactor);
}

static HI_S32 SAMPLE_IVE_Gmm2Proc(SAMPLE_IVE_GMM2_S *pstGmm2)
{
    HI_S32 s32Ret = HI_FAILURE;
    IVE_HANDLE IveHandle;
    HI_BOOL bFinish = HI_FALSE;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bInstant = HI_TRUE;
    HI_U32 u32FrmNum;
    HI_S32 s32CurIdx = 0;
    HI_U32 u32TotalFrm = SAMPLE_IVE_GMM2_TOTAL_FRM;

    for (u32FrmNum = 1; u32FrmNum < u32TotalFrm && s_bStopSignal == HI_FALSE; u32FrmNum++) {
        SAMPLE_PRT("Proc Frame %d/%d\n", u32FrmNum, u32TotalFrm);
        s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstGmm2->astSrc[s32CurIdx]), pstGmm2->pFpSrc);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);

        // To building a stable background model quickly at the begin, some parameters are set specially.
        if (pstGmm2->stGmm2Ctrl.u8ModelNum == 1) {
            // If the parameter u8ModelNum is set to 1, the parameter u16FreqReduFactor
            // is usually set to a small value at the first N frames. Here, N = 500.
            pstGmm2->stGmm2Ctrl.u16FreqReduFactor = (u32FrmNum >= SAMPLE_IVE_GMM2_FRM_500) ?
                SAMPLE_IVE_GMM2_BIG_FREQ_REDU_FACTOR : SAMPLE_IVE_GMM2_SMALL_FREQ_REDU_FACTOR;
        } else {
            // If the parameter u8ModelNum is more than 1, the global life mode should be used at the first N frames,
            // and the parameter u16GlbLifeUpdateFactor is usually set to a big value. Here, N = 500.
            if (u32FrmNum >= 500) {
                pstGmm2->stGmm2Ctrl.enLifeUpdateFactorMode = IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_PIX;
            } else {
                pstGmm2->stGmm2Ctrl.u16GlbLifeUpdateFactor = 0xFFFF / u32FrmNum;
            }
        }

        // GMM2
        s32Ret = HI_MPI_IVE_GMM2(&IveHandle, &pstGmm2->astSrc[s32CurIdx], &pstGmm2->stFactor, &pstGmm2->stFg,
            &pstGmm2->stBg, &pstGmm2->stMatchModelInfo, &pstGmm2->stModel, &pstGmm2->stGmm2Ctrl, bInstant);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_GMM2 failed!\n", s32Ret);

        // factor adjustment
        if (u32FrmNum > 1) {
            s32Ret = SAMPLE_IVE_GenFgMask(&(pstGmm2->stFg), &(pstGmm2->stFgMask));
            SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),SAMPLE_IVE_GenFgMask failed!\n", s32Ret);

            SAMPLE_IVE_AdjustmentFactor(&(pstGmm2->stFactor), &(pstGmm2->stFgMask), &(pstGmm2->astSrc[s32CurIdx]),
                &(pstGmm2->astSrc[1 - s32CurIdx]), &(pstGmm2->stDiffImg), &(pstGmm2->stMagImg),
                &(pstGmm2->stCurNormMag), &(pstGmm2->stLastNormMag), &(pstGmm2->stBlob));
        } else {
            s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
            while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
                usleep(IVE_QUERY_SLEEP_TIME); /* sleep 100 us */
                s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
            }
            SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);
        }

        s32Ret = SAMPLE_COMM_IVE_WriteFile(&(pstGmm2->stFg), pstGmm2->pFpFg);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Write fg file failed!\n", s32Ret);
        s32Ret = SAMPLE_COMM_IVE_WriteFile(&(pstGmm2->stBg), pstGmm2->pFpBg);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Write bg file failed!\n", s32Ret);
        // change cur image to last image
        s32CurIdx = 1 - s32CurIdx;
    }

    return s32Ret;
}

static HI_VOID SAMPLE_IVE_GMM2Stop(HI_VOID)
{
    SAMPLE_IVE_Gmm2_Uninit(&s_stGmm2);
    (HI_VOID)memset_s(&s_stGmm2, sizeof(s_stGmm2), 0, sizeof(s_stGmm2));
    SAMPLE_COMM_IVE_IveMpiExit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

HI_VOID SAMPLE_IVE_Gmm2(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32Width = IVE_CIF_WIDTH;
    HI_U32 u32Height = IVE_CIF_HEIGHT;
    HI_CHAR *pchSrcFileName = "./data/input/gmm2/gmm2_352x288_sp400_frm1000.yuv";
    HI_CHAR achSrcFileName[PATH_MAX] = {0};
    HI_CHAR achFgFileName[PATH_MAX] = {0};
    HI_CHAR achBgFileName[PATH_MAX] = {0};

    s_bStopSignal = HI_FALSE;
    SAMPLE_CHECK_EXPR_RET_VOID(
        (strlen(pchSrcFileName) > PATH_MAX) || (realpath(pchSrcFileName, achSrcFileName) == NULL), "invalid path!\n");
    SAMPLE_CHECK_EXPR_RET_VOID(realpath("./data/output/gmm2", achFgFileName) == NULL, "invalid path");
    s32Ret = strcat_s(achFgFileName, PATH_MAX, "/fg_352x288_sp400.yuv");
    SAMPLE_CHECK_EXPR_RET_VOID(s32Ret < 0, "invali param");
    SAMPLE_CHECK_EXPR_RET_VOID(strlen(achFgFileName) > PATH_MAX, "fg file path is invalid!\n");
    SAMPLE_CHECK_EXPR_RET_VOID(realpath("./data/output/gmm2", achBgFileName) == NULL, "invalid path");
    s32Ret = strcat_s(achBgFileName, PATH_MAX, "/bg_352x288_sp400.yuv");
    SAMPLE_CHECK_EXPR_RET_VOID(s32Ret < 0, "invali param");
    SAMPLE_CHECK_EXPR_RET_VOID(strlen(achBgFileName) > PATH_MAX, "bg file path is invalid!\n");
    (HI_VOID)memset_s(&s_stGmm2, sizeof(s_stGmm2), 0, sizeof(s_stGmm2));
    SAMPLE_COMM_IVE_CheckIveMpiInit();
    s32Ret = SAMPLE_IVE_Gmm2_Init(&s_stGmm2, u32Width, u32Height, achSrcFileName, achFgFileName, achBgFileName);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, GMM2_FAIL, "Error(%#x),SAMPLE_IVE_Gmm2_Init failed!\n", s32Ret);
    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_GMM2Stop();
        return;
    }

    s32Ret = SAMPLE_IVE_Gmm2Proc(&s_stGmm2);
    if (s32Ret == HI_SUCCESS) {
        SAMPLE_PRT("Process success!\n");
    }

    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_GMM2Stop();
        return;
    }
    s_bStopSignal = HI_TRUE;

    SAMPLE_IVE_Gmm2_Uninit(&s_stGmm2);
    (HI_VOID)memset_s(&s_stGmm2, sizeof(s_stGmm2), 0, sizeof(s_stGmm2));

GMM2_FAIL:
    s_bStopSignal = HI_TRUE;
    SAMPLE_COMM_IVE_IveMpiExit();
}

HI_VOID SAMPLE_IVE_Gmm2_HandleSig(HI_VOID)
{
    s_bStopSignal = HI_TRUE;
}
