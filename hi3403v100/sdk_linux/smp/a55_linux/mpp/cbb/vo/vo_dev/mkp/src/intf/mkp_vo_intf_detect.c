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
#include "mkp_vo_intf_detect.h"
#include "vou.h"
#include "mkp_vo_init.h"
#include "drv_vo_dev.h"

#if vo_desc("interface status")

#if defined(CONFIG_OT_VO_VGA) || defined(CONFIG_OT_VO_CVBS)

static td_s32 vo_check_query_intf_type(ot_vo_intf_type intf_type)
{
    if (vo_drv_is_cvbs_intf(intf_type) && vo_drv_is_vga_intf(intf_type)) {
        vo_err_trace("vo query intf type %u is illegal, both cvbs and vga is illegal!\n", intf_type);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_dev_intf_type(ot_vo_dev dev, ot_vo_intf_type intf_type)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (((intf_type == OT_VO_INTF_CVBS) && (vo_drv_is_cvbs_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE)) ||
        ((intf_type == OT_VO_INTF_VGA) && (vo_drv_is_vga_intf(dev_ctx->vou_attr.intf_type) != TD_TRUE))) {
        vo_info_trace("vo dev %d, intf type %u, not cvbs/vga!\n", dev, dev_ctx->vou_attr.intf_type);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_check_query_intf_detect_status(ot_vo_dev dev)
{
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx == TD_NULL) {
        vo_info_trace("vo dev%d: is not enable!\n", dev);
        return OT_ERR_VO_NOT_ENABLE;
    }

    if (dev_ctx->load_detect_info.load_detect_enable != TD_TRUE) {
        vo_err_trace("vo dev %d not enable detect!\n", dev);
        return OT_ERR_VO_NOT_ENABLE;
    }

    if (dev_ctx->load_detect_info.is_detected == TD_TRUE) {
        vo_err_trace("vo dev %d now is detecting!\n", dev);
        return OT_ERR_VO_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 vo_get_query_intf_type_dev(ot_vo_intf_type intf_type, ot_vo_dev *intf_dev)
{
    td_s32 ret;
    ot_vo_dev dev;
    td_u32 dev_disable_num = 0;
    vo_dev_info *dev_ctx = TD_NULL;

    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        /* 需要先reset，根据模块参数vdac_detect_cycle来控制load_detect_enable */
        vo_reset_load_detect(dev);
        dev_ctx = vo_get_dev_ctx(dev);
        if ((dev_ctx == TD_NULL) || (dev_ctx->vo_enable != TD_TRUE)) {
            vo_info_trace("vo dev%d: ctx is null or not enable(%d)!\n",
                dev, (dev_ctx == TD_NULL) ? (-1) : (td_s32)dev_ctx->vo_enable);
            dev_disable_num++;
            continue;
        }

        ret = vo_check_dev_intf_type(dev, intf_type);
        if (ret != TD_SUCCESS) {
            continue;
        }

        /* 一个接口类型当前只会配置在一个设备上 */
        *intf_dev = dev;
        return TD_SUCCESS;
    }

    if (dev_disable_num == OT_VO_MAX_PHYS_DEV_NUM) {
        vo_err_trace("all phy dev is disabled!\n");
        return OT_ERR_VO_NOT_ENABLE;
    }

    vo_err_trace("enabled phy dev is not intf type (%u)(cvbs/vga)!\n", intf_type);
    return OT_ERR_VO_ILLEGAL_PARAM;
}

static td_s32 vo_check_vga_detect_support(td_void)
{
    ot_vo_mod_param *vo_mod_param = vo_get_vo_mod_param();

    if (vo_mod_param->vga_detect_en == TD_FALSE) {
        vo_err_trace("mod param vga_detect_en false don't support vga detect!\n");
        return OT_ERR_VO_NOT_SUPPORT;
    }

    return TD_SUCCESS;
}

static ot_vo_intf_plug_status vo_get_intf_plug_status(ot_vo_dev dev)
{
    if (vou_drv_get_dev_intf_detect_status(dev) == TD_TRUE) {
        return OT_VO_INTF_STATUS_PLUG;
    }

    return OT_VO_INTF_STATUS_NO_PLUG;
}

static td_s32 vo_query_intf_status_wait_callback(const td_void *param)
{
    vo_dev_info *dev_ctx = (vo_dev_info *)param;
    td_s32 condition;

    condition = (td_s32)(dev_ctx->load_detect_info.int_user_count == dev_ctx->load_detect_info.int_cur_count);
    return condition;
}

static td_s32 vo_query_pub_intf_status(vo_intf_status_info *intf_info, ot_vo_intf_type intf_type)
{
    td_s32 ret;
    td_s32 ret_int;
    ot_vo_dev dev;
    td_ulong flags = 0;
    vo_dev_info *dev_ctx = TD_NULL;

    intf_info->intf_status.plug_status = OT_VO_INTF_STATUS_NO_PLUG;

    vo_down_sem_return();
    ret = vo_get_query_intf_type_dev(intf_type, &dev);
    if (ret != TD_SUCCESS) {
        vo_up_sem();
        return ret;
    }

    ret = vo_check_query_intf_detect_status(dev);
    if (ret != TD_SUCCESS) {
        vo_up_sem();
        return ret;
    }

    vo_dev_spin_lock(&flags);
    dev_ctx = vo_get_dev_ctx(dev);
    dev_ctx->load_detect_info.is_detected = TD_TRUE;
    dev_ctx->load_detect_info.is_query_intf_status = TD_TRUE;
    vo_dev_spin_unlock(&flags);

    vo_up_sem();
    ret_int = osal_wait_event_timeout_uninterruptible(&dev_ctx->load_detect_info.vo_wait_query_intf_status,
        vo_query_intf_status_wait_callback, dev_ctx, (VO_QUERY_INTF_STATUS_WAIT_TIMEOUT));

    if (osal_down_interruptible(vo_get_dev_sema()) != 0) {
        vo_dev_spin_lock(&flags);
        dev_ctx->load_detect_info.is_detected = TD_FALSE;
        dev_ctx->load_detect_info.is_query_intf_status = TD_FALSE;
        vo_dev_spin_unlock(&flags);
        return -ERESTARTSYS;
    }

    ret = vo_check_event_timeout_interruptible_ret(ret_int);
    vo_dev_spin_lock(&flags);
    if (ret == TD_SUCCESS) {
        intf_info->intf_status.plug_status = vo_get_intf_plug_status(dev);
    }

    dev_ctx->load_detect_info.is_detected = TD_FALSE;
    dev_ctx->load_detect_info.is_query_intf_status = TD_FALSE;
    vo_dev_spin_unlock(&flags);
    vo_up_sem();

    return ret;
}

td_s32 vo_query_intf_status(vo_intf_status_info *intf_info)
{
    if (vo_check_query_intf_type(intf_info->intf_type) != TD_SUCCESS) {
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (vo_drv_is_cvbs_intf(intf_info->intf_type)) {
        return vo_query_pub_intf_status(intf_info, OT_VO_INTF_CVBS);
    }

    if (vo_drv_is_vga_intf(intf_info->intf_type)) {
        if (vo_check_vga_detect_support() != TD_SUCCESS) {
            return OT_ERR_VO_NOT_SUPPORT;
        }
        return vo_query_pub_intf_status(intf_info, OT_VO_INTF_VGA);
    }

    vo_err_trace("intf type %u, not support query intf status!\n", intf_info->intf_type);
    intf_info->intf_status.plug_status = OT_VO_INTF_STATUS_NO_PLUG;
    return OT_ERR_VO_NOT_SUPPORT;
}

#endif

#endif

#if vo_desc("load detect")

static td_bool vo_get_load_detect_enable(td_u32 vdac_detect_cycle)
{
    if (vdac_detect_cycle == 0) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_void vo_init_load_detect_pub(ot_vo_dev dev, td_bool is_reset)
{
    vou_load_detect_info *detect_info = TD_NULL;
    ot_vo_mod_param *vo_mod_param = vo_get_vo_mod_param();
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx == TD_NULL) {
        return;
    }

    /* 查询当前设备上面是否有接口支持负载检测 */
    if (vo_drv_is_dev_support_load_detect(dev) != TD_TRUE) {
        return;
    }

    detect_info = &dev_ctx->load_detect_info;
    detect_info->load_detect_enable = vo_get_load_detect_enable(vo_mod_param->vdac_detect_cycle);
    if (detect_info->load_detect_enable != TD_TRUE) {
        return;
    }

    detect_info->int_max_count = vo_mod_param->vdac_detect_cycle;
    detect_info->int_cur_count = detect_info->int_max_count;
    detect_info->int_user_count = detect_info->int_max_count;
    detect_info->is_query_intf_status = TD_FALSE;

    /* 复位负载检测信息和初始化负载检测信息有差异 */
    if (is_reset) {
        vo_drv_load_detect_reset(dev);
    } else {
        vo_drv_load_detect_init(dev, vo_mod_param->vga_detect_en);
    }
}

td_void vo_reset_load_detect(ot_vo_dev dev)
{
    vo_init_load_detect_pub(dev, TD_TRUE);
}

td_void vo_init_load_detect(ot_vo_dev dev)
{
    vo_init_load_detect_pub(dev, TD_FALSE);
}

td_void vo_deinit_load_detect(ot_vo_dev dev)
{
    vou_load_detect_info *detect_info = TD_NULL;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx == TD_NULL) {
        return;
    }

    /* 查询当前设备上面是否有接口支持负载检测 */
    if (vo_drv_is_dev_support_load_detect(dev) != TD_TRUE) {
        return;
    }

    detect_info = &dev_ctx->load_detect_info;
    if (detect_info->load_detect_enable != TD_TRUE) {
        return;
    }

    detect_info->load_detect_enable = TD_FALSE;
    detect_info->int_max_count = 0;
    detect_info->int_cur_count = 0;
    vo_drv_load_detect_de_init(dev);
}

td_void vo_load_detect(ot_vo_dev dev)
{
    vou_load_detect_info *detect_info = TD_NULL;
    vo_dev_info *dev_ctx = vo_get_dev_ctx(dev);
    if (dev_ctx == TD_NULL) {
        return;
    }

    detect_info = &dev_ctx->load_detect_info;
    if (detect_info->load_detect_enable == TD_FALSE) {
        return;
    }

    /* 每个检测周期的第2个中断处理检测结果 */
    if (detect_info->int_cur_count == detect_info->int_max_count) {
        vo_drv_enable_load_detect(dev);
    } else if (detect_info->int_cur_count == (detect_info->int_max_count - 2)) {  /* 2: 第2个中断时处理 */
        vo_drv_disable_load_detect(dev);
        detect_info->int_user_count = detect_info->int_cur_count;
        if (detect_info->is_query_intf_status == TD_TRUE) {
            osal_wakeup(&detect_info->vo_wait_query_intf_status);
        }
    }

    detect_info->int_cur_count--;
    if (detect_info->int_cur_count == 0) {
        detect_info->int_cur_count = detect_info->int_max_count;
    }
}

#endif

