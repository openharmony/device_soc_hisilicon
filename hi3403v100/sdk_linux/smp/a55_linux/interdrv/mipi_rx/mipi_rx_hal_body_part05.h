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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART05_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART05_H_

        phy_sync_sof3_link.bits.cil_sof0_word4_3 = n_sync_code[sensor_lane_idx][0][0];
        phy_sync_sof3_link.bits.cil_sof1_word4_3 = nxt_sync_code[sensor_lane_idx][0][0];
    }

    mipi_rx_phy_cfg->phy_sync_sof0_link.u32 = phy_sync_sof0_link.u32;
    mipi_rx_phy_cfg->phy_sync_sof1_link.u32 = phy_sync_sof1_link.u32;
    mipi_rx_phy_cfg->phy_sync_sof2_link.u32 = phy_sync_sof2_link.u32;
    mipi_rx_phy_cfg->phy_sync_sof3_link.u32 = phy_sync_sof3_link.u32;
}

void mipi_rx_drv_set_phy_sync_config(const lvds_dev_attr_t *p_attr, unsigned int lane_bitmap,
                                     const unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    int raw_type;
    unsigned int i;
    unsigned int mask;
    unsigned int phy_lane_bitmap;

    switch (p_attr->input_data_type) {
        case DATA_TYPE_RAW_8BIT:
            raw_type = 0x1;
            break;

        case DATA_TYPE_RAW_10BIT:
            raw_type = 0x2;
            break;

        case DATA_TYPE_RAW_12BIT:
            raw_type = 0x3;
            break;

        case DATA_TYPE_RAW_14BIT:
            raw_type = 0x4;
            break;

        case DATA_TYPE_RAW_16BIT:
            raw_type = 0x5;
            break;

        default:
            return;
    }

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mask = 0xf << (MIPI_RX_PHY_LANE_NUM * i);
        if (lane_bitmap & mask) {
            phy_lane_bitmap = (lane_bitmap & mask) >> (MIPI_RX_PHY_LANE_NUM * i);
            mipi_rx_drv_set_phy_sync_dct(i, raw_type, p_attr->sync_code_endian, phy_lane_bitmap);
            mipi_rx_drv_set_lvds_phy_sync_code(i, p_attr->lane_id, p_attr->sync_code, nxt_sync_code, phy_lane_bitmap);
        }
    }
}

unsigned int mipi_rx_drv_get_lane_mask(combo_dev_t devno, lane_divide_mode_t mode)
{
    unsigned int mask = 0x0;

    switch (mode) {
        case LANE_DIVIDE_MODE_0:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0xff;
            }
            break;
        case LANE_DIVIDE_MODE_1:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0xf;
            } else if (devno == 2) { /* mipi_dev 2 */
                mask = 0xf0;
            }
            break;
        case LANE_DIVIDE_MODE_2:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0xf;
            } else if (devno == 2) { /* mipi_dev 2 */
                mask = 0x50;
            } else if (devno == 3) { /* mipi_dev 3 */
                mask = 0xa0;
            }
            break;
        case LANE_DIVIDE_MODE_3:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0x5;
            } else if (devno == 1) { /* mipi_dev 1 */
                mask = 0xa;
            } else if (devno == 2) { /* mipi_dev 2 */
                mask = 0x50;
            } else if (devno == 3) { /* mipi_dev 3 */
                mask = 0xa0;
            }
            break;
        default:
            break;
    }

    return mask;
}

static int mipi_rx_is_lane_valid_mode1(combo_dev_t devno, short lane_id)
{
    if (devno == 0) {
        return (lane_id >= 0 && lane_id <= MIPI_RX_HAL_VALUE_4) ? 1 : 0;
    }
    if (devno == MIPI_RX_HAL_VALUE_2) {
        return (lane_id >= MIPI_RX_HAL_VALUE_4 && lane_id <= MIPI_RX_HAL_VALUE_7) ? 1 : 0;
    }
    return 0;
}

static int mipi_rx_is_lane_valid_mode2(combo_dev_t devno, short lane_id)
{
    if (devno == 0) {
        return (lane_id >= 0 && lane_id <= MIPI_RX_HAL_VALUE_4) ? 1 : 0;
    }
    if (devno == MIPI_RX_HAL_VALUE_2) {
        return (lane_id == MIPI_RX_HAL_VALUE_4 || lane_id == MIPI_RX_HAL_VALUE_6) ? 1 : 0;
    }
    if (devno == MIPI_RX_HAL_VALUE_3) {
        return (lane_id == MIPI_RX_HAL_VALUE_5 || lane_id == MIPI_RX_HAL_VALUE_7) ? 1 : 0;
    }
    return 0;
}

