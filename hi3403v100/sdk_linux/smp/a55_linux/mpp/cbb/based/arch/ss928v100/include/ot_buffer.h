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

#ifndef OT_BUFFER_H
#define OT_BUFFER_H

#include "ot_math.h"
#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_video.h"
#include "securec.h"

#ifdef __KERNEL__
#include "ot_osal.h"
#endif

/* Constants for reported magic-number values. */
#define OT_BUF_ALIGN_100                       100
#define OT_BUF_ALIGN_1024                      1024
#define OT_BUF_ALIGN_128                       128
#define OT_BUF_ALIGN_16                        16
#define OT_BUF_ALIGN_2                         2
#define OT_BUF_ALIGN_2048                      2048
#define OT_BUF_ALIGN_256                       256
#define OT_BUF_ALIGN_32                        32
#define OT_BUF_ALIGN_512                       512
#define OT_BUF_ALIGN_64                        64
#define OT_BUF_ALIGN_7                         7
#define OT_BUF_ALIGN_8                         8
#define OT_BUF_ALIGN_8192                      8192
#define OT_BUF_DIVISOR_16                      16
#define OT_BUF_DIVISOR_2                       2
#define OT_BUF_DIVISOR_3                       3
#define OT_BUF_FACTOR_2                        2
#define OT_BUF_FACTOR_3                        3
#define OT_BUF_FACTOR_4                        4
#define OT_BUF_FACTOR_43                       43
#define OT_BUF_FACTOR_59                       59
#define OT_BUF_FACTOR_8                        8
#define OT_BUF_SHIFT_1                         1
#define OT_BUF_SHIFT_3                         3
#define OT_BUF_SHIFT_6                         6
#define OT_BUF_SIZE_LIMIT_2048                 2048
#define OT_BUF_SIZE_LIMIT_4096                 4096
#define OT_BUF_SIZE_LIMIT_6144                 6144
#define OT_BUF_SIZE_LIMIT_8192                 8192
#define OT_BUF_VALUE_10                        10
#define OT_BUF_VALUE_1024                      1024
#define OT_BUF_VALUE_12                        12
#define OT_BUF_VALUE_128                       128
#define OT_BUF_VALUE_127                       127
#define OT_BUF_VALUE_14                        14
#define OT_BUF_VALUE_15                        15
#define OT_BUF_VALUE_16                        16
#define OT_BUF_VALUE_20                        20
#define OT_BUF_VALUE_31                        31
#define OT_BUF_VALUE_256                       256
#define OT_BUF_VALUE_255                       255
#define OT_BUF_VALUE_32                        32
#define OT_BUF_VALUE_384                       384
#define OT_BUF_VALUE_512                       512
#define OT_BUF_VALUE_64                        64
#define OT_BUF_VALUE_70                        70
#define OT_BUF_VALUE_768                       768
#define OT_BUF_VALUE_1000                      1000
#define OT_BUF_RAW_LINE_COMPRESS_RATIO         1538
#define OT_BUF_RAW_FRAME_COMPRESS_RATIO        2000
#define OT_BUF_VALUE_8                         8

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(__GNUC__) || defined(__clang__)
#define OT_BUF_STATIC static __attribute__((unused))
#else
#define OT_BUF_STATIC static
#endif

#define OT_MAXI_NUM_LIMIT 30000

#define OT_VI_SEG_RATIO_8BIT_LUMA 1300
#define OT_VPSS_SEG_RATIO_8BIT_LUMA 1300
#define OT_SEG_RATIO_8BIT_LUMA    1600
#define OT_SEG_RATIO_8BIT_CHROMA  2000

#define OT_SEG_WIDTH_BIT_ALIGN 128

typedef struct {
    td_u32            width;
    td_u32            height;
    td_u32            align;
    ot_data_bit_width bit_width;
    ot_pixel_format   pixel_format;
    ot_compress_mode  compress_mode;
} ot_pic_buf_attr;

typedef struct {
    td_bool           share_buf_en;
    td_u32            frame_buf_ratio;
    ot_pic_buf_attr   pic_buf_attr;
} ot_venc_buf_attr;

OT_BUF_STATIC td_u32 ot_common_get_bit_width(ot_data_bit_width bit_width)
{
    td_u32 res_bit_width;

    switch (bit_width) {
        case OT_DATA_BIT_WIDTH_8: {
            res_bit_width = OT_BUF_VALUE_8;
            break;
        }
        case OT_DATA_BIT_WIDTH_10: {
            res_bit_width = OT_BUF_VALUE_10;
            break;
        }
        case OT_DATA_BIT_WIDTH_12: {
            res_bit_width = OT_BUF_VALUE_12;
            break;
        }
        case OT_DATA_BIT_WIDTH_14: {
            res_bit_width = OT_BUF_VALUE_14;
            break;
        }
        case OT_DATA_BIT_WIDTH_16: {
            res_bit_width = OT_BUF_VALUE_16;
            break;
        }
        default: {
            res_bit_width = 0;
            break;
        }
    }
    return res_bit_width;
}

OT_BUF_STATIC td_void ot_copy_vb_calc_cfg(ot_vb_calc_cfg *dst_cfg, ot_vb_calc_cfg *src_cfg)
{
    if (dst_cfg == TD_NULL || src_cfg == TD_NULL) {
        return;
    }

    dst_cfg->vb_size     = src_cfg->vb_size;
    dst_cfg->head_stride = src_cfg->head_stride;
    dst_cfg->head_size   = src_cfg->head_size;
    dst_cfg->head_y_size = src_cfg->head_y_size;
    dst_cfg->main_stride = src_cfg->main_stride;
    dst_cfg->main_size   = src_cfg->main_size;
    dst_cfg->main_y_size = src_cfg->main_y_size;
    return;
}

