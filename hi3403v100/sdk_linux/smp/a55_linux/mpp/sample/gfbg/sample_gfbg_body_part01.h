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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART01_H_


#line 1 "sample_gfbg.c"
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>

#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#include "securec.h"
#include "loadbmp.h"

#include "gfbg.h"
#include "ot_common_tde.h"
#include "ss_mpi_tde.h"
#include "sample_comm.h"
#include "sdk_module_init.h"

#define FILE_LENGTH_MAX 12
#define CMAP_LENGTH_MAX 256
#define WIDTH_1080P 1920
#define HEIGHT_1080P 1080
#define WIDTH_800 800
#define HEIGHT_600 600

#define SAMPLE_IMAGE_WIDTH     300
#define SAMPLE_IMAGE_HEIGHT    150
#define SAMPLE_IMAGE_NUM       20
#define GFBG_RED_1555          0xFC00
#define GFBG_RED_8888          0xFFff0000
#define WIDTH_1920             1920
#define HEIGHT_1080            1080

#define GRAPHICS_LAYER_G0      0
#define GRAPHICS_LAYER_G1      1
#define GRAPHICS_LAYER_G2      2
#define GRAPHICS_LAYER_G3      3

#define SAMPLE_IMAGE1_PATH        "./res/%d.bmp"
#define SAMPLE_IMAGE2_PATH        "./res/1280_720.bits"
#define SAMPLE_CURSOR_PATH        "./res/cursor.bmp"

#if (defined(CONFIG_OT_GFBG_SUPPORT) && defined(CONFIG_OT_VO_SUPPORT))
#define GFBG_BE_WITH_VO    1
#else
#define GFBG_BE_WITH_VO    0
#endif

static td_char g_exit_flag = 0;
static int g_sample_gfbg_exit = 0;
ot_vo_intf_type g_vo_intf_type = OT_VO_INTF_BT1120;
ot_vo_dev g_vo_dev = 0;
osd_color_format g_osd_color_fmt = OSD_COLOR_FORMAT_RGB1555;

static struct fb_bitfield g_r16 = {10, 5, 0};
static struct fb_bitfield g_g16 = {5, 5, 0};
static struct fb_bitfield g_b16 = {0, 5, 0};
static struct fb_bitfield g_a16 = {15, 1, 0};

static struct fb_bitfield g_a32 = {24, 8, 0};
static struct fb_bitfield g_r32 = {16, 8, 0};
static struct fb_bitfield g_g32 = {8,  8, 0};
static struct fb_bitfield g_b32 = {0,  8, 0};

static struct fb_bitfield g_a4 = {0, 0, 0};
static struct fb_bitfield g_r4 = {0, 4, 0};
static struct fb_bitfield g_g4 = {0, 4, 0};
static struct fb_bitfield g_b4 = {0, 4, 0};

td_u16 g_cmap_red[CMAP_LENGTH_MAX] = {0xff, 0, 0, 0xff};
td_u16 g_cmap_green[CMAP_LENGTH_MAX] = {0, 0xff, 0, 0xff};
td_u16 g_cmap_blue[CMAP_LENGTH_MAX] = {0, 0, 0xff, 0xff};
td_u16 g_cmap_alpha[CMAP_LENGTH_MAX] = {0xff, 0xff, 0xff, 0xff};

pthread_t g_gfbg_thread = 0;
pthread_t g_gfbg_thread1 = 0;

td_phys_addr_t g_phyaddr = 0;
td_phys_addr_t g_canvas_addr = 0;

#ifdef CONFIG_SUPPORT_SAMPLE_ROTATION
static td_phys_addr_t g_canvas_phy = 0;
static td_void *g_canvas_vir = NULL;
static td_phys_addr_t g_picture_phy = 0;
static td_void *g_picture_vir = NULL;
#endif

typedef struct {
    td_s32 fd; /* fb's file describe */
    td_s32 layer; /* which graphic layer */
    td_s32 ctrlkey; /* {0,1,2,3}={1buffer, 2buffer, 0buffer pan display, 0buffer refresh} */
    td_bool compress; /* image compressed or not */
    ot_fb_color_format color_format; /* color format. */
} pthread_gfbg_sample_info;

typedef struct {
    ot_vo_dev vo_dev;
    ot_vo_intf_type vo_intf_type;
}vo_device_info;

#ifdef CONFIG_SUPPORT_SAMPLE_ROTATION
static td_s32 gfbg_put_layer_info(pthread_gfbg_sample_info *info);
static td_s32 gfbg_get_canvas(pthread_gfbg_sample_info *info);
static td_void gfbg_put_canvas(td_void);
static td_void gfbg_init_surface(ot_tde_surface *src_surface, ot_tde_surface *dst_surface,
    ot_tde_rect *src_rect, ot_tde_rect *dst_rect, td_u32 byte_per_pixel);
