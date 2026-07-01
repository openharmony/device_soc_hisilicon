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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART03_H_

        lane_id = lane_id - MIPI_RX_PHY_LANE_NUM;
    } else { /* dev3 */
        lane_id = (lane_id - MIPI_RX_PHY_LANE_NUM) / 2; /* 2: 1, 3 convert to 0, 1 */
    }

    switch (lane_id) {
        case 0: /* lane 0 */
            lane_id0_chn.bits.lane0_id = lane_idx;
            break;

        case 1: /* lane 1 */
            lane_id0_chn.bits.lane1_id = lane_idx;
            break;

        case 2: /* lane 2 */
            lane_id0_chn.bits.lane2_id = lane_idx;
            break;

        case 3: /* lane 3 */
            lane_id0_chn.bits.lane3_id = lane_idx;
            break;

        case 4: /* lane 4 */
            lane_id1_chn.bits.lane4_id = lane_idx;
            break;

        case 5: /* lane 5 */
            lane_id1_chn.bits.lane5_id = lane_idx;
            break;

        case 6: /* lane 6 */
            lane_id1_chn.bits.lane6_id = lane_idx;
            break;

        case 7: /* lane 7 */
            lane_id1_chn.bits.lane7_id = lane_idx;
            break;

        default:
            break;
    }

    global_ctrl_regs->lane_id0_chn.u32 = lane_id0_chn.u32;
    global_ctrl_regs->lane_id1_chn.u32 = lane_id1_chn.u32;
}

void mipi_rx_drv_set_link_lane_id(combo_dev_t devno, input_mode_t input_mode, const short *p_lane_id)
{
    int i;
    int lane_num;

    if (input_mode == INPUT_MODE_MIPI) {
        lane_num = MIPI_LANE_NUM;
    } else {
        lane_num = LVDS_LANE_NUM;
    }

    for (i = 0; i < lane_num; i++) {
        if (is_valid_id(p_lane_id[i])) {
            mipi_rx_set_lane_id(devno, i, p_lane_id[i]);
        }
    }
}

void mipi_rx_drv_set_mem_cken(combo_dev_t devno, int enable)
{
    u_chn0_mem_ctrl chn0_mem_ctrl;
    u_chn1_mem_ctrl chn1_mem_ctrl;
    u_chn2_mem_ctrl chn2_mem_ctrl;
    u_chn3_mem_ctrl chn3_mem_ctrl;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    switch (devno) {
        case 0: /* chn 0 */
            chn0_mem_ctrl.u32 = mipi_rx_sys_regs->chn0_mem_ctrl.u32;
            chn0_mem_ctrl.bits.chn0_mem_ck_gt = enable;
            mipi_rx_sys_regs->chn0_mem_ctrl.u32 = chn0_mem_ctrl.u32;
            break;
        case 1: /* chn 1 */
            chn1_mem_ctrl.u32 = mipi_rx_sys_regs->chn1_mem_ctrl.u32;
            chn1_mem_ctrl.bits.chn1_mem_ck_gt = enable;
            mipi_rx_sys_regs->chn1_mem_ctrl.u32 = chn1_mem_ctrl.u32;
            break;
        case 2: /* chn 2 */
            chn2_mem_ctrl.u32 = mipi_rx_sys_regs->chn2_mem_ctrl.u32;
            chn2_mem_ctrl.bits.chn2_mem_ck_gt = enable;
            mipi_rx_sys_regs->chn2_mem_ctrl.u32 = chn2_mem_ctrl.u32;
            break;
        case 3: /* chn 3 */
            chn3_mem_ctrl.u32 = mipi_rx_sys_regs->chn3_mem_ctrl.u32;
            chn3_mem_ctrl.bits.chn3_mem_ck_gt = enable;
            mipi_rx_sys_regs->chn3_mem_ctrl.u32 = chn3_mem_ctrl.u32;
            break;
        default:
            break;
    }
}

void mipi_rx_drv_set_clr_cken(combo_dev_t devno, int enable)
{
    u_chn0_clr_en chn0_clr_en;
    u_chn1_clr_en chn1_clr_en;
    u_chn2_clr_en chn2_clr_en;
    u_chn3_clr_en chn3_clr_en;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    switch (devno) {
        case 0: /* chn 0 */
            chn0_clr_en.u32 = mipi_rx_sys_regs->chn0_clr_en.u32;
            chn0_clr_en.bits.chn0_clr_en_lvds = enable;
            chn0_clr_en.bits.chn0_clr_en_align = enable;
            mipi_rx_sys_regs->chn0_clr_en.u32 = chn0_clr_en.u32;
            break;
        case 1: /* chn 1 */
            chn1_clr_en.u32 = mipi_rx_sys_regs->chn1_clr_en.u32;
            chn1_clr_en.bits.chn1_clr_en_lvds = enable;
            chn1_clr_en.bits.chn1_clr_en_align = enable;
            mipi_rx_sys_regs->chn1_clr_en.u32 = chn1_clr_en.u32;
            break;
        case 2: /* chn 2 */
            chn2_clr_en.u32 = mipi_rx_sys_regs->chn2_clr_en.u32;
            chn2_clr_en.bits.chn2_clr_en_lvds = enable;
            chn2_clr_en.bits.chn2_clr_en_align = enable;
            mipi_rx_sys_regs->chn2_clr_en.u32 = chn2_clr_en.u32;
            break;
        case 3: /* chn 3 */
            chn3_clr_en.u32 = mipi_rx_sys_regs->chn3_clr_en.u32;
            chn3_clr_en.bits.chn3_clr_en_lvds = enable;
            chn3_clr_en.bits.chn3_clr_en_align = enable;
            mipi_rx_sys_regs->chn3_clr_en.u32 = chn3_clr_en.u32;
            break;
        default:
            break;
    }
}

/* magic numbers convert to register configs  */
static void mipi_rx_drv_set_phy_en_link(int phy_id, unsigned int lane_bitmap)
{
    u_phy_en_link phy_en_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_en_link.u32 = mipi_rx_phy_cfg->phy_en_link.u32;

    if (lane_bitmap & 0x5) {
        phy_en_link.bits.phy_da_d0_valid = lane_bitmap & 0x1;
        phy_en_link.bits.phy_da_d2_valid = (lane_bitmap & 0x4) >> MIPI_RX_HAL_SHIFT_2;
        phy_en_link.bits.phy_d0_term_en = lane_bitmap & 0x1;
        phy_en_link.bits.phy_d2_term_en = (lane_bitmap & 0x4) >> MIPI_RX_HAL_SHIFT_2;
        phy_en_link.bits.phy_clk_term_en = 1;
    }

    if (lane_bitmap & 0xa) {
        phy_en_link.bits.phy_da_d1_valid = (lane_bitmap & 0x2) >> 1;
        phy_en_link.bits.phy_da_d3_valid = (lane_bitmap & 0x8) >> MIPI_RX_HAL_SHIFT_3;
        phy_en_link.bits.phy_d1_term_en = (lane_bitmap & 0x2) >> 1;
        phy_en_link.bits.phy_d3_term_en = (lane_bitmap & 0x8) >> MIPI_RX_HAL_SHIFT_3;
        phy_en_link.bits.phy_clk2_term_en = 1;
    }

    mipi_rx_phy_cfg->phy_en_link.u32 = phy_en_link.u32;
}

static void mipi_rx_drv_set_phy_mode(int phy_id, input_mode_t input_mode, unsigned int lane_bitmap)
{
    u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    const int cmos_en = 0;

    mipi_rx_unused(input_mode);

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;

    phy_mode_link.bits.phy_rg_en_d = phy_mode_link.bits.phy_rg_en_d | (lane_bitmap & 0xf);
    phy_mode_link.bits.phy_rg_en_cmos = cmos_en;
    phy_mode_link.bits.phy_rg_en_clk = 1;
    phy_mode_link.bits.phy_rg_mipi_mode = 1;

    if (lane_bitmap & 0xa) {
        phy_mode_link.bits.phy_rg_en_clk2 = 1;
        phy_mode_link.bits.phy_rg_mipi_mode2 = 1;
    }

    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_drv_set_phy_en(unsigned int lane_bitmap)
{
    u_phy_en phy_en;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    phy_en.u32 = mipi_rx_sys_regs->phy_en.u32;

    if (lane_bitmap & 0xf) {
        phy_en.bits.phy0_en = 1;
    }

    if (lane_bitmap & 0xf0) {
        phy_en.bits.phy1_en = 1;
    }

    if (lane_bitmap & 0xf00) {
        phy_en.bits.phy2_en = 1;
    }

    mipi_rx_sys_regs->phy_en.u32 = phy_en.u32;
}

static void mipi_rx_drv_set_lane_en(unsigned int lane_bitmap, unsigned lane_mask)
{
    u_lane_en lane_en;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    lane_en.u32 = mipi_rx_sys_regs->lane_en.u32;
    lane_en.u32 = lane_en.u32 & (~lane_mask);
    lane_en.u32 = lane_en.u32 | (lane_bitmap & lane_mask);
    mipi_rx_sys_regs->lane_en.u32 = lane_en.u32;
}

static void mipi_rx_drv_set_phy_cil_en(unsigned int lane_bitmap, int enable)
{
    u_phy_cil_ctrl phy_cil_ctrl;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    phy_cil_ctrl.u32 = mipi_rx_sys_regs->phy_cil_ctrl.u32;

    if (lane_bitmap & 0xf) {
        phy_cil_ctrl.bits.phycil0_cken = enable;
    }

    if (lane_bitmap & 0xf0) {
        phy_cil_ctrl.bits.phycil1_cken = enable;
    }

    if (lane_bitmap & 0xf00) {
        phy_cil_ctrl.bits.phycil2_cken = enable;
    }

    mipi_rx_sys_regs->phy_cil_ctrl.u32 = phy_cil_ctrl.u32;
}

static void mipi_rx_drv_set_phy_cfg_mode(input_mode_t input_mode, unsigned int lane_bitmap)
{
    u_phycfg_mode phycfg_mode;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;
    unsigned int cfg_mode;
    unsigned int cfg_mode_sel;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    phycfg_mode.u32 = mipi_rx_sys_regs->phycfg_mode.u32;

    if (input_mode == INPUT_MODE_MIPI) {
        cfg_mode = 0;
        cfg_mode_sel = 0;
    } else if (input_mode == INPUT_MODE_SUBLVDS ||
               input_mode == INPUT_MODE_LVDS ||
               input_mode == INPUT_MODE_HISPI) {
        cfg_mode = 1;
        cfg_mode_sel = 0;
    } else {
        cfg_mode = 2; /* 2: other input_mode */
        cfg_mode_sel = 1;
    }

    if (lane_bitmap & 0x5) {
        phycfg_mode.bits.phycil0_0_cfg_mode = cfg_mode;
        phycfg_mode.bits.phycil0_cfg_mode_sel = cfg_mode_sel;
    }

    if (lane_bitmap & 0xa) {
        phycfg_mode.bits.phycil0_1_cfg_mode = cfg_mode;
        phycfg_mode.bits.phycil0_cfg_mode_sel = cfg_mode_sel;
    }

    if (lane_bitmap & 0x50) {
        phycfg_mode.bits.phycil1_0_cfg_mode = cfg_mode;
        phycfg_mode.bits.phycil1_cfg_mode_sel = cfg_mode_sel;
    }

    if (lane_bitmap & 0xa0) {
        phycfg_mode.bits.phycil1_1_cfg_mode = cfg_mode;
        phycfg_mode.bits.phycil1_cfg_mode_sel = cfg_mode_sel;
    }

    mipi_rx_sys_regs->phycfg_mode.u32 = phycfg_mode.u32;
}

static void mipi_rx_drv_set_phy_cfg_en(unsigned int lane_bitmap, int enable)
{
    u_phycfg_en phycfg_en;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    phycfg_en.u32 = mipi_rx_sys_regs->phycfg_en.u32;

    if (lane_bitmap & 0xf) {
        phycfg_en.bits.phycil0_cfg_en = enable;
    }

    if (lane_bitmap & 0xf0) {
        phycfg_en.bits.phycil1_cfg_en = enable;
    }

    if (lane_bitmap & 0xf00) {
        phycfg_en.bits.phycil2_cfg_en = enable;
    }

    mipi_rx_sys_regs->phycfg_en.u32 = phycfg_en.u32;
}

void mipi_rx_drv_set_phy_config(input_mode_t input_mode, unsigned int lane_bitmap, unsigned int lane_mask)
{
    unsigned int i;
    unsigned int mask;
    unsigned int phy_lane_bitmap;

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mask = 0xf << (MIPI_RX_PHY_LANE_NUM * i);
        if (lane_bitmap & mask) {
            phy_lane_bitmap = (lane_bitmap & mask) >> (MIPI_RX_PHY_LANE_NUM * i);
            mipi_rx_drv_set_phy_en_link(i, phy_lane_bitmap);
            mipi_rx_drv_set_phy_mode(i, input_mode, phy_lane_bitmap);
        }
    }

    mipi_rx_drv_set_phy_en(lane_bitmap);
    mipi_rx_drv_set_lane_en(lane_bitmap, lane_mask);
    mipi_rx_drv_set_phy_cil_en(lane_bitmap, 1);
    mipi_rx_drv_set_phy_cfg_mode(input_mode, lane_bitmap);
    mipi_rx_drv_set_phy_cfg_en(lane_bitmap, 1);
}

