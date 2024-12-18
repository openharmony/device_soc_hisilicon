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
 * Description: dfx mem
 * This file should be changed only infrequently and with great care.
 */

#ifndef DFX_MEM_H
#define DFX_MEM_H

#include "errcode.h"
#include "dfx_os_st.h"

errcode_t dfx_mem_get_sys_pool_info(mdm_mem_info_t *info);
#endif