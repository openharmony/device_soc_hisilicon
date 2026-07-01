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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART06_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART06_H_

#if (MEDIA_WORK == 1)
static td_void sample_uvc_video_save_image(device_info *dev, struct v4l2_buffer *buf,
    td_u32 sequence, uvc_ctrl_info *ctrl_info)
{
    td_u32 i;
    ot_size pic_size;
    td_u32 stride;
    const td_char *type_name = ctrl_info->type_name;

    pic_size.width = ctrl_info->width;
    pic_size.height = ctrl_info->height;
    stride = dev->plane_fmt[0].bytesperline;

    for (i = 0; i < dev->num_planes; i++) {
        td_void *data = dev->buffers[buf->index].mem[i];
        td_u32 length;

        if (sample_uvc_video_is_mplane(dev)) {
            length = buf->m.planes[i].bytesused;

            if (!dev->write_data_prefix) {
                data += buf->m.planes[i].data_offset;
                length -= buf->m.planes[i].data_offset;
            }
        } else {
            length = buf->bytesused;
        }

        if (sample_uvc_media_send_data(data, length, stride, &pic_size, type_name) != TD_SUCCESS) {
            sample_print("media_send_data failed!\n");
        }
    }
}
#else
static td_void sample_uvc_video_save_image(device_info *dev, struct v4l2_buffer *buf,
    td_u32 sequence, uvc_ctrl_info *ctrl_info)
{
    td_u32 size;
    td_u32 j;
    td_char *filename;
    const td_char *p;
    td_bool append;
    td_s32 ret = 0;
    td_s32 fd;
    const td_char *pattern = ctrl_info->pattern;

    size = strlen(pattern) + 12;    /* 12:margin */
    filename = malloc(size);
    if (filename == TD_NULL) {
        return;
    }

    p = strchr(pattern, '#');
    if (p != TD_NULL) {
        (td_void)sprintf_s(filename, size, "%.*s%06u%s", (td_s32)(p - pattern), pattern, sequence, p + 1);
        append = TD_FALSE;
    } else {
        (td_void)strcpy_s(filename, size, pattern);
        append = TD_TRUE;
    }

    fd = open(filename, O_CREAT | O_WRONLY | (append ? O_APPEND : O_TRUNC),
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    free(filename);
    if (fd == -1) {
        return;
    }

    for (j = 0; j < dev->num_planes; j++) {
        td_void *data = dev->buffers[buf->index].mem[j];
        td_u32 len;

        if (sample_uvc_video_is_mplane(dev)) {
            len = buf->m.planes[j].bytesused;

            if (!dev->write_data_prefix) {
                data += buf->m.planes[j].data_offset;
                len -= buf->m.planes[j].data_offset;
            }
        } else {
            len = buf->bytesused;
        }

        ret = write(fd, data, len);
        if (ret < 0) {
            sample_print("write error\n");
            break;
        } else if (ret != (td_s32)len) {
            sample_print("write error: only %d bytes written instead of %u\n", ret, len);
        }
    }
    close(fd);
}
#endif

static td_void sample_uvc_get_buf_cfg(device_info *dev, struct v4l2_plane planes[], td_u32 len, struct v4l2_buffer *buf)
{
    ot_unused(len);

    (td_void)memset_s(buf, sizeof(struct v4l2_buffer), 0, sizeof(struct v4l2_buffer));

    buf->type = dev->type;
    buf->memory = dev->memtype;
    buf->length = VIDEO_MAX_PLANES;
    buf->m.planes = planes;
}

static td_s32 sample_uvc_dqueue_buf(device_info *dev, td_u32 index, struct v4l2_buffer *buf)
{
    if (ioctl(dev->fd, VIDIOC_DQBUF, buf) < 0) {
        if (errno != EIO) {
            return TD_FAILURE;
        }
        buf->type = dev->type;
        buf->memory = dev->memtype;
        if (dev->memtype == V4L2_MEMORY_USERPTR) {
            sample_uvc_video_buffer_fill_userptr(dev, &dev->buffers[index], buf);
        }
    }
    return TD_SUCCESS;
}

static td_void sample_uvc_print_fps(struct timespec *start, td_u32 size, td_u32 frame_num, struct timespec *ts)
{
    td_double bps;
    td_double fps;

    ts->tv_sec -= start->tv_sec;
    ts->tv_nsec -= start->tv_nsec;
    if (ts->tv_nsec < 0) {
        ts->tv_sec--;
        ts->tv_nsec += 1000000000;  /* 1000000000:time */
    }

    bps = size / (ts->tv_nsec / 1000.0 + 1000000.0 * ts->tv_sec) * 1000000.0;       /* 1000.0,1000000.0:time */
    fps = frame_num / (ts->tv_nsec / 1000.0 + 1000000.0 * ts->tv_sec) * 1000000.0;  /* 1000.0,1000000.0:time */

    sample_print("Captured %u frames in %ld.%06ld seconds (%f fps, %f B/s).\n",
        frame_num, ts->tv_sec, ts->tv_nsec / 1000, fps, bps);                       /* 1000:time */
}

static td_s32 sample_uvc_capture(device_info *dev, uvc_ctrl_info *ctrl_info,
    struct timespec *ts, td_u32 *frame_num, td_u32 *size)
{
    td_u32 i;
    td_s32 ret;
    struct v4l2_plane planes[VIDEO_MAX_PLANES];
    struct v4l2_buffer buf;
    td_u32 skip = ctrl_info->skip;

    for (i = 0; i < ctrl_info->nframes; ++i) {
        if (g_uvc_exit == TD_TRUE) {
            break;
        }

        (td_void)memset_s(planes, sizeof(planes), 0, sizeof(planes));

        sample_uvc_get_buf_cfg(dev, planes, VIDEO_MAX_PLANES, &buf);

        ret = sample_uvc_dqueue_buf(dev, i, &buf);
        if (ret != TD_SUCCESS) {
            return TD_FAILURE;
        }

        if (sample_uvc_video_is_capture(dev)) {
            sample_uvc_video_verify_buffer(dev, &buf);
        }

        *size += buf.bytesused;

        clock_gettime(CLOCK_MONOTONIC, ts);

        if (sample_uvc_video_is_capture(dev) && ctrl_info->pattern && skip == 0) {
            sample_uvc_video_save_image(dev, &buf, i, ctrl_info);
        }

        if (skip > 0) {
            --skip;
        }

        if (ctrl_info->delay > 0) {
            usleep(ctrl_info->delay * 1000);    /* 1000:time */
        }

        (td_void)fflush(stdout);

        if (ctrl_info->pause == i + 1) {
            sample_uvc_pause_wait();
        }

        if (i >= ctrl_info->nframes - dev->nbufs && !ctrl_info->do_requeue_last) {
            continue;
        }

        ret = sample_uvc_video_queue_buffer(dev, buf.index, ctrl_info->fill);
        if (ret < 0) {
            sample_print("Unable to requeue buffer.\n");
            return TD_FAILURE;
        }
    }

    *frame_num = i;

    return TD_SUCCESS;
}

static td_s32 sample_uvc_video_do_capture(device_info *dev, uvc_ctrl_info *ctrl_info)
{
    struct timespec start;
    struct timespec ts;
    td_u32 size;
    td_s32 ret;
    td_u32 frame_num;

    if (ctrl_info->pause == 0) {
        sample_uvc_pause_wait();
    }

    ret = sample_uvc_video_enable(dev, TD_TRUE);
    if (ret < 0) {
        goto done;
    }

    if (ctrl_info->do_queue_late) {
        sample_uvc_video_queue_all_buffers(dev, ctrl_info->fill);
    }

    size = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);

    ret = sample_uvc_capture(dev, ctrl_info, &ts, &frame_num, &size);
    if (ret != TD_SUCCESS) {
        goto done;
    }

    ret = sample_uvc_video_enable(dev, TD_FALSE);
    if (ret < 0) {
        return ret;
    }

    if (ctrl_info->nframes == 0) {
        sample_print("No frames captured.\n");
        goto done;
    }

    if (ts.tv_sec == start.tv_sec && ts.tv_nsec == start.tv_nsec) {
        sample_print("Captured %u frames (%u bytes) 0 seconds\n", frame_num, size);
        goto done;
    }

    sample_uvc_print_fps(&start, size, frame_num, &ts);

done:
    return sample_uvc_video_free_buffers(dev);
}

