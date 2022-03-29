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

/*
 * \brief Initializes the surface module.
 * \attention \n
 * When ::HI_GO_Init is called, this API is also called.
 *
 * \param N/A
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 *
 * \see \n
 * ::HI_GO_DeinitSurface
 */
HI_S32 HI_GO_InitSurface(HI_VOID);

/*
 * \brief Deinitializes the surface module.
 * \attention \n
 * When ::HI_GO_Deinit is called, this API is also called.
 *
 * \param N/A
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 *
 * \see \n
 * ::HI_GO_InitSurface
 */
HI_S32 HI_GO_DeinitSurface(HI_VOID);

/*
 * \brief Sets the alpha value of a surface.
 * \attention \n
 * N/A
 * \param[in]  Surface Surface handle.
 * \param[in]  Alpha Alpha value, ranging from 0 to 255. The value 0 indicates transparent, and the value 255 indicates
 *             opaque.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_INVHANDLE
 *
 * \see \n
 * ::HI_GO_GetSurfaceAlpha
 */
HI_S32 HI_GO_SetSurfaceAlpha(HI_HANDLE Surface, HI_U8 Alpha);

/*
 * \brief Obtains the alpha value of a surface.
 * \attention \n
 * N/A
 * \param[in]  Surface Surface handle.
 * \param[out] pAlpha Pointer to the received alpha value. The pointer cannot be null.
 * Ϊ CNend
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 *
 * \see \n
 * ::HI_GO_SetSurfaceAlpha
 */
HI_S32 HI_GO_GetSurfaceAlpha(HI_HANDLE Surface, HI_U8 *pAlpha);

/*
 * \brief Enables or disables the colorkey of a surface.
 * \attention \n
 * N/A
 * \param[in] Surface Surface handle.
 * \param[in] Enable Colorkey enable. HI_TRUE: enabled; HI_FALSE: disabled.
 *            HI_FALSEʹ CNend
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_EnableSurfaceColorKey(HI_HANDLE Surface, HI_BOOL Enable);

/*
 * \brief Sets the colorkey value of a surface.
 * \attention \n
 * N/A
 * \param[in] Surface Surface handle.
 * \param[in] ColorKey Colorkey value. For the RGB format, the colorkey is padded with 32-bit colors.
 *            For the CLUT format, the colorkey is padded with color index.
 *
 *            CLUTʽʹɫ䡣CNend
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_INVHANDLE
 *
 * \see \n
 * ::HI_GO_GetSurfaceColorKey
 */
HI_S32 HI_GO_SetSurfaceColorKey(HI_HANDLE Surface, HI_COLOR ColorKey);

/*
 * \brief Obtains the colorkey value of a surface.
 * \attention \n
 * N/A
 * \param[in]  Surface Surface handle.
 * \param[out] pColorKey Pointer to the received colorkey value. The value cannot be empty.
 *
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_NOCOLORKEY
 *
 * \see \n
 * ::HI_GO_SetSurfaceColorKey
 */
HI_S32 HI_GO_GetSurfaceColorKey(HI_HANDLE Surface, HI_COLOR *pColorKey);

/*
 * \brief Sets the palette of a surface.
 * \attention \n
 * N/A
 * \param[in] Surface Surface handle.
 * \param[in] Palette Palette.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVSURFACEPF
 *
 * \see \n
 * ::HI_GO_GetSurfaceColorKey
 */
HI_S32 HI_GO_SetSurfacePalette(HI_HANDLE Surface, const HI_PALETTE Palette);

/*
 * \brief Obtains the palette of a surface.
 * \attention \n
 * N/A
 * \param[in]  Surface Surface handle.
 * \param[out] Palette Pointer to the received palette.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVSURFACEPF
 *
 * \see \n
 * ::HI_GO_SetSurfacePalette
 */
HI_S32 HI_GO_GetSurfacePalette(HI_HANDLE Surface, HI_PALETTE Palette);

/*
 * \brief Locks a surface and obtains its memory pointer.
 * \attention \n
 * Before accessing a surface, you need to call the API to lock the surface.\n
 * You cannot lock the same surface repeatedly.
 *
 * ܶͬһsurfaceظ CNend
 * \param[in] Surface Surface handle.
 * \param[out] pData Pixel format information related to memory accessing.
 * \param[in] bSync  Synchronization.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_LOCKED
 *
 * \see \n
 * ::HI_GO_UnlockSurface
 */
HI_S32 HI_GO_LockSurface(HI_HANDLE Surface, HI_PIXELDATA pData, HI_BOOL bSync);

/*
 * \brief Unlocks a surface.
 * \attention \n
 * After accessing a surface, you need to call the API to unlock it.
 *
 * \param[in] Surface Surface handle.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NOTLOCKED
 *
 * \see \n
 * ::HI_GO_LockSurface
 */
HI_S32 HI_GO_UnlockSurface(HI_HANDLE Surface);

/*
 * \brief Obtains the dimensions of a surface.
 * \attention \n
 * pWidth and pHeight cannot be empty concurrently.
 *
 * \param[in]  Surface Surface handle.
 * \param[out]  pWidth Width output address of a surface. The value cannot be empty.
 *
 * \param[out]  pHeight Height output address of a surface. The value cannot be empty.
 *
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_GetSurfaceSize(HI_HANDLE Surface, HI_S32 *pWidth, HI_S32 *pHeight);

/*
 * \brief Obtains the pixel format of a surface.
 * \attention \n
 * N/A
 * \param[in]  Surface Surface Surface handle.
 * \param[out] pPixelFormat Output address of the pixel format. The value cannot be empty.
 *
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_GetSurfacePixelFormat(HI_HANDLE Surface, HIGO_PF_E *pPixelFormat);

/*
 * \brief Encapsulates the user memory into a surface.
 * \attention \n
 * All the attributes of pSurInfo must be correct. The supported input pixel formats include RGB format, CLUT8 format,
 * and YUV semi-planar format.
 *
 *
 * \param[in]  pSurInfo User memory information. The value cannot be empty.
 * \param[out] pSurface Handle information. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVSURFACESIZE
 * \retval ::HIGO_ERR_INVSURFACEPF
 * \retval ::HIGO_ERR_INVPARAM
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NOMEM
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_CreateSurfaceFromMem(const HIGO_SURINFO_S *pSurInfo, HI_HANDLE *pSurface);

/*
 * \brief Creates a memory surface.
 * \attention \n
 * The surface in the format of HIGO_PF_CLUT1, HIGO_PF_CLUT4, HIGO_PF_YUV420, or HIGO_PF_YUV422 cannot be created.
 *
 *
 * \param[in] Width Surface width.
 * \param[in] Height Surface height.
 * \param[in] PixelFormat Surface pixel format.
 * \param[out] pSurface Surface handle.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVSURFACESIZE
 * \retval ::HIGO_ERR_INVSURFACEPF
 * \retval ::HIGO_ERR_NOMEM
 *
 * \see \n
 * ::HI_GO_FreeSurface
 */
HI_S32 HI_GO_CreateSurface(HI_S32 Width, HI_S32 Height, HIGO_PF_E PixelFormat, HI_HANDLE *pSurface);

/*
 * \brief Creates a child surface that shares the same memory with its parent surface.
 *
 * \attention \n
 * N/A
 *
 * \param[in] Surface Parent surface handle.
 * \param[in] pRect Region of the child surface in the parent surface. The value cannot be empty.
 *            surfaceе򣬲Ϊ CNend
 * \param[out] pSubSurface Child surface handle. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_NOMEM
 *
 * \see \n
 * ::HI_GO_FreeSurface
 */
HI_S32 HI_GO_CreateSubSurface(HI_HANDLE Surface, const HI_RECT *pRect, HI_HANDLE *pSubSurface);

/*
 * \brief Destroys a surface.
 * \attention \n
 * N/A
 * \param[in] Surface Surface handle.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INUSE
 *
 * \see \n
 * ::HI_GO_CreateSurface \n
 * ::HI_GO_DecImgData
 */
HI_S32 HI_GO_FreeSurface(HI_HANDLE Surface);

/*
 * \brief Controls whether to synchronize the drawing operations based on a surface.
 *
 * \attention \n
 *      When a surface is created, the operations based on the surface are synchronized by default. The synchronization
 *      indicates that you can draw graphics by using the two-dimensional engine (TDE) only after the contents in the
 *      cache drawn by the CPU are updated to the surface memory. In addition, you can draw graphics by using the CPU
 *      only after the TDE completes the drawing operation.
 *      This API is applicable to all the operations related to the surface.
 *
 *      µsurfaceڴУʹCPU֮ǰҪTDEɡ
 *      ýӿڶsurfaceĲЧ CNend
 *
 * \param[in] hSurface Surface handle, not used.
 * \param[in] bAutoSync Automatic sync enable.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_EnableSurfaceAutoSync(HI_HANDLE hSurface, HI_BOOL bAutoSync);

/*
 * \brief Synchronizes the operations performed on a surface. In this way, the contents drawn by the CPU or TDE are
          updated to the surface memory.
 *
 * \attention \n
 * If you disable the automatic sync function by calling ::HI_GO_EnableSurfaceAutoSync, you need to synchronize the
 * operations by calling HI_S32 HI_GO_SyncSurface when drawing graphics using the TDE or CPU.
 * Otherwise, an error occurs during drawing.
 *
 * ƽȷCNend
 * \param[in] hSurface Surface handle, not used.
 * \param[in] mode Sync mode. For details, see the description of ::HIGO_SYNC_MODE_E.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_SyncSurface(HI_HANDLE hSurface, HIGO_SYNC_MODE_E mode);

/*
 * \brief Sets the name of a surface. After setting the surface name, you can view the internal information about the
 *        surface (such as memory usage, width, height, and pixel format) by running cat /proc/higo in the command line
 *        window.
 *
 * ڲϢڴռãߣظʽ CNend
 * \attention \n
 * The name contains a maximum of 16 characters including the end character '/0'.
 *
 *
 * \param[in] hSurface Surface handle.
 * \param[in] pName String of a surface name.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_SetSurfaceName(HI_HANDLE hSurface, const HI_CHAR *pName);

/*
 * \brief Obtains the name of a surface.
 * \attention \n
 * The name contains a maximum of 16 characters including the end character '/0'.
 *
 *
 * \param[in] hSurface Surface handle.
 * \param[in] pNameBuf Buffer for storing names.
 * \param[in] BufLen Buffer size.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVPARAM
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_GetSurfaceName(HI_HANDLE hSurface,  HI_CHAR *pNameBuf, HI_U32 BufLen);

/*
 * \brief Obtains the memory type.
 * \attention \n
 *
 * \param[in]  Surface Surface handle.
 * \param[out]  pMemType Pointer to the memory type. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_GetSurfaceMemType(HI_HANDLE Surface, HIGO_MEMTYPE_E *pMemType);


/*
 * \brief Queries the memory type.
 * \attention \n
 *
 * \param[in]   Surface  surface
 * \param[out]  pOwner   Pointer to the owner type. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_GetSurfaceOwner(HI_HANDLE Surface, HIGO_OWNER_E *pOwner);

/*
 * \brief Creates an OS surface.
 * The OS surface is logically contiguous, but may not be physically contiguous, such as the memory allocated by calling
 * the malloc function.
 * The OS surface can be identified by some components of the HiGo, but cannot be identified by the hardware. When the
 * MMZ is insufficient, the OS surface can be used.
 *
 * ߼һڴ򣨵ϲһûֱӵmalloc򣩣\n
 * ⲻܱӲʶ𣬵HIGOĳЩǿԱʶģͿĳ̶ֳϻMMZڴĲ CNend
 *
 * \attention \n
 * Note the following points when using the OS surface:
 * 1) The OS surface can be used during the BMP, GIF, or PNG software decoding when the picture is not scaled and the
 *    picture format is not converted.
 * 2) The RGB data stored in the surface can be encoded as a BMP file.
 * 3) Texts can be output to the OS surface.
 * 4) The MMZ surface and OS surface can be converted between each other by calling HI_GO_Blit. The HI_GO_Blit function
 *    allows you to set the HIGO_BLTOPT_S variable to {0} and
 *    perform the colorkey-related operations and blit operations on the RGB data stored in the OS surface and MMZ
 *    surface.
 *    Other operations are not supported.
 * 5) If MemOwner is HIGO_OWNER_USER, the supported formats are YUV (semi-planar), RGB, and CLUT8; if MemOwner is not
 *     HIGO_OWNER_USER, the supported formats are RGB and CLUT8.
 * The following describes how to create the surfaces that store the data in common YUV formats:
 * HI_U32 u32AlignWidth_Y,u32AlignHeight_Y; //Pitch of the Y component after byte alignment
 * HI_U32 u32AlignWidth_C,u32AlignHeight_C; //Pitch of the UV component after byte alignment
 * HI_U32 uExpectWidth,uExpectHeight;       //Size of the source picture or the expected size of the picture
 * HIGO_PF_E szPixelFormat;                 //Pixel format of the source picture or the expected pixel format of the
 * picture, such as HIGO_PF_YUV400, HIGO_PF_YUV420, HIGO_PF_YUV422, HIGO_PF_YUV422_V, or HIGO_PF_YUV444
 * HIGO_SURINFOEX_S SurfaceInfoEx assign to {0};
 *
 * SurfaceInfoEx.Width assign to uExpectWidth;
 * SurfaceInfoEx.Height assign to uExpectHeight;
 * SurfaceInfoEx.PixelFormat assign to szPixelFormat;
 * SurfaceInfoEx.MemType assign to HIGO_MEMTYPE_MMZ;
 * SurfaceInfoEx.MemOwner assign to HIGO_OWNER_USER;
 *
 * SurfaceInfoEx.Pitch[0] assign to u32AlignWidth_Y;
 * SurfaceInfoEx.pPhyAddr[0] assign to HI_MMZ_New(SurfaceInfoEx.Pitch[0]*u32AlignHeight_Y, 4, NULL, "higoSurface");
 * SurfaceInfoEx.pVirAddr[0] assign to HI_MMZ_Map((HI_U32)SurfaceInfoEx.pPhyAddr[0], 0);
 *
 * SurfaceInfoEx.Pitch[1] assign to u32AlignWidth_C;
 * SurfaceInfoEx.pPhyAddr[1] assign to HI_MMZ_New(SurfaceInfoEx.Pitch[1]*u32AlignHeight_C, 4, NULL, "higoSurfaceCbCr");
 * SurfaceInfoEx.pVirAddr[1] assign to HI_MMZ_Map((HI_U32)SurfaceInfoEx.pPhyAddr[1], 0);
 *
 * ret assign to HI_GO_CreateSurfaceEx(&SurfaceInfoEx, &picSurface);
 *
 * ---------------------------------------------------------------------------------------
 * The following describes the values of u32AlignWidth_Y, u32AlignHeight_Y, u32AlignWidth_C, and u32AlignHeight_C
 * based on the value of szPixelFormat:
 * 1) If szPixelFormat is YUV400:
 * u32AlignWidth_Y assign to (uExpectWidth + 127) + (~(127));       //128-byte alignment
 * u32AlignHeight_Y assign to (uExpectHeight + 7) + (~(7));        //8-byte alignment
 * u32AlignWidth_C assign to 0;
 * u32AlignHeight_C assign to0;
 * 2) If szPixelFormat is YUV420:
 * u32AlignWidth_Y assign to (uExpectWidth + 127) + (~(127));       //128-byte alignment
 * u32AlignHeight_Y assign to (uExpectHeight + 15) + (~(15));       //16-byte alignment
 * u32AlignWidth_C assign to u32AlignWidth_Y;
 * u32AlignHeight_C assign to u32AlignHeight_Y/2;
 * 3) If szPixelFormat is YUV422:
 * u32AlignWidth_Y assign to (uExpectWidth + 127) + (~(127));       //128-byte alignment
 * u32AlignHeight_Y assign to (uExpectHeight + 7) + (~(7));         //8-byte alignment
 * u32AlignWidth_C assign to u32AlignWidth_Y;
 * u32AlignHeight_C assign to u32AlignHeight_Y;
 * 4) If szPixelFormat is YUV422_V:
 * u32AlignWidth_Y assign to (uExpectWidth + 127) + (~(127));       //128-byte alignment
 * u32AlignHeight_Y assign to (uExpectHeight + 15) + (~(15));       //16-byte alignment
 * u32AlignWidth_C assign to u32AlignWidth_Y*2;
 * u32AlignHeight_C assign to u32AlignHeight_Y/2;
 * 5) If szPixelFormat is YUV444:
 * u32AlignWidth_Y assign to (uExpectWidth + 127) + (~(127));       //128-byte alignment
 * u32AlignHeight_Y assign to (uExpectHeight + 7) + (~(7));        //8-byte alignment
 * u32AlignWidth_C assign to u32AlignWidth_Y*2;
 * u32AlignHeight_C assign to u32AlignHeight_Y;
 *
 * 1).ţ޸ʽת£֧bmpgifpngʹ
 * 2).֧RGBʽsurfaceıbmpʹ
 * 3).ֽ֧text͵surface
 * 4).MMZOSsurface֮תͨHI_GO_BlitУHI_GO_Blit֧HIGO_BLTOPT_S ={0}
 * 	 ColorkeyزsurfaceRGBʽblitϲ֧֡
 * 5).MemOwnerΪHIGO_OWNER_USERָ֧ʽYUV(semi-planner), RGBCLUT8ʽʽ֧RGBCLUT8ʽ
 * һôYUVֳøʽsurface
 * HI_U32 u32AlignWidth_Y,u32AlignHeight_Y; //YĴС
 * HI_U32 u32AlignWidth_C,u32AlignHeight_C; //UVĴС
 * HI_U32 uExpectWidth,uExpectHeight;       //ԭͼƬĴСͼƬĴС
 * HIGO_PF_E szPixelFormat;                 //ԭͼƬظʽͼƬظʽHIGO_PF_YUV400,HIGO_PF_YUV420,
 * HIGO_PF_YUV422,HIGO_PF_YUV422_V,HIGO_PF_YUV444
 * HIGO_SURINFOEX_S SurfaceInfoEx assign to {0};
 *
 * SurfaceInfoEx.Width assign to uExpectWidth;
 * SurfaceInfoEx.Height assign to uExpectHeight;
 * SurfaceInfoEx.PixelFormat assign toszPixelFormat;
 * SurfaceInfoEx.MemType assign to HIGO_MEMTYPE_MMZ;
 * SurfaceInfoEx.MemOwner assign to HIGO_OWNER_USER;
 *
 * SurfaceInfoEx.Pitch[0] assign tou32AlignWidth_Y;
 * SurfaceInfoEx.pPhyAddr[0] assign to HI_MMZ_New(SurfaceInfoEx.Pitch[0]*u32AlignHeight_Y, 4, NULL, "higoSurface");
 * SurfaceInfoEx.pVirAddr[0] assign to HI_MMZ_Map((HI_U32)SurfaceInfoEx.pPhyAddr[0], 0);
 *
 * SurfaceInfoEx.Pitch[1] assign to u32AlignWidth_C;
 * SurfaceInfoEx.pPhyAddr[1] assign to HI_MMZ_New(SurfaceInfoEx.Pitch[1]*u32AlignHeight_C, 4, NULL, "higoSurfaceCbCr");
 * SurfaceInfoEx.pVirAddr[1] assign to HI_MMZ_Map((HI_U32)SurfaceInfoEx.pPhyAddr[1], 0);
 *
 * ret assign to HI_GO_CreateSurfaceEx(&SurfaceInfoEx, &picSurface);
 *
 * ---------------------------------------------------------------------------------------
 * ˵һ¸szPixelFormat˵u32AlignWidth_Yu32AlignHeight_Yu32AlignWidth_Cu32AlignHeight_Cȡֵ
 * 1 ) YUV400
 * u32AlignWidth_Y   ֵΪ (uExpectWidth + 127) +(~(127));       //128 byte
 * u32AlignHeight_Y  ֵΪ (uExpectHeight + 7) + (~(7));	      //8 byte
 * u32AlignWidth_C   ֵΪ 0;
 * u32AlignHeight_C  ֵΪ 0;
 * 2 ) YUV420
 * u32AlignWidth_Y   ֵΪ (uExpectWidth + 127) +(~(127));       //128 byte
 * u32AlignHeight_Y  ֵΪ (uExpectHeight + 15) + (~(15));	      //16 byte
 * u32AlignWidth_C   ֵΪ u32AlignWidth_Y;
 * u32AlignHeight_C  ֵΪ u32AlignHeight_Y / 2;
 * 3 ) YUV422
 * u32AlignWidth_Y   ֵΪ (uExpectWidth + 127) +(~(127));       //128 byte
 * u32AlignHeight_Y  ֵΪ (uExpectHeight + 7) + (~(7));	      //8 byte
 * u32AlignWidth_C   ֵΪ u32AlignWidth_Y;
 * u32AlignHeight_C  ֵΪ u32AlignHeight_Y;
 * 4 ) YUV422_V
 * u32AlignWidth_Y   ֵΪ (uExpectWidth + 127) +(~(127));       //128 byte
 * u32AlignHeight_Y  ֵΪ (uExpectHeight + 15) + (~(15));	      //16 byte
 * u32AlignWidth_C   ֵΪ u32AlignWidth_Y * 2;
 * u32AlignHeight_C  ֵΪ u32AlignHeight_Y / 2;
 * 5 ) YUV444
 * u32AlignWidth_Y   ֵΪ (uExpectWidth + 127) +(~(127));       //128 byte
 * u32AlignHeight_Y  ֵΪ (uExpectHeight + 7) + (~(7));	      //8 byte
 * u32AlignWidth_C   ֵΪ u32AlignWidth_Y * 2;
 * u32AlignHeight_C  ֵΪ u32AlignHeight_Y; CNend
 * \param[in]   pSurInfo      Pointer to the surface information. The value cannot be empty.
 *
 * \param[out]  pSurface      Pointer to a surface. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_CreateSurfaceEx(const HIGO_SURINFOEX_S  *pSurInfo, HI_HANDLE *pSurface);

/*
 * \brief Set common Palette.
 * \attention \n
 * N/A
 * \param[in]   Palette.
 * \param[out]  none
 *
 * \retval ::HI_SUCCESS
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_SetPubPalette(const HI_PALETTE Palette);

/*
 * \brief Get common Palette.
 * \attention \n
 * N/A
 * \param[in]   none
 * \param[out]  Palette.
 *
 * \retval ::HI_SUCCESS
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_GetPubPalette(HI_PALETTE Palette);

/*
 * \brief Set clip region of surface.
 * \attention \n
 * Blit just support single Clip.
 *
 * (HI_GO_SetSurfaceClipRect) CNend
 *
 * \param[in]   Surface handle.
 * \param[in]   Pointer of Region.
 * \param[in]   Number of clip region.
 * \param[out]  none
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NOMEM
 *
 * \see \n
 * N/A
 */
HI_S32 HI_GO_SetSurfaceClipRgn(HI_HANDLE Surface, const HI_REGION *pRegion, HI_U32 u32Num);

/*
 * \brief Get clip region of surface.
 * \attention  \n
 * N/A
 * \param[in]   Surface handle.
 * \param[out]   Pointer of Region.
 * \param[out]   Number of clip region.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 * \see \n
 * N/A
 */
HI_S32 HI_GO_GetSurfaceClipRgn(HI_HANDLE Surface, HI_REGION **ppRegion, HI_U32 *pu32Num);

/*
 * \brief Set Clip Rect.
 * \attention  \n
 * N/A
 * \param[in]   Surface handle.
 * \param[in]   Clip Rect.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 * \see \n
 * N/A
 */
HI_S32 HI_GO_SetSurfaceClipRect(HI_HANDLE hSurface, const HI_RECT *pRect);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HI_GO_SURFACE_H */
