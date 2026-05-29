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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART01_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART01_H_


#line 1 "tde_osictl_k.c"
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

#include "tde_osictl_k.h"

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
#include "tde_proc.h"
#endif

#include "ot_math.h"
#include "securec.h"
#include "wmalloc.h"
#include "tde_handle.h"
#include "tde_osictl.h"
#include "tde_osilist.h"
#include "tde_hal.h"
#include "tde_adp.h"


/* TDE osi ctl macro definition */
#define TDE_CLUT_SIZE (256 * 4)

#define tde_min(a, b) (((a) > (b)) ? (b) : (a))

#define drv_tde_mb_src_val(mb_src, theight, twidth)        \
    do {                                                   \
        theight = (mb_src)->src_rect->height;              \
        twidth = (mb_src)->src_rect->width;                \
    } while (0)

#define drv_tde_mb_src_addr(mb_src, tu64Yphy_addr, tu64CbCrphy_addr, tu64phy)    \
        do {                                                                     \
            tu64Yphy_addr = (mb_src)->mb_surface->y_addr;                        \
            tu64CbCrphy_addr = (mb_src)->mb_surface->cbcr_phys_addr;              \
            tu64phy = (mb_src)->dst_surface->phys_addr;                           \
        } while (0)

#define tde_get_yc422r_fillvalue(value) (((value)&(0xffffff)) | ((((value) >> (8)) & (0xff)) << (24)))

#define DRD_RIGHT_LIMIT 16383
#define DRD_LEFT_LIMIT (-16383)
#define DRD_MAX_LINE_WIDTH 256

static td_bool tde_osi_check_format_support_draw(drv_tde_color_fmt format)
{
#if defined CONFIG_TDE_DMA_CORNER_V1
    if ((format != DRV_TDE_COLOR_FMT_CLUT2) && (format != DRV_TDE_COLOR_FMT_CLUT4)) {
        tde_error("This operation only support clut2 or clut4!\n");
        return TD_FALSE;
    }
#elif defined CONFIG_TDE_DMA_CORNER_V2
    if (format >= DRV_TDE_COLOR_FMT_YCBCR422) {
        tde_error("This color format %d not support!should be in [%d %d]\n", format, 0, DRV_TDE_COLOR_FMT_AYCBCR8888);
        return TD_FALSE;
    }
#endif
    return TD_TRUE;
}

static td_s32 tde_check_subbyte_startx(td_s32 startx, td_u32 w, drv_tde_color_fmt format)
{
    td_s32 bpp;
    bpp = tde_osi_get_bpp_by_fmt(format);
    if (bpp < 0) {
        tde_error("Unknown color format!\n");
        return TD_FAILURE;
    }
    if (bpp < 8) { /* 8 bits */
        /* when writing, subbyte format align ask start point byte align */
        if (((startx) * bpp % 8) || ((w) * bpp % 8)) { /* 8 bits */
            tde_error("The input start or write width does not meet the single byte alignment!\n");
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

#define ycc2rgb(y, cb, cr, r, g, b)                                                            \
    do {                                                                                       \
        r = (((298 * ((y) - 16) + 409 * ((cr) - 128)) >> 7) + 1) >> 1;                         \
        g = (((298 * ((y) - 16) - 100 * ((cb) - 128) - 208 * ((cr) - 128)) >> 7) + 1) >> 1;    \
        b = (((298 * ((y) - 16) + 517 * ((cb) - 128)) >> 7) + 1) >> 1;                         \
    } while (0)

#define rgb2ycc(r, g, b, y, cb, cr)                                          \
    do {                                                                     \
        y  = ((((263 * (r) + 516 * (g) + 100 * (b)) >> 9) + 1) >> 1) + 16;   \
        cb = ((((-152 * (r) - 298 * (g) + 450 * (b)) >> 9) + 1) >> 1) + 128; \
        cr = ((((450 * (r) - 377 * (g) - 73 * (b)) >> 9) + 1) >> 1) + 128;   \
    } while (0)

#define TDE_FOUR_BITS_SHIFT  4
#define TDE_THREE_BITS_SHIFT 3
#define TDE_TWO_BITS_SHIFT   2
#define TDE_ONE_BIT_SHIFT    1
#define TDE_ZERO_BIT_SHIFT   0
#define TDE_MAX_PATTERNWIDTH 256

/* TDE osi ctl struct definition */
#if (TDE_CAPABILITY & SLICE)
typedef struct {
    td_u32 des_width;
    td_u32 des_hoffset_pix;
    td_u32 des_h_scan_ord;
    td_u32 des_crop_en;
    td_u32 des_crop_start_x;
    td_u32 des_crop_end_x;

    td_u32 xdpos;
    td_u32 h_scan_ord;
    td_u32 width;

    td_u32 hlmsc_en;
    td_u32 hchmsc_en;
    td_u32 hratio;
    td_s32 hor_loffset;
    td_s32 hor_coffset;
    td_u32 zme_ow;
    td_u32 zme_iw;
    td_u32 hpzme_en;
    td_u32 hpzme_mode;
    td_u32 hpzme_width;

    td_u32 des_xst_pos_blk;
    td_u32 des_xed_pos_blk;
    td_u32 xed_pos_blk;
    td_u32 xst_pos_cord;
    td_u32 xed_pos_cord;
    td_u32 xst_pos_cord_in;
    td_u32 xed_pos_cord_in;
    td_u32 hor_loffset_cfg_int_comp;
    td_s32 hor_loffset_cfg_int;
    td_s32 xst_pos_cord_in_offset;
    td_s32 xed_pos_cord_in_offset;
    td_u32 xst_pos_cord_in_tap_rgb;
    td_u32 xed_pos_cord_in_tap_rgb;
    td_u32 node_cfg_zme_iw_rgb;
    td_u32 hor_loffset_cfg_fraction;
    td_u32 hor_loffset_pix_fraction;
    td_u32 hor_loffset_fraction;
    td_s32 hor_loffset_int;
    td_s32 node_cfg_hor_loffset_rgb;
    td_u32 xst_pos_cord_in_tap_luma;
    td_u32 xed_pos_cord_in_tap_luma;
    td_u32 xst_pos_cord_in_chroma;
    td_u32 xed_pos_cord_in_chroma;
    td_u32 hor_coffset_cfg_int_comp;
    td_s32 hor_coffset_cfg_int;
    td_s32 xst_pos_cord_in_offset_chroma;
    td_s32 xed_pos_cord_in_offset_chroma;
    td_u32 xst_pos_cord_in_tap_chroma;
    td_u32 xed_pos_cord_in_tap_chroma;
    td_u32 xst_pos_cord_in_tap_chroma_x2;
    td_u32 xed_pos_cord_in_tap_chroma_x2;
    td_u32 xst_pos_cord_in_tap_sp;
    td_u32 xed_pos_cord_in_tap_sp;
    td_u32 node_cfg_zme_iw_sp;
    td_u32 hor_coffset_cfg_fraction;
    td_u32 hor_coffset_pix_fraction;
    td_u32 hor_coffset_fraction;
    td_s32 hor_loffset_int_sp;
    td_s32 hor_coffset_int_sp;
    td_u32 hor_loffset_int_sp_complent;
    td_u32 hor_coffset_int_sp_complent;
    td_s32 node_cfg_hor_loffset_sp;
    td_s32 node_cfg_hor_coffset_sp;
    td_u32 xst_pos_cord_in_tap;
    td_u32 xed_pos_cord_in_tap;

    td_u32 hor_loffset_int_complement;
    td_u32 node_num;

    td_u32 xst_pos_cord_in_tap_hpzme;
    td_u32 xed_pos_cord_in_tap_hpzme;
    td_u32 xst_pos_cord_in_tap_hpzme_hso;
    td_u32 xed_pos_cord_in_tap_hpzme_hso;
    td_u32 u32422v_pro;
    td_u32 hor_loffset_int_beyond;
    td_u32 hor_loffset_int_beyond_complent;

    td_u32 slice_width;
    td_u32 slice_wi;
    td_s32 slice_c_ofst;
    td_s32 slice_l_ofst;
    td_u32 slice_hoffset;
    td_u32 slice_wo;
    td_u32 slice_w_hpzme;
    td_u32 slice_dst_width;
    td_u32 slice_dst_hoffset;
    td_u32 fmt;
} tde_slice_data;
#endif

/* pixel format transform type */
typedef enum {
    TDE_COLORFMT_TRANSFORM_ARGB_ARGB = 0,
    TDE_COLORFMT_TRANSFORM_ARGB_YCBCR,
    TDE_COLORFMT_TRANSFORM_CLUT_ARGB,
    TDE_COLORFMT_TRANSFORM_CLUT_YCBCR,
    TDE_COLORFMT_TRANSFORM_CLUT_CLUT,
    TDE_COLORFMT_TRANSFORM_YCBCR_ARGB,
    TDE_COLORFMT_TRANSFORM_YCBCR_YCBCR,
    TDE_COLORFMT_TRANSFORM_AN_AN,
    TDE_COLORFMT_TRANSFORM_ARGB_AN,
    TDE_COLORFMT_TRANSFORM_YCBCR_AN,
    TDE_COLORFMT_TRANSFORM_BUTT,
} tde_colorfmt_transform;

/* CLUT table use */
typedef enum {
    TDE_CLUT_COLOREXPENDING = 0, /* color expend */
    TDE_CLUT_COLORCORRECT,       /* color correct */
    TDE_CLUT_CLUT_BYPASS,
    TDE_CLUT_USAGE_BUTT
} tde_clut_usage;

typedef enum {
    TDE_OPERATION_SINGLE_SRC1 = 0,
    TDE_OPERATION_SINGLE_SRC2,
    TDE_OPERATION_DOUBLE_SRC,
    TDE_OPERATION_BUTT
} tde_operation_category;

typedef enum {
    TDE_PATTERN_OPERATION_SINGLE_SRC = 0,
    TDE_PATTERN_OPERATION_DOUBLE_SRC,
    TDE_PATTERN_OPERATION_BUTT
} tde_pattern_operation_category;

typedef struct {
    td_u8 alpha_bits;
    td_u8 red_bits;
    td_u8 green_bits;
    td_u8 blue_bits;
    td_u8 alpha_offset;
    td_u8 red_offset;
    td_u8 green_offset;
    td_u8 blue_offset;
} tde2_fmt_bitoffset;

#if (TDE_CAPABILITY & RESIZE)
typedef struct {
    td_s32 ori_in_width;    /* original image width */
    td_s32 ori_in_height;   /* original image height */
    td_s32 zme_out_width;   /* output full image width */
    td_s32 zme_out_height;  /* output full image height */

    td_s32 update_instart_w;  /* the start_x of update area in original image */
    td_s32 update_instart_h;  /* the start_y of update area in original image */
    td_s32 update_in_width;   /* the width of update area in original image */
    td_s32 update_in_height;  /* the height of update area in original image */
} tde_update_config;

typedef struct {
    td_s32 zme_instart_w;  /* the start_x of needed readin area in original image */
    td_s32 zme_instart_h;  /* the start_y of needed readin area in original image */
    td_s32 zme_in_width;   /* the width of needed readin area in original image */
    td_s32 zme_in_height;  /* the height of needed readin area in original image */

    td_s32 zme_outstart_w;  /* the start_x of needed update area in output image */
    td_s32 zme_outstart_h;  /* the start_y of needed update area in output image */
    td_s32 zme_out_width;   /* the width of needed update area in output image */
    td_s32 zme_out_height;  /* the height of needed update area in output image */

    td_s32 zme_hphase;      /* the start phase of horizontal scale */
    td_s32 zme_vphase;      /* the start phase of vertical scale */
    td_s32 def_offsetup;    /* the up offset of deflicker */
    td_s32 def_offsetdown;  /* the down offset of deflicker */
} tde_update_info;
#endif

/* TDE osi ctl inner variables definition */
tde_color_fmt g_tde_common_drv_color_fmt[DRV_TDE_COLOR_FMT_MAX + 1] = {
    TDE_DRV_COLOR_FMT_RGB444, TDE_DRV_COLOR_FMT_RGB444,
    TDE_DRV_COLOR_FMT_RGB555, TDE_DRV_COLOR_FMT_RGB555,
    TDE_DRV_COLOR_FMT_RGB565, TDE_DRV_COLOR_FMT_RGB565,
    TDE_DRV_COLOR_FMT_RGB888, TDE_DRV_COLOR_FMT_RGB888,
    TDE_DRV_COLOR_FMT_ARGB4444, TDE_DRV_COLOR_FMT_ARGB4444, TDE_DRV_COLOR_FMT_ARGB4444, TDE_DRV_COLOR_FMT_ARGB4444,
    TDE_DRV_COLOR_FMT_ARGB1555, TDE_DRV_COLOR_FMT_ARGB1555, TDE_DRV_COLOR_FMT_ARGB1555, TDE_DRV_COLOR_FMT_ARGB1555,
    TDE_DRV_COLOR_FMT_ARGB8565, TDE_DRV_COLOR_FMT_ARGB8565, TDE_DRV_COLOR_FMT_ARGB8565, TDE_DRV_COLOR_FMT_ARGB8565,
    TDE_DRV_COLOR_FMT_ARGB8888, TDE_DRV_COLOR_FMT_ARGB8888, TDE_DRV_COLOR_FMT_ARGB8888, TDE_DRV_COLOR_FMT_ARGB8888,
    TDE_DRV_COLOR_FMT_ARGB8888,
    TDE_DRV_COLOR_FMT_CLUT1, TDE_DRV_COLOR_FMT_CLUT2, TDE_DRV_COLOR_FMT_CLUT4,
    TDE_DRV_COLOR_FMT_CLUT8,
    TDE_DRV_COLOR_FMT_ACLUT44, TDE_DRV_COLOR_FMT_ACLUT88,
    TDE_DRV_COLOR_FMT_A1, TDE_DRV_COLOR_FMT_A8,
    TDE_DRV_COLOR_FMT_YCBCR888, TDE_DRV_COLOR_FMT_AYCBCR8888, TDE_DRV_COLOR_FMT_YCBCR422, TDE_DRV_COLOR_FMT_PKGVYUY,
    TDE_DRV_COLOR_FMT_BYTE, TDE_DRV_COLOR_FMT_HALFWORD,
    TDE_DRV_COLOR_FMT_YCBCR400MBP,
    TDE_DRV_COLOR_FMT_YCBCR422MBH, TDE_DRV_COLOR_FMT_YCBCR422MBV,
    TDE_DRV_COLOR_FMT_YCBCR420MB, TDE_DRV_COLOR_FMT_YCBCR420MB, TDE_DRV_COLOR_FMT_YCBCR420MB,
    TDE_DRV_COLOR_FMT_YCBCR420MB,
    TDE_DRV_COLOR_FMT_YCBCR444MB, TDE_DRV_COLOR_FMT_MAX
};

static tde_argb_order_mode g_tde_argb_order[DRV_TDE_COLOR_FMT_MAX + 1] = {
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR, TDE_DRV_ORDER_RGBA, TDE_DRV_ORDER_BGRA,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR, TDE_DRV_ORDER_RGBA, TDE_DRV_ORDER_BGRA,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR, TDE_DRV_ORDER_RGBA, TDE_DRV_ORDER_BGRA,
    TDE_DRV_ORDER_ARGB, TDE_DRV_ORDER_ABGR, TDE_DRV_ORDER_RGBA, TDE_DRV_ORDER_BGRA,
    TDE_DRV_ORDER_RABG,
    TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX, TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX,
    TDE_DRV_ORDER_MAX
};

/* if local deflicker flag */
static td_bool g_region_deflicker = TD_FALSE;
static td_bool g_is_resize_filter = TD_TRUE;

/* TDE osi ctl inner interface definition */
static td_void tde_unify_rect(drv_tde_rect *src_rect, drv_tde_rect *dst_rect);
static tde_colorfmt_category tde_osi_get_fmt_category(drv_tde_color_fmt fmt);

static tde_colorfmt_transform tde_osi_get_fmt_trans_type(drv_tde_color_fmt src2_fmt, drv_tde_color_fmt dst_fmt);

static td_s32 tde_osi_set_clut_opt(const drv_tde_surface *clut_sur, const drv_tde_surface *out_sur,
                                   tde_clut_usage *clut_usage, td_bool clut_reload, tde_hw_node *hw_node);

static tde_clut_usage tde_osi_get_clut_usage(drv_tde_color_fmt src_fmt, drv_tde_color_fmt dst_fmt);

static td_s32 tde_osi_get_scan_info_ex(const drv_tde_single_src *single_src, const drv_tde_opt *opt,
                                       tde_scandirection_mode *src_direction,
                                       tde_scandirection_mode *dst_direction);


static td_s32 tde_osi_get_inter_rect(const drv_tde_rect *rect1, const drv_tde_rect *rect2, drv_tde_rect *inter_rect);

static td_s32 tde_osi_set_mb_para(td_s32 handle, const drv_tde_mb_src *mb_src, const drv_tde_mb_opt *mb_opt);

static td_s32 tde_osi_set_filter_node(td_s32 handle, tde_hw_node *node, drv_tde_double_src *double_src,
                                      drv_tde_deflicker_mode deflicker_mode, drv_tde_deflicker_mode enFliterMode);

static td_s32 tde_osi_1_source_fill(td_s32 handle, drv_tde_surface *dst_surface,
                                    drv_tde_rect *dst_rect, drv_tde_fill_color *fill_color,
                                    const drv_tde_opt *opt);

static td_s32 tde_osi_single_src_2_blit(td_s32 handle, const drv_tde_single_src *single_src,
                                        const drv_tde_opt *opt, td_bool mmz_for_src, td_bool mmz_for_dst);

static td_s32 tde_osi_2_source_fill(td_s32 handle, const drv_tde_single_src *single_src,
                                    const drv_tde_fill_color *fill_color, const drv_tde_opt *opt);

static td_s32 tde_osi_set_color_key(const drv_tde_double_src *double_src,
                                    tde_hw_node *hw_node, drv_tde_color_key colorkey_value,
                                    drv_tde_color_key_mode colorkey_mode,
                                    tde_clut_usage clut_usage);
static td_s32 tde_osi_set_blend(tde_hw_node *hw_node, drv_tde_alpha_blending alpha_blending_cmd,
    drv_tde_blend_opt blend_opt, tde_alu_mode *enAluMode, td_bool bCheckBlend);

#if (TDE_CAPABILITY & ROP)
static td_s32 tde_osi_set_rop(tde_hw_node *hw_node, const tde_rop_opt *rop_opt, tde_alu_mode *alu_mode);
#endif

#if (TDE_CAPABILITY & COLORIZE)
static td_s32 tde_osi_set_colorize(tde_hw_node *hw_node, drv_tde_alpha_blending alpha_blending_cmd,
    td_s32 color_resize);
#endif

static td_s32 tde_osi_check_double_src_pattern_fill_para(const drv_tde_double_src *double_src,
                                                         const drv_tde_pattern_fill_opt *opt);

static td_s32 tde_osi_check_single_src_pattern_fill_para(const drv_tde_single_src *single_src,
                                                         const drv_tde_pattern_fill_opt *opt);

static tde_pattern_operation_category tde_osi_check_single_src_pattern_operation(const drv_tde_double_src *double_src,
    const drv_tde_pattern_fill_opt *opt);

static tde_pattern_operation_category tde_osi_double_src_pattern_operation(const drv_tde_double_src *double_src,
                                                                           const drv_tde_pattern_fill_opt *opt);

static td_s32 tde_osi_single_src_1_blit(td_s32 handle, const drv_tde_single_src *single_src,
                                        td_bool mmz_for_src, td_bool mmz_for_dst);

static td_s32 tde_osi_set_foreground_color_key(tde_hw_node *hw_node, const drv_tde_surface *src_surface,
                                               const drv_tde_opt *opt, tde_clut_usage clut_usage);

static tde_operation_category tde_osi_single_src_operation(const drv_tde_double_src *double_src,
                                                           const drv_tde_opt *opt);

static tde_operation_category tde_osi_double_src_operation(drv_tde_double_src *double_src, const drv_tde_opt *opt);

static td_s32 tde_osi_check_surface(const drv_tde_surface *surface, drv_tde_rect *rect);

static tde_operation_category tde_osi_get_opt_category(drv_tde_double_src *double_src, const drv_tde_opt *opt);

static td_void tde_osi_convert_surface(const drv_tde_surface *sur, const drv_tde_rect *rect,
                                       const tde_scandirection_mode *scan_info,
                                       tde_surface_msg *drv_sur);

static td_s32 tde_osi_set_clip_para(drv_tde_double_src *double_src, const drv_tde_opt *opt, tde_hw_node *hw_node);

static td_s32 tde_osi_set_base_opt_para_for_blit(const drv_tde_opt *opt, const drv_tde_surface *src1,
                                                 const drv_tde_surface *src2, tde_operation_category opt_category,
                                                 tde_hw_node *hw_node);

static td_void tde_osi_adj_clip_para(tde_hw_node *hw_node);

static td_s32 tde_osi_set_node_finish(td_s32 handle, tde_hw_node *hw_node,
                                      td_u32 work_buf_num, tde_node_subm_type subm_type);


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART01_H_ */
