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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART01_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART01_H_


#line 1 "gfbg_graphic_hal.c"
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

#include "gfbg_graphic_hal.h"
#include "ot_osal.h"
#include "ot_debug.h"
#include "gfbg_graphics_drv.h"
#include "gfbg_coef.h"
#include "ot_math.h"
#include "gfbg_def.h"

/* MACRO DEFINITION */
#define HAL_PRINT       OT_PRINT

#define VOU_REGS_ADDR   0x17A00000
#define VOU_REGS_SIZE   0x40000

#define SMART_RECT_LAYER_G3   3
#define SMART_RECT_LAYER_G4   4

#define FB_8K_SCREEN_WIDTH_G0 3840
#define FB_8K_SCREEN_HEIGHT_G0 4320

volatile vdp_regs_type *g_gfbg_reg = TD_NULL;

td_s32 fb_hal_vou_init(td_void)
{
    if (g_gfbg_reg == TD_NULL) {
        g_gfbg_reg = (volatile vdp_regs_type *)osal_ioremap(VOU_REGS_ADDR, (td_u32)VOU_REGS_SIZE);
    }

    if (g_gfbg_reg == TD_NULL) {
        osal_printk("ioremap_nocache failed\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_void fb_hal_vou_exit(td_void)
{
    if (g_gfbg_reg != TD_NULL) {
        osal_iounmap((void *)g_gfbg_reg, (td_u32)VOU_REGS_SIZE);
        g_gfbg_reg = TD_NULL;
    }
}

static td_void fb_hal_write_reg(td_u32 *address, td_u32 value)
{
    if (address == TD_NULL) {
        return;
    }
    *(volatile td_u32 *)address = value;
    return;
}

static td_u32 fb_hal_read_reg(const td_u32 *address)
{
    if (address == TD_NULL) {
        return 0;
    }
    return *(volatile td_u32 *)(address);
}

/*****************************************************************************
 Prototype         : fb_vou_get_abs_addr
 Description       : Get the absolute address of the layer (video layer and graphics layer)
*****************************************************************************/
static td_ulong fb_vou_get_abs_addr(hal_disp_layer layer, td_ulong reg)
{
    td_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            reg_abs_addr = (reg) + (layer - HAL_DISP_LAYER_VHD0) * VHD_REGS_LEN;
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            reg_abs_addr = (reg) + (layer - HAL_DISP_LAYER_GFX0) * GFX_REGS_LEN;
            break;
        /* one wbc dev */
        case HAL_DISP_LAYER_WBC:
            reg_abs_addr = (reg);
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
    }
    return reg_abs_addr;
}

/*
 * Prototype         : fb_vou_get_chn_abs_addr
 * Description       : Get the absolute address of the video channel
 */
static td_ulong fb_vou_get_chn_abs_addr(hal_disp_outputchannel chan, td_ulong reg)
{
    volatile td_ulong reg_abs_addr;

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:
            reg_abs_addr = reg + (chan - HAL_DISP_CHANNEL_DHD0) * DHD_REGS_LEN;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
    }

    return reg_abs_addr;
}

static td_u32 hal_get_addr_chnabs(hal_disp_outputchannel chan, const volatile td_u32 *value)
{
    volatile td_ulong addr_reg;
    addr_reg = fb_vou_get_chn_abs_addr(chan, (td_ulong)(uintptr_t)value);
    return fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
}

static td_u32 hal_get_addr_abs(volatile td_ulong *addr_reg, hal_disp_layer layer, const volatile td_u32 *value)
{
    *addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)value);
    return fb_hal_read_reg((td_u32 *)(uintptr_t)(*addr_reg));
}

static td_ulong fb_vou_get_gfx_abs_addr(hal_disp_layer layer, td_ulong reg)
{
    volatile td_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            reg_abs_addr = reg + (layer - HAL_DISP_LAYER_GFX0) * GRF_REGS_LEN;
            break;

        default:
            HAL_PRINT("Error layer id found in FUNC:%s,LINE:%d\n", __FUNCTION__, __LINE__);
            return 0;
    }

    return reg_abs_addr;
}

td_void hal_disp_get_int_state_vcnt(hal_disp_outputchannel chan, td_u32 *vcnt)
{
    volatile u_dhd0_state dhd0_state;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:{
            dhd0_state.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_state.u32));
            *vcnt = dhd0_state.bits.vcnt;
            break;
        }
        default: {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return;
        }
    }

    return;
}

/*
 * Name : fb_hal_disp_get_intf_enable
 * Desc : Get the status (enable,disable status) of display interface.
 */
