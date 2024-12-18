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
 * Description: shell cmds APIs implementation about ripple
 * Author: none
 * Create: 2020
 */

#ifndef LWIP_RIPPLE_SHELL_H
#define LWIP_RIPPLE_SHELL_H
#include "lwip/opt.h"

#ifdef __cplusplus
  extern "C" {
#endif

#if LWIP_RIPPLE
u32_t os_shell_rpl(int argc, const char **argv);
u32_t os_shell_l2test(int argc, const char **argv);
u32_t os_rte_debug(int argc, const char **argv);
#if LWIP_NAT64
u32_t os_shell_nat64_debug(int argc, const char **argv);
#endif
#if LWIP_IP6IN4
u32_t os_shell_ip6in4(int argc, const char **argv);
#endif
#endif /* LWIP_RIPPLE */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_RIPPLE_SHELL_H */
