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
 * Description: Header file of msg_ant_sel_rom
 */

#ifndef MSG_ANT_SEL_ROM_H
#define MSG_ANT_SEL_ROM_H

#include "osal_types.h"

typedef struct {
    osal_u8 cfg_type; /* 配置的场景类型：tx方向七种，rx方向一种 */
    osal_u8 value;    /* 配置的天线分集参数：0-3 */
    osal_u8 reserve[2];
} mac_cfg_ant_sel_param_stru;

#endif