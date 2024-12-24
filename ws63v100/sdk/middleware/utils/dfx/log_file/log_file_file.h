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

#ifndef LOG_FILE_FILE_H
#define LOG_FILE_FILE_H

#include <stdint.h>
#include "errcode.h"

errcode_t logfile_create_path(store_file_cfg_t *cfg);
errcode_t logfile_prepare_file_fd(store_file_info_t *file_info, store_file_cfg_t *cfg);
errcode_t logfile_remove_files(store_service_t service_type, store_file_cfg_t *cfg);

errcode_t logfile_single_file_write(store_file_info_t *file_info, uint8_t *data, uint32_t data_len);
errcode_t logfile_multi_file_write(store_file_info_t *file_info, uint8_t *data, uint32_t data_len);

errcode_t logfile_write_cache_to_file(store_file_info_t *file_info);

#endif /* LOG_FILE_FILE_H */
