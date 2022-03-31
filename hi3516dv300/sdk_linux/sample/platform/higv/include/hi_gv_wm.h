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

#ifndef HI_GV_WM_H
#define HI_GV_WM_H

#include "hi_type.h"
#include "hi_gv_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Stack capacity */
#define HIGV_MAX_WINNUM 50

typedef enum {
    HI_GV_WNDMEM_SHARED = 0, /* 所有窗口共用一块内存 */
    HI_GV_WNDMEM_SEP,        /* 单独使用一块内存 */
    HI_GV_WNDMEM_BUTT,
} HIGV_WndMemType;

/*
* brief Push window to statck.
* param[in] layerHandle  Layer handle.
* param[in] windowHandle Window handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_FULL Stack full.
* see ::HI_GV_WM_PopWindow
*/
HI_S32 HI_GV_WM_PushWindow(HIGV_HANDLE layerHandle, HIGV_HANDLE windowHandle);

/*
* brief Pop up window from stack.
* param[in] layerHandle  Layer handle.
* param[in] windowHandle Window handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_EMPTY Stack empty.
* see ::HI_GV_WM_PushWindow
*/
HI_S32 HI_GV_WM_PopWindow(HIGV_HANDLE layerHandle, HIGV_HANDLE windowHandle);

/*
* brief Get top window of stack.
* param[in] layerHandle  Layer handle.
* param[out] windowHandle Window handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_EMPTY Stack empty.
*/
HI_S32 HI_GV_WM_GetTopWindow(HIGV_HANDLE layerHandle, HIGV_HANDLE *windowHandle);

/*
* brief Clear window of the stack.
* param[in] layerHandle  Layer handle.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_WM_ClearAllWindow(HIGV_HANDLE layerHandle);

/*
* brief Get capture mouse window.
* param[in]  layerHandle    Layer handle.
* param[out] widgetHandle  The window which capture mouse.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_WM_GetMouseCapture(HIGV_HANDLE layerHandle, HIGV_HANDLE *widgetHandle);

/*
* brief Set the capture mouse window.
* param[in] layerHandle  Layer handle.
* param[in] widgetHandle Set the window which capture mouse.
* retval ::HI_SUCCESS
* N/A.
*/
HI_S32 HI_GV_WM_SetMouseCapture(HIGV_HANDLE layerHandle, HIGV_HANDLE widgetHandle);

/*
* brief Get the higo layer handle corresponding layer.
* attention :
* param[in] layerHandle     Layer handle.
* param[out] higoLayer Higo layer handle.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_WM_GetHigoLayer(HIGV_HANDLE layerHandle, HIGV_HANDLE *higoLayer);

/*
* brief Touch msg bind with widget.
* param[in] layerHandle  Layer handle.
* param[in] isBind Bind tag, HI_TRUE or HI_FALSE.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_WM_BindTouchMsg(HIGV_HANDLE layerHandle, HI_BOOL isBind);

/*
* brief Get bind status.
* param[in] layerHandle  Layer handle.
* param[in] isBind Bind status .
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_WM_GetBindTouchStatus(HIGV_HANDLE layerHandle, HI_U32 *isBind);

/*
* brief Set window mem mode.
* param[in] layerHandle  Layer handle.
* param[in] mode Window mem mode .
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_WM_SetWndMemMode(HIGV_HANDLE layerHandle, HIGV_WndMemType mode);

/*
* brief Get window mem mode.
* param[in] layerHandle  Layer handle.
* param[in] mode Window mem mode .
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_WM_GetWndMemMode(HIGV_HANDLE layerHandle, HIGV_WndMemType *mode);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_WM_H */
