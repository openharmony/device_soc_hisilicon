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

#include "sys_drv.h"
#include "ot_osal.h"
#include "mkp_sys.h"
#include "sys_hal.h"
#include "sys_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define PHASE      32
#define COEFF_BUTT (PHASE / 2 + 1)

#define SYS_DCI_ADJ_COEF_NUM 33
#define SYS_DCI_DIV_LUT_NUM  64

#ifndef arry_size
#define arry_size(x) (sizeof(x) / sizeof((x)[0]))
#endif

td_s32 sys_drv_init(td_void)
{
    if (sys_hal_init() != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 sys_drv_exit(td_void)
{
    sys_hal_exit();
    return TD_SUCCESS;
}

static td_s32 sys_drv_vo_bus_ioctrl(sys_func func_id, td_void *io_args, td_bool *handled)
{
    *handled = TD_TRUE;
    switch (func_id) {
        case SYS_VO_BUS_RESET_SEL: {
            td_bool *reset = (td_bool *)io_args;
            sys_hal_vo_bus_reset_sel(*reset);
            break;
        }

        case SYS_VO_CFG_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_vo_cfg_clk_en(*clk_en);
            break;
        }

        case SYS_VO_APB_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_vo_apb_clk_en(*clk_en);
            break;
        }

        case SYS_VO_BUS_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_vo_bus_clk_en(*clk_en);
            break;
        }
        default:
            *handled = TD_FALSE;
            break;
    }
    return TD_SUCCESS;
}

static td_void sys_drv_vo_base_clk_ioctrl(td_s32 dev, sys_func func_id, td_void *io_args, td_bool *handled)
{
    *handled = TD_TRUE;
    switch (func_id) {
        case SYS_VO_HD_CLKOUT_PHASIC_REVERSE_EN: {
            td_bool *clk_reverse_en = (td_bool *)io_args;
            sys_hal_vo_out_hd_phase_ctrl(dev, *clk_reverse_en);
            break;
        }

        case SYS_VO_DEV_DIV_MOD: {
            td_u32 *div_mod = (td_u32 *)io_args;
            sys_hal_vo_dev_div_mode(dev, *div_mod);
            break;
        }

        case SYS_VO_HD_CLK_SEL: {
            td_u32 *clk_sel = (td_u32 *)io_args;
            sys_hal_vo_hd_clk_sel(dev, *clk_sel);
            break;
        }

        case SYS_VO_CORE_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_vo_core_clk_en(dev, *clk_en);
            break;
        }

        case SYS_VO_DEV_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_vo_dev_clk_en(dev, *clk_en);
            break;
        }

        case SYS_VO_SD_DATE_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_vo_sd_date_clk_en(dev, *clk_en);
            break;
        }

        case SYS_VO_LCD_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_vo_lcd_clk_en(dev, *clk_en);
            break;
        }

        case SYS_VO_LCD_MCLK_DIV: {
            td_u32 *div_mode = (td_u32 *)io_args;
            sys_hal_vo_lcd_mclk_div(dev, *div_mode);
            break;
        }
        default:
            *handled = TD_FALSE;
            break;
    }
}

static td_void sys_drv_vo_intf_clk_ioctrl(td_s32 dev, sys_func func_id, td_void *io_args, td_bool *handled)
{
    *handled = TD_TRUE;
    switch (func_id) {
        case SYS_VO_BT1120_CLK_EN: {
            td_bool *bt1120_clk_en = (td_bool *)io_args;
            sys_hal_vo_bt_clk_en(dev, *bt1120_clk_en);
            break;
        }

        case SYS_VO_BT1120_CLK_SEL: {
            td_u32 *bt1120_clk_ch_sel = (td_u32 *)io_args;
            sys_hal_vo_bt_clk_sel(dev, *bt1120_clk_ch_sel);
            break;
        }

        case SYS_VO_DEV_DAC_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_vo_vdac_clk_en(dev, *clk_en);
            break;
        }

        case SYS_VO_HDMI_CLK_EN: {
            td_bool *hdmi_clk_en = (td_bool *)io_args;
            sys_hal_vo_hdmi_clk_en(dev, *hdmi_clk_en);
            break;
        }

        case SYS_VO_HDMI_CLK_SEL: {
            td_bool *clk_sel = (td_bool *)io_args;
            sys_hal_vo_hdmi_clk_sel(dev, *clk_sel);
            break;
        }

        case SYS_VO_MIPI_CLK_EN: {
            td_bool *mipi_clk_en = (td_bool *)io_args;
            sys_hal_vo_mipi_clk_en(dev, *mipi_clk_en);
            break;
        }

        case SYS_VO_MIPI_CLK_SEL: {
            td_bool *clk_sel = (td_bool *)io_args;
            sys_hal_vo_mipi_clk_sel(dev, *clk_sel);
            break;
        }
        default:
            *handled = TD_FALSE;
            break;
    }
}

static td_s32 sys_drv_vo_dev_ioctrl(td_s32 dev, sys_func func_id, td_void *io_args, td_bool *handled)
{
    sys_drv_vo_base_clk_ioctrl(dev, func_id, io_args, handled);
    if (*handled == TD_TRUE) {
        return TD_SUCCESS;
    }

    sys_drv_vo_intf_clk_ioctrl(dev, func_id, io_args, handled);
    return TD_SUCCESS;
}

static td_s32 sys_drv_vo_pll_ioctrl(const ot_mpp_chn *mpp_chn, sys_func func_id,
    td_void *io_args, td_bool *handled)
{
    td_s32 pll = mpp_chn->dev_id;

    *handled = TD_TRUE;
    switch (func_id) {
        case SYS_VO_PLL_POSTDIV2_SET: {
            td_u32 bits_set = *(td_u32 *)io_args;
            sys_hal_vo_set_pll_postdiv2(pll, bits_set);
            break;
        }

        case SYS_VO_PLL_POSTDIV1_SET: {
            td_u32 bits_set = *(td_u32 *)io_args;
            sys_hal_vo_set_pll_postdiv1(pll, bits_set);
            break;
        }

        case SYS_VO_PLL_FRAC_SET: {
            td_u32 bits_set = *(td_u32 *)io_args;
            sys_hal_vo_set_pll_frac(pll, bits_set);
            break;
        }

        case SYS_VO_PLL_REFDIV_SET: {
            td_u32 bits_set = *(td_u32 *)io_args;
            sys_hal_vo_set_pll_refdiv(pll, bits_set);
            break;
        }

        case SYS_VO_PLL_FBDIV_SET: {
            td_u32 bits_set = *(td_u32 *)io_args;
            sys_hal_vo_set_pll_fbdiv(pll, bits_set);
            break;
        }

        case SYS_VO_PLL_POWER_CTRL: {
            td_bool *power_down = (td_bool *)io_args;
            sys_hal_vo_set_pll_power_ctrl(pll, *power_down);
            break;
        }
        default:
            *handled = TD_FALSE;
            break;
    }
    return TD_SUCCESS;
}

static td_s32 sys_drv_other_ioctrl(sys_func func_id, td_void *io_args, td_bool *handled)
{
    *handled = TD_TRUE;
    switch (func_id) {
        case SYS_TDE_RESET_SEL: {
            td_bool *reset = (td_bool *)io_args;
            sys_hal_tde_reset_sel(*reset);
            break;
        }
        case SYS_TDE_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_tde_clk_en(*clk_en);
            break;
        }
        case SYS_AIO_CLK_SEL: {
            td_u32 *clk_sel = (td_u32 *)io_args;
            sys_hal_aio_clk_sel(*clk_sel);
            break;
        }
        case SYS_AIO_RESET_SEL: {
            td_bool *reset = (td_bool *)io_args;
            sys_hal_aio_reset_sel(*reset);
            break;
        }
        case SYS_AIO_CLK_EN: {
            td_bool *clk_en = (td_bool *)io_args;
            sys_hal_aio_clk_en(*clk_en);
            break;
        }
        case SYS_ACODEC_RESET_SEL: {
            td_bool *reset = (td_bool *)io_args;
            sys_hal_acodec_reset_sel(*reset);
            break;
        }

#ifdef CONFIG_OT_SVP_NPU_V2R1
        case SYS_SVP_NPU0_RESET_SEL: {
            td_bool *reset = (td_bool *)io_args;
            sys_hal_svp_npu0_reset_sel(*reset);
            break;
        }
#endif

        default:
            *handled = TD_FALSE;
            break;
    }
    return TD_SUCCESS;
}

td_s32 sys_drv_drv_ioctrl(const ot_mpp_chn *mpp_chn, sys_func func_id, td_void *io_args)
{
    td_s32 ret;
    td_bool handled;

    if ((mpp_chn == TD_NULL) || (io_args == TD_NULL)) {
        sys_err_trace("null pointer args!\n");
        return TD_FAILURE;
    }

    ret = sys_drv_vo_bus_ioctrl(func_id, io_args, &handled);
    if (handled == TD_TRUE) {
        return ret;
    }
    ret = sys_drv_vo_dev_ioctrl(mpp_chn->dev_id, func_id, io_args, &handled);
    if (handled == TD_TRUE) {
        return ret;
    }
    ret = sys_drv_vo_pll_ioctrl(mpp_chn, func_id, io_args, &handled);
    if (handled == TD_TRUE) {
        return ret;
    }
    ret = sys_drv_other_ioctrl(func_id, io_args, &handled);
    if (handled == TD_TRUE) {
        return ret;
    }

    sys_err_trace("invalid sys ioctl cmd %u!\n", func_id);
    return TD_FAILURE;
}

td_u64 sys_drv_get_local_pts(td_void)
{
    td_u64 sys_cnt = sys_hal_get_local_sys_cnt();

    return osal_div_u64(sys_cnt, 24); /* 24MHz */
}

#ifdef CONFIG_OT_VI_PTS
td_u64 sys_drv_get_vi_local_pts(td_s32 dev_id)
{
    td_u64 sys_cnt = 0;
#ifdef CONFIG_KERNEL
    sys_hal_vi_get_sys_cnt(dev_id, &sys_cnt);
#endif
    return osal_div_u64(sys_cnt, 24); /* 24MHz */
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
