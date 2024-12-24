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
 * Description: gpu proc by diag
 */

#ifndef __DIAG_GPU_PROC_H__
#define __DIAG_GPU_PROC_H__

#include "errcode.h"

typedef struct {
    uint32_t proc_en;
    uint32_t lut_en;
} gpu_gmmu_proc_info_t;

errcode_t diag_gpu_proc_init(void);

#endif /* __DIAG_GPU_PROC_H__ */
