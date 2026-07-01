/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TIMER_EXT_H
#define TIMER_EXT_H

#include <pthread.h>
#include <sys/time.h>
#include "ot_mw_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
typedef void *TD_TIMER_HANDLE;
typedef void (*SS_TIMER_PROC)(TD_VOID *clientData, struct timeval *nowP);

/** timer config info */
typedef struct OtTimerConf {
    struct timeval *now;       /* timer create time point */
    long intervalMs;          /* periodic timer timeout interval or one shot timer timeout interval */
    TD_BOOL periodic;          /* periodic or ont-shot */
    SS_TIMER_PROC timerProc; /* time out call back */
    TD_VOID *clientData;
} OT_TIMER_S;

/* ---------------------------------------------
                  Function Claim
   --------------------------------------------- */

/**
  * @brief  init a  timer group.
  * @param[in] bBlock :   timer group flag.
  * @return non 0 timer group handle.
  * @return 0 init failure.
  */
TD_S32 SS_TIMER_Init(TD_BOOL bBlock);

/**
 * @brief  deinit  timer group.
 * @param[in] grpHdl :   timer group handle.
 * @return 0  deinit Successs.
 * @return non 0  error happened
 */
TD_S32 SS_TIMER_DeInit(TD_S32 grpHdl);

/**
 * @brief  create a  timer.
 * @param[in] grpHdl :   timer group handle.
 * @param[in] timerConf : timer attribute.
 * @return non 0  create Successs and return timer handle.
 * @return 0 create failed.
 */
TD_MW_PTR SS_TIMER_Create(TD_S32 grpHdl, const OT_TIMER_S *timerConf);

/**
 * @brief  reset timer.
 * @param[in] grpHdl :   timer group handle.
 * @param[in] tmrHdle : timer to reset.
 * @param[in] timeVal : scheduled start time.
 * @param[in] timeLen : timing duration; unit: millisecond.
 * @return 0  destroy Successs.
 * @return non 0  error happened
 */
TD_S32 SS_TIMER_Reset(TD_S32 grpHdl, TD_MW_PTR tmrHdle, const struct timeval *timeVal, TD_U32 timeLen);

/**
 * @brief  set tickvalue  of timer group.
 * @param[in] grpHdl : timer group handle.
 * @param[in] u32TickValUs : tick value.
 * @return 0  set Successs.
 * @return non 0  error happened
 */
TD_S32 SS_TIMER_SetTickValue(TD_S32 grpHdl, TD_U32 u32TickValUs);

/**
 * @brief  destory timer.
 * @param[in] grpHdl : timer group handle.
 * @param[in] tmrHdle : timer to destroy.
 * @return 0  destroy Successs.
 * @return non 0  error happened
 */
TD_S32 SS_TIMER_Destroy(TD_S32 grpHdl, TD_MW_PTR tmrHdle);

/**
 * @brief  clean up  timers group.
 * @param[in] grpHdl : timers group handle.
 * @return 0  set Successs.
 * @return non 0  error happened
 */
TD_S32 SS_TIMER_CleanUp(TD_S32 grpHdl);

/**
 * @brief  set periodic attr of timer.
 * @param[in] tmrHdle : timer handle.
 * @param[out] periodic : flag of peridic.
 * @return 0  set Successs.
 * @return non 0  error happened
 */
TD_S32 SS_TIMER_SetPeriodicAttr(TD_MW_PTR tmrHdle, TD_BOOL periodic);

/**
 * @brief  get past time of timer.
 * @param[in] tmrHdle : timer handle.
 * @param[out] pu32Time : pointer to past time in million second.
 * @return 0  get past time Successs.
 * @return non 0  error happened
 */
TD_S32 SS_TIMER_GetPastTime(TD_MW_PTR tmrHdle, TD_U32 *pu32Time);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

