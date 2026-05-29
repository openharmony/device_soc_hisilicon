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
#include "ddr_hal.h"
#include "ddr_dmc.h"
/* Get byte number */
DDR_AO_FUNC unsigned int ddr_hal_phy_get_byte_num(unsigned int base_dmc)
{
    unsigned int byte_num;

    /* memery width -> byte number */
    byte_num =
        ((ddr_reg_read(base_dmc + DDR_DMC_CFG_DDRMODE) >> DMC_MEM_WIDTH_BIT) & DMC_MEM_WIDTH_MASK) <<
        1;
    /* for codedex */
    if (byte_num > DDR_PHY_BYTE_MAX) {
        byte_num = DDR_PHY_BYTE_MAX;
    }

    return byte_num;
}
/* Special interleave path mode */
DDR_AO_FUNC void ddr_hal_axi_special_intlv_en(void)
{
    unsigned int action;

    action = ddr_reg_read(DDR_REG_BASE_AXI + DDR_AXI_ACTION);
    ddr_reg_write((action & (~(0x1 << SPECIAL_INTLV_EN_BIT))) | (0x1 << SPECIAL_INTLV_EN_BIT),
        DDR_REG_BASE_AXI + DDR_AXI_ACTION);
}
