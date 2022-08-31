/*
* Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef __HIFB_USERSPACE_H__
#define __HIFB_USERSPACE_H__

#define HIFB_GET_MEMBUF _IOR('F', 0x21, struct hifb_membuf_info)
#define HIFB_REFRESH_FRAMEINFO _IOWR('F', 0x25, struct hifb_frame_info)

#define HIFB_GET_FSCREENINFO _IOR('F', 0x22, struct fb_fix_screeninfo)
#define HIFB_GET_VSCREENINFO _IOR('F', 0x23, struct fb_var_screeninfo)
#define HIFB_PUT_VSCREENINFO _IOW('F', 0x24, struct fb_var_screeninfo)

struct hifb_membuf_info {
    int fd;
};

#ifndef __KERNEL__
#include "display_type.h"

struct hifb_frame_info {
    IRect rect;
    unsigned int stride;
    unsigned long bufaddr;
    int format;
    int in_fence;
    int out_fence;
};
#endif
#endif
