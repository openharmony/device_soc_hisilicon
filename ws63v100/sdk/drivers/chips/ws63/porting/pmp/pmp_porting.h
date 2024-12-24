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
 * Description: Provides pmp port template \n
 *
 * History: \n
 * 2022-09-26， Create file. \n
 */
#ifndef PMP_PORTING_H
#define PMP_PORTING_H

#include <stdint.h>
#include "arch_encoding.h"
#include "arch_barrier.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_port_pmp PMP
 * @ingroup  drivers_port
 * @{
 */

/**
 * @if Eng
 * @brief  Register hal funcs objects into hal_pmp module.
 * @else
 * @brief  将hal funcs对象注册到hal_module模块中
 * @endif
 */
void pmp_port_register_hal_funcs(void);

/**
 * @if Eng
 * @brief  Unregister hal funcs objects from hal_pmp module.
 * @else
 * @brief  从hal_pmp模块注销hal funcs对象
 * @endif
 */
void pmp_port_unregister_hal_funcs(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif