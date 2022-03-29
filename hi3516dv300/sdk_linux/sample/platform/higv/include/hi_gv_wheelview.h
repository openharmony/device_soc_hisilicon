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

#ifndef HI_GV_WHEELVIEW_H
#define HI_GV_WHEELVIEW_H
#include "hi_type.h"
#include "hi_gv_conf.h"
#include "hi_gv_widget.h"

#ifdef HIGV_USE_WIDGET_WHEELVIEW
/* add include here */
#include "hi_gv_resm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    HI_U32 rowNum; /* The count of display item */
    HIGV_HANDLE dataModel; /* Data Model Handle */
    HIGV_HANDLE upCoverSkin; /* Up Cover Skin */
    HIGV_HANDLE downCoverSkin; /* Down Cover Skin */
    HI_BOOL isCircleScroll; /* Circle tag */
    HI_FLOAT scrollParam; /* Scroll Param */
    HI_FLOAT flingParam; /* Fling Param */
    HI_U32 leftMargin; /* Left margin */
    HI_U32 rightMargin; /* Right margin */
    HI_U32 topMargin; /* Up margin */
    HI_U32 btmMargin; /* Bottom margin */
} HigvWheelViewStyle;

/*
* brief Create wheelview.
* param[in] creatInfo    The Layout information of wheelview.
* param[out] wheelViewHandle   the pointer of wheelview handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_INVA
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_WheelView_Create(const HIGV_WCREATE_S *creatInfo, HIGV_HANDLE *wheelViewHandle);

/*
* brief Set selected item of  wheelview.
* param[in] wheelViewHandle WheelView handle.
* param[in] item  Item index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_WheelView_GetSelItem
*/
HI_S32 HI_GV_WheelView_SetSelItem(HIGV_HANDLE wheelViewHandle, HI_U32 item);

/*
* brief Get the item of WheelView.
* param[in] wheelViewHandle  WheelView handle.
* param[out] item Item index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_WheelView_SetSelItem
*/
HI_S32 HI_GV_WheelView_GetSelItem(HIGV_HANDLE wheelViewHandle, HI_U32 *item);

/*
* brief Get the total item number of WheelView.
* param[in]  wheelViewHandle  WheelView handle.
* param[out] itemNum Toal item number.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_WheelView_GetItemNum(HIGV_HANDLE wheelViewHandle, HI_U32 *itemNum);

/*
* brief Set up image resource.
* param[in] wheelViewHandle Image handle.
* param[in] handleRes The resource ID of image.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_WheelView_SetUpImage(HIGV_HANDLE wheelViewHandle, HI_RESID handleRes);

/*
* brief Set up image resource.
* param[in] wheelViewHandle Image handle.
* param[in] handleRes The resource ID of image.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_WheelView_SetDownImage(HIGV_HANDLE wheelViewHandle, HI_RESID handleRes);

/*
* brief Enable wheelview touch.
* param[in] wheelViewHandle Image handle.
* param[in] enable  Enable flag.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_WheelView_Enable(HIGV_HANDLE wheelViewHandle, HI_BOOL enable);

/*
* brief Is touch enable.
* param[in] wheelViewHandle Image handle.
* retval ::HI_TRUE  Touch is Enable.
* retval ::HI_FALSE Touch is Disable.
*/
HI_BOOL HI_GV_WheelView_IsEnable(HIGV_HANDLE wheelViewHandle);

/*
* rief Set scroll param.
* param[in] wheelViewHandle wheelview handle.
* param[in] scrollParam scroll index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_WheelView_SetScrollParam(HIGV_HANDLE wheelViewHandle, HI_FLOAT scrollParam);

/*
* brief Set fling param.
* param[in] wheelViewHandle wheelview handle.
* param[in] flingParam fling index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_WheelView_SetFlingParam(HIGV_HANDLE wheelViewHandle, HI_FLOAT flingParam);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_WHEELVIEW_H */
