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
 * Description: Provides HAL SFC \n
 *
 * History: \n
 * 2022-12-01, Create file. \n
 */
#include "common_def.h"
#include "hal_sfc.h"

uintptr_t g_sfc_global_conf_regs = NULL;
uintptr_t g_sfc_bus_regs = NULL;
uintptr_t g_sfc_bus_dma_regs = NULL;
uintptr_t g_sfc_cmd_regs = NULL;
uintptr_t g_sfc_cmd_databuf = NULL;

errcode_t hal_sfc_regs_init(void)
{
    g_sfc_global_conf_regs = sfc_port_get_sfc_global_conf_base_addr();
    g_sfc_bus_regs = sfc_port_get_sfc_bus_regs_base_addr();
    g_sfc_bus_dma_regs = sfc_port_get_sfc_bus_dma_regs_base_addr();
    g_sfc_cmd_regs = sfc_port_get_sfc_cmd_regs_base_addr();
    g_sfc_cmd_databuf = sfc_port_get_sfc_cmd_databuf_base_addr();
    if ((g_sfc_global_conf_regs == NULL) || (g_sfc_bus_regs == NULL) ||
        (g_sfc_bus_dma_regs == NULL) || (g_sfc_cmd_regs == NULL) ||
        (g_sfc_cmd_databuf == NULL)) {
        return ERRCODE_SFC_REG_ADDR_INVALID;
    }

    return ERRCODE_SUCC;
}

void hal_sfc_regs_deinit(void)
{
    g_sfc_global_conf_regs = NULL;
    g_sfc_bus_regs = NULL;
    g_sfc_bus_dma_regs = NULL;
    g_sfc_cmd_regs = NULL;
    g_sfc_cmd_databuf = NULL;
}