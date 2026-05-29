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

#ifndef VPSS_MOD_INIT_H
#define VPSS_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

td_s32 vpss_set_init_vpss_reg(td_void *vpss_reg, td_u32 ip_index);
td_s32 vpss_set_init_vpss_irq(td_s32 vpss_irq, td_u32 ip_index);

td_s32 vpss_module_init(td_void);
td_void vpss_module_exit(td_void);

td_s32 ot_vpss_mod_init(void);
td_void ot_vpss_mod_exit(void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef VPSS_MOD_INIT_H */
