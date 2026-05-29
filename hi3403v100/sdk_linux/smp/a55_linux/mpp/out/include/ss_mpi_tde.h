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

#ifndef __SS_MPI_TDE_H__
#define __SS_MPI_TDE_H__

#include "ot_common_tde.h"


#ifdef __cplusplus
extern "C" {
#endif

/* API Declaration */
td_s32 ss_tde_open(td_void);
td_void ss_tde_close(td_void);
td_s32 ss_tde_begin_job(td_void);
td_s32 ss_tde_end_job(td_s32 handle, td_bool is_sync, td_bool is_block, td_u32 time_out);
td_s32 ss_tde_cancel_job(td_s32 handle);
td_s32 ss_tde_wait_for_done(td_s32 handle);
td_s32 ss_tde_wait_all_done(td_void);
td_s32 ss_tde_reset(td_void);
td_s32 ss_tde_quick_fill(td_s32 handle, const ot_tde_none_src *none_src, td_u32 fill_data);
td_s32 ss_tde_quick_draw_rect(td_s32 handle, const ot_tde_none_src *none_src,
    const ot_tde_corner_rect_info *corner_rect_info);
td_s32 ss_tde_draw_multi_rect(td_s32 handle, const ot_tde_surface *dst_surface, const ot_tde_corner_rect *corner_rect,
    td_u32 num);
td_s32 ss_tde_draw_line(td_s32 handle, const ot_tde_surface *dst_surface, const ot_tde_line *line, td_u32 num);
td_s32 ss_tde_quick_copy(td_s32 handle, const ot_tde_single_src *single_src);
td_s32 ss_tde_quick_resize(td_s32 handle, const ot_tde_single_src *single_src);
td_s32 ss_tde_quick_deflicker(td_s32 handle, const ot_tde_single_src *single_src);
td_s32 ss_tde_solid_draw(td_s32 handle, const ot_tde_single_src *single_src, const ot_tde_fill_color *fill_color,
    const ot_tde_opt *opt);
td_s32 ss_tde_rotate(td_s32 handle, const ot_tde_single_src *single_src, ot_tde_rotate_angle rotate);
td_s32 ss_tde_bit_blit(td_s32 handle, const ot_tde_double_src *double_src, const ot_tde_opt *opt);
td_s32 ss_tde_pattern_fill(td_s32 handle, const ot_tde_double_src *double_src,
    const ot_tde_pattern_fill_opt *fill_opt);
td_s32 ss_tde_mb_blit(td_s32 handle, const ot_tde_mb_src *mb_src, const ot_tde_mb_opt *opt);
td_s32 ss_tde_bitmap_mask_rop(td_s32 handle, const ot_tde_triple_src *triple_src, ot_tde_rop_mode rop_code_color,
    ot_tde_rop_mode rop_code_alpha);
td_s32 ss_tde_bitmap_mask_blend(td_s32 handle, const ot_tde_triple_src *triple_src, td_u8 alpha,
    ot_tde_alpha_blending blend_mode);
td_s32 ss_tde_set_deflicker_level(ot_tde_deflicker_level deflicker_level);
td_s32 ss_tde_get_deflicker_level(ot_tde_deflicker_level *deflicker_level);
td_s32 ss_tde_set_alpha_threshold_value(td_u8 threshold_value);
td_s32 ss_tde_get_alpha_threshold_value(td_u8 *threshold_value);
td_s32 ss_tde_set_alpha_threshold_state(td_bool threshold_en);
td_s32 ss_tde_get_alpha_threshold_state(td_bool *threshold_en);
td_s32 ss_tde_enable_rgn_deflicker(td_bool rgn_deflicker_en);

#ifdef __cplusplus
}
#endif

#endif /* __SOURCE_MSP_API_TDE_SRC_LINUX_API__ */
