/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef __HI_VDEC_EXP_H__
#define __HI_VDEC_EXP_H__

#include "hi_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiVDEC_EXPORT_CALLBACK_S {
    void (*pfnVdecNotify)(int module_id, int channel, int event_type, void *args);
    int (*pfnVdecSetscheduler_Dec)(int *sched_period);
    int (*pfnVdecSetscheduler_Stm)(int *sched_period);
    int (*pfnVdecSetscheduler_Syn)(int *sched_period);
} VDEC_EXPORT_CALLBACK_S;

typedef HI_S32 FN_VDEC_RegisterExpCallback(VDEC_EXPORT_CALLBACK_S *pstExpCallback);

typedef struct hiVDEC_EXPORT_SYMBOL_S {
    FN_VDEC_RegisterExpCallback *pfnVdecRegisterExpCallback;
} VDEC_EXPORT_SYMBOL_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* __HI_VDEC_EXP_H__ */

