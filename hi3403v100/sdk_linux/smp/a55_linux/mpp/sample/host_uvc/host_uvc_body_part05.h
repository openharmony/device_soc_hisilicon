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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART05_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART05_H_

        sample_print("%d control%s reset.\n", ret, ret > 1 ? "s" : "");
    }
}

static td_void sample_uvc_video_send_extension(device_info *dev, td_u32 id, const td_char *val)
{
    td_u8 data[64] = {0};
    td_u32 len;
    struct uvc_xu_control_query ext = {
        .unit = id,
        .query = UVC_GET_LEN,
        .selector = (td_u8)strtol(val, NULL, 0),
        .size = 0x2,
        .data = data,
    };
    if (ioctl(dev->fd, UVCIOC_CTRL_QUERY, &ext) != 0) {
        sample_print("Invalid Unit '%u'? or Invalid channel '%u'?\n", id, ext.selector);
        return;
    }

    (td_void)memcpy_s(&len, sizeof(len), data, sizeof(len));
    for (td_u32 i = 0; i < len; i++) {
        data[i] = (td_u8)i;
    }

    ext.query = UVC_SET_CUR;
    ext.size = (td_u16)len;
    if (ioctl(dev->fd, UVCIOC_CTRL_QUERY, &ext) != 0) {
        sample_print("send extension cmd (unit:%u channel:%u) fail.\n", id, ext.selector);
        return;
    } else {
        sample_print("send extension cmd (unit:%u channel:%u) success.\n", id, ext.selector);
    }

    return;
}

static td_void sample_uvc_video_enum_frame_intervals(device_info *dev, td_u32 pixel_format,
    td_u32 width, td_u32 height)
{
    struct v4l2_frmivalenum frmivalenum;
    td_u32 i;
    td_s32 ret;

    for (i = 0; ; ++i) {
        (td_void)memset_s(&frmivalenum, sizeof(frmivalenum), 0, sizeof(frmivalenum));
        frmivalenum.index = i;
        frmivalenum.pixel_format = pixel_format;
        frmivalenum.width = width;
        frmivalenum.height = height;
        ret = ioctl(dev->fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmivalenum);
        if (ret < 0) {
            break;
        }

        if (i != frmivalenum.index) {
            sample_print("Warning: driver returned wrong ival index %u.\n", frmivalenum.index);
        }
        if (pixel_format != frmivalenum.pixel_format) {
            sample_print("Warning: driver returned wrong ival pixel format %08x.\n", frmivalenum.pixel_format);
        }
        if (width != frmivalenum.width) {
            sample_print("Warning: driver returned wrong ival width %u.\n", frmivalenum.width);
        }
        if (height != frmivalenum.height) {
            sample_print("Warning: driver returned wrong ival height %u.\n", frmivalenum.height);
        }

        if (i != 0) {
            printf(", ");
        }

        switch (frmivalenum.type) {
            case V4L2_FRMIVAL_TYPE_DISCRETE:
                printf("%u/%u", frmivalenum.discrete.numerator, frmivalenum.discrete.denominator);
                break;

            case V4L2_FRMIVAL_TYPE_CONTINUOUS:
                printf("%u/%u - %u/%u", frmivalenum.stepwise.min.numerator, frmivalenum.stepwise.min.denominator,
                    frmivalenum.stepwise.max.numerator, frmivalenum.stepwise.max.denominator);
                return;

            case V4L2_FRMIVAL_TYPE_STEPWISE:
                printf("%u/%u - %u/%u (by %u/%u)", frmivalenum.stepwise.min.numerator,
                    frmivalenum.stepwise.min.denominator, frmivalenum.stepwise.max.numerator,
                    frmivalenum.stepwise.max.denominator, frmivalenum.stepwise.step.numerator,
                    frmivalenum.stepwise.step.denominator);
                return;

            default:
                break;
        }
    }
}

static td_void sample_uvc_video_enum_frame_sizes(device_info *dev, td_u32 pixelformat)
{
    struct v4l2_frmsizeenum frame;
    td_u32 i;
    td_s32 ret;

    for (i = 0; ; ++i) {
        (td_void)memset_s(&frame, sizeof(frame), 0, sizeof(frame));
        frame.index = i;
        frame.pixel_format = pixelformat;
        ret = ioctl(dev->fd, VIDIOC_ENUM_FRAMESIZES, &frame);
        if (ret < 0) {
            break;
        }

        if (i != frame.index) {
            sample_print("Warning: driver returned wrong frame index %u.\n", frame.index);
        }
        if (pixelformat != frame.pixel_format) {
            sample_print("Warning: driver returned wrong frame pixel format %08x.\n", frame.pixel_format);
        }

        switch (frame.type) {
            case V4L2_FRMSIZE_TYPE_DISCRETE:
                printf("\tFrame size: %ux%u (", frame.discrete.width, frame.discrete.height);
                sample_uvc_video_enum_frame_intervals(dev, frame.pixel_format, frame.discrete.width,
                    frame.discrete.height);
                printf(")\n");
                break;

            case V4L2_FRMSIZE_TYPE_CONTINUOUS:
                printf("\tFrame size: %ux%u - %ux%u (", frame.stepwise.min_width, frame.stepwise.min_height,
                    frame.stepwise.max_width, frame.stepwise.max_height);
                sample_uvc_video_enum_frame_intervals(dev, frame.pixel_format,
                    frame.stepwise.max_width, frame.stepwise.max_height);
                printf(")\n");
                break;

            case V4L2_FRMSIZE_TYPE_STEPWISE:
                printf("\tFrame size: %ux%u - %ux%u (by %ux%u) (\n",
                    frame.stepwise.min_width, frame.stepwise.min_height,
                    frame.stepwise.max_width, frame.stepwise.max_height,
                    frame.stepwise.step_width, frame.stepwise.step_height);
                sample_uvc_video_enum_frame_intervals(dev, frame.pixel_format,
                    frame.stepwise.max_width, frame.stepwise.max_height);
                printf(")\n");
                break;

            default:
                break;
        }
    }
}

static td_void sample_uvc_video_enum_formats(device_info *dev, enum v4l2_buf_type type)
{
    struct v4l2_fmtdesc fmt;
    td_u32 i;
    td_s32 ret;

    for (i = 0; ; ++i) {
        (td_void)memset_s(&fmt, sizeof(fmt), 0, sizeof(fmt));
        fmt.index = i;
        fmt.type = type;
        ret = ioctl(dev->fd, VIDIOC_ENUM_FMT, &fmt);
        if (ret < 0) {
            break;
        }

        if (i != fmt.index) {
            sample_print("Warning: driver returned wrong format index %u.\n", fmt.index);
        }
        if (type != fmt.type) {
            sample_print("Warning: driver returned wrong format type %u.\n", fmt.type);
        }

        printf("\tFormat %u: %s (%08x)\n", i, sample_uvc_v4l2_format_name(fmt.pixelformat), fmt.pixelformat);
        printf("\tType: %s (%u)\n", sample_uvc_v4l2_buf_type_name(fmt.type), fmt.type);
        printf("\tName: %.32s\n", fmt.description);
        sample_uvc_video_enum_frame_sizes(dev, fmt.pixelformat);
        printf("\n");
    }
}

static td_void sample_uvc_video_enum_inputs(device_info *dev)
{
    struct v4l2_input input;
    td_u32 i;
    td_s32 ret;

    for (i = 0; ; ++i) {
        (td_void)memset_s(&input, sizeof(input), 0, sizeof(input));
        input.index = i;
        ret = ioctl(dev->fd, VIDIOC_ENUMINPUT, &input);
        if (ret < 0) {
            break;
        }

        if (i != input.index) {
            sample_print("Warning: driver returned wrong input index %u.\n", input.index);
        }

        sample_print("\tInput %u: %s.\n", i, input.name);
    }

    sample_print("\n");
}

static td_s32 sample_uvc_video_get_input(device_info *dev)
{
    td_s32 input;
    td_s32 ret;

    ret = ioctl(dev->fd, VIDIOC_G_INPUT, &input);
    if (ret < 0) {
        sample_print("Unable to get current input.\n");
        return ret;
    }

    return input;
}

static td_s32 sample_uvc_video_set_input(device_info *dev, td_s32 input)
{
    td_s32 _input = input;
    td_s32 ret;

    ret = ioctl(dev->fd, VIDIOC_S_INPUT, &_input);
    if (ret < 0) {
        sample_print("Unable to select input %d\n", input);
    }

    return ret;
}

static td_s32 sample_uvc_video_set_quality(device_info *dev, td_u32 quality)
{
    struct v4l2_jpegcompression jpeg;
    td_s32 ret;

    if (quality == (td_u32)(-1)) {
        return 0;
    }

    (td_void)memset_s(&jpeg, sizeof(jpeg), 0, sizeof(jpeg));
    jpeg.quality = quality;

    ret = ioctl(dev->fd, VIDIOC_S_JPEGCOMP, &jpeg);
    if (ret < 0) {
        sample_print("Unable to set quality to %u\n", quality);
        return ret;
    }

    ret = ioctl(dev->fd, VIDIOC_G_JPEGCOMP, &jpeg);
    if (ret >= 0) {
        sample_print("Quality set to %d\n", jpeg.quality);
    }

    return 0;
}

static void sample_uvc_free_dev_pattern(device_info *dev)
{
    for (td_u32 i = 0; i < dev->num_planes; i++) {
        if (dev->pattern[i] != TD_NULL) {
            free(dev->pattern[i]);
            dev->pattern[i] = TD_NULL;
        }
    }
}

static td_s32 sample_uvc_video_load_test_pattern(device_info *dev, const td_char *file_name)
{
    td_u32 size;
    td_s32 fd = -1;
    td_s32 ret;

    if (file_name != TD_NULL) {
        fd = open_by_real_path(file_name, O_RDONLY);
        if (fd == -1) {
            sample_print("Unable to open test pattern file '%s'\n", file_name);
            return -errno;
        }
    }

    (td_void)memset_s(dev->pattern, sizeof(dev->pattern), 0, sizeof(dev->pattern));
    for (td_u32 i = 0; i < dev->num_planes; i++) {
        size = dev->buffers[0].size[i];
        dev->pattern[i] = malloc(size);
        if (dev->pattern[i] == TD_NULL) {
            ret = -ENOMEM;
            goto fail;
        }

        if (file_name != TD_NULL) {
            ret = read(fd, dev->pattern[i], size);
            if (ret != (td_s32)size && dev->plane_fmt[i].bytesperline != 0) {
                sample_print("Test pattern file size %d doesn't match image size %u\n", ret, size);
                ret = -EINVAL;
                goto fail;
            }
        } else {
            uint8_t *data = dev->pattern[i];
            if (dev->plane_fmt[i].bytesperline == 0) {
                sample_print("Compressed format detected for plane %u, test pattern not generated automatically\n", i);
                ret = -EINVAL;
                goto fail;
            }

            for (td_u32 j = 0; j < dev->plane_fmt[i].sizeimage; ++j) {
                *data++ = j;
            }
        }

        dev->patternsize[i] = size;
    }

    ret = TD_SUCCESS;
    goto done;
fail:
    sample_uvc_free_dev_pattern(dev);
done:
    close(fd);
    return ret;
}

static td_s32 sample_uvc_video_prepare_capture(device_info *dev, td_s32 nbufs, td_u32 offset,
    const td_char *filename, enum buffer_fill_mode fill)
{
    td_u32 padding;
    td_s32 ret;

    padding = (fill & BUFFER_FILL_PADDING) ? 4096 : 0;  /* 4096:padding */
    if ((ret = sample_uvc_video_alloc_buffers(dev, nbufs, offset, padding)) < 0) {
        return ret;
    }

    if (sample_uvc_video_is_output(dev)) {
        ret = sample_uvc_video_load_test_pattern(dev, filename);
        if (ret < 0) {
            return ret;
        }
    }

    return 0;
}

static td_s32 sample_uvc_video_queue_all_buffers(device_info *dev, enum buffer_fill_mode fill)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < dev->nbufs; ++i) {
        ret = sample_uvc_video_queue_buffer(dev, i, fill);
        if (ret < 0) {
            return ret;
        }
    }

    return 0;
}

static td_void sample_uvc_video_verify_buffer(device_info *dev, struct v4l2_buffer *buf)
{
    buffer_info *buffer = &dev->buffers[buf->index];
    td_u32 plane;
    td_u32 i;

    for (plane = 0; plane < dev->num_planes; ++plane) {
        const uint8_t *data = buffer->mem[plane] + buffer->size[plane];
        td_u32 errors = 0;
        td_u32 dirty = 0;
        td_u32 length;

        if (sample_uvc_video_is_mplane(dev)) {
            length = buf->m.planes[plane].bytesused;
        } else {
            length = buf->bytesused;
        }

        if (dev->plane_fmt[plane].sizeimage && dev->plane_fmt[plane].sizeimage != length) {
            if (buffer->padding[plane] == 0) {
                continue;
            }
        }
        for (i = 0; i < buffer->padding[plane]; ++i) {
            if (data[i] != 0x55) {
                errors++;
                dirty = i + 1;
            }
        }

        if (errors == 0) {
            continue;
        }

        sample_print("Warning: %u bytes overwritten among %u first padding bytes for plane %u\n", errors, dirty, plane);

        dirty = (dirty + 15) & ~15;         /* 15:align */
        dirty = dirty > 32 ? 32 : dirty;    /* 32:limit */

        for (i = 0; i < dirty; ++i) {
            printf("%02x ", data[i]);
            if (i % 16 == 15) {             /* 15:end, 16:circle */
                printf("\n");
            }
        }
    }
}


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART05_H_ */
