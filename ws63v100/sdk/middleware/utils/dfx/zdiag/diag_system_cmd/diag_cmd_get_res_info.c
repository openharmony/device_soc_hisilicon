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
 * Description: diag get os res info
 * This file should be changed only infrequently and with great care.
 */

#include "diag_cmd_get_res_info.h"
#include "dfx_adapt_layer.h"
#include "dfx_os_st.h"
#include "dfx_res.h"

errcode_t diag_cmd_get_res_info(uint16_t cmd_id, uint8_t *cmd_param, uint16_t cmd_param_size,
                                diag_option_t *option)
{
    osal_os_resource_use_stat_t os_resource = { 0 };
    unused(cmd_param);
    unused(cmd_param_size);

    dfx_os_get_resource_status(&os_resource);
    return uapi_diag_report_packet(cmd_id, option, (const uint8_t *)&os_resource,
        sizeof(osal_os_resource_use_stat_t), true);
}