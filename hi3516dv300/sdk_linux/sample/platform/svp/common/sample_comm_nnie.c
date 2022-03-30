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
#include "sample_comm_nnie.h"
#include "mpi_sys.h"
#include "sample_comm_svp.h"

HI_S32 SAMPLE_COMM_SVP_NNIE_ParamDeinit(SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam)
{
    SAMPLE_SVP_CHECK_EXPR_RET(pstNnieParam == NULL, HI_INVALID_VALUE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, pstNnieParam can't be NULL!\n");

    if ((pstNnieParam->stTaskBuf.u64PhyAddr != 0) && (pstNnieParam->stTaskBuf.u64VirAddr != 0)) {
        SAMPLE_SVP_MMZ_FREE(pstNnieParam->stTaskBuf.u64PhyAddr, pstNnieParam->stTaskBuf.u64VirAddr);
        pstNnieParam->stTaskBuf.u64PhyAddr = 0;
        pstNnieParam->stTaskBuf.u64VirAddr = 0;
    }

    if ((pstNnieParam->stStepBuf.u64PhyAddr != 0) && (pstNnieParam->stStepBuf.u64VirAddr != 0)) {
        SAMPLE_SVP_MMZ_FREE(pstNnieParam->stStepBuf.u64PhyAddr, pstNnieParam->stStepBuf.u64VirAddr);
        pstNnieParam->stStepBuf.u64PhyAddr = 0;
        pstNnieParam->stStepBuf.u64VirAddr = 0;
    }
    return HI_SUCCESS;
}

static HI_S32 SAMPLE_SVP_NNIE_FillForwardInfo(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg, SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam)
{
    HI_U32 i, j;
    HI_U32 u32Num;
    HI_U32 u32Offset = 0;

    for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
        /* fill forwardCtrl info */
        if (pstNnieParam->pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_ROI) {
            pstNnieParam->astForwardWithBboxCtrl[i].enNnieId = pstNnieCfg->aenNnieCoreId[i];
            pstNnieParam->astForwardWithBboxCtrl[i].u32SrcNum = pstNnieParam->pstModel->astSeg[i].u16SrcNum;
            pstNnieParam->astForwardWithBboxCtrl[i].u32DstNum = pstNnieParam->pstModel->astSeg[i].u16DstNum;
            pstNnieParam->astForwardWithBboxCtrl[i].u32ProposalNum = 1;
            pstNnieParam->astForwardWithBboxCtrl[i].u32NetSegId = i;
            pstNnieParam->astForwardWithBboxCtrl[i].stTmpBuf = pstNnieParam->stTmpBuf;
            pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u64PhyAddr =
                pstNnieParam->stTaskBuf.u64PhyAddr + u32Offset;
            pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u64VirAddr =
                pstNnieParam->stTaskBuf.u64VirAddr + u32Offset;
            pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u32Size = pstNnieParam->au32TaskBufSize[i];
        } else if ((pstNnieParam->pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_CNN) ||
            (pstNnieParam->pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_RECURRENT)) {
            pstNnieParam->astForwardCtrl[i].enNnieId = pstNnieCfg->aenNnieCoreId[i];
            pstNnieParam->astForwardCtrl[i].u32SrcNum = pstNnieParam->pstModel->astSeg[i].u16SrcNum;
            pstNnieParam->astForwardCtrl[i].u32DstNum = pstNnieParam->pstModel->astSeg[i].u16DstNum;
            pstNnieParam->astForwardCtrl[i].u32NetSegId = i;
            pstNnieParam->astForwardCtrl[i].stTmpBuf = pstNnieParam->stTmpBuf;
            pstNnieParam->astForwardCtrl[i].stTskBuf.u64PhyAddr = pstNnieParam->stTaskBuf.u64PhyAddr + u32Offset;
            pstNnieParam->astForwardCtrl[i].stTskBuf.u64VirAddr = pstNnieParam->stTaskBuf.u64VirAddr + u32Offset;
            pstNnieParam->astForwardCtrl[i].stTskBuf.u32Size = pstNnieParam->au32TaskBufSize[i];
        }
        u32Offset += pstNnieParam->au32TaskBufSize[i];

        /* fill src blob info */
        for (j = 0; j < pstNnieParam->pstModel->astSeg[i].u16SrcNum; j++) {
            /* Recurrent blob */
            if (pstNnieParam->pstModel->astSeg[i].astSrcNode[j].enType == SVP_BLOB_TYPE_SEQ_S32) {
                pstNnieParam->astSegData[i].astSrc[j].enType = pstNnieParam->pstModel->astSeg[i].astSrcNode[j].enType;
                pstNnieParam->astSegData[i].astSrc[j].unShape.stSeq.u32Dim =
                    pstNnieParam->pstModel->astSeg[i].astSrcNode[j].unShape.u32Dim;
                pstNnieParam->astSegData[i].astSrc[j].u32Num = pstNnieCfg->u32MaxInputNum;
                pstNnieParam->astSegData[i].astSrc[j].unShape.stSeq.u64VirAddrStep =
                    pstNnieCfg->au64StepVirAddr[i * SAMPLE_SVP_NNIE_EACH_SEG_STEP_ADDR_NUM];
            } else {
                pstNnieParam->astSegData[i].astSrc[j].enType = pstNnieParam->pstModel->astSeg[i].astSrcNode[j].enType;
                pstNnieParam->astSegData[i].astSrc[j].unShape.stWhc.u32Chn =
                    pstNnieParam->pstModel->astSeg[i].astSrcNode[j].unShape.stWhc.u32Chn;
                pstNnieParam->astSegData[i].astSrc[j].unShape.stWhc.u32Height =
                    pstNnieParam->pstModel->astSeg[i].astSrcNode[j].unShape.stWhc.u32Height;
                pstNnieParam->astSegData[i].astSrc[j].unShape.stWhc.u32Width =
                    pstNnieParam->pstModel->astSeg[i].astSrcNode[j].unShape.stWhc.u32Width;
                pstNnieParam->astSegData[i].astSrc[j].u32Num = pstNnieCfg->u32MaxInputNum;
            }
        }

        /* fill dst blob info */
        if (pstNnieParam->pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_ROI) {
            SAMPLE_SVP_CHECK_EXPR_RET((HI_U64)pstNnieCfg->u32MaxRoiNum * pstNnieCfg->u32MaxInputNum >
                SAMPLE_SVP_NNIE_MAX_MEM,
                HI_ERR_SVP_NNIE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,u32Num should be less than %u!\n",
                SAMPLE_SVP_NNIE_MAX_MEM);
            u32Num = pstNnieCfg->u32MaxRoiNum * pstNnieCfg->u32MaxInputNum;
        } else {
            u32Num = pstNnieCfg->u32MaxInputNum;
        }

        for (j = 0; j < pstNnieParam->pstModel->astSeg[i].u16DstNum; j++) {
            if (pstNnieParam->pstModel->astSeg[i].astDstNode[j].enType == SVP_BLOB_TYPE_SEQ_S32) {
                pstNnieParam->astSegData[i].astDst[j].enType = pstNnieParam->pstModel->astSeg[i].astDstNode[j].enType;
                pstNnieParam->astSegData[i].astDst[j].unShape.stSeq.u32Dim =
                    pstNnieParam->pstModel->astSeg[i].astDstNode[j].unShape.u32Dim;
                pstNnieParam->astSegData[i].astDst[j].u32Num = u32Num;
                pstNnieParam->astSegData[i].astDst[j].unShape.stSeq.u64VirAddrStep =
                    pstNnieCfg->au64StepVirAddr[i * SAMPLE_SVP_NNIE_EACH_SEG_STEP_ADDR_NUM + 1];
            } else {
                pstNnieParam->astSegData[i].astDst[j].enType = pstNnieParam->pstModel->astSeg[i].astDstNode[j].enType;
                pstNnieParam->astSegData[i].astDst[j].unShape.stWhc.u32Chn =
                    pstNnieParam->pstModel->astSeg[i].astDstNode[j].unShape.stWhc.u32Chn;
                pstNnieParam->astSegData[i].astDst[j].unShape.stWhc.u32Height =
                    pstNnieParam->pstModel->astSeg[i].astDstNode[j].unShape.stWhc.u32Height;
                pstNnieParam->astSegData[i].astDst[j].unShape.stWhc.u32Width =
                    pstNnieParam->pstModel->astSeg[i].astDstNode[j].unShape.stWhc.u32Width;
                pstNnieParam->astSegData[i].astDst[j].u32Num = u32Num;
            }
        }
    }
    return HI_SUCCESS;
}

