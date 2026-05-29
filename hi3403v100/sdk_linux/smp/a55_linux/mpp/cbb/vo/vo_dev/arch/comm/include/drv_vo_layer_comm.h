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

#ifndef DRV_VO_LAYER_COMM_H
#define DRV_VO_LAYER_COMM_H

#include "inner_vo.h"
#include "drv_vo_coef_comm.h"
#include "hal_vo_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

#if vo_desc("layer")
#define VO_LAYER_CSC_SCALE2P_DEF_VAL 0xa
#define VO_LAYER_CSC_CLIP_MIN 0x0
#define VO_LAYER_CSC_CLIP_MAX 0xfff
#endif

#if vo_desc("layer csc")
td_s32 vo_drv_get_csc_matrix(ot_vo_csc_matrix csc_matrix, const csc_coef **csc_tmp);
td_void vo_drv_calculate_yuv2rgb(const hal_csc_value *csc_value, const csc_coef *csc_tmp, csc_coef *coef);
td_void vo_drv_calculate_rgb2yuv(const hal_csc_value *csc_value, const csc_coef *csc_tmp, csc_coef *coef);
td_void vou_drv_calc_csc_matrix(const ot_vo_csc *csc, ot_vo_csc_matrix csc_matrix, csc_coef *coef);
#endif
#endif /* #if vo_desc("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif

#endif /* end of DRV_VO_LAYER_COMM_H */
