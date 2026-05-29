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

#include "hal_vo_video.h"
#include "ot_board.h"
#include "drv_vo.h"
#include "hal_vo.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

td_void hal_cbm_set_cbm_attr(hal_disp_layer layer, ot_vo_dev dev)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_link_ctrl link_ctrl;

    link_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->link_ctrl.u32));
    if (layer == HAL_DISP_LAYER_VHD2) {
        link_ctrl.bits.v2_link = dev;
    } else if (layer == HAL_DISP_LAYER_GFX3) {
        link_ctrl.bits.g3_link = dev;
    }
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->link_ctrl.u32), link_ctrl.u32);
}

td_void hal_cbm_set_cbm_bkg(hal_cbmmix mixer, const hal_disp_bkcolor *bkg)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_cbm_bkg1 cbm_bkg1;
    volatile reg_cbm_bkg2 cbm_bkg2;

    if (mixer == HAL_CBMMIX1) {
        cbm_bkg1.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->cbm_bkg1.u32));
        cbm_bkg1.bits.cbm_bkgy1 = (bkg->bkg_y);
        cbm_bkg1.bits.cbm_bkgcb1 = (bkg->bkg_cb);
        cbm_bkg1.bits.cbm_bkgcr1 = (bkg->bkg_cr);
        hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->cbm_bkg1.u32), cbm_bkg1.u32);
    } else if (mixer == HAL_CBMMIX2) {
        cbm_bkg2.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->cbm_bkg2.u32));
        cbm_bkg2.bits.cbm_bkgy2 = (bkg->bkg_y);
        cbm_bkg2.bits.cbm_bkgcb2 = (bkg->bkg_cb);
        cbm_bkg2.bits.cbm_bkgcr2 = (bkg->bkg_cr);
        hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->cbm_bkg2.u32), cbm_bkg2.u32);
    }
}

static td_bool hal_cbm_get_cbm1_mixer_layer_id(ot_vo_layer layer, td_u8 *layer_id)
{
    const vo_hal_cbm_mixer cbm1_mixer[] = {
        { OT_VO_LAYER_V0, 0x1 },
        { OT_VO_LAYER_V2, 0x3 },
        { OT_VO_LAYER_G0, 0x2 },
        { OT_VO_LAYER_G3, 0x4 },
        { VO_LAYER_BUTT,  0x0 }
    };

    td_u32 cbm1_len = sizeof(cbm1_mixer) / sizeof(vo_hal_cbm_mixer);
    return hal_cbm_get_cbm_mixer_layer_id(cbm1_mixer, cbm1_len, layer, layer_id);
}

static td_bool hal_cbm_get_cbm2_mixer_layer_id(ot_vo_layer layer, td_u8 *layer_id)
{
    const vo_hal_cbm_mixer cbm2_mixer[] = {
        { OT_VO_LAYER_V1, 0x1 },
        { OT_VO_LAYER_V2, 0x3 },
        { OT_VO_LAYER_G1, 0x2 },
        { OT_VO_LAYER_G3, 0x4 },
        { VO_LAYER_BUTT,  0x0 }
    };

    td_u32 cbm2_len = sizeof(cbm2_mixer) / sizeof(vo_hal_cbm_mixer);
    return hal_cbm_get_cbm_mixer_layer_id(cbm2_mixer, cbm2_len, layer, layer_id);
}

static td_void hal_cbm_set_cbm1_mixer_prio(ot_vo_layer layer, td_u8 prio)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_cbm_mix1 cbm_mix1;
    td_u8 layer_id = 0;

    /* check layer availability */
    if (hal_cbm_get_cbm1_mixer_layer_id(layer, &layer_id) != TD_TRUE) {
        return;
    }

     /* set mixer prio */
    cbm_mix1.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->cbm_mix1.u32));

    switch (prio) {
        case 0:  /* 0: prio 0 */
            cbm_mix1.bits.mixer_prio0 = layer_id;
            break;

        case 1:  /* 1: prio 1 */
            cbm_mix1.bits.mixer_prio1 = layer_id;
            break;

        case 2:  /* 2: prio 2 */
            cbm_mix1.bits.mixer_prio2 = layer_id;
            break;

        case 3:  /* 3: prio 3 */
            cbm_mix1.bits.mixer_prio3 = layer_id;
            break;

        default:
            vo_err_trace("error priority id %d found\n", prio);
            return;
    }

    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->cbm_mix1.u32), cbm_mix1.u32);
}

static td_void hal_cbm_set_cbm2_mixer_prio(ot_vo_layer layer, td_u8 prio)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_cbm_mix2 cbm_mix2;
    td_u8 layer_id = 0;

    /* check layer availability */
    if (hal_cbm_get_cbm2_mixer_layer_id(layer, &layer_id) != TD_TRUE) {
        return;
    }

    /* set mixer prio */
    cbm_mix2.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->cbm_mix2.u32));

    switch (prio) {
        case 0:  /* 0: prio 0 */
            cbm_mix2.bits.mixer_prio0 = layer_id;
            break;

        case 1:  /* 1: prio 1 */
            cbm_mix2.bits.mixer_prio1 = layer_id;
            break;

        case 2:  /* 2: prio 2 */
            cbm_mix2.bits.mixer_prio2 = layer_id;
            break;

        case 3:  /* 3: prio 3 */
            cbm_mix2.bits.mixer_prio3 = layer_id;
            break;

        default:
            vo_err_trace("error priority id %d found\n", prio);
            return;
    }

    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->cbm_mix2.u32), cbm_mix2.u32);
}

td_void hal_cbm_set_cbm_mixer_prio(ot_vo_layer layer, td_u8 prio, td_u8 mixer_id)
{
    if (mixer_id == HAL_CBMMIX1) {
        hal_cbm_set_cbm1_mixer_prio(layer, prio);
    } else if (mixer_id == HAL_CBMMIX2) {
        hal_cbm_set_cbm2_mixer_prio(layer, prio);
    }
}

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

/* for CMP and DCMP */
#define CMP_SEG_OFFSET  (0x80 / 4)
#define DCMP_SEG_OFFSET (0x20 / 4)

td_void hal_write_mrg_reg(td_u32 *address, td_u32 value)
{
    volatile reg_vdp_regs *reg = vo_hal_get_reg();
    vo_coef_addr *coef_addr = vo_drv_get_coef_buf_addr();
    td_u32 addr = VOU_REGS_ADDR + (td_ulong)(td_uintptr_t)address - (td_ulong)(td_uintptr_t)(&(reg->voctrl.u32));
    td_u32 *tmp = TD_NULL;

    td_u32 v0_mrg_addr = VOU_REGS_ADDR + MRG_OFFSET_ADDR;

    /* 意味着在写V0_MRG_CTRL ，重定向到DDR */
    if ((addr >= v0_mrg_addr) && (addr < v0_mrg_addr + MRG_REGS_LEN)) {
        tmp = (addr - v0_mrg_addr) / REG_BYTES + (td_u32 *)coef_addr->coef_vir_addr[VO_COEF_BUF_REGION_V0];
        *(volatile td_u32 *)tmp = value;
        /* 意味着在写V1_MRG_CTRL，重定向到DDR */
    } else {
        *(volatile td_u32 *)address = value;
    }
}

td_u32 hal_read_mrg_reg(const td_u32 *address)
{
    volatile reg_vdp_regs *reg = vo_hal_get_reg();
    vo_coef_addr *coef_addr = vo_drv_get_coef_buf_addr();
    td_u32 addr = VOU_REGS_ADDR + (td_ulong)(td_uintptr_t)address - (td_ulong)(td_uintptr_t)(&(reg->voctrl.u32));
    td_u32 *tmp = TD_NULL;

    td_u32 v0_mrg_addr = VOU_REGS_ADDR + MRG_OFFSET_ADDR;

    /* 意味着在读V0_MRG_CTRL ，重定向到DDR */
    if ((addr >= v0_mrg_addr) && (addr < v0_mrg_addr + MRG_REGS_LEN)) {
        tmp = (addr - v0_mrg_addr) / REG_BYTES + (td_u32 *)coef_addr->coef_vir_addr[VO_COEF_BUF_REGION_V0];

        return *(volatile td_u32 *)tmp;
        /* 意味着在读V1_MRG_CTRL ，重定向到DDR */
    } else {
        return *(volatile td_u32 *)(address);
    }
}

td_u32 hal_layer_get_layer_max_area_num(hal_disp_layer layer)
{
    td_u32 max_area_num = 0;

    if (layer == HAL_DISP_LAYER_VHD0) {
        max_area_num = V0_REGION_NUM;
    }

    return max_area_num;
}

td_void hal_para_set_para_addr_vhd_chn08(td_phys_addr_t para_addr_vhd_chn08)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();

    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->para_haddr_vhd_chn08), get_high_addr(para_addr_vhd_chn08));
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->para_addr_vhd_chn08), get_low_addr(para_addr_vhd_chn08));
}

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
