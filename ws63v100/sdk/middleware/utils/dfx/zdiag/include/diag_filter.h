/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: diag filter
 * This file should be changed only infrequently and with great care.
 */
#ifndef DIAG_FILTER_H
#define DIAG_FILTER_H
#include <stdint.h>

#include "diag.h"
#include "diag_common.h"

typedef void (*zdiag_filter_notify_hook)(bool enable);

void zdiag_set_enable(bool enable, diag_addr addr);
bool zdiag_is_enable(void);
void zdiag_set_level_enable(uint8_t level, bool enable);
void zdiag_set_id_enable(uint32_t id, bool enable);

bool zdiag_log_enable(uint8_t level, uint32_t id);
void zdiag_filter_register_notify_hook(zdiag_filter_notify_hook hook);
void zdiag_filter_init(void);

void zdiag_state_beat_heart_pkt_recv(diag_addr peer_addr);
uint32_t zdiag_state_get_last_recv_time(diag_addr peer_addr);
diag_addr zdiag_get_connect_tool_addr(void);
void zdiag_level_proc(uint8_t level);
void diag_highest_level_proc(uint8_t level);
#endif
