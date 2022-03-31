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

HI_S32 HI_GO_CreateWindowEx(const HIGO_WNDINFO_S *pInfo, HI_HANDLE *pWindow);

HI_S32 HI_GO_CreateWindow(HI_HANDLE hLayer, const HI_RECT *pRect, HI_U32 LayerNum,
    HI_BOOL bHasPixelAlpha, HI_HANDLE *pWindow, HIGO_BUFFERTYPE_E eBufferType);

HI_S32 HI_GO_DestroyWindow(HI_HANDLE Window);

HI_S32 HI_GO_GetWindowRect(HI_HANDLE Window, HI_RECT *pRect);

HI_S32 HI_GO_SetWindowPos(HI_HANDLE Window, HI_S32 StartX, HI_S32 StartY);

HI_S32 HI_GO_ResizeWindow(HI_HANDLE Window, HI_S32 Width, HI_S32 Height);

HI_S32 HI_GO_GetWindowZOrder(HI_HANDLE Window, HI_U32 *pZOrder);

HI_S32 HI_GO_ChangeWindowZOrder(HI_HANDLE Window, HIGO_ZORDER_E EnType);

HI_S32 HI_GO_GetWindowOpacity(HI_HANDLE Window, HI_U8 *pOpacity);

HI_S32 HI_GO_SetWindowOpacity(HI_HANDLE Window, HI_U8 Opacity);

HI_S32 HI_GO_SetWindowColorkey(HI_HANDLE Window, HI_COLOR ColorKey);

HI_S32 HI_GO_GetWindowSurface(HI_HANDLE Window, HI_HANDLE *pSurface);

HI_S32 HI_GO_UpdateWindow(HI_HANDLE Window, const HI_RECT *pRect);

HI_S32 HI_GO_ChangeWindowLayer(HI_HANDLE hWindow, HI_HANDLE hNewLayer);

HI_S32 HI_GO_FlipWindowSurface(HI_HANDLE hWindow);

HI_S32 HI_GO_SetWindowMode(HIGO_WNDMEM_E enMode);

HI_S32 HI_GO_GetWindowMode(HIGO_WNDMEM_E *penMode);

HI_S32 HI_GO_GetWindowClipRgn(HI_HANDLE hWindow, HI_REGION **ppClipRgn, HI_U32 *pu32Num);

HI_S32 HI_GO_GetWindowInvRgn(HI_HANDLE hWindow, HIGO_INVRGN_S **ppInvRgn, HI_U32 *pu32Num);

HI_S32 HI_GO_SetWindowDispRegion(HI_HANDLE window, HI_RECT *region);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* HI_GO_WINC_H */
