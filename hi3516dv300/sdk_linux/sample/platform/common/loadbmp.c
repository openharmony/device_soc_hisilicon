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
#include <unistd.h>
#include <string.h>
#include "securec.h"
#include "loadbmp.h"

OSD_COMP_INFO s_OSDCompInfo[OSD_COLOR_FMT_BUTT] = {
    {0, 4, 4, 4}, /* RGB444 */
    {4, 4, 4, 4}, /* ARGB4444 */
    {0, 5, 5, 5}, /* RGB555 */
    {0, 5, 6, 5}, /* RGB565 */
    {1, 5, 5, 5}, /* ARGB1555 */
    {0, 0, 0, 0}, /* RESERVED */
    {0, 8, 8, 8}, /* RGB888 */
    {8, 8, 8, 8}  /* ARGB8888 */
};

static HI_U16 OSD_MAKECOLOR_U16(HI_U8 r, HI_U8 g, HI_U8 b, OSD_COMP_INFO compinfo)
{
    HI_U8 r1, g1, b1;
    HI_U16 pixel = 0;
    HI_U32 tmp = 15;

    r1 = r >> (8 - compinfo.rlen); /* 8bit */
    g1 = g >> (8 - compinfo.glen); /* 8bit */
    b1 = b >> (8 - compinfo.blen); /* 8bit */
    while (compinfo.alen) {
        pixel |= (1 << tmp);
        tmp--;
        compinfo.alen--;
    }

    pixel |= (r1 | (g1 << compinfo.blen) | (b1 << (compinfo.blen + compinfo.glen)));
    return pixel;
}

HI_S32 GetBmpInfo(const char *filename, OSD_BITMAPFILEHEADER *pBmpFileHeader, OSD_BITMAPINFO *pBmpInfo)
{
    FILE *pFile = HI_NULL;
    hi_char *path = HI_NULL;
    HI_U16 bfType;

    if ((filename == NULL) || (pBmpFileHeader == NULL) || (pBmpInfo == NULL)) {
        printf("null pointer\n");
        return -1;
    }

    path = realpath(filename, HI_NULL);
    if (path == HI_NULL) {
        return HI_FAILURE;
    }

    if ((pFile = fopen(path, "rb")) == NULL) {
        printf("Open file failed:%s!\n", filename);
        free(path);
        return -1;
    }

    (void)fread(&bfType, 1, sizeof(bfType), pFile);
    if (bfType != 0x4d42) {
        printf("not bitmap file\n");
        (HI_VOID)fclose(pFile);
        free(path);
        return -1;
    }

    (void)fread(pBmpFileHeader, 1, sizeof(OSD_BITMAPFILEHEADER), pFile);
    (void)fread(pBmpInfo, 1, sizeof(OSD_BITMAPINFO), pFile);
    (HI_VOID)fclose(pFile);
    free(path);

    return 0;
}

