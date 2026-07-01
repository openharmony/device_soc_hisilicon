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

#ifndef __SDHCI_ONCHIPROM_H__
#define __SDHCI_ONCHIPROM_H__

#include <lib.h>
#include <types.h>
#include <platform.h>

#define SDHCI_REG_BASE          REG_BASE_EMMC

#define SDHCI_DMA_ADDRESS    0x00
#define SDHCI_ARGUMENT2        SDHCI_DMA_ADDRESS

#define SDHCI_BLOCK_SIZE    0x04

#define sdhci_make_cmd_fun(c, f) ((((c) & 0xff) << 8) | ((f) & 0xff))
#define SDHCI_BLOCK_COUNT    0x06
#define SDHCI_ARGUMENT        0x08

#define SDHCI_TRANSFER_MODE     0x0C
#define  SDHCI_TRNS_DMA         0x0001
#define  SDHCI_TRNS_BLK_CNT_EN  0x0002
#define  SDHCI_TRNS_READ        0x0010
#define  SDHCI_TRNS_MULTI       0x0020

#define SDHCI_COMMAND        0x0E
#define  SDHCI_CMD_RESP_MASK    0x03
#define  SDHCI_CMD_RESP_NONE    0x0000
#define  SDHCI_CMD_RESP_LONG    0x0001
#define  SDHCI_CMD_RESP_SHORT   0x0002
#define  SDHCI_CMD_CRC          0x0008
#define  SDHCI_CMD_INDEX    0x10
#define  SDHCI_CMD_DATA        0x20
#define  SDHCI_CMD_ABORTCMD    0xC0

#define SDHCI_RESPONSE        0x10

#define SDHCI_BUFFER        0x20

#define SDHCI_PRESENT_STATE    0x24
#define  SDHCI_CMD_INHIBIT    0x00000001
#define  SDHCI_DATA_INHIBIT    0x00000002
#define  SDHCI_DOING_WRITE    0x00000100
#define  SDHCI_DOING_READ    0x00000200
#define  SDHCI_SPACE_AVAILABLE    0x00000400
#define  SDHCI_DATA_AVAILABLE    0x00000800
#define  SDHCI_CARD_PRESENT    0x00010000
#define  SDHCI_CARD_DETECT_PIN    0x00040000
#define  SDHCI_WRITE_PROTECT    0x00080000
#define  SDHCI_PSTATE_DAT_0     0x00100000
#define  SDHCI_DATA_LVL_MASK    0x00F00000
#define   SDHCI_DATA_LVL_SHIFT    20

#define SDHCI_HOST_CONTROL    0x28
#define  SDHCI_CTRL_LED        0x01
#define  SDHCI_CTRL_4BITBUS    0x02
#define  SDHCI_CTRL_DMA_MASK    0x18
#define   SDHCI_CTRL_SDMA    0x00
#define   SDHCI_CTRL_ADMA1    0x08
#define   SDHCI_CTRL_ADMA32    0x10
#define   SDHCI_CTRL_ADMA64    0x18
#define   SDHCI_CTRL_8BITBUS    0x20
#define   SDHCI_CTRL_CD_LEVEL    0x40
#define   SDHCI_CTRL_CD_DETECT    0x80

#define SDHCI_POWER_CONTROL    0x29
#define  SDHCI_POWER_ON        0x01
#define  SDHCI_POWER_180    0x0A
#define  SDHCI_POWER_300    0x0C
#define  SDHCI_POWER_330    0x0E

#define SDHCI_BLOCK_GAP_CONTROL    0x2A
#define  SDHCI_BOOT_EN        0x20

#define SDHCI_WAKE_UP_CONTROL    0x2B
#define  SDHCI_WAKE_ON_INT    0x01
#define  SDHCI_WAKE_ON_INSERT    0x02
#define  SDHCI_WAKE_ON_REMOVE    0x04

#define SDHCI_CLOCK_CONTROL    0x2C
#define  SDHCI_PROG_CLOCK_MODE    0x0020
#define  SDHCI_CLOCK_INT_EN    0x0001
#define  SDHCI_CLOCK_INT_STABLE    0x0002
#define  SDHCI_CLOCK_CARD_EN    0x0004

#define SDHCI_TIMEOUT_CONTROL    0x2E

#define SDHCI_SOFTWARE_RESET    0x2F
#define  SDHCI_RESET_ALL    0x01
#define  SDHCI_RESET_CMD    0x02
#define  SDHCI_RESET_DATA    0x04

