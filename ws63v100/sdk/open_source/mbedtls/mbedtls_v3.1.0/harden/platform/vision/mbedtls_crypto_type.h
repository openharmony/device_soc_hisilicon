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
#else

#include <stdint.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef NULL
    #define NULL                0L
#endif

#define TD_NULL                 0L
#define TD_SUCCESS              0
#define TD_FAILURE              (-1)

typedef unsigned char           td_uchar;
typedef unsigned char           td_u8;
typedef unsigned short          td_u16;
typedef unsigned int            td_u32;
typedef unsigned long           td_ulong;

typedef char                    td_char;
typedef signed char             td_s8;
typedef short                   td_s16;
typedef int                     td_s32;
typedef long                    td_slong;

typedef float                   td_float;
typedef double                  td_double;

typedef void                    td_void;

typedef unsigned long long  td_u64;
typedef long long           td_s64;

#ifdef CONFIG_PHYS_ADDR_BIT_WIDTH_64
    typedef unsigned long long  td_phys_addr_t;
#else
    typedef unsigned int        td_phys_addr_t;
#endif

typedef td_u32                  td_handle;
typedef uintptr_t               td_uintptr_t;
typedef unsigned int            td_fr32;

typedef enum {
    TD_FALSE = 0,
    TD_TRUE  = 1,
} td_bool;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MBEDTLS_CRYPTO_TYPE_H */

