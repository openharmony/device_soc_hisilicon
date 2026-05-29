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

#ifndef __MMC_H__
#define __MMC_H__

#define REG_MMC_BASE    REG_BASE_EMMC  /* EMMC: 0x100e_0000 */

//CMD
#define MMC_CMD_GO_IDLE_STATE		0
#define MMC_CMD_SEND_OP_COND		1
#define MMC_CMD_ALL_SEND_CID		2
#define MMC_CMD_SET_RELATIVE_ADDR	3
#define MMC_CMD_SELECT_CARD         7
#define MMC_CMD_STOP_TRANSMISSION	12
#define MMC_CMD_SET_BLOCKLEN		16
#define MMC_CMD_READ_SINGLE_BLOCK	17
#define MMC_CMD_READ_MULTIPLE_BLOCK	18
#define SD_CMD_APP_SEND_OP_COND		41
#define MMC_CMD_APP_CMD             55
#define SD_CMD_SEND_IF_COND         8
#define SD_CMD_APP_SET_BUS_WIDTH	6

#define OCR_BUSY					0x80000000
#define OCR_HCS						0x40000000

#define CARD_WIDTH_1BIT             (0x00)
#define CARD_WIDTH_4BIT	            (0x01)

#define MMC_VDD_32_33				0x00100000 /* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34				0x00200000 /* VDD voltage 3.3 ~ 3.4 */
#endif /* __MMC_H__ */

