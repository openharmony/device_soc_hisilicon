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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART06_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART06_H_

        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.lvfir_mode = lvfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_vafir_mode(td_u32 vafir_mode)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vafir_mode = vafir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_vratio(td_u32 vratio)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vratio = vratio;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_vtp_offset(td_u32 vtp_offset)
{
    volatile u_g0_zme_voffset g0_zme_voffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_voffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_voffset.u32));
    g0_zme_voffset.bits.vtp_offset = vtp_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_voffset.u32), g0_zme_voffset.u32);

    return;
}

td_void hal_g0_zme_set_vbtm_offset(td_u32 vbtm_offset)
{
    volatile u_g0_zme_voffset g0_zme_voffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_voffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_voffset.u32));
    g0_zme_voffset.bits.vbtm_offset = vbtm_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_voffset.u32), g0_zme_voffset.u32);

    return;
}

/**********************************************************************************
*  Begin   : Graphic layer ZME relative hal functions.
**********************************************************************************/


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART06_H_ */
