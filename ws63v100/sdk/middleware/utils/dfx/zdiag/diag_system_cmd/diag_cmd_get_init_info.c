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
 * Description: diag get init info
 * This file should be changed only infrequently and with great care.
 */

#include "diag_cmd_get_init_info.h"
#include "dfx_adapt_layer.h"

#if defined(DIAG_SUPPORT_INIT_INFO)
#include "boot_msg.h"

errcode_t diag_cmd_get_init_info(uint16_t cmd_id, uint8_t *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    dfx_sys_boot_recode_t sys_boot_msg = {0};

    unused(cmd_param);
    unused(cmd_param_size);

    for (unsigned i = 0; i < DFX_BOOT_ITEM_MAX; i++) {
        memset_s(&sys_boot_msg, sizeof(dfx_sys_boot_recode_t), 0, sizeof(dfx_sys_boot_recode_t));

        sys_boot_msg.boot_item = (uint32_t)i;
        uapi_get_boot_msg_time((uint32_t)i, &sys_boot_msg);

        uapi_diag_report_packet(cmd_id, option, (uint8_t *)&sys_boot_msg,
            (uint16_t)sizeof(dfx_sys_boot_recode_t), true);
    }

    return ERRCODE_SUCC;
}
#endif