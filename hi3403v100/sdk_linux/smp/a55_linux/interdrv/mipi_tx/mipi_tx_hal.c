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

#include "mipi_tx_hal.h"
#include "ot_osal.h"
#include "type.h"
#include "mipi_tx.h"
#include "mipi_tx_reg.h"
#include "mipi_tx_def.h"
#include <linux/types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if mipi_tx_desc("pub")

static volatile mipi_tx_regs_type_t *g_mipi_tx_regs_va = NULL;
static unsigned int g_mipi_tx_irq_num = MIPI_TX_IRQ;
static unsigned int g_actual_phy_data_rate;
static unsigned int g_reg_map_flag = 0;

void mipi_tx_drv_set_irq_num(unsigned int irq_num)
{
    g_mipi_tx_irq_num = irq_num;
}

void mipi_tx_drv_set_regs(const mipi_tx_regs_type_t *regs)
{
    g_mipi_tx_regs_va = (mipi_tx_regs_type_t *)regs;
}

static void mipi_tx_set_actual_phy_data_rate(unsigned int actual_date_rate)
{
    g_actual_phy_data_rate = actual_date_rate;
}

static unsigned int mipi_tx_get_actual_phy_data_rate(void)
{
    return g_actual_phy_data_rate;
}

static void write_reg32(unsigned long addr, unsigned int value, unsigned int mask)
{
    unsigned int t;

    t = osal_readl(addr);
    t &= ~mask;
    t |= value & mask;
    osal_writel(t, addr);
}

static void set_phy_reg(unsigned int addr, unsigned char value)
{
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg1.u32 = (0x10000 + addr);
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x2;
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x0;
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg1.u32 = value;
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x2;
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x0;
    osal_isb();
    osal_dsb();
    osal_dmb();
}

void mipi_tx_drv_get_dev_status(mipi_tx_dev_phy_t *mipi_tx_phy_ctx)
{
    volatile reg_hori0_det hori0_det;
    volatile reg_hori1_det hori1_det;
    volatile reg_vert_det vert_det;
    volatile reg_vsa_det vsa_det;

    hori0_det.u32 = g_mipi_tx_regs_va->hori0_det.u32;
    hori1_det.u32 = g_mipi_tx_regs_va->hori1_det.u32;
    vert_det.u32 = g_mipi_tx_regs_va->vert_det.u32;
    vsa_det.u32 = g_mipi_tx_regs_va->vsa_det.u32;

    mipi_tx_phy_ctx->hact_det = hori0_det.bits.hact_det;
    mipi_tx_phy_ctx->hall_det = hori0_det.bits.hline_det;
    mipi_tx_phy_ctx->hbp_det  = hori1_det.bits.hbp_det;
    mipi_tx_phy_ctx->hsa_det  = hori1_det.bits.hsa_det;

    mipi_tx_phy_ctx->vact_det = vert_det.bits.vact_det;
    mipi_tx_phy_ctx->vall_det = vert_det.bits.vall_det;
    mipi_tx_phy_ctx->vsa_det  = vsa_det.bits.vsa_det;
}

#endif

#if mipi_tx_desc("phy pll")

#ifdef OT_FPGA
static void mipi_tx_drv_set_phy_pll_setx(unsigned int phy_data_rate)
{
    unsigned char pll_set0;
    unsigned char pll_set1;
    unsigned char pll_set2;
    unsigned char pll_set3;
    unsigned char pll_set4;

    /* step2 : */
    /* pll_set0 */
    pll_set0 = 0x0;
    set_phy_reg(PLL_SET_0X14, pll_set0);

    /* pll_set1 */
    pll_set1 = 0xB;
    set_phy_reg(PLL_SET_0X15, pll_set1);

    /* pll_set2 */
    pll_set2 = 0x21;
    set_phy_reg(PLL_SET_0X16, pll_set2);

    /* pll_set3 */
    pll_set3 = 0x1;
    set_phy_reg(PLL_SET_0X17, pll_set3);

    /* pll_set4 */
    pll_set4 = 0x28;
    set_phy_reg(PLL_SET_0X1E, pll_set4);

#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy pll info=======\n");
    osal_printk("pll_set0(0x14): 0x%x\n", pll_set0);
    osal_printk("pll_set1(0x15): 0x%x\n", pll_set1);
    osal_printk("pll_set2(0x16): 0x%x\n", pll_set2);
    osal_printk("pll_set3(0x17): 0x%x\n", pll_set3);
    osal_printk("pll_set4(0x1e): 0x%x\n", pll_set4);
    osal_printk("=========================\n");
#endif
}

#else

static int mipi_tx_power(unsigned int base, unsigned int exp)
{
    int result = 1;
    while (exp) {
        if (exp & 0x01) {
            result = result * base;
        }
        base = base * base;
        exp = exp >> 1;
    }

    return result;
}

static unsigned char mipi_tx_drv_get_phy_pll_set_0x63(unsigned int phy_data_rate, unsigned char pll_set_0x65)
{
    unsigned char set_0x63;

    set_0x63 = mipi_tx_ceil(phy_data_rate, MIPI_TX_REF_CLK) *
        mipi_tx_power(2, (pll_set_0x65 % 8)) % 256; /* 2, 8, 256: pll cal param */

    return set_0x63;
}

static unsigned char mipi_tx_drv_get_phy_pll_set_0x64(unsigned int phy_data_rate)
{
    unsigned char set_0x64;

    set_0x64 = mipi_tx_ceil(phy_data_rate, MIPI_TX_REF_CLK) / 256; /* 256 pll cal param */

    return set_0x64;
}

static void mipi_tx_drv_set_phy_pll_setx(unsigned int phy_data_rate)
{
    unsigned char pll_set_0x63;
    unsigned char pll_set_0x64;
    const unsigned char pll_set_0x65 = 0x0; /* always zero */

    /* pll set_0x63, set_0x64, set_0x65 */
    pll_set_0x63 = mipi_tx_drv_get_phy_pll_set_0x63(phy_data_rate, pll_set_0x65);
    pll_set_0x64 = mipi_tx_drv_get_phy_pll_set_0x64(phy_data_rate);

    set_phy_reg(PLL_SET_0X63, pll_set_0x63);
    set_phy_reg(PLL_SET_0X64, pll_set_0x64);
    set_phy_reg(PLL_SET_0X65, pll_set_0x65);

#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy pll info=======\n");
    osal_printk("pll_set_0x63(0x14): 0x%x\n", pll_set_0x63);
    osal_printk("pll_set_0x64(0x15): 0x%x\n", pll_set_0x64);
    osal_printk("pll_set_0x65(0x16): 0x%x\n", pll_set_0x65);
    osal_printk("=========================\n");
#endif
}
#endif /* end of #ifdef OT_FPGA */

#endif /* end of #if mipi_tx_desc("phy pll") */

#if mipi_tx_desc("phy time parameter")

#ifdef OT_FPGA

static void mipi_tx_drv_get_phy_timing_parameters(mipi_tx_phy_timing_parameters *tp)
{
    tp->data_tpre_delay = DATA0_TPRE_DELAY_VALUE;
    tp->clk_post_delay = CLK_POST_DELAY_VALUE;
    tp->clk_tlpx = CLK_TLPX_VALUE;
    tp->clk_tclk_prepare = CLK_TCLK_PREPARE_VALUE;
    tp->clk_tclk_zero = CLK_TCLK_ZERO_VALUE;
    tp->clk_tclk_trail = CLK_TCLK_TRAIL_VALUE;
    tp->data_tlpx = DATA0_TLPX_VALUE;
    tp->data_ths_prepare = DATA0_THS_PREPARE_VALUE;
    tp->data_ths_zero = DATA0_THS_ZERO_VALUE;
    tp->data_ths_trail = DATA0_THS_TRAIL_VALUE;
}

#else

static inline unsigned char mipi_tx_drv_get_phy_timing_parameters_by_type(unsigned int timing_type)
{
    unsigned int actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();

    return (actual_phy_data_rate * timing_type + ROUNDUP_VALUE) / INNER_PEROID;
}

static void mipi_tx_drv_get_phy_timing_parameters(mipi_tx_phy_timing_parameters *tp)
{
    /* DATA0~3 TPRE-DELAY */
    tp->data_tpre_delay = mipi_tx_drv_get_phy_timing_parameters_by_type(TPRE_DELAY);

    /* CLK_TLPX */
    tp->clk_tlpx = mipi_tx_drv_get_phy_timing_parameters_by_type(TLPX);

    /* CLK_TCLK_PREPARE */
    tp->clk_tclk_prepare = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_PREPARE);

    /* CLK_TCLK_ZERO */
    tp->clk_tclk_zero = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_ZERO);

    /* CLK_TCLK_TRAIL */
    tp->clk_tclk_trail = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_TRAIL);

    /* DATA_TLPX */
    tp->data_tlpx = mipi_tx_drv_get_phy_timing_parameters_by_type(TLPX);

    /* DATA_THS_PREPARE */
    tp->data_ths_prepare = mipi_tx_drv_get_phy_timing_parameters_by_type(THS_PREPARE);

    /* DATA_THS_ZERO */
    tp->data_ths_zero = mipi_tx_drv_get_phy_timing_parameters_by_type(THS_ZERO);

    /* DATA_THS_TRAIL */
    tp->data_ths_trail = mipi_tx_drv_get_phy_timing_parameters_by_type(THS_TRAIL);

    /* CLK POST DELAY */
    tp->clk_post_delay = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_POST) +
        tp->data_ths_trail + 4; /* 4: 4hs is compensate */
}
#endif

/*
 * set global operation timing parameters.
 */
static void mipi_tx_drv_set_phy_timing_parameters(const mipi_tx_phy_timing_parameters *tp)
{
    /* DATA0~3 TPRE-DELAY */
    set_phy_reg(DATA0_TPRE_DELAY, tp->data_tpre_delay);
    set_phy_reg(DATA1_TPRE_DELAY, tp->data_tpre_delay);
    set_phy_reg(DATA2_TPRE_DELAY, tp->data_tpre_delay);
    set_phy_reg(DATA3_TPRE_DELAY, tp->data_tpre_delay);

    /* CLK_POST_DELAY */
    set_phy_reg(CLK_POST_DELAY, tp->clk_post_delay);

    /* CLK_TLPX */
    set_phy_reg(CLK_TLPX, tp->clk_tlpx);

    /* CLK_TCLK_PREPARE */
    set_phy_reg(CLK_TCLK_PREPARE, tp->clk_tclk_prepare);

    /* CLK_TCLK_ZERO */
    set_phy_reg(CLK_TCLK_ZERO, tp->clk_tclk_zero);

    /* CLK_TCLK_TRAIL */
    set_phy_reg(CLK_TCLK_TRAIL, tp->clk_tclk_trail);

    /*
     * DATA_TLPX
     * DATA_THS_PREPARE
     * DATA_THS_ZERO
     * DATA_THS_TRAIL
     */
    set_phy_reg(DATA0_TLPX, tp->data_tlpx);
    set_phy_reg(DATA0_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA0_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA0_THS_TRAIL, tp->data_ths_trail);
    set_phy_reg(DATA1_TLPX, tp->data_tlpx);
    set_phy_reg(DATA1_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA1_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA1_THS_TRAIL, tp->data_ths_trail);
    set_phy_reg(DATA2_TLPX, tp->data_tlpx);
    set_phy_reg(DATA2_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA2_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA2_THS_TRAIL, tp->data_ths_trail);
    set_phy_reg(DATA3_TLPX, tp->data_tlpx);
    set_phy_reg(DATA3_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA3_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA3_THS_TRAIL, tp->data_ths_trail);

#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy timing parameters=======\n");
    osal_printk("data_tpre_delay(0x30/40/50/60): 0x%x\n", tp->data_tpre_delay);
    osal_printk("clk_post_delay(0x21): 0x%x\n", tp->clk_post_delay);
    osal_printk("clk_tlpx(0x22): 0x%x\n", tp->clk_tlpx);
    osal_printk("clk_tclk_prepare(0x23): 0x%x\n", tp->clk_tclk_prepare);
    osal_printk("clk_tclk_zero(0x24): 0x%x\n", tp->clk_tclk_zero);
    osal_printk("clk_tclk_trail(0x25): 0x%x\n", tp->clk_tclk_trail);
    osal_printk("data_tlpx(0x32/42/52/62): 0x%x\n", tp->data_tlpx);
    osal_printk("data_ths_prepare(0x33/43/53/63): 0x%x\n", tp->data_ths_prepare);
    osal_printk("data_ths_zero(0x34/44/54/64): 0x%x\n", tp->data_ths_zero);
    osal_printk("data_ths_trail(0x35/45/55/65): 0x%x\n", tp->data_ths_trail);
    osal_printk("=========================\n");
#endif
}

#endif /* end of #if mipi_tx_desc("phy time parameter") */

#if mipi_tx_desc("phy cfg")

#ifdef OT_FPGA
static void mipi_tx_drv_set_phy_hs_lp_switch_time(const mipi_tx_phy_timing_parameters *tp)
{
    /* data lp2hs,hs2lp time */
    g_mipi_tx_regs_va->datalane_time.u32 = 0x6001D;
    /* clk lp2hs,hs2lp time */
    g_mipi_tx_regs_va->clklane_time.u32 = 0x26001E;

#ifdef MIPI_TX_DEBUG
    osal_printk("datalane_time(0x9C): 0x%x\n", g_mipi_tx_regs_va->datalane_time.u32);
    osal_printk("clklane_time(0x98): 0x%x\n", g_mipi_tx_regs_va->clklane_time.u32);
#endif
}

#else

/*
 * set data lp2hs,hs2lp time
 * set clk lp2hs,hs2lp time
 * unit: hsclk
 */
static void mipi_tx_drv_set_phy_hs_lp_switch_time(const mipi_tx_phy_timing_parameters *tp)
{
    unsigned char clk_post_delay = tp->clk_post_delay;
    unsigned char clk_tlpx = tp->clk_tlpx;
    unsigned char clk_tclk_prepare = tp->clk_tclk_prepare;
    unsigned char clk_tclk_zero = tp->clk_tclk_zero;
    unsigned char data_tpre_delay = tp->data_tpre_delay;
    unsigned char data_tlpx = tp->data_tlpx;
    unsigned char data_ths_prepare = tp->data_ths_prepare;
    unsigned char data_ths_zero = tp->data_ths_zero;
    unsigned char data_ths_trail = tp->data_ths_trail;

    /* data lp2hs,hs2lp time */
    g_mipi_tx_regs_va->datalane_time.u32 = data_tpre_delay + data_tlpx + data_ths_prepare + data_ths_zero +
        4 + (data_ths_trail + 1) * 65536; /* 4, 65536: phy timing params */

    /* clk lp2hs,hs2lp time */
    g_mipi_tx_regs_va->clklane_time.u32 = clk_tlpx + clk_tclk_prepare + clk_tclk_zero +
        3 + (clk_post_delay + 1 + data_ths_trail) * 65536; /* 3, 65536: phy timing params */

#ifdef MIPI_TX_DEBUG
    osal_printk("datalane_time(0x9C): 0x%x\n", g_mipi_tx_regs_va->datalane_time.u32);
    osal_printk("clklane_time(0x98): 0x%x\n", g_mipi_tx_regs_va->clklane_time.u32);
#endif
}
#endif

static void mipi_tx_drv_set_input_enable(unsigned char input_en)
{
    volatile reg_operation_mode operation_mode = {0};

    operation_mode.u32 = g_mipi_tx_regs_va->operation_mode.u32;
    if (input_en == 0) {
        operation_mode.u32 = 0;
        g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;
        return;
    }

    operation_mode.bits.mem_ck_en = 1;
    operation_mode.bits.input_en = 1;
    operation_mode.bits.hss_abnormal_rst = 1;
    operation_mode.bits.read_empty_vsync_en = 1;
    g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;
}

void mipi_tx_drv_set_phy_cfg(void)
{
    unsigned int actual_data_rate = mipi_tx_get_actual_phy_data_rate();
    mipi_tx_phy_timing_parameters tp = {0};

    /*
     * reset phy before setting phy:
     * forcepll: pll enable
     * enableclk: enable clk lane
     * rstz: unreset
     * shutdownz: powerup
     */
    g_mipi_tx_regs_va->phy_ctrl.u32 = 0x0;
    osal_udelay(1000); /* 1000: 1000us */

    /* disable input, done in mipi_tx_drv_set_controller_cfg */
    mipi_tx_drv_set_input_enable(0);

    /* pwr_up unreset */
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    osal_udelay(1);
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x1;

    /* set phy pll parameters setx to generate data rate */
    mipi_tx_drv_set_phy_pll_setx(actual_data_rate);

    /* get global operation timing parameters */
    mipi_tx_drv_get_phy_timing_parameters(&tp);

    /* set global operation timing parameters */
    mipi_tx_drv_set_phy_timing_parameters(&tp);

    /*
     * dsi controller
     * set hs switch to lp and lp switch to hs time
     */
    mipi_tx_drv_set_phy_hs_lp_switch_time(&tp);

    /*
     * unreset phy after setting phy:
     * forcepll: pll enable
     * enableclk: enable clk lane
     * rstz: unreset
     * shutdownz: powerup
     */
    g_mipi_tx_regs_va->phy_ctrl.u32 = 0xF;

    osal_msleep(1);
}

#endif /* #if mipi_tx_desc("phy cfg") */

#if mipi_tx_desc("controller cfg")

static void set_output_format(const combo_dev_cfg_t *dev_cfg)
{
    int color_coding = 0;

    if (dev_cfg->out_mode == OUT_MODE_CSI) {
        if (dev_cfg->out_format == OUT_FORMAT_YUV420_8BIT_NORMAL) {
            color_coding = MIPI_TX_CSI_YUV420_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV420_8BIT_LEGACY) {
            color_coding = MIPI_TX_CSI_LEGACY_YUV420_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV422_8BIT) {
            color_coding = MIPI_TX_CSI_YUV422_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_888) {
            color_coding = MIPI_TX_CSI_RGB888;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_8BIT) {
            color_coding = MIPI_TX_CSI_RAW8;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_10BIT) {
            color_coding = MIPI_TX_CSI_RAW10;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_12BIT) {
            color_coding = MIPI_TX_CSI_RAW12;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_14BIT) {
            color_coding = MIPI_TX_CSI_RAW14;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_16BIT) {
            color_coding = MIPI_TX_CSI_RAW16;
        }
    } else {
        if (dev_cfg->out_format == OUT_FORMAT_RGB_16BIT) {
            color_coding = MIPI_TX_DSI_RGB_16BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_18BIT) {
            color_coding = MIPI_TX_DSI_RGB_18BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_18BIT_LOOSELY) {
            color_coding = MIPI_TX_DSI_RGB_18BIT_LOOSELY;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_24BIT) {
            color_coding = MIPI_TX_DSI_RGB_24BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV420_12BIT) {
            color_coding = MIPI_TX_DSI_YCBCR420_12BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV422_16BIT) {
            color_coding = MIPI_TX_DSI_YCBCR422_16BIT;
        }
    }

    g_mipi_tx_regs_va->data_type.u32 = color_coding;
#ifdef MIPI_TX_DEBUG
    osal_printk("set_output_format: 0x%x\n", color_coding);
#endif
}

static void mipi_tx_hal_set_cmd_video_mode(cmd_video_mode cmd_video)
{
    volatile reg_mode_cfg mode_cfg;
    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;
    mode_cfg.bits.cmd_video_mode = cmd_video;
    g_mipi_tx_regs_va->mode_cfg.u32 = mode_cfg.u32;
}

static void mipi_tx_hal_set_video_mode_type(unsigned int video_mode_type)
{
    volatile reg_mode_cfg mode_cfg;

    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;
    mode_cfg.bits.video_mode_type = video_mode_type;
    g_mipi_tx_regs_va->mode_cfg.u32 = mode_cfg.u32;
}

static void set_video_mode_cfg(const combo_dev_cfg_t *dev_cfg)
{
    int video_mode_type;
    volatile reg_read_memory_delay_ctrl read_memory_delay_ctrl;

    if (dev_cfg->video_mode == NON_BURST_MODE_SYNC_PULSES) {
        video_mode_type = 0; /* 0: Non-burst with sync pulse */
    } else if (dev_cfg->video_mode == NON_BURST_MODE_SYNC_EVENTS) {
        video_mode_type = 1; /* 1: Non-burst with sync */
    } else {
        video_mode_type = 2; /* 2: burst mode */
    }

    if ((dev_cfg->out_mode == OUT_MODE_CSI) ||
        (dev_cfg->out_mode == OUT_MODE_DSI_CMD)) {
        video_mode_type = 2; /* 2: burst mode */
    }

    /*
     * mode config
     * other: burst mode
     * 00: Non-burst with sync pulse
     * 01: Non-burst with sync
     */
    mipi_tx_hal_set_cmd_video_mode(COMMAND_MODE);
    mipi_tx_hal_set_video_mode_type(video_mode_type);

    g_mipi_tx_regs_va->video_lp_en.u32 = 0x3f;

    /* delay value setting. */
    read_memory_delay_ctrl.u32 = g_mipi_tx_regs_va->read_memory_delay_ctrl.u32;
    if ((dev_cfg->video_mode == NON_BURST_MODE_SYNC_PULSES) ||
        (dev_cfg->video_mode == NON_BURST_MODE_SYNC_EVENTS)) {
        read_memory_delay_ctrl.bits.delay_abnormal = 0x1;
    } else if (dev_cfg->video_mode == BURST_MODE) {
        read_memory_delay_ctrl.bits.delay_abnormal = 0x0;
    }
    g_mipi_tx_regs_va->read_memory_delay_ctrl.u32 = read_memory_delay_ctrl.u32;
}

static void set_timing_config(const combo_dev_cfg_t *dev_cfg)
{
    unsigned int hsa_time;
    unsigned int hbp_time;
    unsigned int hline_time;
    unsigned int actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();
    const unsigned int htime_tmp = 1000 / 8; /* 1000, 8: 1000Hz / 8bit */

    if (dev_cfg->pixel_clk == 0) {
        osal_printk("dev_cfg->pixel_clk is 0, illegal.\n");
        return;
    }

    hsa_time = actual_phy_data_rate * dev_cfg->sync_info.hsa_pixels * htime_tmp / dev_cfg->pixel_clk;
    hbp_time = actual_phy_data_rate * dev_cfg->sync_info.hbp_pixels  * htime_tmp / dev_cfg->pixel_clk;
    hline_time = actual_phy_data_rate * (dev_cfg->sync_info.hsa_pixels +
        dev_cfg->sync_info.hact_pixels +
        dev_cfg->sync_info.hbp_pixels +
        dev_cfg->sync_info.hfp_pixels) * htime_tmp / dev_cfg->pixel_clk;

    g_mipi_tx_regs_va->videom_hsa_size.u32 = hsa_time;
    g_mipi_tx_regs_va->videom_hbp_size.u32 = hbp_time;
    g_mipi_tx_regs_va->videom_hline_size.u32 = hline_time;

    g_mipi_tx_regs_va->videom_vsa_lines.u32 = dev_cfg->sync_info.vsa_lines;
    g_mipi_tx_regs_va->videom_vbp_lines.u32 = dev_cfg->sync_info.vbp_lines;
    g_mipi_tx_regs_va->videom_vfp_lines.u32 = dev_cfg->sync_info.vfp_lines;
    g_mipi_tx_regs_va->videom_vactive_lines.u32 = dev_cfg->sync_info.vact_lines;

#ifdef MIPI_TX_DEBUG
    osal_printk("hsa_pixels(0x48): 0x%x\n", dev_cfg->sync_info.hsa_pixels);
    osal_printk("hbp_pixels(0x4c): 0x%x\n", dev_cfg->sync_info.hbp_pixels);
    osal_printk("hact_pixels(0x50): 0x%x\n", dev_cfg->sync_info.hact_pixels);
    osal_printk("hfp_pixels(0x50): 0x%x\n", dev_cfg->sync_info.hfp_pixels);
    osal_printk("vact_lines(0x54): 0x%x\n", dev_cfg->sync_info.vact_lines);
    osal_printk("vbp_lines(0x58): 0x%x\n", dev_cfg->sync_info.vbp_lines);
    osal_printk("vfp_lines(0x5c): 0x%x\n", dev_cfg->sync_info.vfp_lines);
    osal_printk("vsa_lines(0x60): 0x%x\n", dev_cfg->sync_info.vsa_lines);
#endif
}

static void mipi_tx_set_lane_id(int lane_idx, short lane_id)
{
    reg_lane_id lane_id_reg;
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;

    switch (lane_id) {
        case 0:
            lane_id_reg.bits.lane0_id = lane_idx;
            break;

        case 1:
            lane_id_reg.bits.lane1_id = lane_idx;
            break;

        case 2: /* 2: lane2 */
            lane_id_reg.bits.lane2_id = lane_idx;
            break;

        case 3: /* 3: lane3 */
            lane_id_reg.bits.lane3_id = lane_idx;
            break;

        default:
            break;
    }
    g_mipi_tx_regs_va->lane_id.u32 = lane_id_reg.u32;
}

static void mipi_tx_set_disable_lane_id(int lane_idx, short invalid_lane_num)
{
    reg_lane_id lane_id_reg;
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;

    switch (invalid_lane_num) {
        case 1:
            lane_id_reg.bits.lane3_id = lane_idx;
            break;

        case 2: /* 2: 2lanes */
            lane_id_reg.bits.lane2_id = lane_idx;
            break;

        case 3: /* 3: 3lanes */
            lane_id_reg.bits.lane1_id = lane_idx;
            break;

        default:
            break;
    }

    g_mipi_tx_regs_va->lane_id.u32 = lane_id_reg.u32;
}

static void set_lane_config(const short lane_id[], int lane_id_len)
{
    const int max_lane_num = LANE_MAX_NUM;
    int i;
    int invalid_lane_num = 0;
    const int unused_id = 3; /* 3, for unused lane */
#ifdef MIPI_TX_DEBUG
    reg_lane_id lane_id_reg;
#endif

    g_mipi_tx_regs_va->lane_num.u32 = mipi_tx_get_lane_num(lane_id, lane_id_len) - 1;

    for (i = 0; i < max_lane_num; i++) {
        if (lane_id[i] == MIPI_TX_DISABLE_LANE_ID) {
            invalid_lane_num++;
            mipi_tx_set_disable_lane_id(unused_id, invalid_lane_num);
            continue;
        }
        mipi_tx_set_lane_id(i, lane_id[i]);
    }

#ifdef MIPI_TX_DEBUG
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;
    osal_printk("LANE_ID: 0x%x\n", lane_id_reg.u32);
#endif
}

static void mipi_tx_hal_set_request_hsclk_en(unsigned char clk_en)
{
    volatile reg_clklane_ctrl clk_lane_ctrl;

    clk_lane_ctrl.u32 = g_mipi_tx_regs_va->clklane_ctrl.u32;
    clk_lane_ctrl.bits.txrequesthsclk = clk_en;
    g_mipi_tx_regs_va->clklane_ctrl.u32 = clk_lane_ctrl.u32;
}

static void mipi_tx_hal_set_clklane_continue_en(unsigned char clk_en)
{
    volatile reg_clklane_ctrl clk_lane_ctrl;

    clk_lane_ctrl.u32 = g_mipi_tx_regs_va->clklane_ctrl.u32;
    clk_lane_ctrl.bits.clklane_continue = clk_en;
    g_mipi_tx_regs_va->clklane_ctrl.u32 = clk_lane_ctrl.u32;
}

static void mipi_tx_drv_phy_reset(void)
{
    volatile reg_phy_ctrl phy_ctrl;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x0;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
}

static void mipi_tx_drv_phy_unreset(void)
{
    volatile reg_phy_ctrl phy_ctrl;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x1;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
}

static void mipi_tx_drv_set_clkmgr_cfg(void)
{
    unsigned int actual_phy_data_rate;

    actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();
    if (actual_phy_data_rate / 160 < 2) { /* 160: half of 320, 2: half */
        g_mipi_tx_regs_va->crg_cfg.u32 = 0x102; /* 258 */
    } else {
        g_mipi_tx_regs_va->crg_cfg.u32 = 0x100 +
            (actual_phy_data_rate + 160 - 1) / 160; /* align to 160 */
    }
#ifdef MIPI_TX_DEBUG
    osal_printk("crg_cfg: 0x%x\n", g_mipi_tx_regs_va->crg_cfg.u32);
#endif
}

static void mipi_tx_drv_set_controller_cmd_tran_mode(unsigned char low_power_en)
{
    volatile reg_command_tran_mode command_tran_mode;

    command_tran_mode.u32 = g_mipi_tx_regs_va->command_tran_mode.u32;
    command_tran_mode.bits.max_rd_pkt_size_tran = low_power_en;
    command_tran_mode.bits.gen_sw_0p = low_power_en;
    command_tran_mode.bits.gen_sw_1p = low_power_en;
    command_tran_mode.bits.gen_sw_2p = low_power_en;
    command_tran_mode.bits.gen_sr_0p = low_power_en;
    command_tran_mode.bits.gen_sr_1p = low_power_en;
    command_tran_mode.bits.gen_sr_2p = low_power_en;
    command_tran_mode.bits.gen_lw = low_power_en;
    command_tran_mode.bits.dcs_sw_0p = low_power_en;
    command_tran_mode.bits.dcs_sw_1p = low_power_en;
    command_tran_mode.bits.dcs_sr_0p = low_power_en;
    command_tran_mode.bits.dcs_lw = low_power_en;
    g_mipi_tx_regs_va->command_tran_mode.u32 = command_tran_mode.u32;
}

static void mipi_tx_drv_set_controller_cmd_datatype(const combo_dev_cfg_t *dev_cfg)
{
    volatile reg_datatype0 datatype0;
    unsigned int dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_16BIT;
    if (dev_cfg->out_mode == OUT_MODE_CSI) {
        if (dev_cfg->out_format == OUT_FORMAT_YUV420_8BIT_NORMAL) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_YUV420_8BIT_NORMAL;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV420_8BIT_LEGACY) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_YUV420_8BIT_LEGACY;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV422_8BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_YUV422_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_888) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RBG_888;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_8BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_10BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_10BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_12BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_12BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_14BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_14BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_16BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_16BIT;
        }
        datatype0.u32 = g_mipi_tx_regs_va->datatype0.u32;
        datatype0.bits.dt_pixel = dt_pixel;
        datatype0.bits.dt_hss = MIPI_TX_CSI_DATA_TYPE_LINE_START;
        datatype0.bits.dt_vse = MIPI_TX_CSI_DATA_TYPE_FRAME_END;
        datatype0.bits.dt_vss = MIPI_TX_CSI_DATA_TYPE_FRAME_START;
        g_mipi_tx_regs_va->datatype0.u32 = datatype0.u32;
        g_mipi_tx_regs_va->datatype1.u32 = 0x31081909;
        g_mipi_tx_regs_va->csi_ctrl.u32 = 0x1111;
    } else {
        g_mipi_tx_regs_va->datatype0.u32 = 0x111213D;
        g_mipi_tx_regs_va->datatype1.u32 = 0x31081909;
        g_mipi_tx_regs_va->csi_ctrl.u32 = 0x10100;
    }
}

static void mipi_tx_cal_actual_phy_data_rate(unsigned int input_data_rate)
{
    unsigned int data_rate;
    unsigned int actual_data_rate;
    data_rate = input_data_rate * 110 / 100; /* 110, 100: data_rate * 1.1 for datatype, ECC, Checksum, lp~hs turn */

    actual_data_rate = mipi_tx_align_up(data_rate, MIPI_TX_REF_CLK);
    mipi_tx_set_actual_phy_data_rate(actual_data_rate);
}

void mipi_tx_drv_set_controller_cfg(const combo_dev_cfg_t *dev_cfg)
{
    /* set 1st level int mask */
    g_mipi_tx_regs_va->gint_msk = 0x1ffff;

    /* OPERATION_MODE, disable input */
    mipi_tx_drv_set_input_enable(0);

    /* vc_id */
    g_mipi_tx_regs_va->video_vc.u32 = 0x0;

    /* output format */
    set_output_format(dev_cfg);

    /* txescclk,timeout */
    mipi_tx_cal_actual_phy_data_rate(dev_cfg->phy_data_rate);
    mipi_tx_drv_set_clkmgr_cfg();

    /* cmd transmission mode */
    mipi_tx_drv_set_controller_cmd_tran_mode(LOW_POWER_MODE);

    /* crc,ecc,eotp tran */
    if (dev_cfg->out_mode == OUT_MODE_CSI) {
        g_mipi_tx_regs_va->pck_en.u32 = 0x0C; /* csi */
    } else {
        g_mipi_tx_regs_va->pck_en.u32 = 0x1E; /* dsi */
    }
    /* gen_vcid_rx */
    g_mipi_tx_regs_va->gen_vc.u32 = 0x0;

    /* video mode cfg */
    set_video_mode_cfg(dev_cfg);
    if ((dev_cfg->out_mode == OUT_MODE_DSI_VIDEO) ||
        (dev_cfg->out_mode == OUT_MODE_CSI)) {
        g_mipi_tx_regs_va->videom_pkt_size.u32 = dev_cfg->sync_info.hact_pixels;
    } else {
        g_mipi_tx_regs_va->command_pkt_size.u32 = dev_cfg->sync_info.hact_pixels;
    }

    /* num_chunks/null_size */
    g_mipi_tx_regs_va->videom_num_chunks.u32 = 0x0;
    g_mipi_tx_regs_va->videom_null_size.u32 = 0x0;

    /* timing config */
    set_timing_config(dev_cfg);

    /* invact,outvact time */
    g_mipi_tx_regs_va->lp_cmd_byte.u32 = 0xff0000;

    if (dev_cfg->out_mode == OUT_MODE_CSI) {
        g_mipi_tx_regs_va->command_pkt_size.u32 = 0;
        g_mipi_tx_regs_va->hsrd_to_set.u32 = 0x0;
    }
    g_mipi_tx_regs_va->hs_lp_to_set.u32 = 0x0;
    g_mipi_tx_regs_va->lprd_to_set.u32 = 0x0;
    g_mipi_tx_regs_va->hswr_to_set.u32 = 0x0;
    /* lp_wr_to_cnt */
    g_mipi_tx_regs_va->lpwr_to_set.u32 = 0x0;
    /* bta_to_cnt */
    g_mipi_tx_regs_va->bta_to_set.u32 = 0x0;
    /* read cmd hsclk */
    g_mipi_tx_regs_va->read_cmd_time.u32 = 0x7fff; /* 0x7fff hsclk */

    /* set data type */
    mipi_tx_drv_set_controller_cmd_datatype(dev_cfg);

    /* lanes num and id */
    set_lane_config(dev_cfg->lane_id, LANE_MAX_NUM);

    /* phy_txrequlpsclk */
    g_mipi_tx_regs_va->ulps_ctrl.u32 = 0x0;

    /* tx triggers */
    g_mipi_tx_regs_va->tx_triggers.u32 = 0x0;

    g_mipi_tx_regs_va->vid_shadow_ctrl.u32 = 0x0;

    /* int0 mask disable */
    g_mipi_tx_regs_va->int0_mask.u32 = 0x0;

    /* clk continue disable */
    mipi_tx_hal_set_clklane_continue_en(0);
}

#endif /* #if mipi_tx_desc("controller cfg") */

#if mipi_tx_desc("set cmd")

static int mipi_tx_wait_cmd_fifo_empty(void)
{
    reg_command_status cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;

        wait_count++;

        osal_udelay(1);

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when send cmd buffer \n");
            return -1;
        }
    } while (cmd_pkt_status.bits.command_empty == 0);

    return 0;
}

static int mipi_tx_wait_write_fifo_empty(void)
{
    reg_command_status cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;

        wait_count++;

        osal_udelay(1);

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when send data buffer \n");
            return -1;
        }
    } while (cmd_pkt_status.bits.pld_write_empty == 0);

    return 0;
}

static int mipi_tx_wait_write_fifo_not_full(void)
{
    reg_command_status cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;
        if (wait_count > 0) {
            osal_udelay(1);
            mipi_tx_err("write fifo full happened wait count = %u\n", wait_count);
        }
        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when wait write fifo not full buffer \n");
            return -1;
        }
        wait_count++;
    } while (cmd_pkt_status.bits.pld_write_full == 1);

    return 0;
}

/*
 * set payloads data by writing register
 * each 4 bytes in cmd corresponds to one register
 */
static void mipi_tx_drv_set_payload_data(const unsigned char *cmd, unsigned short cmd_size)
{
    reg_command_payload gen_pld_data;
    int i;
    int j;

    gen_pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;

    for (i = 0; i < (cmd_size / MIPI_TX_BYTES_PER_WORD); i++) {
        gen_pld_data.bits.command_pld_b1 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1];
        gen_pld_data.bits.command_pld_b2 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2];
        gen_pld_data.bits.command_pld_b3 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3];
        gen_pld_data.bits.command_pld_b4 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE4];

        mipi_tx_wait_write_fifo_not_full();
        g_mipi_tx_regs_va->command_payload.u32 = gen_pld_data.u32;
    }

    j = cmd_size % MIPI_TX_BYTES_PER_WORD;
    if (j != 0) {
        if (j > 0) {
            gen_pld_data.bits.command_pld_b1 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1];
        }
        if (j > 1) {
            gen_pld_data.bits.command_pld_b2 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2];
        }
        if (j > 2) { /* 2 bytes */
            gen_pld_data.bits.command_pld_b3 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3];
        }

        mipi_tx_wait_write_fifo_not_full();
        g_mipi_tx_regs_va->command_payload.u32 = gen_pld_data.u32;
    }

#ifdef MIPI_TX_DEBUG
        osal_printk("\n=====set cmd=======\n");
        osal_printk("gen_pld_data(0x70): 0x%x\n", gen_pld_data.u32);
#endif

    return;
}

static void mipi_tx_drv_set_cmd_work_param(mipi_tx_work_mode_t work_mode, unsigned char lp_clk_en)
{
    unsigned char trans_mode[MIPI_TX_WORK_MODE_BUTT] = {LOW_POWER_MODE, HIGH_SPEED_MODE};

    /* set work mode */
    mipi_tx_drv_set_controller_cmd_tran_mode(trans_mode[work_mode]);

    /* set lp clk en */
    mipi_tx_hal_set_request_hsclk_en(lp_clk_en);
}

static void mipi_tx_drv_set_command_header(unsigned short data_type, unsigned short data_param)
{
    volatile reg_command_header gen_hdr;

    gen_hdr.u32 = g_mipi_tx_regs_va->command_header.u32;
    gen_hdr.bits.command_virtualchannel = 0; /* always 0 */
    gen_hdr.bits.command_datatype = data_type;
    gen_hdr.bits.command_wordcount_low = get_low_byte(data_param);
    gen_hdr.bits.command_wordcount_high = get_high_byte(data_param);
    g_mipi_tx_regs_va->command_header.u32 = gen_hdr.u32;
#ifdef MIPI_TX_DEBUG
    osal_printk("\n=====set cmd=======\n");
    osal_printk("gen_hdr(0x6C): 0x%x\n", gen_hdr.u32);
#endif
}

int mipi_tx_drv_set_cmd_info(const cmd_info_t *cmd_info)
{
    unsigned char *cmd = NULL;

    /* set work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(cmd_info->work_mode, cmd_info->lp_clk_en);

    if (cmd_info->cmd != NULL) {
        cmd = (unsigned char *)osal_kmalloc(cmd_info->cmd_size, osal_gfp_kernel);
        if (cmd == NULL) {
            mipi_tx_err("kmalloc fail,please check,need %d bytes\n", cmd_info->cmd_size);
            return  -1;
        }

        if (osal_copy_from_user(cmd, cmd_info->cmd, cmd_info->cmd_size)) {
            osal_kfree(cmd);
            cmd = NULL;
            return  -1;
        }

        mipi_tx_drv_set_payload_data(cmd, cmd_info->cmd_size);

        osal_kfree(cmd);
        cmd = NULL;
    }

    mipi_tx_drv_set_command_header(cmd_info->data_type, cmd_info->cmd_size);

    /* wait transfer end */
    osal_udelay(350); /* 350us */

    mipi_tx_wait_cmd_fifo_empty();
    mipi_tx_wait_write_fifo_empty();

#ifdef MIPI_TX_DEBUG
    osal_printk("cmd_info->data_type: 0x%x\n", cmd_info->data_type);
    osal_printk("cmd_info->cmd_size: 0x%x\n", cmd_info->cmd_size);
#endif

    /* resume work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(mipi_tx_get_work_mode(), mipi_tx_get_lp_clk_en());

    return 0;
}

#endif /* #if mipi_tx_desc("set cmd") */

#if mipi_tx_desc("get cmd")

static int mipi_tx_wait_read_fifo_not_empty(void)
{
    reg_int0_status int_st0;
    reg_int1_status int_st1;
    unsigned int wait_count;
    reg_command_status cmd_pkt_status;

    wait_count = 0;
    do {
        int_st1.u32 =  g_mipi_tx_regs_va->int1_status.u32;
        int_st0.u32 =  g_mipi_tx_regs_va->int0_status.u32;

        /*
         * 0x0000101e:
         * to_lp_rx, rxecc_multi_err, rxecc_single_err, rxcrc_err, rxpkt_size_err
         */
        if ((int_st1.u32 & 0x0000101e) != 0) {
            mipi_tx_err("err happened when read data, int_st1 = 0x%x,int_st0 = %x\n", int_st1.u32, int_st0.u32);
            return -1;
        }

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when read data\n");
            return -1;
        }

        wait_count++;

        osal_udelay(1);

        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;
    } while (cmd_pkt_status.bits.pld_read_empty == 0x1);

    return 0;
}

static int mipi_tx_wait_read_fifo_empty(void)
{
    reg_command_payload pld_data;
    reg_int1_status int_st1;
    unsigned int wait_count;

    wait_count = 0;
    do {
        int_st1.u32 = g_mipi_tx_regs_va->int1_status.u32;
        if ((int_st1.bits.rxpld_rd_err) == 0x0) {
            pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        }
        wait_count++;
        osal_udelay(1);
        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when clear cmd data buffer, the last read data is 0x%x\n", pld_data.u32);
            return -1;
        }
    } while ((int_st1.bits.rxpld_rd_err) == 0x0);

    return 0;
}

static int mipi_tx_send_short_packet(short unsigned data_type, unsigned short data_param)
{
    mipi_tx_drv_set_command_header(data_type, data_param);

    if (mipi_tx_wait_cmd_fifo_empty() != 0) {
        return -1;
    }

    return 0;
}

static int mipi_tx_get_read_fifo_data(unsigned int get_data_size, unsigned char *data_buf)
{
    reg_command_payload pld_data;
    unsigned int i;
    unsigned int j;

    for (i = 0; i < get_data_size / MIPI_TX_BYTES_PER_WORD; i++) {
        if (mipi_tx_wait_read_fifo_not_empty() != 0) {
            return -1;
        }
        pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1] = pld_data.bits.command_pld_b1;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2] = pld_data.bits.command_pld_b2;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3] = pld_data.bits.command_pld_b3;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE4] = pld_data.bits.command_pld_b4;
    }

    j = get_data_size % MIPI_TX_BYTES_PER_WORD;

    if (j != 0) {
        if (mipi_tx_wait_read_fifo_not_empty() != 0) {
            return -1;
        }
        pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        if (j > 0) {
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1] = pld_data.bits.command_pld_b1;
        }
        if (j > 1) {
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2] = pld_data.bits.command_pld_b2;
        }
        if (j > 2) { /* 2 bytes */
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3] = pld_data.bits.command_pld_b3;
        }
    }

    return 0;
}

static void mipi_tx_reset(void)
{
    volatile reg_phy_ctrl phy_ctrl = {0};
    volatile reg_ctrl_reset ctrl_reset = {0};

    ctrl_reset.u32 = g_mipi_tx_regs_va->ctrl_reset.u32;
    ctrl_reset.bits.ctrl_reset = 0x0;
    g_mipi_tx_regs_va->ctrl_reset.u32 = ctrl_reset.u32;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x0;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
    osal_udelay(1);

    ctrl_reset.u32 = g_mipi_tx_regs_va->ctrl_reset.u32;
    ctrl_reset.bits.ctrl_reset = 0x1;
    g_mipi_tx_regs_va->ctrl_reset.u32 = ctrl_reset.u32;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x1;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
    osal_udelay(1);
    return;
}

int mipi_tx_drv_get_cmd_info(const get_cmd_info_t *get_cmd_info)
{
    unsigned char* data_buf = NULL;

    data_buf = (unsigned char*)osal_kmalloc(get_cmd_info->get_data_size, osal_gfp_kernel);
    if (data_buf == NULL) {
        return -1;
    }

    if (mipi_tx_wait_read_fifo_empty() != 0) {
        goto fail0;
    }

    /* set work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(get_cmd_info->work_mode, get_cmd_info->lp_clk_en);

    if (mipi_tx_send_short_packet(get_cmd_info->data_type, get_cmd_info->data_param) != 0) {
        goto fail0;
    }

    if (mipi_tx_get_read_fifo_data(get_cmd_info->get_data_size, data_buf) != 0) {
        /* fail will block mipi data lane ,so need reset */
        mipi_tx_reset();
        goto fail0;
    }

    /* resume work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(mipi_tx_get_work_mode(), mipi_tx_get_lp_clk_en());

    osal_copy_to_user(get_cmd_info->get_data, data_buf, get_cmd_info->get_data_size);

    osal_kfree(data_buf);
    data_buf = NULL;
    return 0;

fail0:
    osal_kfree(data_buf);
    data_buf = NULL;
    return -1;
}

#endif /* #if mipi_tx_desc("get cmd") */

#if mipi_tx_desc("enable & disable")

void mipi_tx_drv_enable_input(const out_mode_t output_mode)
{
    if ((output_mode == OUT_MODE_DSI_VIDEO) ||
        (output_mode == OUT_MODE_CSI)) {
        mipi_tx_hal_set_cmd_video_mode(VIDEO_MODE);
    }

    /* set to high speed mode */
    if (output_mode == OUT_MODE_DSI_CMD) {
        mipi_tx_drv_set_controller_cmd_tran_mode(HIGH_SPEED_MODE);
    }

    osal_udelay(1);
    mipi_tx_hal_set_request_hsclk_en(1);
    mipi_tx_hal_set_clklane_continue_en(0);
    osal_udelay(1);

    /* enable input */
    mipi_tx_drv_set_input_enable(1);

    /* reset mipi_tx controller */
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    /* reset phy digital module by controller */
    mipi_tx_drv_phy_reset();
    osal_udelay(1);
    mipi_tx_drv_phy_unreset();
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x1;

#if MIPI_TX_INTERRUPTS_ENABLE
    /* enable int */
    mipi_tx_enable_int(0x1);
#endif
}

