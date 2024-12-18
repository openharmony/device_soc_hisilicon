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
 * Description: Provides RISCV hal pmp \n
 *
 * History: \n
 * 2022-09-26, Create file. \n
 */
#ifndef HAL_PMP_RISCV31_H
#define HAL_PMP_RISCV31_H

#include "hal_pmp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_pmp_riscv31 PMP RISCV31
 * @ingroup  drivers_hal_pmp
 * @{
 */

/**
 * @if Eng
 * @brief  Get interface between pmp driver and pmp hal.
 * @return pmp event handler structure
 * @else
 * @brief  获取Driver层pmp和HAL层pmp的接口
 * @return PMP 事件处理结构体地址
 * @endif
 */
hal_pmp_funcs_t *hal_pmp_riscv31_funcs_get(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif