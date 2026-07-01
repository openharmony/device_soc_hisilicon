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

#ifndef FORMAT_COMMON_H
#define FORMAT_COMMON_H

#include <stdint.h>
#include "plugin_common.h"
#include "format_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum {
    STATE_IDLE,
    STATE_INIT,
    STATE_START,
    STATE_STOP,
} FormatState;

#define CHK_HANDLE_VALID(ctx) \
do { \
    if ((ctx) == NULL || (ctx)->innerCtx != (ctx)) { \
        MEDIA_HAL_LOGE(MODULE_NAME, "handle invalid"); \
        return MEDIA_HAL_ERR; \
    } \
} while (0)

CodecFormat CodecTypeToCodecFormat(CodecType type);
CodecType CodecFormatToCodecType(CodecFormat type);
SubtitleFormat SubTypeToSubFormat(SubtitleFormatType type);
void FormatPacketToFrame(const FormatPacket *packet, FormatFrame *frame);
void FormatFrameToPacket(const FormatFrame *frame, FormatPacket *packet);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // FORMAT_COMMON_H
/** @} */
