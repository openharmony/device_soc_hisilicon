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
 * Description: Common Boot for flash erase
 *
 * Create: 2023-01-09
 */

#ifndef BOOT_ERASE_H
#define BOOT_ERASE_H

#define FLASH_CHIP_ERASE_SIZE 0xffffffff
#define FLASH_CHIP_ERASE_FILE_LEN 0

bool loader_download_is_flash_all_erased(void);
void loader_download_set_flash_all_erase(bool all_erased);
uint32_t loader_erase_all_process(const uart_ctx *cmd_ctx, bool *write_flash);
#endif