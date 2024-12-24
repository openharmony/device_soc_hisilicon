/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: OS Abstract Layer.
 */

#ifndef OSAL_INNER_H
#define OSAL_INNER_H

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define va_num_args_impl(_1, _2, _3, _4, _5, N, ...) N
#define va_num_args(...) va_num_args_impl(__VA_ARGS__, 5, 4, 3, 2, 1)
#define all_unused_impl_(nargs) osal_unused##nargs
#define all_unused_impl(nargs) all_unused_impl_(nargs)

#define osal_unused1(var) (void)(var)
#define osal_unused2(y, z) osal_unused1(y), osal_unused1(z)
#define osal_unused3(x, y, z) osal_unused1(x), osal_unused2(y, z)
#define osal_unused4(a, b, x, y) osal_unused2(a, b), osal_unused2(x, y)
#define osal_unused5(a, b, x, y, z) osal_unused2(a, b), osal_unused3(x, y, z)
#define osal_unused(...) all_unused_impl(va_num_args(__VA_ARGS__))(__VA_ARGS__)

#define osal_log(fmt, ...) osal_printk("[%s:%d]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif
