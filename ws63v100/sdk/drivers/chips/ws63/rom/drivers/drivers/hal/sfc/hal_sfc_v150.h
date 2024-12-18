/**
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
 *
 * Description: Provides v150 HAL SFC \n
 *
 * History: \n
 * 2022-12-01, Create file. \n
 */
#ifndef HAL_SFC_V150_H
#define HAL_SFC_V150_H

#include "hal_sfc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_sfc_v150 SFC v150
 * @ingroup  drivers_hal_sfc
 * @{
 */

/**
 * @if Eng
 * @brief  Get interface of SFC driver and SFC hal.
 * @else
 * @brief  获取Driver层SFC和HAL层SFC的接口
 * @endif
 */
hal_sfc_funcs_t *hal_sfc_v150_funcs_get(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif