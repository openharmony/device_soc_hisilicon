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
 * Description: osal types header file
 */

#ifndef __OSAL_TYPES_H__
#define __OSAL_TYPES_H__

/* 基本数据类型定义 */
typedef unsigned char           osal_uchar;
typedef unsigned char           osal_u8;
typedef unsigned short          osal_u16;
typedef unsigned int            osal_u32;
typedef unsigned long long      osal_u64;
typedef unsigned long           osal_ulong;
typedef char                    osal_char;
typedef signed char             osal_s8;
typedef short                   osal_s16;
typedef int                     osal_s32;
typedef long long               osal_s64;
typedef long                    osal_slong;
typedef float                   osal_float;
typedef double                  osal_double;
typedef unsigned long           osal_size_t;
typedef unsigned long           osal_length_t;
typedef osal_u32                osal_handle;
typedef osal_u8                 osal_bool;
// typedef unsigned int            uintptr_t;
typedef void                    osal_void;
typedef void*                   osal_pvoid;

typedef osal_u8                 osal_byte;
typedef osal_byte*              osal_pbyte;
typedef osal_u32                size_t;
typedef unsigned long long      uint64_t;

typedef unsigned int            osal_uintptr_t;
typedef unsigned int            osal_phys_addr_t;
typedef unsigned int            osal_virt_addr_t;

/* defines */
#ifndef NULL
#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif
#endif

#define OSAL_U32_MAX            0xFFFFFFFF
#define OSAL_U64_MAX            0xFFFFFFFFFFFFFFFFUL
#define OSAL_U16_MAX            0xFFFF
#define OSAL_U8_MAX             0xFF
#define OSAL_U4_MAX             0x0f

#define OSAL_FALSE 0
#define OSAL_TRUE  1

#define OSAL_OK 0
#define OSAL_NOK 1

#ifdef __cplusplus
#define OSAL_NULL       0
#else
#define OSAL_NULL    ((void *)0)
#endif

#define OSAL_SYS_WAIT_FOREVER           0xFFFFFFFF

#endif /* __OSAL_TYPES_H__ */
