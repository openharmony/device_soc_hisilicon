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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "hi_debug.h"
#include "hi_comm_ive.h"
#include "sample_comm_ive.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* consts */
#define IMG_FULL_CHN    3 // Full channel / three channel, for YUV444, RGB888
#define IMG_HALF_CHN    2 // Half channel, for YUV420/422
#define THREE_TIMES     3
#define TWO_TIMES       2

/* Commonly used numerical units */
#define HI_KB               1024
#define HI_MB               (1024 * 1024)
#define HI_MS_OF_SEC        1000 // 1s in milliseconds
#define HI_NS_OF_MS         1000000 // Nanoseconds in 1ms
#define HI_BYTE_BITS        8 // Number of bits in 1 byte
#define HI_OVEN_BASE        2 // Even base
#define HI_INT8_BITS        8 // 8-bit integer number of bits
#define HI_INT16_BITS       16 // 16-bit integer number of bits
#define HI_INT32_BITS       32 // 32-bit integer number of bits
#define HI_INT64_BITS       64 // The number of bits of a 64-bit integer
#define HI_PER_BASE         100

/* debug log level */
#define HI_DLEV_NONE        0 // disable
#define HI_DLEV_ERR         1 // error
#define HI_DLEV_WARN        2 // warning
#define HI_DLEV_INFO        3 // informational
#define HI_DLEV_DBG         4 // debug normal
#define HI_DLEV_VERB        5 // debug vervose
#define HI_DLEV_BUTT        6

#define BOX_NUM_DEVIDE_2    2

#define LOGI(format, ...) LOG_ONLY(HI_DLEV_INFO, format, ##__VA_ARGS__)

/* log with file/name */
#define LOG_ONLY(lev, format, ...) do { \
    if (g_hiDbgLev >= (lev)) { \
        printf(format, ##__VA_ARGS__); \
    } \
} while (0)

/* rect box */
typedef struct RectBox {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
} RectBox;

/* Alignment type */
typedef enum AlignType {
    ALIGN_TYPE_2 = 2, // Align by 2 bytes
    ALIGN_TYPE_16 = 16, // Align by 16 bytes
    ALIGN_TYPE_32 = 32, // Align by 32 bytes
} AlignType;

typedef struct HiSampleIveResizeCropInfo {
    IVE_SRC_IMAGE_S stSrc;
    FILE* pFpSrc;
    FILE* pFpDst;
} SampleIveResizeCropInfo;

static SampleIveResizeCropInfo g_stResizeCropInfo;

/* debug level */
int g_hiDbgLev = HI_DLEV_INFO;

int HiAlign16(int num)
{
    return (((num) + 16 - 1) / 16 * 16); // 16: Align16
}

int HiAlign32(int num)
{
    return (((num) + 32 - 1) / 32 * 32); // 32: Align32
}

/* Take the file name part of the path */
const char* HiPathName(const char* path)
{
    HI_ASSERT(path);

    const char *p = strrchr(path, '/');
    if (p) {
        return p + 1;
    }
    return path;
}

/* Calculate the stride of a channel */
static uint32_t IveCalStride(IVE_IMAGE_TYPE_E enType, uint32_t width, AlignType align)
{
    uint32_t size = 1;

    switch (enType) {
        case IVE_IMAGE_TYPE_U8C1:
        case IVE_IMAGE_TYPE_S8C1:
        case IVE_IMAGE_TYPE_S8C2_PACKAGE:
        case IVE_IMAGE_TYPE_S8C2_PLANAR:
        case IVE_IMAGE_TYPE_U8C3_PACKAGE:
        case IVE_IMAGE_TYPE_U8C3_PLANAR:
            size = sizeof(HI_U8);
            break;
        case IVE_IMAGE_TYPE_S16C1:
        case IVE_IMAGE_TYPE_U16C1:
            size = sizeof(HI_U16);
            break;
        case IVE_IMAGE_TYPE_S32C1:
        case IVE_IMAGE_TYPE_U32C1:
            size = sizeof(uint32_t);
            break;
        case IVE_IMAGE_TYPE_S64C1:
        case IVE_IMAGE_TYPE_U64C1:
            size = sizeof(uint64_t);
            break;
        default:
            break;
    }

    if (align == ALIGN_TYPE_16) {
        return HiAlign16(width * size);
    } else if (align == ALIGN_TYPE_32) {
        return HiAlign32(width * size);
    } else {
        HI_ASSERT(0);
        return 0;
    }
}

/* Create ive image buffer based on type and size */
int IveImgCreate(IVE_IMAGE_S* img,
    IVE_IMAGE_TYPE_E enType, uint32_t width, uint32_t height)
{
    HI_ASSERT(img);
    uint32_t oneChnSize;
    uint32_t size;
    int ret;

    if (memset_s(img, sizeof(*img), 0, sizeof(*img)) != EOK) {
        HI_ASSERT(0);
    }
    img->enType = enType;
    img->u32Width = width;
    img->u32Height = height;
    img->au32Stride[0] = IveCalStride(img->enType, img->u32Width, ALIGN_TYPE_16);

    switch (enType) {
        case IVE_IMAGE_TYPE_U8C1:
        case IVE_IMAGE_TYPE_S8C1: // Only 1 channel
            size = img->au32Stride[0] * img->u32Height;
            ret = HI_MPI_SYS_MmzAlloc(&img->au64PhyAddr[0], (void**)&img->au64VirAddr[0], NULL, NULL, size);
            SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), HI_MPI_SYS_MmzAlloc!\n", ret);
            break;
        // The size is equivalent to 1.5 times (3/2) of the pixel, which is equivalent to 2 channels
        case IVE_IMAGE_TYPE_YUV420SP:
        // The size is equivalent to 2 times the pixel, which is equivalent to 2 channels
        case IVE_IMAGE_TYPE_YUV422SP:
            if (enType == IVE_IMAGE_TYPE_YUV420SP) {
                size = img->au32Stride[0] * img->u32Height * THREE_TIMES / TWO_TIMES;
            } else {
                size = img->au32Stride[0] * img->u32Height * TWO_TIMES;
            }
            ret = HI_MPI_SYS_MmzAlloc(&img->au64PhyAddr[0], (void**)&img->au64VirAddr[0], NULL, NULL, size);
            SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), HI_MPI_SYS_MmzAlloc!\n", ret);

            // Set the stride of the address of channel 1, both of which require channel 1
            img->au32Stride[1] = img->au32Stride[0];
            img->au64PhyAddr[1] = img->au64PhyAddr[0] + img->au32Stride[0] * (uint64_t)img->u32Height;
            img->au64VirAddr[1] = img->au64VirAddr[0] + img->au32Stride[0] * (uint64_t)img->u32Height;
            break;

        case IVE_IMAGE_TYPE_U8C3_PLANAR: // 3 channels, often used for RGB
            oneChnSize = img->au32Stride[0] * img->u32Height;
            size = oneChnSize * 3; // 3 channels have the same size
            ret = HI_MPI_SYS_MmzAlloc(&img->au64PhyAddr[0], (void**)&img->au64VirAddr[0], NULL, NULL, size);
            SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), HI_MPI_SYS_MmzAlloc!\n", ret);

            // Set the address and stride of channel 1 and channel 2
            img->au64VirAddr[1] = img->au64VirAddr[0] + oneChnSize;
            img->au64PhyAddr[1] = img->au64PhyAddr[0] + oneChnSize;
            img->au32Stride[1] = img->au32Stride[0];
            img->au64VirAddr[2] = img->au64VirAddr[1] + oneChnSize; // 2: au64VirAddr array subscript, not out of bounds
            img->au64PhyAddr[2] = img->au64PhyAddr[1] + oneChnSize; // 2: au64VirAddr array subscript, not out of bounds
            img->au32Stride[2] = img->au32Stride[0]; // 2: au64VirAddr array subscript, not out of bounds
            break;

        // Types not currently supported: YVC420P, YUV422P, S8C2_PACKAGE, S8C2_PLANAR,
        // S32C1, U32C1, S64C1, U64C1, S16C1, U16C1, U8C3_PACKAGE,etc.
        default:
            HI_ASSERT(0);
            break;
    }
    return HI_SUCCESS;
}

