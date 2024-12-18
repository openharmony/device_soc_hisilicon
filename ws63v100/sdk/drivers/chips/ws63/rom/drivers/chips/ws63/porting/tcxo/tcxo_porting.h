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
 * Description: Provides tcxo port \n
 *
 * History: \n
 * 2022-08-16， Create file. \n
 */

#ifndef TCXO_PORTING_H
#define TCXO_PORTING_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HAL_TCXO_TICKS_PER_SECOND 24000000
#define HAL_TCXO_TICKS_PER_M_SECOND 24000
#define HAL_TCXO_TICKS_PER_U_SECOND 24
#define HAL_TCXO_COUNT_VALUE_B_VALID_TRUE 1

#define TCXO_TICKS_PER_M_SECOND 24000
#define TCXO_TICKS_PER_U_SECOND 24

#define TCXO_LOCK_GET_ATTE 0xFFFFFFFF

/**
 * @brief  Get the base address of a specified TCXO.
 * @return The base address of specified TCXO.
 */
uintptr_t tcxo_porting_base_addr_get(void);

/**
 * @brief  Register hal funcs objects into hal_tcxo module.
 */
void tcxo_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_tcxo module.
 */
void tcxo_port_unregister_hal_funcs(void);

/**
 * @brief  get tcxo ticks per microseconds.
 */
uint32_t tcxo_porting_ticks_per_usec_get(void);

/**
 * @brief  set tcxo ticks per microseconds.
 */
void tcxo_porting_ticks_per_usec_set(uint32_t ticks);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
