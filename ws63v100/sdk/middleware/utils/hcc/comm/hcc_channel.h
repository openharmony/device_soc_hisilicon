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
 * Description: hcc channel.
 * Author:
 * Create: 2021-09-23
 */

#ifndef HCC_CHANNEL_HEADER
#define HCC_CHANNEL_HEADER

#include "td_type.h"
#include "hcc.h"
#include "hcc_cfg_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
td_s32 hcc_add_handler(hcc_handler *hcc);
td_void hcc_delete_handler(td_u8 chl);
hcc_handler *hcc_get_bus_handler(td_u8 bus_type);

td_bool hcc_chan_is_busy(td_u8 chl);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* HCC_CHANNEL_HEADER */