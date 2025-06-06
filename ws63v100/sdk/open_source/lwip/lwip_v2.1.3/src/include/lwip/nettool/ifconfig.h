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
 * Description: ifconfig shell cmd API implementation
 * Author: none
 * Create: 2020
 */

#ifndef LWIP_IFCONFIG_H
#define LWIP_IFCONFIG_H
#include "lwip/opt.h"

#ifdef __cplusplus
extern "C" {
#endif

u32_t lwip_ifconfig(int argc, const char **argv);

u32_t os_shell_netif(int argc, const char **argv);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IFCONFIG_H */
