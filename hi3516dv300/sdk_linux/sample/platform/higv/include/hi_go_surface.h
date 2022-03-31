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

#ifndef HI_GO_SURFACE_H
#define HI_GO_SURFACE_H

#include "hi_go_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum {
    HIGO_PF_CLUT8 = 0,  /* Palette */
    HIGO_PF_CLUT1,
    HIGO_PF_CLUT4,
    HIGO_PF_4444,   /*
                     * Each pixel occupies 16 bits, and the A/R/G/B components each occupies 4 bits. They are sorted
                     * by address in descending order.
                     *
                     */
    HIGO_PF_0444,   /*
                     * Each pixel occupies 16 bits, and the A/R/G/B components each occupies 4 bits. They are sorted
                     * by address in descending order. The A component does not take effect.
                     * ռ16bitARGBÿռ4bitַɸ, A
                     */

    HIGO_PF_1555,   /*
                     * Each pixel occupies 16 bits, the R/G/B components each occupies 5 bits, and the A component
                     * occupies 1 bit. They are sorted by address in descending order.
                     * 16bitRGBÿռ5bitA1bitַɸ
                     */
    HIGO_PF_0555,   /*
                     * Each pixel occupies 16 bits, the R/G/B components each occupies 5 bits, and the A component
                     * occupies 1 bit. They are sorted by address in descending order. The A component does not take
                     * effect.
                     * A
                     */
    HIGO_PF_565,    /*
                     * Each pixel occupies 16 bits, and the R/G/B components each occupies 5 bits, 6 bits, and 5 bits
                     * respectively. They are sorted by address in descending order.
                     * 16bitRGBÿֱռ5bit6bit5bitַɸ
                     */
    HIGO_PF_8565,   /*
                     * Each pixel occupies 24 bits, and the A/R/G/B components each occupies 8 bits, 5 bits, 6 bits,
                     * and 5 bits respectively. They are sorted by address in descending order.
                     * ռ24bitARGBÿֱռ8bit, 5bit6bit5bitַɸ
                     */
    HIGO_PF_8888,   /*
                     * Each pixel occupies 32 bits, and the A/R/G/B components each occupies 8 bits. They are sorted
                     * by address in descending order.
                     *
                     */
    HIGO_PF_0888,   /*
                     * Each pixel occupies 24 bits, and the A/R/G/B components each occupies 8 bits. They are sorted
                     * by address in descending order. The A component does not take effect.
                     * ռ24bitARGBÿռ8bitַɸУA
                     */

    HIGO_PF_YUV400, /*
                     * Semi-planar YUV 400 format defined by HiSilicon
                     * YUV 400ʽ
                     */
    HIGO_PF_YUV420, /*
                     * Semi-planar YUV 420 format defined by HiSilicon
                     * YUV 420ʽ
                     */
    HIGO_PF_YUV422, /*
                     * Semi-planar YUV 422 format and horizontal sampling format defined by
                     *
                     */
    HIGO_PF_YUV422_V, /*
                       * Semi-planar YUV 422 format and vertical sampling format defined by
                       *
                       */
    HIGO_PF_YUV444, /*
                     * Semi-planar YUV 444 format defined by HiSilicon,
                     * ˼semi-planar YUV 444ʽ
                     */

    HIGO_PF_A1,
    HIGO_PF_A8,

    HIGO_PF_YUV888,
    HIGO_PF_YUV8888,
    HIGO_PF_RLE8,
    HIGO_PF_BUTT
} HIGO_PF_E;

/* Component type */
/* */
typedef enum {
    HIGO_PDFORMAT_RGB = 0,
    HIGO_PDFORMAT_RGB_PM,   /* pre-multiplied */
    HIGO_PDFORMAT_Y,
    HIGO_PDFORMAT_UV,
    HIGO_PDFORMAT_UV_LE,

    HIGO_PDFORMAT_BUTT
} HIGO_PDFORMAT_E;

typedef struct {
    HIGO_PDFORMAT_E Format;   /* Component type */
    HI_VOID        *pData;    /* Pointer to the virtual address of a component */
    HI_VOID        *pPhyData; /* Pointer to the physical address of a component */
    HI_U32          Pitch;    /* Component pitch */
    HI_U32          Bpp;      /* Bytes per pixel */
    HI_U32          Offset;
} HIGO_PIXELDATA_S;

