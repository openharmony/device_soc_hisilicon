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

#ifndef HI_GV_TIMER_H
#define HI_GV_TIMER_H

#include "hi_type.h"
#include "hi_gv_conf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* * The timer Max number */
#define TIMER_MAX_NUM 0x40

/*
* brief Create timer.
* param[in] widgetHandle Widget handle.
* param[in] timerId Timer ID.
* param[in] speed  Timer interval.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_EXIST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
* see ::HI_GV_Timer_Destroy
*/
HI_S32 HI_GV_Timer_Create(HIGV_HANDLE widgetHandle, HI_U32 timerId, HI_U32 speed);

/*
* brief Destroy timer.
* param[in] widgetHandle Widget handle.
* param[in] timerId Timer ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP Couldn't stop timer.
* see ::HI_GV_Timer_Create
*/
HI_S32 HI_GV_Timer_Destroy(HIGV_HANDLE widgetHandle, HI_U32 timerId);

/*
* brief Start timer.
* param[in] widgetHandle Widget handle.
* param[in] timerId  Timer ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP
* see ::HI_GV_Timer_Stop
*/
HI_S32 HI_GV_Timer_Start(HIGV_HANDLE widgetHandle, HI_U32 timerId);

/*
* brief Stop timer.
* param[in] widgetHandle Widget handle.
* param[in] timerId  Timer ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP Couldn't stop timer.
* see::HI_GV_Timer_Start
*/
HI_S32 HI_GV_Timer_Stop(HIGV_HANDLE widgetHandle, HI_U32 timerId);

/*
* brief Reset timer.
* param[in] widgetHandle  Widget handle.
* param[in] timerId  Timer ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP Couldn't stop timer.
*/
HI_S32 HI_GV_Timer_Reset(HIGV_HANDLE widgetHandle, HI_U32 timerId);

/*
* brief Set timer speed.
* param[in] widgetHandle  Widget handle.
* param[in] timerId  Timer ID.
* param[in] speed  Timer speed(unit: ms).
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Timer_SetSpeed(HIGV_HANDLE widgetHandle, HI_U32 timerId, HI_U32 speed);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* HI_GV_TIMER_H */