int ImgYuvCrop(const IVE_IMAGE_S *src, IVE_IMAGE_S *dst, const RectBox* origBox)
{
    RectBox box = *origBox;
    int boxWidth = box.xmax - box.xmin;
    int boxHeight = box.ymax - box.ymin;
    int ret;

    HI_ASSERT(boxWidth > 0 && boxWidth <= src->u32Width);
    HI_ASSERT(boxHeight > 0 && boxHeight <= src->u32Height);
    HI_ASSERT(src->au64VirAddr[0]);
    HI_ASSERT(src->au32Stride[0] >= src->u32Width);

    // Adjust the width/height of the box to a multiple of 2
    if (boxWidth == 1 || boxHeight == 1) {
        printf("box dstWidth=1 && dstHeight=1\n");
        return -1;
    }
    if (boxWidth % HI_OVEN_BASE) {
        box.xmax--;
        boxWidth--;
    }
    if (boxHeight % HI_OVEN_BASE) {
        box.ymax--;
        boxHeight--;
    }
    // Create empty dst img
    LOGI("ImgYuvCrop boxWidth:%d, ImgYuvCrop boxHeight:%d, entype:%d\n", boxWidth, boxWidth, src->enType);
    ret = IveImgCreate(dst, src->enType, boxWidth, boxHeight);
    HI_ASSERT(!ret);
    // Use IVE DMA to copy to improve performance
    // copy box from src to dst
    // Y
    int srcStrideY = src->au32Stride[0];
    int dstStrideY = dst->au32Stride[0];
    uint8_t *srcBufY = (uint8_t*)((uintptr_t)src->au64VirAddr[0]);
    uint8_t *dstBufY = (uint8_t*)((uintptr_t)dst->au64VirAddr[0]);
    uint8_t *srcPtrY = &srcBufY[box.ymin * srcStrideY];
    uint8_t *dstPtrY = dstBufY;
    for (int h = 0; h < boxHeight; h++, srcPtrY += srcStrideY, dstPtrY += dstStrideY) {
        if (memcpy_s(dstPtrY, boxWidth, srcPtrY + box.xmin, boxWidth) != EOK) {
            HI_ASSERT(0);
        }
    }
    HI_ASSERT(dstPtrY - dstBufY == boxHeight * dstStrideY);

    // UV
    int srcStrideUV = src->au32Stride[1];
    int dstStrideUV = dst->au32Stride[1];
    uint8_t *srcBufUV = (uint8_t*)((uintptr_t)src->au64VirAddr[1]);
    uint8_t *dstBufUV = (uint8_t*)((uintptr_t)dst->au64VirAddr[1]);
    uint8_t *srcPtrUV = &srcBufUV[(box.ymin / BOX_NUM_DEVIDE_2) * srcStrideUV];
    uint8_t *dstPtrUV = dstBufUV;
    for (int h = 0; h < (boxHeight / BOX_NUM_DEVIDE_2); h++, srcPtrUV += srcStrideUV, dstPtrUV += dstStrideUV) {
        if (memcpy_s(dstPtrUV, boxWidth, srcPtrUV + box.xmin, boxWidth) != EOK) {
            HI_ASSERT(0);
        }
    }
    HI_ASSERT(dstPtrUV - dstBufUV == (boxHeight / BOX_NUM_DEVIDE_2) * dstStrideUV);

    return ret;
}

