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

#ifndef HEVCPARSER_H
#define HEVCPARSER_H

#include "ffbitsreader.h"

#define FF_DELTAPOC_MAX_LEN 64

typedef struct {
    TD_U8 maxSubLayersMinus1;
    TD_BOOL temporalIdNestingFlag;
    TD_U8 generalProfileSpace;
    TD_U8 generalTierFlag;
    TD_U8 generalProfileIdc;
    TD_U32 picWidthInLumaSamples;
    TD_U32 picHeightInLumaSamples;
    TD_U32 generalProfileCompatibilityFlag;
    TD_U32 generalFlag0;  // generalflag 6 bytes
    TD_U16 generalFlag1;
    TD_U32 minSpatialSegmentationIdc;
    TD_U32 chromaFormatIdc;
    TD_U32 bitDepthLumaMinus8;
    TD_U32 bitDepthChromaMinus8;
} HevcSpsInfo;

TD_BOOL FfHevcParserParseSps(HevcSpsInfo *outSpsInfo, const TD_U8 *sps, TD_U32 spsLen);

#endif
