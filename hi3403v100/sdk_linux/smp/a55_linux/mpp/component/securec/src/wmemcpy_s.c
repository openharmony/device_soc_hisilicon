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
/*
 * [Standardize-exceptions] Use unsafe function: Portability
 * [reason] Use unsafe function to implement security function to maintain platform compatibility.
 *          And sufficient input validation is performed before calling
 */

#include "securecutil.h"

/*
 * <FUNCTION DESCRIPTION>
 *   The wmemcpy_s function copies n successive wide characters
 *    from the object pointed to by src into the object pointed to by dest.t.
 *
 * <INPUT PARAMETERS>
 *    dest                   Destination buffer.
 *    destMax                Size of the destination buffer.
 *    src                     Buffer to copy from.
 *    count                  Number of characters to copy.
 *
 * <OUTPUT PARAMETERS>
 *    dest buffer            is updated.
 *
 * <RETURN VALUE>
 *    EOK                    Success
 *    EINVAL                 dest is  NULL  and destMax != 0 and count <= destMax
 *                           and destMax <= SECUREC_WCHAR_MEM_MAX_LEN
 *    EINVAL_AND_RESET       dest != NULL and src is NULL and destMax != 0
 *                           and destMax <= SECUREC_WCHAR_MEM_MAX_LEN and count <= destMax
 *    ERANGE                 destMax > SECUREC_WCHAR_MEM_MAX_LEN or destMax is 0 or
 *                           (count > destMax and dest is  NULL and destMax != 0
 *                            and destMax <= SECUREC_WCHAR_MEM_MAX_LEN)
 *    ERANGE_AND_RESET       count > destMax and dest  !=  NULL and destMax != 0
 *                           and destMax <= SECUREC_WCHAR_MEM_MAX_LEN
 *    EOVERLAP_AND_RESET     dest buffer and source buffer are overlapped and
 *                           count <= destMax destMax != 0 and destMax <= SECUREC_WCHAR_MEM_MAX_LEN
 *                           and dest  !=  NULL  and src != NULL and dest != src
 *
 *    if an error occurred, dest will be filled with 0 when dest and destMax valid .
 *    If the source and destination overlap, the behavior of wmemcpy_s is undefined.
 *    Use wmemmove_s to handle overlapping regions.
 */
errno_t wmemcpy_s(wchar_t *dest, size_t destMax, const wchar_t *src, size_t count)
{
    if (destMax == 0 || destMax > SECUREC_WCHAR_MEM_MAX_LEN) {
        SECUREC_ERROR_INVALID_PARAMTER("wmemcpy_s");
        return ERANGE;
    }
    if (count > destMax) {
        SECUREC_ERROR_INVALID_PARAMTER("wmemcpy_s");
        if (dest != NULL) {
            (void)SECUREC_MEMSET_FUNC_OPT(dest, 0, destMax * sizeof(wchar_t));
            return ERANGE_AND_RESET;
        }
        return ERANGE;
    }
    return memcpy_s(dest, destMax * sizeof(wchar_t), src, count * sizeof(wchar_t));
}

