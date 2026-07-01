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

#ifndef __SAMPLE_VENC_H__
#define __SAMPLE_VENC_H__

#include "ot_type.h"
#include "sample_comm.h"

td_s32 sample_comm_venc_start_get_stream(ot_venc_chn venc_chn[], td_s32 cnt);
td_s32 sample_comm_venc_stop_get_stream(td_void);
ot_pixel_format sample_comm_venc_get_pixel_format(td_void);

td_s32 init_ot_encoder(td_void);
td_s32 start_ot_encoder(td_void);
td_s32 stop_ot_encoder(td_void);
td_s32 set_encoder_idr(td_void);

td_void set_user_config_format(ot_payload_type *format, ot_pic_size *wh, td_s32 *c);

#endif /* __SAMPLE_VENC_H__ */
