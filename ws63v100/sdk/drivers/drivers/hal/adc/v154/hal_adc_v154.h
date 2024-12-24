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
 * Description: Provides V154 HAL adc \n
 *
 * History: \n
 * 2024-02-01， Create file. \n
 */
#ifndef HAL_ADC_V154_H
#define HAL_ADC_V154_H

#include "hal_adc.h"
#include "hal_adc_v154_regs_op.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_adc_v154 ADC V154
 * @ingroup  drivers_hal_adc
 * @{
 */

/**
 * @if Eng
 * @brief  ADC type infomation.
 * @else
 * @brief  adc类型信息。
 * @endif
 */
typedef struct {
    adc_channel_t channel;
    hal_adc_channel_type_t channel_type;
    uint32_t scan_fifo_start_addr;
} hal_adc_type_info_t;

/**
 * @if Eng
 * @brief  Get the instance of v154.
 * @return The instance of v154.
 * @else
 * @brief  获取v154实例。
 * @return v154实例。
 * @endif
 */
hal_adc_funcs_t *hal_adc_v154_funcs_get(void);

/**
 * @if Eng
 * @brief  Handler of the adc interrupt request.
 * @else
 * @brief  adc中断处理函数。
 * @endif
 */
void hal_adc_irq_handler(void);

/**
 * @if Eng
 * @brief  ADC get channel config.
 * @retval ADC type infomation. For details, see @ref hal_adc_type_info_t.
 * @else
 * @brief  adc获取通道配置。
 * @retval adc类型信息 参考 @ref hal_adc_type_info_t 。
 * @endif
 */
hal_adc_type_info_t *adc_port_get_cfg(void);

/**
 * @if Eng
 * @brief  ADC get channel config.
 * @param  [in]  ch The adc channel.
 * @param  [in]  on Set or clear of adc channel.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @param  [in]  ch adc通道。
 * @param  [in]  on 设置或清除通道。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败 参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_adc_v154_channel_set(adc_channel_t ch, bool on);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif