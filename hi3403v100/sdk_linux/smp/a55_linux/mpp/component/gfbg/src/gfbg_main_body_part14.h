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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART14_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART14_H_

#ifdef __LITEOS__
static td_s32 gfbg_disp_setdispsize(td_u32 layer_id, td_u32 width, td_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    td_u32 pitch;
    unsigned long lock_flag;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if ((display_info->display_width == width) && (display_info->display_height == height)) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_SUCCESS;
    }
    /*
     * for width and height check
     * width and height should less than max_screen_size
     */
    if (gfbg_disp_check_param(info, width, height) != TD_SUCCESS) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }

    display_info->display_width = width;
    display_info->display_height = height;
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_INRECT;

    pitch = (((width * gfbg_get_bits_per_pixel(info)) >> 3) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT); /* 3: 8bits */
    if (pitch > info->oinfo.stride) {
        info->oinfo.stride = pitch;
        info->oinfo.sarea.height = info->vinfo.yres = display_info->display_height;
        info->oinfo.sarea.width = info->vinfo.xres = display_info->display_width;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;
    }

    gfbg_set_dispbufinfo(layer_id);

    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        display_info->screen_width = width;
        display_info->screen_height = height;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;
    }

    /*
     * here we need to think about how to resist flicker again,
     * we use VO do flicker resist before , but now if the display H size is the same as the screen,
     * VO will not do flicker resist, so should choose TDE to do flicker resist
     */
    gfbg_select_antiflicker_mode(par);
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return TD_SUCCESS;
}
#else
static td_void gfbg_fill_var_info(gfbg_display_info *display_info, struct fb_var_screeninfo *var)
{
    var->xres = display_info->display_width;
    var->yres = display_info->display_height;
    if (var->xres_virtual < display_info->display_width) {
        var->xres_virtual = display_info->display_width;
    }

    if (var->yres_virtual < display_info->display_height) {
        var->yres_virtual = display_info->display_height;
    }
    return;
}

static td_s32 gfbg_disp_setdispsize(td_u32 layer_id, td_u32 width, td_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    struct fb_var_screeninfo *var = &info->var;
    struct fb_fix_screeninfo *fix = &info->fix;
    gfbg_display_info *display_info = &par->display_info;
    td_u32 pitch;
    unsigned long lock_flag;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    gfbg_stride_attr attr = {0};
#endif
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if ((display_info->display_width == width) && (display_info->display_height == height)) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_SUCCESS;
    }
    /*
     * for width and height check
     * width and height should less than max_screen_size
     */
    if (gfbg_disp_check_param(info, width, height) != TD_SUCCESS) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }

    display_info->display_width = width;
    display_info->display_height = height;
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_INRECT;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;
    attr.width = width;
    attr.format = gfbg_getfmtbyargb(&var->red, &var->green, &var->blue, &var->transp, var->bits_per_pixel);
    pitch = (((width * gfbg_get_bits_per_pixel(info))>> 3) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT); /* 3: 8bits */
    gfbg_recalculate_stride(NULL, &pitch, &attr);
#else
    pitch = (((width * gfbg_get_bits_per_pixel(info))>> 3) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT); /* 3: 8bits */
#endif
    if (pitch > fix->line_length) {
        fix->line_length = pitch;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;
    }
    /*
     * If the user calls FBIOPUT_LAYER_INFO to set display_width and display_height,then sync to xres yres,
     * Otherwise, there will be an error in the memory address in gfbg_set_dispbufinfo.
     */
    gfbg_fill_var_info(display_info, var);
    gfbg_set_dispbufinfo(layer_id);

    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        display_info->screen_width = width;
        display_info->screen_height = height;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;
    }

    /*
     * here we need to think about how to resist flicker again,
     * we use VO do flicker resist before , but now if the display H size is the same as the screen,
     * VO will not do flicker resist, so should choose TDE to do flicker resist
     */
    gfbg_select_antiflicker_mode(par);
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return TD_SUCCESS;
}
#endif

/*
 * we handle it by two case:
 * case 1 : if VO support Zoom, we only change screen size, g_display size keep not change
 * case 2: if VO can't support zoom, g_display size should keep the same as screen size
 */
