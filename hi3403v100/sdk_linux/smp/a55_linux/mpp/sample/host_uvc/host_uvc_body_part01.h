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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART01_H_


#line 1 "host_uvc.c"
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

#include "host_uvc.h"
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <getopt.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/uvcvideo.h>
#include <linux/usb/video.h>
#include "media_vdec.h"
#include "sdk_module_init.h"

/* set MEDIA_WORK to 0 to write file on disk */
#define MEDIA_WORK  1

#define array_size(a)    (sizeof(a) / sizeof((a)[0]))

#define SAMPLE_UVC_V4L_BUFFERS_DEFAULT  8
#define SAMPLE_UVC_V4L_BUFFERS_MAX      32

#define SAMPLE_UVC_OPT_ENUM_FORMATS   256
#define SAMPLE_UVC_OPT_ENUM_INPUTS    257
#define SAMPLE_UVC_OPT_SKIP_FRAMES    258
#define SAMPLE_UVC_OPT_NO_QUERY       259
#define SAMPLE_UVC_OPT_SLEEP_FOREVER  260
#define SAMPLE_UVC_OPT_USERPTR_OFFSET 261
#define SAMPLE_UVC_OPT_REQUEUE_LAST   262
#define SAMPLE_UVC_OPT_STRIDE         263
#define SAMPLE_UVC_OPT_FD             264
#define SAMPLE_UVC_OPT_TSTAMP_SRC     265
#define SAMPLE_UVC_OPT_FIELD          266
#define SAMPLE_UVC_OPT_LOG_STATUS     267
#define SAMPLE_UVC_OPT_BUFFER_SIZE    268
#define SAMPLE_UVC_OPT_PREMULTIPLIED  269
#define SAMPLE_UVC_OPT_QUEUE_LATE     270
#define SAMPLE_UVC_OPT_DATA_PREFIX    271
#define SAMPLE_UVC_OPT_RESET_CONTROLS 272

#define SAMPLE_UVC_MAX_FILE_NAME_LEN  23

#define DEVICE_UVC_SIZE_NUM           20

ot_size g_device_size[DEVICE_UVC_SIZE_NUM] = {0};
td_u32 g_device_size_num = 0;

td_bool g_uvc_exit = TD_FALSE;

buf_type g_buf_types[] = {
    { V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE, 1, "Video capture mplanes", "capture-mplane", },
    { V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE, 1, "Video output", "output-mplane", },
    { V4L2_BUF_TYPE_VIDEO_CAPTURE, 1, "Video capture", "capture", },
    { V4L2_BUF_TYPE_VIDEO_OUTPUT, 1, "Video output mplanes", "output", },
    { V4L2_BUF_TYPE_VIDEO_OVERLAY, 0, "Video overlay", "overlay" },
    { V4L2_BUF_TYPE_META_CAPTURE, 1, "Meta-data capture", "meta-capture", },
};

format_info g_pixel_formats[] = {
    { "YUYV", V4L2_PIX_FMT_YUYV, 1 },
    { "MJPEG", V4L2_PIX_FMT_MJPEG, 1 },
    { "H264", V4L2_PIX_FMT_H264, 1 },
    { "H265", V4L2_PIX_FMT_H265, 1 },
    { "NV12", V4L2_PIX_FMT_NV12, 1 },
    { "NV21", V4L2_PIX_FMT_NV21, 1 },
};

field_info g_fields[] = {
    { "filed_any", V4L2_FIELD_ANY },
    { "filed_none", V4L2_FIELD_NONE },
    { "filed_top", V4L2_FIELD_TOP },
    { "filed_bottom", V4L2_FIELD_BOTTOM },
    { "filed_interlaced", V4L2_FIELD_INTERLACED },
    { "filed_seq_tb", V4L2_FIELD_SEQ_TB },
    { "filed_seq_bt", V4L2_FIELD_SEQ_BT },
    { "filed_alternate", V4L2_FIELD_ALTERNATE },
    { "filed_interlaced_tb", V4L2_FIELD_INTERLACED_TB },
    { "filed_interlaced_bt", V4L2_FIELD_INTERLACED_BT },
};

static struct option g_opts[] = {
    {"buffer-size", 1, 0, SAMPLE_UVC_OPT_BUFFER_SIZE},
    {"buffer-type", 1, 0, 'B'},
    {"capture", 2, 0, 'c'},
    {"check-overrun", 0, 0, 'C'},
    {"data-prefix", 0, 0, SAMPLE_UVC_OPT_DATA_PREFIX},
    {"delay", 1, 0, 'd'},
    {"enum-formats", 0, 0, SAMPLE_UVC_OPT_ENUM_FORMATS},
    {"enum-inputs", 0, 0, SAMPLE_UVC_OPT_ENUM_INPUTS},
    {"fd", 1, 0, SAMPLE_UVC_OPT_FD},
    {"field", 1, 0, SAMPLE_UVC_OPT_FIELD},
    {"file", 2, 0, 'F'},
    {"fill-frames", 0, 0, 'I'},
    {"format", 1, 0, 'f'},
    {"help", 0, 0, 'h'},
    {"input", 1, 0, 'i'},
    {"list-controls", 0, 0, 'l'},
    {"log-status", 0, 0, SAMPLE_UVC_OPT_LOG_STATUS},
    {"nbufs", 1, 0, 'n'},
    {"no-query", 0, 0, SAMPLE_UVC_OPT_NO_QUERY},
    {"offset", 1, 0, SAMPLE_UVC_OPT_USERPTR_OFFSET},
    {"pause", 2, 0, 'p'},
    {"premultiplied", 0, 0, SAMPLE_UVC_OPT_PREMULTIPLIED},
    {"quality", 1, 0, 'q'},
    {"queue-late", 0, 0, SAMPLE_UVC_OPT_QUEUE_LATE},
    {"get-control", 1, 0, 'r'},
    {"requeue-last", 0, 0, SAMPLE_UVC_OPT_REQUEUE_LAST},
    {"reset-controls", 0, 0, SAMPLE_UVC_OPT_RESET_CONTROLS},
    {"realtime", 2, 0, 'R'},
    {"size", 1, 0, 's'},
    {"set-control", 1, 0, 'w'},
    {"send-extension", 1, 0, 'x'},
    {"skip", 1, 0, SAMPLE_UVC_OPT_SKIP_FRAMES},
    {"sleep-forever", 0, 0, SAMPLE_UVC_OPT_SLEEP_FOREVER},
    {"stride", 1, 0, SAMPLE_UVC_OPT_STRIDE},
    {"time-per-frame", 1, 0, 't'},
    {"timestamp-source", 1, 0, SAMPLE_UVC_OPT_TSTAMP_SRC},
    {"userptr", 0, 0, 'u'},
    {0, 0, 0, 0}
};

static td_bool g_pause_resume;
static struct termios g_pause_term;
static td_bool g_pause_no_term;
static td_bool g_pause_term_configured;
static td_char g_pause_filename[SAMPLE_UVC_MAX_FILE_NAME_LEN];

static td_void sample_uvc_pause_signal_handler(td_s32 signal __attribute__((__unused__)))
{
    g_pause_resume = TD_TRUE;
}

#if (MEDIA_WORK == 1)
static td_void sample_uvc_exit_signal_handler(td_s32 signal __attribute__((__unused__)))
{
    g_uvc_exit = TD_TRUE;
}
#endif

static td_s32 open_by_real_path(const td_char *file_path, int flags)
{
    td_char regular_path[PATH_MAX] = {0};

    if (file_path == NULL) {
        return -1;
    }
    if (strlen(file_path) < PATH_MAX && realpath(file_path, regular_path) != NULL) {
        int fd = open(regular_path, flags);
        return fd;
    }
    sample_print("realpath(%s) fail errno %d\n", file_path, errno);
    return -1;
}

static td_void sample_uvc_pause_wait(td_void)
{
    td_s32 ret;
    td_s32 fd;

    fd = open(g_pause_filename, O_CREAT, 0);
    if (fd != -1) {
        close(fd);
    }

    if (g_pause_no_term) {
        sample_print("Paused waiting for SIGUSR1\n");
        while (!g_pause_resume) {
            pause();
        }
        goto done;
    }

    sample_print("Paused waiting for key press or SIGUSR1\n");
    g_pause_resume = TD_FALSE;

    while (!g_pause_resume) {
        fd_set read_fds;
        td_char c;

        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);

        ret = select(1, &read_fds, TD_NULL, TD_NULL, TD_NULL);
        if (ret < 0 && errno != EINTR) {
            break;
        }
        if (ret == 1) {
            ret = read(0, &c, 1);
            break;
        }
    }

done:
    unlink(g_pause_filename);
}

static td_void sample_uvc_pause_cleanup(td_void)
{
    if (g_pause_term_configured) {
        tcsetattr(0, TCSANOW, &g_pause_term);
    }

    unlink(g_pause_filename);
}

static td_s32 sample_uvc_pause_init(td_void)
{
    struct sigaction sig_usr1;
    struct termios term;
    td_s32 ret;

    (td_void)sprintf_s(g_pause_filename, SAMPLE_UVC_MAX_FILE_NAME_LEN, ".host_uvc.wait.%d", getpid());

    (td_void)memset_s(&sig_usr1, sizeof(sig_usr1), 0, sizeof(sig_usr1));
    sig_usr1.sa_handler = sample_uvc_pause_signal_handler;
    ret = sigaction(SIGUSR1, &sig_usr1, TD_NULL);
    if (ret < 0) {
        sample_print("Unable to install SIGUSR1 handler\n");
        return -errno;
    }

    ret = tcgetattr(0, &term);
    if (ret < 0) {
        if (errno == ENOTTY) {
            g_pause_no_term = TD_TRUE;
            return 0;
        }

        sample_print("Unable to retrieve terminal attributes\n");
        return -errno;
    }

    g_pause_term = term;
    g_pause_term_configured = TD_TRUE;

    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 0;

    ret = tcsetattr(0, TCSANOW, &term);
    if (ret < 0) {
        sample_print("Unable to set terminal attributes\n");
        return -errno;
    }

    return 0;
}

