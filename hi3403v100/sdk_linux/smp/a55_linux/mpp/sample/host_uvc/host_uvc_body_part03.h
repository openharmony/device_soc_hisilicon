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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART03_H_

        return ret;
    }

    sample_uvc_print_format(dev, &format);

    return 0;
}

static td_s32 sample_uvc_video_set_frame_rate(device_info *dev, struct v4l2_fract *time_per_frame)
{
    struct v4l2_streamparm stream_parm = {0};
    td_s32 ret;

    stream_parm.type = dev->type;

    ret = ioctl(dev->fd, VIDIOC_G_PARM, &stream_parm);
    if (ret < 0) {
        sample_print("get frame rate failed!\n");
        return ret;
    }

    printf("Current frame rate: %u/%u\n", stream_parm.parm.capture.timeperframe.numerator,
        stream_parm.parm.capture.timeperframe.denominator);

    printf("Setting frame rate to: %u/%u\n", time_per_frame->numerator,
        time_per_frame->denominator);

    stream_parm.parm.capture.timeperframe.numerator = time_per_frame->numerator;
    stream_parm.parm.capture.timeperframe.denominator = time_per_frame->denominator;

    ret = ioctl(dev->fd, VIDIOC_S_PARM, &stream_parm);
    if (ret < 0) {
        sample_print("Unable to set frame rate.\n");
        return ret;
    }

    ret = ioctl(dev->fd, VIDIOC_G_PARM, &stream_parm);
    if (ret < 0) {
        sample_print("Unable to get frame rate.\n");
        return ret;
    }

    printf("Frame rate set: %u/%u\n", stream_parm.parm.capture.timeperframe.numerator,
        stream_parm.parm.capture.timeperframe.denominator);
    return 0;
}

static td_s32 sample_uvc_video_buffer_mmap(device_info *dev, buffer_info *buffer, struct v4l2_buffer *v4l2buf)
{
    td_u32 length;
    td_u32 offset;
    td_u32 i;
    td_s32 ret;

    (td_void)memset_s(buffer->mem, sizeof(buffer->mem), 0, sizeof(buffer->mem));
    for (i = 0; i < dev->num_planes; i++) {
        if (sample_uvc_video_is_mplane(dev)) {
            length = v4l2buf->m.planes[i].length;
            offset = v4l2buf->m.planes[i].m.mem_offset;
        } else {
            length = v4l2buf->length;
            offset = v4l2buf->m.offset;
        }

        buffer->mem[i] = mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, dev->fd, offset);
        if (buffer->mem[i] == MAP_FAILED) {
            sample_print("Unable to map buffer %u/%u\n", buffer->idx, i);
            goto fail;
        }

        buffer->size[i] = length;
        buffer->padding[i] = 0;

        printf("Buffer %u/%u mapped\n", buffer->idx, i);
    }

    return 0;
fail:
    for (i = 0; i < dev->num_planes; i++) {
        if (buffer->mem[i] == TD_NULL) {
            continue;
        }
        if (sample_uvc_video_is_mplane(dev)) {
            length = v4l2buf->m.planes[i].length;
            offset = v4l2buf->m.planes[i].m.mem_offset;
        } else {
            length = v4l2buf->length;
            offset = v4l2buf->m.offset;
        }
        ret = munmap(buffer->mem[i], length);
        if (ret != 0) {
            sample_print("munmap fail, errno %d\n", errno);
        }
        buffer->mem[i] = TD_NULL;
    }
    return -1;
}

static td_s32 sample_uvc_video_buffer_munmap(const device_info *dev, buffer_info *buffer)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < dev->num_planes; i++) {
        ret = munmap(buffer->mem[i], buffer->size[i]);
        if (ret < 0) {
            sample_print("Unable to unmap buffer %u/%u\n", buffer->idx, i);
        }

        buffer->mem[i] = TD_NULL;
    }

    return 0;
}

static td_s32 sample_uvc_video_buffer_alloc_userptr(device_info *dev, buffer_info *buffer,
    struct v4l2_buffer *v4l2buf, td_u32 offset, td_u32 padding)
{
    td_s32 page_size = getpagesize();
    td_u32 length;
    td_u32 i;
    td_s32 ret;

    (td_void)memset_s(buffer->mem, sizeof(buffer->mem), 0, sizeof(buffer->mem));
    for (i = 0; i < dev->num_planes; i++) {
        if (sample_uvc_video_is_mplane(dev)) {
            length = v4l2buf->m.planes[i].length;
        } else {
            length = v4l2buf->length;
        }

        ret = posix_memalign(&buffer->mem[i], page_size, length + offset + padding);
        if (ret != 0 || buffer->mem[i] == NULL) {
            sample_print("Unable to allocate buffer %u/%u (%d)\n", buffer->idx, i, ret);
            goto fail;
        }

        buffer->mem[i] += offset;
        buffer->size[i] = length;
        buffer->padding[i] = padding;

        printf("Buffer %u/%u allocated\n", buffer->idx, i);
    }

