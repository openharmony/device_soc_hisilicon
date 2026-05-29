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
#ifndef OT_DPU_MATCH_MOD_INIT_H
#define OT_DPU_MATCH_MOD_INIT_H
#include "ot_type.h"

td_s32 dpu_match_set_init_irq(td_u32 dpu_match_irq);

td_s32 dpu_match_init(td_void);
td_void dpu_match_exit(td_void);
int ot_dpu_match_mod_init(void);
void ot_dpu_match_mod_exit(void);
#endif
