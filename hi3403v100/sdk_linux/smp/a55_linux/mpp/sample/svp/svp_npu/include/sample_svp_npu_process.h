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

#ifndef SAMPLE_SVP_NPU_PROCESS_H
#define SAMPLE_SVP_NPU_PROCESS_H

#include "ot_type.h"

/* function : show the sample of acl resnet50 */
td_void sample_svp_npu_acl_resnet50(td_void);

/* function : show the sample of acl resnet50_multithread */
td_void sample_svp_npu_acl_resnet50_multi_thread(td_void);

/* function : show the sample of resnet50 dyanamic batch with mem cached */
td_void sample_svp_npu_acl_resnet50_dynamic_batch(td_void);

/* function : show the sample of lstm */
td_void sample_svp_npu_acl_lstm(td_void);

/* function : show the sample of rfcn */
td_void sample_svp_npu_acl_rfcn(td_void);

/* function : show the sample of e2e yolo. RPN processed by npu */
td_void sample_svp_npu_acl_e2e_yolo_npu(td_u32 index);

/* function : show the sample of yolo. RPN processed by npu */
td_void sample_svp_npu_acl_yolo_npu(td_u32 index);

/* function : show the sample of e2e yolo. RPN processed by cpu */
td_void sample_svp_npu_acl_e2e_yolo_cpu(td_u32 index);

/* function : show the sample of yolo. RPN processed by cpu */
td_void sample_svp_npu_acl_yolo_cpu(td_u32 index);

/* function : show the sample of sign handle */
td_void sample_svp_npu_acl_handle_sig(td_void);

#endif
