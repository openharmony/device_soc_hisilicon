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
#include "drv_hdmi_infoframe.h"
#include "drv_hdmi_intf.h"
#include "drv_hdmi_common.h"
#include "hdmi_hal.h"

#define HDMI_AVI_VERSION3 3
#define HDMI_AVI_VERSION4 4
#define HDMI_AVI_Y2_MASK (0x1 << 2)

#define AVI_FRAME_COLOSR_SPACE_MASK 0x3
#define AVI_FRAME_SCAN_INFOMATION_MASK 0x3
#define AVI_FRAME_NUP_SCAN_MASK 0x3
#define AVI_FRAME_CONTENT_TYPE_MASK 0x3
#define AVI_FRAME_BAR_MODE_MASK 0xFF
#define AVI_INFORFRAME_BAR_MODE_OFFSET 8
#define AVI_INFORFRAME_LENGTH 13
#define AUDIO_INFORFRAME_LENGTH 10
#define DYN_RAN_INFORFRAME_LENGTH 26
#define DRM_INFORFRAME_META_MASK 0xFF
#define DRM_INFORFRAME_META_OFFSET 8
#define HDMI_INFOFRAME_BUFFER_SIZE 32
#define HDMI_INFOFRAME_CHECCKSUM 256
#define AUDIO_FRAME_CHANNEL_MASK 0x7
#define AUDIO_FRAME_SAMPLE_FREQ_MASK 0x7
#define AUDIO_FRAME_SAMPLE_SIZE_MASK 0x3
#define AUDIO_FRAME_EXT_CONDE_TYPE_MASK 0x1F
#define AUDIO_FRAME_LEVEL_SHIFT_MASK 0xF
#define AUDIO_FRAME_LFE_PLAYBACK_LEVEL_MASK 0x3
#define VENDOR_FARAME_3D_STRUCT_MASK 0xF
#define VSIF_DOLBY_DATA_LEN 0x18
#define VSIF_4K_DATA_LEN 0x5
#define VSIF_3D_DATA_LEN 0x7
#define VSIF_HDR_DATA_LEN 0x4
#define VSIF_DEFAULT_DATA_LEN 0x7

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wenum-conversion"
#endif

static td_void hdmi_infoframe_checksum(td_void *buffer, td_u32 size)
{
    td_u32 i;
    td_u8 checksum = 0;
    td_u8 *ptr = TD_NULL;

    ptr = buffer;
    /* compute checksum */
    for (i = 0; i < size; i++) {
        checksum += ptr[i];
    }
    if (checksum > 0) {
        ptr[AVI_OFFSET_CHECKSUM] = HDMI_INFOFRAME_CHECCKSUM - checksum;
    } else {
        ptr[AVI_OFFSET_CHECKSUM] = 0;
    }
}

/*
 * hdmi_avi_infoframe_pack() - write HDMI AVI infoframe to binary buffer
 * @frame: HDMI AVI infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 * packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 * returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
static hdmi_video_code_vic avi_header_pack(struct hdmi_avi_infoframe *frame, td_void *buffer, hdmi_3d_mode _3d_mode)
{
    td_u8 *ptr = TD_NULL;
    td_bool _3d_enable;
    hdmi_video_code_vic video_code;

    _3d_enable = (_3d_mode != HDMI_3D_BUTT) ? TD_FALSE : TD_TRUE;
    video_code = drv_hdmi_vic_search(frame->video_timing, frame->picture_aspect, _3d_enable);

    ptr = buffer;
    ptr[AVI_OFFSET_TYPE] = frame->type;
    if (((td_u32)frame->colorspace & HDMI_AVI_Y2_MASK) || (video_code > HDMI_5120X2160P100_64_27)) {
        /* Y2 = 1 or vic >= 128, version shall use 3 */
        frame->version = HDMI_AVI_VERSION3;
    } else if (frame->colorimetry == HDMI_COLORIMETRY_EXTEND &&
               frame->extended_colorimetry == HDMI_EXTENDED_COLORIMETRY_ADDITIONAL) {
        /* (C1,C0)=(1,1) and (EC2,EC1,EC0)=(1,1,1), version shall use 4 */
        frame->version = HDMI_AVI_VERSION4;
    }
    ptr[AVI_OFFSET_VERSION]  = frame->version;
    ptr[AVI_OFFSET_LENGTH]   = frame->length;
    ptr[AVI_OFFSET_CHECKSUM] = 0;

    return video_code;
}

static td_s32 hdmi_avi_infoframe_pack(struct hdmi_avi_infoframe *frame, td_void *buffer, td_u32 size, hdmi_3d_mode mode)
{
    td_u32 length;
    td_u8 *ptr = TD_NULL;
    hdmi_video_code_vic video_code;

    length = frame->length + HDMI_INFOFRAME_HEADER_SIZE;
    if (size < length) {
        return -1;
    }
    (td_void)memset_s(buffer, size, 0, size);
    video_code = avi_header_pack(frame, buffer, mode);

    ptr = buffer;
    /* 5, offset for byte, bit[6:5]: color space */
    ptr[AVI_OFFSET_PB1] = ((frame->colorspace & AVI_FRAME_COLOSR_SPACE_MASK) << 5) |
             (frame->scan_mode & AVI_FRAME_SCAN_INFOMATION_MASK);
    if (frame->active_info_valid) {
        /* 4, offset for byte, [4]: active information present */
        hdmi_set_bit(ptr[AVI_OFFSET_PB1], 4);
    }
    if (frame->horizontal_bar_valid) {
        /* 3, offset for byte, [3]: horizontal bar valid */
        hdmi_set_bit(ptr[AVI_OFFSET_PB1], 3);
    }
    if (frame->vertical_bar_valid) {
        /* 2, offset for byte, [2]: vertical bar valid */
        hdmi_set_bit(ptr[AVI_OFFSET_PB1], 2);
    }
    /* 6, offset for byte, [7,6]: colorimetry */
    ptr[AVI_OFFSET_PB2] = ((frame->colorimetry & AVI_FRAME_COLORIMETRY_MASK) << 6) |
            /* 4, offset for byte, [5,4]: frame aspect ratio */
            ((frame->picture_aspect & AVI_FRAME_PIC_ASPECT_MASK) << 4) |
            (frame->active_aspect & AVI_FRAME_ACTIVE_ASPECT_MASK);
    /* 4, offset for byte, [6,4]: Extended Colorimetry */
    ptr[AVI_OFFSET_PB3] = (((td_u32)frame->extended_colorimetry & AVI_FRAME_EXT_COLORIMETRY_MASK) << 4) |
            /* 2, offset for byte, [3,2]: RGB quantization range */
            (((td_u32)frame->quantization_range & AVI_FRAME_QUANT_RANGE_MASK) << 2) |
            ((td_u32)frame->nups & AVI_FRAME_NUP_SCAN_MASK);
    if (frame->itc) {
        /* 7, offset for byte, [7]: IT content */
        hdmi_set_bit(ptr[AVI_OFFSET_PB3], 7);
    }
    ptr[AVI_OFFSET_VIC] = (td_u8)video_code;
    /* 6, offset for byte, [7,6]: quantization range */
    ptr[AVI_OFFSET_PB5] = (((td_u32)frame->ycc_quantization_range & AVI_FRAME_YCC_QUANT_RANGE_MASK) << 6) |
        /* 4, offset for byte, [5,4]: quantization range */
        (((td_u32)frame->content_type & AVI_FRAME_CONTENT_TYPE_MASK) << 4) |
        ((frame->pixel_repeat - 1) & AVI_FRAME_PIXEL_REPET_MASK);
    ptr[AVI_OFFSET_TOP_BAR_LOWER] = frame->top_bar & AVI_FRAME_BAR_MODE_MASK;
    ptr[AVI_OFFSET_TOP_BAR_UPPER] = (frame->top_bar >> AVI_INFORFRAME_BAR_MODE_OFFSET) & AVI_FRAME_BAR_MODE_MASK;
    ptr[AVI_OFFSET_BOTTOM_BAR_LOWER] = frame->bottom_bar & AVI_FRAME_BAR_MODE_MASK;
    ptr[AVI_OFFSET_BOTTOM_BAR_UPPER] =
        (frame->bottom_bar >> AVI_INFORFRAME_BAR_MODE_OFFSET) & AVI_FRAME_BAR_MODE_MASK;
    ptr[AVI_OFFSET_LEFT_BAR_LOWER] = frame->left_bar & AVI_FRAME_BAR_MODE_MASK;
    ptr[AVI_OFFSET_LEFT_BAR_UPPER] = (frame->left_bar >> AVI_INFORFRAME_BAR_MODE_OFFSET) & AVI_FRAME_BAR_MODE_MASK;
    ptr[AVI_OFFSET_RIGHT_BAR_LOWER] = frame->right_bar & AVI_FRAME_BAR_MODE_MASK;
    ptr[AVI_OFFSET_RIGHT_BAR_UPPER] =
        (frame->right_bar >> AVI_INFORFRAME_BAR_MODE_OFFSET) & AVI_FRAME_BAR_MODE_MASK;
    hdmi_infoframe_checksum(buffer, length);

    return length;
}

/*
 * hdmi_audio_infoframe_pack() - write HDMI audio infoframe to binary buffer
 * @frame: HDMI audio infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 * packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 * returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
static td_s32 hdmi_audio_infoframe_pack(const struct hdmi_audio_infoframe *frame, td_void *buffer, td_u32 size)
{
    td_u32 length;
    td_u8 *ptr = TD_NULL;

    ptr = buffer;
    length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;
    if (size < length) {
        return -1;
    }
    (td_void)memset_s(buffer, size, 0, size);

    ptr[AUDIO_OFFSET_TYPE]     = frame->type;
    ptr[AUDIO_OFFSET_VERSION]  = frame->version;
    ptr[AUDIO_OFFSET_LENGHT]   = frame->length;
    ptr[AUDIO_OFFSET_CHECKSUM] = 0;

    /* start infoframe payload */
    /* 4, offset for byte, [7,4]: coding type. */
    ptr[AUDIO_OFFSET_PB1] = (((td_u32)frame->coding_type & AUDIO_FRAME_CODE_TYPE_MASK) << 4) |
                            (frame->channels & AUDIO_FRAME_CHANNEL_MASK);
    /* 2, offset for byte, [4,2]: sample frequency. */
    ptr[AUDIO_OFFSET_PB2] = (((td_u32)frame->sample_frequency & AUDIO_FRAME_SAMPLE_FREQ_MASK) << 2) |
                            ((td_u32)frame->sample_size & AUDIO_FRAME_SAMPLE_SIZE_MASK);
    ptr[AUDIO_OFFSET_FORMAT] = (td_u32)frame->coding_type_ext & AUDIO_FRAME_EXT_CONDE_TYPE_MASK;
    ptr[AUDIO_OFFSET_CA] = frame->channel_allocation;
    /* 3, offset for byte, [6,3]: level shift value. */
    ptr[AUDIO_OFFSET_PB5] = ((td_u32)frame->level_shift_value & AUDIO_FRAME_LEVEL_SHIFT_MASK) << 3;
    ptr[AUDIO_OFFSET_PB5] |= (td_u32)frame->lfe_playback_level & AUDIO_FRAME_LFE_PLAYBACK_LEVEL_MASK;
    if (frame->downmix_inhibit) {
        /* 7, offset for byte, [7]: Downmix Inhibit. */
        hdmi_set_bit(ptr[AUDIO_OFFSET_PB5], 7);
    }
    hdmi_infoframe_checksum(buffer, length);

    return length;
}

