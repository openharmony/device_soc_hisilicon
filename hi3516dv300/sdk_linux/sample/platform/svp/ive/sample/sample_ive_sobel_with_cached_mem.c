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

typedef struct hiSAMPLE_IVE_SOBEL_S {
    IVE_SRC_IMAGE_S stSrc1;
    IVE_SRC_IMAGE_S stSrc2;
    IVE_DST_IMAGE_S stDstH1;
    IVE_DST_IMAGE_S stDstH2;
    IVE_DST_IMAGE_S stDstV1;
    IVE_DST_IMAGE_S stDstV2;
    IVE_SOBEL_CTRL_S stSobelCtrl;
    FILE *pFpSrc;
    FILE *pFpDstH1;
    FILE *pFpDstH2;
    FILE *pFpDstV1;
    FILE *pFpDstV2;
} SAMPLE_IVE_SOBEL_S;

static HI_BOOL s_bFlushCached = HI_TRUE;
static SAMPLE_IVE_SOBEL_S s_stSobel;
static HI_BOOL s_bStopSignal = HI_FALSE;

/* function : show Sobel uninit */
static HI_VOID SAMPLE_IVE_Sobel_Uninit(SAMPLE_IVE_SOBEL_S *pstSobel)
{
    IVE_MMZ_FREE(pstSobel->stSrc1.au64PhyAddr[0], pstSobel->stSrc1.au64VirAddr[0]);
    IVE_MMZ_FREE(pstSobel->stSrc2.au64PhyAddr[0], pstSobel->stSrc2.au64VirAddr[0]);
    IVE_MMZ_FREE(pstSobel->stDstH1.au64PhyAddr[0], pstSobel->stDstH1.au64VirAddr[0]);
    IVE_MMZ_FREE(pstSobel->stDstH2.au64PhyAddr[0], pstSobel->stDstH2.au64VirAddr[0]);
    IVE_MMZ_FREE(pstSobel->stDstV1.au64PhyAddr[0], pstSobel->stDstV1.au64VirAddr[0]);
    IVE_MMZ_FREE(pstSobel->stDstV2.au64PhyAddr[0], pstSobel->stDstV2.au64VirAddr[0]);

    IVE_CLOSE_FILE(pstSobel->pFpSrc);
    IVE_CLOSE_FILE(pstSobel->pFpDstH1);
    IVE_CLOSE_FILE(pstSobel->pFpDstH2);
    IVE_CLOSE_FILE(pstSobel->pFpDstV1);
    IVE_CLOSE_FILE(pstSobel->pFpDstV2);
}
/* function : show Sobel init */
static HI_S32 SAMPLE_IVE_Sobel_Init(SAMPLE_IVE_SOBEL_S *pstSobel, HI_CHAR *pchSrcFileName, HI_U32 u32Width,
    HI_U32 u32Height)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR path[PATH_MAX] = {0};
    HI_CHAR *file = "./data/output/sobel";
    HI_S8 as8Mask[IVE_MASK_NUM_25] = {
        0, 0, 0, 0, 0,
        0, -1, 0, 1, 0,
        0, -2, 0, 2, 0,
        0, -1, 0, 1, 0,
        0, 0, 0, 0, 0
    };

    (HI_VOID)memset_s(pstSobel, sizeof(SAMPLE_IVE_SOBEL_S), 0, sizeof(SAMPLE_IVE_SOBEL_S));

    s32Ret = SAMPLE_COMM_IVE_CreateImageByCached(&(pstSobel->stSrc1), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, SOBEL_INIT_FAIL, "Error(%#x),Create src1 image failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImageByCached(&(pstSobel->stSrc2), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, SOBEL_INIT_FAIL, "Error(%#x),Create src2 image failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstSobel->stDstH1), IVE_IMAGE_TYPE_S16C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, SOBEL_INIT_FAIL, "Error(%#x),Create dstH1 image failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstSobel->stDstH2), IVE_IMAGE_TYPE_S16C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, SOBEL_INIT_FAIL, "Error(%#x),Create dstH2 image failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstSobel->stDstV1), IVE_IMAGE_TYPE_S16C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, SOBEL_INIT_FAIL, "Error(%#x),Create dstV1 image failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstSobel->stDstV2), IVE_IMAGE_TYPE_S16C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, SOBEL_INIT_FAIL, "Error(%#x),Create dstV2 image failed!\n", s32Ret);

    pstSobel->stSobelCtrl.enOutCtrl = IVE_SOBEL_OUT_CTRL_BOTH;
    (HI_VOID)memcpy_s(pstSobel->stSobelCtrl.as8Mask, sizeof(as8Mask), as8Mask, sizeof(as8Mask));
    s32Ret = HI_FAILURE;
    pstSobel->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(pstSobel->pFpSrc == HI_NULL, SOBEL_INIT_FAIL, "Error,Open file %s failed!\n",
        pchSrcFileName);

    SAMPLE_CHECK_EXPR_GOTO(realpath(file, path) == NULL, SOBEL_INIT_FAIL, "invalid path!\n");
    s32Ret = strcat_s(path, PATH_MAX, "/sobelh1.dat");
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != EOK, SOBEL_INIT_FAIL, "stract_s failed!\n");
    s32Ret = HI_FAILURE;
    pstSobel->pFpDstH1 = fopen(path, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstSobel->pFpDstH1 == HI_NULL, SOBEL_INIT_FAIL, "Error,Open file %s failed!\n", path);

    SAMPLE_CHECK_EXPR_GOTO(realpath(file, path) == NULL, SOBEL_INIT_FAIL, "invalid path!\n");
    s32Ret = strcat_s(path, PATH_MAX, "/sobelh2.dat");
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != EOK, SOBEL_INIT_FAIL, "stract_s failed!\n");
    s32Ret = HI_FAILURE;
    pstSobel->pFpDstH2 = fopen(path, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstSobel->pFpDstH2 == HI_NULL, SOBEL_INIT_FAIL, "Error,Open file %s failed!\n", path);
    SAMPLE_CHECK_EXPR_GOTO(realpath(file, path) == NULL, SOBEL_INIT_FAIL, "invalid path!\n");
    s32Ret = strcat_s(path, PATH_MAX, "/sobelv1.dat");
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != EOK, SOBEL_INIT_FAIL, "stract_s failed!\n");
    s32Ret = HI_FAILURE;
    pstSobel->pFpDstV1 = fopen(path, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstSobel->pFpDstV1 == HI_NULL, SOBEL_INIT_FAIL, "Error,Open file %s failed!\n", path);
    SAMPLE_CHECK_EXPR_GOTO(realpath(file, path) == NULL, SOBEL_INIT_FAIL, "invalid path!\n");
    s32Ret = strcat_s(path, PATH_MAX, "/sobelv2.dat");
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != EOK, SOBEL_INIT_FAIL, "stract_s failed!\n");
    s32Ret = HI_FAILURE;
    pstSobel->pFpDstV2 = fopen(path, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstSobel->pFpDstV2 == HI_NULL, SOBEL_INIT_FAIL, "Error,Open file %s failed!\n", path);
    s32Ret = HI_SUCCESS;

