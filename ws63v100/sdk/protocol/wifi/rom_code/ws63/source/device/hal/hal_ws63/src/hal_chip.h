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
 * Description: Header file for hal_chip.c.
 */

#ifndef HAL_CHIP_H
#define HAL_CHIP_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hal_chip_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  宏定义
*****************************************************************************/
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_CHIP_H

/*****************************************************************************
   函数声明
*****************************************************************************/
osal_u32 hal_chip_init(osal_void);
osal_void hal_chip_exit(osal_void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_chip.h */
