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

#include "vb_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define VB_SUPPLEMENT_MASK (OT_VB_SUPPLEMENT_JPEG_MASK | \
                            OT_VB_SUPPLEMENT_MOTION_DATA_MASK | \
                            OT_VB_SUPPLEMENT_DNG_MASK | \
                            OT_VB_SUPPLEMENT_BNR_MOT_MASK | \
                            OT_VB_SUPPLEMENT_MISC_MASK)

#ifdef CONFIG_OT_VB_SUPPLEMENT_MASK_SUPPORT
td_u32 vb_drv_get_supplement_cap_mask(void)
{
    return VB_SUPPLEMENT_MASK;
}
#endif

td_bool vb_drv_is_low_delay_supported(td_void)
{
    return TD_TRUE;
}

td_bool vb_drv_is_isp_frame_info_supported(td_void)
{
#ifdef CONFIG_OT_ISP_SUPPORT
    return TD_TRUE;
#else
    return TD_FALSE;
#endif
}

td_bool vb_drv_is_misc_supported(td_u32 mask)
{
    ot_unused(mask);
    return TD_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
