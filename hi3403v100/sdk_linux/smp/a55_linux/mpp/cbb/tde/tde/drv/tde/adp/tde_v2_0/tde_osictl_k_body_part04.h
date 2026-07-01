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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART04_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART04_H_

    td_u16 code2;
    switch (color_trans_type) {
        case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
            return 0x0;

        case TDE_COLORFMT_TRANSFORM_ARGB_YCBCR:
            return 0x2;

        case TDE_COLORFMT_TRANSFORM_CLUT_ARGB:
            return 0x8;

        case TDE_COLORFMT_TRANSFORM_CLUT_YCBCR:
            return 0xa;

        case TDE_COLORFMT_TRANSFORM_YCBCR_ARGB:
            code2 = 0x2 | 0x4;
            if ((src2_fmt >= DRV_TDE_COLOR_FMT_CLUT1) && (src2_fmt <= DRV_TDE_COLOR_FMT_ACLUT88)) {
                code2 = 0;
            }
            return code2;

        case TDE_COLORFMT_TRANSFORM_YCBCR_YCBCR:
            code2 = 0x0;
            if ((src2_fmt >= DRV_TDE_COLOR_FMT_CLUT1) && (src2_fmt <= DRV_TDE_COLOR_FMT_ACLUT88)) {
                code2 = 0x2;
            }
            return code2;

        default:
            return 0x8000;
    }
}

/*
 * Function:      tde_osi_double_src_get_opt_code
 * Description:   get dual source operate encode
 * Input:         src1_fmt background pixel format
                  src2_fmt foreground pixel format
                  dst_fmt: target pixel format
 * Return:        code value
 */
static td_u16 tde_osi_double_src_get_opt_code(drv_tde_color_fmt src1_fmt, drv_tde_color_fmt src2_fmt,
                                              drv_tde_color_fmt dst_fmt)
{
    td_u16 code1;
    td_u16 code2;
    tde_colorfmt_transform color_trans_type;

    color_trans_type = tde_osi_get_fmt_trans_type(src2_fmt, src1_fmt);
    code1 = tde_osi_get_code1(color_trans_type);
    if (code1 == 0x8000) {
        return code1;
    }

    color_trans_type = tde_osi_get_fmt_trans_type(src1_fmt, dst_fmt);

    code2 = tde_osi_get_code2(color_trans_type, src2_fmt);
    if (code2 == 0x8000) {
        return code2;
    }
    return (code1 | code2);
}

static td_u16 tde_osi_single_src2_get_opt_code(drv_tde_color_fmt src2_fmt, drv_tde_color_fmt dst_fmt)
{
    td_u16 code;
    tde_colorfmt_transform color_trans_type;

    color_trans_type = tde_osi_get_fmt_trans_type(src2_fmt, dst_fmt);

    switch (color_trans_type) {
        case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
            code = 0x0;
            break;

        case TDE_COLORFMT_TRANSFORM_ARGB_YCBCR:
            code = 0x5;
            break;

        case TDE_COLORFMT_TRANSFORM_CLUT_ARGB:
            code = 0x8;
            break;

        case TDE_COLORFMT_TRANSFORM_CLUT_CLUT:
            code = 0x0;
            break;

        case TDE_COLORFMT_TRANSFORM_CLUT_YCBCR:
            code = 0xA;
            break;

        case TDE_COLORFMT_TRANSFORM_YCBCR_ARGB:
            code = 0x1;
            break;

        case TDE_COLORFMT_TRANSFORM_YCBCR_YCBCR:
            code = 0x0;
            break;

        default:
            code = 0x8000;
    }
    return code;
}

/*
 * Function:      tde_osi_get_conv_by_code
 * Description:   get format conversion manner by format conversion code
 * Input:         code  format conversion code
 *                conv  format conversion struct
 * Return:        encode value
 */
static td_void tde_osi_get_conv_by_code(td_u16 code, tde_conv_mode_cmd *conv)
{
    conv->in_conv = code & 0x1;
    conv->out_conv = (code >> 1) & 0x1;
    conv->in_rgb2_yc = ((code >> 2) & 0x1);   /* 2 Expand the digits */
    conv->in_src1_conv = ((code >> 4) & 0x1); /* 4 Expand the digits */

    return;
}

/*
 * Function:      tde_osi_get_bpp_by_fmt
 * Description:   get pixel bit of pixel format
 * Input:         fmt  target pixel format
 * Return:        -1 fail; other:pixel bit
 */
td_s32 tde_osi_get_bpp_by_fmt(drv_tde_color_fmt fmt)
{
    if (tde_osi_get_bpp_by_fmt_16(fmt) == 16) { /* 16 bpp fmt */
        return 16;
    } else if (tde_osi_get_bpp_by_fmt_24(fmt) == 24) { /* 24 bpp fmt */
        return 24;
    } else if (tde_osi_get_bpp_by_fmt_32(fmt) == 32) { /* 32 bpp fmt */
        return 32;
    } else if (tde_osi_get_bpp_by_fmt_1(fmt) == 1) {
        return 1;
    } else if (tde_osi_get_bpp_by_fmt_2(fmt) == 2) { /* 2 bpp fmt */
        return 2;
    } else if (tde_osi_get_bpp_by_fmt_4(fmt) == 4) { /* 4 bpp fmt */
        return 4;
    } else if (tde_osi_get_bpp_by_fmt_8(fmt) == 8) { /* 8 bpp fmt */
        return 8;
    } else {
        return -1;
    }
}

static td_s32 tde_osi_get_bpp_by_fmt_16(drv_tde_color_fmt fmt)
{
    switch (fmt) {
        case DRV_TDE_COLOR_FMT_RGB444:
        case DRV_TDE_COLOR_FMT_BGR444:
        case DRV_TDE_COLOR_FMT_RGB555:
        case DRV_TDE_COLOR_FMT_BGR555:
        case DRV_TDE_COLOR_FMT_RGB565:
        case DRV_TDE_COLOR_FMT_BGR565:
        case DRV_TDE_COLOR_FMT_ARGB4444:
        case DRV_TDE_COLOR_FMT_ABGR4444:
        case DRV_TDE_COLOR_FMT_RGBA4444:
        case DRV_TDE_COLOR_FMT_BGRA4444:
        case DRV_TDE_COLOR_FMT_ARGB1555:
        case DRV_TDE_COLOR_FMT_ABGR1555:
        case DRV_TDE_COLOR_FMT_RGBA1555:
        case DRV_TDE_COLOR_FMT_BGRA1555:
        case DRV_TDE_COLOR_FMT_ACLUT88:
        case DRV_TDE_COLOR_FMT_YCBCR422:
        case DRV_TDE_COLOR_FMT_HALFWORD:
        case DRV_TDE_COLOR_FMT_PKGVYUY:
            return 16; /* 16 bpp fmt */
        default:
            return -1;
    }
}

static td_s32 tde_osi_get_bpp_by_fmt_24(drv_tde_color_fmt fmt)
{
    switch (fmt) {
        case DRV_TDE_COLOR_FMT_RGB888:
        case DRV_TDE_COLOR_FMT_BGR888:
        case DRV_TDE_COLOR_FMT_ARGB8565:
        case DRV_TDE_COLOR_FMT_ABGR8565:
        case DRV_TDE_COLOR_FMT_RGBA8565:
        case DRV_TDE_COLOR_FMT_BGRA8565:
        case DRV_TDE_COLOR_FMT_YCBCR888:
            return 24; /* 24 bpp fmt */
        default:
            return -1;
    }
}

static td_s32 tde_osi_get_bpp_by_fmt_32(drv_tde_color_fmt fmt)
{
    switch (fmt) {
        case DRV_TDE_COLOR_FMT_ARGB8888:
        case DRV_TDE_COLOR_FMT_ABGR8888:
        case DRV_TDE_COLOR_FMT_RGBA8888:
        case DRV_TDE_COLOR_FMT_BGRA8888:
        case DRV_TDE_COLOR_FMT_AYCBCR8888:
        case DRV_TDE_COLOR_FMT_RABG8888:
            return 32; /* 32 bpp fmt */
        default:
            return -1;
    }
}

