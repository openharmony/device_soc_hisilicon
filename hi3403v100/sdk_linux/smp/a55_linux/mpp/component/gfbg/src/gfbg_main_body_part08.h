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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART08_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART08_H_

#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    td_u32 layer_id = 0;

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }
    if (osal_copy_from_user(&layer_id, argp, sizeof(td_u32))) {
        return -EFAULT;
    }

    if ((layer_id >= g_drv_ops.layer_count) || (g_drv_ops.capability[layer_id].is_layer_support == TD_FALSE)) {
        gfbg_error("LayerId %u should be in [0, %u) or it is not supported!\n", layer_id, g_drv_ops.layer_count);
        return -EFAULT;
    }

    if (is_cursor_layer(layer_id)) {
        gfbg_error("Detach cursor to itself or other cursor layer!\n");
        return TD_FAILURE;
    }
    return ot_fb_cursor_detach(layer_id);
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_set_layer_info(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;

    par = (gfbg_par *)info->par;
    return gfbg_onputlayerinfo(info, par, argp);
}

static td_s32 drv_gfbg_get_layer_info(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_layer_info layer_info = {0};
    par = (gfbg_par *)info->par;
    gfbg_get_layerinfo(par, &layer_info);
    return osal_copy_to_user(argp, &layer_info, sizeof(ot_fb_layer_info));
}

