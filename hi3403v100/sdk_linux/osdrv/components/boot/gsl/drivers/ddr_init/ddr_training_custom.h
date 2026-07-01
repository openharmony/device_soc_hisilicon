/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 */

#ifndef DDR_TRAINING_CUSTOM_H
#define DDR_TRAINING_CUSTOM_H

/* config DDRC, PHY, DDRT typte */
#define DDR_DDRC_V520_S14_CONFIG
#define DDR_PHY_S14_CONFIG
#define DDR_DDRT_V2_0_SHF0_CONFIG

/* config special item */
/* s40/t28/t16 not support dcc training */
#define DDR_CHANNEL_MAP_PHY0_DMC0_PHY1_DMC2
#define DDR_DCC_TRAINING_CONFIG

#define DDR_WL_TRAINING_DISABLE
#define DDR_GATE_TRAINING_DISABLE
#define DDR_TRAINING_UART_DISABLE

#define DDR_PHY_NUM              2 /* phy number */

#define DDR_DMC_PER_PHY_MAX      2 /* dmc number per phy max */

/* config DDRC, PHY, DDRT base address */
/* [CUSTOM] DDR PHY0 base register */
#define DDR_REG_BASE_PHY0        0x11150000
/* [CUSTOM] DDR PHY1 base register */
#define DDR_REG_BASE_PHY1        0x11152000
/* [CUSTOM] DDR DMC0 base register */
#define DDR_REG_BASE_DMC0        0x11148000
/* [CUSTOM] DDR DMC1 base register */
#define DDR_REG_BASE_DMC1        0x11149000
/* [CUSTOM] DDR DMC2 base register */
#define DDR_REG_BASE_DMC2        0x1114a000
/* [CUSTOM] DDR DMC3 base register */
#define DDR_REG_BASE_DMC3        0x1114b000
/* [CUSTOM] DDR DDRT base register */
#define DDR_REG_BASE_DDRT        0x11160000
/* [CUSTOM] DDR training item system control */
#define DDR_REG_BASE_SYSCTRL     0x11020000
#define DDR_REG_BASE_AXI         0x11140000
/* Serial Configuration */
#define DDR_REG_BASE_UART0       0x11040000

/* config offset address */
/* Assume sysctrl offset address for DDR training as follows,
if not please define. */
/* [CUSTOM] PHY1 ddrt reversed data */
#define SYSCTRL_DDRT_PATTERN            0xa8
/* [CUSTOM] PHY2 ddrt reversed data */
#define SYSCTRL_DDRT_PATTERN_SEC        0xac
/* [CUSTOM] ddr sw training item */
#define SYSCTRL_DDR_TRAINING_CFG        0xa0 /* RANK0 */
#define SYSCTRL_DDR_TRAINING_CFG_SEC    0xa4 /* RANK1 */
/* [CUSTOM] ddr training stat */
#define SYSCTRL_DDR_TRAINING_STAT       0xb0
/* [CUSTOM] ddr training version flag */
#define SYSCTRL_DDR_TRAINING_VERSION_FLAG    0xb4

/* [CUSTOM] ddr hw training item */
#define SYSCTRL_DDR_HW_PHY0_RANK0       0x90
#define SYSCTRL_DDR_HW_PHY0_RANK1       0x94
#define SYSCTRL_DDR_HW_PHY1_RANK0       0x98
#define SYSCTRL_DDR_HW_PHY1_RANK1       0x9c

/* config other special */
/* [CUSTOM] DDR training start address. MEM_BASE_DDR */
#define DDRT_CFG_BASE_ADDR       0x40000000
/* [CUSTOM] SRAM start address.
NOTE: Makefile will parse it, plase define it as Hex. eg: 0xFFFF0C00 */
#define DDR_TRAINING_RUN_STACK      0x04010c00

#define DDR_RELATE_REG_DECLARE
/* [CUSTOM] ddr training version flag */
#define SYSCTRL_DDR_TRAINING_VERSION_FLAG  0xb4
#define SYSCTRL_DDR_TRAINING_CTX        0xb8
#define SYSCTRL_DDR_TRAINING_PHY        0xbc
#define SYSCTRL_DDR_SIZE    0xc4
struct tr_custom_reg {
    unsigned int ddrt_clk_reg;
    unsigned int age_compst_en[DDR_PHY_NUM];
    unsigned int trfc_en[DDR_PHY_NUM];
    unsigned int ddrt_ctrl;
};

int hal_ddr_boot_cmd_save(struct tr_custom_reg *custom_reg);
void hal_ddr_boot_cmd_restore(const struct tr_custom_reg *custom_reg);
int ddr_boot_cmd_save(struct tr_custom_reg *custom_reg);
void ddr_boot_cmd_restore(const struct tr_custom_reg *custom_reg);
int ddr_get_cksel(void);
#endif /* DDR_TRAINING_CUSTOM_H */
