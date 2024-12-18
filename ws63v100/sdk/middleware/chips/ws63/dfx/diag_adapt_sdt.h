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
 * Description: adapt sdt
 * This file should be changed only infrequently and with great care.
 */

#ifndef ZDIAG_ADAPT_SDT_H
#define ZDIAG_ADAPT_SDT_H

#include "diag.h"
#include "log_oam_msg.h"
#include "log_oam_logger.h"

void zdiag_adapt_sdt_msg_proc(uint8_t *buf1, uint32_t len1, uint8_t *buf2, uint32_t len2);
errcode_t zdiag_adapt_sdt_msg_dispatch(uint32_t msg_id, uint8_t *data, uint32_t data_len);
void diag_register_debug_cmd_callback(oam_cmd_handle_callback func);
void diag_debug_cmd_proc(uint8_t *data, uint32_t len);

#ifndef FORBID_AUTO_LOG_REPORT
void diag_auto_log_report_enable(void);
#endif

#endif
