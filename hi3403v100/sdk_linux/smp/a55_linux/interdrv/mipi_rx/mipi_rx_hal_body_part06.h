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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART06_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART06_H_
        }
        if (phy_int_status & MIPI_ESC_CLK) {
            g_phy_err_int_cnt[phy_id].clk_fsm_escape_err_cnt++;
        }
        if (phy_int_status & MIPI_ESC_D0) {
            g_phy_err_int_cnt[phy_id].d0_fsm_escape_err_cnt++;
        }
        if (phy_int_status & MIPI_ESC_D1) {
            g_phy_err_int_cnt[phy_id].d1_fsm_escape_err_cnt++;
        }
        if (phy_int_status & MIPI_ESC_D2) {
            g_phy_err_int_cnt[phy_id].d2_fsm_escape_err_cnt++;
        }
        if (phy_int_status & MIPI_ESC_D3) {
            g_phy_err_int_cnt[phy_id].d3_fsm_escape_err_cnt++;
        }
        if (phy_int_status & MIPI_TIMEOUT_CLK2) {
            g_phy_err_int_cnt[phy_id].clk2_fsm_timeout_err_cnt++;
        }
        if (phy_int_status & MIPI_TIMEOUT_CLK) {
            g_phy_err_int_cnt[phy_id].clk_fsm_timeout_err_cnt++;
        }
        if (phy_int_status & MIPI_TIMEOUT_D0) {
            g_phy_err_int_cnt[phy_id].d0_fsm_timeout_err_cnt++;
        }
        if (phy_int_status & MIPI_TIMEOUT_D1) {
            g_phy_err_int_cnt[phy_id].d1_fsm_timeout_err_cnt++;
        }
        if (phy_int_status & MIPI_TIMEOUT_D2) {
            g_phy_err_int_cnt[phy_id].d2_fsm_timeout_err_cnt++;
        }
        if (phy_int_status & MIPI_TIMEOUT_D3) {
            g_phy_err_int_cnt[phy_id].d3_fsm_timeout_err_cnt++;
        }
    }
}
static void mipi_int_statics(combo_dev_t devno)
{
    unsigned int mipi_ctrl_int;
    mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_ctrl_int = mipi_ctrl_regs->mipi_ctrl_int.u32;

    if (mipi_ctrl_int) {
        mipi_ctrl_regs->mipi_ctrl_int_raw.u32 = 0xffffffff;
    }
    if (mipi_ctrl_int & CMD_FIFO_WRITE_ERR) {
        g_mipi_err_int_cnt[devno].cmd_fifo_wrerr_cnt++;
    }
    if (mipi_ctrl_int & DATA_FIFO_WRITE_ERR) {
        g_mipi_err_int_cnt[devno].data_fifo_wrerr_cnt++;
    }
    if (mipi_ctrl_int & CMD_FIFO_READ_ERR) {
        g_mipi_err_int_cnt[devno].cmd_fifo_rderr_cnt++;
    }
    if (mipi_ctrl_int & DATA_FIFO_READ_ERR) {
        g_mipi_err_int_cnt[devno].data_fifo_rderr_cnt++;
    }
}
static void mipi_crc_intr_count1(combo_dev_t devno, unsigned int mipi_crc_intr_int)
{
    if (mipi_crc_intr_int & MIPI_VC0_CRC_ERR) {
        g_mipi_err_int_cnt[devno].vc0_err_crc_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC1_CRC_ERR) {
        g_mipi_err_int_cnt[devno].vc1_err_crc_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC2_CRC_ERR) {
        g_mipi_err_int_cnt[devno].vc2_err_crc_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC3_CRC_ERR) {
        g_mipi_err_int_cnt[devno].vc3_err_crc_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC0_ECC_ERR) {
        g_mipi_err_int_cnt[devno].vc0_err_ecc_corrected_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC1_ECC_ERR) {
        g_mipi_err_int_cnt[devno].vc1_err_ecc_corrected_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC2_ECC_ERR) {
        g_mipi_err_int_cnt[devno].vc2_err_ecc_corrected_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC3_ECC_ERR) {
        g_mipi_err_int_cnt[devno].vc3_err_ecc_corrected_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_MULT_ECC_ERR) {
        g_mipi_err_int_cnt[devno].err_ecc_double_cnt++;
    }
}
static void mipi_crc_intr_count2(combo_dev_t devno, unsigned int mipi_crc_intr_int)
{
    if (mipi_crc_intr_int & MIPI_VC0_FRAME_NUM_ERR) {
        g_mipi_err_int_cnt[devno].vc0_err_frame_num_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC1_FRAME_NUM_ERR) {
        g_mipi_err_int_cnt[devno].vc1_err_frame_num_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC2_FRAME_NUM_ERR) {
        g_mipi_err_int_cnt[devno].vc2_err_frame_num_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC3_FRAME_NUM_ERR) {
        g_mipi_err_int_cnt[devno].vc3_err_frame_num_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC0_MISMATCH) {
        g_mipi_err_int_cnt[devno].vc0_err_frame_s_e_num_mismatch_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC1_MISMATCH) {
        g_mipi_err_int_cnt[devno].vc1_err_frame_s_e_num_mismatch_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC2_MISMATCH) {
        g_mipi_err_int_cnt[devno].vc2_err_frame_s_e_num_mismatch_cnt++;
    }
    if (mipi_crc_intr_int & MIPI_VC3_MISMATCH) {
        g_mipi_err_int_cnt[devno].vc3_err_frame_s_e_num_mismatch_cnt++;
    }
}
static void mipi_crc_intr_statics(combo_dev_t devno)
{
    unsigned int mipi_crc_intr_int;
    mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_crc_intr_int = mipi_ctrl_regs->mipi_crc_intr_st.u32;

    if (mipi_crc_intr_int) {
        mipi_ctrl_regs->mipi_crc_intr_raw.u32 = 0xffffffff;
    }
    mipi_crc_intr_count1(devno, mipi_crc_intr_int);
    mipi_crc_intr_count2(devno, mipi_crc_intr_int);
}
static void lvds_err_int_count1(combo_dev_t devno, unsigned int lvds_ctrl_int)
{
    if (lvds_ctrl_int & LVDS_VSYNC) {
        g_lvds_err_int_cnt[devno].lvds_vsync_cnt++;
    }
    if (lvds_ctrl_int & CMD_RD_ERR) {
        g_lvds_err_int_cnt[devno].cmd_rd_err_cnt++;
    }
    if (lvds_ctrl_int & CMD_WR_ERR) {
        g_lvds_err_int_cnt[devno].cmd_wr_err_cnt++;
    }
    if (lvds_ctrl_int & LVDS_POP_ERR) {
        g_lvds_err_int_cnt[devno].pop_err_cnt++;
    }
    if (lvds_ctrl_int & LVDS_STAT_ERR) {
        g_lvds_err_int_cnt[devno].lvds_state_err_cnt++;
    }
    if (lvds_ctrl_int & LINK0_READ_ERR) {
        g_lvds_err_int_cnt[devno].link0_rd_err_cnt++;
    }
    if (lvds_ctrl_int & LINK1_READ_ERR) {
        g_lvds_err_int_cnt[devno].link1_rd_err_cnt++;
    }
    if (lvds_ctrl_int & LINK2_READ_ERR) {
        g_lvds_err_int_cnt[devno].link2_rd_err_cnt++;
    }
    if (lvds_ctrl_int & LINK0_WRITE_ERR) {
        g_lvds_err_int_cnt[devno].link0_wr_err_cnt++;
    }
    if (lvds_ctrl_int & LINK1_WRITE_ERR) {
        g_lvds_err_int_cnt[devno].link1_wr_err_cnt++;
    }
    if (lvds_ctrl_int & LINK2_WRITE_ERR) {
        g_lvds_err_int_cnt[devno].link2_wr_err_cnt++;
    }
}
static void lvds_err_int_count2(combo_dev_t devno, unsigned int lvds_ctrl_int)
{
    if (lvds_ctrl_int & LANE0_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane0_sync_err_cnt++;
    }
    if (lvds_ctrl_int & LANE1_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane1_sync_err_cnt++;
    }
    if (lvds_ctrl_int & LANE2_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane2_sync_err_cnt++;
    }
    if (lvds_ctrl_int & LANE3_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane3_sync_err_cnt++;
    }
    if (lvds_ctrl_int & LANE4_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane4_sync_err_cnt++;
    }
    if (lvds_ctrl_int & LANE5_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane5_sync_err_cnt++;
    }
    if (lvds_ctrl_int & LANE6_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane6_sync_err_cnt++;
    }
    if (lvds_ctrl_int & LANE7_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane7_sync_err_cnt++;
    }
}
static void lvds_int_statics(combo_dev_t devno)
{
    unsigned int lvds_ctrl_int;
    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    lvds_ctrl_int = lvds_ctrl_regs->lvds_ctrl_int.u32;

    if (lvds_ctrl_int) {
        lvds_ctrl_regs->lvds_ctrl_int_raw.u32 = 0xffffffff;
    }
    lvds_err_int_count1(devno, lvds_ctrl_int);
    lvds_err_int_count2(devno, lvds_ctrl_int);
}
static void align_int_statis(combo_dev_t devno)
{
    unsigned int align_int;
    volatile global_ctrl_regs_t *global_ctrl_regs = get_global_ctrl_regs(devno);

    align_int = global_ctrl_regs->align_int.u32;

    if (align_int) {
        global_ctrl_regs->align_int_raw.u32 = 0xffffffff;
    }
    if (align_int & ALIGN_FIFO_FULL_ERR) {
        g_align_err_int_cnt[devno].fifo_full_err_cnt++;
    }
    if (align_int & ALIGN_LANE0_ERR) {
        g_align_err_int_cnt[devno].lane0_align_err_cnt++;
    }
    if (align_int & ALIGN_LANE1_ERR) {
        g_align_err_int_cnt[devno].lane1_align_err_cnt++;
    }
    if (align_int & ALIGN_LANE2_ERR) {
        g_align_err_int_cnt[devno].lane2_align_err_cnt++;
    }
    if (align_int & ALIGN_LANE3_ERR) {
        g_align_err_int_cnt[devno].lane3_align_err_cnt++;
    }
    if (align_int & ALIGN_LANE4_ERR) {
        g_align_err_int_cnt[devno].lane4_align_err_cnt++;
    }
    if (align_int & ALIGN_LANE5_ERR) {
        g_align_err_int_cnt[devno].lane5_align_err_cnt++;
    }
    if (align_int & ALIGN_LANE6_ERR) {
        g_align_err_int_cnt[devno].lane6_align_err_cnt++;
    }
    if (align_int & ALIGN_LANE7_ERR) {
        g_align_err_int_cnt[devno].lane7_align_err_cnt++;
    }
}
static int mipi_rx_interrupt_route(int irq, void *dev_id)
{
    int i;
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    volatile global_ctrl_regs_t *global_ctrl_regs = NULL;

    mipi_rx_unused(irq);
    mipi_rx_unused(dev_id);

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mipi_rx_phy_cil_int_statis(i);
    }
    for (i = 0; i < MIPI_RX_MAX_DEV_NUM; i++) {
        global_ctrl_regs = get_global_ctrl_regs(i);
        if (global_ctrl_regs->chn_int_raw.u32) {
        } else {
            continue;
        }
        mipi_int_statics(i);
        mipi_crc_intr_statics(i);
        lvds_int_statics(i);
        align_int_statis(i);
        global_ctrl_regs->chn_int_raw.u32 = 0xffffffff;
    }
    mipi_rx_sys_regs->mipi_int_raw.u32 = 0xffffffff;

    return OSAL_IRQ_HANDLED;
}
static int mipi_rx_drv_reg_init(void)
{
    if (g_mipi_rx_regs_va == NULL) {
        g_mipi_rx_regs_va = (mipi_rx_regs_type_t *)osal_ioremap(MIPI_RX_REGS_ADDR, (unsigned int)MIPI_RX_REGS_SIZE);
        if (g_mipi_rx_regs_va == NULL) {
            ot_err("remap mipi_rx reg addr fail\n");
            return -1;
        }
        g_reg_map_flag = 1;
    }
    return 0;
}
static void mipi_rx_drv_reg_exit(void)
{
    if (g_reg_map_flag == 1) {
        if (g_mipi_rx_regs_va != NULL) {
            osal_iounmap((void *)g_mipi_rx_regs_va, (unsigned int)MIPI_RX_REGS_SIZE);
            g_mipi_rx_regs_va = NULL;
        }
        g_reg_map_flag = 0;
    }
}
static int mipi_rx_register_irq(void)
{
    int ret;

    ret = osal_request_irq(g_mipi_rx_irq_num, mipi_rx_interrupt_route, NULL, "MIPI_RX", mipi_rx_interrupt_route);
    if (ret < 0) {
        ot_err("mipi_rx: failed to register irq.\n");
        return -1;
    }
    return 0;
}
static void mipi_rx_unregister_irq(void)
{
    osal_free_irq(g_mipi_rx_irq_num, mipi_rx_interrupt_route);
}
/* magic nums convert to register configs */
static void mipi_rx_drv_hw_init(void)
{
    unsigned long mipi_rx_crg_addr;
#ifndef OT_FPGA
    int i;
#endif

#ifndef OT_FPGA
    mipi_rx_crg_addr = (unsigned long)osal_ioremap(MIPI_RX_CRG_ADDR, (unsigned long)0x4);
    if (mipi_rx_crg_addr == NULL) {
        return;
    }
    /* cil clk & bus clk */
    write_reg32(mipi_rx_crg_addr, 1 << MIPI_RX_HAL_SHIFT_4, 0x1 << MIPI_RX_HAL_SHIFT_4);
    write_reg32(mipi_rx_crg_addr, 1 << MIPI_RX_HAL_SHIFT_5, 0x1 << MIPI_RX_HAL_SHIFT_5);

    /* reset */
    write_reg32(mipi_rx_crg_addr, 1 << 0, 0x1 << 0);
    osal_udelay(10); /* 10us */
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << 0);

    osal_iounmap((void *)mipi_rx_crg_addr, (unsigned long)0x4); // len is 0x4
#else
    mipi_rx_crg_addr = (unsigned long)osal_ioremap(MIPI_RX_CRG_ADDR, (unsigned long)0x4); // len is 0x4
    if (mipi_rx_crg_addr == NULL) {
        return;
    }
    /* cil clk & bus clk */
    write_reg32(mipi_rx_crg_addr, 1 << 1, 0x1 << 1);
    write_reg32(mipi_rx_crg_addr, 1 << MIPI_RX_HAL_SHIFT_8, 0x1 << MIPI_RX_HAL_SHIFT_8);

    /* reset */
    write_reg32(mipi_rx_crg_addr, 1 << 0, 0x1 << 0);
    osal_udelay(10); /* 10us */
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << 0);
    osal_iounmap((void *)mipi_rx_crg_addr, (unsigned long)0x4); // len is 0x4
#endif
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART06_H_ */