static td_bool sample_uvc_video_is_mplane(const device_info *dev)
{
    return (dev->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE || dev->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE);
}

static td_bool sample_uvc_video_is_meta(const device_info *dev)
{
    return (dev->type == V4L2_BUF_TYPE_META_CAPTURE);
}

static td_bool sample_uvc_video_is_capture(const device_info *dev)
{
    return (dev->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE || dev->type == V4L2_BUF_TYPE_VIDEO_CAPTURE ||
        dev->type == V4L2_BUF_TYPE_META_CAPTURE);
}

static td_bool sample_uvc_video_is_output(const device_info *dev)
{
    return (dev->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE || dev->type == V4L2_BUF_TYPE_VIDEO_OUTPUT);
}

static const td_char *sample_uvc_v4l2_buf_type_name(enum v4l2_buf_type type)
{
    td_u32 i;

    for (i = 0; i < array_size(g_buf_types); ++i) {
        if (g_buf_types[i].type == type) {
            return g_buf_types[i].name;
        }
    }

    if (type & V4L2_BUF_TYPE_PRIVATE) {
        return "Private";
    } else {
        return "Unknown";
    }
}

static td_void sample_uvc_list_formats(td_void)
{
    td_u32 i;

    for (i = 0; i < array_size(g_pixel_formats); i++) {
        sample_print("%s (\"%u%u%u%u\", %d planes)\n", g_pixel_formats[i].name, g_pixel_formats[i].fourcc & 0xff,
            (g_pixel_formats[i].fourcc >> 8) & 0xff, (g_pixel_formats[i].fourcc >> 16) & 0xff,  /* 8,16:shift */
            (g_pixel_formats[i].fourcc >> 24) & 0xff, g_pixel_formats[i].n_planes);             /* 24:shift */
    }
}

static const format_info *sample_uvc_v4l2_format_by_fourcc(td_u32 fourcc)
{
    td_u32 i;

    for (i = 0; i < array_size(g_pixel_formats); ++i) {
        if (g_pixel_formats[i].fourcc == fourcc) {
            return &g_pixel_formats[i];
        }
    }

    return TD_NULL;
}

static const format_info *sample_uvc_v4l2_format_by_name(const td_char *name)
{
    td_u32 i;

    for (i = 0; i < array_size(g_pixel_formats); ++i) {
        if (strcasecmp(g_pixel_formats[i].name, name) == 0) {
            return &g_pixel_formats[i];
        }
    }

    return TD_NULL;
}

static const td_char *sample_uvc_v4l2_format_name(td_u32 fourcc)
{
    const format_info *format;
    static td_char format_name[5];     /* 5: array len */
    td_u32 i;

    format = sample_uvc_v4l2_format_by_fourcc(fourcc);
    if (format) {
        return format->name;
    }

    for (i = 0; i < 4; ++i) {       /* 4: format */
        format_name[i] = fourcc & 0xff;
        fourcc >>= 8;               /* 8: shift */
    }

    format_name[4] = '\0';     /* 4: end */
    return format_name;
}

static const td_char *sample_uvc_v4l2_field_name(enum v4l2_field field)
{
    td_u32 i;

    for (i = 0; i < array_size(g_fields); ++i) {
        if (g_fields[i].field == field) {
            return g_fields[i].name;
        }
    }

    return "unknown";
}

static td_void sample_uvc_video_set_buf_type(device_info *dev, enum v4l2_buf_type type)
{
    dev->type = type;
}

static td_bool sample_uvc_video_has_valid_buf_type(const device_info *dev)
{
    return (td_s32)dev->type != -1;
}

static td_void sample_uvc_video_init(device_info *dev)
{
    dev->fd = -1;
    dev->memtype = V4L2_MEMORY_MMAP;
    dev->buffers = TD_NULL;
    dev->type = (enum v4l2_buf_type)-1;
}

static td_bool sample_uvc_video_has_fd(const device_info *dev)
{
    return dev->fd != -1;
}

static td_s32 sample_uvc_video_open(device_info *dev, const td_char *devname)
{
    td_char regular_path[PATH_MAX] = {0};

    if (sample_uvc_video_has_fd(dev)) {
        sample_print("Can't open device (already open).\n");
        return -1;
    }
    if (strlen(devname) >= PATH_MAX || realpath(devname, regular_path) == NULL) {
        sample_print("Can't get regular path of %s\n", devname);
        return -1;
    }
    dev->fd = open(regular_path, O_RDWR);
    if (dev->fd < 0) {
        sample_print("Error opening device %s\n", devname);
        return dev->fd;
    }

    printf("Device %s opened.\n", devname);

    dev->opened = TD_TRUE;

    return 0;
}

static td_s32 sample_uvc_video_query_cap(device_info *dev, td_u32 *capabilities)
{
    struct v4l2_capability cap;
    td_u32 caps;
    td_bool is_support_video;
    td_bool is_support_meta;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART01_H_ */
