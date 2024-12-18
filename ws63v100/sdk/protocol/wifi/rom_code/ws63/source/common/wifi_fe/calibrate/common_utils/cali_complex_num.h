/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: 校准复数运算相关
 */

#ifndef __CALI_COMPLEX_NUM_H__
#define __CALI_COMPLEX_NUM_H__

#include "cali_outer_os_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define CALI_CORDIC_ANGLE_ERROR_CODE 0xFFFF
typedef struct {
    osal_s32 real;
    osal_s32 imag;
} cali_complex_stru;

cali_complex_stru cali_complex_minus(cali_complex_stru a, cali_complex_stru b);
osal_s32 cali_div_round_closest(osal_s32 a, osal_s32 b);
cali_complex_stru cali_complex_negate(cali_complex_stru a);
cali_complex_stru cali_complex_add(cali_complex_stru a, cali_complex_stru b);
cali_complex_stru cali_complex_conj(cali_complex_stru a);
cali_complex_stru cali_complex_multiply(cali_complex_stru a, cali_complex_stru b);
cali_complex_stru cali_complex_divide(cali_complex_stru complex_num, cali_complex_stru complex_denom);
cali_complex_stru cali_complex_div_with_fix_bit(cali_complex_stru complex_num,
    cali_complex_stru complex_denom, osal_u8 fix_bit);
cali_complex_stru cali_complex_multiply_minus(cali_complex_stru a, cali_complex_stru b,
    cali_complex_stru c, cali_complex_stru d);
osal_void cali_print_complex_data(const char *name, cali_complex_stru value);
osal_s32 cali_right_shift_round_closest(osal_s32 num, osal_u8 bits);
cali_complex_stru cali_complex_shift_left(cali_complex_stru complex_num, osal_u8 bits);
cali_complex_stru cali_complex_shift_right(cali_complex_stru complex_num, osal_u8 bits);
osal_s32 cali_div_floor(osal_s32 a, osal_s32 b);
osal_s32 cali_cordic_angle(osal_s32 data_i_in, osal_s32 data_q_in, osal_s32 width);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
