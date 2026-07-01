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

#include "gfbg_comm.h"
#define GFBG_OSD_MODE_ARGB8888                 0
#define GFBG_OSD_MODE_RGB888                   1
#define GFBG_OSD_MODE_ARGB1555                 2
#define GFBG_OSD_MODE_ARGB4444                 3
#define GFBG_COMP_RATIO_1000                   1000
#define GFBG_COMP_RATIO_2000                   2000
#define GFBG_ALIGN_16                          16
#define GFBG_ALIGN_32                          32
#define GFBG_ALIGN_128                         128
#define GFBG_ALIGN_MASK_31                     31
#define GFBG_ALIGN_MASK_127                    127
#define GFBG_BIT_DEPTH_1                       1
#define GFBG_BIT_DEPTH_4                       4
#define GFBG_BIT_DEPTH_5                       5
#define GFBG_BIT_DEPTH_6                       6
#define GFBG_BIT_DEPTH_8                       8
#define GFBG_RGB_CHANNEL_NUM                   3
#define GFBG_EXP_WIDTH_LIMIT                   720
#define GFBG_EXP_NUM_NONE                      0
#define GFBG_EXP_NUM_LOW                       2
#define GFBG_EXP_NUM_HIGH                      14
#define GFBG_BUFFER_INIT_MB_LIMIT_LOW          9
#define GFBG_BUFFER_INIT_MB_LIMIT_HIGH         18
#define GFBG_BUFFER_INIT_BITS_7946             7946
#define GFBG_BUFFER_INIT_BITS_7000             7000
#define GFBG_BUFFER_INIT_BITS_8082             8082
#define GFBG_BUFFER_BUDGET_BITS                9216
#define GFBG_BUDGET_MIN_BITS                   64
#define GFBG_CMP_STRIDE_ROUND                  126
#define GFBG_LOSSLESS_RATIO_NUM                13
#define GFBG_LOSSLESS_ROUND                    9
#define GFBG_LOSSLESS_RATIO_DEN                10

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
static td_u32 gfbg_node_max(td_u32 a, td_u32 b)
{
    return (a > b) ? a : b;
}

static td_u32 gfbg_node_min(td_u32 a, td_u32 b)
{
    return (a > b) ? b : a;
}

typedef struct {
    td_u32 bit_dep_a;
    td_u32 bit_dep_rgb;
    td_u32 comp_ratio;
    td_u32 last_mb_extra_bits;
} gfbg_stride_calc;

static td_bool gfbg_v2_get_mode(const gfbg_stride_attr *attr, td_u32 *osd_mode, td_u32 *comp_ratio)
{
    if ((attr->format != OT_FB_FORMAT_RGB888) && (attr->format != OT_FB_FORMAT_ARGB8888) &&
        (attr->format != OT_FB_FORMAT_ARGB1555) && (attr->format != OT_FB_FORMAT_ARGB4444)) {
        return TD_FALSE;
    }

    if (attr->format == OT_FB_FORMAT_ARGB8888) {
        *osd_mode = GFBG_OSD_MODE_RGB888;
        *comp_ratio = GFBG_COMP_RATIO_2000;
    } else if (attr->format == OT_FB_FORMAT_RGB888) {
        *osd_mode = GFBG_OSD_MODE_ARGB8888;
        *comp_ratio = GFBG_COMP_RATIO_1000;
    } else if (attr->format == OT_FB_FORMAT_ARGB1555) {
        *osd_mode = GFBG_OSD_MODE_ARGB1555;
        *comp_ratio = GFBG_COMP_RATIO_1000;
    } else {
        *osd_mode = GFBG_OSD_MODE_ARGB4444;
        *comp_ratio = GFBG_COMP_RATIO_1000;
    }
    return TD_TRUE;
}

