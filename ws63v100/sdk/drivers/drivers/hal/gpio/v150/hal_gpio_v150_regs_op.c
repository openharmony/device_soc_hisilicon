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
 * Description: Provides V150 gpio register operation api \n
 *
 * History: \n
 * 2022-11-27, Create file. \n
 */

#include <stdint.h>
#include "common_def.h"
#include "hal_gpio_v150_regs_op.h"

void hal_gpio_v150_intr_rebase(uint32_t channel, uint32_t group)
{
    hal_gpio_gpio_int_en_disable_all(channel, group);
    hal_gpio_gpio_int_mask_mask_all(channel, group);
    hal_gpio_gpio_int_eoi_clr_all(channel, group);
}