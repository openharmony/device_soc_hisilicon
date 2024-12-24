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
 * Description: UPG debug functions header file
 */

#ifndef UPG_DEBUG_H
#define UPG_DEBUG_H

#include "upg_config.h"
#include "upg_definitions.h"
#include "stdio.h"
#include "debug_print.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (UPG_CFG_DEBUG_PRINT_ENABLED == YES)
#define upg_log_dbg(fmt, arg...)
#define upg_log_info(fmt, arg...) print_str(fmt, ##arg)
#define upg_log_err(fmt, arg...)  print_str(fmt, ##arg)
#else
#define upg_log_dbg(fmt, arg...)
#define upg_log_info(fmt, arg...)
#define upg_log_err(fmt, arg...)
#endif /* UPG_CFG_DEBUG_PRINT_ENABLED */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* UPG_DEBUG_H */
