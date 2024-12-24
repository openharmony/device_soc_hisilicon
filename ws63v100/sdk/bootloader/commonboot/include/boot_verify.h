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
 * Description: Common Boot for Standard Application Core
 *
 * Create: 2023-01-09
 */

#ifndef BOOT_VERIFY_BOOT_H
#define BOOT_VERIFY_BOOT_H

#include <stdint.h>
#define STRUCT_VERSION                  0x10000

uint32_t sign_flashboot_verify(uint32_t head_addr);
uint32_t sign_loaderboot_verify(uint32_t head_addr);
void non_secure_boot(uint32_t addr);

#endif