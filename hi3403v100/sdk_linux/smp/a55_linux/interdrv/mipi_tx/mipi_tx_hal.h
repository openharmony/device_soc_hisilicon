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

#ifndef __MIPI_TX_HAL_H__
#define __MIPI_TX_HAL_H__

#include "ot_mipi_tx.h"
#include "mipi_tx_reg.h"

#define MIPI_TX_BITS_PER_BYTE 8
#define MIPI_TX_INTERRUPTS_ENABLE 0

#define MIPI_TX_BYTES_PER_WORD 4
#define MIPI_TX_CMD_BYTE1 0
#define MIPI_TX_CMD_BYTE2 1
#define MIPI_TX_CMD_BYTE3 2
#define MIPI_TX_CMD_BYTE4 3

#define MIPI_TX_REGS_ADDR   0x17A80000
#define MIPI_TX_REGS_SIZE   0x10000

#define MIPI_TX_IRQ         120

#define MIPI_TX_PHY_DATA_RATE      594
#define MIPI_TX_PHY_DATA_RATE_MIN  320

#define MIPI_TX_REF_CLK     27

#ifdef OT_FPGA

/* crg */
#define MIPI_TX_CRG         0x173F2000
#define MIPI_TX_CRG2        0x173f2008

/* phy addr */
#define PLL_SET_0X14           0x14
#define PLL_SET_0X15           0x15
#define PLL_SET_0X16           0x16
#define PLL_SET_0X17           0x17
#define PLL_SET_0X1E           0x1E
#define PLL_SET_0X12           0x12
#define PLL_SET_0X13           0x13

#define DATA0_TPRE_DELAY    0x30
#define DATA1_TPRE_DELAY    0x40
#define DATA2_TPRE_DELAY    0x50
#define DATA3_TPRE_DELAY    0x60

/* clk */
#define CLK_POST_DELAY      0x21
#define CLK_TLPX            0x22
#define CLK_TCLK_PREPARE    0x23
#define CLK_TCLK_ZERO       0x24
#define CLK_TCLK_TRAIL      0x25

/* data0 */
#define DATA0_TLPX          0x32
#define DATA0_THS_PREPARE   0x33
#define DATA0_THS_ZERO      0x34
#define DATA0_THS_TRAIL     0x35

/* data1 */
#define DATA1_TLPX          0x42
#define DATA1_THS_PREPARE   0x43
#define DATA1_THS_ZERO      0x44
#define DATA1_THS_TRAIL     0x45

/* data2 */
#define DATA2_TLPX          0x52
#define DATA2_THS_PREPARE   0x53
#define DATA2_THS_ZERO      0x54
#define DATA2_THS_TRAIL     0x55

/* data3 */
#define DATA3_TLPX          0x62
#define DATA3_THS_PREPARE   0x63
#define DATA3_THS_ZERO      0x64
#define DATA3_THS_TRAIL     0x65

#define DATA0_TPRE_DELAY_VALUE 0x9
#define DATA1_TPRE_DELAY_VALUE 0x9
#define DATA2_TPRE_DELAY_VALUE 0x9
#define DATA3_TPRE_DELAY_VALUE 0x9

#define CLK_POST_DELAY_VALUE   0x0
#define CLK_TLPX_VALUE         0x6
#define CLK_TCLK_PREPARE_VALUE 0x5
#define CLK_TCLK_ZERO_VALUE    0x13
#define CLK_TCLK_TRAIL_VALUE   0x6

#define DATA0_TLPX_VALUE        0x6
#define DATA0_THS_PREPARE_VALUE 0x5
#define DATA0_THS_ZERO_VALUE    0x9
#define DATA0_THS_TRAIL_VALUE   0x6

#define DATA1_TLPX_VALUE        0x6
#define DATA1_THS_PREPARE_VALUE 0x5
#define DATA1_THS_ZERO_VALUE    0x9
#define DATA1_THS_TRAIL_VALUE   0x6

#define DATA2_TLPX_VALUE        0x6
#define DATA2_THS_PREPARE_VALUE 0x5
#define DATA2_THS_ZERO_VALUE    0x9
#define DATA2_THS_TRAIL_VALUE   0x6

#define DATA3_TLPX_VALUE        0x6
#define DATA3_THS_PREPARE_VALUE 0x5
#define DATA3_THS_ZERO_VALUE    0x9
#define DATA3_THS_TRAIL_VALUE   0x6

#else

/* crg */
#define MIPI_TX_CRG         0x11018140

/* timing parameter */
#define TLPX                60
#define TCLK_PREPARE        60
#define TCLK_ZERO           260
#define TCLK_TRAIL          70
#define TCLK_POST           95
#define TPRE_DELAY          100
#define THS_PREPARE         80
#define THS_ZERO            180
#define THS_TRAIL           80

/* phy addr */
#define PLL_SET_0X14        0x14
#define PLL_SET_0X15        0x15
#define PLL_SET_0X16        0x16
#define PLL_SET_0X1E        0x1E
#define PLL_SET_0X12        0x12
#define PLL_SET_0X13        0x13

#define PLL_SET_0X63        0x63
#define PLL_SET_0X64        0x64
#define PLL_SET_0X65        0x65

#define CLK_POST_DELAY      0x18
#define DATA0_TPRE_DELAY    0x28
#define DATA1_TPRE_DELAY    0x38
#define DATA2_TPRE_DELAY    0x48
#define DATA3_TPRE_DELAY    0x58

#define CLK_TLPX            0x10
#define CLK_TCLK_PREPARE    0x11
#define CLK_TCLK_ZERO       0x12
#define CLK_TCLK_TRAIL      0x13

/* data0 */
#define DATA0_TLPX          0x20
#define DATA0_THS_PREPARE   0x21
#define DATA0_THS_ZERO      0x22
#define DATA0_THS_TRAIL     0x23

/* data1 */
#define DATA1_TLPX          0x30
#define DATA1_THS_PREPARE   0x31
#define DATA1_THS_ZERO      0x32
#define DATA1_THS_TRAIL     0x33

/* data2 */
#define DATA2_TLPX          0x40
#define DATA2_THS_PREPARE   0x41
#define DATA2_THS_ZERO      0x42
#define DATA2_THS_TRAIL     0x43

/* data3 */
#define DATA3_TLPX          0x50
#define DATA3_THS_PREPARE   0x51
#define DATA3_THS_ZERO      0x52
#define DATA3_THS_TRAIL     0x53

#define DATA0_TPRE_DELAY_VALUE 0x6
#define DATA1_TPRE_DELAY_VALUE 0x6
#define DATA2_TPRE_DELAY_VALUE 0x6
#define DATA3_TPRE_DELAY_VALUE 0x6

#define CLK_POST_DELAY_VALUE   0x11
#define CLK_TLPX_VALUE         0x4
#define CLK_TCLK_PREPARE_VALUE 0x4
#define CLK_TCLK_ZERO_VALUE    0x13
#define CLK_TCLK_TRAIL_VALUE   0x5

#define DATA0_TLPX_VALUE        0x4
#define DATA0_THS_PREPARE_VALUE 0x5
#define DATA0_THS_ZERO_VALUE    0xD
#define DATA0_THS_TRAIL_VALUE   0x5

#define DATA1_TLPX_VALUE        0x4
#define DATA1_THS_PREPARE_VALUE 0x5
#define DATA1_THS_ZERO_VALUE    0xD
#define DATA1_THS_TRAIL_VALUE   0x5

#define DATA2_TLPX_VALUE        0x4
#define DATA2_THS_PREPARE_VALUE 0x5
#define DATA2_THS_ZERO_VALUE    0xD
#define DATA2_THS_TRAIL_VALUE   0x5

#define DATA3_TLPX_VALUE        0x4
#define DATA3_THS_PREPARE_VALUE 0x5
#define DATA3_THS_ZERO_VALUE    0xD
#define DATA3_THS_TRAIL_VALUE   0x5

#endif /* end of #ifdef OT_FPGA */

#define MIPI_TX_CSI_DATA_TYPE_FRAME_START 0x00
#define MIPI_TX_CSI_DATA_TYPE_FRAME_END   0x01
#define MIPI_TX_CSI_DATA_TYPE_LINE_START  0x02
#define MIPI_TX_CSI_DATA_TYPE_LINE_END    0x03

#define MIPI_TX_CSI_DATA_TYPE_YUV420_8BIT_NORMAL 0x18
#define MIPI_TX_CSI_DATA_TYPE_YUV420_8BIT_LEGACY 0x1A
#define MIPI_TX_CSI_DATA_TYPE_YUV422_8BIT        0x1E
#define MIPI_TX_CSI_DATA_TYPE_RBG_888            0x24
#define MIPI_TX_CSI_DATA_TYPE_RAW_8BIT           0x2A
#define MIPI_TX_CSI_DATA_TYPE_RAW_10BIT          0x2B
#define MIPI_TX_CSI_DATA_TYPE_RAW_12BIT          0x2C
#define MIPI_TX_CSI_DATA_TYPE_RAW_14BIT          0x2D
#define MIPI_TX_CSI_DATA_TYPE_RAW_16BIT          0x2E

#define MIPI_TX_READ_TIMEOUT_CNT 1000
#define MIPI_TX_MAX_RESET_PHY_TIMES 10

#define PREPARE_COMPENSATE    10
#define ROUNDUP_VALUE     7999
#define INNER_PEROID    8000   /* 8 * 1000 ,1000 is 1us = 1000ns, 8 is division ratio */

#define mipi_tx_align_up(x, a) ((((x) + ((a) - 1)) / (a)) * (a))
#define mipi_tx_ceil(x, a)     (((x) + (a) - 1) / (a))

/*
 * global operation timing parameters
 */
typedef struct {
    unsigned char data_tpre_delay;
    unsigned char clk_post_delay;
    unsigned char clk_tlpx;
    unsigned char clk_tclk_prepare;
    unsigned char clk_tclk_zero;
    unsigned char clk_tclk_trail;
    unsigned char data_tlpx;
    unsigned char data_ths_prepare;
    unsigned char data_ths_zero;
    unsigned char data_ths_trail;
} mipi_tx_phy_timing_parameters;

typedef struct {
    unsigned int vall_det;
    unsigned int vact_det;
    unsigned int hall_det;
    unsigned int hact_det;
    unsigned int hbp_det;
    unsigned int hsa_det;
    unsigned int vsa_det;
} mipi_tx_dev_phy_t;

typedef enum {
    HIGH_SPEED_MODE = 0,
    LOW_POWER_MODE = 1,
} command_trans_mode;

__inline static unsigned char get_low_byte(unsigned short short_data)
{
    return (unsigned char)(short_data & 0xff);
}

__inline static unsigned char get_high_byte(unsigned short short_data)
{
    return (unsigned char)((short_data & 0xff00) >> MIPI_TX_BITS_PER_BYTE);
}

void mipi_tx_drv_set_irq_num(unsigned int irq_num);
void mipi_tx_drv_set_regs(const mipi_tx_regs_type_t *regs);

void mipi_tx_drv_set_phy_cfg(void);
void mipi_tx_drv_get_dev_status(mipi_tx_dev_phy_t *mipi_tx_phy_ctx);
void mipi_tx_drv_set_controller_cfg(const combo_dev_cfg_t *dev_cfg);

int mipi_tx_drv_set_cmd_info(const cmd_info_t *cmd_info);

int mipi_tx_drv_get_cmd_info(const get_cmd_info_t *get_cmd_info);
void mipi_tx_drv_enable_input(const out_mode_t out_mode);
void mipi_tx_drv_disable_input(void);

int mipi_tx_drv_init(int smooth);
void mipi_tx_drv_exit(void);

#endif /* end of #ifndef __MIPI_TX_HAL_H__ */
