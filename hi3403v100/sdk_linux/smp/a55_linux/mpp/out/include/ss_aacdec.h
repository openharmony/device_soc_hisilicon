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

#ifndef __SS_AACDEC_H__
#define __SS_AACDEC_H__

#include "ot_aacdec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 ss_aacdec_get_version(ot_aacdec_version *version);

ot_aac_decoder ss_aacdec_init_decoder(ot_aacdec_transport_type transport_type);

td_void ss_aacdec_free_decoder(ot_aac_decoder aac_decoder);

td_s32 ss_aacdec_set_raw_mode(ot_aac_decoder aac_decoder, td_s32 chn_num, td_s32 sample_rate);

td_s32 ss_aacdec_find_sync_header(ot_aac_decoder aac_decoder, td_u8 **in_buf, td_s32 *bytes_left);

td_s32 ss_aacdec_frame(ot_aac_decoder aac_decoder, td_u8 **in_buf, td_s32 *bytes_left, td_s16 *out_pcm);

td_s32 ss_aacdec_set_eos_flag(ot_aac_decoder aac_decoder, td_s32 eos_flag);

td_s32 ss_aacdec_get_last_frame_info(ot_aac_decoder aac_decoder, ot_aacdec_frame_info *frame_info);

td_s32 ss_aacdec_flush_codec(ot_aac_decoder aac_decoder);

td_s32 ss_aacdec_register_mod(const td_void *mod_handle);

td_void *ss_aac_sbrdec_get_handle(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
