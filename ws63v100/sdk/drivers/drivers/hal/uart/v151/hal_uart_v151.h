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
 * Description: Provides v151 hal uart \n
 *
 * History: \n
 * 2023-02-17, Create file. \n
 */
#ifndef HAL_UART_V151_H
#define HAL_UART_V151_H

#include "hal_uart.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_uart_v151 UART V151
 * @ingroup  drivers_hal_uart
 * @{
 */

hal_uart_funcs_t *hal_uart_v151_funcs_get(void);

/**
 * @if Eng
 * @brief  Handler of the uart interrupt request.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 * @else
 * @brief  UART中断处理函数
 * @param  [in]  bus 串口号， 参考 @ref uart_bus_t
 * @endif
 */
void hal_uart_irq_handler(uart_bus_t bus);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif