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

#ifndef OT_VFMW_MOD_INIT_H
#define OT_VFMW_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int vfmw_module_init(void);
void vfmw_module_exit(void);

td_void pdt_set_scd_int(td_s32 scd_int);
td_void pdt_set_pxp_int(td_s32 vdm_pxp_int);
td_void pdt_set_mdma_int(td_s32 mdma_int);
td_void pdt_set_vdm_reg_addr(td_void *vdm_reg_addr);
td_void vfmw_set_max_chn_num(td_s32 chn_num);

td_s32 ot_vfmw_mod_init(td_s32 chn_num);
td_void ot_vfmw_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif