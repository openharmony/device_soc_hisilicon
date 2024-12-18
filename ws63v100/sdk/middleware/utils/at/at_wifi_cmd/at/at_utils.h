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
 * Description: AT internal interface.
 */

#ifndef __AT_H__
#define __AT_H__

#include <td_base.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_MFG_TEST
#define EXT_AT_MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#define ext_at_mac2str(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#else
#define EXT_AT_MACSTR "%02x:%02x:%02x:%02x:**:**"
#define ext_at_mac2str(a) (a)[0], (a)[1], (a)[2], (a)[3]
#endif

td_u32 at_cmd_process(TD_CONST td_u8 *at_cmd_line);
td_u32 at_param_null_check(td_s32 argc, TD_CONST td_char **argv);
td_u32 integer_check(TD_CONST td_char *val);
td_u32 alnum_check(TD_CONST td_char *val);
td_u32 cmd_strtoaddr(TD_CONST td_char *param, td_uchar *mac_addr, td_u32 addr_len);
char *at_parse_string(TD_CONST char *value, td_u32 *len);
int at_proc_task_body(td_void *param);

td_void at_help_cmd_register(td_void);
td_s32 convert_bin_to_dec(td_s32 pbin);
td_void at_str_to_hex(TD_CONST td_char *param, td_u32 len, unsigned char *value);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif
