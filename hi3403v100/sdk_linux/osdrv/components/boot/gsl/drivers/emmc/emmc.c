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
#include <platform.h>
#include <common.h>

#define debug_printf(fmt, args...)

static unsigned int g_ocr;

void mmc_pin_mux_config(uint32_t const *pin_mux)
{
    uint32_t peri_val;

    peri_val = reg_get(REG_BASE_SCTL + REG_SC_SYSSTAT);

    reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_CLK, pin_mux[0x0]);
    reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_CMD, pin_mux[0x1]);
    reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DATA0, pin_mux[0x2]);
    reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DATA1, pin_mux[0x3]);
    reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DATA2, pin_mux[0x4]);
    reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DATA3, pin_mux[0x5]);
    if (peri_val & EMMC_BOOT_8BIT) {
        reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DATA4, pin_mux[0x6]);
        reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DATA5, pin_mux[0x7]);
        reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DATA6, pin_mux[0x8]);
        reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DATA7, pin_mux[0x9]);
        reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_DS, pin_mux[0xa]);
    }
    reg_set(REG_HP_IO_CFG_BASE + IO_CFG_EMMC_RST, pin_mux[0xb]);
}

void mmc_muxctrl_config(void)
{
    uint32_t emmc_stat_reg;
    uint32_t const pin_mux_internel[0xc] = {
        0x1142, 0x1342, 0x1342, 0x1342, 0x1342, 0x1342, 0x1342,
        0x1342, 0x1342, 0x1342, 0x1102, 0x1102
    };
    uint32_t const pin_mux[0xc] = {
        0x1042, 0x1042, 0x1042, 0x1042, 0x1042, 0x1042, 0x1042,
        0x1042, 0x1042, 0x1042, 0x1002, 0x1002
    };

    emmc_stat_reg = reg_get(REG_BASE_SCTL + REG_PERI_EMMC_STAT);
    if (emmc_stat_reg & 0x10) {
        /* Use externel pull up/down */
        mmc_pin_mux_config(pin_mux);
    } else {
        /* Use internal pull up/down */
        mmc_pin_mux_config(pin_mux_internel);
    }
}

unsigned int is_bootmode(void)
{
    return !(reg_get(REG_BASE_SCTL + REG_PERI_EMMC_STAT) & EMMC_NORMAL_MODE);
}

unsigned int mmc_get_hcs(void)
{
    return g_ocr & OCR_HCS;
}

static int mmc_send_cmd(uint32_t cmd, uint32_t arg)
{
    sdhci_writel(0xFFFFFFFF, SDHCI_INT_STATUS);
    sdhci_writel(arg, SDHCI_ARGUMENT);
    sdhci_writew(cmd, SDHCI_COMMAND);

    if (sdhci_check_int_status(SDHCI_INT_RESPONSE, 3000)) { /* timeout 3000ms */
        debug_printf("send cmd error\n");
        return -1;
    }

    return 0;
}

