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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART02_H_

    td_bool is_support_capture;
    td_bool is_support_output;
    td_bool is_support_mplane;
    td_s32 ret;

    (td_void)memset_s(&cap, sizeof(cap), 0, sizeof(cap));
    ret = ioctl(dev->fd, VIDIOC_QUERYCAP, &cap);
    if (ret < 0) {
        return 0;
    }

    caps = (cap.capabilities & V4L2_CAP_DEVICE_CAPS) ? cap.device_caps : cap.capabilities;

    is_support_video = caps & (V4L2_CAP_VIDEO_CAPTURE_MPLANE | V4L2_CAP_VIDEO_CAPTURE |
        V4L2_CAP_VIDEO_OUTPUT_MPLANE | V4L2_CAP_VIDEO_OUTPUT);
    is_support_meta = caps & (V4L2_CAP_META_CAPTURE);
    is_support_capture = caps & (V4L2_CAP_VIDEO_CAPTURE_MPLANE | V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_META_CAPTURE);
    is_support_output = caps & (V4L2_CAP_VIDEO_OUTPUT_MPLANE | V4L2_CAP_VIDEO_OUTPUT);
    is_support_mplane = caps & (V4L2_CAP_VIDEO_CAPTURE_MPLANE | V4L2_CAP_VIDEO_OUTPUT_MPLANE);

    printf("Device `%s' on `%s' (driver '%s') supports%s%s%s%s %s mplanes.\n", cap.card, cap.bus_info, cap.driver,
        is_support_video ? " video," : "", is_support_meta ? " meta-data," : "",
        is_support_capture ? " capture," : "", is_support_output ? " output," : "",
        is_support_mplane ? "with" : "without");

    *capabilities = caps;

    return 0;
}

static td_s32 sample_uvc_cap_get_buf_type(td_u32 capa)
{
    if (capa & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
        return V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    }

    if (capa & V4L2_CAP_VIDEO_OUTPUT_MPLANE) {
        return V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    }

    if (capa & V4L2_CAP_VIDEO_CAPTURE) {
        return  V4L2_BUF_TYPE_VIDEO_CAPTURE;
    }

    if (capa & V4L2_CAP_VIDEO_OUTPUT) {
        return V4L2_BUF_TYPE_VIDEO_OUTPUT;
    }

    if (capa & V4L2_CAP_META_CAPTURE) {
        return V4L2_BUF_TYPE_META_CAPTURE;
    }

    sample_print("Device supports neither capture nor output.\n");
    return -EINVAL;
}

static td_void sample_uvc_video_close(device_info *dev)
{
    td_u32 i;

    for (i = 0; i < dev->num_planes; i++) {
        if (dev->pattern[i] != TD_NULL) {
            free(dev->pattern[i]);
            dev->pattern[i] = TD_NULL;
        }
    }

    if (dev->buffers != TD_NULL) {
        free(dev->buffers);
        dev->buffers = TD_NULL;
    }

    if (dev->opened) {
        close(dev->fd);
    }
}

static td_void sample_uvc_video_log_status(device_info *dev)
{
    ioctl(dev->fd, VIDIOC_LOG_STATUS);
}

static td_s32 sample_uvc_query_control(device_info *dev, td_u32 id, struct v4l2_query_ext_ctrl *query_ext_ctrl)
{
    td_s32 ret;
    struct v4l2_queryctrl query_ctrl = {0};

    query_ctrl.id = id;

    ret = ioctl(dev->fd, VIDIOC_QUERYCTRL, &query_ctrl);
    if (ret < 0) {
        ret = -errno;
        sample_print("unable to query control 0x%8.8x\n", id);
        return ret;
    }

    (td_void)memset_s(query_ext_ctrl, sizeof(*query_ext_ctrl), 0, sizeof(*query_ext_ctrl));
    query_ext_ctrl->id = query_ctrl.id;
    query_ext_ctrl->type = query_ctrl.type;
    (td_void)memcpy_s(query_ext_ctrl->name, sizeof(query_ext_ctrl->name),
        query_ctrl.name, sizeof(query_ext_ctrl->name));
    query_ext_ctrl->minimum = query_ctrl.minimum;
    query_ext_ctrl->maximum = query_ctrl.maximum;
    query_ext_ctrl->step = query_ctrl.step;
    query_ext_ctrl->default_value = query_ctrl.default_value;
    query_ext_ctrl->flags = query_ctrl.flags;

    if (query_ctrl.type == V4L2_CTRL_TYPE_STRING && !(query_ctrl.flags & V4L2_CTRL_FLAG_HAS_PAYLOAD)) {
        query_ext_ctrl->elem_size = query_ctrl.maximum + 1;
        query_ext_ctrl->elems = 1;
    }

    return 0;
}

static td_s32 sample_uvc_query_ext_control(device_info *dev, td_u32 id, struct v4l2_query_ext_ctrl *query_ext_ctrl)
{
    struct v4l2_queryctrl query_ctrl = {0};
    td_s32 ret;

    (td_void)memset_s(query_ext_ctrl, sizeof(*query_ext_ctrl), 0, sizeof(*query_ext_ctrl));
    query_ext_ctrl->id = id;

    ret = ioctl(dev->fd, VIDIOC_QUERY_EXT_CTRL, query_ext_ctrl);
    if (ret < 0) {
        ret = -errno;
    }
    if (!ret || ret == -EINVAL) {
        return ret;
    }

    if (ret != -ENOTTY) {
        sample_print("unable to query control 0x%8.8x\n", id);
        return ret;
    }

    query_ctrl.id = id;

    ret = ioctl(dev->fd, VIDIOC_QUERYCTRL, &query_ctrl);
    if (ret < 0) {
        ret = -errno;
        sample_print("unable to query control 0x%8.8x\n", id);
        return ret;
    }

    (td_void)memset_s(query_ext_ctrl, sizeof(*query_ext_ctrl), 0, sizeof(*query_ext_ctrl));
    query_ext_ctrl->id = query_ctrl.id;
    query_ext_ctrl->type = query_ctrl.type;
    (td_void)memcpy_s(query_ext_ctrl->name, sizeof(query_ext_ctrl->name),
        query_ctrl.name, sizeof(query_ext_ctrl->name));
    query_ext_ctrl->minimum = query_ctrl.minimum;
    query_ext_ctrl->maximum = query_ctrl.maximum;
    query_ext_ctrl->step = query_ctrl.step;
    query_ext_ctrl->default_value = query_ctrl.default_value;
    query_ext_ctrl->flags = query_ctrl.flags;
    if (query_ctrl.type == V4L2_CTRL_TYPE_STRING && !(query_ctrl.flags & V4L2_CTRL_FLAG_HAS_PAYLOAD)) {
        query_ext_ctrl->elem_size = query_ctrl.maximum + 1;
        query_ext_ctrl->elems = 1;
    }
    return 0;
}

static td_s32 sample_uvc_get_control(device_info *dev, const struct v4l2_query_ext_ctrl *query_ext_ctrl,
    struct v4l2_ext_control *ctrl, td_u32 which)
{
    struct v4l2_ext_controls controls = {0};
    struct v4l2_control control;
    td_s32 ret;

    (td_void)memset_s(ctrl, sizeof(*ctrl), 0, sizeof(*ctrl));

    controls.which = which;
    controls.count = 1;
    controls.controls = ctrl;

    ctrl->id = query_ext_ctrl->id;

    if (query_ext_ctrl->flags & V4L2_CTRL_FLAG_HAS_PAYLOAD) {
        ctrl->size = query_ext_ctrl->elems * query_ext_ctrl->elem_size;
        ctrl->ptr = malloc(ctrl->size);
        if (ctrl->ptr == TD_NULL) {
            return -ENOMEM;
        }
    }

    ret = ioctl(dev->fd, VIDIOC_G_EXT_CTRLS, &controls);
    if (ret != -1) {
        return 0;
    }

    if (query_ext_ctrl->flags & V4L2_CTRL_FLAG_HAS_PAYLOAD) {
        free(ctrl->ptr);
        ctrl->ptr = TD_NULL;
    }

    if ((query_ext_ctrl->flags & V4L2_CTRL_FLAG_HAS_PAYLOAD) || (query_ext_ctrl->type == V4L2_CTRL_TYPE_INTEGER64) ||
        ((errno != EINVAL) && (errno != ENOTTY))) {
        return -errno;
    }

    control.id = query_ext_ctrl->id;
    ret = ioctl(dev->fd, VIDIOC_G_CTRL, &control);
    if (ret < 0) {
        return -errno;
    }

    ctrl->value = control.value;
    return 0;
}

static td_s32 sample_uvc_set_control(device_info *dev, const struct v4l2_query_ext_ctrl *query_ext_ctrl,
    struct v4l2_ext_control *ctrl)
{
    struct v4l2_ext_controls controls = {0};
    struct v4l2_control control;
    td_s32 ret;

    controls.ctrl_class = V4L2_CTRL_ID2CLASS(ctrl->id);
    controls.count = 1;
    controls.controls = ctrl;

    ctrl->id = query_ext_ctrl->id;

    ret = ioctl(dev->fd, VIDIOC_S_EXT_CTRLS, &controls);
    if (ret != -1) {
        return 0;
    }

    if ((query_ext_ctrl->flags & V4L2_CTRL_FLAG_HAS_PAYLOAD) || (query_ext_ctrl->type == V4L2_CTRL_TYPE_INTEGER64) ||
        ((errno != EINVAL) && (errno != ENOTTY))) {
        return -1;
    }

    control.id = ctrl->id;
    control.value = ctrl->value;
    ret = ioctl(dev->fd, VIDIOC_S_CTRL, &control);
    if (ret != -1) {
        ctrl->value = control.value;
    }

    return ret;
}

static td_s32 sample_uvc_video_get_format(device_info *dev)
{
    struct v4l2_format format = {0};
    td_u32 i;
    td_s32 ret;

    format.type = dev->type;

    ret = ioctl(dev->fd, VIDIOC_G_FMT, &format);
    if (ret < 0) {
        sample_print("get format failed!\n");
        return ret;
    }

    if (sample_uvc_video_is_mplane(dev)) {
        dev->width = format.fmt.pix_mp.width;
        dev->height = format.fmt.pix_mp.height;
        dev->num_planes = format.fmt.pix_mp.num_planes;

        printf("video format: %s (%08x) %ux%u field %s, %u planes: \n",
            sample_uvc_v4l2_format_name(format.fmt.pix_mp.pixelformat), format.fmt.pix_mp.pixelformat,
            format.fmt.pix_mp.width, format.fmt.pix_mp.height,
            sample_uvc_v4l2_field_name(format.fmt.pix_mp.field),
            format.fmt.pix_mp.num_planes);

        for (i = 0; i < format.fmt.pix_mp.num_planes; i++) {
            dev->plane_fmt[i].bytesperline = format.fmt.pix_mp.plane_fmt[i].bytesperline;
            dev->plane_fmt[i].sizeimage = format.fmt.pix_mp.plane_fmt[i].bytesperline ?
                format.fmt.pix_mp.plane_fmt[i].sizeimage : 0;

            printf(" * stride %u, buffer size %u\n", format.fmt.pix_mp.plane_fmt[i].bytesperline,
                format.fmt.pix_mp.plane_fmt[i].sizeimage);
        }
    } else if (sample_uvc_video_is_meta(dev)) {
        dev->width = 0;
        dev->height = 0;
        dev->num_planes = 1;

        printf("meta-data format: %s (%08x) buffer size %u\n",
            sample_uvc_v4l2_format_name(format.fmt.meta.dataformat), format.fmt.meta.dataformat,
            format.fmt.meta.buffersize);
    } else {
        dev->width = format.fmt.pix.width;
        dev->height = format.fmt.pix.height;
        dev->num_planes = 1;

        dev->plane_fmt[0].bytesperline = format.fmt.pix.bytesperline;
        dev->plane_fmt[0].sizeimage = format.fmt.pix.bytesperline ? format.fmt.pix.sizeimage : 0;

        printf("video format: %s (%08x) %ux%u (stride %u) field %s buffer size %u\n",
            sample_uvc_v4l2_format_name(format.fmt.pix.pixelformat), format.fmt.pix.pixelformat,
            format.fmt.pix.width, format.fmt.pix.height, format.fmt.pix.bytesperline,
            sample_uvc_v4l2_field_name(format.fmt.pix_mp.field),
            format.fmt.pix.sizeimage);
    }

    return 0;
}

