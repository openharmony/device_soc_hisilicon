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

#include "ddr_hal_context.h"
#include "ddr_ddrt.h"

/* Some special DDRT need read register repeatedly */
DDR_AO_FUNC unsigned int ddr_hal_ddrt_get_mem_width(void)
{
    return ((ddr_reg_read(ddr_hal_get_cur_dmc() + DDR_DMC_CFG_DDRMODE) >>
        DMC_MEM_WIDTH_BIT) & DMC_MEM_WIDTH_MASK);
}

// #ifdef DDR_CAPAT_ADAPT_CFG
/**
  @brief Initial ddrt before capacity self-adaptation.
**/
DDR_AO_FUNC void ddr_ddrt_init_for_capacity(void)
{
    ddrt_burst_config_desc ddrt_burst_config_reg;

    ddrt_burst_config_reg.val = 0;
    ddrt_burst_config_reg.bits.ddrt_burst_size = DDRT_DATA_16BYTE;
    /* set ddrt_burst_len=1 */
    ddrt_burst_config_reg.bits.ddrt_burst_len = 0;
    ddr_reg_write(ddrt_burst_config_reg.val, DDR_REG_BASE_DDRT + DDRT_BURST_CONFIG);
    if (ddr_hal_get_phy_dram_type(0) == PHY_DRAMCFG_TYPE_LPDDR4) {
        ddr_reg_write(DDRT_LP4_MEM_CFG, DDR_REG_BASE_DDRT + DDRT_MEM_CONFIG);
    } else {
        ddr_reg_write(DDRT_NOLP4_MEM_CFG, DDR_REG_BASE_DDRT + DDRT_MEM_CONFIG);
    }
    /* set ddrt_burst_num=1 */
    ddr_reg_write(0x0, DDR_REG_BASE_DDRT + DDRT_BURST_NUM);
    ddr_reg_write(0xffffffff, DDR_REG_BASE_DDRT + DDRT_ADDR_NUM);
    /* set loop=1 */
    ddr_reg_write(0x0, DDR_REG_BASE_DDRT + DDRT_LOOP_NUM);
}

/**
  @brief Process ddr in dword by ddrt.

  @param      addr      Address in ddr.
  @param      val       Value to be checked of writed.
  @param      mode      Ddrt mode, only write/only read/write and read.
  @param      bit_mask  Select high/low 16bit dq to check.
  @param[out] status    Result of check.
                          DDRT_RUNNING : 0x0, ddrt is runing
                          DDRT_ERROR : 0x1, ddrt is done, values ??read and written are different.
                          DDRT_NORMAL : 0x3, ddrt is done, values ??read and written are the same.
                          DDRT_TIMEOUT : 0x11, ddrt timeout.
**/
DDR_AO_FUNC void ddr_ddrt_process_dword(
    unsigned int val, unsigned long long addr, unsigned int mode, unsigned int bit_mask, unsigned int *status)
{
#define DDRT_CHECK_MAX_LOOP 10000000
    unsigned int cnt = 0;
    addr_in_64bit addr_64;

    addr_64.addr = addr;
    ddr_reg_write((unsigned int)(addr_64.dword.high_32_bit), DDR_REG_BASE_DDRT + DDRT_DDR_BASE_ADDR);
    ddr_reg_write((unsigned int)(addr_64.dword.low_32_bit), DDR_REG_BASE_DDRT + DDRT_ADDR);
    ddr_reg_write(val, DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
    ddr_reg_write(mode, DDR_REG_BASE_DDRT + DDRT_OP);

    while (cnt < DDRT_CHECK_MAX_LOOP && ddr_reg_read(DDR_REG_BASE_DDRT + DDRT_STATUS) == 0) {
        cnt++;
    }
    if (cnt >= DDRT_CHECK_MAX_LOOP) {
        *status = DDRT_TIMEOUT;
        return;
    }

    if (bit_mask == DDRT_MASK_LOW_16BIT || bit_mask == DDRT_MASK_HIGH_16BIT) {
        if (ddr_reg_read(DDR_REG_BASE_DDRT + DDRT_DQ_ERR_CNT0 + DDRT_DQ_ERR_OFFSET * bit_mask) != 0 ||
            ddr_reg_read(DDR_REG_BASE_DDRT + DDRT_DQ_ERR_CNT1 + DDRT_DQ_ERR_OFFSET * bit_mask) != 0 ||
            ddr_reg_read(DDR_REG_BASE_DDRT + DDRT_DQ_ERR_CNT2 + DDRT_DQ_ERR_OFFSET * bit_mask) != 0 ||
            ddr_reg_read(DDR_REG_BASE_DDRT + DDRT_DQ_ERR_CNT3 + DDRT_DQ_ERR_OFFSET * bit_mask) != 0) {
            *status = DDRT_ERROR;
        } else {
            *status = DDRT_NORMAL;
        }
    } else {
        *status = ddr_reg_read(DDR_REG_BASE_DDRT + DDRT_STATUS);
    }
    return;
}
// #endif /* DDR_CAPAT_ADAPT_CFG */
