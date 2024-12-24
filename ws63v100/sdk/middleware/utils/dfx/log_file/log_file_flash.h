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
 * Description: offline log file saved to the storage
 */

#ifndef LOG_FILE_FLASH_H
#define LOG_FILE_FLASH_H

#include "errcode.h"

#define FLASH_SECTOR_SIZE           4096
#define MAX_INDEX_NUM               65535
#define MAX_UNCOVER_FLAG_INTERVAL   5000

errcode_t logfile_write_cache_to_flash(store_file_info_t *file_info);

errcode_t logfile_flash_prepare(store_file_info_t *file_info);

errcode_t logfile_flash_erase(store_service_t service_type, const store_file_cfg_t *cfg);

#endif /* LOG_FILE_FLASH_H */