void mipi_tx_drv_disable_input(void)
{
    volatile reg_operation_mode operation_mode;

    /* disable int */
#if MIPI_TX_INTERRUPTS_ENABLE
    mipi_tx_enable_int(0x0);
#endif

    /* set to disable input */
    operation_mode.u32 = g_mipi_tx_regs_va->operation_mode.u32;
    operation_mode.bits.input_en = 0x0;
    g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;

    /* set to lp mode */
    mipi_tx_drv_set_controller_cmd_tran_mode(LOW_POWER_MODE);

    /* set to command mode */
    mipi_tx_hal_set_cmd_video_mode(COMMAND_MODE);

    /* default: clklane disable */
    mipi_tx_hal_set_request_hsclk_en(0);
    mipi_tx_hal_set_clklane_continue_en(0);

    mipi_tx_reset();
}

#endif /* #if mipi_tx_desc("enable & disable") */

#if mipi_tx_desc("init")

static int mipi_tx_drv_reg_init(void)
{
    if (g_mipi_tx_regs_va == NULL) {
        g_mipi_tx_regs_va = (volatile mipi_tx_regs_type_t *)osal_ioremap(
            MIPI_TX_REGS_ADDR, (unsigned int)MIPI_TX_REGS_SIZE);
        if (g_mipi_tx_regs_va == NULL) {
            mipi_tx_err("remap mipi_tx reg addr fail\n");
            return -1;
        }
        g_reg_map_flag = 1;
    }

    return 0;
}

static void mipi_tx_drv_reg_exit(void)
{
    if (g_reg_map_flag == 1) {
        if (g_mipi_tx_regs_va != NULL) {
            osal_iounmap((void *)g_mipi_tx_regs_va, (unsigned int)MIPI_TX_REGS_SIZE);
            g_mipi_tx_regs_va = NULL;
        }
        g_reg_map_flag = 0;
    }
}

#if MIPI_TX_INTERRUPTS_ENABLE
static void mipi_tx_enable_int(int enable)
{
    volatile reg_int1_mask int1_mask;

    int1_mask.u32 = g_mipi_tx_regs_va->int1_mask.u32;
    int1_mask.bits.mask_vss = enable & 0x1;
    g_mipi_tx_regs_va->int1_mask.u32 = int1_mask.u32;
}

static void mipi_tx_clear_int(void)
{
    volatile reg_int1_status int1_stat;
    volatile reg_int0_status int0_stat;

    /* warning: read register to clear interrupt status, do not delete it. */
    int1_stat.u32 = g_mipi_tx_regs_va->int1_status.u32;
    int0_stat.u32 = g_mipi_tx_regs_va->int0_status.u32;

    /* ignore compile warning: variable 'int_stat' set but not used */
    (void)int1_stat.u32;
    (void)int0_stat.u32;
}

static int mipi_tx_interrupt_route(int irq, void* dev_id)
{
    mipi_tx_clear_int();
    return OSAL_IRQ_HANDLED;
}

static int mipi_tx_register_irq(void)
{
    int ret;
    ret = osal_request_irq(g_mipi_tx_irq_num, mipi_tx_interrupt_route, NULL, "MIPI_TX", mipi_tx_interrupt_route);
    if (ret < 0) {
        mipi_tx_err("mipi_tx: failed to register irq.\n");
        return -1;
    }

    return 0;
}

static void mipi_tx_unregister_irq(void)
{
    osal_free_irq(g_mipi_tx_irq_num, mipi_tx_interrupt_route);
}
#endif

static void mipi_tx_drv_hw_init(int smooth)
{
#ifdef OT_FPGA
    /*
     * reset operation for fpga version.
     */
    unsigned long mipi_tx_crg_addr;
    unsigned long mipi_tx_crg_addr2;

    mipi_tx_crg_addr = (unsigned long)(uintptr_t)osal_ioremap(MIPI_TX_CRG, (unsigned long)0x4);
    if (mipi_tx_crg_addr == 0) {
        return;
    }
    mipi_tx_crg_addr2 = (unsigned long)(uintptr_t)osal_ioremap(MIPI_TX_CRG2, (unsigned long)0x4);
    if (mipi_tx_crg_addr2 == 0) {
        goto exit;
    }

    write_reg32(mipi_tx_crg_addr, 0x3, 0x3);
    write_reg32(mipi_tx_crg_addr, 0x0, 0x3);

    write_reg32(mipi_tx_crg_addr2, 0, 0x1);
    write_reg32(mipi_tx_crg_addr2, 1, 0x1);

    osal_iounmap((void *)(uintptr_t)mipi_tx_crg_addr2, (unsigned long)0x4); // len is 0x4
exit:
    osal_iounmap((void *)(uintptr_t)mipi_tx_crg_addr, (unsigned long)0x4); // len is 0x4

#else
    unsigned long mipi_tx_crg_addr;

    mipi_tx_crg_addr = (unsigned long)(uintptr_t)osal_ioremap(MIPI_TX_CRG, (unsigned long)0x4);

    /* mipi_tx gate clk enable */
    write_reg32(mipi_tx_crg_addr, 1, 0x1);

    /* reset */
    if (smooth == FALSE) {
        write_reg32(mipi_tx_crg_addr, 1 << 1, 0x1 << 1); /* 1: bit1 */
        osal_udelay(1);
    }

    /* unreset */
    write_reg32(mipi_tx_crg_addr, 0 << 1, 0x1 << 1); /* 1: bit1 */

    /* select ref clk 27MHz */
    write_reg32(mipi_tx_crg_addr, 1 << 2, 0x3 << 2); /* 2: bit2 */

    osal_iounmap((void *)(uintptr_t)mipi_tx_crg_addr, (unsigned long)0x4); // len is 0x4
#endif
}

int mipi_tx_drv_init(int smooth)
{
    int ret;

    ret = mipi_tx_drv_reg_init();
    if (ret < 0) {
        mipi_tx_err("mipi_tx_drv_reg_init fail!\n");
        goto fail0;
    }

    mipi_tx_drv_hw_init(smooth);

#if MIPI_TX_INTERRUPTS_ENABLE
    ret = mipi_tx_register_irq();
    if (ret < 0) {
        mipi_tx_err("mipi_tx_register_irq fail!\n");
        goto fail1;
    }
#endif

    return 0;

#if MIPI_TX_INTERRUPTS_ENABLE
fail1:
    mipi_tx_drv_reg_exit();
#endif

fail0:
    return -1;
}

void mipi_tx_drv_exit(void)
{
#if MIPI_TX_INTERRUPTS_ENABLE
    mipi_tx_unregister_irq();
#endif
    mipi_tx_drv_reg_exit();
}

#endif /* #if mipi_tx_desc("init") */

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif
