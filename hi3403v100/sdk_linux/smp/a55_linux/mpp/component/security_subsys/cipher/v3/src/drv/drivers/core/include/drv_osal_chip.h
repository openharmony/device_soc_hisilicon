/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef DRV_OSAL_CHIP_H
#define DRV_OSAL_CHIP_H

#include "cipher_cfg.h"

#if defined(CHIP_HASH_VER_V100) || defined(CHIP_HASH_VER_V200) || defined(CHIP_HASH_VER_V300)
#define CHIP_HASH_SUPPORT
#endif

#if defined(CHIP_SYMC_VER_V100) || defined(CHIP_SYMC_VER_V200) || defined(CHIP_SYMC_VER_V300)
#define CHIP_SYMC_SUPPORT
#endif

#if defined(CHIP_TRNG_VER_V100) || defined(CHIP_TRNG_VER_V200)
#define CHIP_TRNG_SUPPORT
#endif

#if defined(CHIP_IFEP_RSA_VER_V100) || defined(CHIP_PKE_VER_V200)
#define CHIP_RSA_SUPPORT
#endif

#if defined(CHIP_HDCP_VER_V100) || defined(CHIP_HDCP_VER_V200) || defined(CHIP_HDCP_VER_V300)
#define CHIP_HDCP_SUPPORT
#endif

#if defined(CHIP_PKE_VER_V100) || defined(CHIP_PKE_VER_V200)
#define CHIP_PKE_SUPPORT
#endif

#if defined(CRYPTO_SWITCH_CPU) && defined(CRYPTO_SEC_CPU)
#error "CRYPTO_SEC_CPU and CPU_SWITCH_SUPPORT cannot be defined simultaneously"
#endif

#endif /* DRV_OSAL_CHIP_H */