OT_BUF_STATIC td_bool ot_common_is_pixel_format_package_422(ot_pixel_format pixel_format)
{
    if ((pixel_format == OT_PIXEL_FORMAT_YUYV_PACKAGE_422) || (pixel_format == OT_PIXEL_FORMAT_YVYU_PACKAGE_422) ||
        (pixel_format == OT_PIXEL_FORMAT_UYVY_PACKAGE_422) || (pixel_format == OT_PIXEL_FORMAT_VYUY_PACKAGE_422) ||
        (pixel_format == OT_PIXEL_FORMAT_YYUV_PACKAGE_422) || (pixel_format == OT_PIXEL_FORMAT_YYVU_PACKAGE_422) ||
        (pixel_format == OT_PIXEL_FORMAT_UVYY_PACKAGE_422) || (pixel_format == OT_PIXEL_FORMAT_VUYY_PACKAGE_422) ||
        (pixel_format == OT_PIXEL_FORMAT_VY1UY0_PACKAGE_422)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

OT_BUF_STATIC td_void ot_common_get_compress_none_pic_buf_cfg(ot_pic_buf_attr *buf_attr,
    td_s32 bit_width, ot_vb_calc_cfg *calc_cfg)
{
    td_u32 align_height;
    ot_vb_calc_cfg cfg          = {0};

    if (buf_attr == TD_NULL || calc_cfg == TD_NULL) {
        return;
    }

    if ((buf_attr->width > OT_MAXI_NUM_LIMIT) || (buf_attr->height > OT_MAXI_NUM_LIMIT)) {
        (td_void)memset_s(calc_cfg, sizeof(*calc_cfg), 0, sizeof(*calc_cfg));
        return;
    }

    align_height = OT_ALIGN_UP(buf_attr->height, OT_BUF_ALIGN_2);
    cfg.main_stride = OT_ALIGN_UP((buf_attr->width * (td_u32)bit_width +
        OT_BUF_ALIGN_7) >> OT_BUF_FACTOR_3, buf_attr->align);
    cfg.main_y_size = cfg.main_stride * align_height;

    if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_420)) {
        cfg.main_size = ((cfg.main_stride * align_height) * OT_BUF_SHIFT_3) >> 1;
    } else if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_422)) {
        cfg.main_size = cfg.main_stride * align_height * OT_BUF_FACTOR_2;
    } else if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_400) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_S16C1)) {
        cfg.main_size = cfg.main_stride * align_height;
    } else if (ot_common_is_pixel_format_package_422(buf_attr->pixel_format) == TD_TRUE) {
        cfg.main_stride = OT_ALIGN_UP((buf_attr->width * (td_u32)bit_width +
            OT_BUF_ALIGN_7) >> OT_BUF_FACTOR_3, buf_attr->align) * OT_BUF_FACTOR_2;
        cfg.main_size = cfg.main_stride * align_height;
    } else {
        cfg.main_size = cfg.main_stride * align_height * OT_BUF_FACTOR_3;
    }

    cfg.vb_size = cfg.main_size;

    ot_copy_vb_calc_cfg(calc_cfg, &cfg);
    return;
}

OT_BUF_STATIC td_void ot_common_get_compact_pic_buf_cfg(ot_pic_buf_attr *buf_attr, ot_mod_id mod_id,
    td_u32 bit_width, ot_vb_calc_cfg *cfg)
{
    td_u32 c_size;
    td_u32 align_c_height;
    td_u32 align_height;
    td_u32 cmp_ratio_luma;
    const td_u32 cmp_ratio_chroma = OT_SEG_RATIO_8BIT_CHROMA;

    if (buf_attr == TD_NULL || cfg == TD_NULL) {
        return;
    }

    if (mod_id == OT_ID_VI) {
        cmp_ratio_luma = OT_VI_SEG_RATIO_8BIT_LUMA;
    } else if (mod_id == OT_ID_VPSS) {
        cmp_ratio_luma = OT_VPSS_SEG_RATIO_8BIT_LUMA;
    } else {
        cmp_ratio_luma = OT_SEG_RATIO_8BIT_LUMA;
    }

    if ((buf_attr->width > OT_MAXI_NUM_LIMIT) || (buf_attr->height > OT_MAXI_NUM_LIMIT)) {
        (td_void)memset_s(cfg, sizeof(*cfg), 0, sizeof(*cfg));
        return;
    }

    align_height     = OT_ALIGN_UP(buf_attr->height, OT_BUF_ALIGN_2);
    cfg->main_stride = OT_ALIGN_UP(buf_attr->width * bit_width, OT_SEG_WIDTH_BIT_ALIGN);

    if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_420)) {
        align_c_height = align_height / OT_BUF_DIVISOR_2;
    } else if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_422)) {
        align_c_height = align_height;
    } else if (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_400) {
        align_c_height = 0;
    } else {
        align_c_height = align_height * OT_BUF_FACTOR_2;
    }

#ifdef __KERNEL__
    cfg->main_y_size = osal_div64_u64(cfg->main_stride * align_height * OT_BUF_VALUE_1000, cmp_ratio_luma * bit_width);
    c_size = osal_div64_u64(cfg->main_stride * align_c_height * OT_BUF_VALUE_1000, cmp_ratio_chroma * bit_width);
#else
    cfg->main_y_size = cfg->main_stride * align_height * OT_BUF_VALUE_1000 / (cmp_ratio_luma * bit_width);
    c_size = cfg->main_stride * align_c_height * OT_BUF_VALUE_1000 / (cmp_ratio_chroma * bit_width);
#endif

    c_size = OT_ALIGN_UP(c_size, OT_DEFAULT_ALIGN);
    cfg->main_y_size = OT_ALIGN_UP(cfg->main_y_size, OT_DEFAULT_ALIGN);

    cfg->main_stride = cfg->main_stride >> OT_BUF_SHIFT_3;
    cfg->head_y_size = cfg->head_stride * align_height;
    cfg->head_size = cfg->head_stride * (align_height + align_c_height);
    cfg->main_size = cfg->main_y_size + c_size;

    return;
}

OT_BUF_STATIC td_void ot_common_get_compact_none_pic_buf_cfg(ot_pic_buf_attr *buf_attr,
    ot_vb_calc_cfg *cfg)
{
    td_u32 align_height;

    if (buf_attr == TD_NULL || cfg == TD_NULL) {
        return;
    }

    if ((buf_attr->width > OT_MAXI_NUM_LIMIT) || (buf_attr->height > OT_MAXI_NUM_LIMIT)) {
        (td_void)memset_s(cfg, sizeof(*cfg), 0, sizeof(*cfg));
        return;
    }

    align_height = OT_ALIGN_UP(buf_attr->height, OT_BUF_ALIGN_2);
    cfg->main_stride = OT_ALIGN_UP(buf_attr->width, buf_attr->align);
    cfg->head_y_size = cfg->head_stride * align_height;
    cfg->main_y_size = cfg->main_stride * align_height;

    if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_420)) {
        cfg->head_size = (cfg->head_stride * align_height * OT_BUF_SHIFT_3) >> 1;
        cfg->main_size = (cfg->main_stride * align_height * OT_BUF_SHIFT_3) >> 1;
    } else if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_422)) {
        cfg->head_size = cfg->head_stride * align_height * OT_BUF_FACTOR_2;
        cfg->main_size = cfg->main_stride * align_height * OT_BUF_FACTOR_2;
    } else if (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_400) {
        cfg->head_size = cfg->head_stride * align_height;
        cfg->main_size = cfg->main_stride * align_height;
    } else {
        cfg->head_size = cfg->head_stride * align_height * OT_BUF_FACTOR_3;
        cfg->main_size = cfg->main_stride * align_height * OT_BUF_FACTOR_3;
    }

    return;
}

