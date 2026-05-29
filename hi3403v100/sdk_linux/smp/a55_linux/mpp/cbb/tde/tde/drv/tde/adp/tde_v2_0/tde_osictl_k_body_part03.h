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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART03_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART03_H_

#if (TDE_CAPABILITY & ROP)
static td_bool tde_osi_is_single_src_to_rop(drv_tde_rop_mode rop)
{
    switch (rop) {
        case DRV_TDE_ROP_BLACK:
        case DRV_TDE_ROP_NOTCOPYPEN:
        case DRV_TDE_ROP_COPYPEN:
        case DRV_TDE_ROP_WHITE:
            return TD_TRUE;

        default:
            return TD_FALSE;
    }
}
#endif

/*
 * Function:      tde_osi_get_clut_usage
 * Description:   get CLUT table usage
 * Input:         src_fmt foreground pixel format
                  dst_fmt  target pixel format
 * Return:        TDE_CLUT_USAGE_E:  clut  usage
 */
static tde_clut_usage tde_osi_get_clut_usage(drv_tde_color_fmt src_fmt, drv_tde_color_fmt dst_fmt)
{
    tde_colorfmt_transform color_trans_type;

    color_trans_type = tde_osi_get_fmt_trans_type(src_fmt, dst_fmt);

    switch (color_trans_type) {
        case TDE_COLORFMT_TRANSFORM_CLUT_ARGB:
        case TDE_COLORFMT_TRANSFORM_CLUT_YCBCR:
            return TDE_CLUT_COLOREXPENDING; /* color expand */

        case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
        case TDE_COLORFMT_TRANSFORM_YCBCR_YCBCR:
            return TDE_CLUT_COLORCORRECT; /* color adjust */

        case TDE_COLORFMT_TRANSFORM_CLUT_CLUT:
            return TDE_CLUT_CLUT_BYPASS;

        default:
            return TDE_CLUT_USAGE_BUTT;
    }
}

static td_bool tde_osi_whether_contain_alpha(drv_tde_color_fmt color_format)
{
    switch (color_format) {
        case DRV_TDE_COLOR_FMT_ARGB4444:
        case DRV_TDE_COLOR_FMT_ABGR4444:
        case DRV_TDE_COLOR_FMT_RGBA4444:
        case DRV_TDE_COLOR_FMT_BGRA4444:
        case DRV_TDE_COLOR_FMT_ARGB1555:
        case DRV_TDE_COLOR_FMT_ABGR1555:
        case DRV_TDE_COLOR_FMT_RGBA1555:
        case DRV_TDE_COLOR_FMT_BGRA1555:
        case DRV_TDE_COLOR_FMT_ARGB8565:
        case DRV_TDE_COLOR_FMT_ABGR8565:
        case DRV_TDE_COLOR_FMT_RGBA8565:
        case DRV_TDE_COLOR_FMT_BGRA8565:
        case DRV_TDE_COLOR_FMT_ARGB8888:
        case DRV_TDE_COLOR_FMT_ABGR8888:
        case DRV_TDE_COLOR_FMT_RGBA8888:
        case DRV_TDE_COLOR_FMT_BGRA8888:
        case DRV_TDE_COLOR_FMT_AYCBCR8888:
        case DRV_TDE_COLOR_FMT_RABG8888:
            return TD_TRUE;
        default:
            return TD_FALSE;
    }
}

static td_s32 tde_osi_set_clut_opt(const drv_tde_surface *clut_sur, const drv_tde_surface *out_sur,
                                   tde_clut_usage *pen_clut_usage, td_bool clut_reload, tde_hw_node *hw_node)
{
    tde_colorfmt_category fmt_cate;
    tde_clut_cmd clut_cmd;
    td_phys_addr_t clut_phyaddr;

    if (!((clut_sur->clut_phys_addr != (td_phys_addr_t)0xffffffffffffffffUL) && (clut_sur->clut_phys_addr != 0))) {
        return TD_SUCCESS;
    }
    clut_phyaddr = clut_sur->clut_phys_addr;
    fmt_cate = tde_osi_get_fmt_category(out_sur->color_format);
    /* when user input the type of clut is not consistent with output format,return error */
    if ((!clut_sur->is_ycbcr_clut && (fmt_cate == TDE_COLORFMT_CATEGORY_YCBCR)) ||
        (clut_sur->is_ycbcr_clut && (fmt_cate == TDE_COLORFMT_CATEGORY_ARGB))) {
        tde_error("clut fmt not same\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    /*
     * Because of unsupported output CSC, input and background color zone is consistent.
     * In trine sources operation,clut need use background, so output color zone instand of background's
     */
    *pen_clut_usage = tde_osi_get_clut_usage(clut_sur->color_format, out_sur->color_format);

    if (*pen_clut_usage >= TDE_CLUT_CLUT_BYPASS) {
        return TD_SUCCESS;
    }

    if (*pen_clut_usage == TDE_CLUT_COLOREXPENDING) {
        clut_cmd.clut_mode = TDE_COLOR_EXP_CLUT_MODE;
    } else {
        clut_cmd.clut_mode = TDE_COLOR_CORRCT_CLUT_MODE;
    }

    if (osal_div_u64_rem(clut_phyaddr, 4)) { /* 4 alg data */
        tde_error("clut_phys_addr=0x%lx is not aligned!\n", (td_ulong)clut_sur->clut_phys_addr);
        return DRV_ERR_TDE_NOT_ALIGNED;
    }

    clut_cmd.phy_clut_addr = clut_sur->clut_phys_addr;

    if (tde_hal_node_set_clut_opt(hw_node, &clut_cmd, clut_reload) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return TD_SUCCESS;
}

static td_s32 tde_osi_check_single_opt(const drv_tde_opt *opt)
{
    td_bool real = (((opt->blend_opt.global_alpha_en != TD_TRUE) && (opt->blend_opt.global_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.pixel_alpha_en != TD_TRUE) && (opt->blend_opt.pixel_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.src1_alpha_premulti != TD_TRUE) && (opt->blend_opt.src1_alpha_premulti != TD_FALSE)) ||
        ((opt->blend_opt.src2_alpha_premulti != TD_TRUE) && (opt->blend_opt.src2_alpha_premulti != TD_FALSE)) ||
        ((opt->clut_reload != TD_TRUE) && (opt->clut_reload != TD_FALSE)) ||
        ((opt->resize != TD_TRUE) && (opt->resize != TD_FALSE)) ||
        ((opt->is_compress != TD_TRUE) && (opt->is_compress != TD_FALSE)) ||
        ((opt->is_decompress != TD_TRUE) && (opt->is_decompress != TD_FALSE)));

    /* return error, if enable color key */
    if (opt->colorkey_mode != DRV_TDE_COLOR_KEY_MODE_NONE) {
        tde_error("It doesn't support colorkey in single source mode!\n");
        return -1;
    }

    if (opt->alpha_blending_cmd >= DRV_TDE_ALPHA_BLENDING_MAX) {
        tde_error("alpha_blending_cmd error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (real) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->deflicker_mode >= DRV_TDE_DEFLICKER_LEVEL_MODE_MAX) ||
        (opt->deflicker_mode < DRV_TDE_DEFLICKER_LEVEL_MODE_NONE)) {
        tde_error("deflicker_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->mirror >= DRV_TDE_MIRROR_MAX) || (opt->mirror < DRV_TDE_MIRROR_NONE)) {
        tde_error("mirror error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->filter_mode >= DRV_TDE_FILTER_MODE_MAX) || (opt->filter_mode < DRV_TDE_FILTER_MODE_COLOR)) {
        tde_error("filter_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}
static td_s32 tde_osi_check_dst_fmt(drv_tde_color_fmt fmt)
{
    if (((fmt >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) && (fmt <= DRV_TDE_COLOR_FMT_JPG_YCBCR422MBVP)) ||
        (fmt >= DRV_TDE_COLOR_FMT_JPG_YCBCR444MBP) || ((fmt >= DRV_TDE_COLOR_FMT_CLUT1) &&
        (fmt <= DRV_TDE_COLOR_FMT_A8))) {
        tde_error("This fmt %d doesn't support!\n", fmt);
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}
/*
 * Function:      tde_osi_check_single_src_to_opt
 * Description:   check if valid of foreground single source operate
 * Input:         src2_fmt foreground pixel format
                  dst_fmt  target pixel format
                  opt     operate attribute pointer
 * Return:        0  valid parameter;
                  -1 invalid parameter;
 * Others:        none
 */
static td_s32 tde_osi_check_single_src_to_opt(drv_tde_color_fmt src2_fmt, drv_tde_color_fmt dst_fmt,
                                              const drv_tde_opt *opt)
{
    tde_colorfmt_transform color_trans_type;
    td_s32 ret = tde_osi_check_single_opt(opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

#if (TDE_CAPABILITY & ROP)
    /* if operate type is ROP and it is not single operate,return error */
    if ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_ROP) {
        if (((opt->rop_alpha >= DRV_TDE_ROP_MAX) || (opt->rop_alpha < DRV_TDE_ROP_BLACK)) ||
            ((opt->rop_color >= DRV_TDE_ROP_MAX) || (opt->rop_color < DRV_TDE_ROP_BLACK))) {
            return DRV_ERR_TDE_INVALID_PARA;
        }

        if ((!tde_osi_is_single_src_to_rop(opt->rop_alpha)) || (!tde_osi_is_single_src_to_rop(opt->rop_color))) {
            tde_error("Only support single s2 rop!\n");
            return -1;
        }
    }
#endif
    /* single source can not do blend operate */
    if ((td_u32)(opt->alpha_blending_cmd) & DRV_TDE_ALPHA_BLENDING_BLEND) {
        tde_error("Alu mode error!\n");
        return -1;
    }
    ret = tde_osi_check_dst_fmt(dst_fmt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    color_trans_type = tde_osi_get_fmt_trans_type(src2_fmt, dst_fmt);
    if (color_trans_type == TDE_COLORFMT_TRANSFORM_BUTT) {
        tde_error("Unknown color transport type!\n");
        return -1;
    }

    if (color_trans_type == TDE_COLORFMT_TRANSFORM_CLUT_CLUT) {
        /* unsupported deflicker,zoom, Rop, mirror,colorize */
        if ((opt->resize) || (opt->deflicker_mode != DRV_TDE_DEFLICKER_LEVEL_MODE_NONE) ||
            ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_ROP) ||
            ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_COLORIZE)) {
            tde_error("It doesn't support deflicker or ROP or mirror!\n");
            return -1;
        }
    }

    return 0;
}

static td_s32 tde_osi_check_bool_single_opt(const drv_tde_opt *opt)
{
    td_bool real = (((opt->blend_opt.global_alpha_en != TD_TRUE) && (opt->blend_opt.global_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.pixel_alpha_en != TD_TRUE) && (opt->blend_opt.pixel_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.src1_alpha_premulti != TD_TRUE) && (opt->blend_opt.src1_alpha_premulti != TD_FALSE)) ||
        ((opt->blend_opt.src2_alpha_premulti != TD_TRUE) && (opt->blend_opt.src2_alpha_premulti != TD_FALSE)) ||
        ((opt->clut_reload != TD_TRUE) && (opt->clut_reload != TD_FALSE)) ||
        ((opt->resize != TD_TRUE) && (opt->resize != TD_FALSE)) ||
        ((opt->is_compress != TD_TRUE) && (opt->is_compress != TD_FALSE)) ||
        ((opt->is_decompress != TD_TRUE) && (opt->is_decompress != TD_FALSE)));
    if (real) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->deflicker_mode >= DRV_TDE_DEFLICKER_LEVEL_MODE_MAX) ||
        (opt->deflicker_mode < DRV_TDE_DEFLICKER_LEVEL_MODE_NONE)) {
        tde_error("deflicker_mode (%d) error!should be in [%d,%d]\n", opt->deflicker_mode,
            DRV_TDE_DEFLICKER_LEVEL_MODE_NONE, DRV_TDE_DEFLICKER_LEVEL_MODE_BOTH);
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->mirror >= DRV_TDE_MIRROR_MAX) || (opt->mirror < DRV_TDE_MIRROR_NONE)) {
        tde_error("mirror (%d) error!should be in [%d,%d]\n", opt->mirror,
            DRV_TDE_MIRROR_NONE, DRV_TDE_MIRROR_BOTH);
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->filter_mode >= DRV_TDE_FILTER_MODE_MAX) || (opt->filter_mode < DRV_TDE_FILTER_MODE_COLOR)) {
        tde_error("filter_mode (%d) error!should be in [%d,%d]\n", opt->filter_mode,
            DRV_TDE_FILTER_MODE_COLOR, DRV_TDE_FILTER_MODE_NONE);
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_fmt_single_opt(drv_tde_color_fmt src1_fmt, const drv_tde_opt *opt)
{
    td_s32 ret;

    if ((src1_fmt == DRV_TDE_COLOR_FMT_YCBCR422) || (src1_fmt == DRV_TDE_COLOR_FMT_PKGVYUY)) {
        tde_error("This operation doesn't support PKG!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (opt->alpha_blending_cmd >= DRV_TDE_ALPHA_BLENDING_MAX) {
        tde_error("alpha_blending_cmd (%d) error!\n", opt->alpha_blending_cmd);
        return DRV_ERR_TDE_INVALID_PARA;
    }
    ret = tde_osi_check_bool_single_opt(opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_category(tde_colorfmt_category src1_category, tde_colorfmt_category src2_category,
    tde_colorfmt_category dst_category, const drv_tde_opt *opt, td_bool temp_fmt)
{
    td_bool is_invalid;

    is_invalid = (src1_category >= TDE_COLORFMT_CATEGORY_BYTE) || (src2_category >= TDE_COLORFMT_CATEGORY_BYTE) ||
        (dst_category >= TDE_COLORFMT_CATEGORY_BYTE);
    if (is_invalid == TD_TRUE) {
        tde_error("unknown format!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    is_invalid = ((src1_category == TDE_COLORFMT_CATEGORY_ARGB) || (src1_category == TDE_COLORFMT_CATEGORY_YCBCR)) &&
        (src2_category == TDE_COLORFMT_CATEGORY_AN) && (!temp_fmt);
    if (is_invalid == TD_TRUE) {
        tde_error("Target must have alpha component!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    is_invalid = (src1_category == TDE_COLORFMT_CATEGORY_CLUT) && ((src2_category != TDE_COLORFMT_CATEGORY_CLUT) ||
        (dst_category != TDE_COLORFMT_CATEGORY_CLUT));
    if (is_invalid == TD_TRUE) {
        tde_error("Unsupported operation!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    is_invalid = ((opt->deflicker_mode != DRV_TDE_DEFLICKER_LEVEL_MODE_NONE) || (opt->resize) ||
        (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE)) && (src1_category == TDE_COLORFMT_CATEGORY_CLUT);
    if (is_invalid == TD_TRUE) {
        tde_error("It doesn't support deflicker or ROP or mirror!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    is_invalid = (src1_category == TDE_COLORFMT_CATEGORY_AN) && (src2_category == TDE_COLORFMT_CATEGORY_AN) &&
        (dst_category == TDE_COLORFMT_CATEGORY_AN) && (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE);
    if (is_invalid == TD_TRUE) {
        tde_error("It doesn't support ROP or mirror!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_cmd_single_opt(const drv_tde_opt *opt)
{
#if (TDE_CAPABILITY & ROP)
    if ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_ROP) {
        if (((opt->rop_alpha >= DRV_TDE_ROP_MAX) || (opt->rop_alpha < DRV_TDE_ROP_BLACK)) ||
            ((opt->rop_color >= DRV_TDE_ROP_MAX) || (opt->rop_color < DRV_TDE_ROP_BLACK))) {
            tde_error("enRopCode error!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
#endif
    if ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_BLEND) {
        if ((opt->blend_opt.blend_cmd >= DRV_TDE_BLEND_CMD_MAX) ||
            (opt->blend_opt.blend_cmd < DRV_TDE_BLEND_CMD_NONE)) {
            tde_error("Unknown blend cmd!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }

        if (opt->blend_opt.blend_cmd == DRV_TDE_BLEND_CMD_CONFIG) {
            if ((opt->blend_opt.src1_blend_mode >= DRV_TDE_BLEND_MAX) ||
                (opt->blend_opt.src2_blend_mode >= DRV_TDE_BLEND_MAX)) {
                tde_error("Unknown blend mode!\n");
                return DRV_ERR_TDE_INVALID_PARA;
            }
        }
    }
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_check_double_src_opt
 * Description:   check if valid of dual source operate
 * Input:         src1_fmt background pixel format
                  src2_fmt foreground pixel format
                  dst_fmt  target pixel format
                  opt    operate attribute operate
 * Return:        0  valid parameter;
                  -1 invalid parameter;
 */
static td_s32 tde_osi_check_double_src_opt(drv_tde_color_fmt src1_fmt, drv_tde_color_fmt src2_fmt,
                                           drv_tde_color_fmt dst_fmt, const drv_tde_opt *opt)
{
    tde_colorfmt_category src1_category;
    tde_colorfmt_category src2_category;
    tde_colorfmt_category dst_category;
    td_bool temp_fmt;
    td_s32 ret = tde_osi_check_dst_fmt(dst_fmt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    ret = tde_osi_check_fmt_single_opt(src1_fmt, opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /* get background pixel format category */
    src1_category = tde_osi_get_fmt_category(src1_fmt);

    /* get foreground pixel format category  */
    src2_category = tde_osi_get_fmt_category(src2_fmt);

    /* get target pixel format category  */
    dst_category = tde_osi_get_fmt_category(dst_fmt);

    temp_fmt = tde_osi_whether_contain_alpha(dst_fmt);

    ret = tde_osi_check_category(src1_category, src2_category, dst_category, opt, temp_fmt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = tde_osi_check_cmd_single_opt(opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

static td_u16 tde_osi_get_code1(tde_colorfmt_transform color_trans_type)
{
    switch (color_trans_type) {
        case TDE_COLORFMT_TRANSFORM_ARGB_ARGB:
            return 0x0;
        case TDE_COLORFMT_TRANSFORM_ARGB_YCBCR:
            return 0x5;
        case TDE_COLORFMT_TRANSFORM_CLUT_ARGB:
            return 0x8;
        case TDE_COLORFMT_TRANSFORM_CLUT_YCBCR:
            return 0x8 | 0x10 | 0x1;
        case TDE_COLORFMT_TRANSFORM_YCBCR_ARGB:
            return 0x1;
        case TDE_COLORFMT_TRANSFORM_YCBCR_YCBCR:
            return 0x0;
        default:
            return 0x8000;
    }
}

static td_u16 tde_osi_get_code2(tde_colorfmt_transform color_trans_type, drv_tde_color_fmt src2_fmt)
{
#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART03_H_ */
