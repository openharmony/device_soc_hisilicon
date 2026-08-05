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

#include "sdhci.h"
#include "fat.h"
#include "securecutil.h"
#include "cipher.h"
#include <types.h>
#include <common.h>
#include <platform.h>
#include <lib.h>
#include <mmc.h>


#define MMC_BLOCK_SIZE      512
#define SD_REG_BASE            REG_BASE_SDIO0
block_dev_desc_t *mmc_dev;
static char *mmc_buf;
unsigned int hcs;
static unsigned int sdio_dma_enable;

#define debug_printf(fmt, args...)

static inline void sd_writel(uint32_t val, unsigned long reg)
{
    writel(val, SD_REG_BASE + reg);
}

static inline void sd_writew(uint16_t val, unsigned long reg)
{
    writew(val, SD_REG_BASE + reg);
}

static inline void sd_writeb(uint8_t val, unsigned long reg)
{
    writeb(val, SD_REG_BASE + reg);
}

static inline uint32_t sd_readl(unsigned long reg)
{
    return readl(SD_REG_BASE + reg);
}

static inline uint16_t sd_readw(unsigned long reg)
{
    return readw(SD_REG_BASE + reg);
}

static inline uint8_t sd_readb(unsigned long reg)
{
    return readb(SD_REG_BASE + reg);
}

static void sdio0_clk_enable(void)
{
    unsigned int reg;

    /* enable ck_en */
    reg = readl(REG_CRG_BASE + PERI_CRG_SDIO0);
    reg |= MMC_CLK_EN;
    writel(reg, REG_CRG_BASE + PERI_CRG_SDIO0);
}

static void sd_crg_init(void)
{
    unsigned int reg;

    sdio0_clk_enable();

    /* mmc_srst_req */
    reg = readl(REG_CRG_BASE + PERI_CRG_SDIO0);
    reg |= MMC_RST;
    writel(reg, REG_CRG_BASE + PERI_CRG_SDIO0);

    udelay(25); /* delay 25us */

    /* undo mmc_srst_req */
    reg &= ~MMC_RST;
    writel(reg, REG_CRG_BASE + PERI_CRG_SDIO0);
    udelay(1);

    /* card_clk_en = 0*/
    reg = sd_readw(SDHCI_CLOCK_CONTROL);
    reg &= ~SDHCI_CLOCK_INT_EN;
    sd_writew(reg, SDHCI_CLOCK_CONTROL);

    reg = readl(REG_CRG_BASE + PERI_CRG_SDIO0);
    reg &= ~(0x7 << MMC_CLK_SEL_SHIFT);
    reg |= (0 << MMC_CLK_SEL_SHIFT);
    writel(reg, REG_CRG_BASE + PERI_CRG_SDIO0);

    reg = readl(REG_CRG_BASE + PERI_SDIO0_DRV_DLL);
    reg &= ~MMC_DRV_PHASE_MASK;
    reg |= (16 << MMC_DRV_PHASE_SHIFT); /* drv_clk_phase_sel=16 (180 degree) */
    writel(reg, REG_CRG_BASE + PERI_SDIO0_DRV_DLL);

    udelay(25); /* delay 25us */

    /* card_clk_en = 1*/
    reg = sd_readw(SDHCI_CLOCK_CONTROL);
    reg |= SDHCI_CLOCK_INT_EN;
    sd_writew(reg, SDHCI_CLOCK_CONTROL);
}

static int sd_check_int_status(uint32_t mask, uint32_t timeout)
{
    unsigned int reg;
    unsigned int wait_time =  timeout * timer_get_divider();

    timer_start();
    for (;;) {
    reg = sd_readl(SDHCI_INT_STATUS);
        if (reg & mask) {
            break;
        }
        if (timer_get_val() > wait_time) {
            debug_printf("wait int status time out, reg = 0x%x, mask = 0x%x\n",
                     reg, mask);
            return -1;
        }
        if (reg & SDHCI_INT_ERROR_MASK) {
            debug_printf("int err: reg = 0x%x\n", reg);
            return -1;
        }
    }

    return 0;
}

static void sd_reset(uint8_t mask)
{
    unsigned long timeout;

    debug_printf("reg SDHCI_PRESENT_STATE = 0x%x\n",
             sd_readl(SDHCI_PRESENT_STATE));

    sd_writeb(mask, SDHCI_SOFTWARE_RESET);

    /* Wait max 100 ms */
    timeout = 100 * timer_get_divider();

    timer_start();
    /* hw clears the bit when it's done */
    while (sd_readb(SDHCI_SOFTWARE_RESET) & mask) {
        if (timer_get_val() > timeout) {
            debug_printf("sd: Reset 0x%x never completed.\n",
                     (int)mask);
            return;
        }
    }
}

