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
 * Description: oal type head file.
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __OAL_TYPES_DEVICE_ROM_H__
#define __OAL_TYPES_DEVICE_ROM_H__

#include "osal_types.h"
#include "common_error_code_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef INLINE__
#ifdef INLINE_TO_FORCEINLINE
#define INLINE__    __inline__ __attribute__((always_inline))
#else
#define INLINE__    __inline
#endif
#endif
#define NOINLINE__ __attribute__ ((noinline))
#define OAL_PTR_NULL (0L) /* 空指针定义，指针判断是否为空时与OAL_PTR_NULL进行比较 */

#ifndef NULL
#define NULL OAL_PTR_NULL
#endif

/* linux错误码 */
#define OAL_EFAIL 1   /* 内核通用错误返回值 -1 */
#define OAL_EINVAL 22 /* Invalid argument */

typedef enum {
    OAL_FALSE = 0,
    OAL_TRUE = 1,
    OAL_BUTT
} oal_bool_enum;
typedef osal_u8 oal_bool_enum_uint8;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of oal_types_device.h */
