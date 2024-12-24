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
 * Description: ARCH interface for project
 *
 * Create:  2021-07-06
 */
#ifndef ARCH_PORT_H
#define ARCH_PORT_H

#include <stdint.h>
#include <stdbool.h>
#include "chip_core_irq.h"
#include "memory_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef USE_CMSIS_OS
#define irq_prio(irq_id) m_auc_int_pri[irq_id]
#else
#define irq_prio(irq_id) g_auc_int_pri[irq_id]
#endif

extern uint8_t m_auc_int_pri[BUTT_IRQN];
extern uint8_t g_auc_int_pri[BUTT_IRQN];
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
