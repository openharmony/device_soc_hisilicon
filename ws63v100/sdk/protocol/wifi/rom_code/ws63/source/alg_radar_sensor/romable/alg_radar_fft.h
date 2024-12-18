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
 * Description: radar_sensor_fft.h
 */

#ifndef __ALG_RADAR_FFT_H__
#define __ALG_RADAR_FFT_H__

#include "wlan_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define PI 3.1415926535

typedef struct {
    float real;
    float imag;
} alg_radar_complex;

osal_void alg_radar_fft_complex(alg_radar_complex *x, osal_u32 fft_size, osal_s16 ifft_flag);
osal_void alg_radar_fftshift(alg_radar_complex *x, osal_u32 fft_size);
osal_void alg_radar_complex_abs(alg_radar_complex *x, float *res, osal_u32 sample_len);
float alg_radar_mean(float *x, osal_u32 sample_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif