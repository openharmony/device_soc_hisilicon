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

HI_S32 HI_GV_Clock_Init(HIGV_HANDLE clockHandle, const HIGV_CLOCK_STYLE_S *style);

HI_S32 HI_GV_Clock_SetFlash(HIGV_HANDLE clockHandle, HI_BOOL flash);

HI_S32 HI_GV_Clock_SetTimeUnit(HIGV_HANDLE clockHandle, const HI_CHAR *unit);

HI_S32 HI_GV_Clock_Run(HIGV_HANDLE clockHandle, HI_BOOL run);

HI_S32 HI_GV_Clock_ClearContent(HIGV_HANDLE clockHandle);

HI_S32 HI_GV_Clock_SetUTC(HIGV_HANDLE clockHandle, time_t t);

HI_S32 HI_GV_Clock_GetUTC(HIGV_HANDLE clockHandle, time_t *t);

HI_S32 HI_GV_Clock_SetSwitchItemStep(HIGV_HANDLE clockHandle, HI_U32 step, HI_U32 interval);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_SPIN_H */