static td_s32 gfbg_disp_setscreensize(td_u32 layer_id, td_u32 width, td_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;

    /* If the chip does not support scaling, it is consistent with the process of setting the display area. */
    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        return gfbg_disp_setdispsize(layer_id, width, height);
    }

    display_info->screen_width = width;
    display_info->screen_height = height;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;

    /* Here  we need to think about how to resist flicker again, we use VO do flicker resist before ,
     * but now if the g_display H size is the same as the screen, VO will not do flicker resist, so should choose
     * TDE to do flicker resist
     */
    gfbg_select_antiflicker_mode(par);

    return TD_SUCCESS;
}

static td_void gfbg_buf_freemem(td_phys_addr_t phyaddr)
{
    cmpi_mmz_free(phyaddr, TD_NULL);
}

static td_s32 gfbg_freeccanbuf(gfbg_par *par)
{
    ot_fb_surface *canvas_sur = TD_NULL;
    if (par == TD_NULL) {
        return TD_FAILURE;
    }
    canvas_sur   = &par->canvas_sur;

    if (canvas_sur->phys_addr != 0) {
        gfbg_buf_freemem(canvas_sur->phys_addr);
    }
    canvas_sur->phys_addr = 0;

    if (par->rotate_vb != 0) {
        cmpi_mmz_free(par->rotate_vb, TD_NULL);
        par->rotate_vb = 0;
    }

    return TD_SUCCESS;
}

static td_void set_par_stride(struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    td_u32 stride;
    td_phys_addr_t display_addr;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    gfbg_stride_attr attr = {0};
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;
    attr.width = gfbg_get_xres_virtual(info);
    attr.format = gfbg_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp,
        info->var.bits_per_pixel);
    stride = (((gfbg_get_xres_virtual(info) * gfbg_get_bits_per_pixel(info)) >> 3) + GFBG_ALIGNMENT) & /* 2^3 */
              (~GFBG_ALIGNMENT);
    gfbg_recalculate_stride(NULL, &stride, &attr);
#else
    stride = (((gfbg_get_xres_virtual(info) * gfbg_get_bits_per_pixel(info)) >> 3) + GFBG_ALIGNMENT) & /* 2^3 */
              (~GFBG_ALIGNMENT);
#endif
    if (stride != gfbg_get_line_length(info) || (gfbg_get_yres(info) != display_info->y_res)) {
#ifdef __LITEOS__
        info->oinfo.stride = stride;
#else
        info->fix.line_length = stride;
#endif
        gfbg_set_dispbufinfo(par->layer_id);
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;
    }

    display_addr = (gfbg_get_smem_start(info) + stride * gfbg_get_yoffset(info) +
                    gfbg_get_xoffset(info) * (gfbg_get_bits_per_pixel(info) >> 3)) & /* 3 /8bits */
                   0xfffffff0; /* 0xfffffff0 16 align */
    if (display_addr != refresh_info->screen_addr) {
        refresh_info->screen_addr = display_addr;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DISPLAYADDR;
    }
    return;
}

static td_s32 set_par_resolution(const struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    unsigned long lock_flag;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if (gfbg_get_xres(info) != display_info->x_res || gfbg_get_yres(info) != display_info->y_res) {
        if ((gfbg_get_xres(info) == 0) || (gfbg_get_yres(info) == 0)) {
            if (par->show == TD_TRUE) {
                par->show = TD_FALSE;
                par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_SHOW;
            }
        }

         /*
          * The following two functions have a sleep operation, you must unlock before calling,
          * and lock the global variable inside the function.
          */
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        if (gfbg_disp_setdispsize(par->layer_id, gfbg_get_xres(info), gfbg_get_yres(info)) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if (gfbg_disp_setscreensize(par->layer_id, gfbg_get_xres(info), gfbg_get_yres(info)) != TD_SUCCESS) {
            return TD_FAILURE;
        }

        gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    }

    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

/*
 * Function        : gfbg_set_par
 * Description     : set the variable parmater and make it use
 * Input           : struct fb_info *info
 * Return          : return 0
 */
static td_s32 gfbg_set_par(struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    ot_fb_color_format format;
    unsigned long lock_flag;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);

    par->modifying = TD_TRUE;

    /* set the stride if stride change */
    set_par_stride(info);

    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    /* If xres or yres change */
    if (set_par_resolution(info) != TD_SUCCESS) {
        return TD_FAILURE;
    }

#ifdef __LITEOS__
    format = info->vinfo.format;
#else
    format = gfbg_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp,
                               gfbg_get_bits_per_pixel(info));
#endif
    if ((par->color_format != format)) {
        gfbg_freeccanbuf(par);
        gfbg_set_fmt(par, format);
        par->cursor_info.cursor.cursor.format = format;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_FMT;
    }

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    display_info->x_res = gfbg_get_xres(info);
    display_info->y_res = gfbg_get_yres(info);
    display_info->vir_x_res = gfbg_get_xres_virtual(info);
    display_info->vir_y_res = gfbg_get_yres_virtual(info);

    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return 0;
}

