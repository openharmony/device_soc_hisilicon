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

#include "sys_hal.h"

#include "ot_osal.h"
#include "ot_type.h"
#include "ot_board.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

static td_void *g_reg_crg_base_addr = TD_NULL;
static td_void *g_reg_sys_base_addr = TD_NULL;
static td_void *g_reg_ddr0_base_addr = TD_NULL;
static td_void *g_reg_misc_base_addr = TD_NULL;
static td_void *g_reg_crg_pll_addr = TD_NULL;
static td_void *g_reg_otp_user_base_addr = TD_NULL;

static td_bool g_sys_reg_by_dts = TD_FALSE;

static osal_spinlock_t g_crg_spin_lock = {0};
#define sys_crg_spin_lock(flags) osal_spin_lock_irqsave(&g_crg_spin_lock, &(flags))
#define sys_crg_spin_unlock(flags) osal_spin_unlock_irqrestore(&g_crg_spin_lock, &(flags))

#define io_crg_address(x) ((td_uintptr_t)g_reg_crg_base_addr + ((x) - (CRG_REGS_ADDR) - (CRG_REGS_ADDR_OFFSET)))
#define io_sys_address(x) ((td_uintptr_t)g_reg_sys_base_addr + ((x) - (SYS_REGS_ADDR)))
#define io_ddr0_address(x) ((td_uintptr_t)g_reg_ddr0_base_addr + ((x) - (DDRC0_REG_ADDR)))
#define io_misc_address(x) ((td_uintptr_t)g_reg_misc_base_addr + ((x) - (MISC_REGS_ADDR)))
#define io_crg_pll_address(x) ((td_uintptr_t)g_reg_crg_pll_addr + ((x) - (CRG_REGS_ADDR)))
#define io_otp_user_address(x) ((td_uintptr_t)g_reg_otp_user_base_addr + ((x) - (OTP_USER_REGS_ADDR)))

td_void sys_set_reg_by_dts(td_bool is_reg_by_dts)
{
    g_sys_reg_by_dts = is_reg_by_dts;
}

td_void sys_set_crg_base_addr(td_void *crg_base_addr)
{
    g_reg_crg_base_addr = crg_base_addr;
}

td_void sys_set_sys_base_addr(td_void *sys_base_addr)
{
    g_reg_sys_base_addr = sys_base_addr;
}

td_void sys_set_ddr0_base_addr(td_void *ddr0_base_addr)
{
    g_reg_ddr0_base_addr = ddr0_base_addr;
}

td_void sys_set_misc_base_addr(td_void *misc_base_addr)
{
    g_reg_misc_base_addr = misc_base_addr;
}

static td_void ot_reg_set_bit(unsigned long value, unsigned long offset, unsigned long addr)
{
    unsigned long t;
    unsigned long mask;
    unsigned long ul_flags;

    sys_crg_spin_lock(ul_flags);
    mask = 1 << offset;
    t = osal_readl((const volatile void *)(td_uintptr_t)addr);
    t &= ~mask;
    t |= (value << offset) & mask;
    osal_writel(t, (volatile void *)(td_uintptr_t)addr);
    sys_crg_spin_unlock(ul_flags);
}

static td_void ot_reg_write32(unsigned long value, unsigned long mask, unsigned long addr)
{
    unsigned long t;
    unsigned long ul_flags;

    sys_crg_spin_lock(ul_flags);
    t = osal_readl((const volatile void *)(td_uintptr_t)addr);
    t &= ~mask;
    t |= value & mask;
    osal_writel(t, (volatile void *)(td_uintptr_t)addr);
    sys_crg_spin_unlock(ul_flags);
}

static inline td_void ot_reg_read(unsigned int *pvalue, unsigned long addr)
{
    *pvalue = osal_readl((const volatile void *)(td_uintptr_t)addr);
}

td_u64 sys_hal_get_local_sys_cnt(td_void)
{
    td_u64 tmp_sys_cnt;
    td_ulong update_reg_addr;
    td_u32 syscnt_reg0;
    td_u32 syscnt_reg1;
    td_ulong syscnt_reg_addr;

    update_reg_addr = io_sys_address(SYS_REGS_ADDR + 0x104C);
    ot_reg_write32(0xc3, 0xff, update_reg_addr);

    syscnt_reg_addr = io_sys_address(SYS_REGS_ADDR + 0x1050);
    ot_reg_read(&syscnt_reg0, syscnt_reg_addr);
    ot_reg_read(&syscnt_reg1, syscnt_reg_addr + 0x4);

    tmp_sys_cnt = syscnt_reg1;
    tmp_sys_cnt <<= 32; /* 32 */
    tmp_sys_cnt |= syscnt_reg0;

    return tmp_sys_cnt;
}

