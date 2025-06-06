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
 * Description: mbedtls harden adapt internal header file.
 *
 * Create: 2023-05-10
*/

#ifndef MBEDTLS_CRYPTO_TYPE_H
#define MBEDTLS_CRYPTO_TYPE_H

#ifdef __KERNEL__
#include <linux/types.h>
#elif (defined(__LITEOS__) || defined(TD_SUPPORT_STDLIB) || defined(CONFIG_CGRA_CORE))
#include <stdint.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif
#endif

/* defines */
#define TD_NULL             NULL
#define TD_NULL_PTR         TD_NULL
#define TD_FALSE 0
#define TD_TRUE  1
#define TD_SUCCESS              0
#define TD_FAILURE              (-1)

typedef unsigned char           td_uchar;
typedef unsigned char           td_u8;
typedef unsigned short          td_u16;
typedef unsigned int            td_u32;
typedef unsigned long long      td_u64;
typedef unsigned long           td_ulong;
typedef char                    td_char;
typedef signed char             td_s8;
typedef short                   td_s16;
typedef int                     td_s32;
typedef long long               td_s64;
typedef long                    td_slong;
typedef float                   td_float;
typedef double                  td_double;
typedef void                    td_void;
typedef td_u8                   td_bool;
typedef td_u32                  td_handle;
typedef td_u8                   td_byte;
typedef td_byte*                td_pbyte;
typedef void*                   td_pvoid;
typedef volatile td_u32         td_u32_reg;
typedef unsigned long           td_size_t;
typedef signed   long           td_ssize_t;
typedef unsigned long           td_length_t;
typedef unsigned long long      td_mem_size_t;
typedef long long               td_mem_handle_t;
typedef unsigned int            td_fr32; // 分数，分子高16bit，分母低16bit

#if ((!defined(_LINUX_TYPES_H)) && (!defined(_STDINT_H)) && (!defined(CONFIG_CGRA_CORE)) && \
    (!defined(__DEFINED_uintptr_t)))
typedef unsigned int            uintptr_t;
#define __DEFINED_uintptr_t
#endif
typedef uintptr_t            td_uintptr_t;

#ifdef __LP64__
typedef unsigned long long      td_phys_addr_t;
typedef unsigned long long      td_virt_addr_t;
#else
typedef unsigned int            td_phys_addr_t;
typedef unsigned int            td_virt_addr_t;
#endif
typedef unsigned int            td_phys_addr_bit32;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MBEDTLS_CRYPTO_TYPE_H */

