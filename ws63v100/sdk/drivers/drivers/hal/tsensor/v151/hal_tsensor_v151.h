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
 * Description: Provides v151 hal tsensor \n
 *
 * History: \n
 * 2023-02-28， Create file. \n
 */
#ifndef HAL_TSENSOR_V151_H
#define HAL_TSENSOR_V151_H

#include "hal_tsensor.h"
#include "hal_tsensor_v151_regs_op.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_tsensor_v151 TSENSOR V151
 * @ingroup  drivers_hal_tsensor
 * @{
 */

/**
 * @if Eng
 * @brief  Get the instance of v151.
 * @return The instance of v151.
 * @else
 * @brief  获取v151实例。
 * @return v151实例。
 * @endif
 */
hal_tsensor_funcs_t *hal_tsensor_v151_funcs_get(void);

/**
 * @if Eng
 * @brief  Handler of the timer interrupt request.
 * @else
 * @brief  Tsensor中断处理函数。
 * @endif
 */
void hal_tsensor_irq_handler(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