static int mipi_rx_is_lane_valid_mode3(combo_dev_t devno, short lane_id)
{
    if (devno == 0) {
        return (lane_id == 0 || lane_id == MIPI_RX_HAL_VALUE_2) ? 1 : 0;
    }
    if (devno == 1) {
        return (lane_id == 1 || lane_id == MIPI_RX_HAL_VALUE_3) ? 1 : 0;
    }
    if (devno == MIPI_RX_HAL_VALUE_2) {
        return (lane_id == MIPI_RX_HAL_VALUE_4 || lane_id == MIPI_RX_HAL_VALUE_6) ? 1 : 0;
    }
    if (devno == MIPI_RX_HAL_VALUE_3) {
        return (lane_id == MIPI_RX_HAL_VALUE_5 || lane_id == MIPI_RX_HAL_VALUE_7) ? 1 : 0;
    }
    return 0;
}

/* magic numbers means land_id and mipi_dev */
int mipi_rx_drv_is_lane_valid(combo_dev_t devno, short lane_id, lane_divide_mode_t mode)
{
    switch (mode) {
        case LANE_DIVIDE_MODE_0:
            return (devno == 0 && lane_id >= 0 && lane_id <= MIPI_RX_HAL_VALUE_7) ? 1 : 0;
        case LANE_DIVIDE_MODE_1:
            return mipi_rx_is_lane_valid_mode1(devno, lane_id);
        case LANE_DIVIDE_MODE_2:
            return mipi_rx_is_lane_valid_mode2(devno, lane_id);
        case LANE_DIVIDE_MODE_3:
            return mipi_rx_is_lane_valid_mode3(devno, lane_id);
        default:
            return 0;
    }
}

static void mipi_rx_drv_hw_init(void);
void mipi_rx_drv_set_hs_mode(lane_divide_mode_t mode)
{
    u_hs_mode_select hs_mode_sel;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs = NULL;
    unsigned int i;

    mipi_rx_drv_hw_init();

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mipi_rx_set_phy_rg_ext_en(i, g_phy_mode[mode][i].phy_rg_ext_en);
        mipi_rx_set_phy_rg_ext2_en(i, g_phy_mode[mode][i].phy_rg_ext2_en);
        mipi_rx_set_phy_rg_int_en(i, g_phy_mode[mode][i].phy_rg_int_en);
        mipi_rx_set_phy_rg_drveclk2_enz(i, g_phy_mode[mode][i].phy_rg_drveclk2_enz);
        mipi_rx_set_phy_rg_drveclk_enz(i, g_phy_mode[mode][i].phy_rg_drveclk_enz);
    }

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    hs_mode_sel.u32 = mipi_rx_sys_regs->hs_mode_select.u32;
    hs_mode_sel.bits.hs_mode = g_hs_mode[mode];
    mipi_rx_sys_regs->hs_mode_select.u32 = hs_mode_sel.u32;
}

void mipi_rx_drv_set_chn_int_mask(combo_dev_t devno)
{
    u_mipi_int_msk mipi_int_msk;
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    mipi_int_msk.u32 = mipi_rx_sys_regs->mipi_int_msk.u32;

    if (devno == 0) { /* chn 0 */
        mipi_int_msk.bits.int_chn0_mask = 0x1;
    } else if (devno == 1) { /* chn 1 */
        mipi_int_msk.bits.int_chn1_mask = 0x1;
    } else if (devno == 2) { /* chn 2 */
        mipi_int_msk.bits.int_chn2_mask = 0x1;
    } else if (devno == 3) { /* chn 3 */
        mipi_int_msk.bits.int_chn3_mask = 0x1;
    }

    mipi_rx_sys_regs->mipi_int_msk.u32 = mipi_int_msk.u32;
}

void mipi_rx_drv_set_lvds_ctrl_int_mask(combo_dev_t devno, unsigned int mask)
{
    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    lvds_ctrl_regs->lvds_ctrl_int_msk.u32 = mask;
}

void mipi_rx_drv_set_mipi_ctrl_int_mask(combo_dev_t devno, unsigned int mask)
{
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_ctrl_regs->mipi_ctrl_int_msk.u32 = mask;
}

void mipi_rx_drv_set_mipi_csi_int_mask(combo_dev_t devno, unsigned int mask)
{
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);
    mipi_ctrl_regs->mipi_crc_intr_msk.u32 = mask;
}

void mipi_rx_drv_set_align_int_mask(combo_dev_t devno, unsigned int mask)
{
    volatile global_ctrl_regs_t *global_ctrl_regs = get_global_ctrl_regs(devno);

    global_ctrl_regs->align_int_msk.u32 = mask;
    global_ctrl_regs->chn_int_mask.u32 = 0xf;
}

static void mipi_rx_enable_disable_clock(combo_dev_t combo_dev, int enable)
{
    unsigned long mipi_rx_clock_addr;

    mipi_rx_clock_addr = (unsigned long)osal_ioremap(mipi_rx_pixel_crg_addr(combo_dev), (unsigned long)0x4);
    if (mipi_rx_clock_addr == NULL) {
        ot_err("mipi_rx clock ioremap failed!\n");
        return;
    }
    set_bit(enable, 4, mipi_rx_clock_addr); /* 4 : offset */
    osal_iounmap((void *)mipi_rx_clock_addr, (unsigned long)0x4); // len is 0x4
}

void mipi_rx_drv_enable_clock(combo_dev_t combo_dev)
{
    mipi_rx_enable_disable_clock(combo_dev, 1);
}

void mipi_rx_drv_disable_clock(combo_dev_t combo_dev)
{
    mipi_rx_enable_disable_clock(combo_dev, 0);
}

static void sensor_enable_disable_clock(sns_clk_source_t sns_clk_source, int enable)
{
    unsigned long sensor_clock_addr;
    const unsigned offset = 4;

    sensor_clock_addr = (unsigned long)osal_ioremap(sns_crg_addr(sns_clk_source), (unsigned long)0x4);
    if (sensor_clock_addr == NULL) {
        ot_err("sensor clock ioremap failed!\n");
        return;
    }

    set_bit(enable, offset, sensor_clock_addr);

    osal_iounmap((void *)sensor_clock_addr, (unsigned long)0x4); // len is 0x4
}

void sensor_drv_enable_clock(sns_clk_source_t sns_clk_source)
{
    sensor_enable_disable_clock(sns_clk_source, 1);
}

void sensor_drv_disable_clock(sns_clk_source_t sns_clk_source)
{
    sensor_enable_disable_clock(sns_clk_source, 0);
}

static void mipi_rx_core_reset_unreset(combo_dev_t combo_dev, int reset)
{
    unsigned long mipi_rx_reset_addr;
    const unsigned offset = 0;

#ifndef OT_FPGA
    mipi_rx_reset_addr = (unsigned long)osal_ioremap(mipi_rx_pixel_crg_addr(combo_dev), (unsigned long)0x4);
    if (mipi_rx_reset_addr == NULL) {
        ot_err("mipi_rx clock ioremap failed!\n");
        return;
    }
    set_bit(reset, offset, mipi_rx_reset_addr);
    osal_iounmap((void *)mipi_rx_reset_addr, (unsigned long)0x4); // len is 0x4
#else
    mipi_rx_reset_addr = (unsigned long)osal_ioremap(MIPI_RX_RST_CRG_ADDR, (unsigned long)0x4);
    if (mipi_rx_reset_addr == NULL) {
        ot_err("mipi_rx clock ioremap failed!\n");
        return;
    }
    set_bit(reset, offset + combo_dev, mipi_rx_reset_addr);
    osal_iounmap((void *)mipi_rx_reset_addr, (unsigned long)0x4); // len is 0x4
#endif
}

void mipi_rx_drv_core_reset(combo_dev_t combo_dev)
{
    mipi_rx_core_reset_unreset(combo_dev, 1);
}

void mipi_rx_drv_core_unreset(combo_dev_t combo_dev)
{
    mipi_rx_core_reset_unreset(combo_dev, 0);
}

static void sensor_reset_unreset(sns_rst_source_t sns_reset_source, int reset)
{
    unsigned long sensor_reset_addr;
    const unsigned offset = 0;

#ifndef OT_FPGA
    sensor_reset_addr = (unsigned long)osal_ioremap(sns_crg_addr(sns_reset_source), (unsigned long)0x4);
    if (sensor_reset_addr == NULL) {
        ot_err("sensor reset ioremap failed!\n");
        return;
    }

    set_bit(reset, offset, sensor_reset_addr);
    set_bit(reset, offset + 1, sensor_reset_addr);

    osal_iounmap((void *)sensor_reset_addr, (unsigned long)0x4); // len is 0x4
#else
    sensor_reset_addr = (unsigned long)osal_ioremap(SNS_RST_CRG_ADDR, (unsigned long)0x4);
    if (sensor_reset_addr == NULL) {
        ot_err("sensor reset ioremap failed!\n");
        return;
    }

    set_bit(reset, offset + sns_reset_source, sensor_reset_addr);

    osal_iounmap((void *)sensor_reset_addr, (unsigned long)0x4); // len is 0x4
#endif
}

void sensor_drv_reset(sns_rst_source_t sns_reset_source)
{
    sensor_reset_unreset(sns_reset_source, 1);
}

void sensor_drv_unreset(sns_rst_source_t sns_reset_source)
{
    sensor_reset_unreset(sns_reset_source, 0);
}

