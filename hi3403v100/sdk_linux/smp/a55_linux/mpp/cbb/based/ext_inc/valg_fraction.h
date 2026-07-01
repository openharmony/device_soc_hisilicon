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

#ifndef VALG_FRACTION_H
#define VALG_FRACTION_H

#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* the minimum fraction can be represented is 1/64 */
#define DENOMINATOR_MAX 64
#define FRACTION_MIN    FRACTION32(DENOMINATOR_MAX, 1)

/* return 1 if x is fraction 32 */
#define is_fraction32(x) (DENOMINATOR32(x) != 0)

/* integer into fraction.
 * note: i must <= 65535.
 * it means integer when denominator is 0. */
#define int_to_fraction32(i) \
    ((DENOMINATOR32(i) == 0) ? (FRACTION32(1, i)) : (i))

    /* fraction into integer. */
#define fraction32_to_int(f1) \
        (NUMERATOR32(f1) / DENOMINATOR32(f1))
     /* f1+f2. f1,f2 is fraction.
     * return value is fraction. */
#define fraction32_addition(f1, f2) \
        FRACTION32(DENOMINATOR32( \
            f1) * DENOMINATOR32(f2), NUMERATOR32(f1) * DENOMINATOR32(f2) + NUMERATOR32(f2) * DENOMINATOR32(f1))
     /* f1-f2. f1,f2 is fraction
     * return value is fraction. */
#define fraction32_subtract(f1, f2) \
        FRACTION32(DENOMINATOR32( \
            f1) * DENOMINATOR32(f2), NUMERATOR32(f1) * DENOMINATOR32(f2) - NUMERATOR32(f2) * DENOMINATOR32(f1))
     /* f1*f2. f1,f2 is fraction
     * return value is fraction. */
#define fraction32_multiply(f1, f2) \
        FRACTION32(DENOMINATOR32(f1) * DENOMINATOR32(f2), NUMERATOR32(f1) * NUMERATOR32(f2))
     /* f1/f2. f1,f2 is fraction.
     * return value is fraction. */
#define fraction32_division(f1, f2) \
        FRACTION32(DENOMINATOR32(f1) * NUMERATOR32(f2), NUMERATOR32(f1) * DENOMINATOR32(f2))

#define fraction32_compare(f1, f2) \
    CMP(NUMERATOR32(f1) * DENOMINATOR32(f2), NUMERATOR32(f2) * DENOMINATOR32(f1))

static __inline td_s32 fraction32_add(td_fr32 x, td_fr32 y)
{
    td_s32 sum;
    td_s32 f1;
    td_s32 f2;

    f1 = int_to_fraction32(x);
    f2 = int_to_fraction32(y);
    sum = (NUMERATOR32(f1) * DENOMINATOR32(f2) + DENOMINATOR32(f1) * NUMERATOR32(f2))
          / (DENOMINATOR32(f1) * DENOMINATOR32(f2));
    return sum;
}

/* x-y. x,y is unsigned int or fraction.
 * return value is int */
static __inline td_s32 fraction32_sub(td_fr32 x, td_fr32 y)
{
    td_s32 diff;
    td_s32 f1;
    td_s32 f2;

    f1 = int_to_fraction32(x);
    f2 = int_to_fraction32(y);
    diff = (NUMERATOR32(f1) * DENOMINATOR32(f2) - NUMERATOR32(f2) * DENOMINATOR32(f1))
           / (DENOMINATOR32(f1) * DENOMINATOR32(f2));
    return diff;
}

/* x*y. x,y is unsigned int or fraction.
 * return value is int
 */
static __inline td_s32 fraction32_mul(td_fr32 x, td_fr32 y)
{
    td_s32 product;
    td_s32 f1;
    td_s32 f2;

    f1 = int_to_fraction32(x);
    f2 = int_to_fraction32(y);
    product = (NUMERATOR32(f1) * NUMERATOR32(f2))
              / (DENOMINATOR32(f1) * DENOMINATOR32(f2));
    return product;
}

/* x/y. x,y is unsigned int or fraction.
 * return value is int. */
static __inline td_s32 fraction32_div(td_fr32 x, td_fr32 y)
{
    td_s32 quotient;
    td_s32 f1;
    td_s32 f2;

    f1 = int_to_fraction32(x);
    f2 = int_to_fraction32(y);
    quotient = (NUMERATOR32(f1) * DENOMINATOR32(f2))
               / (DENOMINATOR32(f1) * NUMERATOR32(f2));
    return quotient;
}
static __inline td_s32 fraction32_cmp(td_fr32 x, td_fr32 y)
{
    td_u32 f1;
    td_u32 f2;

    f1 = int_to_fraction32(x);
    f2 = int_to_fraction32(y);
    return fraction32_compare(f1, f2);
}

static __inline td_s32 int_add_frac32(td_s32 i32, td_fr32 fr32)
{
    td_s32 sum;
    td_s32 f2;

    f2 = int_to_fraction32(fr32);
    sum = i32 + NUMERATOR32(f2) / DENOMINATOR32(f2);
    return sum;
}

static __inline td_s32 int_sub_frac32(td_s32 i32, td_fr32 fr32)
{
    td_s32 diff;
    td_s32 f2;

    f2 = int_to_fraction32(fr32);
    diff = (i32 * DENOMINATOR32(f2) - NUMERATOR32(f2)) / DENOMINATOR32(f2);
    return diff;
}

static __inline td_s32 int_mul_frac32(td_s32 i32, td_fr32 fr32)
{
    td_s32 product;
    td_s32 f2;

    f2 = int_to_fraction32(fr32);
    product = i32 * NUMERATOR32(f2) / DENOMINATOR32(f2);
    return product;
}

static __inline td_s32 int_div_frac32(td_s32 i32, td_fr32 fr32)
{
    td_s32 quotient;
    td_s32 f2;

    f2 = int_to_fraction32(fr32);
    quotient = i32 * DENOMINATOR32(f2) / NUMERATOR32(f2);
    return quotient;
}
static __inline td_s32 int_cmp_frac32(td_s32 i32, td_fr32 fr32)
{
    td_s32 f2;

    f2 = int_to_fraction32(fr32);
    return CMP(i32 * DENOMINATOR32(f2), NUMERATOR32(f2));
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* VALG_FRACTION_H */

