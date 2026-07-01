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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART05_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART05_H_

    bottom = (bottom1 > bottom2) ? bottom2 : bottom1;

    if ((left > right) || (top > bottom)) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    inter_rect->pos_x = left;
    inter_rect->pos_y = top;
    inter_rect->width = right - left + 1;
    inter_rect->height = bottom - top + 1;
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_is_rect1_in_rect2
 * Description:   Rect1 is if inside Rect2
 * Input:         rect1  input rectangle1
                  rect2  input rectangle2
 * Return:        1  have inter zone
                  0  no inter zone
 */
static td_bool tde_osi_is_rect1_in_rect2(const drv_tde_rect *rect1, const drv_tde_rect *rect2)
{
    td_s32 right1 = rect1->pos_x + rect1->width - 1;
    td_s32 right2 = rect2->pos_x + rect2->width - 1;

    td_s32 bottom1 = rect1->pos_y + rect1->height - 1;
    td_s32 bottom2 = rect2->pos_y + rect2->height - 1;

    if ((rect1->pos_x >= rect2->pos_x) &&
        (rect1->pos_y >= rect2->pos_y) &&
        (right1 <= right2) &&
        (bottom1 <= bottom2)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

/*
 * Function:      tde_osi_set_mb_para
 * Description:   MB operate setting parameter interface
 * Input:         handle: task handle
 *                dst_surface:  target bitmap information struct
 *                dst_rect: target bitmap operate zone
 *                mb_opt:  operate parameter setting struct
 */
static td_s32 tde_osi_set_mb_para(td_s32 handle, const drv_tde_mb_src *mb_src, const drv_tde_mb_opt *mb_opt)
{
    drv_tde_surface *fg_surface = TD_NULL;
    drv_tde_opt *opt = TD_NULL;
    td_s32 ret;
    drv_tde_single_src single_src;

    fg_surface = (drv_tde_surface *)tde_malloc(sizeof(drv_tde_surface));
    if (fg_surface == TD_NULL) {
        tde_error ("malloc fg_surface failed, size=%ld!\n", (unsigned long)(sizeof(drv_tde_surface)));
        return DRV_ERR_TDE_NO_MEM;
    }
    opt = (drv_tde_opt *)tde_malloc(sizeof(drv_tde_opt));
    if (opt == TD_NULL) {
        tde_error ("malloc pstOpt failed, size=%ld!\n", (unsigned long)(sizeof(drv_tde_opt)));
        tde_free(fg_surface);
        return DRV_ERR_TDE_NO_MEM;
    }
    fg_surface->phys_addr = mb_src->mb_surface->y_addr;
    fg_surface->stride = mb_src->mb_surface->y_stride;
    fg_surface->width = mb_src->mb_surface->y_width;
    fg_surface->color_format = tde_osi_covert_mb_fmt(mb_src->mb_surface->mb_color_format);
    fg_surface->height = mb_src->mb_surface->y_height;
    fg_surface->cbcr_phys_addr = mb_src->mb_surface->cbcr_phys_addr;
    fg_surface->cbcr_stride = mb_src->mb_surface->cbcr_stride;

#if (TDE_CAPABILITY & DEFLICKER)
    opt->deflicker_mode = (mb_opt->is_deflicker) ? DRV_TDE_DEFLICKER_LEVEL_MODE_BOTH :
	    DRV_TDE_DEFLICKER_LEVEL_MODE_NONE;
#endif

    opt->out_alpha_from = (mb_opt->is_set_out_alpha) ? DRV_TDE_OUT_ALPHA_FROM_GLOBALALPHA :
	    DRV_TDE_OUT_ALPHA_FROM_NORM;
    opt->resize = (mb_opt->resize_en == DRV_TDE_MB_RESIZE_NONE) ? TD_FALSE : TD_TRUE;
    opt->clip_rect = mb_opt->clip_rect;
    opt->global_alpha = mb_opt->out_alpha;
    opt->clip_mode = mb_opt->clip_mode;

    single_src.src_surface      = fg_surface;
    single_src.src_rect         = mb_src->src_rect;
    single_src.dst_surface      = mb_src->dst_surface;
    single_src.dst_rect         = mb_src->dst_rect;
    ret = tde_osi_single_src_2_blit(handle, &single_src, opt, TD_FALSE, TD_FALSE);
    tde_free((td_void *)fg_surface);
    tde_free((td_void *)opt);
    return ret;
}


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART05_H_ */