OT_BUF_STATIC td_void ot_common_get_compress_pic_buf_cfg(ot_pic_buf_attr *buf_attr, ot_mod_id mod_id,
    td_u32 bit_width, ot_vb_calc_cfg *calc_cfg)
{
    ot_vb_calc_cfg cfg          = {0};

    if (buf_attr == TD_NULL || calc_cfg == TD_NULL) {
        return;
    }

    if ((buf_attr->width > OT_MAXI_NUM_LIMIT) || (buf_attr->height > OT_MAXI_NUM_LIMIT)) {
        (td_void)memset_s(calc_cfg, sizeof(*calc_cfg), 0, sizeof(*calc_cfg));
        return;
    }

    if (buf_attr->width <= OT_BUF_SIZE_LIMIT_4096) {
        cfg.head_stride = OT_BUF_VALUE_16;
    } else if (buf_attr->width <= OT_BUF_SIZE_LIMIT_8192) {
        cfg.head_stride = OT_BUF_VALUE_32;
    } else {
        cfg.head_stride = OT_BUF_VALUE_64;
    }

    if (buf_attr->compress_mode == OT_COMPRESS_MODE_SEG_COMPACT) {
        cfg.head_stride = OT_BUF_VALUE_32;
    }

    if (bit_width == OT_BUF_VALUE_8) {
        /* 256 compact width */
        if (buf_attr->compress_mode == OT_COMPRESS_MODE_SEG_COMPACT && buf_attr->width > OT_BUF_VALUE_256) {
            ot_common_get_compact_pic_buf_cfg(buf_attr, mod_id, bit_width, &cfg);
        } else {
            ot_common_get_compact_none_pic_buf_cfg(buf_attr, &cfg);
        }
    }

    cfg.head_size = OT_ALIGN_UP(cfg.head_size, buf_attr->align);

    cfg.vb_size = cfg.head_size + cfg.main_size;

    ot_copy_vb_calc_cfg(calc_cfg, &cfg);
    return;
}

OT_BUF_STATIC td_u32 ot_vi_get_raw_bit_width(ot_pixel_format pixel_format)
{
    td_u32 bit_width;

    switch (pixel_format) {
        case OT_PIXEL_FORMAT_RGB_BAYER_8BPP:
            bit_width = OT_BUF_VALUE_8; /* 8:single pixel width */
            break;
        case OT_PIXEL_FORMAT_RGB_BAYER_10BPP:
            bit_width = OT_BUF_VALUE_10; /* 10:single pixel width */
            break;
        case OT_PIXEL_FORMAT_RGB_BAYER_12BPP:
            bit_width = OT_BUF_VALUE_12; /* 12:single pixel width */
            break;
        case OT_PIXEL_FORMAT_RGB_BAYER_14BPP:
            bit_width = OT_BUF_VALUE_14; /* 14:single pixel width */
            break;
        case OT_PIXEL_FORMAT_RGB_BAYER_16BPP:
            bit_width = OT_BUF_VALUE_16; /* 16:single pixel width */
            break;
        default:
            bit_width = 0;
            break;
    }

    return bit_width;
}

OT_BUF_STATIC td_void ot_common_vi_get_raw_buf_cfg_with_compress_ratio(ot_pic_buf_attr *buf_attr,
                                                                       td_u32 compress_ratio,
                                                                       ot_vb_calc_cfg *calc_cfg)
{
    td_u32 bit_width = ot_vi_get_raw_bit_width(buf_attr->pixel_format);
    td_u32 width = buf_attr->width;
    td_u32 height = buf_attr->height;
    td_u32 size = 0;
    td_u32 stride = 0;
    td_u32 raw_compress_ratio = 0;

    if ((width > OT_MAXI_NUM_LIMIT) || (height > OT_MAXI_NUM_LIMIT)) {
        (td_void)memset_s(calc_cfg, sizeof(*calc_cfg), 0, sizeof(*calc_cfg));
        return;
    }

    /* align: 0 is automatic mode, alignment size following system. Non-0 for specified alignment size */
    if (buf_attr->align == 0) {
        buf_attr->align = OT_DEFAULT_ALIGN;
    } else if (buf_attr->align > OT_MAX_ALIGN) {
        buf_attr->align = OT_MAX_ALIGN;
    } else {
        buf_attr->align = (OT_ALIGN_UP(buf_attr->align, OT_DEFAULT_ALIGN));
    }

    if (buf_attr->compress_mode == OT_COMPRESS_MODE_LINE) {
        raw_compress_ratio = OT_BUF_RAW_LINE_COMPRESS_RATIO; /* 1538: raw_line_compress_ratio */
    } else if (buf_attr->compress_mode == OT_COMPRESS_MODE_FRAME) {
        /* 2000: raw_frame_compress_ratio */
        raw_compress_ratio = (compress_ratio == 0 ? OT_BUF_RAW_FRAME_COMPRESS_RATIO : compress_ratio);
    }

    if (buf_attr->compress_mode == OT_COMPRESS_MODE_NONE) {
        stride = OT_ALIGN_UP(OT_ALIGN_UP(width * bit_width, OT_BUF_ALIGN_8) / OT_BUF_ALIGN_8, buf_attr->align);
        size = stride * height;
    } else if (buf_attr->compress_mode == OT_COMPRESS_MODE_LINE) {
        td_u32 tmp;
        tmp = OT_ALIGN_UP((OT_BUF_ALIGN_16 + width * bit_width * OT_BUF_VALUE_1000 / raw_compress_ratio +
            OT_BUF_SIZE_LIMIT_8192 + OT_BUF_VALUE_127) / OT_BUF_VALUE_128, OT_BUF_FACTOR_2);
        stride = OT_ALIGN_UP(tmp * OT_BUF_ALIGN_16, buf_attr->align);
        size = stride * height;
    } else if (buf_attr->compress_mode == OT_COMPRESS_MODE_FRAME) {
        if ((OT_DIV_UP(width, OT_BUF_VALUE_32) * height) <= OT_BUF_SIZE_LIMIT_4096) {
            size = (height * width * bit_width * OT_BUF_DIVISOR_3 / OT_BUF_FACTOR_2 + (width +
                OT_BUF_VALUE_31) / OT_BUF_VALUE_32 * height + OT_BUF_VALUE_255) / OT_BUF_VALUE_256 *
                OT_BUF_FACTOR_2 * OT_BUF_VALUE_16;
        } else {
            size = height * width * bit_width * OT_BUF_VALUE_1000 / (raw_compress_ratio * OT_BUF_FACTOR_8);
        }
        size = OT_ALIGN_UP(size, buf_attr->align);
    }

    calc_cfg->vb_size     = size;
    calc_cfg->head_stride = 0;
    calc_cfg->head_size   = 0;
    calc_cfg->head_y_size = 0;
    calc_cfg->main_stride = stride;
    calc_cfg->main_size   = size;
    calc_cfg->main_y_size = size;
}

OT_BUF_STATIC td_void ot_common_vi_get_raw_buf_cfg(ot_pic_buf_attr *buf_attr, ot_vb_calc_cfg *calc_cfg)
{
    ot_common_vi_get_raw_buf_cfg_with_compress_ratio(buf_attr, 0, calc_cfg);
}

OT_BUF_STATIC td_void ot_common_get_pic_buf_cfg(ot_pic_buf_attr *buf_attr,
    ot_vb_calc_cfg *calc_cfg)
{
    td_u32 bit_width;

    if (buf_attr == TD_NULL || calc_cfg == TD_NULL) {
        return;
    }

    if ((buf_attr->width > OT_MAXI_NUM_LIMIT) || (buf_attr->height > OT_MAXI_NUM_LIMIT)) {
        (td_void)memset_s(calc_cfg, sizeof(*calc_cfg), 0, sizeof(*calc_cfg));
        return;
    }

    /* align: 0 is automatic mode, alignment size following system. Non-0 for specified alignment size */
    if (buf_attr->align == 0) {
        buf_attr->align = OT_DEFAULT_ALIGN;
    } else if (buf_attr->align > OT_MAX_ALIGN) {
        buf_attr->align = OT_MAX_ALIGN;
    } else {
        buf_attr->align = (OT_ALIGN_UP(buf_attr->align, OT_DEFAULT_ALIGN));
    }

    if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_8BPP) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_10BPP) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_12BPP) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_14BPP) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_16BPP)) {
        ot_common_vi_get_raw_buf_cfg(buf_attr, calc_cfg);
        return;
    }

    bit_width = ot_common_get_bit_width(buf_attr->bit_width);
    if (buf_attr->compress_mode == OT_COMPRESS_MODE_NONE) {
        ot_common_get_compress_none_pic_buf_cfg(buf_attr, bit_width, calc_cfg);
    } else {
        /* use vgs id to distinguish vi/vpss mod id. */
        ot_common_get_compress_pic_buf_cfg(buf_attr, OT_ID_VGS, bit_width, calc_cfg);
    }
}

OT_BUF_STATIC td_void ot_common_get_vi_compact_seg_buf_size(ot_pic_buf_attr *buf_attr,
    ot_vb_calc_cfg *calc_cfg)
{
    td_u32 bit_width;

    if (buf_attr == TD_NULL || calc_cfg == TD_NULL) {
        return;
    }

    if ((buf_attr->width > OT_MAXI_NUM_LIMIT) || (buf_attr->height > OT_MAXI_NUM_LIMIT)) {
        (td_void)memset_s(calc_cfg, sizeof(*calc_cfg), 0, sizeof(*calc_cfg));
        return;
    }

    /* align: 0 is automatic mode, alignment size following system. Non-0 for specified alignment size */
    if (buf_attr->align == 0) {
        buf_attr->align = OT_DEFAULT_ALIGN;
    } else if (buf_attr->align > OT_MAX_ALIGN) {
        buf_attr->align = OT_MAX_ALIGN;
    } else {
        buf_attr->align = (OT_ALIGN_UP(buf_attr->align, OT_DEFAULT_ALIGN));
    }

    if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_8BPP) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_10BPP) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_12BPP) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_14BPP) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_16BPP)) {
        ot_common_vi_get_raw_buf_cfg(buf_attr, calc_cfg);
        return;
    }

    bit_width = ot_common_get_bit_width(buf_attr->bit_width);
    if (buf_attr->compress_mode == OT_COMPRESS_MODE_NONE) {
        ot_common_get_compress_none_pic_buf_cfg(buf_attr, bit_width, calc_cfg);
    } else {
        ot_common_get_compress_pic_buf_cfg(buf_attr, OT_ID_VI, bit_width, calc_cfg);
    }
}

OT_BUF_STATIC td_void ot_common_get_vpss_compact_seg_buf_size(ot_pic_buf_attr *buf_attr,
    ot_vb_calc_cfg *calc_cfg)
{
    td_u32 bit_width;

    if (buf_attr == TD_NULL || calc_cfg == TD_NULL) {
        return;
    }

    if ((buf_attr->width > OT_MAXI_NUM_LIMIT) || (buf_attr->height > OT_MAXI_NUM_LIMIT)) {
        (td_void)memset_s(calc_cfg, sizeof(*calc_cfg), 0, sizeof(*calc_cfg));
        return;
    }

    /* align: 0 is automatic mode, alignment size following system. Non-0 for specified alignment size */
    if (buf_attr->align == 0) {
        buf_attr->align = OT_DEFAULT_ALIGN;
    } else if (buf_attr->align > OT_MAX_ALIGN) {
        buf_attr->align = OT_MAX_ALIGN;
    } else {
        buf_attr->align = (OT_ALIGN_UP(buf_attr->align, OT_DEFAULT_ALIGN));
    }

    bit_width = ot_common_get_bit_width(buf_attr->bit_width);
    if (buf_attr->compress_mode == OT_COMPRESS_MODE_NONE) {
        ot_common_get_compress_none_pic_buf_cfg(buf_attr, bit_width, calc_cfg);
    } else {
        ot_common_get_compress_pic_buf_cfg(buf_attr, OT_ID_VPSS, bit_width, calc_cfg);
    }
}

OT_BUF_STATIC td_u32 ot_common_get_pic_buf_size(ot_pic_buf_attr *buf_attr)
{
    ot_vb_calc_cfg calc_cfg;

    if (buf_attr == TD_NULL) {
        return 0;
    }

    ot_common_get_pic_buf_cfg(buf_attr, &calc_cfg);

    return calc_cfg.vb_size;
}