static void mipi_rx_drv_set_phy_cmv(int phy_id, phy_cmv_mode_t cmv_mode, unsigned int lane_bitmap)
{
    int mipi_cmv_mode = 0;
    u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg = NULL;

    if (cmv_mode == PHY_CMV_GE1200MV) {
        mipi_cmv_mode = 0;
    } else if (cmv_mode == PHY_CMV_LT1200MV) {
        mipi_cmv_mode = 1;
    }

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;

    if (lane_bitmap & 0xa) {
        phy_mode_link.bits.phy_rg_mipi_mode2 = mipi_cmv_mode;
    }

    if (lane_bitmap & 0x5) {
        phy_mode_link.bits.phy_rg_mipi_mode = mipi_cmv_mode;
    }

    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

void mipi_rx_drv_set_phy_cmvmode(input_mode_t input_mode, phy_cmv_mode_t cmv_mode, unsigned int lane_bitmap)
{
    unsigned int i;
    unsigned int mask;
    unsigned int phy_lane_bitmap;

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mask = 0xf << (MIPI_RX_PHY_LANE_NUM * i);
        if (lane_bitmap & mask) {
            phy_lane_bitmap = (lane_bitmap & mask) >> (MIPI_RX_PHY_LANE_NUM * i);
            mipi_rx_drv_set_phy_cmv(i, cmv_mode, phy_lane_bitmap);
        }
    }

    mipi_rx_drv_set_phy_cfg_mode(input_mode, lane_bitmap);
    mipi_rx_drv_set_phy_cfg_en(lane_bitmap, 1);
}

void mipi_rx_drv_set_lvds_image_rect(combo_dev_t devno, const img_rect_t *p_img_rect, short total_lane_num)
{
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;
    u_lvds_imgsize lvds_img_size;
    u_lvds_crop_start0 crop_start0;
    u_lvds_crop_start1 crop_start1;
    u_lvds_crop_start2 crop_start2;
    u_lvds_crop_start3 crop_start3;
    unsigned int width_per_lane;
    unsigned int x_per_lane;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    if (total_lane_num == 0) {
        return;
    }

    width_per_lane = (p_img_rect->width / total_lane_num);
    x_per_lane = (p_img_rect->x / total_lane_num);

    lvds_img_size.u32 = ctrl_reg_attr->lvds_imgsize.u32;
    crop_start0.u32 = ctrl_reg_attr->lvds_crop_start0.u32;
    crop_start1.u32 = ctrl_reg_attr->lvds_crop_start1.u32;
    crop_start2.u32 = ctrl_reg_attr->lvds_crop_start2.u32;
    crop_start3.u32 = ctrl_reg_attr->lvds_crop_start3.u32;

    lvds_img_size.bits.lvds_imgwidth_lane = width_per_lane - 1;
    lvds_img_size.bits.lvds_imgheight = p_img_rect->height - 1;

    crop_start0.bits.lvds_start_x0_lane = x_per_lane;
    crop_start0.bits.lvds_start_y0 = p_img_rect->y;

    crop_start1.bits.lvds_start_x1_lane = x_per_lane;
    crop_start1.bits.lvds_start_y1 = p_img_rect->y;

    crop_start2.bits.lvds_start_x2_lane = x_per_lane;
    crop_start2.bits.lvds_start_y2 = p_img_rect->y;

    crop_start3.bits.lvds_start_x3_lane = x_per_lane;
    crop_start3.bits.lvds_start_y3 = p_img_rect->y;

    ctrl_reg_attr->lvds_imgsize.u32 = lvds_img_size.u32;
    ctrl_reg_attr->lvds_crop_start0.u32 = crop_start0.u32;
    ctrl_reg_attr->lvds_crop_start1.u32 = crop_start1.u32;
    ctrl_reg_attr->lvds_crop_start2.u32 = crop_start2.u32;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART03_H_ */
