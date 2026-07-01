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

#include "pin_mux.h"
#include "sys_cfg.h"
#include <asm/barrier.h>

/* mux with sensor2 slave/i2c4/i2c5/spi1/mipirx1/sensor2/sensor3 */
static void vi_bt1120_mode_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0158, 0x0206); /* VI_CLK */
    sys_writel(iocfg2_base + 0x016C, 0x0006); /* VI_DATA0 */
    sys_writel(iocfg2_base + 0x0178, 0x0006); /* VI_DATA1 */
    sys_writel(iocfg2_base + 0x017C, 0x0006); /* VI_DATA2 */
    sys_writel(iocfg2_base + 0x0174, 0x0006); /* VI_DATA3 */
    sys_writel(iocfg2_base + 0x0160, 0x0206); /* VI_DATA4 */
    sys_writel(iocfg2_base + 0x015C, 0x0206); /* VI_DATA5 */
    sys_writel(iocfg2_base + 0x0164, 0x0206); /* VI_DATA6 */
    sys_writel(iocfg2_base + 0x0154, 0x0206); /* VI_DATA7 */
    sys_writel(iocfg2_base + 0x0194, 0x0006); /* VI_DATA8 */
    sys_writel(iocfg2_base + 0x0190, 0x0006); /* VI_DATA9 */
    sys_writel(iocfg2_base + 0x0184, 0x0006); /* VI_DATA10 */
    sys_writel(iocfg2_base + 0x0180, 0x0006); /* VI_DATA11 */
    sys_writel(iocfg2_base + 0x0188, 0x0006); /* VI_DATA12 */
    sys_writel(iocfg2_base + 0x018C, 0x0006); /* VI_DATA13 */
    sys_writel(iocfg2_base + 0x0170, 0x0006); /* VI_DATA14 */
    sys_writel(iocfg2_base + 0x0168, 0x0006); /* VI_DATA15 */
}

#if SENSOR_SLAVE_EN
static void sensor0_slave_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x01D0, 0x0201); /* SENSOR0_HS */
    sys_writel(iocfg2_base + 0x01D4, 0x0201); /* SENSOR0_VS */
}

static void sensor1_slave_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0144, 0x0201); /* SENSOR1_HS */
    sys_writel(iocfg2_base + 0x0148, 0x0101); /* SENSOR1_VS */
}

/* mux with VI BT1120 */
static void sensor2_slave_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0158, 0x0205); /* SENSOR2_HS */
    sys_writel(iocfg2_base + 0x015C, 0x0205); /* SENSOR2_VS */
}

static void sensor_slave_pin_mux(int sensor_index)
{
    switch (sensor_index) {
        case PIN_MUX_SENSOR0_SLAVE:
            sensor0_slave_pin_mux();
            break;
        case PIN_MUX_SENSOR1_SLAVE:
            sensor1_slave_pin_mux();
            break;
        case PIN_MUX_SENSOR2_SLAVE:
            sensor2_slave_pin_mux();
            break;
        default:
            break;
    }
}
#endif

static void sensor0_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x01C8, 0x02d1); /* SENSOR0_CLK */
    sys_writel(iocfg2_base + 0x01CC, 0x0101); /* SENSOR0_RSTN */
}

static void sensor1_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0150, 0x02d1); /* SENSOR1_CLK */
    sys_writel(iocfg2_base + 0x014C, 0x0201); /* SENSOR1_RSTN */
}

/* mux with VI BT1120/i2c5/spi1 */
static void sensor2_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x01E8, 0x02d4); /* SENSOR2_CLK */
    sys_writel(iocfg2_base + 0x0160, 0x0205); /* SENSOR2_RSTN */
}

/* mux with  VI BT1120/spi1 */
static void sensor3_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0154, 0x02d2); /* SENSOR2_CLK */
}