static td_void sample_uvc_print_format(device_info *dev, const struct v4l2_format *fmt)
{
    td_u32 i;

    if (sample_uvc_video_is_mplane(dev)) {
        printf("video format set: %s (%08x) %ux%u field %s, %u planes: \n",
            sample_uvc_v4l2_format_name(fmt->fmt.pix_mp.pixelformat), fmt->fmt.pix_mp.pixelformat,
            fmt->fmt.pix_mp.width, fmt->fmt.pix_mp.height,
            sample_uvc_v4l2_field_name(fmt->fmt.pix_mp.field),
            fmt->fmt.pix_mp.num_planes);

        for (i = 0; i < fmt->fmt.pix_mp.num_planes; i++) {
            printf(" * stride %u, buffer size %u\n",
                fmt->fmt.pix_mp.plane_fmt[i].bytesperline,
                fmt->fmt.pix_mp.plane_fmt[i].sizeimage);
        }
    } else if (sample_uvc_video_is_meta(dev)) {
        printf("meta-data format: %s (%08x) buffer size %u\n",
            sample_uvc_v4l2_format_name(fmt->fmt.meta.dataformat), fmt->fmt.meta.dataformat,
            fmt->fmt.meta.buffersize);
    } else {
        printf("video format set: %s (%08x) %ux%u (stride %u) field %s buffer size %u\n",
            sample_uvc_v4l2_format_name(fmt->fmt.pix.pixelformat), fmt->fmt.pix.pixelformat,
            fmt->fmt.pix.width, fmt->fmt.pix.height, fmt->fmt.pix.bytesperline,
            sample_uvc_v4l2_field_name(fmt->fmt.pix.field),
            fmt->fmt.pix.sizeimage);
    }
}

static td_s32 sample_uvc_get_device_size(device_info *dev, td_u32 pixelformat, enum v4l2_buf_type type)
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

        g_device_size[i].width = frame.discrete.width;
        g_device_size[i].height = frame.discrete.height;
        g_device_size_num++;
    }

    if (i >= DEVICE_UVC_SIZE_NUM) {
        sample_print("DEVICE_UVC_SIZE_NUM(%d) is less than %u, change it's value by yourself!\n",
            DEVICE_UVC_SIZE_NUM, i);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_uvc_check_video_size(device_info *dev, td_u32 pixelformat, td_u32 width, td_u32 height)
{
    td_u32 i;
    td_s32 ret;

    if (pixelformat == V4L2_PIX_FMT_NV21 || pixelformat == V4L2_PIX_FMT_NV12) {
        return TD_SUCCESS;
    }

    ret = sample_uvc_get_device_size(dev, pixelformat, V4L2_BUF_TYPE_VIDEO_CAPTURE);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    for (i = 0; i < g_device_size_num; i++) {
        if ((width == g_device_size[i].width) && (height == g_device_size[i].height)) {
            return TD_SUCCESS;
        }
    }

    sample_print("usb device not support this video size(w:%u h:%u) or not support this format!\n", width, height);
    return TD_FAILURE;
}

static td_s32 sample_uvc_video_set_format(device_info *dev, uvc_ctrl_info *ctrl_info)
{
    struct v4l2_format format = {0};
    td_u32 i;
    td_s32 ret;

    format.type = dev->type;

    if (sample_uvc_video_is_mplane(dev)) {
        const format_info *info = sample_uvc_v4l2_format_by_fourcc(ctrl_info->pixelformat);

        format.fmt.pix_mp.width = ctrl_info->width;
        format.fmt.pix_mp.height = ctrl_info->height;
        format.fmt.pix_mp.pixelformat = ctrl_info->pixelformat;
        format.fmt.pix_mp.field = ctrl_info->field;
        format.fmt.pix_mp.num_planes = info->n_planes;
        format.fmt.pix_mp.flags = ctrl_info->fmt_flags;

        for (i = 0; i < format.fmt.pix_mp.num_planes; i++) {
            format.fmt.pix_mp.plane_fmt[i].bytesperline = ctrl_info->stride;
            format.fmt.pix_mp.plane_fmt[i].sizeimage = ctrl_info->buffer_size;
        }
    } else if (sample_uvc_video_is_meta(dev)) {
        format.fmt.meta.dataformat = ctrl_info->pixelformat;
        format.fmt.meta.buffersize = ctrl_info->buffer_size;
    } else {
        format.fmt.pix.width = ctrl_info->width;
        format.fmt.pix.height = ctrl_info->height;
        format.fmt.pix.pixelformat = ctrl_info->pixelformat;
        format.fmt.pix.field = ctrl_info->field;
        format.fmt.pix.bytesperline = ctrl_info->stride;
        format.fmt.pix.sizeimage = ctrl_info->buffer_size;
        format.fmt.pix.priv = V4L2_PIX_FMT_PRIV_MAGIC;
        format.fmt.pix.flags = ctrl_info->fmt_flags;
    }

    ret = sample_uvc_check_video_size(dev, ctrl_info->pixelformat, format.fmt.pix.width, format.fmt.pix.height);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = ioctl(dev->fd, VIDIOC_S_FMT, &format);
    if (ret < 0) {
        sample_print("set format failed!\n");


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART02_H_ */
