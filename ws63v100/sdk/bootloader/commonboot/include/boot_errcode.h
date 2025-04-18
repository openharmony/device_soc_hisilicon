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

#ifndef BOOT_ERRCODE_H
#define BOOT_ERRCODE_H

#include "errcode.h"

/* Generic errno */
#define ERRCODE_BOOT_YMODEM_TIMEOUT                         0xC35A69A6
#define ERRCODE_BOOT_YMODEM_CANCEL                          0xC35A69A7
#define ERRCODE_BOOT_YMODEM_FRAME                           0xC35A69A8
#define ERRCODE_BOOT_YMODEM_CS                              0xC35A69A9
#define ERRCODE_BOOT_YMODEM_EOT                             0xC35A69AA
#define ERRCODE_BOOT_YMODEM_SEQ                             0xC35A69AB
#define ERRCODE_BOOT_YMODEM_LENTH                           0xC35A69AC

/* secure verify 0x800015C0~0x800015CF */
#define ERRCODE_BOOT_VERIFY_CHECK_ENABLE                    0x800015C0
#define ERRCODE_BOOT_VERIFY_INVALID_ROOT_KEY                0x800015C1
#define ERRCODE_BOOT_VERIFY_INVALID_IMAGE_ID                0x800015C2
#define ERRCODE_BOOT_VERIFY_PKE_VERIFY                      0x800015C3
#define ERRCODE_BOOT_VERIFY_INVALID_VERSION                 0x800015C4
#define ERRCODE_BOOT_VERIFY_INVALID_MSID                    0x800015C5
#define ERRCODE_BOOT_VERIFY_INVALID_DIE_ID                  0x800015C6
#define ERRCODE_BOOT_VERIFY_HASH_CALCULATION                0x800015C7
#define ERRCODE_BOOT_VERIFY_INVALID_HASH_RESULT             0x800015C8
#define ERRCODE_BOOT_VERIFY_EFUSE_READ                      0x800015C9
#define ERRCODE_BOOT_VERIFY_PARAM_NULL                      0x800015CA
#define ERRCODE_BOOT_VERIFY_HASH_DATA_IS_TOO_LONG           0x800015CB
#define ERRCODE_BOOT_VERIFY_MEMCPY_ERR                      0x800015CC

#endif