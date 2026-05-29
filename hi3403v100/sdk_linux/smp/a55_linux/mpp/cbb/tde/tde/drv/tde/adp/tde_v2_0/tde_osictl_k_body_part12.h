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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART12_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART12_H_

#if (TDE_CAPABILITY & ROTATE)
static td_s32 tde_osi_rotate_check_surface(const tde_surface_msg *surface)
{
    td_u32 bpp;
    td_phys_addr_t phys_addr;
    td_u32 stride;

    bpp = (td_u32)tde_osi_get_bpp_by_fmt((drv_tde_color_fmt)surface->color_format);
    phys_addr = (td_phys_addr_t)(surface->phys_addr + (td_u64)(surface->ypos) * (td_u64)(surface->pitch) +
                  (((td_u64)(surface->xpos) * (td_u64)(bpp)) >> 3)); /* 3 bpp narrow 8 */
    stride = surface->pitch;

    if ((phys_addr % 16 != 0) || (stride % 16 != 0)) { /* 16 align */
        tde_error ("rotate operation:phys_addr and stride must 16 align!\n");
        return DRV_ERR_TDE_NOT_ALIGNED;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_quick_rotate_set(tde_hw_node *hw_node, const drv_tde_single_src *single_src,
                                       drv_tde_rotate_angle rotate_angle)
{
    tde_scandirection_mode src_scan_info = {0};
    tde_scandirection_mode dst_scan_info = {0};
    tde_surface_msg src_drv_surface = {0};
    tde_surface_msg dst_drv_surface = {0};

    if (tde_hal_node_set_base_operate(hw_node, TDE_NORM_BLIT_1OPT, TDE_ALU_NONE, 0) < 0) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    src_scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    src_scan_info.ver_scan = TDE_SCAN_UP_DOWN;
    dst_scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    dst_scan_info.ver_scan = TDE_SCAN_UP_DOWN;
    if (rotate_angle == DRV_TDE_ROTATE_CLOCKWISE_180) {
        src_scan_info.hor_scan = TDE_SCAN_RIGHT_LEFT;
        src_scan_info.ver_scan = TDE_SCAN_DOWN_UP;
    }

    tde_osi_convert_surface(single_src->src_surface, single_src->src_rect, &src_scan_info, &src_drv_surface);
    /* phys_addr and stride must 16Bytes align */
    if (tde_osi_rotate_check_surface(&src_drv_surface) != TD_SUCCESS) {
        return DRV_ERR_TDE_NOT_ALIGNED;
    }
    tde_hal_node_set_src2(hw_node, &src_drv_surface);

    tde_osi_convert_surface(single_src->dst_surface, single_src->dst_rect, &dst_scan_info, &dst_drv_surface);

    /* phys_addr and stride must 16Bytes align */
    if (tde_osi_rotate_check_surface(&dst_drv_surface) != TD_SUCCESS) {
        return DRV_ERR_TDE_NOT_ALIGNED;
    }
    tde_hal_node_set_tqt(hw_node, &dst_drv_surface, DRV_TDE_OUT_ALPHA_FROM_NORM);

    if (rotate_angle != DRV_TDE_ROTATE_CLOCKWISE_180) {
        tde_hal_node_set_rotate(hw_node, rotate_angle);
    }
    return TD_SUCCESS;
}

td_s32 tde_osi_quick_rotate(td_s32 handle, drv_tde_single_src *single_src, drv_tde_rotate_angle rotate_angle)
{
    td_s32 ret;
    tde_hw_node *hw_node = TD_NULL;
    drv_tde_double_src double_src = {0};
    td_bool is_null = (single_src == TD_NULL) || (single_src->src_surface == TD_NULL) ||
        (single_src->src_rect == TD_NULL) || (single_src->dst_surface == TD_NULL) ||
        (single_src->dst_rect == TD_NULL);

    if (is_null == TD_TRUE) {
        return DRV_ERR_TDE_NULL_PTR;
    }
    if ((single_src->src_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) ||
        (tde_osi_check_surface(single_src->src_surface, single_src->src_rect) != TD_SUCCESS) ||
        (tde_osi_check_surface(single_src->dst_surface, single_src->dst_rect) != TD_SUCCESS) ||
        (tde_osi_check_rotate_para(single_src, rotate_angle) != TD_SUCCESS)) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_hal_node_init_nd(&hw_node) < 0) {
        return DRV_ERR_TDE_NO_MEM;
    }

    /*
     * set:
     * set opt
     * set src2
     * set tqt
     * set rotate
     */
    ret = tde_osi_quick_rotate_set(hw_node, single_src, rotate_angle);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }
    /* set filter node */
    if ((rotate_angle == DRV_TDE_ROTATE_CLOCKWISE_180) && (single_src->src_surface->color_format >=
        DRV_TDE_COLOR_FMT_YCBCR422)) {
        double_src.bg_surface = TD_NULL;
        double_src.bg_rect = TD_NULL;
        double_src.fg_surface = single_src->src_surface;
        double_src.fg_rect = single_src->src_rect;
        double_src.dst_surface = single_src->dst_surface;
        double_src.dst_rect = single_src->dst_rect;
        ret = tde_osi_set_filter_node(handle, hw_node, &double_src, DRV_TDE_DEFLICKER_LEVEL_MODE_NONE,
                                      (drv_tde_deflicker_mode)DRV_TDE_FILTER_MODE_NONE);
        if (ret < 0) {
            tde_hal_free_node_buf(hw_node);
            return ret;
        }
#if (TDE_CAPABILITY & SLICE)
        tde_hal_free_node_buf(hw_node);
#endif
        return TD_SUCCESS;
    }

    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret < 0) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    return TD_SUCCESS;
}
#endif

#if (TDE_CAPABILITY & DEFLICKER)
/*
 * Function:      tde_osi_quick_flicker
 * Description:   deflicker source bitmap,output to target bitmap,source and target can be the same
 * Input:         src_surface: source bitmap info struct
 *                dst_surface: terget bitmap info struct
 *                pFuncComplCB: callback function pointer when operate is over;if null, to say to no need to notice
 */
td_s32 tde_osi_quick_flicker(td_s32 handle, drv_tde_surface *src_surface, drv_tde_rect *src_rect,
                             drv_tde_surface *dst_surface, drv_tde_rect *dst_rect)
{
    td_s32 ret;
    drv_tde_opt option = { 0 };
    drv_tde_double_src double_src;

    if (src_surface == TD_NULL) {
        tde_error("null pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    option.deflicker_mode = DRV_TDE_DEFLICKER_LEVEL_MODE_BOTH;
    option.resize = TD_TRUE;
    if (src_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    double_src.bg_surface  = TD_NULL;
    double_src.bg_rect  = TD_NULL;
    double_src.fg_surface = src_surface;
    double_src.fg_rect = src_rect;
    double_src.dst_surface = dst_surface;
    double_src.dst_rect = dst_rect;
    ret = tde_osi_blit(handle, &double_src, &option);
    if (ret < 0) {
        return ret;
    }
    return TD_SUCCESS;
}
#endif

/*
 * Function:      tde_osi_blit
 * Description:   operate pstBackGround with pstForeGround,which result output to dst_surface,
                  operate setting is in pstOpt
 * Input:         opt:  operate parameter setting struct
 */
td_s32 tde_osi_blit(td_s32 handle, drv_tde_double_src *double_src, const drv_tde_opt *opt)
{
    tde_operation_category opt_category;
    td_s32 ret;
    drv_tde_single_src single_src;

    if (double_src == TD_NULL) {
        return DRV_ERR_TDE_NULL_PTR;
    }

    opt_category = tde_osi_get_opt_category(double_src, opt);
    switch (opt_category) {
        case TDE_OPERATION_SINGLE_SRC1:
            if (double_src->bg_surface == TD_NULL) {
                single_src.src_surface = double_src->fg_surface;
                single_src.src_rect = double_src->fg_rect;
            } else {
                single_src.src_surface = double_src->bg_surface;
                single_src.src_rect = double_src->bg_rect;
            }
            single_src.dst_surface = double_src->dst_surface;
            single_src.dst_rect = double_src->dst_rect;
            return tde_osi_single_src_1_blit(handle, &single_src, TD_FALSE, TD_FALSE);
            break;
        case TDE_OPERATION_SINGLE_SRC2:
            if (double_src->bg_surface == TD_NULL) {
                single_src.src_surface = double_src->fg_surface;
                single_src.src_rect = double_src->fg_rect;
            } else {
                single_src.src_surface = double_src->bg_surface;
                single_src.src_rect = double_src->bg_rect;
            }
            single_src.dst_surface = double_src->dst_surface;
            single_src.dst_rect = double_src->dst_rect;
            ret = tde_osi_single_src_2_blit(handle, &single_src, opt, TD_FALSE, TD_FALSE);
            if (ret < 0) {
                return ret;
            }
            break;
        case TDE_OPERATION_DOUBLE_SRC:
            ret = tde_osi_double_src_2_blit(handle, double_src, opt);
            if (ret < 0) {
                return ret;
            }
            break;
        default:
            return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_quick_fill
 * Description:   quick fill fixed value to target bitmap, fill value is referred to target bitmap
 * Input:         dst_surface: target bitmap info struct
 *                fill_data: fill value
 *                pFuncComplCB: callback function pointer when operate is over;if null, to say to no need to notice
 */
td_s32 tde_osi_quick_fill(td_s32 handle, drv_tde_surface *dst_surface, drv_tde_rect *dst_rect,
                          td_u32 fill_data)
{
    drv_tde_fill_color fill_color;

    if (dst_surface == TD_NULL) {
        tde_error("null pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    fill_color.color_format = dst_surface->color_format;
    fill_color.color_value = fill_data;

    return tde_osi_1_source_fill(handle, dst_surface, dst_rect, &fill_color, TD_NULL);
}

/*
 * Function:      tde_osi_quick_draw
 * Description:   quick fill fixed value to target bitmap, fill value is referred to target bitmap
 * Input:         dst_surface: target bitmap info struct
 *                fill_data: fill value
 *                func_complete_callback: callback function pointer when operate is over;if null,
 *                to say to no need to notice
 * Output:        none
 * Return:        none
 * Others:        none
 */
td_s32 tde_osi_quick_draw(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_rect *dst_rect,
                          const drv_tde_corner_rect_info *corner_rect)
{
    return tde_osi_des_dma_draw_rect(handle, dst_surface, dst_rect, corner_rect, TD_NULL);
}

td_s32 tde_osi_multi_draw(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_rect *corner_rect_region,
    const drv_tde_corner_rect_info *corner_rect_list, td_u32 num)
{
    td_u32 i;
    td_s32 ret;
    drv_tde_rect dst_rect = {0};
    drv_tde_corner_rect_info corner_rect_info = {0};

    for (i = 0; i < num; i++) {
        dst_rect.pos_x = corner_rect_region[i].pos_x;
        dst_rect.pos_y = corner_rect_region[i].pos_y;
        dst_rect.width = corner_rect_region[i].width;
        dst_rect.height = corner_rect_region[i].height;
        corner_rect_info.width = corner_rect_list[i].width;
        corner_rect_info.height = corner_rect_list[i].height;
        corner_rect_info.inner_color = corner_rect_list[i].inner_color;
        corner_rect_info.outer_color = corner_rect_list[i].outer_color;
        ret = tde_osi_des_dma_draw_rect(handle, dst_surface, &dst_rect, &corner_rect_info, TD_NULL);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    return ret;
}

static td_s32 tde_osi_draw_line_check_format(const drv_tde_surface *dst_surface)
{
    if (dst_surface->color_format >= DRV_TDE_COLOR_FMT_RGB444 &&
        dst_surface->color_format <= DRV_TDE_COLOR_FMT_RABG8888) {
        return TD_SUCCESS;
    }
    if (dst_surface->color_format >= DRV_TDE_COLOR_FMT_CLUT4 &&
        dst_surface->color_format <= DRV_TDE_COLOR_FMT_ACLUT88) {
        return TD_SUCCESS;
    }
    if (dst_surface->color_format >= DRV_TDE_COLOR_FMT_A8 &&
        dst_surface->color_format <= DRV_TDE_COLOR_FMT_AYCBCR8888) {
        return TD_SUCCESS;
    }
    tde_error("unsupported color format %d\n", dst_surface->color_format);
    return DRV_ERR_TDE_INVALID_PARA;
}

static td_s32 tde_osi_draw_line_check(const drv_tde_line *line)
{
    if (line == TD_NULL) {
        tde_error("The draw_line pointer is null!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    /* logic limit */
    if ((line->thick == 0) || (line->thick > DRD_MAX_LINE_WIDTH)) {
        tde_error("invalid line width:%d.\n", line->thick);
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (line->start_x >= DRD_LEFT_LIMIT && line->start_x <= DRD_RIGHT_LIMIT &&
        line->start_y >= DRD_LEFT_LIMIT && line->start_y <= DRD_RIGHT_LIMIT &&
        line->end_x >= DRD_LEFT_LIMIT && line->end_x <= DRD_RIGHT_LIMIT &&
        line->end_y >= DRD_LEFT_LIMIT && line->end_y <= DRD_RIGHT_LIMIT) {
        return TD_SUCCESS;
    }
    tde_error("invalid (start_x,start_y):(%d,%d);(end_x,end_y):(%d,%d)\n", line->start_x, line->start_y,
        line->end_x, line->end_y);
    return DRV_ERR_TDE_INVALID_PARA;
}

td_s32 tde_osi_draw_line(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_line *line, td_u32 num)
{
    td_u32 task_num = num / TDE_MAX_LINE_NUM;
    td_u32 task_num_tail = num % TDE_MAX_LINE_NUM;
    const drv_tde_line *tmp_line = line;
    td_u32 i;
    td_u32 j;
    td_s32 ret;

    ret = tde_osi_draw_line_check_format(dst_surface);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    for (i = 0; i < task_num; i++) {
        for (j = 0; j < TDE_MAX_LINE_NUM; j++) {
            tmp_line = line + TDE_MAX_LINE_NUM * i + j;
            ret = tde_osi_draw_line_check(tmp_line);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        }
        /* one task 4 lines */
        tmp_line = line + TDE_MAX_LINE_NUM * i;
        ret = tde_osi_des_dma_draw_line(handle, dst_surface, tmp_line, TDE_MAX_LINE_NUM);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }

    for (j = 0; j < task_num_tail; j++) {
        tmp_line = line + TDE_MAX_LINE_NUM * i + j;
        ret = tde_osi_draw_line_check(tmp_line);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    /* the last task be left(1~3) lines */
    tmp_line = line + TDE_MAX_LINE_NUM * i;
    ret = tde_osi_des_dma_draw_line(handle, dst_surface, tmp_line, task_num_tail);
    return ret;
}

/*
 * Function:      tde_osi_single_src_1_blit
 * Description:   source1 operate realization
 * Input:         dst_surface: target bitmap info struct
 *                dst_rect: target bitmap operate zone
 * Return:        success/fail
 */
static td_s32 tde_osi_single_src_1_blit(td_s32 handle, const drv_tde_single_src *single_src,
                                        td_bool mmz_for_src, td_bool mmz_for_dst)
{
    td_s32 ret;
    tde_hw_node *hw_node = TD_NULL;
    tde_surface_msg src_drv_surface = { 0 };
    tde_surface_msg dst_drv_surface = { 0 };
    tde_scandirection_mode src_scan_info = { 0 };
    tde_scandirection_mode dst_scan_info = { 0 };

    if ((single_src->src_surface == TD_NULL) || (single_src->src_rect == TD_NULL) ||
        (single_src->dst_surface == TD_NULL) || (single_src->dst_rect == TD_NULL)) {
        return DRV_ERR_TDE_NULL_PTR;
    }

    if ((single_src->src_surface->color_format == DRV_TDE_COLOR_FMT_YCBCR422) ||
        (single_src->src_surface->color_format == DRV_TDE_COLOR_FMT_PKGVYUY)) {
        tde_error("This operation doesn't support PKG!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (single_src->src_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    tde_unify_rect(single_src->src_rect, single_src->dst_rect);

    if (tde_hal_node_init_nd(&hw_node) < 0) {
        return DRV_ERR_TDE_NO_MEM;
    }
    if (tde_hal_node_set_base_operate(hw_node, TDE_QUIKE_COPY, TDE_SRC1_BYPASS, 0) < 0) {
        tde_hal_free_node_buf(hw_node);
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    if (tde_osi_get_scan_info_ex(single_src, TD_NULL, &src_scan_info, &dst_scan_info) < 0) {
        tde_hal_free_node_buf(hw_node);
        return DRV_ERR_TDE_INVALID_PARA;
    }

    tde_osi_convert_surface(single_src->src_surface, single_src->src_rect, &src_scan_info, &src_drv_surface);
    src_drv_surface.cma = mmz_for_src;

    tde_hal_node_set_src1(hw_node, &src_drv_surface);

    tde_osi_set_ext_alpha(single_src->src_surface, TD_NULL, hw_node);

    tde_osi_convert_surface(single_src->dst_surface, single_src->dst_rect, &dst_scan_info, &dst_drv_surface);
    dst_drv_surface.cma = mmz_for_dst;


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART12_H_ */
