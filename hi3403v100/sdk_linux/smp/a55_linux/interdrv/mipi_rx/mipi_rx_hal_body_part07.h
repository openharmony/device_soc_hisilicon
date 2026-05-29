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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART07_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART07_H_

#ifndef OT_FPGA
    /* autodeskew default value */
    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; ++i) {
        mipi_rx_set_cil_int_mask(i, MIPI_CIL_INT_MASK);
        mipi_rx_set_phy_skew_link(i, SKEW_LINK);
        mipi_rx_set_phy_deskew_cal_link(i, MIPI_DESKEW_CAL);
        mipi_rx_set_phy_fsmo_link(i, MIPI_FSMO_VALUE);
    }
#endif
}

/* magic nums convert to register configs */
static void mipi_rx_drv_hw_exit(void)
{
    unsigned long mipi_rx_crg_addr;

#ifndef OT_FPGA
    mipi_rx_crg_addr = (unsigned long)osal_ioremap(MIPI_RX_CRG_ADDR, (unsigned long)0x4);
    if (mipi_rx_crg_addr == NULL) {
        return;
    }

    /* reset */
    write_reg32(mipi_rx_crg_addr, 1 << 0, 0x1 << 0);

    /* cil clk & bus clk */
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << MIPI_RX_HAL_SHIFT_4);
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << MIPI_RX_HAL_SHIFT_5);

    osal_iounmap((void *)mipi_rx_crg_addr, (unsigned long)0x4); // len is 0x4
#else
    mipi_rx_crg_addr = (unsigned long)osal_ioremap(0x11018540, (unsigned long)0x4);
    if (mipi_rx_crg_addr == NULL) {
        return;
    }

    /* reset */
    write_reg32(mipi_rx_crg_addr, 1, 0x1);

    osal_iounmap((void *)mipi_rx_crg_addr, (unsigned long)0x4); // len is 0x4
#endif
}

int mipi_rx_drv_init(void)
{
    int ret;

    ret = mipi_rx_drv_reg_init();
    if (ret < 0) {
        ot_err("mipi_rx_drv_reg_init fail!\n");
        goto fail0;
    }

    ret = mipi_rx_register_irq();
    if (ret < 0) {
        ot_err("mipi_rx_register_irq fail!\n");
        goto fail1;
    }

    mipi_rx_drv_hw_init();

    return 0;

fail1:
    mipi_rx_drv_reg_exit();
fail0:
    return -1;
}

void mipi_rx_drv_exit(void)
{
    mipi_rx_unregister_irq();
    mipi_rx_drv_reg_exit();
    mipi_rx_drv_hw_exit();
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART07_H_ */
