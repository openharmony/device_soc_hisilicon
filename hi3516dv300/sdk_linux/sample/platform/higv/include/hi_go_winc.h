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

#ifndef HI_GO_WINC_H
#define HI_GO_WINC_H

#include "hi_type.h"
#include "hi_go_comm.h"
#include "hi_go_surface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/** \addtogroup      HIGO_WINC */
/** @{ */  /** <![HIGO_WINC]  */

/* Number of buffers used by the window. */
typedef enum {
    HIGO_BUFFER_SINGLE = 0, /* The window uses a buffer. */
    HIGO_BUFFER_DOUBLE,     /* The window uses dual buffers. */
    HIGO_BUFFER_BUTT
} HIGO_BUFFERTYPE_E;

/* Maximum number of window layers. */
#define HIGO_WINTREE_NUM 17

typedef struct {
    HI_HANDLE hLayer;             /* Graphics layer on which a window is displayed. */
    HI_RECT rect;                 /* Window range. */
    HI_U32 LayerNum;              /* Display level of a window, ranging from 0 to 15. The greater the number,
                                   * the upper the position of the window.
                                   *
                                   */
    HIGO_PF_E PixelFormat;        /* Pixel format of a window. */
    HIGO_BUFFERTYPE_E BufferType; /*
                                   * Number of surface buffers required by a window.
                                   *
                                   */
    HI_BOOL bPubPallette;         /* Window surface use single Palette or not.
                                   *
                                   * ֻеڵڴģʽΪڴʱñ־Ч
                                   */
} HIGO_WNDINFO_S;
/** @} */  /*! <!-- Structure Definition end */
typedef enum {
    HIGO_WNDMEM_SHARED = 0, /* All windows share memory. */
    HIGO_WNDMEM_SEP,        /* Window uses it's own memory. */
    HIGO_WNDMEM_BUTT,
} HIGO_WNDMEM_E;

typedef struct {
    HI_HANDLE hWindow;
    HI_REGION InvRgn;
} HIGO_INVRGN_S;

/** \addtogroup      HIGO_WINC */
/** @{ */  /** <![HIGO_WINC] */

/**
\brief Creates a window on a specified graphics layer and sets the pixel format of a window.

\attention \n
 The new window is always located on the top of a specified graphics layer. The parts of the canvas surface that
 exceed the graphics layer are cropped.

\param[in] pInfo Information for creating a window. Only the following pixel formats are supported:

    HIGO_PF_CLUT8
    HIGO_PF_4444
    HIGO_PF_0444
    HIGO_PF_1555
    HIGO_PF_0555
    HIGO_PF_565
    HIGO_PF_8565
    HIGO_PF_8888
    HIGO_PF_0888 CNend
    The number of the layer where a window is located must range from 0 to 15. The greater the ID, the upper the
position of the window.

\param[out] pWindow  Address for storing window handles. The value cannot be empty.


\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVPF

\see \n
::HI_GO_DestroyWindow
*/
HI_S32 HI_GO_CreateWindowEx(const HIGO_WNDINFO_S *pInfo, HI_HANDLE *pWindow);

/**
\brief Creates a window on a specified graphics layer.
\attention \n
If the pixel format of a graphics layer contains pixel alpha, you can choose whether to use pixel alpha for the
surface of the window.

\param[in] hLayer  Handle of the graphics layer.
\param[in] pRect Rectangle window region. If the value is empty, it indicates the entire desktop region.

\param[in] LayerNum Number of the layer where a window is located. The number ranges from 0 to 15.

\param[in] bHasPixelAlpha Whether a window contains pixel alpha.

\param[out] pWindow  Address for storing window handles. The value cannot be empty.

\param[out] eBufferType Type of the surface buffer used by a window.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_DestroyWindow
*/
HI_S32 HI_GO_CreateWindow(HI_HANDLE hLayer, const HI_RECT *pRect, HI_U32 LayerNum,
    HI_BOOL bHasPixelAlpha, HI_HANDLE *pWindow, HIGO_BUFFERTYPE_E eBufferType);

/**
\brief Destroys a window.
\attention \n
\param[in] Window Window handle to be destroyed.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVPARAM

\see \n
::HI_GO_CreateWindow
*/
HI_S32 HI_GO_DestroyWindow(HI_HANDLE Window);

/**
\brief Obtains a window rectangle.
\attention \n
N/A.
\param[in] Window Window handle.
\param[out] pRect Window rectangle address. The value cannot be empty.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_SetWindowPos \n
::HI_GO_ResizeWindow
*/
HI_S32 HI_GO_GetWindowRect(HI_HANDLE Window, HI_RECT *pRect);

/**
\brief Sets the start position of a window on the attached layer.
\attention \n
N/A.
\param[in] Window Window handle.
\param[in] StartX Horizontal coordinate.
\param[in] StartY Vertical coordinate.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_SetWindowPos(HI_HANDLE Window, HI_S32 StartX, HI_S32 StartY);

/**
\brief Changes the window size.
\attention \n
After the window size is changed, you need to redraw the window.
\param[in] Window Window handle.
\param[in] Width Window width.
\param[in] Height Window height.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVSURFACESIZE
\retval ::HIGO_ERR_INVSURFACEPF
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_NOMEM

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_ResizeWindow(HI_HANDLE Window, HI_S32 Width, HI_S32 Height);

/**
\brief Obtains the z-order of a window. The smaller the z-order, the lower the position of the window.

\attention \n
N/A.
\param[in] Window Window handle.
\param[out] pZOrder Address for storing the window Z-order. The value cannot be empty.


\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_ChangeWindowZOrder
*/
HI_S32 HI_GO_GetWindowZOrder(HI_HANDLE Window, HI_U32 *pZOrder);

