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
#ifndef SAMPLE_MAU_DIST_H
#define SAMPLE_MAU_DIST_H
#include "ot_type.h"

/*
 * function : show the sample of mau cosine distance
 */
td_void sample_svp_mau_cos_dist(td_char idx_flag);

/*
 * function : show the sample of mau euclidean distance
 */
td_void sample_svp_mau_euclid_dist(td_char out_type_flag);

/*
 * function : mau_cos_dist sample signal handle
 */
td_void sample_svp_mau_cos_dist_handle_sig(td_void);

/*
 * function : mau_euclid_dist sample signal handle
 */
td_void sample_svp_mau_euclid_dist_handle_sig(td_void);

#endif
