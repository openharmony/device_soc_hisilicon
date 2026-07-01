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
#ifndef _LINUX_CTYPE_H
#define _LINUX_CTYPE_H

/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */

#define CTYPE_U	0x01 /* upper */
#define CTYPE_L	0x02 /* lower */
#define CTYPE_D	0x04 /* digit */
#define CTYPE_C	0x08 /* cntrl */
#define CTYPE_P	0x10 /* punct */
#define CTYPE_S	0x20 /* white space (space/lf/tab) */
#define CTYPE_X	0x40 /* hex digit */
#define CTYPE_SP	0x80 /* hard space (0x20) */

#define CTYPE_TABLE_SIZE 256
extern unsigned char _ctype[CTYPE_TABLE_SIZE];

#define ctype_ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((ctype_ismask(c) & (CTYPE_U | CTYPE_L | CTYPE_D)) != 0)
#define isalpha(c)	((ctype_ismask(c) & (CTYPE_U | CTYPE_L)) != 0)
#define iscntrl(c)	((ctype_ismask(c) & (CTYPE_C)) != 0)
#define isdigit(c)	((ctype_ismask(c) & (CTYPE_D)) != 0)
#define isgraph(c)	((ctype_ismask(c) & (CTYPE_P | CTYPE_U | CTYPE_L | CTYPE_D)) != 0)
#define islower(c)	((ctype_ismask(c) & (CTYPE_L)) != 0)
#define isprint(c)	((ctype_ismask(c) & (CTYPE_P | CTYPE_U | CTYPE_L | CTYPE_D | CTYPE_SP)) != 0)
#define ispunct(c)	((ctype_ismask(c) & (CTYPE_P)) != 0)
#define isspace(c)	((ctype_ismask(c) & (CTYPE_S)) != 0)
#define isupper(c)	((ctype_ismask(c) & (CTYPE_U)) != 0)
#define isxdigit(c)	((ctype_ismask(c) & (CTYPE_D | CTYPE_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

static inline unsigned char ctype_tolower(unsigned char c)
{
    if (isupper(c)) {
        c -= 'A' - 'a';
    }
    return c;
}

static inline unsigned char ctype_toupper(unsigned char c)
{
    if (islower(c)) {
        c -= 'a' - 'A';
    }
    return c;
}

#define tolower(c) ctype_tolower(c)
#define toupper(c) ctype_toupper(c)

#endif