static void sd_set_boot_clock_width(void)
{
    uint8_t ctrl;
    uint16_t clk = 0;
    unsigned long timeout;

    sd_writew(0, SDHCI_CLOCK_CONTROL);
    clk |= SDHCI_CLOCK_INT_EN;
    sd_writew(clk, SDHCI_CLOCK_CONTROL);

    /* Wait max 20 ms */
    timeout = 20 * timer_get_divider();
    timer_start();
    while (!((clk = sd_readw(SDHCI_CLOCK_CONTROL))
         & SDHCI_CLOCK_INT_STABLE)) {
        if (timer_get_val() > timeout) {
            debug_printf("wait arasan host clock stable time out\n");
            /* todo: error code must be set */
            break;
        }
    }

    clk |= SDHCI_CLOCK_CARD_EN;
    sd_writew(clk, SDHCI_CLOCK_CONTROL);

    /* set 1bit data width */
    ctrl = sd_readb(SDHCI_HOST_CONTROL);
    ctrl &= ~SDHCI_CTRL_8BITBUS;
    ctrl &= ~SDHCI_CTRL_4BITBUS;
    sd_writeb(ctrl, SDHCI_HOST_CONTROL);

    return;
}

static int sd_controller_init(void)
{
    sd_writel(0xFFFFFFFF, SDHCI_INT_STATUS);
    /* enable all interrupt */
    sd_writel(0xFFFFFFFF, SDHCI_INT_ENABLE);
    sd_writeb(SDHCI_POWER_ON | SDHCI_POWER_330, SDHCI_POWER_CONTROL);
    mdelay(100); /* delay 100ms */
    sd_set_boot_clock_width();
    mdelay(1);
    sd_writeb(0xE, SDHCI_TIMEOUT_CONTROL);

    return 0;
}

static int sd_send_cmd(uint32_t cmd, uint32_t arg)
{
    sd_writel(0xFFFFFFFF, SDHCI_INT_STATUS);
    sd_writel(arg, SDHCI_ARGUMENT);
    sd_writew(cmd, SDHCI_COMMAND);

    if (sd_check_int_status(SDHCI_INT_RESPONSE, 1000)) {  /* 1000ms timeout */
        debug_printf("send cmd error\n");
        return -1;
    }

    return 0;
}

