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

#ifndef __ONCHIPROM_H__
#define __ONCHIPROM_H__

/* define vrl table of xloader loaded address & size */
#define VRL_TABLE_LOAD_ADDRESS	REG_BASE_LP_RAM
#define VRL_TABLE_LOAD_SIZE		(4*1024)

/* define xloader loaded address & size */
#define XLOADER_IMAGE_LOAD_ADDRESS	(VRL_TABLE_LOAD_ADDRESS + VRL_TABLE_LOAD_SIZE)
#ifdef EDA_SPEEDUP
#define XLOADER_IMAGE_LOAD_SIZE		(4*1024)
#else
#define XLOADER_IMAGE_LOAD_SIZE		(28*1024)
#endif

#define ONCHIPROM_IMAGE_LOAD_MAX	(100*1024)

/* define ufs work space area */

/* define onchiprom static area */
#define ONCHIPROM_STATIC_AREA_ADDRESS		(REG_BASE_LP_RAM + 0x1CE00)
#define ONCHIPROM_STATIC_AREA_SIZE			(0x100)

/* define secure engine static area */
#define SECURE_ENGINE_STATIC_AREA_ADDRESS	(REG_BASE_LP_RAM + 0x1CF00)
#define SECURE_ENGINE_STATIC_AREA_SIZE		(0x100)

/* define secure engine work space address & size */
#define SECURE_ENGINE_WORK_SPACE_ADDRESS	(REG_BASE_LP_RAM + 0x1D000)
#define SECURE_ENGINE_WORK_SPACE_SIZE		(0x1800)

/* define secure engine work space address & size */
#define USB_STATIC_WORK_SPACE_ADDRESS		(REG_BASE_LP_RAM + 0x1E800)
#define USB_STATIC_WORK_SPACE_SIZE			(0x1800)

/* xloader image length address */
#define VRL_TABLE_IMAGE_LENGTH_OFFEST_ADDR	(0xFFC)

#define LPRAM_MEMORY_MAP_OFFEST_ADDRESS		SECURE_ENGINE_STATIC_AREA_ADDRESS
#define LPRAM_MEMORY_MAP_OFFEST		(REG_BASE_LP_RAM_ACORE - REG_BASE_LP_RAM)

/* static variable */
#define STATIC_ERROR_CODE_ADDR			(ONCHIPROM_STATIC_AREA_ADDRESS + 0x4)
#define STATIC_TIMER_STATUS_ADDR		(ONCHIPROM_STATIC_AREA_ADDRESS + 0x8)
#define STATIC_TIMER_COUNT_ADDR			(ONCHIPROM_STATIC_AREA_ADDRESS + 0xC)

/* define boot mode */
#define BOOT_MODE_USB_DOWNLOAD	0
#define BOOT_MODE_NORMAL_BOOT	1

#define BOOT_MODE_EMMC_BOOT		0

typedef void (*PJUMPTOADDR)(void);      /* addr of xloader */

#endif /* end of __ONCHIPROM_H__ */

