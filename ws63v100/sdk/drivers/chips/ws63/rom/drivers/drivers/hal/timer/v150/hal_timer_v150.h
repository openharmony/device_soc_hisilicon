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
 * Description: Provides V150 HAL timer \n
 *
 * History: \n
 * 2022-12-08, Create file. \n
 */
#ifndef HAL_TIMER_V150_H
#define HAL_TIMER_V150_H

#include <stdint.h>
#include "hal_drv_timer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_timer_v150 TIMER V150
 * @ingroup  drivers_hal_timer
 * @{
 */

/**
 * @if Eng
 * @brief  Definition of the timer mode.
 * @else
 * @brief  Timer模式定义。
 * @endif
 */
typedef enum control_reg_mode_v150 {
    /** @if Eng  Timer mode: one shot mode(Same as TIMER_V150_MODE_ONE_SHOT_1).
     *  @else    定时器控制模式：单触发模式（等同于TIMER_V150_MODE_ONE_SHOT_1）。
     *  @endif */
    TIMER_V150_MODE_ONE_SHOT,
    /** @if Eng  Timer mode: periodic mode.
     *  @else    定时器控制模式：周期触发模式。
     *  @endif */
    TIMER_V150_MODE_PERIODIC,
    /** @if Eng  Timer mode: one shot mode(Same as TIMER_V150_MODE_ONE_SHOT).
     *  @else    定时器控制模式：单触发模式（等同于TIMER_V150_MODE_ONE_SHOT）。
     *  @endif */
    TIMER_V150_MODE_ONE_SHOT_1,
    /** @if Eng  Timer mode: free run mode.
     *  @else    定时器控制模式：自由运行模式。
     *  @endif */
    TIMER_V150_MODE_FREERUN
} control_reg_mode_v150_t;

/**
 * @if Eng
 * @brief  Get the interface instance of timer V150.
 * @return The interface instance of @ref hal_timer_funcs_t.
 * @else
 * @brief  获取Timer的接口实例。
 * @return @ref hal_timer_funcs_t 的接口实例。
 * @endif
 */
hal_timer_funcs_t *hal_timer_v150_get_funcs(void);

/**
 * @if Eng
 * @brief  Handler of the timer interrupt request.
 * @param  [in]  index Index of the hardware timer. For detail, see @ref timer_index_t.
 * @else
 * @brief  timer中断处理函数。
 * @param  [in]  index 硬件定时器索引值，参考 @ref timer_index_t 。
 * @endif
 */
void hal_timer_v150_irq_handler(timer_index_t index);

/**
 * @if Eng
 * @brief  clear timer interrupt.
 * @param  [in]  index Index of the hardware timer. For detail, see @ref timer_index_t.
 * @else
 * @brief  清除timer模块内部的中断。
 * @param  [in]  index 硬件定时器索引值，参考 @ref timer_index_t 。
 * @endif
 */
void hal_timer_v150_interrupt_clear(timer_index_t index);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif