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
 * Description: osal adapt debug
 * Create: 2023-5-17
 */
#ifdef OSAL_IRQ_RECORD_DEBUG
#ifndef __OSAL_ADAPT_DEBUG_H__
#define __OSAL_ADAPT_DEBUG_H__

#include "td_type.h"
#include "soc_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define RECORD_CNT_MAX 5

typedef enum {
    IRQ_LOCK = 0,
    IRQ_UNLOCK,
    IRQ_RESTORE,
    IRQ_TYPE_BUFF
} irq_type_enum;

typedef struct record {
    td_u32 caller[IRQ_TYPE_BUFF][RECORD_CNT_MAX];
    td_u32 stauts[IRQ_TYPE_BUFF][RECORD_CNT_MAX];
    td_u32 idx[IRQ_TYPE_BUFF];
} debug_irq_record;

debug_irq_record *osal_get_irq_record(void);
void osal_irq_record(irq_type_enum type, td_u32 caller, td_u32 stauts);
void osal_clear_irq_record(void);
void osal_print_irq_record(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
#endif /* #ifdef OSAL_IRQ_RECORD_DEBUG */