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

#ifndef DDR_OSAL_H
#define DDR_OSAL_H

#include "types.h"
#include "common.h"

#ifndef DDR_AO_FUNC
#define DDR_AO_FUNC
#endif

#ifndef DDR_AO_DATA
#define DDR_AO_DATA
#endif

#ifdef CFG_FPGA_VERIFY
#define DDR_FPGA_VERIFY
#endif /* CFG_FPGA_VERIFY */

#ifdef CFG_EDA_VERIFY
#define DDR_EDA_VERIFY
#endif /* CFG_EDA_VERIFY */

/* register operations */
static inline unsigned int ddr_reg_read(unsigned long addr)
{
    return (*(volatile unsigned int *)((uintptr_t)addr));
}
inline void ddr_reg_write(unsigned int val, unsigned long addr)
{
    (*(volatile unsigned int *)((uintptr_t)addr)) = val;
}

/* DDR log */
#define ddr_serial_puts(s) log_serial_puts((s))
#define ddr_serial_put_hex(x) serial_put_hex(x)

#if defined(DDR_TRAINING_CMD) || defined(DDR_TRAINING_UBOOT)
#define ddr_serial_puts uart_early_puts
#define ddr_serial_put_hex uart_early_put_hex
#endif

#endif /* DDR_OSAL_H */
