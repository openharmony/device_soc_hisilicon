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
 * Description: Header file for hal_p2p_rom.c.
 */

#ifndef HAL_P2P_ROM_H
#define HAL_P2P_ROM_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "osal_types.h"
#include "hal_device_rom.h"

osal_void hh503_irq_p2p_ct_window_end_isr(hal_device_stru *hal_device, osal_u8 data);
osal_void hal_vap_set_noa(const hal_to_dmac_vap_stru *hal_vap, osal_u32 start_tsf, osal_u32 duration,
    osal_u32 interval, osal_u8 count);
osal_u32 hal_get_p2p_noa_oper(osal_void);

/* 钩子函数声明 */
typedef osal_u32 (*hal_p2p_ct_window_end)(hal_device_stru *hal_device, osal_u8 data);
typedef osal_u32 (*hal_p2p_set_noa)(const hal_to_dmac_vap_stru *hal_vap, osal_u32 start_tsf, osal_u32 duration,
    osal_u32 interval, osal_u8 count);
#endif /* end of HAL_P2P_ROM_H */