/*
 * hdmi_vendor_infoframe_pack() - write a HDMI vendor infoframe to binary buffer
 * @frame: HDMI vendor infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 * packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 * returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
static td_s32 hdmi_vendor_specific_infoframe_pack(const struct hdmi_vendor_specific_infoframe *frame,
    td_void *buffer, td_u32 size)
{
    errno_t ret;
    td_u32 length;
    td_u8 *ptr = TD_NULL;

    ptr = buffer;
    length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;
    if (size < length) {
        return -1;
    }
    (td_void)memset_s(buffer, size, 0, size);

    ptr[VENDOR_OFFSET_TYPE]    = frame->type;
    ptr[VENDOR_OFFSET_VERSION] = frame->version;
    ptr[VENDOR_OFFSET_LENGHT]  = frame->length;
    ptr[VENDOR_OFFSET_CHECSUM] = 0;

    if (frame->vsif_content.h14_vsif_content.ieee == HDMI14_IEEE_OUI) {
        const hdmi_14_vsif_content *vsif_content = &frame->vsif_content.h14_vsif_content;
        const hdmi_user_vsif_content *user_content = &frame->vsif_content.cea861_vsif;

        /* IEEE OUI */
        ptr[VENDOR_OFFSET_IEEE_LOWER] = HDMI14_IEEE_OUI & 0xff;
        /* 8, lower byte of word */
        ptr[VENDOR_OFFSET_IEEE_UPPER] = (HDMI14_IEEE_OUI >> 8) & 0xff;
        /* 16, len of word */
        ptr[VENDOR_OFFSET_IEEE] = (HDMI14_IEEE_OUI >> 16) & 0xff;
        /* 5, offset for byte, [7,5]: hdmi video format */
        ptr[VENDOR_OFFSET_VIDEO_FMT] = ((td_u32)vsif_content->format & VENDOR_FARAME_VIDEO_FMT_MASK) << 5;
        if (vsif_content->format == HDMI_VIDEO_FORMAT_4K) {
            ptr[VENDOR_OFFSET_VIC] = vsif_content->vic;
        } else if (vsif_content->format == HDMI_VIDEO_FORMAT_3D) {
            /* 4, offset for byte, [7,4]: 3d structure */
            ptr[VENDOR_OFFSET_VIC] = ((td_u32)vsif_content->_3d_structure & VENDOR_FARAME_3D_STRUCT_MASK) << 4;
        }
        /* if the 3D_Meta_present bit is clear(=0), mean that there have no 3D_Metadata */
        if (vsif_content->_3d_meta_present == 0) {
            /* have user data  */
            if ((user_content->len != 0) && (length + user_content->len <= size)) {
                ptr[VENDOR_OFFSET_LENGHT] += user_content->len;
                ret = memcpy_s(&ptr[length], (size - length), user_content->data, user_content->len);
                hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);
                length += user_content->len;
            }
        }
    }
    hdmi_infoframe_checksum(buffer, length);

    return length;
}

hdmi_video_timing drv_hdmi_video_timing_lookup(hdmi_video_code_vic vic, hdmi_picture_aspect aspect)
{
    return drv_hdmi_video_timing_get(vic, aspect);
}

hdmi_video_timing drv_hdmi_vsif_video_timing_lookup(hdmi_vsif_vic vic)
{
    return drv_hdmi_vsif_video_timing_get(vic);
}

td_void drv_hdmi_infoframe_send(const hdmi_infoframe_type *info_frame,
    hdmi_infoframe_id infoframe_id, hdmi_infoframe *infoframe)
{
    td_u8 buffer[HDMI_INFOFRAME_BUFFER_SIZE] = {0};
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_return_void(info_frame);
    hdmi_if_null_return_void(infoframe);

    hdmi_dev = osal_container_of(info_frame, __typeof__(*hdmi_dev), info_frame);
    hdmi_if_null_return_void(hdmi_dev);

    /* pack infoframe */
    switch (infoframe_id) {
        case HDMI_INFOFRAME_TYPE_VENDOR:
            infoframe->vendor_infoframe.type = HDMI_INFOFRAME_TYPE_VENDOR;
            infoframe->vendor_infoframe.version = 1;
            hdmi_vendor_specific_infoframe_pack(&infoframe->vendor_infoframe, buffer, HDMI_INFOFRAME_BUFFER_SIZE);
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            infoframe->avi_infoframe.type = HDMI_INFOFRAME_TYPE_AVI;
            infoframe->avi_infoframe.version = 2; /* 2, avi infoframe version */
            infoframe->avi_infoframe.length = AVI_INFORFRAME_LENGTH;
            hdmi_avi_infoframe_pack(&infoframe->avi_infoframe, buffer, HDMI_INFOFRAME_BUFFER_SIZE,
                hdmi_dev->attr.vo_attr.stereo_mode);
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO:
            infoframe->audio_infoframe.type = HDMI_INFOFRAME_TYPE_AUDIO;
            infoframe->audio_infoframe.version = 1;
            infoframe->audio_infoframe.length = AUDIO_INFORFRAME_LENGTH;
            hdmi_audio_infoframe_pack(&infoframe->audio_infoframe, buffer, HDMI_INFOFRAME_BUFFER_SIZE);
            break;
        default:
            return;
    }
    drv_hdmi_infoframe_enable_set(info_frame, infoframe_id, TD_FALSE);
    hal_call_void(hdmi_dev, hal_hdmi_infoframe_set, infoframe_id, buffer, HDMI_INFOFRAME_BUFFER_SIZE);
    drv_hdmi_infoframe_enable_set(info_frame, infoframe_id, TD_TRUE);

    return;
}

td_void drv_hdmi_infoframe_enable_set(const hdmi_infoframe_type *info_frame, hdmi_infoframe_id infoframe_id,
    td_bool enable)
{
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_return_void(info_frame);

    hdmi_dev = osal_container_of(info_frame, __typeof__(*hdmi_dev), info_frame);
    hal_call_void(hdmi_dev, hal_hdmi_infoframe_enable_set, infoframe_id, enable);

    return;
}

td_s32 drv_hdmi_avi_infoframe_send(hdmi_infoframe_type *info_frame, td_bool enable)
{
    errno_t ret;
    hdmi_infoframe     infoframe      = {0};
    hdmi_device        *hdmi_dev      = TD_NULL;
    hdmi_vo_attr       *vo_attr       = TD_NULL;
    hdmi_avi_infoframe *avi_infoframe = TD_NULL;

    hdmi_if_null_return(info_frame, OT_ERR_HDMI_NULL_PTR);
    if (enable == TD_FALSE) {
        drv_hdmi_infoframe_enable_set(info_frame, HDMI_INFOFRAME_TYPE_AVI, TD_FALSE);
        return TD_SUCCESS;
    }

    hdmi_dev = osal_container_of(info_frame, __typeof__(*hdmi_dev), info_frame);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    vo_attr  = &hdmi_dev->attr.vo_attr;

    avi_infoframe = &info_frame->avi_infoframe;
    ret = memset_s(avi_infoframe, sizeof(info_frame->avi_infoframe), 0, sizeof(hdmi_avi_infoframe));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    avi_infoframe->disp_fmt = vo_attr->disp_fmt;
    avi_infoframe->video_timing = vo_attr->video_timing;
    avi_infoframe->picture_aspect = vo_attr->picture_aspect;
    avi_infoframe->active_aspect = vo_attr->active_aspect;
    avi_infoframe->nups = vo_attr->picture_scaling;
    avi_infoframe->pixel_repeat = vo_attr->pixel_repeat;
    avi_infoframe->active_info_valid = TD_TRUE;
    avi_infoframe->extended_colorimetry = vo_attr->extended_colorimetry;
    avi_infoframe->quantization_range = vo_attr->rgb_quantization;
    avi_infoframe->ycc_quantization_range = vo_attr->ycc_quantization;
#if defined(HDMI_PRODUCT_SS626V100)
    if (vo_attr->disp_fmt == HDMI_VIDEO_FORMAT_7680X4320P_30) {
        avi_infoframe->colorspace  = HDMI_COLORSPACE_YCBCR420;
    } else {
        avi_infoframe->colorspace  = hdmi_dev->attr.app_attr.out_color_space;
    }
#else
    avi_infoframe->colorspace  = hdmi_dev->attr.app_attr.out_color_space;
#endif
    avi_infoframe->colorimetry = hdmi_dev->attr.app_attr.xvycc_mode ? HDMI_COLORIMETRY_EXTEND : vo_attr->colorimetry;

    ret = memcpy_s(&infoframe.avi_infoframe, sizeof(infoframe.avi_infoframe),
        avi_infoframe, sizeof(hdmi_avi_infoframe));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    drv_hdmi_infoframe_send(info_frame, HDMI_INFOFRAME_TYPE_AVI, &infoframe);

    return TD_SUCCESS;
}