/* destory ive image */
void IveImgDestroy(IVE_IMAGE_S* img)
{
    for (int i = 0; i < IMG_FULL_CHN; i++) {
        if (img->au64PhyAddr[0] && img->au64VirAddr[0]) {
            HI_MPI_SYS_MmzFree(img->au64PhyAddr[i], (void*)((uintptr_t)img->au64VirAddr[i]));
            img->au64PhyAddr[i] = 0;
            img->au64VirAddr[i] = 0;
        }
    }
    if (memset_s(img, sizeof(*img), 0, sizeof(*img)) != EOK) {
        HI_ASSERT(0);
    }
}

/* function : SampleIveResizeCropUninit */
static HI_VOID SampleIveResizeCropUninit(SampleIveResizeCropInfo* pstResizeCropInfo)
{
    IveImgDestroy(&pstResizeCropInfo->stSrc);

    IVE_CLOSE_FILE(pstResizeCropInfo->pFpSrc);
    IVE_CLOSE_FILE(pstResizeCropInfo->pFpDst);
}

/* function : Resize init */
static HI_S32 SampleIveResizeInit(SampleIveResizeCropInfo* pstResizeCropInfo,
    HI_CHAR* pchSrcFileName, HI_CHAR* pchDstFileName, HI_U32 u32Width, HI_U32 u32Height)
{
    HI_S32 s32Ret;

    memset_s(pstResizeCropInfo, sizeof(SampleIveResizeCropInfo), 0, sizeof(SampleIveResizeCropInfo));

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstResizeCropInfo->stSrc, IVE_IMAGE_TYPE_YUV420SP, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_INIT_FAIL, "Error(%#x),Create Src Image failed!\n", s32Ret);

    s32Ret = HI_FAILURE;
    pstResizeCropInfo->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(HI_NULL == pstResizeCropInfo->pFpSrc, RESIZE_INIT_FAIL,
        "Error,Open file %s failed!\n", pchSrcFileName);

    pstResizeCropInfo->pFpDst = fopen(pchDstFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(HI_NULL == pstResizeCropInfo->pFpDst, RESIZE_INIT_FAIL,
        "Error,Open file %s failed!\n", pchDstFileName);

    s32Ret = HI_SUCCESS;

RESIZE_INIT_FAIL:

    if (HI_SUCCESS != s32Ret) {
        SampleIveResizeCropUninit(pstResizeCropInfo);
    }
    return s32Ret;
}

/* function : Crop init */
static HI_S32 SampleIveCropInit(SampleIveResizeCropInfo* pstResizeCropInfo,
    HI_CHAR* pchSrcFileName, HI_CHAR* pchDstFileName, HI_U32 u32Width, HI_U32 u32Height)
{
    HI_S32 s32Ret;

    memset_s(pstResizeCropInfo, sizeof(SampleIveResizeCropInfo), 0, sizeof(SampleIveResizeCropInfo));

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstResizeCropInfo->stSrc, IVE_IMAGE_TYPE_YUV420SP, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CROP_INIT_FAIL, "Error(%#x),Create Src Image failed!\n", s32Ret);

    s32Ret = HI_FAILURE;
    pstResizeCropInfo->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(HI_NULL == pstResizeCropInfo->pFpSrc, CROP_INIT_FAIL,
        "Error,Open file %s failed!\n", pchSrcFileName);

    pstResizeCropInfo->pFpDst = fopen(pchDstFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(HI_NULL == pstResizeCropInfo->pFpDst, CROP_INIT_FAIL,
        "Error,Open file %s failed!\n", pchDstFileName);

    s32Ret = HI_SUCCESS;

CROP_INIT_FAIL:

    if (HI_SUCCESS != s32Ret) {
        SampleIveResizeCropUninit(pstResizeCropInfo);
    }
    return s32Ret;
}

