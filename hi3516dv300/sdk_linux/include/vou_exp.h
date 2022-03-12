/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#include "hi_common.h"

#ifndef __HI_VOU_EXP_H__
#define __HI_VOU_EXP_H__

typedef struct {
    void (*pfnVoNotify)(int module_id, int vodev);
} vou_export_callback_s;

typedef HI_S32 fn_vo_register_extern_callback(vou_export_callback_s *pstExpCallback);

typedef struct {
    fn_vo_register_extern_callback *pfn_vo_register_exp_callback;
} vou_export_symbol_s;

#endif /* __HI_VOU_EXP_H__ */