#define SDHCI_INT_STATUS    0x30
#define SDHCI_INT_ENABLE    0x34
#define SDHCI_SIGNAL_ENABLE    0x38
#define  SDHCI_INT_RESPONSE    0x00000001
#define  SDHCI_INT_DATA_END    0x00000002
#define  SDHCI_INT_BLK_GAP    0x00000004
#define  SDHCI_INT_DMA_END    0x00000008
#define  SDHCI_INT_SPACE_AVAIL    0x00000010
#define  SDHCI_INT_DATA_AVAIL    0x00000020
#define  SDHCI_INT_CARD_INSERT    0x00000040
#define  SDHCI_INT_CARD_REMOVE    0x00000080
#define  SDHCI_INT_CARD_INT    0x00000100
#define  SDHCI_INT_ERROR    0x00008000
#define  SDHCI_INT_TIMEOUT    0x00010000
#define  SDHCI_INT_CRC        0x00020000
#define  SDHCI_INT_END_BIT    0x00040000
#define  SDHCI_INT_INDEX    0x00080000
#define  SDHCI_INT_DATA_TIMEOUT    0x00100000
#define  SDHCI_INT_DATA_CRC    0x00200000
#define  SDHCI_INT_DATA_END_BIT    0x00400000
#define  SDHCI_INT_BUS_POWER    0x00800000
#define  SDHCI_INT_ACMD12ERR    0x01000000
#define  SDHCI_INT_ADMA_ERROR    0x02000000

#define  SDHCI_INT_NORMAL_MASK    0x00007FFF
#define  SDHCI_INT_ERROR_MASK    0xFFFF8000

#define  SDHCI_INT_CMD_MASK    (SDHCI_INT_RESPONSE | SDHCI_INT_TIMEOUT | \
        SDHCI_INT_CRC | SDHCI_INT_END_BIT | SDHCI_INT_INDEX)
#define  SDHCI_INT_DATA_MASK    (SDHCI_INT_DATA_END | SDHCI_INT_DMA_END | \
        SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL | \
        SDHCI_INT_DATA_TIMEOUT | SDHCI_INT_DATA_CRC | \
        SDHCI_INT_DATA_END_BIT | SDHCI_INT_ADMA_ERROR | \
        SDHCI_INT_BLK_GAP)
#define SDHCI_INT_ALL_MASK    ((unsigned int)-1)

#define SDHCI_ACMD12_ERR    0x3C

#define SDHCI_HOST_CONTROL2        0x3E
#define  SDHCI_CTRL_UHS_MASK        0x0007
#define   SDHCI_CTRL_UHS_SDR12        0x0000
#define   SDHCI_CTRL_UHS_SDR25        0x0001
#define   SDHCI_CTRL_UHS_SDR50        0x0002
#define   SDHCI_CTRL_UHS_SDR104        0x0003
#define   SDHCI_CTRL_UHS_DDR50        0x0004
#define   SDHCI_CTRL_HS_SDR200        0x0005 /* reserved value in SDIO spec */
#define  SDHCI_CTRL_VDD_180        0x0008
#define  SDHCI_CTRL_DRV_TYPE_MASK    0x0030
#define   SDHCI_CTRL_DRV_TYPE_B        0x0000
#define   SDHCI_CTRL_DRV_TYPE_A        0x0010
#define   SDHCI_CTRL_DRV_TYPE_C        0x0020
#define   SDHCI_CTRL_DRV_TYPE_D        0x0030
#define  SDHCI_CTRL_EXEC_TUNING        0x0040
#define  SDHCI_CTRL_TUNED_CLK        0x0080
#define  SDHCI_CTRL_PRESET_VAL_ENABLE    0x8000

#define SDHCI_CAPABILITIES    0x40
#define  SDHCI_TIMEOUT_CLK_MASK    0x0000003F
#define  SDHCI_TIMEOUT_CLK_SHIFT 0
#define  SDHCI_TIMEOUT_CLK_UNIT    0x00000080
#define  SDHCI_CLOCK_BASE_MASK    0x00003F00
#define  SDHCI_CLOCK_V3_BASE_MASK    0x0000FF00
#define  SDHCI_CLOCK_BASE_SHIFT    8
#define  SDHCI_MAX_BLOCK_MASK    0x00030000
#define  SDHCI_MAX_BLOCK_SHIFT  16
#define  SDHCI_CAN_DO_8BIT    0x00040000
#define  SDHCI_CAN_DO_ADMA2    0x00080000
#define  SDHCI_CAN_DO_ADMA1    0x00100000
#define  SDHCI_CAN_DO_SDMA    0x00400000
#define  SDHCI_CAN_VDD_330    0x01000000
#define  SDHCI_CAN_VDD_300    0x02000000
#define  SDHCI_CAN_VDD_180    0x04000000
#define  SDHCI_CAN_64BIT    0x10000000

