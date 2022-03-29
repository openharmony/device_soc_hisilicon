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

#ifndef HI_GV_COREMOD_H
#define HI_GV_COREMOD_H

#include "hi_type.h"
#include "hi_gv_conf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum {
    /* *
        VSYNC (HIGV_MODID_VSYNC + 14) WIDGET (HIGV_COMMON_MODID_START
       + 15) */
    HIGV_MODID_NULL = HIGV_COMMON_MODID_START,
    HIGV_MODID_WIDGET,
    HIGV_MODID_CONTAINER,
    HIGV_MODID_GC,
    HIGV_MODID_MTASK,
    HIGV_MODID_MSGM,
    HIGV_MODID_TIMER,
    HIGV_MODID_RESM,
    HIGV_MODID_IM,
    HIGV_MODID_WM,
    HIGV_MODID_DDB,
    HIGV_MODID_ADM,
    HIGV_MODID_PARSER,
    HIGV_MODID_ANIM,
    HIGV_MODID_VSYNC,
    HIGV_MODID_CORE_BUTT
} HIGV_MODID_CORE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HI_GV_COREMOD_H */
