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
 * Description: diag mem adapt
 * This file should be changed only infrequently and with great care.
 */

#ifndef DIAG_ADAPT_CMD_MEM_H
#define DIAG_ADAPT_CMD_MEM_H

#include "osal_types.h"

osal_s32 reg_rw_check_addr(osal_u32 start_addr, osal_u32 bytes_cnt);
#endif