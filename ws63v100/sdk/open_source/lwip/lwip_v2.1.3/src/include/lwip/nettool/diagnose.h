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
 * Description: debug and display shell cmd API implementation
 * Author: none
 * Create: 2020
 */

#ifndef LWIP_DIAGNOSE_H
#define LWIP_DIAGNOSE_H
#include "lwip/opt.h"

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef LWIP_DEBUG_INFO
u32_t os_shell_netdebug(int argc, const char **argv);
u32_t os_shell_ipdebug(int argc, const char **argv);
#endif /* LWIP_DEBUG_INFO */
u32_t os_shell_display_version(int argc, const char **argv);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_DIAGNOSE_H */
