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

#ifndef PIN_MUX_H
#define PIN_MUX_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SPI_EN 0
#define SENSOR_SLAVE_EN 1
#define I2S_EN 0
#define UART_EN 0

#define VO_INTF_NAME_LEN 16

typedef enum {
    PIN_MUX_SENSOR_0 = 0,
    PIN_MUX_SENSOR_1 = 1,
    PIN_MUX_SENSOR_2 = 2,
    PIN_MUX_SENSOR_3 = 3,
} pin_mux_sensor;

typedef enum {
    PIN_MUX_SENSOR0_SLAVE = 0,
    PIN_MUX_SENSOR1_SLAVE = 1,
    PIN_MUX_SENSOR2_SLAVE = 2,
} pin_mux_sensor_slave;

typedef enum {
    PIN_MUX_I2C_0 = 0,
    PIN_MUX_I2C_1 = 1,
    PIN_MUX_I2C_2 = 2,
    PIN_MUX_I2C_3 = 3,
    PIN_MUX_I2C_4 = 4,
    PIN_MUX_I2C_5 = 5,
} pin_mux_i2c;

typedef enum {
    PIN_MUX_SPI_0 = 0,
    PIN_MUX_SPI_1 = 1,
    PIN_MUX_SPI_2_GROUP0 = 2,
    PIN_MUX_SPI_2_GROUP1 = 3,
    PIN_MUX_SPI_3 = 4,
    PIN_MUX_SPI_3WIRES = 5,
} pin_mux_spi;

typedef enum {
    MIPI_RX_0 = 0,
    MIPI_RX_1 = 1,
} mipi_rx_index;

typedef enum {
    VO_BT1120_MODE = 0,
    VO_BT656_MODE = 1,
    VO_RGB_6BIT_MODE = 2,
    VO_RGB_8BIT_MODE = 3,
    VO_RGB_16BIT_MODE = 4,
    VO_RGB_18BIT_MODE = 5,
    VO_RGB_24BIT_MODE = 6,
    VO_MIPI_TX_MODE = 7,
    VO_NONE_MODE = 8,
} vo_mode_index;

typedef enum {
    VI_MIPI_RX_MODE = 0,
    VI_CMOS_BT1120_MODE = 1,
    VI_THERMO_MODE = 2,
    VI_THERMO_T3_MODE = 3,
    VI_NONE_MODE = 4,
} vi_mode_index;

typedef enum {
    MIPI_TX_DRIVE_CAP_LEVEL0 = 0,
    MIPI_TX_DRIVE_CAP_LEVEL1,
    MIPI_TX_DRIVE_CAP_LEVEL2,
    MIPI_TX_DRIVE_CAP_LEVEL3,
} mipi_tx_drive_cap;

typedef enum {
    PIN_MUX_UART_0 = 0,
    PIN_MUX_UART_1 = 1,
    PIN_MUX_UART_2 = 2,
    PIN_MUX_UART_3 = 3,
    PIN_MUX_UART_4 = 4,
} pin_mux_uart;


#define MIPI_TX_DRIVE_CAP_PHY_REG 0x66

#define MIPI_TX_DRIVE_CAP_LEVEL0_VALUE 0x00
#define MIPI_TX_DRIVE_CAP_LEVEL1_VALUE 0x10
#define MIPI_TX_DRIVE_CAP_LEVEL2_VALUE 0x18
#define MIPI_TX_DRIVE_CAP_LEVEL3_VALUE 0x1C

void pin_mux(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* PIN_MUX_H */