/* Maximum number of data components */
/* */
#define MAX_PARTPIXELDATA 3

/* The following sync modes can be used together. The non-automatic sync mode is also available. */
/* */
typedef enum {
    HIGO_SYNC_MODE_CPU = 0x01, /* Sync mode. In this mode, CPU operations are required. */
                               /* */
    HIGO_SYNC_MODE_TDE = 0x02, /* Sync mode. In this mode, the 2D acceleration operation is required. */
                               /* */
    HIGO_SYNC_MODE_BUTT,
} HIGO_SYNC_MODE_E;

typedef enum {
    HIGO_MEMTYPE_MMZ = 0, /* The memory is an media memory zone (MMZ) memory. */
    HIGO_MEMTYPE_OS,      /* The memory is an operating system (OS) memory. */
    HIGO_MEMTYPE_BUTT
}HIGO_MEMTYPE_E;

typedef enum {
    HIGO_OWNER_USER = 0, /* The memory is managed by users. the memory is allocated and destroyed by users.
                          *
                          */
    HIGO_OWNER_HIGO,     /*
                          * The memory is managed by the HiGo module rather than users.
                          *
                          */
    HIGO_OWNER_BUTT
}HIGO_OWNER_E;

typedef struct {
    HI_S32 Width;                         /* Surface width */
    HI_S32 Height;                        /* Surface height */
    HIGO_PF_E PixelFormat;                /* Pixel format of a surface */
    HI_U32   Pitch[MAX_PARTPIXELDATA];    /* Pitch of a surface */
    HI_CHAR* pPhyAddr[MAX_PARTPIXELDATA]; /* Physical address of a surface */
    HI_CHAR* pVirAddr[MAX_PARTPIXELDATA]; /* Virtual address of a surface */
    HIGO_MEMTYPE_E MemType;               /* Type of the surface memory */
    HI_BOOL bPubPalette;                  /*
                                           * Use common Palette or not
                                           * ֻClutʽЧ.ʹùĵɫ,ʹԴĵɫ
                                           */
}HIGO_SURINFO_S;

typedef struct {
    HI_S32 Width;                               /* Surface width */
    HI_S32 Height;                              /* Surface height */
    HIGO_PF_E PixelFormat;                      /* Pixel format of a surface */

    /*
     *   Pitch of a surface
     *   Pitch[0] indicates the pitch in RGB format or the pitch of the Y component in semi-planar format.
     *   Pitch[1] indicates the pitch of the C component in semi-planar format.
     *   Pitch[2] is reserved.
     */
    /*
     *
     *   Pitch[0]ʾRGBʽм࣬ Semi-plannerYм
     *   Pitch[1]ʾSemi-plannerCм
     *   Pitch[2]ʱʹ
     */
    HI_U32   Pitch[MAX_PARTPIXELDATA];

    /*
     *   Physical address of a surface
     *   pPhyAddr[0] indicates the physical address in RGB format or the physical address of the Y component in
     *   semi-planar format.
     *   pPhyAddr[1] indicates the physical address of the C component in semi-planar format.
     */
    /*
     *
     *   pPhyAddr[0]ʾRGBʽַ Semi-plannerYַ
     *   pPhyAddr[1]ʾSemi-plannerCַ
     */
    HI_CHAR* pPhyAddr[MAX_PARTPIXELDATA];

    /*
     *  Virtual address of a surface
     *  pVirAddr[0] indicates the virtual address in RGB format or the virtual address of the Y component in semi-planar
     *   format.
     *  pVirAddr[1] indicates the virtual address of the C component in semi-planar format.
     */
    /*
     *
     *   pVirAddr[0]ʾRGBʽַ Semi-plannerYַ
     *   pVirAddr[1]ʾSemi-plannerCַ
     */
    HI_CHAR* pVirAddr[MAX_PARTPIXELDATA]; /*
                                           *
                                           * pVirAddr[0]ʾRGBʽַ Semi-plannerYַ
                                           * pVirAddr[1]ʾSemi-plannerCַ
                                           */
    HIGO_MEMTYPE_E MemType;     /* Type of the surface memory */
    HI_BOOL bPubPalette;        /* Use common Palette or not */
    HIGO_OWNER_E   MemOwner;    /* Memory source. For example, the memory is allocated by users or the HiGo module. */
                                /* */
}HIGO_SURINFOEX_S;

/* Data component structure */
/* */
typedef HIGO_PIXELDATA_S HI_PIXELDATA[MAX_PARTPIXELDATA];

HI_S32 HI_GO_InitSurface(HI_VOID);

HI_S32 HI_GO_DeinitSurface(HI_VOID);

HI_S32 HI_GO_SetSurfaceAlpha(HI_HANDLE Surface, HI_U8 Alpha);

HI_S32 HI_GO_GetSurfaceAlpha(HI_HANDLE Surface, HI_U8 *pAlpha);

HI_S32 HI_GO_EnableSurfaceColorKey(HI_HANDLE Surface, HI_BOOL Enable);

HI_S32 HI_GO_SetSurfaceColorKey(HI_HANDLE Surface, HI_COLOR ColorKey);

HI_S32 HI_GO_GetSurfaceColorKey(HI_HANDLE Surface, HI_COLOR *pColorKey);

HI_S32 HI_GO_SetSurfacePalette(HI_HANDLE Surface, const HI_PALETTE Palette);

HI_S32 HI_GO_GetSurfacePalette(HI_HANDLE Surface, HI_PALETTE Palette);

HI_S32 HI_GO_LockSurface(HI_HANDLE Surface, HI_PIXELDATA pData, HI_BOOL bSync);

HI_S32 HI_GO_UnlockSurface(HI_HANDLE Surface);

HI_S32 HI_GO_GetSurfaceSize(HI_HANDLE Surface, HI_S32 *pWidth, HI_S32 *pHeight);

HI_S32 HI_GO_GetSurfacePixelFormat(HI_HANDLE Surface, HIGO_PF_E *pPixelFormat);

HI_S32 HI_GO_CreateSurfaceFromMem(const HIGO_SURINFO_S *pSurInfo, HI_HANDLE *pSurface);

HI_S32 HI_GO_CreateSurface(HI_S32 Width, HI_S32 Height, HIGO_PF_E PixelFormat, HI_HANDLE *pSurface);

HI_S32 HI_GO_CreateSubSurface(HI_HANDLE Surface, const HI_RECT *pRect, HI_HANDLE *pSubSurface);

HI_S32 HI_GO_FreeSurface(HI_HANDLE Surface);

HI_S32 HI_GO_EnableSurfaceAutoSync(HI_HANDLE hSurface, HI_BOOL bAutoSync);

HI_S32 HI_GO_SyncSurface(HI_HANDLE hSurface, HIGO_SYNC_MODE_E mode);

HI_S32 HI_GO_SetSurfaceName(HI_HANDLE hSurface, const HI_CHAR *pName);

HI_S32 HI_GO_GetSurfaceName(HI_HANDLE hSurface,  HI_CHAR *pNameBuf, HI_U32 BufLen);

HI_S32 HI_GO_GetSurfaceMemType(HI_HANDLE Surface, HIGO_MEMTYPE_E *pMemType);

HI_S32 HI_GO_GetSurfaceOwner(HI_HANDLE Surface, HIGO_OWNER_E *pOwner);

HI_S32 HI_GO_CreateSurfaceEx(const HIGO_SURINFOEX_S  *pSurInfo, HI_HANDLE *pSurface);

HI_S32 HI_GO_SetPubPalette(const HI_PALETTE Palette);

HI_S32 HI_GO_GetPubPalette(HI_PALETTE Palette);

HI_S32 HI_GO_SetSurfaceClipRgn(HI_HANDLE Surface, const HI_REGION *pRegion, HI_U32 u32Num);

HI_S32 HI_GO_GetSurfaceClipRgn(HI_HANDLE Surface, HI_REGION **ppRegion, HI_U32 *pu32Num);

HI_S32 HI_GO_SetSurfaceClipRect(HI_HANDLE hSurface, const HI_RECT *pRect);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* HI_GO_SURFACE_H */
