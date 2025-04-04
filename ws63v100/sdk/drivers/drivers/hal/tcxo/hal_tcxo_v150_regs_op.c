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
 * Description: Provides V150 tcxo register operation api \n
 *
 * History: \n
 * 2022-08-16， Create file. \n
 */

#include <stdint.h>
#include "common_def.h"
#include "hal_tcxo_v150_regs_op.h"

tcxo_regs_t *g_tcxo_regs = NULL;

errcode_t hal_tcxo_v150_regs_init(void)
{
    if (tcxo_porting_base_addr_get() == 0) {
        return ERRCODE_TCXO_REG_ADDR_INVALID;
    }

    g_tcxo_regs = (tcxo_regs_t *)tcxo_porting_base_addr_get();

    return ERRCODE_SUCC;
}

void hal_tcxo_v150_regs_deinit(void)
{
    g_tcxo_regs = NULL;
}

uint64_t hal_tcxo_reg_count_get(void)
{
    tcxo_count_data_t tmp_count;
    uint64_t count = 0;

#ifdef CONFIG_TCXO_WITH_TWO_DATA_REGS
    tmp_count.d32 = g_tcxo_regs->count1;
    count |= (uint64_t)tmp_count.d32 << 32U;

    tmp_count.d32 = g_tcxo_regs->count0;
    count |= (uint64_t)tmp_count.d32;
#else
    tmp_count.d32 = g_tcxo_regs->count3;
    count = (uint64_t)tmp_count.b.data << 48U;

    tmp_count.d32 = g_tcxo_regs->count2;
    count |= (uint64_t)tmp_count.b.data << 32U;

    tmp_count.d32 = g_tcxo_regs->count1;
    count |= (uint64_t)tmp_count.b.data << 16U;

    tmp_count.d32 = g_tcxo_regs->count0;
    count |= (uint64_t)tmp_count.b.data;
#endif

    return count;
}