static hi_s32 SAMPLE_SVP_NNIE_GetBlobMemSize(SVP_NNIE_NODE_S astNnieNode[], HI_U32 u32NodeNum, HI_U32 u32TotalStep,
    SVP_BLOB_S astBlob[], HI_U32 u32Align, HI_U32 *pu32TotalSize, HI_U32 au32BlobSize[])
{
    HI_U32 i;
    HI_U64 u64Size;
    HI_U64 u64TotalSize = *pu32TotalSize;
    HI_U64 u64Stride;

    for (i = 0; i < u32NodeNum; i++) {
        if (SVP_BLOB_TYPE_S32 == astNnieNode[i].enType || SVP_BLOB_TYPE_VEC_S32 == astNnieNode[i].enType ||
            SVP_BLOB_TYPE_SEQ_S32 == astNnieNode[i].enType) {
            u64Size = sizeof(HI_U32);
        } else {
            u64Size = sizeof(HI_U8);
        }
        if (SVP_BLOB_TYPE_SEQ_S32 == astNnieNode[i].enType) {
            if (SAMPLE_SVP_NNIE_ALIGN_16 == u32Align) {
                u64Stride = SAMPLE_SVP_NNIE_ALIGN16(astNnieNode[i].unShape.u32Dim * u64Size);
            } else {
                u64Stride = SAMPLE_SVP_NNIE_ALIGN32(astNnieNode[i].unShape.u32Dim * u64Size);
            }
            SAMPLE_SVP_CHECK_EXPR_RET(u64Stride > SAMPLE_SVP_NNIE_MAX_MEM, HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
                SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,u64Stride should be less than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
            u64Size = u32TotalStep * u64Stride;
            SAMPLE_SVP_CHECK_EXPR_RET(u64Size > SAMPLE_SVP_NNIE_MAX_MEM, HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
                SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,u64Size should be less than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
            au32BlobSize[i] = u64Size;
        } else {
            if (SAMPLE_SVP_NNIE_ALIGN_16 == u32Align) {
                u64Stride = SAMPLE_SVP_NNIE_ALIGN16(astNnieNode[i].unShape.stWhc.u32Width * u64Size);
            } else {
                u64Stride = SAMPLE_SVP_NNIE_ALIGN32(astNnieNode[i].unShape.stWhc.u32Width * u64Size);
            }
            SAMPLE_SVP_CHECK_EXPR_RET(u64Stride > SAMPLE_SVP_NNIE_MAX_MEM, HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
                SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,u64Stride should be less than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
            u64Size = astBlob[i].u32Num * u64Stride;
            SAMPLE_SVP_CHECK_EXPR_RET(u64Size > SAMPLE_SVP_NNIE_MAX_MEM, HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
                SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,u64Size should be less than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
            u64Size *= astNnieNode[i].unShape.stWhc.u32Height;
            SAMPLE_SVP_CHECK_EXPR_RET(u64Size > SAMPLE_SVP_NNIE_MAX_MEM, HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
                SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,u64Size should be less than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
            u64Size *= astNnieNode[i].unShape.stWhc.u32Chn;
            SAMPLE_SVP_CHECK_EXPR_RET(u64Size > SAMPLE_SVP_NNIE_MAX_MEM, HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
                SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,u64Size should be less than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
            au32BlobSize[i] = (HI_U32)u64Size;
        }
        u64TotalSize += au32BlobSize[i];
        SAMPLE_SVP_CHECK_EXPR_RET(u64TotalSize > SAMPLE_SVP_NNIE_MAX_MEM, HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,u64TotalSize should be less than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
        *pu32TotalSize = (HI_U32)u64TotalSize;
        astBlob[i].u32Stride = (HI_U32)u64Stride;
    }
    return HI_SUCCESS;
}

static HI_S32 SAMPLE_SVP_NNIE_GetTaskAndBlobBufSize(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg,
    SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam, HI_U32 *pu32TotalTaskBufSize, HI_U32 *pu32TmpBufSize,
    SAMPLE_SVP_NNIE_BLOB_SIZE_S astBlobSize[], HI_U32 *pu32TotalSize, HI_U32 u32BlobSizeNUm)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i, j;
    HI_U64 u64TotalStep = 0;

    /* Get each seg's task buf size */
    s32Ret = HI_MPI_SVP_NNIE_GetTskBufSize(pstNnieCfg->u32MaxInputNum, pstNnieCfg->u32MaxRoiNum, pstNnieParam->pstModel,
        pstNnieParam->au32TaskBufSize, pstNnieParam->pstModel->u32NetSegNum);
    SAMPLE_SVP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, HI_MPI_SVP_NNIE_GetTaskSize failed!\n");

    /* Get total task buf size */
    *pu32TotalTaskBufSize = 0;
    for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
        SAMPLE_SVP_CHECK_EXPR_RET((HI_U64)(*pu32TotalTaskBufSize) + pstNnieParam->au32TaskBufSize[i] >
            SAMPLE_SVP_NNIE_MAX_MEM,
            s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, total task buf size can't be greater than %u!\n",
            SAMPLE_SVP_NNIE_MAX_MEM);
        *pu32TotalTaskBufSize += pstNnieParam->au32TaskBufSize[i];
    }

    /* Get tmp buf size */
    *pu32TmpBufSize = pstNnieParam->pstModel->u32TmpBufSize;
    SAMPLE_SVP_CHECK_EXPR_RET((HI_U64)*pu32TotalTaskBufSize + *pu32TmpBufSize > SAMPLE_SVP_NNIE_MAX_MEM, s32Ret,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, task_buf + tmpbuf can't be greater than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
    *pu32TotalSize += *pu32TotalTaskBufSize + *pu32TmpBufSize;

    /* calculate Blob mem size */
    for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum && i < u32BlobSizeNUm; i++) {
        if (pstNnieParam->pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_RECURRENT) {
            for (j = 0; j < pstNnieParam->astSegData[i].astSrc[0].u32Num; j++) {
                u64TotalStep += *(SAMPLE_SVP_NNIE_CONVERT_64BIT_ADDR(HI_S32,
                    pstNnieParam->astSegData[i].astSrc[0].unShape.stSeq.u64VirAddrStep) + j);
                SAMPLE_SVP_CHECK_EXPR_RET(u64TotalStep > SAMPLE_SVP_NNIE_MAX_MEM, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                    "Error, u64TotalStep can't be greater than %u!\n", SAMPLE_SVP_NNIE_MAX_MEM);
            }
        }
        /* the first seg's Src Blob mem size, other seg's src blobs from the output blobs of
        those segs before it or from software output results */
        if (i == 0) {
            s32Ret = SAMPLE_SVP_NNIE_GetBlobMemSize(&(pstNnieParam->pstModel->astSeg[i].astSrcNode[0]),
                pstNnieParam->pstModel->astSeg[i].u16SrcNum, (HI_U32)u64TotalStep,
                &(pstNnieParam->astSegData[i].astSrc[0]), SAMPLE_SVP_NNIE_ALIGN_16, pu32TotalSize,
                &(astBlobSize[i].au32SrcSize[0]));
            SAMPLE_SVP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Error, SAMPLE_SVP_NNIE_GetBlobMemSize failed!\n");
        }

        /* Get each seg's Dst Blob mem size */
        s32Ret = SAMPLE_SVP_NNIE_GetBlobMemSize(&(pstNnieParam->pstModel->astSeg[i].astDstNode[0]),
            pstNnieParam->pstModel->astSeg[i].u16DstNum, (HI_U32)u64TotalStep, &(pstNnieParam->astSegData[i].astDst[0]),
            SAMPLE_SVP_NNIE_ALIGN_16, pu32TotalSize, &(astBlobSize[i].au32DstSize[0]));
        SAMPLE_SVP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error, SAMPLE_SVP_NNIE_GetBlobMemSize failed!\n");
    }
    return s32Ret;
}

static HI_S32 SAMPLE_SVP_NNIE_ParamInit(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg, SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam)
{
    HI_U32 i = 0, j = 0;
    HI_U32 u32TotalSize = 0;
    HI_U32 u32TotalTaskBufSize = 0;
    HI_U32 u32TmpBufSize = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Offset = 0;
    HI_U64 u64PhyAddr = 0;
    HI_U8 *pu8VirAddr = NULL;
    SAMPLE_SVP_NNIE_BLOB_SIZE_S astBlobSize[SVP_NNIE_MAX_NET_SEG_NUM] = {0};

    /* fill forward info */
    s32Ret = SAMPLE_SVP_NNIE_FillForwardInfo(pstNnieCfg, pstNnieParam);
    SAMPLE_SVP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,SAMPLE_SVP_NNIE_FillForwardCtrl failed!\n");

    /* Get taskInfo and Blob mem size */
    s32Ret = SAMPLE_SVP_NNIE_GetTaskAndBlobBufSize(pstNnieCfg, pstNnieParam, &u32TotalTaskBufSize, &u32TmpBufSize,
        astBlobSize, &u32TotalSize, SVP_NNIE_MAX_NET_SEG_NUM);
    SAMPLE_SVP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,SAMPLE_SVP_NNIE_GetTaskAndBlobBufSize failed!\n");

    /* Malloc mem */
    s32Ret = SAMPLE_COMM_SVP_MallocCached("SAMPLE_NNIE_TASK", NULL, (HI_U64 *)&u64PhyAddr, (void **)&pu8VirAddr,
        u32TotalSize);
    SAMPLE_SVP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,Malloc memory failed!\n");
    (HI_VOID)memset_s(pu8VirAddr, u32TotalSize, 0, u32TotalSize);
    SAMPLE_COMM_SVP_FlushCache(u64PhyAddr, (void *)pu8VirAddr, u32TotalSize);

    /* fill taskinfo mem addr */
    pstNnieParam->stTaskBuf.u32Size = u32TotalTaskBufSize;
    pstNnieParam->stTaskBuf.u64PhyAddr = u64PhyAddr;
    pstNnieParam->stTaskBuf.u64VirAddr = (HI_U64)(HI_UINTPTR_T)pu8VirAddr;

    /* fill Tmp mem addr */
    pstNnieParam->stTmpBuf.u32Size = u32TmpBufSize;
    pstNnieParam->stTmpBuf.u64PhyAddr = u64PhyAddr + u32TotalTaskBufSize;
    pstNnieParam->stTmpBuf.u64VirAddr = (HI_U64)(HI_UINTPTR_T)pu8VirAddr + u32TotalTaskBufSize;

    /* fill forward ctrl addr */
    for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
        if (pstNnieParam->pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_ROI) {
            pstNnieParam->astForwardWithBboxCtrl[i].stTmpBuf = pstNnieParam->stTmpBuf;
            pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u64PhyAddr =
                pstNnieParam->stTaskBuf.u64PhyAddr + u32Offset;
            pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u64VirAddr =
                pstNnieParam->stTaskBuf.u64VirAddr + u32Offset;
            pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u32Size = pstNnieParam->au32TaskBufSize[i];
        } else if ((pstNnieParam->pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_CNN) ||
            (pstNnieParam->pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_RECURRENT)) {
            pstNnieParam->astForwardCtrl[i].stTmpBuf = pstNnieParam->stTmpBuf;
            pstNnieParam->astForwardCtrl[i].stTskBuf.u64PhyAddr = pstNnieParam->stTaskBuf.u64PhyAddr + u32Offset;
            pstNnieParam->astForwardCtrl[i].stTskBuf.u64VirAddr = pstNnieParam->stTaskBuf.u64VirAddr + u32Offset;
            pstNnieParam->astForwardCtrl[i].stTskBuf.u32Size = pstNnieParam->au32TaskBufSize[i];
        }
        u32Offset += pstNnieParam->au32TaskBufSize[i];
    }

    /* fill each blob's mem addr */
    u64PhyAddr = u64PhyAddr + u32TotalTaskBufSize + u32TmpBufSize;
    pu8VirAddr = pu8VirAddr + u32TotalTaskBufSize + u32TmpBufSize;
    for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
        /* first seg has src blobs, other seg's src blobs from the output blobs of
        those segs before it or from software output results */
        if (i == 0) {
            for (j = 0; j < pstNnieParam->pstModel->astSeg[i].u16SrcNum; j++) {
                if (j != 0) {
                    u64PhyAddr += astBlobSize[i].au32SrcSize[j - 1];
                    pu8VirAddr += astBlobSize[i].au32SrcSize[j - 1];
                }
                pstNnieParam->astSegData[i].astSrc[j].u64PhyAddr = u64PhyAddr;
                pstNnieParam->astSegData[i].astSrc[j].u64VirAddr = (HI_U64)(HI_UINTPTR_T)pu8VirAddr;
            }
            u64PhyAddr += astBlobSize[i].au32SrcSize[j - 1];
            pu8VirAddr += astBlobSize[i].au32SrcSize[j - 1];
        }

        /* fill the mem addrs of each seg's output blobs */
        for (j = 0; j < pstNnieParam->pstModel->astSeg[i].u16DstNum; j++) {
            if (j != 0) {
                u64PhyAddr += astBlobSize[i].au32DstSize[j - 1];
                pu8VirAddr += astBlobSize[i].au32DstSize[j - 1];
            }
            pstNnieParam->astSegData[i].astDst[j].u64PhyAddr = u64PhyAddr;
            pstNnieParam->astSegData[i].astDst[j].u64VirAddr = SAMPLE_SVP_NNIE_CONVERT_PTR_TO_ADDR(HI_U64, pu8VirAddr);
        }
        u64PhyAddr += astBlobSize[i].au32DstSize[j - 1];
        pu8VirAddr += astBlobSize[i].au32DstSize[j - 1];
    }
    return s32Ret;
}

HI_S32 SAMPLE_COMM_SVP_NNIE_ParamInit(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg, SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam)
{
    HI_S32 s32Ret;

    SAMPLE_SVP_CHECK_EXPR_RET((pstNnieCfg == NULL || pstNnieParam == NULL), HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,pstNnieCfg and pstNnieParam can't be NULL!\n");
    SAMPLE_SVP_CHECK_EXPR_RET((pstNnieParam->pstModel == NULL), HI_ERR_SVP_NNIE_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,pstNnieParam->pstModel can't be NULL!\n");

    s32Ret = SAMPLE_SVP_NNIE_ParamInit(pstNnieCfg, pstNnieParam);
    SAMPLE_SVP_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, SAMPLE_SVP_NNIE_ParamInit failed!\n");

    return s32Ret;
FAIL:
    s32Ret = SAMPLE_COMM_SVP_NNIE_ParamDeinit(pstNnieParam);
    SAMPLE_SVP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, SAMPLE_COMM_SVP_NNIE_ParamDeinit failed!\n");
    return HI_FAILURE;
}

HI_S32 SAMPLE_COMM_SVP_NNIE_UnloadModel(SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel)
{
    SAMPLE_SVP_CHECK_EXPR_RET(pstNnieModel == HI_NULL, HI_ERR_SVP_NNIE_NULL_PTR, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, pstNnieModel is NULL!\n");

    if ((pstNnieModel->stModelBuf.u64PhyAddr != 0) && (pstNnieModel->stModelBuf.u64VirAddr != 0)) {
        SAMPLE_SVP_MMZ_FREE(pstNnieModel->stModelBuf.u64PhyAddr, pstNnieModel->stModelBuf.u64VirAddr);
        pstNnieModel->stModelBuf.u64PhyAddr = 0;
        pstNnieModel->stModelBuf.u64VirAddr = 0;
    }
    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_SVP_NNIE_LoadModel(const HI_CHAR *pszModelFile, SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel)
{
    HI_S32 s32Ret = HI_INVALID_VALUE;
    HI_U64 u64PhyAddr = 0;
    HI_U8 *pu8VirAddr = NULL;
    HI_SL slFileSize = 0;
    HI_ULONG ulSize;
    HI_CHAR path[PATH_MAX] = {0};

    /* Get model file size */
    SAMPLE_SVP_CHECK_EXPR_RET(pszModelFile == HI_NULL, HI_ERR_SVP_NNIE_NULL_PTR, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, pszModelFile is NULL!\n");
    SAMPLE_SVP_CHECK_EXPR_RET(pstNnieModel == HI_NULL, HI_ERR_SVP_NNIE_NULL_PTR, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, pstNnieModel is NULL!\n");
    SAMPLE_SVP_CHECK_EXPR_RET((strlen(pszModelFile) > PATH_MAX) || (realpath(pszModelFile, path) == HI_NULL),
        HI_ERR_SVP_NNIE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, file_name is invalid!\n");
    FILE *fp = fopen(path, "rb");
    SAMPLE_SVP_CHECK_EXPR_RET(fp == NULL, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, open model file failed!\n");
    s32Ret = fseek(fp, 0L, SEEK_END);
    SAMPLE_SVP_CHECK_EXPR_GOTO(s32Ret == -1, FAIL_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, fseek failed!\n");
    slFileSize = ftell(fp);
    SAMPLE_SVP_CHECK_EXPR_GOTO(slFileSize <= 0, FAIL_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, ftell failed!\n");
    s32Ret = fseek(fp, 0L, SEEK_SET);
    SAMPLE_SVP_CHECK_EXPR_GOTO(s32Ret == -1, FAIL_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error, fseek failed!\n");

    /* malloc model file mem */
    s32Ret =
        SAMPLE_COMM_SVP_MallocMem("SAMPLE_NNIE_MODEL", NULL, (HI_U64 *)&u64PhyAddr, (void **)&pu8VirAddr, slFileSize);
    SAMPLE_SVP_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Malloc memory failed!\n", s32Ret);

    pstNnieModel->stModelBuf.u32Size = (HI_U32)slFileSize;
    pstNnieModel->stModelBuf.u64PhyAddr = u64PhyAddr;
    pstNnieModel->stModelBuf.u64VirAddr = (HI_U64)(HI_UINTPTR_T)pu8VirAddr;

    ulSize = fread(pu8VirAddr, slFileSize, 1, fp);
    SAMPLE_SVP_CHECK_EXPR_GOTO(ulSize != 1, FAIL_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error,read model file failed!\n");

    /* load model */
    s32Ret = HI_MPI_SVP_NNIE_LoadModel(&pstNnieModel->stModelBuf, &pstNnieModel->stModel);
    SAMPLE_SVP_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,HI_MPI_SVP_NNIE_LoadModel failed!\n");

    (HI_VOID)fclose(fp);
    return HI_SUCCESS;

FAIL_1:
    SAMPLE_SVP_MMZ_FREE(pstNnieModel->stModelBuf.u64PhyAddr, pstNnieModel->stModelBuf.u64VirAddr);
    pstNnieModel->stModelBuf.u32Size = 0;
FAIL_0:
    if (fp != NULL) {
        (HI_VOID)fclose(fp);
    }
    return HI_FAILURE;
}

HI_S32 SAMPLE_COMM_SVP_NNIE_FillRect(VIDEO_FRAME_INFO_S *pstFrmInfo, SAMPLE_SVP_NNIE_RECT_ARRAY_S *pstRect,
    HI_U32 u32Color)
{
    VGS_HANDLE VgsHandle = -1;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i, j;
    VGS_TASK_ATTR_S stVgsTask;
    VGS_ADD_COVER_S stVgsAddCover;
    static HI_U32 u32Frm = 0;

    SAMPLE_SVP_CHECK_EXPR_RET(pstFrmInfo == HI_NULL, HI_ERR_SVP_NNIE_NULL_PTR, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, pstFrmInfo is NULL!\n");
    SAMPLE_SVP_CHECK_EXPR_RET(pstRect == HI_NULL, HI_ERR_SVP_NNIE_NULL_PTR, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error, pstRect is NULL!\n");
    u32Frm++;
    if (pstRect->u32TotalNum == 0) {
        return s32Ret;
    }
    s32Ret = HI_MPI_VGS_BeginJob(&VgsHandle);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Vgs begin job fail,Error(%#x)\n", s32Ret);
        return s32Ret;
    }

    (HI_VOID)memcpy_s(&stVgsTask.stImgIn, sizeof(VIDEO_FRAME_INFO_S), pstFrmInfo, sizeof(VIDEO_FRAME_INFO_S));
    (HI_VOID)memcpy_s(&stVgsTask.stImgOut, sizeof(VIDEO_FRAME_INFO_S), pstFrmInfo, sizeof(VIDEO_FRAME_INFO_S));

    stVgsAddCover.enCoverType = COVER_QUAD_RANGLE;
    stVgsAddCover.u32Color = u32Color;
    stVgsAddCover.stQuadRangle.bSolid = HI_FALSE;
    stVgsAddCover.stQuadRangle.u32Thick = 2; /* thick value: 2 */
    for (i = 0; i < pstRect->u32ClsNum; i++) {
        for (j = 0; j < pstRect->au32RoiNum[i]; j++) {
            (HI_VOID)memcpy_s(stVgsAddCover.stQuadRangle.stPoint, sizeof(pstRect->astRect[i][j].astPoint),
                pstRect->astRect[i][j].astPoint, sizeof(pstRect->astRect[i][j].astPoint));
            s32Ret = HI_MPI_VGS_AddCoverTask(VgsHandle, &stVgsTask, &stVgsAddCover);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("HI_MPI_VGS_AddCoverTask fail,Error(%#x)\n", s32Ret);
                HI_MPI_VGS_CancelJob(VgsHandle);
                return s32Ret;
            }
        }
    }

    s32Ret = HI_MPI_VGS_EndJob(VgsHandle);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VGS_EndJob fail,Error(%#x)\n", s32Ret);
        HI_MPI_VGS_CancelJob(VgsHandle);
        return s32Ret;
    }

    return s32Ret;
}