static int LoadBMP(const char *filename, OSD_LOGO_T *pVideoLogo)
{
    HI_S32 ret;
    FILE *pFile = HI_NULL;
    HI_U16 i, j;
    hi_char *path = HI_NULL;

    HI_U32 w, h;
    HI_U16 Bpp;
    HI_U16 dstBpp;

    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    HI_U8 *pOrigBMPBuf = HI_NULL;
    HI_U8 *pRGBBuf = HI_NULL;
    HI_U32 stride;

    if ((filename == NULL) || (pVideoLogo == NULL)) {
        printf("null pointer\n");
        return -1;
    }

    path = realpath(filename, HI_NULL);
    if (path == HI_NULL) {
        return HI_FAILURE;
    }

    if (GetBmpInfo(path, &bmpFileHeader, &bmpInfo) < 0) {
        free(path);
        return -1;
    }

    Bpp = bmpInfo.bmiHeader.biBitCount / 8; /* 8bit */
    if (Bpp < 2) { /* 2byte */
        /* only support 1555 8888 888 bitmap */
        printf("bitmap format not supported!\n");
        free(path);
        return -1;
    }

    if (bmpInfo.bmiHeader.biCompression != 0) {
        printf("not support compressed bitmap file!\n");
        free(path);
        return -1;
    }

    if (bmpInfo.bmiHeader.biHeight < 0) {
        printf("bmpInfo.bmiHeader.biHeight < 0\n");
        free(path);
        return -1;
    }

    if ((pFile = fopen(path, "rb")) == NULL) {
        printf("Open file failed:%s!\n", filename);
        free(path);
        return -1;
    }

    pVideoLogo->width = (HI_U16)bmpInfo.bmiHeader.biWidth;
    pVideoLogo->height =
        (HI_U16)((bmpInfo.bmiHeader.biHeight > 0) ? bmpInfo.bmiHeader.biHeight : (-bmpInfo.bmiHeader.biHeight));
    w = pVideoLogo->width;
    h = pVideoLogo->height;

    stride = w * Bpp;
    if (stride % 4) { /* 4 align */
        stride = (stride & 0xfffc) + 4; /* 4 align */
    }

    /* RGB8888 or RGB1555 */
    pOrigBMPBuf = (HI_U8 *)malloc(h * stride);
    if (pOrigBMPBuf == NULL) {
        printf("not enough memory to malloc!\n");
        (HI_VOID)fclose(pFile);
        free(path);
        return -1;
    }

    pRGBBuf = pVideoLogo->pRGBBuffer;

    (HI_VOID)fseek(pFile, bmpFileHeader.bfOffBits, 0);
    if (fread(pOrigBMPBuf, 1, h * stride, pFile) != (h * stride)) {
        printf("fread error!line:%d\n", __LINE__);
        perror("fread:");
    }

    if (Bpp > 2) { /* 2 byte */
        dstBpp = 4; /* 4 align */
    } else {
        dstBpp = 2; /* 2 align */
    }

    if (pVideoLogo->stride == 0) {
        pVideoLogo->stride = pVideoLogo->width * dstBpp;
    }

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            ret = memcpy_s(pRGBBuf + i * pVideoLogo->stride + j * dstBpp, Bpp,
                pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp, Bpp);
            if (ret != EOK) {
                free(pOrigBMPBuf);
                pOrigBMPBuf = NULL;
                (HI_VOID)fclose(pFile);
                free(path);
                printf("copy bmp failed!line:%d\n", __LINE__);
                return HI_FAILURE;
            }

            if (dstBpp == 4) { /* 4 align */
                *(pRGBBuf + i * pVideoLogo->stride + j * dstBpp + 3) = 0x80; /* alpha + 3 */
            }
        }
    }

    free(pOrigBMPBuf);
    pOrigBMPBuf = NULL;

    (HI_VOID)fclose(pFile);
    free(path);
    return 0;
}