    return 0;
fail:
    for (i = 0; i < dev->num_planes; i++) {
        if (buffer->mem[i] != TD_NULL) {
            free(buffer->mem[i]);
            buffer->mem[i] = TD_NULL;
        }
    }
    return -1;
}

static td_void sample_uvc_video_buffer_free_userptr(const device_info *dev, buffer_info *buffer)
{
    td_u32 i;

    for (i = 0; i < dev->num_planes; i++) {
        free(buffer->mem[i]);
        buffer->mem[i] = TD_NULL;
    }
}

static td_void sample_uvc_video_buffer_fill_userptr(device_info *dev, buffer_info *buffer, struct v4l2_buffer *v4l2buf)
{
    td_u32 i;

    if (!sample_uvc_video_is_mplane(dev)) {
        v4l2buf->m.userptr = (td_ulong)(uintptr_t)buffer->mem[0];
        return;
    }

    for (i = 0; i < dev->num_planes; i++) {
        v4l2buf->m.planes[i].m.userptr = (td_ulong)(uintptr_t)buffer->mem[i];
    }
}

static td_void sample_uvc_get_ts_flags(td_u32 buf_flag, const td_char **ts_type, const td_char **ts_src)
{
    switch (buf_flag & V4L2_BUF_FLAG_TSTAMP_SRC_MASK) {
        case V4L2_BUF_FLAG_TSTAMP_SRC_SOE:
            *ts_src = "soe";
            break;
        case V4L2_BUF_FLAG_TSTAMP_SRC_EOF:
            *ts_src = "eof";
            break;
        default:
            *ts_src = "inv";
    }

    switch (buf_flag & V4L2_BUF_FLAG_TIMESTAMP_MASK) {
        case V4L2_BUF_FLAG_TIMESTAMP_COPY:
            *ts_type = "copy";
            break;
        case V4L2_BUF_FLAG_TIMESTAMP_MONOTONIC:
            *ts_type = "monotonic";
            break;
        case V4L2_BUF_FLAG_TIMESTAMP_UNKNOWN:
            *ts_type = "unknown";
            break;
        default:
            *ts_type = "inv";
    }
}

static td_s32 sample_uvc_map_buffer(device_info *dev, buffer_info *buffers,
    td_u32 count, td_u32 offset, td_u32 padding)
{
    td_u32 i;
    td_s32 ret;
    struct v4l2_buffer buffer;
    struct v4l2_plane planes[VIDEO_MAX_PLANES];

    for (i = 0; i < count; ++i) {
        const td_char *ts_type;
        const td_char *ts_source;

        (td_void)memset_s(&buffer, sizeof(buffer), 0, sizeof(buffer));
        (td_void)memset_s(planes, sizeof(planes), 0, sizeof(planes));

        buffer.index = i;
        buffer.type = dev->type;
        buffer.memory = dev->memtype;
        buffer.length = VIDEO_MAX_PLANES;
        buffer.m.planes = planes;

        ret = ioctl(dev->fd, VIDIOC_QUERYBUF, &buffer);
        if (ret < 0) {
            sample_print("Unable to query buffer %u\n", i);
            return ret;
        }
        sample_uvc_get_ts_flags(buffer.flags, &ts_type, &ts_source);
        printf("length: %u offset: %u timestamp type/source: %s/%s\n",
            buffer.length, buffer.m.offset, ts_type, ts_source);

        buffers[i].idx = i;

        if (dev->memtype == V4L2_MEMORY_USERPTR) {
            ret = sample_uvc_video_buffer_alloc_userptr(dev, &buffers[i], &buffer, offset, padding);
        } else if (dev->memtype == V4L2_MEMORY_MMAP) {
            ret = sample_uvc_video_buffer_mmap(dev, &buffers[i], &buffer);
        }

        if (ret < 0) {
            return ret;
        }
    }

    dev->timestamp_type = buffer.flags & V4L2_BUF_FLAG_TIMESTAMP_MASK;
    dev->buffers = buffers;
    dev->nbufs = count;

    return TD_SUCCESS;
}

static td_s32 sample_uvc_video_alloc_buffers(device_info *dev, td_s32 nbufs, td_u32 offset, td_u32 padding)
{
    struct v4l2_requestbuffers rb;
    buffer_info *buffers;
    td_s32 ret;

    (td_void)memset_s(&rb, sizeof(rb), 0, sizeof(rb));
    rb.count = nbufs;
    rb.type = dev->type;
    rb.memory = dev->memtype;

    ret = ioctl(dev->fd, VIDIOC_REQBUFS, &rb);
    if (ret < 0) {
        sample_print("Unable to request buffers.\n");
        return ret;
    }

    printf("%u buffers requested.\n", rb.count);

    buffers = calloc(rb.count, sizeof(buffer_info));
    if (buffers == TD_NULL) {
        return -ENOMEM;
    }
    ret = sample_uvc_map_buffer(dev, buffers, rb.count, offset, padding);
    if (ret != TD_SUCCESS) {
        free(buffers);
        return ret;
    }

    return 0;
}

