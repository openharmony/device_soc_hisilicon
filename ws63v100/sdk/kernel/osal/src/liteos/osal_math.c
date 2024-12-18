/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: math
 *
 * Create: 2021-12-16
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include "soc_osal.h"

/* the result of u64/u32. */
unsigned long long osal_div_u64(unsigned long long dividend, unsigned int divisor)
{
    return div_u64(dividend, divisor);
}

/* the result of s64/s32. */
long long osal_div_s64(long long dividend, int divisor)
{
    return div_s64(dividend, divisor);
}

/* the result of u64/u64. */
unsigned long long osal_div64_u64(unsigned long long dividend, unsigned long long divisor)
{
    return div64_u64(dividend, divisor);
}

/* the result of s64/s64. */
long long osal_div64_s64(long long dividend, long long divisor)
{
    return div64_s64(dividend, divisor);
}

/* the remainder of u64/u32. */
unsigned long long osal_div_u64_rem(unsigned long long dividend, unsigned int divisor)
{
    unsigned int remainder = 0;

    div_u64_rem(dividend, divisor, &remainder);

    return remainder;
}

/* the remainder of s64/s32. */
long long osal_div_s64_rem(long long dividend, int divisor)
{
    int remainder = 0;

    div_s64_rem(dividend, divisor, &remainder);

    return remainder;
}

/* the remainder of u64/u64. */
unsigned long long osal_div64_u64_rem(unsigned long long dividend, unsigned long long divisor)
{
    unsigned long long remainder = 0;

    div64_u64_rem(dividend, divisor, &remainder);
    return remainder;
}

unsigned int osal_get_random_int(void)
{
    return (unsigned int)random();
}
