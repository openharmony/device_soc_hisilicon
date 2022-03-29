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

#define SAMPLE_IVE_PSP_WIDTH_250         250
#define SAMPLE_IVE_PSP_HEIGHT_250        250
#define SAMPLE_IVE_PSP_WIDTH_96          96
#define SAMPLE_IVE_PSP_HEIGHT_112        112
#define SAMPLE_IVE_PSP_POINT_PAIR_NUM_68 68
#define SAMPLE_IVE_PSP_POINT_PAIR_NUM_5  5
#define SAMPLE_IVE_PSP_POINT_FIX_BIT     2
#define SAMPLE_IVE_PSP_IDX_1             1
#define SAMPLE_IVE_PSP_IDX_2             2
#define SAMPLE_IVE_PSP_IDX_3             3
#define SAMPLE_IVE_PSP_LAND_MASK_OFFSET  4
#define SAMPLE_IVE_PSP_MAX_ROI           64
#define SAMPLE_IVE_PSP_MAX_POINT_PAIR    64

typedef struct hiSAMPLE_IVE_PERSP_TRANS_S {
    IVE_SRC_IMAGE_S stSrc;
    IVE_RECT_U32_S astRoi[SAMPLE_IVE_PSP_MAX_ROI];
    HI_U16 u16RoiNum;
    IVE_DST_IMAGE_S astDst[SAMPLE_IVE_PSP_MAX_ROI];
    IVE_SRC_MEM_INFO_S astPointPair[SAMPLE_IVE_PSP_MAX_POINT_PAIR];
    IVE_PERSP_TRANS_CTRL_S stPerspTransCtrl;

    FILE *pFpSrc;
    FILE *pFpDst;
} SAMPLE_IVE_PERSP_TRANS_S;

static SAMPLE_IVE_PERSP_TRANS_S s_stPerspTrans;
static HI_BOOL s_bStopSignal = HI_FALSE;

static HI_VOID SAMPLE_IVE_PerspTrans_Uninit(SAMPLE_IVE_PERSP_TRANS_S *pstPerspTrans)
{
    HI_U16 i;

    IVE_MMZ_FREE(pstPerspTrans->stSrc.au64PhyAddr[0], pstPerspTrans->stSrc.au64VirAddr[0]);

    for (i = 0; i < pstPerspTrans->u16RoiNum; i++) {
        IVE_MMZ_FREE(pstPerspTrans->astDst[i].au64PhyAddr[0], pstPerspTrans->astDst[i].au64VirAddr[0]);
        IVE_MMZ_FREE(pstPerspTrans->astPointPair[i].u64PhyAddr, pstPerspTrans->astPointPair[i].u64VirAddr);
    }

    IVE_CLOSE_FILE(pstPerspTrans->pFpSrc);
    IVE_CLOSE_FILE(pstPerspTrans->pFpDst);
}

static HI_S32 SAMPLE_IVE_PerspTrans_Init(SAMPLE_IVE_PERSP_TRANS_S *pstPerspTrans, HI_U32 u32SrcWidth,
    HI_U32 u32SrcHeight, HI_U32 u32DstWidth, HI_U32 u32DstHeight, IVE_RECT_U32_S astRoi[], HI_U16 u16RoiNum,
    HI_U16 u16MaxPointPairNum, HI_CHAR *pchSrcFileName, HI_CHAR *pchDstFileName)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Size;
    HI_U16 i, j;
    HI_U16 au16LandMark[] = {107, 109, 30, 52,
                             149, 117, 66, 52,
                             123, 135, 48, 72,
                             99,  157, 34, 92,
                             144, 157, 63, 92
                            };
    IVE_PERSP_TRANS_POINT_PAIR_S *pstTmp = NULL;

    s32Ret =
        memcpy_s(pstPerspTrans->astRoi, sizeof(IVE_RECT_U32_S) * u16RoiNum, astRoi, sizeof(IVE_RECT_U32_S) * u16RoiNum);
    SAMPLE_CHECK_EXPR_RET(s32Ret != EOK, HI_ERR_IVE_ILLEGAL_PARAM, "memcpy_s failed!\n");

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstPerspTrans->stSrc), IVE_IMAGE_TYPE_YUV420SP, u32SrcWidth, u32SrcHeight);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, PERSP_TRANS_INIT_FAIL, "Error(%#x),Create src image failed!\n",
        s32Ret);

    for (i = 0; i < u16RoiNum; i++) {
        s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstPerspTrans->astDst[i]), IVE_IMAGE_TYPE_YUV420SP, u32DstWidth,
            u32DstHeight);
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, PERSP_TRANS_INIT_FAIL, "Error(%#x),Create src image failed!\n",
            s32Ret);
    }

    u32Size = sizeof(IVE_PERSP_TRANS_POINT_PAIR_S) * u16MaxPointPairNum;
    for (i = 0; i < u16RoiNum; i++) {
        s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstPerspTrans->astPointPair[i]), u32Size);
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, PERSP_TRANS_INIT_FAIL, "Error(%#x),Create src image failed!\n",
            s32Ret);
    }

    pstPerspTrans->stPerspTransCtrl.enAlgMode = IVE_PERSP_TRANS_ALG_MODE_AFFINE;
    pstPerspTrans->stPerspTransCtrl.enCscMode = IVE_PERSP_TRANS_CSC_MODE_NONE;
    pstPerspTrans->stPerspTransCtrl.u16RoiNum = u16RoiNum;
    pstPerspTrans->stPerspTransCtrl.u16PointPairNum = SAMPLE_IVE_PSP_POINT_PAIR_NUM_5; /* point pair num: 5 */
    pstPerspTrans->u16RoiNum = u16RoiNum;

    for (i = 0; i < u16RoiNum; i++) {
        pstTmp = (IVE_PERSP_TRANS_POINT_PAIR_S *)(HI_UINTPTR_T)pstPerspTrans->astPointPair[i].u64VirAddr;
        for (j = 0; j < pstPerspTrans->stPerspTransCtrl.u16PointPairNum; j++) {
            /* x: au16LandMark[j * 4] << 2 */
            pstTmp->stSrcPoint.u14q2X = au16LandMark[j * SAMPLE_IVE_PSP_LAND_MASK_OFFSET] <<
                SAMPLE_IVE_PSP_POINT_FIX_BIT;
            /* y: au16LandMark[j * 4 + 1] << 2 */
            pstTmp->stSrcPoint.u14q2Y = au16LandMark[j * SAMPLE_IVE_PSP_LAND_MASK_OFFSET + SAMPLE_IVE_PSP_IDX_1] <<
                SAMPLE_IVE_PSP_POINT_FIX_BIT;
            /* x:  au16LandMark[j * 4 + 2] << 2 */
            pstTmp->stDstPoint.u14q2X = au16LandMark[j * SAMPLE_IVE_PSP_LAND_MASK_OFFSET + SAMPLE_IVE_PSP_IDX_2] <<
                SAMPLE_IVE_PSP_POINT_FIX_BIT;
            /* y: au16LandMark[j * 4 + 3] << 2 */
            pstTmp->stDstPoint.u14q2Y = au16LandMark[j * SAMPLE_IVE_PSP_LAND_MASK_OFFSET + SAMPLE_IVE_PSP_IDX_3] <<
                SAMPLE_IVE_PSP_POINT_FIX_BIT;

            pstTmp++;
        }
    }

    s32Ret = HI_FAILURE;
    pstPerspTrans->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(pstPerspTrans->pFpSrc == NULL, PERSP_TRANS_INIT_FAIL, "Error,Open file %s failed!\n",
        pchSrcFileName);

    pstPerspTrans->pFpDst = fopen(pchDstFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstPerspTrans->pFpDst == NULL, PERSP_TRANS_INIT_FAIL, "Error,Open file %s failed!\n",
        pchDstFileName);
    s32Ret = HI_SUCCESS;

    return s32Ret;

PERSP_TRANS_INIT_FAIL:
    SAMPLE_IVE_PerspTrans_Uninit(pstPerspTrans);

    return s32Ret;
}

static HI_S32 SAMPLE_IVE_PerspTransProc(SAMPLE_IVE_PERSP_TRANS_S *pstPerspTrans)
{
    HI_S32 s32Ret = HI_FAILURE;
    IVE_HANDLE IveHandle;
    HI_BOOL bFinish = HI_FALSE;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bInstant = HI_TRUE;

    s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstPerspTrans->stSrc), pstPerspTrans->pFpSrc);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_PerspTrans(&IveHandle, &pstPerspTrans->stSrc, pstPerspTrans->astRoi,
        pstPerspTrans->astPointPair, pstPerspTrans->astDst, &pstPerspTrans->stPerspTransCtrl, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_PerspTrans failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
        usleep(IVE_QUERY_SLEEP_TIME);
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstPerspTrans->astDst[0], pstPerspTrans->pFpDst);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);

    return s32Ret;
}

static HI_VOID SAMPLE_IVE_PerspTransStop(HI_VOID)
{
    SAMPLE_IVE_PerspTrans_Uninit(&s_stPerspTrans);
    (HI_VOID)memset_s(&s_stPerspTrans, sizeof(s_stPerspTrans), 0, sizeof(s_stPerspTrans));
    SAMPLE_COMM_IVE_IveMpiExit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}
HI_VOID SAMPLE_IVE_PerspTrans(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32SrcWidth = SAMPLE_IVE_PSP_WIDTH_250;
    HI_U32 u32SrcHeight = SAMPLE_IVE_PSP_HEIGHT_250;
    HI_U32 u32DstWidth = SAMPLE_IVE_PSP_WIDTH_96;
    HI_U32 u32DstHeight = SAMPLE_IVE_PSP_HEIGHT_112;
    HI_CHAR *pchSrcFileName = "./data/input/psp/src/Amelia_Vega_250x250_420sp.yuv";
    HI_CHAR                achDstFileName[PATH_MAX] = {0};
    HI_CHAR                achSrcFileName[PATH_MAX] = {0};
    IVE_RECT_U32_S astRoi[2] = {0};
    HI_U16 u16RoiNum = 1;
    HI_U16 u16MaxPointPairNum = SAMPLE_IVE_PSP_POINT_PAIR_NUM_68;
    s_bStopSignal = HI_FALSE;

    (HI_VOID)memset_s(&s_stPerspTrans, sizeof(s_stPerspTrans), 0, sizeof(s_stPerspTrans));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    SAMPLE_CHECK_EXPR_RET_VOID(realpath(pchSrcFileName, achSrcFileName) == NULL, "invalid path");
    SAMPLE_CHECK_EXPR_RET_VOID(strlen(achSrcFileName) > PATH_MAX, "psp src file path is invalid!\n");

    SAMPLE_CHECK_EXPR_RET_VOID(realpath("./data/output/psp", achDstFileName) == NULL, "invalid path");
    s32Ret = strcat_s(achDstFileName, PATH_MAX, "/Amelia_Vega_Affine_96x112_420sp.yuv");
    SAMPLE_CHECK_EXPR_RET_VOID(s32Ret < 0, "invali param");
    SAMPLE_CHECK_EXPR_RET_VOID(strlen(achDstFileName) > PATH_MAX, "psp dst file path is invalid!\n");

    astRoi[0].u32X = 0;
    astRoi[0].u32Y = 0;
    astRoi[0].u32Width = SAMPLE_IVE_PSP_WIDTH_250;
    astRoi[0].u32Height = SAMPLE_IVE_PSP_HEIGHT_250;
    s32Ret = SAMPLE_IVE_PerspTrans_Init(&s_stPerspTrans, u32SrcWidth, u32SrcHeight, u32DstWidth, u32DstHeight, astRoi,
        u16RoiNum, u16MaxPointPairNum, achSrcFileName, achDstFileName);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, PERSP_TRANS_FAIL, "Error(%#x),SAMPLE_IVE_PerspTrans_Init failed!\n",
        s32Ret);

    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_PerspTransStop();
        return;
    }

    s32Ret = SAMPLE_IVE_PerspTransProc(&s_stPerspTrans);
    if (s32Ret == HI_SUCCESS) {
        SAMPLE_PRT("Process success!\n");
    }
    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_PerspTransStop();
        return;
    }
    SAMPLE_IVE_PerspTrans_Uninit(&s_stPerspTrans);
    (HI_VOID)memset_s(&s_stPerspTrans, sizeof(s_stPerspTrans), 0, sizeof(s_stPerspTrans));

PERSP_TRANS_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}

/* function : PerspTrans sample signal handle */
HI_VOID SAMPLE_IVE_PerspTrans_HandleSig(HI_VOID)
{
    s_bStopSignal = HI_TRUE;
}
