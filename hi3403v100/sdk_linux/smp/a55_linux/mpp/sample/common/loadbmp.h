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

#ifndef __LOAD_BMP_H__
#define __LOAD_BMP_H__

#include "ot_type.h"

/* the color format OSD supported */
typedef enum {
    OSD_COLOR_FORMAT_RGB444 = 0,
    OSD_COLOR_FORMAT_RGB4444 = 1,
    OSD_COLOR_FORMAT_RGB555 = 2,
    OSD_COLOR_FORMAT_RGB565 = 3,
    OSD_COLOR_FORMAT_RGB1555 = 4,
    OSD_COLOR_FORMAT_RGB888 = 6,
    OSD_COLOR_FORMAT_RGB8888 = 7,
    OSD_COLOR_FORMAT_CLUT2 = 8,
    OSD_COLOR_FORMAT_CLUT4 = 9,
    OSD_COLOR_FORMAT_BUTT
} osd_color_format;

typedef struct {
    td_u8 r;
    td_u8 g;
    td_u8 b;
    td_u8 reserved;
} osd_rgb;

typedef struct {
    osd_color_format color_format; /* color format */
    td_u16 height;                 /* operation height */
    td_u16 width;                  /* operation width */
    td_u16 stride;                 /* surface stride */
    td_u16 reserved;
    td_u8 *virt_addr; /* virtual address */
} osd_surface;

typedef struct {
    td_u32 width;   /* out */
    td_u32 height;  /* out */
    td_u32 stride;  /* in */
    td_u8 *rgb_buf; /* in/out */
} osd_logo;

typedef struct {
    td_u16 bi_size;
    td_u32 bi_width;
    td_s32 bi_height;
    td_u16 bi_planes;
    td_u16 bi_bit_count;
    td_u32 bi_compression;
    td_u32 bi_size_image;
    td_u32 bi_x_pels_per_meter;
    td_u32 bi_y_pels_per_meter;
    td_u32 bi_clr_used;
    td_u32 bi_clr_important;
} osd_bit_map_info_header;

typedef struct {
    td_u32 bf_size;
    td_u16 bf_reserved1;
    td_u16 bf_reserved2;
    td_u32 bf_off_bits;
} osd_bit_map_file_header;

typedef struct {
    td_u8 blue;
    td_u8 green;
    td_u8 red;
    td_u8 reserved;
} osd_rgb_quad;

typedef struct {
    osd_bit_map_info_header bmp_header;
    osd_rgb_quad bmp_colors[1];
} osd_bit_map_info;

typedef struct {
    td_u8 a_len;
    td_u8 r_len;
    td_u8 g_len;
    td_u8 b_len;
} osd_component_info;

typedef struct {
    td_u32 width;
    td_u32 height;
    td_u32 stride;
} canvas_size_info;

typedef canvas_size_info bmp_data_size_info;

#ifdef __cplusplus
extern "C" {
#endif

td_s32 load_image(const td_char *filename, osd_logo *video_logo);
td_s32 load_bit_map_to_surface(const td_char *file_name, const osd_surface *surface, td_u8 *virt);
td_s32 create_surface_by_bit_map(const td_char *file_name, osd_surface *surface, td_u8 *virt);
td_s32 create_surface_by_canvas(const td_char *file_name, osd_surface *surface, td_u8 *virt,
    const canvas_size_info *canvas_size);
td_s32 get_bmp_info(const td_char *filename, osd_bit_map_file_header *bmp_file_header, osd_bit_map_info *bmp_info);

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef __LOAD_BMP_H__ */
