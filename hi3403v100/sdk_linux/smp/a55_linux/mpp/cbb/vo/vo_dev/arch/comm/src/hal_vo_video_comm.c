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

#include "hal_vo_video_comm.h"
#include "hal_vo_comm.h"
#include "drv_vo.h"
#include "hal_vo.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */
#if vo_desc("UBOOT_VO")

#if vo_desc("video")

td_void hal_video_set_layer_ck_gt_en(hal_disp_layer layer, td_bool ck_gt_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_voctrl voctrl;

    /* v3 not support to return */
    if (layer > HAL_DISP_LAYER_VHD2) {
        return;
    }

    voctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->voctrl.u32));
    if (layer == HAL_DISP_LAYER_VHD0) {
        voctrl.bits.v0_ck_gt_en = ck_gt_en;
    } else if (layer == HAL_DISP_LAYER_VHD1) {
        voctrl.bits.v1_ck_gt_en = ck_gt_en;
    } else {
        voctrl.bits.v2_ck_gt_en = ck_gt_en;
    }
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->voctrl.u32), voctrl.u32);
}

td_void hal_video_set_layer_up_mode(hal_disp_layer layer, td_u32 up_mode)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_ctrl v0_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_video_layer_return(layer);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_ctrl.u32));
    v0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_ctrl.bits.rgup_mode = up_mode;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_ctrl.u32);
}

td_void hal_layer_enable_layer(hal_disp_layer layer, td_u32 enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_ctrl v0_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_video_layer_return(layer);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_ctrl.u32));
    v0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_ctrl.bits.surface_en = enable;
    v0_ctrl.bits.nosec_flag = 1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_ctrl.u32);
}

td_void hal_layer_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vid_src_info vid_src_info;
    volatile td_ulong addr_reg;

    vo_hal_check_video_layer_return(layer);

    addr_reg = vou_get_vid_abs_addr(layer, (td_uintptr_t)&(vo_reg->vid_src_info.u32));
    vid_src_info.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    vid_src_info.bits.data_type = data_fmt;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, vid_src_info.u32);
}

td_void hal_video_hfir_set_ck_gt_en(hal_disp_layer layer, td_u32 ck_gt_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_hfir_ctrl v0_hfir_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_video_layer_return(layer);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_hfir_ctrl.u32));
    v0_hfir_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_hfir_ctrl.bits.ck_gt_en = ck_gt_en;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_hfir_ctrl.u32);
}

td_void hal_video_hfir_set_mid_en(hal_disp_layer layer, td_u32 mid_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_hfir_ctrl v0_hfir_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_video_layer_return(layer);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_hfir_ctrl.u32));
    v0_hfir_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_hfir_ctrl.bits.mid_en = mid_en;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_hfir_ctrl.u32);
}

td_void hal_video_hfir_set_hfir_mode(hal_disp_layer layer, td_u32 hfir_mode)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_hfir_ctrl v0_hfir_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_video_layer_return(layer);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_hfir_ctrl.u32));
    v0_hfir_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_hfir_ctrl.bits.hfir_mode = hfir_mode;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_hfir_ctrl.u32);
}

td_void hal_video_hfir_set_coef(hal_disp_layer layer, const hfir_coef *coef)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_hfircoef01 v0_hfir_coef01;
    volatile reg_v0_hfircoef23 v0_hfir_coef23;
    volatile reg_v0_hfircoef45 v0_hfir_coef45;
    volatile reg_v0_hfircoef67 v0_hfir_coef67;
    volatile td_ulong addr_reg;

    vo_hal_check_video_layer_return(layer);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_hfircoef01.u32));
    v0_hfir_coef01.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_hfir_coef01.bits.coef0 = coef->coef0;
    v0_hfir_coef01.bits.coef1 = coef->coef1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_hfir_coef01.u32);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_hfircoef23.u32));
    v0_hfir_coef23.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_hfir_coef23.bits.coef2 = coef->coef2;
    v0_hfir_coef23.bits.coef3 = coef->coef3;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_hfir_coef23.u32);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_hfircoef45.u32));
    v0_hfir_coef45.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_hfir_coef45.bits.coef4 = coef->coef4;
    v0_hfir_coef45.bits.coef5 = coef->coef5;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_hfir_coef45.u32);

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_hfircoef67.u32));
    v0_hfir_coef67.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_hfir_coef67.bits.coef6 = coef->coef6;
    v0_hfir_coef67.bits.coef7 = coef->coef7;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_hfir_coef67.u32);
}

td_void hal_layer_set_reg_up(hal_disp_layer layer)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_upd v0_upd;
    volatile td_ulong addr_reg;

    if (vo_hal_is_video_layer(layer)) {
        addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_upd.u32));
        v0_upd.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
        /* video layer register update */
        v0_upd.bits.regup = 0x1;
        hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_upd.u32);
    }
}

td_bool hal_cbm_get_cbm_mixer_layer_id(const vo_hal_cbm_mixer *cbm_mixer, td_u32 cbm_len,
    ot_vo_layer layer, td_u8 *layer_id)
{
    td_u32 index;

    for (index = 0; index < cbm_len; index++) {
        if (layer == cbm_mixer[index].layer) {
            *layer_id = cbm_mixer[index].layer_id;
            return TD_TRUE;
        }
    }

    vo_err_trace("error layer id %d found\n", layer);
    return TD_FALSE;
}

#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("video")

/* disable all area */
td_void hal_video_set_all_area_disable(hal_disp_layer layer)
{
    td_u32 max_area_num;
    td_u32 area_num;
    td_u32 offset = 0;

    if (vo_hal_is_video_layer(layer)) {
        if (layer == HAL_DISP_LAYER_VHD0) {
            offset = 0;
        } else if (layer == HAL_DISP_LAYER_VHD1) {
            offset = V0_REGION_NUM * REGION_OFFSET;
        }

        max_area_num = hal_layer_get_layer_max_area_num(layer);
        for (area_num = 0; area_num < max_area_num; area_num++) {
            vdp_fdr_vid_set_mrg_en(layer, offset + area_num * REGION_OFFSET, TD_FALSE);
        }
    }
}

td_void hal_para_set_para_up_vhd_chn(td_u32 chn_num)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_para_up_vhd para_up_vhd;

    para_up_vhd.u32 = (1 << chn_num);
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->para_up_vhd.u32), para_up_vhd.u32);
}

#endif

#if vo_desc("video layer multi region")

td_ulong vo_hal_get_layer_mrg_abs_addr(hal_disp_layer layer, td_ulong reg)
{
    ot_unused(layer);
    return reg;
}

td_void vdp_fdr_vid_set_mrg_en(hal_disp_layer layer, td_u32 offset, td_u32 mrg_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_mrg_ctrl v0_mrg_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_mrg_video_layer_return(layer);

    addr_reg = vo_hal_get_layer_mrg_abs_addr(layer, (td_uintptr_t)(&(vo_reg->v0_mrg_ctrl.u32) + offset));
    v0_mrg_ctrl.u32 = hal_read_mrg_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_mrg_ctrl.bits.mrg_en = mrg_en;
    hal_write_mrg_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_mrg_ctrl.u32);
}

#endif
#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
