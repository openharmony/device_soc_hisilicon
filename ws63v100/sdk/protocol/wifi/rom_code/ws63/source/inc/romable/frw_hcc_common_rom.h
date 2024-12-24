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
 * Description: Header file of device and host frw_hcc_common public interface.
 */

#ifndef __FRW_HCC_COMMON_ROM_H__
#define __FRW_HCC_COMMON_ROM_H__

#include "osal_types.h"
#ifndef _PRE_WLAN_FEATURE_CENTRALIZE
#include "hcc_cfg_comm.h"
#include "hcc_if.h"
#include "hcc_comm.h"
#endif

/*****************************************************************************
  1 宏定义
*****************************************************************************/

/*****************************************************************************
  2 枚举定义
*****************************************************************************/

#ifdef _PRE_WLAN_FEATURE_CENTRALIZE
#define HCC_HEAD_SIZE 4
static INLINE__ osal_u16 hcc_get_head_len(osal_void)
{
    return HCC_HEAD_SIZE;
}
#endif

#endif // __FRW_HCC_COMMON_ROM_H__
