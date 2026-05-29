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
#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned long int uintptr_t;

typedef unsigned long size_t;

typedef int errno_t;

#define BITS_PER_LONG 32

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

typedef int bool;

#define TRUE    1
#define FALSE   0

#define ERROR   (-1)
#define OK      0
#define TD_SUCCESS	0
#define TD_FAILURE	(-1)
#define DATA_EMPTY	(-2)

#define AUTH_SUCCESS	(0x3CA5965A)
#define AUTH_FAILURE	(0xC35A69A5)

#define IN
#define OUT

#define PRIVATE static

typedef signed char     int8_t;
typedef unsigned char   uint8_t;
typedef signed short    int16_t;
typedef unsigned short  uint16_t;
typedef signed int      int32_t;
typedef unsigned int    uint32_t;
typedef long            int64_t;
typedef unsigned long   uint64_t;

#endif

