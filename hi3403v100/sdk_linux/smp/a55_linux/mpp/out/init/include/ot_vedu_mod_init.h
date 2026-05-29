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

#ifndef OT_VEDU_MOD_INIT_H
#define OT_VEDU_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int vpu_mod_init(void);
void vpu_mod_exit(void);
td_s32 vedu_set_irq(td_s32 vpu_id, td_s32 irq);
td_s32 jpeg_set_irq(td_s32 vpu_id, td_s32 irq);
td_s32 vedu_set_high_profile(td_bool high_profile);
td_s32 vedu_check_apll_clk(td_void);
td_void vedu_set_vedu_enable(td_u32 *vedu_en, td_u32 len);
td_void vedu_get_vedu_enable(td_u32 *vedu_en, td_u32 len);
td_void vedu_set_vedu_addr(td_s32 vpu_id, td_void *addr);
td_void jpge_set_jpge_addr(td_s32 vpu_id, td_void *addr);
td_u64 vedu_get_hal_addr(td_s32 vpu_id);

td_s32 ot_vedu_mod_init(td_void);
td_void ot_vedu_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_VEDU_MOD_INIT_H */
