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

#include "secinput.h"
#if defined(SECUREC_VXWORKS_PLATFORM) && !SECUREC_IN_KERNEL && \
    (!defined(SECUREC_SYSAPI4VXWORKS) && !defined(SECUREC_CTYPE_MACRO_ADAPT))
#include <ctype.h>
#endif

/*
 * <NAME>
 *    vsscanf_s
 *
 *
 * <FUNCTION DESCRIPTION>
 *    The vsscanf_s function is equivalent to sscanf_s, with the variable argument list replaced by argList
 *    The vsscanf_s function reads data from buffer into the location given by
 *    each argument. Every argument must be a pointer to a variable with a type
 *    that corresponds to a type specifier in format. The format argument controls
 *    the interpretation of the input fields and has the same form and function
 *    as the format argument for the scanf function.
 *    If copying takes place between strings that overlap, the behavior is undefined.
 *
 * <INPUT PARAMETERS>
 *    buffer                Stored data
 *    format                Format control string, see Format Specifications.
 *    argList               pointer to list of arguments
 *
 * <OUTPUT PARAMETERS>
 *    argList               the converted value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Each of these functions returns the number of fields successfully converted
 *    and assigned; the return value does not include fields that were read but
 *    not assigned. A return value of 0 indicates that no fields were assigned.
 *    return -1 if an error occurs.
 */
int vsscanf_s(const char *buffer, const char *format, va_list argList)
{
    size_t count;               /* If initialization causes  e838 */
    int retVal;
    SecFileStream fStr;

    /* Validation section */
    if (buffer == NULL || format == NULL) {
        SECUREC_ERROR_INVALID_PARAMTER("vsscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
    count = strlen(buffer);
    if (count == 0 || count > SECUREC_STRING_MAX_LEN) {
        SecClearDestBuf(buffer, format, argList);
        SECUREC_ERROR_INVALID_PARAMTER("vsscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
#if defined(SECUREC_VXWORKS_PLATFORM) && !SECUREC_IN_KERNEL
    /*
     * On vxworks platform when buffer is white string, will set first %s argument to zero.Like following usage:
     * "   \v\f\t\r\n", "%s", str, strSize
     * Do not check all character, just first and last character then consider it is white string
     */
    if (isspace((int)(unsigned char)buffer[0]) != 0 && isspace((int)(unsigned char)buffer[count - 1]) != 0) {
        SecClearDestBuf(buffer, format, argList);
    }
#endif
    SECUREC_FILE_STREAM_FROM_STRING(&fStr, buffer, count);
    retVal = SecInputS(&fStr, format, argList);
    if (retVal < 0) {
        SECUREC_ERROR_INVALID_PARAMTER("vsscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
    return retVal;
}
#if SECUREC_EXPORT_KERNEL_SYMBOL
EXPORT_SYMBOL(vsscanf_s);
#endif

