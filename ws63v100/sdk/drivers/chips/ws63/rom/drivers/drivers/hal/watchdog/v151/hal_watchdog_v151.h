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
 * Description: Provides V151 HAL watchdog \n
 *
 * History: \n
 * 2022-11-26, Create file. \n
 */
#ifndef HAL_WATCHDOG_V151_H
#define HAL_WATCHDOG_V151_H

#include "hal_watchdog.h"
#include "hal_watchdog_v151_regs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_watchdog_v151 Watchdog V151
 * @ingroup  drivers_hal_watchdog
 * @{
 */

/**
 * @if Eng
 * @brief  Get functions of the watchdog v151.
 * @else
 * @brief  获取watchdog v151 的实例
 * @endif
 */
hal_watchdog_funcs_t *hal_watchdog_v151_funcs_get(void);

/**
 * @if Eng
 * @brief  Handler of the watchdog interrput request.
 * @param  [in]  param request param
 * @else
 * @brief  WATCHDOG中断处理函数
 * @param  [in]  param 中断请求的参数
 * @endif
 */
void hal_watchdog_v151_irq_handler(uintptr_t param);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
