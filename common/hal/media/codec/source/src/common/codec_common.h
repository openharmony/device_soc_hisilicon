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

#ifndef CODEC_COMMON_H
#define CODEC_COMMON_H

#include <pthread.h>
#include <sys/time.h>
#include "codec_type.h"
#include "ot_mw_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

enum {
    VID_BITRATE_LEVEL1 = 1,
    VID_BITRATE_LEVEL2 = 52428800,
    VID_BITRATE_LEVEL3 = 83886080,
    VID_BITRATE_LEVEL4 = 104857600,
    AUD_BITRATE_LEVEL1 = 10000000,
};

#define ALGIN_LEVEL_2 2
#define ALGIN_LEVEL_4 4
#define ALGIN_LEVEL_16 16
#define ALGIN_LEVEL_INVALID (-1)

#define MS_SCALE   1000

#define AV_INVALID_PTS (-1)

#define NO_WAIT  0

#define ZERO_ARG  0

#define MAX_MSG_QUEUE_SIZE  50

#define MAX_MSG_QUEUE_PAYLOAD_SIZE  128

#define AVPLAY_SPEED_DECIMAL_PRECISION  1000

// 1024 * 16 (16k)
#define VIDEO_WIDTH_HEIGHT_MAX 16384

// 268435456 = 256M = 1024 * 1024 * 256
#define AVPLAY_MAX_VBBUFFER_SIZE 268435456

// comes from ffmpeg, more detail pls refer to ffmpeg doc
// avc decoding in ffmepg needs 64 padding bytes for input buffer
#ifndef AV_INPUT_BUFFER_PADDING_SIZE
#define AV_INPUT_BUFFER_PADDING_SIZE 64
#endif

#define AV_UNUSED_ATTR __attribute__((unused))

#define AVPLAY_RETURN_STR_IF_INVALID(str, len)                                  \
    do {                                                                        \
        if (((str) == TD_NULL) || ((len) == 0)) {                                \
            MEDIA_HAL_LOGE(MODULE_NAME, "" #str " is invalid");   \
            return TD_FAILURE;                                                  \
        }                                                                       \
    } while (0)

#define AVPLAY_COND_WAIT(cond, mutex)               \
    do {                                            \
        (void)pthread_cond_wait(&(cond), &(mutex)); \
    } while (0)

#define AVPLAY_COND_SIGNAL(cond)            \
    do {                                    \
        (void)pthread_cond_signal(&(cond)); \
    } while (0)

td_s32 Int32Multiple(td_s32 firstNumber, td_s32 secondeNumner, td_s32 *result);

td_u32 Uint32Multiple(td_u32 firstNumber, td_u32 secondeNumner, td_u32 *result);

AvCodecMime GetCodecMime(const Param *attr, int len);

td_s64 CalDiffTimeBetween(struct timeval timeStart, struct timeval timeEnd, const char *tag,
    const char* diffName, bool showLog);

const char* GetCodecTypeName(CodecType typeEnum);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* CODEC_COMMON_H */
