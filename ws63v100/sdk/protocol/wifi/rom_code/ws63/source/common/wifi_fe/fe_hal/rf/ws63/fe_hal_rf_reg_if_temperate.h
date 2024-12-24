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
 * Description: 温度补偿rf寄存器实现
*/
#ifndef _FE_HAL_RF_REG_IF_TEMPERATE_H_
#define _FE_HAL_RF_REG_IF_TEMPERATE_H_
#include "osal_types.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
enum {
    TEMP_CODE_COMP_BANK_SEL_0 = 0,  // 温度码补偿使用bank0
    TEMP_CODE_COMP_BANK_SEL_1 = 1,  // 温度码补偿使用bank1
    TEMP_CODE_COMP_BANK_SEL_BUTT
};
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // _FE_HAL_RF_REG_IF_TEMPERATE_H_
