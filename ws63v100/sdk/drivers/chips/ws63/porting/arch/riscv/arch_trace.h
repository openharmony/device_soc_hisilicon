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
 * Description: ARCH cpu trace interface for project
 *
 * Create:  2021-10-25
 */
#ifndef ARCH_TRACE_H
#define ARCH_TRACE_H

#include <stdint.h>
#include "platform_core.h"
#ifdef SUPPORT_CPU_TRACE
#include "cpu_trace.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define  RISCV_TRACE_MEM_REGION_START   TRACE_MEM_REGION_START
#define  RISCV_TRACE_MEM_REGION_LENGTH  TRACE_MEM_REGION_LENGTH

#ifdef SUPPORT_CPU_TRACE
typedef cpu_trace_item_t  riscv_cpu_trace_item_t;
#endif

/**
 * @brief  Disable cpu trace for riscv.
 */
static inline void riscv_cpu_trace_disable(void)
{
#ifdef SUPPORT_CPU_TRACE
    cpu_trace_disable();
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
