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
extern "C"{
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

/*
 * \brief Initializes a display device.
 * \attention \n
 * When ::HI_GO_Init is called, this API is also called.
 *
 * \param N/A.
 * \retval ::HI_FAILURE
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INITFAILED
 * \see \n
 * ::HI_GO_Init \n
 * ::HI_GO_DeinitDisplay
 */
HI_S32 HI_GO_InitDisplay(HI_VOID);

/*
 * \brief Deinitializes a display device.
 * \attention \n
 * When ::HI_GO_Deinit is called, this API is also called.
 *
 * \param N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_DEINITFAILED
 * \see \n
 * ::HI_GO_Deinit \n
 * ::HI_GO_InitDisplay
 */
HI_S32 HI_GO_DeinitDisplay(HI_VOID);

/*
 * \brief Obtains the default parameters of an SD or HD graphics layer based on its ID.
 * If you do not want to use default values, you can set the members of pLayerInfo.
 *
 * Ҫʹ÷ĬֱֵpLayerInfoԱ CNend
 * \attention \n
 * \param[in] LayerID Layer ID.
 * \param[in]  pLayerInfo Obtained parameters of a graphics layer when it is created.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVLAYERID
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVPARAM
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_GetLayerDefaultParam(HIGO_LAYER_E LayerID, HIGO_LAYER_INFO_S *pLayerInfo);

/*
 * \brief Creates a graphics layer.
 * \attention \n
 * The platform determines whether VO hardware scaling is supported.
 * If VO hardware scaling is supported, the display size is scaled to fit the screen
 * when the display size is inconsistent with the screen size.
 * If VO hardware scaling is not supported, the display size and screen size must be the same.
 *
 * ƽ̨֧VO Ӳţdisplaysizescreensizeһµ£ջŵscreensizeĴС
 * ƽ̨֧VO Ӳţʹdisplaysizescreensizeһ£ҲǿҪdisplaysizescreensizeһ CNend
 * \param[in]  pLayerInfo Basic attributes of a graphics layer. The value cannot be empty.
 *
 * \param[out] pLayer Pointer to the handle of a graphics layer. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVSIZE
 * \retval ::HIGO_ERR_INVLAYERID
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_INVFLUSHTYPE
 * \retval ::HIGO_ERR_INVANILEVEL
 * \retval ::HIGO_ERR_INVSIZE
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \retval ::HIGO_ERR_ALREADYBIND
 * \retval ::HIGO_ERR_INVLAYERSURFACE
 * \see \n
 * ::HI_GO_DestroyLayer
 */
HI_S32 HI_GO_CreateLayer(const HIGO_LAYER_INFO_S *pLayerInfo, HI_HANDLE *pLayer);

/*
 * \brief Destroys a graphics layer.
 * \attention \n
 * If a graphics layer is in use (for example, the desktop based on the layer is not destroyed),
 * the layer cannot be destroyed.
 *
 * \param[in] Layer Layer handle.
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * ::HI_GO_CreateLayer
 */
HI_S32 HI_GO_DestroyLayer(HI_HANDLE Layer);

/*
 * \brief Sets the position of a graphics layer on the screen.
 * \attention \n
 * The value takes effect at once and you do not need to refresh it.
 * If the start position of a layer exceeds the screen,
 * it is automatically adjusted to the boundary of the screen.
 *
 * \param[in] Layer  Layer handle.
 * \param[in] u32StartX Horizontal coordinate of the position where a layer appears on the screen.
 *
 * \param[in] u32StartY Vertical coordinate of the position where a layer appears on the screen.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVLAYERPOS
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetLayerPos
 */
HI_S32 HI_GO_SetLayerPos(HI_HANDLE Layer, HI_U32 u32StartX, HI_U32 u32StartY);

/*
 * \brief Obtains the position where a layer appears on the screen.
 * \attention \n
 * N/A.
 * \param[in] Layer Layer handle.
 * \param[out] pStartX Pointer of the horizontal coordinate of the position where a layer appears on the screen.
 *             The value cannot be empty.
 * \param[out] pStartY Pointer of the vertical coordinate of the position where a layer appears on the screen.
 *             The value cannot be empty.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetLayerPos
 */
HI_S32 HI_GO_GetLayerPos(HI_HANDLE Layer, HI_U32 *pStartX, HI_U32 *pStartY);

/*
 * \brief Sets the output height and width of a graphics layer on the display device.
 *
 * \attention \n
 * This API supports zoom in operations. The maximum range is specified when a graphics layer is created.\n
 *
 * \param[in]  Layer Layer handle.
 * \param[in] u32SWidth  Actual output width.
 * \param[in] u32SHeight Actual output height.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVSIZE
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetScreenSize
 */
HI_S32 HI_GO_SetScreenSize(HI_HANDLE Layer, HI_U32 u32SWidth, HI_U32 u32SHeight);

/*
 * \brief Obtains the output height and width of a graphics layer on the display device.
 *
 * \attention \n
 * N/A.
 * \param[in]  Layer Layer handle.
 * \param[out] pSWidth  Pointer to the actual output width.
 * \param[out] pSHeight Pointer to the actual output height.
 * \retval none
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetScreenSize
 */
HI_S32 HI_GO_GetScreenSize(HI_HANDLE Layer, HI_U32 *pSWidth, HI_U32 *pSHeight);

/*
 * \brief Sets the global alpha value of a layer surface.
 * \attention \n
 * The value takes effect at once, and you do not need to refresh it.
 *
 * \param[in] Layer Layer handle.
 * \param[in] Alpha Global alpha value. The value cannot be empty and ranges from 0 to 255.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_SETALPHAFAILED
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetLayerAlpha
 */
HI_S32 HI_GO_SetLayerAlpha(HI_HANDLE Layer, HI_U8 Alpha);

/*
 * \brief Sets layer premul state.
 * \attention \n
 * The value takes effect at once, and you do not need to refresh it.
 *
 * \param[in] Layer Layer handle.
 * \param[in] enable or disable layer premu.
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \see \n
 * ::HI_GO_SetLayerPreMul
 */
HI_S32 HI_GO_SetLayerPreMul(HI_HANDLE Layer, HI_BOOL bPreMul);

/*
 * \brief Obtains the global alpha value of a layer surface.
 * \attention \n
 * N/A.
 * \param[in] Layer Layer handle.
 * \param[out] pAlpha Pointer to the global alpha value.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetLayerAlpha
 */
HI_S32 HI_GO_GetLayerAlpha(HI_HANDLE Layer, HI_U8 *pAlpha);

/*
 * \brief Obtains the surface of a graphics layer.
 * \attention \n
 * Surface of a graphics layer. It cannot be released by calling HI_GO_FreeSurface.
 * The surface can be released only when the corresponding layer is destroyed.
 * After a process is switched, you must obtain the layer surface again by calling HI_S32 HI_GO_GetLayerSurface.
 *
 * л֮øýӿ»ȡͼsurface; CNend
 * \param[in] Layer handle.
 * \param[out] pSurface Pointer to the surface handle. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVLAYERSURFACE
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_GetLayerSurface(HI_HANDLE Layer, HI_HANDLE *pSurface);

/*
 * \brief Shows or hides a graphics layer.
 * \attention \n
 * The value takes effect at once, and you do not need to refresh it.
 *
 * \param[in] Layer Layer handle.
 * \param[in] bVisbile Show/hide flag. HI_TRUE: show; HI_FALSE: hide.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetLayerStatus
 */
HI_S32 HI_GO_ShowLayer(HI_HANDLE Layer, HI_BOOL bVisbile);

/*
 * \brief Obtains the current status of a graphics layer.
 * \attention \n
 * N/A.
 * \param[in] Layer Layer handle.
 * \param[out] pLayerStatus Pointer to the current status of a graphics layer. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 *  * ::HI_GO_ShowLayer
 */
HI_S32 HI_GO_GetLayerStatus(HI_HANDLE Layer, HIGO_LAYER_STATUS_S *pLayerStatus);

/*
 * \brief Refreshes a graphics layer.
 * \attention \n
 * 1. After drawing, you need to refresh the layer to display the drawing result.
 * 2. There are two display modes after a graphics layer is refreshed. If there is no window,
 *    the contents of the layer surface are displayed; if there is a window, its contents are displayed.
 *
 * 2.ˢģʽһûκδڵʱʾlayersurfaceݣʾеݡCNend
 * \param[in] Layer Layer handle.
 * \param[in] pRect Rectangle to be refreshed. If the value is NULL, the entire screen is refreshed.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVLAYERSURFACE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 * \retval ::HIGO_ERR_EMPTYRECT
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_RefreshLayer(HI_HANDLE Layer, const HI_RECT *pRect);

/*
 * \brief Sets the canvas buffer of a graphics layer.
 * \attention \n
 * If the stream sources of the HD and SD graphics layer are the same,
 * the canvas buffer of the SD graphics layer can be shared with the HD graphics layer.
 *
 * \param[in] Layer     Layer handle.
 * \param[in] hSurface  Surface of a graphics layer. If the value is INVALID_HANDLE, there is no user handle.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVLAYERSURFACE
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_INVPARAM
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_SetLayerSurface(HI_HANDLE Layer, HI_HANDLE hSurface);

/*
 * \brief Sets the background color of a graphics layer.
 * \attention \n
 * The background color of a graphics layer takes effect only when widows are overlaid with each other.
 *
 * \param[in] Layer Layer handle.
 * \param[in] Color Background color of a graphics layer.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_SetLayerBGColor(HI_HANDLE Layer, HI_COLOR Color);

/*
 * \brief Obtains the background color of a graphics layer.
 * \attention \n
 * This API is available only when there are windows on graphics layers.
 * Otherwise, the configured background color does not take effect.
 *
 * \param[in] Layer Layer handle.
 * \param[in] pColor Used for storing the background color of a graphics layer.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_GetLayerBGColor(HI_HANDLE Layer, HI_COLOR *pColor);

/*
 * \brief Sets the size of a display buffer.
 * \attention \n
 * N/A.
 * \param[in] Layer      Layer handle.
 * \param[in] u32DWidth     Pointer to the width.
 * \param[in] u32DHeight    Pointer to the height.
 * \param[out] N/A .
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVSIZE
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * ::HI_GO_GetDisplaySize
 */
HI_S32 HI_GO_SetDisplaySize(HI_HANDLE Layer, HI_U32 u32DWidth, HI_U32 u32DHeight);

/*
 * \brief Obtains the size of a display buffer.
 * \attention \n
 * N/A.
 * \param[in] Layer      Layer handle.
 * \param[in] pDWidth    Pointer to the width. The value cannot be empty.
 * \param[in] pDHeight   Pointer to the height. The value cannot be empty.
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * ::HI_GO_SetDisplaySize
 */
HI_S32 HI_GO_GetDisplaySize(HI_HANDLE Layer, HI_U32 *pDWidth, HI_U32 *pDHeight);

/*
 * \brief Sets the refresh mode.
 * \attention \n
 * N/A.
 * \param[in] Layer         Layer handle.
 * \param[in] FlushType     Refresh mode of a graphics layer.
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVFLUSHTYPE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetFlushType
 */
HI_S32 HI_GO_SetFlushType(HI_HANDLE Layer, HIGO_LAYER_FLUSHTYPE_E FlushType);

/*
 * \brief Obtains the refresh mode.
 * \attention \n
 * N/A.
 * \param[in] Layer       Layer handle.
 * \param[in] *pFlushType Refresh mode of a graphics layer. The value cannot be empty.
 *
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetFlushType
 */
HI_S32 HI_GO_GetFlushType(HI_HANDLE Layer, HIGO_LAYER_FLUSHTYPE_E *pFlushType);

/*
 * \brief Sets the transparency of a graphics layer.
 * \attention \n
 * N/A.
 * \param[in] Layer       Layer handle.
 * \param[in] pKey        Pointer to the transparency information. The value cannot be empty.
 *
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetLayerColorkey
 */
HI_S32 HI_GO_SetLayerColorkey(HI_HANDLE Layer, const HIGO_LAYER_KEY_S *pKey);

/*
 * \brief Obtains the transparency information about a graphics layer.
 * \attention \n
 * N/A.
 * \param[in] Layer       Layer handle.
 * \param[in] pKey        Pointer to the transparency information.
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetLayerColorkey
 */
HI_S32 HI_GO_GetLayerColorkey(HI_HANDLE Layer, HIGO_LAYER_KEY_S *pKey);

/*
 * \brief Sets the palette of a graphics layer.
 * \attention \n
 * N/A.
 * \param[in] Layer       Layer handle.
 * \param[in] Palette     Palette of a graphics layer.
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetLayerPalette
 */
HI_S32 HI_GO_SetLayerPalette(HI_HANDLE Layer, const HI_PALETTE Palette);

/*
 * \brief Obtains the palette of a graphics layer.
 * \attention \n
 * N/A.
 * \param[in] Layer       Layer handle.
 * \param[in] Palette     Palette of a graphics layer.
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetLayerPalette
 */
HI_S32 HI_GO_GetLayerPalette(HI_HANDLE Layer, HI_PALETTE Palette);

/*
 * \brief Waits for the blanking area.
 * \attention \n
 * N/A
 *
 * \param[in] Layer       Layer handle.
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetLayerPalette
 */
HI_S32 HI_GO_WaitForBlank(HI_HANDLE Layer);

/*
 * \brief Configures the alpha information about a graphics layer. This API is an extended API.
 *
 * \attention \n
 * The alpha0 and alpha1 of the data structure HIGO_LAYER_ALPHA_S are valid only in ARGB1555 format.
 * This API is used to implement the translucent effect in ARGB1555 format.
 *
 * ARGB1555ʽʵְ͸Ч CNend
 * \param[in] Layer       Layer handle.
 * \param[in] pAlphaInfo   Alpha information about a graphics layer. The value cannot be empty.
 *
 * \param[out] N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetLayerAlphaEx
 */
HI_S32 HI_GO_SetLayerAlphaEx(HI_HANDLE Layer, HIGO_LAYER_ALPHA_S *pAlphaInfo);

/*
 * \brief Obtains the alpha information about a graphics layer. This API is an extended API.
 *
 * \attention \n
 * N/A.
 * \param[in] Layer       Layer handle.
 * \param[out] pAlphaInfo  Alpha information about a graphics layer. The value cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetLayerAlphaEx
 */
HI_S32 HI_GO_GetLayerAlphaEx(HI_HANDLE Layer, HIGO_LAYER_ALPHA_S *pAlphaInfo);

/*
 * \brief set the Z order of graphics layer in the sample display.
 * \attention \n
 * this function make effect immediately.
 * \param[in] Layer Layer handle.
 * \param[in] ZFlag zorder flag.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVORDERFLAG
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_CANNOTCHANGE
 * \see \n
 * ::HI_GO_GetLayerZorder
 */
HI_S32 HI_GO_SetLayerZorder(HI_HANDLE Layer, HIGO_ZORDER_E enZOrder);

/*
 * \brief get the Z order of graphics layer in the sample display.
 * \attention \n
 *
 * \param[in] Layer    Layer handle.
 * \param[out] pZOrder Z Order Information.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * ::HI_GO_ChangeLayerZorder
 */
HI_S32 HI_GO_GetLayerZorder(HI_HANDLE Layer, HI_U32 *pu32ZOrder);

/*
 * \brief Sets frame input encode mode of a graphic layer.
 *
 * \attention \n
 * MONO is the default mode.
 *
 * \param[in] Layer       Layer handle.
 * \param[in] EncPicFrm  Frame encode mode.The value cannot be empty.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetStereoMode
 */
HI_S32 HI_GO_SetStereoMode(HI_HANDLE Layer, HIGO_STEREO_MODE_E InputEnc);

/*
 * \brief Obtains frame encode mode of a graphic layer.
 * \attention \n
 *
 * \param[in] Layer       Layer handle.
 * \param[out] pInputEnc  Frame encode mode.The value cannot be empty.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetStereoMode
 */
HI_S32 HI_GO_GetStereoMode(HI_HANDLE Layer, const HIGO_STEREO_MODE_E *pInputEnc);

/*
 * \brief set the 3d depth, only support depth in Horizontal .
 * \attention \n
 *
 * \param[in]  Layer       Layer handle.
 * \param[out] StereoDepth  3D depth.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetStereoDepth
 */
HI_S32 HI_GO_SetStereoDepth(HI_HANDLE Layer, HI_S32 s32StereoDepth);

/*
 * \brief get the 3d depth, only support depth in Horizontal .
 * \attention \n
 *
 * \param[in]  Layer       Layer handle.
 * \param[out] pStereoDepth  3D depth.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetStereoDepth
 */
HI_S32 HI_GO_GetStereoDepth(HI_HANDLE Layer, HI_S32 *ps32StereoDepth);

/*
 * \brief Sets compress mode of a graphic layer.
 * \attention \n
 * When compress is enable, only HIGO_PF_8888 is supported;SD layer and  STEREO TOPANDBOTTOM
 * are also not supported.
 *
 * \param[in] Layer       Layer handle.
 * \param[in] StereoMode  Stereo mode, the value cannot be empty.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_GetStereoMode
 */
HI_S32 HI_GO_SetCompression(HI_HANDLE Layer, HI_BOOL bEnable);

/*
 * \brief Sets compress mode of a graphic layer.
 * \attention \n
 * N/A.
 * \param[in] Layer       Layer handle.
 * \param[out] pbEnable  Stereo mode, the value cannot be empty.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_SetStereoMode
 */
HI_S32 HI_GO_GetCompression(HI_HANDLE Layer, const HI_BOOL *pbEnable);

/*
 * \brief Create ScrollText.
 * \attention \n
 * N/A.
 * \param[in] pstScrollAttr  ScrollText attribute,the value cannot be empty.
 * \param[out] phScrollText  ScrollText handle.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_FB
 * \retval ::HIGO_ERR_DEPEND_FB
 * \retval ::HIGO_ERR_INVSURFACEPF
 * \retval ::HIGO_ERR_INUSE
 * \see \n
 * ::HI_GO_CreateScrollText
 */
HI_S32 HI_GO_CreateScrollText(HIGO_SCROLLTEXT_ATTR_S *pstScrollAttr, HI_HANDLE *phScrollText);

/*
 * \brief Fill data to ScrollText cache buffer.
 * \attention \n
 * N/A.
 * \param[in] hScrollText  ScrollText handle.
 * \param[out] pstScrollData ScrollText data information.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_DEPEND_FB
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * ::HI_GO_FillScrollText
 */
HI_S32 HI_GO_FillScrollText(HI_HANDLE hScrollText, HIGO_SCROLLTEXT_DATA_S *pstScrollData);

/*
 * \brief Pause the ScrollText.
 * \attention \n
 * N/A.
 * \param[in] hScrollText  ScrollText handle.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_PauseScrollText
 */
HI_S32 HI_GO_PauseScrollText(HI_HANDLE hScrollText);

/*
 * \brief Resume the ScrollText,the ScrollText has Paused.
 * \attention \n
 * N/A.
 * \param[in] hScrollText  ScrollText handle.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_ResumeScrollText
 */
HI_S32 HI_GO_ResumeScrollText(HI_HANDLE hScrollText);

/*
 * \brief Destroy ScrollText.
 * \attention \n
 * N/A.
 * \param[in] hScrollText ScrollText handle.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_DEPEND_FB
 * \see \n
 * ::HI_GO_DestoryScrollText
 */
HI_S32 HI_GO_DestoryScrollText(HI_HANDLE hScrollText);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HI_GO_GDEV_H */