static int mmc_card_init(void)
{
    uint32_t cmd;
    uint32_t arg;
    uint32_t timeout;
    uint32_t retry;
    uint32_t reg;
    uint32_t bus_width;
    sdhci_writel(0x0, SDHCI_EMMC_HW_RESET);
    udelay(100); /* delay 100us */
    sdhci_writel(0x1, SDHCI_EMMC_HW_RESET);
    udelay(1000); /* delay 1000us */

    /* Send CMD0 to reset card into idle state */
    cmd = sdhci_make_cmd_fun(MMC_CMD_GO_IDLE_STATE, SDHCI_CMD_RESP_NONE);
    mmc_send_cmd(cmd, 0);
    udelay(2000); /* delay 2000us */

    retry = 3000; /* retry 3000 times */
    while (retry--) {
        /* Send CMD1 for initialization and wait to complete */
        cmd = sdhci_make_cmd_fun(MMC_CMD_SEND_OP_COND,
            SDHCI_CMD_RESP_SHORT);
        arg = MMC_VDD_165_195 | OCR_HCS;
        mmc_send_cmd(cmd, arg);

        g_ocr = sdhci_readl(SDHCI_RESPONSE);
        if (g_ocr & OCR_BUSY) {
            break;
        }
        mdelay(1);
    }

    if (retry == 0) {
    return -1;
    }
    /* Send CMD2 to get card cid numbers */
    cmd = sdhci_make_cmd_fun(MMC_CMD_ALL_SEND_CID, SDHCI_CMD_CRC | SDHCI_CMD_RESP_LONG);
    mmc_send_cmd(cmd, 0);

    /* Send CMD3  to set card default address(0001) */
    cmd = sdhci_make_cmd_fun(MMC_CMD_SET_RELATIVE_ADDR, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    mmc_send_cmd(cmd, 1 << 0x10);

    /* Send CMD7 for card from standby state to transfer state */
    cmd = sdhci_make_cmd_fun(MMC_CMD_SELECT_CARD, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    mmc_send_cmd(cmd, 1 << 0x10);

    /* Get buswidth */
    reg = readl(REG_BASE_SCTL + REG_SC_SYSSTAT);
    bus_width = (reg & EMMC_BOOT_8BIT) ?
        EXT_CSD_BUS_WIDTH_8 : EXT_CSD_BUS_WIDTH_4;

    /* Send CMD6 */
    cmd = sdhci_make_cmd_fun(MMC_CMD_SWITCH,
        SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    arg = (MMC_SWITCH_MODE_WRITE_BYTE << MMC_SWITCH_ACCESS_SHIFT) |
        (EXT_CSD_BUS_WIDTH << MMC_SWITCH_INDEX_SHIFT) |
        (bus_width << MMC_SWITCH_VALUE_SHIFT);
    mmc_send_cmd(cmd, arg);

    timeout = 10000; /* timeout 10000 * 100us*/
    while (!(sdhci_readl(SDHCI_PRESENT_STATE) & SDHCI_PSTATE_DAT_0)) {
        if (timeout-- == 0) {
            debug_info("wait DAT0 ready timeout\n", 0);
            break;
        }
        udelay(100); /* delay 100us */
    }

    return 0;
}

void mmc_change_clock(void)
{
    uint16_t val;
    uint32_t reg;
    unsigned long timeout;

    /* disable card clock */
    val = sdhci_readw(SDHCI_CLOCK_CONTROL);
    val &= ~SDHCI_CLOCK_CARD_EN;
    sdhci_writew(val, SDHCI_CLOCK_CONTROL);

    udelay(25); /* delay 25us */

    /* disable inter clock */
    val = sdhci_readw(SDHCI_CLOCK_CONTROL);
    val &= ~SDHCI_CLOCK_INT_EN;
    sdhci_writew(val, SDHCI_CLOCK_CONTROL);

    /* eMMC clk sel */
    reg = readl(REG_CRG_BASE + PERI_CRG_EMMC);
    reg &= ~MMC_CLK_SEL_MASK;
    reg |= (MMC_CLK_25M << MMC_CLK_SEL_SHIFT);
    writel(reg, REG_CRG_BASE + PERI_CRG_EMMC);

    /* config drv_clk_phase_sel=16 (180 degree) */
    reg = readl(REG_CRG_BASE + PERI_EMMC_DRV_DLL);
    reg &= ~MMC_DRV_PHASE_MASK;
    reg |= (0x10 << MMC_DRV_PHASE_SHIFT);
    writel(reg, REG_CRG_BASE + PERI_EMMC_DRV_DLL);

    /* config tuning cclk */
    sdhci_writeb(0x10, SDHCI_AT_CTRL);

    /* config tuning_cclk_sel=0 (0 degree) */
    sdhci_writeb(0x0, SDHCI_AT_STAT);

    udelay(25); /* delay 25us */

    /* enable internal clock */
    val = sdhci_readw(SDHCI_CLOCK_CONTROL);
    val |= SDHCI_CLOCK_INT_EN;
    sdhci_writew(val, SDHCI_CLOCK_CONTROL);

    /* Wait internal clock stable for max 20 ms */
    timeout = 20 * timer_get_divider();
    timer_start();
    while (!((val = sdhci_readw(SDHCI_CLOCK_CONTROL)) &
         SDHCI_CLOCK_INT_STABLE)) {
        if (timer_get_val() > timeout) {
            debug_info("wait host clock fast stable time out\n", 0);
            break;
        }
    }

    /* enable card clock */
    val |= SDHCI_CLOCK_CARD_EN;
    sdhci_writew(val, SDHCI_CLOCK_CONTROL);

    udelay(75); /* delay 75us */
}

int mmc_init(void)
{
    int ret;
    uint32_t bootmode;

    mmc_muxctrl_config();

    ret = sdhci_init();
    if (ret) {
        return ret;
    }

    bootmode = is_bootmode();
    if (bootmode) {
        /* write 0 to clear ocr information */
        writel(0, REG_BASE_SCTL + REG_SAVE_EMMC_OCR);
        return sdhci_start_boot();
    }

    ret = mmc_card_init();
    if (ret) {
        debug_info("mmc_card_init error\n", 0);
        return ret;
    }

    mmc_change_clock();
    /* save ocr information to general reg */
    writel(g_ocr, REG_BASE_SCTL + REG_SAVE_EMMC_OCR);

    return 0;
}

int mmc_mb_read_pio(void const *dst, size_t src, size_t cnt, size_t read_type)
{
    uint8_t *buf = (uint8_t *)dst;
    uint32_t cmd;
    uint16_t mode;
    size_t size;
    uint32_t spacc_channel_number = 1;
    spacc_decrypt_params decrypt_params = {0};

    /* set host block size 512 */
    sdhci_writew(EMMC_BLOCK_SIZE, SDHCI_BLOCK_SIZE);

    /* Send CMD16 to set blocksize */
    cmd = sdhci_make_cmd_fun(MMC_CMD_SET_BLOCKLEN,
        SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    mmc_send_cmd(cmd, EMMC_BLOCK_SIZE);

    /* set data timeout */
    sdhci_writeb(0xE, SDHCI_TIMEOUT_CONTROL);

    /* set host block count */
    sdhci_writew(cnt, SDHCI_BLOCK_COUNT);

    /* Send CMD23 to set blockcount */
    cmd = sdhci_make_cmd_fun(MMC_CMD_SET_BLOCK_COUNT,
        SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    mmc_send_cmd(cmd, cnt);

    /* set transfer mode */
    mode = SDHCI_TRNS_BLK_CNT_EN | SDHCI_TRNS_MULTI | SDHCI_TRNS_READ;
    sdhci_writew(mode, SDHCI_TRANSFER_MODE);

    /* Send CMD18 for multiple block read */
    if (g_ocr & OCR_HCS) {
        cmd = sdhci_make_cmd_fun(MMC_CMD_READ_MULTIPLE_BLOCK, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT | SDHCI_CMD_DATA);
        mmc_send_cmd(cmd, src / EMMC_BLOCK_SIZE);
    } else {
        cmd = sdhci_make_cmd_fun(MMC_CMD_READ_MULTIPLE_BLOCK, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT | SDHCI_CMD_DATA);
        mmc_send_cmd(cmd, src);
    }

    while (cnt) {
        if (sdhci_check_int_status(SDHCI_INT_DATA_AVAIL, 3000)) /* timeout 3000ms */
            return TD_FAILURE;

        sdhci_writel(SDHCI_INT_DATA_AVAIL, SDHCI_INT_STATUS);

        if (read_type == READ_DATA_BY_CPU) {
            size = EMMC_BLOCK_SIZE;
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
    if (drv_spacc_decrypt(decrypt_params) != TD_SUCCESS) {
        return TD_FAILURE;
    }
            buf += EMMC_BLOCK_SIZE;
        }

        cnt--;
    }

    return TD_SUCCESS;
}

size_t mmc_read(void *ptr, size_t src, size_t size, size_t read_type)
{
    size_t mmc_block_count;
    uint32_t bootmode;

    if (size % EMMC_BLOCK_SIZE) {
        mmc_block_count = size / EMMC_BLOCK_SIZE + 1;
    } else
        mmc_block_count = size / EMMC_BLOCK_SIZE;

    bootmode = is_bootmode();
    if (bootmode) {
        return sdhci_read_boot_data((uint32_t)(uintptr_t)ptr, mmc_block_count, read_type);
    }
    return mmc_mb_read_pio(ptr, src, mmc_block_count, read_type);
}

void mmc_deinit()
{
    sdhci_deinit();
}
