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

#ifndef HI_GV_SCROLLVIEW_H
#define HI_GV_SCROLLVIEW_H

/* add include here */
#include "hi_gv_widget.h"
#ifdef HIGV_USE_WIDGET_SCROLLVIEW
#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: It is only valid for touch operation or mouse wheel operation */
typedef enum {
    SCROLLVIEW_DIRECTION_HORI = 0,  /* Horizal direction */
    SCROLLVIEW_DIRECTION_VERT,  /* Vertical direction */
    SCROLLVIEW_DIRECTION_BUTT
} HIGV_SCROLLVIEW_DIRECTION_E;

typedef enum {
    SCROLLVIEW_STYLE_DEFAULTE = 0x00,
    SCROLLVIEW_STYLE_ALWAY_CENTER = 0x01,
    SCROLLVIEW_STYLE_BUTT = 0x10000
} HIGV_SCROLLVIEW_STYLE_E;

typedef struct hiHIGV_SCROLLVIEW_Init_S {
    HIGV_HANDLE hVerticalScrollBar; /* Bind vertical scrollbar handle */
    HIGV_HANDLE hHorizontalScrollbar; /* Bind horizontal scrollbar handle */
    HI_U32 LeftMargin; /* Invalid value */
    HI_U32 RightMargin; /* Child widget right margin of background */
    HI_U32 TopMargin; /* Invalid value */
    HI_U32 BottomMargin; /* Child widget bottom margin of background */
    HI_U32 Step; /* Scroll step */
    HI_U32 Interval; /* Scroll interval */
    HI_U32 ScrollContentWidth; /* The scroll content invariable width */
    HI_U32 ScrollContentHeight; /* The scroll content invariable height */
    HI_U32 ScrollViewStyle; /* Scroll view private style */
    HIGV_SCROLLVIEW_DIRECTION_E Direction; /* Scroll direction */
} HIGV_SCROLLVIEW_Init_S;

typedef struct {
    HI_S32 cordX; /* ӿƫx */
    HI_S32 cordY; /* ӿƫy */
} HigvScrollViewCord;

/*
* brief Init the scrollview.
* param[in]  scrollViewHandle Scroll view handle.
* param[in]  initAttr The info of scrollview when created.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_EXIST
*/
HI_S32 HI_GV_ScrollView_Init(HIGV_HANDLE scrollViewHandle, const HIGV_SCROLLVIEW_Init_S *initAttr);

/*
* brief Set content invariable size.
* param[in]  scrollViewHandle Scroll view handle.
* param[in]  width The invariable width.
* param[in]  height The invariable height.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_PAERM
*/
HI_S32 HI_GV_ScrollView_SetContentSize(HIGV_HANDLE scrollViewHandle, HI_U32 width, HI_U32 height);

/*
* brief Move the view to center.
* param[in]  scrollViewHandle scroll view handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_ScrollView_MoveToCenter(HIGV_HANDLE scrollViewHandle);

/*
* brief Get the content rectangle.
* param[in]  scrollViewHandle Scroll view handle.
* param[out] rect Rectangle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_GetContentRect(HIGV_HANDLE scrollViewHandle, HI_RECT *rect);

/*
* brief Get the view coordinate in the background content.
* attention \n
     _____________________
    |  background content |
    |        ____         |
    |       |view|        |
    |       |____|        |
    |_____________________|
* param[in]  scrollViewHandle Scroll view handle.
* param[out] cordX X abscissa.
* param[out] cordY Y abscissa.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_GetViewCoordinate(HIGV_HANDLE scrollViewHandle, HIGV_CORD *cordX, HIGV_CORD *cordY);

/*
* brief Set the view coordinate in the background content.
* param[in]  scrollViewHandle Scroll view handle.
* param[out] cordX X abscissa.
* param[out] cordY Y abscissa.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_SetViewCoordinate(HIGV_HANDLE scrollViewHandle, HIGV_CORD cordX, HIGV_CORD cordY);

/*
* brief Set scroll step.
* param[in]  scrollViewHandle Scroll view handle.
* param[in]  step Scroll step.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_SetStep(HIGV_HANDLE scrollViewHandle, HI_U32 step);

/*
* brief Set scroll Interval.
* param[in]  scrollViewHandle Scroll view handle.
* param[in]  interval Interval.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_SetInterval(HIGV_HANDLE scrollViewHandle, HI_U32 interval);

/*
* brief Bind the scrollbar to scrollbar.
* param[in]  scrollViewHandle Scroll view handle.
* param[in]  handleVertical vertical scrollbar.
* param[in]  handleHorizontal horizontal scrollbar.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_BindScrollBar(HIGV_HANDLE scrollViewHandle, HIGV_HANDLE handleVertical,
    HIGV_HANDLE handleHorizontal);

/*
* brief Check the focus widget in the view.
* param[in]  scrollViewHandle Scroll view handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_CheckFocusPos(HIGV_HANDLE scrollViewHandle);

/*
* brief Set scroll sensibility.
* param[in]  scrollViewHandle Scroll view handle.
* param[in]  scrollParam scroll sensibility param.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_SetScrollParam(HIGV_HANDLE scrollViewHandle, HI_FLOAT scrollParam);

/*
* brief Set fling sensibility.
* param[in]  scrollViewHandle Scroll view handle.
* param[in]  flingParam fling sensibility param.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollView_SetFlingParam(HIGV_HANDLE scrollViewHandle, HI_FLOAT flingParam);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_SCROLLVIEW_H */
