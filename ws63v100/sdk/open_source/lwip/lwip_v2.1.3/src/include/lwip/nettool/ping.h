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
 * Description: shell cmds APIs implementation about ping and ping6
 * Author: none
 * Create: 2020
 */

#ifndef LWIP_PING_H
#define LWIP_PING_H
#include "lwip/opt.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined LWIP_SHELL_CMD_PING_RETRY_TIMES || defined __DOXYGEN__
#define LWIP_SHELL_CMD_PING_RETRY_TIMES     4
#endif

#if !defined LWIP_SHELL_CMD_PING_TIMEOUT || defined __DOXYGEN__
#define LWIP_SHELL_CMD_PING_TIMEOUT     2000
#endif

u32_t os_shell_ping(int argc, const char **argv);
u32_t os_shell_ping6(int argc, const char **argv);

#ifdef CONFIG_SIGMA_SUPPORT
typedef void (*ping_result_callback)(u32_t succ_cnt, u32_t failed_cnt);
void ping_add_ext_callback(ping_result_callback func);
#endif

#ifdef __cplusplus
}
#endif

#endif /* LWIP_PING_H */
