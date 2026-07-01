/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 */
#ifndef OT_SYS_MOD_INIT_H
#define OT_SYS_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

td_void sys_set_reg_by_dts(td_bool is_reg_by_dts);

td_void sys_set_crg_base_addr(td_void *crg_base_addr);

td_void sys_set_sys_base_addr(td_void *sys_base_addr);

td_void sys_set_ddr0_base_addr(td_void *ddr0_base_addr);

td_void sys_set_misc_base_addr(td_void *misc_base_addr);

td_s32 sys_do_mod_init(td_void);

td_void sys_do_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of #ifndef OT_SYS_MOD_INIT_H */