static td_void sample_uvc_usage(const td_char *argv0)
{
    printf("sample_uvc_usage: %s device [options]\n", argv0);
    printf("supported options:\n");
    printf("-f, --format format             set the video format\n");
    printf("-F, --file[=name]               write file\n");
    printf("-h, --help                      show help info\n");
    printf("-s, --size WxH                  set the frame size (eg. 1920x1080)\n\n");
    printf("inquire USB device format: ./sample_uvc /dev/video0 --enum-formats\n\n");
    printf("example of setting USB device format:\n");
    printf("    ./sample_uvc /dev/video0 -fH264  -s1920x1080 -Ftest.h264\n");
    printf("    ./sample_uvc /dev/video0 -fH265  -s1920x1080 -Ftest.h265\n");
    printf("    ./sample_uvc /dev/video0 -fMJPEG -s1920x1080 -Ftest.mjpg\n");
    printf("    ./sample_uvc /dev/video0 -fYUYV  -s1920x1080 -Ftest.yuv\n");
    printf("    ./sample_uvc /dev/video0 -fNV21  -s640x360   -Ftest.yuv\n\n");
    printf("note: set macro MEDIA_WORK to 0 to write file on disk.\n\n");
}

#if (MEDIA_WORK == 1)
static const td_char *sample_uvc_video_get_data_type(device_info *dev)
{
    struct v4l2_format fmt;
    td_s32 ret;
    (td_void)memset_s(&fmt, sizeof(fmt), 0, sizeof(fmt));
    fmt.type = dev->type;

    ret = ioctl(dev->fd, VIDIOC_G_FMT, &fmt);
    if (ret < 0) {
        sample_print("Unable to get format.\n");
        return TD_NULL;
    }

    return sample_uvc_v4l2_format_name(fmt.fmt.pix_mp.pixelformat);
}
#endif

static td_s32 sample_uvc_get_opt(td_s32 argc, td_char *argv[], device_info *dev, uvc_ctrl_info *ctrl_info)
{
    td_s32 c;
    td_char *endptr;

    while ((c = getopt_long(argc, argv, "B:c::Cd:f:F::hi:Iln:p::q:r:R::s:t:uw:x:", g_opts, TD_NULL)) != -1) {
        switch (c) {
            case 'f':
                if (strcmp("help", optarg) == 0) {
                    sample_uvc_list_formats();
                    return TD_FAILURE;
                }
                ctrl_info->do_set_format = TD_TRUE;
                const format_info *info = sample_uvc_v4l2_format_by_name(optarg);
                if (info == TD_NULL) {
                    sample_print("Unsupported video format '%s'\n", optarg);
                    return TD_FAILURE;
                }
                ctrl_info->pixelformat = info->fourcc;
                break;
            case 'F':
                ctrl_info->do_file = TD_TRUE;
                if (optarg != TD_NULL) {
                    ctrl_info->pattern = optarg;
                }
                break;
            case 's':
                ctrl_info->do_capture = TD_TRUE;
                ctrl_info->do_set_format = TD_TRUE;
                ctrl_info->width = (td_u32)strtol(optarg, &endptr, 10); /* 10:base */
                if (*endptr != 'x' || endptr == optarg) {
                    sample_print("Invalid size '%s'\n", optarg);
                    return TD_FAILURE;
                }
                ctrl_info->height = (td_u32)strtol(endptr + 1, &endptr, 10);    /* 10:base */
                if (*endptr != 0) {
                    sample_print("Invalid size '%s'\n", optarg);
                    return TD_FAILURE;
                }
                break;
            case SAMPLE_UVC_OPT_ENUM_FORMATS:
                ctrl_info->do_enum_formats = TD_TRUE;
                break;
            case 'h':
            default:
                sample_uvc_usage(argv[0]);
                return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_void sample_uvc_ctrl_info_init(uvc_ctrl_info *ctrl_info)
{
    ctrl_info->pause = (td_u32)-1;
    ctrl_info->quality = (td_u32)-1;
    ctrl_info->time_per_frame.numerator = 1;
    ctrl_info->time_per_frame.denominator = 25;     /* 25 frames per second */
    ctrl_info->pattern = "frame-#.bin";
    ctrl_info->nbufs = SAMPLE_UVC_V4L_BUFFERS_DEFAULT;
    ctrl_info->memtype = V4L2_MEMORY_MMAP;
    ctrl_info->nframes = (td_u32)-1;
    ctrl_info->width = 640;     /* 640:width */
    ctrl_info->height = 480;    /* 480:height */
    ctrl_info->pixelformat = V4L2_PIX_FMT_YUYV;
    ctrl_info->fill = BUFFER_FILL_NONE;
    ctrl_info->rt_priority = 1;
}

static td_s32 sample_uvc_prepare_to_set_ctrl(td_s32 argc, td_char *argv[], device_info *dev, uvc_ctrl_info *ctrl_info,
    td_s32 *type)
{
    td_s32 ret;
    td_u32 capabilities = V4L2_CAP_VIDEO_CAPTURE;

    if (ctrl_info->pause != (td_u32)(-1)) {
        ret = sample_uvc_pause_init();
        if (ret < 0) {
            return TD_FAILURE;
        }
    }

    if ((ctrl_info->fill & BUFFER_FILL_PADDING) && (ctrl_info->memtype != V4L2_MEMORY_USERPTR)) {
        sample_print("Buffer overrun can only be checked in USERPTR mode.\n");
        return TD_FAILURE;
    }

    if (!sample_uvc_video_has_fd(dev)) {
        if (optind >= argc) {
            sample_uvc_usage(argv[0]);
            return TD_FAILURE;
        }
        ret = sample_uvc_video_open(dev, argv[optind]);
        if (ret < 0) {
            return TD_FAILURE;
        }
    }

    if (!ctrl_info->no_query) {
        ret = sample_uvc_video_query_cap(dev, &capabilities);
        if (ret < 0) {
            return TD_FAILURE;
        }
    }

    *type = sample_uvc_cap_get_buf_type(capabilities);
    if (*type < 0) {
        return TD_FAILURE;
    }

    if (!ctrl_info->do_file) {
        ctrl_info->pattern = TD_NULL;
    }

    return TD_SUCCESS;
}

static td_void sample_uvc_set_ctrl(device_info *dev, uvc_ctrl_info *ctrl_info, td_s32 type)
{
    td_s32 ret;

    if (!sample_uvc_video_has_valid_buf_type(dev)) {
        sample_uvc_video_set_buf_type(dev, type);
    }

    dev->memtype = ctrl_info->memtype;

    if (ctrl_info->do_log_status) {
        sample_uvc_video_log_status(dev);
    }

    if (ctrl_info->do_get_control) {
        struct v4l2_query_ext_ctrl query;

        ret = sample_uvc_query_ext_control(dev, ctrl_info->ctrl_name, &query);
        if (ret == 0) {
            sample_uvc_video_get_control(dev, &query, TD_FALSE);
        }


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART06_H_ */