/* Allocate memory for structures such as ive resize ctrl */
static int IveCtrlAlloc(IVE_MEM_INFO_S *memInfo, uint32_t size, HI_BOOL cached)
{
    HI_ASSERT(memInfo && size > 0);
    int ret;

    if (cached) {
        ret = HI_MPI_SYS_MmzAlloc_Cached(&memInfo->u64PhyAddr,
            (void**)&memInfo->u64VirAddr, NULL, NULL, size);
    } else {
        ret = HI_MPI_SYS_MmzAlloc(&memInfo->u64PhyAddr,
            (void**)&memInfo->u64VirAddr, NULL, NULL, size);
    }
    HI_ASSERT(!ret);

    memInfo->u32Size = size;
    return ret;
}

/* Release memory allocated for ive resize ctrl etc */
static void IveCtrlFree(IVE_MEM_INFO_S *memInfo)
{
    if (HI_MPI_SYS_MmzFree(memInfo->u64PhyAddr, (void*)((uintptr_t)memInfo->u64VirAddr)) < 0) {
        HI_ASSERT(0);
    }

    memInfo->u64PhyAddr = 0;
    memInfo->u64VirAddr = 0;
}

/*
 * Image scaling is performed once with IVE.
 * Only U8C3/U8C1 planers are supported, but YUV is not supported.
 */
static int IveResizeOnce(const IVE_IMAGE_S *src, IVE_IMAGE_S *dst, uint32_t dstWidth, uint32_t dstHeight)
{
    static const uint32_t resizeCtrlNum = 49;
    IVE_RESIZE_CTRL_S resizeCtrl = { 0 };
    IVE_HANDLE iveHnd;
    HI_BOOL finish = HI_FALSE;
    int ret;

    HI_ASSERT((src->u32Height % HI_OVEN_BASE) == 0 && (src->u32Width % HI_OVEN_BASE) == 0);

    // Prepare resizeCtrl
    resizeCtrl.enMode = IVE_RESIZE_MODE_LINEAR;
    resizeCtrl.u16Num = 1;
    // pstResizeCtrl.stMem memory needs at least 25*U8C1_NUM + 49 * (pstResizeCtrl->u16Num-U8C1_NUM) bytes,
    // where U8C1_NUM is the number of U8C1 images in the mixed image array.
    ret = IveCtrlAlloc(&resizeCtrl.stMem, resizeCtrlNum * resizeCtrl.u16Num, HI_FALSE);
    HI_ASSERT(!ret);

    // Create empty dst img
    ret = IveImgCreate(dst, src->enType, dstWidth, dstHeight);
    HI_ASSERT(!ret);

    // resize request
    ret = HI_MPI_IVE_Resize(&iveHnd, (IVE_IMAGE_S*)src, dst, &resizeCtrl, HI_TRUE);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != ret, FAIL, "Error(%#x), HI_MPI_IVE_Resize failed!\n", ret);

    // waiting resize result, blocking
    ret = HI_MPI_IVE_Query(iveHnd, &finish, HI_TRUE);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != ret, FAIL, "Error(%#x), HI_MPI_IVE_Query failed!\n", ret);

    if (resizeCtrl.stMem.u64VirAddr) {
        IveCtrlFree(&resizeCtrl.stMem);
    }

    return ret;

    FAIL:
        IveImgDestroy(dst);
        if (memset_s(dst, sizeof(*dst), 0, sizeof(*dst)) != EOK) {
            HI_ASSERT(0);
        }

        if (resizeCtrl.stMem.u64VirAddr) {
            IveCtrlFree(&resizeCtrl.stMem);
        }
        return ret;
}

/* Amplify the integer to the given multiple range */
int IntZoomTo(int n, double rate, double rateMin, double rateMax)
{
    HI_ASSERT(rateMin < 1 && rateMax > 1);
    int ret;

    if (!rateMin) {
        HI_ASSERT(rateMin);
        return n;
    } else {
        if (rate > rateMax) {
            ret = n * (int)rateMax;
        } else if (rate < rateMin) {
            ret = n / (int)(1 / rateMin);
        } else {
            ret = (int)(n * rate);
        }
        return ret < 1 ? 1 : ret;
    }
}

