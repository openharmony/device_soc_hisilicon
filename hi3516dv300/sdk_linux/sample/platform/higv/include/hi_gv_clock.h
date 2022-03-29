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

#ifndef HI_GV_CLOCK_H
#define HI_GV_CLOCK_H

/* add include here */
#include <time.h>
#include "hi_gv_widget.h"
#ifdef HIGV_USE_WIDGET_CLOCK
#ifdef __cplusplus
extern "C" {
#endif

typedef struct hiHIG_TIME_S {
    HI_U32 year; /* Year , */
    HI_U32 month; /* Month , */
    HI_U32 day; /* Day , */
    HI_U32 week; /* Week , */
    HI_U32 hour; /* Hour , */
    HI_U32 minute; /* Minute, */
    HI_U32 second; /* Second , */
} HIGV_TIME_S;

typedef enum {
    HIGV_CURSORRES_TYPE_COLOR = 0,
    HIGV_CURSORRES_TYPE_IMAGE = 1,
    HIGV_CURSORRES_TYPE_BUTT,
} HIGV_CURSORRES_TYPE_E;

typedef enum {
    HIGV_CLOCK_MODE_TEXT = 0,
    /* User set time information by text , */
    HIGV_CLOCK_MODE_FORMAT,
    /* User set time information by time format , */
    HIGV_CLOCK_MODE_BUTT,
} HIGV_CLOCK_MODE_E;

typedef struct {
    HIGV_CLOCK_MODE_E DispMode; /* display mode ; */
    HIGV_CURSORRES_TYPE_E CursorResType; /* The  cursor resource type ; */
    /* The resource ID of cursor ; */
    HI_U32 CursorRes;
} HIGV_CLOCK_STYLE_S;

/*
* brief Init the clock.
* param[in]  clockHandle Clock handle.
* param[in]  style The style of clock when created.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_EMPTY
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_Clock_Init(HIGV_HANDLE clockHandle, const HIGV_CLOCK_STYLE_S *style);

/*
* brief Set the colon whether blink.
* param[in]  clockHandle Clock handle.
* param[out] flash Whether blink.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Clock_SetFlash(HIGV_HANDLE clockHandle, HI_BOOL flash);

/*
* brief Set time unit of clock widget.
*
* (1)"second"-룬"minute"-֣"hour"-ʱ"day"-գ"week"-ܣ"month"-£"year"-
* (2)øýӿڻԶclockؼ
* param[in]  clockHandle Clock handle.
* param[out] unit  Time unit.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_EMPTY
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_Clock_SetTimeUnit(HIGV_HANDLE clockHandle, const HI_CHAR *unit);

/*
* brief Control the clock run state.
* param[in]  clockHandle Clock handle.
* param[out] run  Start or stop.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_EMPTY
*/
HI_S32 HI_GV_Clock_Run(HIGV_HANDLE clockHandle, HI_BOOL run);

/*
* brief Clear the content of clock.
* param[in]  clockHandle Clock handle.ؼ
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_EMPTY
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_Clock_ClearContent(HIGV_HANDLE clockHandle);

/*
* brief Set UTC time.
* param[in]  clockHandle Clock handle.
* param[in]  t UTC.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_EMPTY
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_Clock_SetUTC(HIGV_HANDLE clockHandle, time_t t);

/*
* brief Get UTC time.
* param[in]  clockHandle Clock handle.
* param[out]  t The pointer of UTC.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_EMPTY
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_Clock_GetUTC(HIGV_HANDLE clockHandle, time_t *t);

/*
* brief Set the item switch step.
* attention : Only support the year item.
* param[in]  clockHandle Clock handle.
* param[in]  step The step of switch item.
* param[in]  interval The interval time of switch item
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_EMPTY
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_Clock_SetSwitchItemStep(HIGV_HANDLE clockHandle, HI_U32 step, HI_U32 interval);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_SPIN_H */
