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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART04_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART04_H_

            var->transp = g_a4;
            var->red    = g_r4;
            var->green  = g_g4;
            var->blue   = g_b4;
            var->bits_per_pixel = 4; /* 4 bits per pixel */
            var->xres_virtual = WIDTH_1920;
            var->yres_virtual = HEIGHT_1080 * 2; /* alloc 2 buf */
            var->xres = WIDTH_1920;
            var->yres = HEIGHT_1080;
            var->activate = 0;
            var->xoffset = 0;
            var->yoffset = 0;
            break;
        default:
            return TD_FAILURE;
    }

    if (ioctl(info->fd, FBIOPUT_VSCREENINFO, var) < 0) {
        sample_print("put variable screen info failed!\n");
        close(info->fd);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_cmap_init(pthread_gfbg_sample_info *info)
{
    struct fb_cmap cmap;
    cmap.start = 0;
    cmap.len = CMAP_LENGTH_MAX;
    cmap.red = g_cmap_red;
    cmap.green = g_cmap_green;
    cmap.blue = g_cmap_blue;
    cmap.transp = g_cmap_alpha;

    if (ioctl(info->fd, FBIOPUTCMAP, &cmap) < 0) {
        sample_print("put cmap info failed!\n");
        close(info->fd);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_get_fix_and_mmap(pthread_gfbg_sample_info *info, struct fb_fix_screeninfo *fix,
                                      td_void **viraddr)
{
    if (info == TD_NULL || fix == TD_NULL || viraddr == TD_NULL) {
        return TD_FAILURE;
    }
    if (ioctl(info->fd, FBIOGET_FSCREENINFO, fix) < 0) {
        sample_print("get fix screen info failed!\n");
        close(info->fd);
        return TD_FAILURE;
    }

    *viraddr = mmap(TD_NULL, WIDTH_1920 * HEIGHT_1080,
                    PROT_READ | PROT_WRITE, MAP_SHARED, info->fd, 0);
    if (*viraddr == TD_NULL) {
        sample_print("mmap /dev/fb2 failed!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_start_draw_rect(ot_tde_none_src *none_src, ot_tde_corner_rect_info *conner_rect)
{
    td_s32 handle;
    td_u32 i;

    handle = ss_tde_begin_job();
    if (handle == OT_ERR_TDE_INVALID_HANDLE) {
        return TD_FAILURE;
    }

    /* times draw rect,once commit */
    for (i = 0; i < 5; i++) { /* 5 draw 5 rect */
        if (ss_tde_quick_draw_rect(handle, none_src, conner_rect) < 0) {
            ss_tde_cancel_job(handle);
            return TD_FAILURE;
        }
        none_src->dst_rect->pos_x += 200; /* 200 alg data */
        none_src->dst_rect->pos_y += 200; /* 200 alg data */
    }

    if (ss_tde_end_job(handle, TD_FALSE, TD_TRUE, 10) < 0) { /* 10 timeout */
        ss_tde_cancel_job(handle);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_draw_rect(td_void *viraddr, td_u32 index,
                               struct fb_var_screeninfo var, struct fb_fix_screeninfo fix)
{
    ot_tde_surface dst_surface = {0};
    ot_tde_rect dst_rect = {0};
    ot_tde_none_src none_src = {0};
    ot_tde_corner_rect_info conner_rect = {0};

    if (viraddr == TD_NULL) {
        return TD_FAILURE;
    }
    /* clear buffer,fill white */
    if (memset_s(viraddr, WIDTH_1920 * HEIGHT_1080, 0x33, WIDTH_1920 * HEIGHT_1080) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    if (ss_tde_open() < 0) {
        return TD_FAILURE;
    }

    dst_surface.color_format = OT_TDE_COLOR_FORMAT_CLUT4;
    dst_surface.width = WIDTH_1920;
    dst_surface.height = HEIGHT_1080;
    dst_surface.stride = fix.line_length;
    dst_surface.phys_addr = (index % 2) ? (fix.smem_start + fix.line_length * var.yres) : /* 2 alg data */
                            (fix.smem_start);
    dst_rect.pos_x = 0;
    dst_rect.pos_y = 0;
    dst_rect.height = 100; /* 100 alg data */
    dst_rect.width = 100; /* 100 alg data */
    none_src.dst_surface = &dst_surface;
    none_src.dst_rect = &dst_rect;
    /* conner rect */
    conner_rect.width = 10; /* 10 alg data */
    conner_rect.height = 20; /* 20 alg data */
    conner_rect.inner_color = 0x1;
    conner_rect.outer_color = (index % 2) ? 0x0 : 0x2; /* 2 alg data,0x2 for cmap index */

    if (sample_start_draw_rect(&none_src, &conner_rect) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_pandisplay(pthread_gfbg_sample_info *info, td_u32 index,
                                struct fb_var_screeninfo *var)
{
    if (info == TD_NULL || var == TD_NULL) {
        return TD_FAILURE;
    }
    if ((index % 2) == 0) { /* 2 alg data */
        var->yoffset = 0;
    } else {
        var->yoffset = var->yres;
    }

    if (ioctl(info->fd, FBIOPAN_DISPLAY, var) < 0) {
        sample_print("FBIOPAN_DISPLAY failed!\n");
        return TD_FAILURE;
    }

    sample_print("expected: buffer%d!\n", ((index % 2 == 0) ? 0 : 1)); /* 2 alg data */
    sample_print("wait 1 seconde.\n");
    sleep(1);
    ss_tde_close();
    return TD_SUCCESS;
}

static td_void *sample_gfbg_g3_clut(void *data)
{
    pthread_gfbg_sample_info *info = TD_NULL;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    td_s32 ret;
    td_void *viraddr = TD_NULL;
    td_u32 j = 0;

    if (data == TD_NULL) {
        return TD_NULL;
    }

    info = (pthread_gfbg_sample_info*)data;
    if (info->layer != GRAPHICS_LAYER_G3) {
        sample_print("++++%s:%d:only G3 support clut\n", __FUNCTION__, __LINE__);
        return TD_NULL;
    }

    ret = sample_bind_layer_to_vo(OT_VO_LAYER_G3, g_vo_dev);
    if (ret != TD_SUCCESS) {
        return TD_NULL;
    }

    ret = sample_var_init(info, &var);
    if (ret != TD_SUCCESS) {
        return TD_NULL;
    }

    ret = sample_cmap_init(info);
    if (ret != TD_SUCCESS) {
        return TD_NULL;
    }

    ret = sample_get_fix_and_mmap(info, &fix, &viraddr);
    if (ret != TD_SUCCESS) {
        return TD_NULL;
    }

    while (j < 20) { /* 20 times */
        if (sample_draw_rect(viraddr, j, var, fix) != TD_SUCCESS) {
            munmap(viraddr, WIDTH_1920 * HEIGHT_1080);
            close(info->fd);
            return TD_NULL;
        }
        if (sample_pandisplay(info, j, &var) != TD_SUCCESS) {
            munmap(viraddr, WIDTH_1920 * HEIGHT_1080);
            close(info->fd);
            return TD_NULL;
        }
        j++;
    }
    munmap(viraddr, WIDTH_1920 * HEIGHT_1080);
    close(info->fd);
    sample_print("[end]\n");
    return TD_NULL;
}


static td_void sample_gfbg_handle_sig(td_s32 signo)
{
    static int sig_handled = 0;
    if (!sig_handled && (signo == SIGINT || signo == SIGTERM)) {
        sig_handled = 1;
        g_sample_gfbg_exit = 1;
    }
}

static td_void sample_gfbg_usage2(td_void)
{
    printf("\n\n/****************index******************/\n");
    printf("please choose the case which you want to run:\n");
    printf("\t0:  ARGB8888 standard mode\n");
    printf("\t1:  ARGB1555 BUF_DOUBLE mode\n");
    printf("\t2:  ARGB1555 BUF_ONE mode\n");
    printf("\t3:  ARGB1555 BUF_NONE mode\n");
    printf("\t4:  ARGB1555 BUF_DOUBLE mode with compress\n");
    printf("\t5:  ARGB8888 BUF_DOUBLE mode with compress\n");
    printf("\t6:  CLUT4 BUF_NONE mode\n");
    printf("\t7:  rotate mode\n");
    return;
}

static td_void sample_gfbg_usage1(td_char *s_prg_nm)
{
    printf("usage : %s <index> <device> <intf>\n", s_prg_nm);
    sample_gfbg_usage2();
    printf("\n/****************device******************/\n");
    printf("\t 0) VO device 0#, default.\n");
    printf("\t 1) VO device 1#.\n");
    printf("\n/****************intf******************/\n");
    printf("\t 0) VO HDMI output, default.\n");
    printf("\t 1) VO BT1120 output.\n");

    return;
}

static td_s32 sample_gfbg_start_vo(vo_device_info *vo_dev_info)
{
#if GFBG_BE_WITH_VO
    ot_vo_intf_type gfbg_vo_intf_type = vo_dev_info->vo_intf_type;
    g_vo_intf_type = vo_dev_info->vo_intf_type;
    ot_vo_dev vo_dev = vo_dev_info->vo_dev;
    ot_vo_pub_attr pub_attr;
    td_u32  vo_frm_rate;
    ot_size size;
    td_s32 ret;
    sample_vo_cfg vo_config = {0};

    /* step 1(start vo):  start vo device. */
    pub_attr.intf_type = gfbg_vo_intf_type;
    pub_attr.intf_sync = OT_VO_OUT_1080P60;
    pub_attr.bg_color = COLOR_RGB_BLUE;
    ret = sample_comm_vo_get_width_height(pub_attr.intf_sync, &size.width, &size.height, &vo_frm_rate);
    if (ret != TD_SUCCESS) {
        sample_print("get vo width and height failed with %d!\n", ret);
        return ret;
    }
    ret = sample_comm_vo_start_dev(vo_dev, &pub_attr, &vo_config.user_sync, vo_config.dev_frame_rate);
    if (ret != TD_SUCCESS) {
        sample_print("start vo device failed with %d!\n", ret);
        return ret;
    }
    /*
     * step 2(start vo): bind G3 to VO device.
     * do this after VO device started.
     * set bind relationship.
     */
    ret = ss_mpi_vo_unbind_layer(OT_VO_LAYER_G3, vo_dev);
    if (ret != TD_SUCCESS) {
        sample_print("un_bind_graphic_layer failed with %d!\n", ret);
        return ret;
    }

    ret = ss_mpi_vo_bind_layer(OT_VO_LAYER_G3, vo_dev);
    if (ret != TD_SUCCESS) {
        sample_print("bind_graphic_layer failed with %d!\n", ret);
        return ret;
    }
    /* step 3(start vo): start hdmi device. */
    if (gfbg_vo_intf_type == OT_VO_INTF_HDMI) {
        sample_comm_vo_hdmi_start(pub_attr.intf_sync);
    }
    /* if it's displayed on bt1120, we should start bt1120 */
    if (gfbg_vo_intf_type == OT_VO_INTF_BT1120) {
        sample_comm_vo_bt1120_start(vo_dev, &pub_attr);
    }
    return TD_SUCCESS;
#else
    return TD_SUCCESS;
#endif
}

static td_void sample_gfbg_stop_vo(vo_device_info *vo_dev_info)
{
#if GFBG_BE_WITH_VO
    ot_vo_intf_type gfbg_vo_intf_type = vo_dev_info->vo_intf_type;
    ot_vo_dev vo_dev = vo_dev_info->vo_dev;

    if (gfbg_vo_intf_type == OT_VO_INTF_HDMI) {
        sample_comm_vo_hdmi_stop();
    }
    sample_comm_vo_stop_dev(vo_dev);
    return;
#else
    return;
#endif
}

static td_s32 sample_gfbg_standard_mode(vo_device_info *vo_dev_info)
{
    td_s32 ret;
    pthread_gfbg_sample_info info0;
    pthread_gfbg_sample_info info1;
    ot_vo_dev vo_dev = vo_dev_info->vo_dev;
    ot_vb_cfg vb_conf;

    /* step  1: init variable */
    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    /* step 2: mpp system init. */
    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        return ret;
    }
    /*
     * step 3: start VO device.
     * NOTE: step 3 is optional when VO is running on other system.
     */
    ret = sample_gfbg_start_vo(vo_dev_info);
    if (ret != TD_SUCCESS) {
        sample_print("VO device %d start failed\n", vo_dev_info->vo_dev);
        goto sample_gfbg_standard_mode_0;
    }
    /* step 4:  start gfbg. */
    info0.layer = vo_dev;    /* VO device number */
    info0.fd = -1;
    info0.ctrlkey = 2; /* 2 none buffer */
    info0.compress = TD_FALSE; /* compress opened or not */
    info0.color_format = OT_FB_FORMAT_ARGB8888;
    if (pthread_create(&g_gfbg_thread, 0, sample_gfbg_pandisplay, (td_void *)(&info0)) != 0) {
        sample_print("start gfbg thread0 failed!\n");
        goto sample_gfbg_standard_mode_1;
    }

    info1.layer = GRAPHICS_LAYER_G3;
    info1.fd = -1;
    info1.ctrlkey = 3; /* 3 point 150,150 */
    info1.compress = TD_FALSE; /* compress opened or not */
    info1.color_format = OT_FB_FORMAT_ARGB8888;
    if (pthread_create(&g_gfbg_thread1, 0, sample_gfbg_pandisplay, (td_void *)(&info1)) != 0) {
        sample_print("start gfbg thread1 failed!\n");
        goto sample_gfbg_standard_mode_1;
    }
    sample_gfbg_to_exit();
sample_gfbg_standard_mode_1:
    sample_gfbg_stop_vo(vo_dev_info);
sample_gfbg_standard_mode_0:
    sample_comm_sys_exit();
    return ret;
}

static td_s32 sample_gfbg_double_buf_mode(vo_device_info *vo_dev_info)
{
    td_s32 ret;
    pthread_gfbg_sample_info info0;
    ot_vo_dev vo_dev = vo_dev_info->vo_dev;
    g_vo_intf_type = vo_dev_info->vo_intf_type;
    ot_vb_cfg vb_conf;

    /* step  1: init variable */
    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    /* step 2: mpp system init. */
    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        return ret;
    }
    /*
     * step 3: start VO device.
     * NOTE: step 3 is optional when VO is running on other system.
     */
    ret = sample_gfbg_start_vo(vo_dev_info);
    if (ret != TD_SUCCESS) {
        sample_print("VO device %d start failed\n", vo_dev_info->vo_dev);
        goto sample_gfbg_double_buf_mode_0;
    }
    /* step 4:  start gfbg. */
    info0.layer = vo_dev;
    info0.fd = -1;
    info0.ctrlkey = 1;   /* double buffer */
    info0.compress = TD_FALSE;
    info0.color_format = OT_FB_FORMAT_ABGR1555;
    if (pthread_create(&g_gfbg_thread, 0, sample_gfbg_refresh, (td_void*)(&info0)) != 0) {
        sample_print("start gfbg thread failed!\n");
        goto sample_gfbg_double_buf_mode_1;
    }
    sample_gfbg_to_exit();
sample_gfbg_double_buf_mode_1:
    sample_gfbg_stop_vo(vo_dev_info);
sample_gfbg_double_buf_mode_0:
    sample_comm_sys_exit();
    return ret;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART04_H_ */
