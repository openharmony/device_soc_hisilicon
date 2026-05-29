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

#ifndef __SAMPLE_YUV_H__
#define __SAMPLE_YUV_H__

#include "ot_type.h"

#define UVC_SAVE_FILE 0

td_s32 do_close_yuv_capture(td_void);
td_void set_yuv_property(td_void);
td_void set_compsite_yuv_property(td_void);

td_s32 sample_start_get_yuv(td_s32 vpss_grp, td_s32 vpss_chn);
td_s32 sample_stop_get_yuv(td_void);

#endif /* __SAMPLE_YUV_H__ */
