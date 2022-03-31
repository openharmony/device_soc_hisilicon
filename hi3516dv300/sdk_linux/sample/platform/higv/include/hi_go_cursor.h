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
#ifndef _HI_GO_CURSOR_H
#define _HI_GO_CURSOR_H

#include "hi_go_comm.h"
#include "hi_go_gdev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    HI_HANDLE hCursor; /*
                        * The maximum resolution is 128x128, and the minimum resolution is 16x16 for the cursor
                        * picture. Both pixel alpha and colorkey are supported.
                        *
                        */
    HI_U32 HotspotX;   /*
                        * Horizontal coordinate of the cursor hot spot relative to the cursor picture,
                        *
                        */
    HI_U32 HotspotY;  /*
                       * Vertical coordinate of the cursor+ hot spot relative to the cursor picture,
                       *
                       */
} HIGO_CURSOR_INFO_S;

/* Status of the cursor layer */
typedef struct {
    HI_BOOL bShow; /* Whether to display the cursor layer. */
} HIGO_CURSOR_STATUS_S;

/*
 * \brief Initializes the cursor module.
 * \attention \n
 * \param N/A.
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE The display fails.
 * \retval ::HIGO_ERR_INITFAILED
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 * ::HI_GO_DeinitCursor
 */
HI_S32 HI_GO_InitCursor(void);

/*
 * \brief Deinitializes the cursor module.
 * \attention \n
 * \param N/A.
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE The display fails.
 * \retval ::HIGO_ERR_NOTINIT
 * \see \n
 * ::HI_GO_InitCursor
 */
HI_S32 HI_GO_DeinitCursor(void);

/*
 * \brief  Displays the cursor layer.
 * \attention \n
 * 1. The cursor of the output layer is located in the middle of the screen by default.
 * ĹλĬĻм CNend
 * 2. You need set the cursor information before the cursor layer is
 * displayed.
 * status of the cursor
 * layer. HI_TRUE: visible; HI_FASLE: invisible.
 * HI_TRUE:ɼ HI_FASLE:㲻ɼCNend
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE The display fails.
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NOCURSORINF
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 * ::HI_GO_GetCursorStatus
 * ::HI_GO_SetCursorInfo
 */
HI_S32 HI_GO_ShowCursor(HI_BOOL bVisible);

/*
 * \brief Obtains the current status of the cursor layer.
 * \attention \n
 * N/A.
 * \param[out] pCursorStatus Pointer to the current status of the cursor layer. The value cannot be
 * empty.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 * ::HI_GO_ShowLayer
 */
HI_S32 HI_GO_GetCursorStatus(HIGO_CURSOR_STATUS_S *pCursorStatus);

/*
 * \brief Moves the cursor to a specified position on a specified screen.
 * \attention \n
 * The cursor position can be changed only when the cursor status is
 * show.
 * \param[in] x Horizontal coordinate (in pixel) of the cursor on the screen relative to the canvas surface of a
 * graphics layer.
 * \param[in] y Vertical coordinate (in pixel) of the cursor on the screen relative to the canvas surface of a graphics
 * layer.
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE The display fails.
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 */
HI_S32 HI_GO_SetCursorPos(HI_S32 x, HI_S32 y);

/*
 * \brief Obtains the coordinates of the cursor on the screen.
 * \attention \n
 * N/A.
 * \param[out] pX Horizontal coordinate (in pixel) of the cursor on the screen.
 * \param[out] pY Vertical coordinate (in pixel) of the cursor on the screen.
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE The display fails.
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 */
HI_S32 HI_GO_GetCursorPos(HI_S32 *pX, HI_S32 *pY);

/*
 * \brief Sets the hot spot of the displayed cursor picture.
 * \attention \n
 * The minimum resolution is 16x16, and the maximum resolution is 128x128 for the cursor picture. Both colorkey and
 * alpha are supported.
 * \param[in] pCursorInfo Information about the cursor picture and coordinates of the hot
 * spot.
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE The display fails.
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVPARAM
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVSURFACESIZE
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_INVHOTSPOT
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 */
HI_S32 HI_GO_SetCursorInfo(const HIGO_CURSOR_INFO_S *pCursorInfo);

/*
 * \brief Obtains the information about the displayed cursor picture.
 * \attention \n
 * \param[in] pCursorInfo Information about the cursor picture and coordinates of the hot
 * spot.
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE The display fails.
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 */
HI_S32 HI_GO_GetCursorInfo(HIGO_CURSOR_INFO_S *pCursorInfo);

/*
 * \brief Attaches the cursor to one or more graphics layers.
 *
 * CNend \attention \n The graphics layers to which the cursor is attached must be
 * opened.
 * \retval ::HI_SUCCESS Layer ID.
 * \retval ::HI_FAILURE Success.
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_INVLAYERID
 * \retval ::HIGO_ERR_NOTOPEN
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 */
HI_S32 HI_GO_AttchCursorToLayer(HI_HANDLE hLayer);

/*
 * \brief Detaches the cursor from graphics layers.
 * \attention \n
 * \param[in] hLayer Layer ID.
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE The display fails.
 * \retval ::HIGO_ERR_INVLAYERID
 * \retval ::HIGO_ERR_DEPEND_CURSOR
 * \see \n
 */
HI_S32 HI_GO_DetachCursorFromLayer(HI_HANDLE hLayer);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
