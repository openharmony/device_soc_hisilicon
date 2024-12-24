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
 * Description: hmac_ant_sel head
 * Create: 2023-3-6
 */

#ifndef HMAC_ANT_SEL_H
#define HMAC_ANT_SEL_H

#include "osal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    HMAC_ANT_SEL_CONTROL_FROM_PHY = 0,
    HMAC_ANT_SEL_CONTROL_FROM_MAC,
    HMAC_ANT_SEL_CONTROL_FROM_BUTT
} hmac_phy_ant_sel_param_enum;

static osal_u32 hmac_ant_sel_init_weakref(osal_void) __attribute__ ((weakref("hmac_ant_sel_init"), used));
static osal_void hmac_ant_sel_deinit_weakref(osal_void) __attribute__ ((weakref("hmac_ant_sel_deinit"), used));

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_auto_adjust_freq_rom.h */
