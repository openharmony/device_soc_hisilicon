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

#ifndef __OT_SAMPLE_FOV2LDC_H__
#define __OT_SAMPLE_FOV2LDC_H__

#include "ot_type.h"
#include "ot_common_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define FOV_PREC_BITS 20

typedef enum {
    OT_FOV_TYPE_DIAGONAL = 0,
    OT_FOV_TYPE_HOROZONTAL,
    OT_FOV_TYPE_VERTICAL,
    OT_FOV_TYPE_BUTT
} ot_fov_type;

typedef struct {
    td_u32 width;
    td_u32 height;
    ot_fov_type type; /* 0--diagonal,1--horizontal,2--vertical */
    td_u32      fov; /* decimal bits 20bit */
} ot_fov_attr;

td_s32 ot_sample_fov_to_ldcv2(const ot_fov_attr *fov_attr, ot_ldc_v2_attr *ldcv2_attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of __cplusplus */

#endif /* __OT_SAMPLE_FOV2LDC_H__ */

