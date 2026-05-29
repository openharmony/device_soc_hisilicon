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

#ifndef OT_VI_MOD_INIT_H
#define OT_VI_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_void vi_get_vicap_ip_info(td_u32 *irq_num, td_u32 *ip_num);
td_void vi_set_vicap_irq_num(td_u32 vicap_id, td_u32 irq_index, td_u32 irq_num);
td_void vi_get_viproc_ip_info(td_u32 *irq_num, td_u32 *ip_num);
td_void vi_set_viproc_irq_num(td_u32 viproc_id, td_u32 irq_index, td_u32 irq_num);
td_s32 vi_set_high_profile(td_u32 high_profile);
td_s32 vi_mod_init(td_void);
td_void vi_mod_exit(td_void);

td_s32 ot_vi_mod_init(td_u32 high_profile);
td_void ot_vi_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OT_VI_MOD_INIT_H */