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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART06_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART06_H_

#ifdef CONFIG_SUPPORT_SAMPLE_ROTATION
static td_void gfbg_rotate(ot_vo_dev vo_dev)
{
    td_s32 ret;
    pthread_gfbg_sample_info info;

    info.layer = vo_dev;
    info.fd = -1;
    info.ctrlkey = 1; /* 0 : one buf 1: double buf */
    info.compress = TD_FALSE;
    info.color_format = OT_FB_FORMAT_ARGB1555;

    if (vo_dev == SAMPLE_VO_DEV_DHD0) {
        info.fd = open("/dev/fb0", O_RDWR, 0);
        if (info.fd < 0) {
            sample_print("open /dev/fb0 failed!\n");
            return;
        }
    } else {
        info.fd = open("/dev/fb1", O_RDWR, 0);
        if (info.fd < 0) {
            sample_print("open /dev/fb1 failed!\n");
            return;
        }
    }

    ret = gfbg_get_canvas(&info);
    if (ret != TD_SUCCESS) {
        close(info.fd);
        return;
    }

    ret = gfbg_draw(&info);
    if (ret != TD_SUCCESS) {
        gfbg_put_canvas();
        close(info.fd);
        return;
    }

    ret = gfbg_put_rotation(&info);
    if (ret != TD_SUCCESS) {
        gfbg_put_canvas();
        close(info.fd);
        return;
    }

    ret = gfbg_refresh(&info);
    if (ret != TD_SUCCESS) {
        gfbg_put_canvas();
        close(info.fd);
        return;
    }

    gfbg_put_canvas();
    close(info.fd);

    return;
}

