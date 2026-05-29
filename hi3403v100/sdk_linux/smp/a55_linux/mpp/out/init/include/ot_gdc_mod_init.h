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
#ifndef OT_GDC_MOD_INIT_H
#define OT_GDC_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

td_s32 gdc_set_init_gdc_irq(td_s32 gdc_irq, td_u32 ip_index);
td_void gdc_set_max_job_num(td_u32 num);
td_void gdc_set_max_task_num(td_u32 num);
td_void gdc_set_max_node_num(td_u32 num);

td_s32 gdc_module_init(td_void);
td_void gdc_module_exit(td_void);

int ot_gdc_mod_init(void);
void ot_gdc_mod_exit(void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_GDC_MOD_INIT_H */
