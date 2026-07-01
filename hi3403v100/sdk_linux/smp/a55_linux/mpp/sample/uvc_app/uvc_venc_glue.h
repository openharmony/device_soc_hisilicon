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

#ifndef __UVC_VENC_GLUE_H__
#define __UVC_VENC_GLUE_H__

#include "frame_cache.h"

void clear_ok_queue();
void add_multi_payload_header(frame_node_t *node, unsigned long frame_type);
void set_multi_payload_size(frame_node_t *node);

#endif // __UVC_VENC_GLUE_H__
