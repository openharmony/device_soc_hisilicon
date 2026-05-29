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
#ifndef OT_RC_MOD_INIT_H
#define OT_RC_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int rc_module_init(void);
void rc_module_exit(void);

td_s32 ot_rc_mod_init(td_void);
td_void ot_rc_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_RC_MOD_INIT_H */
