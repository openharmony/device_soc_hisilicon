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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART02_H_

    if ((info->layer != GRAPHICS_LAYER_G0) && (info->layer != GRAPHICS_LAYER_G1)) {
        return TD_SUCCESS;
    }
    for (i = 0; i < 1; i++) {
        if (i % 2) { /* 2 for 0 or 1 */
            var.yoffset = var.yres;
        } else {
            var.yoffset = 0;
        }
        ptemp = (show_screen + var.yres * fix_screen_stride * (i % 2)); /* 2 for 0 or 1 */
        sample_draw_rect_by_cpu(ptemp, clr_fmt, &var);
        /*
         * note : not acting on ARGB8888, for ARGB8888 format image's alpha, you can change ptemp[x][y]'s value
         * GFBG_RED_8888 = 0xffff00000 means alpha=255(show),red.0x00ff0000 means alpha=0,red(hide).
         */
        if (sample_put_alpha_and_colorkey(info, clr_fmt) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        sleep(2); /* 2 second */
    }
    return TD_SUCCESS;
}

static td_s32 sample_quick_copy_by_tde(td_u32 byte_per_pixel, ot_tde_color_format tde_clr_fmt,
    td_u32 fix_screen_stride, td_u64 hide_screen_phy)
{
    td_s32 handle;
    ot_tde_rect src_rect = {0};
    ot_tde_rect dst_rect = {0};
    ot_tde_surface src_surface = {0};
    ot_tde_surface dst_surface = {0};
    ot_tde_single_src single_src = {0};

    /* TDE job step 0. open tde */
    src_rect.pos_x = dst_rect.pos_x = 0;
    src_rect.pos_y = dst_rect.pos_y = 0;
    src_rect.height = SAMPLE_IMAGE_HEIGHT;
    src_rect.width = SAMPLE_IMAGE_WIDTH;
    dst_rect.height = src_rect.height;
    dst_rect.width = src_rect.width;

    dst_surface.color_format = tde_clr_fmt;
    dst_surface.width = WIDTH_1080P;
    dst_surface.height = HEIGHT_1080P;
    dst_surface.stride = fix_screen_stride;
    dst_surface.phys_addr = (td_phys_addr_t)hide_screen_phy;

    src_surface.color_format = tde_clr_fmt;
    src_surface.width = SAMPLE_IMAGE_WIDTH;
    src_surface.height = SAMPLE_IMAGE_HEIGHT;
    src_surface.stride = byte_per_pixel * SAMPLE_IMAGE_WIDTH;
    src_surface.phys_addr = g_phyaddr;
    src_surface.support_alpha_ex_1555 = TD_TRUE;
    src_surface.alpha_max_is_255  = TD_TRUE;
    src_surface.alpha0 = 0XFF;
    src_surface.alpha1 = 0XFF;

    /* TDE job step 1. start job */
    handle = ss_tde_begin_job();
    if (handle == OT_ERR_TDE_INVALID_HANDLE) {
        return TD_FAILURE;
    }
    single_src.src_surface = &src_surface;
    single_src.dst_surface = &dst_surface;
    single_src.src_rect = &src_rect;
    single_src.dst_rect = &dst_rect;
    if (ss_tde_quick_copy(handle, &single_src) < 0) {
        sample_print("tde_quick_copy:%d failed!\n", __LINE__);
        ss_tde_cancel_job(handle);
        return TD_FAILURE;
    }

    /* TDE job step 2. submit job */
    if (ss_tde_end_job(handle, TD_FALSE, TD_TRUE, 10) < 0) { /* 10 timeout */
        sample_print("line:%d,tde_end_job failed!\n", __LINE__);
        ss_tde_cancel_job(handle);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void sample_get_param(pthread_gfbg_sample_info *info, ot_fb_color_format *clr_fmt,
    ot_tde_color_format *tde_clr_fmt, td_u32 *color)
{
    switch (info->color_format) {
        case OT_FB_FORMAT_ARGB8888:
            *clr_fmt = OT_FB_FORMAT_ARGB8888;
            *color = GFBG_RED_8888;
            *tde_clr_fmt = OT_TDE_COLOR_FORMAT_ARGB8888;
            break;
        default:
            *clr_fmt = OT_FB_FORMAT_ARGB1555;
            *color = GFBG_RED_1555;
            *tde_clr_fmt = OT_TDE_COLOR_FORMAT_ARGB1555;
            break;
    }
    return;
}

static td_s32 sample_draw_line_by_cpu(pthread_gfbg_sample_info *info, td_u8 *show_screen, td_u32 fix_screen_stride,
    td_u64 *hide_screen_phy, td_s32 i)
{
    td_s32 x;
    td_s32 y;
    td_u8 *hide_screen = TD_NULL;
    td_void *show_line = TD_NULL;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    ot_fb_color_format clr_fmt;
    ot_tde_color_format tde_clr_fmt = OT_TDE_COLOR_FORMAT_ARGB1555;
    td_u32 color = GFBG_RED_1555;

    if (ioctl(info->fd, FBIOGET_VSCREENINFO, &var) < 0) {
        sample_print("get variable screen info failed!\n");
        return TD_FAILURE;
    }

    if (ioctl(info->fd, FBIOGET_FSCREENINFO, &fix) < 0) {
        return TD_FAILURE;
    }

    sample_get_param(info, &clr_fmt, &tde_clr_fmt, &color);
    /* TDE step1: draw two red line */
    var.yoffset = (i % 2 == 1) ? var.yres : 0; /* 2 for 0 1 */

    hide_screen = show_screen + (fix_screen_stride * var.yres) * (i % 2); /* 2 for 0 1 */
    if  (memset_s(hide_screen, fix_screen_stride * var.yres, 0x00, fix_screen_stride * var.yres) != EOK) {
        return TD_FAILURE;
    }
    *hide_screen_phy = fix.smem_start + (i % 2) * fix_screen_stride * var.yres; /* 2 for 0 1 */
    show_line = hide_screen;
    for (y = (HEIGHT_1080P / 2 - 2); y < (HEIGHT_1080P / 2 + 2); y++) { /* 2 for alg data */
        for (x = 0; x < WIDTH_1080P; x++) {
            if (clr_fmt == OT_FB_FORMAT_ARGB8888) {
                *((td_u32*)show_line + y * var.xres + x) = color;
            } else {
                *((td_u16*)show_line + y * var.xres + x) = color;
            }
        }
    }
    for (y = 0; y < HEIGHT_1080P; y++) {
        for (x = (WIDTH_1080P / 2 - 2); x < (WIDTH_1080P / 2 + 2); x++) { /* 2 for alg data */
            if (clr_fmt == OT_FB_FORMAT_ARGB8888) {
                *((td_u32*)show_line + y * var.xres + x) = color;
            } else {
                *((td_u16*)show_line + y * var.xres + x) = color;
            }
        }
    }
    if (ioctl(info->fd, FBIOPAN_DISPLAY, &var) < 0) {
            sample_print("FBIOPAN_DISPLAY failed!\n");
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_draw_line_and_picture(pthread_gfbg_sample_info *info, td_void *viraddr, td_u8 *show_screen,
    td_u32 fix_screen_stride)
{
    td_s32 i;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    td_u64 hide_screen_phy = 0;
    td_u32 byte_per_pixel;
    ot_fb_color_format clr_fmt;
    ot_tde_color_format tde_clr_fmt = OT_TDE_COLOR_FORMAT_ARGB1555;
    td_u32 color = GFBG_RED_1555;
    td_char image_name[128]; /* 128 for char length */
    td_u8 *dst = TD_NULL;

    sample_get_param(info, &clr_fmt, &tde_clr_fmt, &color);

    if (ioctl(info->fd, FBIOGET_VSCREENINFO, &var) < 0) {
        return TD_FAILURE;
    }
    byte_per_pixel = var.bits_per_pixel / 8; /* 8 for 1 byte */

    if (ioctl(info->fd, FBIOGET_FSCREENINFO, &fix) < 0) {
        return TD_FAILURE;
    }

    sample_print("expected:two red line!\n");
    for (i = 0; i < SAMPLE_IMAGE_NUM; i++) {
        if (g_exit_flag == 'q') {
            printf("process exit...\n");
            break;
        }

        if (sample_draw_line_by_cpu(info, show_screen, fix_screen_stride, &hide_screen_phy, i) != TD_SUCCESS) {
            return TD_FAILURE;
        }

        /* TDE step2: draw gui picture */
        if (snprintf_s(image_name, sizeof(image_name), 12, SAMPLE_IMAGE1_PATH, i % 2) == -1) { /* 12 2 length */
            return TD_FAILURE;
        }
        dst = (td_u8*)viraddr;
        if (sample_gfbg_load_bmp(image_name, dst) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if (sample_quick_copy_by_tde(byte_per_pixel, tde_clr_fmt, fix_screen_stride, hide_screen_phy) !=
            TD_SUCCESS) {
            return TD_FAILURE;
        }

        var.yoffset = (i % 2 == 1) ? var.yres : 0; /* 2 for 0 1 */
        if (ioctl(info->fd, FBIOPAN_DISPLAY, &var) < 0) {
            sample_print("FBIOPAN_DISPLAY failed!\n");
            return TD_FAILURE;
        }
        sleep(1);
    }
    return TD_SUCCESS;
}

static td_s32 sample_move_cursor(pthread_gfbg_sample_info *info, struct fb_var_screeninfo *var, td_u8 *show_screen)
{
    td_s32 i;
    ot_fb_point point = {0, 0};

    point.x_pos = (info->ctrlkey == 3) ? 150 : 0; /* 3 150:for case;alg data */
    point.y_pos = (info->ctrlkey == 3) ? 150 : 0; /* 3 150:for case;alg data */

    if (sample_gfbg_load_bmp(SAMPLE_CURSOR_PATH, show_screen) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (ioctl(info->fd, FBIOPAN_DISPLAY, var) < 0) {
        sample_print("FBIOPAN_DISPLAY failed!\n");
        return TD_FAILURE;
    }
    sample_print("show cursor\n");
    sleep(2); /* 2 second */
    for (i = 0; i < 100; i++) { /* 100 for times */
        if (g_exit_flag == 'q') {
            printf("process exit...\n");
            break;
        }
        point.x_pos += 2; /* 2 pos */
        point.y_pos += 2; /* 2 pos */
        if (ioctl(info->fd, FBIOPUT_SCREEN_ORIGIN_GFBG, &point) < 0) {
            sample_print("set screen original show position failed!\n");
            return TD_FAILURE;
        }
        usleep(70 * 1000); /* 70 1000 for sleep */
    }
    for (i = 0; i < 100; i++) { /* 100 for times */
        if (g_exit_flag == 'q') {
            printf("process exit...\n");
            break;
        }
        point.x_pos += 2; /* 2 pos */
        point.y_pos -= 2; /* 2 pos */
        if (ioctl(info->fd, FBIOPUT_SCREEN_ORIGIN_GFBG, &point) < 0) {
            sample_print("set screen original show position failed!\n");
            return TD_FAILURE;
        }
        usleep(70 * 1000); /* 70 1000 for sleep */
    }
    sample_print("move the cursor\n");
    sleep(1);
    return TD_SUCCESS;
}

static td_s32 sample_show_bitmap(pthread_gfbg_sample_info *info, td_u8 *show_screen, td_u32 fix_screen_stride,
    td_u32 byte_per_pixel)
{
    td_s32 ret;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    td_void *viraddr = TD_NULL;

    if (ioctl(info->fd, FBIOGET_VSCREENINFO, &var) < 0) {
        sample_print("get variable screen info failed!\n");
        return TD_FAILURE;
    }

    if (ioctl(info->fd, FBIOGET_FSCREENINFO, &fix) < 0) {
        sample_print("get fix screen info failed!\n");
        return TD_FAILURE;
    }

    switch (info->ctrlkey) {
        /* 2 means none buffer and just for pan display. */
        case 2:
            if (ss_mpi_sys_mmz_alloc(&g_phyaddr, ((void**)&viraddr), TD_NULL, TD_NULL, SAMPLE_IMAGE_WIDTH *
                SAMPLE_IMAGE_HEIGHT * byte_per_pixel) == TD_FAILURE) {
                sample_print("allocate memory (max_w*max_h*%d bytes) failed\n", byte_per_pixel);
                return TD_FAILURE;
            }
            ret = ss_tde_open();
            if (ret < 0) {
                sample_print("tde_open failed :%d!\n", ret);
                ss_mpi_sys_mmz_free(g_phyaddr, viraddr);
                g_phyaddr = 0;
                return TD_FAILURE;
            }
            if (sample_draw_line_and_picture(info, viraddr, show_screen, fix_screen_stride) != TD_SUCCESS) {
                ss_tde_close();
                ss_mpi_sys_mmz_free(g_phyaddr, viraddr);
                g_phyaddr = 0;
                return TD_FAILURE;
            }
            ss_mpi_sys_mmz_free(g_phyaddr, viraddr);
            g_phyaddr = 0;
            ss_tde_close();
            break;
        case 3: /* 3 mouse case */
            /* move cursor */
            if (sample_move_cursor(info, &var, show_screen) != TD_SUCCESS) {
                return TD_FAILURE;
            }
            break;
        default:
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_show_process(pthread_gfbg_sample_info *info)
{
    td_u8 *show_screen = TD_NULL;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    td_u32 fix_screen_stride;
    td_bool show;

    if (ioctl(info->fd, FBIOGET_VSCREENINFO, &var) < 0) {
        sample_print("get variable screen info failed!\n");
        goto ERR1;
    }

    if (ioctl(info->fd, FBIOGET_FSCREENINFO, &fix) < 0) {
        sample_print("get fix screen info failed!\n");
        goto ERR1;
    }
    fix_screen_stride = fix.line_length;
    show_screen = mmap(TD_NULL, fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, info->fd, 0);
    if (show_screen == MAP_FAILED) {
        sample_print("mmap framebuffer failed!\n");
        goto ERR1;
    }

    if (memset_s(show_screen, fix.smem_len, 0x0, fix.smem_len) != EOK) {
        goto ERR2;
    }

    if (sample_time_to_play(info, show_screen, fix_screen_stride) != TD_SUCCESS) {
        goto ERR2;
    }
    /* 8 for 1 byte */
    if (sample_show_bitmap(info, show_screen, fix_screen_stride, var.bits_per_pixel / 8) != TD_SUCCESS) {
        goto ERR2;
    }
    munmap(show_screen, fix.smem_len);
    show = TD_FALSE;
    if (ioctl(info->fd, FBIOPUT_SHOW_GFBG, &show) < 0) {
        sample_print("FBIOPUT_SHOW_GFBG failed!\n");
        close(info->fd);
        return TD_FAILURE;
    }
    close(info->fd);
    info->fd = -1;
    return TD_SUCCESS;

ERR2:
    munmap(show_screen, fix.smem_len);
    show_screen = TD_NULL;
ERR1:
    close(info->fd);
    info->fd = -1;
    return TD_FAILURE;
}

static td_void* sample_gfbg_pandisplay(td_void *data)
{
    pthread_gfbg_sample_info *info = TD_NULL;
    td_char thdname[64]; /* 64 for char length */
    td_char file[FILE_LENGTH_MAX] = {0};

    if (data == TD_NULL) {
        return TD_NULL;
    }
    info = (pthread_gfbg_sample_info*)data;
    if (snprintf_s(thdname, sizeof(thdname), 17, "GFBG%d_pandisplay", info->layer) == -1) { /* 17 for char length */
        printf("%s:%d:snprintf_s failed.\n", __FUNCTION__, __LINE__);
        return TD_NULL;
    }
    prctl(PR_SET_NAME, thdname, 0, 0, 0);

    if (sample_get_file_name(info, file, FILE_LENGTH_MAX) != TD_SUCCESS) {
        return TD_NULL;
    }

    if (sample_init_frame_buffer(info, file) != TD_SUCCESS) {
        return TD_NULL;
    }

    if (sample_init_var(info) != TD_SUCCESS) {
        close(info->fd);
        info->fd = -1;
        return TD_NULL;
    }

    /* map the physical video memory for user use */
    if (sample_show_process(info) != TD_SUCCESS) {
        return TD_NULL;
    }
    sample_print("[end]\n");
    return TD_NULL;
}

static td_s32 sample_init_frame_buffer_ex(pthread_gfbg_sample_info *info, const char *input_file)
{
    ot_fb_colorkey color_key;
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

    /* all layer support colorkey */
    color_key.enable = TD_TRUE;
    color_key.value = 0x0;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART02_H_ */
