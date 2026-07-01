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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART21_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART21_H_

/*
 * Name : gfbg_check_layerinfo
 * Desc :check layer information: buf refresh mode,position,canvas width
         and height, display width and height, screen width and height.
 */
static td_s32 gfbg_check_layerinfo(const ot_fb_layer_info* layer_info)
{
    if (layer_info->mask & OT_FB_LAYER_MASK_BUF_MODE) {
        if (layer_info->buf_mode > OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE) {
            gfbg_error("buf_mode(%d) is error, should between %d and %d\n", layer_info->buf_mode,
                       OT_FB_LAYER_BUF_DOUBLE, OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE);
            return TD_FAILURE;
        }
    }

    /* Detection of anti-flicker mode */
    if (layer_info->mask & OT_FB_LAYER_MASK_ANTIFLICKER_MODE) {
        if (layer_info->antiflicker_level > OT_FB_LAYER_ANTIFLICKER_AUTO) {
            gfbg_error("antiflicker_level(%d) is error, should between %d and %d\n", layer_info->antiflicker_level,
                       OT_FB_LAYER_ANTIFLICKER_NONE, OT_FB_LAYER_ANTIFLICKER_AUTO);
            return TD_FAILURE;
        }
    }

    /* check the width and height */
    if (layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) {
        if (layer_info->display_width % 2 || layer_info->display_height % 2) { /* 2 pixel align */
            gfbg_error("Disaplay W(%u) and H(%u) should align to 2!\n", layer_info->display_width,
                       layer_info->display_height);
            return TD_FAILURE;
        }
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE) {
        if (layer_info->screen_width % 2 || layer_info->screen_height % 2) { /* 2 pixel align */
            gfbg_error("Screenaplay W(%u) and H(%u) should align to 2!\n", layer_info->screen_width,
                       layer_info->screen_height);
            return TD_FAILURE;
        }
    }

    /* check pre-multiplier value. */
    if (layer_info->mask & OT_FB_LAYER_MASK_MUL) {
        if (TD_TRUE != layer_info->is_premul && TD_FALSE != layer_info->is_premul) {
            gfbg_error("pstLayerInfo->is_premul should be TRUE or FALSE but it is %d\n", layer_info->is_premul);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 check_display_size(const struct fb_info *info, const gfbg_par *par, const ot_fb_layer_info *layer_info,
                                 td_bool is_interlace)
{
    td_u32 pitch;
    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        /*
         * If the chip does not support scaling,
         * if both the display size and the screen size are set , both must be equal.
         */
        if ((layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) && (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE)) {
            if ((layer_info->display_width != layer_info->screen_width) ||
                (layer_info->display_height != layer_info->screen_height)) {
                gfbg_error("Layer %d doesn't support scaling. Display(%d, %d) is different with Screen(%d, %d).\n",
                    par->layer_id, layer_info->display_width, layer_info->display_height,
                    layer_info->screen_width, layer_info->screen_height);
                return TD_FAILURE;
            }
        }
    }

    /* Modify the display size, the memory size has changed, limited by the size of the memory */
    if (layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) {
        pitch = (layer_info->display_width * gfbg_get_bits_per_pixel(info)) >> 3; /* 3 for 8bit */
        pitch = (pitch + 0xf) & 0xfffffff0;
        if (gfbg_check_mem_enough(info, pitch, layer_info->display_height) == TD_FAILURE) {
            gfbg_error("memory is not enough!\n");
            return TD_FAILURE;
        }

        if (layer_info->display_width == 0 || layer_info->display_height == 0) {
            gfbg_error("display width/height shouldn't be 0!\n");
            return TD_FAILURE;
        }
        /*
         * For interlaced output, the height of the layer must be even.
         * Progressive output without this limit.
         */
        if (is_interlace && ((layer_info->display_height % 2) != 0)) { /* 2 for align */
            gfbg_error("display_height(%d) of layer_id %d should be even when vodev output is interlace\n",
                       layer_info->display_height, par->layer_id);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 onputlayerinfo_check_size(const struct fb_info *info, const gfbg_par *par,
                                        const ot_fb_layer_info *layer_info, td_bool is_interlace)
{
    /* Check the display size */
    if (check_display_size(info, par, layer_info, is_interlace) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* Check the canvas size */
    if (layer_info->mask & OT_FB_LAYER_MASK_CANVAS_SIZE) {
        if ((layer_info->canvas_width == 0) || (layer_info->canvas_height == 0)) {
            gfbg_error("canvas width/height shouldn't be 0\n");
            return TD_FAILURE;
        }
    }

    /* Check the screen size */
    if (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE) {
        if ((layer_info->screen_width == 0) || (layer_info->screen_height == 0)) {
            gfbg_error("screen width/height shouldn't be 0\n");
            return TD_FAILURE;
        }
       /*
        * For interlaced output, the height of the layer must be even.
        * Progressive output without this limit.
        */
        if (is_interlace && ((layer_info->screen_height % 2) != 0)) { /* 2 for align */
            gfbg_error("screen_height(%d) of layer_id %d should be even when vodev output is interlace\n",
                layer_info->screen_height, par->layer_id);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_void gfbg_calculate_argb8888_cmp_stride(const struct fb_info *info, td_u32 *cmp_stride, td_u32 *uncmp_stride)
{
    gfbg_stride_attr attr = {0};

    /* only argb8888 can support compress with economize memory */
    attr.format = gfbg_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue,
        &info->var.transp, info->var.bits_per_pixel);
    attr.width = gfbg_get_xres(info);
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;

    if (attr.format == OT_FB_FORMAT_ARGB8888) {
        gfbg_recalculate_stride(cmp_stride, uncmp_stride, &attr);
    } else {
        *cmp_stride = *uncmp_stride = gfbg_get_line_length(info);
    }

    return;
}

static td_s32 onputlayerinfo_check_buf_mode(struct fb_info *info, gfbg_par *par, ot_fb_layer_info *layer_info,
                                            td_bool is_interlace)
{
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    td_u32 uncmp_layer_size;
    td_u32 cmp_layer_size;
    td_u32 uncmp_stride;
    td_u32 cmp_stride;

    /* Modify the display buf mode, the memory size has changed, limited by the size of the memory */
    if (layer_info->mask & OT_FB_LAYER_MASK_BUF_MODE) {
        if (layer_info->buf_mode == OT_FB_LAYER_BUF_ONE) {
            uncmp_layer_size = gfbg_get_line_length(info) * gfbg_get_yres(info);
            cmp_layer_size = uncmp_layer_size;
        } else if ((layer_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE) ||
                   (layer_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
            gfbg_calculate_argb8888_cmp_stride(info, &cmp_stride, &uncmp_stride);
            uncmp_layer_size = 2 * uncmp_stride * gfbg_get_yres(info); /* 2 buf */
            cmp_layer_size = 2 * cmp_stride * gfbg_get_yres(info); /* 2 buf */
        } else {
            uncmp_layer_size = 0;
            cmp_layer_size = 0;
        }

        if (gfbg_get_smem_len(info) < uncmp_layer_size && gfbg_get_smem_len(info) < cmp_layer_size) {
            /*
             * layer real memory size:%d KBytes, expected:%d KBtyes
             * real:gfbg_get_smem_len(info)/1024, expectde:uncmp_layer_size/1024 or cmp_layer_size/1024
             */
            gfbg_error("No enough mem!real:%dKB;expected:uncmp%dKB;cmp%dKB\n",
                gfbg_get_smem_len(info) / 1024, uncmp_layer_size / 1024, cmp_layer_size / 1024); /* 1024 for KB */
            return TD_FAILURE;
        }

        if (gfbg_get_smem_len(info) < uncmp_layer_size && gfbg_get_smem_len(info) >= cmp_layer_size) {
            /* this should be FBIOPUT_COMPRESSION_GFBG */
            compress_info->is_economize_memory = TD_TRUE;
        }

        /* If compression is enabled, only 2buf mode can be set. */
        if (compress_info->compress_open) {
            if (layer_info->buf_mode != OT_FB_LAYER_BUF_DOUBLE &&
                layer_info->buf_mode != OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE) {
                gfbg_error("only GFBG_LAYER_BUF_DOUBLE/GFBG_LAYER_BUF_DOUBLE_IMMEDIATE support compress!\n");
                return TD_FAILURE;
            }
        }
    }

    /* if x>width or y>height ,how to deal with: see nothing in screen or return failure. */
    if (layer_info->mask & OT_FB_LAYER_MASK_POS) {
        if ((layer_info->x_pos < 0) || (layer_info->y_pos < 0)) {
            gfbg_error("It's not supported to set start pos of layer to negative!\n");
            return TD_FAILURE;
        }
        /*
         * For interlaced output, the start of the layer must be even.
         * Progressive output without this limit.
         */
        if (is_interlace && (layer_info->y_pos % 2 != 0)) { /* 2 for align */
            gfbg_error("y_pos should be even for interlace vodev!\n");
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 onputlayerinfo_check_premult(const gfbg_par* par, const ot_fb_layer_info *layer_info)
{
    /* Limit the pre-multiplication and color value. */
    if ((layer_info->mask & OT_FB_LAYER_MASK_MUL) && par->ckey.key_enable) {
        gfbg_error("Colorkey and premul couldn't take effect at same time!\n");
        return TD_FAILURE;
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_MUL) {
        /* return TD_FAILURE, not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444 */
        if ((layer_info->is_premul == TD_TRUE) &&
            (par->color_format == OT_FB_FORMAT_ARGB4444 || par->color_format == OT_FB_FORMAT_ARGB1555)) {
            gfbg_error("not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444\n");
            return TD_FAILURE;
        }
        /* not allow to set pre-mult mode when the GlobalAlpha is 1 */
        if ((layer_info->is_premul == TD_TRUE) &&
            (par->alpha.alpha_chn_en == TD_TRUE && par->alpha.global_alpha == 1)) {
            gfbg_error("not allow to set pre-mult mode when the GlobalAlpha is 1\n");
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_void onputlayerinfo_set_with_mask(gfbg_par* par, const ot_fb_layer_info *layer_info)
{
    gfbg_display_info *display_info = &par->display_info;
    ot_fb_point pos;

    if (layer_info->mask & OT_FB_LAYER_MASK_MUL) {
        display_info->is_premul = layer_info->is_premul;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_BMUL;
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_BUF_MODE) {
        gfbg_set_bufmode(par->layer_id, layer_info->buf_mode);
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_POS) {
        pos.x_pos = layer_info->x_pos;
        pos.y_pos = layer_info->y_pos;
        gfbg_set_layerpos(par, &pos);
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_ANTIFLICKER_MODE) {
        gfbg_set_antiflickerlevel(par->layer_id, layer_info->antiflicker_level);
    }
    return;
}

static td_s32 onputlayerinfo_process(gfbg_par* par, const ot_fb_layer_info *layer_info)
{
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    td_s32 ret = TD_SUCCESS;
    unsigned long lock_flag;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;

    /* deal with layer_info->mask */
    onputlayerinfo_set_with_mask(par, layer_info);

    /* set screen size in the scaling mode */
    if (g_drv_ops.capability[par->layer_id].is_vo_scale) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
         /*
          * The following two functions have a sleep operation inside,
          * you must unlock before calling, and lock the global amount inside the function.
          */
        if (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE) {
            ret = gfbg_disp_setscreensize(par->layer_id, layer_info->screen_width, layer_info->screen_height);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        }
        if (layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) {
            ret = gfbg_disp_setdispsize(par->layer_id, layer_info->display_width, layer_info->display_height);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        }
        gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    } else {
        /* no scaling mode, no buffer mode, screen size and display size can be set */
        if (refresh_info->buf_mode != OT_FB_LAYER_BUF_NONE) {
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            if (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE) {
                ret = gfbg_disp_setscreensize(par->layer_id, layer_info->screen_width, layer_info->screen_height);
            }
            if (ret != TD_SUCCESS) {
                return ret;
            }
            if (layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) {
                ret = gfbg_disp_setdispsize(par->layer_id, layer_info->display_width, layer_info->display_height);
            }
            if (ret != TD_SUCCESS) {
                return ret;
            }
            gfbg_spin_lock_irqsave(&par->lock, lock_flag);
        }
    }

    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_s32 gfbg_onputlayerinfo(struct fb_info *info, gfbg_par* par, const td_void __user *argp)
{
    td_s32 ret = TD_SUCCESS;
    ot_fb_layer_info layer_info;
    td_bool is_interlace = TD_FALSE;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        gfbg_warning("you shouldn't put cursor layer info!");
        return TD_SUCCESS;
    }

    if (argp == TD_NULL) {
        gfbg_error("NULL arg!\n");
        return -EINVAL;
    }

    if (osal_copy_from_user(&layer_info, argp, sizeof(ot_fb_layer_info))) {
        return -EFAULT;
    }

    ret = gfbg_check_layerinfo(&layer_info);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * Check the display size
     * Check the canvas size
     * Check the screen size
     */
    is_interlace = gfbg_is_interlace(par);
    if (onputlayerinfo_check_size(info, par, &layer_info, is_interlace) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * Check when modify buf mode
     * Check when modify pos
     */
    if (onputlayerinfo_check_buf_mode(info, par, &layer_info, is_interlace) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /* Check pre-mult */
    if (onputlayerinfo_check_premult(par, &layer_info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * avoid modifying register in vo isr before all params has been recorded!
     * In vo irq,flag modifying will be checked.
     */
    ret = onputlayerinfo_process(par, &layer_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    ret = gfbg_alloccanbuf(info, &layer_info);
    if (ret != TD_SUCCESS) {
        /*
         * There is no error returned here, because the user can also
         * specify this memory; in addition, even if the allocation is successful,
         * The user also needs to call FBIOGET_CANVAS_BUF to get it to operate.
         */
        gfbg_warning("alloc canvas buffer failed\n");
    }
    gfbg_refresh_again(par->layer_id);
    return ret;
}

static td_s32 gfbg_dosetcolreg(const gfbg_cmp_reg *color_reg, const struct fb_info *info, td_bool update_finished_reg)
{
    gfbg_par *par = (gfbg_par *)info->par;

    td_u32 argb = ((color_reg->transp & 0xff) << 24) | ((color_reg->red & 0xff) << 16) | /* 16 24 alg data */
        ((color_reg->green & 0xff) << 8) | (color_reg->blue & 0xff); /* 8 alg data */

    if (color_reg->regno > 255) { /* 255 is larger than */
        gfbg_warning("regno: %d, larger than 255!\n", color_reg->regno);
        return TD_FAILURE;
    }
    g_drv_ops.gfbg_drv_set_color_reg(par->layer_id, color_reg->regno, argb, update_finished_reg);
    return TD_SUCCESS;
}

/* G.FUN.01: fb_ops.fb_setcolreg requires this callback signature. */
static td_s32 gfbg_setcolreg(unsigned regno, unsigned red, unsigned green, unsigned blue, unsigned transp,
    struct fb_info *info)
{
    gfbg_cmp_reg cmp_reg = {
        0
    };
    cmp_reg.regno = regno;
    cmp_reg.red = red;
    cmp_reg.green = green;
    cmp_reg.blue = blue;
    cmp_reg.transp = transp;
    return gfbg_dosetcolreg(&cmp_reg, info, TD_TRUE);
}

static td_void gfbg_set_color_reg(struct fb_cmap *cmap, struct fb_info *info)
{
    td_u32 i;
    td_s32 start;
    unsigned short *red = TD_NULL;
    unsigned short *green = TD_NULL;
    unsigned short *blue = TD_NULL;
    unsigned short *transp = TD_NULL;
    gfbg_cmp_reg cmp_reg = {
        0
    };

#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART21_H_ */
