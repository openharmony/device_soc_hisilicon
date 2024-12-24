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
 * Description: Header file
 */

#ifndef MSG_REKEY_OFFLOAD_ROM_H
#define MSG_REKEY_OFFLOAD_ROM_H
#include "osal_types.h"

#define MAC_REKEY_OFFLOAD_KCK_LEN              16
#define MAC_REKEY_OFFLOAD_KEK_LEN              16
#define MAC_REKEY_OFFLOAD_REPLAY_LEN           8

typedef struct rekey_offload {
    osal_u8   kck[MAC_REKEY_OFFLOAD_KCK_LEN];
    osal_u8   kek[MAC_REKEY_OFFLOAD_KEK_LEN];
    osal_u8   replay_ctr[MAC_REKEY_OFFLOAD_REPLAY_LEN];
} mac_rekey_offload_stru;
 
typedef struct {
    mac_rekey_offload_stru rekey_offload;
    osal_u8 rekey_info_exist;
} mac_rekey_offload_param_sync;
#endif