static int LoadBMPEx(const char *filename, OSD_LOGO_T *pVideoLogo, OSD_COLOR_FMT_E enFmt)
{
    HI_S32 ret;
    FILE *pFile = HI_NULL;
    HI_U16 i, j;
    hi_char *path = HI_NULL;

    HI_U32 w, h;
    HI_U16 Bpp;

    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    HI_U8 *pOrigBMPBuf = HI_NULL;
    HI_U8 *pRGBBuf = HI_NULL;
    HI_U32 stride;
    HI_U8 r, g, b;
    HI_U8 *pStart = HI_NULL;
    HI_U16 *pDst = HI_NULL;

    if ((filename == NULL) || (pVideoLogo== NULL)) {
        printf("null pointer\n");
        return -1;
    }

    path = realpath(filename, HI_NULL);
    if (path == HI_NULL) {
        return HI_FAILURE;
    }

    if (GetBmpInfo(path, &bmpFileHeader, &bmpInfo) < 0) {
        free(path);
        return -1;
    }

    Bpp = bmpInfo.bmiHeader.biBitCount / 8; /* 8bit */
    if (Bpp < 2) { /* 2 byte */
        /* only support 1555.8888  888 bitmap */
        printf("bitmap format not supported!\n");
        free(path);
        return -1;
    }

    if (bmpInfo.bmiHeader.biCompression != 0) {
        printf("not support compressed bitmap file!\n");
        free(path);
        return -1;
    }

    if (bmpInfo.bmiHeader.biHeight < 0) {
        printf("bmpInfo.bmiHeader.biHeight < 0\n");
        free(path);
        return -1;
    }

    if ((pFile = fopen(path, "rb")) == NULL) {
        printf("Open file failed:%s!\n", filename);
        free(path);
        return -1;
    }

    pVideoLogo->width = (HI_U16)bmpInfo.bmiHeader.biWidth;
    pVideoLogo->height =
        (HI_U16)((bmpInfo.bmiHeader.biHeight > 0) ? bmpInfo.bmiHeader.biHeight : (-bmpInfo.bmiHeader.biHeight));
    w = pVideoLogo->width;
    h = pVideoLogo->height;

    stride = w * Bpp;
    if (stride % 4) { /* 4 align */
        stride = (stride & 0xfffc) + 4; /* 4 align */
    }

    /* RGB8888 or RGB1555 */
    pOrigBMPBuf = (HI_U8 *)malloc(h * stride);
    if (pOrigBMPBuf == NULL) {
        printf("not enough memory to malloc!\n");
        (HI_VOID)fclose(pFile);
        free(path);
        return -1;
    }

    pRGBBuf = pVideoLogo->pRGBBuffer;

    (HI_VOID)fseek(pFile, bmpFileHeader.bfOffBits, 0);
    if (fread(pOrigBMPBuf, 1, h * stride, pFile) != (h * stride)) {
        printf("fread (%d*%d)error!line:%d\n", h, stride, __LINE__);
        perror("fread:");
    }

    if (enFmt >= OSD_COLOR_FMT_RGB888) {
        pVideoLogo->stride = pVideoLogo->width * 4; /* 4 * width */
    } else {
        pVideoLogo->stride = pVideoLogo->width * 2; /* 2 * width */
    }

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            if (Bpp == 3) { /* 3 byte */
                switch (enFmt) {
                    case OSD_COLOR_FMT_RGB444:
                    case OSD_COLOR_FMT_RGB555:
                    case OSD_COLOR_FMT_RGB565:
                    case OSD_COLOR_FMT_RGB1555:
                    case OSD_COLOR_FMT_RGB4444:
                        /* start color convert */
                        pStart = pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp;
                        pDst = (HI_U16 *)(pRGBBuf + i * pVideoLogo->stride + j * 2); /* one step 2 */
                        r = *(pStart); /* [0] */
                        g = *(pStart + 1); /* [1] */
                        b = *(pStart + 2); /* [2] */
                        *pDst = OSD_MAKECOLOR_U16(r, g, b, s_OSDCompInfo[enFmt]);
                        break;

                    case OSD_COLOR_FMT_RGB888:
                    case OSD_COLOR_FMT_RGB8888:
                        ret = memcpy_s(pRGBBuf + i * pVideoLogo->stride + j * 4, Bpp, /* one step 4 */
                            pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp, Bpp);
                        if (ret != EOK) {
                            free(pOrigBMPBuf);
                            pOrigBMPBuf = NULL;
                            (HI_VOID)fclose(pFile);
                            free(path);
                            printf("copy bmp failed!line:%d\n", __LINE__);
                            return HI_FAILURE;
                        }
                        *(pRGBBuf + i * pVideoLogo->stride + j * 4 + 3) = 0xff; /* one step 4 + 3 */
                        break;

                    default:
                        printf("file(%s), line(%d), no such format!\n", __FILE__, __LINE__);
                        break;
                }
            } else if ((Bpp == 2) || (Bpp == 4)) { /* 2 or 4 byte */
                ret = memcpy_s(pRGBBuf + i * pVideoLogo->stride + j * Bpp, Bpp,
                    pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp, Bpp);
                if (ret != EOK) {
                    free(pOrigBMPBuf);
                    pOrigBMPBuf = NULL;
                    (HI_VOID)fclose(pFile);
                    free(path);
                    printf("copy bmp failed!line:%d\n", __LINE__);
                    return HI_FAILURE;
                }
            }
        }
    }

    free(pOrigBMPBuf);
    pOrigBMPBuf = NULL;

    (HI_VOID)fclose(pFile);
    free(path);
    return 0;
}


