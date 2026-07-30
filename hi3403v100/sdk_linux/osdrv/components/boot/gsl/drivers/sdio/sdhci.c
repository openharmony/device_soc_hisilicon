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

#include "emmc.h"
#include "sdhci.h"
#include "cipher.h"
#include <lib.h>
#include <types.h>
#include <common.h>
#include <platform.h>

#define debug_printf(fmt, args...)

int sdhci_check_int_status(uint32_t mask, uint32_t timeout)
{
    unsigned int reg;
    unsigned int wait_time =  timeout * timer_get_divider();

    timer_start();
	for (;;) {
    reg = sdhci_readl(SDHCI_INT_STATUS);
		if (reg & mask) {
			break;
		}
        if (timer_get_val() > wait_time) {
            debug_printf("wait int status time out, reg = 0x%x, mask = 0x%x\n", reg, mask);
            return -1;
        }
        if (reg & SDHCI_INT_ERROR_MASK) {
            debug_printf("int err: reg = 0x%x\n", reg);
            return -1;
        }
    }

    return 0;
}

static void sdhci_set_boot_width_clock(void)
{
    uint8_t ctrl;
    uint16_t clk;
    unsigned long timeout;
    uint32_t reg;

    /* enable internal clock */
    clk = sdhci_readw(SDHCI_CLOCK_CONTROL);
    clk |= SDHCI_CLOCK_INT_EN;
    sdhci_writew(clk, SDHCI_CLOCK_CONTROL);

    /* Wait internal clock stable for max 20 ms */
    timeout = 20 * timer_get_divider();
    timer_start();
    while (!((clk = sdhci_readw(SDHCI_CLOCK_CONTROL))
         & SDHCI_CLOCK_INT_STABLE)) {
        if (timer_get_val() > timeout) {
            debug_printf("wait host clock stable time out\n");
            break;
        }
    }

    clk |= SDHCI_CLOCK_CARD_EN;
    sdhci_writew(clk, SDHCI_CLOCK_CONTROL);

    udelay(75); /* delay 75us */

    /* set data width */
    ctrl = sdhci_readb(SDHCI_HOST_CONTROL);

    reg = readl(REG_BASE_SCTL + REG_SC_SYSSTAT);
    if (reg & EMMC_BOOT_8BIT) {
        ctrl &= ~SDHCI_CTRL_4BITBUS;
        ctrl |= SDHCI_CTRL_8BITBUS;
    } else {
        ctrl &= ~SDHCI_CTRL_8BITBUS;
        ctrl |= SDHCI_CTRL_4BITBUS;
    }

    sdhci_writeb(ctrl, SDHCI_HOST_CONTROL);

    return;
}

static void sdhci_reset(uint8_t mask)
{
    unsigned long timeout;

    debug_printf("reg SDHCI_PRESENT_STATE = 0x%x\n",
             sdhci_readl(SDHCI_PRESENT_STATE));

    sdhci_writeb(mask, SDHCI_SOFTWARE_RESET);

    /* Wait max 100 ms */
    timeout = 100 * timer_get_divider();

    timer_start();
    /* hw clears the bit when it's done */
    while (sdhci_readb(SDHCI_SOFTWARE_RESET) & mask) {
        if (timer_get_val() > timeout) {
            debug_printf("sdhci: Reset 0x%x never completed.\n",
                     (int)mask);
            return;
        }
    }
}

static int sdhci_controller_init(void)
{
    sdhci_reset(SDHCI_RESET_ALL);
    sdhci_writel(0xFFFFFFFF, SDHCI_INT_STATUS);

    /* enable all interrupt */
    sdhci_writel(0xFFFFFFFF, SDHCI_INT_ENABLE);

    sdhci_writew(SDHCI_CARD_IS_EMMC, SDHCI_EMMC_CTRL);

    sdhci_writeb(SDHCI_POWER_ON | SDHCI_POWER_180, SDHCI_POWER_CONTROL);

    sdhci_set_boot_width_clock();

    udelay(75); /* delay 75us need attention */

    return 0;
}

static uint32_t mmc_clk_sel()
{
    uint32_t val;

    val = reg_get(REG_BASE_SCTL + REG_PERI_EMMC_STAT);
    /* 400k need to be set when normal mode enable*/
    if (val & EMMC_NORMAL_MODE)
        return MMC_CLK_400K;

    /* boot mode */
    switch (mmc_clk_sel_val(val)) {
        case 0x0:
            return MMC_CLK_25M;
        case 0x1:
            return MMC_CLK_400K;
        case 0x2:
            return MMC_CLK_50M;
        default:
            return MMC_CLK_400K;
    }
}

