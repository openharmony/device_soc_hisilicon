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

#ifndef BOOT_DEF_H
#define BOOT_DEF_H

#include "common_def.h"

#define SZ_1KB 1024
#ifndef CONFIG_LOADERBOOT_SUPPORT_DYNAMIC_PACKET_SIZE
#define READ_SIZE SZ_1KB
#else
#define READ_SIZE CONFIG_FLASH_WRITE_SIZE
#endif
#define SZ_1MB (SZ_1KB * SZ_1KB)
#define SZ_4KB 4096

#define BITS_PER_BYTE               8
#define HEXADECIMAL                 16
#define DECIMAL                     10

#endif