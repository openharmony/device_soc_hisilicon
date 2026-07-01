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

#define SECUREC_MEMSET_PARAM_OK(dest, destMax, count) (SECUREC_LIKELY((destMax) <= SECUREC_MEM_MAX_LEN && \
    (dest) != NULL && (count) <= (destMax)))

#if SECUREC_WITH_PERFORMANCE_ADDONS

/* Use union to clear strict-aliasing warning */
typedef union {
    SecStrBuf32 buf32;
    SecStrBuf31 buf31;
    SecStrBuf30 buf30;
    SecStrBuf29 buf29;
    SecStrBuf28 buf28;
    SecStrBuf27 buf27;
    SecStrBuf26 buf26;
    SecStrBuf25 buf25;
    SecStrBuf24 buf24;
    SecStrBuf23 buf23;
    SecStrBuf22 buf22;
    SecStrBuf21 buf21;
    SecStrBuf20 buf20;
    SecStrBuf19 buf19;
    SecStrBuf18 buf18;
    SecStrBuf17 buf17;
    SecStrBuf16 buf16;
    SecStrBuf15 buf15;
    SecStrBuf14 buf14;
    SecStrBuf13 buf13;
    SecStrBuf12 buf12;
    SecStrBuf11 buf11;
    SecStrBuf10 buf10;
    SecStrBuf9 buf9;
    SecStrBuf8 buf8;
    SecStrBuf7 buf7;
    SecStrBuf6 buf6;
    SecStrBuf5 buf5;
    SecStrBuf4 buf4;
    SecStrBuf3 buf3;
    SecStrBuf2 buf2;
} SecStrBuf32Union;
/* C standard initializes the first member of the consortium. */
static const SecStrBuf32 g_allZero = {{
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}};
static const SecStrBuf32 g_allFF = {{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
}};

/* Clear conversion warning strict aliasing" */
SECUREC_INLINE const SecStrBuf32Union *SecStrictAliasingCast(const SecStrBuf32 *buf)
{
    return (const SecStrBuf32Union *)buf;
}

#ifndef SECUREC_MEMSET_THRESHOLD_SIZE
#define SECUREC_MEMSET_THRESHOLD_SIZE 32UL
#endif

static inline void SecUnalignedSet(void *dest, int c, size_t count)
{
    unsigned char *pDest = (unsigned char *)dest;
    while (count > 0) {
        *pDest++ = (unsigned char)c;
        --count;
    }
}
#define SECUREC_UNALIGNED_SET(dest, c, count) SecUnalignedSet((dest), (c), (count))

#define SECUREC_SET_VALUE_BY_STRUCT(dest, dataName, n) do { \
    *(SecStrBuf##n *)(dest) = *(const SecStrBuf##n *)(&((SecStrictAliasingCast(&(dataName)))->buf##n)); \
} SECUREC_WHILE_ZERO

static inline void SecAlignedSetOptZeroFF(void *dest, int c, size_t count)
{
    SecUnalignedSet(dest, c, count);
}
#define SECUREC_ALIGNED_SET_OPT_ZERO_FF(dest, c, count) SecAlignedSetOptZeroFF((dest), (c), (count))

#define SECUREC_SMALL_MEM_SET(dest, c, count) do { \
    if (SECUREC_ADDR_ALIGNED_8((dest))) { \
        SECUREC_ALIGNED_SET_OPT_ZERO_FF((dest), (c), (count)); \
    } else { \
        SECUREC_UNALIGNED_SET((dest), (c), (count)); \
    } \
} SECUREC_WHILE_ZERO

/*
 * Performance optimization
 */
#define SECUREC_MEMSET_OPT(dest, c, count) do { \
    if ((count) > SECUREC_MEMSET_THRESHOLD_SIZE) { \
        SECUREC_MEMSET_PREVENT_DSE((dest), (c), (count)); \
    } else { \
        SECUREC_SMALL_MEM_SET((dest), (c), (count)); \
    } \
} SECUREC_WHILE_ZERO
#endif

/*
 * Handling errors
 */
SECUREC_INLINE errno_t SecMemsetError(void *dest, size_t destMax, int c)
{
    /* Check destMax is 0 compatible with _sp macro */
    if (destMax == 0 || destMax > SECUREC_MEM_MAX_LEN) {
        SECUREC_ERROR_INVALID_RANGE("memset_s");
        return ERANGE;
    }
    if (dest == NULL) {
        SECUREC_ERROR_INVALID_PARAMTER("memset_s");
        return EINVAL;
    }
    SECUREC_MEMSET_PREVENT_DSE(dest, c, destMax); /* Set entire buffer to value c */
    SECUREC_ERROR_INVALID_RANGE("memset_s");
    return ERANGE_AND_RESET;
}

/*
 * <FUNCTION DESCRIPTION>
 *    The memset_s function copies the value of c (converted to an unsigned char)
 *     into each of the first count characters of the object pointed to by dest.
 *
 * <INPUT PARAMETERS>
 *    dest                Pointer to destination.
 *    destMax             The size of the buffer.
 *    c                   Character to set.
 *    count               Number of characters.
 *
 * <OUTPUT PARAMETERS>
 *    dest buffer         is updated.
 *
 * <RETURN VALUE>
 *    EOK                 Success
 *    EINVAL              dest == NULL and destMax != 0 and destMax <= SECUREC_MEM_MAX_LEN
 *    ERANGE              destMax > SECUREC_MEM_MAX_LEN or (destMax is 0 and count > destMax)
 *    ERANGE_AND_RESET    count > destMax and destMax != 0 and destMax <= SECUREC_MEM_MAX_LEN and dest != NULL
 *
 *    if return ERANGE_AND_RESET then fill dest to c ,fill length is destMax
 */
errno_t memset_s(void *dest, size_t destMax, int c, size_t count)
{
    if (SECUREC_MEMSET_PARAM_OK(dest, destMax, count)) {
        SECUREC_MEMSET_PREVENT_DSE(dest, c, count);
        return EOK;
    }
    /* Meet some runtime violation, return error code */
    return SecMemsetError(dest, destMax, c);
}

#if SECUREC_EXPORT_KERNEL_SYMBOL
EXPORT_SYMBOL(memset_s);
#endif

#if SECUREC_WITH_PERFORMANCE_ADDONS
/*
 * Performance optimization
 */
errno_t memset_sOptAsm(void *dest, size_t destMax, int c, size_t count)
{
    if (SECUREC_MEMSET_PARAM_OK(dest, destMax, count)) {
        SECUREC_MEMSET_OPT(dest, c, count);
        return EOK;
    }
    /* Meet some runtime violation, return error code */
    return SecMemsetError(dest, destMax, c);
}

/*
 * Performance optimization, trim judgement on "destMax <= SECUREC_MEM_MAX_LEN"
 */
errno_t memset_sOptTc(void *dest, size_t destMax, int c, size_t count)
{
    if (SECUREC_LIKELY(count <= destMax && dest != NULL)) {
        SECUREC_MEMSET_OPT(dest, c, count);
        return EOK;
    }
    /* Meet some runtime violation, return error code */
    return SecMemsetError(dest, destMax, c);
}
#endif

