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

#ifndef __SS_AACENC_H__
#define __SS_AACENC_H__

#include "ot_aacenc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 ss_aacenc_get_version(ot_aacenc_version *version);

td_s32 ss_aacenc_init_default_config(ot_aacenc_config *config);

td_s32 ss_aacenc_open(ot_aac_encoder **aac_encoder, const ot_aacenc_config *config);

td_s32 ss_aacenc_frame(ot_aac_encoder *aac_encoder, td_s16 *input_buf, td_u8 *output_buf, td_s32 *num_out_bytes);

td_void ss_aacenc_close(ot_aac_encoder *aac_encoder);

td_s32 ss_aacenc_register_mod(const td_void *mod_handle);

td_void *ss_aac_sbrenc_get_handle(td_void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
