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
 * 2023-09-14， Create file. \n
 */

#ifndef SOC_PORTING_H
#define SOC_PORTING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CLK24M_TCXO 1
#define CLK40M_TCXO 0
uint32_t get_tcxo_freq(void);
void boot_clock_adapt(void);
void switch_flash_clock_to_pll(void);
void config_sfc_ctrl_ds(void);
bool is_tcxo_24mhz(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
