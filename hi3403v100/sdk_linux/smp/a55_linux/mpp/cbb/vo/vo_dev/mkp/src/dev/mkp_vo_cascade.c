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
#include "mkp_vo_cascade.h"

#ifdef CONFIG_OT_VO_CASCADE

#if vo_desc("cascade pub")

static td_bool vo_is_cas_attr_config(td_void)
{
    ot_vo_dev phy_dev = vo_drv_get_cas_phy_dev();
    vo_dev_info *dev_ctx = vo_get_dev_ctx(phy_dev);

    if (dev_ctx == TD_NULL) {
        return TD_FALSE;
    }

    return dev_ctx->dev_cas.cas_config;
}

static td_bool vo_get_cas_attr_slave_flag(td_void)
{
    ot_vo_dev phy_dev = vo_drv_get_cas_phy_dev();
    vo_dev_info *dev_ctx = vo_get_dev_ctx(phy_dev);

    if (dev_ctx == TD_NULL) {
        vo_err_trace("vo dev id %d ctx is null!\n", phy_dev);
        return TD_FALSE;
    }

    return dev_ctx->dev_cas.cas_attr.is_slave;
}

static ot_vo_cas_mode vo_get_cas_attr_mode(td_void)
{
    ot_vo_dev phy_dev = vo_drv_get_cas_phy_dev();
    vo_dev_info *dev_ctx = vo_get_dev_ctx(phy_dev);

    if (dev_ctx == TD_NULL) {
        vo_err_trace("vo dev id %d ctx is null!\n", phy_dev);
        return OT_VO_CAS_MODE_SINGLE;
    }

    return dev_ctx->dev_cas.cas_attr.cas_mode;
}

static ot_vo_cas_rgn vo_get_cas_attr_rgn(td_void)
{
    ot_vo_dev phy_dev = vo_drv_get_cas_phy_dev();
    vo_dev_info *dev_ctx = vo_get_dev_ctx(phy_dev);

    if (dev_ctx == TD_NULL) {
        vo_err_trace("vo dev id %d ctx is null!\n", phy_dev);
        return OT_VO_CAS_RGN_64;
    }

    return dev_ctx->dev_cas.cas_attr.cas_rgn;
}

static td_s32 vo_check_cas_phy_dev(td_void)
{
    ot_vo_layer phy_layer = vo_drv_get_cas_phy_layer();
    if (vo_is_layer_enabled(phy_layer) != TD_TRUE) {
        vo_err_trace("vo phy cas layer(%d) is disable, must be enabled!\n", phy_layer);
        return OT_ERR_VO_NOT_PERM;
    }

    if (vo_is_cas_attr_config() != TD_TRUE) {
        vo_err_trace("vo phy cas dev(%d) cascade attr is not configured, must be configured!\n",
            vo_drv_get_cas_phy_dev());
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

/* 调用者需要保证输入参数的有效性 */
static td_s32 vo_check_ioctl_cas_dev(const vo_ioctl_dev_param *ioctl_dev)
{
    vo_check_dev_id_return(ioctl_dev->dev);
    vo_check_no_cas_dev_return(ioctl_dev->dev);
    vo_init_dev_ctx_return(ioctl_dev->dev);

    return TD_SUCCESS;
}

td_bool vo_is_cas_layer(ot_vo_layer layer)
{
    ot_vo_dev dev = vo_get_video_layer_bind_dev(layer);
    if (dev == OT_INVALID_DEV) {
        return TD_FALSE;
    }

    return vo_is_cas_dev(dev);
}

#endif

#if vo_desc("cascade multi rgn")

static vo_cas_mrg_rect_cfg g_vo_cas_mrg_rect_cfg = {0};

static vo_cas_mrg_rect_cfg *vo_get_cas_mrg_rect_cfg(td_void)
{
    return &g_vo_cas_mrg_rect_cfg;
}

static td_void vo_reset_cas_mrg_rect_cfg(td_void)
{
    vo_cas_mrg_rect_cfg *cas_cfg = vo_get_cas_mrg_rect_cfg();

    (td_void)memset_s(cas_cfg, sizeof(vo_cas_mrg_rect_cfg), 0x0, sizeof(vo_cas_mrg_rect_cfg));
}

static vo_cas_mrg_rect_ctx *vo_get_cas_mrg_rect_ctx(td_s32 area_id)
{
    vo_cas_mrg_rect_cfg *cas_cfg = vo_get_cas_mrg_rect_cfg();

    return &cas_cfg->mrg_rect[area_id];
}

static td_void vo_set_cas_mrg_rect_ctx(td_s32 area_id, const ot_rect *last_rect, const ot_rect *rect)
{
    vo_cas_mrg_rect_ctx *cas_rect = vo_get_cas_mrg_rect_ctx(area_id);

    (td_void)memcpy_s(&cas_rect->last_rect, sizeof(ot_rect), last_rect, sizeof(ot_rect));
    (td_void)memcpy_s(&cas_rect->rect, sizeof(ot_rect), rect, sizeof(ot_rect));
}

static td_void vo_set_cas_mrg_rect(td_s32 area_id, const ot_rect *rect)
{
    vo_cas_mrg_rect_ctx *cas_rect = TD_NULL;
    vo_cas_mrg_rect_cfg *cas_cfg = vo_get_cas_mrg_rect_cfg();

    if ((area_id < 0) || (area_id >= MAX_REGION_NUM)) {
        vo_err_trace("invalid area_id %d! should be [0, %d]\n", area_id, MAX_REGION_NUM - 1);
        return;
    }

    cas_cfg->mrg_rect_valid = TD_TRUE;
    cas_rect = vo_get_cas_mrg_rect_ctx(area_id);
    if (cas_rect->mrg_status == VO_CAS_MRG_STATUS_INIT) {
        vo_set_cas_mrg_rect_ctx(area_id, rect, rect);
        cas_rect->mrg_status = VO_CAS_MRG_STATUS_FIRST;
        return;
    }

    if ((cas_rect->mrg_status == VO_CAS_MRG_STATUS_FIRST) || (cas_rect->mrg_status == VO_CAS_MRG_STATUS_UPDATE_SAME)) {
        if (osal_memcmp(&cas_rect->rect, rect, sizeof(ot_rect)) == 0) {
            cas_rect->mrg_status = VO_CAS_MRG_STATUS_UPDATE_SAME;
        } else {
            vo_set_cas_mrg_rect_ctx(area_id, &cas_rect->rect, rect);
            cas_rect->mrg_status = VO_CAS_MRG_STATUS_UPDATE_NO_SAME;
        }

        return;
    }

    /* 不相同的状态时在vo_cas_refresh_mrg_rect_same中处理恢复处理 */
}

static td_bool vo_cas_is_mrg_rect_change(td_void)
{
    td_s32 area_id;
    vo_cas_mrg_rect_ctx *cas_rect = TD_NULL;

    for (area_id = 0; area_id < MAX_REGION_NUM; area_id++) {
        cas_rect = vo_get_cas_mrg_rect_ctx(area_id);
        if (cas_rect->mrg_status == VO_CAS_MRG_STATUS_UPDATE_NO_SAME) {
            return TD_TRUE;
        }
    }

    return TD_FALSE;
}

static td_void vo_cas_refresh_mrg_rect_same(td_void)
{
    td_s32 area_id;
    vo_cas_mrg_rect_ctx *cas_rect = TD_NULL;

    for (area_id = 0; area_id < MAX_REGION_NUM; area_id++) {
        cas_rect = vo_get_cas_mrg_rect_ctx(area_id);
        if (cas_rect->mrg_status == VO_CAS_MRG_STATUS_UPDATE_NO_SAME) {
            (td_void)memcpy_s(&cas_rect->last_rect, sizeof(ot_rect), &cas_rect->rect, sizeof(ot_rect));
            cas_rect->mrg_status = VO_CAS_MRG_STATUS_UPDATE_SAME;
        }
    }
}

/* 当前是否级联从模式的级联设备 */
static td_bool vo_cas_is_slave_slave(ot_vo_layer layer)
{
    ot_vo_layer hw_layer;

    /* 未配置级联时不处理 */
    if (vo_is_cas_attr_config() != TD_TRUE) {
        return TD_FALSE;
    }

    /* 只有级联从模式才需要处理，级联主模式不需要处理 */
    if (vo_get_cas_attr_slave_flag() != TD_TRUE) {
        return TD_FALSE;
    }

    hw_layer = vo_drv_get_hw_layer(layer);
    if (hw_layer != vo_drv_get_cas_phy_layer()) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

static td_s32 vo_cas_get_display_rect(ot_rect *display_rect, ot_rect *src_rect, ot_rect *dst_rect)
{
    if ((display_rect->width <= (td_u32)src_rect->x) || (display_rect->height <= (td_u32)src_rect->y)) {
        vo_err_trace("channel rect(x:%d,y:%d) is illegal, should smaller than display rect(w:%u,h:%u)\n",
            src_rect->x, src_rect->y, display_rect->width, display_rect->height);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* 将裁剪属性整理赋值 */
    dst_rect->x = src_rect->x;
    dst_rect->y = src_rect->y;
    dst_rect->width = src_rect->width;
    dst_rect->height = src_rect->height;

    /* 考虑到VSD的画布大小可以大于视频层显示大小，所以在这个vpss处理通路上面，添加一个裁剪的处理 */
    dst_rect->x = (td_u32)(MIN2((td_u32)src_rect->x, display_rect->width));
    dst_rect->y = (td_u32)(MIN2((td_u32)src_rect->y, display_rect->height));
    dst_rect->width = MIN2(dst_rect->width, display_rect->width - (td_u32)dst_rect->x);
    dst_rect->height = MIN2(dst_rect->height, display_rect->height - (td_u32)dst_rect->y);

    return TD_SUCCESS;
}

td_void vo_set_cas_layer_multi_rgn_rect(ot_vo_layer layer, td_s32 area_id, const ot_rect *rect)
{
    if (vo_cas_is_slave_slave(layer) != TD_TRUE) {
        return;
    }

    vo_set_cas_mrg_rect(area_id, rect);
}

/* 规避三片级联从片从模式时多区域配置只有第一次生效问题，在没有图像时也需要配置区域的宽高等配置 */
td_void vo_cas_hd_disp_chn_display_rect(ot_vo_layer layer, ot_vo_chn chn)
{
    td_s32 ret;
    ot_rect src_rect;
    ot_rect rect;
    vo_layer_info *layer_ctx = vo_get_layer_ctx(layer);
    vo_chn_info *chn_ctx = vo_get_chn_ctx(layer, chn);

    /* 检查是否有必要做多区域配置，只有级联从模式时的级联视频层才操作 */
    if (vo_cas_is_slave_slave(layer) != TD_TRUE) {
        return;
    }

    if (chn_ctx->area_id == OT_INVALID_VALUE) {
        return;
    }

    src_rect.x = chn_ctx->chn_display_pos.x;
    src_rect.y = chn_ctx->chn_display_pos.y;
    src_rect.width = chn_ctx->chn_attr.rect.width;
    src_rect.height = chn_ctx->chn_attr.rect.height;
    ret = vo_cas_get_display_rect(&layer_ctx->vou_video_attr.display_rect, &src_rect, &rect);
    if (ret != TD_SUCCESS) {
        vo_err_trace("chn(%d,%d), channel rect parameter is illegal!\n", layer, chn);
        return;
    }

    vo_drv_cas_set_mrg_rect(layer, &rect, chn_ctx->area_id);
    vo_set_cas_mrg_rect(chn_ctx->area_id, &rect);
}

static td_void vo_cas_layer_reload_mrg_para_up(ot_vo_layer layer)
{
    ot_vo_dev dev = vo_drv_get_cas_phy_dev();
    vo_cas_mrg_rect_cfg *cas_cfg = vo_get_cas_mrg_rect_cfg();

    vo_info_trace("layer(%d), mrg rect change!\n", layer);

    /* 恢复成主模式 */
    if (cas_cfg->mrg_para_up == VO_CAS_MRG_PARA_UP_INIT) {
        vo_drv_set_cas_slave_mode(TD_FALSE); /* 默认下配置为主模式，否则正常的VO使用无中断 */
        vo_drv_set_dev_reg_up(dev);
        cas_cfg->mrg_para_up = VO_CAS_MRG_PARA_UP_SLAVE_MASTER;
        vo_info_trace("layer(%d), set slave mode to salve master!\n", layer);
        return;
    }

    /* 恢复成PLL输出 */
    if (cas_cfg->mrg_para_up == VO_CAS_MRG_PARA_UP_SLAVE_MASTER) {
        vo_drv_set_dev_clk_sel(dev, 0);     /* 0: HPLL */
        cas_cfg->mrg_para_up = VO_CAS_MRG_PARA_UP_PLL;
        vo_info_trace("layer(%d), set clock to HPLL!\n", layer);
        return;
    }

    /* 第一次刷新系数 */
    if (cas_cfg->mrg_para_up == VO_CAS_MRG_PARA_UP_PLL) {
        vo_drv_vhd_coef_para_up(VO_COEF_BUF_BT1120_CAS);
        cas_cfg->mrg_para_up = VO_CAS_MRG_PARA_UP_REFRESH1;
        vo_info_trace("layer(%d), refresh mrg cas para up 1!\n", layer);
        return;
    }

    /* 第二次刷新系数 */
    if (cas_cfg->mrg_para_up == VO_CAS_MRG_PARA_UP_REFRESH1) {
        vo_drv_vhd_coef_para_up(VO_COEF_BUF_BT1120_CAS);
        cas_cfg->mrg_para_up = VO_CAS_MRG_PARA_UP_REFRESH2;
        vo_info_trace("layer(%d), refresh2 mrg cas para up 2!\n", layer);
        return;
    }

    /* 第三次刷新系数 */
    if (cas_cfg->mrg_para_up == VO_CAS_MRG_PARA_UP_REFRESH2) {
        vo_drv_vhd_coef_para_up(VO_COEF_BUF_BT1120_CAS);
        cas_cfg->mrg_para_up = VO_CAS_MRG_PARA_UP_REFRESH3;
        vo_info_trace("layer(%d), refresh mrg cas para up 3!\n", layer);
        return;
    }

    if (cas_cfg->mrg_para_up == VO_CAS_MRG_PARA_UP_REFRESH3) {
        /* 恢复成级联从模式输出 */
        vo_drv_set_dev_clk_sel(dev, 1);     /* 1: VI CMOS8时钟 */
        vo_drv_set_cas_slave_mode(TD_TRUE);
        vo_drv_set_dev_reg_up(dev);

        /* 控制块恢复成一样 */
        vo_cas_refresh_mrg_rect_same();
        cas_cfg->mrg_para_up = VO_CAS_MRG_PARA_UP_INIT;
        vo_info_trace("layer(%d), recover slave mode to salve and clock to vi!\n", layer);
    }
}

#endif

#if vo_desc("cascade api")

td_s32 vo_notify_cas_check_set_pub_attr(ot_vo_dev phy_dev, const ot_vo_pub_attr *pub_attr)
{
    ot_vo_intf_sync min_intf_sync;
    ot_vo_intf_sync max_intf_sync;
    vo_dev_info *phy_dev_ctx = vo_get_dev_ctx(phy_dev);

    /* 未配置BT1120接口时和配置用户时序时不检查 */
    if (((pub_attr->intf_type & OT_VO_INTF_BT1120) == TD_FALSE) ||
        (pub_attr->intf_sync == OT_VO_OUT_USER)) {
        return TD_SUCCESS;
    }

    /* 未配置级联时不检查 */
    if (phy_dev_ctx->dev_cas.cas_config != TD_TRUE) {
        return TD_SUCCESS;
    }

    /* 单双沿时序规格如下，4K等时序仅支持双沿148.5Mhz，不支持单沿297Mhz, 双沿支持如下时序，按需求驱动 */
    if (phy_dev_ctx->dev_cas.cas_attr.cas_edge_mode == OT_VO_CAS_MODE_SINGLE_EDGE) {
        min_intf_sync = OT_VO_OUT_640x480_60;
        max_intf_sync = OT_VO_OUT_2560x1600_60;
    } else {
        min_intf_sync = OT_VO_OUT_2560x1440_60;
        max_intf_sync = OT_VO_OUT_3840x2160_30;
    }

    if ((pub_attr->intf_sync < min_intf_sync) || (pub_attr->intf_sync > max_intf_sync)) {
        vo_err_trace("vo dev(%d) intf sync %s should be [%s, %s] when cas_edge_mode is %u!\n",
            phy_dev, vo_get_intf_sync_name(pub_attr->intf_sync), vo_get_intf_sync_name(min_intf_sync),
            vo_get_intf_sync_name(max_intf_sync), phy_dev_ctx->dev_cas.cas_attr.cas_edge_mode);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* 三片级联场景时低时序逻辑概率会有低带宽问题，规格返回不支持 */
    if (phy_dev_ctx->dev_cas.cas_attr.is_slave == TD_TRUE) {
        if ((pub_attr->intf_sync >= OT_VO_OUT_640x480_60) && (pub_attr->intf_sync <= OT_VO_OUT_576P50)) {
            vo_err_trace("vo dev(%d) intf sync %s not support when is_slave is TRUE!\n",
                phy_dev, vo_get_intf_sync_name(pub_attr->intf_sync));
            return OT_ERR_VO_ILLEGAL_PARAM;
        }
    }

    return TD_SUCCESS;
}

td_s32 vo_notify_cas_check_enable_dev(ot_vo_dev phy_dev)
{
    vo_dev_info *phy_dev_ctx = vo_get_dev_ctx(phy_dev);

    if (phy_dev_ctx->dev_cas.cas_config != TD_TRUE) {
        return TD_SUCCESS;
    }

    if (phy_dev_ctx->vou_attr.intf_type != OT_VO_INTF_BT1120) {
        vo_err_trace("vo dev(%d) intf type should be BT.1120 when config cascade!\n", phy_dev);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* 隔行时序不支持，其他时序目前放开支持 */
    if (vo_is_dev_interlaced_typical_intf_sync(phy_dev)) {
        vo_err_trace("vo dev(%d) intf sync should not be interlaced when config cascade!\n", phy_dev);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_void vo_notify_cas_enable_dev_clk(ot_vo_dev phy_dev)
{
    td_u32 hd_clk_sel = 0; /* hd0默认HPLL, 还有一个选择是vi cmos port8 */
    td_u32 vo_out_clk_div = VO_CLK_DIV1;
    vo_dev_info *phy_dev_ctx = vo_get_dev_ctx(phy_dev);

    /* 双沿输出时需要配置成2分频 */
    if (phy_dev_ctx->dev_cas.cas_attr.cas_edge_mode == OT_VO_CAS_MODE_DUAL_EDGE) {
        vo_out_clk_div = VO_CLK_DIV2;
    }

    /* 级联从模式时需要配置成VI时钟 */
    if (phy_dev_ctx->dev_cas.cas_attr.is_slave == TD_TRUE) {
        hd_clk_sel = 1; /* 1: VI CMOS8时钟 */
        vo_drv_cas_set_dev_pll_clk(phy_dev);
    }

    vo_drv_set_dev_clk_sel(phy_dev, hd_clk_sel);
    vo_drv_set_out_clk_div(phy_dev, vo_out_clk_div);
}

td_void vo_notify_cas_enable_dev(ot_vo_dev phy_dev)
{
    vo_dev_info *phy_dev_ctx = vo_get_dev_ctx(phy_dev);

    phy_dev_ctx->dev_cas.cas_regup = TD_FALSE;
    if (phy_dev_ctx->dev_cas.cas_config != TD_TRUE) {
        vo_drv_set_cas_slave_mode(TD_FALSE); /* 默认下配置为主模式，否则正常的VO使用无中断 */
        return;
    }

    vo_notify_cas_enable_dev_clk(phy_dev);
    vo_drv_set_cas_slave_mode(phy_dev_ctx->dev_cas.cas_attr.is_slave);
}

td_void vo_notify_cas_disable_dev(ot_vo_dev phy_dev)
{
    vo_dev_info *phy_dev_ctx = vo_get_dev_ctx(phy_dev);

    if (phy_dev_ctx->dev_cas.cas_config != TD_TRUE) {
        return;
    }

    vo_drv_clear_cas_all_pos_enable();
    vo_drv_set_cas_enable(TD_FALSE);
    vo_drv_set_out_clk_div(phy_dev, VO_CLK_DIV1);
    vo_drv_set_cas_slave_mode(TD_FALSE);
    vo_drv_set_dev_clk_sel(phy_dev, 0);

    /* 恢复控制块 */
    vo_reset_cas_mrg_rect_cfg();
}

td_void vo_notify_cas_layer_chn_enable(ot_vo_layer cas_layer, td_s32 area_id, td_bool enable)
{
    ot_vo_dev cas_dev = vou_drv_get_video_layer_bind_dev(cas_layer);
    if (vo_is_cas_dev(cas_dev)) {
        vo_drv_set_cas_pos_enable(area_id, enable);
    }
}

td_void vo_notify_cas_chn_rgns_sort(ot_vo_layer layer, ot_vo_chn chn, td_bool switch_size)
{
    vo_chn_info *chn_ctx = vo_get_chn_ctx(layer, chn);

    if (chn_ctx->chn_cas_attr.is_bind_cas_pos == TD_TRUE) {
        vo_chn_cas_rgns_sort(layer, switch_size);
    }
}

#endif

#if vo_desc("cascade int process")

td_bool g_is_vo_cas_odd_frame = TD_FALSE;

static td_bool vo_get_cas_odd_frame_flag(td_void)
{
    return g_is_vo_cas_odd_frame;
}

static td_void vo_set_cas_odd_frame_flag(td_bool cas_odd_frame_flag)
{
    g_is_vo_cas_odd_frame = cas_odd_frame_flag;
}

static td_void vo_get_cas_disp_dev_dual_master(ot_vo_dev *dev)
{
    ot_vo_dev cas_odd_dev;
    ot_vo_dev cas_even_dev;
    td_bool is_vo_cas_odd_frame;

    cas_odd_dev = vo_drv_get_odd_cas_dev();
    cas_even_dev = vo_drv_get_even_cas_dev();
    is_vo_cas_odd_frame = vo_get_cas_odd_frame_flag();
    *dev = (is_vo_cas_odd_frame == TD_TRUE) ? cas_odd_dev : cas_even_dev;

    vo_set_cas_odd_frame_flag(!is_vo_cas_odd_frame);
}

static td_s32 vo_get_cas_disp_dev_dual_slave(ot_vo_dev *dev)
{
    td_s32 ret;
    ot_vo_dev cas_odd_dev;
    ot_vo_dev cas_even_dev;
    ot_vo_cas_mode cas_mode;
    td_bool is_even;
    vo_dev_info *phy_dev_ctx = vo_get_dev_ctx(*dev);

    if (phy_dev_ctx == TD_NULL) {
        vo_err_trace("phy_dev %d ctx is null\n", *dev);
        return OT_ERR_VO_NULL_PTR;
    }

    if (phy_dev_ctx->dev_cas.cas_regup != TD_TRUE) {
        vo_drv_set_dev_reg_up(vo_drv_get_cas_phy_dev());
        phy_dev_ctx->dev_cas.cas_regup = TD_TRUE;
    }

    ret = vo_drv_get_cas_mode(&cas_mode, &is_even);
    if (ret != TD_SUCCESS) {
        vo_err_trace("invalid cascade img_id from master chip!\n");
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (cas_mode == OT_VO_CAS_MODE_SINGLE) {
        vo_err_trace("slave chip is dual mode when master chip is single mode!\n");
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    cas_odd_dev = vo_drv_get_odd_cas_dev();
    cas_even_dev = vo_drv_get_even_cas_dev();
    *dev = (is_even == TD_FALSE) ? cas_odd_dev : cas_even_dev;
    return TD_SUCCESS;
}

td_s32 vo_get_cas_disp_dev(ot_vo_dev *dev)
{
    td_s32 ret;
    ot_vo_cas_mode cas_mode;

    if (vo_is_dev_config_cas(*dev) != TD_TRUE) {
        return TD_SUCCESS;
    }

    cas_mode = vo_get_cas_attr_mode();
    if (cas_mode == OT_VO_CAS_MODE_SINGLE) {                /* cas_mode:single */
        *dev = vo_drv_get_odd_cas_dev();
    } else if (vo_get_cas_attr_slave_flag() != TD_TRUE) { /* cas_mode:dual + is_slave:false */
        vo_get_cas_disp_dev_dual_master(dev);

        /* 奇偶帧情况要清屏 */
        vou_drv_disable_all_area(vo_drv_get_cas_phy_layer());
    } else {
        ret = vo_get_cas_disp_dev_dual_slave(dev);      /* cas_mode:dual + is_slave:true */
        if (ret != TD_SUCCESS) {
            return ret;
        }

        /* 奇偶帧情况要清屏 */
        vou_drv_disable_all_area(vo_drv_get_cas_phy_layer());
    }

    vo_drv_clear_cas_all_pos_enable();
    return TD_SUCCESS;
}

td_s32 vo_set_cas_cfg(ot_vo_dev cas_dev)
{
    td_bool is_vblank = TD_TRUE;
    td_bool is_even;
    ot_vo_layer phy_dev = vo_drv_get_cas_phy_dev();
    vo_dev_info *phy_dev_ctx = vo_get_dev_ctx(phy_dev);

    vo_drv_get_dev_int_state_vblank(phy_dev, &is_vblank);
    if (is_vblank == TD_TRUE) {
        vo_debug_trace("vo dev %d:DHD Timing is in V-blank! missing img_id\n", phy_dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (phy_dev_ctx == TD_NULL) {
        vo_err_trace("phy_dev %d ctx is null\n", phy_dev);
        return OT_ERR_VO_NULL_PTR;
    }

    vo_drv_set_cas_pattern(phy_dev_ctx->dev_cas.cas_pattern);
    if (phy_dev_ctx->dev_cas.cas_attr.is_slave == TD_FALSE) {
        is_even = vo_get_cas_odd_frame_flag();
    } else {
        is_even = (cas_dev == vo_drv_get_even_cas_dev()) ?  TD_TRUE : TD_FALSE;
    }

    vo_drv_set_cas_mode(phy_dev_ctx->dev_cas.cas_attr.is_slave, phy_dev_ctx->dev_cas.cas_attr.cas_mode, is_even);
    vo_drv_set_dev_reg_up(phy_dev);

    return TD_SUCCESS;
}

#endif

#if vo_desc("cascade int process")

static td_void vo_get_cas_sort_pos_num(ot_vo_layer layer, td_u32 chn_pos_num, td_u32 *chn_pos,
    td_u32 *sort_num)
{
    td_u32 i;
    td_u32 j;
    vo_chn_info *tmp_chn_ctx = TD_NULL;
    vo_layer_info *layer_ctx = vo_get_layer_ctx(layer);

    /* 导出使能且正显示道且绑定级联区域的通道 */
    for (i = 0, j = 0; (i < OT_VO_MAX_CHN_NUM) && (i < chn_pos_num); i++) {
        tmp_chn_ctx = vo_get_chn_ctx(layer, i);
        if (tmp_chn_ctx == TD_NULL) {
            continue;
        }

        if (tmp_chn_ctx->batch_attr.batch_set == TD_FALSE) {
            if ((tmp_chn_ctx->vo_ch_enable == TD_TRUE) &&
                (tmp_chn_ctx->chn_show == TD_TRUE) &&
                (tmp_chn_ctx->chn_cas_attr.is_bind_cas_pos == TD_TRUE)) {
                layer_ctx->pri_table[j] = i;
                chn_pos[j] = tmp_chn_ctx->chn_cas_attr.pos;
                j++;
            } else {
                tmp_chn_ctx->area_id_tmp = OT_INVALID_VALUE;
            }
        } else {
            if ((tmp_chn_ctx->batch_attr.enable_tmp == TD_TRUE) &&
                (tmp_chn_ctx->batch_attr.show_tmp == TD_TRUE) &&
                (tmp_chn_ctx->batch_attr.chn_cas_attr_tmp.is_bind_cas_pos == TD_TRUE)) {
                layer_ctx->pri_table[j] = i;
                chn_pos[j] = tmp_chn_ctx->batch_attr.chn_cas_attr_tmp.pos;
                j++;
            } else {
                tmp_chn_ctx->area_id_tmp = OT_INVALID_VALUE;
            }
        }
    }

    *sort_num = j;
}

td_void vo_chn_cas_rgns_sort(ot_vo_layer layer, td_bool switch_size)
{
    td_u32 i;
    td_u32 j;
    td_u32 sort_ch_num = 0;
    td_u32 tmp_pos;
    td_u32 chn_pos[OT_VO_MAX_CHN_NUM] = {0};
    vo_chn_info *tmp_chn_ctx = TD_NULL;
    vo_layer_info *layer_ctx = vo_get_layer_ctx(layer);

    /* 导出使能且正显示的通道 */
    vo_get_cas_sort_pos_num(layer, OT_VO_MAX_CHN_NUM, chn_pos, &sort_ch_num);
    /*
     * 检查层最多可以使能显示的通道数
     * 这个检查事实上只能针对走高清处理的通道，对于标清处理的，不会进入这个函数，
     * 同时，标清由于是通过软件拼接，也并不需要对这个数目进行检测，以最大通道数目进行限制
     */
    if (sort_ch_num > vou_drv_get_layer_chn_num(layer)) {
        vo_err_trace("the layer(%d) should only enable %u chns!\n", layer, vou_drv_get_layer_chn_num(layer));
        return;
    }

    /* 通道数大于等2个才需要排序 */
    if ((sort_ch_num > 1) && (sort_ch_num <= VO_PRIORITY_MAX)) {
        vo_sort_chn(layer, sort_ch_num);
    }

    /* 对pos从小到大排序 */
    for (i = 0; i < sort_ch_num; i++) {
        for (j = i + 1; j < sort_ch_num; j++) {
            if (chn_pos[i] > chn_pos[j]) {
                tmp_pos = chn_pos[i];
                chn_pos[i] = chn_pos[j];
                chn_pos[j] = tmp_pos;
            }
        }
    }

    /* 将排序后的pos和chn一一对应起来，并将排序结果记录到通道上下文 */
    for (i = 0; i < sort_ch_num; i++) {
        tmp_chn_ctx = vo_get_chn_ctx(layer, layer_ctx->pri_table[i]);
        tmp_chn_ctx->area_id_tmp = chn_pos[i];
        vo_debug_trace("pri_table[%u] %u\n", i, layer_ctx->pri_table[i]);
    }

    /*
     * 这个判定不能放到上面for循环中，如果切层时导致某一个层没有使能通道(没有需要排序的通道)
     * 则for循环不会进入，导致hd_chn_rgn_upd标志未置
     */
    if (switch_size == TD_TRUE) {
        /* 切大小时，整体设备可切换大小时生效 */
        layer_ctx->hd_chn_rgn_upd = TD_FALSE;
    } else {
        /* 切隐藏显示或使能新通道时，切换效果下一个中断即时生效 */
        layer_ctx->hd_chn_rgn_upd = TD_TRUE;
    }
}

#endif

#if vo_desc("cascade attr")

static td_s32 vo_check_cas_attr(const ot_vo_cas_attr *cas_attr)
{
    if (vo_check_bool_value(cas_attr->is_slave) != TD_SUCCESS) {
        vo_err_trace("vo cascade attr is_slave %u illegal!\n", cas_attr->is_slave);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (cas_attr->cas_rgn >= OT_VO_CAS_RGN_BUTT) {
        vo_err_trace("vo cascade attr cas_rgn %u illegal!\n", cas_attr->cas_rgn);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (cas_attr->cas_mode >= OT_VO_CAS_MODE_BUTT) {
        vo_err_trace("vo cascade attr cas_mode %u illegal!\n", cas_attr->cas_mode);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (cas_attr->cas_edge_mode >= OT_VO_CAS_MODE_EDGE_BUTT) {
        vo_err_trace("vo cascade attr cas_edge_mode %u illegal!\n", cas_attr->cas_edge_mode);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* 从片从模式是否支持双路模式的规格约束判断 */
    if (vo_drv_is_support_slave_dual_mode() == TD_FALSE) {
        if ((cas_attr->is_slave == TD_TRUE) && (cas_attr->cas_mode == OT_VO_CAS_MODE_DUAL)) {
            vo_err_trace("vo not support dual mode %u when is_slave is %u!\n", cas_attr->cas_mode, cas_attr->is_slave);
            return OT_ERR_VO_NOT_SUPPORT;
        }
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_set_cas_attr(ot_vo_dev phy_dev, const ot_vo_cas_attr *cas_attr)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(phy_dev);

    if (dev_ctx->config == TD_TRUE) {
        vo_err_trace("vo cas phy dev(%d) can't set cascade when is config!\n", phy_dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return vo_check_cas_attr(cas_attr);
}

static td_s32 vo_do_set_cas_attr(const ot_vo_cas_attr *cas_attr)
{
    td_s32 ret;
    vo_dev_info *phy_dev_ctx = TD_NULL;
    ot_vo_dev phy_dev = vo_drv_get_cas_phy_dev();
    if (vo_init_dev_ctx(phy_dev) != TD_SUCCESS) {
        return OT_ERR_VO_NO_MEM;
    }

    ret = vo_check_set_cas_attr(phy_dev, cas_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    phy_dev_ctx = vo_get_dev_ctx(phy_dev);
    (td_void)memcpy_s(&phy_dev_ctx->dev_cas.cas_attr, sizeof(ot_vo_cas_attr), cas_attr, sizeof(ot_vo_cas_attr));
    phy_dev_ctx->dev_cas.cas_config = TD_TRUE;
    return TD_SUCCESS;
}

static td_s32 vo_do_get_cas_attr(ot_vo_cas_attr *cas_attr)
{
    vo_dev_info *phy_dev_ctx = TD_NULL;
    ot_vo_dev phy_dev = vo_drv_get_cas_phy_dev();
    if (vo_init_dev_ctx(phy_dev) != TD_SUCCESS) {
        return OT_ERR_VO_NO_MEM;
    }

    phy_dev_ctx = vo_get_dev_ctx(phy_dev);
    (td_void)memcpy_s(cas_attr, sizeof(ot_vo_cas_attr), &phy_dev_ctx->dev_cas.cas_attr, sizeof(ot_vo_cas_attr));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_cas_attr(ot_vo_cas_attr *cas_attr, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_cas_attr(cas_attr);
    } else {
        ret = vo_do_get_cas_attr(cas_attr);
    }
    vo_up_sem();

    return ret;
}

#endif

#if vo_desc("enable cas dev")

static td_s32 vo_check_cas_dev_single_mode(ot_vo_dev cas_dev)
{
    if ((vo_get_cas_attr_mode() == OT_VO_CAS_MODE_SINGLE) &&
        (cas_dev != vo_drv_get_odd_cas_dev())) {
        vo_err_trace("vo cas_dev(%d) must be VO_CAS_DEV_1 when cas single mode!\n", cas_dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_enable_cas_dev(ot_vo_dev cas_dev)
{
    td_s32 ret;

    ret = vo_check_cas_phy_dev();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_cas_dev_single_mode(cas_dev);
}

static td_s32 vo_check_disable_cas_dev(ot_vo_dev cas_dev)
{
    td_s32 ret;
    ot_vo_layer cas_layer;

    ret = vo_check_cas_dev_single_mode(cas_dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    cas_layer = ot_vo_get_cas_layer(cas_dev);
    vo_check_layer_id_return(cas_layer);

    if (vo_is_layer_chn_enabled(cas_layer)) {
        vo_err_trace("vo dev (%d) layer(%d) exist chn still enabled!\n", cas_dev, cas_layer);
        return OT_ERR_VO_NOT_DISABLE;
    }

    return TD_SUCCESS;
}

static td_void vo_set_cas_dev_cfg(ot_vo_dev cas_dev)
{
    ot_vo_layer phy_dev = vo_drv_get_cas_phy_dev();
    vo_dev_info *phy_dev_ctx = vo_get_dev_ctx(phy_dev);
    vo_dev_info *cas_dev_ctx = vo_get_dev_ctx(cas_dev);

    if (phy_dev_ctx == TD_NULL) {
        vo_err_trace("phy_dev %d ctx is null\n", phy_dev);
        return;
    }

    /* 物理设备配置拷贝到级联设备 */
    (td_void)memcpy_s(&cas_dev_ctx->vou_attr, sizeof(ot_vo_pub_attr), &phy_dev_ctx->vou_attr, sizeof(ot_vo_pub_attr));
    cas_dev_ctx->full_frame_rate = phy_dev_ctx->full_frame_rate;

    if (vo_get_cas_attr_mode() == OT_VO_CAS_MODE_DUAL) {
        cas_dev_ctx->full_frame_rate /= VO_CAS_DUAL_MODE_RATIO;
    }
}

static td_void vo_set_cas_layer_cfg(ot_vo_layer cas_layer)
{
    ot_vo_layer phy_layer = vo_drv_get_cas_phy_layer();
    vo_layer_info *phy_layer_ctx = vo_get_layer_ctx(phy_layer);
    vo_layer_info *cas_layer_ctx = vo_get_layer_ctx(cas_layer);

    if (phy_layer_ctx == TD_NULL) {
        vo_err_trace("phy_layer %d ctx is null\n", phy_layer);
        return;
    }

    /* 物理设备视频层配置拷贝到级联设备视频层 */
    (td_void)memcpy_s(&cas_layer_ctx->vou_video_attr, sizeof(ot_vo_video_layer_attr), &phy_layer_ctx->vou_video_attr,
        sizeof(ot_vo_video_layer_attr));
    (td_void)memcpy_s(&cas_layer_ctx->crop_info, sizeof(ot_crop_info), &phy_layer_ctx->crop_info, sizeof(ot_crop_info));
    (td_void)memcpy_s(&cas_layer_ctx->frm_ctrl, sizeof(fps_ctrl), &phy_layer_ctx->frm_ctrl, sizeof(fps_ctrl));
    (td_void)memcpy_s(&cas_layer_ctx->pts_ctl, sizeof(vo_pts_contrl), &phy_layer_ctx->pts_ctl, sizeof(vo_pts_contrl));
    cas_layer_ctx->target_frame_rate = phy_layer_ctx->target_frame_rate;

    if (vo_get_cas_attr_mode() == OT_VO_CAS_MODE_DUAL) {
        cas_layer_ctx->frm_ctrl.frame_key /= VO_CAS_DUAL_MODE_RATIO;
        cas_layer_ctx->frm_ctrl.full_fps /= VO_CAS_DUAL_MODE_RATIO;
    }
}

static td_s32 vo_do_enable_cas_dev(ot_vo_dev cas_dev)
{
    td_s32 ret;
    td_ulong flags = 0;
    ot_vo_layer cas_layer;
    vo_dev_info *cas_dev_ctx = TD_NULL;
    vo_layer_info *cas_layer_ctx = TD_NULL;

    /* 已关闭返回成功 */
    if (vo_is_dev_enabled(cas_dev)) {
        vo_warn_trace("vo dev(%d) has been enabled!\n", cas_dev);
        return TD_SUCCESS;
    }

    ret = vo_check_enable_cas_dev(cas_dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    cas_layer = ot_vo_get_cas_layer(cas_dev);
    vo_check_layer_id_return(cas_layer);

    ret = vo_init_layer_ctx(cas_layer);
    if (ret != TD_SUCCESS) {
        return OT_ERR_VO_NO_MEM;
    }

    /* 拷贝物理设备和层配置到级联设备和层 */
    vo_set_cas_dev_cfg(cas_dev);
    vo_set_cas_layer_cfg(cas_layer);

    ret = vo_alloc_layer_vb(cas_layer);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    cas_layer_ctx = vo_get_layer_ctx(cas_layer);
    cas_layer_ctx->video_enable = TD_TRUE;
    cas_layer_ctx->video_config = TD_TRUE;

    vo_dev_spin_lock(&flags);
    cas_dev_ctx = vo_get_dev_ctx(cas_dev);
    cas_dev_ctx->vo_enable = TD_TRUE;
    vo_dev_spin_unlock(&flags);

    /* 根据物理设备配置级联的VBI位置信息 */
    vo_drv_set_cas_vbi_pos();
    return TD_SUCCESS;
}

static td_s32 vo_do_disable_cas_dev(ot_vo_dev cas_dev)
{
    td_s32 ret;
    td_ulong flags = 0;
#ifdef CONFIG_OT_VO_BATCH
    td_ulong list_flags = 0;
#endif

    ot_vo_layer cas_layer;
    vo_dev_info *cas_dev_ctx = TD_NULL;
    vo_layer_info *cas_layer_ctx = TD_NULL;

    /* 已关闭返回成功 */
    if (vo_is_dev_enabled(cas_dev) == TD_FALSE) {
        vo_warn_trace("vo dev(%d) has been disabled!\n", cas_dev);
        return TD_SUCCESS;
    }

    ret = vo_check_disable_cas_dev(cas_dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    cas_layer = ot_vo_get_cas_layer(cas_dev);
    vo_check_layer_id_return(cas_layer);

    ret = vo_init_layer_ctx(cas_layer);
    if (ret != TD_SUCCESS) {
        return OT_ERR_VO_NO_MEM;
    }

    vo_dev_spin_lock(&flags);
    cas_dev_ctx = vo_get_dev_ctx(cas_dev);
    cas_dev_ctx->vo_enable = TD_FALSE;
    vo_dev_spin_unlock(&flags);

    ret = vo_release_layer_vb(cas_layer);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    cas_layer_ctx = vo_get_layer_ctx(cas_layer);
    cas_layer_ctx->video_enable = TD_FALSE;
    cas_layer_ctx->video_config = TD_FALSE;

    /* 重置某些变量 */
    vo_reset_dev_vars(cas_dev);
    vo_reset_layer_vars(cas_layer);

    return TD_SUCCESS;
}

td_s32 vo_ioctl_enable_disable_cas_dev(const vo_ioctl_dev_param *ioctl_dev, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_cas_dev(ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_ENABLE) {
        ret = vo_do_enable_cas_dev(ioctl_dev->dev);
    } else {
        ret = vo_do_disable_cas_dev(ioctl_dev->dev);
    }
    vo_up_sem();

    return ret;
}

td_s32 vo_disable_cas_dev(const vo_ioctl_dev_param *ioctl_dev)
{
    return vo_ioctl_enable_disable_cas_dev(ioctl_dev, VO_IOCTL_TYPE_DISABLE);
}

#endif

#if vo_desc("cas pattern")

static td_s32 vo_do_set_cas_pattern(ot_vo_dev cas_dev, td_u32 pattern)
{
    td_ulong flags = 0;
    vo_dev_info *cas_dev_ctx = vo_get_dev_ctx(cas_dev);

    if (pattern >= OT_VO_MAX_CAS_PATTERN) {
        vo_err_trace("vo cascade pattern %u illegal!\n", pattern);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    vo_dev_spin_lock(&flags);
    cas_dev_ctx->dev_cas.cas_pattern = pattern;
    vo_dev_spin_unlock(&flags);
    return TD_SUCCESS;
}

static td_void vo_do_get_cas_pattern(ot_vo_dev cas_dev, td_u32 *pattern)
{
    td_ulong flags = 0;
    vo_dev_info *cas_dev_ctx = vo_get_dev_ctx(cas_dev);

    vo_dev_spin_lock(&flags);
    *pattern = cas_dev_ctx->dev_cas.cas_pattern;
    vo_dev_spin_unlock(&flags);
}

td_s32 vo_ioctl_cas_pattern(vo_ioctl_cas_pattern_param *ioctl_cas_pattern, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_cas_dev(&ioctl_cas_pattern->cas_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_cas_pattern(ioctl_cas_pattern->cas_dev.dev, ioctl_cas_pattern->pattern);
    } else {
        vo_do_get_cas_pattern(ioctl_cas_pattern->cas_dev.dev, &(ioctl_cas_pattern->pattern));
    }
    vo_up_sem();

    return ret;
}

#endif

#if vo_desc("bind cas chn")

static td_u32 vo_get_max_cas_pos(ot_vo_cas_rgn cas_rgn)
{
    td_u32 max_pos = OT_VO_MAX_CAS_POS_64RGN;

    if (cas_rgn >= OT_VO_CAS_RGN_BUTT) {
        vo_err_trace("vo cas rgn %u is invalid, use default %u!\n", cas_rgn, max_pos);
        return max_pos;
    }

    if (cas_rgn == OT_VO_CAS_RGN_32) {
        max_pos = OT_VO_MAX_CAS_POS_32RGN;
    }

    return max_pos;
}

static td_s32 vo_check_cas_pos(td_u32 pos)
{
    ot_vo_cas_rgn cas_rgn = vo_get_cas_attr_rgn();
    td_u32 max_pos = vo_get_max_cas_pos(cas_rgn);
    if (pos >= max_pos) {
        vo_err_trace("cas pos %u is out of range pos[0,%u) when cas rgn is %u!\n", pos, max_pos, cas_rgn);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_cas_pos_bind_dev(td_u32 pos, ot_vo_dev cas_dev)
{
    if (vo_is_dev_enabled(cas_dev) != TD_TRUE) {
        vo_err_trace("vo cas dev(%d) is not enabled, not support bind pos chn!\n", cas_dev);
        return OT_ERR_VO_NOT_ENABLE;
    }

    return vo_check_cas_pos(pos);
}

#endif

#if vo_desc("enable cas")

static td_s32 vo_do_enable_cas(td_void)
{
    td_s32 ret;
    td_ulong flags = 0;
    ot_vo_dev phy_dev = vo_drv_get_cas_phy_dev();
    ot_vo_layer phy_layer = vo_drv_get_cas_phy_layer();
    vo_dev_info *phy_dev_ctx = TD_NULL;

    ret = vo_check_cas_phy_dev();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (vo_init_dev_ctx(phy_dev) != TD_SUCCESS) {
        return OT_ERR_VO_NO_MEM;
    }

    vo_dev_spin_lock(&flags);
    /* 关闭CSC使能 */
    vou_drv_layer_csc_enable(phy_layer, TD_FALSE);
    /* 使能硬件级联 */
    vo_drv_set_cas_enable(TD_TRUE);
    phy_dev_ctx = vo_get_dev_ctx(phy_dev);
    phy_dev_ctx->dev_cas.cas_enable = TD_TRUE;
    vo_dev_spin_unlock(&flags);

    return TD_SUCCESS;
}

static td_s32 vo_do_disable_cas(td_void)
{
    td_ulong flags = 0;
    ot_vo_dev phy_dev = vo_drv_get_cas_phy_dev();
    ot_vo_layer phy_layer = vo_drv_get_cas_phy_layer();
    vo_dev_info *phy_dev_ctx = TD_NULL;

    if (vo_init_dev_ctx(phy_dev) != TD_SUCCESS) {
        return OT_ERR_VO_NO_MEM;
    }

    /* 允许重复去使能 */
    phy_dev_ctx = vo_get_dev_ctx(phy_dev);
    if (phy_dev_ctx->dev_cas.cas_enable == TD_FALSE) {
        vo_warn_trace("vo phy dev(%d) has been disabled cas!\n", phy_dev);
        return TD_SUCCESS;
    }

    vo_dev_spin_lock(&flags);
    /* 关闭硬件级联 */
    vo_drv_set_cas_enable(TD_FALSE);
    /* 打开CSC使能 */
    vou_drv_layer_csc_enable(phy_layer, TD_TRUE);
    phy_dev_ctx->dev_cas.cas_enable = TD_FALSE;
    vo_dev_spin_unlock(&flags);
    return TD_SUCCESS;
}

td_s32 vo_ioctl_enable_disable_cas(vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_ENABLE) {
        ret = vo_do_enable_cas();
    } else {
        ret = vo_do_disable_cas();
    }
    vo_up_sem();
    return ret;
}

#endif

#endif
