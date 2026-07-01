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

#include "securec.h"

/*
 * <FUNCTION DESCRIPTION>
 *   The  swprintf_s  function  is  the  wide-character  equivalent  of the sprintf_s function
 *
 * <INPUT PARAMETERS>
 *    strDest                   Storage location for the output.
 *    destMax                   Maximum number of characters to store.
 *    format                    Format-control string.
 *    ...                        Optional arguments
 *
 * <OUTPUT PARAMETERS>
 *    strDest                    is updated
 *
 * <RETURN VALUE>
 *    return  the number of wide characters stored in strDest, not  counting the terminating null wide character.
 *    return -1  if an error occurred.
 *
 * If there is a runtime-constraint violation, strDest[0] will be set to the '\0' when strDest and destMax valid
 */
int swprintf_s(wchar_t *strDest, size_t destMax, const wchar_t *format, ...)
{
    int ret;                    /* If initialization causes  e838 */
    va_list argList;

    va_start(argList, format);
    ret = vswprintf_s(strDest, destMax, format, argList);
    va_end(argList);
    (void)argList;              /* To clear e438 last value assigned not used , the compiler will optimize this code */

    return ret;
}

