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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART03_H_

    if (ioctl(info->fd, FBIOPUT_COLORKEY_GFBG, &color_key) < 0) {
        sample_print("FBIOPUT_COLORKEY_GFBG!\n");
        close(info->fd);
        info->fd = -1;
        return TD_FAILURE;
    }

    if (ioctl(info->fd, FBIOPUT_SCREEN_ORIGIN_GFBG, &point) < 0) {
        sample_print("set screen original show position failed!\n");
        close(info->fd);
        info->fd = -1;
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_init_var_ex(pthread_gfbg_sample_info *info)
{
    struct fb_var_screeninfo var;

    /* step 3. get the variable screen information */
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

    var.xres_virtual = WIDTH_1080P;
    var.yres_virtual = HEIGHT_1080P;
    var.xres = WIDTH_1080P;
    var.yres = HEIGHT_1080P;
    var.activate = FB_ACTIVATE_NOW;

    /* step 5. set the variable screen information */
    if (ioctl(info->fd, FBIOPUT_VSCREENINFO, &var) < 0) {
        sample_print("put variable screen info failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_init_layer_info(pthread_gfbg_sample_info *info)
{
    ot_fb_layer_info layer_info = {0};
    switch (info->ctrlkey) {
        case 0: /* 0 case */
            layer_info.buf_mode = OT_FB_LAYER_BUF_ONE;
            layer_info.mask = OT_FB_LAYER_MASK_BUF_MODE;
            break;
        case 1: /* 1 case */
            layer_info.buf_mode = OT_FB_LAYER_BUF_DOUBLE;
            layer_info.mask = OT_FB_LAYER_MASK_BUF_MODE;
            break;
        default:
            layer_info.buf_mode = OT_FB_LAYER_BUF_NONE;
            layer_info.mask = OT_FB_LAYER_MASK_BUF_MODE;
    }
    if (ioctl(info->fd, FBIOPUT_LAYER_INFO, &layer_info) < 0) {
        sample_print("PUT_LAYER_INFO failed!\n");
        close(info->fd);
        info->fd = -1;
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_init_compress(pthread_gfbg_sample_info *info)
{
    td_bool show = TD_TRUE;
    if (ioctl(info->fd, FBIOPUT_SHOW_GFBG, &show) < 0) {
        sample_print("FBIOPUT_SHOW_GFBG failed!\n");
        close(info->fd);
        info->fd = -1;
        return TD_FAILURE;
    }

    if (info->compress == TD_TRUE) {
        if (ioctl(info->fd, FBIOPUT_COMPRESSION_GFBG, &info->compress) < 0) {
            sample_print("FBIOPUT_COMPRESSION_GFBG failed!\n");
            close(info->fd);
            info->fd = -1;
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_init_canvas(pthread_gfbg_sample_info *info, ot_fb_buf *canvas_buf, td_void **buf,
    td_void **viraddr)
{
    td_u32 byte_per_pixel;
    ot_fb_color_format clr_fmt;

    switch (info->color_format) {
        case OT_FB_FORMAT_ARGB8888:
            byte_per_pixel = 4; /* 4 bytes */
            clr_fmt = OT_FB_FORMAT_ARGB8888;
            break;
        default:
            byte_per_pixel = 2; /* 2 bytes */
            clr_fmt = OT_FB_FORMAT_ARGB1555;
            break;
    }

    if (ss_mpi_sys_mmz_alloc(&g_canvas_addr, buf, TD_NULL, TD_NULL, WIDTH_1080P * HEIGHT_1080P *
        (byte_per_pixel)) == TD_FAILURE) {
        sample_print("allocate memory (max_w*max_h*%d bytes) failed\n", byte_per_pixel);
        close(info->fd);
        info->fd = -1;
        return TD_FAILURE;
    }
    canvas_buf->canvas.phys_addr = g_canvas_addr;
    canvas_buf->canvas.height = HEIGHT_1080P;
    canvas_buf->canvas.width = WIDTH_1080P;
    canvas_buf->canvas.pitch = WIDTH_1080P * (byte_per_pixel);
    canvas_buf->canvas.format = clr_fmt;
    if (memset_s(*buf, WIDTH_1080P * HEIGHT_1080P * (byte_per_pixel), 0x00, canvas_buf->canvas.pitch *
        canvas_buf->canvas.height) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        ss_mpi_sys_mmz_free(g_canvas_addr, *buf);
        g_canvas_addr = 0;
        close(info->fd);
        return TD_FAILURE;
    }

    /* change bmp */
    if (ss_mpi_sys_mmz_alloc(&g_phyaddr, viraddr, TD_NULL, TD_NULL, SAMPLE_IMAGE_WIDTH * SAMPLE_IMAGE_HEIGHT *
        byte_per_pixel) == TD_FAILURE) {
        sample_print("allocate memory (max_w*max_h*%d bytes) failed\n", byte_per_pixel);
        ss_mpi_sys_mmz_free(g_canvas_addr, *buf);
        g_canvas_addr = 0;
        close(info->fd);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_draw_line_by_cpu_ex(pthread_gfbg_sample_info *info, ot_fb_buf *canvas_buf, td_void *buf)
{
    td_u32 x;
    td_u32 y;
    td_u32 color;
    td_s32 ret;
    ot_fb_color_format clr_fmt;

    switch (info->color_format) {
        case OT_FB_FORMAT_ARGB8888:
            clr_fmt = OT_FB_FORMAT_ARGB8888;
            color = GFBG_RED_8888;
            break;
        default:
            clr_fmt = OT_FB_FORMAT_ARGB1555;
            color = GFBG_RED_1555;
            break;
    }
    for (y = (HEIGHT_1080P / 2 - 2); y < (HEIGHT_1080P / 2 + 2); y++) { /* 2 alg data */
        for (x = 0; x < WIDTH_1080P; x++) {
            if (clr_fmt == OT_FB_FORMAT_ARGB8888) {
                *((td_u32*)buf + y * WIDTH_1080P + x) = color;
            } else {
                *((td_u16*)buf + y * WIDTH_1080P + x) = color;
            }
        }
    }
    for (y = 0; y < HEIGHT_1080P; y++) {
        for (x = (WIDTH_1080P / 2 - 2); x < (WIDTH_1080P / 2 + 2); x++) { /* 2 alg data */
            if (clr_fmt == OT_FB_FORMAT_ARGB8888) {
                *((td_u32*)buf + y * WIDTH_1080P + x) = color;
            } else {
                *((td_u16*)buf + y * WIDTH_1080P + x) = color;
            }
        }
    }
    canvas_buf->update_rect.x = 0;
    canvas_buf->update_rect.y = 0;
    canvas_buf->update_rect.width = WIDTH_1080P;
    canvas_buf->update_rect.height = HEIGHT_1080P;
    ret = ioctl(info->fd, FBIO_REFRESH, canvas_buf);
    if (ret < 0) {
        sample_print("REFRESH failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void sample_source_cfg_tde(pthread_gfbg_sample_info *info, ot_tde_surface *src_surface, ot_tde_rect *src_rect,
    ot_tde_surface *dst_surface, ot_tde_rect *dst_rect)
{
    td_u32 byte_per_pixel;
    ot_tde_color_format tde_clr_fmt;

    switch (info->color_format) {
        case OT_FB_FORMAT_ARGB8888:
            tde_clr_fmt = OT_TDE_COLOR_FORMAT_ARGB8888;
            byte_per_pixel = 4; /* 4 bytes */
            break;
        default:
            tde_clr_fmt = OT_TDE_COLOR_FORMAT_ARGB1555;
            byte_per_pixel = 2; /* 2 bytes */
            break;
    }
    src_rect->pos_x   = 0;
    src_rect->pos_y   = 0;
    src_rect->height = SAMPLE_IMAGE_HEIGHT;
    src_rect->width  = SAMPLE_IMAGE_WIDTH;
    dst_rect->pos_x   = 0;
    dst_rect->pos_y   = 0;
    dst_rect->height = src_rect->height;
    dst_rect->width  = src_rect->width;

    dst_surface->color_format = tde_clr_fmt;
    dst_surface->width = WIDTH_1080P;
    dst_surface->height = HEIGHT_1080P;
    dst_surface->stride  = WIDTH_1080P * byte_per_pixel;

    src_surface->color_format = tde_clr_fmt;
    src_surface->width = SAMPLE_IMAGE_WIDTH;
    src_surface->height = SAMPLE_IMAGE_HEIGHT;
    src_surface->stride = byte_per_pixel * SAMPLE_IMAGE_WIDTH;
    src_surface->phys_addr = g_phyaddr;
    src_surface->support_alpha_ex_1555 = TD_TRUE;
    src_surface->alpha_max_is_255 = TD_TRUE;
    src_surface->alpha0 = 0XFF;
    src_surface->alpha1 = 0XFF;
    return;
}

static td_s32 sample_draw_picture_by_tde(pthread_gfbg_sample_info *info, ot_fb_buf *canvas_buf, td_u32 i,
    td_void *viraddr)
{
    td_char image_name[128]; /* 128 for char length */
    td_u8 *dst = TD_NULL;
    ot_tde_rect src_rect = {0};
    ot_tde_rect dst_rect = {0};
    ot_tde_surface src_surface = {0};
    ot_tde_surface dst_surface = {0};
    td_s32 handle;
    ot_tde_single_src single_src = {0};
    td_s32 ret;

    if (snprintf_s(image_name, sizeof(image_name), 12, SAMPLE_IMAGE1_PATH, i % 2) == -1) { /* 12 2 char length */
        sample_print("%s:%d:snprintf_s failed.\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    dst = (td_u8*)viraddr;
    if (sample_gfbg_load_bmp(image_name, dst) != TD_SUCCESS) {
        sample_print("sample_gfbg_load_bmp failed!\n");
        return TD_FAILURE;
    }

    sample_source_cfg_tde(info, &src_surface, &src_rect, &dst_surface, &dst_rect);
    dst_surface.phys_addr = canvas_buf->canvas.phys_addr;

    /* 1. start job */
    handle = ss_tde_begin_job();
    if (handle == OT_ERR_TDE_INVALID_HANDLE) {
        sample_print("start job failed!\n");
        return TD_FAILURE;
    }
    single_src.src_surface = &src_surface;
    single_src.dst_surface = &dst_surface;
    single_src.src_rect = &src_rect;
    single_src.dst_rect = &dst_rect;
    ret = ss_tde_quick_copy(handle, &single_src);
    if (ret < 0) {
        sample_print("tde_quick_copy:%d failed,ret=0x%x!\n", __LINE__, ret);
        ss_tde_cancel_job(handle);
        return TD_FAILURE;
    }

    /* 3. submit job */
    ret = ss_tde_end_job(handle, TD_FALSE, TD_TRUE, 10); /* timeout 10s */
    if (ret < 0) {
        sample_print("line:%d,tde_end_job failed,ret=0x%x!\n", __LINE__, ret);
        ss_tde_cancel_job(handle);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_time_to_play_ex(pthread_gfbg_sample_info *info, ot_fb_buf *canvas_buf, td_void *buf,
    td_void *viraddr)
{
    td_s32 ret;
    td_u32 i;

    ret = ss_tde_open();
    if (ret < 0) {
        sample_print("tde_open failed :%d!\n", ret);
        return TD_FAILURE;
    }

    sample_print("[begin]\n");
    sample_print("expected:two red line!\n");
    /* time to play */
    for (i = 0; i < SAMPLE_IMAGE_NUM; i++) {
        if (g_exit_flag == 'q') {
            printf("process exit...\n");
            break;
        }
        /* draw two lines by cpu */
        if (sample_draw_line_by_cpu_ex(info, canvas_buf, buf) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        sleep(2); /* 2 second */

        if (sample_draw_picture_by_tde(info, canvas_buf, i, viraddr) != TD_SUCCESS) {
            return TD_FAILURE;
        }

        canvas_buf->update_rect.x = 0;
        canvas_buf->update_rect.y = 0;
        canvas_buf->update_rect.width = WIDTH_1080P;
        canvas_buf->update_rect.height = HEIGHT_1080P;
        ret = ioctl(info->fd, FBIO_REFRESH, canvas_buf);
        if (ret < 0) {
            sample_print("REFRESH failed!\n");
            return TD_FAILURE;
        }
        sleep(2); /* 2 second */
    }
    return TD_SUCCESS;
}

static td_void* sample_gfbg_refresh(td_void *data)
{
    td_char file[FILE_LENGTH_MAX] = {0};
    ot_fb_buf canvas_buf;
    td_void *buf = TD_NULL;
    td_void *viraddr = TD_NULL;
    pthread_gfbg_sample_info *info = TD_NULL;

    prctl(PR_SET_NAME, "GFBG_REFRESH", 0, 0, 0);
    if (data == TD_NULL) {
        return TD_NULL;
    }
    info = (pthread_gfbg_sample_info*)data;

    if (sample_get_file_name(info, file, FILE_LENGTH_MAX) != TD_SUCCESS) {
        return TD_NULL;
    }

    if (sample_init_frame_buffer_ex(info, file) != TD_SUCCESS) {
        return TD_NULL;
    }

    if (sample_init_var_ex(info) != TD_SUCCESS) {
        close(info->fd);
        info->fd = -1;
        return TD_NULL;
    }

    if (sample_init_layer_info(info) != TD_SUCCESS) {
        return TD_NULL;
    }

    if (sample_init_compress(info) != TD_SUCCESS) {
        return TD_NULL;
    }

    if (sample_init_canvas(info, &canvas_buf, &buf, &viraddr) != TD_SUCCESS) {
        return TD_NULL;
    }

    if (sample_time_to_play_ex(info, &canvas_buf, buf, viraddr) != TD_SUCCESS) {
        goto ERR;
    }
    sample_print("[end]\n");
ERR:
    ss_mpi_sys_mmz_free(g_phyaddr, viraddr);
    g_phyaddr = 0;
    ss_mpi_sys_mmz_free(g_canvas_addr, buf);
    g_canvas_addr = 0;
    close(info->fd);
    return TD_NULL;
}

static td_s32 sample_bind_layer_to_vo(ot_vo_layer layer, ot_vo_dev dev)
{
    td_s32 ret;
    ret = ss_mpi_vo_unbind_layer(layer, dev);
    if (ret != TD_SUCCESS) {
        sample_print("unbind failed!\n");
        return ret;
    }
    ret = ss_mpi_vo_bind_layer(layer, dev);
    if (ret != TD_SUCCESS) {
        sample_print("bind failed!\n");
        return ret;
    }
    return ret;
}

static td_s32 sample_var_init(pthread_gfbg_sample_info *info, struct fb_var_screeninfo *var)
{
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    /* step 1. open framebuffer */
    info->fd = open("/dev/fb2", O_RDWR, 0);
    if (info->fd < 0) {
        sample_print("open /dev/fb2 failed!\n");
        return TD_FAILURE;
    }
    /* step 2. get the variable screen information */
    if (ioctl(info->fd, FBIOGET_VSCREENINFO, var) < 0) {
        sample_print("get variable screen info failed!\n");
        close(info->fd);
        return TD_FAILURE;
    }
    /* step 3. modify the variable screen info */
    switch (info->color_format) {
        case OT_FB_FORMAT_4BPP:


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART03_H_ */