static td_s32 gfbg_draw(pthread_gfbg_sample_info *info);
static td_s32 gfbg_refresh(const pthread_gfbg_sample_info *info);
static td_void gfbg_get_var_by_format(pthread_gfbg_sample_info *info, struct fb_var_screeninfo *var_info);
static td_void gfbg_put_rotation_degree(struct fb_var_screeninfo *var_info, ot_fb_rotate_mode *rotate_mode);
static td_s32 gfbg_put_rotation(pthread_gfbg_sample_info *info);
static td_void gfbg_rotate(ot_vo_dev vo_dev);
#endif
static td_void sample_gfbg_to_exit_signal(td_void);

static int sample_gfbg_getchar(td_void)
{
    int c;
    if (g_sample_gfbg_exit == 1) {
        sample_gfbg_to_exit_signal();
        sample_comm_sys_exit();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
        exit(-1);
    }

    c = getchar();

    if (g_sample_gfbg_exit == 1) {
        sample_gfbg_to_exit_signal();
        sample_comm_sys_exit();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
        exit(-1);
    }

    return c;
}

static td_s32 sample_gfbg_load_bmp(const char *filename, td_u8 *addr)
{
    osd_surface surface;
    osd_bit_map_file_header bmp_file_header;
    osd_bit_map_info bmp_info;

    if (get_bmp_info(filename, &bmp_file_header, &bmp_info) < 0) {
        sample_print("get_bmp_info err!\n");
        return TD_FAILURE;
    }
    surface.color_format = g_osd_color_fmt;
    create_surface_by_bit_map(filename, &surface, addr);
    return TD_SUCCESS;
}

static td_void sample_gfbg_to_exit(td_void)
{
    td_char ch = 0;

    while (ch != 'q') {
        printf("\npress 'q' to exit this sample.\n");
        ch = (td_char)sample_gfbg_getchar();
        while (ch == '\n') {
            ch = (td_char)sample_gfbg_getchar();
        }
        if (ch == 'q') {
            g_exit_flag = ch;
            continue;
        } else {
            printf("input invalid! please try again.\n");
        }
    }
    if (g_gfbg_thread != 0) {
        pthread_join(g_gfbg_thread, 0);
        g_gfbg_thread = 0;
    }

    if (g_gfbg_thread1 != 0) {
        pthread_join(g_gfbg_thread1, 0);
        g_gfbg_thread1 = 0;
    }
    return;
}

static td_s32 sample_get_file_name(pthread_gfbg_sample_info *info, td_char *file, td_u32 file_length)
{
    switch (info->layer) {
        case GRAPHICS_LAYER_G0:
            if (strncpy_s(file, file_length, "/dev/fb0", strlen("/dev/fb0")) != EOK) {
                printf("%s:%d:strncpy_s failed.\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            break;
        case GRAPHICS_LAYER_G1:
            if (strncpy_s(file, file_length, "/dev/fb1", strlen("/dev/fb1")) != EOK) {
                printf("%s:%d:strncpy_s failed.\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            break;
        case GRAPHICS_LAYER_G2:
            if (strncpy_s(file, file_length, "/dev/fb2", strlen("/dev/fb2")) != EOK) {
                printf("%s:%d:strncpy_s failed.\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            break;
        case GRAPHICS_LAYER_G3:
            if (strncpy_s(file, file_length, "/dev/fb2", strlen("/dev/fb2")) != EOK) {
                printf("%s:%d:strncpy_s failed.\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            break;
        default:
            if (strncpy_s(file, file_length, "/dev/fb0", strlen("/dev/fb0")) != EOK) {
                printf("%s:%d:strncpy_s failed.\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            break;
    }
    return TD_SUCCESS;
}

static td_s32 sample_init_frame_buffer(pthread_gfbg_sample_info *info, const char *input_file)
{
    td_bool show;
    ot_fb_point point = {0, 0};
    td_char file[PATH_MAX] = {0};

    if (strlen(input_file) >= PATH_MAX || realpath(input_file, file) == TD_NULL) {
        return TD_FAILURE;
    }
    /* step 1. open framebuffer device overlay 0 */
    info->fd = open(file, O_RDWR, 0);
    if (info->fd < 0) {
        sample_print("open %s failed!\n", file);
        return TD_FAILURE;
    }

    show = TD_FALSE;
    if (ioctl(info->fd, FBIOPUT_SHOW_GFBG, &show) < 0) {
        sample_print("FBIOPUT_SHOW_GFBG failed!\n");
        close(info->fd);
        info->fd = -1;
        return TD_FAILURE;
    }

    /* step 2. set the screen original position */
    switch (info->ctrlkey) {
        case 3: /* 3 mouse case */
            point.x_pos = 150; /* 150 x pos */
            point.y_pos = 150; /* 150 y pos */
            break;
        default:
            point.x_pos = 0;
            point.y_pos = 0;
    }

    if (ioctl(info->fd, FBIOPUT_SCREEN_ORIGIN_GFBG, &point) < 0) {
        sample_print("set screen original show position failed!\n");
        close(info->fd);
        info->fd = -1;
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_init_var(pthread_gfbg_sample_info *info)
{
    struct fb_var_screeninfo var;

    if (ioctl(info->fd, FBIOGET_VSCREENINFO, &var) < 0) {
        sample_print("get variable screen info failed!\n");
        return TD_FAILURE;
    }

    switch (info->color_format) {
        case OT_FB_FORMAT_ARGB8888:
            var.transp = g_a32;
            var.red = g_r32;
            var.green = g_g32;
            var.blue = g_b32;
            var.bits_per_pixel = 32; /* 32 for 4 byte */
            g_osd_color_fmt = OSD_COLOR_FORMAT_RGB8888;
            break;
        default:
            var.transp = g_a16;
            var.red = g_r16;
            var.green = g_g16;
            var.blue = g_b16;
            var.bits_per_pixel = 16; /* 16 for 2 byte */
            break;
    }

    switch (info->ctrlkey) {
        case 3: /* 3 mouse case */
            var.xres_virtual = 48; /* 48 for alg data */
            var.yres_virtual = 48; /* 48 for alg data */
            var.xres = 48; /* 48 for alg data */
            var.yres = 48; /* 48 for alg data */
            break;
        default:
            var.xres_virtual = WIDTH_1080P;
            var.yres_virtual = HEIGHT_1080P * 2; /* 2 for 2buf */
            var.xres = WIDTH_1080P;
            var.yres = HEIGHT_1080P;
    }
    var.activate       = FB_ACTIVATE_NOW;

    if (ioctl(info->fd, FBIOPUT_VSCREENINFO, &var) < 0) {
        sample_print("put variable screen info failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_put_alpha_and_colorkey(pthread_gfbg_sample_info *info, ot_fb_color_format clr_fmt)
{
    td_s32 ret;
    ot_fb_alpha alpha = {0};
    ot_fb_colorkey color_key;
    sample_print("expected: the red box will appear!\n");
    sleep(2); /* 2 second */
    alpha.alpha_en = TD_TRUE;
    alpha.alpha0 = 0x0;
    alpha.alpha1 = 0x0;
    if (ioctl(info->fd, FBIOPUT_ALPHA_GFBG,  &alpha) < 0) {
        sample_print("set alpha failed!\n");
        return TD_FAILURE;
    }
    sample_print("expected: after set alpha = 0, the red box will disappear!\n");
    sleep(2); /* 2 second */

    alpha.alpha0 = 0;
    alpha.alpha1 = 0xFF;
    if (ioctl(info->fd, FBIOPUT_ALPHA_GFBG,  &alpha) < 0) {
        sample_print("set alpha failed!\n");
        return TD_FAILURE;
    }
    sample_print("expected:after set set alpha = 0xFF, the red box will appear again!\n");
    sleep(2); /* 2 second */

    sample_print("expected: the red box will erased by colorkey!\n");
    color_key.enable = TD_TRUE;
    color_key.value = (clr_fmt == OT_FB_FORMAT_ARGB8888) ? GFBG_RED_8888 : GFBG_RED_1555;
    ret = ioctl(info->fd, FBIOPUT_COLORKEY_GFBG, &color_key);
    if (ret < 0) {
        sample_print("FBIOPUT_COLORKEY_GFBG failed!\n");
        return TD_FAILURE;
    }
    sleep(2); /* 2 second */
    sample_print("expected: the red box will appear again!\n");
    color_key.enable = TD_FALSE;
    ret = ioctl(info->fd, FBIOPUT_COLORKEY_GFBG, &color_key);
    if (ret < 0) {
        sample_print("FBIOPUT_COLORKEY_GFBG failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void sample_draw_rect_by_cpu(td_void *ptemp, ot_fb_color_format clr_fmt, struct fb_var_screeninfo *var)
{
    td_s32 x;
    td_s32 y;
    for (y = 100; y < 300; y++) { /* 100 300 for y */
        for (x = 0; x < 300; x++) { /* 300 for x */
            if (clr_fmt == OT_FB_FORMAT_ARGB8888) {
                *((td_u32*)ptemp + y * var->xres + x) = GFBG_RED_8888;
            } else {
                *((td_u16*)ptemp + y * var->xres + x) = GFBG_RED_1555;
            }
        }
    }
    return;
}

static td_s32 sample_time_to_play(pthread_gfbg_sample_info *info, td_u8 *show_screen, td_u32 fix_screen_stride)
{
    td_bool show;
    td_void *ptemp = TD_NULL;
    td_s32 i;
    struct fb_var_screeninfo var;
    ot_fb_color_format clr_fmt;

    if (ioctl(info->fd, FBIOGET_VSCREENINFO, &var) < 0) {
        sample_print("get variable screen info failed!\n");
        return TD_FAILURE;
    }

    show = TD_TRUE;
    if (ioctl(info->fd, FBIOPUT_SHOW_GFBG, &show) < 0) {
        sample_print("FBIOPUT_SHOW_GFBG failed!\n");
        return TD_FAILURE;
    }

    switch (info->color_format) {
        case OT_FB_FORMAT_ARGB8888:
            clr_fmt = OT_FB_FORMAT_ARGB8888;
            break;
        default:
            clr_fmt = OT_FB_FORMAT_ARGB1555;
            break;
    }
    /* only for G0 or G1 */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART01_H_ */
