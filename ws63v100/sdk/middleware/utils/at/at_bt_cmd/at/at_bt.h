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
 * limitations under the License. *
 * Description: At bt header \n
 */

#ifndef __AT_BT_H__
#define __AT_BT_H__

#include "td_base.h"
#include "at.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define EXT_RECONN_PERIOD_MIN 1

TD_CONST char *at_ssid_txt(TD_CONST td_u8 *ssid, td_u32 ssid_len);
td_void los_at_bt_cmd_register(td_void);
td_void los_at_sta_factory_test_cmd_register(td_void);
td_void los_at_mesh_cmd_register(td_void);

typedef enum {
    MPC_BT_SUCC = 0x00,
    MPC_BT_FAIL = 0x01,
} mpc_bt_err_code;

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of at_bt.h */
