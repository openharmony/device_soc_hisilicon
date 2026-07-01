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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART11_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART11_H_

#ifndef __LITEOS__
static td_s32 gfbg_check_output(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (((var->yres % 2) != 0) && (gfbg_is_interlace(par))) { /* 2 for calculate */
        gfbg_error("yres(%d) of layer_id %d should be even when vodev output is interlace\n", var->yres, layer_id);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_check_virtual_resolution(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (var->xres < gfbg_min_width(layer_id)) {
        gfbg_error("xres(%d) of layer_id %d can't be less than min_width(%d)\n", var->xres, layer_id,
                   gfbg_min_width(layer_id));
        return TD_FAILURE;
    }
    if (var->yres < gfbg_min_height(layer_id)) {
        gfbg_error("yres(%d) of layer_id %d can't be less than min_height(%d)\n", var->yres, layer_id,
                   gfbg_min_height(layer_id));
        return TD_FAILURE;
    }

    if (var->xres > var->xres_virtual) {
        gfbg_error("xres(%d) of layer_id %d should be less than xres_virtual(%d)\n", var->xres, layer_id,
                   var->xres_virtual);
        return TD_FAILURE;
    }
    if (var->yres > var->yres_virtual) {
        gfbg_error("yres(%d) of layer_id %d should be less than yres_virtual(%d)\n", var->yres, layer_id,
                   var->yres_virtual);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_check_offset(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if ((var->xoffset + var->xres > var->xres_virtual) || (var->xoffset > var->xres_virtual)) {
        gfbg_error("the sum of layer%d's xoffset(%d) and xres(%d) should be less than xres_virtual(%d)\n", layer_id,
                   var->xoffset, var->xres, var->xres_virtual);
        return -EINVAL;
    }

    if ((var->yoffset + var->yres > var->yres_virtual) || (var->yoffset > var->yres_virtual)) {
        gfbg_error("the sum of layer%d's yoffset(%d) and yres(%d) should be less than yres_virtual(%d)\n", layer_id,
                   var->yoffset, var->yres, var->yres_virtual);
        return -EINVAL;
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_check_total(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    td_u32 hor_total;
    td_u32 ver_total;
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    hor_total = var->left_margin + var->xres + var->right_margin + var->hsync_len;
    if (hor_total == 0) {
        gfbg_error("the sum of layer%d's left_margin(%d),xres(%d),right_margin(%d),hsync_len(%d) can't be 0\n",
            layer_id, var->left_margin, var->xres, var->right_margin, var->hsync_len);
        return TD_FAILURE;
    }
    ver_total = var->yres + var->lower_margin + var->vsync_len + var->upper_margin;
    if (ver_total == 0) {
        gfbg_error("the sum of layer%d's left_margin(%d),xres(%d),right_margin(%d),hsync_len(%d) can't be 0\n",
            layer_id, var->upper_margin, var->yres, var->lower_margin, var->vsync_len);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/*
 * Function        : gfbg_check_var
 * Description     : check if the parameter for framebuffer is supported.
 * Return          : return 0, if the parameter is supported, otherwise,return error
 */
static td_s32 gfbg_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 expected_len;
    td_u32 layer_id;

    if ((info == TD_NULL) || (var == TD_NULL) || (info->par == TD_NULL)) {
        return TD_FAILURE;
    }

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        gfbg_error("cursor layer doesn't support this operation!\n");
        return TD_FAILURE;
    }

    if (gfbg_check_fmt(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /*
     * For interlaced output check
     * the actual height of the layer must be an even number
     * Progressive output without this limit
     */
    if (gfbg_check_output(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * for virtual resolution check
     * virtual resolution can't be less than minimal resolution
     */
    if (gfbg_check_virtual_resolution(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /* check if the offset is valid */
    if (gfbg_check_offset(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * for hor_total and ver_total check
     * The FB driver in the Linux kernel will use u32HTotal and u32VTotal as divisors
     * so they cannot be 0
     */
    if (gfbg_check_total(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    gfbg_info("xres:%d, yres:%d, xres_virtual:%d, yres_virtual:%d\n", var->xres, var->yres, var->xres_virtual,
              var->yres_virtual);
    /* for mem len check */
    expected_len = var->yres_virtual * ((((var->xres_virtual * var->bits_per_pixel) >> 3) + /* 8 bit (2^3) */
        GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT));

    if (info->fix.smem_len && (expected_len > info->fix.smem_len)) {
        gfbg_error("layer %d don't has enough mem! expected: %d KBytes, real:%d KBytes\n", layer_id,
                   expected_len / 1024, info->fix.smem_len / 1024); /* 1024 for KB */
        return -EINVAL;
    }

    return TD_SUCCESS;
}
#endif

static td_void gfbg_set_dispbufinfo(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)(info->par);
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_dispbuf_info *disp_buf_info = &refresh_info->disp_buf_info;
    td_u32 uncompress_stride;
    td_u32 buf_size;
    uncompress_stride = (info->var.xres_virtual * info->var.bits_per_pixel / 8 + /* 8 one byte */
        GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);

    /*
     * there's a limit from hardware that screen buf should be 16 bytes aligned,maybe it's proper
     * to get this info from drv adapter
     */
    if (gfbg_check_memory_enough(info, uncompress_stride) == TD_TRUE &&
        info->var.bits_per_pixel == 32) { /* 32 only for argb8888 */
        buf_size = ((uncompress_stride * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    } else {
        /* according real stride,calculate offset */
        buf_size = ((gfbg_get_line_length(info) * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    }
    if (gfbg_get_smem_len(info) == 0) {
        return;
    } else if ((gfbg_get_smem_len(info) >= buf_size) && (gfbg_get_smem_len(info) < buf_size * 2)) { /* 2 alg data */
        disp_buf_info->phys_addr[0] = gfbg_get_smem_start(info);
        disp_buf_info->phys_addr[1] = gfbg_get_smem_start(info);
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
        disp_buf_info->vir_addr[0] = gfbg_get_screen_base(info);
        disp_buf_info->vir_addr[1] = gfbg_get_screen_base(info);
#endif
    } else if (gfbg_get_smem_len(info) >= buf_size * 2) { /* 2 alg data */
        disp_buf_info->phys_addr[0] = gfbg_get_smem_start(info);
        disp_buf_info->phys_addr[1] = gfbg_get_smem_start(info) + buf_size;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
        disp_buf_info->vir_addr[0] = gfbg_get_screen_base(info);
        disp_buf_info->vir_addr[1] = gfbg_get_screen_base(info) + buf_size;
#endif
    }
    return;
}

static td_s32 gfbg_refresh_1buf_prepare_dst(ot_fb_buf *dst_rect, const gfbg_par *par,
                                            const gfbg_display_info *display_info,
                                            const gfbg_dispbuf_info *display_buf_info, const struct fb_info *info)
{
    td_u32 bytes_per_pixel = 2;

    dst_rect->canvas.format = par->color_format;
    dst_rect->canvas.height = display_info->display_height;
    dst_rect->canvas.width = display_info->display_width;
    dst_rect->canvas.pitch = gfbg_get_line_length(info);

    if (display_info->rotate_mode  == OT_FB_ROTATE_90 || display_info->rotate_mode == OT_FB_ROTATE_270) {
        dst_rect->canvas.height = display_info->display_width;
        dst_rect->canvas.width = display_info->display_height;
        if (dst_rect->canvas.format == OT_FB_FORMAT_ARGB1555 || dst_rect->canvas.format == OT_FB_FORMAT_ARGB4444) {
            bytes_per_pixel = 2; /* 2 for bit */
        } else if (dst_rect->canvas.format == OT_FB_FORMAT_ARGB8888) {
            bytes_per_pixel = 4; /* 4 for bit */
        }
        dst_rect->canvas.pitch = ((bytes_per_pixel * dst_rect->canvas.width + GFBG_ALIGN - 1) /
                                 GFBG_ALIGN) * GFBG_ALIGN;

        if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
            (par->color_format != OT_FB_FORMAT_ARGB8888)) {
            gfbg_error("rotate only support ARGB4444, ARGB1555, ARGB8888 which is %d\n", par->color_format);
            return TD_FAILURE;
        }
        dst_rect->canvas.phys_addr = par->rotate_vb;
    } else {
        /*
         * The target address is selected as the display buf configured for the interrupt,
         * which is indicated by index for interrupt.
         */
        dst_rect->canvas.phys_addr = display_buf_info->phys_addr[display_buf_info->index_for_int];

        /* If compression is not open, you do not have to configure a compressed address for this refresh */
    }
    return TD_SUCCESS;
}

static td_void gfbg_refresh_1buf_prepare_addr(gfbg_refresh_info *refresh_info, td_u32 layer_id, gfbg_osd_data *osd_data,
                                              struct fb_info *info, ot_fb_buf *dst_rect)
{
    unsigned long lock_flag;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_dispbuf_info *display_buf_info = &refresh_info->disp_buf_info;
    td_u32 buf_size = ((gfbg_get_line_length(info) * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    /* 1buf does not support compression, close compression configuration */
    refresh_info->disp_buf_info.compress = TD_FALSE;

    g_drv_ops.gfbg_drv_get_osd_data(layer_id, osd_data);

    if (osd_data->buffer_phy_addr != par->refresh_info.disp_buf_info.phys_addr[0] && display_buf_info->phys_addr[0]) {
        gfbg_spin_lock_irqsave(&par->lock, lock_flag);
        par->modifying = TD_TRUE;
        /* Notify the interrupt handler to modify the display address. */
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DISPLAYADDR;

        /*
         * The buf address in the display information is configured to
         * the screen display address for refreshing the screen.
         */
        refresh_info->screen_addr = display_buf_info->phys_addr[display_buf_info->index_for_int];
        /* If compression is not open, you do not have to configure a compressed address for this refresh */
        if (par->compress_info.compress_open) {
            /* Use half of the video memory instead of another buffer */
            refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 part */
        }
        display_buf_info->stride = 4 * dst_rect->canvas.width; /* 4 for argb 8888 */

        par->modifying = TD_FALSE;
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    }
}

static td_void gfbg_refresh_1buf_prepare_opt(gfbg_blit_opt *blit_opt, gfbg_par *par)
{
    blit_opt->call_back = TD_TRUE;
    /* Non-blocking mode */
    blit_opt->block = TD_FALSE;

    if (par->display_info.antiflicker_mode == GFBG_ANTIFLICKER_TDE) {
        blit_opt->antiflicker_level = OT_FB_LAYER_ANTIFLICKER_NONE;
    }

    blit_opt->region_deflicker = TD_TRUE;

    blit_opt->param = &(par->layer_id);

    if (par->display_info.rotate_mode == OT_FB_ROTATE_180) {
        blit_opt->mirror_mode = OT_FB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = par->display_info.mirror_mode;
    }
}

static td_void gfbg_refresh_1buf_prepare_global_refresh(const ot_fb_buf *canvas_buf, ot_fb_buf *dst_rect,
                                                        gfbg_blit_opt *blit_opt)
{
    if (canvas_buf->canvas.height != dst_rect->canvas.height || canvas_buf->canvas.width != dst_rect->canvas.width) {
        /* Rotate 0 or 180 degrees, zoomed, then global refresh */
        blit_opt->scale = TD_TRUE;

        dst_rect->update_rect.x = 0;
        dst_rect->update_rect.y = 0;
        dst_rect->update_rect.width = dst_rect->canvas.width;
        dst_rect->update_rect.height = dst_rect->canvas.height;
    } else {
        /* Rotate 0 or 180 degrees, no zoomed, then partial refresh */
        dst_rect->update_rect = canvas_buf->update_rect;
    }
}

static td_void gfbg_refresh_1buf_prepare_compress(volatile gfbg_compress_info *compress_info, ot_fb_buf *dst_rect,
                                                  gfbg_par *par, gfbg_blit_opt *blit_opt)
{
    td_s32 ret;
    unsigned long lock_flag;
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if (compress_info->compress_open) {
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE to
         * indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        ret = memcpy_s((void *)&compress_info->update_rect, sizeof(ot_fb_rect), &dst_rect->update_rect,
            sizeof(ot_fb_rect));
        gfbg_unlock_unequal_eok_return_void(ret, &par->lock, lock_flag);
        compress_info->update_finished = TD_FALSE;
        blit_opt->compress = TD_TRUE;
    } else {
        blit_opt->compress = TD_FALSE;
    }

    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
}

static td_s32 gfbg_refresh_1buf_blit(const ot_fb_buf *canvas_buf, const ot_fb_buf *dst_rect,
    const gfbg_blit_opt *blit_opt)
{
    ot_tde_export_func *tde_export_func = TD_NULL;

    tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((tde_export_func == TD_NULL) || (tde_export_func->drv_tde_module_begin_job == TD_NULL) ||
        (tde_export_func->drv_tde_module_end_job == TD_NULL)) {
        gfbg_error("can't get TDE export function, it may be TDE module has not been inserted!\n");
        return TD_FAILURE;
    }
    /*
     * The user buf is used as the source by blit, and the user buf is moved to the display buf with
     * the target set (with the target showing the buff address) as the target.
     */
    if (gfbg_drv_blit(canvas_buf, dst_rect, blit_opt, TD_TRUE) < 0) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/* This function has a lock operation, so you can't call it if the caller has a lock operation. */
static td_s32 gfbg_refresh_1buf(td_u32 layer_id, const ot_fb_buf *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_dispbuf_info *display_buf_info = &refresh_info->disp_buf_info;
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    gfbg_osd_data osd_data;
    td_s32 ret;
    gfbg_blit_opt blit_opt = {0};
    ot_fb_buf dst_rect;

    ret = gfbg_refresh_1buf_prepare_dst(&dst_rect, par, display_info, display_buf_info, info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    gfbg_refresh_1buf_prepare_addr(refresh_info, layer_id, &osd_data, info, &dst_rect);

    gfbg_refresh_1buf_prepare_opt(&blit_opt, par);

    gfbg_refresh_1buf_prepare_global_refresh(canvas_buf, &dst_rect, &blit_opt);

    gfbg_refresh_1buf_prepare_compress(compress_info, &dst_rect, par, &blit_opt);

#ifdef CONFIG_GFBG_LOW_DELAY_SUPPORT
    if (is_layer_support_low_delay(layer_id) == TD_TRUE) {
        blit_opt.is_sync = TD_TRUE;
        blit_opt.block = TD_TRUE;
    }
#endif
    ret = gfbg_refresh_1buf_blit(canvas_buf, &dst_rect, &blit_opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    display_buf_info->refresh_handle = ret;
    ret = memcpy_s(&(refresh_info->user_buffer), sizeof(ot_fb_buf), canvas_buf, sizeof(ot_fb_buf));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/* unit rect */
static td_void gfbg_unite_rect(ot_fb_rect *dst_rect, const ot_fb_rect *src_rect)
{
    td_s32 ret;
    ot_fb_rect rect;
    rect.x = (dst_rect->x < src_rect->x) ? dst_rect->x : src_rect->x;
    rect.y = (dst_rect->y < src_rect->y) ? dst_rect->y : src_rect->y;
    rect.width = ((dst_rect->x + dst_rect->width) > (src_rect->x + src_rect->width)) ?
        (dst_rect->x + dst_rect->width - rect.x) : (src_rect->x + src_rect->width - rect.x);
    rect.height = ((dst_rect->y + dst_rect->height) > (src_rect->y + src_rect->height)) ?
        (dst_rect->y + dst_rect->height - rect.y) : (src_rect->y + src_rect->height - rect.y);
    ret = memcpy_s(dst_rect, sizeof(ot_fb_rect), &rect, sizeof(ot_fb_rect));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return;
    }
    return;
}

/* check these two rectangle cover each other */
static td_bool gfbg_iscontain(ot_fb_rect parent_rect, ot_fb_rect child_rect)
{
#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART11_H_ */
