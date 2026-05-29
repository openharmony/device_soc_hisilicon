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

#include "mkp_vo_proc.h"
#include "mkp_vo_dev.h"
#include "mkp_vo_cascade.h"
#include "sys_ext.h"
#include "drv_vo_dev_comm.h"
#include "drv_vo_dev.h"
#include "mkp_vo_init.h"

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT

#if vo_desc("print public")

static const td_char *vo_proc_print_bool(td_bool value)
{
    return (value == TD_TRUE) ? "Y" : "N";
}

#endif

#if vo_desc("check public")

static td_bool vo_proc_is_dev_enable(const vo_dev_info *dev_ctx)
{
    if (dev_ctx == TD_NULL) {
        return TD_FALSE;
    }

    return dev_ctx->vo_enable;
}

static td_bool vo_proc_is_user_dev_enable(const vo_dev_info *dev_ctx)
{
    if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
        return TD_FALSE;
    }

    if (dev_ctx->vou_attr.intf_sync != OT_VO_OUT_USER) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

#endif

#if vo_desc("version")

static td_void vo_proc_show_version(osal_proc_entry_t *s)
{
    osal_seq_printf(s, "\n[VO] Version: [" OT_MPP_VERSION "], Build Time["__DATE__", "__TIME__"]\n\n");
}

#endif

#if vo_desc("module param")

static td_void vo_proc_show_mod_param(osal_proc_entry_t *s)
{
    ot_vo_mod_param *vo_mod_param = vo_get_vo_mod_param();

    call_sys_print_proc_title(s, "vo module param");
    osal_seq_printf(s, "%13s%16s%15s%19s\n", "exit_dev_en", "dev_clk_ext_en", "vga_detect_en", "vdac_detect_cycle");
    osal_seq_printf(s, "%13s%16s%15s%19u\n", vo_proc_print_bool(vo_mod_param->exit_dev_en),
        vo_proc_print_bool(vo_mod_param->dev_clk_ext_en), vo_proc_print_bool(vo_mod_param->vga_detect_en),
        vo_mod_param->vdac_detect_cycle);
}

#endif

#if vo_desc("dev")

static const td_char *vo_proc_get_intf_mux_name(td_u32 intf_type_index)
{
    const td_char *intf_type_name[VO_MAX_INTF_TYPE_NUM] = {
        "     CVBS", "      VGA", "   BT.656", "  BT.1120", "     HDMI", " RGB_6BIT",
        " RGB_8BIT", "RGB_16BIT", "RGB_18BIT", "RGB_24BIT", "     MIPI", "   MIPI_S",
        "    HDMI1"
        };

    if (intf_type_index >= VO_MAX_INTF_TYPE_NUM) {
        return "         ";
    }

    return intf_type_name[intf_type_index];
}

static td_s32 vo_proc_get_intf_mux(td_u32 intf_type_index, td_char *mux_name, td_u32 mux_name_len)
{
    td_s32 ret;

    ret = strncpy_s(mux_name, mux_name_len, vo_proc_get_intf_mux_name(intf_type_index), VO_MAX_INTFNAME_LEN - 1);
    if (ret != EOK) {
        vo_err_trace("get intf type index %u mux name fail\n", intf_type_index);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 vo_proc_print_mux(ot_vo_intf_type intf_type, td_char (*mux_name)[VO_MAX_INTFNAME_LEN])
{
    td_s32 ret;
    td_u32 intf_type_index;
    td_u32 mux_index;
    td_u32 mux_num = 0;

    /* 每次传递进来时都需要初始化下，因为可能不是默认值 */
    for (mux_index = 0; mux_index < VO_MAX_MUX_NUM; mux_index++) {
        ret = strncpy_s(mux_name[mux_index], VO_MAX_INTFNAME_LEN, "         ", VO_MAX_INTFNAME_LEN - 1);
        if (ret != EOK) {
            vo_err_trace("init mux name fail\n");
            return TD_FAILURE;
        }
    }

    for (intf_type_index = 0; intf_type_index < VO_MAX_INTF_TYPE_NUM; intf_type_index++) {
        if ((intf_type >> intf_type_index) & 0x01L) {
            /* 数组访问越界保护,字符数组只有4个,超过后跳出循环,以免数组访问越界 */
            if (mux_num >= VO_MAX_MUX_NUM) {
                break;
            }

            ret = vo_proc_get_intf_mux(intf_type_index, mux_name[mux_num], VO_MAX_INTFNAME_LEN);
            if (ret != TD_SUCCESS) {
                return TD_FAILURE;
            }
            mux_num++;
        }
    }

    for (mux_index = 0; mux_index < VO_MAX_MUX_NUM; mux_index++) {
        mux_name[mux_index][VO_MAX_INTFNAME_LEN - 1] = '\0';
    }

    return TD_SUCCESS;
}

static td_char *vo_proc_print_intf_sync(ot_vo_intf_sync intf_sync)
{
    return vo_get_intf_sync_name(intf_sync);
}

static const td_char *vo_proc_print_clk_src(ot_vo_clk_src clk_src)
{
    const td_char *clk_src_name[OT_VO_CLK_SRC_BUTT] = {"PLL", "LCDMCLK", "PLL FOUT4", "FIXED"};

    if (clk_src >= OT_VO_CLK_SRC_BUTT) {
        return "-";
    }

    return clk_src_name[clk_src];
}

static td_void vo_proc_show_dev_cfg(osal_proc_entry_t *s)
{
    td_s32 ret;
    ot_vo_dev dev;
    td_u32 src_clk = 0;
    td_u32 pixel_clk = 0;
    vo_dev_info *dev_ctx = TD_NULL;
    td_char mux_name[VO_MAX_MUX_NUM][VO_MAX_INTFNAME_LEN] = {0};

    call_sys_print_proc_title(s, "vo device config");
    osal_seq_printf(s, "%8s%8s%11s%11s%11s%11s%14s%10s%16s%9s%11s\n", "dev_id", "dev_en", "mux1", "mux2",
        "mux3", "mux4", "intf_sync", "bg_color", "dev_frame_rate", "src_clk", "pixel_clk");

    for (dev = 0; dev < OT_VO_MAX_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        osal_seq_printf(s, "%8u%8s", dev, vo_proc_print_bool(dev_ctx->vo_enable));
        ret = vo_proc_print_mux(dev_ctx->vou_attr.intf_type, mux_name);
        if (ret != TD_SUCCESS) {
            osal_seq_printf(s, "%11s%11s%11s%11s", "", "", "", "");
        } else {
            osal_seq_printf(s, "%11s%11s%11s%11s", mux_name[VO_INTF_MUX0], mux_name[VO_INTF_MUX1],
                mux_name[VO_INTF_MUX2], mux_name[VO_INTF_MUX3]);
        }

        osal_seq_printf(s, "%14s%#10x%16u", vo_proc_print_intf_sync(dev_ctx->vou_attr.intf_sync),
            dev_ctx->vou_attr.bg_color, dev_ctx->full_frame_rate);
        ret = vo_get_dev_src_pixel_clk(dev, &src_clk, &pixel_clk);
        if (ret != TD_SUCCESS) {
            osal_seq_printf(s, "%9s%11s\n", "-", "-");
        } else {
            osal_seq_printf(s, "%9u%11u\n", src_clk, pixel_clk);
        }
    }
}

static td_void vo_proc_show_dev_sync(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    ot_vo_sync_info *sync_info = TD_NULL;

    call_sys_print_proc_title(s, "vo device user sync info");
    osal_seq_printf(s, "%8s%7s%5s%7s%6s%5s%5s%6s%5s%5s%6s%7s%6s%6s%5s%5s%5s%5s%5s\n",
                    "dev_id", "syncm", "iop", "intfb", "vact", "vbb", "vfb", "hact", "hbb", "hfb", "hmid",
                    "bvact", "bvbb", "bvfb", "hpw", "vpw", "idv", "ihs", "ivs");

    for (dev = 0; dev < OT_VO_MAX_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_user_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        sync_info = &dev_ctx->vou_attr.sync_info;
        osal_seq_printf(s, "%8u%7u%5u%7u%6u%5u%5u%6u%5u%5u%6u%7u%6u%6u%5u%5u%5u%5u%5u\n", dev,
                        sync_info->syncm, sync_info->iop, sync_info->intfb,
                        sync_info->vact, sync_info->vbb, sync_info->vfb,
                        sync_info->hact, sync_info->hbb, sync_info->hfb, sync_info->hmid,
                        sync_info->bvact, sync_info->bvbb, sync_info->bvfb,
                        sync_info->hpw, sync_info->vpw,
                        sync_info->idv, sync_info->ihs, sync_info->ivs);
    }
}

static td_void vo_proc_show_dev_clk(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    ot_vo_user_sync_attr *user_sync_attr = TD_NULL;
    ot_vo_pll *vo_pll = TD_NULL;

    call_sys_print_proc_title(s, "vo device clock info");

    osal_seq_printf(s, "%8s%10s%8s%9s%9s%11s%11s%11s%7s%9s%9s%16s\n",
                    "dev_id", "clk_src", "fb_div", "frac", "ref_div", "post_div1", "post_div2", "lcd_m_clk", "fixed",
                    "dev_div", "pre_div", "clk_reverse_en");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_user_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        user_sync_attr = &dev_ctx->vo_user_sync_info.user_sync_attr;
        vo_pll = &dev_ctx->vo_user_sync_info.user_sync_attr.vo_pll;

        osal_seq_printf(s, "%8u%10s", dev, vo_proc_print_clk_src(user_sync_attr->clk_src));

        if ((user_sync_attr->clk_src == OT_VO_CLK_SRC_PLL) ||
            (user_sync_attr->clk_src == OT_VO_CLK_SRC_PLL_FOUT4)) {
            osal_seq_printf(s, "%8u%9u%9u%11u%11u",
                            vo_pll->fb_div, vo_pll->frac, vo_pll->ref_div,
                            vo_pll->post_div1, vo_pll->post_div2);

            osal_seq_printf(s, "%27u", dev_ctx->vo_user_sync_info.dev_div);
            osal_seq_printf(s, "%9u", dev_ctx->vo_user_sync_info.pre_div);
        } else if (user_sync_attr->clk_src == OT_VO_CLK_SRC_LCDMCLK) {
            osal_seq_printf(s, "%59u", user_sync_attr->lcd_m_clk_div);
            osal_seq_printf(s, "%16u", dev_ctx->vo_user_sync_info.dev_div);
            osal_seq_printf(s, "%9u", dev_ctx->vo_user_sync_info.pre_div);
        } else {
            osal_seq_printf(s, "%66u", user_sync_attr->fixed_clk);
            osal_seq_printf(s, "%9u", dev_ctx->vo_user_sync_info.dev_div);
            osal_seq_printf(s, "%9u", dev_ctx->vo_user_sync_info.pre_div);
        }

        osal_seq_printf(s, "%16s\n", vo_proc_print_bool(dev_ctx->vo_user_sync_info.clk_reverse_en));
    }
}

static td_void vo_proc_show_dev_vtth_cfg(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    td_u32 vtth;
    vo_dev_info *dev_ctx = TD_NULL;

    call_sys_print_proc_title(s, "vo device vtth and param config");

    osal_seq_printf(s, "%8s%6s%17s%15s%17s%20s%14s%13s\n", "dev_id", "vtth",
        "less_buf_enable", "less_buf_vtth", "user_notify_en", "user_notify_vtth", "vo_bypass_en", "uboot_vo_en");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        /* 只有物理设备才支持，获取时需要保证有效性，防止越界 */
        vtth = vo_drv_get_dev_vtth(dev);
        osal_seq_printf(s, "%8d%6d%17s%15d%17s%20d%14s%13s\n", dev, vtth,
            vo_proc_print_bool(dev_ctx->less_buf_attr.enable), dev_ctx->less_buf_attr.vtth,
            vo_proc_print_bool(dev_ctx->user_notify_attr.enable), dev_ctx->user_notify_attr.vtth,
            vo_proc_print_bool(dev_ctx->dev_param.vo_bypass_en),
            vo_proc_print_bool(dev_ctx->uboot_vo_enabled));
    }
}

static td_void vo_proc_show_dev(osal_proc_entry_t *s)
{
    vo_proc_show_dev_cfg(s);
    vo_proc_show_dev_sync(s);
    vo_proc_show_dev_clk(s);
    vo_proc_show_dev_vtth_cfg(s);
}

#endif

#if vo_desc("interface")

static td_void vo_proc_show_intf_status(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    td_bool vdac_status;

    call_sys_print_proc_title(s, "vo interface status");
    osal_seq_printf(s, "%8s%13s\n", "dev_id", "vdac_status");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        if (vo_drv_is_dev_support_load_detect(dev) != TD_TRUE) {
            osal_seq_printf(s, "%8u%13s\n", dev, "-");
            continue;
        }

        vdac_status = vo_drv_get_vdac_status(dev);
        osal_seq_printf(s, "%8u%13s\n", dev, vo_proc_print_bool(vdac_status));
    }
}

#ifdef CONFIG_OT_VO_VGA
static td_void vo_proc_show_intf_vga_param(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    ot_vo_vga_param *vga_param = TD_NULL;

    call_sys_print_proc_title(s, "vo interface vga param");
    osal_seq_printf(s, "%8s%8s%6s%10s%5s%12s%11s%6s%18s\n", "dev_id", "matrix", "luma", "contrast", "hue",
                    "saturation", "ex_csc_en", "gain", "sharpen_strength");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        if (vo_drv_is_vga_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
            continue;
        }

        vga_param = &(dev_ctx->vga_param);
        osal_seq_printf(s, "%8u%8u%6u%10u%5u%12u%11u%6u%18u\n", dev, vga_param->csc.csc_matrix, vga_param->csc.luma,
                        vga_param->csc.contrast, vga_param->csc.hue, vga_param->csc.saturation,
                        vga_param->csc.ex_csc_en, vga_param->gain, vga_param->sharpen_strength);
    }
}
#endif

#ifdef CONFIG_OT_VO_HDMI
static td_void vo_proc_show_intf_hdmi_param(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    ot_vo_hdmi_param *hdmi_param = TD_NULL;

    call_sys_print_proc_title(s, "vo interface hdmi param");
    osal_seq_printf(s, "%8s%8s%6s%10s%5s%12s%11s\n", "dev_id", "matrix", "luma", "contrast", "hue",
                    "saturation", "ex_csc_en");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        if (vo_drv_is_hdmi_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
            continue;
        }

        hdmi_param = &(dev_ctx->hdmi_param);
        osal_seq_printf(s, "%8u%8u%6u%10u%5u%12u%11u\n", dev, hdmi_param->csc.csc_matrix, hdmi_param->csc.luma,
                        hdmi_param->csc.contrast, hdmi_param->csc.hue, hdmi_param->csc.saturation,
                        hdmi_param->csc.ex_csc_en);
    }
}
#endif

#ifdef CONFIG_OT_VO_HDMI1
static td_void vo_proc_show_intf_hdmi1_param(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    ot_vo_hdmi_param *hdmi1_param = TD_NULL;

    call_sys_print_proc_title(s, "vo interface hdmi1 param");
    osal_seq_printf(s, "%8s%8s%6s%10s%5s%12s%11s\n", "dev_id", "matrix", "luma", "contrast", "hue",
                    "saturation", "ex_csc_en");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        if (vo_drv_is_hdmi1_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
            continue;
        }

        hdmi1_param = &(dev_ctx->hdmi1_param);
        osal_seq_printf(s, "%8u%8u%6u%10u%5u%12u%11u\n", dev, hdmi1_param->csc.csc_matrix, hdmi1_param->csc.luma,
                        hdmi1_param->csc.contrast, hdmi1_param->csc.hue, hdmi1_param->csc.saturation,
                        hdmi1_param->csc.ex_csc_en);
    }
}
#endif

#ifdef CONFIG_OT_VO_RGB
static td_void vo_proc_show_intf_rgb_param(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    ot_vo_rgb_param *param = TD_NULL;

    call_sys_print_proc_title(s, "vo interface rgb param");
    osal_seq_printf(s, "%8s%8s%6s%10s%5s%12s%11s%13s%14s\n", "dev_id", "matrix", "luma", "contrast", "hue",
                    "saturation", "ex_csc_en", "rgb_inverted", "bit_inverted");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        if (vo_drv_is_rgb_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
            continue;
        }

        param = &(dev_ctx->rgb_param);
        osal_seq_printf(s, "%8u%8u%6u%10u%5u%12u%11u%13s%14s\n", dev, param->csc.csc_matrix, param->csc.luma,
                        param->csc.contrast, param->csc.hue, param->csc.saturation,
                        param->csc.ex_csc_en, vo_proc_print_bool(param->rgb_inverted_en),
                        vo_proc_print_bool(param->bit_inverted_en));
    }
}
#endif

#ifdef CONFIG_OT_VO_BT1120

static const td_char *vo_proc_print_clk_edge(ot_vo_clk_edge clk_edge)
{
    const td_char *clk_edge_name[OT_VO_CLK_EDGE_BUTT] = {"SINGLE", "DUAL"};

    if (clk_edge >= OT_VO_CLK_EDGE_BUTT) {
        return "";
    }

    return clk_edge_name[clk_edge];
}

static const td_char *vo_proc_get_bt_param_clk_edge(ot_vo_dev dev)
{
    ot_vo_clk_edge clk_edge;

#ifdef CONFIG_OT_VO_CASCADE
    /* 级联开启时此配置无效，显示为无效值 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx->dev_cas.cas_config == TD_TRUE) {
        return "-";
    }
#endif

    clk_edge = vo_drv_get_bt_param_clk_edge(dev);
    return vo_proc_print_clk_edge(clk_edge);
}

static td_void vo_proc_show_intf_bt_param(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    ot_vo_bt_param *param = TD_NULL;

    call_sys_print_proc_title(s, "vo interface bt.1120/bt.656 param");
    osal_seq_printf(s, "%8s%13s%14s%10s\n", "dev_id", "yc_inverted", "bit_inverted", "clk_edge");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        if (vo_drv_is_bt_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
            continue;
        }

        param = &(dev_ctx->bt_param);
        osal_seq_printf(s, "%8u%13s%14s%10s\n", dev, vo_proc_print_bool(param->yc_inverted_en),
                        vo_proc_print_bool(param->bit_inverted_en), vo_proc_get_bt_param_clk_edge(dev));
    }
}
#endif

#ifdef CONFIG_OT_VO_MIPI
static td_void vo_proc_show_intf_mipi_param(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    ot_vo_mipi_param *mipi_param = TD_NULL;

    call_sys_print_proc_title(s, "vo interface mipi param");
    osal_seq_printf(s, "%8s%8s%6s%10s%5s%12s%11s\n", "dev_id", "matrix", "luma", "contrast", "hue",
                    "saturation", "ex_csc_en");

    /* 仅支持物理设备 */
    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        if (vo_drv_is_mipi_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
            continue;
        }

        mipi_param = &(dev_ctx->mipi_param);
        osal_seq_printf(s, "%8u%8u%6u%10u%5u%12u%11u\n", dev, mipi_param->csc.csc_matrix, mipi_param->csc.luma,
                        mipi_param->csc.contrast, mipi_param->csc.hue, mipi_param->csc.saturation,
                        mipi_param->csc.ex_csc_en);
    }
}
#endif

static td_void vo_proc_show_intf(osal_proc_entry_t *s)
{
    vo_proc_show_intf_status(s);

#ifdef CONFIG_OT_VO_VGA
    vo_proc_show_intf_vga_param(s);
#endif

#ifdef CONFIG_OT_VO_BT1120
    vo_proc_show_intf_bt_param(s);
#endif

#ifdef CONFIG_OT_VO_HDMI
    vo_proc_show_intf_hdmi_param(s);
#endif

#ifdef CONFIG_OT_VO_HDMI1
    vo_proc_show_intf_hdmi1_param(s);
#endif

#ifdef CONFIG_OT_VO_RGB
    vo_proc_show_intf_rgb_param(s);
#endif

#ifdef CONFIG_OT_VO_MIPI
    vo_proc_show_intf_mipi_param(s);
#endif
}

#endif

#if vo_desc("cascade")

#ifdef CONFIG_OT_VO_CASCADE

static const td_char *vo_proc_print_cas_rgn(ot_vo_cas_rgn cas_rgn)
{
    const td_char *cas_rgn_name[OT_VO_CAS_RGN_BUTT] = {"64Rgn", "32Rgn"};

    if (cas_rgn >= OT_VO_CAS_RGN_BUTT) {
        return "";
    }

    return cas_rgn_name[cas_rgn];
}

static const td_char *vo_proc_print_cas_mode(ot_vo_cas_mode cas_mode)
{
    const td_char *cas_mode_name[OT_VO_CAS_MODE_BUTT] = {"SINGLE", "DUAL"};

    if (cas_mode >= OT_VO_CAS_MODE_BUTT) {
        return "";
    }

    return cas_mode_name[cas_mode];
}

static const td_char *vo_proc_print_cas_edge_mode(ot_vo_cas_data_transmission_mode cas_edge_mode)
{
    const td_char *cas_edge_mode_name[OT_VO_CAS_MODE_EDGE_BUTT] = {"SINGLE_EDGE", "DUAL_EDGE"};

    if (cas_edge_mode >= OT_VO_CAS_MODE_EDGE_BUTT) {
        return "";
    }

    return cas_edge_mode_name[cas_edge_mode];
}

static td_void vo_proc_show_dev_cas(osal_proc_entry_t *s)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;
    vo_dev_cas_info *dev_cas = TD_NULL;

    call_sys_print_proc_title(s, "vo device cas config");

    osal_seq_printf(s, "%8s%12s%12s%13s%11s%9s%10s%15s\n", "dev_id", "cas_config", "cas_enable", "cas_pattern",
        "is_slave", "cas_rgn", "cas_mode", "cas_edge_mode");

    /* 只显示物理设备0和级联设备 */
    for (dev = 0; dev < OT_VO_MAX_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (vo_proc_is_dev_enable(dev_ctx) != TD_TRUE) {
            continue;
        }

        if ((vo_drv_is_virt_dev(dev) == TD_TRUE) ||
            ((vo_is_phy_dev(dev) == TD_TRUE) && (dev != vo_drv_get_cas_phy_dev()))) {
            continue;
        }

        dev_cas = &(dev_ctx->dev_cas);
        osal_seq_printf(s, "%8d%12s%12s%13d", dev, vo_proc_print_bool(dev_cas->cas_config),
            vo_proc_print_bool(dev_cas->cas_enable), dev_cas->cas_pattern);

        if (dev_ctx->dev_cas.cas_config != TD_TRUE) {
            osal_seq_printf(s, "%11s%9s%10s%15s\n", "", "", "", "");
            continue;
        }

        osal_seq_printf(s, "%11s%9s%10s%15s\n", vo_proc_print_bool(dev_cas->cas_attr.is_slave),
            vo_proc_print_cas_rgn(dev_cas->cas_attr.cas_rgn), vo_proc_print_cas_mode(dev_cas->cas_attr.cas_mode),
            vo_proc_print_cas_edge_mode(dev_cas->cas_attr.cas_edge_mode));
    }
}

static td_void vo_proc_show_cas(osal_proc_entry_t *s)
{
    vo_proc_show_dev_cas(s);
}

#endif

#endif

#if vo_desc("proc show")

td_s32 vo_proc_show(osal_proc_entry_t *s)
{
    vo_proc_show_version(s);

    /* 未初始化仅显示大标题，不显示子标题 */
    if (vo_is_vo_started() != TD_TRUE) {
        return TD_SUCCESS;
    }

    if ((ckfn_sys_entry() == TD_FALSE) || (ckfn_sys_print_proc_title() == TD_FALSE)) {
        vo_err_trace("no sys ko!\n");
        return OT_ERR_VO_NOT_READY;
    }

    vo_down_sem_return();
    vo_proc_show_mod_param(s);
    vo_proc_show_dev(s);
    vo_proc_show_intf(s);

#ifdef CONFIG_OT_VO_CASCADE
    vo_proc_show_cas(s);
#endif

    vo_up_sem();
    return TD_SUCCESS;
}

#endif /* end of #if vo_desc("proc show") */

#endif /* end of #ifdef CONFIG_OT_PROC_SHOW_SUPPORT */
