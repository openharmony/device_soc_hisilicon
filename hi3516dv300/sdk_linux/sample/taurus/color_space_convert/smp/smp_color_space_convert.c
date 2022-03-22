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

#define LOGI(format, ...) LOG_ONLY(HI_DLEV_INFO, format, ##__VA_ARGS__)

/* log with file/name */
#define LOG_ONLY(lev, format, ...) do { \
    if (g_hiDbgLev >= (lev)) { \
        printf(format, ##__VA_ARGS__); \
    } \
}   while (0)

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

typedef struct HiSampleIveColorSpaceConvInfo {
    IVE_SRC_IMAGE_S stSrc;
    FILE* pFpSrc;
    FILE* pFpDst;
} SampleIveColorSpaceConvInfo;

static SampleIveColorSpaceConvInfo g_stColorSpaceInfo;

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
        case IVE_IMAGE_TYPE_YUV422SP: // The size is equivalent to 2 times the pixel, which is equivalent to 2 channels
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

/* function : Canny uninit */
static HI_VOID SampleIveColorConvertUninit(SampleIveColorSpaceConvInfo* pstColorConvertInfo)
{
    IveImgDestroy(&pstColorConvertInfo->stSrc);

    IVE_CLOSE_FILE(pstColorConvertInfo->pFpSrc);
    IVE_CLOSE_FILE(pstColorConvertInfo->pFpDst);
}

/* function : Crop init */
static HI_S32 SampleIveColorConvertInit(SampleIveColorSpaceConvInfo* g_stColorSpaceInfo,
    HI_CHAR* pchSrcFileName, HI_CHAR* pchDstFileName, HI_U32 u32Width, HI_U32 u32Height)
{
    HI_S32 s32Ret;

    memset_s(g_stColorSpaceInfo, sizeof(SampleIveColorSpaceConvInfo), 0, sizeof(SampleIveColorSpaceConvInfo));

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&g_stColorSpaceInfo->stSrc, IVE_IMAGE_TYPE_YUV420SP, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, COLOR_CONVERT_INIT_FAIL,
        "Error(%#x), Create Src Image failed!\n", s32Ret);

    s32Ret = HI_FAILURE;
    g_stColorSpaceInfo->pFpSrc = fopen(pchSrcFileName, "rb");
    SAMPLE_CHECK_EXPR_GOTO(HI_NULL == g_stColorSpaceInfo->pFpSrc, COLOR_CONVERT_INIT_FAIL,
        "Error, Open file %s failed!\n", pchSrcFileName);

    g_stColorSpaceInfo->pFpDst = fopen(pchDstFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(HI_NULL == g_stColorSpaceInfo->pFpDst, COLOR_CONVERT_INIT_FAIL,
        "Error, Open file %s failed!\n", pchDstFileName);

    s32Ret = HI_SUCCESS;

COLOR_CONVERT_INIT_FAIL:

    if (HI_SUCCESS != s32Ret) {
        SampleIveColorConvertUninit(g_stColorSpaceInfo);
    }
    return s32Ret;
}

static HI_S32 SampleIveReadFile(SampleIveColorSpaceConvInfo* g_stColorSpaceInfo)
{
    HI_S32 s32Ret = SAMPLE_COMM_IVE_ReadFile(&(g_stColorSpaceInfo->stSrc), g_stColorSpaceInfo->pFpSrc);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, "Error(%#x), Read src file failed!\n", s32Ret);
    return s32Ret;
}

/* ive image RGB to BGR */
int ImgRgbToBgr(IVE_IMAGE_S *img)
{
    uint8_t *rp = NULL;
    uint8_t *bp = NULL;
    uint8_t c;
    int i, j;

    // Replace with IVE DMA to improve performance
    HI_ASSERT(img->enType == IVE_IMAGE_TYPE_U8C3_PLANAR);
    HI_ASSERT(img->au32Stride[0] >= img->u32Width);
    HI_ASSERT(img->au32Stride[1] >= img->u32Width);
    HI_ASSERT(img->au32Stride[2] >= img->u32Width); // 2: au32Stride array subscript, not out of bounds

    rp = (uint8_t*)(uintptr_t)img->au64VirAddr[0];
    bp = (uint8_t*)(uintptr_t)img->au64VirAddr[2]; // 2: VirAddr array subscript, not out of bounds
    HI_ASSERT(rp && bp);
    for (i = 0; i < img->u32Height; i++) {
        for (j = 0; j < img->u32Width; j++) {
            c = rp[j];
            rp[j] = bp[j];
            bp[j] = c;
        }
        rp += img->au32Stride[0];
        bp += img->au32Stride[2]; // 2: au32Stride array subscript, not out of bounds
    }
    return 0;
}

