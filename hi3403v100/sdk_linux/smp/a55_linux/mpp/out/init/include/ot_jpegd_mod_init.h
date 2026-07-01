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

#ifndef OT_JPEGD_MOD_INIT_H
#define OT_JPEGD_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int jpegd_module_init(void);
void jpegd_module_exit(void);
td_void jpegd_set_reg_addr(td_void **addr, td_u32 len);
td_void jpegd_set_jpegd_irq(td_u32 *irq, td_u32 len);

td_s32 ot_jpegd_mod_init(td_void);
td_void ot_jpegd_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of #ifndef OT_JPEGD_MOD_INIT_H */
