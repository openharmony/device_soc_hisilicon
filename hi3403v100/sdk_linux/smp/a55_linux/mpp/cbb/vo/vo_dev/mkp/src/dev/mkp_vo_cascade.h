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
#ifndef MKP_VO_CASCADE_H
#define MKP_VO_CASCADE_H

#include "ot_common_video.h"
#include "ot_common_vo.h"
#include "mkp_vo.h"
#include "mkp_vo_ioctl.h"
#include "hal_vo_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#ifdef CONFIG_OT_VO_CASCADE

#define VO_CAS_DUAL_MODE_RATIO  2

typedef enum {
    VO_CAS_MRG_STATUS_INIT = 0,         /* 初始化状态 */
    VO_CAS_MRG_STATUS_FIRST,            /* 第一次配置时 */
    VO_CAS_MRG_STATUS_UPDATE_SAME,      /* 刷新一样 */
    VO_CAS_MRG_STATUS_UPDATE_NO_SAME,   /* 刷新不一样 */

    VO_CAS_MRG_STATUS_BUTT,
} vo_cas_mrg_status;

typedef enum {
    VO_CAS_MRG_PARA_UP_INIT = 0,        /* 初始化状态 */
    VO_CAS_MRG_PARA_UP_SLAVE_MASTER,    /* 从片主模式状态 */
    VO_CAS_MRG_PARA_UP_PLL,             /* PLL配置高帧率状态 */
    VO_CAS_MRG_PARA_UP_REFRESH1,         /* 刷新系数状态 */
    VO_CAS_MRG_PARA_UP_REFRESH2,         /* 刷新系数状态 */
    VO_CAS_MRG_PARA_UP_REFRESH3,         /* 刷新系数状态 */

    VO_CAS_MRG_PARA_UP_BUTT,
} vo_cas_mrg_para_up;

typedef struct {
    vo_cas_mrg_status mrg_status;   /* 多区域RECT配置状态 */
    ot_rect last_rect;  /* 区域上一个配置 */
    ot_rect rect;       /* 区域当前新配置 */
} vo_cas_mrg_rect_ctx;

typedef struct {
    td_bool mrg_rect_valid;          /* 多区域配置有效标记 */
    vo_cas_mrg_para_up mrg_para_up;   /* 多区域PARA UP刷新状态 */
    vo_cas_mrg_rect_ctx  mrg_rect[MAX_REGION_NUM];  /* 多区域配置 */
} vo_cas_mrg_rect_cfg;

td_bool vo_is_cas_layer(ot_vo_layer layer);
td_void vo_cas_hd_disp_chn_display_rect(ot_vo_layer layer, ot_vo_chn chn);
td_void vo_set_cas_layer_multi_rgn_rect(ot_vo_layer layer, td_s32 area_id, const ot_rect *rect);
td_s32 vo_notify_cas_check_set_pub_attr(ot_vo_dev phy_dev, const ot_vo_pub_attr *pub_attr);
td_s32 vo_notify_cas_check_enable_dev(ot_vo_dev phy_dev);
td_void vo_notify_cas_enable_dev(ot_vo_dev phy_dev);
td_void vo_notify_cas_disable_dev(ot_vo_dev phy_dev);
td_void vo_notify_cas_layer_chn_enable(ot_vo_layer cas_layer, td_s32 area_id, td_bool enable);
td_void vo_notify_cas_chn_rgns_sort(ot_vo_layer layer, ot_vo_chn chn, td_bool switch_size);
td_void vo_chn_cas_rgns_sort(ot_vo_layer layer, td_bool switch_size);

td_s32 vo_get_cas_disp_dev(ot_vo_dev *dev);
td_s32 vo_set_cas_cfg(ot_vo_dev cas_dev);

td_s32 vo_ioctl_cas_attr(ot_vo_cas_attr *cas_attr, vo_ioctl_type ioctl_type);

td_s32 vo_ioctl_enable_disable_cas_dev(const vo_ioctl_dev_param *ioctl_dev, vo_ioctl_type ioctl_type);
td_s32 vo_disable_cas_dev(const vo_ioctl_dev_param *ioctl_dev);

td_s32 vo_ioctl_cas_pattern(vo_ioctl_cas_pattern_param *ioctl_cas_pattern, vo_ioctl_type ioctl_type);

td_s32 vo_ioctl_enable_disable_cas(vo_ioctl_type ioctl_type);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef MKP_VO_CASCADE_H */

