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

#ifndef OT_VO_MOD_INIT_H
#define OT_VO_MOD_INIT_H

#include "ot_defines.h"
#include "ot_type.h"
#include "ot_osal.h"
#include "ot_vo_export.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

td_void vo_set_init_vo_reg(td_void *reg);
td_void vo_set_init_vo_irq(td_u32 vo_irq);
td_s32 vo_mod_init(td_void);
td_void vo_mod_exit(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_VO_MOD_INIT_H */