/*
 * resize ive image.
 * Call ive_resize multiple times to achieve arbitrary scaling.
 * To simplify implementation, each scaled maximum agreement 14 times, this time width, height only 2 pixels aligned.
 * When the zoom directions are different in the two directions, for example,
 * one direction (such as X) zooms in and the other direction zooms out, no special processing is required.
 * At this time, the zoom ratio in one direction or both directions exceeds the standard,
 * and no special treatment is required.
 */
int IveImgResize(
    const IVE_IMAGE_S *src, IVE_IMAGE_S *dst,
    uint32_t dstWidth, uint32_t dstHeight)
{
    static const double rateMax = 14.0; // Maximum magnification
    static const double rateMin = 1.0 / rateMax; // The minimum magnification, that is, the maximum reduction

    uint32_t srcWidth = src->u32Width;
    uint32_t srcHeight = src->u32Height;
    HI_ASSERT(srcWidth > 0 && srcHeight > 0);
    HI_ASSERT(!(srcWidth % HI_OVEN_BASE) && !(srcHeight % HI_OVEN_BASE));
    HI_ASSERT(dstWidth > 0 && dstHeight > 0);
    HI_ASSERT(!(dstWidth % HI_OVEN_BASE) && !(dstHeight % HI_OVEN_BASE));
    int ret;

    // Magnification
    double widthRate = ((double)dstWidth) / (double)srcWidth; // >1 means zoom in, <1 means zoom out
    double heightRate = ((double)dstHeight) / (double)srcHeight; // >1 means zoom in, <1 means zoom out

    // Separate processing according to zoom factor
    if (widthRate > rateMax || widthRate < rateMin ||
        heightRate > rateMax || heightRate < rateMin) {
        // When the zoom factor exceeds the maximum value of one IVE resize, recursive processing...
        uint32_t midWidth = (uint32_t)IntZoomTo((int)srcWidth, widthRate, rateMin, rateMax);
        uint32_t midHeight = (uint32_t)IntZoomTo((int)srcHeight, heightRate, rateMin, rateMax);
        // Make sure it is an even number. When it is an odd number, the zoom is reduced by one,
        // otherwise it is increased by one
        if (midWidth % HI_OVEN_BASE) {
            midWidth += widthRate > 1 ? -1 : 1;
        }
        if (midHeight % HI_OVEN_BASE) {
            midHeight += heightRate > 1 ? -1 : 1;
        }

        // Scaling at once
        IVE_IMAGE_S midImg;
        ret = IveResizeOnce(src, &midImg, midWidth, midHeight);
        LOGI("IveResizeOnce(dw=%u, dh=%u) \n", midWidth, midHeight);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), IveResizeOnce failed!\n", ret);

        // Scaling at once
        ret = IveImgResize(&midImg, dst, dstWidth, dstHeight);
        IveImgDestroy(&midImg);
        LOGI("sub call IveImgResize(dw=%u, dh=%u) FAIL\n", dstWidth, dstHeight);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), sub call IveImgResize failed!\n", ret);
    } else { // The scaling factor does not exceed the maximum value of one IVE resize, and it is directly finished
        ret = IveResizeOnce(src, dst, dstWidth, dstHeight);
        LOGI("finish IveResizeOnce(dw=%u, dh=%u)\n", dstWidth, dstHeight);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), IveResizeOnce failed!\n", ret);
    }
    return ret;
}

static HI_S32 SampleIveReadResizeCropFile(SampleIveResizeCropInfo* pstResizeCropInfo)
{
    HI_S32 s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstResizeCropInfo->stSrc), pstResizeCropInfo->pFpSrc);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, "Error(%#x),Read src file failed!\n", s32Ret);
    return s32Ret;
}

