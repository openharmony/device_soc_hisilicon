/**
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
 *
 * Description: LiteOS NMI adapt header. \n
 *
 * History: \n
 * 2023-11-09, Create file. \n
 */

#ifndef NMI_ADAPT_H
#define NMI_ADAPT_H

#include "arch/exception.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern VOID OsClearNMI(VOID);
extern VOID OsExcInfoDisplay(UINT32 excType, const ExcContext *excBufAddr);
extern VOID irq_wdt_handler(VOID);
extern VOID do_hard_fault_handler(ExcContext *exc_buf_addr);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif