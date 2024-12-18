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
 * Description: BT HAL CPU CORE MODULE.
 *
 * Create: 2019-12-19
 */
#include "hal_cpu_core.h"
#include "platform_core.h"
#include "chip_io.h"

#define CMU_CLOCK_SOFT_RST2_REG                     (GLB_CTL_M_RB_BASE + 0xB8)
#define CMU_CLOCK_SOFT_RST_GLB_B_CRG_BIT            0
#define CMU_CLOCK_SOFT_RST_GLB_B_LGC_BIT            1
#define CMU_CLOCK_SOFT_RST_GLB_B_CPU_BIT            2

bool hal_cpu_is_bt_enabled(void)
{
    if ((readw(CMU_CLOCK_SOFT_RST2_REG) & BIT(CMU_CLOCK_SOFT_RST_GLB_B_CPU_BIT)) != 0) {
        return true;
    } else {
        return false;
    }
}
