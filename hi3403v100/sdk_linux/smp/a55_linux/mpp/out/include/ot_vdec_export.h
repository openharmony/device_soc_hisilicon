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

#ifndef OT_VDEC_EXPORT_H
#define OT_VDEC_EXPORT_H

#include "ot_common.h"

typedef struct {
    void (*vdec_notify)(int mod_id, int chn, int event_type, void *args);
    int (*vdec_set_scheduler_dec)(int *sched_period);
    int (*vdec_set_scheduler_stm)(int *sched_period);
    int (*vdec_set_scheduler_syn)(int *sched_period);
} ot_vdec_export_callback;

typedef td_s32 ot_vdec_register_export_callback(const ot_vdec_export_callback *export_callback);

typedef struct {
    ot_vdec_register_export_callback *register_export_callback;
} ot_vdec_export_symbol;

ot_vdec_export_symbol *ot_vdec_get_export_symbol(td_void);

#endif /* OT_VDEC_EXPORT_H */

