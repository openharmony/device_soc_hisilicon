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
 * Description: adapt PSD
 * This file should be changed only infrequently and with great care.
 */

#ifndef ZDIAG_ADAPT_PSD_H
#define ZDIAG_ADAPT_PSD_H

#include "diag.h"

errcode_t diag_cmd_psd_enable(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
typedef void (*psd_enable_callback)(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
void uapi_zdiag_psd_enable_unregister(void);
void uapi_zdiag_psd_enable_register(psd_enable_callback cb);
psd_enable_callback uapi_zdiag_psd_enable_get_cb(void);

#endif