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
#include "higv_cextfile.h"
#include "sample_utils.h"
#include "hi_gv_parser.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */

#define  TIMER_SPEED  (3000)

static HI_U32 g_timerId = 2;

HI_S32 LABEL_WIN_Onrefresh(HIGV_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam)
{
    HIGV_UNUSED(wParam);
    HIGV_UNUSED(lParam);

    HI_S32 ret = HI_GV_Timer_Create(widget, g_timerId, TIMER_SPEED);
    HIGV_CHECK("HI_GV_Timer_Create", ret);

    ret = HI_GV_Timer_Start(widget, g_timerId);
    HIGV_CHECK("HI_GV_Timer_Start", ret);

    return HIGV_PROC_GOON;
}

HI_S32 LABEL_WIN_Ontimer(HIGV_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam)
{
    HIGV_UNUSED(widget);
    HIGV_UNUSED(wParam);
    HIGV_UNUSED(lParam);

    HI_U32 ret = (HI_U32)HI_GV_PARSER_LoadViewById(IMAGE_WIN);
    ret |= (HI_U32)HI_GV_Widget_Show(IMAGE_WIN);
    ret |= (HI_U32)HI_GV_Widget_Active(IMAGE_WIN);
    ret |= (HI_U32)HI_GV_Widget_Hide(LABEL_WIN);

    HIGV_CHECK("HI_GV_Widget_Show", ret);
    return HIGV_PROC_GOON;
}

HI_S32 LABEL_WIN_Onhide(HIGV_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam)
{
    HIGV_UNUSED(wParam);
    HIGV_UNUSED(lParam);

    HI_U32 ret = (HI_U32)HI_GV_Timer_Stop(widget, g_timerId);
    ret |= (HI_U32)HI_GV_Timer_Destroy(widget, g_timerId);

    HIGV_CHECK("HI_GV_Timer_Destory", ret);

    return HIGV_PROC_GOON;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