td_bool fb_hal_disp_get_intf_enable(hal_disp_outputchannel chan, td_bool *intf_en)
{
    volatile u_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:
            addr_reg = fb_vou_get_chn_abs_addr(chan, (td_ulong)(uintptr_t)&(g_gfbg_reg->dhd0_ctrl.u32));
            dhd0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *intf_en = dhd0_ctrl.bits.intf_en;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_disp_get_intf_sync(hal_disp_outputchannel chan, hal_disp_syncinfo *sync_info)
{
    volatile u_dhd0_ctrl dhd0_ctrl;
    volatile u_dhd0_vsync1 dhd0_vsync1;
    volatile u_dhd0_vsync2 dhd0_vsync2;
    volatile u_dhd0_hsync1 dhd0_hsync1;
    volatile u_dhd0_hsync2 dhd0_hsync2;
    volatile u_dhd0_vplus1 dhd0_vplus1;
    volatile u_dhd0_vplus2 dhd0_vplus2;
    volatile u_dhd0_pwr dhd0_pwr;
    if ((g_gfbg_reg == TD_NULL) || (sync_info == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:{
            dhd0_ctrl.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_ctrl.u32));
            sync_info->iop = dhd0_ctrl.bits.iop;

            dhd0_hsync1.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_hsync1.u32));
            sync_info->hact = dhd0_hsync1.bits.hact + 1;
            sync_info->hbb = dhd0_hsync1.bits.hbb + 1;

            dhd0_hsync2.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_hsync2.u32));
            sync_info->hmid = (dhd0_hsync2.bits.hmid == 0) ? 0 : (dhd0_hsync2.bits.hmid + 1);
            sync_info->hfb = dhd0_hsync2.bits.hfb + 1;

            /* Config VHD interface vertical timing */
            dhd0_vsync1.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_vsync1.u32));
            sync_info->vact = dhd0_vsync1.bits.vact + 1;
            sync_info->vbb = dhd0_vsync1.bits.vbb + 1;

            dhd0_vsync2.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_vsync2.u32));
            sync_info->vfb = dhd0_vsync2.bits.vfb + 1;

            /* Config VHD interface vertical bottom timing,no use in progressive mode */
            dhd0_vplus1.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_vplus1.u32));
            sync_info->bvact = dhd0_vplus1.bits.bvact + 1;
            sync_info->bvbb = dhd0_vplus1.bits.bvbb + 1;

            dhd0_vplus2.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_vplus2.u32));
            sync_info->bvfb = dhd0_vplus2.bits.bvfb + 1;

            /* Config VHD interface vertical bottom timing, */
            dhd0_pwr.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_pwr.u32));
            sync_info->hpw = dhd0_pwr.bits.hpw + 1;
            sync_info->vpw = dhd0_pwr.bits.vpw + 1;
            break;
        }
        default: {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
        }
    }
    return TD_TRUE;
}

td_bool fb_hal_disp_get_intf_mux_sel(hal_disp_outputchannel chan, vo_intf_type *intf_type)
{
    volatile u_vo_mux vo_mux;
    if (chan > HAL_DISP_CHANNEL_DSD0) {
        HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if ((g_gfbg_reg == TD_NULL) || (intf_type == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    vo_mux.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->vo_mux.u32));

    switch (vo_mux.bits.digital_sel) {
        case 0: { /* 0 alg data */
            *intf_type = HAL_DISP_INTF_BT1120;
            break;
        }
        case 1: { /* 1 alg data */
            *intf_type = HAL_DISP_INTF_BT656;
            break;
        }

        default: {
            return TD_FALSE;
        }
    }

    return TD_TRUE;
}

/*****************************************************************************
 Prototype         : fb_hal_disp_get_disp_iop
 Description       : Interlace or Progressive
*****************************************************************************/
td_bool fb_hal_disp_get_disp_iop(hal_disp_outputchannel chan, td_bool *iop)
{
    u_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (iop == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:{
            addr_reg = fb_vou_get_chn_abs_addr(chan, (td_ulong)(uintptr_t)&(g_gfbg_reg->dhd0_ctrl.u32));
            dhd0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *iop = dhd0_ctrl.bits.iop;
            break;
        }

        default:
            gfbg_graphics_error("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_disp_get_vt_thd_mode(hal_disp_outputchannel chan, td_bool *field_mode)
{
    volatile u_dhd0_vtthd dhd0_vtthd;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:
            addr_reg = fb_vou_get_chn_abs_addr(chan, (td_ulong)(uintptr_t)&(g_gfbg_reg->dhd0_vtthd.u32));
            dhd0_vtthd.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *field_mode = dhd0_vtthd.bits.thd1_mode;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_disp_get_int_state_vcnt(hal_disp_outputchannel chan, td_u32 *vcnt)
{
    volatile u_dhd0_state dhd0_state;
    volatile td_ulong addr_reg;

    if (chan <= HAL_DISP_CHANNEL_DSD0) {
        addr_reg = fb_vou_get_chn_abs_addr(chan, (td_ulong) & (g_gfbg_reg->dhd0_state.u32));
        dhd0_state.u32 = fb_hal_read_reg((td_u32*)(uintptr_t)addr_reg);
        *vcnt = dhd0_state.bits.vcnt;
    } else {
        HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

#ifdef CONFIG_OT_VO_FB_SEPARATE
/* Get interrupt status */
td_u32 fb_hal_disp_get_int_status(td_u32 int_msk)
{
    volatile u_vomskintsta1 vomskintsta1;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return 0;
    }

    /* read interrupt status */
    vomskintsta1.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->vomskintsta1.u32));

    return (vomskintsta1.u32 & int_msk);
}
#endif

/*
 * Name : fb_hal_disp_clear_int_status
 * Desc : Clear interrupt status.
 */
td_bool fb_hal_disp_clear_int_status(td_u32 int_msk)
{
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
#ifdef CONFIG_OT_VO_FB_SEPARATE
    /* read interrupt status */
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->vomskintsta1.u32), int_msk);
#else
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->vomskintsta.u32), int_msk);
#endif
    return TD_TRUE;
}

td_bool fb_hal_video_set_layer_disp_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    volatile u_g0_dfpos g0_dfpos;
    volatile u_g0_dlpos g0_dlpos;


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART01_H_ */