/* cut YUV P/SP image to U8C1 image(Grayscale image) */
int ImgYuvCropU8c1(
    const IVE_IMAGE_S *src, IVE_IMAGE_S *dst,
    const RectBox* origBox, int dstWidth, int dstHeight)
{
    RectBox box = *origBox;
    int boxWidth = box.xmax - box.xmin + 1;
    int boxHeight = box.ymax - box.ymin + 1;
    int ret;

    HI_ASSERT(boxWidth > 0 && boxWidth <= src->u32Width);
    HI_ASSERT(boxHeight > 0 && boxHeight <= src->u32Height);
    HI_ASSERT(src->au64VirAddr[0]);
    HI_ASSERT(src->au32Stride[0] >= src->u32Width);

    // Adjust the width/height of the box to a multiple of 2
    if (boxWidth == 1 || boxHeight == 1) {
        printf("box dstWidth=1 && dstHeight=1\n");
        return -1;
    }
    if (boxWidth % HI_OVEN_BASE) {
        box.xmax--;
        boxWidth--;
    }
    if (boxHeight % HI_OVEN_BASE) {
        box.ymax--;
        boxHeight--;
    }

    // Create empty dst img
    ret = IveImgCreate(dst, IVE_IMAGE_TYPE_U8C1, boxWidth, boxHeight);
    HI_ASSERT(!ret);

    // Use IVE DMA to copy to improve performance
    // copy box from src to dst
    int srcStride = src->au32Stride[0];
    int dstStride = dst->au32Stride[0];
    uint8_t *srcBuf = (uint8_t*)((uintptr_t)src->au64VirAddr[0]);
    uint8_t *dstBuf = (uint8_t*)((uintptr_t)dst->au64VirAddr[0]);
    uint8_t *srcPtr = &srcBuf[box.ymin * srcStride];
    uint8_t *dstPtr = dstBuf;
    for (int h = 0; h < boxHeight; h++, srcPtr += srcStride, dstPtr += dstStride) {
        if (memcpy_s(dstPtr, boxWidth, srcPtr + box.xmin, boxWidth) != EOK) {
            HI_ASSERT(0);
        }
    }
    HI_ASSERT(dstPtr - dstBuf == boxHeight * dstStride);

    // When the user does not specify dstWidth/dstHeight,
    // or the specified value is the same as the box, there is no need to resize
    if (dstWidth <= 0 || dstHeight <= 0 ||
        (dstWidth == boxWidth && dstHeight == boxHeight)) {
        return 0;
    }

    // resize
    IVE_IMAGE_S realDst;
    ret = IveImgResize(dst, &realDst, dstWidth, dstHeight);
    IveImgDestroy(dst);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), yuv_cut_to_u8c1 FAIL!\n", ret);
    *dst = realDst;
    return ret;
}

void SampleIveImgResize(void)
{
    HI_U16 u32Width = 1920; // 1920: Original image size Width
    HI_U16 u32Height = 1080; // 1080: Original image size Height
    HI_U16 resizeWidth = 800; // 800: resize image size Width
    HI_U16 resizeHeight = 480; // 480: resize image size Height
    HI_S32 s32Ret;
    HI_CHAR* pchSrcFileName = "./data/input/resize_img/UsePic_1920_1080_420.yuv";
    HI_CHAR achDstFileName[IVE_FILE_NAME_LEN];
    IVE_IMAGE_S u8c1Img;
    RectBox cropU8c1Box = {0};
    cropU8c1Box.xmin = 100; // 100: crop u8c1 image size xmin
    cropU8c1Box.xmax = 1800; // 1800: crop u8c1 image size xmax
    cropU8c1Box.ymin = 100; // 100: crop u8c1 image size ymin
    cropU8c1Box.ymax = 1000; // 1000: crop u8c1 image size ymax

    // Initialize the g_stResizeCropInfo structure
    memset_s(&g_stResizeCropInfo, sizeof(g_stResizeCropInfo), 0, sizeof(g_stResizeCropInfo));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    if (snprintf_s(achDstFileName, sizeof(achDstFileName), sizeof(achDstFileName) - 1,
        "./data/output/resize_res/complete_%s.yuv", "resize") < 0) {
        HI_ASSERT(0);
    }
    s32Ret = SampleIveResizeInit(&g_stResizeCropInfo, pchSrcFileName, achDstFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_CROP_FAIL, "Error(%#x), SampleIveResizeInit failed!\n", s32Ret);

    s32Ret = SampleIveReadResizeCropFile(&g_stResizeCropInfo);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_CROP_FAIL,
        "Error(%#x), SampleIveReadResizeCropFile failed!\n", s32Ret);

    s32Ret = ImgYuvCropU8c1(&(g_stResizeCropInfo.stSrc), &u8c1Img, &cropU8c1Box, resizeWidth, resizeHeight);
    LOGI("resize width:%d, resize height:%d, entype:%d\n", u8c1Img.u32Width, u8c1Img.u32Height, u8c1Img.enType);
    LOGI("Congratulations resize success\n");
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_CROP_FAIL, "Error(%#x), ImgYuvCropU8c1 failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&u8c1Img, g_stResizeCropInfo.pFpDst); // write yuv file
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_CROP_FAIL,
        "Error(%#x), SAMPLE_COMM_IVE_WriteFile failed!\n", s32Ret);

    IveImgDestroy(&u8c1Img);
    SampleIveResizeCropUninit(&g_stResizeCropInfo);
    memset_s(&g_stResizeCropInfo, sizeof(g_stResizeCropInfo), 0, sizeof(g_stResizeCropInfo));

