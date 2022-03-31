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

#ifndef HI_GO_GDEV_H
#define HI_GO_GDEV_H

#include "hi_go_comm.h"
#include "hi_go_surface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Definition of the graphics layer ID. */
typedef enum {
    HIGO_LAYER_SD_0 = 0,
    HIGO_LAYER_SD_1,
    HIGO_LAYER_HD_0,
    HIGO_LAYER_HD_1,
    HIGO_LAYER_HD_2,
    HIGO_LAYER_HD_3,
    HIGO_LAYER_AD_0,
    HIGO_LAYER_AD_1,
    HIGO_LAYER_BUTT,
} HIGO_LAYER_E;

/* Standard-definition (SD) graphics layer 0. */
#define HIGO_LAYER_RGB0 HIGO_LAYER_SD_0

/* SD graphics layer 1. */
#define HIGO_LAYER_RGB1 HIGO_LAYER_SD_1

#define GRAPHICS_LAYER_MAX_NUM 4

/*
 * The following macro defines the buffer mode of each graphics layer of the HiGo.
 * The canvas buffer is used for drawing,
 * and the display buffer is used for display output.
 *
 * display bufferʾbuffer.
 */
#define HIGO_LAYER_BUFFER_SINGLE      0x02 /*
                                            * One canvas buffer, and no display buffer.
                                            *
                                            */
#define HIGO_LAYER_BUFFER_DOUBLE      0x04 /*
                                            * One canvas buffer, and one display buffer. Dual buffers are supported.
                                            *
                                            */
#define HIGO_LAYER_BUFFER_TRIPLE      0x08 /*
                                            * One canvas buffer, and two display buffers.
                                            * The flip function is supported.
                                            *
                                            * ˢµʱȴ
                                            */
#define HIGO_LAYER_BUFFER_OVER        0x10 /*
                                            * One canvas buffer, and two display buffers.
                                            * The flip function is supported.
                                            * If tasks are being performed during refresh,
                                            * the current frame is discarded.
                                            *
                                            * ˢµʱæǰ֡
                                            */
#define HIGO_LAYER_BUFFER_FENCE       0x12 /* fence refresh type, fenceˢģʽ */

/*
 * Refresh mode of graphics layers for complying with old definitions. The mode is not recommended.
 *
 */
typedef enum {
    HIGO_LAYER_FLUSH_FLIP        = HIGO_LAYER_BUFFER_TRIPLE,
    HIGO_LAYER_FLUSH_DOUBBUFER   = HIGO_LAYER_BUFFER_DOUBLE,
    HIGO_LAYER_FLUSH_NORMAL      = HIGO_LAYER_BUFFER_SINGLE,
    HIGO_LAYER_FLUSH_OVER        = HIGO_LAYER_BUFFER_OVER,
    HIGO_LAYER_FLUSH_FENCE       = HIGO_LAYER_BUFFER_FENCE,
    HIGO_LAYER_FLUSH_BUTT
} HIGO_LAYER_FLUSHTYPE_E;

/*
 * Anti-flicker level, ranging from low to high. The higher the level,
 * the better the anti-flicker effect, but the more blurred the picture.
 *
 */
typedef enum {
    HIGO_LAYER_DEFLICKER_NONE = 0,
    HIGO_LAYER_DEFLICKER_LOW,
    HIGO_LAYER_DEFLICKER_MIDDLE,
    HIGO_LAYER_DEFLICKER_HIGH,
    HIGO_LAYER_DEFLICKER_AUTO,
    HIGO_LAYER_DEFLICKER_BUTT
} HIGO_LAYER_DEFLICKER_E;

/* Frame encode format. */
typedef enum {
    HIGO_ENCPICFRM_MONO = 0x0,             /* Normal display, no 3D TV. */
    HIGO_ENCPICFRM_STEREO_SIDEBYSIDE_HALF, /*
                                            * L/R frames are downscaled horizontally by 2 andpacked side-by-side
                                            * into a single frame, left on lefthalf of frame.
                                            *
                                            */
    HIGO_ENCPICFRM_STEREO_TOPANDBOTTOM,    /*
                                            * L/R frames are downscaled vertically by 2 andpacked
                                            * into a single frame, left on top.
                                            *
                                            */
    HIGO_ENCPICFRM_BUTT
} HIGO_ENCPICFRM_E;

/* 3D STEREO mode. */
typedef enum {
    HIGO_STEREO_MODE_HW_FULL = 0x0, /*
                                     * 3d stereo function use hardware and transfer full frame to vo,
                                     * note: hardware doesn't support the mode
                                     * if encoder picture delivery method is top and bottom
                                     */
    HIGO_STEREO_MODE_HW_HALF,       /* 3d stereo function use hardware and transfer half frame to vo */
    HIGO_STEREO_MODE_SW_EMUL,       /* 3d stereo function use software emulation                     */
    HIGO_STEREO_MODE_BUTT
} HIGO_STEREO_MODE_E;

/* Layer attribute parameters. */
typedef struct {
    HI_S32                 ScreenWidth;    /*
                                            * Height of a graphics layer on the screen.
                                            * The value must be greater than 0.
                                            *
                                            */
    HI_S32                 ScreenHeight;   /*
                                            * Width of a graphics layer on the screen.
                                            * The value must be greater than 0.
                                            *
                                            */
    HI_S32                 CanvasWidth;    /* Width of the canvas buffer of a graphics layer. If the value is 0,
                                            * no canvas buffer is created.
                                            *
                                            */
    HI_S32                 CanvasHeight;   /*
                                            * Height of the canvas buffer of a graphics layer.
                                            * If the value is 0, no canvas buffer is created.
                                            *
                                            */
    HI_S32                 DisplayWidth;   /*
                                            * Width of the display buffer of a graphics layer.
                                            * If the value is 0, the value of ScreenWidth is used.
                                            *
                                            */
    HI_S32                 DisplayHeight;  /*
                                            * Height of the display buffer of a graphics layer.
                                            * If the value is 0, the value of ScreenHeight is used.
                                            *
                                            */
    HIGO_LAYER_FLUSHTYPE_E LayerFlushType; /*
                                            * Refresh mode of the layer. You can choose the refresh mode
                                            * based on the actual scenario to improve the refresh efficiency.
                                            * If the value is 0, the dual-buffer+flip refresh mode is used by default.
                                            *
                                            * ˢЧ,0ʱĬʹ˫+Flipˢģʽ
                                            */
    HIGO_LAYER_DEFLICKER_E AntiLevel;      /* Anti-flicker level of a graphics layer. */
    HIGO_PF_E              PixelFormat;    /*
                                            * Pixel format of a graphics layer. The format must be supported by
                                            * the hardware layer. You need to choose the pixel format parameters of
                                            * the layer based on hardware devices.
                                            *
                                            * ݲͬӲ豸ѡͼظʽ
                                            */
    HIGO_LAYER_E           LayerID;        /*
                                            * Hardware ID of a graphics layer. The supported graphics layer depends on
                                            * the chip platform. For example, the Hi3720 supports an HD graphics layer
                                            * and an SD graphics layer.
                                            *
                                            */
} HIGO_LAYER_INFO_S;

/* Status of a graphics layer. */
typedef struct {
    HI_BOOL bShow;          /* Whether to display a graphics layer. */
} HIGO_LAYER_STATUS_S;

typedef struct {
    HI_COLOR ColorKey;      /* Transparency of a graphics layer. */
    HI_BOOL bEnableCK;      /* Whether the colorkey of a graphics layer is enabled. */
} HIGO_LAYER_KEY_S;

typedef struct {
    HI_BOOL bAlphaEnable;   /* Alpha pixel enable flag. */
    HI_BOOL bAlphaChannel;  /* Alpha channel enable flag. */
    HI_U8   Alpha0;         /* Alpha0 value. It is valid in ARGB1555 format. */
    HI_U8   Alpha1;         /* Alpha1 value. It is valid in ARGB1555 format. */
    HI_U8   GlobalAlpha;    /*
                             * Global alpha. This value is valid only when the alpha channel is valid.
                             *
                             */
} HIGO_LAYER_ALPHA_S;

typedef struct {
    HI_HANDLE   Layer;            /* The layer the scrolltext want to show                 */
    HI_RECT       stScrollRect;   /* the position you wanted to show on the layer          */
    HIGO_PF_E   ePixelFormat;     /* the color format of scrolltext content                */
    HI_U16        u16CacheNum;    /* The cached buffer number for store scrolltext content */
    HI_U16        u16RefreshFreq; /* The refresh frequency you wanted                      */
    HI_BOOL     bDeflicker;       /* Whether enable antificker                             */
} HIGO_SCROLLTEXT_ATTR_S;

typedef struct {
    HI_U32 u32PhyAddr;   /* The physical address of the scrolltext content buffer                   */
    HI_U8  *pu8VirAddr;  /* The virtual address of the scrolltext content buffer                    */
    HI_U32 u32BufferLen; /* The virtual or physical address Length of the scrolltext content buffer */
    HI_U32 u32Stride;    /* The stride of the scrolltext content buffer                             */
} HIGO_SCROLLTEXT_DATA_S;

HI_S32 HI_GO_InitDisplay(HI_VOID);

HI_S32 HI_GO_DeinitDisplay(HI_VOID);

HI_S32 HI_GO_GetLayerDefaultParam(HIGO_LAYER_E LayerID, HIGO_LAYER_INFO_S *pLayerInfo);

HI_S32 HI_GO_CreateLayer(const HIGO_LAYER_INFO_S *pLayerInfo, HI_HANDLE *pLayer);

HI_S32 HI_GO_DestroyLayer(HI_HANDLE Layer);

