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
 * Description:  OAM TRACE for Suzhe BT Core
 *
 * Create: 2021-10-25
 */

#ifndef OAM_TRACE_H
#define OAM_TRACE_H

#include <stdint.h>
#ifdef USE_CMSIS_OS
#include "arch/exception.h"
#endif

#include "stdbool.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

bool is_valid_txt_addr(uint32_t pc);

/**
 * @brief  Rigister Exception handle function.
 */
void register_os_exec_hook(void);

#ifdef USE_CMSIS_OS
/**
 * @brief  Invoked by exception to dump exception information.
 * @param  exc_type Exception type.
 * @param  exc_buff_addr Exception buff addr.
 */
void exec_fault_handler(uint32_t exc_type, const ExcContext *exc_buff_addr);
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif