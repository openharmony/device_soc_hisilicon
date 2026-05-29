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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART05_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART05_H_

    bpp = (td_u32)tde_hal_getbpp_by_fmt(drv_surface->color_format);
    phy_addr = drv_surface->phys_addr + (td_u64)(drv_surface->ypos) * (td_u64)(drv_surface->pitch) +
                  (((td_u64)(drv_surface->xpos) * (td_u64)(bpp)) >> 3); /* 3 bpp narrow 8 */
    hw_node->des_ch0_addr_low.bits.des_ch0_addr_low = get_low_addr(phy_addr);
    hw_node->des_ch0_addr_high.bits.des_ch0_addr_high = get_high_addr(phy_addr);

    cb_crphy_addr = drv_surface->cbcr_phys_addr + (td_u64)(drv_surface->ypos) * (td_u64)(drv_surface->pitch) +
                    (((td_u64)(drv_surface->xpos) * (td_u64)(bpp)) >> 3); /* 3 bpp narrow 8 */
    hw_node->des_ch1_addr_low.bits.des_ch1_addr_low = get_low_addr(cb_crphy_addr);
    hw_node->des_ch1_addr_high.bits.des_ch1_addr_hi = get_high_addr(cb_crphy_addr);

    hw_node->des_ch1_stride.bits.des_ch1_stride = (td_u32)drv_surface->pitch;

    if (drv_surface->color_format == TDE_DRV_COLOR_FMT_YCBCR422) {
        hw_node->des_dswm.bits.des_h_dswm_mode = 1;
    }
    hw_node->des_ch0_stride.bits.des_ch0_stride = (td_u32)drv_surface->pitch;
    return;
}

td_void tde_hal_node_set_corner_rect(tde_hw_node *hw_node, const tde_corner_rect_info *corner_hal_info)
{
    if ((hw_node != TD_NULL) && (corner_hal_info != TD_NULL)) {
        hw_node->tde_dma_corner_reso.bits.dma_corner_width = corner_hal_info->width - 1;
        hw_node->tde_dma_corner_reso.bits.dma_corner_height = corner_hal_info->height - 1;

        hw_node->tde_dma_corner_ctrl.bits.dma_corner_en = 0x1;
        hw_node->tde_dma_corner_ctrl.bits.dma_des_en = 0x1;
#if defined CONFIG_TDE_DMA_CORNER_V2
        hw_node->tde_dma_other_value.bits.dma_other_value = corner_hal_info->outer_color;
        hw_node->tde_dma_corner_value.bits.dma_corner_value = corner_hal_info->inner_color;
#elif defined CONFIG_TDE_DMA_CORNER_V1
        hw_node->tde_dma_corner_ctrl.bits.dma_other_value = corner_hal_info->outer_color;
        hw_node->tde_dma_corner_ctrl.bits.dma_corner_value = corner_hal_info->inner_color;
#endif
    }

    return;
}

#ifdef CONFIG_TDE_DRD_LINE_SUPPORT
td_void tde_hal_node_draw_line(tde_hw_node *hw_node, const tde_line_info *line_hal_info, td_u32 length)
{
    if (length < TDE_MAX_LINE_NUM) {
        return;
    }

    if ((hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_PKGVYUY) ||
        (hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_YCBCR888) ||
        (hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_AYCBCR8888) ||
        (hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_YCBCR422)) {
        hw_node->tde_drd_mask.bits.align_mode = 0; /* 0 mode for yuv */
    } else {
        hw_node->tde_drd_mask.bits.align_mode = 0x2; /* 0x2 mode for non yuv */
    }

    hw_node->tde_drd_mask.bits.drd_enable = 0x1;
    hw_node->tde_drd_mask.bits.line0_enable = line_hal_info[0].enable;
    hw_node->tde_drd_mask.bits.line1_enable = line_hal_info[1].enable;
    hw_node->tde_drd_mask.bits.line2_enable = line_hal_info[2].enable; /* 2 line2 */
    hw_node->tde_drd_mask.bits.line3_enable = line_hal_info[3].enable; /* 3 line3 */

    hw_node->tde_drd_line_width.bits.line0_width = line_hal_info[0].thick - 1;
    hw_node->tde_drd_line_width.bits.line1_width = line_hal_info[1].thick - 1;
    hw_node->tde_drd_line_width.bits.line2_width = line_hal_info[2].thick - 1; /* 2 line2 */
    hw_node->tde_drd_line_width.bits.line3_width = line_hal_info[3].thick - 1; /* 3 line3 */

    hw_node->tde_drd_line0_color_cfg.bits.line0_color_fill = line_hal_info[0].color;
    hw_node->tde_drd_line1_color_cfg.bits.line1_color_fill = line_hal_info[1].color;
    hw_node->tde_drd_line2_color_cfg.bits.line2_color_fill = line_hal_info[2].color; /* 2 line2 */
    hw_node->tde_drd_line3_color_cfg.bits.line3_color_fill = line_hal_info[3].color; /* 3 line3 */

    hw_node->tde_drd_line0_st.bits.line0_st_x = line_hal_info[0].start_x;
    hw_node->tde_drd_line0_st.bits.line0_st_y = line_hal_info[0].start_y;
    hw_node->tde_drd_line0_ed.bits.line0_ed_x = line_hal_info[0].end_x;
    hw_node->tde_drd_line0_ed.bits.line0_ed_y = line_hal_info[0].end_y;

    hw_node->tde_drd_line1_st.bits.line1_st_x = line_hal_info[1].start_x;
    hw_node->tde_drd_line1_st.bits.line1_st_y = line_hal_info[1].start_y;
    hw_node->tde_drd_line1_ed.bits.line1_ed_x = line_hal_info[1].end_x;
    hw_node->tde_drd_line1_ed.bits.line1_ed_y = line_hal_info[1].end_y;

    hw_node->tde_drd_line2_st.bits.line2_st_x = line_hal_info[2].start_x; /* 2 line2 */
    hw_node->tde_drd_line2_st.bits.line2_st_y = line_hal_info[2].start_y; /* 2 line2 */
    hw_node->tde_drd_line2_ed.bits.line2_ed_x = line_hal_info[2].end_x; /* 2 line2 */
    hw_node->tde_drd_line2_ed.bits.line2_ed_y = line_hal_info[2].end_y; /* 2 line2 */

    hw_node->tde_drd_line3_st.bits.line3_st_x = line_hal_info[3].start_x; /* 3 line3 */
    hw_node->tde_drd_line3_st.bits.line3_st_y = line_hal_info[3].start_y; /* 3 line3 */
    hw_node->tde_drd_line3_ed.bits.line3_ed_x = line_hal_info[3].end_x; /* 3 line3 */
    hw_node->tde_drd_line3_ed.bits.line3_ed_y = line_hal_info[3].end_y; /* 3 line3 */
    return;
}
#endif


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART05_H_ */
