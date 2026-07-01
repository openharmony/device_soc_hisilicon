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
#ifndef OT_IVE_MOD_INIT_H
#define OT_IVE_MOD_INIT_H
#include "ot_type.h"

typedef struct {
    td_u16 mod_node_num;
    td_u8 power_save_en;
} ot_ive_mod_param;

td_s32 ive_set_init_ive_reg(const td_s32 *ive_reg);

td_s32 ive_set_init_ive_irq(td_u32 ive_irq);

td_s32 ive_std_mod_init(const ot_ive_mod_param *mod_param);

td_void ive_std_mod_exit(td_void);

int ot_ive_mod_init(ot_ive_mod_param *mod_param);
void ot_ive_mod_exit(void);

#endif