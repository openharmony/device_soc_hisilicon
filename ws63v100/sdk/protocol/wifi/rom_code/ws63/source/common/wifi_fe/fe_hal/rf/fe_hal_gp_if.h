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
 * Description: 校准用到的gp接口
 */
#ifndef _FE_HAL_GP_IF_H_
#define _FE_HAL_GP_IF_H_

#include "osal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_u32 fe_hal_gp_get_ppa_lut_lock_state(osal_void);
osal_void fe_hal_gp_set_ppa_lut_lock_state(osal_u32 val);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif