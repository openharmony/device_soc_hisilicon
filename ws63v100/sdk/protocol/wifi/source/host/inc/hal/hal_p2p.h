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
 * Description: Header file of HAL external public interfaces .
 * Create: 2022-8-10
 */

#ifndef HAL_P2P_H
#define HAL_P2P_H

#include "osal_types.h"

#include "hal_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void hal_vap_set_noa(const hal_to_dmac_vap_stru *hal_vap, osal_u32 start_tsf, osal_u32 duration,
    osal_u32 interval, osal_u8 count);
osal_void hal_vap_set_noa_timeout_val(osal_u16 value);
osal_void hal_vap_set_noa_offset(osal_u16 offset);
osal_void hal_vap_set_ext_noa_disable(osal_void);
osal_void hal_vap_set_ext_noa_enable(osal_void);
osal_void hal_vap_set_ext_noa_para(hal_to_dmac_vap_stru *hal_vap, osal_u32 duration, osal_u32 interval);
osal_void hal_vap_set_ops(const hal_to_dmac_vap_stru *hal_vap, osal_u8 ops_ctrl, osal_u8 ct_window);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_p2p.h */