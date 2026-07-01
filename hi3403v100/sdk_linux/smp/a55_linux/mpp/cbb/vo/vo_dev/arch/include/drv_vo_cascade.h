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

#ifndef DRV_VO_CASCADE_H
#define DRV_VO_CASCADE_H

#include "ot_common_video.h"
#include "ot_common_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

td_bool vo_drv_is_cas_dev(ot_vo_dev dev);

ot_vo_dev vo_drv_get_cas_phy_dev(td_void);
ot_vo_layer vo_drv_get_cas_phy_layer(td_void);
ot_vo_dev vo_drv_get_odd_cas_dev(td_void);
ot_vo_dev vo_drv_get_even_cas_dev(td_void);
td_bool vo_drv_is_support_slave_dual_mode(td_void);
td_void vo_drv_set_cas_enable(td_bool enable);
td_void vo_drv_set_cas_vbi_pos(td_void);
td_void vo_drv_set_cas_slave_mode(td_bool is_slave);
td_void vo_drv_set_cas_pattern(td_u32 pattern);
td_void vo_drv_set_cas_mode(td_bool is_slave, ot_vo_cas_mode cas_mode, td_bool is_even);
td_s32 vo_drv_get_cas_mode(ot_vo_cas_mode *cas_mode, td_bool *is_even);
td_void vo_drv_set_cas_pos_enable(td_u32 pos, td_bool enable);
td_void vo_drv_clear_cas_all_pos_enable(td_void);

td_void vo_drv_cas_set_mrg_rect(ot_vo_layer layer, const ot_rect *rect, td_s32 area_id);
td_void vo_drv_cas_set_dev_pll_clk(ot_vo_dev dev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of DRV_VO_CASCADE_H */
