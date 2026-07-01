/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART05_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART05_H_

    if ((info == TD_NULL) || (info->par == TD_NULL)) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    if (par->layer_id >= GFBG_MAX_LAYER_NUM) {
        gfbg_error("layer %u is not supported!should be in [%d, %d]!\n", par->layer_id, 0, GFBG_MAX_LAYER_NUM - 1);
        return TD_FAILURE;
    }
    /* assure layer is legal */
    if (!g_drv_ops.capability[par->layer_id].is_layer_support) {
        gfbg_error("layer %u is not supported!\n", par->layer_id);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
static td_bool gfbg_open_is_cursor_layer(struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    ot_fb_alpha alpha = {
        TD_TRUE, TD_FALSE, GFBG_ALPHA_TRANSPARENT,
        GFBG_ALPHA_OPAQUE, GFBG_ALPHA_OPAQUE, 0
    };
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        if (!atomic_read(&par->ref_count)) {
            (td_void)memset_s(par, sizeof(gfbg_par), 0, sizeof(gfbg_par));

            par->layer_id = layer_id;
            atomic_set(&par->ref_count, 0);

            /* Configuring alpha properties */
            gfbg_set_alpha(par, &alpha);
        }
        atomic_inc(&par->ref_count);

        return TD_TRUE;
    }
    return TD_FALSE;
}
#ifdef __LITEOS__
static td_s32 gfbg_open_default_param(struct fb_info *info, ot_fb_color_format *color_format)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (is_4k_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_4K]);
    } else if (is_hd_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_HD]);
    } else if (is_sd_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_SD]);
    } else if (is_ad_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_AD]);
    } else if (is_cursor_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_CURSOR]);
    } else {
        gfbg_error("error layer id:%u\n", par->layer_id);
    }
    *color_format = info->vinfo.format;
    if (*color_format == OT_FB_FORMAT_BUTT) {
        gfbg_error("Invalid default color format!\n");
        return TD_FAILURE;
    }
    info->oinfo.stride = (info->oinfo.sarea.width * (info->oinfo.bpp >> 3) + GFBG_ALIGNMENT) & /* 8 is bits 2^3 */
                          (~GFBG_ALIGNMENT);

    return TD_SUCCESS;
}
#else
static td_s32 gfbg_open_default_param(struct fb_info *info, ot_fb_color_format *color_format)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    struct fb_var_screeninfo *var = TD_NULL;
    struct fb_fix_screeninfo *fix = TD_NULL;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    gfbg_stride_attr attr = {0};
#endif
    var = &info->var;
    fix = &info->fix;

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (is_4k_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_4K];
    } else if (is_hd_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_HD];
    } else if (is_sd_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_SD];
    } else if (is_ad_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_AD];
    } else if (is_cursor_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_CURSOR];
    } else {
        gfbg_error("error layer id:%u\n", par->layer_id);
    }
    /* transform colorfmt form bitfiled to gfbg format, and record it */
    *color_format = gfbg_getfmtbyargb(&var->red, &var->green, &var->blue,
        &var->transp, var->bits_per_pixel);
    if (*color_format == OT_FB_FORMAT_BUTT) {
        gfbg_error("Invalid default color format!\n");
        return TD_FAILURE;
    }
    /* By default it is width*4 (32 bits per pixel / 8) */
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;
    attr.width = var->xres_virtual;
    attr.format = *color_format;
    /* 3 for 8bits */
    fix->line_length = (var->xres_virtual * (var->bits_per_pixel >> 3) + GFBG_ALIGNMENT) &
                        (~GFBG_ALIGNMENT);
    gfbg_recalculate_stride(NULL, &(fix->line_length), &attr);
#else
    /* 3 for 8bits */
    fix->line_length = (var->xres_virtual * (var->bits_per_pixel >> 3) + GFBG_ALIGNMENT) &
                        (~GFBG_ALIGNMENT);
#endif
    return TD_SUCCESS;
}
#endif

static td_void gfbg_open_init_display(const struct fb_info *info, td_u32 layer_id, const gfbg_osd_data *osd_data)
{
    gfbg_par *par = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    par = (gfbg_par *)info->par;

    (td_void)memset_s(par, sizeof(gfbg_par), 0, sizeof(gfbg_par));
    par->layer_id = layer_id;
    atomic_set(&par->ref_count, 0);

    display_info = &par->display_info;
    display_info->display_width    = gfbg_get_xres(info);
    display_info->display_height   = gfbg_get_yres(info);
    display_info->screen_width     = gfbg_get_xres(info);
    display_info->screen_height    = gfbg_get_yres(info);
    display_info->vir_x_res         = gfbg_get_xres_virtual(info);
    display_info->vir_y_res         = gfbg_get_yres_virtual(info);
    display_info->x_res            = gfbg_get_xres(info);
    display_info->y_res            = gfbg_get_yres(info);
    display_info->max_screen_width  = osd_data->screen_width;
    display_info->max_screen_height = osd_data->screen_height;
    display_info->mirror_mode       = OT_FB_MIRROR_NONE;
    display_info->rotate_mode       = OT_FB_ROTATE_NONE;
    par->rotate_vb = 0;

    init_waitqueue_head(&(par->vbl_event));
    init_waitqueue_head(&(par->do_refresh_job));
    init_waitqueue_head(&(par->refresh_job_done));

    return;
}
static td_void gfbg_open_init_config(const struct fb_info *info, td_u32 layer_id, gfbg_osd_data *osd_data,
                                     ot_fb_color_format color_format, gfbg_cursor_info *cursor_info)
{
    gfbg_par *par = TD_NULL;
    ot_fb_alpha alpha = {
        TD_TRUE, TD_FALSE, GFBG_ALPHA_TRANSPARENT,
        GFBG_ALPHA_OPAQUE, GFBG_ALPHA_OPAQUE, 0
    };
    gfbg_display_info *display_info = TD_NULL;
    unsigned long lock_flag;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    ot_unused(cursor_info);

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    gfbg_set_bufmode(par->layer_id, OT_FB_LAYER_BUF_BUTT);
    gfbg_set_alpha(par, &alpha);
    gfbg_set_dispbufinfo(layer_id);
    gfbg_set_fmt(par, color_format);
    /* Anti-flicker when interlaced */
    display_info->need_antiflicker = (osd_data->scan_mode == GFBG_SCANMODE_I) ? (TD_TRUE) : (TD_FALSE);
    gfbg_set_antiflickerlevel(par->layer_id, OT_FB_LAYER_ANTIFLICKER_AUTO);

#ifdef CURSOR
    if (is_soft_cursor() && !is_cursor_layer(layer_id)) {
        cursor_info->cursor.cursor.phys_addr = gfbg_get_smem_start(info) + gfbg_get_smem_len(info);
        cursor_info->cursor.cursor.pitch = GFBG_SOFTCURSOR_STRIDE;
        cursor_info->cursor.cursor.format = color_format;
        gfbg_set_cursorinfo(par, cursor_info);
    }
#endif
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    g_drv_ops.gfbg_drv_layer_default_setting(layer_id);
    g_drv_ops.gfbg_drv_set_layer_alpha(layer_id, par->alpha);
    g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, par->color_format);
}
static td_void gfbg_open_init_compress(const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_rect rect;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    layer_id = par->layer_id;
    compress_info = &par->compress_info;
    refresh_info = &par->refresh_info;
#ifdef MDDRDETECT
    if (g_drv_ops.gfbg_drv_get_ddr_zone_cfg != TD_NULL) {
        /* Memory detection is open by default. This function is not supported at this time. */
        g_drv_ops.gfbg_drv_get_ddr_zone_cfg(layer_id, (td_u32*)&compress_info->start_section,
            (td_u32*)&compress_info->zone_nums);
    }
#endif
    /* Decompression information settings */
    compress_info->new_start_section = compress_info->start_section;
    compress_info->new_zone_nums = compress_info->zone_nums;
    compress_info->clear_zone = TD_FALSE;

    g_drv_ops.gfbg_drv_enable_dcmp(layer_id, TD_FALSE); /* Clear decompression status */

    rect.x = 0;
    rect.y = 0;
    rect.width = (td_s32)gfbg_get_xres(info);
    rect.height = (td_s32)gfbg_get_yres(info);

    g_drv_ops.gfbg_drv_set_layer_rect(par->layer_id, &rect, &rect);
    g_drv_ops.gfbg_drv_set_layer_src_image_reso(par->layer_id, &rect);

    g_drv_ops.gfbg_drv_set_layer_stride(par->layer_id, gfbg_get_line_length(info));

    g_drv_ops.gfbg_drv_set_pre_mul(par->layer_id, display_info->is_premul);
    if (g_drv_ops.capability[layer_id].is_key_rgb || g_drv_ops.capability[layer_id].is_key_alpha) {
        g_drv_ops.gfbg_drv_set_layer_key_mask(par->layer_id, &par->ckey);
    }
    /* Set to memory address */
    g_drv_ops.gfbg_drv_set_layer_addr(par->layer_id, gfbg_get_smem_start(info));
    refresh_info->screen_addr = gfbg_get_smem_start(info);
    refresh_info->gb_screen_addr = 0x0; /* Clear the decompression address */
    compress_info->layer_addr_update = TD_TRUE;

    return;
}
static td_void gfbg_open_init_finish(const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    unsigned long lock_flag;

    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;

#ifndef CONFIG_OT_VO_FB_SEPARATE
    /* The soft interrupt handlers are registered in the context of the graphics layer. */
    g_drv_ops.gfbg_drv_set_int_callback((fb_vo_int_type)GFBG_INTTYPE_VO, gfbg_interrupt_vo_callback,
                                        par->layer_id, &par->layer_id);
#endif
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);

    compress_info->compress_open = TD_FALSE;
    compress_info->is_losslessa = TD_FALSE;
    compress_info->is_lossless = TD_FALSE;

    par->modifying = TD_TRUE;
    gfbg_set_show(par, TD_TRUE);
    par->param_modify_mask = GFBG_LAYER_PARAMODIFY_SHOW;
    par->modifying = TD_FALSE;
    par->layer_open = TD_TRUE;

    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    /* enable layer for vo to decide config csc or not */
    g_drv_ops.gfbg_drv_set_layer_enable(par->layer_id, TD_TRUE);

    return;
}
static td_s32 gfbg_open_start(struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    gfbg_cursor_info cursor_info;
    gfbg_osd_data osd_data = {0};
    td_u32 layer_id;
    ot_fb_color_format color_format;

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        gfbg_error("layer_id is [%u] and more than max[%u]\n", layer_id, GFBG_MAX_LAYER_NUM);
        return TD_FAILURE;
    }
    if (!atomic_read(&par->ref_count)) {
        vou_graphics_init();
        (td_void)memset_s(&cursor_info, sizeof(gfbg_cursor_info), 0, sizeof(gfbg_cursor_info));
        if (g_drv_ops.gfbg_drv_get_osd_data(layer_id, &osd_data) != TD_SUCCESS) {
            gfbg_error("Failed to get osd data!\n");
            return TD_FAILURE;
        }
        /* Configure the layer's default variable parameter var_info according to the type of layer */
        if (g_drv_ops.gfbg_open_layer(layer_id) != TD_SUCCESS) {
            gfbg_error("Open graphic layer %u# failed!\n", layer_id);
            return TD_FAILURE;
        }
        /* get default var and fix */
        if (gfbg_open_default_param(info, &color_format) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        /* Initialize the display information in private data */
        gfbg_open_init_display(info, layer_id, &osd_data);

        /* Initialization lock */
        gfbg_spin_lock_init(&par->lock);

        /* gfbg set bufmode\alpha\displaybufinfo\fmt */
        gfbg_open_init_config(info, layer_id, &osd_data, color_format, &cursor_info);

        /* gfbg compress init */
        gfbg_open_init_compress(info);

        /* gfbg set callback and init finish */
        gfbg_open_init_finish(info);
    }
    return TD_SUCCESS;
}
/*
 * Function        : gfbg_open
 * Description     : open the framebuffer and using the default parameter to set the layer
                     struct fb_info *info
 * Return          : return 0
 */
#ifdef __LITEOS__
void *g_gfbg_info_ptr = TD_NULL;
static td_s32 gfbg_open(struct fb_vtable_s *vtable)
#else
static td_s32 gfbg_open(struct fb_info *info, td_s32 user)
#endif
{
#ifdef __LITEOS__
    struct gfbg_info *info = (struct gfbg_info *)vtable;
#endif
    gfbg_par *par = TD_NULL;
    ot_unused(user);

    /* check input param */
    if (gfbg_open_check_param(info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;

    /* check layer is cursor and is soft cursor */
    if (gfbg_open_is_cursor_layer(info) == TD_TRUE) {
        return TD_SUCCESS;
    }
    /* open the layer first */
    if (gfbg_open_start(info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /* increase reference count */
    atomic_inc(&par->ref_count);
#ifdef __LITEOS__
    g_gfbg_info_ptr = (void *)info;
#endif
    return TD_SUCCESS;
}
/*
 * Function        : gfbg_release
 * Description     : open the framebuffer and disable the layer
                     struct fb_info *info
 * Return          : return 0 if succeed, otherwise return -EINVAL
 */
static td_void gfbg_release_info(gfbg_par *par)
{
    td_u32 layer_id;
    unsigned long lock_flag;

    if ((g_drv_ops.gfbg_drv_set_layer_enable == TD_NULL) || (g_drv_ops.gfbg_drv_updata_layer_reg == TD_NULL) ||
        (g_drv_ops.gfbg_drv_set_int_callback == TD_NULL) || (g_drv_ops.gfbg_close_smart_rect == TD_NULL) ||
        (g_drv_ops.gfbg_close_layer == TD_NULL)) {
        gfbg_error("g_drv_ops is NULL!\n");
        return;
    }
#ifdef MDDRDETECT
    if (g_drv_ops.gfbg_drv_clear_ddr_dectect_zone == TD_NULL) {
        gfbg_error("gfbg_drv_clear_ddr_dectect_zone failed!\n");
        return;
    }
#endif
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    layer_id = par->layer_id;
    gfbg_set_show(par, TD_FALSE);

    if ((!is_cursor_layer(par->layer_id)) || (!is_soft_cursor())) {
        /* disable the compress */
        if (par->compress_info.compress_open) {
            /* No memory alloc, no need to release */
            par->compress_info.compress_open = TD_FALSE;
            par->compress_info.is_losslessa = TD_FALSE;
            par->compress_info.is_lossless = TD_FALSE;
        }
        /* clear wbc interrupt when close fb */
        g_drv_ops.gfbg_drv_set_layer_enable(par->layer_id, TD_FALSE);
        g_drv_ops.gfbg_drv_updata_layer_reg(par->layer_id);

        /* vo has spin lock, you need to release the spin lock first */
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        g_drv_ops.gfbg_drv_set_int_callback((fb_vo_int_type)GFBG_INTTYPE_VO, TD_NULL, layer_id, TD_NULL);
        gfbg_spin_lock_irqsave(&par->lock, lock_flag);
        g_drv_ops.gfbg_close_smart_rect(par->layer_id);
#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART05_H_ */
