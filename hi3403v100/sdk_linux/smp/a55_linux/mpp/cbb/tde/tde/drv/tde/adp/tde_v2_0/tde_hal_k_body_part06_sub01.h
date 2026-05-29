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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB01_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB01_H_

static td_u32 tde_node_max(td_u32 a, td_u32 b)
{
    return (a > b) ? a : b;
}

static td_u32 tde_node_min(td_u32 a, td_u32 b)
{
    return (a > b) ? b : a;
}

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
static td_u32 tde_node_clip(td_s32 a)
{
    return ((a >= 0) ? a : 0);
}
#endif


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB01_H_ */
