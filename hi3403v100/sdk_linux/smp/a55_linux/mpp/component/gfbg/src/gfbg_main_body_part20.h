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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART20_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART20_H_

#ifdef __LITEOS__
static td_s32 alloc_new_canvas_buffer(const struct fb_info *info, const ot_fb_layer_info *layer_info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    ot_fb_surface *canvas_surface = &par->canvas_sur;
    td_u32 layer_size;
    td_u32 pitch;
    td_char *buf = TD_NULL;
    td_char name[16]; /* 16 name max length */
    struct fb_info *info_temp = TD_NULL;
    /*
     * Applying fb_info without private data is not for registration,
     * but for the convenience of passing to gfbg_fill_data to empty the memory.
     */
    info_temp = gfbg_alloc(0);
    if (info_temp == TD_NULL) {
        return TD_FAILURE;
    }

    /* 16 bytes alignment */
    pitch = (((layer_info->canvas_width * gfbg_get_bits_per_pixel(info)) >> 3) + 15) >> 4; /* 3 15 4 for align */
    pitch = pitch << 4; /* 4 for align */

    layer_size = pitch * layer_info->canvas_height;
    /* alloc new buffer */
    if (snprintf_s(name, sizeof(name), 13, "gfbg_canvas%01u", par->layer_id) < 0) { /* 13:for char length */
        gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        gfbg_free(info_temp);
        return TD_FAILURE;
    }
    canvas_surface->phys_addr = gfbg_buf_allocmem(name, layer_size, gfbg_get_layer_mmz_names(par->layer_id));
    if (canvas_surface->phys_addr == 0) {
        gfbg_error("alloc canvas buffer no mem");
        gfbg_free(info_temp);
        return TD_FAILURE;
    }

    buf = (td_char *)gfbg_buf_map(canvas_surface->phys_addr, layer_size);
    if (buf == TD_NULL) {
        gfbg_error("map canvas buffer failed!\n");
        gfbg_buf_freemem(canvas_surface->phys_addr);
        gfbg_free(info_temp);
        return TD_FAILURE;
    }
    /* gfbg_fill_data is faster than memset_s */
    info_temp->oinfo.fbmem = (uintptr_t)canvas_surface->phys_addr;
    (td_void)memset_s(info_temp->oinfo.fbmem, layer_size, 0x0, layer_size);

    gfbg_buf_ummap(buf);

    gfbg_info("alloc new memory for canvas buffer success\n");
    canvas_surface->width = layer_info->canvas_width;
    canvas_surface->height = layer_info->canvas_height;
    canvas_surface->pitch = pitch;
    par->canvas_sur.format =  info->vinfo.format;
    gfbg_free(info_temp);

    return TD_SUCCESS;
}
#else
static td_s32 alloc_new_canvas_buffer(const struct fb_info *info, const ot_fb_layer_info *layer_info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    ot_fb_surface *canvas_surface = &par->canvas_sur;
    td_u32 layer_size;
    td_u32 pitch;
    td_char *buf = TD_NULL;
    td_char name[16]; /* 16 name max length */
    struct fb_info *info_temp = TD_NULL;
    /*
     * Applying fb_info without private data is not for registration,
     * but for the convenience of passing to gfbg_fill_data to empty the memory.
     */
    info_temp = framebuffer_alloc(0, TD_NULL);
    if (info_temp == TD_NULL) {
        return TD_FAILURE;
    }

    /* 16 bytes alignment */
    pitch = (((layer_info->canvas_width * gfbg_get_bits_per_pixel(info)) >> 3) + 15) >> 4; /* 3 4 15 alg data */
    pitch = pitch << 4; /* 4 alg data */

    layer_size = pitch * layer_info->canvas_height;
    /* alloc new buffer */
    if (snprintf_s(name, sizeof(name), 13, "gfbg_canvas%01u", par->layer_id) < 0) { /* 13:for char length */
        gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        framebuffer_release(info_temp);
        return TD_FAILURE;
    }
    canvas_surface->phys_addr = gfbg_buf_allocmem(name, layer_size, gfbg_get_layer_mmz_names(par->layer_id));
    if (canvas_surface->phys_addr == 0) {
        gfbg_error("alloc canvas buffer no mem");
        framebuffer_release(info_temp);
        return TD_FAILURE;
    }

    buf = (td_char *)gfbg_buf_map(canvas_surface->phys_addr, layer_size);
    if (buf == TD_NULL) {
        gfbg_error("map canvas buffer failed!\n");
        gfbg_buf_freemem(canvas_surface->phys_addr);
        framebuffer_release(info_temp);
        return TD_FAILURE;
    }
    /* gfbg_fill_data is faster than memset_s */
    info_temp->fix.smem_start = (td_ulong)canvas_surface->phys_addr;
    info_temp->screen_base = buf;
    if (gfbg_fill_data(info_temp, 0, layer_size) != TD_SUCCESS) {
        gfbg_error("gfbg_fill_data failed!\n");
    }
    gfbg_buf_ummap(buf);

    gfbg_info("alloc new memory for canvas buffer success\n");
    canvas_surface->width = layer_info->canvas_width;
    canvas_surface->height = layer_info->canvas_height;
    canvas_surface->pitch = pitch;
    canvas_surface->format = gfbg_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp,
                                               info->var.bits_per_pixel);
    framebuffer_release(info_temp);
    return TD_SUCCESS;
}
#endif

static td_s32 gfbg_alloccanbuf(const struct fb_info *info, const ot_fb_layer_info *layer_info)
{
    gfbg_par *par = TD_NULL;
    ot_fb_surface *canvas_surface = TD_NULL;

    if ((info == TD_NULL) || (layer_info == TD_NULL)) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    canvas_surface = &par->canvas_sur;

    if (!(layer_info->mask & OT_FB_LAYER_MASK_CANVAS_SIZE)) {
        return TD_SUCCESS;
    }

    /* if  with old canvas buffer */
    if (canvas_surface->phys_addr != 0) {
        /* if old is the same with new , then return, else free the old buffer */
        if ((layer_info->canvas_width == canvas_surface->width) &&
            (layer_info->canvas_height == canvas_surface->height)) {
            gfbg_info("mem size is the same , no need alloc new memory");
            return TD_SUCCESS;
        }

        /* free new old buffer */
        gfbg_info("free old canvas buffer\n");
        gfbg_freeccanbuf(par);
    }
    if (layer_info->canvas_width > GFBG_4K_DEF_WIDTH || layer_info->canvas_height > GFBG_4K_DEF_HEIGHT) {
        gfbg_info("unsupported too large w(%d) and h(%d)!max width (%d), max height (%d)\n",
            layer_info->canvas_width, layer_info->canvas_height, GFBG_4K_DEF_WIDTH, GFBG_4K_DEF_HEIGHT);
        return TD_FAILURE;
    }
    /* new canvas buffer */
    if (alloc_new_canvas_buffer(info, layer_info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 onrefresh_check_param(const gfbg_par* par, const ot_fb_buf *canvas_buf, ot_fb_layer_buf buf_mode)
{
    ot_unused(par);
    if (canvas_buf->canvas.format >= OT_FB_FORMAT_BUTT) {
        return TD_FAILURE;
    }

    if (buf_mode == OT_FB_LAYER_BUF_BUTT) {
        gfbg_error("doesn't support FBIO_REFRESH operation when refresh mode is FB_LAYER_BUF_BUTT!\n");
        return TD_FAILURE;
    }

    if ((canvas_buf->update_rect.x >=  (td_s32)canvas_buf->canvas.width) ||
        (canvas_buf->update_rect.y >= (td_s32)canvas_buf->canvas.height) ||
        (canvas_buf->update_rect.width <= 0) || (canvas_buf->update_rect.height <= 0) ||
        (canvas_buf->canvas.width == 0) || (canvas_buf->canvas.height == 0)) {
        gfbg_error("rect error: update rect:(%d,%d,%d,%d), canvas range:(%d,%d)\n",
                   canvas_buf->update_rect.x, canvas_buf->update_rect.y,
                   canvas_buf->update_rect.width, canvas_buf->update_rect.height,
                   canvas_buf->canvas.width, canvas_buf->canvas.height);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 onrefresh_get_canvas_buf(gfbg_par *par, td_void __user *argp, ot_fb_buf *canvas_buf)
{
    unsigned long lock_flag;
    ot_fb_layer_buf buf_mode;
    if (osal_copy_from_user(canvas_buf, argp, sizeof(ot_fb_buf))) {
        return -EFAULT;
    }

    gfbg_get_bufmode(par, &buf_mode);
    /*
     * check canvas format
     * check canvas phyaddr
     * check buf_mode
     * check canvas update rect legality
     */
    if (onrefresh_check_param(par, canvas_buf, buf_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* update canvas update_rect */
    if (canvas_buf->update_rect.x + canvas_buf->update_rect.width > (td_s32)canvas_buf->canvas.width) {
        canvas_buf->update_rect.width = canvas_buf->canvas.width - canvas_buf->update_rect.x;
    }

    if (canvas_buf->update_rect.y + canvas_buf->update_rect.height > (td_s32)canvas_buf->canvas.height) {
        canvas_buf->update_rect.height =  canvas_buf->canvas.height - canvas_buf->update_rect.y;
    }

    /* Check the legality of physical address and size */
    /* CMPI_CheckMmzphy_addr Call a semaphore, Cannot be used in the interrupt */
    if (!osal_in_interrupt()) {
        if (gfbg_check_phyaddr(canvas_buf) != TD_SUCCESS) {
            gfbg_error("check mem share failed !phys addr is(0x%lx).\n", (td_ulong)canvas_buf->canvas.phys_addr);
            return TD_FAILURE;
        }
    }

    if (buf_mode == OT_FB_LAYER_BUF_NONE) {
        /* Check if the format of the canvas supported or not by gfbg */
        if ((par->layer_id >= GFBG_MAX_LAYER_NUM) || (canvas_buf->canvas.format >= OT_FB_FORMAT_BUTT)) {
            return TD_FAILURE;
        }
        if (!g_drv_ops.capability[par->layer_id].is_color_format[canvas_buf->canvas.format]) {
            gfbg_error("Unsupported PIXEL FORMAT!\n");
            return -EINVAL;
        }
        /*
         * there's a limit from hardware that the start address of screen buf
         * should be 16byte aligned.
         */
        gfbg_spin_lock_irqsave(&par->lock, lock_flag);
        if ((canvas_buf->canvas.phys_addr & 0xf) || (canvas_buf->canvas.pitch & 0xf)) {
            gfbg_error("addr 0x%lx or pitch: 0x%x is not 16 bytes align !\n", (td_ulong)canvas_buf->canvas.phys_addr,
                       canvas_buf->canvas.pitch);
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            return TD_FAILURE;
        }
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    } else {
        /*
         * Check if the format of the canvas and the format of
         * GFBG belong to the category that TDE can support
         */
        if ((!gfbg_tde_is_support_fmt(canvas_buf->canvas.format)) || (!gfbg_tde_is_support_fmt(par->color_format))) {
            gfbg_error("Unsupported PIXEL FORMAT!CanvasFmt:%d,par->color_format:%d\n", canvas_buf->canvas.format,
                       par->color_format);
            return -EINVAL;
        }
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_onrefresh(gfbg_par* par, td_void __user *argp)
{
    td_s32 ret;
    ot_fb_buf canvas_buf;
    ot_fb_layer_buf buf_mode;
    gfbg_display_info *diplay_info = &par->display_info;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        gfbg_warning("you shouldn't refresh cursor layer!\n");
        return TD_SUCCESS;
    }

    if (argp == TD_NULL) {
        gfbg_error("NULL arg!\n");
        return -EINVAL;
    }

    /* get canvas buffer and check legality */
    ret = onrefresh_get_canvas_buf(par, argp, &canvas_buf);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    gfbg_get_bufmode(par, &buf_mode);
    if ((buf_mode == OT_FB_LAYER_BUF_NONE) && (diplay_info->mirror_mode != OT_FB_MIRROR_NONE)) {
        gfbg_error("Can't do mirror when the layer buf is none!\n");
        return -EINVAL;
    }

    if ((buf_mode == OT_FB_LAYER_BUF_NONE) && (diplay_info->rotate_mode != OT_FB_ROTATE_NONE)) {
        gfbg_error("Can't do rotate when the layer buf is none!\n");
        return -EINVAL;
    }
    ret = gfbg_refresh(par->layer_id, &canvas_buf, buf_mode);

    return ret;
}

static td_s32 gfbg_set_mirrormode(gfbg_par *par, ot_fb_mirror_mode mirror_mode)
{
    gfbg_display_info *display_info = &par->display_info;
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    ot_fb_layer_buf buf_mode;

    gfbg_get_bufmode(par, &buf_mode);

    if ((buf_mode == OT_FB_LAYER_BUF_BUTT) || (buf_mode == OT_FB_LAYER_BUF_NONE)) {
        gfbg_error("doesn't support FBIOPUT_MIRROR_MODE operation when in standard mode or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }

    if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
        (par->color_format != OT_FB_FORMAT_ARGB8888)) {
        gfbg_error("Mirror doesn't support this format:%d.\n!\n", par->color_format);
        return -EINVAL;
    }

    if (mirror_mode >= OT_FB_MIRROR_BUTT) {
        gfbg_error("The input mirror mode is wrong!\n");
        return -EINVAL;
    }

    if ((mirror_mode != OT_FB_MIRROR_NONE) && (display_info->rotate_mode != OT_FB_ROTATE_NONE)) {
        gfbg_error("Can't do mirror when rotate!\n");
        return -EINVAL;
    }

    if ((mirror_mode != OT_FB_MIRROR_NONE) && (compress_info->compress_open == TD_TRUE)) {
        gfbg_error("Can't do mirror when compression is on!\n");
        return -EINVAL;
    }

    display_info->mirror_mode = mirror_mode;
    return TD_SUCCESS;
}

static td_s32 rotate_vb_init(const struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    td_void *mmz_name = TD_NULL;
    td_u32 size;
    td_char name[GFBG_ROTBUF_NAME_LEN];
    td_u64 *vir_rotate_vb = TD_NULL;
    ot_mpp_chn chn;
    chn.mod_id  = OT_ID_FB;
    chn.dev_id = gfbg_vou_get_dev_id(par->layer_id);
    chn.chn_id = 0;

    size = gfbg_get_line_length(info) * (((display_info->display_height + 15) / 16) * 16); /* 15 16 align */

    if (func_entry(sys_export_func, OT_ID_SYS)->pfn_get_mmz_name(&chn, &mmz_name)) {
        gfbg_error("get mmz name fail!\n");
        return ENOMEM;
    }

    if (snprintf_s(name, sizeof(name), 13, "gfbg%01u_rotate", par->layer_id) < 0) { /* 13:for char length */
        gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    par->rotate_vb = cmpi_mmz_malloc(mmz_name, name, size, TD_FALSE);
    if (par->rotate_vb == 0) {
        gfbg_error("get buffer fail,size %u !\n", size);
        return ENOMEM;
    }

    vir_rotate_vb = cmpi_remap_nocache(par->rotate_vb, size);
    if (vir_rotate_vb == TD_NULL) {
        gfbg_error("get buffer fail,size %u !\n", size);
        cmpi_mmz_free(par->rotate_vb, TD_NULL);
        return ENOMEM;
    }

    (td_void)memset_s(vir_rotate_vb, size, 0x0, size);

    cmpi_unmap(vir_rotate_vb);
    return TD_SUCCESS;
}

static td_s32 gfbg_set_rotatemode(const struct fb_info *info, ot_fb_rotate_mode rotate_mode)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    ot_fb_layer_buf buf_mode;
    volatile gfbg_compress_info *compress_info = &par->compress_info;

    gfbg_get_bufmode(par, &buf_mode);

    if (buf_mode == OT_FB_LAYER_BUF_BUTT || buf_mode == OT_FB_LAYER_BUF_NONE) {
        gfbg_error("doesn't support FBIOPUT_ROTATE_MODE when in standard mode or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }

    if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
        (par->color_format != OT_FB_FORMAT_ARGB8888)) {
        gfbg_error("rotate only support FB_FORMAT_ARGB4444 and FB_FORMAT_ARGB1555 which is %d\n!\n",
                   par->color_format);
        return -EINVAL;
    }

    if (rotate_mode >= OT_FB_ROTATE_BUTT) {
        gfbg_error("The input rotate mode is wrong!\n");
        return -EINVAL;
    }

    if (compress_info->compress_open && rotate_mode != OT_FB_ROTATE_NONE) {
        gfbg_error("Can't rotate when in compress mode!\n");
        return -EINVAL;
    }

    if ((rotate_mode != OT_FB_ROTATE_NONE) && (display_info->mirror_mode != OT_FB_MIRROR_NONE)) {
        gfbg_error("Can't do rotate when mirror!\n");
        return -EINVAL;
    }

    if ((rotate_mode == OT_FB_ROTATE_90 || rotate_mode == OT_FB_ROTATE_270) && (par->rotate_vb == 0)) {
        /* init rotate vb */
        if (rotate_vb_init(info) != TD_SUCCESS) {
            return ENOMEM;
        }
    }

    display_info->rotate_mode = rotate_mode;

    return TD_SUCCESS;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART20_H_ */