SOBEL_INIT_FAIL:
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_IVE_Sobel_Uninit(pstSobel);
    }
    return s32Ret;
}

/* function : show Sobel proc */
static HI_S32 SAMPLE_IVE_SobelProc(SAMPLE_IVE_SOBEL_S *pstSobel)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bInstant = HI_TRUE;
    IVE_HANDLE IveHandle;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;

    s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstSobel->stSrc1), pstSobel->pFpSrc);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);
    /* src1 address and src2 address can not be overlap */
    (HI_VOID)memcpy_s(SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_VOID, pstSobel->stSrc2.au64VirAddr[0]),
        pstSobel->stSrc2.au32Stride[0] * pstSobel->stSrc2.u32Height,
        SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_VOID, pstSobel->stSrc1.au64VirAddr[0]),
        pstSobel->stSrc2.au32Stride[0] * pstSobel->stSrc2.u32Height);
    s32Ret = HI_MPI_SYS_MmzFlushCache(pstSobel->stSrc1.au64PhyAddr[0],
        SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_VOID, pstSobel->stSrc1.au64VirAddr[0]),
        pstSobel->stSrc1.au32Stride[0] * pstSobel->stSrc1.u32Height);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_SYS_MmzFlushCache failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_Sobel(&IveHandle, &pstSobel->stSrc1, &pstSobel->stDstH1, &pstSobel->stDstV1,
        &pstSobel->stSobelCtrl, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Sobel failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
        usleep(IVE_QUERY_SLEEP_TIME); /* sleep 100 us */
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }

    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstSobel->stDstH1, pstSobel->pFpDstH1);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Write dstH1 failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstSobel->stDstV1, pstSobel->pFpDstV1);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Write dstV1 failed!\n", s32Ret);

    // Second sobel
    // The result of sobel my be error,if you do not call HI_MPI_SYS_MmzFlushCache to flush cache
    if (s_bFlushCached == HI_TRUE) {
        s32Ret = HI_MPI_SYS_MmzFlushCache(pstSobel->stSrc2.au64PhyAddr[0],
            SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_VOID, pstSobel->stSrc2.au64VirAddr[0]),
            pstSobel->stSrc2.au32Stride[0] * pstSobel->stSrc2.u32Height);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_SYS_MmzFlushCache failed!\n", s32Ret);
    }

    s32Ret = HI_MPI_IVE_Sobel(&IveHandle, &pstSobel->stSrc2, &pstSobel->stDstH2, &pstSobel->stDstV2,
        &pstSobel->stSobelCtrl, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Sobel failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
        usleep(IVE_QUERY_SLEEP_TIME); /* sleep 100 us */
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstSobel->stDstH2, pstSobel->pFpDstH2);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Write dstH2 failed!\n", s32Ret);
    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstSobel->stDstV2, pstSobel->pFpDstV2);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Write dstV2 failed!\n", s32Ret);
    return s32Ret;
}

static HI_VOID SAMPLE_IVE_SobelPause(HI_VOID)
{
    SAMPLE_IVE_Sobel_Uninit(&s_stSobel);
    (HI_VOID)memset_s(&s_stSobel, sizeof(s_stSobel), 0, sizeof(s_stSobel));
    SAMPLE_COMM_IVE_IveMpiExit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

/* function : show Sobel sample */
HI_VOID SAMPLE_IVE_Sobel(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32Width = IVE_D1_WIDTH;
    HI_U32 u32Height = IVE_D1_HEIGHT;
    HI_CHAR *pchSrcFileName = "./data/input/sobel/sobel.yuv";
    HI_CHAR achSrcFileName[PATH_MAX] = {0};
    s_bStopSignal = HI_FALSE;

    SAMPLE_CHECK_EXPR_RET_VOID(
        (strlen(pchSrcFileName) > PATH_MAX) || (realpath(pchSrcFileName, achSrcFileName) == NULL), "invalid path!\n");
    (HI_VOID)memset_s(&s_stSobel, sizeof(s_stSobel), 0, sizeof(s_stSobel));
    SAMPLE_COMM_IVE_CheckIveMpiInit();
    s32Ret = SAMPLE_IVE_Sobel_Init(&s_stSobel, achSrcFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, SOBEL_FAIL, "Error(%#x),SAMPLE_IVE_Sobel_Init failed!\n", s32Ret);
    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_SobelPause();
        return;
    }

    s32Ret = SAMPLE_IVE_SobelProc(&s_stSobel);
    if (s32Ret == HI_SUCCESS) {
        SAMPLE_PRT("Process success!\n");
    }
    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_SobelPause();
        return;
    }

    s_bStopSignal = HI_TRUE;
    SAMPLE_IVE_Sobel_Uninit(&s_stSobel);
    (HI_VOID)memset_s(&s_stSobel, sizeof(s_stSobel), 0, sizeof(s_stSobel));

SOBEL_FAIL:
    s_bStopSignal = HI_TRUE;
    SAMPLE_COMM_IVE_IveMpiExit();
}

/* function : Sobel sample signal handle */
HI_VOID SAMPLE_IVE_Sobel_HandleSig(HI_VOID)
{
    s_bStopSignal = HI_TRUE;
}