static int sdhci_hardware_init(void)
{
    unsigned int reg;
    unsigned int clk_sel;

    /* enable ck_en */
    reg = readl(REG_CRG_BASE + PERI_CRG_EMMC);
    reg |= (MMC_CLK_EN | MMC_AHB_CLK_EN);
    writel(reg, REG_CRG_BASE + PERI_CRG_EMMC);

    /* mmc_srst_req */
    reg |= MMC_RST;
    writel(reg, REG_CRG_BASE + PERI_CRG_EMMC);

    udelay(25); /* delay 25us */

    /* undo mmc_srst_req */
    reg &= ~MMC_RST;
    writel(reg, REG_CRG_BASE + PERI_CRG_EMMC);
    udelay(1);

    /* clk2card_on = 0 */
    reg = sdhci_readw(SDHCI_CLOCK_CONTROL);
    reg &= ~SDHCI_CLOCK_CARD_EN;
    sdhci_writew(reg, SDHCI_CLOCK_CONTROL);
    udelay(25); /* delay 25us */

    /* card_clk_en = 0 */
    reg = sdhci_readw(SDHCI_CLOCK_CONTROL);
    reg &= ~SDHCI_CLOCK_INT_EN;
    sdhci_writew(reg, SDHCI_CLOCK_CONTROL);

    /* eMMC clk sel */
    reg = readl(REG_CRG_BASE + PERI_CRG_EMMC);
    reg &=  ~MMC_CLK_SEL_MASK;
    clk_sel = mmc_clk_sel();
    reg |= (clk_sel << MMC_CLK_SEL_SHIFT);
    writel(reg, REG_CRG_BASE + PERI_CRG_EMMC);

    /* drv_clk_phase_sel=16 (180 degree) */
    reg = readl(REG_CRG_BASE + PERI_EMMC_DRV_DLL);
    reg &= ~MMC_DRV_PHASE_MASK;
    reg |= (0x10 << MMC_DRV_PHASE_SHIFT);
    writel(reg, REG_CRG_BASE + PERI_EMMC_DRV_DLL);

    /* tuning_cclk_en */
    sdhci_writel(0x10, SDHCI_AT_CTRL);

    /* tuning_cclk_sel=45C or tuning_cclk_sel=0C */
    if (clk_sel == MMC_CLK_50M)
        sdhci_writel(0x4, SDHCI_AT_STAT);
    else
        sdhci_writel(0x0, SDHCI_AT_STAT);

    udelay(5); /* delay 5us */

    return 0;
}

int32_t sdhci_init(void)
{
    int32_t ret;

    ret = sdhci_hardware_init();
    if (ret) {
        debug_printf("sdhci_hardware_init error\n");
        return ret;
    }

    ret = sdhci_controller_init();
    if (ret) {
        debug_printf("sdhci_controller_init error\n");
        return ret;
    }

    return ret;
}

int32_t sdhci_deinit(void)
{
    sdhci_reset(SDHCI_RESET_ALL);
    sdhci_writeb(0, SDHCI_POWER_CONTROL);
    return 0;
}

int32_t sdhci_start_boot(void)
{
    uint16_t mode;
    int ret;

    sdhci_writew(EMMC_BLOCK_SIZE, SDHCI_BLOCK_SIZE);
    sdhci_writew((EMMC_BOOT_PARTITION_SIZE / EMMC_BLOCK_SIZE),
             SDHCI_BLOCK_COUNT);
    sdhci_writel(0xFFFFFFFF, SDHCI_INT_STATUS);
    sdhci_writeb(0xE, SDHCI_TIMEOUT_CONTROL);

    mode = SDHCI_TRNS_BLK_CNT_EN | SDHCI_TRNS_MULTI | SDHCI_TRNS_READ;
    sdhci_writew(mode, SDHCI_TRANSFER_MODE);

    sdhci_writel(0x0, SDHCI_EMMC_HW_RESET);
    udelay(100); /* delay 100us */
    sdhci_writel(0x1, SDHCI_EMMC_HW_RESET);
    udelay(1000); /* delay 1000us */

    sdhci_writew(MAN_BOOT_EN | VALIDATE_BOOT, SDHCI_BOOT_CTRL);

    ret = sdhci_check_int_status(SDHCI_INT_DATA_AVAIL, 2000); /* timeout 2000ms */
    if (ret) {
        debug_info("get boot data error\n", 0);
        return -1;
    }

    debug_printf("boot data start ok\n");
    return 0;
}

int sdhci_read_block_pio(uint32_t data_addr, uint32_t block, size_t read_type)
{
    uint32_t size;
    uint8_t *buf;
    int ret;
    uint32_t spacc_channel_number = 1;
    spacc_decrypt_params decrypt_params = {0};

    size = EMMC_BLOCK_SIZE;
    buf = (uint8_t *)(uintptr_t)(data_addr + EMMC_BLOCK_SIZE * block);

    if (read_type == READ_DATA_BY_CPU) {
        while (size) {
            *(uint32_t *)buf = sdhci_readl(SDHCI_BUFFER);
            buf += 0x4;
            size -= 0x4;
        }
    } else {
        decrypt_params.chn = spacc_channel_number;
        decrypt_params.dst_addr = (uint32_t)(uintptr_t)buf;
        decrypt_params.src_addr = (uint32_t)(REG_BASE_EMMC + SDHCI_DMA_BUF_ADDR);
        decrypt_params.length = EMMC_BLOCK_SIZE;
        decrypt_params.alg = SYMC_ALG_DMA;
        decrypt_params.mode = SYMC_MODE_CBC;
        ret = drv_spacc_decrypt(decrypt_params);
        if (ret != TD_SUCCESS)
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

int32_t sdhci_read_boot_data(uint32_t data_addr, uint32_t read_block, size_t read_type)
{
    uint32_t blocks = 0;
    int ret;

	while (1) {
        ret = sdhci_check_int_status(SDHCI_INT_DATA_AVAIL, 2000); /* timeout 2000ms */
        if (ret) {
            debug_printf("wait data available int time out\n");
            return TD_FAILURE;
        }

        sdhci_writel(SDHCI_INT_DATA_AVAIL, SDHCI_INT_STATUS);
        ret = sdhci_read_block_pio(data_addr, blocks, read_type);
        if (ret != TD_SUCCESS)
            return TD_FAILURE;

        blocks++;
		if (blocks == read_block)
			break;
    }

    return TD_SUCCESS;
}

