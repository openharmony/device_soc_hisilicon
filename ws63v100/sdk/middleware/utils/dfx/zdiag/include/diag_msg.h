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
 * Description: zdiag msg
 * This file should be changed only infrequently and with great care.
 */

#ifndef __ZDIAG_MSG_H__
#define __ZDIAG_MSG_H__

#include "errcode.h"
#include "diag_common.h"
#include "diag_channel.h"

errcode_t diag_msg_proc(uint16_t msg_id, uint8_t *msg, uint32_t msg_len);

void uapi_zdiag_set_offline_log_enable(bool enable);
bool uapi_zdiag_offline_log_is_enable(void);

#endif
