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
 * Description:  calculate mips
 *
 * Create:  2018-10-15
 */
#ifndef HAL_MIPS_H
#define HAL_MIPS_H

/** @defgroup connectivity_drivers_hal_mips MIPS
  * @ingroup  connectivity_drivers_hal
  * @{
  */
typedef void (*hal_exception_dump_mips_callback)(void);

/* register mips callback function */
void hal_register_mips_start_callback(hal_exception_dump_mips_callback callback);
void hal_register_mips_stop_callback(hal_exception_dump_mips_callback callback);

/* start calculating ticks  */
void hal_calculate_mips_start(void);

/* start calculating ticks  */
void hal_calculate_mips_stop(void);

/**
  * @}
  */
#endif
