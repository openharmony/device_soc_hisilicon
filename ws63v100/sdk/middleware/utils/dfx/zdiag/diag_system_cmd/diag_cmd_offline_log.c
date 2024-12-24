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
 * Description: diag offline log enable
 */

#include "diag_cmd_offline_log.h"
#include "dfx_adapt_layer.h"

#if defined(CONFIG_DFX_SUPPORT_OFFLINE_LOG_FILE) && defined(DFX_YES) && \
    (CONFIG_DFX_SUPPORT_OFFLINE_LOG_FILE == DFX_YES)
#include "diag_msg.h"
#include "log_file.h"

errcode_t diag_cmd_set_offline_log(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);

    bool offline_enabled = false;
    uint32_t enable = *(uint32_t *)cmd_param;

    if (enable == (uint32_t)-1) {
        offline_enabled = uapi_zdiag_offline_log_is_enable();
    } else if (enable == 0 || enable == 1) {
        offline_enabled = (enable == 0) ? false : true;
        uapi_zdiag_set_offline_log_enable(offline_enabled);
        if (offline_enabled != uapi_zdiag_offline_log_is_enable()) {
            uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Set Offline log failed\r\n"),
                                    strlen("Set Offline log failed\r\n") + 1, true);
            return ERRCODE_SUCC;
        }
    } else if (enable == 2) { /* 2 means logfile suspend */
        (void)uapi_logfile_suspend(STORE_DIAG);
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Offline log suspend\r\n"),
                                strlen("Offline log suspend\r\n") + 1, true);
        return ERRCODE_SUCC;
    } else if (enable == 3) { /* 3 means logfile resume */
        (void)uapi_logfile_resume(STORE_DIAG);
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Offline log resume\r\n"),
                                strlen("Offline log resume\r\n") + 1, true);
        return ERRCODE_SUCC;
    } else {
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Invaild Parameter\r\n"),
                                strlen("Invaild Parameter\r\n") + 1, true);
        return ERRCODE_SUCC;
    }

    if (offline_enabled) {
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Offline log Enabled\r\n"),
                                strlen("Offline log Enabled\r\n") + 1, true);
    } else {
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Offline log Disabled\r\n"),
                                strlen("Offline log Disabled\r\n") + 1, true);
    }

    return ERRCODE_SUCC;
}
#endif /* CONFIG_DFX_SUPPORT_OFFLINE_LOG_FILE */
