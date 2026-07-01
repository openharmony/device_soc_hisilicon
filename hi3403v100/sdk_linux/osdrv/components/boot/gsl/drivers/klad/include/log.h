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
#ifndef LOG_H
#define LOG_H

#include "lib.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define print_error_check(a, b) \
    printf("[%s][%04d]check %s and %s failed.\n", __FUNCTION__, __LINE__, #a, #b)
#define print_error_func(func, ret) \
    printf("[%s][%04d]call [%s] failed, return 0x%x.\n", __FUNCTION__, __LINE__, #func, ret)
#define print_debug_func() \
    printf("[%s][%04d]mark.\n", __FUNCTION__, __LINE__)
#define log_printf(fmt...) \
    do { \
        printf("\033[1;31m"); \
        printf("[%s][%d] ", __func__, __LINE__); \
        printf(fmt); \
        printf("\033[0m"); \
    } while (0)
#define print_val(v) \
    printf("[%-40s]  0x%08x.\n", #v, v)
#define print_str(v, l) \
    do { \
        int j = 0; \
        printf("[%-40s]  ", #v); \
        for (j = 0; j < (l); j++) \
            printf("%02x", (v)[j]); \
        printf(".\n"); \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* deob.h */
