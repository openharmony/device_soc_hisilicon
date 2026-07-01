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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART02_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART02_H_

static td_s32 tde_osi_check_resize_para(td_u32 in_width, td_u32 in_height,
                                        td_u32 out_width, td_u32 out_height);
static td_bool tde_osi_whether_contain_alpha(drv_tde_color_fmt color_fmt);
static td_void tde_osi_set_ext_alpha(const drv_tde_surface *back_ground, const drv_tde_surface *fore_ground,
                                     tde_hw_node *hw_node);

static td_s32 tde_osi_pre_check_surface_ex(const drv_tde_surface *surface, drv_tde_rect *rect);

static td_s32 tde_osi_raster_fmt_check_align(const drv_tde_surface *surface);

static td_s32 tde_osi_check_mb_blit_para(const drv_tde_mb_src *mb_src, const drv_tde_mb_opt *mb_opt);

static td_s32 tde_osi_get_double_cycle_data(const drv_tde_rect *mb_rect, td_u32 *i, td_u32 *j);
static td_s32 tde_osi_get_bpp_by_fmt_1(drv_tde_color_fmt fmt);
static td_s32 tde_osi_get_bpp_by_fmt_2(drv_tde_color_fmt fmt);
static td_s32 tde_osi_get_bpp_by_fmt_4(drv_tde_color_fmt fmt);
static td_s32 tde_osi_get_bpp_by_fmt_8(drv_tde_color_fmt fmt);
static td_s32 tde_osi_get_bpp_by_fmt_16(drv_tde_color_fmt fmt);
static td_s32 tde_osi_get_bpp_by_fmt_24(drv_tde_color_fmt fmt);
static td_s32 tde_osi_get_bpp_by_fmt_32(drv_tde_color_fmt fmt);
static td_s32 tde_osi_check_single_src_to_para(const drv_tde_surface *fore_ground, const drv_tde_rect *fore_ground_rect,
                                               const drv_tde_surface *dst_surface, const drv_tde_rect *dst_rect,
                                               const drv_tde_opt *opt);

static td_s32 tde_osi_double_src_2_blit(td_s32 handle, drv_tde_double_src *double_src, const drv_tde_opt *opt);

static td_void tde_unify_rect(drv_tde_rect *src_rect, drv_tde_rect *dst_rect)
{
    if (src_rect->height != dst_rect->height) {
        src_rect->height = tde_min(src_rect->height, dst_rect->height);
        dst_rect->height = src_rect->height;
    }
    if (src_rect->width != dst_rect->width) {
        src_rect->width = tde_min(src_rect->width, dst_rect->width);
        dst_rect->width = src_rect->width;
    }

    return;
}

static tde2_fmt_bitoffset g_fmt_bit_and_offset_array[DRV_TDE_COLOR_FMT_AYCBCR8888 + 1] = {
    { 0, 4, 4, 4, 12, 8,  4,  0 },  /* DRV_TDE_COLOR_FMT_RGB444 */
    { 0, 4, 4, 4, 12, 0,  4,  8 },  /* DRV_TDE_COLOR_FMT_BGR444 */
    { 0, 5, 5, 5, 15, 10, 5,  0 }, /* DRV_TDE_COLOR_FMT_RGB555 */
    { 0, 5, 5, 5, 15, 0,  5,  10 }, /* DRV_TDE_COLOR_FMT_BGR555 */
    { 0, 5, 6, 5, 16, 11, 5,  0 }, /* DRV_TDE_COLOR_FMT_RGB565 */
    { 0, 5, 6, 6, 16, 0,  5,  11 }, /* DRV_TDE_COLOR_FMT_BGR565 */
    { 0, 8, 8, 8, 24, 16, 8,  0 }, /* DRV_TDE_COLOR_FMT_RGB888 */
    { 0, 8, 8, 8, 24, 0,  8,  16 }, /* DRV_TDE_COLOR_FMT_BGR888 */
    { 4, 4, 4, 4, 12, 8,  4,  0 },  /* DRV_TDE_COLOR_FMT_ARGB4444 */
    { 4, 4, 4, 4, 12, 0,  4,  8 },  /* DRV_TDE_COLOR_FMT_ABGR4444 */
    { 4, 4, 4, 4, 0,  12, 8,  4 },  /* DRV_TDE_COLOR_FMT_RGBA4444 */
    { 4, 4, 4, 4, 0,  4,  8,  12 },  /* DRV_TDE_COLOR_FMT_BGRA4444 */
    { 1, 5, 5, 5, 15, 10, 5,  0 }, /* DRV_TDE_COLOR_FMT_ARGB1555 */
    { 1, 5, 5, 5, 15, 0,  5,  10 }, /* DRV_TDE_COLOR_FMT_ABGR1555 */
    { 1, 5, 5, 5, 0,  11, 6,  1 },  /* DRV_TDE_COLOR_FMT_RGBA1555 */
    { 1, 5, 5, 5, 0,  1,  6,  11 },  /* DRV_TDE_COLOR_FMT_BGRA1555 */
    { 8, 5, 6, 5, 16, 11, 5,  0 }, /* DRV_TDE_COLOR_FMT_ARGB8565 */
    { 8, 5, 6, 5, 16, 0,  5,  11 }, /* DRV_TDE_COLOR_FMT_ABGR8565 */
    { 8, 5, 6, 5, 0,  19, 13, 8 }, /* DRV_TDE_COLOR_FMT_RGBA8565 */
    { 8, 5, 6, 6, 0,  8,  13, 19 }, /* DRV_TDE_COLOR_FMT_BGRA8565 */
    { 8, 8, 8, 8, 24, 16, 8,  0 }, /* DRV_TDE_COLOR_FMT_ARGB8888 */
    { 8, 8, 8, 8, 24, 0,  8,  16 }, /* DRV_TDE_COLOR_FMT_ABGR8888 */
    { 8, 8, 8, 8, 0,  24, 16, 8 }, /* DRV_TDE_COLOR_FMT_RGBA8888 */
    { 8, 8, 8, 8, 0,  8,  16, 24 }, /* DRV_TDE_COLOR_FMT_BGRA8888 */
    { 8, 8, 8, 8, 16, 24, 0,  8 }, /* DRV_TDE_COLOR_FMT_RABG8888 */

    { 0, 0, 0, 0, 0,  0,  0, 0 },
    { 0, 0, 0, 0, 0,  0,  0, 0 },
    { 0, 0, 0, 0, 0,  0,  0, 0 },
    { 0, 0, 0, 0, 0,  0,  0, 0 },
    { 0, 0, 0, 0, 0,  0,  0, 0 },
    { 0, 0, 0, 0, 0,  0,  0, 0 },
    { 0, 0, 0, 0, 0,  0,  0, 0 },
    { 0, 0, 0, 0, 0,  0,  0, 0 },
    { 0, 8, 8, 8, 24, 16, 8, 0 }, /* DRV_TDE_COLOR_FMT_YCBCR888 */
    { 8, 8, 8, 8, 24, 16, 8, 0 }, /* DRV_TDE_COLOR_FMT_AYCBCR8888 */
};

tde_color_fmt tde_get_common_drv_color_fmt(td_u32 count)
{
    return g_tde_common_drv_color_fmt[count];
}

static td_s32 color_convert_check(const drv_tde_fill_color *fill_color, const drv_tde_surface *sur,
                                  const td_u32 *out_color, tde_colorfmt_transform *color_trans)
{
    *color_trans = tde_osi_get_fmt_trans_type(fill_color->color_format, sur->color_format);

    if (((fill_color->color_format >= DRV_TDE_COLOR_FMT_CLUT1) &&
        (fill_color->color_format <= DRV_TDE_COLOR_FMT_A8)) ||
        (fill_color->color_format >= DRV_TDE_COLOR_FMT_YCBCR422)) {
        tde_error("Unsupported color!\n");
        return -1;
    }
    return 0;
}

typedef struct {
    td_u8 a;
    td_u8 r;
    td_u8 g;
    td_u8 b;
} tde_osi_argb;

static td_void tde_osi_get_fill_argb(const drv_tde_fill_color *fill_color, const drv_tde_surface *sur,
    tde_osi_argb *argb)
{
    argb->a = (fill_color->color_value >> g_fmt_bit_and_offset_array[fill_color->color_format].alpha_offset) &
        (0xff >> (8 - g_fmt_bit_and_offset_array[fill_color->color_format].alpha_bits)); /* 8 Data from the */
    argb->r = (fill_color->color_value >> g_fmt_bit_and_offset_array[fill_color->color_format].red_offset) &
        (0xff >> (8 - g_fmt_bit_and_offset_array[fill_color->color_format].red_bits)); /* 8 Data from the */
    argb->g = (fill_color->color_value >> g_fmt_bit_and_offset_array[fill_color->color_format].green_offset) &
        (0xff >> (8 - g_fmt_bit_and_offset_array[fill_color->color_format].green_bits)); /* 8 Data from the */
    argb->b = (fill_color->color_value >> g_fmt_bit_and_offset_array[fill_color->color_format].blue_offset) &
        (0xff >> (8 - g_fmt_bit_and_offset_array[fill_color->color_format].blue_bits)); /* 8 Data from the */

    if ((DRV_TDE_COLOR_FMT_ARGB1555 <= fill_color->color_format) &&
        (fill_color->color_format <= DRV_TDE_COLOR_FMT_BGRA1555)) {
        argb->a = (argb->a != 0) ? sur->alpha1 : sur->alpha0;
    } else {
        argb->a = argb->a << (8 - g_fmt_bit_and_offset_array[fill_color->color_format].alpha_bits); /* 8 Data bit */
    }
    argb->r = argb->r << (8 - g_fmt_bit_and_offset_array[fill_color->color_format].red_bits);   /* 8 Data bit */
    argb->g = argb->g << (8 - g_fmt_bit_and_offset_array[fill_color->color_format].green_bits); /* 8 Data bit */
    argb->b = argb->b << (8 - g_fmt_bit_and_offset_array[fill_color->color_format].blue_bits);  /* 8 Data bit */
}

static td_s32 tde_osi_color_convert(const drv_tde_fill_color *fill_color, const drv_tde_surface *sur, td_u32 *out_color)
{
    tde_osi_argb argb;
    td_u8 y;
    td_u8 cb;
    td_u8 cr;
    td_s32 ret;
    tde_colorfmt_transform color_trans;

    ret = color_convert_check(fill_color, sur, out_color, &color_trans);
    if (ret != 0) {
        return ret;
    }
    tde_osi_get_fill_argb(fill_color, sur, &argb);

    switch (color_trans) {
        case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
        case TDE_COLORFMT_TRANSFORM_YCBCR_YCBCR:
            *out_color = (argb.a << 24) + (argb.r << 16) + (argb.g << 8) + argb.b; /* 8 16 24 bit */
            return 0;
        case TDE_COLORFMT_TRANSFORM_ARGB_YCBCR:
            rgb2ycc(argb.r, argb.g, argb.b, y, cb, cr);
            *out_color = (argb.a << 24) + (y << 16) + (cb << 8) + cr; /* 8 16 24 bit */
            return 0;
        case TDE_COLORFMT_TRANSFORM_YCBCR_ARGB:
            ycc2rgb(argb.r, argb.g, argb.b, y, cb, cr);
            *out_color = (argb.a << 24) + (y << 16) + (cb << 8) + cr; /* 8 16 24 bit */
            return 0;
        default:
            tde_error("Unsupported color transport!\n");
            return -1;
    }
}

/*
 * Function:      tde_osi_check_resize_para
 * Description:   check zoom ratio limit
 * Return:        TDE_COLORFMT_CATEGORY_E   pixel format category
 */
