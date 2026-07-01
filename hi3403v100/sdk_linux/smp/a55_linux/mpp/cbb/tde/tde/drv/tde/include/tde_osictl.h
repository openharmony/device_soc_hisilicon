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

#ifndef TDE_OSICTL_H
#define TDE_OSICTL_H

#include "ot_drv_tde.h"
#include "tde_adp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 tde_osi_open(td_void);

td_s32 tde_osi_close(td_void);

/*
  * Function:      tde_osi_begin_job
  * Description:   get TDE task handle
  * Input:         handle:get task handle pointer
  * Return:        success / error code
  */
td_s32 tde_osi_begin_job(td_s32 *handle, td_void *private_data);

td_s32 tde_osi_begin_job_ex(td_s32 *handle);

/*
 * Function:      tde_osi_end_job
 * Description:   submit TDE task
 * Input:         handle: task handle
 *                is_block: if block
 *                time_out: timeout value(unit by 10ms)
 *                is_sync: if synchronization or not of sumbitting operate
 *                func_compl_cb: callback function of completing operate
 * Return:        success / error code
 */
td_s32 tde_osi_end_job(drv_tde_end_job_cmd *end_job, drv_tde_func_callback func_compl_cb, td_void *func_para);

/*
 * Function:      tde_osi_cancel_job
 * Description:   delete TDE task created, only effectively callede before endjob
 *                release software resource of list of its task
 * Input:         handle: task handle
 * Return:        success / error code
 */
td_s32 tde_osi_cancel_job(td_s32 handle);

/*
 * Function:      tde_osi_wait_for_done
 * Description:   wait for completing of submiting TDE operate
 * Input:         handle: task handle
 * Return:        success / error code
 */
td_s32 tde_osi_wait_for_done(td_s32 handle, td_u32 time_out);

td_s32 tde_osi_wait_all_done(td_bool is_sync);

td_s32 tde_osi_quick_copy(td_s32 handle, const drv_tde_single_src *single_src);

/*
 * Function:      tde_osi_quick_fill
 * Description:   quickly fill fixed value to target bitmap
 * Input:         handle: task handle
 *                fill_data: fill data, pixel format is the same as target bitmap format
 * Return:        success / error code
 */
td_s32 tde_osi_quick_fill(td_s32 handle, drv_tde_surface *dst_surface, drv_tde_rect *dst_rect, td_u32 fill_data);

td_s32 tde_osi_quick_draw(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_rect *dst_rect,
                          const drv_tde_corner_rect_info *corner_rect);
td_s32 tde_osi_multi_draw(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_rect *corner_rect_region,
                          const drv_tde_corner_rect_info *corner_rect_list, td_u32 num);

td_s32 tde_osi_draw_line(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_line *line, td_u32 num);
/*
 * Function:      tde_osi_quick_resize
 * Description:   zoom the size of source bitmap to the size of target bitmap,
                  source and target are the same bitmap
 * Input:         handle: task handle
 *                src_surface: source bitmap info struct
 *                src_rect: source bitmap operate rect
 *                dst_surface: target bitmap info struct
 *                dst_rect: target bitmap operate rect
 * Return:        success / error code
 */
td_s32 tde_osi_quick_resize(td_s32 handle, drv_tde_surface *src_surface, drv_tde_rect *src_rect,
                            drv_tde_surface *dst_surface, drv_tde_rect *dst_rect);

/*
 * Function:      tde_osi_quick_flicker
 * Description:   deflicker source bitmap,output to target bitmap,
                  source and target can be the same bitmap
 * Input:         handle: task handle
 *                src_surface: source bitmap info struct
 *                src_rect: source bitmap operate rect
 *                dst_surface: target bitmap info struct
 *                dst_rect: target bitmap operate rect
 * Return:        success / error code
 */
td_s32 tde_osi_quick_flicker(td_s32 handle, drv_tde_surface *src_surface, drv_tde_rect *src_rect,
                             drv_tde_surface *dst_surface, drv_tde_rect *dst_rect);

/*
 * Function:      tde_osi_blit
 * Description:   operate pstBackGround with pstForeGround, which result output to pDst, operate setting is in pOpt
 * Input:         handle:task handle
 *                opt: operate parameter setting struct
 * Return:        success / error code
 */
td_s32 tde_osi_blit(td_s32 handle, drv_tde_double_src *double_src, const drv_tde_opt *opt);

/*
 * Function:      tde_osi_mb_blit
 * Description:   MB blit interface
 * Input:         handle: task handle
 *                mb_opt:  operate parameter setting struct
 * Return:        >0: return task id of current operate ; <0: fail
 */
td_s32 tde_osi_mb_blit(td_s32 handle, drv_tde_mb_src *mb_src, drv_tde_mb_opt *mb_opt);

/*
 * Function:      tde_osi_bitmap_mask_rop
 * Description:   Ropmask source with mask bitmap at first, and then ropmask source1 with middle bitmap
 *                output result into target bitmap
 * Input:         handle: task handle
 * Return:        success / error code
 */
#if (TDE_CAPABILITY & MASKROP)
td_s32 tde_osi_bitmap_mask_rop(td_s32 handle, const drv_tde_triple_src *triple_src, drv_tde_rop_mode rop_color,
                               drv_tde_rop_mode rop_alpha);
#endif
/*
 * Function:      tde_osi_bitmap_mask_blend
 * Description:   blendmask source2 with mask bitmap at first, and then blend source1 with middle bitmap
 * 			      output result to target bitmap
 * Input:         handle: task handle
 * Return:        success / error code
 */
#if (TDE_CAPABILITY & MASKBLEND)
td_s32 tde_osi_bitmap_mask_blend(td_s32 handle, const drv_tde_triple_src *triple_src, td_u8 alpha,
                                 drv_tde_alpha_blending blend_mode);

#endif
/*
 * Function:      tde_osi_solid_draw
 * Description:   output result of operate single color value with source bitmap,operate setting is in pOpt
 * Input:         handle: task handle
 *                single_src: source bitmap information struct
 *                fill_color: color info
 *                opt: operate parameter setting struct
 * Return:        success / error code
 */
td_s32 tde_osi_solid_draw(td_s32 handle, const drv_tde_single_src *single_src, drv_tde_fill_color *fill_color,
                          const drv_tde_opt *opt);

td_s32 tde_osi_set_deflicker_level(drv_tde_deflicker_level deflicker_level);

td_s32 tde_osi_get_deflicker_level(drv_tde_deflicker_level *deflicker_level);

td_s32 tde_osi_set_alpha_threshold_value(td_u8 threshold_value);

td_s32 tde_osi_get_alpha_threshold_value(td_u8 *threshold_value);

td_s32 tde_osi_set_alpha_threshold_state(td_bool alpha_threshold_en);

td_s32 tde_osi_get_alpha_threshold_state(td_bool *alpha_threshold_en);
/*
 * Function:      tde_osi_pattern_fill
 * Description:   pattern fill
 * Input:         none
 * Output:        handle:task handle
                  opt:pattern fill operate option
 * Return:        success/error code
 * Others:        pattern width is 256 at most, height is not litmited
 */
td_s32 tde_osi_pattern_fill(td_s32 handle, const drv_tde_double_src *double_src, const drv_tde_pattern_fill_opt *opt);

td_s32 tde_osi_enable_region_deflicker(td_bool region_deflicker);

td_s32 tde_cal_scale_rect(const drv_tde_rect *src_rect, const drv_tde_rect *dst_rect, drv_tde_rect *rect_in_src,
                          drv_tde_rect *rect_in_dst);
#ifdef CONFIG_TDE_BLIT_EX
td_s32 tde_osi_single_blit_ex(td_s32 handle, drv_tde_surface *src_surface, drv_tde_rect *src_rect,
                              drv_tde_surface *dst_surface, drv_tde_rect *dst_rect, drv_tde_opt *opt,
                              td_bool mmz_for_src, td_bool mmz_for_dst);

#endif

td_s32 tde_osi_quick_rotate(td_s32 handle, drv_tde_single_src *single_src, drv_tde_rotate_angle rotate_angle);

td_s32 tde_osi_get_bpp_by_fmt(drv_tde_color_fmt fmt);

td_s32 tde_osi_set_gfbg_mmz_addr(td_phys_addr_t phys_addr, td_u32 phys_len, td_u32 layer_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TDE_OSICTL_H */