static void sensor_pin_mux(int sensor_index)
{
    switch (sensor_index) {
        case PIN_MUX_SENSOR_0:
            sensor0_pin_mux();
            break;
        case PIN_MUX_SENSOR_1:
            sensor1_pin_mux();
            break;
        case PIN_MUX_SENSOR_2:
            sensor2_pin_mux();
            break;
        case PIN_MUX_SENSOR_3:
            sensor3_pin_mux();
            break;
        default:
            break;
    }
}

/* mux with VO BT1120/RGB */
static void vo_mipi_tx_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + (0x00D0 + 0x08), 0x0201); /* VSYNC_TE_MIPITX */
    sys_writel(iocfg2_base + 0x00A0, 0x0000); /* DSI_D3N */
    sys_writel(iocfg2_base + 0x00A4, 0x0000); /* DSI_D3P */
    sys_writel(iocfg2_base + 0x00A8, 0x0000); /* DSI_D1N */
    sys_writel(iocfg2_base + 0x00AC, 0x0000); /* DSI_D1P */
    sys_writel(iocfg2_base + 0x00B0, 0x0000); /* DSI_CKN */
    sys_writel(iocfg2_base + 0x00B4, 0x0000); /* DSI_CKP */
    sys_writel(iocfg2_base + 0x00B8, 0x0000); /* DSI_D0N */
    sys_writel(iocfg2_base + 0x00BC, 0x0000); /* DSI_D0P */
    sys_writel(iocfg2_base + 0x00C0, 0x0000); /* DSI_D2N */
    sys_writel(iocfg2_base + 0x00C4, 0x0000); /* DSI_D2P */
}

static void vo_mipi_tx_enable(void)
{
    void *crg_base = sys_config_get_reg_crg();
    unsigned long addr = (unsigned long)(crg_base + 0x8140);

    /* mipi_tx gate clk enable */
    write_reg32(addr, 1, 0x1); /* bit 0 */

    /* unreset */
    write_reg32(addr, 0 << 1, 0x1 << 1);  /* 1: bit 1 */

    /* select ref clk 27MHz */
    write_reg32(addr, 1 << 2, 0x3 << 2); /* 2: bit 2 */
}

static inline void set_phy_reg_isb(void)
{
    isb();
#ifdef CONFIG_64BIT
    dsb(sy);
#else
    dsb();
#endif
#ifdef CONFIG_64BIT
    dmb(sy);
#else
    dmb();
#endif
}

static void set_phy_reg(unsigned int addr, unsigned char value)
{
    void *mipi_tx_base = sys_config_get_reg_mipi_tx();

    set_phy_reg_isb();
    sys_writel(mipi_tx_base + 0xb8, 0x10000 + addr);
    set_phy_reg_isb();
    sys_writel(mipi_tx_base + 0xb4, 0x2);
    set_phy_reg_isb();
    sys_writel(mipi_tx_base + 0xb4, 0x0);
    set_phy_reg_isb();
    sys_writel(mipi_tx_base + 0xb8, value);
    set_phy_reg_isb();
    sys_writel(mipi_tx_base + 0xb4, 0x2);
    set_phy_reg_isb();
    sys_writel(mipi_tx_base + 0xb4, 0x0);
    set_phy_reg_isb();
}

static void vo_set_pin_drive_cap(mipi_tx_drive_cap cap)
{
    vo_mipi_tx_enable();
    switch (cap) {
        case MIPI_TX_DRIVE_CAP_LEVEL0:
            set_phy_reg(MIPI_TX_DRIVE_CAP_PHY_REG, MIPI_TX_DRIVE_CAP_LEVEL0_VALUE);
            break;

        case MIPI_TX_DRIVE_CAP_LEVEL1:
            set_phy_reg(MIPI_TX_DRIVE_CAP_PHY_REG, MIPI_TX_DRIVE_CAP_LEVEL1_VALUE);
            break;

        case MIPI_TX_DRIVE_CAP_LEVEL2:
            set_phy_reg(MIPI_TX_DRIVE_CAP_PHY_REG, MIPI_TX_DRIVE_CAP_LEVEL2_VALUE);
            break;

        case MIPI_TX_DRIVE_CAP_LEVEL3:
            set_phy_reg(MIPI_TX_DRIVE_CAP_PHY_REG, MIPI_TX_DRIVE_CAP_LEVEL3_VALUE);
            break;
        default:
            break;
    }
}

/* mux with MIPI_TX/RGB/SPI2 */
static void vo_bt_pin_mux(int vo_bt_mode)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    /* some bt pins' drv cap set by mipi_tx controller */
    vo_set_pin_drive_cap(MIPI_TX_DRIVE_CAP_LEVEL3);

    sys_writel(iocfg2_base + 0x00C8, 0x0682); /* VO_BT1120_CLK */
    sys_writel(iocfg2_base + 0x00A8,    0x2); /* VO_BT1120_DATA0 */
    sys_writel(iocfg2_base + 0x00AC,    0x2); /* VO_BT1120_DATA1 */
    sys_writel(iocfg2_base + 0x00B0,    0x2); /* VO_BT1120_DATA2 */
    sys_writel(iocfg2_base + 0x00B4,    0x2); /* VO_BT1120_DATA3 */
    sys_writel(iocfg2_base + 0x00B8,    0x2); /* VO_BT1120_DATA4 */
    sys_writel(iocfg2_base + 0x00C0,    0x2); /* VO_BT1120_DATA5 */
    sys_writel(iocfg2_base + 0x00C4,    0x2); /* VO_BT1120_DATA6 */
    sys_writel(iocfg2_base + 0x00BC,    0x2); /* VO_BT1120_DATA7 */

    if (vo_bt_mode == VO_BT656_MODE) {
        return;
    }
    sys_writel(iocfg2_base + 0x00D4, 0x0242); /* VO_BT1120_DATA8 */
    sys_writel(iocfg2_base + 0x00A0,    0x2); /* VO_BT1120_DATA9 */
    sys_writel(iocfg2_base + 0x00A4,    0x2); /* VO_BT1120_DATA10 */
    sys_writel(iocfg2_base + 0x00D0, 0x0242); /* VO_BT1120_DATA11 */
    sys_writel(iocfg2_base + 0x00CC, 0x0242); /* VO_BT1120_DATA12 */
    sys_writel(iocfg2_base + (0x00D0 + 0x08), 0x0242); /* VO_BT1120_DATA13 */
    sys_writel(iocfg2_base + 0x00E0, 0x0242); /* VO_BT1120_DATA14 */
    sys_writel(iocfg2_base + 0x00DC, 0x0242); /* VO_BT1120_DATA15 */
}

/* mux with VO BT1120/MIPI_TX/spi2 */
static void vo_rgb_pin_mux(int vo_rgb_mode)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    /* some rgb pins' drv cap set by mipi_tx controller */
    vo_set_pin_drive_cap(MIPI_TX_DRIVE_CAP_LEVEL2);

    sys_writel(iocfg2_base + 0x0098, 0x0223); /* RGB_CLK */
    sys_writel(iocfg2_base + 0x0080, 0x0213); /* RGB_DE */
    sys_writel(iocfg2_base + 0x008C, 0x0213); /* RGB_HS */
    sys_writel(iocfg2_base + 0x0090, 0x0213); /* RGB_VS */
    sys_writel(iocfg2_base + 0x00C0,    0x3); /* RGB_DATA0 */
    sys_writel(iocfg2_base + 0x00B8,    0x3); /* RGB_DATA1 */
    sys_writel(iocfg2_base + 0x00CC, 0x0233); /* RGB_DATA2 */
    sys_writel(iocfg2_base + 0x00D0, 0x0233); /* RGB_DATA3 */
    sys_writel(iocfg2_base + 0x00AC,    0x3); /* RGB_DATA4 */
    sys_writel(iocfg2_base + 0x00B4,    0x3); /* RGB_DATA5 */

    if (vo_rgb_mode == VO_RGB_6BIT_MODE) {
        return;
    }

    sys_writel(iocfg2_base + 0x00B0,    0x3); /* RGB_DATA6 */
    sys_writel(iocfg2_base + 0x00A8,    0x3); /* RGB_DATA7 */

    if (vo_rgb_mode == VO_RGB_8BIT_MODE) {
        return;
    }

    sys_writel(iocfg2_base + 0x00A0,    0x3); /* RGB_DATA8 */
    sys_writel(iocfg2_base + 0x00A4,    0x3); /* RGB_DATA9 */
    sys_writel(iocfg2_base + 0x00C8, 0x0233); /* RGB_DATA10 */
    sys_writel(iocfg2_base + 0x00D4, 0x0233); /* RGB_DATA11 */
    sys_writel(iocfg2_base + 0x0084, 0x0213); /* RGB_DATA12 */
    sys_writel(iocfg2_base + 0x0094, 0x0213); /* RGB_DATA13 */
    sys_writel(iocfg2_base + 0x0088, 0x0213); /* RGB_DATA14 */
    sys_writel(iocfg2_base + 0x009C, 0x0213); /* RGB_DATA15 */

    if (vo_rgb_mode == VO_RGB_16BIT_MODE) {
        return;
    }

    sys_writel(iocfg2_base + 0x00E0, 0x0233); /* RGB_DATA16 */
    sys_writel(iocfg2_base + (0x00D0 + 0x08), 0x0233); /* RGB_DATA17 */

    if (vo_rgb_mode == VO_RGB_18BIT_MODE) {
        return;
    }

    sys_writel(iocfg2_base + 0x00BC,    0x3); /* RGB_DATA18 */
    sys_writel(iocfg2_base + 0x00C4,    0x3); /* RGB_DATA19 */
    sys_writel(iocfg2_base + 0x0068, 0x0203); /* RGB_DATA20 */
    sys_writel(iocfg2_base + 0x006C, 0x0203); /* RGB_DATA21 */
    sys_writel(iocfg2_base + 0x0064, 0x0203); /* RGB_DATA22 */
    sys_writel(iocfg2_base + 0x0060, 0x0213); /* RGB_DATA23 */
}

static void vo_pin_mux(int vo_mode_type)
{
    switch (vo_mode_type) {
        case VO_BT1120_MODE:
        case VO_BT656_MODE:
            vo_bt_pin_mux(vo_mode_type);
            break;
        case VO_MIPI_TX_MODE:
            vo_mipi_tx_pin_mux();
            break;
        case VO_RGB_6BIT_MODE:
        case VO_RGB_8BIT_MODE:
        case VO_RGB_16BIT_MODE:
        case VO_RGB_18BIT_MODE:
        case VO_RGB_24BIT_MODE:
            vo_rgb_pin_mux(vo_mode_type);
            break;
        case VO_NONE_MODE:
            sys_config_print("==========no vo config!==========\n");
            break;
        default:
            break;
    }
}

static void i2c0_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x013C, 0x2031); /* I2C0_SDA */
    sys_writel(iocfg2_base + 0x0140, 0x2031); /* I2C0_SCL */
}

/* mux with hdmi */
static void i2c1_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x00E8, 0x2032); /* I2C1_SDA */
    sys_writel(iocfg2_base + 0x00EC, 0x2032); /* I2C1_SCL */
}

/* mux with spi0/3 wire spi */
static void i2c2_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x01DC, 0x0072); /* I2C2_SDA */
    sys_writel(iocfg2_base + (0x01D0 + 0x08), 0x0072); /* I2C2_SCL */
}

/* mux with spi0/3 wires spi */
static void i2c3_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x01E4, 0x0062); /* I2C3_SDA */
    sys_writel(iocfg2_base + 0x01E0, 0x0062); /* I2C3_SCL */
}

/* mux with spi1/vi bt1120/sensor2 slave */
static void i2c4_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0158, 0x0062); /* I2C4_SDA */
    sys_writel(iocfg2_base + 0x015C, 0x0062); /* I2C4_SCL */
}

/* mux with spi1/vi bt1120/sensor2 */
static void i2c5_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0160, 0x0072); /* I2C5_SDA */
    sys_writel(iocfg2_base + 0x0164, 0x0072); /* I2C5_SCL */
}

static void i2c_pin_mux(int i2c_index)
{
    switch (i2c_index) {
        case PIN_MUX_I2C_0:
            i2c0_pin_mux();
            break;
        case PIN_MUX_I2C_1:
            i2c1_pin_mux();
            break;
        case PIN_MUX_I2C_2:
            i2c2_pin_mux();
            break;
        case PIN_MUX_I2C_3:
            i2c3_pin_mux();
            break;
        case PIN_MUX_I2C_4:
            i2c4_pin_mux();
            break;
        case PIN_MUX_I2C_5:
            i2c5_pin_mux();
            break;
        default:
            break;
    }
}

#if SPI_EN
/* mux with 3wires spi/i2c2/i2c3 */
static void spi0_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + (0x01D0 + 0x08), 0x1251); /* SPI0_SCLK */
    sys_writel(iocfg2_base + 0x01DC, 0x1051); /* SPI0_SDO */
    sys_writel(iocfg2_base + 0x01E0, 0x1251); /* SPI0_SDI */
    sys_writel(iocfg2_base + 0x01E4, 0x1241); /* SPI0_CSN */
}

/* mux with i2c5/i2c4/vi bt1120/sensor2/sensor3/sensor2 slave */
static void spi1_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0164, 0x1261); /* SPI1_SCLK */
    sys_writel(iocfg2_base + 0x0160, 0x1051); /* SPI1_SDO */
    sys_writel(iocfg2_base + 0x015C, 0x1251); /* SPI1_SDI */
    sys_writel(iocfg2_base + 0x0158, 0x1241); /* SPI1_CSN0 */
    sys_writel(iocfg2_base + 0x0154, 0x1241); /* SPI1_CSN1 */
}

/* mux with vo BT.1120/RGB */
static void spi2_group0_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x00C8, 0x1261); /* SPI2_SCLK */
    sys_writel(iocfg2_base + 0x00CC, 0x1251); /* SPI2_SDO */
    sys_writel(iocfg2_base + 0x00D0, 0x1201); /* SPI2_SDI */
    sys_writel(iocfg2_base + 0x00D4, 0x1251); /* SPI2_CSN */
}

static void spi2_group1_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0074, 0x1263); /* SPI2_SCLK */
    sys_writel(iocfg2_base + 0x007C, 0x1253); /* SPI2_SDO */
    sys_writel(iocfg2_base + 0x0078, 0x1203); /* SPI2_SDI */
    sys_writel(iocfg2_base + 0x0070, 0x1253); /* SPI2_CSN */
}

/* mux with i2s */
static void spi3_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0110, 0x01b3); /* SPI3_SCLK */
    sys_writel(iocfg2_base + 0x0108, 0x0153); /* SPI3_SDO */
    sys_writel(iocfg2_base + 0x0104, 0x0203); /* SPI3_SDI */
    sys_writel(iocfg2_base + 0x010C, 0x0203); /* SPI3_CSN */
}

/* mux with i2c2/i2c3/spi0 */
static void spi_3wire_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + (0x01D0 + 0x08), 0x0203); /* SPI_3WIRE_CLK */
    sys_writel(iocfg2_base + 0x01DC, 0x0203); /* SPI_3WIRE_CSN */
    sys_writel(iocfg2_base + 0x01E0, 0x0203); /* SPI_3WIRE_DATA */
}

static void spi_pin_mux(int spi_index)
{
    switch (spi_index) {
        case PIN_MUX_SPI_0:
            spi0_pin_mux();
            break;
        case PIN_MUX_SPI_1:
            spi1_pin_mux();
            break;
        case PIN_MUX_SPI_2_GROUP0:
            spi2_group0_pin_mux();
            break;
        case PIN_MUX_SPI_2_GROUP1:
            spi2_group1_pin_mux();
            break;
        case PIN_MUX_SPI_3:
            spi3_pin_mux();
            break;
        case PIN_MUX_SPI_3WIRES:
            spi_3wire_pin_mux();
            break;
        default:
            break;
    }
}
#endif

static void mipi0_rx_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x01B0, 0x0000); /* MIPI_RX0_CK1N */
    sys_writel(iocfg2_base + 0x01B4, 0x0000); /* MIPI_RX0_CK1P */
    sys_writel(iocfg2_base + 0x01C0, 0x0000); /* MIPI_RX0_D3N */
    sys_writel(iocfg2_base + 0x01C4, 0x0000); /* MIPI_RX0_D3P */
    sys_writel(iocfg2_base + 0x01B8, 0x0000); /* MIPI_RX0_D1N */
    sys_writel(iocfg2_base + 0x01BC, 0x0000); /* MIPI_RX0_D1P */
    sys_writel(iocfg2_base + 0x01A8, 0x0000); /* MIPI_RX0_CK0N */
    sys_writel(iocfg2_base + 0x01AC, 0x0000); /* MIPI_RX0_CK0P */
    sys_writel(iocfg2_base + 0x0198, 0x0000); /* MIPI_RX0_D0N */
    sys_writel(iocfg2_base + 0x019C, 0x0000); /* MIPI_RX0_D0P */
    sys_writel(iocfg2_base + 0x01A0, 0x0000); /* MIPI_RX0_D2N */
    sys_writel(iocfg2_base + 0x01A4, 0x0000); /* MIPI_RX0_D2P */
}

/* mux with vi bt1120 */
static void mipi1_rx_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0180, 0x0000); /* MIPI_RX1_CK1N */
    sys_writel(iocfg2_base + 0x0184, 0x0000); /* MIPI_RX1_CK1P */
    sys_writel(iocfg2_base + 0x0190, 0x0000); /* MIPI_RX1_D3N */
    sys_writel(iocfg2_base + 0x0194, 0x0000); /* MIPI_RX1_D3P */
    sys_writel(iocfg2_base + 0x0188, 0x0000); /* MIPI_RX1_D1N */
    sys_writel(iocfg2_base + 0x018C, 0x0000); /* MIPI_RX1_D1P */
    sys_writel(iocfg2_base + 0x0178, 0x0000); /* MIPI_RX1_CK0N */
    sys_writel(iocfg2_base + 0x017C, 0x0000); /* MIPI_RX1_CK0P */
    sys_writel(iocfg2_base + 0x0168, 0x0000); /* MIPI_RX1_D0N */
    sys_writel(iocfg2_base + 0x016C, 0x0000); /* MIPI_RX1_D0P */
    sys_writel(iocfg2_base + 0x0170, 0x0000); /* MIPI_RX1_D2N */
    sys_writel(iocfg2_base + 0x0174, 0x0000); /* MIPI_RX1_D2P */
}

/* mux with mipirx1 */
static void thermo_sensor_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0150, 0x1187); /* HT_MCK */
    sys_writel(iocfg2_base + 0x014C, 0x11F7); /* HT_RSTN */
    sys_writel(iocfg2_base + 0x0148, 0x1167); /* HT_SD0 */
    sys_writel(iocfg2_base + 0x0144, 0x1167); /* HT_SD1 */
    sys_writel(iocfg2_base + 0x0158, 0x11F7); /* HT_SD2 */
    sys_writel(iocfg2_base + 0x0154, 0x11F7); /* HT_SD3 */

    sys_writel(iocfg2_base + 0x0194, 0x1207); /* HT_CLK_OUT */
    sys_writel(iocfg2_base + 0x0168, 0x1207); /* HT_VS */
    sys_writel(iocfg2_base + 0x0170, 0x1207); /* HT_HS */
    sys_writel(iocfg2_base + 0x0178, 0x1207); /* HT_PS */

    sys_writel(iocfg2_base + 0x0190, 0x1207); /* HT_DO0 */
    sys_writel(iocfg2_base + 0x0184, 0x1207); /* HT_DO1 */
    sys_writel(iocfg2_base + 0x0188, 0x1207); /* HT_DO2 */
    sys_writel(iocfg2_base + 0x0180, 0x1207); /* HT_DO3 */
    sys_writel(iocfg2_base + 0x017C, 0x1207); /* HT_DO4 */
    sys_writel(iocfg2_base + 0x0174, 0x1207); /* HT_DO5 */
    sys_writel(iocfg2_base + 0x016C, 0x1207); /* HT_DO6 */
}

/* mux with mipirx1 */
static void thermo_t3_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    vi_bt1120_mode_mux();

    sys_writel(iocfg2_base + 0x0150, 0x11F7); /* HT_MCK */
    sys_writel(iocfg2_base + 0x014C, 0x11F7); /* HT_RSTN */
    sys_writel(iocfg2_base + 0x0154, 0x11F7); /* HT_SD3 */
    sys_writel(iocfg2_base + 0x0144, 0x11F7); /* HT_SD1 */
    sys_writel(iocfg2_base + 0x0148, 0x11F7); /* HT_SD0 */
}

static void mipi_rx_pin_mux(int mipi_rx_type)
{
    switch (mipi_rx_type) {
        case MIPI_RX_0:
            mipi0_rx_pin_mux();
            break;
        case MIPI_RX_1:
            mipi1_rx_pin_mux();
            break;
        default:
            break;
    }
}

static void vi_pin_mux(int vi_intf_type)
{
    switch (vi_intf_type) {
        case VI_MIPI_RX_MODE:
            mipi_rx_pin_mux(MIPI_RX_0);
            mipi_rx_pin_mux(MIPI_RX_1);
            break;
        case VI_CMOS_BT1120_MODE:
            mipi_rx_pin_mux(MIPI_RX_0);
            vi_bt1120_mode_mux();
            break;
        case VI_THERMO_MODE:
            mipi_rx_pin_mux(MIPI_RX_0);
            thermo_sensor_pin_mux();
            break;
        case VI_THERMO_T3_MODE:
            mipi_rx_pin_mux(MIPI_RX_0);
            thermo_t3_pin_mux();
            break;
        default:
            break;
    }
}

/* mux with i2c1 */
static void hdmi_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x00E4, 0x2801); /* HDMI_HOTPLUG */
    sys_writel(iocfg2_base + 0x00E8, 0x6801); /* HDMI_SDA */
    sys_writel(iocfg2_base + 0x00EC, 0x6801); /* HDMI_SCL */
}

#if I2S_EN
/* mux with spi3 */
static void i2s_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x010C, 0x0232); /* I2S_BCLK */
    sys_writel(iocfg2_base + 0x0108, 0x0152); /* I2S_WS */
    sys_writel(iocfg2_base + 0x0100, 0x0202); /* I2S_SD_RX */
    sys_writel(iocfg2_base + 0x0104, 0x0252); /* I2S_SD_TX */
    sys_writel(iocfg2_base + 0x0110, 0x0142); /* I2S_MCLK */
}
#endif

static void amp_unmute_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();
    void *gpio_base = sys_config_get_reg_gpio();

    /* GPIO10_0 */
    sys_writel(iocfg2_base + 0x00FC, 0x0201);

    /* output high */
    sys_writel(gpio_base + 0xA400, 0x01);
    sys_writel(gpio_base + 0xA004, 0x01);
}
#if UART_EN
/* mux with GPIO10_6/UART0_RXD/GPIO10_7/UART0_TXD */
static void uart0_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0124, 0x1101); /* UART0_RXD */
    sys_writel(iocfg2_base + 0x0128, 0x1201); /* UART0_TXD */
}
/* mux with GPIO5_4/UART1_RXD/RGMII1_RXD3/RGB_DATA23/GPIO5_5/UART1_TXD/EPHY1_RSTN/RGB_DATA22 */
static void uart1_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0060, 0x1201); /* UART1_RXD */
    sys_writel(iocfg2_base + 0x0064, 0x1201); /* UART1_TXD */
}

/* mux with GPIO6_0/UART2_RXD/RGMII1_RXD0/SPI2_CSN/GPIO6_1/UART2_TXD/RGMII1_RXCK/SPI2_SCLK */
static void uart2_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0070, 0x1201); /* UART2_RXD */
    sys_writel(iocfg2_base + 0x0074, 0x1201); /* UART2_TXD */
}

/* mux with GPIO11_0/UART3_RXD/SPI1_CSN0/FLASH_TRIG/UART5_RXD/GPIO11_1/UART3_TXD/SPI1_SDO/
 * SHUTTER_TRIG/UART5_TXD */
static void uart3_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x012C, 0x1201); /* UART3_RXD */
    sys_writel(iocfg2_base + 0x0130, 0x1201); /* UART3_TXD */
}

/* mux with GPIO11_2/UART4_RXD/SPI1_SDI/UART5_RTSN/GPIO11_3/UART4_TXD/SPI1_SCLK/UART5_CTSN */
static void uart4_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0134, 0x1201); /* UART4_RXD */
    sys_writel(iocfg2_base + 0x0138, 0x1201); /* UART4_TXD */
}

static void uart_pin_mux(int uart_index)
{
    switch (uart_index) {
        case PIN_MUX_UART_0:
            uart0_pin_mux();
            break;
        case PIN_MUX_UART_1:
            uart1_pin_mux();
            break;
        case PIN_MUX_UART_2:
            uart2_pin_mux();
            break;
        case PIN_MUX_UART_3:
            uart3_pin_mux();
            break;
        case PIN_MUX_UART_4:
            uart4_pin_mux();
            break;
        default:
            break;
    }
}

#endif

/* mux with GPIO11_2/UART4_RXD/SPI1_SDI/UART5_RTSN/GPIO11_3/UART4_TXD/SPI1_SCLK/UART5_CTSN */
static void gpio11_2_3_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base + 0x0134, 0x1200); /* GPIO11_2 */
    sys_writel(iocfg2_base + 0x0138, 0x1200); /* GPIO11_3 */
}

void pin_mux(void)
{
    int vo_intf_type = sys_config_get_vo_intf_type();
    int vi_intf_type = sys_config_get_vi_intf_type();
    int i2c_en       = sys_config_get_i2c_en();
    int hdmi_en      = sys_config_get_hdmi_en();
    int audio_en     = sys_config_get_audio_en();
    int gpio11_2_3_en = sys_config_get_gpio11_2_3_en();

    if (vi_intf_type != VI_NONE_MODE) {
        sensor_pin_mux(PIN_MUX_SENSOR_0);
        sensor_pin_mux(PIN_MUX_SENSOR_1);
        sensor_pin_mux(PIN_MUX_SENSOR_2);
        sensor_pin_mux(PIN_MUX_SENSOR_3);
        sensor_slave_pin_mux(PIN_MUX_SENSOR0_SLAVE);
        sensor_slave_pin_mux(PIN_MUX_SENSOR1_SLAVE);
    }

    vo_pin_mux(vo_intf_type);

    if (i2c_en != 0) {
        i2c_pin_mux(PIN_MUX_I2C_0);
        i2c_pin_mux(PIN_MUX_I2C_2);
        i2c_pin_mux(PIN_MUX_I2C_3);
        i2c_pin_mux(PIN_MUX_I2C_5);
        i2c_pin_mux(PIN_MUX_I2C_4);
    }

    vi_pin_mux(vi_intf_type);

    if (hdmi_en != 0) {
        hdmi_pin_mux();
    }

    if (audio_en != 0) {
        amp_unmute_pin_mux();
    }

    if (gpio11_2_3_en != 0) {
        gpio11_2_3_pin_mux();
    }
}