OT_BUF_STATIC td_u32 ot_hnr_width_align(td_u32 size)
{
    td_u32 calc_size;
    const td_u32 shift_offset = 6; /* left offet is 3, right offset is 3 */

    calc_size = OT_ALIGN_UP(size + shift_offset, OT_BUF_VALUE_32) + OT_BUF_FACTOR_2; /* hnr need align up to 32 */

    return calc_size;
}

OT_BUF_STATIC td_u32 ot_hnr_heigh_align(td_u32 size)
{
    td_u32 calc_size;
    const td_u32 shift_offset = OT_BUF_ALIGN_7; /* top offet is 3, bottom offset is 4 */

    /* hnr vb offset: 3 Rows and 4 columns */
    calc_size = OT_ALIGN_UP(size + shift_offset, OT_BUF_VALUE_32) + OT_BUF_FACTOR_3; /* hnr need align up to 32 */

    return calc_size;
}

OT_BUF_STATIC td_u32 ot_hnr_get_vb_size(td_u32 width, td_u32 height, ot_compress_mode  compress_mode)
{
    ot_pic_buf_attr buf_attr;
    ot_vb_calc_cfg calc_cfg;

    buf_attr.width         = ot_hnr_width_align(width);
    buf_attr.height        = ot_hnr_heigh_align(height);
    buf_attr.align         = 0;
    buf_attr.bit_width     = OT_DATA_BIT_WIDTH_8;
    buf_attr.pixel_format  = OT_PIXEL_FORMAT_RGB_BAYER_12BPP;
    buf_attr.compress_mode = compress_mode;
    ot_common_get_pic_buf_cfg(&buf_attr, &calc_cfg);

    return calc_cfg.vb_size + OT_BUF_FACTOR_4; /* hnr vb offset: 3 Rows and 4 columns */
}

OT_BUF_STATIC td_u32 ot_hnr_get_pic_buf_size(td_u32 width, td_u32 height)
{
    td_u32 compress_none_buf_size;
    td_u32 compress_line_buf_size;

    if ((width > OT_MAXI_NUM_LIMIT) || (height > OT_MAXI_NUM_LIMIT)) {
        return 0;
    }

    compress_none_buf_size = ot_hnr_get_vb_size(width, height, OT_COMPRESS_MODE_NONE);
    compress_line_buf_size = ot_hnr_get_vb_size(width, height, OT_COMPRESS_MODE_LINE);

    return MAX2(compress_none_buf_size, compress_line_buf_size);
}

OT_BUF_STATIC td_bool ot_vdec_check_pic_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    if ((type == OT_PT_H265) && (width <= OT_VDH_H265D_MAX_WIDTH) && (height <= OT_VDH_H265D_MAX_HEIGHT)) {
        return TD_TRUE;
    }
    if ((type == OT_PT_H264) && (width <= OT_VDH_H264D_MAX_WIDTH) && (height <= OT_VDH_H264D_MAX_HEIGHT)) {
        return TD_TRUE;
    }
    if ((type == OT_PT_MP4VIDEO) && (width <= OT_VDH_MPEG4_MAX_WIDTH) && (height <= OT_VDH_MPEG4_MAX_HEIGHT)) {
        return TD_TRUE;
    }
    if ((type == OT_PT_JPEG || type == OT_PT_MJPEG) &&
        (width <= OT_JPEGD_MAX_WIDTH) && (height <= OT_JPEGD_MAX_HEIGHT)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

OT_BUF_STATIC td_u32 ot_vdec_get_pic_buf_size(ot_payload_type type,
    ot_pic_buf_attr *buf_attr)
{
    td_u32 align_width;
    td_u32 align_height;
    td_u32 header_size;
    td_u32 header_stride;
    td_u32 extra_size = 0;
    td_u32 size;

    if (buf_attr == TD_NULL) {
        return 0;
    }

    if (ot_vdec_check_pic_size(type, buf_attr->width, buf_attr->height) == TD_FALSE) {
        return 0;
    }

    if ((type == OT_PT_H264) || (type == OT_PT_MP4VIDEO)) {
        align_width = OT_ALIGN_UP(buf_attr->width, OT_H264D_ALIGN_W);
        align_height = OT_ALIGN_UP(buf_attr->height, OT_H264D_ALIGN_H);
        if (buf_attr->width <= OT_BUF_SIZE_LIMIT_8192) {
            header_stride = OT_ALIGN_UP(align_width, OT_BUF_ALIGN_2048) >> OT_BUF_SHIFT_6;
        } else {
            header_stride = OT_ALIGN_UP(align_width, OT_BUF_ALIGN_8192) >> OT_BUF_SHIFT_6;
        }
        header_size = header_stride * (OT_ALIGN_UP(align_height, OT_H264D_ALIGN_H) >> OT_BUF_ALIGN_2);
        size = ((header_size + align_width * align_height) * OT_BUF_SHIFT_3) >> OT_BUF_SHIFT_1;
    } else if (type == OT_PT_H265) {
        align_width = OT_ALIGN_UP(buf_attr->width, OT_H265D_ALIGN_W);
        align_height = OT_ALIGN_UP(buf_attr->height, OT_H265D_ALIGN_H);
        if (buf_attr->width <= OT_BUF_SIZE_LIMIT_8192) {
            header_stride = OT_ALIGN_UP(align_width, OT_BUF_ALIGN_2048) >> OT_BUF_SHIFT_6;
        } else {
            header_stride = OT_ALIGN_UP(align_width, OT_BUF_ALIGN_8192) >> OT_BUF_SHIFT_6;
        }
        header_size = header_stride * (OT_ALIGN_UP(align_height, OT_H265D_ALIGN_H) >> OT_BUF_ALIGN_2);

        if (buf_attr->bit_width == OT_DATA_BIT_WIDTH_10) {
            extra_size = OT_ALIGN_UP((align_width * OT_BUF_ALIGN_2) >> OT_BUF_FACTOR_3, OT_BUF_VALUE_16) *
                OT_ALIGN_UP(align_height, OT_BUF_VALUE_32);
        }

        size = ((header_size + align_width * align_height + extra_size) * OT_BUF_SHIFT_3) >> OT_BUF_SHIFT_1;
    } else if ((type == OT_PT_JPEG) || (type == OT_PT_MJPEG)) {
        /* for OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420 */
        align_width = OT_ALIGN_UP(buf_attr->width, OT_JPEGD_ALIGN_W);
        align_height = OT_ALIGN_UP(buf_attr->height, OT_JPEGD_ALIGN_H);
        if (buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
            size = align_width * align_height * OT_BUF_FACTOR_2; /* 2:Y+1/2U+1/2V */
        } else {
            size = (align_width * align_height * OT_BUF_FACTOR_3) >> OT_BUF_SHIFT_1; /* 3:Y+1/4U+1/4V */
        }
    } else {
        size = 0;
    }

    return size;
}

OT_BUF_STATIC td_u32 ot_vdec_get_tmv_buf_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    td_u32 width_in_mb;
    td_u32 height_in_mb;
    td_u32 col_mb_size;
    td_u32 size;

    if (ot_vdec_check_pic_size(type, width, height) == TD_FALSE) {
        return 0;
    }

    if ((type == OT_PT_H264) || (type == OT_PT_MP4VIDEO)) {
        width_in_mb = OT_ALIGN_UP(width, OT_BUF_ALIGN_16) >> OT_BUF_FACTOR_4;
        height_in_mb = OT_ALIGN_UP(height, OT_BUF_ALIGN_16) >> OT_BUF_FACTOR_4;
        col_mb_size = OT_BUF_VALUE_16 * OT_BUF_FACTOR_4;
        size = OT_ALIGN_UP((col_mb_size * width_in_mb * height_in_mb), OT_BUF_ALIGN_128);
    } else if (type == OT_PT_H265) {
        width_in_mb = OT_ALIGN_UP(width, OT_BUF_ALIGN_64) >> OT_BUF_FACTOR_4;
        height_in_mb = OT_ALIGN_UP(height, OT_BUF_ALIGN_64) >> OT_BUF_FACTOR_4;
        col_mb_size = OT_BUF_FACTOR_4 * OT_BUF_FACTOR_4;
        size = OT_ALIGN_UP((col_mb_size * width_in_mb * height_in_mb), OT_BUF_ALIGN_128);
    } else {
        size = 0;
    }

    return size;
}

OT_BUF_STATIC td_bool ot_venc_check_pic_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
{
    if (type != OT_PT_H264 && type != OT_PT_H265) {
        return TD_FALSE;
    }

    if (attr == NULL) {
        return TD_FALSE;
    }

    if (attr->share_buf_en != TD_FALSE && attr->share_buf_en != TD_TRUE) {
        return TD_FALSE;
    }

    if (attr->frame_buf_ratio < OT_BUF_VALUE_70 || attr->frame_buf_ratio > OT_BUF_ALIGN_100) {
        return TD_FALSE;
    }

    if (type == OT_PT_H264 &&
        (attr->pic_buf_attr.width > OT_VENC_H264_MAX_WIDTH || attr->pic_buf_attr.height > OT_VENC_H264_MAX_HEIGHT)) {
        return TD_FALSE;
    }

    if (type == OT_PT_H265 &&
        (attr->pic_buf_attr.width > OT_VENC_H265_MAX_WIDTH || attr->pic_buf_attr.height > OT_VENC_H265_MAX_HEIGHT)) {
        return TD_FALSE;
    }

    if (attr->pic_buf_attr.bit_width != OT_DATA_BIT_WIDTH_8) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

OT_BUF_STATIC td_u32 ot_venc_get_h264_header_stride(td_u32 width)
{
    if (width <= OT_BUF_SIZE_LIMIT_2048) {
        return OT_BUF_VALUE_128;
    } else if (width <= OT_BUF_SIZE_LIMIT_4096) {
        return OT_BUF_VALUE_256;
    } else if (width <= OT_BUF_SIZE_LIMIT_6144) {
        return OT_BUF_VALUE_384;
    }
    return OT_BUF_VALUE_512;
}

OT_BUF_STATIC td_u32 ot_venc_get_h265_header_stride(td_u32 width)
{
    if (width <= OT_BUF_SIZE_LIMIT_2048) {
        return OT_BUF_VALUE_256;
    } else if (width <= OT_BUF_SIZE_LIMIT_4096) {
        return OT_BUF_VALUE_512;
    } else if (width <= OT_BUF_SIZE_LIMIT_6144) {
        return OT_BUF_VALUE_768;
    }
    return OT_BUF_VALUE_1024;
}

OT_BUF_STATIC td_u32 ot_venc_get_h264_pic_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
{
    td_u32 y_header_stride;
    td_u32 c_header_stride;
    td_u32 y_header_size;
    td_u32 c_header_size;
    td_u32 y_size;
    td_u32 c_size;
    td_u32 blk_height_y;
    td_u32 blk_height_c;
    td_u32 pme_stride;
    td_u32 pme_size;
    td_u32 pme_ext_size;
    td_u32 width;
    td_u32 height;
    td_bool share_buf;
    td_u32 frame_buf_ratio;
    td_u32 pic_size;

    width = attr->pic_buf_attr.width;
    height = attr->pic_buf_attr.height;
    share_buf = attr->share_buf_en;
    frame_buf_ratio = attr->frame_buf_ratio;

    y_header_stride = ot_venc_get_h264_header_stride(width);
    c_header_stride = y_header_stride / OT_BUF_DIVISOR_2;

    y_header_size = y_header_stride * (OT_ALIGN_UP(height, OT_BUF_ALIGN_16) / OT_BUF_ALIGN_16);
    c_header_size = c_header_stride * (OT_ALIGN_UP(height, OT_BUF_ALIGN_16) / OT_BUF_ALIGN_16);

    if (share_buf == TD_FALSE) {
        blk_height_y = 0;
        blk_height_c = 0;
    } else {
        blk_height_y = ((OT_BUF_DIVISOR_16 * OT_BUF_FACTOR_4 + OT_BUF_DIVISOR_16 + OT_BUF_DIVISOR_16 +
            OT_BUF_VALUE_15) / OT_BUF_DIVISOR_16) * OT_BUF_DIVISOR_16;
        blk_height_c = blk_height_y / OT_BUF_DIVISOR_2;
    }

    y_size = OT_ALIGN_UP(width * (height + blk_height_y) * frame_buf_ratio / OT_BUF_ALIGN_100, OT_BUF_VALUE_256);
    c_size = OT_ALIGN_UP(width * (height / OT_BUF_ALIGN_2 + blk_height_c) * frame_buf_ratio /
        OT_BUF_ALIGN_100, OT_BUF_VALUE_256);

    pme_stride = OT_ALIGN_UP(width, OT_BUF_ALIGN_64) / OT_BUF_ALIGN_64;
    pme_size = pme_stride * (OT_ALIGN_UP(height, OT_BUF_ALIGN_16) / OT_BUF_ALIGN_16) * OT_BUF_VALUE_64;
    pme_ext_size = OT_ALIGN_UP(width, OT_BUF_ALIGN_64);

    if (share_buf == TD_FALSE) {
        pic_size = y_header_size + c_header_size + y_size + c_size;
    } else {
        pme_size += pme_ext_size;
        pic_size = (y_header_size + c_header_size) * OT_BUF_FACTOR_2 + y_size + c_size + pme_size;
    }

    return pic_size;
}

OT_BUF_STATIC td_u32 ot_venc_get_h265_pic_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
{
    td_u32 y_header_stride;
    td_u32 c_header_stride;
    td_u32 y_header_size;
    td_u32 c_header_size;
    td_u32 y_size;
    td_u32 c_size;
    td_u32 blk_height_y;
    td_u32 blk_height_c;
    td_u32 pme_stride;
    td_u32 pme_size;
    td_u32 pme_ext_size;
    td_u32 width;
    td_u32 height;
    td_bool share_buf;
    td_u32 frame_buf_ratio;
    td_u32 pic_size;

    width = attr->pic_buf_attr.width;
    height = attr->pic_buf_attr.height;
    share_buf = attr->share_buf_en;
    frame_buf_ratio = attr->frame_buf_ratio;

    y_header_stride = ot_venc_get_h265_header_stride(width);
    c_header_stride = y_header_stride / OT_BUF_DIVISOR_2;

    y_header_size = y_header_stride * (OT_ALIGN_UP(height, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32);
    c_header_size = c_header_stride * (OT_ALIGN_UP(height, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32);

    if (share_buf == TD_FALSE) {
        blk_height_y = 0;
        blk_height_c = 0;
    } else {
        blk_height_y = ((OT_BUF_VALUE_20 * OT_BUF_FACTOR_4 + OT_BUF_VALUE_16 + OT_BUF_VALUE_32 +
            OT_BUF_VALUE_15) / OT_BUF_VALUE_16) * OT_BUF_VALUE_16;
        blk_height_c = blk_height_y / OT_BUF_DIVISOR_2;
    }

    y_size = OT_ALIGN_UP(width * (height + blk_height_y) * frame_buf_ratio / OT_BUF_ALIGN_100, OT_BUF_VALUE_256);
    c_size = OT_ALIGN_UP(width * (height / OT_BUF_ALIGN_2 + blk_height_c) * frame_buf_ratio /
        OT_BUF_ALIGN_100, OT_BUF_VALUE_256);

    pme_stride = OT_ALIGN_UP(width, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32;
    pme_size = pme_stride * (OT_ALIGN_UP(height, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32) * OT_BUF_VALUE_64;
    pme_ext_size = OT_ALIGN_UP(width, OT_BUF_ALIGN_32) * OT_BUF_FACTOR_2;

    if (share_buf == TD_FALSE) {
        pic_size = y_header_size + c_header_size + y_size + c_size;
    } else {
        pme_size += pme_ext_size;
        pic_size = (y_header_size + c_header_size) * OT_BUF_FACTOR_2 + y_size + c_size + pme_size;
    }

    return pic_size;
}

OT_BUF_STATIC td_u32 ot_venc_get_pic_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
{
    if (ot_venc_check_pic_buf_size(type, attr) == TD_FALSE) {
        return 0;
    }

    if (type == OT_PT_H264) {
        return ot_venc_get_h264_pic_buf_size(type, attr);
    } else {
        return ot_venc_get_h265_pic_buf_size(type, attr);
    }
}

OT_BUF_STATIC td_bool ot_venc_check_pic_info_buf_size(ot_payload_type type, const ot_venc_buf_attr *attr)
{
    if (type != OT_PT_H264 && type != OT_PT_H265) {
        return TD_FALSE;
    }

    if (attr == NULL) {
        return TD_FALSE;
    }

    if (attr->share_buf_en != TD_FALSE && attr->share_buf_en != TD_TRUE) {
        return TD_FALSE;
    }

    if (type == OT_PT_H264 &&
        (attr->pic_buf_attr.width > OT_VENC_H264_MAX_WIDTH || attr->pic_buf_attr.height > OT_VENC_H264_MAX_HEIGHT)) {
        return TD_FALSE;
    }

    if (type == OT_PT_H265 &&
        (attr->pic_buf_attr.width > OT_VENC_H265_MAX_WIDTH || attr->pic_buf_attr.height > OT_VENC_H265_MAX_HEIGHT)) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

OT_BUF_STATIC td_u32 ot_venc_get_pic_info_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
{
    td_u32 pic_info_size;
    td_u32 tmv_size;
    td_u32 pme_stride;
    td_u32 pme_size;
    td_u32 pme_info_size;
    td_u32 width;
    td_u32 height;
    td_bool share_buf;

    if (ot_venc_check_pic_info_buf_size(type, attr) == TD_FALSE) {
        return 0;
    }

    width = attr->pic_buf_attr.width;
    height = attr->pic_buf_attr.height;
    share_buf = attr->share_buf_en;

    if (type == OT_PT_H264) {
        tmv_size = 0;
        pme_stride = OT_ALIGN_UP(width, OT_BUF_ALIGN_64) / OT_BUF_ALIGN_64;
        pme_size = pme_stride * (OT_ALIGN_UP(height, OT_BUF_ALIGN_16) / OT_BUF_ALIGN_16) * OT_BUF_VALUE_64;
        pme_info_size = (OT_ALIGN_UP(width, OT_BUF_ALIGN_1024) / OT_BUF_VALUE_16) *
            (OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16) * OT_BUF_FACTOR_2;
    } else {
        tmv_size = ((OT_ALIGN_UP(width, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32) *
            (OT_ALIGN_UP(height, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32) + 1) / OT_BUF_FACTOR_2 * OT_BUF_FACTOR_2 *
            OT_BUF_VALUE_16;
        pme_stride = OT_ALIGN_UP(width, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32;
        pme_size = pme_stride * (OT_ALIGN_UP(height, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32) * OT_BUF_VALUE_64;
        pme_info_size = (OT_ALIGN_UP(width, OT_BUF_ALIGN_512) / OT_BUF_VALUE_32) *
            (OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_32) * OT_BUF_FACTOR_2;
    }

    if (share_buf == TD_FALSE) {
        pic_info_size = tmv_size + pme_size + pme_info_size;
    } else {
        pic_info_size = tmv_size + pme_info_size;
    }

    return pic_info_size;
}

OT_BUF_STATIC td_u32 ot_venc_get_qpmap_stride(td_u32 width)
{
    if (width > OT_MAXI_NUM_LIMIT) {
        return 0;
    }

    return OT_ALIGN_UP(width, OT_BUF_ALIGN_256) / OT_BUF_VALUE_16;
}

OT_BUF_STATIC td_u32 ot_venc_get_qpmap_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 align_height;

    if ((width > OT_MAXI_NUM_LIMIT) || (height > OT_MAXI_NUM_LIMIT)) {
        return 0;
    }

    stride = ot_venc_get_qpmap_stride(width);
    if (type == OT_PT_H265) {
        align_height = OT_ALIGN_UP(height, OT_BUF_ALIGN_32) / OT_BUF_VALUE_16;
    } else if (type == OT_PT_H264) {
        align_height = OT_ALIGN_UP(height, OT_BUF_ALIGN_16) / OT_BUF_ALIGN_16;
    } else {
        align_height = 0;
    }

    return stride * align_height;
}

OT_BUF_STATIC td_u32 ot_venc_get_skip_weight_stride(ot_payload_type type, td_u32 width)
{
    td_u32 stride;

    if (width > OT_MAXI_NUM_LIMIT) {
        return 0;
    }

    if (type == OT_PT_H265) {
        stride = OT_ALIGN_UP(width, OT_BUF_ALIGN_1024) / OT_BUF_VALUE_64;
    } else if (type == OT_PT_H264) {
        stride = OT_ALIGN_UP(width, OT_BUF_ALIGN_512) / OT_BUF_VALUE_32;
    } else {
        stride = 0;
    }

    return stride;
}

OT_BUF_STATIC td_u32 ot_venc_get_skip_weight_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 align_height;

    if ((width > OT_MAXI_NUM_LIMIT) || (height > OT_MAXI_NUM_LIMIT)) {
        return 0;
    }

    stride = ot_venc_get_skip_weight_stride(type, width);

    if (type == OT_PT_H265) {
        align_height = OT_ALIGN_UP(height, OT_BUF_ALIGN_32) / OT_BUF_ALIGN_32;
    } else if (type == OT_PT_H264) {
        align_height = OT_ALIGN_UP(height, OT_BUF_ALIGN_16) / OT_BUF_ALIGN_16;
    } else {
        align_height = 0;
    }

    return stride * align_height;
}

OT_BUF_STATIC td_u32 ot_venc_get_roimap_stride(ot_payload_type type, td_u32 width)
{
    td_u32 stride;

    if (width > OT_MAXI_NUM_LIMIT) {
        return 0;
    }

    if (type == OT_PT_JPEG || type == OT_PT_MJPEG) {
        // OT_BUF_ALIGN_1024 64: hardware align requirement
        stride = OT_ALIGN_UP(width, OT_BUF_ALIGN_1024) / OT_BUF_VALUE_64;
    } else {
        stride = 0;
    }

    return stride;
}

OT_BUF_STATIC td_u32 ot_venc_get_roimap_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 align_height;

    if ((width > OT_MAXI_NUM_LIMIT) || (height > OT_MAXI_NUM_LIMIT)) {
        return 0;
    }

    stride = ot_venc_get_roimap_stride(type, width);

    if (type == OT_PT_JPEG || type == OT_PT_MJPEG) {
        // OT_BUF_ALIGN_16: hardware align requirement
        align_height = OT_ALIGN_UP(height, OT_BUF_ALIGN_16) / OT_BUF_ALIGN_16;
    } else {
        align_height = 0;
    }

    return stride * align_height;
}

OT_BUF_STATIC td_bool ot_venc_is_mosaic_blk_valid(td_u32 blk_size)
{
    // 4, 8, 16: blk size
    if (blk_size == OT_BUF_FACTOR_4 || blk_size == OT_BUF_VALUE_8 || blk_size == OT_BUF_VALUE_16 ||
        // 32, 64, 128: blk size
        blk_size == OT_BUF_VALUE_32 || blk_size == OT_BUF_VALUE_64 || blk_size == OT_BUF_VALUE_128) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

OT_BUF_STATIC td_u32 ot_venc_get_mosaic_map_stride(td_u32 width, td_u32 blk_size)
{
    td_u32 blk_width;

    if (blk_size == 0 || width > OT_VENC_H265_MAX_WIDTH || !ot_venc_is_mosaic_blk_valid(blk_size)) {
        return 0;
    }

    blk_width = OT_ALIGN_UP(width, blk_size) / blk_size;

    return OT_ALIGN_UP(blk_width, OT_BUF_VALUE_128) / OT_BUF_VALUE_8; // 128, 8: hardware align requirement
}

OT_BUF_STATIC td_u32 ot_venc_get_mosaic_map_size(td_u32 width, td_u32 height, td_u32 blk_size)
{
    td_u32 stride;
    td_u32 blk_height;

    if (blk_size == 0 || width > OT_VENC_H265_MAX_WIDTH ||
        height > OT_VENC_H265_MAX_HEIGHT || !ot_venc_is_mosaic_blk_valid(blk_size)) {
        return 0;
    }

    stride = ot_venc_get_mosaic_map_stride(width, blk_size);
    blk_height = OT_ALIGN_UP(height, blk_size) / blk_size;

    return stride * blk_height;
}

OT_BUF_STATIC td_u32 ot_avs_get_buf_size(td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 size;

    stride = OT_ALIGN_UP(width, OT_BUF_ALIGN_128) * OT_BUF_FACTOR_2;
    size = stride * OT_ALIGN_UP(height, OT_BUF_ALIGN_64);

    return size;
}

OT_BUF_STATIC td_u32 ot_vpss_get_mcf_color_buf_size(td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 size;

    stride = OT_ALIGN_UP(width, OT_DEFAULT_ALIGN);
    size = stride * height * OT_BUF_FACTOR_59 / OT_BUF_VALUE_32;

    return size;
}

OT_BUF_STATIC td_u32 ot_vpss_get_mcf_mono_buf_size(td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 size;

    stride = OT_ALIGN_UP(width, OT_DEFAULT_ALIGN);

    if (width > OT_BUF_SIZE_LIMIT_4096) { /* 4096: line buffer */
        stride += OT_BUF_VALUE_64;  /* 64: for split */
    }

    size = stride * height * OT_BUF_FACTOR_43 / OT_BUF_VALUE_32;

    return size;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OT_BUFFER_H */

