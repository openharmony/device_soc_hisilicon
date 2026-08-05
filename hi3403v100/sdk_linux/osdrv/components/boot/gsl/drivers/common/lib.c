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
#include <lib.h>
#include <common.h>
#include <platform.h>

#define BIT_LEN_OF_BYTE    8

size_t strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc) {
    }

    return sc - s;
}

int memcmp(const void *cs, const void *ct, size_t count)
{
    const unsigned char *su1 = NULL;
    const unsigned char *su2 = NULL;
    int res = 0;

    for (su1 = cs, su2 = ct; count > 0; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0) {
            break;
        }
    return res;
}

#ifdef CFG_DEBUG
#ifdef PRINT_TO_MEM
#define MEM_FOR_PRINT_BASE 0xf8400000
u32 mem_print_base;
#endif

#define CFG_PBSIZE 256

void printf(const char *fmt, ...)
{
}

size_t strnlen(const char *s, size_t count)
{
    const char *sc;
    for (sc = s; count-- && *sc != '\0'; ++sc);
    return sc - s;
}

void set_time_mark()
{
    timer_start();
}

unsigned long  get_time_ms()
{
    return timer_get_val() / timer_get_divider();
}
#endif

void debug_info(const char *printbuffer, char value)
{
}

/**
 * strncmp - Compare two length-limited strings
 * @cs: One string
 * @ct: Another string
 * @count: The maximum number of bytes to compare
 */
int strncmp(const char *cs, const char *ct, size_t count)
{
    register signed char cmp_result = 0;

    while (count) {
        if ((cmp_result = *cs - *ct++) != 0 || !*cs++) {
            break;
        }
        count--;
    }

    return cmp_result;
}

/**
 * strcmp - Compare two strings
 * @cs: One string
 * @ct: Another string
 */
int strcmp(const char *cs, const char *ct)
{
    register signed char cmp_result;

    cmp_result = *cs - *ct++;
    while ((cmp_result == 0) && (*cs++ != '\0')) {
        cmp_result = *cs - *ct++;
    }

    return cmp_result;
}

void *boot_mem_set(void *s, unsigned int c, size_t count)
{
    unsigned long *sl = (unsigned long *) s;
    unsigned long cl = 0;
    char *s8 = NULL;
    int i;

    /* do it one word at a time (32 bits or 64 bits) while possible */
    if (((uintptr_t)s & (sizeof(*sl) - 1)) == 0) {
        for (i = 0; i < sizeof(*sl); i++) {
            cl <<= BIT_LEN_OF_BYTE;
            cl |= c & 0xff;
        }
        while (count >= sizeof(*sl)) {
            *sl++ = cl;
            count -= sizeof(*sl);
        }
    }
    /* fill 8 bits at a time */
    s8 = (char *)sl;
    while (count--) {
        *s8++ = c;
    }

    return s;
}

void *boot_mem_copy(void *dest, const void *src, size_t count)
{
    unsigned long *dl = (unsigned long *)dest;
    unsigned long *sl = (unsigned long *)src;
    char *dst8 = NULL;
    char *s8 = NULL;

    if (src == dest) {
        return dest;
    }

    /* while all data is aligned (common case), copy a word at a time */
    if ((((uintptr_t)dest | (uintptr_t)src) & (sizeof(*dl) - 1)) == 0) {
        while (count >= sizeof(*dl)) {
            *dl++ = *sl++;
            count -= sizeof(*dl);
        }
    }
    /* copy the reset one byte at a time */
    dst8 = (char *)dl;
    s8 = (char *)sl;
    while (count--) {
        *dst8++ = *s8++;
    }

    return dest;
}