static td_s32 sample_uvc_video_free_buffers(device_info *dev)
{
    struct v4l2_requestbuffers rb = {0};
    td_u32 i;
    td_s32 ret;

    if (dev->nbufs == 0) {
        return 0;
    }

    for (i = 0; i < dev->nbufs; ++i) {
        if (dev->memtype == V4L2_MEMORY_USERPTR) {
            sample_uvc_video_buffer_free_userptr(dev, &dev->buffers[i]);
        } else if (dev->memtype == V4L2_MEMORY_MMAP) {
            ret = sample_uvc_video_buffer_munmap(dev, &dev->buffers[i]);
            if (ret < 0) {
                return ret;
            }
        }
    }

    rb.count = 0;
    rb.type = dev->type;
    rb.memory = dev->memtype;

    ret = ioctl(dev->fd, VIDIOC_REQBUFS, &rb);
    if (ret < 0) {
        return ret;
    }

    sample_print("%u buffers released.\n", dev->nbufs);

    free(dev->buffers);
    dev->nbufs = 0;
    dev->buffers = TD_NULL;

    return 0;
}

static td_void sample_uvc_get_time(device_info *dev, struct v4l2_buffer *buf)
{
    if (sample_uvc_video_is_output(dev)) {
        buf->flags = dev->buffer_output_flags;
        if (dev->timestamp_type == V4L2_BUF_FLAG_TIMESTAMP_COPY) {
            struct timespec ts;

            clock_gettime(CLOCK_MONOTONIC, &ts);
            buf->timestamp.tv_sec = ts.tv_sec;
            buf->timestamp.tv_usec = ts.tv_nsec / 1000;  /* 1000: ratio */
        }
    }
}

static td_s32 sample_uvc_video_queue_buffer(device_info *dev, td_s32 index, enum buffer_fill_mode fill)
{
    struct v4l2_buffer buf = {0};
    struct v4l2_plane planes[VIDEO_MAX_PLANES] = {0};
    td_u32 i;

    buf.index = index;
    buf.type = dev->type;
    buf.memory = dev->memtype;

    sample_uvc_get_time(dev, &buf);

    if (sample_uvc_video_is_mplane(dev)) {
        buf.m.planes = planes;
        buf.length = dev->num_planes;
    }

    if (dev->memtype == V4L2_MEMORY_USERPTR) {
        if (sample_uvc_video_is_mplane(dev)) {
            for (i = 0; i < dev->num_planes; i++) {
                buf.m.planes[i].m.userptr = (td_ulong)(uintptr_t)dev->buffers[index].mem[i];
                buf.m.planes[i].length = dev->buffers[index].size[i];
            }
        } else {
            buf.m.userptr = (td_ulong)(uintptr_t)dev->buffers[index].mem[0];
            buf.length = dev->buffers[index].size[0];
        }
    }

    for (i = 0; i < dev->num_planes; i++) {
        if (sample_uvc_video_is_output(dev)) {
            if (sample_uvc_video_is_mplane(dev)) {
                buf.m.planes[i].bytesused = dev->patternsize[i];
            } else {
                buf.bytesused = dev->patternsize[i];
            }

            errno_t ret = memcpy_s(dev->buffers[buf.index].mem[i], dev->patternsize[i],
                dev->pattern[i], dev->patternsize[i]);
            if (ret != EOK) {
                sample_print("memcpy_s buffers mem fail %d\n", ret);
                return -1;
            }
        } else {
            if (fill & BUFFER_FILL_FRAME) {
                (td_void)memset_s(dev->buffers[buf.index].mem[i], dev->buffers[index].size[i],
                    0x55, dev->buffers[index].size[i]);
            }
            if (fill & BUFFER_FILL_PADDING) {
                (td_void)memset_s(dev->buffers[buf.index].mem[i] + dev->buffers[index].size[i],
                    dev->buffers[index].padding[i], 0x55, dev->buffers[index].padding[i]);
            }
        }
    }

    return ioctl(dev->fd, VIDIOC_QBUF, &buf);
}

static td_s32 sample_uvc_video_enable(device_info *dev, td_bool enable)
{
    td_s32 type = dev->type;
    td_s32 ret;

    ret = ioctl(dev->fd, enable ? VIDIOC_STREAMON : VIDIOC_STREAMOFF, &type);
    if (ret < 0) {
        sample_print("Unable to %s streaming.\n", enable ? "start" : "stop");
        return ret;
    }

    return 0;
}

static td_s32 sample_uvc_video_for_each_control(device_info *dev,
    td_s32(*callback)(device_info *dev, const struct v4l2_query_ext_ctrl *query))
{
    struct v4l2_query_ext_ctrl query_ext_ctrl;
    td_s32 nctrls = 0;
    td_u32 id;
    td_s32 ret;
    td_bool ctrl_remain = TD_TRUE;

    id = 0;
    while (ctrl_remain == TD_TRUE) {
        id |= V4L2_CTRL_FLAG_NEXT_CTRL | V4L2_CTRL_FLAG_NEXT_COMPOUND;

        ret = sample_uvc_query_ext_control(dev, id, &query_ext_ctrl);
        if (ret == -EINVAL) {
            ctrl_remain = TD_FALSE;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART03_H_ */
