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
 * Implementing memory data movement
 */
SECUREC_INLINE void SecUtilMemmove(void *dst, const void *src, size_t count)
{
    unsigned char *pDest = (unsigned char *)dst;
    const unsigned char *pSrc = (const unsigned char *)src;
    size_t maxCount = count;

    if (dst <= src || pDest >= (pSrc + maxCount)) {
        /*
         * Non-Overlapping Buffers
         * Copy from lower addresses to higher addresses
         */
        while (maxCount > 0) {
            --maxCount;
            *pDest = *pSrc;
            ++pDest;
            ++pSrc;
        }
    } else {
        /*
         * Overlapping Buffers
         * Copy from higher addresses to lower addresses
         */
        pDest = pDest + maxCount - 1;
        pSrc = pSrc + maxCount - 1;
        while (maxCount > 0) {
            --maxCount;
            *pDest = *pSrc;
            --pDest;
            --pSrc;
        }
    }
}

/*
 * <FUNCTION DESCRIPTION>
 *    The memmove_s function copies count bytes of characters from src to dest.
 *    This function can be assigned correctly when memory overlaps.
 * <INPUT PARAMETERS>
 *    dest                    Destination object.
 *    destMax                 Size of the destination buffer.
 *    src                     Source object.
 *    count                   Number of characters to copy.
 *
 * <OUTPUT PARAMETERS>
 *    dest buffer             is updated.
 *
 * <RETURN VALUE>
 *    EOK                     Success
 *    EINVAL                  dest is  NULL and destMax != 0 and destMax <= SECUREC_MEM_MAX_LEN
 *    EINVAL_AND_RESET        dest != NULL and src is NULL and destMax != 0 and destMax <= SECUREC_MEM_MAX_LEN
 *    ERANGE                  destMax > SECUREC_MEM_MAX_LEN or destMax is 0
 *    ERANGE_AND_RESET        count > destMax and dest  !=  NULL and src != NULL and destMax != 0
 *                            and destMax <= SECUREC_MEM_MAX_LEN
 *
 *    If an error occurred, dest will  be filled with 0 when dest and destMax valid.
 *    If some regions of the source area and the destination overlap, memmove_s
 *    ensures that the original source bytes in the overlapping region are copied
 *    before being overwritten.
 */
errno_t memmove_s(void *dest, size_t destMax, const void *src, size_t count)
{
    if (destMax == 0 || destMax > SECUREC_MEM_MAX_LEN) {
        SECUREC_ERROR_INVALID_RANGE("memmove_s");
        return ERANGE;
    }
    if (dest == NULL || src == NULL) {
        SECUREC_ERROR_INVALID_PARAMTER("memmove_s");
        if (dest != NULL) {
            (void)SECUREC_MEMSET_FUNC_OPT(dest, 0, destMax);
            return EINVAL_AND_RESET;
        }
        return EINVAL;
    }
    if (count > destMax) {
        (void)SECUREC_MEMSET_FUNC_OPT(dest, 0, destMax);
        SECUREC_ERROR_INVALID_RANGE("memmove_s");
        return ERANGE_AND_RESET;
    }
    if (dest == src) {
        return EOK;
    }

    if (count > 0) {
        SecUtilMemmove(dest, src, count);
    }
    return EOK;
}

#if SECUREC_EXPORT_KERNEL_SYMBOL
EXPORT_SYMBOL(memmove_s);
#endif

