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

#ifndef H26X_PARSER_H
#define H26X_PARSER_H

#include "ot_mw_type.h"
#include "libavformat/avformat.h"

TD_S32 H26xGetResolution(enum AVCodecID vidCodecId, const TD_U8 *data, TD_U32 dataLen,
    TD_U32 *width, TD_U32 *height);
TD_BOOL H26xIsIdrFrame(enum AVCodecID vidCodecId, const TD_U8 *data, TD_U32 dataLen);

#endif
