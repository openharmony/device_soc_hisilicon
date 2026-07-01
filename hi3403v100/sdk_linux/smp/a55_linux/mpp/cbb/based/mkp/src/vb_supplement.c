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

#include "vb_supplement.h"
#include "vb_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#ifdef CONFIG_OT_VB_SUPPLEMENT_MASK_SUPPORT
td_s32 vb_check_supplement(const ot_vb_supplement_cfg *conf)
{
    td_u32 mask = vb_drv_get_supplement_cap_mask();
    if (conf->supplement_cfg & (~mask)) {
        ot_trace_vb(OT_DBG_ERR, "VB supplement conf(%#x) is out of mask range(%#x)!\n",
                    conf->supplement_cfg, mask);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}
#endif

td_u32 vb_get_supplement_size(const ot_vb_supplement_cfg *conf, td_bool cached)
{
    td_u32 size = 0;

    if (cached == TD_TRUE) {
        if (vb_drv_is_misc_supported(conf->supplement_cfg) == TD_TRUE) {
            size += sizeof(ot_video_supplement_misc);
        }
        if (conf->supplement_cfg & OT_VB_SUPPLEMENT_JPEG_MASK) {
            size += sizeof(ot_jpeg_dcf);
        }
        if (vb_drv_is_isp_frame_info_supported()) {
            size += sizeof(ot_isp_frame_info);
        }
        if (conf->supplement_cfg & OT_VB_SUPPLEMENT_MOTION_DATA_MASK) {
            size += sizeof(dis_inner_motion_data_info);
        }
        if (conf->supplement_cfg & OT_VB_SUPPLEMENT_DNG_MASK) {
            size += sizeof(ot_dng_image_dynamic_info);
        }
        if (conf->supplement_cfg & OT_VB_SUPPLEMENT_BNR_MOT_MASK) {
            size += sizeof(bnr_mot_info);
        }
    } else {
        if (vb_drv_is_low_delay_supported()) {
            size = sizeof(low_delay_info);
        }
    }

    return size;
}

td_void vb_value_supplement_cached_addr(td_phys_addr_t phy, td_void *vir,
    ot_video_supplement *supplement, const ot_vb_supplement_cfg *conf)
{
    td_u32 size = 0;

    if (vb_drv_is_misc_supported(conf->supplement_cfg) == TD_TRUE) {
        supplement->misc_info_phys_addr = phy + size;
        supplement->misc_info_virt_addr = (td_void *)((td_uintptr_t)vir + size);
        size += sizeof(ot_video_supplement_misc);
    }
    if (conf->supplement_cfg & OT_VB_SUPPLEMENT_JPEG_MASK) {
        supplement->jpeg_dcf_phys_addr = phy + size;
        supplement->jpeg_dcf_virt_addr = (td_void *)((td_uintptr_t)vir + size);
        size += sizeof(ot_jpeg_dcf);
    }
    if (vb_drv_is_isp_frame_info_supported()) {
        supplement->isp_info_phys_addr = phy + size;
        supplement->isp_info_virt_addr = (td_void *)((td_uintptr_t)vir + size);
        size += sizeof(ot_isp_frame_info);
    }
    if (conf->supplement_cfg & OT_VB_SUPPLEMENT_MOTION_DATA_MASK) {
        supplement->motion_data_phys_addr = phy + size;
        supplement->motion_data_virt_addr = (td_void *)((td_uintptr_t)vir + size);
        size += sizeof(dis_inner_motion_data_info);
    }
    if (conf->supplement_cfg & OT_VB_SUPPLEMENT_DNG_MASK) {
        supplement->frame_dng_phys_addr = phy + size;
        supplement->frame_dng_virt_addr = (td_void *)((td_uintptr_t)vir + size);
        size += sizeof(ot_dng_image_dynamic_info);
    }
    if (conf->supplement_cfg & OT_VB_SUPPLEMENT_BNR_MOT_MASK) {
        supplement->bnr_rnt_phys_addr = phy + size;
        supplement->bnr_mot_virt_addr = (td_void *)((td_uintptr_t)vir + size);
        size += sizeof(bnr_mot_info);
    }

    return;
}

td_void vb_value_supplement_no_cache_addr(td_phys_addr_t phy, td_void *vir,
    ot_video_supplement *supplement, const ot_vb_supplement_cfg *conf)
{
    ot_unused(conf);
    supplement->low_delay_phys_addr = phy;
    supplement->low_delay_virt_addr = vir;

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
