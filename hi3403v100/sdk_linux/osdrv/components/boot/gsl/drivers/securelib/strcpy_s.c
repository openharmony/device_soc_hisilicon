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

#include "securecutil.h"

#ifndef SECUREC_STRCPY_WITH_PERFORMANCE
#define SECUREC_STRCPY_WITH_PERFORMANCE 1
#endif

#define SECUREC_STRCPY_PARAM_OK(strDest, destMax, strSrc) ((destMax) > 0 && \
    (destMax) <= SECUREC_STRING_MAX_LEN && (strDest) != NULL && (strSrc) != NULL && (strDest) != (strSrc))

#if (!SECUREC_IN_KERNEL) && SECUREC_STRCPY_WITH_PERFORMANCE
#ifndef SECUREC_STRCOPY_THRESHOLD_SIZE
#define SECUREC_STRCOPY_THRESHOLD_SIZE   32UL
#endif
/* The purpose of converting to void is to clean up the alarm */
static inline void SecSmallStrCopy(char *strDest, const char *strSrc, size_t lenWithTerm)
{
    while (lenWithTerm > 0) {
        *strDest++ = *strSrc++;
        --lenWithTerm;
    }
}
#define SECUREC_SMALL_STR_COPY(strDest, strSrc, lenWithTerm) SecSmallStrCopy((strDest), (strSrc), (lenWithTerm))
#endif

#if SECUREC_IN_KERNEL || (!SECUREC_STRCPY_WITH_PERFORMANCE)
#define SECUREC_STRCPY_OPT(dest, src, lenWithTerm) SECUREC_MEMCPY_WARP_OPT((dest), (src), (lenWithTerm))
#else
/*
 * Performance optimization. lenWithTerm  include '\0'
 */
#define SECUREC_STRCPY_OPT(dest, src, lenWithTerm) do { \
    if ((lenWithTerm) > SECUREC_STRCOPY_THRESHOLD_SIZE) { \
        SECUREC_MEMCPY_WARP_OPT((dest), (src), (lenWithTerm)); \
    } else { \
        SECUREC_SMALL_STR_COPY((dest), (src), (lenWithTerm)); \
    } \
} SECUREC_WHILE_ZERO
#endif

/*
 * Check Src Range
 */
SECUREC_INLINE errno_t CheckSrcRange(char *strDest, size_t destMax, const char *strSrc)
{
    size_t tmpDestMax = destMax;
    const char *tmpSrc = strSrc;
    /* Use destMax as boundary checker and destMax must be greater than zero */
    while (*tmpSrc != '\0' && tmpDestMax > 0) {
        ++tmpSrc;
        --tmpDestMax;
    }
    if (tmpDestMax == 0) {
        strDest[0] = '\0';
        SECUREC_ERROR_INVALID_RANGE("strcpy_s");
        return ERANGE_AND_RESET;
    }
    return EOK;
}

/*
 * Handling errors
 */
errno_t strcpy_error(char *strDest, size_t destMax, const char *strSrc)
{
    if (destMax == 0 || destMax > SECUREC_STRING_MAX_LEN) {
        SECUREC_ERROR_INVALID_RANGE("strcpy_s");
        return ERANGE;
    }
    if (strDest == NULL || strSrc == NULL) {
        SECUREC_ERROR_INVALID_PARAMTER("strcpy_s");
        if (strDest != NULL) {
            strDest[0] = '\0';
            return EINVAL_AND_RESET;
        }
        return EINVAL;
    }
    return CheckSrcRange(strDest, destMax, strSrc);
}

/*
 * <FUNCTION DESCRIPTION>
 *    The strcpy_s function copies the string pointed to  strSrc
 *          (including the terminating null character) into the array pointed to by strDest
 *    The destination string must be large enough to hold the source string,
 *    including the terminating null character. strcpy_s will return EOVERLAP_AND_RESET
 *    if the source and destination strings overlap.
 *
 * <INPUT PARAMETERS>
 *    strDest                          Location of destination string buffer
 *    destMax                        Size of the destination string buffer.
 *    strSrc                            Null-terminated source string buffer.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                         is updated.
 *
 * <RETURN VALUE>
 *    EOK                               Success
 *    EINVAL                          strDest is  NULL and destMax != 0 and destMax <= SECUREC_STRING_MAX_LEN
 *    EINVAL_AND_RESET       strDest !=  NULL and strSrc is NULL and destMax != 0 and destMax <= SECUREC_STRING_MAX_LEN
 *    ERANGE                         destMax is 0 and destMax > SECUREC_STRING_MAX_LEN
 *    ERANGE_AND_RESET      strDest have not enough space  and all other parameters are valid  and not overlap
 *    EOVERLAP_AND_RESET   dest buffer and source buffer are overlapped and all  parameters are valid
 *
 *    If there is a runtime-constraint violation, strDest[0] will be set to the '\0' when strDest and destMax valid
 */
errno_t strcpy_s(char *strDest, size_t destMax, const char *strSrc)
{
    if (SECUREC_STRCPY_PARAM_OK(strDest, destMax, strSrc)) {
        size_t srcStrLen;
        SECUREC_CALC_STR_LEN(strSrc, destMax, &srcStrLen);
        ++srcStrLen; /* The length include '\0' */

        if (srcStrLen <= destMax) {
            /* Use mem overlap check include '\0' */
            if (SECUREC_MEMORY_NO_OVERLAP(strDest, strSrc, srcStrLen)) {
                /* Performance optimization srcStrLen include '\0' */
                SECUREC_STRCPY_OPT(strDest, strSrc, srcStrLen);
                return EOK;
            } else {
                strDest[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("strcpy_s");
                return EOVERLAP_AND_RESET;
            }
        }
    }
    return strcpy_error(strDest, destMax, strSrc);
}

#if SECUREC_EXPORT_KERNEL_SYMBOL
EXPORT_SYMBOL(strcpy_s);
#endif

