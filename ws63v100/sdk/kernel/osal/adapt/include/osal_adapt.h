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
 * Description: osal adapt header \n
 *
 */

#ifndef OSAL_ADAPT_H
#define OSAL_ADAPT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef void (*print_cb_t)(const char *fmt, ...);

extern void test_suite_uart_send(const char *str);
void osal_dcache_flush_all(void);
void osal_register_print(print_cb_t print);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
