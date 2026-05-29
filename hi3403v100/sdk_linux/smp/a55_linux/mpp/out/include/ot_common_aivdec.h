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

#ifndef OT_COMMON_AIVDEC_H
#define OT_COMMON_AIVDEC_H

#include "ot_common_aiv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    ot_aiv_model model;
} ot_aivdec_model;

typedef struct {
    ot_mpp_chn attach_vdec_chn;
} ot_aivdec_enable_cfg;

typedef struct {
    td_bool enable;             /* RW; Range:[0, 1]: switch AI enhance without channel disabled. */
    td_s32 ref_frame_duration;  /* RW; Range:[0, 100]: affect the update frequency of ref-frame. */
    td_s32 model_qp_offset;     /* RW; Range:[-10, 10]: affect the model QP selection. */
    td_s32 ref_mode_preference; /* RW; Range:[0, 10]: preference to choose ref-model or single-model. */
    td_s32 enhance_range;       /* RW; Range:[0, 10]: affect the size of enhancement area at input frame. */
    td_s32 debreath_level;      /* RW; Range:[0, 10]: reserved. affect the de-breath filter strength. */
} ot_aivdec_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