void mipi_rx_drv_get_mipi_imgsize_statis(combo_dev_t devno, short vc, img_size_t *p_size)
{
    u_mipi_imgsize0_statis mipi_imgsize0_statis;
    u_mipi_imgsize1_statis mipi_imgsize1_statis;
    u_mipi_imgsize2_statis mipi_imgsize2_statis;
    u_mipi_imgsize3_statis mipi_imgsize3_statis;

    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    if (vc == 0) {
        mipi_imgsize0_statis.u32 = mipi_ctrl_regs->mipi_imgsize0_statis.u32;
        p_size->width = mipi_imgsize0_statis.bits.imgwidth_statis_vc0;
        p_size->height = mipi_imgsize0_statis.bits.imgheight_statis_vc0;
    } else if (vc == 1) {
        mipi_imgsize1_statis.u32 = mipi_ctrl_regs->mipi_imgsize1_statis.u32;
        p_size->width = mipi_imgsize1_statis.bits.imgwidth_statis_vc1;
        p_size->height = mipi_imgsize1_statis.bits.imgheight_statis_vc1;
    } else if (vc == 2) { /* 2: vc2 */
        mipi_imgsize2_statis.u32 = mipi_ctrl_regs->mipi_imgsize2_statis.u32;
        p_size->width = mipi_imgsize2_statis.bits.imgwidth_statis_vc2;
        p_size->height = mipi_imgsize2_statis.bits.imgheight_statis_vc2;
    } else if (vc == 3) { /* 3: vc3 */
        mipi_imgsize3_statis.u32 = mipi_ctrl_regs->mipi_imgsize3_statis.u32;
        p_size->width = mipi_imgsize3_statis.bits.imgwidth_statis_vc3;
        p_size->height = mipi_imgsize3_statis.bits.imgheight_statis_vc3;
    }
}

void mipi_rx_drv_get_lvds_imgsize_statis(combo_dev_t devno, short vc, img_size_t *p_size)
{
    u_lvds_imgsize0_statis lvds_imgsize0_statis;
    u_lvds_imgsize1_statis lvds_imgsize1_statis;
    u_lvds_imgsize2_statis lvds_imgsize2_statis;
    u_lvds_imgsize3_statis lvds_imgsize3_statis;

    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    if (vc == 0) {
        lvds_imgsize0_statis.u32 = lvds_ctrl_regs->lvds_imgsize0_statis.u32;
        p_size->width = lvds_imgsize0_statis.bits.lvds_imgwidth0;
        p_size->height = lvds_imgsize0_statis.bits.lvds_imgheight0;
    } else if (vc == 1) {
        lvds_imgsize1_statis.u32 = lvds_ctrl_regs->lvds_imgsize1_statis.u32;
        p_size->width = lvds_imgsize1_statis.bits.lvds_imgwidth1;
        p_size->height = lvds_imgsize1_statis.bits.lvds_imgheight1;
    } else if (vc == 2) { /* 2: vc2 */
        lvds_imgsize2_statis.u32 = lvds_ctrl_regs->lvds_imgsize2_statis.u32;
        p_size->width = lvds_imgsize2_statis.bits.lvds_imgwidth2;
        p_size->height = lvds_imgsize2_statis.bits.lvds_imgheight2;
    } else if (vc == 3) { /* 3: vc3 */
        lvds_imgsize3_statis.u32 = lvds_ctrl_regs->lvds_imgsize3_statis.u32;
        p_size->width = lvds_imgsize3_statis.bits.lvds_imgwidth3;
        p_size->height = lvds_imgsize3_statis.bits.lvds_imgheight3;
    }
}

void mipi_rx_drv_get_lvds_lane_imgsize_statis(combo_dev_t devno, short lane, img_size_t *p_size)
{
    u_lvds_lane_imgsize_statis lvds_lane_imgsize_statis;

    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    lvds_lane_imgsize_statis.u32 = lvds_ctrl_regs->lvds_lane_imgsize_statis[lane].u32;
    p_size->width = lvds_lane_imgsize_statis.bits.lane_imgwidth + 1;
    p_size->height = lvds_lane_imgsize_statis.bits.lane_imgheight;
}

static void mipi_rx_phy_cil_int_statis(int phy_id)
{
    unsigned int phy_int_status;

    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_int_status = mipi_rx_phy_cfg->mipi_cil_int_link.u32;

    if (phy_int_status) {
        mipi_rx_phy_cfg->mipi_cil_int_raw_link.u32 = 0xffffffff;

        if (phy_int_status & MIPI_ESC_CLK2) {
            g_phy_err_int_cnt[phy_id].clk2_fsm_escape_err_cnt++;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART05_H_ */