static td_void drv_hdmi_vendor_fill_format(const hdmi_vo_attr *vo_attr, hdmi_14_vsif_content *h14_vsif_content)
{
    td_u32 cnt;
    td_u32 vic = drv_hdmi_vic_search(vo_attr->video_timing, vo_attr->picture_aspect, TD_FALSE);
    hdmi_video_4k_def *video = TD_NULL;

    if (((vic >= HDMI_3840X2160P24_16_9 && vic <= HDMI_3840X2160P30_16_9) ||
        vic == HDMI_4096X2160P24_256_135) && vo_attr->stereo_mode == HDMI_3D_BUTT) {
        h14_vsif_content->format = HDMI_VIDEO_FORMAT_4K;
        for (cnt = 0; cnt <= CEA861_F_VIDEO_CODES_MAX_4K; cnt++) {
            video = drv_hdmi_video_codes_4k_get(cnt);
            if (video != TD_NULL && video->timing == vo_attr->video_timing) {
                h14_vsif_content->vic = video->hdmi_vic;
                break;
            }
        }
    } else if (vo_attr->stereo_mode < HDMI_3D_BUTT) {
        h14_vsif_content->format = HDMI_VIDEO_FORMAT_3D;
    } else {
        h14_vsif_content->format = HDMI_VIDEO_FORMAT_NONE;
    }
    h14_vsif_content->_3d_structure = vo_attr->stereo_mode;
}

td_s32 drv_hdmi_vendor_infoframe_send(hdmi_infoframe_type *info_frame, td_bool enable)
{
    errno_t ret;
    hdmi_infoframe infoframe = {0};
    hdmi_vo_attr *vo_attr = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_vendor_infoframe *vsif = TD_NULL;
    hdmi_14_vsif_content *h14_vsif_content = TD_NULL;

    hdmi_if_null_return(info_frame, OT_ERR_HDMI_NULL_PTR);

    if (!enable) {
        drv_hdmi_infoframe_enable_set(info_frame, HDMI_INFOFRAME_TYPE_VENDOR, TD_FALSE);
        return TD_SUCCESS;
    }
    hdmi_dev = osal_container_of(info_frame, __typeof__(*hdmi_dev), info_frame);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    vo_attr = &hdmi_dev->attr.vo_attr;
    vsif = &info_frame->vendor_infoframe;
    (td_void)memset_s(vsif, sizeof(info_frame->vendor_infoframe), 0, sizeof(hdmi_vendor_infoframe));

    h14_vsif_content = &vsif->vsif_content.h14_vsif_content;
    h14_vsif_content->ieee = HDMI14_IEEE_OUI;
    drv_hdmi_vendor_fill_format(vo_attr, h14_vsif_content);
    vsif->length = VSIF_DEFAULT_DATA_LEN;
    /* add user vendor data */
    vsif->vsif_content.cea861_vsif.len = hdmi_dev->vendor_data.len;
    ret = memcpy_s(vsif->vsif_content.cea861_vsif.data, HDMI_VENDOR_USER_DATA_MAX_LEN,
        hdmi_dev->vendor_data.data, hdmi_dev->vendor_data.len);
    hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);
    ret = memcpy_s(&infoframe.vendor_infoframe, sizeof(infoframe.vendor_infoframe),
        vsif, sizeof(hdmi_vendor_infoframe));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    drv_hdmi_infoframe_send(info_frame, HDMI_INFOFRAME_TYPE_VENDOR, &infoframe);

    return TD_SUCCESS;
}

static td_void drv_hdmi_audio_fill_infoframe(const hdmi_ao_attr *ao_attr, hdmi_audio_infoframe *audio_infoframe)
{
    if (ao_attr->sound_intf == HDMI_AUDIO_INTF_SPDIF || ao_attr->sound_intf == HDMI_AUDIO_INTF_HBRA) {
        hdmi_info("audio channel refer to stream \n");
    } else {
        hdmi_info("audio channel %u \n", ao_attr->channels);
        audio_infoframe->channels = (ao_attr->channels >= 2) ? (ao_attr->channels - 1) : 0; /* 2, channels number */
    }

    if (ao_attr->audio_code == HDMI_AUDIO_CODING_TYPE_AC3 || ao_attr->audio_code == HDMI_AUDIO_CODING_TYPE_DTS ||
        ao_attr->audio_code == HDMI_AUDIO_CODING_TYPE_EAC3 || ao_attr->audio_code == HDMI_AUDIO_CODING_TYPE_DTS_HD) {
        audio_infoframe->coding_type = ao_attr->audio_code;
    } else {
        audio_infoframe->coding_type = HDMI_AUDIO_CODING_TYPE_STREAM;
    }
    hdmi_info("aduio code : %u \n", ao_attr->audio_code);

    switch (ao_attr->channels) {
        case HDMI_AUDIO_FORMAT_3CH:
            audio_infoframe->channel_allocation = 0x1; /* 1 channel */
            break;
        case HDMI_AUDIO_FORMAT_6CH:
            audio_infoframe->channel_allocation = 0xb; /* 3, 4 channel */
            break;
        case HDMI_AUDIO_FORMAT_8CH:
            audio_infoframe->channel_allocation = 0x13; /* 1, 2, 5 channel */
            break;
        default:
            audio_infoframe->channel_allocation = 0x0; /* 0 channel */
            break;
    }
}

td_s32 drv_hdmi_audio_infoframe_send(hdmi_infoframe_type *info_frame, td_bool enable)
{
    errno_t ret;
    hdmi_infoframe infoframe = {0};
    hdmi_ao_attr *ao_attr = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_audio_infoframe *audio_infoframe = TD_NULL;

    hdmi_if_null_return(info_frame, OT_ERR_HDMI_NULL_PTR);

    if (!enable) {
        drv_hdmi_infoframe_enable_set(info_frame, HDMI_INFOFRAME_TYPE_AUDIO, TD_FALSE);
        return TD_SUCCESS;
    }
    hdmi_dev = osal_container_of(info_frame, __typeof__(*hdmi_dev), info_frame);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    ao_attr = &hdmi_dev->attr.ao_attr;
    audio_infoframe = &info_frame->audio_infoframe;
    ret = memset_s(audio_infoframe, sizeof(info_frame->audio_infoframe), 0, sizeof(hdmi_audio_infoframe));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    drv_hdmi_audio_fill_infoframe(ao_attr, audio_infoframe);
    ret = memcpy_s(&infoframe.audio_infoframe, sizeof(infoframe.audio_infoframe),
        audio_infoframe, sizeof(hdmi_audio_infoframe));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    drv_hdmi_infoframe_send(info_frame, HDMI_INFOFRAME_TYPE_AUDIO, &infoframe);

    return TD_SUCCESS;
}
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif

