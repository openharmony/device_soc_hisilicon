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

#ifndef HI_GO_BLITER_H
#define HI_GO_BLITER_H

#include "hi_go_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Pixel-level alpha blending operation. */
typedef enum {
    HIGO_COMPOPT_NONE = 0, /* Porter/Duff None-blending operation. */
    HIGO_COMPOPT_CLEAR,    /* Porter/Duff clear blending operation. */
    HIGO_COMPOPT_SRC,      /* Porter/Duff SRC blending operation. */
    HIGO_COMPOPT_SRCOVER,  /* Porter/Duff Srcover blending operation. */
    HIGO_COMPOPT_DSTOVER,  /* Porter/Duff DstOver blending operation. */
    HIGO_COMPOPT_SRCIN,    /* Porter/Duff SrcIn blending operation. */
    HIGO_COMPOPT_DSTIN,    /* Porter/Duff DstIn blending operation. */
    HIGO_COMPOPT_SRCOUT,   /* Porter/Duff SrcOut blending operation. */
    HIGO_COMPOPT_DSTOUT,   /* Porter/Duff DstOut blending operation. */
    HIGO_COMPOPT_SRCATOP,  /* Porter/Duff SrcAtop blending operation. */
    HIGO_COMPOPT_DSTATOP,  /* Porter/Duff DstAtop blending operation. */
    HIGO_COMPOPT_ADD,      /* Porter/Duff DstAtop blending operation. */
    HIGO_COMPOPT_XOR,      /* Porter/Duff Xor blending operation. */
    HIGO_COMPOPT_DST,      /* Porter/Duff DstAtop blending operation. */
    HIGO_COMPOPT_AKS,      /*
                            * Assume that the destination surface is not transparent. After the alpha blending is
                            * performed, the source alpha is retained.
                            *
                            * alphaϣԴalpha
                            */
    HIGO_COMPOPT_AKD,      /*
                            * Assume that the destination surface is not transparent.
                            * After the alpha blending is performed, the destination alpha is retained.
                            *
                            */

    HIGO_COMPOPT_BUTT
} HIGO_COMPOPT_E;

/* Operation mode corresponding to colorkey. */
typedef enum {
    HIGO_CKEY_NONE = 0, /* Do not use the colorkey. */
    HIGO_CKEY_SRC,      /* Use the source colorkey. */
    HIGO_CKEY_DST,      /* Use the destination colorkey. */

    HIGO_CKEY_BUTT
} HIGO_CKEY_E;

/* Two raster of operations (ROPs). */
typedef enum {
    HIGO_ROP_BLACK = 0, /* <Blackness> */
    HIGO_ROP_PSDon,     /* <~(PS+D)> */
    HIGO_ROP_PSDna,     /* <~PS & D> */
    HIGO_ROP_PSn,       /* <~PS> */
    HIGO_ROP_DPSna,     /* <PS &> ~D */
    HIGO_ROP_Dn,        /* <~D> */
    HIGO_ROP_PSDx,      /* <PS^D> */
    HIGO_ROP_PSDan,     /* <~(PS&D)> */
    HIGO_ROP_PSDa,      /* <PS & D> */
    HIGO_ROP_PSDxn,     /* <~(PS^D)> */
    HIGO_ROP_D,         /* <D> */
    HIGO_ROP_PSDno,     /* <~PS + D> */
    HIGO_ROP_PS,        /* <PS> */
    HIGO_ROP_DPSno,     /* <PS + ~D> */
    HIGO_ROP_PSDo,      /* <PS+D> */
    HIGO_ROP_WHITE,     /* <Whiteness> */

    HIGO_ROP_BUTT
} HIGO_ROP_E;

typedef enum {
    HIGO_ROTATE_NONE = 0,
    HIGO_ROTATE_90,     /* Rotate 90 degrees clockwise. */
    HIGO_ROTATE_180,    /* Rotate 180 degrees clockwise. */
    HIGO_ROTATE_270,    /* Rotate 270 degrees clockwise. */

    HIGO_ROTATE_BUTT
} HIGO_ROTATE_E;

typedef enum {
    HIGO_MIRROR_NONE = 0,

    HIGO_MIRROR_LR,     /* Mirror the left and the right. */
    HIGO_MIRROR_TB,     /* Mirror the top and the bottom. */

    HIGO_MIRROR_BUTT
} HIGO_MIRROR_E;

typedef struct {
    HI_BOOL EnableGlobalAlpha;      /* Global alpha enable flag. */
    HI_BOOL EnablePixelAlpha;       /* Pixel alpha enable flag. */
    HIGO_COMPOPT_E PixelAlphaComp;  /* Pixel alpha operation. */
    HIGO_CKEY_E    ColorKeyFrom;    /* Colorkey operation. */
    HI_BOOL        EnableRop;       /* Enable the ROP2 operation. */
    HIGO_ROP_E     Rop;             /* ROP2 operation type. */
    HIGO_ROP_E     RopAlpha;        /* Type of the ROP alpha operation. */
} HIGO_BLTOPT2_S;

/* */
typedef struct {
    HI_BOOL EnableGlobalAlpha;      /*
                                     * Global alpha enable flag. If this flag is enabled, the PixelAlphaComp blending
                                     * mode must be specified.
                                     *
                                     */
    HI_BOOL EnablePixelAlpha;       /* Pixel alpha enable flag. */
    HIGO_COMPOPT_E PixelAlphaComp;  /* Pixel alpha operation. */
    HIGO_CKEY_E    ColorKeyFrom;    /* colorkey operation. */
    HI_BOOL        EnableRop;       /* Enable the ROP2 operation. */
    HIGO_ROP_E     Rop;             /* ROP2 operation type. */
    HIGO_ROP_E     RopAlpha;        /* Type of the ROP alpha operation. */
    HI_BOOL        EnableScale;     /* Enable the scaling function. */
    HIGO_ROTATE_E  RotateType;      /* Rotation type. */
    HIGO_MIRROR_E  MirrorType;      /* Mirror type . */
} HIGO_BLTOPT_S;

typedef struct {
    HI_BOOL EnableGlobalAlpha;      /* Global alpha enable flag. */
    HIGO_COMPOPT_E PixelAlphaComp;  /* Pixel alpha operation. */
    HI_BOOL        EnableRop;       /* Enable the ROP2 operation. */
    HIGO_ROP_E     RopColor;        /* ROP2 operation type. */
    HIGO_ROP_E     RopAlpha;
} HIGO_MASKOPT_S;

/* Anti-flicker level. */
typedef enum {
    HIGO_DEFLICKER_AUTO = 0, /*
                              * Anti-flicker level, ranging from low to high. The higher the level, the better the
                              * anti-flicker effect, but the more blurred the picture.
                              *
                              * ֵΪLOW~HIGH,ֵԽ˸ЧԽãԽģ
                              */
    HIGO_DEFLICKER_LOW,
    HIGO_DEFLICKER_MIDDLE,
    HIGO_DEFLICKER_HIGH,
    HIGO_DEFLICKER_BUTT
}HIGO_DEFLICKEROPT_E;

/* Anti-flicker level. */
typedef struct {
    HIGO_DEFLICKEROPT_E DefLevel;
}HIGO_DEFLICKEROPT_S;

/*
 * \brief Initializes the Bliter module.
 * \attention \n
 * When ::HI_GO_Init is called, this API is also called.
 * \param  N/A.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_DEPEND_TDE
 *
 * \see \n
 * ::HI_GO_Init \n
 * ::HI_GO_DeinitBliter
 */
HI_S32 HI_GO_InitBliter(HI_VOID);

/*
 * \brief Deinitializes the Bliter module.
 * \attention \n
 * When ::HI_GO_Deinit is called, this API is also called.
 * \param N/A.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 *
 * \see \n
 * ::HI_GO_Deinit \n
 * ::HI_GO_InitBliter
 */
HI_S32 HI_GO_DeinitBliter(HI_VOID);

/*
 * \brief Fills in a rectangle.
 * \attention \n
 * N/A.
 * \param[in] Surface Surface handle.
 * \param[in] pRect Size of the rectangle to be filled in. If the parameter is not set, it indicates that the entire
 * surface is filled in.
 * \param[in] Color Color Fill color. For the RGB format, 32-bit color is filled in; for the palette, color index(0-255)
 * is filled in.
 * \param[in] CompositeOpt Blending mode.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_OUTOFPAL
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_FillRect(HI_HANDLE Surface, const HI_RECT *pRect, HI_COLOR Color, HIGO_COMPOPT_E CompositeOpt);

/*
 * \brief Transfers bit blocks. During the transfer, color space conversion (CSC), scaling, and rotation are supported.
 *
 * \attention \n
 * Only the YUV-to-RGB CSC is supported. \n
 * The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
 * Scaling, rotation, and mirror cannot be combined. \n
 * For scaling, rotation, and mirror, the source and destination pixel formats must be the same, but the format cannot
 * be YUV or CLUT.\n
 *
 * ֲ֧colorkeyalphaROPcolorkey+ROP\
 * šת񲻿ʹ \n
 * šתҪԴĿظʽȫͬYUVʽCLUTʽ \n CNend
 *
 * \param[in] SrcSurface Source surface handle.
 * \param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.
 *
 * \param[in] DstSurface Destination surface handle.
 * \param[in] pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination
 * surface.
 * \param[in] pBlitOpt Blending mode.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVMIRRORTYPE
 * \retval ::HIGO_ERR_INVROTATETYPE
 * \retval ::HIGO_ERR_INVCKEYTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_NOCOLORKEY
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 * \retval ::HIGO_ERR_INTERNAL
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_Blit(HI_HANDLE SrcSurface, const HI_RECT *pSrcRect, HI_HANDLE DstSurface, const HI_RECT *pDstRect,
    const HIGO_BLTOPT_S *pBlitOpt);

/*
 * \brief set line width.
 * \param[in] Surface Destination surface handle.
 * \param[in] Width is for line width.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_DEPEND_TDE
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_SetLineWidth(HI_HANDLE Surface, HI_U32 Width);

/*
 * \brief Draws a line segment. Cropping is not supported.
 * \attention \n
 * Cropping is not supported. Users must keep the entire line within the surface region.
 *
 *
 * \param[in] Surface Destination surface handle.
 * \param[in] x0 Horizontal coordinate of the start point.
 * \param[in] y0 Vertical coordinate of the start point.
 * \param[in] x1 Horizontal coordinate of the end point.
 * \param[in] y1 Vertical coordinate of the end point .
 * \param[in] color Line segment color.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_DEPEND_TDE
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_DrawLine(HI_HANDLE Surface, HI_S32 x0, HI_S32 y0, HI_S32 x1, HI_S32 y1, HI_COLOR color);

/*
 * \brief Draws an ellipse.
 * \attention \n
 * Cropping is not supported. Users must keep the entire ellipse within the surface region.
 *
 *
 * \param[in] Surface Destination surface handle.
 * \param[in] sx Horizontal coordinate of the ellipse center.
 * \param[in] sy Vertical coordinate of the ellipse center.
 * \param[in] rx X axis radius .
 * \param[in] ry Y axis radius.
 * \param[in] color Ellipse color.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_DrawEllipse(HI_HANDLE Surface, HI_S32 sx, HI_S32 sy, HI_S32 rx, HI_S32 ry, HI_COLOR color);

/*
 * \brief Draws a circle. Cropping is not supported.
 * \attention \n
 * Cropping is not supported. Users must keep the entire circle within the surface region.
 *
 *
 * \param[in] Surface Destination surface handle.
 * \param[in] x Horizontal coordinate of the circle center.
 * \param[in] y Vertical coordinate of the circle center.
 * \param[in] r Radius.
 * \param[in] color Circle color. The circle is filled in with 32-bit colors.
 *
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_DrawCircle(HI_HANDLE Surface, HI_S32 x, HI_S32 y, HI_S32 r, HI_COLOR color);

/*
 * \brief Draws a rectangle.
 * \attention \n
 * If the rectangle is empty, it indicates that the rectangle size is the same as surface.
 * Cropping is not supported; therefore, you must ensure that the entire rectangle is within the surface.
 *
 * ֲ֧üʹ߱뱣֤surface CNend
 *
 * \param[in] Surface Destination surface handle.
 * \param[in] pRect Rectangle region.
 * \param[in] color Rectangle color.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_DrawRect(HI_HANDLE Surface, const HI_RECT *pRect, HI_COLOR color);

/*
 * \brief Performs the mask ROP or mask blend transfer on the raster bitmap.ԹդλͼMask RopMask Blend
 *        Ʋ CNend
 * \attention \n
 * The mask bitmap is in A1 or A8 format.
 * You need to overlay the mask bitmap with the source bitmap, overlay the result with the destination bitmap, and then
 * output the final result to the destination bitmap.
 * If the ROP and blending operation are perform at the same time, only the ROP takes effect.
 *
 * ȽMaskλͼԴλͼһεӣȻõӵĽĿĿλͼ
 * RopBlendͬʱֻһЧģ߶ѡֻRopЧ CNend
 *
 * \param[in] SrcSurface Source surface handle.
 * \param[in] pSrcRect Source rectangle.
 * \param[in] DstSurface Destination surface handle.
 * \param[in] pDstRect Destination rectangle.
 * \param[in] MaskSurface Mask surface handle.
 * \param[in] pMaskRect Mask rectangle.
 * \param[in] pOpt Operation option.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_INVRECT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_INTERNAL
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_MaskBlit(HI_HANDLE SrcSurface, const HI_RECT *pSrcRect, HI_HANDLE DstSurface, const HI_RECT *pDstRect,
    HI_HANDLE MaskSurface, const HI_RECT *pMaskRect, const HIGO_MASKOPT_S *pOpt);

/*
 * \brief Transfers bit blocks. During the transfer, CSC is supported.
 *        ת CNend
 * \attention \n
 * Only the YUV-to-RGB CSC is supported. \n
 * The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
 *
 * ֲ֧colorkeyalphaROPcolorkey+ROP\ CNend
 *
 * \param[in] SrcSurface Source surface handle.
 * \param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.
 *
 * \param[in] DstSurface Destination surface handle.
 * \param[in] pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination
 * surface.
 * \param[in] pBlitOpt Blending mode. If the parameter is not set, default settings are used.
 *
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_INVCKEYTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_BitBlit(HI_HANDLE SrcSurface, const HI_RECT *pSrcRect, HI_HANDLE DstSurface, const HI_RECT *pDstRect,
                     const HIGO_BLTOPT2_S *pBlitOpt);

/*
 * \brief Transfers bit blocks. During the transfer, CSC and scaling are supported.
 *        ʵɫʿռת,  CNend
 * \attention \n
 * Only the YUV-to-RGB CSC is supported. \n
 * The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
 *
 * ֲ֧colorkeyalphaROPcolorkey+ROP\ CNend
 *
 * \param[in] SrcSurface  Source surface handle.
 * \param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.
 *
 * \param[in] DstSurface Destination surface handle.
 * \param[in] pDstRect pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire
 *            destination
 * surface.
 * \param[in] pBlitOpt Blending mode. If the parameter is not set, default settings are used.
 *            ΪʹĬϲ CNend
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_INVCKEYTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_StretchBlit(HI_HANDLE SrcSurface, const HI_RECT *pSrcRect, HI_HANDLE DstSurface, const HI_RECT *pDstRect,
    const HIGO_BLTOPT2_S *pBlitOpt);

/*
 * \brief  Fills in the pattern.
 * \attention \n
 * \param[in] SrcSurface  Source surface handle.
 * \param[in] pSrcRect  Source region for transfer. If the parameter is not set, it indicates the entire source surface.
 *
 * \param[in] DstSurface  Destination surface handle.
 * \param[in] pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination
 * surface.
 * \param[in] pParOpt  Blending mode. If the parameter is not set, default settings are used.
 *
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_INVCKEYTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_NOCOLORKEY
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_PatternBlit(HI_HANDLE SrcSurface, const HI_RECT *pSrcRect, HI_HANDLE DstSurface, const HI_RECT *pDstRect,
    const HIGO_BLTOPT2_S *pParOpt);

/*
 * \brief Performs anti-flicker and transfer. This API is used for the second anti-flicker.
 *        ӿڵڶο˸CNend
 * \attention \n
 * This API is used in the following two application scenarios:
 * 1. The automatic anti-flicker effect of a graphics layer is poor.
 * 2. The buffer mode of a graphics layer is single-buffer mode (HIGO_LAYER_BUFFER_SINGLE). In this case, users need to
 * perform anti-flicker by themselves.
 * When the sizes of the source bitmap and destination bitmap are different, scaling is performed automatically.
 *
 * 1 ͼԶ˸ЧǺܺʱʹӿһΡ
 * 2 ͼbufferģʽǵbufferģʽ(HIGO_LAYER_BUFFER_SINGLE)ʱûҪԼ˸
 * ԴĿСͬʱԶšCNend
 *
 * \param[in] SrcSurface  SrcSurface Source surface handle.
 * \param[in] pSrcRect  pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source
 *                     surface.
 * \param[in] DstSurface  DstSurface Destination surface handle.
 * \param[in] pDstRect  pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire
 *                      destination
 * surface.
 * \param[in] pDefOpt  pDefOpt Anti-flicker level. If this parameter is not set, it indicates the automatic level.
 *
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_INVCKEYTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_NOCOLORKEY
 * \retval ::HIGO_ERR_INVPARAM
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_DeflickerBlit(HI_HANDLE SrcSurface, const HI_RECT *pSrcRect, HI_HANDLE DstSurface, const HI_RECT *pDstRect,
    const HIGO_DEFLICKEROPT_S *pDefOpt);

/*
 * \brief Do the alpha blending between the foreground surface and the background surface, transfers the result to the
 *        destination surface. The background surface and the destination surface must be in the RGB color space.
 * You can performs color space coversion, scale, or mirror, colorkey with alpha blending. úʵǰͱ
 * Ŀ깦ܣĿsurfaceλRGBռ䣬ӹпԸɫռתšȲ֧ת
 * \attention \n
 * Only the YUV-to-RGB CSC is supported. \n
 * The operations of colorkey, alpha, ROP, and colorkey+ROP are supported, rotation is not supported.\
 * Scaling, and mirror cannot be combined. \n
 * For scaling, and mirror, the source and destination pixel formats must be the same, but the format cannot
 * be YUV or CLUT.\n
 *
 * ֲ֧colorkeyalphaROPcolorkey+ROP֧\
 * š񲻿ʹ \n
 * ŻҪԴĿظʽȫͬYUVʽCLUTʽ \n CNend
 *
 * \param[in] BckSurface background surface handle.
 * \param[in] pBckRect background region for transfer. If the parameter is not set,
 *            it indicates the entire background surface.
 *
 * \param[in] ForSurface foreground surface handle.
 * \param[in] pForRect foreground region for transfer. If not set, it indicates the entire foreground surface.
 * surface.
 * \param[in] DstSurface destination surface handle.
 * \param[in] pDstRect destination region for transfer. If not set, it indicates the entire destination surface.
 * surface.
 * \param[in] pBlitOpt Blending mode.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVMIRRORTYPE
 * \retval ::HIGO_ERR_INVROTATETYPE
 * \retval ::HIGO_ERR_INVCKEYTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_NOCOLORKEY
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 * \retval ::HIGO_ERR_INTERNAL
 *
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_Blit3Source(HI_HANDLE BckSurface, const HI_RECT *pBckRect, HI_HANDLE ForSurface, const HI_RECT *pForRect,
    HI_HANDLE DstSurface, const HI_RECT *pDstRect, const HIGO_BLTOPT_S *pBlitOpt);
/*
 * \brief Fills in a rounded rectangle.
 * \attention \n
 * N/A.
 * \param[in] Surface Surface handle.
 * \param[in] pRect Size of the rectangle to be filled in. If the parameter is not set, it indicates that the entire
 * surface is filled in.
 * \param[in] Color Color Fill color. For the RGB format, 32-bit color is filled in; for the palette, color index(0-255)
 * is filled in.
 * \param[in] s32Radius radius.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_OUTOFPAL
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_FillRoundRect(HI_HANDLE Surface, const HI_RECT *pRect, HI_COLOR Color, HI_S32 s32Radius);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HI_GO_BLITER_H */
