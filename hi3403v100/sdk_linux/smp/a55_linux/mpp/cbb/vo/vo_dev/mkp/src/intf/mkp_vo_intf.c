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
#include "mkp_vo_intf.h"
#include "vou.h"
#include "mkp_vo_init.h"

#if vo_desc("UBOOT_VO")
#ifdef CONFIG_OT_HDMI_SUPPORT
td_void vo_init_hdmi_param(ot_vo_hdmi_param *hdmi_param)
{
    hdmi_param->csc.csc_matrix = OT_VO_CSC_MATRIX_BT601FULL_TO_BT601FULL;
    hdmi_param->csc.luma = VO_CSC_DEF_VAL;
    hdmi_param->csc.contrast = VO_CSC_DEF_VAL;
    hdmi_param->csc.hue = VO_CSC_DEF_VAL;
    hdmi_param->csc.saturation = VO_CSC_DEF_VAL;
    hdmi_param->csc.ex_csc_en = TD_FALSE;
}
#endif

#ifdef CONFIG_OT_VO_VGA
td_void vo_init_vga_param(ot_vo_vga_param *vga_param)
{
    vga_param->csc.csc_matrix = OT_VO_CSC_MATRIX_BT601FULL_TO_RGBFULL;
    vga_param->csc.luma = VO_CSC_DEF_VAL;
    vga_param->csc.contrast = VO_CSC_DEF_VAL;
    vga_param->csc.hue = VO_CSC_DEF_VAL;
    vga_param->csc.saturation = VO_CSC_DEF_VAL;
    vga_param->csc.ex_csc_en = TD_FALSE;
    vga_param->gain = vo_drv_get_vga_gain_def();
    vga_param->sharpen_strength = VO_VGA_SHARPEN_STRENGTH_DEF;
}
#endif

#ifdef CONFIG_OT_VO_RGB
td_void vo_init_rgb_param(ot_vo_rgb_param *rgb_param)
{
    rgb_param->csc.csc_matrix = OT_VO_CSC_MATRIX_BT601FULL_TO_RGBFULL;
    rgb_param->csc.luma = VO_CSC_DEF_VAL;
    rgb_param->csc.contrast = VO_CSC_DEF_VAL;
    rgb_param->csc.hue = VO_CSC_DEF_VAL;
    rgb_param->csc.saturation = VO_CSC_DEF_VAL;
    rgb_param->csc.ex_csc_en = TD_FALSE;
    rgb_param->rgb_inverted_en = TD_FALSE;
    rgb_param->bit_inverted_en = TD_FALSE;
}
#endif

#ifdef CONFIG_OT_VO_BT1120
td_void vo_init_bt_param(ot_vo_bt_param *bt_param)
{
    bt_param->yc_inverted_en = TD_FALSE;
    bt_param->bit_inverted_en = TD_FALSE;
    bt_param->clk_edge = OT_VO_CLK_EDGE_SINGLE;
}
#endif

#ifdef CONFIG_OT_VO_MIPI
td_void vo_init_mipi_param(ot_vo_mipi_param *mipi_param)
{
    mipi_param->csc.csc_matrix = OT_VO_CSC_MATRIX_BT601FULL_TO_RGBFULL;
    mipi_param->csc.luma = VO_CSC_DEF_VAL;
    mipi_param->csc.contrast = VO_CSC_DEF_VAL;
    mipi_param->csc.hue = VO_CSC_DEF_VAL;
    mipi_param->csc.saturation = VO_CSC_DEF_VAL;
    mipi_param->csc.ex_csc_en = TD_FALSE;
}
#endif

td_void vo_init_dev_intf_param(vo_dev_info *dev_ctx)
{
#ifdef CONFIG_OT_VO_HDMI
    vo_init_hdmi_param(&dev_ctx->hdmi_param);
#endif

#ifdef CONFIG_OT_VO_HDMI1
    vo_init_hdmi_param(&dev_ctx->hdmi1_param);
#endif

#ifdef CONFIG_OT_VO_VGA
    vo_init_vga_param(&dev_ctx->vga_param);
    dev_ctx->vga_param_config = TD_FALSE;
#endif

#ifdef CONFIG_OT_VO_RGB
    vo_init_rgb_param(&dev_ctx->rgb_param);
#endif

#ifdef CONFIG_OT_VO_BT1120
    vo_init_bt_param(&dev_ctx->bt_param);
#endif

#ifdef CONFIG_OT_VO_MIPI
    vo_init_mipi_param(&dev_ctx->mipi_param);
#endif
}

#if vo_desc("hdmi color space")

#ifdef CONFIG_OT_HDMI_SUPPORT
static td_void vo_get_hdmi_yuv_csc_param(ot_vo_csc_matrix csc_matrix, hdmi_csc_param *csc_param)
{
    if (csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_BT601LIMIT) {
        csc_param->colorimetry = HDMI_COLORIMETRY_ITU601;
        csc_param->quantization = HDMI_QUANT_RANGE_LIMITED;
    } else if (csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_BT709LIMIT) {
        csc_param->colorimetry = HDMI_COLORIMETRY_ITU709;
        csc_param->quantization = HDMI_QUANT_RANGE_LIMITED;
    } else if (csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_BT601FULL) {
        csc_param->colorimetry = HDMI_COLORIMETRY_ITU601;
        csc_param->quantization = HDMI_QUANT_RANGE_FULL;
    } else if (csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_BT709FULL) {
        csc_param->colorimetry = HDMI_COLORIMETRY_ITU709;
        csc_param->quantization = HDMI_QUANT_RANGE_FULL;
    }
}

static td_void vo_get_hdmi_rgb_csc_param(ot_vo_csc_matrix csc_matrix, hdmi_csc_param *csc_param)
{
    if (csc_matrix <= OT_VO_CSC_MATRIX_BT601FULL_TO_RGBFULL) {
        csc_param->colorimetry = HDMI_COLORIMETRY_ITU601;
        csc_param->quantization = HDMI_QUANT_RANGE_FULL;
    } else if (csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_RGBFULL) {
        csc_param->colorimetry = HDMI_COLORIMETRY_ITU709;
        csc_param->quantization = HDMI_QUANT_RANGE_FULL;
    } else if (csc_matrix <= OT_VO_CSC_MATRIX_BT601FULL_TO_RGBLIMIT) {
        csc_param->colorimetry = HDMI_COLORIMETRY_ITU601;
        csc_param->quantization = HDMI_QUANT_RANGE_LIMITED;
    } else if (csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT) {
        csc_param->colorimetry = HDMI_COLORIMETRY_ITU709;
        csc_param->quantization = HDMI_QUANT_RANGE_LIMITED;
    }
}

static td_void vo_get_hdmi_color_space_param(ot_vo_csc_matrix csc_matrix,
    hdmi_video_param *video_param, hdmi_csc_param *csc_param)
{
    /* 上层函数可保证输入的HDMI的csc矩阵的有效性 */
    if ((csc_matrix >= OT_VO_CSC_MATRIX_BT601LIMIT_TO_BT601LIMIT) &&
        (csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_BT709FULL)) {
        video_param->pixel_encoding = HDMI_EXT_COLORSPACE_YCBCR444;
        vo_get_hdmi_yuv_csc_param(csc_matrix,  csc_param);
    } else {
        video_param->pixel_encoding = HDMI_EXT_COLORSPACE_RGB;
        vo_get_hdmi_rgb_csc_param(csc_matrix, csc_param);
    }
}

static hdmi_dev_id vo_get_hdmi_id_by_intf_type(ot_vo_intf_type intf_type)
{
    hdmi_dev_id hdmi_id = HDMI_DEV_ID_0;

    /* 上层函数保证输入接口类型只能为HDMI和HDMI1 */
#ifdef CONFIG_OT_VO_HDMI1
    if (intf_type == OT_VO_INTF_HDMI1) {
        hdmi_id = HDMI_DEV_ID_1;
    }
#else
    ot_unused(intf_type);
#endif

    return hdmi_id;
}

static td_s32 vo_set_hdmi_color_space(ot_vo_csc_matrix csc_matrix, ot_vo_intf_type intf_type)
{
    td_s32 ret;
    hdmi_dev_id hdmi_id;
    hdmi_video_param video_param = {0};
    hdmi_csc_param csc_param = {0};
    hdmi_export_func *hdmi_func = TD_NULL;

    hdmi_func = func_entry(hdmi_export_func, OT_ID_HDMI);
    if ((hdmi_func == TD_NULL) ||
        (hdmi_func->pfn_video_param_set == TD_NULL) ||
        (hdmi_func->pfn_csc_param_set == TD_NULL)) {
        vo_err_trace("hdmi's export funcs is null!\n");
        return OT_ERR_VO_NOT_SUPPORT;
    }

    /* HDMI要求需要先设置VIDEO参数,在设置CSC参数，不然可能在设置CSC参数出错 */
    hdmi_id = vo_get_hdmi_id_by_intf_type(intf_type);
    vo_get_hdmi_color_space_param(csc_matrix, &video_param, &csc_param);
    ret = hdmi_func->pfn_video_param_set(hdmi_id, &video_param);
    if (ret != TD_SUCCESS) {
        vo_err_trace("set hdmi id %u video param err 0x%x!\n", hdmi_id, ret);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    ret = hdmi_func->pfn_csc_param_set(hdmi_id, &csc_param);
    if (ret != TD_SUCCESS) {
        vo_err_trace("set hdmi id %u csc param err 0x%x!\n", hdmi_id, ret);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    return TD_SUCCESS;
}

#endif

#endif

#if vo_desc("intf param")

td_void vo_set_dev_intf_param(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    ot_vo_intf_type intf_type = dev_ctx->vou_attr.intf_type;

#ifdef CONFIG_OT_VO_HDMI
    if (vo_drv_is_hdmi_intf(intf_type)) {
        vo_drv_set_hdmi_param(dev, &dev_ctx->hdmi_param);
        /* 启动VO设备时不排除HDMI模块没启动场景，此时不关心返回值, HDMI接口无输出 */
        if (vo_set_hdmi_color_space(dev_ctx->hdmi_param.csc.csc_matrix, OT_VO_INTF_HDMI) != TD_SUCCESS) {
            vo_err_trace("vo device %d set hdmi color space fail when enable vo dev!\n", dev);
        }
    }
#endif

#ifdef CONFIG_OT_VO_HDMI1
    if (vo_drv_is_hdmi1_intf(intf_type)) {
        vo_drv_set_hdmi1_param(dev, &dev_ctx->hdmi1_param);
        /* 启动VO设备时不排除HDMI模块没启动场景，此时不关心返回值, HDMI接口无输出 */
        if (vo_set_hdmi_color_space(dev_ctx->hdmi1_param.csc.csc_matrix, OT_VO_INTF_HDMI1) != TD_SUCCESS) {
            vo_err_trace("vo device %d set hdmi1 color space fail when enable vo dev!\n", dev);
        }
    }
#endif

#ifdef CONFIG_OT_VO_VGA
    if (vo_drv_is_vga_intf(intf_type)) {
        /* 如果用户没有配置VGA参数,那么使用对应时序默认值并更新上下文中VGA参数 */
        if (dev_ctx->vga_param_config == TD_FALSE) {
            dev_ctx->vga_param.gain = vo_drv_get_intf_vga_dac_gc(dev_ctx->vou_attr.intf_sync);
        }
        vo_drv_set_vga_param(dev, &dev_ctx->vga_param);
    }
#endif

#ifdef CONFIG_OT_VO_RGB
    if ((vo_drv_is_dev_support_rgb(dev) == TD_TRUE) &&
        (vo_drv_is_rgb_intf(intf_type))) {
        vo_drv_set_rgb_param(dev, &dev_ctx->rgb_param);
    }
#endif

#ifdef CONFIG_OT_VO_BT1120
    if (vo_drv_is_bt_intf(intf_type)) {
        /* clk edge maybe configured when vo enable, so refresh the value */
        dev_ctx->bt_param.clk_edge = vo_drv_get_bt_param_clk_edge(dev);
        vo_drv_set_bt_param(dev, &dev_ctx->bt_param);
    }
#endif

#ifdef CONFIG_OT_VO_MIPI
    if (vo_drv_is_mipi_intf(intf_type)) {
        vo_drv_set_mipi_param(dev, &dev_ctx->mipi_param);
    }
#endif
}

#endif /* #if vo_desc("intf param") */

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("vga param")

#ifdef CONFIG_OT_VO_VGA

static td_s32 vo_check_support_vga_param(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev:%d is not phy device, not support vga param!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (vo_drv_is_dev_support_vga(dev) != TD_TRUE) {
        vo_err_trace("Vo device %d doesn't support VGA!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    if (vo_drv_is_vga_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
        vo_err_trace("Vo %d doesn't set VGA output!\n", dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_vga_param(const ot_vo_vga_param *vga_param)
{
    td_s32 ret;

    ret = vo_drv_check_vga_csc_matrix(vga_param->csc.csc_matrix);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vo_check_csc_no_matrix(&vga_param->csc);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (vga_param->gain > VO_VGA_GAIN_MAX) {
        vo_err_trace("Vo VGA gain value %u out of range [0,%u)!\n", vga_param->gain, VO_VGA_GAIN_MAX + 1);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if ((vga_param->sharpen_strength < VO_VGA_SHARPEN_STRENGTH_MIN)
        || (vga_param->sharpen_strength > VO_VGA_SHARPEN_STRENGTH_MAX)) {
        vo_err_trace("Vga sharpen strength %d can not be set over [%d,%d]!\n", vga_param->sharpen_strength,
            VO_VGA_SHARPEN_STRENGTH_MIN, VO_VGA_SHARPEN_STRENGTH_MAX);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param)
{
    td_s32 ret;

    ret = vo_check_support_vga_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_vga_param(vga_param);
}

static td_s32 vo_do_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_vga_param(dev, vga_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* 用户设置VGA参数后,VO内部不再使用对应时序默认值,以用户配置为准 */
    dev_ctx->vga_param_config = TD_TRUE;
    (td_void)memcpy_s(&dev_ctx->vga_param, sizeof(ot_vo_vga_param), vga_param, sizeof(ot_vo_vga_param));
    vo_drv_set_vga_param(dev, vga_param);
    return TD_SUCCESS;
}

static td_s32 vo_do_get_vga_param(ot_vo_dev dev, ot_vo_vga_param *vga_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_support_vga_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (dev_ctx->vga_param_config == TD_FALSE) {
        /* 如果用户没有配置VGA参数,那么使用对应时序默认值并更新上下文中VGA参数 */
        dev_ctx->vga_param.gain = vo_drv_get_intf_vga_dac_gc(dev_ctx->vou_attr.intf_sync);
    }

    (td_void)memcpy_s(vga_param, sizeof(ot_vo_vga_param), &dev_ctx->vga_param, sizeof(ot_vo_vga_param));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_vga_param(vo_ioctl_vga_param_param *ioctl_vga_param, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_vga_param->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_vga_param(ioctl_vga_param->ioctl_dev.dev, &(ioctl_vga_param->vga_param));
    } else {
        ret = vo_do_get_vga_param(ioctl_vga_param->ioctl_dev.dev, &(ioctl_vga_param->vga_param));
    }
    vo_up_sem();

    return ret;
}

#endif

#endif

#ifdef CONFIG_OT_HDMI_SUPPORT
#if vo_desc("hdmi param")

static td_s32 vo_check_support_hdmi_param(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev:%d is not phy device, not support hdmi param!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (vo_drv_is_dev_support_hdmi(dev) != TD_TRUE) {
        vo_err_trace("vo device %d doesn't support HDMI!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    if (vo_drv_is_hdmi_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
        vo_err_trace("vo %d doesn't set HDMI output!\n", dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_hdmi_param(const ot_vo_hdmi_param *hdmi_param)
{
    td_s32 ret;

    ret = vo_drv_check_hdmi_csc_matrix(hdmi_param->csc.csc_matrix);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_csc_no_matrix(&hdmi_param->csc);
}

static td_s32 vo_check_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param)
{
    td_s32 ret;

    ret = vo_check_support_hdmi_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_hdmi_param(hdmi_param);
}

static td_s32 vo_do_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param)
{
    td_s32 ret;
    ot_vo_csc_matrix org_csc_matrix;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_hdmi_param(dev, hdmi_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    org_csc_matrix = dev_ctx->hdmi_param.csc.csc_matrix;
    (td_void)memcpy_s(&dev_ctx->hdmi_param, sizeof(ot_vo_hdmi_param), hdmi_param, sizeof(ot_vo_hdmi_param));
    vo_drv_set_hdmi_param(dev, hdmi_param);

    /* 客户场景：客户不刷新CSC矩阵刷新亮度等参数时，不下发HDMI配置，以免出现配置HDMI的PHY配置导致HDMI屏幕黑屏等问题 */
    if (org_csc_matrix == hdmi_param->csc.csc_matrix) {
        return TD_SUCCESS;
    }

    /* 配置HDMI参数时, HDMI相关配置出错时返回给上层体现出来 */
    ret = vo_set_hdmi_color_space(hdmi_param->csc.csc_matrix, OT_VO_INTF_HDMI);
    if (ret != TD_SUCCESS) {
        vo_err_trace("vo device %d set hdmi color space fail when set hdmi param!\n", dev);
    }
    return ret;
}

static td_s32 vo_do_get_hdmi_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_support_hdmi_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(hdmi_param, sizeof(ot_vo_hdmi_param), &dev_ctx->hdmi_param, sizeof(ot_vo_hdmi_param));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_hdmi_param(vo_ioctl_hdmi_param_param *ioctl_hdmi_param, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_hdmi_param->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_hdmi_param(ioctl_hdmi_param->ioctl_dev.dev, &(ioctl_hdmi_param->hdmi_param));
    } else {
        ret = vo_do_get_hdmi_param(ioctl_hdmi_param->ioctl_dev.dev, &(ioctl_hdmi_param->hdmi_param));
    }
    vo_up_sem();

    return ret;
}
#endif

#if vo_desc("hdmi1 param")

#ifdef CONFIG_OT_VO_HDMI1

static td_s32 vo_check_support_hdmi1_param(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev:%d is not phy device, not support hdmi1 param!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (vo_drv_is_dev_support_hdmi1(dev) != TD_TRUE) {
        vo_err_trace("vo device %d doesn't support HDMI1!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    if (vo_drv_is_hdmi1_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
        vo_err_trace("vo %d doesn't set HDMI1 output!\n", dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_set_hdmi1_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi1_param)
{
    td_s32 ret;

    ret = vo_check_support_hdmi1_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_hdmi_param(hdmi1_param);
}

static td_s32 vo_do_set_hdmi1_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi1_param)
{
    td_s32 ret;
    ot_vo_csc_matrix org_csc_matrix;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_hdmi1_param(dev, hdmi1_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    org_csc_matrix = dev_ctx->hdmi1_param.csc.csc_matrix;
    (td_void)memcpy_s(&dev_ctx->hdmi1_param, sizeof(ot_vo_hdmi_param), hdmi1_param, sizeof(ot_vo_hdmi_param));
    vo_drv_set_hdmi1_param(dev, hdmi1_param);

    /* 客户场景：客户不刷新CSC矩阵刷新亮度等参数时，不下发HDMI配置，以免出现配置HDMI的PHY配置导致HDMI屏幕黑屏等问题 */
    if (org_csc_matrix == hdmi1_param->csc.csc_matrix) {
        return TD_SUCCESS;
    }

    /* 配置HDMI参数时, HDMI相关配置出错时返回给上层体现出来 */
    ret = vo_set_hdmi_color_space(hdmi1_param->csc.csc_matrix, OT_VO_INTF_HDMI1);
    if (ret != TD_SUCCESS) {
        vo_err_trace("vo device %d set hdmi1 color space fail when set hdmi1 param!\n", dev);
    }
    return ret;
}

static td_s32 vo_do_get_hdmi1_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi1_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_support_hdmi1_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(hdmi1_param, sizeof(ot_vo_hdmi_param), &dev_ctx->hdmi1_param, sizeof(ot_vo_hdmi_param));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_hdmi1_param(vo_ioctl_hdmi_param_param *ioctl_hdmi_param, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_hdmi_param->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_hdmi1_param(ioctl_hdmi_param->ioctl_dev.dev, &(ioctl_hdmi_param->hdmi_param));
    } else {
        ret = vo_do_get_hdmi1_param(ioctl_hdmi_param->ioctl_dev.dev, &(ioctl_hdmi_param->hdmi_param));
    }
    vo_up_sem();

    return ret;
}

#endif

#endif
#endif

#if vo_desc("rgb param")

#ifdef CONFIG_OT_VO_RGB

static td_s32 vo_check_support_rgb_param(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev:%d is not phy device, not support rgb param!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (vo_drv_is_dev_support_rgb(dev) != TD_TRUE) {
        vo_err_trace("vo device %d doesn't support rgb!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    if (vo_drv_is_rgb_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
        vo_err_trace("vo %d doesn't set rgb output!\n", dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_drv_check_rgb_csc_matrix(rgb_param->csc.csc_matrix);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (vo_check_bool_value(rgb_param->rgb_inverted_en) != TD_SUCCESS) {
        vo_err_trace("vo(%d) rgb param rgb_inverted_en %u out of range!\n", dev, rgb_param->rgb_inverted_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (vo_check_bool_value(rgb_param->bit_inverted_en) != TD_SUCCESS) {
        vo_err_trace("vo(%d) rgb param bit_inverted_en %u out of range!\n", dev, rgb_param->bit_inverted_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if ((vo_drv_is_rgb_serial_intf(dev_ctx->vou_attr.intf_type) == TD_TRUE) &&
        (rgb_param->rgb_inverted_en == TD_TRUE)) {
        vo_err_trace("for serial rgb6/8bit, vo %d doesn't support enable rgb inverted!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    return vo_check_csc_no_matrix(&rgb_param->csc);
}

static td_s32 vo_check_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param)
{
    td_s32 ret;

    ret = vo_check_support_rgb_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_rgb_param(dev, rgb_param);
}

static td_s32 vo_do_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_rgb_param(dev, rgb_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(&dev_ctx->rgb_param, sizeof(ot_vo_rgb_param), rgb_param, sizeof(ot_vo_rgb_param));
    vo_drv_set_rgb_param(dev, rgb_param);

    return ret;
}

static td_s32 vo_do_get_rgb_param(ot_vo_dev dev, ot_vo_rgb_param *rgb_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_support_rgb_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(rgb_param, sizeof(ot_vo_rgb_param), &dev_ctx->rgb_param, sizeof(ot_vo_rgb_param));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_rgb_param(vo_ioctl_rgb_param_param *ioctl_rgb_param, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_rgb_param->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_rgb_param(ioctl_rgb_param->ioctl_dev.dev, &(ioctl_rgb_param->rgb_param));
    } else {
        ret = vo_do_get_rgb_param(ioctl_rgb_param->ioctl_dev.dev, &(ioctl_rgb_param->rgb_param));
    }
    vo_up_sem();

    return ret;
}

#endif

#endif

#if vo_desc("bt param")

#ifdef CONFIG_OT_VO_BT1120

static td_s32 vo_check_support_bt_param(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev:%d is not phy device, not support bt.1120/bt.656 param!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    /* 设备不支持BT接口(BT1120和BT656其中之一)时返回不支持，仅支持其中1个时也可以返回成功 */
    if ((vo_drv_is_dev_support_bt(dev) != TD_TRUE)) {
        vo_err_trace("vo device %d doesn't support bt.1120 or bt.656!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    /* 设备属性可以保证接口类型的有效，这里不细分判断 */
    if (vo_drv_is_bt_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
        vo_err_trace("vo %d doesn't set bt.1120 or bt.656 output!\n", dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_bt_param_clk_edge(ot_vo_dev dev, const ot_vo_bt_param *bt_param)
{
#ifdef CONFIG_OT_VO_BT1120_DUAL_EDGE
    /* bt.656只能单沿输出 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if ((vo_drv_is_bt656_intf(dev_ctx->vou_attr.intf_type) == TD_TRUE) &&
        (bt_param->clk_edge != OT_VO_CLK_EDGE_SINGLE)) {
        vo_err_trace("for bt.656, vo %d doesn't support dual edge output!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    /* 约束设备使能后不允许动态修改时钟单双沿输出 */
    dev_ctx->bt_param.clk_edge = vo_drv_get_bt_param_clk_edge(dev);
    if ((dev_ctx->vo_enable == TD_TRUE) && (bt_param->clk_edge != dev_ctx->bt_param.clk_edge)) {
        vo_err_trace("vo(%d) doesn't support change clk_edge(%d) to (%u) after dev enabled!\n",
            dev, dev_ctx->bt_param.clk_edge, bt_param->clk_edge);
        return OT_ERR_VO_NOT_DISABLE;
    }
#else
    if (bt_param->clk_edge != OT_VO_CLK_EDGE_SINGLE) {
        vo_err_trace("current chip doesn't support dual edge output!\n");
        return OT_ERR_VO_NOT_SUPPORT;
    }
#endif

    return TD_SUCCESS;
}

static td_s32 vo_check_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_check_bool_value(bt_param->yc_inverted_en) != TD_SUCCESS) {
        vo_err_trace("vo(%d) bt param yc_inverted_en %u out of range!\n", dev, bt_param->yc_inverted_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (vo_check_bool_value(bt_param->bit_inverted_en) != TD_SUCCESS) {
        vo_err_trace("vo(%d) bt param bit_inverted_en %u out of range!\n", dev, bt_param->bit_inverted_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (bt_param->clk_edge >= OT_VO_CLK_EDGE_BUTT) {
        vo_err_trace("vo(%d) bt param clk_edge %u out of range!\n", dev, bt_param->clk_edge);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if ((vo_drv_is_bt656_intf(dev_ctx->vou_attr.intf_type) == TD_TRUE) &&
        (bt_param->yc_inverted_en == TD_TRUE)) {
        vo_err_trace("for bt.656, vo %d doesn't support enable yc inverted!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    return vo_check_bt_param_clk_edge(dev, bt_param);
}

static td_s32 vo_check_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param)
{
    td_s32 ret;

    ret = vo_check_support_bt_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_bt_param(dev, bt_param);
}

static td_s32 vo_do_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_bt_param(dev, bt_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(&dev_ctx->bt_param, sizeof(ot_vo_bt_param), bt_param, sizeof(ot_vo_bt_param));
    vo_drv_set_bt_param_clk_edge(dev, bt_param->clk_edge);
    vo_drv_set_bt_param(dev, bt_param);
    return TD_SUCCESS;
}

static td_s32 vo_do_get_bt_param(ot_vo_dev dev, ot_vo_bt_param *bt_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_support_bt_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    dev_ctx->bt_param.clk_edge = vo_drv_get_bt_param_clk_edge(dev);
    (td_void)memcpy_s(bt_param, sizeof(ot_vo_bt_param), &dev_ctx->bt_param, sizeof(ot_vo_bt_param));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_bt_param(vo_ioctl_bt_param_param *ioctl_bt_param, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_bt_param->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_bt_param(ioctl_bt_param->ioctl_dev.dev, &(ioctl_bt_param->bt_param));
    } else {
        ret = vo_do_get_bt_param(ioctl_bt_param->ioctl_dev.dev, &(ioctl_bt_param->bt_param));
    }
    vo_up_sem();

    return ret;
}

#endif

#endif

#if vo_desc("mipi param")

#ifdef CONFIG_OT_VO_MIPI
static td_s32 vo_check_support_mipi_param(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev:%d is not phy device, not support mipi param!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (vo_drv_is_dev_support_mipi(dev) != TD_TRUE) {
        vo_err_trace("vo device %d doesn't support mipi!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    /* 设备属性可以保证接口类型的有效，这里不细分判断 */
    if (vo_drv_is_mipi_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE) {
        vo_err_trace("vo %d doesn't set mipi output!\n", dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_mipi_param(const ot_vo_mipi_param *mipi_param)
{
    td_s32 ret;

    ret = vo_drv_check_mipi_csc_matrix(mipi_param->csc.csc_matrix);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_csc_no_matrix(&mipi_param->csc);
}

static td_s32 vo_check_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param)
{
    td_s32 ret;

    ret = vo_check_support_mipi_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_mipi_param(mipi_param);
}

static td_s32 vo_do_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_mipi_param(dev, mipi_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(&dev_ctx->mipi_param, sizeof(ot_vo_mipi_param), mipi_param, sizeof(ot_vo_mipi_param));
    vo_drv_set_mipi_param(dev, mipi_param);
    return TD_SUCCESS;
}

static td_s32 vo_do_get_mipi_param(ot_vo_dev dev, ot_vo_mipi_param *mipi_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_support_mipi_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(mipi_param, sizeof(ot_vo_mipi_param), &dev_ctx->mipi_param, sizeof(ot_vo_mipi_param));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_mipi_param(vo_ioctl_mipi_param_param *ioctl_mipi_param, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_mipi_param->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_mipi_param(ioctl_mipi_param->ioctl_dev.dev, &(ioctl_mipi_param->mipi_param));
    } else {
        ret = vo_do_get_mipi_param(ioctl_mipi_param->ioctl_dev.dev, &(ioctl_mipi_param->mipi_param));
    }
    vo_up_sem();

    return ret;
}

#endif

#endif /* #if vo_desc("mipi param") */

#endif /* #if vo_desc("KERNEL_VO") */
