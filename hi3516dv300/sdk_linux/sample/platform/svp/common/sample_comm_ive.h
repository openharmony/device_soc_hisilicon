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
#ifndef SAMPLE_COMM_IVE_H
#define SAMPLE_COMM_IVE_H

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
#include <limits.h>

#include "hi_debug.h"
#include "hi_comm_ive.h"
#include "mpi_ive.h"
#include "sample_comm.h"

#define VIDEO_WIDTH         352
#define VIDEO_HEIGHT        288
#define IVE_ALIGN           16
#define IVE_CHAR_CALW       8
#define IVE_CHAR_CALH       8
#define IVE_CHAR_NUM        (IVE_CHAR_CALW * IVE_CHAR_CALH)
#define IVE_FILE_NAME_LEN   256
#define IVE_RECT_NUM        64
#define VPSS_CHN_NUM        2
#define IVE_MASK_NUM_25     25
#define IVE_D1_WIDTH        720
#define IVE_D1_HEIGHT       576
#define IVE_CIF_WIDTH       352
#define IVE_CIF_HEIGHT      288

#define IVE_QUERY_SLEEP_TIME  100
#define SAMPLE_IVE_FRM_TIMEOUT  20000

#define SAMPLE_ALIGN_BACK(x, a) ((a) * (((x) / (a))))

typedef struct hiSAMPLE_IVE_SWITCH_S {
    HI_BOOL bVenc;
    HI_BOOL bVo;
} SAMPLE_IVE_SWITCH_S;

typedef struct hiSAMPLE_IVE_RECT_S {
    POINT_S astPoint[4];
} SAMPLE_IVE_RECT_S;

typedef struct hiSAMPLE_RECT_ARRAY_S {
    HI_U16 u16Num;
    SAMPLE_IVE_RECT_S astRect[IVE_RECT_NUM];
} SAMPLE_RECT_ARRAY_S;

typedef struct hiIVE_LINEAR_DATA_S {
    HI_S32 s32LinearNum;
    HI_S32 s32ThreshNum;
    POINT_S *pstLinearPoint;
} IVE_LINEAR_DATA_S;

typedef struct hiSAMPLE_IVE_DRAW_RECT_MSG_S {
    VIDEO_FRAME_INFO_S stFrameInfo;
    SAMPLE_RECT_ARRAY_S stRegion;
} SAMPLE_IVE_DRAW_RECT_MSG_S;

#define IVE_MMZ_FREE(phy, vir)                                         \
    do {                                                               \
        if (((phy) != 0) && ((vir) != 0)) {                            \
            HI_MPI_SYS_MmzFree((phy), (HI_VOID *)(HI_UINTPTR_T)(vir)); \
            (phy) = 0;                                                 \
            (vir) = 0;                                                 \
        }                                                              \
    } while (0)

#define IVE_CLOSE_FILE(fp)  \
    do {                    \
        if ((fp) != NULL) { \
            (void)fclose((fp));   \
            (fp) = NULL;    \
        }                   \
    } while (0)

#define SAMPLE_PAUSE()                                                      \
    do {                                                                    \
        printf("---------------press Enter key to exit!---------------\n"); \
        (void)getchar();                                                    \
    } while (0)
#define SAMPLE_CHECK_EXPR_RET(expr, ret, fmt...) \
    do {                                         \
        if (expr) {                              \
            SAMPLE_PRT(fmt);                     \
            return (ret);                        \
        }                                        \
    } while (0)

#define SAMPLE_CHECK_EXPR_RET_VOID(expr, fmt...) \
    do {                                         \
        if (expr) {                              \
            SAMPLE_PRT(fmt);                     \
            return;                              \
        }                                        \
    } while (0)

#define SAMPLE_CHECK_EXPR_GOTO(expr, label, fmt...) \
    do {                                            \
        if (expr) {                                 \
            SAMPLE_PRT(fmt);                        \
            goto label;                             \
        }                                           \
    } while (0)

#define SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(Type, Addr) (Type *)(HI_UINTPTR_T)(Addr)


HI_VOID SAMPLE_COMM_IVE_CheckIveMpiInit(HI_VOID);

HI_S32 SAMPLE_COMM_IVE_IveMpiExit(HI_VOID);

HI_S32 SAMPLE_COMM_IVE_ReadFile(IVE_IMAGE_S *pstImg, FILE *pFp);

HI_S32 SAMPLE_COMM_IVE_WriteFile(IVE_IMAGE_S *pstImg, FILE *pFp);

HI_U16 SAMPLE_COMM_IVE_CalcStride(HI_U32 u32Width, HI_U8 u8Align);

HI_VOID SAMPLE_COMM_IVE_BlobToRect(IVE_CCBLOB_S *pstBlob, SAMPLE_RECT_ARRAY_S *pstRect, HI_U16 u16RectMaxNum,
    HI_U16 u16AreaThrStep, HI_U32 u32SrcWidth, HI_U32 u32SrcHeight, HI_U32 u32DstWidth, HI_U32 u32DstHeight);

HI_S32 SAMPLE_COMM_IVE_CreateImage(IVE_IMAGE_S *pstImg, IVE_IMAGE_TYPE_E enType, HI_U32 u32Width, HI_U32 u32Height);

HI_S32 SAMPLE_COMM_IVE_CreateMemInfo(IVE_MEM_INFO_S *pstMemInfo, HI_U32 u32Size);

HI_S32 SAMPLE_COMM_IVE_CreateImageByCached(IVE_IMAGE_S *pstImg, IVE_IMAGE_TYPE_E enType, HI_U32 u32Width,
    HI_U32 u32Height);

HI_S32 SAMPLE_COMM_IVE_CreateData(IVE_DATA_S *pstData, HI_U32 u32Width, HI_U32 u32Height);

HI_S32 SAMPLE_COMM_IVE_VbInit(PIC_SIZE_E *paenSize, SIZE_S *pastSize, HI_U32 u32VpssChnNum);

HI_S32 SAMPLE_COMM_IVE_DmaImage(VIDEO_FRAME_INFO_S *pstFrameInfo, IVE_DST_IMAGE_S *pstDst, HI_BOOL bInstant);

HI_S32 SAMPLE_COMM_VGS_FillRect(VIDEO_FRAME_INFO_S *pstFrmInfo, SAMPLE_RECT_ARRAY_S *pstRect, HI_U32 u32Color);

HI_S32 SAMPLE_COMM_IVE_StartVpss(SIZE_S *pastSize, HI_U32 u32VpssChnNum);

HI_VOID SAMPLE_COMM_IVE_StopVpss(HI_U32 u32VpssChnNum);

HI_S32 SAMPLE_COMM_IVE_StartVo(HI_VOID);

HI_VOID SAMPLE_COMM_IVE_StopVo(HI_VOID);

HI_S32 SAMPLE_COMM_IVE_StartViVpssVencVo(SAMPLE_VI_CONFIG_S *pstViConfig, SAMPLE_IVE_SWITCH_S *pstSwitch,
    PIC_SIZE_E *penExtPicSize);

HI_VOID SAMPLE_COMM_IVE_StopViVpssVencVo(SAMPLE_VI_CONFIG_S *pstViConfig, SAMPLE_IVE_SWITCH_S *pstSwitch);

#endif
