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
 * Description: mini string.h
 */

#ifndef	__STRING_H__
#define	__STRING_H__

#include <bits/alltypes.h>
#include "features.h"

#ifdef __cplusplus
extern "C" {
#endif

void *memcpy (void *__restrict, const void *__restrict, size_t);
size_t strlen (const char *);
int memcmp (const void *cs, const void *ct, unsigned int count);
void *memset (void *, int, size_t);
void *memmove (void *, const void *, size_t);
int strcmp (const char *, const char *);
#ifdef __cplusplus
}
#endif

#endif
