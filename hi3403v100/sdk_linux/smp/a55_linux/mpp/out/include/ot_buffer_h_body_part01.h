/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART01_H_

#include "ot_math.h"
#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_video.h"
#include "securec.h"

#ifdef __KERNEL__
#include "ot_osal.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OT_MAXI_NUM_LIMIT 30000

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

static td_u32 ot_common_get_bit_width(ot_data_bit_width bit_width)
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
static td_void ot_copy_vb_calc_cfg(ot_vb_calc_cfg *dst_cfg, ot_vb_calc_cfg *src_cfg)
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
static td_bool ot_common_is_pixel_format_package_422(ot_pixel_format pixel_format)
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
static td_void ot_common_get_compress_none_pic_buf_cfg(ot_pic_buf_attr *buf_attr,
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
    align_height = OT_ALIGN_UP(buf_attr->height, OT_BUF_FACTOR_2);
    cfg.main_stride = OT_ALIGN_UP((buf_attr->width * (td_u32)bit_width +
        OT_BUF_ALIGN_7) >> OT_BUF_FACTOR_3, buf_attr->align);
    cfg.main_y_size = cfg.main_stride * align_height;

    if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_420)) {
        cfg.main_size = ((cfg.main_stride * align_height) * OT_BUF_FACTOR_3) >> 1;
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
static td_void ot_common_get_compact_pic_buf_cfg(ot_pic_buf_attr *buf_attr, ot_mod_id mod_id,
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
    align_height     = OT_ALIGN_UP(buf_attr->height, OT_BUF_FACTOR_2);
    cfg->main_stride = OT_ALIGN_UP(buf_attr->width * bit_width, OT_SEG_WIDTH_BIT_ALIGN);

    if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_420)) {
        align_c_height = align_height / OT_BUF_FACTOR_2;
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

    cfg->main_stride = cfg->main_stride >> OT_BUF_FACTOR_3;
    cfg->head_y_size = cfg->head_stride * align_height;
    cfg->head_size = cfg->head_stride * (align_height + align_c_height);
    cfg->main_size = cfg->main_y_size + c_size;

    return;
}
static td_void ot_common_get_compact_none_pic_buf_cfg(ot_pic_buf_attr *buf_attr,
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
    align_height = OT_ALIGN_UP(buf_attr->height, OT_BUF_FACTOR_2);
    cfg->main_stride = OT_ALIGN_UP(buf_attr->width, buf_attr->align);
    cfg->head_y_size = cfg->head_stride * align_height;
    cfg->main_y_size = cfg->main_stride * align_height;

    if ((buf_attr->pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (buf_attr->pixel_format == OT_PIXEL_FORMAT_YUV_SEMIPLANAR_420)) {
        cfg->head_size = (cfg->head_stride * align_height * OT_BUF_FACTOR_3) >> 1;
        cfg->main_size = (cfg->main_stride * align_height * OT_BUF_FACTOR_3) >> 1;
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
static td_void ot_common_get_compress_pic_buf_cfg(ot_pic_buf_attr *buf_attr, ot_mod_id mod_id,
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
static td_u32 ot_vi_get_raw_bit_width(ot_pixel_format pixel_format)
{
    switch (pixel_format) {
        case OT_PIXEL_FORMAT_RGB_BAYER_8BPP:
            return OT_BUF_VALUE_8; /* 8:single pixel width */
        case OT_PIXEL_FORMAT_RGB_BAYER_10BPP:
            return OT_BUF_VALUE_10; /* 10:single pixel width */
        case OT_PIXEL_FORMAT_RGB_BAYER_12BPP:
            return OT_BUF_VALUE_12; /* 12:single pixel width */
        case OT_PIXEL_FORMAT_RGB_BAYER_14BPP:
            return OT_BUF_VALUE_14; /* 14:single pixel width */
        case OT_PIXEL_FORMAT_RGB_BAYER_16BPP:
            return OT_BUF_VALUE_16; /* 16:single pixel width */
        default:
            return 0;
    }
}
static td_void ot_common_vi_get_raw_buf_cfg_with_compress_ratio(ot_pic_buf_attr *buf_attr,
    td_u32 compress_ratio, ot_vb_calc_cfg *calc_cfg)
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
        stride = OT_ALIGN_UP(OT_ALIGN_UP(width * bit_width, OT_BUF_VALUE_8) / OT_BUF_VALUE_8, buf_attr->align);
        size = stride * height;
    } else if (buf_attr->compress_mode == OT_COMPRESS_MODE_LINE) {
        td_u32 tmp;
        tmp = OT_ALIGN_UP((OT_BUF_VALUE_16 + width * bit_width * OT_BUF_VALUE_1000 / raw_compress_ratio +
            OT_BUF_SIZE_LIMIT_8192 + OT_BUF_VALUE_127) / OT_BUF_VALUE_128, OT_BUF_FACTOR_2);
        stride = OT_ALIGN_UP(tmp * OT_BUF_VALUE_16, buf_attr->align);
        size = stride * height;
    } else if (buf_attr->compress_mode == OT_COMPRESS_MODE_FRAME) {
        if ((OT_DIV_UP(width, OT_BUF_VALUE_32) * height) <= OT_BUF_SIZE_LIMIT_4096) {
            size = (height * width * bit_width * OT_BUF_FACTOR_3 / OT_BUF_FACTOR_2 + (width +
                OT_BUF_VALUE_31) / OT_BUF_VALUE_32 * height + OT_BUF_VALUE_255) / OT_BUF_VALUE_256 *
                OT_BUF_FACTOR_2 * OT_BUF_VALUE_16;
        } else {
            size = height * width * bit_width * OT_BUF_VALUE_1000 / (raw_compress_ratio * OT_BUF_VALUE_8);
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
static td_void ot_common_vi_get_raw_buf_cfg(ot_pic_buf_attr *buf_attr, ot_vb_calc_cfg *calc_cfg)
{
    ot_common_vi_get_raw_buf_cfg_with_compress_ratio(buf_attr, 0, calc_cfg);
}
static td_void ot_common_get_pic_buf_cfg(ot_pic_buf_attr *buf_attr,
    ot_vb_calc_cfg *calc_cfg)
{
    td_u32 bit_width;

    if (buf_attr == TD_NULL || calc_cfg == TD_NULL) {
        return;
    }
    if ((buf_attr->width > OT_MAXI_NUM_LIMIT) || (buf_attr->height > OT_MAXI_NUM_LIMIT)) {
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART01_H_ */
