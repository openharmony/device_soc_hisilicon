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
 * Description: get init info function
 */
#ifndef DIAG_CMD_LOGFLASH_TEST_H
#define DIAG_CMD_LOGFLASH_TEST_H

#include "diag.h"

typedef struct {
    uint8_t multi_num;      // 文件的数量
    uint32_t file_size;     // 文件的大小
    uint8_t enable_cache;   // 是否支持缓存
    uint16_t cache_size;    // 缓存的大小
} dfx_test_logfile_cfg_t;

typedef struct {
    uint32_t ctrl_type;
    uint32_t service_type;
} dfx_test_logfile_ctrl_t;

typedef struct {
    uint32_t service_type;
    uint32_t data_type;
    uint32_t data_len;
    uint32_t test_timer;
} dfx_test_logfile_write_t;

errcode_t diag_logfile_init(void);
errcode_t diag_cmd_logflash_test(uint16_t cmd_id, uint8_t *cmd_param, uint16_t cmd_param_size, diag_option_t *option);

#endif /* DIAG_CMD_LOGFLASH_TEST_H */