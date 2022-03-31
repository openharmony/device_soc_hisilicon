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

#ifndef HI_GV_TRACKBAR_H
#define HI_GV_TRACKBAR_H

#include "hi_gv_conf.h"
#ifdef HIGV_USE_WIDGET_TRACKBAR
/* add include here */
#include "hi_type.h"
#include "hi_go.h"
#include "hi_gv_resm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum HI_TRACKBAR_STYLE {
    TRACKBAR_H = 0,
    TRACKBAR_V,
    TRACKBAR_BUT
} TRACKBAR_STYLE;

typedef enum hiHIGV_TRACKBAR_E {    /* Trackbar type */
    HIGV_TRACKBAR_NORMAL = 0,
    HIGV_TRACKBAR_ACTIVE,
    HIGV_TRACKBAR_MOUSEDOWN,
    HIGV_TRACKBAR_BUTT
} HIGV_TRACKBAR_E;

#define HIGV_TRACKBAR_H HIGV_STYLE_DEFINE(HIGV_WIDGET_TRACKBAR, TRACKBAR_H)
#define HIGV_TRACKBAR_V HIGV_STYLE_DEFINE(HIGV_WIDGET_TRACKBAR, TRACKBAR_V)

/*
* brief Set skin of trackbar.
* param[in] trackBarHandle Trackbar handle.
* param[in] handleSkinIndex  Skin index.
* param[in] handleRes    Picture resource ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Track_SetSliImage(HIGV_HANDLE trackBarHandle, HI_U32 handleSkinIndex, HI_RESID handleRes);

/*
* brief Set the skin of slideblock.
* param[in] trackBarHandle    Trackbar handle.
* param[in] handleSkinIndex  Skin index.
* param[in] handleRes   Picture resource ID of track bar.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Track_SetTraImage(HIGV_HANDLE trackBarHandle, HI_U32 handleSkinIndex, HI_RESID handleRes);

/*
* brief Set the value of trackbar.
* param[in] trackBarHandle    Trackbar handle.
* param[in] value     Get current valude of cursor.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Track_SetCurVal(HIGV_HANDLE trackBarHandle, HI_U32 value);

/*
* brief Get current graduation of cursror.
* param[in] trackBarHandle      Trackbar handle.
* param[out] value     Get value of current cursor.
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Track_GetCurVal(HIGV_HANDLE trackBarHandle, HI_U32 *value);

/*
* brief Set the range of cursor.
* param[in] trackBarHandle     Trackbar handle.
* param[in] maxnValue Max value of cursor.
* param[in] minValue  Min value of cursor.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Track_SetRange(HIGV_HANDLE trackBarHandle, HI_U32 minValue, HI_U32 maxValue);

/*
* brief Get the range of cursor.
* param[in]  trackBarHandle     Trackbar handle.
* param[out] maxValue  Max value of cursor.
* param[out] minValue  Min value of cursor.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Track_GetRange(HIGV_HANDLE trackBarHandle, HI_U32 *minValue, HI_U32 *maxValue);

/*
* brief Set gesture enable.
* param[in]  trackBarHandle  Scrollgrid handle.
* param[in]  isEnable   Is enable gesture.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Track_EnableGesture(HIGV_HANDLE trackBarHandle, HI_BOOL isEnable);

/*
* brief Get gesture enable.
* param[in]  trackBarHandle  Scrollgrid handle.
* param[out]  isEnable   Is enable gesture.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Track_IsGestureEnable(HIGV_HANDLE trackBarHandle, HI_BOOL *isEnable);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_TRACKBAR_H */
#endif
