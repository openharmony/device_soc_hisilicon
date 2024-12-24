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
 * Description:  Default memory configurations
 *
 * Create: 2021-03-09
 */
#ifndef MEMORY_CONFIG_H
#define MEMORY_CONFIG_H

#include "memory_config_common.h"

/* APP RAM defines
 * APP has base 512K ITCM (Instruction TCM) for code
 *              256K DTCM (Data TCM) for DATA.
 * APP ITCM                 VECTORS TABLE
 *                          RAM TEXT
 *
 * APP DTCM                 STACK
 *                          RAM
 */
/* L2RAM 640K for APP core */
#define L2RAM_OFFSET         0x160000
#define L2RAM_FOR_APP_START  L2RAM_OFFSET
#define L2RAM_FOR_APP_LENGTH 0xA0000    /* 640K */

#endif
