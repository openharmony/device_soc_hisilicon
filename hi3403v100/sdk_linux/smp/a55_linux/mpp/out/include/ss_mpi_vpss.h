/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SS_MPI_VPSS_H__
#define __SS_MPI_VPSS_H__

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_vb.h"
#include "ot_common_gdc.h"
#include "ot_common_vpss.h"


#ifdef __cplusplus
extern "C" {
#endif

/* group settings */
td_s32 ss_mpi_vpss_create_grp(ot_vpss_grp grp, const ot_vpss_grp_attr *grp_attr);
td_s32 ss_mpi_vpss_destroy_grp(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_start_grp(ot_vpss_grp grp);
td_s32 ss_mpi_vpss_stop_grp(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_reset_grp(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_get_grp_attr(ot_vpss_grp grp, ot_vpss_grp_attr *grp_attr);
td_s32 ss_mpi_vpss_set_grp_attr(ot_vpss_grp grp, const ot_vpss_grp_attr *grp_attr);

td_s32 ss_mpi_vpss_get_grp_param(ot_vpss_grp grp, ot_vpss_grp_param *grp_param);
td_s32 ss_mpi_vpss_set_grp_param(ot_vpss_grp grp, const ot_vpss_grp_param *grp_param);

td_s32 ss_mpi_vpss_get_grp_nrx_adv_param(ot_vpss_grp grp, ot_vpss_nrx_adv_param *nrx_adv_param);
td_s32 ss_mpi_vpss_set_grp_nrx_adv_param(ot_vpss_grp grp, const ot_vpss_nrx_adv_param *nrx_adv_param);

td_s32 ss_mpi_vpss_set_grp_crop(ot_vpss_grp grp, const ot_vpss_crop_info *crop_info);
td_s32 ss_mpi_vpss_get_grp_crop(ot_vpss_grp grp, ot_vpss_crop_info *crop_info);

td_s32 ss_mpi_vpss_send_frame(ot_vpss_grp grp, const ot_video_frame_info *frame_info, td_s32 milli_sec);

td_s32 ss_mpi_vpss_get_grp_frame(ot_vpss_grp grp, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vpss_release_grp_frame(ot_vpss_grp grp, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vpss_enable_backup_frame(ot_vpss_grp grp);
td_s32 ss_mpi_vpss_disable_backup_frame(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_set_chn_sharpen(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_chn_sharpen_param *sharpen_param);
td_s32 ss_mpi_vpss_get_chn_sharpen(ot_vpss_grp grp, ot_vpss_chn chn, ot_vpss_chn_sharpen_param *sharpen_param);

td_s32 ss_mpi_vpss_set_grp_delay(ot_vpss_grp grp, td_u32 delay);
td_s32 ss_mpi_vpss_get_grp_delay(ot_vpss_grp grp, td_u32 *delay);

td_s32 ss_mpi_vpss_set_grp_sizer(ot_vpss_grp grp, const ot_vpss_sizer_info *sizer_info);
td_s32 ss_mpi_vpss_get_grp_sizer(ot_vpss_grp grp, ot_vpss_sizer_info *sizer_info);

td_s32 ss_mpi_vpss_enable_user_frame_rate_ctrl(ot_vpss_grp grp);
td_s32 ss_mpi_vpss_disable_user_frame_rate_ctrl(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_get_ldci_attr(ot_vpss_grp grp, ot_vpss_ldci_attr *ldci_attr);
td_s32 ss_mpi_vpss_set_ldci_attr(ot_vpss_grp grp, const ot_vpss_ldci_attr *ldci_attr);

td_s32 ss_mpi_vpss_set_grp_frame_interrupt_attr(ot_vpss_grp grp, const ot_frame_interrupt_attr *frame_interrupt_attr);
td_s32 ss_mpi_vpss_get_grp_frame_interrupt_attr(ot_vpss_grp grp, ot_frame_interrupt_attr *frame_interrupt_attr);

/* chn settings */
td_s32 ss_mpi_vpss_set_chn_attr(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_chn_attr *chn_attr);
td_s32 ss_mpi_vpss_get_chn_attr(ot_vpss_grp grp, ot_vpss_chn chn, ot_vpss_chn_attr *chn_attr);

td_s32 ss_mpi_vpss_enable_chn(ot_vpss_grp grp, ot_vpss_chn chn);
td_s32 ss_mpi_vpss_disable_chn(ot_vpss_grp grp, ot_vpss_chn chn);

td_s32 ss_mpi_vpss_set_chn_rotation(ot_vpss_grp grp, ot_vpss_chn chn, const ot_rotation_attr *rotation_attr);
td_s32 ss_mpi_vpss_get_chn_rotation(ot_vpss_grp grp, ot_vpss_chn chn, ot_rotation_attr *rotation_attr);

td_s32 ss_mpi_vpss_get_chn_frame(ot_vpss_grp grp, ot_vpss_chn chn, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vpss_release_chn_frame(ot_vpss_grp grp, ot_vpss_chn chn, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vpss_get_grp_rgn_luma(ot_vpss_grp grp, td_u32 num, const ot_rect *rgn, ot_vpss_luma_result *luma_result,
    td_s32 milli_sec);
td_s32 ss_mpi_vpss_get_chn_rgn_luma(ot_vpss_grp grp, ot_vpss_chn chn, td_u32 num, const ot_rect *rgn,
    td_u64 *luma_data, td_s32 milli_sec);

td_s32 ss_mpi_vpss_set_chn_crop(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_crop_info *crop_info);
td_s32 ss_mpi_vpss_get_chn_crop(ot_vpss_grp grp, ot_vpss_chn chn, ot_vpss_crop_info *crop_info);

td_s32 ss_mpi_vpss_attach_vb_pool(ot_vpss_grp grp, ot_vpss_chn chn, ot_vb_pool vb_pool);
td_s32 ss_mpi_vpss_detach_vb_pool(ot_vpss_grp grp, ot_vpss_chn chn);

td_s32 ss_mpi_vpss_set_low_delay_attr(ot_vpss_grp grp, ot_vpss_chn chn, const ot_low_delay_info *low_delay_info);
td_s32 ss_mpi_vpss_get_low_delay_attr(ot_vpss_grp grp, ot_vpss_chn chn, ot_low_delay_info *low_delay_info);

td_s32 ss_mpi_vpss_set_chn_spread_attr(ot_vpss_grp grp, ot_vpss_chn chn, const ot_spread_attr *spread_attr);
td_s32 ss_mpi_vpss_get_chn_spread_attr(ot_vpss_grp grp, ot_vpss_chn chn, ot_spread_attr *spread_attr);

td_s32 ss_mpi_vpss_set_chn_ldc_attr(ot_vpss_grp grp, ot_vpss_chn chn, const ot_ldc_attr *ldc_attr);
td_s32 ss_mpi_vpss_get_chn_ldc_attr(ot_vpss_grp grp, ot_vpss_chn chn, ot_ldc_attr *ldc_attr);

td_s32 ss_mpi_vpss_ldc_pos_query_dst_to_src(ot_vpss_grp grp, ot_vpss_chn chn, const ot_point *dst_point,
    ot_point *src_point);
td_s32 ss_mpi_vpss_ldc_pos_query_src_to_dst(ot_vpss_grp grp, ot_vpss_chn chn, const ot_point *src_point,
    ot_point *dst_point);

td_s32 ss_mpi_vpss_set_grp_fisheye_cfg(ot_vpss_grp grp, const ot_fisheye_cfg *fisheye_cfg);
td_s32 ss_mpi_vpss_get_grp_fisheye_cfg(ot_vpss_grp grp, ot_fisheye_cfg *fisheye_cfg);

td_s32 ss_mpi_vpss_set_chn_fisheye(ot_vpss_grp grp, ot_vpss_chn chn,
    const ot_fisheye_correction_attr *correction_attr);
td_s32 ss_mpi_vpss_get_chn_fisheye(ot_vpss_grp grp, ot_vpss_chn chn, ot_fisheye_correction_attr *correction_attr);

td_s32 ss_mpi_vpss_fisheye_pos_query_dst_to_src(ot_vpss_grp grp, ot_vpss_chn chn, td_u32 region_index,
    const ot_point *dst_point, ot_point *src_point);

td_s32 ss_mpi_vpss_set_ext_chn_attr(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_ext_chn_attr *ext_chn_attr);
td_s32 ss_mpi_vpss_get_ext_chn_attr(ot_vpss_grp grp, ot_vpss_chn chn, ot_vpss_ext_chn_attr *ext_chn_attr);

td_s32 ss_mpi_vpss_enable_quick_send(ot_vpss_grp grp, ot_vpss_chn chn);
td_s32 ss_mpi_vpss_disable_quick_send(ot_vpss_grp grp, ot_vpss_chn chn);

td_s32 ss_mpi_vpss_set_chn_align(ot_vpss_grp grp, ot_vpss_chn chn, td_u32 align);
td_s32 ss_mpi_vpss_get_chn_align(ot_vpss_grp grp, ot_vpss_chn chn, td_u32 *align);

td_s32 ss_mpi_vpss_set_chn_scale_coef_type(ot_vpss_grp grp, ot_vpss_chn chn, ot_scale_coef_type type);
td_s32 ss_mpi_vpss_get_chn_scale_coef_type(ot_vpss_grp grp, ot_vpss_chn chn, ot_scale_coef_type *type);

td_s32 ss_mpi_vpss_set_chn_cfg(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_chn_cfg *chn_cfg);
td_s32 ss_mpi_vpss_get_chn_cfg(ot_vpss_grp grp, ot_vpss_chn chn, ot_vpss_chn_cfg *chn_cfg);

/* module param settings */
td_s32 ss_mpi_vpss_set_mod_param(const ot_vpss_mod_param *mod_param);
td_s32 ss_mpi_vpss_get_mod_param(ot_vpss_mod_param *mod_param);

td_s32 ss_mpi_vpss_get_chn_fd(ot_vpss_grp grp, ot_vpss_chn chn);
td_s32 ss_mpi_vpss_close_fd(td_void);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_VPSS_H__ */
