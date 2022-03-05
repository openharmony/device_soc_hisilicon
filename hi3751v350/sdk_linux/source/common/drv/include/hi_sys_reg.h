/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __HI_SYS_REG_H__
#define __HI_SYS_REG_H__

#include "hi_type.h"
#ifdef CHIP_TYPE_hi3751v350
#include "./sysreg/hi3751v350/hi_reg_sys.h"
#include "./sysreg/hi3751v350/hi_reg_peri.h"
#include "./sysreg/hi3751v350/hi_reg_io.h"
#include "./sysreg/hi3751v350/hi_reg_crg.h"
#endif

#define HI_SYS_BASE_ADDR     0xF8000000
#define HI_PERI_BASE_ADDR    0xF8A20000
#define HI_IO_BASE_ADDR      0xF8A21000
#define HI_CRG_BASE_ADDR     0xF8A22000
#define HI_STB_CRG_BASE_ADDR 0xF8003000

typedef s_sysctrl_regs_type hi_reg_sysctrl;
typedef s_perictrl_regs_type hi_reg_peri;
typedef s_io_regs_type hi_reg_io;
typedef s_crg_regs_type hi_reg_crg;
typedef s_stb_crg_regs_type hi_stb_reg_crg;

extern volatile hi_reg_sysctrl *g_pst_reg_sys_ctrl;
extern volatile hi_reg_crg *g_pst_reg_crg;
extern volatile hi_reg_io *g_pst_reg_io;
extern volatile hi_reg_peri *g_pst_reg_peri;
extern volatile hi_stb_reg_crg *g_pst_stb_reg_crg;
extern volatile hi_void *g_pst_otp_reg_base;

#endif  // __HI_SYS_REG_H__

