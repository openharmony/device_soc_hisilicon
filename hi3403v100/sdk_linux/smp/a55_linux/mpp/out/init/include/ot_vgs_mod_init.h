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

#ifndef VGS_MOD_INIT_H
#define VGS_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

typedef struct {
    td_u32 max_vgs_job;
    td_u32 max_vgs_task;
    td_u32 max_vgs_node;
} ot_vgs_module_params;
td_s32 vgs_set_init_vgs_reg(td_void *vgs_reg, td_u32 ip_index);
td_s32 vgs_set_init_vgs_irq(td_s32 vgs_irq, td_u32 ip_index);

td_void vgs_set_max_job_num(td_u32 num);
td_void vgs_set_max_task_num(td_u32 num);
td_void vgs_set_max_node_num(td_u32 num);

td_s32 vgs_module_init(td_void);
td_void vgs_module_exit(td_void);

td_s32 ot_vgs_mod_init(ot_vgs_module_params *mod_params);
td_void ot_vgs_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef VGS_MOD_INIT_H */