static int sd_card_init(void)
{
    uint32_t cmd;
    uint32_t arg;
    uint32_t rsp;
    uint32_t ctrl;
    uint32_t timeout = 100;

    /* Send CMD0 to reset card into idle state */
    cmd = sdhci_make_cmd_fun(MMC_CMD_GO_IDLE_STATE, SDHCI_CMD_RESP_NONE);
    sd_send_cmd(cmd, 0);
    udelay(2000); /* delay 2000us */

    /* Send CMD8 to make sure if voltage is ok */
    cmd = sdhci_make_cmd_fun(SD_CMD_SEND_IF_COND,
        SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    arg = (MMC_VDD_32_33 | MMC_VDD_33_34) & 0x00ff8000;
    sd_send_cmd(cmd, ((arg != 0) << 0x8) | 0xaa);

    rsp = sd_readl(SDHCI_RESPONSE);
    if ((rsp & 0xff) == 0xaa) {
        arg |= OCR_HCS;
    }
    /* Send ACMD41 for initialization and wait to complete*/
    do {
        cmd = sdhci_make_cmd_fun(MMC_CMD_APP_CMD, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
        sd_send_cmd(cmd, 0);

        cmd = sdhci_make_cmd_fun(SD_CMD_APP_SEND_OP_COND, SDHCI_CMD_RESP_SHORT);
        sd_send_cmd(cmd, arg);
        rsp = sd_readl(SDHCI_RESPONSE);
        if (timeout-- == 0) {
            return -1;
        }
        mdelay(10); /* delay 10ms */
    } while (!(rsp & OCR_BUSY));

    /* card capacity */
    hcs = ((rsp & OCR_HCS) == OCR_HCS);

    /* Send CMD2 to get card cid numbers*/
    cmd = sdhci_make_cmd_fun(MMC_CMD_ALL_SEND_CID,
        SDHCI_CMD_CRC | SDHCI_CMD_RESP_LONG);
    sd_send_cmd(cmd, 0);

    cmd = sdhci_make_cmd_fun(MMC_CMD_SET_RELATIVE_ADDR, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    sd_send_cmd(cmd, 0);

    rsp = sd_readl(SDHCI_RESPONSE);
    cmd = sdhci_make_cmd_fun(MMC_CMD_SELECT_CARD, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    arg = rsp & 0xffff0000;
    sd_send_cmd(cmd, arg);

    cmd = sdhci_make_cmd_fun(MMC_CMD_APP_CMD, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    sd_send_cmd(cmd, arg);

    cmd = sdhci_make_cmd_fun(SD_CMD_APP_SET_BUS_WIDTH, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    sd_send_cmd(cmd, 0x2);

    ctrl = sd_readb(SDHCI_HOST_CONTROL);
    ctrl &= ~SDHCI_CTRL_8BITBUS;
    ctrl |= SDHCI_CTRL_4BITBUS;
    sd_writeb(ctrl, SDHCI_HOST_CONTROL);

    return 0;
}

int sdio_init()
{
    int ret;

    sd_crg_init();

    ret = sd_controller_init();
    if (ret) {
        debug_printf("sd_controller_init error\n");
        return ret;
    }

    ret = sd_card_init();
    if (ret) {
        debug_printf("sd_card_init error\n");
        return ret;
    }

    mmc_buf = gsl_malloc(MMC_BLOCK_SIZE);
    if (!mmc_buf) {
        return -1;
    }
    return 0;
}

void sdio_deinit()
{
    if (mmc_buf) {
        gsl_free(mmc_buf);
        mmc_buf = NULL;
    }
    if (mmc_dev) {
        gsl_free(mmc_dev);
        mmc_dev = NULL;
    }

    sd_reset(SDHCI_RESET_ALL);

    /*power down*/
    sd_writeb(0, SDHCI_POWER_CONTROL);
}

int sd_card_check(void)
{
    uint32_t status1;
    uint32_t status2;
    uint32_t status3;

    status1 = sd_readl(SDHCI_PRESENT_STATE);
    if (!(status1 & SDHCI_CARD_DETECT_PIN)) {
        return FALSE;
    } else {
        status2 = sd_readl(SDHCI_PRESENT_STATE);
        if (!(status2 & SDHCI_CARD_DETECT_PIN)) {
            return FALSE;
        } else {
            status3 = sd_readl(SDHCI_PRESENT_STATE);
            if (!(status3 & SDHCI_CARD_DETECT_PIN)) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

void sdio0_io_config(void)
{
    uint32_t peri_val;

    /* MISC PLWER SEL and POWER EN */
    reg_set(REG_BASE_MISC + MISC_SYS_CTRL, 0x11);

    reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_CARD_POWEN_EN_OFST, 0x1011);
    reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_CARD_DETECT_OFST, 0x1801);
    reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_CCLK_OUT_OFST, 0x14a1);

    peri_val = reg_get(REG_BASE_SCTL + OS_OTP_PO_SDIO_INFO);
    if (sdio_pad_pu(peri_val)) {
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_DATA0_OFST, 0x1411);
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_DATA1_OFST, 0x1411);
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_DATA2_OFST, 0x1411);
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_DATA3_OFST, 0x1411);
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_CCMD_OFST, 0x1411);
    } else {
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_DATA0_OFST, 0x1511);
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_DATA1_OFST, 0x1511);
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_DATA2_OFST, 0x1511);
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_DATA3_OFST, 0x1511);
        reg_set(REG_VDP_AIAO_IO_CFG_BASE + SDIO0_CCMD_OFST, 0x1511);
    }
}

