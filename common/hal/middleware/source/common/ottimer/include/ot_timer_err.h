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
#ifndef __TIMER_ERR_H__
#define __TIMER_ERR_H__

#include "ot_error_def.h"  /* error code generation rules */
#include "ot_defs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* From 0x40 to 0xFF */
typedef enum {
    APP_TIMER_ERR_CANCEL = 0xd4,  /* cancel timer error */
    APP_TIMER_ERR_CLEANUP = 0xd5, /* clean up timer error */
    APP_TIMER_ERR_RESET = 0xd6,   /* reset timer error */
    APP_TIMER_ERR_BUTT = 0xFF
} APP_TIMER_ERR_CODE_E;

#define OT_ERR_TIMER_NULL_PTR OT_APP_DEF_ERR(OT_APPID_TIMER, APP_ERR_LEVEL_ERROR, APP_ERR_NULL_PTR)
#define OT_ERR_TIMER_CANCEL   OT_APP_DEF_ERR(OT_APPID_TIMER, APP_ERR_LEVEL_ERROR, APP_TIMER_ERR_CANCEL)
#define OT_ERR_TIMER_CLEANUP  OT_APP_DEF_ERR(OT_APPID_TIMER, APP_ERR_LEVEL_ERROR, APP_TIMER_ERR_CLEANUP)
#define OT_ERR_TIMER_RESET    OT_APP_DEF_ERR(OT_APPID_TIMER, APP_ERR_LEVEL_ERROR, APP_TIMER_ERR_RESET)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __TIMER_ERR_H__ */
