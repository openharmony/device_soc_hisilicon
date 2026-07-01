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
#ifndef OT_COMMON_FISHEYE_CALIBRATION_H
#define OT_COMMON_FISHEYE_CALIBRATION_H

#include "ot_common_video.h"
#include "ot_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define OT_ERR_FISHEYE_CALIBRATION_ILLEGAL_PARAM  \
        OT_DEFINE_ERR(OT_ID_FISHEYE_CALIBRATION, OT_ERR_LEVEL_ERROR, OT_ERR_ILLEGAL_PARAM)
#define OT_ERR_FISHEYE_CALIBRATION_NULL_PTR       \
        OT_DEFINE_ERR(OT_ID_FISHEYE_CALIBRATION, OT_ERR_LEVEL_ERROR, OT_ERR_NULL_PTR)
#define OT_ERR_FISHEYE_CALIBRATION_NOT_SUPPORT    \
        OT_DEFINE_ERR(OT_ID_FISHEYE_CALIBRATION, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_SUPPORT)
#define OT_ERR_FISHEYE_CALIBRATION_NOMEM          \
        OT_DEFINE_ERR(OT_ID_FISHEYE_CALIBRATION, OT_ERR_LEVEL_ERROR, OT_ERR_NO_MEM)
#define OT_ERR_FISHEYE_CALIBRATION_BUSY           \
        OT_DEFINE_ERR(OT_ID_FISHEYE_CALIBRATION, OT_ERR_LEVEL_ERROR, OT_ERR_BUSY)

typedef enum {
    OT_FISHEYE_CALIBRATION_LEVEL_0 = 0, /* calibration level 0 */
    OT_FISHEYE_CALIBRATION_LEVEL_1,     /* calibration level 1 */
    OT_FISHEYE_CALIBRATION_LEVEL_2,     /* calibration level 2 */

    OT_FISHEYE_CALIBRATION_LEVEL_BUTT
} ot_fisheye_calibration_level;

typedef struct {
    td_s32 offset_v; /* the horizontal offset between image center and physical center of len */
    td_s32 offset_h; /* the vertical offset between image center and physical center of len */
    td_s32 radius_x; /* the X coordinate of physical center of len */
    td_s32 radius_y; /* the Y coordinate of physical center of len */
    td_u32 radius;   /* the radius of len */
} ot_fisheye_calibration_result;

typedef struct {
    ot_fisheye_calibration_result calibration_result;   /* the output of result */
} ot_fisheye_calibration_output;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OT_COMMON_FISHEYE_CALIBRATION_H */

