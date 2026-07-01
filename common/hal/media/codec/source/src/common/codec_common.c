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

#include "codec_common.h"
#include <dlfcn.h>
#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "media_hal_common.h"
#include "securec.h"

#define MODULE_NAME "CODEC_COMMON"

#define CODEC_PATH_MAX_LEN     (PATH_MAX + 1)
#define AVPLAY_MAX_PRINT_STR_LEN  1024

typedef struct {
    CodecType codecType;
    const td_char *typeName;
} CodecTypeMap;

static CodecTypeMap g_typeMaps[] = {
    {VIDEO_DECODER, "vdec"},
    {VIDEO_ENCODER, "venc"},
    {AUDIO_DECODER, "adec"},
    {AUDIO_ENCODER, "aenc"},
    {INVALID_TYPE, "invalid type"},
};

td_s32 Int32Multiple(td_s32 firstNumber, td_s32 secondeNumner, td_s32 *result)
{
    if (result == TD_NULL) {
        return TD_FAILURE;
    }
    if (secondeNumner == 0) {
        *result = 0;
        return TD_SUCCESS;
    }

    const td_s32 intMax = 0x7fffffff;
    if (firstNumber > (intMax / secondeNumner)) {
        return TD_FAILURE;
    }
    *result = firstNumber * secondeNumner;
    return TD_SUCCESS;
}

td_u32 Uint32Multiple(td_u32 firstNumber, td_u32 secondeNumner, td_u32 *result)
{
    if (result == TD_NULL) {
        return TD_FAILURE;
    }
    if (secondeNumner == 0) {
        *result = 0;
        return TD_SUCCESS;
    }

    const td_u32 uintMax = 0xffffffff;
    if (firstNumber > (uintMax / secondeNumner)) {
        return TD_FAILURE;
    }
    *result = firstNumber * secondeNumner;
    return TD_SUCCESS;
}

AvCodecMime GetCodecMime(const Param *attr, int len)
{
    CHK_NULL_RETURN(attr, TD_FAILURE, "attr null");
    for (int i = 0; i < len; i++) {
        if (attr[i].key == KEY_MIMETYPE) {
            if (attr[i].size != sizeof(AvCodecMime)) {
                MEDIA_HAL_LOGE(MODULE_NAME, "param kParamIndexMimeType size wrong");
                return MEDIA_MIMETYPE_INVALID;
            }
            CHK_NULL_RETURN(attr[i].val, TD_FAILURE, "attr val null");
            AvCodecMime upperMime = *((AvCodecMime *)(attr[i].val));
            return upperMime;
        }
    }
    return MEDIA_MIMETYPE_INVALID;
}

td_s64 CalDiffTimeBetween(struct timeval timeStart, struct timeval timeEnd,
    const char *tag, const char *name, bool showLog)
{
    CHK_NULL_RETURN(tag, TD_FAILURE, "tag null");
    CHK_NULL_RETURN(name, TD_FAILURE, "name null");

    const td_u64 timeUs = 1000000;
    td_s64 diffSecToUs = (td_s64)(timeEnd.tv_sec - timeStart.tv_sec) * timeUs;
    td_s64 diffUsec = (td_s64)(timeEnd.tv_usec - timeStart.tv_usec);
    td_s64 diffTimeUS = diffSecToUs + diffUsec;
    if (showLog) {
        MEDIA_HAL_LOGI(tag, "%s take %lld us", name, diffTimeUS);
    }
    return diffTimeUS;
}

const char* GetCodecTypeName(CodecType typeEnum)
{
    if (typeEnum > INVALID_TYPE || typeEnum < VIDEO_DECODER) {
        return g_typeMaps[INVALID_TYPE].typeName;
    }
    return g_typeMaps[typeEnum].typeName;
}