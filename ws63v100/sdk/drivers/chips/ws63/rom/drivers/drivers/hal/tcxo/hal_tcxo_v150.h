/**
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
 *
 * Description: Provides V150 HAL tcxo \n
 *
 * History: \n
 * 2022-08-16， Create file. \n
 */
#ifndef HAL_TCXO_V150_H
#define HAL_TCXO_V150_H

#include "hal_tcxo.h"
#include "hal_tcxo_v150_regs_op.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_tcxo_v150 TCXO V150
 * @ingroup  drivers_hal_tcxo
 * @{
 */

/**
 * @if Eng
 * @brief  Get the instance of v150.
 * @return The instance of v150.
 * @else
 * @brief  获取v150实例。
 * @return v150实例。
 * @endif
 */
hal_tcxo_funcs_t *hal_tcxo_v150_funcs_get(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif