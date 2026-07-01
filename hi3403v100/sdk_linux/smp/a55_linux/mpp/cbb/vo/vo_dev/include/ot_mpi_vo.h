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

#ifndef OT_MPI_VO_H
#define OT_MPI_VO_H

#include "ot_common_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 ot_mpi_vo_enable(ot_vo_dev dev);
td_s32 ot_mpi_vo_disable(ot_vo_dev dev);
td_s32 ot_mpi_vo_set_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr);
td_s32 ot_mpi_vo_get_pub_attr(ot_vo_dev dev, ot_vo_pub_attr *pub_attr);
td_s32 ot_mpi_vo_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param);
td_s32 ot_mpi_vo_get_dev_param(ot_vo_dev dev, ot_vo_dev_param *dev_param);
td_s32 ot_mpi_vo_close_fd(td_void);
td_s32 ot_mpi_vo_set_dev_frame_rate(ot_vo_dev dev, td_u32 frame_rate);
td_s32 ot_mpi_vo_get_dev_frame_rate(ot_vo_dev dev, td_u32 *frame_rate);
td_s32 ot_mpi_vo_set_user_sync_info(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info);
td_s32 ot_mpi_vo_get_user_sync_info(ot_vo_dev dev, ot_vo_user_sync_info *sync_info);
td_s32 ot_mpi_vo_set_mod_param(const ot_vo_mod_param *mod_param);
td_s32 ot_mpi_vo_get_mod_param(ot_vo_mod_param *mod_param);
td_s32 ot_mpi_vo_set_vtth(ot_vo_dev dev, td_u32 vtth);
td_s32 ot_mpi_vo_get_vtth(ot_vo_dev dev, td_u32 *vtth);
td_s32 ot_mpi_vo_set_less_buf_attr(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr);
td_s32 ot_mpi_vo_get_less_buf_attr(ot_vo_dev dev, ot_vo_less_buf_attr *less_buf_attr);
td_s32 ot_mpi_vo_set_user_notify_attr(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr);
td_s32 ot_mpi_vo_get_user_notify_attr(ot_vo_dev dev, ot_vo_user_notify_attr *user_notify_attr);

td_s32 ot_mpi_vo_query_intf_status(ot_vo_intf_type intf_type, ot_vo_intf_status *status);
td_s32 ot_mpi_vo_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param);
td_s32 ot_mpi_vo_get_vga_param(ot_vo_dev dev, ot_vo_vga_param *vga_param);
td_s32 ot_mpi_vo_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param);
td_s32 ot_mpi_vo_get_hdmi_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi_param);
td_s32 ot_mpi_vo_set_hdmi1_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi1_param);
td_s32 ot_mpi_vo_get_hdmi1_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi1_param);
td_s32 ot_mpi_vo_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param);
td_s32 ot_mpi_vo_get_rgb_param(ot_vo_dev dev, ot_vo_rgb_param *rgb_param);
td_s32 ot_mpi_vo_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param);
td_s32 ot_mpi_vo_get_bt_param(ot_vo_dev dev, ot_vo_bt_param *bt_param);
td_s32 ot_mpi_vo_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param);
td_s32 ot_mpi_vo_get_mipi_param(ot_vo_dev dev, ot_vo_mipi_param *mipi_param);

td_s32 ot_mpi_vo_set_cas_attr(const ot_vo_cas_attr *cas_attr);
td_s32 ot_mpi_vo_get_cas_attr(ot_vo_cas_attr *cas_attr);
td_s32 ot_mpi_vo_enable_cas_dev(ot_vo_dev cas_dev);
td_s32 ot_mpi_vo_disable_cas_dev(ot_vo_dev cas_dev);
td_s32 ot_mpi_vo_set_cas_pattern(ot_vo_dev cas_dev, td_u32 pattern);
td_s32 ot_mpi_vo_get_cas_pattern(ot_vo_dev cas_dev, td_u32 *pattern);
td_s32 ot_mpi_vo_enable_cas(td_void);
td_s32 ot_mpi_vo_disable_cas(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* OT_MPI_VO_H */