static td_s32 tde_osi_get_bpp_by_fmt_8(drv_tde_color_fmt fmt)
{
    switch (fmt) {
        case DRV_TDE_COLOR_FMT_CLUT8:
        case DRV_TDE_COLOR_FMT_ACLUT44:
        case DRV_TDE_COLOR_FMT_A8:
        case DRV_TDE_COLOR_FMT_BYTE:
            return 8; /* 8 bpp fmt */
        default:
            return -1;
    }
}

static td_s32 tde_osi_get_bpp_by_fmt_4(drv_tde_color_fmt fmt)
{
    switch (fmt) {
        case DRV_TDE_COLOR_FMT_CLUT4:
            return 4; /* 4 bpp fmt */
        default:
            return -1;
    }
}

static td_s32 tde_osi_get_bpp_by_fmt_2(drv_tde_color_fmt fmt)
{
    switch (fmt) {
        case DRV_TDE_COLOR_FMT_CLUT2:
            return 2; /* 2 bpp fmt */
        default:
            return -1;
    }
}

static td_s32 tde_osi_get_bpp_by_fmt_1(drv_tde_color_fmt fmt)
{
    switch (fmt) {
        case DRV_TDE_COLOR_FMT_CLUT1:
        case DRV_TDE_COLOR_FMT_A1:
            return 1;
        default:
            return -1;
    }
}

/*
 * Function:      tde_osi_check_src
 * Description:   get scanning direction, avoid lap
 * Input:         pSrc source bitmap
                  dst_surface target bitmap
                  mirror mirror type
 * Output:        pstSrcDirection source scanning information
                  pstDstDirection target scanning information
 * Return:        0  success
                  -1 fail
 * Others:        add  antiscan handle to YCbCr422R
 */
