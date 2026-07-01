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
#ifndef OT_PQP_MOD_INIT_H
#define OT_PQP_MOD_INIT_H
#include "ot_type.h"

typedef struct {
    td_u8 pqp_high_profile_en;
} ot_pqp_mod_param;

td_s32 pqp_set_init_reg(td_u32 *pqp_reg[], td_u32 num);
td_s32 pqp_set_init_irq(const td_u32 pqp_irq[], td_u32 num);
td_s32 pqp_set_mod_param(const ot_pqp_mod_param *param);
td_s32 pqp_mod_init(td_void);
td_void pqp_mod_exit(td_void);

int ot_pqp_mod_init(void);
void ot_pqp_mod_exit(void);
#endif