/*
 * video frame to ive image.
 * Copy the data pointer, do not copy the data.
 */
int FrmToOrigImg(const VIDEO_FRAME_INFO_S* frm, IVE_IMAGE_S *img)
{
    static const int chnNum = 2; // Currently only supports YUV420/422, so only the addresses of 2 channels are copied
    PIXEL_FORMAT_E pixelFormat = frm->stVFrame.enPixelFormat;

    if (memset_s(img, sizeof(*img), 0, sizeof(*img)) != EOK) {
        HI_ASSERT(0);
    }

    img->u32Width = frm->stVFrame.u32Width;
    img->u32Height = frm->stVFrame.u32Height;

    if (pixelFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        img->enType = IVE_IMAGE_TYPE_YUV420SP;
    } else if (pixelFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
        img->enType = IVE_IMAGE_TYPE_YUV422SP;
    } else {
        HI_ASSERT(0);
        return -1;
    }

    for (int i = 0; i < chnNum; i++) {
        img->au64PhyAddr[i] = frm->stVFrame.u64PhyAddr[i];
        img->au64VirAddr[i] = frm->stVFrame.u64VirAddr[i];
        img->au32Stride[i] = frm->stVFrame.u32Stride[i];
    }
    return 0;
}

/*
 * ive image to video frame.
 * Copy the data pointer, do not copy the data.
 */
int OrigImgToFrm(const IVE_IMAGE_S *img, VIDEO_FRAME_INFO_S* frm)
{
    static const int chnNum = 2;
    IVE_IMAGE_TYPE_E enType = img->enType;
    if (memset_s(frm, sizeof(*frm), 0, sizeof(*frm)) != EOK) {
        HI_ASSERT(0);
    }

    frm->stVFrame.u32Width = img->u32Width;
    frm->stVFrame.u32Height = img->u32Height;

    if (enType == IVE_IMAGE_TYPE_YUV420SP) {
        frm->stVFrame.enPixelFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    } else if (enType == IVE_IMAGE_TYPE_YUV422SP) {
        frm->stVFrame.enPixelFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_422;
    } else {
        HI_ASSERT(0);
        return -1;
    }

    for (int i = 0; i < chnNum; i++) {
        frm->stVFrame.u64PhyAddr[i] = img->au64PhyAddr[i];
        frm->stVFrame.u64VirAddr[i] = img->au64VirAddr[i];
        frm->stVFrame.u32Stride[i] = img->au32Stride[i];
    }
    return 0;
}

/* video YUV frame to ive image (U8C1) */
int FrmToU8c1Img(const VIDEO_FRAME_INFO_S* frm, IVE_IMAGE_S *img)
{
    PIXEL_FORMAT_E pixelFormat = frm->stVFrame.enPixelFormat;

    if (memset_s(img, sizeof(*img), 0, sizeof(*img)) != EOK) {
        HI_ASSERT(0);
    }
    if (pixelFormat != PIXEL_FORMAT_YVU_SEMIPLANAR_420 &&
        pixelFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
        LOGI("FrmToU8c1Img() only supp yuv420sp/yuv422sp\n");
        HI_ASSERT(0);
        return -1;
    }

    img->enType = IVE_IMAGE_TYPE_U8C1;
    img->u32Width = frm->stVFrame.u32Width;
    img->u32Height = frm->stVFrame.u32Height;

    img->au64PhyAddr[0] = frm->stVFrame.u64PhyAddr[0];
    img->au64VirAddr[0] = frm->stVFrame.u64VirAddr[0];
    img->au32Stride[0] = frm->stVFrame.u32Stride[0];
    return 0;
}

