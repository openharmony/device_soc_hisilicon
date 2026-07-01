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

#ifndef __OT_MW_TYPE_H__
#define __OT_MW_TYPE_H__

#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef td_uchar                TD_UCHAR;
typedef td_u8                   TD_U8;
typedef td_u16                  TD_U16;
typedef td_u32                  TD_U32;
typedef td_ulong                TD_UL;
typedef td_ulong                TD_ULONG;
typedef uintptr_t               TD_UINTPTR_T;

typedef td_void                 TD_VOID;
typedef td_char                 TD_CHAR;
typedef td_s8                   TD_S8;
typedef td_s16                  TD_S16;
typedef td_s32                  TD_S32;
typedef td_slong                TD_SL;

typedef td_float                TD_FLOAT;
typedef td_double               TD_DOUBLE;

#ifndef _M_IX86
    typedef td_u64              TD_U64;
    typedef td_s64              TD_S64;
#else
    typedef unsigned __int64    TD_U64;
    typedef __int64             TD_S64;
#endif

typedef td_ulong                TD_SIZE_T;
typedef td_ulong                TD_LENGTH_T;
typedef td_phys_addr_t          TD_PHYS_ADDR_T;

typedef td_handle               TD_HANDLE;
typedef td_bool                 TD_BOOL;

/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
#ifndef NULL
    #define NULL                0L
#endif


#ifndef TD_UNUSED
#define TD_UNUSED(x)            ((x) = (x))
#endif

#define TD_MW_INVALID_HANDLE (TD_HANDLE)(-1)

typedef td_void *TD_MW_PTR;

#ifndef TD_NULL_PTR
#define TD_NULL_PTR 0L
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __OT_MW_TYPE_H__ */