HI_S32 HI_GO_SetLayerPos(HI_HANDLE Layer, HI_U32 u32StartX, HI_U32 u32StartY);

HI_S32 HI_GO_GetLayerPos(HI_HANDLE Layer, HI_U32 *pStartX, HI_U32 *pStartY);

HI_S32 HI_GO_SetScreenSize(HI_HANDLE Layer, HI_U32 u32SWidth, HI_U32 u32SHeight);

HI_S32 HI_GO_GetScreenSize(HI_HANDLE Layer, HI_U32 *pSWidth, HI_U32 *pSHeight);

HI_S32 HI_GO_SetLayerAlpha(HI_HANDLE Layer, HI_U8 Alpha);

HI_S32 HI_GO_SetLayerPreMul(HI_HANDLE Layer, HI_BOOL bPreMul);

HI_S32 HI_GO_GetLayerAlpha(HI_HANDLE Layer, HI_U8 *pAlpha);

HI_S32 HI_GO_GetLayerSurface(HI_HANDLE Layer, HI_HANDLE *pSurface);

HI_S32 HI_GO_ShowLayer(HI_HANDLE Layer, HI_BOOL bVisbile);

HI_S32 HI_GO_GetLayerStatus(HI_HANDLE Layer, HIGO_LAYER_STATUS_S *pLayerStatus);

HI_S32 HI_GO_RefreshLayer(HI_HANDLE Layer, const HI_RECT *pRect);

HI_S32 HI_GO_SetLayerSurface(HI_HANDLE Layer, HI_HANDLE hSurface);

HI_S32 HI_GO_SetLayerBGColor(HI_HANDLE Layer, HI_COLOR Color);

HI_S32 HI_GO_GetLayerBGColor(HI_HANDLE Layer, HI_COLOR *pColor);

HI_S32 HI_GO_SetDisplaySize(HI_HANDLE Layer, HI_U32 u32DWidth, HI_U32 u32DHeight);

HI_S32 HI_GO_GetDisplaySize(HI_HANDLE Layer, HI_U32 *pDWidth, HI_U32 *pDHeight);

HI_S32 HI_GO_SetFlushType(HI_HANDLE Layer, HIGO_LAYER_FLUSHTYPE_E FlushType);

HI_S32 HI_GO_GetFlushType(HI_HANDLE Layer, HIGO_LAYER_FLUSHTYPE_E *pFlushType);

HI_S32 HI_GO_SetLayerColorkey(HI_HANDLE Layer, const HIGO_LAYER_KEY_S *pKey);

HI_S32 HI_GO_GetLayerColorkey(HI_HANDLE Layer, HIGO_LAYER_KEY_S *pKey);

HI_S32 HI_GO_SetLayerPalette(HI_HANDLE Layer, const HI_PALETTE Palette);

HI_S32 HI_GO_GetLayerPalette(HI_HANDLE Layer, HI_PALETTE Palette);

HI_S32 HI_GO_WaitForBlank(HI_HANDLE Layer);

HI_S32 HI_GO_SetLayerAlphaEx(HI_HANDLE Layer, HIGO_LAYER_ALPHA_S *pAlphaInfo);

HI_S32 HI_GO_GetLayerAlphaEx(HI_HANDLE Layer, HIGO_LAYER_ALPHA_S *pAlphaInfo);

HI_S32 HI_GO_SetLayerZorder(HI_HANDLE Layer, HIGO_ZORDER_E enZOrder);

HI_S32 HI_GO_GetLayerZorder(HI_HANDLE Layer, HI_U32 *pu32ZOrder);

HI_S32 HI_GO_SetStereoMode(HI_HANDLE Layer, HIGO_STEREO_MODE_E InputEnc);

HI_S32 HI_GO_GetStereoMode(HI_HANDLE Layer, const HIGO_STEREO_MODE_E *pInputEnc);

HI_S32 HI_GO_SetStereoDepth(HI_HANDLE Layer, HI_S32 s32StereoDepth);

HI_S32 HI_GO_GetStereoDepth(HI_HANDLE Layer, HI_S32 *ps32StereoDepth);

HI_S32 HI_GO_SetCompression(HI_HANDLE Layer, HI_BOOL bEnable);

HI_S32 HI_GO_GetCompression(HI_HANDLE Layer, const HI_BOOL *pbEnable);

HI_S32 HI_GO_CreateScrollText(HIGO_SCROLLTEXT_ATTR_S *pstScrollAttr, HI_HANDLE *phScrollText);

HI_S32 HI_GO_FillScrollText(HI_HANDLE hScrollText, HIGO_SCROLLTEXT_DATA_S *pstScrollData);

HI_S32 HI_GO_PauseScrollText(HI_HANDLE hScrollText);

HI_S32 HI_GO_ResumeScrollText(HI_HANDLE hScrollText);

HI_S32 HI_GO_DestoryScrollText(HI_HANDLE hScrollText);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