/**
\brief Changes the z-order of a window.
\attention \n
N/A.
\param[in] Window Window handle.
\param[in] EnType Window z-order adjustment mode.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVZORDERTYPE
\retval ::HIGO_ERR_LOCKED

\see \n
::HI_GO_GetWindowZOrder
*/
HI_S32 HI_GO_ChangeWindowZOrder(HI_HANDLE Window, HIGO_ZORDER_E EnType);

/**
\brief Obtains the window transparency.
\attention \n
N/A.
\param[in] Window Window handle.
\param[out] pOpacity Address for storing the information about window transparency. The value cannot be empty.


\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetWindowOpacity
*/
HI_S32 HI_GO_GetWindowOpacity(HI_HANDLE Window, HI_U8 *pOpacity);

/**
\brief Sets the window transparency.
\attention \n
N/A.
\param[in] Window Window handle.
\param[in] Opacity Window transparency, ranging from 0 to 255. 0: opaque, 255: full transparent.


\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetWindowOpacity
*/
HI_S32 HI_GO_SetWindowOpacity(HI_HANDLE Window, HI_U8 Opacity);

/**
\brief Sets the window colorkey.
\attention \n
\param[in] Window Window handle.
\param[in] ColorKey Colorkey value.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
N/A.
*/
HI_S32 HI_GO_SetWindowColorkey(HI_HANDLE Window, HI_COLOR ColorKey);

/**
\brief Obtains the surface handle of a window.
\attention \n
If the window has dual buffers and you have called HI_GO_FlipWindowSurface, you need to call HI_GO_GetWindowSurface
again to obtain another buffer for drawing.


\param[in] Window   Window handle.
\param[in] pSurface Address for storing window surfaces. The value cannot be empty.


\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_FlipWindowSurface \n
*/
HI_S32 HI_GO_GetWindowSurface(HI_HANDLE Window, HI_HANDLE *pSurface);

/**
\brief Refreshes a window.
\attention \n
After changing the window surface contents, you need to call the API for the changes to take effect.


\param[in] Window Window handle.
\param[in] pRect Updated window region.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.
*/
HI_S32 HI_GO_UpdateWindow(HI_HANDLE Window, const HI_RECT *pRect);

/**
\brief Changes the graphics layer to which a window belongs and retains the window.

\attention \n
N/A.
\param[in] hWindow Window handle.
\param[in] hNewLayer  Handle of the new layer.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.
*/
HI_S32 HI_GO_ChangeWindowLayer(HI_HANDLE hWindow, HI_HANDLE hNewLayer);

/**
\brief Switches the surface of a window. This function is valid only for the window that has double surfaces.

\attention \n
N/A.
\param[in] hWindow Window handle.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.
*/
HI_S32 HI_GO_FlipWindowSurface(HI_HANDLE hWindow);

/**
\brief Set window memory mode.
\attention \n
Must set the mode before Window create,Default mode is HIGO_WNDMEM_SEP.

ռڴ洰ں͹ڴ洰ͬʱ. CNend
\param[in] Window memory mode.

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM

\see \n
N/A.
*/
HI_S32 HI_GO_SetWindowMode(HIGO_WNDMEM_E enMode);

/**
\brief Get window memory mode.
\attention \n
\param[in]
\param[out]  Window memory mode.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR

\see \n
N/A.
*/
HI_S32 HI_GO_GetWindowMode(HIGO_WNDMEM_E *penMode);

/**
\brief Get clip region of window.
\attention \n
In HIGO_WNDMEM_SHARED mode, Can use this API to get window clip region, and to draw.

л. CNend
\param[in] window handle.
\param[out]  window clip region.
\param[out]  number of clip region.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\see \n
N/A.
*/
HI_S32 HI_GO_GetWindowClipRgn(HI_HANDLE hWindow, HI_REGION **ppClipRgn, HI_U32 *pu32Num);

/**
\brief Get window invalid region.
\attention \n

).ÿڵЧΪһ,þֻЧİ,Ҫ
ʹڼк,ܱ֤ʾȷ.
޸Ĵڵ͸(ڴģʽֻ͸Ͳ͸,֧
͸,а͸͸)/ƶʱ,ʹøýӿڻȡ
ɴ˲Ч,Ȼ». CNend
\param[in] window handle.
\param[out]  window invalid region.
\param[out]  number of invalid region.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\see \n
N/A.
*/
HI_S32 HI_GO_GetWindowInvRgn(HI_HANDLE hWindow, HIGO_INVRGN_S **ppInvRgn, HI_U32 *pu32Num);

HI_S32 HI_GO_SetWindowDispRegion(HI_HANDLE window, HI_RECT *region);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HI_GO_WINC_H */
