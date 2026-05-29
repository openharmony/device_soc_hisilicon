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

#ifndef OT_AO_EXPORT_H
#define OT_AO_EXPORT_H

#include "ot_common.h"

typedef struct {
    void (*ao_notify) (int ao_dev);
} ot_ao_export_callback;

typedef td_s32 ot_ao_register_export_callback(ot_ao_export_callback *export_callback);

typedef struct {
    ot_ao_register_export_callback *register_export_callback;
} ot_ao_export_symbol;

ot_ao_export_symbol *ot_ao_get_export_symbol(td_void);

#endif  /* OT_AO_EXPORT_H */

