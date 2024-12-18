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
 * Description:  LOG oam reg query MODULE
 */

#ifndef LOG_OAM_MEM_QUERY_H
#define LOG_OAM_MEM_QUERY_H
#include "product.h"
#if SYS_DEBUG_MODE_ENABLE == YES

#ifndef _LIB_LOG_READER_LOG_OAM_MEM_QUERY_H
#define _LIB_LOG_READER_LOG_OAM_MEM_QUERY_H

#include "log_oam_logger.h"
#ifdef SUPPORT_IPC
#include "ipc_actions.h"
#endif

#if USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG == NO
typedef struct {
    om_msg_header_stru_t header;
    uint32_t mem_start_addr;
    uint16_t data_len;
    uint8_t mode;
    uint8_t reserve;
} __attribute__((packed)) om_mem_req_header_t;
#endif

enum MEM_QUERY_ACTION {
    MEM_ACTION_WRITE,
    MEM_ACTION_READ,
    MEM_ACTION_WRITE_BLOCK,
    MEM_ACTION_READ_BLOCK,
};

void oml_mem_command(uint8_t *data, uint16_t length);

void oml_btc_command(uint8_t *data, uint16_t length);

void oml_mem_register_callback(void);

void oml_btc_cmd_callback(void);

#endif /* end of log_oam_mem_query.h */

#endif  /* end of SYS_DEBUG_MODE_ENABLE == YES */
#endif