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
#include "mkp_vo_dev.h"
#include "mkp_vo_intf.h"
#include "mkp_vo_cascade.h"
#include "drv_vo.h"
#include "drv_vo_comm.h"
#include "mkp_vo_init.h"
#include "vou.h"

#if vo_desc("dev init")

static vo_dev_info *g_vo_dev[OT_VO_MAX_DEV_NUM];

vo_dev_info *vo_get_dev_ctx(ot_vo_dev vo_dev)
{
    return g_vo_dev[vo_dev];
}

static td_void vo_set_dev_ctx(ot_vo_dev vo_dev, vo_dev_info *dev_ctx)
{
    g_vo_dev[vo_dev] = dev_ctx;
}

static td_void vo_init_pub_dev_ctx(vo_dev_info *dev_ctx)
{
    vo_init_dev_param(&(dev_ctx->dev_param));
    vo_init_dev_intf_param(dev_ctx);
}

static td_void vo_init_dev_ctx_less_buf_attr(vo_dev_info *dev_ctx)
{
    ot_vo_less_buf_attr *less_buf_attr = &dev_ctx->less_buf_attr;

    less_buf_attr->enable = TD_FALSE;
    less_buf_attr->vtth = VO_LESS_BUF_DEFAULT_VTTH;
}

static td_void vo_init_dev_ctx_user_notify_attr(vo_dev_info *dev_ctx)
{
    ot_vo_user_notify_attr *user_notify_attr  = &dev_ctx->user_notify_attr;

    user_notify_attr->enable = TD_FALSE;
    user_notify_attr->vtth = VO_USER_NOTIFY_DEFAULT_VTTH;
}

td_s32 vo_init_dev_ctx(ot_vo_dev dev)
{
    td_u32 dev_ctx_len;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    /* 如果已经存在就不初始化 */
    if (dev_ctx != TD_NULL) {
        return TD_SUCCESS;
    }

    /* 分配空间 */
    dev_ctx_len = sizeof(vo_dev_info);
    dev_ctx = (vo_dev_info *)osal_vmalloc(dev_ctx_len);
    if (dev_ctx == TD_NULL) {
        vo_err_trace("alloc device ctx failed!\n");
        return OT_ERR_VO_NO_MEM;
    }

    (td_void)memset_s(dev_ctx, dev_ctx_len, 0, dev_ctx_len);
    vo_init_pub_dev_ctx(dev_ctx);
    vo_init_dev_ctx_less_buf_attr(dev_ctx);
    vo_init_dev_ctx_user_notify_attr(dev_ctx);
    osal_wait_init(&(dev_ctx->load_detect_info.vo_wait_query_intf_status));
    vo_set_dev_ctx(dev, dev_ctx);
    return TD_SUCCESS;
}

static td_void vo_deinit_dev_ctx(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx == TD_NULL) {
        return;
    }

    osal_wait_destroy(&(dev_ctx->load_detect_info.vo_wait_query_intf_status));
    osal_vfree(dev_ctx);
    vo_set_dev_ctx(dev, TD_NULL);
}

td_void vo_init_dev_resource(td_void)
{
    ot_vo_dev dev;

    for (dev = 0; dev < OT_VO_MAX_DEV_NUM; dev++) {
        vo_set_dev_ctx(dev, TD_NULL);
    }
}

td_void vo_deinit_dev_resource(td_void)
{
    ot_vo_dev dev;

    for (dev = 0; dev < OT_VO_MAX_DEV_NUM; dev++) {
        vo_deinit_dev_ctx(dev);
    }
}

td_void vo_init_dev_info(td_void)
{
    /* 如果模块参数exit_dev_en为TD_FALSE，则部分初始化。保留设备使能和设备属性 */
    if ((vo_get_vo_init_flag() == TD_FALSE) || (vo_get_mod_param_exit_dev_en() == TD_TRUE)) {
        vo_drv_dev_info_init();
        vo_init_dev_resource();
    } else {
        vo_drv_dev_info_part_init();
    }
}

td_void vo_deinit_dev_info(td_void)
{
    vo_drv_dev_info_de_init();

    /* 如果模块参数exit_dev_en为TD_FALSE，则部分初始化。保留设备使能和设备属性 */
    if (vo_get_mod_param_exit_dev_en() == TD_TRUE) {
        vo_deinit_dev_resource();
    }
}

static td_void vo_exit_stop_dev(ot_vo_dev dev)
{
    vo_ioctl_dev_param ioctl_dev = {0};

    ioctl_dev.dev = dev;
#ifdef CONFIG_OT_VO_CASCADE
    if (vo_is_cas_dev(dev)) {
        vo_disable_cas_dev(&ioctl_dev);
        return;
    }
#endif

    vo_disable(&ioctl_dev);
}

td_void vo_exit_dev(td_void)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;

    for (dev = 0; dev < OT_VO_MAX_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (dev_ctx == TD_NULL) {
            continue;
        }

        if ((dev_ctx->vo_enable) && (vo_get_mod_param_exit_dev_en() == TD_TRUE)) {
            vo_exit_stop_dev(dev);
        }
    }
}

#endif

#if vo_desc("dev pub api")

td_bool vo_is_dev_config_cas(ot_vo_dev dev)
{
#ifdef CONFIG_OT_VO_CASCADE
    vo_dev_info *dev_ctx = TD_NULL;

    if (dev != VO_DEV_DHD0) {
        return TD_FALSE;
    }

    /* 如下为空有可能属于正常场景，返回FALSE */
    dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx == TD_NULL) {
        return TD_FALSE;
    }

    return dev_ctx->dev_cas.cas_config;
#else
    ot_unused(dev);
#endif
    return TD_FALSE;
}

td_bool vo_is_dev_enabled(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = TD_NULL;

    /* 需要上层来保证设备ID的有效性,这里做好数组访问保护 */
    if (vo_def_check_dev_id(dev) != TD_SUCCESS) {
        vo_warn_trace("Vo device id %d is invalid!\n", dev);
        return TD_FALSE;
    }

    /* 如下场景可能属于正常场景,不打印错误码 */
    dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx == TD_NULL) {
        return TD_FALSE;
    }

    return dev_ctx->vo_enable;
}

td_u32 vo_get_dev_bg_color(ot_vo_dev dev)
{
    /* 需要上层来保证设备ID的有效性 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    return dev_ctx->vou_attr.bg_color;
}

static td_bool vo_is_dev_interlaced_user_intf_sync(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if ((dev_ctx->vou_attr.intf_sync == OT_VO_OUT_USER) &&
        (dev_ctx->vou_attr.sync_info.iop == TD_FALSE)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

td_bool vo_is_dev_interlaced_typical_intf_sync(ot_vo_dev dev)
{
    /* 需要上层来保证设备ID的有效性 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    ot_vo_intf_sync intf_sync = dev_ctx->vou_attr.intf_sync;

    if ((intf_sync == OT_VO_OUT_PAL) ||
        (intf_sync == OT_VO_OUT_NTSC) ||
        (intf_sync == OT_VO_OUT_1080I50) ||
        (intf_sync == OT_VO_OUT_1080I60)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

td_bool vo_is_dev_interlaced_intf_sync(ot_vo_dev dev)
{
    /* 需要上层来保证设备ID的有效性 */
    if ((vo_is_dev_interlaced_typical_intf_sync(dev) == TD_TRUE) ||
        (vo_is_dev_interlaced_user_intf_sync(dev) == TD_TRUE)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

td_void vo_get_dev_max_size(ot_vo_dev dev, ot_size *dev_size)
{
    /* 需要上层来保证设备ID的有效性 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    dev_size->width = dev_ctx->max_width;
    dev_size->height = dev_ctx->max_height;
}

td_u32 vo_get_dev_full_frame_rate(ot_vo_dev dev)
{
    /* 需要上层来保证设备ID的有效性 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    return dev_ctx->full_frame_rate;
}

td_u32 vo_get_dev_less_buf_enable(ot_vo_dev dev)
{
    /* 需要上层来保证设备ID的有效性 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    return dev_ctx->less_buf_attr.enable;
}

td_u32 vo_get_dev_user_notify_enable(ot_vo_dev dev)
{
    /* 需要上层来保证设备ID的有效性 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    return dev_ctx->user_notify_attr.enable;
}

static td_void vo_do_disable_soft(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx == TD_NULL) {
        return;
    }

    dev_ctx->config = TD_FALSE;
    dev_ctx->vo_enable = TD_FALSE;
    dev_ctx->dev_cas.cas_config = TD_FALSE;
    dev_ctx->dev_cas.cas_enable = TD_FALSE;
    vo_drv_disable(dev);
}

/* 设备软关闭 */
static td_void vo_disable_soft(ot_vo_dev dev)
{
    td_ulong flags = 0;

    vo_dev_spin_lock(&flags);
    vo_do_disable_soft(dev);
    vo_dev_spin_unlock(&flags);
}

td_void vo_disable_all_dev_soft(td_void)
{
    ot_vo_dev dev;

    for (dev = 0; dev < OT_VO_MAX_DEV_NUM; dev++) {
        if ((vo_drv_check_dev_id(dev) != TD_SUCCESS) || (vo_get_mod_param_exit_dev_en() != TD_TRUE)) {
            continue;
        }
        vo_disable_soft(dev);
    }
}

#endif

#if vo_desc("intf sync info")

static vo_intf_sync_info g_vo_intf_sync_info[OT_VO_OUT_BUTT] = {
    { OT_VO_OUT_PAL,          "PAL",          720,  576,  25 },
    { OT_VO_OUT_NTSC,         "NTSC",         720,  480,  30 },
    { OT_VO_OUT_960H_PAL,     "960H_PAL",     960,  576,  25 },
    { OT_VO_OUT_960H_NTSC,    "960H_NTSC",    960,  480,  30 },

    { OT_VO_OUT_640x480_60,   "640x480@60",   640,  480,  60 },
    { OT_VO_OUT_480P60,       "480p@60",      720,  480,  60 },
    { OT_VO_OUT_576P50,       "576p@50",      720,  576,  50 },
    { OT_VO_OUT_800x600_60,   "800x600@60",   800,  600,  60 },
    { OT_VO_OUT_1024x768_60,  "1024x768@60",  1024, 768,  60 },
    { OT_VO_OUT_720P50,       "720P@50",      1280, 720,  50 },
    { OT_VO_OUT_720P60,       "720P@60",      1280, 720,  60 },
    { OT_VO_OUT_1280x800_60,  "1280x800@60",  1280, 800,  60 },
    { OT_VO_OUT_1280x1024_60, "1280x1024@60", 1280, 1024, 60 },
    { OT_VO_OUT_1366x768_60,  "1366x768@60",  1366, 768,  60 },
    { OT_VO_OUT_1400x1050_60, "1400x1050@60", 1400, 1050, 60 },
    { OT_VO_OUT_1440x900_60,  "1440x900@60",  1440, 900,  60 },
    { OT_VO_OUT_1680x1050_60, "1680x1050@60", 1680, 1050, 60 },

    { OT_VO_OUT_1080P24,      "1080P@24",     1920, 1080, 24 },
    { OT_VO_OUT_1080P25,      "1080P@25",     1920, 1080, 25 },
    { OT_VO_OUT_1080P30,      "1080P@30",     1920, 1080, 30 },
    { OT_VO_OUT_1080I50,      "1080I50",      1920, 1080, 25 },
    { OT_VO_OUT_1080I60,      "1080I60",      1920, 1080, 30 },
    { OT_VO_OUT_1080P50,      "1080P@50",     1920, 1080, 50 },
    { OT_VO_OUT_1080P60,      "1080P@60",     1920, 1080, 60 },

    { OT_VO_OUT_1600x1200_60, "1600x1200@60", 1600, 1200, 60 },
    { OT_VO_OUT_1920x1200_60, "1920x1200@60", 1920, 1200, 60 },
    { OT_VO_OUT_1920x2160_30, "1920x2160@30", 1920, 2160, 30 },
    { OT_VO_OUT_2560x1440_30, "2560x1440@30", 2560, 1440, 30 },
    { OT_VO_OUT_2560x1440_60, "2560x1440@60", 2560, 1440, 60 },
    { OT_VO_OUT_2560x1600_60, "2560x1600@60", 2560, 1600, 60 },

    { OT_VO_OUT_3840x2160_24, "3840x2160@24", 3840, 2160, 24 },
    { OT_VO_OUT_3840x2160_25, "3840x2160@25", 3840, 2160, 25 },
    { OT_VO_OUT_3840x2160_30, "3840x2160@30", 3840, 2160, 30 },
    { OT_VO_OUT_3840x2160_50, "3840x2160@50", 3840, 2160, 50 },
    { OT_VO_OUT_3840x2160_60, "3840x2160@60", 3840, 2160, 60 },
    { OT_VO_OUT_4096x2160_24, "4096x2160@24", 4096, 2160, 24 },
    { OT_VO_OUT_4096x2160_25, "4096x2160@25", 4096, 2160, 25 },
    { OT_VO_OUT_4096x2160_30, "4096x2160@30", 4096, 2160, 30 },
    { OT_VO_OUT_4096x2160_50, "4096x2160@50", 4096, 2160, 50 },
    { OT_VO_OUT_4096x2160_60, "4096x2160@60", 4096, 2160, 60 },
    { OT_VO_OUT_7680x4320_30, "7680x4320@30", 7680, 4320, 30 },

    { OT_VO_OUT_240x320_50,   "240x320@50",   240,  320,  50 },
    { OT_VO_OUT_320x240_50,   "320x240@50",   320,  240,  50 },
    { OT_VO_OUT_240x320_60,   "240x320@60",   240,  320,  60 },
    { OT_VO_OUT_320x240_60,   "320x240@60",   320,  240,  60 },
    { OT_VO_OUT_800x600_50,   "800x600@50",   800,  600,  50 },

    { OT_VO_OUT_720x1280_60,  "720x1280@60",  720,  1280, 60 },
    { OT_VO_OUT_1080x1920_60, "1080x1920@60", 1080, 1920, 60 },

    { OT_VO_OUT_USER,         "USER",         0,    0,    0  },
};

/* 调用者保证输入参数的有效性,保证数组访问不会越界 */
static vo_intf_sync_info *vo_get_intf_sync_info(ot_vo_intf_sync intf_sync)
{
    return &g_vo_intf_sync_info[intf_sync];
}

td_char *vo_get_intf_sync_name(ot_vo_intf_sync intf_sync)
{
    vo_intf_sync_info *sync_info = TD_NULL;

    if (intf_sync >= OT_VO_OUT_BUTT) {
        return "-";
    }

    sync_info = vo_get_intf_sync_info(intf_sync);
    if (sync_info->intf_sync == intf_sync) {
        return sync_info->name;
    }

    return "-";
}

static td_void vo_get_intf_sync_size(ot_vo_intf_sync intf_sync, td_u32 *width, td_u32 *height)
{
    vo_intf_sync_info *sync_info = TD_NULL;

    /* 标准时序才有效, 上层函数保证有效性 */
    sync_info = vo_get_intf_sync_info(intf_sync);
    if (sync_info->intf_sync == intf_sync) {
        *width = sync_info->width;
        *height = sync_info->height;
    }
}

static td_void vo_get_intf_sync_frame_rate(ot_vo_intf_sync intf_sync, td_u32 *frame_rate)
{
    vo_intf_sync_info *sync_info = TD_NULL;

    /* 标准时序才有效, 上层函数保证有效性 */
    if (vo_is_typical_intf_sync(intf_sync) != TD_TRUE) {
        return;
    }

    sync_info = vo_get_intf_sync_info(intf_sync);
    if (sync_info->intf_sync == intf_sync) {
        *frame_rate = sync_info->frame_rate;
    }
}

static td_void vo_get_intf_sync_size_user(const ot_vo_sync_info *sync_info, td_u32 *width, td_u32 *height)
{
    *width = sync_info->hact;
    *height = (sync_info->iop) ? (sync_info->vact) : (sync_info->vact * 2); /* 隔行时序，高度为2倍 */
}

#endif

#if vo_desc("dev ioctl check pub")

/* 调用者需要保证输入参数的有效性 */
td_s32 vo_check_ioctl_dev(const vo_ioctl_dev_param *ioctl_dev)
{
    vo_check_dev_id_return(ioctl_dev->dev);
    vo_check_cas_dev_return(ioctl_dev->dev);
    vo_init_dev_ctx_return(ioctl_dev->dev);

    return TD_SUCCESS;
}

#endif

#if vo_desc("enable dev")

td_void vo_reset_dev_vars(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    vo_init_pub_dev_ctx(dev_ctx);
    (td_void)memset_s(&dev_ctx->debug_info, sizeof(vo_debug_info), 0, sizeof(vo_debug_info));
}

static td_void vo_enable_clk(td_void)
{
    vo_lpw_bus_reset(TD_FALSE);
    vo_drv_set_all_crg_clk(TD_TRUE);
}

static td_s32 vo_check_enable_user_div(ot_vo_dev dev)
{
    td_s32 ret;
    td_u32 dev_div;
    td_u32 pre_div;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    /* 用户时序没有配置时不检查 */
    if ((dev_ctx->vou_attr.intf_sync != OT_VO_OUT_USER) || (dev_ctx->user_config != TD_TRUE)) {
        return TD_SUCCESS;
    }

    dev_div = dev_ctx->vo_user_sync_info.dev_div;
    pre_div = dev_ctx->vo_user_sync_info.pre_div;
    ret = vo_check_dev_user_div(dev, dev_div, pre_div);
    if (ret != TD_SUCCESS) {
        vo_err_trace("vo dev %d user sync dev_div %u or pre_div %u is illegal, pls check the user sync info!\n",
            dev, dev_div, pre_div);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_dev_vtth(ot_vo_dev dev)
{
    td_s32 ret;
    td_u32 vtth1;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    /* 用户没有配置vtth，使用时序计算出一个vtth1来，否则使用用户配置的vtth1 */
    if (dev_ctx->vtth1_config != TD_TRUE) {
        vtth1 = vo_drv_cal_vtth1_by_sync(dev, dev_ctx->full_frame_rate);
        vo_drv_set_dev_vtth(dev, vtth1);
    }

    /* 检查不允许超过vert_all */
    ret = vo_drv_check_dev_vtth(dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("vo dev %d vtth1(%u) illegal\n", dev, vo_drv_get_dev_vtth(dev));
        return ret;
    }

    ret = vo_drv_check_dev_vtth2(dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("vo dev %d vtth2(%u) illegal\n", dev, vo_drv_get_dev_vtth2(dev));
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_enable(ot_vo_dev dev)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

#ifdef CONFIG_OT_VO_CASCADE
    ret = vo_notify_cas_check_enable_dev(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif

    if (dev_ctx->vo_enable == TD_TRUE) {
        vo_err_trace("vo device %d has enabled!\n", dev);
        return OT_ERR_VO_NOT_DISABLE;
    }

    ret = vo_check_enable_user_div(dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("vo%d's user sync info div is illegal!\n", dev);
        return ret;
    }

    /* 检查vtth，以及vtth1与vtth2的关系 */
    ret = vo_check_dev_vtth(dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("vo%d's vtth1 or vtth2 is illegal!\n", dev);
        return ret;
    }

    return TD_SUCCESS;
}

static td_void vo_enable_dev_clk_sel(ot_vo_dev dev)
{
    /* 用户时序直接返回 */
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx->vou_attr.intf_sync == OT_VO_OUT_USER) {
        return;
    }

    vo_drv_set_dev_clk_sel(dev, 0);
}

static td_void vo_enable_dev_clk(ot_vo_dev dev)
{
    vo_drv_set_dev_clk(dev);

    /* 此处务必遵从逻辑约束：先打开时钟，再配vdp设备使能，否则vdp设备使能不生效 */
    vo_drv_set_dev_clk_en(dev, TD_TRUE);

    vo_enable_dev_clk_sel(dev);

#ifdef CONFIG_OT_VO_CASCADE
    vo_notify_cas_enable_dev(dev);
#endif
}

static td_void vo_do_enable_dev(ot_vo_dev dev)
{
    vo_enable_dev_clk(dev);
    vo_drv_open(dev);
    vo_set_dev_intf_param(dev);
}

static td_void vo_had_enable(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    vo_init_load_detect(dev);

    /* 记录使能信息到DRV层 */
    vo_drv_enable(dev);
    dev_ctx->vo_enable = TD_TRUE;
}

static td_void vo_set_uboot_vo_enabled(ot_vo_dev dev, td_bool uboot_vo_enabled)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    dev_ctx->uboot_vo_enabled = uboot_vo_enabled;
}

static td_s32 vo_do_enable(ot_vo_dev dev)
{
    td_s32 ret;
    td_ulong flags = 0;

    vo_dev_spin_lock(&flags);
    vo_enable_clk();
    vo_dev_spin_unlock(&flags);

    ret = vo_check_enable(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* uboot可能未打开中断，打开垂直时序中断，FB需要用到VO的中断，以及配置vtth相关内容 */
    if (vou_drv_get_dev_enable(dev) == TD_TRUE) {
        vo_info_trace("vo device %d has enabled in uboot!\n", dev);
        vo_drv_do_int_vtth_config(dev, TD_TRUE);
        vo_set_uboot_vo_enabled(dev, TD_TRUE);
    } else {
        vo_do_enable_dev(dev);
    }

    vo_had_enable(dev);

    /*
     * 所有vdp相关的寄存器配置完毕后才能关闭，否则配寄存器不能生效或异常。
     * 时钟是否由设备外控制，如果不是，则由自己打开时钟
     */
    if (vo_get_mod_param_dev_clk_ext_en() == TD_TRUE) {
        vo_drv_set_dev_clk_en(dev, TD_FALSE);
    }

    return TD_SUCCESS;
}

#endif

#if vo_desc("disable dev")

static td_s32 vo_check_disable_pub_attr(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        return TD_SUCCESS;
    }

    /* uboot中可能开启VO设备,此时进入内核后需要配置和uboot一样的设备属性再停止设备以免接口没有停止掉导致的黑屏问题 */
    if (vou_drv_get_dev_enable(dev) != TD_TRUE) {
        return TD_SUCCESS;
    }

    vo_set_uboot_vo_enabled(dev, TD_TRUE);

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d has enabled in uboot, should set the same pub attr "
            "firstly when disable dev in kernel!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_disable(ot_vo_dev dev)
{
    return vo_check_disable_pub_attr(dev);
}

static td_void vo_reset_vtth(ot_vo_dev dev)
{
    if ((dev >= OT_VO_MAX_PHYS_DEV_NUM) || (vo_is_phy_dev(dev) != TD_TRUE)) {
        return;
    }

    vo_drv_set_dev_default_vtth(dev);

#if (defined(CONFIG_OT_VO_LESS_BUF) || defined(CONFIG_OT_VO_USER_NOTIFY))
    if (vo_get_dev_less_buf_enable(dev) == TD_TRUE || vo_get_dev_user_notify_enable(dev) == TD_TRUE) {
        vo_drv_set_dev_default_vtth2(dev);
    }
#endif
}

td_void vo_disable_clk(td_void)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;

    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        /*
         * 优先检查寄存器，然后检查上下文
         * 这里还需要从寄存器上看设备是否使能，
         * 因为开机画面时上下文是不使能的
         */
        if (vou_drv_get_dev_enable(dev) == TD_TRUE) {
            break;
        }

        dev_ctx = vo_get_dev_ctx(dev);
        if (dev_ctx == TD_NULL) {
            continue;
        }

        if (dev_ctx->vo_enable) {
            break;
        }
    }

    /* 表明所有设备都已经关闭 */
    if (dev == OT_VO_MAX_PHYS_DEV_NUM) {
        vo_drv_set_all_crg_clk(TD_FALSE);
    }
}

static td_void vo_disable_phy_dev(ot_vo_dev dev)
{
    /* 时钟是否由设备外控制，如果是，在关闭设备时打开时钟，后面需要配置寄存器 */
    if (vo_get_mod_param_dev_clk_ext_en() == TD_TRUE) {
        vo_drv_set_dev_clk_en(dev, TD_TRUE);
    }

#ifdef CONFIG_OT_VO_CASCADE
    vo_notify_cas_disable_dev(dev);
#endif
    vo_deinit_load_detect(dev);
    vo_drv_close(dev);
    vo_drv_set_dev_clk_en(dev, TD_FALSE);
}

static td_s32 vo_do_disable(ot_vo_dev dev)
{
    td_s32 ret;
    td_ulong flags = 0;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    vo_dev_spin_lock(&flags);
    vo_drv_set_all_crg_clk(TD_TRUE);
    vo_dev_spin_unlock(&flags);

    ret = vo_check_disable(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (vo_is_phy_dev(dev)) {
        vo_disable_phy_dev(dev);
        vo_drv_dev_int_clear(dev);
    }

    /* 记录禁用信息到DRV层 */
    vo_drv_disable(dev);

    dev_ctx->config = TD_FALSE;
    dev_ctx->vo_enable = TD_FALSE;
    dev_ctx->user_config = TD_FALSE;
    dev_ctx->vtth1_config = TD_FALSE;
    dev_ctx->dev_cas.cas_config = TD_FALSE;
    dev_ctx->dev_cas.cas_enable = TD_FALSE;
    vo_reset_dev_vars(dev);

    /* 重置vtth，设0即可，内部会自动调节至最小值 */
    vo_reset_vtth(dev);

    vo_dev_spin_lock(&flags);
    vo_disable_clk();
    vo_dev_spin_unlock(&flags);

    return TD_SUCCESS;
}

td_s32 vo_ioctl_enable_disable(const vo_ioctl_dev_param *ioctl_dev, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_ENABLE) {
        ret = vo_do_enable(ioctl_dev->dev);
    } else {
        ret = vo_do_disable(ioctl_dev->dev);
    }
    vo_up_sem();
    return ret;
}

td_s32 vo_disable(const vo_ioctl_dev_param *ioctl_dev)
{
    return vo_ioctl_enable_disable(ioctl_dev, VO_IOCTL_TYPE_DISABLE);
}

#endif

#if vo_desc("pub attr")

static td_s32 vo_check_set_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (dev_ctx->vo_enable == TD_TRUE) {
        vo_err_trace("vo%d doesn't disabled!\n", dev);
        return OT_ERR_VO_NOT_DISABLE;
    }

    ret = vou_drv_check_dev_pub_attr(dev, pub_attr);
    if (ret != TD_SUCCESS) {
        vo_err_trace("vo%d's pub attr is illegal!\n", dev);
        return ret;
    }

#ifdef CONFIG_OT_VO_CASCADE
    ret = vo_notify_cas_check_set_pub_attr(dev, pub_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif

    return TD_SUCCESS;
}

static td_void vo_set_phy_dev_pub_attr(ot_vo_dev dev)
{
    /*
     * 由于大分辨率时钟频率比较高，比如4k x 2k，设置中断上报阈值需要增加，
     * 在时序确定并记录进上下文时依据时序调整要配置的vtth值
     */
    td_u32 vtth = vo_drv_get_dev_vtth(dev);
    vo_drv_set_dev_vtth(dev, vtth);
}

static td_void vo_set_disp_max_size(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr)
{
    td_u32 max_width = VO_DISP_1080_WIDTH;
    td_u32 max_height = VO_DISP_1080_HEIGHT;
    ot_vo_intf_sync intf_sync = pub_attr->intf_sync;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_typical_intf_sync(intf_sync)) {
        vo_get_intf_sync_size(intf_sync, &max_width, &max_height);
    } else {
        vo_get_intf_sync_size_user(&pub_attr->sync_info, &max_width, &max_height);
    }

    dev_ctx->max_width = max_width;
    dev_ctx->max_height = max_height;
    vou_drv_set_disp_max_size(dev, max_width, max_height);
}

static td_void vo_set_full_frame_rate(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr)
{
    td_u32 frame_rate = VO_DISP_FREQ_VGA;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    vo_get_intf_sync_frame_rate(pub_attr->intf_sync, &frame_rate);
    dev_ctx->full_frame_rate = frame_rate;
}

static td_s32 vo_do_set_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_pub_attr(dev, pub_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    vo_set_full_frame_rate(dev, pub_attr);
    vo_set_disp_max_size(dev, pub_attr);
    vo_drv_set_pub_attr(dev, pub_attr);

    (td_void)memcpy_s(&dev_ctx->vou_attr, sizeof(ot_vo_pub_attr), pub_attr, sizeof(ot_vo_pub_attr));
    dev_ctx->config = TD_TRUE;

    /* 配置对应的垂直时序，等DRV层配置后再配置，以免时序不正确导致默认值不正确 */
    if (vo_is_phy_dev(dev)) {
        vo_set_phy_dev_pub_attr(dev);
    }

    return TD_SUCCESS;
}

static td_void vo_do_get_pub_attr(ot_vo_dev dev, ot_vo_pub_attr *pub_attr)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    (td_void)memcpy_s(pub_attr, sizeof(ot_vo_pub_attr), &dev_ctx->vou_attr, sizeof(ot_vo_pub_attr));
}

td_s32 vo_ioctl_pub_attr(vo_ioctl_pub_attr_param *ioctl_pub_attr, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_pub_attr->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_pub_attr(ioctl_pub_attr->ioctl_dev.dev, &(ioctl_pub_attr->pub_attr));
    } else {
        vo_do_get_pub_attr(ioctl_pub_attr->ioctl_dev.dev, &(ioctl_pub_attr->pub_attr));
    }
    vo_up_sem();

    return ret;
}

#endif

#if vo_desc("dev param")

td_void vo_init_dev_param(ot_vo_dev_param *dev_param)
{
    dev_param->vo_bypass_en = TD_FALSE;
}

#ifdef CONFIG_OT_VO_DEV_BYPASS

static td_s32 vo_check_support_dev_param(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev(%d) is not phy device, not support dev param!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    if ((vo_drv_is_bt1120_intf(dev_ctx->vou_attr.intf_type) == TD_FALSE) &&
        (vo_drv_is_mipi_intf(dev_ctx->vou_attr.intf_type) == TD_FALSE)) {
        vo_err_trace("vo dev(%d): vo bypass func is only supported by intf bt1120 or mipi\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_support_dev_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (dev_ctx->vo_enable == TD_TRUE) {
        vo_err_trace("vo device %d doesn't disabled!\n", dev);
        return OT_ERR_VO_NOT_DISABLE;
    }

    if (vo_check_bool_value(dev_param->vo_bypass_en) != TD_SUCCESS) {
        vo_err_trace("vo%d dev param vo_bypass_en %u is illegal!\n", dev, dev_param->vo_bypass_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_do_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_dev_param(dev, dev_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(&dev_ctx->dev_param, sizeof(ot_vo_dev_param), dev_param, sizeof(ot_vo_dev_param));
    vo_drv_set_dev_param(dev, dev_param);
    return TD_SUCCESS;
}

static td_s32 vo_do_get_dev_param(ot_vo_dev dev, ot_vo_dev_param *dev_param)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_support_dev_param(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(dev_param, sizeof(ot_vo_dev_param), &dev_ctx->dev_param, sizeof(ot_vo_dev_param));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_oper_dev_param(vo_ioctl_dev_param_param *ioctl_dev_param, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_dev_param->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_dev_param(ioctl_dev_param->ioctl_dev.dev, &(ioctl_dev_param->dev_param));
    } else {
        ret = vo_do_get_dev_param(ioctl_dev_param->ioctl_dev.dev, &(ioctl_dev_param->dev_param));
    }
    vo_up_sem();

    return ret;
}

#endif

#endif

#if vo_desc("frame rate")

static td_s32 vo_check_set_dev_frame_rate(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (dev_ctx->vo_enable == TD_TRUE) {
        vo_err_trace("vo %d doesn't disabled!\n", dev);
        return OT_ERR_VO_NOT_DISABLE;
    }

    if (dev_ctx->config == TD_FALSE) {
        vo_err_trace("vo %d doesn't set pub attr!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    if (dev_ctx->vou_attr.intf_sync != OT_VO_OUT_USER) {
        vo_err_trace("vo %d doesn't set user intf_sync!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    return TD_SUCCESS;
}

static td_s32 vo_do_set_dev_frame_rate(ot_vo_dev dev, td_u32 frame_rate)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if ((frame_rate <= 0) || (frame_rate > VO_DEV_MAX_FRAME_RATE)) {
        vo_err_trace("vo %d frame rate should be (0, %d] which is %u!\n", dev, VO_DEV_MAX_FRAME_RATE, frame_rate);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    ret = vo_check_set_dev_frame_rate(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    dev_ctx->full_frame_rate = frame_rate;
    return TD_SUCCESS;
}

td_void vo_do_get_dev_frame_rate(ot_vo_dev dev, td_u32 *frame_rate)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    *frame_rate = dev_ctx->full_frame_rate;
}

td_s32 vo_ioctl_dev_frame_rate(vo_ioctl_dev_frame_rate_param *ioctl_frame_rate, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_frame_rate->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_dev_frame_rate(ioctl_frame_rate->ioctl_dev.dev, ioctl_frame_rate->frame_rate);
    } else {
        vo_do_get_dev_frame_rate(ioctl_frame_rate->ioctl_dev.dev, &(ioctl_frame_rate->frame_rate));
    }
    vo_up_sem();

    return ret;
}

#endif

#if vo_desc("user sync info")
static td_s32 vo_check_dev_user_dev_div(ot_vo_dev dev, td_u32 dev_div)
{
    if ((dev_div < VO_MIN_DIV_MODE) || (dev_div > VO_MAX_DIV_MODE)) {
        vo_err_trace("vo(%d) dev div %u is illegal, it must be in [%u,%u].\n", dev, dev_div, VO_MIN_DIV_MODE,
            VO_MAX_DIV_MODE);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* 校验不同接口的设备分频比 */
    return vo_drv_check_intf_user_dev_div(dev, dev_div);
}

static td_s32 vo_check_intf_user_pre_div(ot_vo_dev dev, td_u32 pre_div)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    ot_vo_intf_type intf_type = dev_ctx->vou_attr.intf_type;

    if ((vo_drv_is_hdmi_intf(intf_type) != TD_TRUE) &&
        (vo_drv_is_hdmi1_intf(intf_type) != TD_TRUE)) {
        if (pre_div != VO_INTF_NO_HDMI_PRE_DIV_MODE) {
            vo_err_trace("vo(%d) pre div %u is illegal, it must be %u when intf type is %u.\n", dev, pre_div,
                VO_INTF_NO_HDMI_PRE_DIV_MODE, intf_type);
            return OT_ERR_VO_ILLEGAL_PARAM;
        }
    } else {
        if (vo_drv_is_support_hdmi_ssc_vdp_div() != TD_TRUE) {
            if (pre_div != VO_INTF_HDMI_PRE_DIV_MODE) {
                vo_err_trace("vo(%d) pre div %u is illegal, it must be %u when intf type is %u.\n", dev, pre_div,
                    VO_INTF_HDMI_PRE_DIV_MODE, intf_type);
                return OT_ERR_VO_ILLEGAL_PARAM;
            }
        }
    }

    return TD_SUCCESS;
}

td_s32 vo_check_dev_user_pre_div(ot_vo_dev dev, td_u32 pre_div)
{
    if ((pre_div < VO_MIN_PRE_DIV_MODE) || (pre_div > VO_MAX_PRE_DIV_MODE)) {
        vo_err_trace("vo(%d) pre div %u is illegal, it must be in [%u,%u].\n", dev, pre_div,
            VO_MIN_PRE_DIV_MODE, VO_MAX_PRE_DIV_MODE);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return vo_check_intf_user_pre_div(dev, pre_div);
}

td_s32 vo_check_dev_user_div(ot_vo_dev dev, td_u32 dev_div, td_u32 pre_div)
{
    td_s32 ret;

    ret = vo_check_dev_user_dev_div(dev, dev_div);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_dev_user_pre_div(dev, pre_div);
}

static td_s32 vo_check_dev_user_sync_info(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info)
{
    td_s32 ret;

    /* 检查时钟分频比是否正确 */
    ret = vo_check_dev_user_div(dev, sync_info->dev_div, sync_info->pre_div);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (vo_check_bool_value(sync_info->clk_reverse_en) != TD_SUCCESS) {
        vo_err_trace("vo(%d) clk_reverse_en %u is illegal!\n", dev, sync_info->clk_reverse_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* 时钟值是否合法，前面需要保证pre_div和dev_div的有效性 */
    return vo_drv_check_dev_clkvalue(dev, sync_info);
}

static td_void vo_get_user_info_config(ot_vo_dev dev, ot_vo_user_sync_info *user_sync_info)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    ot_vo_intf_sync intf_sync = dev_ctx->vou_attr.intf_sync;

    if (intf_sync == OT_VO_OUT_USER) {
        (td_void)memcpy_s(user_sync_info, sizeof(ot_vo_user_sync_info),
            &dev_ctx->vo_user_sync_info, sizeof(ot_vo_user_sync_info));
    } else {
        vo_drv_get_sync_attr_info(dev, &user_sync_info->user_sync_attr);
        vo_drv_get_dev_div_pre_div(dev, &user_sync_info->pre_div, &user_sync_info->dev_div);
        user_sync_info->clk_reverse_en = TD_TRUE;
    }
}

static td_s32 vo_check_dev_div_pre_div_cfg(td_u32 pre_div, td_u32 dev_div)
{
    if ((pre_div == 0) || (dev_div == 0)) {
        vo_err_trace("div number is invalid, pre_div %u dev_div %u\n", pre_div, dev_div);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 vo_get_dev_src_pixel_clk(ot_vo_dev dev, td_u32 *src_clk, td_u32 *pixel_clk)
{
    td_u32 pre_div = VO_DEFAULT_PRE_DIV;
    td_u32 dev_div = VO_DEFAULT_DEV_DIV;
    td_s32 ret;
    ot_vo_user_sync_info user_sync_info = {0};

    /* 物理设备才体现，非物理设备体现无效值 */
    if (vo_is_phy_dev(dev) != TD_TRUE) {
        return OT_ERR_VO_NOT_SUPPORT;
    }

    /* SD设备有固定时钟频点 */
    if (vo_drv_is_phy_dev_sd(dev)) {
        *src_clk = VO_SD_SRC_CLK;
        dev_div = VO_SD_DEV_DIV;
        *pixel_clk = *src_clk / (pre_div * dev_div);
        return TD_SUCCESS;
    }

    /* 针对HD条件下是否为用户时序进行相应配置 */
    vo_get_user_info_config(dev, &user_sync_info);
    ret = vo_drv_get_dev_src_clk_value(&user_sync_info.user_sync_attr, src_clk);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    pre_div = user_sync_info.pre_div;
    dev_div = user_sync_info.dev_div;
    ret = vo_check_dev_div_pre_div_cfg(pre_div, dev_div);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    *pixel_clk = *src_clk / (pre_div * dev_div);
    return TD_SUCCESS;
}

static td_s32 vo_check_do_user_sync_info(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev:%d is a not phy device, not support!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->config != TD_TRUE) {
        vo_err_trace("vo device %d doesn't configured!\n", dev);
        return OT_ERR_VO_NOT_CFG;
    }

    if (dev_ctx->vou_attr.intf_sync != OT_VO_OUT_USER) {
        vo_err_trace("vo device %d only support this in user sync.\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_set_user_sync_info(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_do_user_sync_info(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (dev_ctx->vo_enable == TD_TRUE) {
        vo_err_trace("vo device %d has enabled!\n", dev);
        return OT_ERR_VO_NOT_DISABLE;
    }

    ret = vo_check_dev_user_sync_info(dev, sync_info);
    if (ret != TD_SUCCESS) {
        vo_err_trace("VO%d's user sync info is illegal ret=0x%x!\n", dev, ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 vo_do_set_user_sync_info(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    td_ulong flags = 0;

    ret = vo_check_set_user_sync_info(dev, sync_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* 由于下面要配置寄存器，所以这里需要打开时钟 */
    vo_dev_spin_lock(&flags);
    vo_drv_set_all_crg_clk(TD_TRUE);
    vo_dev_spin_unlock(&flags);

    /* 设置pll 或者 lcd分频器时钟 */
    vo_drv_set_dev_user_intf_sync_attr(dev, sync_info);

    /* 设置分频数 */
    vo_drv_set_dev_div(dev, sync_info->dev_div);

    /* 设置HDMI 分频数 */
    vo_drv_set_hdmi_div(dev, sync_info->pre_div);

    /* 设置时钟相位 */
    vo_drv_set_clk_reverse(dev, sync_info->clk_reverse_en);

    (td_void)memcpy_s(&dev_ctx->vo_user_sync_info, sizeof(ot_vo_user_sync_info), sync_info,
        sizeof(ot_vo_user_sync_info));
    dev_ctx->user_config = TD_TRUE;
    return TD_SUCCESS;
}

td_s32 vo_do_get_user_sync_info(ot_vo_dev dev, ot_vo_user_sync_info *sync_info)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_do_user_sync_info(dev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(sync_info, sizeof(ot_vo_user_sync_info), &dev_ctx->vo_user_sync_info,
        sizeof(ot_vo_user_sync_info));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_user_sync_info(vo_ioctl_user_sync_info_param *ioctl_sync_info, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_sync_info->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_user_sync_info(ioctl_sync_info->ioctl_dev.dev, &(ioctl_sync_info->sync_info));
    } else {
        ret = vo_do_get_user_sync_info(ioctl_sync_info->ioctl_dev.dev, &(ioctl_sync_info->sync_info));
    }
    vo_up_sem();

    return ret;
}

#endif

#if vo_desc("module param")

static ot_vo_mod_param g_vo_mod_param = {
    .exit_dev_en = TD_TRUE,
    .dev_clk_ext_en = TD_FALSE,
    .vdac_detect_cycle = VO_LOADDETECT_DEFAULT_CYCLE,
    .vga_detect_en = TD_FALSE,
};

ot_vo_mod_param *vo_get_vo_mod_param(td_void)
{
    return &g_vo_mod_param;
}

td_bool vo_get_mod_param_exit_dev_en(td_void)
{
    ot_vo_mod_param *vo_mod_param = vo_get_vo_mod_param();
    return vo_mod_param->exit_dev_en;
}

td_bool vo_get_mod_param_dev_clk_ext_en(td_void)
{
    ot_vo_mod_param *vo_mod_param = vo_get_vo_mod_param();
    return vo_mod_param->dev_clk_ext_en;
}

static td_s32 vo_check_vdac_detect_cycle(td_u32 vdac_detect_cycle)
{
    if ((vdac_detect_cycle < VO_LOADDETECT_MIN_CYCLE) && (vdac_detect_cycle != 0)) {
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_mod_param_phy_dev_enabled(td_void)
{
    ot_vo_dev dev;
    vo_dev_info *dev_ctx = TD_NULL;

    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        dev_ctx = vo_get_dev_ctx(dev);
        if (dev_ctx == TD_NULL) {
            continue;
        }

        if (dev_ctx->vo_enable == TD_TRUE) {
            vo_err_trace("vo device %d has enabled!\n", dev);
            return OT_ERR_VO_NOT_DISABLE;
        }
    }

    return TD_SUCCESS;
}
static td_s32 vo_check_vga_detect_en(td_bool vga_detect_en)
{
    if (vo_check_bool_value(vga_detect_en) != TD_SUCCESS) {
        vo_err_trace("the vga_detect_en %u is illegal!\n", vga_detect_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

#ifndef CONFIG_OT_VO_VGA
    if (vga_detect_en != TD_FALSE) {
        vo_err_trace("the vga_detect_en %u is not support!\n", vga_detect_en);
        return OT_ERR_VO_NOT_SUPPORT;
    }
#endif

    return TD_SUCCESS;
}

static td_s32 vo_check_mod_param(const ot_vo_mod_param *mod_param)
{
    td_s32 ret;

    if (vo_check_bool_value(mod_param->exit_dev_en) != TD_SUCCESS) {
        vo_err_trace("the exit_dev_en %u is illegal!\n", mod_param->exit_dev_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

#ifdef CONFIG_OT_VO_EXPORT_FUNCTION
    if (vo_check_bool_value(mod_param->dev_clk_ext_en) != TD_SUCCESS) {
        vo_err_trace("the dev_clk_ext_en %u is illegal!\n", mod_param->dev_clk_ext_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }
#else
    if (mod_param->dev_clk_ext_en != TD_FALSE) {
        vo_err_trace("the dev_clk_ext_en %u is not support!\n", mod_param->dev_clk_ext_en);
        return OT_ERR_VO_NOT_SUPPORT;
    }
#endif

    ret = vo_check_vga_detect_en(mod_param->vga_detect_en);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (vo_check_vdac_detect_cycle(mod_param->vdac_detect_cycle) != TD_SUCCESS) {
        vo_err_trace("the vdac_detect_cycle %u is illegal!\n", mod_param->vdac_detect_cycle);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_do_set_mod_param(const ot_vo_mod_param *mod_param)
{
    td_s32 ret;
    ot_vo_mod_param *vo_mod_param = vo_get_vo_mod_param();

    ret = vo_check_mod_param(mod_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* 物理设备使能时不允许修改模块参数 */
    ret = vo_check_mod_param_phy_dev_enabled();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(vo_mod_param, sizeof(ot_vo_mod_param), mod_param, sizeof(ot_vo_mod_param));
    return TD_SUCCESS;
}

static td_void vo_do_get_mod_param(ot_vo_mod_param *mod_param)
{
    ot_vo_mod_param *vo_mod_param = vo_get_vo_mod_param();

    (td_void)memcpy_s(mod_param, sizeof(ot_vo_mod_param), vo_mod_param, sizeof(ot_vo_mod_param));
}

td_s32 vo_ioctl_mod_param(ot_vo_mod_param *mod_param, vo_ioctl_type ioctl_type)
{
    td_s32 ret = TD_SUCCESS; /* 需要初始化 */

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_mod_param(mod_param);
    } else {
        vo_do_get_mod_param(mod_param);
    }

    vo_up_sem();
    return ret;
}

#endif

#if vo_desc("vtth")

static td_s32 vo_check_set_vtth(ot_vo_dev dev, td_u32 vtth)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev(%d) is not phy device, not support set vtth!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    if (dev_ctx->vo_enable == TD_TRUE) {
        vo_err_trace("can't set dev(%d)'s vtth after vo enabled!\n", dev);
        return OT_ERR_VO_NOT_DISABLE;
    }

    if ((vtth < VO_MIN_VTTH_WATERLINE) || (vtth > VO_MAX_VTTH_WATERLINE)) {
        vo_err_trace("dev(%d) vtth %u illegal, should be [%u, %u]\n", dev, vtth,
            VO_MIN_VTTH_WATERLINE, VO_MAX_VTTH_WATERLINE);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_do_set_vtth(ot_vo_dev dev, td_u32 vtth)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_vtth(dev, vtth);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    vo_drv_set_dev_vtth(dev, vtth);
    dev_ctx->vtth1_config = TD_TRUE;
    return TD_SUCCESS;
}

static td_s32 vo_do_get_vtth(ot_vo_dev dev, td_u32 *vtth)
{
    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev(%d) is not phy device, not support get vtth!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    *vtth = vo_drv_get_dev_vtth(dev);
    return TD_SUCCESS;
}

td_s32 vo_ioctl_vtth(vo_ioctl_vtth_param *ioctl_vtth, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_vtth->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_vtth(ioctl_vtth->ioctl_dev.dev, ioctl_vtth->vtth);
    } else {
        ret = vo_do_get_vtth(ioctl_vtth->ioctl_dev.dev, &(ioctl_vtth->vtth));
    }
    vo_up_sem();

    return ret;
}

#endif

#if vo_desc("less buf attr")

#ifdef CONFIG_OT_VO_LESS_BUF

static td_s32 vo_check_less_buf_attr(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr)
{
    if (vo_check_bool_value(less_buf_attr->enable) != TD_SUCCESS) {
        vo_err_trace("dev(%d) less buf enable(%u) is illegal!\n", dev, less_buf_attr->enable);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* 增加最大值检查，vtth2 < vtth1 在 enable接口进行检查 */
    if ((less_buf_attr->vtth < VO_MIN_VTTH_WATERLINE) || (less_buf_attr->vtth > VO_MAX_VTTH_WATERLINE)) {
        vo_err_trace("dev(%d) less buf vtth(%u) should be [min_vtth, max_vtth][%u, %u]\n",
            dev, less_buf_attr->vtth, VO_MIN_VTTH_WATERLINE, VO_MAX_VTTH_WATERLINE);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_set_less_buf_attr(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    td_s32 ret;

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev(%d) is not phy device, not support set less buf attr!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    /* 设备使能后不允许变更省buf标记,支持配置动态修改下省buf的vtth的值，用于省BUF场景动态配置 */
    if ((vo_is_dev_enabled(dev) == TD_TRUE) &&
        (dev_ctx->less_buf_attr.enable != less_buf_attr->enable)) {
        vo_err_trace("can't change dev(%d)'s less buf enable(old: %u, new: %u) after vo enabled!\n",
            dev, dev_ctx->less_buf_attr.enable, less_buf_attr->enable);
        return OT_ERR_VO_NOT_DISABLE;
    }

    ret = vo_drv_check_less_buf(dev, less_buf_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_less_buf_attr(dev, less_buf_attr);
}

static td_s32 vo_do_set_less_buf_attr(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_less_buf_attr(dev, less_buf_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(&dev_ctx->less_buf_attr, sizeof(ot_vo_less_buf_attr), less_buf_attr, sizeof(ot_vo_less_buf_attr));
    vo_drv_set_dev_less_buf_enable(dev, less_buf_attr->enable);

    if (less_buf_attr->enable == TD_FALSE) {
        return TD_SUCCESS;
    }

    vo_drv_set_dev_vtth2(dev, less_buf_attr->vtth);
    /* 设备已使能后重新配置VTTH2时需要重新刷新配置 */
    vo_drv_refresh_dev_vtth2(dev);
    return TD_SUCCESS;
}

static td_s32 vo_do_get_less_buf_attr(ot_vo_dev dev, ot_vo_less_buf_attr *less_buf_attr)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev(%d) is not phy device, not support get less buf attr!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    (td_void)memcpy_s(less_buf_attr, sizeof(ot_vo_less_buf_attr), &dev_ctx->less_buf_attr, sizeof(ot_vo_less_buf_attr));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_less_buf_attr(vo_ioctl_less_buf_attr_param *ioctl_less_buf, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_less_buf->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_less_buf_attr(ioctl_less_buf->ioctl_dev.dev, &(ioctl_less_buf->less_buf_attr));
    } else {
        ret = vo_do_get_less_buf_attr(ioctl_less_buf->ioctl_dev.dev, &(ioctl_less_buf->less_buf_attr));
    }
    vo_up_sem();

    return ret;
}

#endif

#endif

#if vo_desc("user notify attr")

#ifdef CONFIG_OT_VO_USER_NOTIFY
static td_s32 vo_check_user_notify_attr(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr)
{
    if (vo_check_bool_value(user_notify_attr->enable) != TD_SUCCESS) {
        vo_err_trace("dev(%d) user notify enable(%u) is illegal!\n", dev, user_notify_attr->enable);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    /* 增加最大值检查，vtth2 < vtth1 在 enable接口进行检查 */
    if ((user_notify_attr->vtth < VO_MIN_VTTH_WATERLINE) || (user_notify_attr->vtth > VO_MAX_VTTH_WATERLINE)) {
        vo_err_trace("dev(%d) user notify vtth(%u) should be [min_vtth, max_vtth][%u, %u]\n",
            dev, user_notify_attr->vtth, VO_MIN_VTTH_WATERLINE, VO_MAX_VTTH_WATERLINE);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_set_user_notify_attr(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    td_s32 ret;

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev(%d) is not phy device, not support set user notify attr!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    /* 设备使能后不允许变更省buf标记,支持配置动态修改下省buf的vtth的值，用于通知用户场景动态配置 */
    if ((vo_is_dev_enabled(dev) == TD_TRUE) &&
        (dev_ctx->user_notify_attr.enable != user_notify_attr->enable)) {
        vo_err_trace("can't change dev(%d)'s user notify enable(old: %u, new: %u) after vo enabled!\n",
            dev, dev_ctx->user_notify_attr.enable, user_notify_attr->enable);
        return OT_ERR_VO_NOT_DISABLE;
    }

    ret = vo_drv_check_user_notify(dev, user_notify_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vo_check_user_notify_attr(dev, user_notify_attr);
}

static td_s32 vo_do_set_user_notify_attr(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr)
{
    td_s32 ret;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    ret = vo_check_set_user_notify_attr(dev, user_notify_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(&dev_ctx->user_notify_attr, sizeof(ot_vo_user_notify_attr),
        user_notify_attr, sizeof(ot_vo_user_notify_attr));
    vo_drv_set_dev_user_notify_enable(dev, user_notify_attr->enable);

    if (user_notify_attr->enable == TD_FALSE) {
        return TD_SUCCESS;
    }

    vo_drv_set_dev_vtth2(dev, user_notify_attr->vtth);
    /* 设备已使能后重新配置VTTH2时需要重新刷新配置 */
    vo_drv_refresh_dev_vtth2(dev);
    return TD_SUCCESS;
}

static td_s32 vo_do_get_user_notify_attr(ot_vo_dev dev, ot_vo_user_notify_attr *user_notify_attr)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);

    if (vo_is_phy_dev(dev) != TD_TRUE) {
        vo_err_trace("dev(%d) is not phy device, not support get user notify attr!\n", dev);
        return OT_ERR_VO_NOT_SUPPORT;
    }

    (td_void)memcpy_s(user_notify_attr, sizeof(user_notify_attr), &dev_ctx->user_notify_attr, sizeof(user_notify_attr));
    return TD_SUCCESS;
}

td_s32 vo_ioctl_user_notify_attr(vo_ioctl_user_notify_attr_param *ioctl_user_notify, vo_ioctl_type ioctl_type)
{
    td_s32 ret;

    ret = vo_check_ioctl_dev(&ioctl_user_notify->ioctl_dev);
    if (ret != TD_SUCCESS) {
        vo_err_trace("dev is invalid\n");
        return ret;
    }

    vo_down_sem_return();
    if (ioctl_type == VO_IOCTL_TYPE_SET) {
        ret = vo_do_set_user_notify_attr(ioctl_user_notify->ioctl_dev.dev, &(ioctl_user_notify->user_notify_attr));
    } else {
        ret = vo_do_get_user_notify_attr(ioctl_user_notify->ioctl_dev.dev, &(ioctl_user_notify->user_notify_attr));
    }
    vo_up_sem();

    return ret;
}
#endif

#endif

#if vo_desc("interrupt time")

static td_void vo_calculate_int_rate(ot_vo_dev dev, td_u64 cur_time)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    vo_debug_info *dbg_info = &dev_ctx->debug_info;

    if (dbg_info->last_rate_time == 0) {
        dbg_info->last_rate_time = cur_time;
        dbg_info->int_count = 0;
    }
    if (cur_time - dbg_info->last_rate_time >= VO_TIME_TEN_SECONDS_UNIT_MICROSECOND) {
        dbg_info->int_rate = osal_div64_u64((VO_TIME_ONE_SECOND_UNIT_MICROSECOND * dbg_info->int_count),
            (cur_time - dbg_info->last_rate_time));
        dbg_info->last_rate_time = cur_time;
        dbg_info->int_count = 0;
    }

    dbg_info->int_count++;
}

static td_void vo_calculate_int_time_per_min(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    vo_debug_info *debug_info = &dev_ctx->debug_info;

    debug_info->int_tmp++;
    if (dev_ctx->full_frame_rate != 0) {
        debug_info->int_tmp %= dev_ctx->full_frame_rate;
    }

    if (debug_info->int_tmp == 0) {
        debug_info->int_time_per_min = debug_info->int_time_per_min_tmp;
        debug_info->int_time_per_min_tmp = 0;
    } else {
        debug_info->int_time_per_min_tmp += debug_info->int_time;
    }
}

td_void vo_statistics_interrupt_time(ot_vo_dev dev, td_u64 time_start)
{
    td_u64 time_end;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    vo_debug_info *debug_info = TD_NULL;

    /* 开机画面平滑场景时会打开中断，此时上下文不存在，属于正常场景，返回不处理 */
    if (dev_ctx == TD_NULL) {
        return;
    }

    debug_info = &dev_ctx->debug_info;
    time_end = call_sys_get_time_stamp();

    vo_calculate_int_rate(dev, time_end);

    debug_info->int_time = (td_u32)(time_end - time_start);

    if (debug_info->int_time > debug_info->max_int_time) {
        debug_info->max_int_time = debug_info->int_time;
    }
    vo_calculate_int_time_per_min(dev);

    if (debug_info->last_int_time == 0) {
        /* 第一次中断时，此次中断与上次中断的间隔无效，设置为0，不需要统计 */
        debug_info->int_gap_time = 0;
    } else {
        debug_info->int_gap_time = (td_u32)(time_start - debug_info->last_int_time);
    }
    debug_info->last_int_time = time_start;

    if (debug_info->int_gap_time > debug_info->max_int_gap_time) {
        debug_info->max_int_gap_time = debug_info->int_gap_time;
    }

    /* 120000000表示两分钟更新一次中断最大时间间隔和中断处理最大占用时间 */
    if (((td_u32)(time_start - debug_info->last_time)) >= 120000000) {  /* 120000000: 两分钟 */
        debug_info->max_int_time = 0;
        debug_info->max_int_gap_time = 0;
        debug_info->last_time = time_start;
    }
}

#endif