size_t mmc_block_read(void *dst, uint32_t src, size_t size)
{
    uint8_t *buf = (uint8_t *)dst;
    uint32_t cmd;
    uint16_t mode;
    int ret;
    uint32_t spacc_channel_number = 1;
    spacc_decrypt_params decrypt_params = {0};

    sd_writew(MMC_BLOCK_SIZE, SDHCI_BLOCK_SIZE);
    cmd = sdhci_make_cmd_fun(MMC_CMD_SET_BLOCKLEN, SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT);
    sd_send_cmd(cmd, MMC_BLOCK_SIZE);
    sd_writeb(0xe, SDHCI_TIMEOUT_CONTROL);
    sd_writew(1, SDHCI_BLOCK_COUNT);

    mode = SDHCI_TRNS_READ;
    sd_writew(mode, SDHCI_TRANSFER_MODE);

    /* Send CMD17 for single block read*/
    if (hcs) {
        cmd = sdhci_make_cmd_fun(MMC_CMD_READ_SINGLE_BLOCK,
            SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT
            | SDHCI_CMD_DATA);
        sd_send_cmd(cmd, src / MMC_BLOCK_SIZE);
    } else {
        cmd = sdhci_make_cmd_fun(MMC_CMD_READ_SINGLE_BLOCK,
            SDHCI_CMD_CRC | SDHCI_CMD_RESP_SHORT
            | SDHCI_CMD_DATA);
        sd_send_cmd(cmd, src);
    }

    if (sd_check_int_status(SDHCI_INT_DATA_AVAIL, 2000)) /* 2000ms timeout */
        return -1;

    sd_writel(SDHCI_INT_DATA_AVAIL, SDHCI_INT_STATUS);

    if (!sdio_dma_enable) {
        while (size) {
            *(uint32_t *)buf = sd_readl(SDHCI_BUFFER);
            buf += 0x4;
            size -= 0x4;
        }
    } else {
        decrypt_params.chn = spacc_channel_number;
    decrypt_params.dst_addr = (uint32_t)(uintptr_t)buf;
    decrypt_params.src_addr = (uint32_t)(REG_BASE_SDIO0 + SDHCI_DMA_BUF_ADDR);
    decrypt_params.length = MMC_BLOCK_SIZE;
    decrypt_params.alg = SYMC_ALG_DMA;
    decrypt_params.mode = SYMC_MODE_CBC;
        ret = drv_spacc_decrypt(decrypt_params);
        if (ret != TD_SUCCESS) {
            return -1;
        }
        buf += MMC_BLOCK_SIZE;
    }

    return (size_t)(buf - (uint8_t *)dst);
}

int update_mmc_read(unsigned long src, char *dst, unsigned long size)
{
    unsigned long end;
    unsigned long part_start;
    unsigned long part_end;
    unsigned long part_len;
    unsigned long aligned_start;
    unsigned long aligned_end;
    unsigned long mmc_block_size;
    unsigned long mmc_block_address;
    errno_t err;

    if (size == 0) {
        return -1;
    }

    mmc_block_size = MMC_BLOCK_SIZE;
    mmc_block_address = ~(mmc_block_size - 1);

    end = src + size;

    part_start = ~mmc_block_address & src;
    part_end = ~mmc_block_address & end;

    aligned_start = mmc_block_address & src;
    aligned_end = mmc_block_address & end;

    /* all block aligned accesses */
    if (part_start) {
        part_len = mmc_block_size - part_start;
        if ((mmc_block_read(mmc_buf, aligned_start, mmc_block_size)) < 0)
        {
            return -1;
        }
        err = memcpy_s(dst, part_len, mmc_buf + part_start, part_len);
        if (err != EOK)
        {
        return -1;
        }
        dst += part_len;
        src += part_len;
    }

    if (aligned_end < src) {
        return -1;
    }
    for (; src < aligned_end; src += mmc_block_size, dst += mmc_block_size) {
        if ((mmc_block_read((char *)(dst), src, mmc_block_size)) < 0) {
            return -1;
        }
    }

    if (part_end && src < end) {
        if ((mmc_block_read(mmc_buf, aligned_end, mmc_block_size)) < 0) {
            return -1;
        }
        err = memcpy_s(dst, part_end, mmc_buf, part_end);
        if (err != EOK) {
        return -1;
        }
    }

    return 0;
}

static unsigned long mmc_bread(int dev_num, unsigned long blknr,
    unsigned long blkcnt, void *dst)
{
    int mmc_block_size = MMC_BLOCK_SIZE;
    unsigned long src = blknr * mmc_block_size;

    update_mmc_read(src, (char *) dst, blkcnt * mmc_block_size);

    return blkcnt;
}

int self_sdio_check()
{
    int ret;
    errno_t err;

    sdio_init();

    mmc_dev = gsl_malloc(sizeof(block_dev_desc_t));
    if (!mmc_dev) {
        sdio_deinit();
        return 0;
    }

    err = memset_s(mmc_dev, sizeof(block_dev_desc_t), 0, sizeof(block_dev_desc_t));
    if (err != EOK) {
        sdio_deinit();
        gsl_free(mmc_dev);
        mmc_dev = NULL;
        return 0;
    }

    mmc_dev->dev = 0;
    mmc_dev->lun = 0;
    mmc_dev->target = 0xff;
    mmc_dev->block_read = mmc_bread;
    mmc_dev->removable = 1;
    mmc_dev->blksz = MMC_BLOCK_SIZE;

    ret = fat_register_device(mmc_dev);
    if (ret == 0x3) {
        return 1;
    }

    sdio_deinit();
    gsl_free(mmc_dev);
    mmc_dev = NULL;

    return 0;
}

void enable_sdio_dma()
{
    sdio_dma_enable = 1;
}
