/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: audio dump by zdiag
 */

#ifndef __ZDIAG_AUDIO_DUMP_H__
#define __ZDIAG_AUDIO_DUMP_H__

#include "errcode.h"

typedef struct {
    uint32_t file_size;
    uint32_t mod_handle;
    uint32_t mod_output;
} audio_dump_cmd_item;

errcode_t zdiag_audio_dump_init(void);

#endif /* __ZDIAG_AUDIO_DUMP_H__ */
