/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: netbuf enum type head file.
 * Create: 2022-11-26
 */

#ifndef FRW_UTIL_COMMON_H
#define FRW_UTIL_COMMON_H

#include "frw_util_common_rom.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif

/* 定义功能开关 */
typedef enum {
    EXT_SWITCH_OFF = 0,
    EXT_SWITCH_ON = 1,
    EXT_SWITCH_PAUSE = 2,
    EXT_SWITCH_BUTT
} ext_switch_enum;

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
#endif /* end of frw_util_common.h */