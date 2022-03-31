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

#ifndef HI_GV_PROGRESSBAR_H
#define HI_GV_PROGRESSBAR_H

/* add include here */
#include "hi_go_comm.h"
#include "hi_gv_conf.h"

#ifdef HIGV_USE_WIDGET_PROGRESSBAR
#ifdef __cplusplus
extern "C" {
#endif

/* the macro of horizontal style */
#define PROGRESSBAR_STYLE_HORIZON 0x1

/* the macro of vertical style */
#define PROGRESSBAR_STYLE_VERTICAL 0x2


/* Horizontal style */
#define HIGV_PROGRESSBAR_STYLE_HORIZON HIGV_STYLE_DEFINE(HIGV_WIDGET_PROGRESS, PROGRESSBAR_STYLE_HORIZON)

/* Vertical style */
#define HIGV_PROGRESSBAR_STYLE_VERTICAL HIGV_STYLE_DEFINE(HIGV_WIDGET_PROGRESS, PROGRESSBAR_STYLE_VERTICAL)

/*
* brief Set range of progressbar.
* param[in] progressBarHandle Progressbar handle.
* param[in] min    Min valude.
* param[in] max  Max value.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ProgressBar_SetRange(HIGV_HANDLE progressBarHandle, HI_U32 min, HI_U32 max);

/*
* brief Set step.
* param[in] progressBarHandle Progressbar handle.
* param[in] step   step.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ProgressBar_SetStep(HIGV_HANDLE progressBarHandle, HI_U32 step);

/*
* brief Set the posithion of progressbar.
* attention :If set pos out of the range, value will be min or max, return HI_SUCCESS.
*
* param[in] progressBarHandle Progressbar handle.
* param[in] pos
* retval ::HI_SUCCESS
* \retval ::HI_ERR_COMM_LOST
* see :: HI_GV_ProgressBar_GetPos
*/
HI_S32 HI_GV_ProgressBar_SetPos(HIGV_HANDLE progressBarHandle, HI_U32 pos);

/*
* brief Get the posithion of progressbar.
* param[in] progressBarHandle Progressbar handle.
* param[out] pos  Position.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_ProgressBar_SetPos
*/
HI_S32 HI_GV_ProgressBar_GetPos(HIGV_HANDLE progressBarHandle, HI_U32 *pos);

/*
* brief Set the freground of progressbar.
* param[in] progressBarHandle  Progressbar handle.
* param[in] fgRect The range of progress.
* ΪձʾǰΧСͬ
* param[in] fgStyle   Fregroud style handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ProgressBar_SetFg(HIGV_HANDLE progressBarHandle, const HI_RECT *fgRect, HI_RESID fgStyle);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_PROGRESSBAR_H */
