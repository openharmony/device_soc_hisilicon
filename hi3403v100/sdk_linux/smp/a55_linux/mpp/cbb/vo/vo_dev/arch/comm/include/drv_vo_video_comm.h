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

#ifndef DRV_VO_VIDEO_COMM_H
#define DRV_VO_VIDEO_COMM_H

#include "drv_vo_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

#define VO_ALPHA_OPACITY                    0xFF /* opacity alpha */

#if vo_desc("layer cfg")
td_void vou_drv_layer_enable(ot_vo_layer layer, td_bool enable);
td_void vou_drv_set_layer_data_fmt(ot_vo_layer layer, vou_layer_pixel_format data_fmt);
#endif

#if vo_desc("layer")
td_s32 vou_drv_get_video_layer_bind_dev(ot_vo_layer layer);
#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("layer coef")
td_s32 vo_drv_allocate_mem(td_u32 size, vo_mmz_buffer *vdp_mmz_buffer);
td_void vo_drv_delete_mem(vo_mmz_buffer *vdp_mmz_buffer);

td_void vo_zme_coef_init(td_void);
#endif

#if vo_desc("layer cfg")
ot_vo_layer vo_drv_get_hw_layer(ot_vo_layer layer);
td_void vou_drv_disable_all_area(ot_vo_layer layer);
td_void vo_drv_vhd_coef_para_up(vo_coef_buf buf_name);
#endif

#if vo_desc("layer")
td_u32 vou_drv_get_layer_chn_num(ot_vo_layer layer);
#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif

#endif /* end of DRV_VO_VIDEO_COMM_H */
