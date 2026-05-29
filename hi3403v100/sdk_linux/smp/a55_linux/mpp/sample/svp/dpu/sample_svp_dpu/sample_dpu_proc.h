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
#ifndef SAMPLE_DPU_PROC_H
#define SAMPLE_DPU_PROC_H
#include "ot_type.h"

td_s32 sample_svp_dpu_file_rect_match(td_void);
td_void sample_svp_dpu_file_rect_match_handle_sig(td_void);

/* This case only for function design reference */
td_s32 sample_svp_dpu_vi_vpss_rect_match(td_void);
td_void sample_svp_dpu_vi_vpss_rect_match_handle_sig(td_void);
#endif /* SAMPLE_DPU_PROC_H */
