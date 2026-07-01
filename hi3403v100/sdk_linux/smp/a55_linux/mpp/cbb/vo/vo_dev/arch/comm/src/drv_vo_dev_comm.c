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

#include "drv_vo_comm.h"
#include "drv_vo.h"
#include "hal_vo_dev_comm.h"
#include "hal_vo_dev.h"
#include "hal_vo.h"
#include "hal_vo_video_comm.h"
#include "hal_vo_video.h"
#include "vou.h"
#include "mkp_vo_init.h"
#include "drv_vo_dev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
#if vo_desc("dev drv api")

td_u32 vo_drv_get_dev_bg_color(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->vou_attr.bg_color;
}

ot_vo_intf_type vo_drv_get_dev_intf_type(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->vou_attr.intf_type;
}

ot_vo_intf_sync vo_drv_get_dev_intf_sync(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->vou_attr.intf_sync;
}

td_void vou_drv_set_disp_max_size(ot_vo_dev dev, td_u32 max_width, td_u32 max_height)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    drv_dev_ctx->max_width = max_width;
    drv_dev_ctx->max_height = max_height;
}

#endif

#if vo_desc("dev")

td_void vo_drv_get_sync_info(ot_vo_dev dev, hal_disp_syncinfo *sync_info)
{
    hal_disp_syncinfo *hal_sync = TD_NULL;
    ot_vo_sync_info *vo_sync_info = TD_NULL;
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    /* user sync info */
    if (drv_dev_ctx->vou_attr.intf_sync == OT_VO_OUT_USER) {
        vo_sync_info = &drv_dev_ctx->vou_attr.sync_info;
        sync_info->syncm = vo_sync_info->syncm;
        sync_info->iop = vo_sync_info->iop;
        sync_info->intfb = vo_sync_info->intfb;
        sync_info->vact = vo_sync_info->vact;
        sync_info->vbb = vo_sync_info->vbb;
        sync_info->vfb = vo_sync_info->vfb;
        sync_info->hact = vo_sync_info->hact;
        sync_info->hbb = vo_sync_info->hbb;
        sync_info->hfb = vo_sync_info->hfb;
        sync_info->hmid = vo_sync_info->hmid;
        sync_info->bvact = vo_sync_info->bvact;
        sync_info->bvbb = vo_sync_info->bvbb;
        sync_info->bvfb = vo_sync_info->bvfb;
        sync_info->hpw = vo_sync_info->hpw;
        sync_info->vpw = vo_sync_info->vpw;
        sync_info->idv = vo_sync_info->idv;
        sync_info->ihs = vo_sync_info->ihs;
        sync_info->ivs = vo_sync_info->ivs;
        return;
    }

    /* standard sync info */
    hal_sync = vo_drv_comm_get_sync_timing(drv_dev_ctx->vou_attr.intf_sync);
    (td_void)memcpy_s(sync_info, sizeof(hal_disp_syncinfo), hal_sync, sizeof(hal_disp_syncinfo));
}

td_void vo_drv_set_sync_info(ot_vo_dev dev, const hal_disp_syncinfo *sync_info)
{
    vo_hal_intf_set_sync_info(dev, sync_info);
}

td_bool vo_drv_is_progressive(ot_vo_dev dev)
{
    hal_disp_syncinfo sync_info = { 0 };

    vo_drv_get_sync_info(dev, &sync_info);
    return sync_info.iop;
}

td_bool vou_drv_get_dev_enable(ot_vo_dev dev)
{
    td_bool intf_en = TD_FALSE;

    hal_disp_get_intf_enable(dev, &intf_en);
    return intf_en;
}

static td_s32 vo_drv_check_dev_pll_param(ot_vo_dev dev, const ot_vo_pll *pll)
{
    if ((pll->fb_div > 0xfff) || (pll->frac > 0xffffff) || (pll->ref_div > 0x3f) || (pll->ref_div == 0) ||
        (pll->post_div1 > 0x7) || (pll->post_div1 == 0) || (pll->post_div2 > 0x7) || (pll->post_div2 == 0)) {
        vo_err_trace("dev(%d) pll param (fb_div,frac,ref_div,post_div1,post_div2)="
            "(0x%x,0x%x,0x%x,0x%x,0x%x) illegal.\n",
            dev, pll->fb_div, pll->frac, pll->ref_div, pll->post_div1, pll->post_div2);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return vo_drv_check_dev_pll_ref_div(dev, pll->ref_div);
}

static td_s32 vo_drv_check_dev_pll_postdiv(ot_vo_dev dev, const ot_vo_pll *pll)
{
    if (pll->post_div1 < pll->post_div2) {
        vo_err_trace("dev(%d) pll postdiv (post_div1,post_div2)=(0x%x,0x%x) illegal, it should be div1 >= div2.\n",
            dev, pll->post_div1, pll->post_div2);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_drv_check_dev_pll_foutvco(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info)
{
    td_u32 foutvco;
    td_u32 src_clk;
    td_u32 pixel_clk;
    td_u32 max_clk;
    const td_u32 pll_fout4_divisor = 8; /* 8: pll fout4 */
    const ot_vo_pll *pll = &(sync_info->user_sync_attr.vo_pll);

    /* 0x01000000: 2^24 */
    foutvco = VO_PLL_FREF * pll->fb_div / pll->ref_div + VO_PLL_FREF * pll->frac / VO_PLL_FRAC_PREC / pll->ref_div;
    if ((foutvco < VO_PLL_FOUTVCO_MIN) || (foutvco > VO_PLL_FOUTVCO_MAX)) {
        vo_err_trace("dev(%d) pll foutvco (fb_div,frac,ref_div)=(%u,%u,%u) illegal. foutvco %u, not in [%u, %u]\n", dev,
            pll->fb_div, pll->frac, pll->ref_div, foutvco, VO_PLL_FOUTVCO_MIN, VO_PLL_FOUTVCO_MAX);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* check the pixel clk */
    src_clk = foutvco * VO_CLK_MHZ_UNIT / (pll->post_div1 * pll->post_div2);
    if (sync_info->user_sync_attr.clk_src == OT_VO_CLK_SRC_PLL_FOUT4) {
        src_clk = src_clk / pll_fout4_divisor;
    }
    pixel_clk = src_clk / (sync_info->pre_div * sync_info->dev_div);
    max_clk = vo_drv_dev_get_max_clk(dev);
    if (pixel_clk > max_clk) {
        vo_err_trace("dev(%d) pll (fb_div,frac,ref_div,post_div1,post_div2,pre_div,dev_div)=(%u,%u,%u,%u,%u,%u,%u)"
            " is illegal. pixel clk %uKHz is larger than %uKHz.\n",
            dev, pll->fb_div, pll->frac, pll->ref_div, pll->post_div1, pll->post_div2, sync_info->pre_div,
            sync_info->dev_div, pixel_clk, max_clk);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 vo_drv_check_dev_pll(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info)
{
    td_s32 ret;

    ret = vo_drv_check_dev_pll_param(dev, &sync_info->user_sync_attr.vo_pll);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vo_drv_check_dev_pll_postdiv(dev, &sync_info->user_sync_attr.vo_pll);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_drv_check_dev_pll_foutvco(dev, sync_info);
}

td_s32 vou_drv_check_dev_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync)
{
    td_s32 ret = TD_SUCCESS;

#ifdef CONFIG_OT_VO_VIRTDEV_SUPPORT
    if (vou_drv_is_virt_dev(dev)) {
        if (intf_sync >= OT_VO_OUT_BUTT) {
            vo_err_trace("vo%d's intfsync %d illegal!\n", dev, intf_sync);
            return OT_ERR_VO_NOT_SUPPORT;
        }

        return TD_SUCCESS;
    }
#endif

    if ((intf_sync == OT_VO_OUT_960H_PAL) || (intf_sync == OT_VO_OUT_960H_NTSC)) {
        vo_err_trace("vo%d's intfsync %d illegal, vo doesn't support interlaced sync %d!\n", dev, intf_sync, intf_sync);
        return OT_ERR_VO_NOT_SUPPORT;
    }

#ifdef CONFIG_OT_HDMI_SUPPORT
    ret = vou_drv_check_hdmi_sync(dev, intf_type, intf_sync);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif

#ifdef CONFIG_OT_VO_BT1120
    ret = vou_drv_check_bt_sync(dev, intf_type, intf_sync);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif

#ifdef CONFIG_OT_VO_VGA
    ret = vou_drv_check_vga_sync(dev, intf_type, intf_sync);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif

#ifdef CONFIG_OT_VO_CVBS
    ret = vo_drv_check_cvbs_sync(dev, intf_type, intf_sync);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif

#ifdef CONFIG_OT_VO_MIPI
    ret = vou_drv_check_mipi_sync(dev, intf_type, intf_sync);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif

#ifdef CONFIG_OT_VO_RGB
    ret = vou_drv_check_rgb_sync(dev, intf_type, intf_sync);
#endif

    return ret;
}

static td_s32 vo_drv_check_bool_value(td_bool bool_val)
{
    if ((bool_val != TD_TRUE) && (bool_val != TD_FALSE)) {
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_drv_check_dev_bool_sync_info(ot_vo_dev dev, const ot_vo_sync_info *sync_info)
{
    if ((vo_drv_check_bool_value(sync_info->syncm) != TD_SUCCESS) ||
        (vo_drv_check_bool_value(sync_info->iop) != TD_SUCCESS) ||
        (vo_drv_check_bool_value(sync_info->idv) != TD_SUCCESS) ||
        (vo_drv_check_bool_value(sync_info->ihs) != TD_SUCCESS) ||
        (vo_drv_check_bool_value(sync_info->ivs) != TD_SUCCESS)) {
        vo_err_trace("vo%d: sync's syncm(%u),iop(%u),idv(%u),ihs(%u),ivs(%u) should be %u or %u!\n", dev,
            sync_info->syncm, sync_info->iop, sync_info->idv, sync_info->ihs, sync_info->ivs, TD_TRUE, TD_FALSE);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_drv_check_dev_vertical_sync_info(ot_vo_dev dev, const ot_vo_sync_info *sync_info)
{
    if ((sync_info->vact < VO_MIN_USER_SYNC_VACT) || (sync_info->vact > VO_MAX_USER_SYNC_VACT)) {
        vo_err_trace("vo%d: sync's vact(%u) should be [%u, %u]!\n", dev, sync_info->vact, VO_MIN_USER_SYNC_VACT,
            VO_MAX_USER_SYNC_VACT);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if ((sync_info->vbb < VO_MIN_USER_SYNC_VBB) || (sync_info->vbb > VO_MAX_USER_SYNC_VBB)) {
        vo_err_trace("vo%d: sync's vbb(%u) should be [%u, %u]!\n", dev, sync_info->vbb, VO_MIN_USER_SYNC_VBB,
            VO_MAX_USER_SYNC_VBB);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if ((sync_info->vfb < VO_MIN_USER_SYNC_VFB) || (sync_info->vfb > VO_MAX_USER_SYNC_VFB)) {
        vo_err_trace("vo%d: sync's vfb(%u) should be [%u, %u]!\n", dev, sync_info->vfb, VO_MIN_USER_SYNC_VFB,
            VO_MAX_USER_SYNC_VFB);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_drv_check_dev_horizontal_sync_info(ot_vo_dev dev, const ot_vo_sync_info *sync_info)
{
    if ((sync_info->hact < VO_MIN_USER_SYNC_HACT) || (sync_info->hact > VO_MAX_USER_SYNC_HACT)) {
        vo_err_trace("vo%d: sync's hact(%u) should be [%u, %u]!\n", dev, sync_info->hact, VO_MIN_USER_SYNC_HACT,
            VO_MAX_USER_SYNC_HACT);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (sync_info->hbb < VO_MIN_USER_SYNC_HBB) {
        vo_err_trace("vo%d: sync's hbb(%u) should be [%u, %u]!\n", dev, sync_info->hbb, VO_MIN_USER_SYNC_HBB,
            VO_MAX_USER_SYNC_HBB);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (sync_info->hfb < VO_MIN_USER_SYNC_HFB) {
        vo_err_trace("vo%d: sync's hfb(%u) should be [%u, %u]!\n", dev, sync_info->hfb, VO_MIN_USER_SYNC_HFB,
            VO_MAX_USER_SYNC_HFB);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_drv_check_dev_bottom_sync_info(ot_vo_dev dev, const ot_vo_sync_info *sync_info)
{
    if ((sync_info->iop == 0) && ((sync_info->bvact == 0) || (sync_info->bvbb == 0) || (sync_info->bvfb == 0))) {
        vo_err_trace("vo%d: sync's bvact(%u) bvbb(%u) bvfb(%u) can't be 0 when interlaced timing!\n", dev,
            sync_info->bvact, sync_info->bvbb, sync_info->bvfb);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (sync_info->bvact > VO_MAX_USER_SYNC_BVACT) {
        vo_err_trace("vo%d: sync's bvact(%u) should be [0, %u]!\n", dev, sync_info->bvact, VO_MAX_USER_SYNC_BVACT);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (sync_info->bvbb > VO_MAX_USER_SYNC_BVBB) {
        vo_err_trace("vo%d: sync's bvbb(%u) should be [0, %u]!\n", dev, sync_info->bvbb, VO_MAX_USER_SYNC_BVBB);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (sync_info->bvfb > VO_MAX_USER_SYNC_BVFB) {
        vo_err_trace("vo%d: sync's bvfb(%u) should be [0, %u]!\n", dev, sync_info->bvfb, VO_MAX_USER_SYNC_BVFB);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_drv_check_dev_pulse_sync_info(ot_vo_dev dev, const ot_vo_sync_info *sync_info)
{
    if (sync_info->hpw < VO_MIN_USER_SYNC_HPW) {
        vo_err_trace("vo%d: sync's hpw(%u) should be [%u, %u]!\n", dev, sync_info->hpw, VO_MIN_USER_SYNC_HPW,
            VO_MAX_USER_SYNC_HPW);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if ((sync_info->vpw < VO_MIN_USER_SYNC_VPW) || (sync_info->vpw > VO_MAX_USER_SYNC_VPW)) {
        vo_err_trace("vo%d: sync's vpw(%u) should be [%u, %u]!\n", dev, sync_info->vpw, VO_MIN_USER_SYNC_VPW,
            VO_MAX_USER_SYNC_VPW);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_drv_check_dev_sync_info(ot_vo_dev dev, const ot_vo_sync_info *sync_info)
{
    td_s32 ret;

    ret = vo_drv_check_dev_bool_sync_info(dev, sync_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vo_drv_check_dev_vertical_sync_info(dev, sync_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vo_drv_check_dev_horizontal_sync_info(dev, sync_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vo_drv_check_dev_bottom_sync_info(dev, sync_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_drv_check_dev_pulse_sync_info(dev, sync_info);
}

td_s32 vou_drv_check_dev_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr)
{
    td_s32 ret;

    if (vo_drv_is_phy_dev_hd(dev) || vo_drv_is_phy_dev_sd(dev)) {
        ret = vou_drv_check_dev_intf(dev, pub_attr->intf_type);
        if (ret != TD_SUCCESS) {
            vo_err_trace("vo%d check device's interface %u illegal!\n", dev, pub_attr->intf_type);
            return ret;
        }
    }

    if (pub_attr->intf_sync == OT_VO_OUT_USER) {
        /*
         * user defined sync will be welcome for all interface type
         * WARNING: user must make sure to config right sync, otherwise, screen doesn't work!
         */
        return vo_drv_check_dev_sync_info(dev, &(pub_attr->sync_info));
    }

    return vou_drv_check_dev_sync(dev, pub_attr->intf_type, pub_attr->intf_sync);
}

td_void vo_drv_get_dev_reg_up(ot_vo_dev dev)
{
    td_u64 time_begin;
    td_u64 timeval;

    time_begin = get_sys_time_by_usec();
    do {
        timeval = get_sys_time_by_usec() - time_begin;
        if (timeval > VO_DEV_REG_UP_TIMEOUT) {
            break;
        }
    } while (hal_disp_get_reg_up(dev) != 0);
}

#endif

#if vo_desc("dev layer prio")

td_void vou_drv_set_layer_priority(ot_vo_dev dev, ot_vo_layer layer, td_u32 priority)
{
    vo_drv_dev *dev_ctx = vo_drv_get_dev_ctx(dev);

    dev_ctx->layer[priority] = layer;
    hal_cbm_set_cbm_mixer_prio(layer, priority, dev);

    /* need regup */
    hal_disp_set_reg_up(dev);
}
#endif

#if vo_desc("dev intf")

td_void vou_drv_intf_csc_config(ot_vo_intf_type intf, const ot_vo_csc *csc)
{
    csc_coef coef;

    (td_void)memset_s(&coef, sizeof(csc_coef), 0x0, sizeof(csc_coef));
    vou_drv_calc_csc_matrix(csc, csc->csc_matrix, &coef);
    vo_drv_csc_trans_to_register(&coef);
    vo_hal_intf_set_csc_cfg(intf, &coef);
}

#ifdef CONFIG_OT_VO_VGA
td_void vo_drv_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param)
{
    vo_hal_intf_set_csc_enable(OT_VO_INTF_VGA, TD_TRUE);
    vou_drv_intf_csc_config(OT_VO_INTF_VGA, &vga_param->csc);
    vo_hal_intf_set_dac_gc(OT_VO_INTF_VGA, vga_param->gain);
    vo_hal_intf_set_vga_hsp_cfg(TD_TRUE, (td_u32)vga_param->sharpen_strength);
    hal_disp_set_reg_up(dev);
}
#endif

#ifdef CONFIG_OT_HDMI_SUPPORT
static td_void vo_drv_set_hdmi_mode(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param)
{
    if ((hdmi_param->csc.csc_matrix >= OT_VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL) &&
        (hdmi_param->csc.csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT)) {
        hal_disp_set_hdmi_mode(dev, 1); /* 1: RGB */
    } else {
        hal_disp_set_hdmi_mode(dev, 0); /* 0: YUV */
    }
}

td_void vo_drv_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param)
{
    vo_hal_intf_set_csc_enable(OT_VO_INTF_HDMI, TD_TRUE);
    vou_drv_intf_csc_config(OT_VO_INTF_HDMI, &hdmi_param->csc);
    vo_drv_set_hdmi_mode(dev, hdmi_param);
    hal_disp_set_reg_up(dev);
}
#endif

#ifdef CONFIG_OT_VO_RGB
td_void vo_drv_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param)
{
    vo_drv_dev *dev_ctx = vo_drv_get_dev_ctx(dev);

    vo_hal_intf_set_csc_enable(OT_VO_INTF_RGB_6BIT, TD_TRUE);
    vou_drv_intf_csc_config(OT_VO_INTF_RGB_6BIT, &rgb_param->csc);
    if (vo_drv_is_rgb_parallel_intf(dev_ctx->vou_attr.intf_type)) {
        /* only parallel interface */
        vo_hal_set_intf_rgb_component_order(rgb_param->rgb_inverted_en);
    }

    /* both parallel and serial interface */
    vo_hal_set_intf_rgb_bit_inverse(rgb_param->bit_inverted_en);
    hal_disp_set_reg_up(dev);
}
#endif

#ifdef CONFIG_OT_VO_BT1120
td_void vo_drv_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param)
{
    vo_drv_dev *dev_ctx = vo_drv_get_dev_ctx(dev);

    if (vo_drv_is_bt1120_intf(dev_ctx->vou_attr.intf_type)) {
        /* only bt.1120 interface */
        vo_hal_set_intf_bt_component_order(bt_param->yc_inverted_en);
    }

    /* both bt.1120 and bt.656 interface */
    vo_hal_set_intf_bt_bit_inverse(bt_param->bit_inverted_en);
    /* clk_edge is configured when dev enabled, not configured here */
    hal_disp_set_reg_up(dev);
}

td_void vo_drv_set_bt_param_clk_edge(ot_vo_dev dev, ot_vo_clk_edge clk_edge)
{
    vo_drv_dev *dev_ctx = vo_drv_get_dev_ctx(dev);

    /* clk_edge is configured when dev enabled, also set bt param */
    dev_ctx->clk_edge = clk_edge;
    dev_ctx->bt_param_config = TD_TRUE;
}

ot_vo_clk_edge vo_drv_get_bt_param_clk_edge(ot_vo_dev dev)
{
    vo_drv_dev *dev_ctx = vo_drv_get_dev_ctx(dev);
    return dev_ctx->clk_edge;
}

#endif

#ifdef CONFIG_OT_VO_BT1120_DUAL_EDGE
td_void vo_drv_set_out_clk_div(td_u32 dev, td_u32 vo_out_clk_div)
{
    ot_mpp_chn mpp_chn = {0};

    vo_drv_set_dev_mpp_chn(dev, &mpp_chn);
    call_sys_drv_ioctrl(&mpp_chn, SYS_VO_OUT_HD_DIV, &vo_out_clk_div);
}

#endif

#ifdef CONFIG_OT_VO_MIPI
td_void vo_drv_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param)
{
    vo_hal_intf_set_csc_enable(OT_VO_INTF_MIPI, TD_TRUE);
    vou_drv_intf_csc_config(OT_VO_INTF_MIPI, &mipi_param->csc);
    hal_disp_set_reg_up(dev);
}
#endif

td_void vo_drv_set_dev_mpp_chn(ot_vo_dev dev, ot_mpp_chn *mpp_chn)
{
    mpp_chn->mod_id = OT_ID_VO;
    mpp_chn->dev_id = dev;
    mpp_chn->chn_id = 0;
}

td_bool vo_drv_is_hdmi_intf(ot_vo_intf_type intf_type)
{
    if ((intf_type & OT_VO_INTF_HDMI) != 0) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_bool vo_drv_is_hdmi1_intf(ot_vo_intf_type intf_type)
{
    if ((intf_type & OT_VO_INTF_HDMI1) != 0) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_bool vo_drv_is_bt_intf(ot_vo_intf_type intf_type)
{
    if (((intf_type & OT_VO_INTF_BT1120) != 0) || ((intf_type & OT_VO_INTF_BT656) != 0)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_bool vo_drv_is_bt1120_intf(ot_vo_intf_type intf_type)
{
    if ((intf_type & OT_VO_INTF_BT1120) != 0) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_bool vo_drv_is_bt656_intf(ot_vo_intf_type intf_type)
{
    if ((intf_type & OT_VO_INTF_BT656) != 0) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_bool vo_drv_is_vga_intf(ot_vo_intf_type intf_type)
{
    if ((intf_type & OT_VO_INTF_VGA) != 0) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_bool vo_drv_is_cvbs_intf(ot_vo_intf_type intf_type)
{
    if ((intf_type & OT_VO_INTF_CVBS) != 0) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

#ifdef CONFIG_OT_VO_RGB

td_bool vo_drv_is_rgb_intf(ot_vo_intf_type intf_type)
{
    if (((intf_type & OT_VO_INTF_RGB_6BIT)  != 0) ||
        ((intf_type & OT_VO_INTF_RGB_8BIT)  != 0) ||
        ((intf_type & OT_VO_INTF_RGB_16BIT) != 0) ||
        ((intf_type & OT_VO_INTF_RGB_18BIT) != 0) ||
        ((intf_type & OT_VO_INTF_RGB_24BIT) != 0)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_bool vo_drv_is_rgb_serial_intf(ot_vo_intf_type intf_type)
{
    if (((intf_type & OT_VO_INTF_RGB_6BIT) != 0) || ((intf_type & OT_VO_INTF_RGB_8BIT) != 0)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_bool vo_drv_is_rgb_parallel_intf(ot_vo_intf_type intf_type)
{
    if (((intf_type & OT_VO_INTF_RGB_16BIT) != 0) ||
        ((intf_type & OT_VO_INTF_RGB_18BIT) != 0) ||
        ((intf_type & OT_VO_INTF_RGB_24BIT) != 0)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

#endif

td_bool vo_drv_is_mipi_intf(ot_vo_intf_type intf_type)
{
    if (((intf_type & OT_VO_INTF_MIPI) != 0) || ((intf_type & OT_VO_INTF_MIPI_SLAVE) != 0)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_s32 vo_drv_check_same_intf(ot_vo_dev dev, ot_vo_dev other_dev, ot_vo_intf_type intf_type)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(other_dev);
    if ((intf_type & drv_dev_ctx->vou_attr.intf_type) != 0) {
        vo_err_trace("vo(%d), the intf %u is used in dev(%d)!\n",
            dev, intf_type & drv_dev_ctx->vou_attr.intf_type, other_dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }
    return TD_SUCCESS;
}

td_s32 vo_drv_check_intf_share(ot_vo_dev dev, ot_vo_intf_type intf_type)
{
    ot_vo_dev other_dev;
    vo_drv_dev *drv_dev_ctx = TD_NULL;
    td_s32 ret;

    if ((dev != VO_DEV_DHD0) && (dev != VO_DEV_DHD1)) {
        return TD_SUCCESS;
    }

    for (other_dev = VO_DEV_DHD0; other_dev <= VO_DEV_DHD1; other_dev++) {
        if (other_dev == dev) {
            continue;
        }

        drv_dev_ctx = vo_drv_get_dev_ctx(other_dev);
        if (drv_dev_ctx->config != TD_TRUE) {
            continue;
        }

        ret = vo_drv_check_same_intf(dev, other_dev, intf_type);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        ret = vo_drv_check_other_dev_exclution_intf(other_dev, intf_type);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    return TD_SUCCESS;
}
#endif

#if vo_desc("check dev id")

td_s32 vo_drv_check_dev_id_pub(ot_vo_dev dev)
{
    if ((dev < 0) || (dev >= OT_VO_MAX_DEV_NUM)) {
        return OT_ERR_VO_INVALID_DEV_ID;
    }
    return TD_SUCCESS;
}

#endif

#if vo_desc("check layer id")
td_s32 vo_drv_check_layer_id_pub(ot_vo_layer layer)
{
    if ((layer < 0) || (layer >= OT_VO_MAX_LAYER_NUM)) {
        return OT_ERR_VO_INVALID_LAYER_ID;
    }
    return TD_SUCCESS;
}

#endif

td_void vo_drv_set_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    (td_void)memcpy_s(&drv_dev_ctx->vou_attr, sizeof(ot_vo_pub_attr), pub_attr, sizeof(ot_vo_pub_attr));
    drv_dev_ctx->config = TD_TRUE;
}

td_void vo_drv_enable(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    drv_dev_ctx->vo_enable = TD_TRUE;
}

td_void vo_drv_disable(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    drv_dev_ctx->config = TD_FALSE;
    drv_dev_ctx->vo_enable = TD_FALSE;
    drv_dev_ctx->bt_param_config = TD_FALSE;
    drv_dev_ctx->clk_edge = OT_VO_CLK_EDGE_SINGLE;
    (td_void)memset_s(&drv_dev_ctx->vou_attr, sizeof(ot_vo_pub_attr), 0, sizeof(ot_vo_pub_attr));
}

#if vo_desc("dev")
td_bool vo_drv_is_phy_dev_uhd(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    if (drv_dev_ctx->dev_cap.dev_type == VO_UHD_HW_DEV) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

td_bool vo_drv_is_phy_dev_hd(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    vo_dev_capability *dev_cap = &drv_dev_ctx->dev_cap;

    if ((dev_cap->dev_type == VO_UHD_HW_DEV) || (dev_cap->dev_type == VO_HD_HW_DEV)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

td_bool vo_drv_is_phy_dev_sd(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    if (drv_dev_ctx->dev_cap.dev_type == VO_SD_HW_DEV) {
        return TD_TRUE;
    }

    return TD_FALSE;
}
#endif
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("dev")

td_void vo_drv_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    (td_void)memcpy_s(&drv_dev_ctx->dev_param, sizeof(ot_vo_dev_param), dev_param, sizeof(ot_vo_dev_param));
}

td_u32 vo_drv_get_dev_vtth(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->vtth;
}

td_u32 vo_drv_get_dev_vtth2(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->vtth2;
}

/* 中断中视频层寄存器更新方式 */
td_void vo_drv_int_reg_up_mode(ot_vo_layer layer, vo_int_mode int_mode)
{
    hal_video_set_layer_up_mode(layer, int_mode);
}

static td_u32 vo_drv_get_sync_frame_vert_all(hal_disp_syncinfo *sync_info)
{
    return (sync_info->iop == 1) ?
        (sync_info->vact + sync_info->vbb + sync_info->vfb) :
        (sync_info->vact + sync_info->vbb + sync_info->vfb + sync_info->bvact + sync_info->bvbb + sync_info->bvfb);
}

td_u32 vo_drv_get_sync_field_vert_all(hal_disp_syncinfo *sync_info)
{
    /* 注意隔行时序取所有行数的一半即可，两场中取较小者 */
    return (sync_info->iop == 1) ?
        (sync_info->vact + sync_info->vbb + sync_info->vfb) :
        ((sync_info->vact + sync_info->vbb + sync_info->vfb + sync_info->bvact + sync_info->bvbb + sync_info->bvfb) /
        2); /* 2，场占一半行数 */
}

td_s32 vo_drv_check_dev_vtth(ot_vo_dev dev)
{
    hal_disp_syncinfo sync_info;
    td_u32 vtth_temp;
    td_u16 vert_all;
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    vo_drv_get_sync_info(dev, &sync_info);

    vtth_temp = drv_dev_ctx->vtth;
    /* 逐行和隔行时序要分别计算 */
    vert_all = vo_drv_get_sync_field_vert_all(&sync_info);
    if (vtth_temp >= vert_all) {
        vo_err_trace("vo dev %d vtth: %u is not in logic [%d,%u]\n", dev, vtth_temp, 0, vert_all - 1);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }
    return TD_SUCCESS;
}

td_u32 vo_drv_cal_vtth1_by_sync(ot_vo_dev dev, td_u32 full_frame_rate)
{
    td_u32 dev_vtth;
    hal_disp_syncinfo sync_info;
    td_u32 frame_rate = full_frame_rate;
    td_u32 frame_vert_all;
    td_u32 field_vert_all;

    vo_drv_get_sync_info(dev, &sync_info);
    /* 区分逐行时序与隔行时序 */
    frame_vert_all = vo_drv_get_sync_frame_vert_all(&sync_info);
    dev_vtth = VO_DEAULT_VTTH_TIME * frame_vert_all * frame_rate / 1000; /* 1000ms */

    /*
     * frame_rate的范围是[1~240]，为确保这个计算值是正确的，
     * 一旦超过总行数，则设置为总行数
     */
    field_vert_all = vo_drv_get_sync_field_vert_all(&sync_info);
    if (dev_vtth >= field_vert_all) {
        vo_warn_trace("dev:%d, cal vtth value(%d) is >= field all(%d), frame_rate(%d).\n", dev, dev_vtth,
            field_vert_all, frame_rate);
        dev_vtth = field_vert_all - 1;
    }

    vo_debug_trace("dev:%d, cal vtth value(%d)\n", dev, dev_vtth);
    return dev_vtth;
}

td_void vo_drv_set_dev_default_vtth(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    if (vo_drv_is_phy_dev_hd(dev) == TD_TRUE) {
        drv_dev_ctx->vtth = VO_HD_VTTH_WATERLINE;
    } else {
        drv_dev_ctx->vtth = VO_SD_VTTH_WATERLINE;
    }
}

td_void vo_drv_set_dev_vtth(ot_vo_dev dev, td_u32 vtth)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    drv_dev_ctx->vtth = vtth;
}

#if (defined(CONFIG_OT_VO_LESS_BUF) || defined(CONFIG_OT_VO_USER_NOTIFY))
static td_u32 vo_drv_get_dev_default_vtth2(ot_vo_dev dev)
{
    td_u32 dft_vtth2;

    dft_vtth2 = (vo_drv_is_phy_dev_hd(dev) == TD_TRUE) ? VO_HD_VTTH_WATERLINE2 : VO_SD_VTTH_WATERLINE2;
    return dft_vtth2;
}

td_void vo_drv_set_dev_default_vtth2(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    drv_dev_ctx->vtth2 = vo_drv_get_dev_default_vtth2(dev);
}

td_void vo_drv_set_dev_vtth2(ot_vo_dev dev, td_u32 vtth2)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    drv_dev_ctx->vtth2 = vtth2;
}
#endif

td_void vo_drv_set_vtth(ot_vo_dev dev)
{
    hal_disp_syncinfo sync_info;
    td_u16 vtth_line = 0;
    td_u16 vtth2_line = 0;
    td_u16 vert_all;
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    vo_drv_get_sync_info(dev, &sync_info);

    if (vo_drv_is_phy_dev_hd(dev) || vo_drv_is_phy_dev_sd(dev)) {
        /* 逐行与隔行时序应该分别计算 */
        vert_all = vo_drv_get_sync_field_vert_all(&sync_info);

        vtth_line = vert_all - 1 - (td_u16)(drv_dev_ctx->vtth);

        /* VO支持省buf特性时使用默认的vtth2,不支持时vtth2给GFBG使用，需要配置和vtth1一致 */
        vtth2_line = vo_drv_get_vtth2_line(dev, vtth_line, vert_all);
    }

    /* 设置垂直时序中断门限 */
    hal_disp_set_vtthd(dev, vtth_line, vtth2_line);
}

td_bool vo_drv_get_dev_dfir_en(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    if (drv_dev_ctx->dev_param.vo_bypass_en == TD_TRUE) {
        return TD_FALSE;
    } else {
        return TD_TRUE;
    }
}

static td_void vo_drv_enable_vdac_power(ot_vo_dev dev)
{
    ot_mpp_chn mpp_chn;
    td_bool dac_clk_en = TD_TRUE;
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    if (drv_dev_ctx->dac_power_up == TD_FALSE) {
        if (drv_dev_ctx->vou_attr.intf_type == OT_VO_INTF_CVBS) {
            vo_hal_intf_set_dac_chn_enable(OT_VO_INTF_CVBS, TD_TRUE);
        } else {
            vo_hal_intf_set_dac_chn_enable(OT_VO_INTF_VGA, TD_TRUE);
        }

        vo_drv_set_dev_mpp_chn(dev, &mpp_chn);
        call_sys_drv_ioctrl(&mpp_chn, SYS_VO_DEV_DAC_EN, &dac_clk_en);
        drv_dev_ctx->dac_power_up = TD_TRUE;
        vo_debug_trace("dev %d DAC power_up!\n", dev);
    }
}

static td_void vo_drv_disable_vdac_power(ot_vo_dev dev)
{
    ot_mpp_chn mpp_chn;
    td_bool dac_clk_en = TD_FALSE;
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    if (drv_dev_ctx->dac_power_up == TD_TRUE) {
        if (drv_dev_ctx->vou_attr.intf_type == OT_VO_INTF_CVBS) {
            vo_hal_intf_set_dac_chn_enable(OT_VO_INTF_CVBS, TD_FALSE);
        } else {
            vo_hal_intf_set_dac_chn_enable(OT_VO_INTF_VGA, TD_FALSE);
        }

        vo_drv_set_dev_mpp_chn(dev, &mpp_chn);
        call_sys_drv_ioctrl(&mpp_chn, SYS_VO_DEV_DAC_EN, &dac_clk_en);
        drv_dev_ctx->dac_power_up = TD_FALSE;
        vo_debug_trace("dev %d DAC power_down!\n", dev);
    }
}

td_bool vo_drv_get_vdac_status(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->dac_power_up;
}

/* 开启检测电路等 */
td_s32 vo_drv_enable_load_detect(ot_vo_dev dev)
{
    vo_drv_load_detect_info *info = TD_NULL;
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    info = &drv_dev_ctx->load_detect_info;
    if (info->detect_enabled == TD_TRUE) {
        return TD_SUCCESS;
    }

    info->int_ocurred = TD_FALSE;
    /* 如果DAC已经powerdown,则将其powerup */
    vo_drv_enable_vdac_power(dev);
    info->detect_enabled = TD_TRUE;
    return TD_SUCCESS;
}

/* 关闭检测电路 */
td_s32 vo_drv_disable_load_detect(ot_vo_dev dev)
{
    vo_drv_load_detect_info *info = TD_NULL;
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    info = &drv_dev_ctx->load_detect_info;
    if (info->detect_enabled == TD_FALSE) {
        return TD_SUCCESS;
    }

    info->detect_enabled = TD_FALSE;

    if (info->int_ocurred) {
        info->int_ocurred = TD_FALSE;
        vo_drv_disable_vdac_power(dev);
        info->is_connected = TD_FALSE;
    } else {
        vo_drv_enable_vdac_power(dev);
        info->is_connected = TD_TRUE;
    }

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_VO_WBC
td_bool vo_drv_is_dev_support_wbc(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->dev_cap.support_wbc;
}
#endif

#if (defined(CONFIG_OT_VO_LESS_BUF) || defined(CONFIG_OT_VO_USER_NOTIFY))
td_void vo_drv_refresh_dev_vtth2(ot_vo_dev dev)
{
    td_u32 int_type;
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);

    /* 未使能时统一在使能接口中配置,不在配置接口时配置 */
    if (drv_dev_ctx->vo_enable != TD_TRUE) {
        return;
    }

    hal_disp_set_intf_enable(dev, TD_FALSE);
    int_type = vo_drv_get_vtth2_int_type(dev);
    vou_drv_int_clear(int_type);
    vo_drv_set_vtth(dev);
    hal_disp_set_intf_enable(dev, TD_TRUE);
    vo_drv_dev_int_enable(dev, TD_TRUE);
    hal_disp_set_reg_up(dev);
}
#endif

#ifdef CONFIG_OT_VO_LESS_BUF
td_void vo_drv_set_dev_less_buf_enable(ot_vo_dev dev, td_bool enable)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    drv_dev_ctx->less_buf_enable = enable;
}

td_bool vo_drv_get_dev_less_buf_enable(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->less_buf_enable;
}
#endif

#ifdef CONFIG_OT_VO_USER_NOTIFY
td_void vo_drv_set_dev_user_notify_enable(ot_vo_dev dev, td_bool enable)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    drv_dev_ctx->user_notify_enable = enable;
}

td_bool vo_drv_get_dev_user_notify_enable(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->user_notify_enable;
}
#endif

td_void vou_drv_get_dev_int_state(ot_vo_dev dev, td_bool *bottom)
{
    hal_disp_get_int_state(dev, bottom);
}

#ifdef CONFIG_OT_VO_CASCADE
td_void vo_drv_get_dev_int_state_vblank(ot_vo_dev dev, td_bool *vblank)
{
    hal_disp_get_int_state_vblank(dev, vblank);
}

td_void vo_drv_set_dev_reg_up(ot_vo_dev dev)
{
    hal_disp_set_reg_up(dev);
}
#endif

td_void vou_drv_get_dev_int_state_vcnt(ot_vo_dev dev, td_u32 *vcnt)
{
    hal_disp_get_int_state_vcnt(dev, vcnt);
}

#ifdef CONFIG_OT_VO_EXPORT_FUNCTION
static td_s32 vo_export_check(ot_vo_dev dev)
{
    if (vo_is_vo_started() != TD_TRUE) {
        vo_err_trace("vo is not started!\n");
        return OT_ERR_VO_NOT_READY;
    }

    if (dev >= OT_VO_MAX_PHYS_DEV_NUM) {
        vo_err_trace("vo dev %d is not phy device, not support!\n", dev);
        return OT_ERR_VO_INVALID_DEV_ID;
    }

    return TD_SUCCESS;
}

/* 客户内核态使用  */
td_void ot_vo_enable_dev_export(ot_vo_dev dev)
{
    if (vo_export_check(dev) != TD_SUCCESS) {
        return;
    }

    hal_disp_set_intf_enable(dev, TD_TRUE);
    hal_disp_set_reg_up(dev);
}

/* 客户内核态使用 */
td_void ot_vo_disable_dev_export(ot_vo_dev dev)
{
    hal_disp_layer hal_layer;

    if (vo_export_check(dev) != TD_SUCCESS) {
        return;
    }
    /* 开机画面时使能了视频层，系统起来后没有关闭视频层，导致使能设备时看到残留画面，因此这里关闭视频层 */
    hal_layer = (hal_disp_layer)vo_drv_get_layer(dev);
    hal_video_set_all_area_disable(hal_layer);
    hal_layer_enable_layer(hal_layer, TD_FALSE);
    hal_layer_set_reg_up(hal_layer);
    hal_disp_set_intf_enable(dev, TD_FALSE);
    hal_disp_set_reg_up(dev);
    vo_drv_get_dev_reg_up(dev);
}

#else

td_void ot_vo_enable_dev_export(ot_vo_dev dev)
{
    vo_err_trace("current chip does not support export function!\n");
}

td_void ot_vo_disable_dev_export(ot_vo_dev dev)
{
    vo_err_trace("current chip does not support export function!\n");
}

#endif /* end of CONFIG_OT_VO_EXPORT_FUNCTION */

td_bool vou_drv_is_virt_dev(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = TD_NULL;

    if (vo_drv_check_dev_id(dev) != TD_SUCCESS) {
        return TD_FALSE;
    }

    drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    if (drv_dev_ctx->dev_cap.dev_type == VO_VIRT_DEV) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

#ifdef OT_DEBUG
td_u32 vo_drv_get_low_bandwidth_cnt(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->low_bandwidth_cnt;
}
#endif

#ifdef CONFIG_OT_VO_LOW_DELAY
td_u32 vo_drv_get_low_delay_err_cnt(ot_vo_dev dev)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    return drv_dev_ctx->low_delay_err;
}
#endif

#endif

#if vo_desc("dev intf")

td_void vo_drv_int_set_mode(ot_vo_dev dev, vo_int_mode int_mode)
{
    hal_disp_set_vtthd_mode(dev, int_mode);
}

#if (defined(CONFIG_OT_VO_CASCADE) || defined(CONFIG_OT_VO_VIRTDEV_SUPPORT))
/* check the device type is same with dev_type */
td_bool vo_drv_check_phy_dev_type(ot_vo_dev dev, vo_dev_type dev_type)
{
    vo_drv_dev *dev_ctx = vo_drv_get_dev_ctx(dev);
    if (dev_ctx->dev_cap.dev_type == dev_type) {
        return TD_TRUE;
    }

    return TD_FALSE;
}
#endif

td_bool vo_drv_is_virt_dev(ot_vo_dev dev)
{
#ifdef CONFIG_OT_VO_VIRTDEV_SUPPORT
    return vo_drv_check_phy_dev_type(dev, VO_VIRT_DEV);
#else
    ot_unused(dev);
#endif

    return TD_FALSE;
}

#ifdef CONFIG_OT_VO_VGA
td_s32 vo_drv_check_vga_csc_matrix(ot_vo_csc_matrix csc_matrix)
{
    if ((csc_matrix < OT_VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL) ||
        (csc_matrix > OT_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT)) {
        vo_err_trace("Vo CSC matrix type(%d) should be bt601_to_rgb/bt709_to_rgb only!\n", csc_matrix);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}
#endif

td_s32 vo_drv_check_hdmi_csc_matrix(ot_vo_csc_matrix csc_matrix)
{
    if (csc_matrix > OT_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT) {
        vo_err_trace("Vo CSC matrix type(%d) should be BT709_TO_BT601/BT601_TO_BT709/BT601_TO_RGB/BT709_TO_RGB only!\n",
            csc_matrix);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_VO_RGB
td_s32 vo_drv_check_rgb_csc_matrix(ot_vo_csc_matrix csc_matrix)
{
    if ((csc_matrix < OT_VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL) ||
        (csc_matrix > OT_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT)) {
        vo_err_trace("Vo CSC matrix type(%d) should be BT601_TO_RGB/BT709_TO_RGB only!\n", csc_matrix);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}
#endif

#ifdef CONFIG_OT_VO_MIPI
td_s32 vo_drv_check_mipi_csc_matrix(ot_vo_csc_matrix csc_matrix)
{
    if (csc_matrix > OT_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT) {
        vo_err_trace("Vo CSC matrix type(%d) should be BT709_TO_BT601/BT601_TO_BT709/BT601_TO_RGB/BT709_TO_RGB only!\n",
            csc_matrix);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}
#endif

#endif

#if vo_desc("dev interrupt")

static td_void vo_drv_set_int_mode(ot_vo_dev dev)
{
    ot_vo_layer layer;
    vo_int_mode int_mode;
    hal_disp_syncinfo sync_info;

    /* 中断产生方式/寄存器更新方式/数据读取方式设置(场或帧)，高清设备且时序为隔行时按场更新 */
    if (vo_drv_is_phy_dev_hd(dev) || vo_drv_is_phy_dev_sd(dev)) {
        layer = vo_drv_get_layer(dev);
        vo_drv_get_sync_info(dev, &sync_info);
        if (sync_info.iop == 0) {
            /* 隔行时序隔行显示 */
            int_mode = VO_INT_MODE_FIELD;
        } else {
            /* 逐行 */
            int_mode = VO_INT_MODE_FRAME;
        }

        vo_drv_int_set_mode(dev, int_mode);
        vo_drv_int_reg_up_mode(layer, int_mode);
    }
}

td_void vo_drv_do_int_vtth_config(ot_vo_dev dev, td_bool int_enable)
{
    vo_drv_set_int_mode(dev);
    vo_drv_set_vtth(dev);
    vo_drv_dev_int_enable(dev, int_enable);
}

td_void vou_drv_int_clear(td_u32 int_clear)
{
    hal_disp_clear_int_status(int_clear);
}

td_void vou_drv_int_disable_all(td_void)
{
    hal_disp_clr_int_mask(VO_INTREPORT_ALL);
}

td_void vou_drv_low_bandwidth(ot_vo_dev dev, td_u32 int_status)
{
    vo_drv_dev *drv_dev_ctx = vo_drv_get_dev_ctx(dev);
    td_u32 int_mask = vo_drv_get_lbw_int_mask(dev);
    if ((int_status & int_mask) != 0) {
        drv_dev_ctx->low_bandwidth_cnt++;
        vo_warn_trace("dev(%d) has %u low bandwidth!\n", dev, drv_dev_ctx->low_bandwidth_cnt);
    }

    /* 清除对应设备的低带宽中断状态 */
    vou_drv_int_clear(int_mask);
}

td_void vo_drv_get_dev_div_pre_div(ot_vo_dev dev, td_u32 *pre_div, td_u32 *dev_div)
{
    td_u32 hdmi_ssc_vdp_div = 0;
    td_u32 div_mode = 0;
    vo_drv_dev *dev_ctx = vo_drv_get_dev_ctx(dev);
    ot_vo_intf_sync intf_sync = dev_ctx->vou_attr.intf_sync;

    /* 前置分频比只有是HDMI接口才生效，进行判断，避免无效配置时的计算错误 */
    if (vo_drv_is_hdmi_intf(dev_ctx->vou_attr.intf_type) ||
        vo_drv_is_hdmi1_intf(dev_ctx->vou_attr.intf_type)) {
        vo_drv_get_hdmi_ssc_vdp_div(intf_sync, &hdmi_ssc_vdp_div);
    }

    vo_drv_get_div_mod(dev, &div_mode);
    *pre_div = hdmi_ssc_vdp_div + 1;
    *dev_div = div_mode + 1;
}

td_s32 vo_drv_get_dev_clk_src_pll(const ot_vo_pll *pll, td_u32 *src_clk)
{
    td_u64 foutvco;
    td_u64 foutvco_fb_div;
    td_u64 foutvco_frac;

    if ((pll->ref_div == 0) || (pll->post_div1 == 0) || (pll->post_div2 == 0)) {
        vo_err_trace("pll is invalid, ref_div %u post_div1 %u post_div2 %u, can not be 0\n",
            pll->ref_div, pll->post_div1, pll->post_div2);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    foutvco_fb_div = osal_div64_u64((VO_PLL_FREF * VO_CLK_MHZ_UNIT * (td_u64)pll->fb_div), (td_u64)pll->ref_div);
    foutvco_frac = osal_div64_u64((VO_PLL_FREF * VO_CLK_MHZ_UNIT * (td_u64)pll->frac),
        VO_PLL_FRAC_PREC * (td_u64)pll->ref_div);
    foutvco = foutvco_fb_div + foutvco_frac;
    *src_clk = (td_u32)osal_div64_u64(foutvco, (td_u64)(pll->post_div1 * pll->post_div2));
    return TD_SUCCESS;
}
#endif

#if vo_desc("cascade")

td_bool vo_drv_is_cas_dev(ot_vo_dev dev)
{
#ifdef CONFIG_OT_VO_CASCADE
    return vo_drv_check_phy_dev_type(dev, VO_CAS_DEV);
#else
    ot_unused(dev);
    return TD_FALSE;
#endif
}

#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