static td_s32 drv_gfbg_get_canvas_buffer(struct fb_info *info, unsigned long arg)
{
    td_s32 ret;
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_buf buf = {0};
    gfbg_refresh_info *refresh_info = TD_NULL;
    par = (gfbg_par *)info->par;
    refresh_info = &par->refresh_info;
    if (!osal_in_interrupt()) {
        ret = ot_mmz_check_phys_addr(par->canvas_sur.phys_addr, par->canvas_sur.pitch * par->canvas_sur.height);
        if (ret != TD_SUCCESS) {
            gfbg_error("check mem share failed !phys addr is(0x%lx). \n",
                (td_ulong)par->canvas_sur.phys_addr);
            return TD_FAILURE;
        }
    }
    ret = memcpy_s(&(buf.canvas), sizeof(ot_fb_surface), &(par->canvas_sur), sizeof(ot_fb_surface));
    gfbg_unequal_eok_return(ret);
    ret = memcpy_s(&(buf.update_rect), sizeof(ot_fb_rect), &(refresh_info->user_buffer.update_rect),
        sizeof(ot_fb_rect));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    if (osal_copy_to_user(argp, &(buf), sizeof(ot_fb_buf))) {
        return -EFAULT;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_refresh_layer(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)info->par;
    return gfbg_onrefresh(par, argp);
}

static td_s32 drv_gfbg_wait_refresh_finish(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_layer_buf buf_mode;
    par = (gfbg_par *)info->par;
    ot_unused(arg);
    gfbg_get_bufmode(par, &buf_mode);
    if ((buf_mode != OT_FB_LAYER_BUF_NONE) && (buf_mode != OT_FB_LAYER_BUF_BUTT)) {
        /* 80 is timeout */
        if (wait_event_timeout(par->vbl_event, par->vblflag, (td_s32)msecs_to_jiffies(80)) == 0) {
            return TD_FAILURE;
        }
    } else {
        gfbg_error("doesn't support FBIO_WAITFOR_FREFRESH_DONE operation when in standard mode"
                   "or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_mirror_mode(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_mirror_mode mirror_mode;
    par = (gfbg_par *)info->par;
    if (osal_copy_from_user(&mirror_mode, argp, sizeof(ot_fb_mirror_mode))) {
        return -EFAULT;
    }
    if (gfbg_set_mirrormode(par, mirror_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_mirror_mode(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_layer_buf buf_mode = 0;
    ot_fb_mirror_mode mirror_mode;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    gfbg_get_bufmode(par, &buf_mode);
    mirror_mode = display_info->mirror_mode;
    if ((buf_mode == OT_FB_LAYER_BUF_BUTT) || (buf_mode == OT_FB_LAYER_BUF_NONE)) {
        gfbg_error("doesn't support FBIOGET_MIRROR_MODE operation when in standard mode"
                   "or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }
    if (osal_copy_to_user(argp, &mirror_mode, sizeof(ot_fb_mirror_mode))) {
        return -EFAULT;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_rotate_mode(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    ot_fb_rotate_mode rotate_mode;

    if (osal_copy_from_user(&rotate_mode, argp, sizeof(ot_fb_rotate_mode))) {
        return -EFAULT;
    /* Reset compression information */
    }
    if (gfbg_get_rotation_support() != TD_TRUE) {
        gfbg_error("doesn't support rotate\n");
        return TD_FAILURE;
    }
    if (gfbg_set_rotatemode(info, rotate_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_rotate_mode(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_layer_buf buf_mode = 0;
    ot_fb_rotate_mode rotate_mode;

    if (gfbg_get_rotation_support() != TD_TRUE) {
        gfbg_error("doesn't support rotate\n");
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    gfbg_get_bufmode(par, &buf_mode);

    if ((buf_mode == OT_FB_LAYER_BUF_BUTT) || (buf_mode == OT_FB_LAYER_BUF_NONE)) {
        gfbg_error("doesn't support FBIOGET_ROTATE_MODE operation when in standard mode"
                   "or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }
    rotate_mode = display_info->rotate_mode;
    if (osal_copy_to_user(argp, &rotate_mode, sizeof(ot_fb_rotate_mode))) {
        return -EFAULT;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_screen_size(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_size  screen_size;
    ot_fb_size  max_screen_size = {0};
    ot_fb_point pos = {0};
    unsigned long lock_flag;

    par = (gfbg_par *)info->par;
    if (is_cursor_layer(par->layer_id) && is_soft_cursor())  {
        gfbg_error("you shouldn't set soft cursor screensize!");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&screen_size, argp, sizeof(ot_fb_size))) {
        return -EFAULT;
    }
    if ((screen_size.width == 0) || (screen_size.height == 0)) {
        gfbg_error("screen width(%u) height(%u) shouldn't be 0\n", screen_size.width, screen_size.height);
        return TD_FAILURE;
    }
    if (screen_size.width % 2 || screen_size.height % 2) { /* 2 for align */
        gfbg_error("stScreenSize (%u, %u) should align to 2!\n", screen_size.width, screen_size.height);
        return TD_FAILURE;
    }
    gfbg_get_maxscreensize(par, &max_screen_size.width, &max_screen_size.height);
    gfbg_get_layerpos(par, &pos);
    if (screen_size.width > max_screen_size.width - pos.x_pos) {
        gfbg_warning("the sum of width(%d) and x_pos(%d) larger than Vodev screen width(%d),"
                     "width will be changed!\n",
                     screen_size.width, pos.x_pos, max_screen_size.width);
        screen_size.width = max_screen_size.width - pos.x_pos;
    }
    if (screen_size.height > max_screen_size.height - pos.y_pos) {
        gfbg_warning("the sum of height(%d) and y_pos(%d) larger than Vodev screen height(%d),"
                     "width will be changed!\n",
                     screen_size.height, pos.y_pos, max_screen_size.height);
        screen_size.height = max_screen_size.height - pos.y_pos;
    }
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;
    if (gfbg_set_screensize(par, &screen_size.width, &screen_size.height) == TD_SUCCESS) {
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;
    }
    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_screen_size(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_size screen_size = {0};
    par = (gfbg_par *)info->par;
    gfbg_get_screensize(par, &screen_size.width, &screen_size.height);
    return osal_copy_to_user(argp, &screen_size, sizeof(ot_fb_size));
}

static td_s32 flip_surface_check_param(const struct fb_info *info, const ot_fb_surfaceex *surface_ex)
{
    gfbg_par *par = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    unsigned long addr;
    unsigned long smem_end;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    if (surface_ex->colorkey.enable != TD_TRUE && surface_ex->colorkey.enable != TD_FALSE) {
        gfbg_error("colorkey.enable(%d) should be TRUE or FALSE!\n", surface_ex->colorkey.enable);
        return TD_FAILURE;
    }

    if (surface_ex->alpha.alpha_en != TD_TRUE && surface_ex->alpha.alpha_en != TD_FALSE) {
        gfbg_error("alpha.alpha_en(%d) should be TRUE or FALSE!\n", surface_ex->alpha.alpha_en);
        return TD_FAILURE;
    }

    if (surface_ex->alpha.alpha_chn_en != TD_TRUE && surface_ex->alpha.alpha_chn_en != TD_FALSE) {
        gfbg_error("alpha.alpha_chn_en (%d) should be TRUE or FALSE!\n", surface_ex->alpha.alpha_chn_en);
        return TD_FAILURE;
    }

    if ((surface_ex->colorkey.enable && !g_drv_ops.capability[par->layer_id].is_key_rgb) &&
        (!g_drv_ops.capability[par->layer_id].is_key_alpha)) {
        gfbg_error("Layer %d doesn't support colorkey!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (surface_ex->colorkey.enable && display_info->is_premul) {
        gfbg_error("colorkey and premul couldn't take effect at the same time!\n");
        return TD_FAILURE;
    }

    addr = (unsigned long)surface_ex->phys_addr;
    smem_end = gfbg_get_smem_start(info) + gfbg_get_smem_len(info) - gfbg_get_yres(info) * gfbg_get_line_length(info);
    if ((addr < gfbg_get_smem_start(info)) || (addr > smem_end)) {
        gfbg_error("the addr is out of range!\n");
        return TD_FAILURE;
    }

    if (gfbg_get_line_length(info) == 0) {
        return TD_FAILURE;
    }

    if (gfbg_get_bits_per_pixel(info) == 0) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

#ifdef __LITEOS__
static td_s32 flip_surface_pan_display(struct fb_info *info, const ot_fb_surfaceex *surface_ex,
    gfbg_colorkeyex *colorkey_ex)
{
    td_s32 ret;
    unsigned long addr;
    unsigned int differ;
    unsigned int x_offset;
    unsigned int y_offset;
    struct fb_overlayinfo_s oinfo;
    addr = (unsigned long)surface_ex->phys_addr;
    differ = addr - gfbg_get_smem_start(info);
    y_offset = differ / gfbg_get_line_length(info);
    /* 3 is 8 bit */
    x_offset = (((differ % gfbg_get_line_length(info)) << 3) / (gfbg_get_bits_per_pixel(info)));

    ret = memcpy_s(&oinfo, sizeof(oinfo), &info->oinfo, sizeof(oinfo));
    gfbg_unequal_eok_return(ret);
    oinfo.sarea.x = x_offset;
    oinfo.sarea.y = y_offset;

    if (gfbg_pan_display(info, &oinfo) < 0) {
        gfbg_error("gfbg_pan_display error !\n");
        return TD_FAILURE;
    }

    info->oinfo.sarea.x = x_offset;
    info->oinfo.sarea.y = y_offset;
    colorkey_ex->key = surface_ex->colorkey.value;
    colorkey_ex->key_enable = surface_ex->colorkey.enable;
    if (gfbg_get_bits_per_pixel(info) <= 8) { /* 8 bit */
        if (surface_ex->colorkey.value >= (1 << gfbg_get_bits_per_pixel(info))) {
            gfbg_error("The value :%d is out of range the palette: %d!\n", surface_ex->colorkey.value,
                1 << gfbg_get_bits_per_pixel(info));
            return TD_FAILURE;
        }
        if ((info->cmap.blue != NULL) || (info->cmap.green != NULL) || (info->cmap.red != NULL)) {
            colorkey_ex.blue_max = colorkey_ex.blue_min = info->cmap.blue[surface_ex->colorkey.value];
            colorkey_ex.green_max = colorkey_ex.green_min = info->cmap.green[surface_ex->colorkey.value];
            colorkey_ex.red_max = colorkey_ex.red_min = info->cmap.red[surface_ex->colorkey.value];
            colorkey_ex.red_mask = 0xff;
            colorkey_ex.green_mask = 0xff;
            colorkey_ex.blue_mask = 0xff;
            colorkey_ex.key_mode = 0;
            colorkey_ex.mask_enable = TD_TRUE;
        }
    } else {
        g_drv_ops.gfbg_drv_color_convert(info, colorkey_ex);
    }
    return TD_SUCCESS;
}
#else
static td_s32 flip_surface_pan_display(struct fb_info *info, const ot_fb_surfaceex *surface_ex,
    gfbg_colorkeyex *colorkey_ex)
{
    td_s32 ret;
    unsigned long addr;
    unsigned int differ;
    unsigned int x_offset;
    unsigned int y_offset;
    struct fb_var_screeninfo var;
    addr = (unsigned long)surface_ex->phys_addr;
    differ = addr - gfbg_get_smem_start(info);
    y_offset = differ / gfbg_get_line_length(info);
    /* 8 bit (2^3) */
    x_offset = (((differ % gfbg_get_line_length(info)) << 3) / (gfbg_get_bits_per_pixel(info)));
    ret = memcpy_s(&var, sizeof(var), &info->var, sizeof(var));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    var.xoffset = x_offset;
    var.yoffset = y_offset;

    if (fb_pan_display(info, &var) < 0) {
        gfbg_error("fb_pan_display error!\n");
        return TD_FAILURE;
    }

    colorkey_ex->key = surface_ex->colorkey.value;
    colorkey_ex->key_enable = surface_ex->colorkey.enable;

    if (gfbg_get_bits_per_pixel(info) <= 8UL) { /* 8 bits */
        if (surface_ex->colorkey.value >= (2UL << gfbg_get_bits_per_pixel(info))) { /* 2 for calculate */
            gfbg_error("The value :%d is out of range the palette: %d!\n", surface_ex->colorkey.value,
                       2 << gfbg_get_bits_per_pixel(info)); /* 2 for calculate */
            return TD_FAILURE;
        }

        colorkey_ex->blue_max = colorkey_ex->blue_min = info->cmap.blue[surface_ex->colorkey.value];
        colorkey_ex->green_max = colorkey_ex->green_min = info->cmap.green[surface_ex->colorkey.value];
        colorkey_ex->red_max = colorkey_ex->red_min = info->cmap.red[surface_ex->colorkey.value];
    } else {
        g_drv_ops.gfbg_drv_color_convert(&info->var, colorkey_ex);
    }

    return TD_SUCCESS;
}
#endif

static td_s32 drv_gfbg_flip_surface(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_surfaceex surface_ex;
    unsigned long lock_flag;
    gfbg_colorkeyex colorkey_ex = {0};

    par = (gfbg_par *)info->par;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        gfbg_error("you shouldn't use FBIOFLIP_SURFACE for soft cursor!");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&surface_ex, argp, sizeof(ot_fb_surfaceex))) {
        return -EFAULT;
    }


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART08_H_ */
