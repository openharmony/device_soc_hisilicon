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

#ifndef OT_TDE_MOD_INIT_H
#define OT_TDE_MOD_INIT_H

#include "ot_type.h"
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/poll.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_void set_tde_irq(unsigned int temp_tde_irq);
td_s32 tde_drv_mod_init(td_void);
td_void tde_drv_mod_exit(td_void);
td_void tde_hal_set_base_vir_addr(td_u32 *temp_base_vir_addr);
td_void tde_set_max_node_num(td_u32 node_num);
td_void tde_set_resize_filter(td_bool is_resize_filter);
td_void tde_set_tde_tmp_buffer(td_u32 tde_tmp_buf);
td_void tde_init_set_rgb_truncation_mode(td_u32 rgb_truncation_mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OT_TDE_MOD_INIT_H */