static void gfbg_v2_calc_stride_data(td_u32 width, td_u32 osd_mode, td_u32 comp_ratio, gfbg_stride_calc *calc)
{
    td_u32 mb_num_x;
    td_u32 buffer_init_bits;
    td_u32 delta;
    td_u32 mb_ori_bits;
    td_u32 budget_mb_bits;
    td_u32 budget_mb_bits_last;
    td_u32 last_mb_width = (width % GFBG_ALIGN_32) ? (width % GFBG_ALIGN_32) : GFBG_ALIGN_32;

    calc->bit_dep_a = (osd_mode <= GFBG_OSD_MODE_RGB888) ? GFBG_BIT_DEPTH_8 :
        ((osd_mode == GFBG_OSD_MODE_ARGB1555) ? GFBG_BIT_DEPTH_1 : GFBG_BIT_DEPTH_4);
    calc->bit_dep_rgb = (osd_mode <= GFBG_OSD_MODE_RGB888) ? GFBG_BIT_DEPTH_8 :
        ((osd_mode == GFBG_OSD_MODE_ARGB1555) ? GFBG_BIT_DEPTH_5 : GFBG_BIT_DEPTH_4);
    calc->comp_ratio = comp_ratio;
    mb_num_x = (width + GFBG_ALIGN_32 - GFBG_BIT_DEPTH_1) / GFBG_ALIGN_32;
    buffer_init_bits = (osd_mode < GFBG_OSD_MODE_ARGB1555) ?
        ((mb_num_x < GFBG_BUFFER_INIT_MB_LIMIT_LOW) ? GFBG_BUFFER_INIT_BITS_7946 : GFBG_BUFFER_INIT_BITS_7000) :
        ((mb_num_x < GFBG_BUFFER_INIT_MB_LIMIT_HIGH) ? GFBG_BUFFER_INIT_BITS_8082 : GFBG_BUFFER_INIT_BITS_7000);
    delta = gfbg_node_max(GFBG_BIT_DEPTH_1,
        (GFBG_BUFFER_BUDGET_BITS - buffer_init_bits + mb_num_x - GFBG_BIT_DEPTH_1) / (mb_num_x));
    mb_ori_bits = GFBG_ALIGN_32 * (calc->bit_dep_rgb * GFBG_RGB_CHANNEL_NUM + calc->bit_dep_a);
    budget_mb_bits = mb_ori_bits * GFBG_COMP_RATIO_1000 / comp_ratio - delta;
    budget_mb_bits = gfbg_node_min(gfbg_node_max(budget_mb_bits, GFBG_BUDGET_MIN_BITS), mb_ori_bits);
    budget_mb_bits_last = (budget_mb_bits * last_mb_width) / GFBG_ALIGN_32;
    calc->last_mb_extra_bits = budget_mb_bits - budget_mb_bits_last;
}

td_void gfbg_recalculate_stride(td_u32 *cmp_stride, td_u32 *uncmp_stride, const gfbg_stride_attr *attr)
{
    td_u32 osd_mode;
    td_u32 comp_ratio;
    gfbg_stride_calc calc;

    if (attr == TD_NULL) {
        return;
    }

    if (gfbg_v2_get_mode(attr, &osd_mode, &comp_ratio) != TD_TRUE) {
        return;
    }

    gfbg_v2_calc_stride_data(attr->width, osd_mode, comp_ratio, &calc);

    if (cmp_stride != NULL) {
        *cmp_stride = (attr->width * (calc.bit_dep_a + calc.bit_dep_rgb * GFBG_RGB_CHANNEL_NUM) *
                       GFBG_COMP_RATIO_1000 / comp_ratio + calc.last_mb_extra_bits + GFBG_CMP_STRIDE_ROUND) /
                       GFBG_ALIGN_MASK_127 + GFBG_BIT_DEPTH_1;
        *cmp_stride = *cmp_stride * GFBG_ALIGN_16;
    }

    if (uncmp_stride != NULL) {
        *uncmp_stride = ((attr->width * (calc.bit_dep_a + calc.bit_dep_rgb * GFBG_RGB_CHANNEL_NUM) /
            GFBG_BIT_DEPTH_8) + GFBG_ALIGN_16 - GFBG_BIT_DEPTH_1) & (~(GFBG_ALIGN_16 - GFBG_BIT_DEPTH_1));
    }
    return;
}
#endif

#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
typedef struct {
    ot_fb_color_format format;
    td_u32 osd_mode;
    td_u32 comp_ratio;
} gfbg_stride_cfg;