RESIZE_CROP_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}

void SampleIveImgCrop(void)
{
    HI_U16 u32Width = 1920; // 1920: Original image size Width
    HI_U16 u32Height = 1080; // 1080: Original image size Height
    RectBox cropBox = {0};
    cropBox.xmin = 16; // 16: crop param xmin
    cropBox.xmax = 240; // 240: crop param xmax
    cropBox.ymin = 16; // 16: crop param ymin
    cropBox.ymax = 240; // 240: crop param ymax

    HI_CHAR* pchSrcFileName = "./data/input/crop_img/UsePic_1920_1080_420.yuv";
    HI_CHAR achDstFileName[IVE_FILE_NAME_LEN];
    IVE_IMAGE_S cropImg;
    HI_S32 s32Ret;

    // Initialize the g_stResizeCropInfo structure
    memset_s(&g_stResizeCropInfo, sizeof(g_stResizeCropInfo), 0, sizeof(g_stResizeCropInfo));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    if (snprintf_s(achDstFileName, sizeof(achDstFileName), sizeof(achDstFileName) - 1,
        "./data/output/crop_res/complete_%s.yuv", "crop") < 0) {
        HI_ASSERT(0);
    }
    s32Ret = SampleIveCropInit(&g_stResizeCropInfo, pchSrcFileName, achDstFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_CROP_FAIL, "Error(%#x), SampleIveCropInit failed!\n", s32Ret);

    s32Ret = SampleIveReadResizeCropFile(&g_stResizeCropInfo);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_CROP_FAIL,
        "Error(%#x), SampleIveReadResizeCropFile failed!\n", s32Ret);

    s32Ret = ImgYuvCrop(&(g_stResizeCropInfo.stSrc), &cropImg, &cropBox);
    LOGI("crop width:%d, crop height:%d, entype:%d\n", cropImg.u32Width, cropImg.u32Height, cropImg.enType);
    LOGI("Congratulations crop success\n");
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_CROP_FAIL, "Error(%#x), ImgYuvCrop failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&cropImg, g_stResizeCropInfo.pFpDst); // write yuv file
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, RESIZE_CROP_FAIL,
        "Error(%#x), SAMPLE_COMM_IVE_WriteFile failed!\n", s32Ret);

    IveImgDestroy(&cropImg);
    memset_s(&cropBox, sizeof(RectBox), 0, sizeof(RectBox));
    SampleIveResizeCropUninit(&g_stResizeCropInfo);
    memset_s(&g_stResizeCropInfo, sizeof(g_stResizeCropInfo), 0, sizeof(g_stResizeCropInfo));

RESIZE_CROP_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
