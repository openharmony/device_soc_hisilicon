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

#ifndef HI_GV_SCALE_VIEW_H
#define HI_GV_SCALE_VIEW_H
#include "hi_gv_widget.h"
#include "hi_type.h"
#include "hi_gv_conf.h"

#ifdef HIGV_USE_WIDGET_SCALEVIEW
/* add include here */
#include "hi_gv_resm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HIGV_SCALEVIEW_STYLE_S {
    HIGV_HANDLE dataModel; /* Data Model Handle */
    HI_U32 rowNum; /* The count of display item */
    HI_U32 leftMargin; /* Left margin */
    HI_U32 rightMargin; /* Right margin */
    HI_U32 topMargin; /* Up margin */
    HI_U32 bottomMargin; /* Bottom margin */
    HI_U32 imgDecIndex; /* imgDecIndex */
    HI_U32 lineHeight;                 /*  ˮƽ߸߶ */
    HI_U32 lineWidth;                 /*  ˮƽ߿ */
    HI_U32 lineDistance;                 /* ˮƽߵĿݵľ */
    HI_U32 hlineColor; /* hlineColor */
    HI_U32 type;	    /* type */
    HIGV_HANDLE scaleFont;	 /* scaleFont */
    HI_COLOR tapTxtColor; /* tapTxtColor */
    HI_DOUBLE sizeGrain; /* С仯 */
    HI_BOOL isNeedTransform;  /* ĿǷҪת */
    HI_BOOL tapAutoMove;  /* ĿǷҪԶ */
    HI_U32 imageSize;                 /*  ԪͼƬ߶ */
    HI_DOUBLE imageSizeGrain;                 /*  ͼƬС仯 */
    HI_BOOL horDisplayMode; /* Ƿʾģʽ */
} HIGV_SCALEVIEW_STYLE;

/*
* brief Initiatlize the scaleview.
* attention : Set the head of ScaleView and row num.
*
* param[out] handleScaleView   ScaleView handle.
* param[in] creatInfo: widget info of ScaleView.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_SCALEVIEW_Create(const HIGV_WCREATE_S *creatInfo, HIGV_HANDLE *handleScaleView);

/*
\brief Set selected item of  ScaleView.
\attention \n

\param[in] handleScaleView ScaleView handle.
\param[in] item  Item index.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
::HI_GV_SCALEVIEW_SetSelItem
*/
HI_S32 HI_GV_SCALEVIEW_SetSelItem(HIGV_HANDLE handleScaleView, HI_U32 item);

/*
\brief Get the item of ScaleView.
\attention \n
N/A.
\param[in] handleScaleView  ScaleView handle.
\param[out] selItem Item index.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL

\see \n
::HI_GV_SCALEVIEW_GetSelItem
*/
HI_S32 HI_GV_SCALEVIEW_GetSelItem(HIGV_HANDLE handleScaleView, HI_U32 *selItem);

/*
\brief Get the total item number of ScaleView.
\attention \n
N/A.
\param[in]  handleScaleView  ScaleView handle.
\param[out] itemNum Toal item number.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_SCALEVIEW_GetItemNum(HIGV_HANDLE handleScaleView, HI_U32 *itemNum);

/*
brief Get the item of ScaleView.
\attention \n
N/A.
\param[in] handleScaleView  ScaleView handle.
\param[out] tapItem Item index.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
::HI_GV_SCALEVIEW_GetTapItem
*/
HI_S32 HI_GV_SCALEVIEW_GetTapItem(HIGV_HANDLE handleScaleView, HI_U32 *tapItem);

/*
brief Get the item of ScaleView.
\attention \n
N/A.
\param[in] handleScaleView  ScaleView handle.
\param[out] itemNum Item index.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
::HI_GV_SCALEVIEW_GetFoucItem
*/
HI_S32 HI_GV_SCALEVIEW_GetFoucItem(HIGV_HANDLE handleScaleView, HI_U32 *itemNum);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_SCALEVIEW_H */
