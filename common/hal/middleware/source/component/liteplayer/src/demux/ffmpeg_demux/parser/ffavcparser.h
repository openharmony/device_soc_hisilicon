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

#ifndef AVCPARSER_H
#define AVCPARSER_H

#include "ot_mw_type.h"

typedef struct AVCSpsInfo {
    TD_U32 chromaFormatIdc;           /* chroma_format_idc */
    TD_U32 picWidthInMbsMinus1;       /* pic_width_in_mbs_minus1 */
    TD_U32 picHeightInMapUnitsMinus1; /* pic_height_in_map_units_minus1 */
    TD_BOOL frameMbsOnlyFlag;         /* frame_mbs_only_flag */
    TD_BOOL adaptFrameFlieFlag;       /* mb_adaptive_frame_field_flag : 0 */
    TD_BOOL directInferenceFlag;      /* direct_8x8_inference_flag : 1 */
    TD_BOOL fameCroppingFlag;         /* frame_cropping_flag : 1 */
    TD_U32 frameCropLeftOffset;       /* frame_crop_left_offset : 0 */
    TD_U32 frameCropRightOffset;      /* frame_crop_right_offset : 0 */
    TD_U32 frameCropTopOffset;        /* frame_crop_top_offset : 0 */
    TD_U32 frameCropBottomOffset;     /* frame_crop_bottom_offset : 4 */
} AvcSpsInfo;

TD_BOOL FfAvcParserParseSps(AvcSpsInfo *outSpsInfo, const TD_U8 *sps, TD_U32 spsSize);
#endif
