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

#ifndef __OT_CTRL_H__
#define __OT_CTRL_H__

// CTRL id
#define OT_XUID_SET_RESET 0x01
#define OT_XUID_SET_STREAM 0x02
#define OT_XUID_SET_RESOLUTION 0x03
#define OT_XUID_SET_IFRAME 0x04
#define OT_XUID_SET_BITRATE 0x05
#define OT_XUID_UPDATE_SYSTEM 0x06

typedef struct _uvcx_base_ctrl_t {
    uint16_t selector;
    uint16_t index;
} __attribute__((__packed__)) uvcx_base_ctrl_t;


typedef struct uvcx_camera_stream_t {
    uint16_t format;
    uint16_t resolution;
} __attribute__((__packed__)) uvcx_camera_stream_t;


#endif // __OT_CTRL_H__
