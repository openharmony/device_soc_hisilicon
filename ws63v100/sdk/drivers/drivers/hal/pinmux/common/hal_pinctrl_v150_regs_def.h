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
 * Description: Provides V150 pinctrl register \n
 *
 * History: \n
 * 2022-09-09, Create file. \n
 */
#ifndef HAL_PINCTRL_V150_REG_DEF_H
#define HAL_PINCTRL_V150_REG_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_pinctrl_v150_reg_def Pinctrl V150 Regs Definition
 * @ingroup  drivers_hal_pinctrl
 * @{
 */

typedef struct pinctrl_config {
    uint16_t mode       : 4;    /*!< Pinmux mode. */
    uint16_t ds         : 4;    /*!< Drive strength. */
    uint16_t pull       : 2;    /*!< Pull down/up. */
    uint16_t reserved   : 6;    /*!< Reserved. */
} pinctrl_config_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif