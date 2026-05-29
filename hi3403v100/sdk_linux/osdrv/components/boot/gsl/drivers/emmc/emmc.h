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

#ifndef __EMMC_H__
#define __EMMC_H__

/* eMMC block size */
#define EMMC_BLOCK_SIZE                 512
/* eMMC boot partition size */
#define EMMC_BOOT_PARTITION_SIZE        (1024*1024)

/* CMD */
#define MMC_CMD_GO_IDLE_STATE       0
#define MMC_CMD_SEND_OP_COND        1
#define MMC_CMD_ALL_SEND_CID        2
#define MMC_CMD_SET_RELATIVE_ADDR   3
#define MMC_CMD_SWITCH              6
#define MMC_CMD_SELECT_CARD         7
#define MMC_CMD_SEND_CSD            9
#define MMC_CMD_STOP_TRANSMISSION   12
#define MMC_CMD_SET_BLOCKLEN        16
#define MMC_CMD_READ_SINGLE_BLOCK   17
#define MMC_CMD_READ_MULTIPLE_BLOCK 18
#define MMC_CMD_SET_BLOCK_COUNT     23

#define MMC_SWITCH_MODE_WRITE_BYTE  0x3
#define MMC_SWITCH_ACCESS_SHIFT     24
#define MMC_SWITCH_INDEX_SHIFT      16
#define MMC_SWITCH_VALUE_SHIFT      8

#define EXT_CSD_BUS_WIDTH           183 /* R/W */
#define EXT_CSD_HS_TIMING           185 /* R/W */

#define EXT_CSD_BUS_WIDTH_1         0 /* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4         1 /* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8         2 /* Card is in 8 bit mode */

#define MMC_VDD_165_195    0x00000080 /* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_32_33      0x00100000 /* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34      0x00200000 /* VDD voltage 3.3 ~ 3.4 */

#define OCR_HCS            0x40000000
#define OCR_BUSY           0x80000000

#endif /* end of __EMMC_H__ */

