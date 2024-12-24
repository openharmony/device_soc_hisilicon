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
 * Description: extern interface
 */

#ifndef OAL_EXT_IF_ROM_H
#define OAL_EXT_IF_ROM_H

#include "oal_types_device_rom.h"
#include "frw_util_rom.h"
#include "oal_mem_rom.h"
#include "oal_net_rom.h"
#include "oal_fsm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define WIFI_DMAC_TCM_TEXT __attribute__((section(".wifi.tcm.text")))
#define WIFI_DMAC_TCM_RODATA __attribute__((section(".wifi.tcm.rodata")))

typedef enum {
    OAL_TRACE_ENTER_FUNC,
    OAL_TRACE_EXIT_FUNC,
    OAL_TRACE_DIRECT_BUTT
} oal_trace_direction_enum;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of oal_ext_if_rom.h */