/*
 * Function        : gfbg_pan_display
 * Description     : pan g_display.
 * Input           : struct fb_var_screeninfo *var
 * Return          : return 0
 */
#ifdef __LITEOS__
static td_s32 gfbg_pan_display(struct fb_vtable_s *vtable, struct fb_overlayinfo_s *oinfo)
#else
static td_s32 gfbg_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
#endif
{
#ifdef __LITEOS__
        struct gfbg_info *info = (struct gfbg_info *)vtable;
#endif

    gfbg_par*               par = (gfbg_par *)info->par;
    gfbg_refresh_info*      refresh_info = &par->refresh_info;
    td_phys_addr_t                    display_addr;
    td_u32                    stride;
    ot_fb_buf                 canvas_buf;
    td_s32                    ret;

    /* set the stride and display start address */
    stride = gfbg_get_line_length(info);

#ifdef __LITEOS__
    display_addr = (gfbg_get_smem_start(info) + (td_u64)stride * oinfo->sarea.y + (td_u64)oinfo->sarea.x *
        (gfbg_get_bits_per_pixel(info) >> 3)) & 0xfffffffffffffff0; /* 3 is 8 bits */
#else
    /* 3 is 8 bits */
    display_addr = (gfbg_get_smem_start(info) + (td_u64)stride * var->yoffset + (td_u64)var->xoffset *
        (gfbg_get_bits_per_pixel(info) >> 3)) & 0xfffffffffffffff0; /* 3 is 8 bits */
#endif
    canvas_buf.canvas.format = par->color_format;
    canvas_buf.canvas.phys_addr = display_addr;
    canvas_buf.canvas.pitch = stride;
    canvas_buf.update_rect.x = 0;
    canvas_buf.update_rect.y = 0;
#ifdef __LITEOS__
    canvas_buf.canvas.width = info->vinfo.xres;
    canvas_buf.canvas.height = info->vinfo.yres;
    canvas_buf.update_rect.width = info->vinfo.xres;
    canvas_buf.update_rect.height = info->vinfo.xres;
    g_layer[par->layer_id].info->activate = info->activate;
#else
    canvas_buf.canvas.width = gfbg_get_xres(info);
    canvas_buf.canvas.height = gfbg_get_yres(info);
    canvas_buf.update_rect.width = (td_s32)gfbg_get_xres(info);
    canvas_buf.update_rect.height = (td_s32)gfbg_get_yres(info);
    g_layer[par->layer_id].info->var.activate = info->var.activate;
#endif
    refresh_info->buf_mode = OT_FB_LAYER_BUF_BUTT;
    ret = gfbg_refresh_0buf(par->layer_id, &canvas_buf);

    return ret;
}

static inline td_void gfbg_get_fmt(const gfbg_par *par, ot_fb_color_format *color_format)
{
    *color_format = par->color_format;
    return;
}

static inline td_void gfbg_set_fmt(gfbg_par *par, ot_fb_color_format color_fmt)
{
    par->color_format = color_fmt;

    return;
}

static inline td_void gfbg_set_alpha(gfbg_par *par, const ot_fb_alpha *alpha)
{
    td_s32 ret;
    ret = memcpy_s(&par->alpha, sizeof(ot_fb_alpha), alpha, sizeof(ot_fb_alpha));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return;
    }
    return;
}

static inline td_void gfbg_get_alpha(const gfbg_par *par, ot_fb_alpha *alpha)
{
    td_s32 ret;
    ret = memcpy_s(alpha, sizeof(ot_fb_alpha), &par->alpha, sizeof(ot_fb_alpha));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return;
    }
    return;
}

static inline td_void gfbg_set_key(gfbg_par *par, const gfbg_colorkeyex *key)
{
    td_s32 ret;
    ret = memcpy_s(&par->ckey, sizeof(gfbg_colorkeyex), key, sizeof(gfbg_colorkeyex));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return;
    }
    return;
}

static inline td_void gfbg_get_key(const gfbg_par *par, gfbg_colorkeyex *key)
{
    td_s32 ret;
    ret = memcpy_s(key, sizeof(gfbg_colorkeyex), &par->ckey, sizeof(gfbg_colorkeyex));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return;
    }
    return;
}

#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART14_H_ */
