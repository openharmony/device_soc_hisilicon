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

#ifndef HI_GV_SCROOBAL_H
#define HI_GV_SCROOBAL_H

/* add include here */
#include "hi_gv_widget.h"

#ifdef HIGV_USE_WIDGET_SCROLLBAR

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    /* Vertical style */
    SCROLLBAR_STYLE_VERTICAL,
    /* Horizontal style */
    SCROLLBAR_STYLE_HORIZONTAL,
    SCROLLBAR_BUTT
} HIGV_SCROLLBAR_SYLE_E;

/*  Vertical  scrollbar */
#define HIGV_SCROLLBAR_STYLE_VERTICAL HIGV_STYLE_DEFINE(HIGV_WIDGET_SCROLLBAR, SCROLLBAR_STYLE_VERTICAL)
/*  Horizontal ScrollBar */
#define HIGV_SCROLLBAR_STYLE_HORIZONTAL HIGV_STYLE_DEFINE(HIGV_WIDGET_SCROLLBAR, SCROLLBAR_STYLE_HORIZONTAL)

/*
* brief Set scrollbar information.
* attention :The offset of slide block is dicided by the height of left & top corner picture.
*
* param[in] scrollBarHandle Scrollbar handle.
* param[in] slideRes Resource information.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollBar_SetSlideRes(HIGV_HANDLE scrollBarHandle, HI_RESID slideRes);

/*
* brief Set the position of slide block.
* param[in] scrollBarHandle Scrollbar handle.
* param[in] pos  Slide block position.
* retval ::HI_SUCCESS Success
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_ScrollBar_GetPos
*/
HI_S32 HI_GV_ScrollBar_SetPos(HIGV_HANDLE scrollBarHandle, HI_U32 pos);

/*
* brief Get the position of slide block.
* param[in]  scrollBarHandle Scrollbar handle.
* param[out] pos Position pointer.
* retval ::HI_SUCCESS Success
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_INVAL Invalid parameter.
* see ::HI_GV_ScrollBar_SetPos
*/
HI_S32 HI_GV_ScrollBar_GetPos(HIGV_HANDLE scrollBarHandle, HI_U32 *pos);

/*
* brief Set the range of slide block.
* param[in] scrollBarHandle  Scrollbar handle.
* param[in] scrollRange The range of slide block,
* retval ::HI_SUCCESS Success
* retval ::HI_ERR_COMM_LOST Invalid handle.
*/
HI_S32 HI_GV_ScrollBar_SetScrollRange(HIGV_HANDLE scrollBarHandle, HI_U32 scrollRange);

/*
* brief Set the length of visual area.
* param[in] scrollBarHandle  Scrollbar handle.
* param[in] len The length of visual area.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
*/
HI_S32 HI_GV_ScrollBar_SetVisibleLen(HIGV_HANDLE scrollBarHandle, HI_U32 len);

/*
* brief Set the length of content.
* param[in] scrollBarHandle  Scrollbar handle.
* param[in] len
* retval ::HI_SUCCESS Success
* retval ::HI_ERR_COMM_LOST Invalid handle.
*/
HI_S32 HI_GV_ScrollBar_SetContentLen(HIGV_HANDLE scrollBarHandle, HI_U32 len);

/*
* brief Set the picture resource of the button which is pressed or normal state.
* ù°ťͰ״̬ͼƬ \attention \n This api is not support 9 pitchs style.
* The normal state and pressed
* state will not effect at the same time.
* ״̬Ͱ״̬µͼƬͬʱЧһ״̬Ч
* һ״̬ЧЧ״̬Ч״̬ʹͬһͼƬ
* ͼƬһţ֧־Źͼʽ
* param[in] scrollBarHandle  Scrollbar handle.
* param[in] upNormalImg Up arrow which is normal state.
* param[in] upPressImg Up arrow which is pressed picture..
* param[in] downNormalImg Down arrow which is normal picture.
* param[in] downPressImg Down arrow which is pressed picture.
* retval ::HI_SUCCESS Success
* retval ::HI_ERR_COMM_LOST Invalid handle.
*/
HI_S32 HI_GV_ScrollBar_SetButtonImg(HIGV_HANDLE scrollBarHandle, HI_RESID upNormalImg, HI_RESID upPressImg,
                                    HI_RESID downNormalImg, HI_RESID downPressImg);

/*
* brief No paint  scrollbar when suspend on the container widget  and content length
* not above container widget length.
*
* param[in] scrollBarHandle Scrollbar handle.
* param[in] isSuspending  Is suspending status.
* retval ::HI_SUCCESS Success
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollBar_SetStatus(HIGV_HANDLE scrollBarHandle, HI_BOOL isSuspending);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_SCROOBAL_H */