/* YUV video frame to RGB ive image */
int FrmToRgbImg(VIDEO_FRAME_INFO_S* srcFrm, IVE_DST_IMAGE_S *dstImg)
{
    HI_ASSERT(srcFrm && dstImg);
    const static int chnNum = 3;
    IVE_HANDLE iveHnd;
    IVE_SRC_IMAGE_S srcImg;
    HI_BOOL finish;
    int ret;

    if (memset_s(&srcImg, sizeof(srcImg), 0, sizeof(srcImg)) != EOK) {
        HI_ASSERT(0);
    }
    srcImg.u32Width = srcFrm->stVFrame.u32Width;
    srcImg.u32Height = srcFrm->stVFrame.u32Height;

    PIXEL_FORMAT_E enPixelFormat = srcFrm->stVFrame.enPixelFormat;
    if (enPixelFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        srcImg.enType = IVE_IMAGE_TYPE_YUV420SP;
    } else if (enPixelFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
        srcImg.enType = IVE_IMAGE_TYPE_YUV422SP;
    } else {
        HI_ASSERT(0);
        return -1;
    }

    // Copy the addresses of the 3 channels separately
    for (int i = 0; i < chnNum; i++) {
        srcImg.au64PhyAddr[i] = srcFrm->stVFrame.u64PhyAddr[i];
        srcImg.au64VirAddr[i] = srcFrm->stVFrame.u64VirAddr[i];
        srcImg.au32Stride[i] = srcFrm->stVFrame.u32Stride[i];
    }

    ret = IveImgCreate(dstImg, IVE_IMAGE_TYPE_U8C3_PLANAR, srcImg.u32Width, srcImg.u32Height);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), IveImgCreate failed!\n", ret);

    IVE_CSC_CTRL_S stCSCCtrl = { IVE_CSC_MODE_PIC_BT601_YUV2RGB};
    ret = HI_MPI_IVE_CSC(&iveHnd, &srcImg, dstImg, &stCSCCtrl, HI_TRUE);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != ret, FAIL, "Error(%#x), HI_MPI_IVE_CSC failed!\n", ret);

    ret = HI_MPI_IVE_Query(iveHnd, &finish, HI_TRUE);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != ret, FAIL, "Error(%#x), HI_MPI_IVE_Query failed!\n", ret);
    return ret;

    FAIL:
        IveImgDestroy(dstImg);
        return ret;
}

/* ive image RGB to YUV */
int ImgRgbToYuv(IVE_IMAGE_S *src, IVE_IMAGE_S *dst, IVE_IMAGE_TYPE_E dstType)
{
    IVE_HANDLE iveHnd;
    HI_BOOL finish;
    HI_S32 ret;

    if (memset_s(dst, sizeof(*dst), 0, sizeof(*dst)) != EOK) {
        HI_ASSERT(0);
    }

    ret = IveImgCreate(dst, dstType, src->u32Width, src->u32Height);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), IveImgCreate failed!\n", ret);

    IVE_CSC_CTRL_S stCSCCtrl = { IVE_CSC_MODE_VIDEO_BT601_RGB2YUV};
    ret = HI_MPI_IVE_CSC(&iveHnd, src, dst, &stCSCCtrl, HI_TRUE);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), HI_MPI_IVE_CSC failed!\n", ret);

    ret = HI_MPI_IVE_Query(iveHnd, &finish, HI_TRUE);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != ret, ret, "Error(%#x), HI_MPI_IVE_Query failed!\n", ret);
    return ret;
}

/* ive image to video frame */
void SampleIveOrigImgToFrm(void)
{
    HI_U16 u32Width = 1920;
    HI_U16 u32Height = 1080;

    HI_CHAR* pchSrcFileName = "./userdata/data/input/color_convert_img/UsePic_1920_1080_420.yuv";
    HI_CHAR achDstFileName[IVE_FILE_NAME_LEN];
    VIDEO_FRAME_INFO_S frm;
    HI_S32 s32Ret;
    // Initialize the g_stColorSpaceInfo structure
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    if (snprintf_s(achDstFileName, sizeof(achDstFileName), sizeof(achDstFileName) - 1,
        "./userdata/data/output/color_convert_res/complete_%s.yuv", "ive_to_video") < 0) {
        HI_ASSERT(0);
    }

    s32Ret = SampleIveColorConvertInit(&g_stColorSpaceInfo, pchSrcFileName, achDstFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL,
        "Error(%#x), SampleIveColorConvertInit failed!\n", s32Ret);

    s32Ret = SampleIveReadFile(&g_stColorSpaceInfo);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), SampleIveReadFile failed!\n", s32Ret);

    s32Ret = OrigImgToFrm(&g_stColorSpaceInfo.stSrc, &frm);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n",
        g_stColorSpaceInfo.stSrc.u32Width, g_stColorSpaceInfo.stSrc.u32Height, g_stColorSpaceInfo.stSrc.enType);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("congratulate origImgToFrm success\n");
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), OrigImgToFrm failed!\n", s32Ret);

    memset_s(&frm, sizeof(VIDEO_FRAME_INFO_S), 0, sizeof(VIDEO_FRAME_INFO_S));
    SampleIveColorConvertUninit(&g_stColorSpaceInfo);
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));

CONVERT_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}

