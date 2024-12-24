/**
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
 * limitations under the License. \n
 *
 * Description: Provides at process service api header. Only for AT module. \n
 */

#ifndef    AT_PROCESS_H
#define    AT_PROCESS_H

#include "at_product.h"

at_ret_t at_proc_cmd_handle(uint16_t channel_id);

uint16_t at_proc_get_current_channel_id(void);

#ifdef CONFIG_AT_SUPPORT_NOTIFY_REPORT
void at_proc_cmd_urc_handle(void);
#endif
 
#ifdef CONFIG_AT_SUPPORT_ASYNCHRONOUS
at_ret_t at_proc_interactivity_handle(uint16_t channel_id);
 
at_ret_t at_proc_timeout_handle(void);
 
at_ret_t at_proc_cmd_result_handle(uint16_t result);
#endif

#endif
