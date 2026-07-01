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

#ifndef OT_VO_EXPORT_H
#define OT_VO_EXPORT_H

#include "ot_common.h"

typedef struct {
    void (*vo_notify)(int mod_id, int dev);
    void (*vo_user_notify)(int mod_id, int dev);
} ot_vo_export_callback;

typedef td_s32 ot_vo_register_export_callback(ot_vo_export_callback *export_callback);

typedef struct {
    ot_vo_register_export_callback *vo_register_export_callback;
} ot_vo_export_symbol;

ot_vo_export_symbol *ot_vo_get_export_symbol(td_void);
td_void ot_vo_enable_dev_export(ot_vo_dev dev);
td_void ot_vo_disable_dev_export(ot_vo_dev dev);

#endif /* OT_VO_EXPORT_H */