#define  SDHCI_SUPPORT_SDR50    0x00000001
#define  SDHCI_SUPPORT_SDR104    0x00000002
#define  SDHCI_SUPPORT_DDR50    0x00000004
#define  SDHCI_DRIVER_TYPE_A    0x00000010
#define  SDHCI_DRIVER_TYPE_C    0x00000020
#define  SDHCI_DRIVER_TYPE_D    0x00000040
#define  SDHCI_RETUNING_TIMER_COUNT_MASK    0x00000F00
#define  SDHCI_RETUNING_TIMER_COUNT_SHIFT    8
#define  SDHCI_USE_SDR50_TUNING            0x00002000
#define  SDHCI_RETUNING_MODE_MASK        0x0000C000
#define  SDHCI_RETUNING_MODE_SHIFT        14
#define  SDHCI_CLOCK_MUL_MASK    0x00FF0000
#define  SDHCI_CLOCK_MUL_SHIFT    16

#define SDHCI_CAPABILITIES_1    0x44

/* 4C-4F reserved for more max current */

#define SDHCI_SET_ACMD12_ERROR    0x50
#define SDHCI_SET_INT_ERROR    0x52

#define SDHCI_ADMA_ERROR    0x54

/* 55-57 reserved */

#define SDHCI_ADMA_ADDRESS    0x58

/* 60-FB reserved */
#define SDHCI_BOOT_TIMEOUT    0x70

#define SDHCI_PRESET_FOR_SDR12 0x66
#define SDHCI_PRESET_FOR_SDR25 0x68
#define SDHCI_PRESET_FOR_SDR50 0x6A
#define SDHCI_PRESET_FOR_SDR104        0x6C
#define SDHCI_PRESET_FOR_DDR50 0x6E
#define SDHCI_PRESET_DRV_MASK  0xC000
#define SDHCI_PRESET_DRV_SHIFT  14
#define SDHCI_PRESET_CLKGEN_SEL_MASK   0x400
#define SDHCI_PRESET_CLKGEN_SEL_SHIFT    10
#define SDHCI_PRESET_SDCLK_FREQ_MASK   0x3FF
#define SDHCI_PRESET_SDCLK_FREQ_SHIFT    0

#define SDHCI_SLOT_INT_STATUS    0xFC

#define SDHCI_HOST_VERSION    0xFE
#define  SDHCI_VENDOR_VER_MASK    0xFF00
#define  SDHCI_VENDOR_VER_SHIFT    8
#define  SDHCI_SPEC_VER_MASK    0x00FF
#define  SDHCI_SPEC_VER_SHIFT    0
#define   SDHCI_SPEC_100    0
#define   SDHCI_SPEC_200    1
#define   SDHCI_SPEC_300    2

#define SDHCI_EMMC_CTRL            0x52C
#define SDHCI_CARD_IS_EMMC        0x0001

#define SDHCI_BOOT_CTRL            0x52E
#define MAN_BOOT_EN                0x1
#define VALIDATE_BOOT            (0x1 << 7)

#define SDHCI_EMMC_HW_RESET        0x534

#define SDHCI_AT_CTRL            0x540
#define SDHCI_AT_STAT            0x544

#define SDHCI_DMA_BUF_ADDR        0x600
/*
 * End of controller registers.
 */

static inline void sdhci_writel(uint32_t val, unsigned long reg)
{
    writel(val, SDHCI_REG_BASE + reg);
}

static inline void sdhci_writew(uint16_t val, unsigned long reg)
{
    writew(val, SDHCI_REG_BASE + reg);
}

static inline void sdhci_writeb(uint8_t val, unsigned long reg)
{
    writeb(val, SDHCI_REG_BASE + reg);
}

static inline uint32_t sdhci_readl(unsigned long reg)
{
    return readl(SDHCI_REG_BASE + reg);
}

static inline uint16_t sdhci_readw(unsigned long reg)
{
    return readw(SDHCI_REG_BASE + reg);
}

static inline uint8_t sdhci_readb(unsigned long reg)
{
    return readb(SDHCI_REG_BASE + reg);
}

int32_t sdhci_init(void);
int32_t sdhci_deinit(void);
int32_t sdhci_start_boot(void);
int32_t sdhci_read_boot_data(uint32_t data_addr, uint32_t data_size, size_t read_type);
int sdhci_check_int_status(uint32_t mask, uint32_t timeout);
int copy_from_sdio(void *buffer, unsigned long maxsize);
void sdio_deinit();
int self_sdio_check();
void enable_sdio_dma();

#endif /* __DWMMC_ONCHIPROM_H__ */

