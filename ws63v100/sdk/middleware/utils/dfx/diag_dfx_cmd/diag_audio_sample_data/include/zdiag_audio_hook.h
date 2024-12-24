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
 * Description: message for capturing data between core a and the dsp.
 * Author:
 */

#ifndef __ZDIAG_AUDIO_HOOK_H__
#define __ZDIAG_AUDIO_HOOK_H__

#include "td_type.h"

typedef enum {
    HOOK_AUDIO_DATA_START,
    HOOK_AUDIO_DATA_STOP,
} hook_audio_data_status;

typedef struct {
    td_u32 cnt; /* 数据帧计数 */
    td_u32 size;
    td_u32 node_id;
} audio_hook_header;

typedef struct {
    audio_hook_header hdr;
    td_s32 data[];
} audio_hook_frame;

typedef struct {
    hook_audio_data_status status;
    td_u32 id;
} audio_hook_node;

td_void hook_audio_data_start(td_u32 node_id);
td_void hook_audio_data_stop(td_u32 node_id);

#endif /* __ZDIAG_AUDIO_HOOK_H__ */
