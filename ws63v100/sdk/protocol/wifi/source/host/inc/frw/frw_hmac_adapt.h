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
 * Description: Header file of dmac public interface.
 */

#ifndef __FRW_HMAC_ADAPT_H__
#define __FRW_HMAC_ADAPT_H__

#include "osal_types.h"
#include "oal_net.h"
#include "frw_msg_rom.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_DMAC_ADAPT_H

osal_u32 frw_hmac_send_data(oal_netbuf_stru *netbuf, osal_u8 vap_id, osal_u8 data_type);
osal_u32 frw_hmac_send_data_adapt(oal_netbuf_stru *netbuf, osal_u8 data_type);
#endif // __FRW_HMAC_ADAPT_H__

