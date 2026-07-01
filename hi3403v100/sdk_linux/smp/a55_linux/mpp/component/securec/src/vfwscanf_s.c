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

#ifndef SECUREC_FOR_WCHAR
#define SECUREC_FOR_WCHAR
#endif

#include "secinput.h"

/*
 * <FUNCTION DESCRIPTION>
 *    The  vfwscanf_s  function  is  the  wide-character  equivalent  of the vfscanf_s function
 *    The vfwscanf_s function reads data from the current position of stream into
 *    the locations given by argument (if any). Each argument must be a pointer
 *    to a variable of a type that corresponds to a type specifier in format.
 *    format controls the interpretation of the input fields and has the same form
 *    and function as the format argument for scanf.
 *
 * <INPUT PARAMETERS>
 *    stream               Pointer to FILE structure.
 *    format               Format control string, see Format Specifications.
 *    argList              pointer to list of arguments
 *
 * <OUTPUT PARAMETERS>
 *    argList              the converted value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Each of these functions returns the number of fields successfully converted
 *    and assigned; the return value does not include fields that were read but
 *    not assigned. A return value of 0 indicates that no fields were assigned.
 *    return -1 if an error occurs.
 */
int vfwscanf_s(FILE *stream, const wchar_t *format, va_list argList)
{
    int retVal; /* If initialization causes  e838 */
    SecFileStream fStr;

    if (stream == NULL || format == NULL) {
        SECUREC_ERROR_INVALID_PARAMTER("vfwscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
    if (stream == SECUREC_STREAM_STDIN) {
        return vwscanf_s(format, argList);
    }

    SECUREC_LOCK_FILE(stream);
    SECUREC_FILE_STREAM_FROM_FILE(&fStr, stream);
    retVal = SecInputSW(&fStr, format, argList);
    SECUREC_UNLOCK_FILE(stream);
    if (retVal < 0) {
        SECUREC_ERROR_INVALID_PARAMTER("vfwscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
    return retVal;
}