static int LoadBMPCanvas(const char *filename, const OSD_LOGO_T *pVideoLogo, OSD_COLOR_FMT_E enFmt)
{
    HI_S32 ret;
    FILE *pFile = HI_NULL;
    HI_U16 i, j;
    hi_char *path = HI_NULL;

    HI_U32 w, h;
    HI_U16 Bpp;

    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    HI_U8 *pOrigBMPBuf = HI_NULL;
    HI_U8 *pRGBBuf = HI_NULL;
    HI_U32 stride;
    HI_U8 r, g, b;
    HI_U8 *pStart = HI_NULL;
    HI_U16 *pDst = HI_NULL;

    if (filename == NULL) {
        printf("OSD_LoadBMP: filename=NULL\n");
        return -1;
    }

    path = realpath(filename, HI_NULL);
    if (path == HI_NULL) {
        return HI_FAILURE;
    }

    if (GetBmpInfo(path, &bmpFileHeader, &bmpInfo) < 0) {
        free(path);
        return -1;
    }

    Bpp = bmpInfo.bmiHeader.biBitCount / 8; /* 8bit */
    if (Bpp < 2) { /* 2 byte */
        /* only support 1555.8888  888 bitmap */
        printf("bitmap format not supported!\n");
        free(path);
        return -1;
    }

    if (bmpInfo.bmiHeader.biCompression != 0) {
        printf("not support compressed bitmap file!\n");
        free(path);
        return -1;
    }

    if (bmpInfo.bmiHeader.biHeight < 0) {
        printf("bmpInfo.bmiHeader.biHeight < 0\n");
        free(path);
        return -1;
    }

    if ((pFile = fopen(path, "rb")) == NULL) {
        printf("Open file failed:%s!\n", filename);
        free(path);
        return -1;
    }

    w = (HI_U16)bmpInfo.bmiHeader.biWidth;
    h = (HI_U16)((bmpInfo.bmiHeader.biHeight > 0) ? bmpInfo.bmiHeader.biHeight : (-bmpInfo.bmiHeader.biHeight));

    stride = w * Bpp;

    if (stride % 4) { /* 4 align */
        stride = (stride & 0xfffc) + 4; /* 4 align */
    }

    /* RGB8888 or RGB1555 */
    pOrigBMPBuf = (HI_U8 *)malloc(h * stride);
    if (pOrigBMPBuf == NULL) {
        printf("not enough memory to malloc!\n");
        (HI_VOID)fclose(pFile);
        free(path);
        return -1;
    }

    pRGBBuf = pVideoLogo->pRGBBuffer;

    if (stride > pVideoLogo->stride) {
        printf("Bitmap's stride(%d) is bigger than canvas's stide(%d). Load bitmap error!\n", stride,
            pVideoLogo->stride);
        free(pOrigBMPBuf);
        (HI_VOID)fclose(pFile);
        free(path);
        return -1;
    }

    if (h > pVideoLogo->height) {
        printf("Bitmap's height(%d) is bigger than canvas's height(%d). Load bitmap error!\n", h, pVideoLogo->height);
        free(pOrigBMPBuf);
        (HI_VOID)fclose(pFile);
        free(path);
        return -1;
    }

    if (w > pVideoLogo->width) {
        printf("Bitmap's width(%d) is bigger than canvas's width(%d). Load bitmap error!\n", w, pVideoLogo->width);
        free(pOrigBMPBuf);
        (HI_VOID)fclose(pFile);
        free(path);
        return -1;
    }

    (HI_VOID)fseek(pFile, bmpFileHeader.bfOffBits, 0);
    if (fread(pOrigBMPBuf, 1, h * stride, pFile) != (h * stride)) {
        printf("fread (%d*%d)error!line:%d\n", h, stride, __LINE__);
        perror("fread:");
    }

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            if (Bpp == 3) { /* 3 byte */
                switch (enFmt) {
                    case OSD_COLOR_FMT_RGB444:
                    case OSD_COLOR_FMT_RGB555:
                    case OSD_COLOR_FMT_RGB565:
                    case OSD_COLOR_FMT_RGB1555:
                    case OSD_COLOR_FMT_RGB4444:
                        /* start color convert */
                        pStart = pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp;
                        pDst = (HI_U16 *)(pRGBBuf + i * pVideoLogo->stride + j * 2); /* one step 2 */
                        r = *(pStart); /* [0] */
                        g = *(pStart + 1); /* [1] */
                        b = *(pStart + 2); /* [2] */
                        *pDst = OSD_MAKECOLOR_U16(r, g, b, s_OSDCompInfo[enFmt]);

                        break;

                    case OSD_COLOR_FMT_RGB888:
                    case OSD_COLOR_FMT_RGB8888:
                        ret = memcpy_s(pRGBBuf + i * pVideoLogo->stride + j * 4, Bpp, /* one step 4 */
                            pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp, Bpp);
                        if (ret != EOK) {
                            free(pOrigBMPBuf);
                            pOrigBMPBuf = NULL;
                            (HI_VOID)fclose(pFile);
                            free(path);
                            printf("copy bmp failed!line:%d\n", __LINE__);
                            return HI_FAILURE;
                        }
                        *(pRGBBuf + i * pVideoLogo->stride + j * 4 + 3) = 0xff; /* one step 4 + 3 */
                        break;

                    default:
                        printf("file(%s), line(%d), no such format!\n", __FILE__, __LINE__);
                        break;
                }
            } else if ((Bpp == 2) || (Bpp == 4)) { /* 2 or 4 byte */
                ret = memcpy_s(pRGBBuf + i * pVideoLogo->stride + j * Bpp, Bpp,
                    pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp, Bpp);
                if (ret != EOK) {
                    free(pOrigBMPBuf);
                    pOrigBMPBuf = NULL;
                    (HI_VOID)fclose(pFile);
                    free(path);
                    printf("copy bmp failed!line:%d\n", __LINE__);
                    return HI_FAILURE;
                }
            }
        }
    }

    free(pOrigBMPBuf);
    pOrigBMPBuf = NULL;

    (HI_VOID)fclose(pFile);
    free(path);
    return 0;
}

