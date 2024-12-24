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
 * Description: Provides efuse driver source \n
 *
 * History: \n
 * 2023-3-4, Create file. \n
 */
#include <stdint.h>
#include <stdio.h>
#include "efuse_porting.h"
#include "hal_efuse_v151_reg_op.h"

efuse_regs_t *g_efuse_regs[CONFIG_EFUSE_REGION_NUM];
efuse_boot_done_regs_t *g_efuse_boot_done_regs;
#if defined(CONFIG_EFUSE_SWITCH_EN)
efuse_switch_en_regs_t *g_efuse_switch_en_regs;
#endif

int32_t hal_efuse_regs_init(int32_t i)
{
    if (i >= CONFIG_EFUSE_REGION_NUM) {
        return -1;
    }
    g_efuse_regs[i] = (efuse_regs_t *)g_efuse_base_addr[i];
    g_efuse_boot_done_regs = (efuse_boot_done_regs_t *)g_efuse_boot_done_addr;
#if defined(CONFIG_EFUSE_SWITCH_EN)
    g_efuse_switch_en_regs = (efuse_switch_en_regs_t *)g_efuse_switch_en_addr;
#endif
    return 0;
}

void hal_efuse_regs_deinit(void)
{
    for (uint8_t i = 0; i < CONFIG_EFUSE_REGION_NUM; i++) {
        g_efuse_regs[i] = NULL;
    }
    g_efuse_boot_done_regs = NULL;
#if defined(CONFIG_EFUSE_SWITCH_EN)
    g_efuse_switch_en_regs = NULL;
#endif
}
