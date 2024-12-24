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
 * Description: diag get memory info
 * This file should be changed only infrequently and with great care.
 */

#include "diag_cmd_get_mem_info.h"
#include "dfx_mem.h"
#include "dfx_adapt_layer.h"
#include "errcode.h"

errcode_t diag_cmd_get_mem_info(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    errcode_t ret;
    mdm_mem_info_t info;

    unused(cmd_param);
    unused(cmd_param_size);

    ret = dfx_mem_get_sys_pool_info(&info);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    uapi_diag_report_packet(cmd_id, option, (uint8_t *)&info, (uint16_t)sizeof(mdm_mem_info_t), true);
    return ERRCODE_SUCC;
}