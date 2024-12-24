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
 * Description: hcc driver implementatioin.
 * Author:
 * Create: 2020-10-20
 */

#ifndef __HCC_HOST_IF_H__
#define __HCC_HOST_IF_H__

#include "osal_types.h"

typedef enum _hcc_chan_type_ {
    HCC_TX = 0x0,
    HCC_RX = 0x1,
    HCC_CHAN_DIR_COUNT
} hcc_chan_type;

typedef enum _plat_reboot_notify_type_ {
    WLAN_REBOOT_NOTIFY,
    REBOOT_NOTIFY_COUNT
} plat_reboot_notify_type;

typedef osal_void (*reboot_notify)(osal_void);
osal_void plat_register_reboot(reboot_notify func, plat_reboot_notify_type type);
#endif /* end of oal_hcc_if.h */