td_s32 sys_hal_vo_bus_reset_sel(td_bool reset)
{
    const td_u32 tmp = (reset == TD_TRUE) ? 1 : 0;
    const td_u32 bit = 0; /* 0: 0bit */

    ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8336_ADDR));
    return TD_SUCCESS;
}

td_s32 sys_hal_vo_apb_clk_en(td_bool clk_en)
{
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;
    const td_u32 bit = 8; /* 8: 8bit */

    ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8336_ADDR));
    return TD_SUCCESS;
}

/* VO AXI BUS CLK */
td_s32 sys_hal_vo_bus_clk_en(td_bool clk_en)
{
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;
    const td_u32 bit = 9; /* 9: 9bit */

    ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8336_ADDR));
    return TD_SUCCESS;
}

td_s32 sys_hal_vo_out_hd_phase_ctrl(td_s32 vo_dev, td_bool reverse)
{
    const td_u32 tmp = (reverse == TD_TRUE) ? 1 : 0;
    const td_u32 bit = 20; /* 20: 20bit */

    if ((vo_dev != 0) && (vo_dev != 1)) {
        return TD_FAILURE;
    }

    ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8338_ADDR));
    return TD_SUCCESS;
}

/* VO DHD0: HPLL, HPLL FOUT4; DHD1: FIXED, LCDMCLKs */
td_s32 sys_hal_vo_hd_clk_sel(td_s32 dev, td_u32 clk_sel)
{
    const td_u32 bit = 12; /* 12: 12bit */
    const td_u32 mask = 0xf;

    if (dev == 0) {
        ot_reg_set_bit(clk_sel, bit, io_crg_address(CRG_PERCTL8340_ADDR));
    } else if (dev == 1) {
        ot_reg_write32(clk_sel << bit, mask << bit, io_crg_address(CRG_PERCTL8341_ADDR));
    } else {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* VO CFG CLK */
td_s32 sys_hal_vo_cfg_clk_en(td_bool clk_en)
{
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;
    const td_u32 bit = 6; /* 6: 6bit */

    ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8340_ADDR));
    return TD_SUCCESS;
}

/* PPC */
td_s32 sys_hal_vo_core_clk_en(td_s32 dev, td_bool clk_en)
{
    const td_u32 bit = 5; /* 5: 5bit */
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;

    if ((dev == 0) || (dev == 1)) {
        ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8340_ADDR));
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 sys_hal_vo_dev_clk_en(td_s32 vo_dev, td_bool clk_en)
{
    const td_u32 bit = 4; /* 4: 4bit */
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;

    if (vo_dev == 0) {
        ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8340_ADDR));
    } else if (vo_dev == 1) {
        ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8341_ADDR));
    } else {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/* VO HD dev div_mode */
td_s32 sys_hal_vo_dev_div_mode(td_s32 vo_dev, td_u32 div_mod)
{
    const td_u32 bit = 24; /* 24: 24bit */
    const td_u32 mask = 0x3;

    if (vo_dev == 0) {
        ot_reg_write32(div_mod << bit, mask << bit, io_crg_address(CRG_PERCTL8340_ADDR));
    } else if (vo_dev == 1) {
        ot_reg_write32(div_mod << bit, mask << bit, io_crg_address(CRG_PERCTL8341_ADDR));
    } else {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/* VO SD DATE */
td_s32 sys_hal_vo_sd_date_clk_en(td_s32 vo_dev, td_bool clk_en)
{
    const td_u32 bit = 4; /* 4: 4bit */
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;

    if (vo_dev != 1) {
        return TD_FAILURE;
    }
    ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8342_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_lcd_clk_en(td_s32 vo_dev, td_bool clk_en)
{
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;
    const td_u32 bit = 27; /* 27: 27bit */
    ot_unused(vo_dev);

    ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8346_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_lcd_mclk_div(td_s32 vo_dev, td_u32 mclk_div)
{
    const td_u32 mask = 0x7ffffff; /* 0x7ffffff: clock div coef */
    ot_unused(vo_dev);

    ot_reg_write32(mclk_div, mask, io_crg_address(CRG_PERCTL8346_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_bt_clk_sel(td_s32 vo_dev, td_u32 bt_clk_ch_sel)
{
    const td_u32 bit = 12; /* 12: 12bit */
    const td_u32 mask = 0x3;

    if ((vo_dev == 0) || (vo_dev == 1)) {
        ot_reg_write32(bt_clk_ch_sel << bit, mask << bit, io_crg_address(CRG_PERCTL8348_ADDR));
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 sys_hal_vo_bt_clk_en(td_s32 vo_dev, td_bool bt_clk_en)
{
    const td_u32 bit_bt = 5; /* 5: 5bit */
    const td_u32 bit_bt_bp = 4; /* 4: 4bit */
    const td_u32 tmp = (bt_clk_en == TD_TRUE) ? 1 : 0;
    ot_unused(vo_dev);

    ot_reg_set_bit(tmp, bit_bt, io_crg_address(CRG_PERCTL8348_ADDR));
    ot_reg_set_bit(tmp, bit_bt_bp, io_crg_address(CRG_PERCTL8348_ADDR));
    return TD_SUCCESS;
}

td_s32 sys_hal_vo_vdac_clk_en(td_s32 vo_dev, td_bool clk_en)
{
    const td_u32 bit = 4; /* 4: 4bit */
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;

    if (vo_dev != 1) {
        return TD_FAILURE;
    }

    ot_reg_set_bit(tmp, bit, io_crg_address(CRG_PERCTL8350_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_hdmi_clk_en(td_s32 vo_dev, td_bool hdmi_clk_en)
{
    const td_u32 bit_pixel = 4; /* 4: 4bit */
    const td_u32 bit_vdp = 5; /* 5: 5bit */
    const td_u32 tmp = (hdmi_clk_en == TD_TRUE) ? 1 : 0;
    ot_unused(vo_dev);

    ot_reg_set_bit(tmp, bit_pixel, io_crg_address(CRG_PERCTL8351_ADDR));
    ot_reg_set_bit(tmp, bit_vdp, io_crg_address(CRG_PERCTL8351_ADDR));
    return TD_SUCCESS;
}

td_s32 sys_hal_vo_hdmi_clk_sel(td_s32 vo_dev, td_u32 clk_ch_sel)
{
    const td_u32 bit = 20; /* 20: 20bit */
    const td_u32 mask = 0x1;

    if (vo_dev == 0) {
        ot_reg_write32(clk_ch_sel << bit, mask << bit, io_crg_address(CRG_PERCTL8351_ADDR));
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 sys_hal_vo_mipi_clk_en(td_s32 vo_dev, td_bool mipi_clk_en)
{
    const td_u32 bit_pixel = 4; /* 4: 4bit */
    const td_u32 bit_vdp = 5; /* 5: 5bit */
    const td_u32 tmp = (mipi_clk_en == TD_TRUE) ? 1 : 0;
    ot_unused(vo_dev);

    ot_reg_set_bit(tmp, bit_pixel, io_crg_address(CRG_PERCTL8352_ADDR));
    ot_reg_set_bit(tmp, bit_vdp, io_crg_address(CRG_PERCTL8352_ADDR));
    return TD_SUCCESS;
}

td_s32 sys_hal_vo_mipi_clk_sel(td_s32 vo_dev, td_u32 clk_ch_sel)
{
    const td_u32 bit = 20; /* 20: 20bit */
    const td_u32 mask = 0x1;

    if ((vo_dev == 0) || (vo_dev == 1)) {
        ot_reg_write32(clk_ch_sel << bit, mask << bit, io_crg_address(CRG_PERCTL8352_ADDR));
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 sys_hal_vo_set_pll_postdiv2(td_s32 pll, td_u32 bits_set)
{
    const td_u32 bit = 28; /* 28: 28bit */
    const td_u32 mask = 0x7;

    if (pll != 0) {
        return TD_FAILURE;
    }

    ot_reg_write32(bits_set << bit, mask << bit, io_crg_pll_address(CRG_PERCTL_PLL224_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_set_pll_postdiv1(td_s32 pll, td_u32 bits_set)
{
    const td_u32 bit = 24; /* 24: 24bit */
    const td_u32 mask = 0x7;

    if (pll != 0) {
        return TD_FAILURE;
    }

    ot_reg_write32(bits_set << bit, mask << bit, io_crg_pll_address(CRG_PERCTL_PLL224_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_set_pll_frac(td_s32 pll, td_u32 bits_set)
{
    const td_u32 bit = 0;
    const td_u32 bit_dsmpd = 25; /* 25: 25bit fractional or integer mode */
    const td_u32 mask = 0xffffff;

    if (pll != 0) {
        return TD_FAILURE;
    }

    ot_reg_write32(bits_set << bit, mask << bit, io_crg_pll_address(CRG_PERCTL_PLL224_ADDR));

    if (bits_set == 0) {
        ot_reg_set_bit(1, bit_dsmpd, io_crg_pll_address(CRG_PERCTL_PLL225_ADDR));   /* 1: integer */
    } else {
        ot_reg_set_bit(0, bit_dsmpd, io_crg_pll_address(CRG_PERCTL_PLL225_ADDR));   /* 0: fractional */
    }

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_set_pll_refdiv(td_s32 pll, td_u32 bits_set)
{
    const td_u32 bit = 12;    /* 12: 12bit */
    const td_u32 mask = 0x3f;

    if (pll != 0) {
        return TD_FAILURE;
    }

    ot_reg_write32(bits_set << bit, mask << bit, io_crg_pll_address(CRG_PERCTL_PLL225_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_set_pll_fbdiv(td_s32 pll, td_u32 bits_set)
{
    const td_u32 bit = 0;     /* 0: 0bit */
    const td_u32 mask = 0xfff;

    if (pll != 0) {
        return TD_FAILURE;
    }

    ot_reg_write32(bits_set << bit, mask << bit, io_crg_pll_address(CRG_PERCTL_PLL225_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_vo_set_pll_power_ctrl(td_s32 pll, td_bool power_down)
{
    const td_u32 bit = 20;     /* 20: 20bit */
    const td_u32 tmp = (power_down == TD_TRUE) ? 1 : 0;

    if (pll != 0) {
        return TD_FAILURE;
    }

    ot_reg_set_bit(tmp, bit, io_crg_pll_address(CRG_PERCTL_PLL225_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_tde_reset_sel(td_bool reset)
{
    const td_u32 tmp = (reset == TD_TRUE) ? 1 : 0;

    ot_reg_set_bit(tmp, 0, io_crg_address(CRG_PERCTL10064_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_tde_clk_en(td_bool clk_en)
{
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;

    ot_reg_set_bit(tmp, 4, io_crg_address(CRG_PERCTL10064_ADDR)); /* 4: offset */
    ot_reg_set_bit(1, 12, io_crg_address(CRG_PERCTL10064_ADDR)); /* 12: offset 1:475MHZ */

    return TD_SUCCESS;
}

td_phys_addr_t sys_hal_get_efuse_addr(td_void)
{
    return TD_NULL;
}

td_phys_addr_t sys_hal_get_sys_base_addr(td_void)
{
    return (td_phys_addr_t)(td_uintptr_t)g_reg_sys_base_addr;
}

td_s32 sys_hal_get_custom_code(td_u32 *custom_code)
{
    *custom_code = (td_u32) * (volatile td_u32 *)(((td_u8 *)g_reg_sys_base_addr) + 0x1100);
    return TD_SUCCESS;
}

td_s32 sys_hal_aio_clk_sel(td_u32 clk_sel)
{
    ot_reg_set_bit(clk_sel, 12, io_crg_address(CRG_PERCTL10784_ADDR)); /* 12: AIAO */

    return TD_SUCCESS;
}

td_s32 sys_hal_aio_reset_sel(td_bool reset)
{
    const td_u32 tmp = (reset == TD_TRUE) ? 1 : 0;

    ot_reg_set_bit(tmp, 0, io_crg_address(CRG_PERCTL10784_ADDR));

    return TD_SUCCESS;
}

td_s32 sys_hal_aio_clk_en(td_bool clk_en)
{
    const td_u32 tmp = (clk_en == TD_TRUE) ? 1 : 0;

    ot_reg_set_bit(tmp, 4, io_crg_address(CRG_PERCTL10784_ADDR)); /* 4: AIAO PLL */
    ot_reg_set_bit(tmp, 5, io_crg_address(CRG_PERCTL10784_ADDR)); /* 5: AIAO */

    return TD_SUCCESS;
}

td_s32 sys_hal_acodec_reset_sel(td_bool reset)
{
    const td_u32 tmp = (reset == TD_TRUE) ? 1 : 0;

    ot_reg_set_bit(tmp, 0, io_crg_address(CRG_PERCTL10912_ADDR));
    ot_reg_set_bit(tmp, 1, io_crg_address(CRG_PERCTL10912_ADDR));

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_SVP_NPU_V2R1
td_s32 sys_hal_svp_npu0_reset_sel(td_bool reset)
{
    const td_u32 tmp = (reset == TD_TRUE) ? 1 : 0;

    ot_reg_set_bit(tmp, 0, io_crg_address(CRG_PERCTL4898_ADDR)); /* cpm */
    ot_reg_set_bit(tmp, 0, io_crg_address(CRG_PERCTL5029_ADDR)); /* ffs */
    ot_reg_set_bit(tmp, 0, io_crg_address(CRG_PERCTL6592_ADDR)); /* pqp */

    return TD_SUCCESS;
}
#endif

static inline td_s32 sys_hal_remap_reg(td_void **reg_ptr, td_phys_addr_t phys_addr, td_ulong size)
{
    if (*reg_ptr == TD_NULL) {
        *reg_ptr = (td_void *)osal_ioremap((td_ulong)phys_addr, size);
        if (*reg_ptr == TD_NULL) {
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

td_s32 sys_hal_init(td_void)
{
    if (osal_spin_lock_init(&g_crg_spin_lock) < 0) {
        osal_printk("spinlock init fail, line: %d. \n", __LINE__);
        return TD_FAILURE;
    }

    if (sys_hal_remap_reg(&g_reg_crg_base_addr, CRG_REGS_ADDR + CRG_REGS_ADDR_OFFSET, CRG_REGS_SIZE) != TD_SUCCESS) {
        osal_printk("remap crg reg fail, line: %d.\n", __LINE__);
        goto sys_hal_fail;
    }

    if (sys_hal_remap_reg(&g_reg_crg_pll_addr, CRG_REGS_ADDR, CRG_REGS_ADDR_OFFSET) != TD_SUCCESS) {
        osal_printk("remap crg pll reg fail, line: %d.\n", __LINE__);
        goto sys_hal_fail;
    }

    if (sys_hal_remap_reg(&g_reg_sys_base_addr, SYS_REGS_ADDR, SYS_REGS_SIZE) != TD_SUCCESS) {
        osal_printk("remap sys reg fail, line: %d.\n", __LINE__);
        goto sys_hal_fail;
    }

    if (sys_hal_remap_reg(&g_reg_ddr0_base_addr, DDRC0_REG_ADDR, DDRC_REGS_SIZE) != TD_SUCCESS) {
        osal_printk("remap ddr0 reg fail, line: %d.\n", __LINE__);
        goto sys_hal_fail;
    }

    if (sys_hal_remap_reg(&g_reg_misc_base_addr, MISC_REGS_ADDR, MISC_REGS_SIZE) != TD_SUCCESS) {
        osal_printk("remap MISC reg fail, line: %d.\n", __LINE__);
        goto sys_hal_fail;
    }
    if (sys_hal_remap_reg(&g_reg_otp_user_base_addr, OTP_USER_REGS_ADDR, OTP_USER_REGS_SIZE) != TD_SUCCESS) {
        goto sys_hal_fail;
    }

    return TD_SUCCESS;

sys_hal_fail:
    sys_hal_exit();
    return TD_FAILURE;
}

td_void sys_hal_exit(td_void)
{
    if (g_sys_reg_by_dts == TD_FALSE) {
        if (g_reg_crg_base_addr != TD_NULL) {
            osal_iounmap(g_reg_crg_base_addr, CRG_REGS_SIZE);
            g_reg_crg_base_addr = TD_NULL;
        }
        if (g_reg_sys_base_addr != TD_NULL) {
            osal_iounmap(g_reg_sys_base_addr, SYS_REGS_SIZE);
            g_reg_sys_base_addr = TD_NULL;
        }
        if (g_reg_ddr0_base_addr != TD_NULL) {
            osal_iounmap(g_reg_ddr0_base_addr, DDRC_REGS_SIZE);
            g_reg_ddr0_base_addr = TD_NULL;
        }

        if (g_reg_misc_base_addr != TD_NULL) {
            osal_iounmap(g_reg_misc_base_addr, MISC_REGS_SIZE);
            g_reg_misc_base_addr = TD_NULL;
        }
    }
    if (g_reg_crg_pll_addr != TD_NULL) {
        osal_iounmap((td_void *)g_reg_crg_pll_addr, CRG_REGS_ADDR_OFFSET);
        g_reg_crg_pll_addr = TD_NULL;
    }

    if (g_reg_otp_user_base_addr != TD_NULL) {
        osal_iounmap(g_reg_otp_user_base_addr, OTP_USER_REGS_SIZE);
        g_reg_otp_user_base_addr = TD_NULL;
    }

    osal_spin_lock_destroy(&g_crg_spin_lock);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
