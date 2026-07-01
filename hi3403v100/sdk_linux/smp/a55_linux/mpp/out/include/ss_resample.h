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

#ifndef __SS_RESAMPLE_H__
#define __SS_RESAMPLE_H__

#include "ot_resample.h"
#include "ot_vqe_register.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_void *ss_resample_create(td_s32 in_rate, td_s32 out_rate, td_s32 chn_num);

td_s32 ss_resample_process(td_void *instance, const td_s16 *in_buf, td_s32 in_samples, td_s16 *out_buf);

td_void ss_resample_destroy(td_void *instance);

td_s32 ss_resample_get_max_output_num(td_void *instance, td_s32 in_samples);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
