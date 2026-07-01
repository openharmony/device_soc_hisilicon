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

#ifndef __MEDIA_VDEC_H__
#define __MEDIA_VDEC_H__

#include "sample_comm.h"

td_s32 sample_uvc_media_init(const td_char *type_name, td_u32 width, td_u32 height);
td_s32 sample_uvc_media_exit(td_void);
td_s32 sample_uvc_media_send_data(td_void *data, td_u32 size, td_u32 stride,
    const ot_size *pic_size, const td_char *type_name);
td_s32 sample_uvc_media_stop_receive_data(td_void);

#endif /* end of #ifndef __MEDIA_VDEC_H__ */
