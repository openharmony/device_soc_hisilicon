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

#include "clk_cfg.h"
#include "sys_cfg.h"

void i2c_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* I2C */
    sys_writel(crg_base + 0x4280, 0x0010); /* i2c0 reset&cken 50M */
    sys_writel(crg_base + 0x4288, 0x0010); /* i2c1 reset&cken 50M */
    sys_writel(crg_base + 0x4290, 0x0010); /* i2c2 reset&cken 50M */
    sys_writel(crg_base + 0x4298, 0x0010); /* i2c3 reset&cken 50M */
    sys_writel(crg_base + 0x42a0, 0x0010); /* i2c4 reset&cken 50M */
    sys_writel(crg_base + 0x42a8, 0x0010); /* i2c5 reset&cken 50M */
}

static void spi_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* SPI */
    sys_writel(crg_base + 0x4480, 0x10); /* ssp0 reset&cken       */
    sys_writel(crg_base + 0x4488, 0x10); /* ssp1 reset&cken       */
    sys_writel(crg_base + 0x4490, 0x10); /* ssp2 reset&cken       */
    sys_writel(crg_base + 0x4498, 0x10); /* ssp3 reset&cken       */
    sys_writel(crg_base + 0x44a0, 0x10); /* 3wire spi reset&cken  */
}

static void mcu_dsp_dpu_ive_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* MCU */
    sys_writel(crg_base + 0x6400, 0x4030); /* mcu reset&cken 500M */

    /* DSP:dsp0 default close clk.it will be opened when insmod dsp ko.reset&cken 750M */
    sys_writel(crg_base + 0x6500, 0x3000);
    /* enable   dsp0 overclock */
    sys_writel(crg_base + 0x650c, 0xA);
    /* DSP:dsp1 default close clk.it will be opened when insmod dsp ko.reset&cken 750M */
    sys_writel(crg_base + 0x6518, 0x3000);
    /* enable   dsp1 overclock */
    sys_writel(crg_base + 0x6524, 0xA);

    /* DPU */
    sys_writel(crg_base + 0x6780, 0x2000); /* dpu reset&cken 600M */

    /* IVE */
    sys_writel(crg_base + 0x67c0, 0x2000); /* ive reset&cken 600M */
}

static void jpge_vdh_jpgd_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* JPGE */
    sys_writel(crg_base + 0x7340, 0x3001); /* jpge reset&cken 672M */

    /* VDH */
    sys_writel(crg_base + 0x73c0, 0x0); /* vdh ckdisable */

    /* JPGD */
    sys_writel(crg_base + 0x7640, 0x1); /* jpgd reset&ckdisable */
}

static void mipirx_vi_isp_vpss_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* MIPIRX */
    sys_writel(crg_base + 0x8540, 0x1); /* mipirx reset&cken */
    sys_writel(crg_base + 0x8560, 0x1); /* mipirx pix0 reset&cken */
    sys_writel(crg_base + 0x8580, 0x1); /* mipirx pix1 reset&cken */
    sys_writel(crg_base + 0x85a0, 0x1); /* mipirx pix2 reset&cken */
    sys_writel(crg_base + 0x85c0, 0x1); /* mipirx pix3 reset&cken */

    /* VICAP */
    sys_writel(crg_base + 0x9140, 0x6003); /* vicap ppc&bus reset&cken, ppc 600M */
    sys_writel(crg_base + 0x9148, 0x0);    /* pt0/1/2/3&chn0/1/2/3 cken */
    sys_writel(crg_base + 0x9164, 0x7001); /* vi port0 reset&cken,600M */
    sys_writel(crg_base + 0x9184, 0x7001); /* vi port1 reset&cken,600M */
    sys_writel(crg_base + 0x91a4, 0x7001); /* vi port2 reset&cken,600M */
    sys_writel(crg_base + 0x91c4, 0x7001); /* vi port3 reset&cken,600M */

    /* ISP */
    sys_writel(crg_base + 0x9150, 0x6003); /* isp0 reset&cken, 600M */
    sys_writel(crg_base + 0x9154, 0x6003); /* isp1 reset&cken, 600M */
    sys_writel(crg_base + 0x9158, 0x6003); /* isp2 reset&cken, 600M */
    sys_writel(crg_base + 0x915c, 0x6003); /* isp3 reset&cken, 600M */

    /* VIPROC */
    sys_writel(crg_base + 0x9740, 0x4001); /* viproc reset&cken, 600M offline */

    /* VPSS */
    sys_writel(crg_base + 0x9b40, 0x4001); /* vpss reset&cken, 600M, offline */
}

static void tde_gdc_vgs_avsp_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* TDE */
    sys_writel(crg_base + 0x9d40, 0x1010); /* tde reset&cken, 475M */

    /* GDC */
    sys_writel(crg_base + 0x9e40, 0x2001); /* gdc reset&cken, 672M */

    /* VGS0 */
    sys_writel(crg_base + 0x9ec0, 0x2001); /* vgs0 reset&ckdisable, 600M */

    /* VGS1 */
    sys_writel(crg_base + 0x9ee0, 0x2001); /* vgs1 reset&ckdisable, 600M */

    /* AVSP */
    sys_writel(crg_base + 0xa040, 0x5000); /* avsp reset&cken, 629M */
}

void clk_cfg(void)
{
    spi_clk_cfg();
    mcu_dsp_dpu_ive_clk_cfg();
    jpge_vdh_jpgd_clk_cfg();
    mipirx_vi_isp_vpss_clk_cfg();
    tde_gdc_vgs_avsp_clk_cfg();
}
