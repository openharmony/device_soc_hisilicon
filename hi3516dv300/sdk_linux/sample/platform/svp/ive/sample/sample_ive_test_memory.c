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

typedef struct hiTEST_MEMORY_S {
    IVE_SRC_IMAGE_S stSrc1;
    IVE_SRC_IMAGE_S stSrc2;
    IVE_DST_IMAGE_S stDst;
    IVE_DST_MEM_INFO_S stHist;
    IVE_SUB_CTRL_S stSubCtrl;
    FILE *pFpSrc;
    FILE *pFpDst;
} TEST_MEMORY_S;

static TEST_MEMORY_S s_stTestMem;
static HI_BOOL s_bStopSignal = HI_FALSE;

/* function : test memory uninit */
static HI_VOID SAMPLE_IVE_TestMemory_Uninit(TEST_MEMORY_S *pstTestMem)
{
    IVE_MMZ_FREE(pstTestMem->stSrc1.au64PhyAddr[0], pstTestMem->stSrc1.au64VirAddr[0]);
    IVE_MMZ_FREE(pstTestMem->stSrc2.au64PhyAddr[0], pstTestMem->stSrc2.au64VirAddr[0]);
    IVE_MMZ_FREE(pstTestMem->stDst.au64PhyAddr[0], pstTestMem->stDst.au64VirAddr[0]);
    IVE_MMZ_FREE(pstTestMem->stHist.u64PhyAddr, pstTestMem->stHist.u64VirAddr);

    IVE_CLOSE_FILE(pstTestMem->pFpSrc);
    IVE_CLOSE_FILE(pstTestMem->pFpDst);
}
/* function : test memory init */
static HI_S32 SAMPLE_IVE_TestMemory_Init(TEST_MEMORY_S *pstTestMem, HI_CHAR *pchSrcFileName, HI_CHAR *pchDstFileName,
    HI_U32 u32Width, HI_U32 u32Height)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Size;

    (HI_VOID)memset_s(pstTestMem, sizeof(TEST_MEMORY_S), 0, sizeof(TEST_MEMORY_S));

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMem->stSrc1), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, TEST_MEMORY_INIT_FAIL, "Error(%#x),Create src1 image failed!\n",
        s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMem->stSrc2), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, TEST_MEMORY_INIT_FAIL, "Error(%#x),Create src2 image failed!\n",
        s32Ret);

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMem->stDst), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, TEST_MEMORY_INIT_FAIL, "Error(%#x),Create dst image failed!\n",
        s32Ret);

    u32Size = IVE_HIST_NUM * sizeof(HI_U32);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstTestMem->stHist), u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, TEST_MEMORY_INIT_FAIL, "Error(%#x),Create hist mem info failed!\n",
        s32Ret);

    pstTestMem->stSubCtrl.enMode = IVE_SUB_MODE_ABS;

    s32Ret = HI_FAILURE;
    pstTestMem->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(pstTestMem->pFpSrc == HI_NULL, TEST_MEMORY_INIT_FAIL, "Error,Open file %s failed!\n",
        pchSrcFileName);

    pstTestMem->pFpDst = fopen(pchDstFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(pstTestMem->pFpDst == HI_NULL, TEST_MEMORY_INIT_FAIL, "Error,Open file %s failed!\n",
        pchDstFileName);

    s32Ret = HI_SUCCESS;

TEST_MEMORY_INIT_FAIL:
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_IVE_TestMemory_Uninit(pstTestMem);
    }
    return s32Ret;
}

/* function : test memory */
static HI_S32 SAMPLE_IVE_TestMemoryProc(TEST_MEMORY_S *pstTestMem)
{
    HI_S32 s32Ret;
    HI_U32 *pu32Hist;
    HI_U32 i;
    IVE_HANDLE IveHandle;
    HI_BOOL bInstant = HI_FALSE;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;
    IVE_SRC_DATA_S stSrcData;
    IVE_DST_DATA_S stDstData;
    IVE_DMA_CTRL_S stDmaCtrl;

    s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstTestMem->stSrc1), pstTestMem->pFpSrc);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);

    stDmaCtrl.enMode = IVE_DMA_MODE_DIRECT_COPY;

    stSrcData.u64VirAddr = pstTestMem->stSrc1.au64VirAddr[0];
    stSrcData.u64PhyAddr = pstTestMem->stSrc1.au64PhyAddr[0];
    stSrcData.u32Width = pstTestMem->stSrc1.u32Width;
    stSrcData.u32Height = pstTestMem->stSrc1.u32Height;
    stSrcData.u32Stride = pstTestMem->stSrc1.au32Stride[0];

    stDstData.u64VirAddr = pstTestMem->stSrc2.au64VirAddr[0];
    stDstData.u64PhyAddr = pstTestMem->stSrc2.au64PhyAddr[0];
    stDstData.u32Width = pstTestMem->stSrc2.u32Width;
    stDstData.u32Height = pstTestMem->stSrc2.u32Height;
    stDstData.u32Stride = pstTestMem->stSrc2.au32Stride[0];
    s32Ret = HI_MPI_IVE_DMA(&IveHandle, &stSrcData, &stDstData, &stDmaCtrl, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_DMA failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_Sub(&IveHandle, &pstTestMem->stSrc1, &pstTestMem->stSrc2, &pstTestMem->stDst,
        &pstTestMem->stSubCtrl, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Sub failed!\n", s32Ret);

    bInstant = HI_TRUE;
    s32Ret = HI_MPI_IVE_Hist(&IveHandle, &pstTestMem->stDst, &pstTestMem->stHist, bInstant);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Hist failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (s32Ret == HI_ERR_IVE_QUERY_TIMEOUT) {
        usleep(IVE_QUERY_SLEEP_TIME); // sleep 100 us
        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error(%#x),HI_MPI_IVE_Query failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstTestMem->stDst, pstTestMem->pFpDst);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Error,Write dst file failed!\n");

    pu32Hist = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U32, pstTestMem->stHist.u64VirAddr);
    if (pu32Hist[0] != pstTestMem->stSrc1.u32Width * pstTestMem->stSrc1.u32Height) {
        s32Ret = HI_FAILURE;
        SAMPLE_PRT("Test mem error,pu32Hist[0] = %d\n", pu32Hist[0]);
        for (i = 1; i < IVE_HIST_NUM; i++) {
            if (pu32Hist[i] != 0) {
                SAMPLE_PRT("Test mem error, pu32Hist[%d] = %d\n", i, pu32Hist[i]);
            }
        }
    }

    return s32Ret;
}

static HI_VOID SAMPLE_IVE_TestMemoryPause(HI_VOID)
{
    SAMPLE_IVE_TestMemory_Uninit(&s_stTestMem);
    (HI_VOID)memset_s(&s_stTestMem, sizeof(s_stTestMem), 0, sizeof(s_stTestMem));
    SAMPLE_COMM_IVE_IveMpiExit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

/* function : Show test memory sample */
HI_VOID SAMPLE_IVE_TestMemory(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32Width = IVE_D1_WIDTH;
    HI_U32 u32Height = IVE_D1_HEIGHT;
    HI_CHAR *pchSrcFile = "./data/input/testmem/test_mem_in.yuv";
    HI_CHAR achDstFile[PATH_MAX] = {0};
    HI_CHAR achSrcFile[PATH_MAX] = {0};
    s_bStopSignal = HI_FALSE;

    SAMPLE_CHECK_EXPR_RET_VOID((strlen(pchSrcFile) > PATH_MAX) || (realpath(pchSrcFile, achSrcFile) == NULL),
        "invalid path!\n");

    SAMPLE_CHECK_EXPR_RET_VOID(realpath("./data/output/testmem", achDstFile) == NULL, "invalid path");
    s32Ret = strcat_s(achDstFile, PATH_MAX, "/test_mem_out.yuv");
    SAMPLE_CHECK_EXPR_RET_VOID(s32Ret < 0, "invali param");
    SAMPLE_CHECK_EXPR_RET_VOID(strlen(achDstFile) > PATH_MAX, "dst file path is invalid!\n");

    (HI_VOID)memset_s(&s_stTestMem, sizeof(s_stTestMem), 0, sizeof(s_stTestMem));
    SAMPLE_COMM_IVE_CheckIveMpiInit();
    s32Ret = SAMPLE_IVE_TestMemory_Init(&s_stTestMem, achSrcFile, achDstFile, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, TEST_MEMORY_FAIL, "Error(%#x),SAMPLE_IVE_TestMemory_Init failed!\n",
        s32Ret);
    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_TestMemoryPause();
        return;
    }

    s32Ret = SAMPLE_IVE_TestMemoryProc(&s_stTestMem);
    if (s32Ret == HI_SUCCESS) {
        SAMPLE_PRT("Process success!\n");
    }
    if (s_bStopSignal == HI_TRUE) {
        SAMPLE_IVE_TestMemoryPause();
        return;
    }
    s_bStopSignal = HI_TRUE;

    SAMPLE_IVE_TestMemory_Uninit(&s_stTestMem);
    (HI_VOID)memset_s(&s_stTestMem, sizeof(s_stTestMem), 0, sizeof(s_stTestMem));

TEST_MEMORY_FAIL:
    s_bStopSignal = HI_TRUE;
    SAMPLE_COMM_IVE_IveMpiExit();
}

/* function : TestMemory sample signal handle */
HI_VOID SAMPLE_IVE_TestMemory_HandleSig(HI_VOID)
{
    s_bStopSignal = HI_TRUE;
}
