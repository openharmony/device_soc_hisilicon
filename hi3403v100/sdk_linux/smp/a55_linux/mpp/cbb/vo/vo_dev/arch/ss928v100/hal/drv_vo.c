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

#include "drv_vo_comm.h"
#include "drv_vo_dev.h"
#include "drv_vo_video.h"
#include "hal_vo_video.h"
#include "hal_vo_dev_comm.h"
#include "hal_vo_layer_comm.h"
#include "hal_vo_gfx_comm.h"
#include "hal_vo_dev.h"
#include "vou.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

#if vo_desc("pub")

static vo_drv_dev g_vo_drv_dev[OT_VO_MAX_DEV_NUM];
static vo_drv_layer g_vo_drv_layer[OT_VO_MAX_LAYER_NUM];

vo_drv_dev *vo_drv_get_dev_ctx(ot_vo_dev vo_dev)
{
    return &g_vo_drv_dev[vo_dev];
}

vo_drv_layer *vo_drv_get_layer_ctx(ot_vo_layer layer)
{
    return &g_vo_drv_layer[layer];
}

td_void vou_drv_def_layer_bind_dev(td_void)
{
    /* default cbm */
    hal_cbm_set_cbm_attr(HAL_DISP_LAYER_VHD2, VO_DEV_DHD0);
    hal_cbm_set_cbm_attr(HAL_DISP_LAYER_GFX3, VO_DEV_DHD0);

    /* default priority dhd0 */
    vou_drv_set_layer_priority(VO_DEV_DHD0, OT_VO_LAYER_V0, VOU_MIX_PRIO0);
    vou_drv_set_layer_priority(VO_DEV_DHD0, OT_VO_LAYER_V2, VOU_MIX_PRIO1);
    vou_drv_set_layer_priority(VO_DEV_DHD0, OT_VO_LAYER_G0, VOU_MIX_PRIO2);
    vou_drv_set_layer_priority(VO_DEV_DHD0, OT_VO_LAYER_G3, VOU_MIX_PRIO3);

    /* default priority dhd1 */
    vou_drv_set_layer_priority(VO_DEV_DHD1, OT_VO_LAYER_V1, VOU_MIX_PRIO0);
    vou_drv_set_layer_priority(VO_DEV_DHD1, OT_VO_LAYER_G1, VOU_MIX_PRIO2);

    /* need regup */
    hal_disp_set_reg_up(VO_DEV_DHD0);
    hal_disp_set_reg_up(VO_DEV_DHD1);
}

td_void vo_drv_default_setting(td_void)
{
    td_u32 i;
    hfir_coef h_coef[LAYER_VID_END + 1] = {
        { 0x3f5, 0xf, 0x3ec, 0x1c, (0x3d0 + 0x08), 0x3d, 0x395, 0x14a },
        { 0x3f5, 0xf, 0x3ec, 0x1c, (0x3d0 + 0x08), 0x3d, 0x395, 0x14a },
        { 0x3f5, 0xf, 0x3ec, 0x1c, (0x3d0 + 0x08), 0x3d, 0x395, 0x14a }
    };

    /* set dac default cablectr */
    vo_hal_intf_set_dac_cablectr(OT_VO_INTF_CVBS, VO_DAC_CABLE_CTR_DEF);

    /* set each video/graphic layer global alpha */
    for (i = LAYER_VID_START; i <= LAYER_GFX_END; i++) {
        hal_layer_set_layer_global_alpha(i, VO_ALPHA_OPACITY); /* global alpha max 255 */
        hal_video_set_layer_alpha(i, VO_ALPHA_OPACITY); /* alpha max 255 */
    }

    /* select graphic layer's alpha range(0: 0~128,1:0~255) */
    for (i = LAYER_GFX_START; i <= LAYER_GFX_END; i++) {
        hal_gfx_set_pixel_alpha_range(i, 1);
    }

    /* set video layer hfir enable */
    for (i = LAYER_VID_START; i <= LAYER_VID_END; i++) {
        hal_video_hfir_set_hfir_mode(i, HAL_HFIRMODE_COPY);
        hal_video_hfir_set_coef(i, &h_coef[i]);
        hal_video_hfir_set_mid_en(i, TD_TRUE);
    }

    /* set each cross bar default PRI */
    vou_drv_def_layer_bind_dev();

    /* outstanding */
    hal_sys_set_outstanding();

    hal_disp_clear_int_status(VO_INTREPORT_ALL);

    /* only set video layer */
    for (i = LAYER_VID_START; i <= LAYER_VID_END; i++) {
        vou_drv_layer_enable(i, TD_FALSE);
        vou_drv_set_layer_data_fmt(i, VOU_LAYER_PIXEL_FORMAT_SP_YCBCR_422);
    }
}

#endif
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("pub")
static vo_coef_addr g_vo_coef_buf_addr = {0};

vo_coef_addr *vo_drv_get_coef_buf_addr(td_void)
{
    return &g_vo_coef_buf_addr;
}
#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