void SampleIveFrmToOrigImg(void)
{
    HI_U16 u32Width = 1920;
    HI_U16 u32Height = 1080;

    HI_CHAR* pchSrcFileName = "./userdata/data/input/color_convert_img/UsePic_1920_1080_420.yuv";
    HI_CHAR achDstFileName[IVE_FILE_NAME_LEN];
    VIDEO_FRAME_INFO_S frm;
    IVE_IMAGE_S img;
    HI_S32 s32Ret;
    // Initialize the g_stColorSpaceInfo structure
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    if (snprintf_s(achDstFileName, sizeof(achDstFileName), sizeof(achDstFileName) - 1,
        "./userdata/data/output/color_convert_res/complete_%s.yuv", "frm_orig_img") < 0) {
        HI_ASSERT(0);
    }

    s32Ret = SampleIveColorConvertInit(&g_stColorSpaceInfo, pchSrcFileName, achDstFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL,
        "Error(%#x), SampleIveColorConvertInit failed!\n", s32Ret);

    s32Ret = SampleIveReadFile(&g_stColorSpaceInfo);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), SampleIveReadFile failed!\n", s32Ret);

    s32Ret = OrigImgToFrm(&g_stColorSpaceInfo.stSrc, &frm);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n",
        g_stColorSpaceInfo.stSrc.u32Width, g_stColorSpaceInfo.stSrc.u32Height, g_stColorSpaceInfo.stSrc.enType);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), OrigImgToFrm failed!\n", s32Ret);

    s32Ret = FrmToOrigImg(&frm, &img);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n", img.u32Width, img.u32Height, img.enType);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("congratulate FrmToOrigImg success\n");
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), FrmToOrigImg failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&img, g_stColorSpaceInfo.pFpDst);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL,
        "Error(%#x), SAMPLE_COMM_IVE_WriteFile failed!\n", s32Ret);

    memset_s(&frm, sizeof(VIDEO_FRAME_INFO_S), 0, sizeof(VIDEO_FRAME_INFO_S));
    memset_s(&img, sizeof(IVE_IMAGE_S), 0, sizeof(IVE_IMAGE_S));
    SampleIveColorConvertUninit(&g_stColorSpaceInfo);
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));

CONVERT_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}

void SampleIveFrmToU8c1Img(void)
{
    HI_U16 u32Width = 1920;
    HI_U16 u32Height = 1080;

    HI_CHAR* pchSrcFileName = "./userdata/data/input/color_convert_img/UsePic_1920_1080_420.yuv";
    HI_CHAR achDstFileName[IVE_FILE_NAME_LEN];
    VIDEO_FRAME_INFO_S frm;
    IVE_IMAGE_S img;
    HI_S32 s32Ret;
    // Initialize the g_stColorSpaceInfo structure
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    if (snprintf_s(achDstFileName, sizeof(achDstFileName), sizeof(achDstFileName) - 1,
        "./userdata/data/output/color_convert_res/complete_%s.yuv", "u8c1") < 0) {
        HI_ASSERT(0);
    };

    s32Ret = SampleIveColorConvertInit(&g_stColorSpaceInfo, pchSrcFileName, achDstFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL,
        "Error(%#x), SampleIveColorConvertInit failed!\n", s32Ret);

    s32Ret = SampleIveReadFile(&g_stColorSpaceInfo);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), SampleIveReadFile failed!\n", s32Ret);

    s32Ret = OrigImgToFrm(&g_stColorSpaceInfo.stSrc, &frm);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n",
        g_stColorSpaceInfo.stSrc.u32Width, g_stColorSpaceInfo.stSrc.u32Height, g_stColorSpaceInfo.stSrc.enType);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), OrigImgToFrm failed!\n", s32Ret);

    s32Ret = FrmToU8c1Img(&frm, &img);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n", img.u32Width, img.u32Height, img.enType);
    LOGI("congratulate FrmToU8c1Img success\n");
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), FrmToU8c1Img failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&img, g_stColorSpaceInfo.pFpDst);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL,
        "Error(%#x), SAMPLE_COMM_IVE_WriteFile failed!\n", s32Ret);

    memset_s(&frm, sizeof(VIDEO_FRAME_INFO_S), 0, sizeof(VIDEO_FRAME_INFO_S));
    memset_s(&img, sizeof(IVE_IMAGE_S), 0, sizeof(IVE_IMAGE_S));
    SampleIveColorConvertUninit(&g_stColorSpaceInfo);
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));

CONVERT_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}

