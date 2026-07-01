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
 *    The fwscanf_s function  is  the  wide-character  equivalent  of the fscanf_s function
 *    The fwscanf_s function reads data from the current position of stream into
 *    the locations given by argument (if any). Each argument must be a pointer
 *    to a variable of a type that corresponds to a type specifier in format.
 *    format controls the interpretation of the input fields and has the same
 *    form and function as the format argument for scanf.
 *
 * <INPUT PARAMETERS>
 *    stream                   Pointer to FILE structure.
 *    format                   Format control string, see Format Specifications.
 *    ...                      Optional arguments.
 *
 * <OUTPUT PARAMETERS>
 *    ...                      The converted value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Each of these functions returns the number of fields successfully converted
 *    and assigned; the return value does not include fields that were read but
 *    not assigned. A return value of 0 indicates that no fields were assigned.
 *    return -1 if an error occurs.
 */
int fwscanf_s(FILE *stream, const wchar_t *format, ...)
{
    int ret;                    /* If initialization causes  e838 */
    va_list argList;

    va_start(argList, format);
    ret = vfwscanf_s(stream, format, argList);
    va_end(argList);
    (void)argList;              /* To clear e438 last value assigned not used , the compiler will optimize this code */

    return ret;
}

