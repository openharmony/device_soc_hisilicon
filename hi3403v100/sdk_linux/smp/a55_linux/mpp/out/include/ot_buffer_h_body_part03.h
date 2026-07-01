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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART03_H_


    pme_stride = OT_ALIGN_UP(width, OT_BUF_VALUE_32) / OT_BUF_VALUE_32;
    pme_size = pme_stride * (OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_32) * OT_BUF_VALUE_64;
    pme_ext_size = OT_ALIGN_UP(width, OT_BUF_VALUE_32) * OT_BUF_FACTOR_2;

    if (share_buf == TD_FALSE) {
        pic_size = y_header_size + c_header_size + y_size + c_size;
    } else {
        pme_size += pme_ext_size;
        pic_size = (y_header_size + c_header_size) * OT_BUF_FACTOR_2 + y_size + c_size + pme_size;
    }

    return pic_size;
}

static td_u32 ot_venc_get_pic_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
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

static td_bool ot_venc_check_pic_info_buf_size(ot_payload_type type, const ot_venc_buf_attr *attr)
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

static td_u32 ot_venc_get_pic_info_buf_size(ot_payload_type type, ot_venc_buf_attr *attr)
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
        pme_stride = OT_ALIGN_UP(width, OT_BUF_VALUE_64) / OT_BUF_VALUE_64;
        pme_size = pme_stride * (OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16) * OT_BUF_VALUE_64;
        pme_info_size = (OT_ALIGN_UP(width, OT_BUF_VALUE_1024) / OT_BUF_VALUE_16) *
            (OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16) * OT_BUF_FACTOR_2;
    } else {
        tmv_size = ((OT_ALIGN_UP(width, OT_BUF_VALUE_32) / OT_BUF_VALUE_32) *
            (OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_32) + 1) / OT_BUF_FACTOR_2 * OT_BUF_FACTOR_2 *
            OT_BUF_VALUE_16;
        pme_stride = OT_ALIGN_UP(width, OT_BUF_VALUE_32) / OT_BUF_VALUE_32;
        pme_size = pme_stride * (OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_32) * OT_BUF_VALUE_64;
        pme_info_size = (OT_ALIGN_UP(width, OT_BUF_VALUE_512) / OT_BUF_VALUE_32) *
            (OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_32) * OT_BUF_FACTOR_2;
    }

    if (share_buf == TD_FALSE) {
        pic_info_size = tmv_size + pme_size + pme_info_size;
    } else {
        pic_info_size = tmv_size + pme_info_size;
    }

    return pic_info_size;
}

static td_u32 ot_venc_get_qpmap_stride(td_u32 width)
{
    if (width > OT_MAXI_NUM_LIMIT) {
        return 0;
    }

    return OT_ALIGN_UP(width, OT_BUF_VALUE_256) / OT_BUF_VALUE_16;
}

static td_u32 ot_venc_get_qpmap_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 align_height;

    if ((width > OT_MAXI_NUM_LIMIT) || (height > OT_MAXI_NUM_LIMIT)) {
        return 0;
    }

    stride = ot_venc_get_qpmap_stride(width);
    if (type == OT_PT_H265) {
        align_height = OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_16;
    } else if (type == OT_PT_H264) {
        align_height = OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16;
    } else {
        align_height = 0;
    }

    return stride * align_height;
}

static td_u32 ot_venc_get_skip_weight_stride(ot_payload_type type, td_u32 width)
{
    td_u32 stride;

    if (width > OT_MAXI_NUM_LIMIT) {
        return 0;
    }

    if (type == OT_PT_H265) {
        stride = OT_ALIGN_UP(width, OT_BUF_VALUE_1024) / OT_BUF_VALUE_64;
    } else if (type == OT_PT_H264) {
        stride = OT_ALIGN_UP(width, OT_BUF_VALUE_512) / OT_BUF_VALUE_32;
    } else {
        stride = 0;
    }

    return stride;
}

static td_u32 ot_venc_get_skip_weight_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 align_height;

    if ((width > OT_MAXI_NUM_LIMIT) || (height > OT_MAXI_NUM_LIMIT)) {
        return 0;
    }

    stride = ot_venc_get_skip_weight_stride(type, width);

    if (type == OT_PT_H265) {
        align_height = OT_ALIGN_UP(height, OT_BUF_VALUE_32) / OT_BUF_VALUE_32;
    } else if (type == OT_PT_H264) {
        align_height = OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16;
    } else {
        align_height = 0;
    }

    return stride * align_height;
}

static td_u32 ot_venc_get_roimap_stride(ot_payload_type type, td_u32 width)
{
    td_u32 stride;

    if (width > OT_MAXI_NUM_LIMIT) {
        return 0;
    }

    if (type == OT_PT_JPEG || type == OT_PT_MJPEG) {
        stride = OT_ALIGN_UP(width, OT_BUF_VALUE_1024) / OT_BUF_VALUE_64; // 1024 64: hardware align requirement
    } else {
        stride = 0;
    }

    return stride;
}

static td_u32 ot_venc_get_roimap_size(ot_payload_type type, td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 align_height;

    if ((width > OT_MAXI_NUM_LIMIT) || (height > OT_MAXI_NUM_LIMIT)) {
        return 0;
    }

    stride = ot_venc_get_roimap_stride(type, width);

    if (type == OT_PT_JPEG || type == OT_PT_MJPEG) {
        align_height = OT_ALIGN_UP(height, OT_BUF_VALUE_16) / OT_BUF_VALUE_16; // 16: hardware align requirement
    } else {
        align_height = 0;
    }

    return stride * align_height;
}

static td_bool ot_venc_is_mosaic_blk_valid(td_u32 blk_size)
{
    // 4, 8, 16: blk size
    if (blk_size == OT_BUF_FACTOR_4 || blk_size == OT_BUF_VALUE_8 || blk_size == OT_BUF_VALUE_16 ||
        // 32, 64, 128: blk size
        blk_size == OT_BUF_VALUE_32 || blk_size == OT_BUF_VALUE_64 || blk_size == OT_BUF_VALUE_128) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

static td_u32 ot_venc_get_mosaic_map_stride(td_u32 width, td_u32 blk_size)
{
    td_u32 blk_width;

    if (blk_size == 0 || width > OT_VENC_H265_MAX_WIDTH || !ot_venc_is_mosaic_blk_valid(blk_size)) {
        return 0;
    }

    blk_width = OT_ALIGN_UP(width, blk_size) / blk_size;

    return OT_ALIGN_UP(blk_width, OT_BUF_VALUE_128) / OT_BUF_VALUE_8; // 128, 8: hardware align requirement
}

static td_u32 ot_venc_get_mosaic_map_size(td_u32 width, td_u32 height, td_u32 blk_size)
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

static td_u32 ot_avs_get_buf_size(td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 size;

    stride = OT_ALIGN_UP(width, OT_BUF_VALUE_128) * OT_BUF_FACTOR_2;
    size = stride * OT_ALIGN_UP(height, OT_BUF_VALUE_64);

    return size;
}

static td_u32 ot_vpss_get_mcf_color_buf_size(td_u32 width, td_u32 height)
{
    td_u32 stride;
    td_u32 size;

    stride = OT_ALIGN_UP(width, OT_DEFAULT_ALIGN);
    size = stride * height * OT_BUF_FACTOR_59 / OT_BUF_VALUE_32;

    return size;
}

static td_u32 ot_vpss_get_mcf_mono_buf_size(td_u32 width, td_u32 height)
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


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_BUFFER_H_BODY_PART03_H_ */
