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

#if SECUREC_WITH_PERFORMANCE_ADDONS
#ifndef SECUREC_MEMCOPY_THRESHOLD_SIZE
#define SECUREC_MEMCOPY_THRESHOLD_SIZE 64UL
#endif

static inline void SecSmallMemCopy(void *dest, const void *src, size_t count)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (count > 0) {
        *d++ = *s++;
        --count;
    }
}
#define SECUREC_SMALL_MEM_COPY(dest, src, count) SecSmallMemCopy((dest), (src), (count))

/*
 * Performance optimization
 */
#define SECUREC_MEMCPY_OPT(dest, src, count) do { \
    if ((count) > SECUREC_MEMCOPY_THRESHOLD_SIZE) { \
        SECUREC_MEMCPY_WARP_OPT((dest), (src), (count)); \
    } else { \
        SECUREC_SMALL_MEM_COPY((dest), (src), (count)); \
    } \
} SECUREC_WHILE_ZERO
#endif

/*
 * Handling errors
 */
SECUREC_INLINE errno_t SecMemcpyError(void *dest, size_t destMax, const void *src, size_t count)
{
    if (destMax == 0 || destMax > SECUREC_MEM_MAX_LEN) {
        SECUREC_ERROR_INVALID_RANGE("memcpy_s");
        return ERANGE;
    }
    if (dest == NULL || src == NULL) {
        SECUREC_ERROR_INVALID_PARAMTER("memcpy_s");
        if (dest != NULL) {
            (void)SECUREC_MEMSET_FUNC_OPT(dest, 0, destMax);
            return EINVAL_AND_RESET;
        }
        return EINVAL;
    }
    if (count > destMax) {
        (void)SECUREC_MEMSET_FUNC_OPT(dest, 0, destMax);
        SECUREC_ERROR_INVALID_RANGE("memcpy_s");
        return ERANGE_AND_RESET;
    }
    if (SECUREC_MEMORY_IS_OVERLAP(dest, src, count)) {
        (void)SECUREC_MEMSET_FUNC_OPT(dest, 0, destMax);
        SECUREC_ERROR_BUFFER_OVERLAP("memcpy_s");
        return EOVERLAP_AND_RESET;
    }
    /* Count is 0 or dest equal src also ret EOK */
    return EOK;
}

#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
    /*
     * The fread API in windows will call memcpy_s and pass 0xffffffff to destMax.
     * To avoid the failure of fread, we don't check desMax limit.
     */
#define SECUREC_MEMCPY_PARAM_OK(dest, destMax, src, count) (SECUREC_LIKELY((count) <= (destMax) && \
    (dest) != NULL && (src) != NULL && \
    (count) > 0 && SECUREC_MEMORY_NO_OVERLAP((dest), (src), (count))))
#else
#define SECUREC_MEMCPY_PARAM_OK(dest, destMax, src, count) (SECUREC_LIKELY((count) <= (destMax) && \
    (dest) != NULL && (src) != NULL && (destMax) <= SECUREC_MEM_MAX_LEN && \
    (count) > 0 && SECUREC_MEMORY_NO_OVERLAP((dest), (src), (count))))
#endif

/*
 * <FUNCTION DESCRIPTION>
 *    The memcpy_s function copies n characters from the object pointed to by src into the object pointed to by dest
 *
 * <INPUT PARAMETERS>
 *    dest                      Destination buffer.
 *    destMax                   Size of the destination buffer.
 *    src                       Buffer to copy from.
 *    count                     Number of characters to copy
 *
 * <OUTPUT PARAMETERS>
 *    dest buffer               is updated.
 *
 * <RETURN VALUE>
 *    EOK                      Success
 *    EINVAL                   dest is  NULL and destMax != 0 and destMax <= SECUREC_MEM_MAX_LEN
 *    EINVAL_AND_RESET         dest != NULL and src is NULL and destMax != 0 and destMax <= SECUREC_MEM_MAX_LEN
 *    ERANGE                   destMax > SECUREC_MEM_MAX_LEN or destMax is 0
 *    ERANGE_AND_RESET         count > destMax and destMax != 0 and destMax <= SECUREC_MEM_MAX_LEN
 *                             and dest  !=  NULL  and src != NULL
 *    EOVERLAP_AND_RESET       dest buffer and source buffer are overlapped and
 *                             count <= destMax destMax != 0 and destMax <= SECUREC_MEM_MAX_LEN and dest  !=  NULL
 *                             and src != NULL  and dest != src
 *
 *    if an error occurred, dest will be filled with 0.
 *    If the source and destination overlap, the behavior of memcpy_s is undefined.
 *    Use memmove_s to handle overlapping regions.
 */
errno_t memcpy_s(void *dest, size_t destMax, const void *src, size_t count)
{
    if (SECUREC_MEMCPY_PARAM_OK(dest, destMax, src, count)) {
        SECUREC_MEMCPY_WARP_OPT(dest, src, count);
        return EOK;
    }
    /* Meet some runtime violation, return error code */
    return SecMemcpyError(dest, destMax, src, count);
}

#if SECUREC_EXPORT_KERNEL_SYMBOL
EXPORT_SYMBOL(memcpy_s);
#endif

#if SECUREC_WITH_PERFORMANCE_ADDONS
/*
 * Performance optimization
 */
errno_t memcpy_sOptAsm(void *dest, size_t destMax, const void *src, size_t count)
{
    if (SECUREC_MEMCPY_PARAM_OK(dest, destMax, src, count)) {
        SECUREC_MEMCPY_OPT(dest, src, count);
        return EOK;
    }
    /* Meet some runtime violation, return error code */
    return SecMemcpyError(dest, destMax, src, count);
}

/* Trim judgement on "destMax <= SECUREC_MEM_MAX_LEN" */
errno_t memcpy_sOptTc(void *dest, size_t destMax, const void *src, size_t count)
{
    if (SECUREC_LIKELY(count <= destMax && dest != NULL && src != NULL && \
                       count > 0 && SECUREC_MEMORY_NO_OVERLAP((dest), (src), (count)))) {
        SECUREC_MEMCPY_OPT(dest, src, count);
        return EOK;
    }
    /* Meet some runtime violation, return error code */
    return SecMemcpyError(dest, destMax, src, count);
}
#endif

