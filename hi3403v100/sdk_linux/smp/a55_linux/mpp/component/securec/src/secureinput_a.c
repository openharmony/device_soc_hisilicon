/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#define SECUREC_FORMAT_OUTPUT_INPUT 1
#ifdef SECUREC_FOR_WCHAR
#undef SECUREC_FOR_WCHAR
#endif

#include "secinput.h"

#include "input.inl"

SECUREC_INLINE int SecIsDigit(SecInt ch)
{
    /* SecInt to unsigned char clear  571, use bit mask to clear negative return of ch */
    return isdigit((int)((unsigned int)(unsigned char)(ch) & 0xffU));
}
SECUREC_INLINE int SecIsXdigit(SecInt ch)
{
    return isxdigit((int)((unsigned int)(unsigned char)(ch) & 0xffU));
}
SECUREC_INLINE int SecIsSpace(SecInt ch)
{
    return isspace((int)((unsigned int)(unsigned char)(ch) & 0xffU));
}

