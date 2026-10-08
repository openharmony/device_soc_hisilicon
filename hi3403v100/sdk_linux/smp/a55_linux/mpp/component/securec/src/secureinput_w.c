/*
 * Copyright 2020 Huawei Technologies Co., Ltd.
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

/* If some platforms don't have wchar.h, don't include it */
#if !(defined(SECUREC_VXWORKS_PLATFORM))
/* If there is no macro below, it will cause vs2010 compiling alarm */
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#ifndef SECUREC_STDC_WANT_SECURE_LIB
/* The order of adjustment is to eliminate alarm of Duplicate Block */
#define SECUREC_STDC_WANT_SECURE_LIB 0
#endif
#ifndef SECUREC_CRTIMP_ALTERNATIVE
#define SECUREC_CRTIMP_ALTERNATIVE     /* Comment microsoft *_s function */
#endif
#endif
#include <wchar.h>
#endif

/* Disable wchar func to clear vs warning */
#define SECUREC_ENABLE_WCHAR_FUNC       0
#define SECUREC_FORMAT_OUTPUT_INPUT     1

#ifndef SECUREC_FOR_WCHAR
#define SECUREC_FOR_WCHAR
#endif

#include "secinput.h"

#include "input.inl"

SECUREC_INLINE unsigned int SecWcharHighBits(SecInt ch)
{
    /* Convert int to unsigned int clear 571 */
    return ((unsigned int)(int)ch & (~0xffU));
}

SECUREC_INLINE unsigned char SecWcharLowByte(SecInt ch)
{
    /* Convert int to unsigned int clear 571 */
    return (unsigned char)((unsigned int)(int)ch & 0xffU);
}

SECUREC_INLINE int SecIsDigit(SecInt ch)
{
    if (SecWcharHighBits(ch) != 0) {
        return 0; /* Same as isdigit */
    }
    return isdigit((int)SecWcharLowByte(ch));
}

SECUREC_INLINE int SecIsXdigit(SecInt ch)
{
    if (SecWcharHighBits(ch) != 0) {
        return 0; /* Same as isxdigit */
    }
    return isxdigit((int)SecWcharLowByte(ch));
}

SECUREC_INLINE int SecIsSpace(SecInt ch)
{
    return iswspace((wint_t)(int)(ch));
}

