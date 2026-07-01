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
#include "mkp_vo_comm.h"
#include "vou.h"
#include "mkp_vo_init.h"

#if vo_desc("UBOOT_VO")
#if vo_desc("dev check")

td_s32 vo_def_check_dev_id(ot_vo_dev dev)
{
    if ((dev < 0) || (dev >= OT_VO_MAX_DEV_NUM)) {
        return OT_ERR_VO_INVALID_DEV_ID;
    }

    return TD_SUCCESS;
}

#endif

#if vo_desc("check attr pub")

td_s32 vo_check_bool_value(td_bool bool_val)
{
    if ((bool_val != TD_TRUE) && (bool_val != TD_FALSE)) {
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 vo_check_rect_align(const ot_rect *rect)
{
    if ((rect->x % VO_RECT_ALIGN) || (rect->y % VO_RECT_ALIGN) ||
        (rect->width % VO_RECT_ALIGN) || (rect->height % VO_RECT_ALIGN)) {
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}
#endif

#if vo_desc("check dev pub")

td_bool vo_is_phy_dev(ot_vo_dev dev)
{
    if (vo_def_check_dev_id(dev) != TD_SUCCESS) {
        vo_err_trace("vo dev %d is invalid!\n", dev);
        return TD_FALSE;
    }

    if (vo_drv_is_virt_dev(dev) || vo_drv_is_cas_dev(dev)) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool vo_is_typical_intf_sync(ot_vo_intf_sync intf_sync)
{
    if ((intf_sync < OT_VO_OUT_BUTT) && (intf_sync != OT_VO_OUT_USER)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

#endif
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("check attr pub")

td_s32 vo_check_csc_no_matrix(const ot_vo_csc *csc)
{
    if (csc->luma > VO_CSC_LUMA_MAX) {
        vo_err_trace("vo csc luma value %u out of range!\n", csc->luma);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (csc->contrast > VO_CSC_CONT_MAX) {
        vo_err_trace("vo csc contrast value %u out of range!\n", csc->contrast);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (csc->hue > VO_CSC_HUE_MAX) {
        vo_err_trace("vo csc hue value %u out of range!\n", csc->hue);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (csc->saturation > VO_CSC_SAT_MAX) {
        vo_err_trace("vo csc saturation value %u out of range!\n", csc->saturation);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    if (vo_check_bool_value(csc->ex_csc_en) != TD_SUCCESS) {
        vo_err_trace("vo csc ext csc switch %u out of range!\n", csc->ex_csc_en);
        return OT_ERR_VO_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

#endif

#if vo_desc("check dev pub")

td_bool vo_is_cas_dev(ot_vo_dev dev)
{
    if (vo_def_check_dev_id(dev) != TD_SUCCESS) {
        vo_err_trace("vo dev %d is invalid!\n", dev);
        return TD_FALSE;
    }

    return vo_drv_is_cas_dev(dev);
}

#endif

#if vo_desc("check get frame")

static td_s32 vo_check_event_vo_state(td_void)
{
    if (vo_is_vo_started() != TD_TRUE) {
        vo_err_trace("event interruptible error: vo is not in STARTED state.\n");
        return OT_ERR_VO_NOT_READY;
    }

    return TD_SUCCESS;
}

td_s32 vo_check_event_timeout_interruptible_ret(td_s32 ret)
{
    if (vo_check_event_vo_state() != TD_SUCCESS) {
        return OT_ERR_VO_NOT_READY;
    }

    if (ret < 0) {
        return -ERESTARTSYS;
    }

    if (ret == 0) {
        vo_err_trace("event interruptible error: time out!\n");
        return OT_ERR_VO_TIMEOUT;
    }

    return TD_SUCCESS;
}

td_s32 vo_check_event_interruptible_ret(td_s32 ret)
{
    if (vo_check_event_vo_state() != TD_SUCCESS) {
        return OT_ERR_VO_NOT_READY;
    }

    if (ret != TD_SUCCESS) {
        vo_err_trace("event interruptible error: interrupted by signal!\n");
        return -ERESTARTSYS;
    }

    return TD_SUCCESS;
}

#endif

#endif /* #if vo_desc("KERNEL_VO") */
