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

#ifndef HOST_UVC_H
#define HOST_UVC_H

#include <linux/videodev2.h>
#include "sample_comm.h"

#ifndef V4L2_PIX_FMT_H265
#define V4L2_PIX_FMT_H265     v4l2_fourcc('H', '2', '6', '5') /* H.265 aka HEVC */
#endif

enum buffer_fill_mode {
    BUFFER_FILL_NONE = 0,
    BUFFER_FILL_FRAME = 1 << 0,
    BUFFER_FILL_PADDING = 1 << 1,
};

typedef struct {
    td_u32 idx;
    td_u32 padding[VIDEO_MAX_PLANES];
    td_u32 size[VIDEO_MAX_PLANES];
    td_void *mem[VIDEO_MAX_PLANES];
} buffer_info;

typedef struct {
    td_s32 fd;
    td_bool opened;
    enum v4l2_buf_type type;
    enum v4l2_memory memtype;
    td_u32 nbufs;
    buffer_info *buffers;
    td_u32 width;
    td_u32 height;
    uint32_t buffer_output_flags;
    uint32_t timestamp_type;
    td_u32 num_planes;
    struct v4l2_plane_pix_format plane_fmt[VIDEO_MAX_PLANES];
    td_void *pattern[VIDEO_MAX_PLANES];
    td_u32 patternsize[VIDEO_MAX_PLANES];
    td_bool write_data_prefix;
} device_info;

typedef struct {
    enum v4l2_buf_type type;
    td_bool supported;
    const td_char *name;
    const td_char *string;
} buf_type;

typedef struct {
    const td_char *name;
    td_u32 fourcc;
    td_char n_planes;
} format_info;

typedef struct {
    const td_char *name;
    enum v4l2_field field;
} field_info;

typedef struct {
    td_u32 nframes;
    td_u32 skip;
    td_u32 delay;
    td_u32 pause;
    td_bool do_requeue_last;
    td_bool do_queue_late;
    enum buffer_fill_mode fill;
    const td_char *pattern;
    const td_char *type_name;

    td_bool do_capture;
    td_bool do_set_format;
    td_u32 pixelformat;
    td_bool do_file;
    td_bool do_set_input;
    td_s32 input;
    td_bool do_list_controls;
    td_u32 nbufs;
    td_u32 quality;
    td_s32 ctrl_name;
    td_bool do_get_control;
    td_bool do_rt;
    td_s32 rt_priority;
    td_u32 width;
    td_u32 height;
    td_u32 stride;
    td_u32 buffer_size;
    td_bool do_set_time_per_frame;
    struct v4l2_fract time_per_frame;
    enum v4l2_memory memtype;
    const td_char *ctrl_value;
    td_bool do_set_control;
    td_s32 extension_name;
    const td_char *extension_channel;
    td_bool do_send_extension;
    td_bool do_enum_formats;
    td_bool do_enum_inputs;
    enum v4l2_field field;
    td_bool do_log_status;
    td_bool no_query;
    td_u32 fmt_flags;
    td_bool do_reset_control;
    td_bool do_sleep_forever;
    td_u32 userptr_offset;
    td_bool do_reset_controls;
} uvc_ctrl_info;

#endif
