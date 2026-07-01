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
#ifndef OT_MAU_MOD_INIT_H
#define OT_MAU_MOD_INIT_H
#include "ot_type.h"

typedef struct {
    td_u16 mau_max_mem_info_num;
    td_u16 mau_max_node_num;
    td_u8 mau_power_save_en;
    td_u8 reserved;
} ot_svp_mau_mod_param;

td_s32 svp_mau_set_init_reg(td_u32 *mau_reg[], td_u32 num);
td_s32 svp_mau_set_init_irq(const td_u32 mau_irq[], td_u32 num);
td_void svp_mau_set_mod_param(const ot_svp_mau_mod_param *param);
td_s32 svp_mau_mod_init(td_void);
td_void svp_mau_mod_exit(td_void);

int ot_dpu_rect_mod_init(void);
void ot_dpu_rect_mod_exit(void);
#endif