static td_s32 gfbg_put_layer_info(pthread_gfbg_sample_info *info)
{
    td_s32 ret;
    ot_fb_layer_info layer_info = {0};

    ret = ioctl(info->fd, FBIOGET_LAYER_INFO, &layer_info);
    if (ret < 0) {
        sample_print("GET_LAYER_INFO failed!\n");
        return TD_FAILURE;
    }
    layer_info.mask = 0;
    layer_info.antiflicker_level = OT_FB_LAYER_ANTIFLICKER_AUTO;
    layer_info.mask |= OT_FB_LAYER_MASK_BUF_MODE;
    layer_info.mask |= OT_FB_LAYER_MASK_ANTIFLICKER_MODE;
    switch (info->ctrlkey) {
        case 0: /* 0 one buf */
            layer_info.buf_mode = OT_FB_LAYER_BUF_ONE;
            break;
        case 1: /* 1 double buf */
            layer_info.buf_mode = OT_FB_LAYER_BUF_DOUBLE;
            break;
        default:
            layer_info.buf_mode = OT_FB_LAYER_BUF_NONE;
    }

    ret = ioctl(info->fd, FBIOPUT_LAYER_INFO, &layer_info);
    if (ret < 0) {
        sample_print("PUT_LAYER_INFO failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_get_canvas(pthread_gfbg_sample_info *info)
{
    td_s32 ret;
    td_u32 byte_per_pixel = (info->color_format == OT_FB_FORMAT_ARGB8888) ? 4 : 2; /* 4 2 bytes per pixel */
    ret = ss_mpi_sys_mmz_alloc(&g_canvas_phy, ((td_void**)&g_canvas_vir), TD_NULL, TD_NULL, WIDTH_800 *
        HEIGHT_600 * byte_per_pixel * 2); /* 2 double canvas buf */
    if (ret != TD_SUCCESS) {
        sample_print("allocate memory (maxW*maxH*%d bytes) failed\n", byte_per_pixel);
        return TD_FAILURE;
    }

    if ((g_canvas_phy == 0) || (g_canvas_vir == NULL)) {
        return TD_FAILURE;
    }
    (td_void)memset_s(g_canvas_vir, WIDTH_800 * HEIGHT_600 * byte_per_pixel * 2, 0xff, /* 2 double canvas buf */
        WIDTH_800 * HEIGHT_600 * byte_per_pixel * 2); /* 2 double canvas buf */
    ret = ss_mpi_sys_mmz_alloc(&g_picture_phy, ((td_void**)&g_picture_vir), NULL, NULL, SAMPLE_IMAGE_WIDTH *
        SAMPLE_IMAGE_HEIGHT * byte_per_pixel);
    if (ret != TD_SUCCESS) {
        sample_print("allocate memory (maxW*maxH*%d bytes) failed\n", byte_per_pixel);
        return TD_FAILURE;
    }
    if ((g_picture_phy == 0) || (g_picture_vir == NULL)) {
        return TD_FAILURE;
    }
    (td_void)memset_s(g_picture_vir, SAMPLE_IMAGE_WIDTH * SAMPLE_IMAGE_HEIGHT * byte_per_pixel, 0xff,
        SAMPLE_IMAGE_WIDTH * SAMPLE_IMAGE_HEIGHT * byte_per_pixel);
    return TD_SUCCESS;
}

static td_void gfbg_put_canvas(td_void)
{
    if ((g_canvas_phy == 0) || (g_canvas_vir == NULL) || (g_picture_phy == 0) || (g_picture_vir == NULL)) {
        return;
    }

    ss_mpi_sys_mmz_free(g_canvas_phy, g_canvas_vir);
    g_canvas_phy = 0;
    g_canvas_vir = NULL;
    ss_mpi_sys_mmz_free(g_picture_phy, g_picture_vir);
    g_picture_phy = 0;
    g_picture_vir = NULL;

    return;
}

static td_void gfbg_init_surface(ot_tde_surface *src_surface, ot_tde_surface *dst_surface,
    ot_tde_rect *src_rect, ot_tde_rect *dst_rect, td_u32 byte_per_pixel)
{
    src_rect->pos_x = dst_rect->pos_x = 0;
    src_rect->pos_y = dst_rect->pos_y = 0;
    src_rect->height = dst_rect->height = SAMPLE_IMAGE_HEIGHT;
    src_rect->width = dst_rect->width = SAMPLE_IMAGE_WIDTH;
    src_surface->color_format = dst_surface->color_format = OT_TDE_COLOR_FORMAT_ARGB1555;
    src_surface->width = SAMPLE_IMAGE_WIDTH;
    src_surface->height = SAMPLE_IMAGE_HEIGHT;
    src_surface->stride = byte_per_pixel * SAMPLE_IMAGE_WIDTH;
    src_surface->phys_addr = g_picture_phy;
    src_surface->support_alpha_ex_1555 = dst_surface->support_alpha_ex_1555 = TD_TRUE;
    src_surface->alpha_max_is_255 = dst_surface->alpha_max_is_255 = TD_TRUE;
    src_surface->alpha0 = dst_surface->alpha0 = 0XFF;
    src_surface->alpha1 = dst_surface->alpha1 = 0XFF;
    dst_surface->width = WIDTH_800;
    dst_surface->height = HEIGHT_600;
    dst_surface->stride = WIDTH_800 * byte_per_pixel;
    dst_surface->phys_addr = g_canvas_phy;
    dst_surface->is_ycbcr_clut = TD_FALSE;
    return;
}

static td_s32 gfbg_draw_by_tde(ot_tde_surface *src_surface, ot_tde_surface *dst_surface,
    ot_tde_rect *src_rect, ot_tde_rect *dst_rect, td_s32 index)
{
    td_s32 ret;
    td_s32 handle;
    ot_tde_single_src single_src = {0};
    if (index == 1) {
        sample_gfbg_load_bmp("./res/1.bmp", g_picture_vir);
    } else {
        sample_gfbg_load_bmp("./res/0.bmp", g_picture_vir);
        dst_surface->phys_addr = g_canvas_phy + dst_surface->stride * dst_surface->height;
    }

    /* 1. start job */
    handle = ss_tde_begin_job();
    if (handle == OT_ERR_TDE_INVALID_HANDLE) {
        sample_print("start job failed!\n");
        return TD_FAILURE;
    }
    single_src.src_surface = src_surface;
    single_src.src_rect = src_rect;
    single_src.dst_surface = dst_surface;
    single_src.dst_rect = dst_rect;
    ret = ss_tde_quick_copy(handle, &single_src);
    if (ret < 0) {
        sample_print("tde_quick_copy:%d failed,ret=0x%x!\n", __LINE__, ret);
        ss_tde_cancel_job(handle);
        return TD_FAILURE;
    }

    /* 3. submit job */
    ret = ss_tde_end_job(handle, TD_FALSE, TD_TRUE, 10000); /* 10000 timeout */
    if (ret < 0) {
        sample_print("Line:%d,tde_end_job failed,ret=0x%x!\n", __LINE__, ret);
        ss_tde_cancel_job(handle);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_draw(pthread_gfbg_sample_info *info)
{
    td_s32 ret;
    ot_tde_rect dst_rect;
    ot_tde_rect src_rect;
    ot_tde_surface dst_surface = {0};
    ot_tde_surface src_surface = {0};
    td_u32 byte_per_pixel = (info->color_format == OT_FB_FORMAT_ARGB8888) ? 4 : 2; /* 4 2 bytes per pixel */

    ret = ss_tde_open();
    if (ret < 0) {
        sample_print("tde_open failed :%d!\n", ret);
        return TD_FAILURE;
    }

    gfbg_init_surface(&src_surface, &dst_surface, &src_rect, &dst_rect, byte_per_pixel);

    ret = gfbg_draw_by_tde(&src_surface, &dst_surface, &src_rect, &dst_rect, 1);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    ret = gfbg_draw_by_tde(&src_surface, &dst_surface, &src_rect, &dst_rect, 0);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ss_tde_close();
    return TD_SUCCESS;
}

static td_s32 gfbg_refresh(const pthread_gfbg_sample_info *info)
{
    td_s32 ret;
    ot_fb_buf canvas_buf;
    td_u32 byte_per_pixel;
    td_s32 i;

    switch (info->color_format) {
        case OT_FB_FORMAT_ARGB8888:
            byte_per_pixel = 4; /* 4 bytes */
            break;
        default:
            byte_per_pixel = 2; /* 2 bytes */
            break;
    }
    canvas_buf.canvas.phys_addr = g_canvas_phy;
    canvas_buf.canvas.height = HEIGHT_600;
    canvas_buf.canvas.width = WIDTH_800;
    canvas_buf.canvas.pitch = WIDTH_800 * (byte_per_pixel);
    canvas_buf.canvas.format = info->color_format;
    canvas_buf.update_rect.x = 0;
    canvas_buf.update_rect.y = 0;
    canvas_buf.update_rect.width = WIDTH_800;
    canvas_buf.update_rect.height = HEIGHT_600;

    for (i = 0; i < 10; i++) { /* 10 times */
        if (i % 2 != 0) { /* 2 change 0.bmp or 1.bmp */
            canvas_buf.canvas.phys_addr = g_canvas_phy + canvas_buf.canvas.pitch * canvas_buf.canvas.height;
        } else {
            canvas_buf.canvas.phys_addr = g_canvas_phy;
        }
        ret = ioctl(info->fd, FBIO_REFRESH, &canvas_buf);
        if (ret < 0) {
            sample_print("REFRESH failed!\n");
            return TD_FAILURE;
        }

        sample_print("wait 1 seconds\n");
        usleep(1 * 1000 * 1000); /* 1000 1seconds */
    }

    return TD_SUCCESS;
}

static td_void gfbg_get_var_by_format(pthread_gfbg_sample_info *info, struct fb_var_screeninfo *var_info)
{
    switch (info->color_format) {
        case OT_FB_FORMAT_ARGB8888:
            var_info->transp = g_a32;
            var_info->red    = g_r32;
            var_info->green  = g_g32;
            var_info->blue   = g_b32;
            var_info->bits_per_pixel = 32; /* 32 bits */
            break;
        default:
            var_info->transp = g_a16;
            var_info->red    = g_r16;
            var_info->green  = g_g16;
            var_info->blue   = g_b16;
            var_info->bits_per_pixel = 16; /* 16 bits */
            break;
    }
}

static td_void gfbg_put_rotation_degree(struct fb_var_screeninfo *var_info, ot_fb_rotate_mode *rotate_mode)
{
    td_char input;
    printf("\n\n/****************index******************/\n");
    printf("please input 1 or 2 or 3 to choose the case which you want to run:\n");
    printf("\t1:  rotate 90\n");
    printf("\t2:  rotate 180\n");
    printf("\t3:  rotate 270\n");

    input = sample_gfbg_getchar();
    if (input == '1') {
        var_info->xres = var_info->xres_virtual = HEIGHT_600;
        var_info->yres = var_info->yres_virtual = WIDTH_800;
        *rotate_mode = OT_FB_ROTATE_90;
    } else if (input == '2') {
        var_info->xres = var_info->xres_virtual = WIDTH_800;
        var_info->yres = var_info->yres_virtual = HEIGHT_600;
        *rotate_mode = OT_FB_ROTATE_180;
    } else if (input == '3') {
        var_info->xres = var_info->xres_virtual = HEIGHT_600;
        var_info->yres = var_info->yres_virtual = WIDTH_800;
        *rotate_mode = OT_FB_ROTATE_270;
    } else {
        var_info->xres = var_info->xres_virtual = WIDTH_800;
        var_info->yres = var_info->yres_virtual = HEIGHT_600;
        *rotate_mode = OT_FB_ROTATE_NONE;
        sample_print("input param invalid, no rotate!\n");
    }
}

static td_s32 gfbg_put_rotation(pthread_gfbg_sample_info *info)
{
    td_s32 ret;
    struct fb_var_screeninfo var_info = {0};
    ot_fb_rotate_mode rotate_mode = OT_FB_ROTATE_BUTT;

    ret = ioctl(info->fd, FBIOGET_VSCREENINFO, &var_info);
    if (ret < 0) {
        sample_print("FBIOGET_VSCREENINFO failed!\n");
        return TD_FAILURE;
    }

    gfbg_get_var_by_format(info, &var_info);

    gfbg_put_rotation_degree(&var_info, &rotate_mode);

    ret = ioctl(info->fd, FBIOPUT_VSCREENINFO, &var_info);
    if (ret < 0) {
        sample_print("PUT_VSCREENINFO failed!\n");
        return TD_FAILURE;
    }

    ret = gfbg_put_layer_info(info);
    if (ret != TD_SUCCESS) {
        sample_print("PUT_LAYER_INFO failed!\n");
        return TD_FAILURE;
    }

    ret = ioctl(info->fd, FBIOPUT_ROTATE_MODE, &rotate_mode);
    if (ret < 0) {
        sample_print("rotate failed!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_gfbg_rotation(vo_device_info *vo_dev_info)
{
    td_s32 ret;
    ot_vb_cfg vb_conf;

    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        goto sample_gfbg_rotation_0;
    }

    /* open display */
    ret = sample_gfbg_start_vo(vo_dev_info);
    if (ret != TD_SUCCESS) {
        sample_print("VO device %d start failed\n", vo_dev_info->vo_dev);
        goto sample_gfbg_rotation_0;
    }

    gfbg_rotate(vo_dev_info->vo_dev);

    sample_gfbg_stop_vo(vo_dev_info);
sample_gfbg_rotation_0:
    sample_comm_sys_exit();
    return (ret);
}
#endif

static td_void sample_gfbg_to_exit_signal(td_void)
{
    printf("\033[0;31mreceive the signal,wait......!\033[0;39m\n");
    if (g_gfbg_thread1) {
        pthread_join(g_gfbg_thread1, 0);
        g_gfbg_thread1 = 0;
    }

    if (g_gfbg_thread) {
        pthread_join(g_gfbg_thread, 0);
        g_gfbg_thread = 0;
    }

    if (g_phyaddr) {
        ss_mpi_sys_mmz_free(g_phyaddr, TD_NULL);
        g_phyaddr = 0;
    }

    if (g_canvas_addr) {
        ss_mpi_sys_mmz_free(g_canvas_addr, TD_NULL);
        g_canvas_addr = 0;
    }

    if (g_canvas_phy) {
        ss_mpi_sys_mmz_free(g_canvas_phy, TD_NULL);
        g_canvas_phy = 0;
    }

    if (g_picture_phy) {
        ss_mpi_sys_mmz_free(g_picture_phy, TD_NULL);
        g_picture_phy = 0;
    }
    return;
}

static td_s32 sample_choose_the_case(char **argv, vo_device_info *vo_dev_info)


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART06_H_ */
