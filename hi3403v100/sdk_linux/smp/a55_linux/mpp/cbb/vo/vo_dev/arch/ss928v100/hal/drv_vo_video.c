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

#include "drv_vo_video.h"
#include "hal_vo.h"
#include "drv_vo.h"
#include "ot_inner_video.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

#define VO_LAYER_DISP_RECT_W_MAX_OF_VHDX 3840
#define VO_LAYER_DISP_RECT_H_MAX_OF_VHDX 3840
#define VO_LAYER_DISP_RECT_W_MAX_OF_VHD1 1920
#define VO_LAYER_DISP_RECT_H_MAX_OF_VHD1 3840
#define VO_LAYER_DISP_RECT_W_MAX_OF_VHD2 1920
#define VO_LAYER_DISP_RECT_H_MAX_OF_VHD2 3840

#define VO_LAYER_CSC_MIN_COEF_VAL   (-2048)   /* 12bit min */
#define VO_LAYER_CSC_MAX_COEF_VAL   2047      /* 12bit max */

#if vo_desc("video coef")

td_void vo_drv_vhd_coef_buf_addr_distribute(vo_coef_addr *coef_addr)
{
    coef_addr->coef_vir_addr[VO_COEF_BUF_REGION_V0] = coef_addr->buf_base_addr.start_vir_addr;
    coef_addr->coef_phys_addr[VO_COEF_BUF_REGION_V0] = coef_addr->buf_base_addr.start_phys_addr;
    hal_para_set_para_addr_vhd_chn08(coef_addr->coef_phys_addr[VO_COEF_BUF_REGION_V0]);
}

#endif

#if vo_desc("layer")

hal_disp_layer vou_drv_convert_layer(ot_vo_layer layer)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    switch (layer) {
        case VO_HAL_LAYER_VHD0:
            disp_layer = HAL_DISP_LAYER_VHD0;
            break;

        case VO_HAL_LAYER_VHD1:
            disp_layer = HAL_DISP_LAYER_VHD1;
            break;

        case VO_HAL_LAYER_VHD2:
            disp_layer = HAL_DISP_LAYER_VHD2;
            break;

        default:
            break;
    }

    return disp_layer;
}

#endif

td_void vou_drv_layer_csc_enable(ot_vo_layer layer, td_bool csc_en)
{
    /* 逻辑问题规避：ck_gt_en打开时读取csc寄存器发生错位，此芯片已解决，打开节能 */
    hal_layer_csc_set_enable_ck_gt_en(layer, csc_en, csc_en);
}

#endif /* #if vo_desc("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
