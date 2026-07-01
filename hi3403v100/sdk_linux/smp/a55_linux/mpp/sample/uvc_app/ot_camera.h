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

#ifndef __OT_CAMERA_H__
#define __OT_CAMERA_H__

typedef enum ot_stream_type_e {
    OT_FORMAT_H264 = 0x1,
    OT_FORMAT_MJPEG = 0x2,
    OT_FORMAT_MJPG = 0x2,
    OT_FORMAT_YUV = 0x3,
    OT_FORMAT_YUV420 = 0x3
} ot_stream_type_e;

typedef enum ot_stream_resolution_e {
    OT_RESOLUTION_1080 = 0x1,
    OT_RESOLUTION_720 = 0x2,
    OT_RESOLUTION_360 = 0x3
} ot_stream_resolution_e;

typedef struct encoder_property {
    unsigned int format;
    unsigned int width;
    unsigned int height;
    unsigned char compsite;
} encoder_property;

#endif // __OT_CAMERA_H__
