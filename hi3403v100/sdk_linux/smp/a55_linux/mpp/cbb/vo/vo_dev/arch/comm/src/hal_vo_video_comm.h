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

#ifndef HAL_VO_VIDEO_COMM_H
#define HAL_VO_VIDEO_COMM_H

#include "hal_vo_def.h"
#include "drv_vo_coef_comm.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

typedef struct {
    ot_vo_layer layer;
    td_u8 layer_id;
} vo_hal_cbm_mixer;

#if vo_desc("layer")
td_void hal_video_set_layer_ck_gt_en(hal_disp_layer layer, td_bool ck_gt_en);
td_void hal_video_set_layer_up_mode(hal_disp_layer layer, td_u32 up_mode);
#endif

#if vo_desc("layer")
/* video layer HFIR relative hal functions */
td_void hal_video_hfir_set_ck_gt_en(hal_disp_layer layer, td_u32 ck_gt_en);
td_void hal_video_hfir_set_mid_en(hal_disp_layer layer, td_u32 mid_en);
td_void hal_video_hfir_set_hfir_mode(hal_disp_layer layer, td_u32 hfir_mode);
td_void hal_video_hfir_set_coef(hal_disp_layer layer, const hfir_coef *coef);

td_void hal_layer_enable_layer(hal_disp_layer layer, td_u32 enable);
td_void hal_layer_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt);
td_void hal_layer_set_reg_up(hal_disp_layer layer);
td_bool hal_cbm_get_cbm_mixer_layer_id(const vo_hal_cbm_mixer *cbm_mixer, td_u32 cbm_len,
    ot_vo_layer layer, td_u8 *layer_id);
#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")
#if vo_desc("layer")
td_void hal_video_set_all_area_disable(hal_disp_layer layer);
#endif

#if vo_desc("layer")
td_void hal_para_set_para_up_vhd_chn(td_u32 chn_num);
#endif

#if vo_desc("layer fdr")

td_void vdp_fdr_vid_set_mrg_en(hal_disp_layer layer, td_u32 offset, td_u32 mrg_en);
td_ulong vo_hal_get_layer_mrg_abs_addr(hal_disp_layer layer, td_ulong reg);

#endif
#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of HAL_VO_VIDEO_COMM_H */
