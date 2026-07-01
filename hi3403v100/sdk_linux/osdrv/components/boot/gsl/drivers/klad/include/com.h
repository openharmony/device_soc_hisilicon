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

#ifndef __COM_H__
#define __COM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define chk_ret(func) \
    do { \
        if ((func) != TD_SUCCESS) { \
            print_error_func(func, TD_FAILURE); \
            return TD_FAILURE; \
        } \
    } while (0)

#define neq_ret(a, b) \
    do { \
        if ((a) != (b)) { \
            print_error_check(a, b); \
            return TD_FAILURE; \
        } \
    } while (0)

#define chk_wd_1(data0, check_wd) \
    neq_ret((uintptr_t)(data0) ^ ((uintptr_t)(data0) << 16), check_wd)
#define chk_wd_2(data0, data1, check_wd) \
    neq_ret((uintptr_t)(data0) ^ (uintptr_t)(data1), check_wd)
#define chk_wd_3(data0, data1, data2, check_wd) \
    neq_ret((uintptr_t)(data0) ^ (uintptr_t)(data1) ^ (uintptr_t)(data2), check_wd)
#define chk_wd_4(data0, data1, data2, data3, check_wd) \
    neq_ret((uintptr_t)(data0) ^ (uintptr_t)(data1) ^ (uintptr_t)(data2) ^ (uintptr_t)(data3), check_wd)
#define chk_wd_5(data0, data1, data2, data3, data4, check_wd) \
    neq_ret((uintptr_t)(data0) ^ (uintptr_t)(data1) ^ (uintptr_t)(data2) ^ (uintptr_t)(data3) ^ \
            (uintptr_t)(data4), check_wd)

#define gen_wd_1(data0) ((uintptr_t)(data0) ^ ((uintptr_t)(data0) << 16))
#define gen_wd_2(data0, data1) ((uintptr_t)(data0) ^ (uintptr_t)(data1))
#define gen_wd_3(data0, data1, data2) ((uintptr_t)(data0) ^ (uintptr_t)(data1) ^ (uintptr_t)(data2))
#define gen_wd_4(data0, data1, data2, data3) \
    ((uintptr_t)(data0) ^ (uintptr_t)(data1) ^ (uintptr_t)(data2) ^ (uintptr_t)(data3))
#define gen_wd_5(data0, data1, data2, data3, data4) \
    ((uintptr_t)(data0) ^ (uintptr_t)(data1) ^ (uintptr_t)(data2) ^ (uintptr_t)(data3) ^ (uintptr_t)(data4))

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
