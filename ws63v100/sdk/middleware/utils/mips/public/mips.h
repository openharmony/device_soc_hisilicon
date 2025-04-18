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
 * Description:  mips
 */

#ifndef MIPS_H
#define MIPS_H

#include <stdint.h>
#include <stdbool.h>
/* get host thread run status */
bool global_thread_status_get(void);

/* update host thread run status */
void global_thread_status_update(bool status);

/* controller isr run time update when btc int out */
void global_isr_time_statistics_update(uint64_t sys_time_start, uint64_t sys_time_end);

/* get controller isr run times */
uint64_t global_isr_time_statistics_get(void);

/**
 * @defgroup LIBS_MIPS MIPS Lib
 * @ingroup LIBS
 * @{
 */
/* start calculating mips */
void mips_compute_run_time_start(void);

/* stop calcualting mips */
void mips_compute_run_time_stop(void);

/* register mips callback function. */
void mips_init(void);

/* get bt thread status when compute mips */
bool mips_get_bt_thread_status(void);

/* set bt thread status when compute mips */
void mips_set_bt_thread_status(bool status);

/* get bt isr status when compute mips */
bool mips_get_bt_isr_status(void);

/* set bt isr status when compute mips */
void mips_set_bt_isr_status(bool status);

/* get platfrom isr run time while bt thread and bt isr running */
uint32_t mips_get_plt_isr_run_time(void);

/* clear plt isr run time data */
void mips_clear_plt_isr_run_time(void);

/**
 * @}
 */
#endif
