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
 * Description: pkt router
 * This file should be changed only infrequently and with great care.
 */

#ifndef __ZDIAG_PKT_ROUTER_H__
#define __ZDIAG_PKT_ROUTER_H__

#include "errcode.h"
#include "diag_pkt.h"
#include "diag_channel.h"

typedef struct {
    diag_pkt_proc_t cur_proc;
} diag_pkt_process_param_t;

typedef struct {
    uint8_t cur_proc; /* diag_pkt_process_param_t */
    diag_pkt_handle_t pkt;
} diag_pkt_msg_t;

errcode_t diag_pkt_router(diag_pkt_handle_t *pkt, diag_pkt_process_param_t *process_param);
void diag_pkt_msg_proc(uint32_t msg_id, uint8_t *msg, uint32_t msg_len);
#endif
