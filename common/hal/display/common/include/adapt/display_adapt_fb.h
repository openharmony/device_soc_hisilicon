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

#ifndef DISPLAY_ADPT_FB
#define DISPLAY_ADPT_FB

#include "gfbg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define TD_BOOL td_bool
typedef ot_fb_rect HIFB_RECT;
typedef ot_fb_buf HIFB_BUFFER_S;

#define FBIOGET_CANVAS_BUFFER     FBIOGET_CANVAS_BUF
#define FBIOGET_VBLANK_HIFB       FBIOGET_VER_BLANK_GFBG
#define FBIOPUT_SCREENSIZE        FBIOPUT_SCREEN_SIZE
#define FBIOGET_SCREENSIZE        FBIOGET_SCREEN_SIZE
#define FBIOPUT_SHOW_HIFB         FBIOPUT_SHOW_GFBG
#define FBIOPUT_ALPHA_HIFB        FBIOPUT_ALPHA_GFBG

#define FBIOPUT_SCREEN_ORIGIN_HIFB FBIOPUT_SCREEN_ORIGIN_GFBG
#define FBIOGET_ALPHA_HIFB         FBIOGET_ALPHA_GFBG

typedef ot_fb_size HIFB_SIZE_S;

/* antiflicker level */
/* Auto means fb will choose a appropriate antiflicker level automatically according to the color info of map */
#define HIFB_LAYER_ANTIFLICKER_NONE  OT_FB_LAYER_ANTIFLICKER_NONE
#define HIFB_LAYER_ANTIFLICKER_LOW   OT_FB_LAYER_ANTIFLICKER_LOW
#define HIFB_LAYER_ANTIFLICKER_MID   OT_FB_LAYER_ANTIFLICKER_MID
#define HIFB_LAYER_ANTIFLICKER_HIGH  OT_FB_LAYER_ANTIFLICKER_HIGH
#define HIFB_LAYER_ANTIFLICKER_AUTO  OT_FB_LAYER_ANTIFLICKER_AUTO
#define HIFB_LAYER_ANTIFLICKER_BUTT  OT_FB_LAYER_ANTIFLICKER_BUTT
typedef ot_fb_layer_antiflicker_level HIFB_LAYER_ANTIFLICKER_LEVEL_E;

/* layer info maskbit */
#define HIFB_LAYERMASK_BUFMODE               OT_FB_LAYER_MASK_BUF_MODE
#define HIFB_LAYERMASK_ANTIFLICKER_MODE      OT_FB_LAYER_MASK_ANTIFLICKER_MODE
#define HIFB_LAYERMASK_POS                   OT_FB_LAYER_MASK_POS
#define HIFB_LAYERMASK_CANVASSIZE            OT_FB_LAYER_MASK_CANVAS_SIZE
#define HIFB_LAYERMASK_DISPSIZE              OT_FB_LAYER_MASK_DISPLAY_SIZE
#define HIFB_LAYERMASK_SCREENSIZE            OT_FB_LAYER_MASK_SCREEN_SIZE
#define HIFB_LAYERMASK_BMUL                  OT_FB_LAYER_MASK_MUL
#define HIFB_LAYERMASK_BUTT                  OT_FB_LAYER_MASK_BUTT
typedef ot_fb_layer_info_maskbit HIFB_LAYER_INFO_MASKBIT;

/* rotate mode */
#define HIFB_ROTATE_NONE  OT_FB_ROTATE_NONE
#define HIFB_ROTATE_90    OT_FB_ROTATE_90
#define HIFB_ROTATE_180   OT_FB_ROTATE_180
#define HIFB_ROTATE_270   OT_FB_ROTATE_270
#define HIFB_ROTATE_BUTT  OT_FB_ROTATE_BUTT
typedef ot_fb_rotate_mode HIFB_ROTATE_MODE_E;

/* surface info */
typedef ot_fb_surface HIFB_SURFACE_S;

#define  HIFB_FMT_RGB565      OT_FB_FORMAT_RGB565
#define  HIFB_FMT_RGB888      OT_FB_FORMAT_RGB888

#define  HIFB_FMT_KRGB444     OT_FB_FORMAT_KRGB444
#define  HIFB_FMT_KRGB555     OT_FB_FORMAT_KRGB555
#define  HIFB_FMT_KRGB888     OT_FB_FORMAT_KRGB888

#define  HIFB_FMT_ARGB4444    OT_FB_FORMAT_ARGB4444
#define  HIFB_FMT_ARGB1555    OT_FB_FORMAT_ARGB1555
#define  HIFB_FMT_ARGB8888    OT_FB_FORMAT_ARGB8888
#define  HIFB_FMT_ARGB8565    OT_FB_FORMAT_ARGB8565

#define  HIFB_FMT_RGBA4444    OT_FB_FORMAT_RGBA4444
#define  HIFB_FMT_RGBA5551    OT_FB_FORMAT_RGBA5551
#define  HIFB_FMT_RGBA5658    OT_FB_FORMAT_RGBA5658
#define  HIFB_FMT_RGBA8888    OT_FB_FORMAT_RGBA8888

#define  HIFB_FMT_BGR565      OT_FB_FORMAT_BGR565
#define  HIFB_FMT_BGR888      OT_FB_FORMAT_BGR888
#define  HIFB_FMT_ABGR4444    OT_FB_FORMAT_ABGR4444
#define  HIFB_FMT_ABGR1555    OT_FB_FORMAT_ABGR1555
#define  HIFB_FMT_ABGR8888    OT_FB_FORMAT_ABGR8888
#define  HIFB_FMT_ABGR8565    OT_FB_FORMAT_ABGR8565
#define  HIFB_FMT_KBGR444     OT_FB_FORMAT_KBGR444
#define  HIFB_FMT_KBGR555     OT_FB_FORMAT_KBGR555
#define  HIFB_FMT_KBGR888     OT_FB_FORMAT_KBGR888

#define  HIFB_FMT_1BPP        OT_FB_FORMAT_1BPP
#define  HIFB_FMT_2BPP        OT_FB_FORMAT_2BPP
#define  HIFB_FMT_4BPP        OT_FB_FORMAT_4BPP
#define  HIFB_FMT_8BPP        OT_FB_FORMAT_8BPP
#define  HIFB_FMT_ACLUT44     OT_FB_FORMAT_ACLUT44
#define  HIFB_FMT_ACLUT88     OT_FB_FORMAT_ACLUT88
#define  HIFB_FMT_PUYVY       OT_FB_FORMAT_PUYVY
#define  HIFB_FMT_PYUYV       OT_FB_FORMAT_PYUYV
#define  HIFB_FMT_PYVYU       OT_FB_FORMAT_PYVYU
#define  HIFB_FMT_YUV888      OT_FB_FORMAT_YUV888
#define  HIFB_FMT_AYUV8888    OT_FB_FORMAT_AYUV8888
#define  HIFB_FMT_YUVA8888    OT_FB_FORMAT_YUVA8888
#define  HIFB_FMT_BUTT        OT_FB_FORMAT_BUTT

typedef ot_fb_color_format HIFB_COLOR_FMT_E;

#define  HIFB_LAYER_BUF_DOUBLE            OT_FB_LAYER_BUF_DOUBLE
#define  HIFB_LAYER_BUF_ONE               OT_FB_LAYER_BUF_ONE
#define  HIFB_LAYER_BUF_NONE              OT_FB_LAYER_BUF_NONE
#define  HIFB_LAYER_BUF_DOUBLE_IMMEDIATE  OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE

typedef ot_fb_point HIFB_POINT_S;
typedef ot_fb_alpha HIFB_ALPHA_S;
typedef ot_fb_layer_info HIFB_LAYER_INFO_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // DISPLAY_ADPT_FB