static td_s32 tde_osi_check_resize_para(td_u32 in_width, td_u32 in_height,
                                        td_u32 out_width, td_u32 out_height)
{
    if (((in_width > TDE_MAX_RECT_WIDTH)) ||
        (in_height > TDE_MAX_RECT_HEIGHT) || ((out_width > TDE_MAX_RECT_WIDTH)) ||
        (out_height > TDE_MAX_RECT_HEIGHT)) {
        if ((in_width != out_width) || (in_height != out_height)) {
            tde_error("input width/height(%d, %d) not equal to output width/height(%d, %d) and greater than max "
                "width/height (%d, %d)\n", in_width, in_height, out_width, out_height,
                TDE_MAX_RECT_WIDTH, TDE_MAX_RECT_HEIGHT);
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }

    if (((out_width * TDE_MAX_MINIFICATION_H) < in_width) ||
        ((out_height * TDE_MAX_MINIFICATION_V) < in_height)) {
        tde_error("Resize parameter error!The zoom-out ratio is less than or equal to 255!"
            "input width/height(%d, %d), output width/height(%d, %d), \n", in_width, in_height, out_width, out_height);
        return -1;
    } else {
        return 0;
    }
}

#if (TDE_CAPABILITY & ROTATE)
static td_s32 tde_osi_check_rotate_para(const drv_tde_single_src *single_src, drv_tde_rotate_angle rotate_angle)
{
    td_bool is_unsupported_format = ((single_src->src_surface->color_format != DRV_TDE_COLOR_FMT_YCBCR422) &&
                                   (single_src->src_surface->color_format != DRV_TDE_COLOR_FMT_ARGB8888) &&
                                   (single_src->src_surface->color_format != DRV_TDE_COLOR_FMT_ARGB4444) &&
                                   (single_src->src_surface->color_format != DRV_TDE_COLOR_FMT_ARGB1555));
    td_bool is_invalid;

    if (rotate_angle >= DRV_TDE_ROTATE_MAX) {
        tde_error("rotate mode error! please choose TDE_ROTATE_CLOCKWISE_90 or 180 or 270\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (single_src->src_surface->color_format != single_src->dst_surface->color_format) {
        tde_error("rotate only support src_surface->color_format == dst_surface->color_format\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    if (is_unsupported_format) {
        tde_error("rotate operation can not support the format!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    is_invalid = ((single_src->src_surface->color_format == DRV_TDE_COLOR_FMT_YCBCR422) ||
        (single_src->src_surface->color_format == DRV_TDE_COLOR_FMT_PKGVYUY)) &&
        ((single_src->src_rect->height & 0x1) || (single_src->src_rect->width & 0x1));
    if (is_invalid == TD_TRUE) {
        tde_error("height, width of YCbCr422R couldn't be odd!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    is_invalid = (single_src->src_surface->phys_addr % 4) || (single_src->dst_surface->phys_addr % 4) || /* 4 align */
        (single_src->src_surface->stride % 4) || (single_src->dst_surface->stride % 4); /* 4 align */
    if (is_invalid == TD_TRUE) {
        tde_error("bitmap address is not 4 aligned nor stride is not 4 aligned!\n");
        return DRV_ERR_TDE_NOT_ALIGNED;
    }

    is_invalid = (rotate_angle != DRV_TDE_ROTATE_CLOCKWISE_180) &&
        ((single_src->src_rect->height != single_src->dst_rect->width) ||
        (single_src->src_rect->width != single_src->dst_rect->height));
    if (is_invalid == TD_TRUE) {
        tde_error("rotate operation rect is wrong!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    is_invalid = (rotate_angle == DRV_TDE_ROTATE_CLOCKWISE_180) &&
        ((single_src->src_rect->height != single_src->dst_rect->height) ||
        (single_src->src_rect->width != single_src->dst_rect->width));
    if (is_invalid == TD_TRUE) {
        tde_error("rotate 180 operation rect is wrong!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return 0;
}
#endif

#if (TDE_CAPABILITY & COMPRESS)
static td_s32 tde_osi_check_compress_para(const drv_tde_surface *fg_surface, const drv_tde_rect *fg_rect,
                                          const drv_tde_surface *dst_surface, const drv_tde_rect *dst_rect,
                                          const drv_tde_opt *opt)
{
    ot_unused(fg_surface);
    ot_unused(fg_rect);
    ot_unused(dst_rect);
    if ((dst_surface->color_format != DRV_TDE_COLOR_FMT_ARGB1555) &&
        (dst_surface->color_format != DRV_TDE_COLOR_FMT_ARGB8888) &&
        (dst_surface->color_format != DRV_TDE_COLOR_FMT_ARGB4444) &&
        (dst_surface->color_format != DRV_TDE_COLOR_FMT_RGB888)   &&
        (dst_surface->color_format != DRV_TDE_COLOR_FMT_YCBCR888) &&
        (dst_surface->color_format != DRV_TDE_COLOR_FMT_RGB565)) {
        tde_error("Compress operation can not support the fmt !\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (opt->mirror) {
        tde_error("Could not support Mirror\n!");
        return -1;
    }
    if (opt->clip_mode != DRV_TDE_CLIP_MODE_NONE) {
        tde_error("Could not support Clip\n!");
        return -1;
    }
    return 0;
}

static td_s32 tde_osi_check_decompress_para(const drv_tde_surface *fg_surface, const drv_tde_rect *fg_rect,
                                            const drv_tde_surface *dst_surface, const drv_tde_rect *dst_rect,
                                            const drv_tde_opt *opt)
{
    ot_unused(fg_rect);
    ot_unused(dst_surface);
    ot_unused(dst_rect);
    if ((fg_surface->color_format != DRV_TDE_COLOR_FMT_ARGB1555) &&
        (fg_surface->color_format != DRV_TDE_COLOR_FMT_ARGB8888) &&
        (fg_surface->color_format != DRV_TDE_COLOR_FMT_ARGB4444) &&
        (fg_surface->color_format != DRV_TDE_COLOR_FMT_RGB888)   &&
        (fg_surface->color_format != DRV_TDE_COLOR_FMT_YCBCR888) &&
        (fg_surface->color_format != DRV_TDE_COLOR_FMT_RGB565)) {
        tde_error("Compress operation can not support the fmt !\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (opt->mirror) {
        tde_error("Could not support Mirror\n!");
        return -1;
    }
    return 0;
}

#endif

static drv_tde_color_fmt tde_osi_covert_mb_fmt(drv_tde_mb_color_fmt fmt)
{
    switch (fmt) {
        case DRV_TDE_MB_COLOR_FMT_JPG_YCBCR400MBP:
            return DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP;
        case DRV_TDE_MB_COLOR_FMT_JPG_YCBCR422MBHP:
            return DRV_TDE_COLOR_FMT_JPG_YCBCR422MBHP;
        case DRV_TDE_MB_COLOR_FMT_JPG_YCBCR422MBVP:
            return DRV_TDE_COLOR_FMT_JPG_YCBCR422MBVP;
        case DRV_TDE_MB_COLOR_FMT_MP1_YCBCR420MBP:
            return DRV_TDE_COLOR_FMT_MP1_YCBCR420MBP;
        case DRV_TDE_MB_COLOR_FMT_MP2_YCBCR420MBP:
            return DRV_TDE_COLOR_FMT_MP2_YCBCR420MBP;
        case DRV_TDE_MB_COLOR_FMT_MP2_YCBCR420MBI:
            return DRV_TDE_COLOR_FMT_MP2_YCBCR420MBI;
        case DRV_TDE_MB_COLOR_FMT_JPG_YCBCR420MBP:
            return DRV_TDE_COLOR_FMT_JPG_YCBCR420MBP;
        case DRV_TDE_MB_COLOR_FMT_JPG_YCBCR444MBP:
            return DRV_TDE_COLOR_FMT_JPG_YCBCR444MBP;
        default:
            return DRV_TDE_COLOR_FMT_MAX;
    }
}

/*
 * Function:      tde_osi_get_fmt_category
 * Description:   get pixel format category info
 * Input:         fmt: pixel format
 * Return:        TDE_COLORFMT_CATEGORY_E  pixel format category
 */
static tde_colorfmt_category tde_osi_get_fmt_category(drv_tde_color_fmt fmt)
{
    /* target is ARGB format */
    if (fmt <= DRV_TDE_COLOR_FMT_RABG8888) {
        return TDE_COLORFMT_CATEGORY_ARGB;
    } else if (fmt <= DRV_TDE_COLOR_FMT_ACLUT88) {
        /* target is CLUT table format */
        return TDE_COLORFMT_CATEGORY_CLUT;
    } else if (fmt <= DRV_TDE_COLOR_FMT_A8) {
        /* target is alpha CLUT table format */
        return TDE_COLORFMT_CATEGORY_AN;
    } else if (fmt <= DRV_TDE_COLOR_FMT_PKGVYUY) {
        /* target is YCbCr format */
        return TDE_COLORFMT_CATEGORY_YCBCR;
    } else if (fmt == DRV_TDE_COLOR_FMT_BYTE) {
        /* byte format */
        return TDE_COLORFMT_CATEGORY_BYTE;
    } else if (fmt == DRV_TDE_COLOR_FMT_HALFWORD) {
        /* halfword  format */
        return TDE_COLORFMT_CATEGORY_HALFWORD;
    } else if (fmt <= DRV_TDE_COLOR_FMT_JPG_YCBCR444MBP) {
        return TDE_COLORFMT_CATEGORY_YCBCR;
    } else {
        /* error format */
        return TDE_COLORFMT_CATEGORY_BUTT;
    }
}

/*
 * Function:      tde_osi_get_fmt_trans_type
 * Description:   get pixel format transform type
 * Input:         src2_fmt: foreground pixel format
                  dst_fmt: target pixel format
 * Return:        TDE_COLORFMT_TRANSFORM_E pixel format transform type
 */
static tde_colorfmt_transform tde_osi_get_fmt_trans_type(drv_tde_color_fmt src2_fmt, drv_tde_color_fmt dst_fmt)
{
    tde_colorfmt_category src_category;
    tde_colorfmt_category dst_category;

    /* get foreground pixel format category */
    src_category = tde_osi_get_fmt_category(src2_fmt);

    /* get target pixel format category */
    dst_category = tde_osi_get_fmt_category(dst_fmt);

    switch (src_category) {
        case TDE_COLORFMT_CATEGORY_ARGB:
            if (dst_category == TDE_COLORFMT_CATEGORY_ARGB) {
                return TDE_COLORFMT_TRANSFORM_ARGB_ARGB;
            } else if (dst_category == TDE_COLORFMT_CATEGORY_YCBCR) {
                return TDE_COLORFMT_TRANSFORM_ARGB_YCBCR;
            } else if (dst_category == TDE_COLORFMT_CATEGORY_AN) {
                return TDE_COLORFMT_TRANSFORM_ARGB_AN;
            }
            return TDE_COLORFMT_TRANSFORM_BUTT;

        case TDE_COLORFMT_CATEGORY_CLUT:
            if (dst_category == TDE_COLORFMT_CATEGORY_ARGB) {
                return TDE_COLORFMT_TRANSFORM_CLUT_ARGB;
            } else if (dst_category == TDE_COLORFMT_CATEGORY_YCBCR) {
                return TDE_COLORFMT_TRANSFORM_CLUT_YCBCR;
            } else if (dst_category == TDE_COLORFMT_CATEGORY_CLUT) {
                return TDE_COLORFMT_TRANSFORM_CLUT_CLUT;
            }
            return TDE_COLORFMT_TRANSFORM_BUTT;

        case TDE_COLORFMT_CATEGORY_YCBCR:
            if (dst_category == TDE_COLORFMT_CATEGORY_ARGB) {
                return TDE_COLORFMT_TRANSFORM_YCBCR_ARGB;
            } else if (dst_category == TDE_COLORFMT_CATEGORY_YCBCR) {
                return TDE_COLORFMT_TRANSFORM_YCBCR_YCBCR;
            } else if (dst_category == TDE_COLORFMT_CATEGORY_AN) {
                return TDE_COLORFMT_TRANSFORM_ARGB_AN;
            }
            return TDE_COLORFMT_TRANSFORM_BUTT;

        case TDE_COLORFMT_CATEGORY_AN:
            if (dst_category == TDE_COLORFMT_CATEGORY_AN) {
                return TDE_COLORFMT_TRANSFORM_AN_AN;
            }
            return TDE_COLORFMT_TRANSFORM_BUTT;

        default:
            return TDE_COLORFMT_TRANSFORM_BUTT;
    }
}

/*
 * Function:      tde_osi_is_single_src_to_rop
 * Description:   query if ROP operate is if single source2 operate
 * Input:         rop: rop operate type
 * Return:        TD_TRUE: single ROP;TD_FALSE: non single ROP
 */


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART02_H_ */