static char *GetExtName(char *filename)
{
    char *pret = HI_NULL;
    HI_U32 fnLen;

    if (filename == NULL) {
        printf("filename can't be null!");
        return NULL;
    }

    fnLen = strlen(filename);
    while (fnLen) {
        pret = filename + fnLen;
        if (*pret == '.') {
            return (pret + 1);
        }

        fnLen--;
    }

    return pret;
}

int LoadImage(const char *filename, OSD_LOGO_T *pVideoLogo)
{
    char *ext = GetExtName((char *)filename);

    if ((filename == HI_NULL) || (pVideoLogo == HI_NULL) || (ext == NULL)) {
        printf("null pointer!\n");
        return -1;
    }
    if (strcmp(ext, "bmp") == 0) {
        if (LoadBMP(filename, pVideoLogo) != 0) {
            printf("OSD_LoadBMP error!\n");
            return -1;
        }
    } else {
        printf("not supported image file!\n");
        return -1;
    }

    return 0;
}

static int LoadImageEx(const char *filename, OSD_LOGO_T *pVideoLogo, OSD_COLOR_FMT_E enFmt)
{
    char *ext = GetExtName((char *)filename);

    if (ext == HI_NULL) {
        printf("LoadImageEx error!\n");
        return -1;
    }

    if (strcmp(ext, "bmp") == 0) {
        if (LoadBMPEx(filename, pVideoLogo, enFmt) != 0) {
            printf("OSD_LoadBMP error!\n");
            return -1;
        }
    } else {
        printf("not supported image file!\n");
        return -1;
    }

    return 0;
}


static int LoadCanvasEx(const char *filename, const OSD_LOGO_T *pVideoLogo, OSD_COLOR_FMT_E enFmt)
{
    char *ext = GetExtName((char *)filename);

    if (ext == HI_NULL) {
        printf("LoadCanvasEx error!\n");
        return -1;
    }

    if (strcmp(ext, "bmp") == 0) {
        if (LoadBMPCanvas(filename, pVideoLogo, enFmt) != 0) {
            printf("OSD_LoadBMP error!\n");
            return -1;
        }
    } else {
        printf("not supported image file!\n");
        return -1;
    }

    return 0;
}


HI_S32 LoadBitMap2Surface(const HI_CHAR *pszFileName, const OSD_SURFACE_S *pstSurface, HI_U8 *pu8Virt)
{
    OSD_LOGO_T stLogo;

    if ((pszFileName == NULL) || (pstSurface == NULL) || (pu8Virt == NULL)) {
        printf("null pointer\n");
        return HI_FAILURE;
    }
    stLogo.stride = pstSurface->u16Stride;
    stLogo.pRGBBuffer = pu8Virt;

    return LoadImage(pszFileName, &stLogo);
}

HI_S32 CreateSurfaceByBitMap(const HI_CHAR *pszFileName, OSD_SURFACE_S *pstSurface, HI_U8 *pu8Virt)
{
    OSD_LOGO_T stLogo;

    if ((pszFileName == NULL) || (pstSurface == NULL) || (pu8Virt == NULL)) {
        printf("null pointer\n");
        return -1;
    }
    stLogo.pRGBBuffer = pu8Virt;
    if (LoadImageEx(pszFileName, &stLogo, pstSurface->enColorFmt) < 0) {
        printf("load bmp error!\n");
        return -1;
    }

    pstSurface->u16Height = stLogo.height;
    pstSurface->u16Width = stLogo.width;
    pstSurface->u16Stride = stLogo.stride;

    return 0;
}

HI_S32 CreateSurfaceByCanvas(const HI_CHAR *pszFileName, OSD_SURFACE_S *pstSurface, HI_U8 *pu8Virt, HI_U32 u32Width,
    HI_U32 u32Height, HI_U32 u32Stride)
{
    OSD_LOGO_T stLogo;

    if ((pstSurface == NULL) || (pu8Virt == NULL) || (pszFileName == NULL)) {
        printf("null pointer\n");
        return HI_FAILURE;
    }
    stLogo.pRGBBuffer = pu8Virt;
    stLogo.width = u32Width;
    stLogo.height = u32Height;
    stLogo.stride = u32Stride;
    if (LoadCanvasEx(pszFileName, &stLogo, pstSurface->enColorFmt) < 0) {
        printf("load bmp error!\n");
        return -1;
    }

    pstSurface->u16Height = u32Height;
    pstSurface->u16Width = u32Width;
    pstSurface->u16Stride = u32Stride;

    return 0;
}
