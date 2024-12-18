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
 * Description: diag mem
 * This file should be changed only infrequently and with great care.
 */

#ifndef __ZDIAG_MEM_H__
#define __ZDIAG_MEM_H__
#include "diag_common.h"
#include "diag_channel.h"
#include "diag_pkt.h"


errcode_t diag_pkt_need_cross_task(diag_pkt_handle_t *pkt);
void diag_pkt_free(diag_pkt_handle_t *pkt);

#endif
