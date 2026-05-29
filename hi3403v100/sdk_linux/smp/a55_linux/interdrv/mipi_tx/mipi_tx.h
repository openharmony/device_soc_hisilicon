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

#ifndef __MIPI_TX_H__
#define __MIPI_TX_H__

#include "ot_mipi_tx.h"

#define MIPI_TX_MAX_SYNC_VACT  4096
#define MIPI_TX_MIN_SYNC_VACT  100
#define MIPI_TX_MAX_SYNC_VBB   256
#define MIPI_TX_MIN_SYNC_VBB   1
#define MIPI_TX_MAX_SYNC_VFB   256
#define MIPI_TX_MIN_SYNC_VFB   1
#define MIPI_TX_MAX_SYNC_HACT  4096
#define MIPI_TX_MIN_SYNC_HACT  1
#define MIPI_TX_MAX_SYNC_HBB   65535
#define MIPI_TX_MIN_SYNC_HBB   1
#define MIPI_TX_MAX_SYNC_HFB   65535
#define MIPI_TX_MIN_SYNC_HFB   1
#define MIPI_TX_MAX_SYNC_HPW   65535
#define MIPI_TX_MIN_SYNC_HPW   1
#define MIPI_TX_MAX_SYNC_VPW   256
#define MIPI_TX_MIN_SYNC_VPW   1

#define MIPI_TX_MAX_PHY_DATA_RATE 2500 /* Mbps */
#define MIPI_TX_MIN_PHY_DATA_RATE 80 /* Mbps */

#define MIPI_TX_MAX_PIXEL_CLK ((MIPI_TX_MAX_PHY_DATA_RATE) * (LANE_MAX_NUM) * 1000 / 8) /* KHz , 8: min 8bit/pixel */
#define MIPI_TX_MIN_PIXEL_CLK ((MIPI_TX_MIN_PHY_DATA_RATE) * (1) * 1000 / 24) /* KHz, 24: max 24bit/pixel */

#define mipi_tx_err(x, ...) \
    do { \
        osal_printk("%s(%d): "x, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define mipi_tx_check_null_ptr_return(ptr) \
    do { \
        if ((ptr) == NULL) { \
            mipi_tx_err("NULL pointer\r\n"); \
            return (-1); \
        } \
    } while (0)

#define mipi_tx_desc(x) 1
#define mipi_tx_unused(x) ((void)(x))

typedef struct {
    mipi_tx_work_mode_t work_mode;
    unsigned char       lp_clk_en;
} mipi_tx_work_param;

typedef struct {
    unsigned int        devno;
    mipi_tx_work_param     work_param;
} mipi_tx_pub_info;

typedef struct {
    combo_dev_cfg_t dev_cfg;
    mipi_tx_work_param work_param;
} mipi_tx_dev_ctx_t;

void mipi_tx_set_work_mode(mipi_tx_work_mode_t work_mode);
mipi_tx_work_mode_t mipi_tx_get_work_mode(void);
void mipi_tx_set_lp_clk_en(unsigned char lp_clk_en);
unsigned char mipi_tx_get_lp_clk_en(void);
int mipi_tx_get_lane_num(const short lane_id[], int lane_id_len);

#endif