void SampleIveFrmToRgbImgToYuv(void)
{
    HI_U16 u32Width = 1920;
    HI_U16 u32Height = 1080;

    HI_CHAR* pchSrcFileName = "./userdata/data/input/color_convert_img/UsePic_1920_1080_420.yuv";
    HI_CHAR achDstFileName[IVE_FILE_NAME_LEN];
    VIDEO_FRAME_INFO_S frm;
    IVE_IMAGE_S img;
    IVE_IMAGE_S dst;
    HI_S32 s32Ret;
    // Initialize the g_stColorSpaceInfo structure
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    if (snprintf_s(achDstFileName, sizeof(achDstFileName), sizeof(achDstFileName) - 1,
        "./userdata/data/output/color_convert_res/complete_%s.yuv", "frm_rgb_yuv") < 0) {
        HI_ASSERT(0);
    }

    s32Ret = SampleIveColorConvertInit(&g_stColorSpaceInfo, pchSrcFileName, achDstFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL,
        "Error(%#x), SampleIveColorConvertInit failed!\n", s32Ret);

    s32Ret = SampleIveReadFile(&g_stColorSpaceInfo);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), SampleIveReadFile failed!\n", s32Ret);

    s32Ret = OrigImgToFrm(&g_stColorSpaceInfo.stSrc, &frm);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n",
        g_stColorSpaceInfo.stSrc.u32Width, g_stColorSpaceInfo.stSrc.u32Height, g_stColorSpaceInfo.stSrc.enType);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), OrigImgToFrm failed!\n", s32Ret);

    s32Ret = FrmToRgbImg(&frm, &img);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n", img.u32Width, img.u32Height, img.enType);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), FrmToRgbImg failed!\n", s32Ret);

    s32Ret = ImgRgbToYuv(&img, &dst, IVE_IMAGE_TYPE_YUV420SP);
    LOGI("congratulate ImgRgbToYuv success\n");
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), ImgRgbToYuv failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&dst, g_stColorSpaceInfo.pFpDst);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL,
        "Error(%#x), SAMPLE_COMM_IVE_WriteFile failed!\n", s32Ret);

    IveImgDestroy(&img);
    IveImgDestroy(&dst);
    memset_s(&frm, sizeof(VIDEO_FRAME_INFO_S), 0, sizeof(VIDEO_FRAME_INFO_S));
    SampleIveColorConvertUninit(&g_stColorSpaceInfo);
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));

CONVERT_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}

void SampleIveFrmToRgbImgToBgr(void)
{
    HI_U16 u32Width = 1920;
    HI_U16 u32Height = 1080;

    HI_CHAR* pchSrcFileName = "./userdata/data/input/color_convert_img/UsePic_1920_1080_420.yuv";
    HI_CHAR achDstFileName[IVE_FILE_NAME_LEN];
    VIDEO_FRAME_INFO_S frm;
    IVE_IMAGE_S img;
    HI_S32 s32Ret;
    // Initialize the g_stColorSpaceInfo structure
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    if (snprintf_s(achDstFileName, sizeof(achDstFileName), sizeof(achDstFileName) - 1,
        "./userdata/data/output/color_convert_res/complete_%s.bgr", "rgb2bgr") < 0) {
        HI_ASSERT(0);
    };

    s32Ret = SampleIveColorConvertInit(&g_stColorSpaceInfo, pchSrcFileName, achDstFileName, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL,
        "Error(%#x), SampleIveColorConvertInit failed!\n", s32Ret);

    s32Ret = SampleIveReadFile(&g_stColorSpaceInfo);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), SampleIveReadFile failed!\n", s32Ret);

    s32Ret = OrigImgToFrm(&g_stColorSpaceInfo.stSrc, &frm);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n",
        g_stColorSpaceInfo.stSrc.u32Width, g_stColorSpaceInfo.stSrc.u32Height, g_stColorSpaceInfo.stSrc.enType);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), OrigImgToFrm failed!\n", s32Ret);

    s32Ret = FrmToRgbImg(&frm, &img);
    LOGI("VIDEO_FRAME_INFO_S====width:%d, height:%d, entype:%d\n",
        frm.stVFrame.u32Width, frm.stVFrame.u32Height, frm.stVFrame.enPixelFormat);
    LOGI("IVE_IMAGE_S====width:%d, height:%d, entype:%d\n", img.u32Width, img.u32Height, img.enType);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), FrmToRgbImg failed!\n", s32Ret);

    s32Ret = ImgRgbToBgr(&img);
    LOGI("congratulate ImgRgbToBgr success\n");
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, CONVERT_FAIL, "Error(%#x), ImgRgbToBgr failed!\n", s32Ret);

    IveImgDestroy(&img);
    memset_s(&frm, sizeof(VIDEO_FRAME_INFO_S), 0, sizeof(VIDEO_FRAME_INFO_S));
    SampleIveColorConvertUninit(&g_stColorSpaceInfo);
    memset_s(&g_stColorSpaceInfo, sizeof(g_stColorSpaceInfo), 0, sizeof(g_stColorSpaceInfo));

CONVERT_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
