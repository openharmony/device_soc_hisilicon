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
#ifndef MKP_VO_COMM_H
#define MKP_VO_COMM_H

#include "ot_common_video.h"
#include "ot_common_vo.h"
#include "mkp_vo.h"

#if vo_desc("UBOOT_VO")

#define VO_RECT_INTERLACED_ALIGN  4

#if vo_desc("check attr pub")
td_s32 vo_check_bool_value(td_bool bool_val);
td_s32 vo_check_rect_align(const ot_rect *rect);
td_s32 vo_check_csc_no_matrix(const ot_vo_csc *csc);
#endif

td_s32 vo_def_check_dev_id(ot_vo_dev dev);

#if vo_desc("check dev pub")
td_bool vo_is_phy_dev(ot_vo_dev dev);
td_bool vo_is_typical_intf_sync(ot_vo_intf_sync intf_sync);
#endif

/* macro definition for check status */
#define vo_check_null_ptr_return(ptr)                              \
    do {                                                           \
        if ((ptr) == TD_NULL) {                                    \
            vo_warn_trace("null ptr!\n");                          \
            return OT_ERR_VO_NULL_PTR;                             \
        }                                                          \
    } while (0)

/* vo device relative */
#define vo_check_dev_id_return(dev)                         \
    do {                                                    \
        if (vo_drv_check_dev_id(dev) != TD_SUCCESS) {       \
            vo_err_trace("dev %d invalid!\n", dev);         \
            return OT_ERR_VO_INVALID_DEV_ID;                \
        }                                                   \
    } while (0)

/* vo video relative */
#define vo_check_layer_id_return(layer)                     \
    do {                                                    \
        if (vo_drv_check_layer_id(layer) != TD_SUCCESS) {   \
            vo_err_trace("layer %d invalid!\n", layer);     \
            return OT_ERR_VO_INVALID_LAYER_ID;              \
        }                                                   \
    } while (0)

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

td_bool vo_is_cas_dev(ot_vo_dev dev);
td_bool vo_is_valid_intf_sync(ot_vo_intf_sync intf_sync);

#if vo_desc("check event interruptible")
td_s32 vo_check_event_timeout_interruptible_ret(td_s32 ret);
td_s32 vo_check_event_interruptible_ret(td_s32 ret);
#endif

#define vo_debug_print_vcnt(dev, format, args...)                             \
    do {                                                                      \
        td_u32 __vcnt = 0;                                                    \
        vou_drv_get_dev_int_state_vcnt(dev, &__vcnt);                         \
        vo_debug_trace("dev %d cnt %d in " format "\n", dev, __vcnt, ##args); \
    } while (0)

/* vo channel relative */
#define vo_check_chn_id_return(layer, chn)                          \
    do {                                                            \
        if (vo_drv_check_chn_id(chn) != TD_SUCCESS) {               \
            vo_err_trace("layer %d invalid chn %d!\n", layer, chn); \
            return OT_ERR_VO_INVALID_CHN_ID;                        \
        }                                                           \
    } while (0)

#define vo_check_cas_dev_return(dev)                                      \
    do {                                                                  \
        if (vo_is_cas_dev(dev) == TD_TRUE) {                              \
            vo_err_trace("vo dev %d is cas device, not support!\n", dev); \
            return OT_ERR_VO_NOT_SUPPORT;                                 \
        }                                                                 \
    } while (0)

#define vo_check_no_cas_dev_return(dev)                                       \
    do {                                                                      \
        if (vo_is_cas_dev(dev) != TD_TRUE) {                                  \
            vo_err_trace("vo dev %d is not cas device, not support!\n", dev); \
            return OT_ERR_VO_NOT_SUPPORT;                                     \
        }                                                                     \
    } while (0)

#endif /* #if vo_desc("KERNEL_VO") */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef MKP_VO_COMM_H */

