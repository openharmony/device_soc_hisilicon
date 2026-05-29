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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART02_H_

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
static td_void ot_common_get_vi_compact_seg_buf_size(ot_pic_buf_attr *buf_attr,
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
static td_void ot_common_get_vpss_compact_seg_buf_size(ot_pic_buf_attr *buf_attr,
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
static td_u32 ot_common_get_pic_buf_size(ot_pic_buf_attr *buf_attr)
{
    ot_vb_calc_cfg calc_cfg;

    if (buf_attr == TD_NULL) {
        return 0;
    }
    ot_common_get_pic_buf_cfg(buf_attr, &calc_cfg);

    return calc_cfg.vb_size;
}
static td_u32 ot_hnr_width_align(td_u32 size)
{
    td_u32 calc_size;
    const td_u32 shift_offset = 6; /* left offet is 3, right offset is 3 */

    calc_size = OT_ALIGN_UP(size + shift_offset, OT_BUF_VALUE_32) + OT_BUF_FACTOR_2; /* hnr need align up to 32 */

    return calc_size;
}
static td_u32 ot_hnr_heigh_align(td_u32 size)
{
    td_u32 calc_size;
    const td_u32 shift_offset = OT_BUF_ALIGN_7; /* top offet is 3, bottom offset is 4 */

    /* hnr vb offset: 3 Rows and 4 columns */
    calc_size = OT_ALIGN_UP(size + shift_offset, OT_BUF_VALUE_32) + OT_BUF_FACTOR_3; /* hnr need align up to 32 */

    return calc_size;
}
static td_u32 ot_hnr_get_vb_size(td_u32 width, td_u32 height, ot_compress_mode  compress_mode)
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
static td_u32 ot_hnr_get_pic_buf_size(td_u32 width, td_u32 height)
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
static td_bool ot_vdec_check_pic_size(ot_payload_type type, td_u32 width, td_u32 height)
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
static td_u32 ot_vdec_get_pic_buf_size(ot_payload_type type,
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
            header_stride = OT_ALIGN_UP(align_width, OT_BUF_SIZE_LIMIT_2048) >> OT_BUF_SHIFT_6;
        } else {
            header_stride = OT_ALIGN_UP(align_width, OT_BUF_SIZE_LIMIT_8192) >> OT_BUF_SHIFT_6;
        }
        header_size = header_stride * (OT_ALIGN_UP(align_height, OT_H264D_ALIGN_H) >> OT_BUF_FACTOR_2);
        size = ((header_size + align_width * align_height) * OT_BUF_FACTOR_3) >> OT_BUF_SHIFT_1;
    } else if (type == OT_PT_H265) {
        align_width = OT_ALIGN_UP(buf_attr->width, OT_H265D_ALIGN_W);
        align_height = OT_ALIGN_UP(buf_attr->height, OT_H265D_ALIGN_H);
        if (buf_attr->width <= OT_BUF_SIZE_LIMIT_8192) {
            header_stride = OT_ALIGN_UP(align_width, OT_BUF_SIZE_LIMIT_2048) >> OT_BUF_SHIFT_6;
        } else {
            header_stride = OT_ALIGN_UP(align_width, OT_BUF_SIZE_LIMIT_8192) >> OT_BUF_SHIFT_6;
        }
        header_size = header_stride * (OT_ALIGN_UP(align_height, OT_H265D_ALIGN_H) >> OT_BUF_FACTOR_2);

        if (buf_attr->bit_width == OT_DATA_BIT_WIDTH_10) {
            extra_size = OT_ALIGN_UP((align_width * OT_BUF_FACTOR_2) >> OT_BUF_FACTOR_3, OT_BUF_VALUE_16) *
                OT_ALIGN_UP(align_height, OT_BUF_VALUE_32);
        }
        size = ((header_size + align_width * align_height + extra_size) * OT_BUF_FACTOR_3) >> OT_BUF_SHIFT_1;
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
static td_u32 ot_vdec_get_tmv_buf_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    td_u32 width_in_mb;
    td_u32 height_in_mb;
    td_u32 col_mb_size;
    td_u32 size;

    if (ot_vdec_check_pic_size(type, width, height) == TD_FALSE) {
        return 0;
    }
    if ((type == OT_PT_H264) || (type == OT_PT_MP4VIDEO)) {
        width_in_mb = OT_ALIGN_UP(width, OT_BUF_VALUE_16) >> OT_BUF_FACTOR_4;
        height_in_mb = OT_ALIGN_UP(height, OT_BUF_VALUE_16) >> OT_BUF_FACTOR_4;
        col_mb_size = OT_BUF_VALUE_16 * OT_BUF_FACTOR_4;
        size = OT_ALIGN_UP((col_mb_size * width_in_mb * height_in_mb), OT_BUF_VALUE_128);
    } else if (type == OT_PT_H265) {
        width_in_mb = OT_ALIGN_UP(width, OT_BUF_VALUE_64) >> OT_BUF_FACTOR_4;
        height_in_mb = OT_ALIGN_UP(height, OT_BUF_VALUE_64) >> OT_BUF_FACTOR_4;
        col_mb_size = OT_BUF_FACTOR_4 * OT_BUF_FACTOR_4;
        size = OT_ALIGN_UP((col_mb_size * width_in_mb * height_in_mb), OT_BUF_VALUE_128);
    } else {
        size = 0;
    }
    return size;
}
static td_bool ot_venc_check_pic_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
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
static td_u32 ot_venc_get_h264_header_stride(td_u32 width)
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
static td_u32 ot_venc_get_h265_header_stride(td_u32 width)
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
static td_u32 ot_venc_get_h264_pic_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
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
    c_header_stride = y_header_stride / OT_BUF_FACTOR_2;

    y_header_size = y_header_stride * (OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16);
    c_header_size = c_header_stride * (OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16);

    if (share_buf == TD_FALSE) {
        blk_height_y = 0;
        blk_height_c = 0;
    } else {
        blk_height_y = ((OT_BUF_VALUE_16 * OT_BUF_FACTOR_4 + OT_BUF_VALUE_16 + OT_BUF_VALUE_16 +
            OT_BUF_VALUE_15) / OT_BUF_VALUE_16) * OT_BUF_VALUE_16;
        blk_height_c = blk_height_y / OT_BUF_FACTOR_2;
    }
    y_size = OT_ALIGN_UP(width * (height + blk_height_y) * frame_buf_ratio / OT_BUF_ALIGN_100, OT_BUF_VALUE_256);
    c_size = OT_ALIGN_UP(width * (height / OT_BUF_FACTOR_2 + blk_height_c) * frame_buf_ratio /
        OT_BUF_ALIGN_100, OT_BUF_VALUE_256);

    pme_stride = OT_ALIGN_UP(width, OT_BUF_VALUE_64) / OT_BUF_VALUE_64;
    pme_size = pme_stride * (OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16) * OT_BUF_VALUE_64;
    pme_ext_size = OT_ALIGN_UP(width, OT_BUF_VALUE_64);

    if (share_buf == TD_FALSE) {
        pic_size = y_header_size + c_header_size + y_size + c_size;
    } else {
        pme_size += pme_ext_size;
        pic_size = (y_header_size + c_header_size) * OT_BUF_FACTOR_2 + y_size + c_size + pme_size;
    }
    return pic_size;
}
static td_u32 ot_venc_get_h265_pic_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
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
    c_header_stride = y_header_stride / OT_BUF_FACTOR_2;

    y_header_size = y_header_stride * (OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_32);
    c_header_size = c_header_stride * (OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_32);

    if (share_buf == TD_FALSE) {
        blk_height_y = 0;
        blk_height_c = 0;
    } else {
        blk_height_y = ((OT_BUF_VALUE_20 * OT_BUF_FACTOR_4 + OT_BUF_VALUE_16 + OT_BUF_VALUE_32 +
            OT_BUF_VALUE_15) / OT_BUF_VALUE_16) * OT_BUF_VALUE_16;
        blk_height_c = blk_height_y / OT_BUF_FACTOR_2;
    }
    y_size = OT_ALIGN_UP(width * (height + blk_height_y) * frame_buf_ratio / OT_BUF_ALIGN_100, OT_BUF_VALUE_256);
    c_size = OT_ALIGN_UP(width * (height / OT_BUF_FACTOR_2 + blk_height_c) * frame_buf_ratio /
        OT_BUF_ALIGN_100, OT_BUF_VALUE_256);
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART02_H_ */