static td_s32 tde_osi_check_src(const drv_tde_single_src *single_src, const tde_scandirection_mode *src_direction,
                                const tde_scandirection_mode *dst_direction)
{
    td_s32 bpp;

    if (single_src->src_surface == TD_NULL) {
        tde_error("single_src->src_surface is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    if (single_src->src_rect == TD_NULL) {
        tde_error("single_src->src_rect is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    if (single_src->dst_surface == TD_NULL) {
        tde_error("single_src->dst_surface is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    if (single_src->dst_rect == TD_NULL) {
        tde_error("single_src->dst_rect is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    bpp = tde_osi_get_bpp_by_fmt(single_src->dst_surface->color_format);
    if (bpp < 0) {
        tde_error("Unknown color format!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (bpp < 8) { /* 8 is bpp */
        /* when writing, 8 subbyte format align ask start point byte align */
        if ((single_src->dst_rect->pos_x * bpp % 8) || (single_src->dst_rect->width * bpp % 8)) {
            tde_error("The input start or write width does not meet the 8 single byte alignment!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
    return TD_SUCCESS;
}

static td_void tde_check_reverse_scan(td_phys_addr_t src_addr, td_phys_addr_t dst_addr,
    tde_scandirection_mode *src_direction, tde_scandirection_mode *dst_direction)
{
    /* There is no overlap between target and source. */
    if ((src_addr <= dst_addr)) {
        src_direction->ver_scan = TDE_SCAN_DOWN_UP;
        dst_direction->ver_scan = TDE_SCAN_DOWN_UP;

        src_direction->hor_scan = TDE_SCAN_RIGHT_LEFT;
        dst_direction->hor_scan = TDE_SCAN_RIGHT_LEFT;
    }
    return;
}

static td_void tde_osi_get_src_direction(drv_tde_mirror_mode mirror, tde_scandirection_mode *src_direction)
{
    switch (mirror) {
        case DRV_TDE_MIRROR_HORIZONTAL:
            src_direction->hor_scan = !(src_direction->hor_scan);
            break;
        case DRV_TDE_MIRROR_VERTICAL:
            src_direction->ver_scan = !(src_direction->ver_scan);
            break;
        case DRV_TDE_MIRROR_BOTH:
            src_direction->hor_scan = !(src_direction->hor_scan);
            src_direction->ver_scan = !(src_direction->ver_scan);
            break;
        default:
            break;
    }
}

static td_s32 tde_osi_get_scan_info_ex(const drv_tde_single_src *single_src, const drv_tde_opt *opt,
    tde_scandirection_mode *src_direction, tde_scandirection_mode *dst_direction)
{
    td_phys_addr_t src_addr;
    td_phys_addr_t dst_addr;
    drv_tde_mirror_mode mirror = DRV_TDE_MIRROR_NONE;
    td_s32 srcd_bpp;
    td_s32 dst_bpp;
    td_s32 ret;

    ret = tde_osi_check_src(single_src, src_direction, dst_direction);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* default scanning direction */
    src_direction->hor_scan = TDE_SCAN_LEFT_RIGHT;
    src_direction->ver_scan = TDE_SCAN_UP_DOWN;
    dst_direction->hor_scan = TDE_SCAN_LEFT_RIGHT;
    dst_direction->ver_scan = TDE_SCAN_UP_DOWN;

    if (opt != TD_NULL) {
        mirror = opt->mirror;
    }

    if (mirror != DRV_TDE_MIRROR_NONE) {
        tde_osi_get_src_direction(mirror, src_direction);
    } else {
        if ((opt != TD_NULL) && (opt->clip_mode == DRV_TDE_CLIP_MODE_OUTSIDE)) {
            return 0;
        }

        /* only if stride is the same, can be do conversion */
        if ((single_src->src_surface->stride != single_src->dst_surface->stride) ||
            (single_src->src_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP)) {
            return 0;
        }
        srcd_bpp = tde_osi_get_bpp_by_fmt(single_src->src_surface->color_format);
        dst_bpp = tde_osi_get_bpp_by_fmt(single_src->dst_surface->color_format);
        if ((dst_bpp < 0) || (srcd_bpp < 0)) {
            tde_error("bits per pixel less than 0!src fmt:%d,dst fmt:%d\n",
                single_src->src_surface->color_format, single_src->dst_surface->color_format);
            return -1;
        }

        src_addr = single_src->src_surface->phys_addr + single_src->src_rect->pos_y *
            single_src->src_surface->stride + ((single_src->src_rect->pos_x * srcd_bpp) / 8); /* 8 bits */
        dst_addr = single_src->dst_surface->phys_addr + single_src->dst_rect->pos_y *
            /* 8 bits */
            single_src->dst_surface->stride  + ((single_src->dst_rect->pos_x * dst_bpp) / 8);

        /* clip和反向不能同时打开，效果会异常 */
        if ((opt != TD_NULL) && (opt->clip_mode != DRV_TDE_CLIP_MODE_NONE) && (opt->resize)) {
            return 0;
        }

        /* source is above of target or on the left of the same direction */
        tde_check_reverse_scan(src_addr, dst_addr, src_direction, dst_direction);
    }
    return 0;
}

/*
 * Function:      tde_osi_get_inter_rect
 * Description:   get inter rect of two rectangles
 * Output:        inter_rect output inter rectangle
 * Return:        0  have inter zone
                  -1 no inter zone
 */
static td_s32 tde_osi_get_inter_rect(const drv_tde_rect *dst_rect, const drv_tde_rect *clip_rect,
                                     drv_tde_rect *inter_rect)
{
    td_s32 left;
    td_s32 top;
    td_s32 right;
    td_s32 bottom;
    td_s32 right1;
    td_s32 bottom1;
    td_s32 right2;
    td_s32 bottom2;
    td_s32 pos_x;
    td_s32 pos_y;

    pos_x = dst_rect->pos_x;
    pos_y = dst_rect->pos_y;
    left = (pos_x > clip_rect->pos_x) ? pos_x : clip_rect->pos_x;
    top = (pos_y > clip_rect->pos_y) ? pos_y : clip_rect->pos_y;

    right1 = pos_x + dst_rect->width - 1;
    right2 = clip_rect->pos_x + clip_rect->width - 1;
    right = (right1 > right2) ? right2 : right1;

    bottom1 = pos_y + dst_rect->height - 1;
    bottom2 = clip_rect->pos_y + clip_rect->height - 1;


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART04_H_ */