static td_bool gfbg_get_stride_cfg(ot_fb_color_format format, gfbg_stride_cfg *cfg)
{
    static const gfbg_stride_cfg cfgs[] = {
        { OT_FB_FORMAT_ARGB8888, GFBG_OSD_MODE_ARGB8888, GFBG_COMP_RATIO_2000 },
        { OT_FB_FORMAT_RGBA8888, GFBG_OSD_MODE_ARGB8888, GFBG_COMP_RATIO_2000 },
        { OT_FB_FORMAT_ABGR8888, GFBG_OSD_MODE_ARGB8888, GFBG_COMP_RATIO_2000 },
        { OT_FB_FORMAT_RGB888, GFBG_OSD_MODE_RGB888, GFBG_COMP_RATIO_1000 },
        { OT_FB_FORMAT_BGR888, GFBG_OSD_MODE_RGB888, GFBG_COMP_RATIO_1000 },
        { OT_FB_FORMAT_ARGB1555, GFBG_OSD_MODE_ARGB1555, GFBG_COMP_RATIO_1000 },
        { OT_FB_FORMAT_ABGR1555, GFBG_OSD_MODE_ARGB1555, GFBG_COMP_RATIO_1000 },
        { OT_FB_FORMAT_ARGB4444, GFBG_OSD_MODE_ARGB4444, GFBG_COMP_RATIO_1000 },
    };
    td_u32 i;

    for (i = 0; i < sizeof(cfgs) / sizeof(cfgs[0]); i++) {
        if (cfgs[i].format == format) {
            *cfg = cfgs[i];
            return TD_TRUE;
        }
    }
    return TD_FALSE;
}

static td_u32 gfbg_get_exp_num(td_u32 osd_mode, td_u32 width)
{
    if ((osd_mode == GFBG_OSD_MODE_ARGB8888) || (osd_mode == GFBG_OSD_MODE_RGB888)) {
        return (width <= GFBG_EXP_WIDTH_LIMIT) ? GFBG_EXP_NUM_LOW : GFBG_EXP_NUM_HIGH;
    }
    return (width <= GFBG_EXP_WIDTH_LIMIT) ? GFBG_EXP_NUM_LOW : GFBG_EXP_NUM_NONE;
}

td_void gfbg_recalculate_stride(td_u32 *cmp_stride, td_u32 *uncmp_stride, const gfbg_stride_attr *attr)
{
    td_u32 exp_num;
    td_u32 extend_width;
    td_u32 bit_dep_a;
    td_u32 bit_dep_rgb;
    gfbg_stride_cfg cfg;

    if (gfbg_get_stride_cfg(attr->format, &cfg) != TD_TRUE) {
        return;
    }

    extend_width  = (attr->width + GFBG_ALIGN_MASK_31) / GFBG_ALIGN_32 * GFBG_ALIGN_32;
    bit_dep_a = (cfg.osd_mode == GFBG_OSD_MODE_ARGB8888) ? GFBG_BIT_DEPTH_8 :
        ((cfg.osd_mode == GFBG_OSD_MODE_RGB888) ? GFBG_BIT_DEPTH_6 :
        ((cfg.osd_mode == GFBG_OSD_MODE_ARGB1555) ? GFBG_BIT_DEPTH_1 : GFBG_BIT_DEPTH_4));
    bit_dep_rgb = (cfg.osd_mode == GFBG_OSD_MODE_ARGB1555) ? GFBG_BIT_DEPTH_5 :
        ((cfg.osd_mode == GFBG_OSD_MODE_ARGB4444) ? GFBG_BIT_DEPTH_4 : GFBG_BIT_DEPTH_8);
    exp_num = gfbg_get_exp_num(cfg.osd_mode, attr->width);

    if (cmp_stride != NULL) {
        *cmp_stride = (extend_width * (bit_dep_a + bit_dep_rgb * GFBG_RGB_CHANNEL_NUM) * GFBG_COMP_RATIO_1000 /
            cfg.comp_ratio + GFBG_ALIGN_MASK_127) / GFBG_ALIGN_128 + exp_num;

        if ((attr->is_lossless != TD_FALSE) || (attr->is_losslessa != TD_FALSE)) {
            *cmp_stride = (((attr->width * (bit_dep_a + bit_dep_rgb * GFBG_RGB_CHANNEL_NUM)) *
                GFBG_LOSSLESS_RATIO_NUM + GFBG_LOSSLESS_ROUND) / GFBG_LOSSLESS_RATIO_DEN) / GFBG_ALIGN_128;
        }
        *cmp_stride = *cmp_stride * GFBG_ALIGN_16;
    }

    if (uncmp_stride != NULL) {
        *uncmp_stride = ((attr->width * (bit_dep_a + bit_dep_rgb * GFBG_RGB_CHANNEL_NUM) / GFBG_BIT_DEPTH_8) +
            GFBG_ALIGN_16 - GFBG_BIT_DEPTH_1) & (~(GFBG_ALIGN_16 - GFBG_BIT_DEPTH_1));
    }
    return;
}
#endif
