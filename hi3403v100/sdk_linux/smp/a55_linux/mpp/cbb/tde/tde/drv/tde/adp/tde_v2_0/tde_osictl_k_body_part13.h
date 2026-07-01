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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART13_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART13_H_

    tde_hal_node_set_tqt(hw_node, &dst_drv_surface, DRV_TDE_OUT_ALPHA_FROM_NORM);
    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret < 0) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_single_para(const drv_tde_single_src *single_src, const drv_tde_opt *opt, tde_hw_node **hw_node,
    tde_scandirection_mode *src_scan_info, tde_scandirection_mode *dst_scan_info)
{
    td_s32 ret;
    drv_tde_double_src double_src;

#if (TDE_CAPABILITY & COMPRESS)
    td_bool compress_or_decompress;
#endif
    ret = tde_osi_check_single_src_to_para(single_src->src_surface, single_src->src_rect,
                                           single_src->dst_surface, single_src->dst_rect, opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (tde_hal_node_init_nd(hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }

    double_src.bg_surface = TD_NULL;
    double_src.bg_rect = TD_NULL;
    double_src.fg_surface = single_src->src_surface;
    double_src.fg_rect = single_src->src_rect;
    double_src.dst_surface = single_src->dst_surface;
    double_src.dst_rect = single_src->dst_rect;
    if (tde_osi_set_clip_para(&double_src, opt, *hw_node) != TD_SUCCESS) {
        tde_hal_free_node_buf(*hw_node);
        return DRV_ERR_TDE_CLIP_AREA;
    }
    if (tde_osi_get_scan_info_ex(single_src, opt, src_scan_info, dst_scan_info) != TD_SUCCESS) {
        tde_hal_free_node_buf(*hw_node);
        return DRV_ERR_TDE_INVALID_PARA;
    }
#if (TDE_CAPABILITY & COMPRESS)
    compress_or_decompress = ((opt->is_compress) || (opt->is_decompress));
    if (compress_or_decompress) {
        src_scan_info->hor_scan = TDE_SCAN_LEFT_RIGHT;
        src_scan_info->ver_scan = TDE_SCAN_UP_DOWN;
        dst_scan_info->hor_scan = TDE_SCAN_LEFT_RIGHT;
        dst_scan_info->ver_scan = TDE_SCAN_UP_DOWN;
    }
#endif
    return TD_SUCCESS;
}

static td_s32 tde_osi_single_opt(const drv_tde_opt *opt, const drv_tde_single_src *single_src,
    tde_surface_msg *drv_surface, tde_hw_node *hw_node)
{
    td_u16 code;
    tde_conv_mode_cmd conv = { 0 };
    td_s32 ret;
    tde_clut_usage clut_usage = TDE_CLUT_USAGE_BUTT;

#if (TDE_CAPABILITY & COMPRESS)
    if (opt->is_compress) {
        tde_hal_node_set_compress_tqt(hw_node, drv_surface, opt->out_alpha_from);
        tde_hal_node_set_compress(hw_node);
    } else {
        tde_hal_node_set_tqt(hw_node, drv_surface, opt->out_alpha_from);
    }
#else
    tde_hal_node_set_tqt(hw_node, drv_surface, opt->out_alpha_from);
#endif
    code = tde_osi_single_src2_get_opt_code(single_src->src_surface->color_format,
                                            single_src->dst_surface->color_format);

    tde_osi_get_conv_by_code(code, &conv);

    if (tde_hal_node_set_color_convert(hw_node, &conv) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    ret = tde_osi_set_clut_opt(single_src->src_surface, single_src->dst_surface, &clut_usage,
                               opt->clut_reload, hw_node);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = tde_osi_set_base_opt_para_for_blit(opt, TD_NULL, single_src->src_surface, TDE_OPERATION_SINGLE_SRC2, hw_node);
    if (ret != TD_SUCCESS) {
        tde_error("Set base opt para for blit failed, ret = 0x%x\n", ret);
    }
    tde_osi_set_ext_alpha(TD_NULL, single_src->src_surface, hw_node);
    return TD_SUCCESS;
}

static td_s32 tde_osi_single_node(td_s32 handle, const drv_tde_opt *opt, const drv_tde_single_src *single_src,
    tde_surface_msg *drv_surface, tde_hw_node *hw_node)
{
    td_bool set_filter_node;
    drv_tde_double_src double_src;
    td_s32 ret;

    set_filter_node = ((opt->resize) || (opt->deflicker_mode != DRV_TDE_DEFLICKER_LEVEL_MODE_NONE) ||
                       (single_src->src_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422));
    if (set_filter_node) {
#if (TDE_CAPABILITY & SLICE)
        if ((single_src->src_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422) && (!(opt->resize))) {
            single_src->src_rect->height = single_src->dst_rect->height;
            single_src->src_rect->width = single_src->dst_rect->width;
        }
#endif
        double_src.bg_surface = TD_NULL;
        double_src.bg_rect = TD_NULL;
        double_src.fg_surface = single_src->src_surface;
        double_src.fg_rect = single_src->src_rect;
        double_src.dst_surface = single_src->dst_surface;
        double_src.dst_rect = single_src->dst_rect;
        ret = tde_osi_set_filter_node(handle, hw_node, &double_src, opt->deflicker_mode,
            (drv_tde_deflicker_mode)opt->filter_mode);
        if (ret != TD_SUCCESS) {
            return ret;
        }
#if (TDE_CAPABILITY & SLICE)
        tde_hal_free_node_buf(hw_node);
#endif
        tde_free((td_void *)drv_surface);
        return TD_SUCCESS;
    }
    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    tde_free((td_void *)drv_surface);
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_single_src_2_blit
 * Description:   source2 operate realization
 * Input:         dst_surface: target bitmap info struct
 *                dst_rect: target bitmap operate zone
 * Return:        success/fail
 */
static td_s32 tde_osi_single_src_2_blit(td_s32 handle, const drv_tde_single_src *single_src,
                                        const drv_tde_opt *opt, td_bool mmz_for_src, td_bool mmz_for_dst)
{
    tde_hw_node *hw_node = TD_NULL;
    tde_surface_msg *drv_surface = TD_NULL;
    td_s32 ret;
    tde_scandirection_mode src_scan_info = { 0 };
    tde_scandirection_mode dst_scan_info = { 0 };

    ret = tde_osi_single_para(single_src, opt, &hw_node, &src_scan_info, &dst_scan_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    drv_surface = (tde_surface_msg *)tde_malloc(sizeof(tde_surface_msg));
    if (drv_surface == TD_NULL) {
        tde_error ("malloc pstDrvSurface failed, size=%ld!\n", (unsigned long)(sizeof(tde_surface_msg)));
        tde_hal_free_node_buf(hw_node);
        return DRV_ERR_TDE_NO_MEM;
    }
    tde_osi_convert_surface(single_src->src_surface, single_src->src_rect, &src_scan_info, drv_surface);
    drv_surface->cma = mmz_for_src;

#if (TDE_CAPABILITY & COMPRESS)
    (opt->is_decompress) ? tde_hal_node_set_src_to_decompress(hw_node, drv_surface)
                         : tde_hal_node_set_src2(hw_node, drv_surface);
#else
    tde_hal_node_set_src2(hw_node, drv_surface);
#endif

    tde_osi_convert_surface(single_src->dst_surface, single_src->dst_rect, &dst_scan_info, drv_surface);
    if (opt->out_alpha_from >= DRV_TDE_OUT_ALPHA_FROM_MAX || opt->out_alpha_from == DRV_TDE_OUT_ALPHA_FROM_BACKGROUND) {
        tde_error("out_alpha_from error!\n");
        ret = DRV_ERR_TDE_INVALID_PARA;
        goto err;
    }

    drv_surface->cma = mmz_for_dst;
    ret = tde_osi_single_opt(opt, single_src, drv_surface, hw_node);
    if (ret != TD_SUCCESS) {
        goto err;
    }

    ret = tde_osi_single_node(handle, opt, single_src, drv_surface, hw_node);
    if (ret != TD_SUCCESS) {
        goto err;
    }

    return TD_SUCCESS;
err:
    tde_hal_free_node_buf(hw_node);
    tde_free((td_void *)drv_surface);
    return ret;
}

static td_s32 tde_osi_check_single_src_to_para(const drv_tde_surface *fore_ground, const drv_tde_rect *fore_ground_rect,
                                               const drv_tde_surface *dst_surface, const drv_tde_rect *dst_rect,
                                               const drv_tde_opt *opt)
{
    td_bool null_ptr;

    if (opt == TD_NULL) {
        tde_error("pstOpt is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    null_ptr = ((dst_surface == TD_NULL) || (dst_rect == TD_NULL) || ((opt == TD_NULL)) ||
                (fore_ground == TD_NULL) || (fore_ground_rect == TD_NULL));

    if (null_ptr) {
        return DRV_ERR_TDE_NULL_PTR;
    }

    if (tde_osi_check_single_src_to_opt(fore_ground->color_format, dst_surface->color_format, opt) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (opt->resize) {
        if (tde_osi_check_resize_para(fore_ground_rect->width, fore_ground_rect->height,
            dst_rect->width, dst_rect->height) != TD_SUCCESS) {
            return DRV_ERR_TDE_MINIFICATION;
        }
    }
#if TDE_CAPABILITY & COMPRESS
    if (opt->is_compress) {
        if (tde_osi_check_compress_para(fore_ground, fore_ground_rect, dst_surface, dst_rect, opt) !=
            TD_SUCCESS) {
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }

    if (opt->is_decompress) {
        if (tde_osi_check_decompress_para(fore_ground, fore_ground_rect, dst_surface, dst_rect, opt) !=
            TD_SUCCESS) {
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
#endif

    return TD_SUCCESS;
}

static td_s32 tde_osi_double_para(drv_tde_double_src *double_src, const drv_tde_opt *opt, tde_hw_node **hw_node)
{
    td_bool null_ptr = ((double_src->bg_surface == TD_NULL) || ((double_src->bg_rect == TD_NULL)) ||
                        (double_src->fg_surface == TD_NULL) || ((double_src->fg_rect == TD_NULL)) ||
                        (double_src->dst_surface == TD_NULL) || ((double_src->dst_rect == TD_NULL)) ||
                        (opt == TD_NULL));

    if (null_ptr) {
        return DRV_ERR_TDE_NULL_PTR;
    }

    if (tde_osi_check_double_src_opt(double_src->bg_surface->color_format, double_src->fg_surface->color_format,
        double_src->dst_surface->color_format, opt) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (opt->resize) {
        if (tde_osi_check_resize_para(double_src->fg_rect->width, double_src->fg_rect->height,
            double_src->dst_rect->width, double_src->dst_rect->height) != TD_SUCCESS) {
            return DRV_ERR_TDE_MINIFICATION;
        }
    }

#if (TDE_CAPABILITY & COMPRESS)
    if (opt->is_compress) {
        if (tde_osi_check_compress_para(double_src->fg_surface, double_src->fg_rect,
            double_src->dst_surface, double_src->dst_rect, opt) < 0) {
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }

    if (opt->is_decompress) {
        if (tde_osi_check_decompress_para(double_src->fg_surface, double_src->fg_rect,
            double_src->dst_surface, double_src->dst_rect, opt) < 0) {
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
#endif

    if (tde_hal_node_init_nd(hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }

    if (tde_osi_set_clip_para(double_src, opt, *hw_node) != TD_SUCCESS) {
        tde_hal_free_node_buf(*hw_node);
        return DRV_ERR_TDE_CLIP_AREA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_double_opt(const drv_tde_double_src *double_src, const drv_tde_opt *opt, tde_hw_node *hw_node,
    tde_surface_msg *drv_surface)
{
    td_s32 ret;
    td_u16 code;
    tde_conv_mode_cmd conv = { 0 };
    tde_clut_usage clut_usage = TDE_CLUT_USAGE_BUTT;

    if (opt->out_alpha_from >= DRV_TDE_OUT_ALPHA_FROM_MAX) {
        tde_error("out_alpha_from error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

#if (TDE_CAPABILITY & COMPRESS)
    if (opt->is_compress) {
        tde_hal_node_set_compress_tqt(hw_node, drv_surface, opt->out_alpha_from);
        tde_hal_node_set_compress(hw_node);
    } else {
        tde_hal_node_set_tqt(hw_node, drv_surface, opt->out_alpha_from);
    }
#else
    tde_hal_node_set_tqt(hw_node, drv_surface, opt->out_alpha_from);
#endif

    code = tde_osi_double_src_get_opt_code(double_src->bg_surface->color_format,
        double_src->fg_surface->color_format, double_src->dst_surface->color_format);

    tde_osi_get_conv_by_code(code, &conv);

    if (tde_hal_node_set_color_convert(hw_node, &conv) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    ret = tde_osi_set_clut_opt(double_src->fg_surface, double_src->dst_surface,
                               &clut_usage, opt->clut_reload, hw_node);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = tde_osi_set_color_key(double_src, hw_node,
        opt->colorkey_value, opt->colorkey_mode, clut_usage);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = tde_osi_set_base_opt_para_for_blit(opt, double_src->bg_surface, double_src->fg_surface,
        TDE_OPERATION_DOUBLE_SRC, hw_node);
    if (ret != TD_SUCCESS) {
        tde_error("Set base opt para for blit failed, ret = 0x%x\n", ret);
    }
    tde_osi_set_ext_alpha(double_src->bg_surface, double_src->fg_surface, hw_node);
    return TD_SUCCESS;
}

static td_s32 tde_osi_double_node(td_s32 handle, drv_tde_double_src *double_src, const drv_tde_opt *opt,
    tde_hw_node *hw_node)
{
    td_s32 ret;
    td_bool set_filter_node;

#if (TDE_CAPABILITY & SLICE)
    set_filter_node = ((opt->resize) || ((opt->deflicker_mode != DRV_TDE_DEFLICKER_LEVEL_MODE_NONE)) ||
                       (double_src->fg_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422) ||
                       (double_src->bg_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422));
#else
    set_filter_node = ((opt->resize) || (opt->deflicker_mode != DRV_TDE_DEFLICKER_LEVEL_MODE_NONE) ||
                       (double_src->fg_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422));
#endif
    if (set_filter_node) {
#if (TDE_CAPABILITY & SLICE)
        double_src->bg_rect->height = double_src->dst_rect->height;
        double_src->bg_rect->width = double_src->dst_rect->width;

        if ((double_src->fg_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422) && (!(opt->resize))) {
            double_src->fg_rect->height = double_src->dst_rect->height;
            double_src->fg_rect->width = double_src->dst_rect->width;
        }
        ret = tde_osi_set_filter_node(handle, hw_node, double_src,
                                      opt->deflicker_mode, opt->filter_mode);
        if (ret != TD_SUCCESS) {
            return ret;
        }
        tde_hal_free_node_buf(hw_node);
#else
        ret = tde_osi_set_filter_node(handle, hw_node, double_src, opt->deflicker_mode,
            (drv_tde_deflicker_mode)opt->filter_mode);
        if (ret != TD_SUCCESS) {
            return ret;
        }
#endif
        return TD_SUCCESS;
    }
    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_double_src_2_blit
 * Description:   dual source operate realization
 * Input:         double_src:  bitmap info struct
 *                opt: config parameter
 * Return:        success/fail
 * Others:        add support for YCbCr422
 */
static td_s32 tde_osi_double_src_2_blit(td_s32 handle, drv_tde_double_src *double_src, const drv_tde_opt *opt)
{
    tde_hw_node *hw_node = TD_NULL;
    tde_scandirection_mode src_scan_info = { 0 };
    tde_scandirection_mode dst_scan_info = { 0 };
    tde_surface_msg drv_surface = { 0 };
    td_s32 ret;
    drv_tde_single_src single_src;

    ret = tde_osi_double_para(double_src, opt, &hw_node);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    single_src.src_surface = double_src->fg_surface;
    single_src.src_rect = double_src->fg_rect;
    single_src.dst_surface = double_src->dst_surface;
    single_src.dst_rect = double_src->dst_rect;
    if (tde_osi_get_scan_info_ex(&single_src, opt, &src_scan_info, &dst_scan_info) != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return DRV_ERR_TDE_INVALID_PARA;
    }

#if (TDE_CAPABILITY & COMPRESS)
    if ((opt->is_compress) || (opt->is_decompress)) {
        src_scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
        src_scan_info.ver_scan = TDE_SCAN_UP_DOWN;
        dst_scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
        dst_scan_info.ver_scan = TDE_SCAN_UP_DOWN;
    }
#endif


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART13_H_ */
