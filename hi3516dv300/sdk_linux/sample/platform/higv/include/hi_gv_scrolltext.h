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

#ifndef HI_GV_SCROLLTEXT_H
#define HI_GV_SCROLLTEXT_H

/* add include here */
#include "hi_type.h"
#include "hi_go.h"
#include "hi_gv_resm.h"
#include "hi_gv_conf.h"

#ifdef HIGV_USE_WIDGET_SCROLLTEXT

#ifdef __cplusplus
extern "C" {
#endif

#define ST_STYLE_ALWAYSSCROLL 0x4

#define HIGV_ST_STYLE_ALWAYSSCROLL HIGV_STYLE_DEFINE(HIGV_WIDGET_SCROLLTEXT, ST_STYLE_ALWAYSSCROLL)

/* scroll direction */
typedef enum {
    /* left scroll */
    HIGV_DIRECTION_LEFT = 0,
    /* right scroll */
    HIGV_DIRECTION_RIGHT,
    HIGV_DIRECTION_BUTT
} HIGV_DIRECTION_E;

/*
* brief Set content of scrolltext.
* param[in] scrollTextHandle Scrolltext handle.
* param[in] image Picture resource ID.
* param[in] string String.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_ScrollText_GetContent
*/
HI_S32 HI_GV_ScrollText_SetContent(HIGV_HANDLE scrollTextHandle, HI_RESID image, const HI_CHAR *string);

/*
* brief Set content of scrolltext.
* param[in] scrollTextHandle Scrolltext handle.
* param[in] image Picture resource ID.
* param[in] string resource ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_ScrollText_GetContentByID
*/
HI_S32 HI_GV_ScrollText_SetContentByID(HIGV_HANDLE scrollTextHandle, HI_RESID image, HI_U32 strID);

/*
* brief Get content of scrolltext.
* param[in] scrollTextHandle Scrolltext handle.
* param[out] image Picture resource ID.
* param[out] string String.
* param[in] textBufLen String length.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_ScrollText_SetContent
*/
HI_S32 HI_GV_ScrollText_GetContent(HIGV_HANDLE scrollTextHandle, HI_RESID *image, HI_CHAR *string, HI_U32 textBufLen);

/*
* brief Set start position of scrolltext.
* param[in] scrollTextHandle  Scrolltext handle.
* param[in] x the x coordinate of picture.
* param[in] y the y coordinate of picture.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ScrollText_SetImagePos(HIGV_HANDLE scrollTextHandle, HI_S32 x, HI_S32 y);

/*
* brief Set start position of scrolltext.
* param[in] scrollTextHandle   Scrolltext handle.
* param[in] x the x coordinate of text.
* param[in] y the y coordinate of text.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ScrollText_SetTextPos(HIGV_HANDLE scrollTextHandle, HI_S32 x, HI_S32 y);

/*
* brief Set interval of scrolltext.
* param[in] scrollTextHandle Scrolltext handle.
* param[in] timeInterval Time interval.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ScrollText_SetTimeInterval(HIGV_HANDLE scrollTextHandle, HI_U32 timeInterval);

/*
* brief Set step of scrolltext.
* param[in] scrollTextHandle
* param[in] step Scroll step.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ScrollText_SetStep(HIGV_HANDLE scrollTextHandle, HI_U32 step);

/*
* brief Set direction of scrolltext.
* param[in] scrollTextHandle Scrolltext handle.
* param[in] direction Scroll direction.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ScrollText_SetDirection(HIGV_HANDLE scrollTextHandle, HIGV_DIRECTION_E direction);

/*
* brief Set widget content.
* param[in] scrollTextHandle Scrolltext handle.
* param[in] status The satus of scrolltext.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ScrollText_SetStatus(HIGV_HANDLE scrollTextHandle, HI_BOOL status);

/*
* brief Get the position of the content.
* param[in] scrollTextHandle Scrolltext handle.
* param[out] x         x coordinate.
* param[out] y        y coordinate.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ScrollText_GetCurPos(HIGV_HANDLE scrollTextHandle, HI_S32 *x, HI_S32 *y);

/*
* brief Reset  the position of the content.
* param[in] scrollTextHandle Scrolltext handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ScrollText_ResetPos(HIGV_HANDLE scrollTextHandle);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_SCROLLTEXT_H */
