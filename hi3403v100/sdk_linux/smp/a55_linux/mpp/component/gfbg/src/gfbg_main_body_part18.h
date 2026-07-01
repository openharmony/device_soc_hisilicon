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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART18_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART18_H_


/* Does the chip support scaling */
static td_bool gfbg_check_imagezoomenable(td_u32 layer_id, const ot_fb_rect *in_rect, const ot_fb_rect *out_rect)
{
    td_bool is_layer_support_zoom_out = TD_TRUE;
    td_bool need_zoom;

    if (!g_drv_ops.capability[layer_id].is_vo_scale) {
        return TD_FALSE;
    }

    if (g_drv_ops.gfbg_drv_is_layer_support_zoom_out) {
        is_layer_support_zoom_out = g_drv_ops.gfbg_drv_is_layer_support_zoom_out(layer_id);
    }

    /* The chip does not support zoomout, and the current image output size is smaller than the input size. */
    if ((is_layer_support_zoom_out == TD_FALSE) &&
        ((out_rect->width < in_rect->width) || (out_rect->height < in_rect->height))) {
        gfbg_error("GFBG layer%d not support zoomout, please check the display and screen size!out_rect[%d, %d]"
            "in_rect[%d, %d]\n", layer_id, out_rect->width, out_rect->height, in_rect->width, in_rect->height);
        return TD_FALSE;
    }

    /* The chip zoomin ratio exceeds the maximum allowed by the chip, and the error is returned. */
    if (out_rect->width > (in_rect->width * GFBG_MAX_ZOOMIN) ||
        out_rect->height > (in_rect->height * GFBG_MAX_ZOOMIN)) {
        gfbg_error("GFBG layer%d in_size(%d, %d) and out_size(%d, %d) do out of ZoomRatio[1, %d]!!\n", layer_id,
                   in_rect->width, in_rect->height, out_rect->width, out_rect->height, GFBG_MAX_ZOOMIN);
        return TD_FALSE;
    }

    need_zoom = (out_rect->width != in_rect->width || out_rect->height != in_rect->height);
    if (need_zoom == TD_FALSE) {
        return TD_FALSE;
    }

    if (need_zoom && (in_rect->width > GFBG_LINE_BUF)) {
        gfbg_error("GFBG layer%d in width: %u is bigger than %d, will not zoom in!!\n", layer_id, in_rect->width,
                   GFBG_LINE_BUF);
        return TD_FALSE;
    }

    return TD_TRUE;
}

static td_void gfbg_dcmp_config(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)(info->par);
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    td_bool dcmp_state_en = TD_FALSE;

    /*
     * In order to support compressed and non-compressed dynamic switching, you cannot use
     * pstCompressInfo->bCompressOpen to determine whether to open compression.
     * To use the compression information that follows the frame, and whether or not
     * to flip to decide whether to switch compression or non-compression
     */
    if (refresh_info->disp_buf_info.compress) {
        /* See GFBG_DRV_EnableDcmp */
        g_drv_ops.gfbg_drv_enable_dcmp(layer_id, TD_TRUE);
        /* See GFBG_DRV_GetDcmpEnableState */
        g_drv_ops.gfbg_drv_get_dcmp_enable_state(layer_id, &dcmp_state_en);
        if (dcmp_state_en) {
            gfbg_set_dcmp_info(info);
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
            /* after recalculate stride, set buf[1] again */
            gfbg_set_dispbufinfo(layer_id);
#endif
        }
    } else {
        /*
         * Set the address and stride to ensure that the compression will g_display correctly when switched to
         * non-compressed
         */
        g_drv_ops.gfbg_drv_set_layer_addr(layer_id, refresh_info->screen_addr);
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, refresh_info->user_buffer.canvas.pitch);
        } else {
            /* dynamic change compress,when compress to uncompress, reset stride */
            if (par->param_modify_mask & GFBG_LAYER_PARAMODIEY_COMPRESS) {
                info->fix.line_length = (info->var.xres_virtual * info->var.bits_per_pixel / 8 + /* 8 one byte */
                    GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
                par->param_modify_mask &= ~GFBG_LAYER_PARAMODIEY_COMPRESS;
            }
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, gfbg_get_line_length(info));
        }

        g_drv_ops.gfbg_drv_enable_dcmp(layer_id, TD_FALSE);
    }
}

/* Callback function for VO vertical timing interrupt */
static td_s32 callback_get_osd_data(gfbg_par *par, td_u32 layer_id)
{
    gfbg_osd_data layer_data = {0};
    gfbg_display_info *display_info = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;

    display_info = &par->display_info;
    refresh_info = &par->refresh_info;

    if (g_drv_ops.gfbg_drv_get_osd_data(par->layer_id, &layer_data) != TD_SUCCESS) {
        gfbg_error("failed to get layer%d's osd data!\n", par->layer_id);
        return TD_FAILURE;
    }
    display_info->max_screen_width = layer_data.screen_width;
    display_info->max_screen_height = layer_data.screen_height;
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_ALPHA) {
        g_drv_ops.gfbg_drv_set_layer_alpha(layer_id, par->alpha);
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_ALPHA;
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_COLORKEY) {
        g_drv_ops.gfbg_drv_set_layer_key_mask(layer_id, &par->ckey);
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_COLORKEY;
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_FMT) {
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, refresh_info->user_buffer.canvas.format);
        } else {
            g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, par->color_format);
        }
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_FMT;
    }
    return TD_SUCCESS;
}

static td_void callback_modify_dynamic_range(gfbg_par *par, td_u32 layer_id)
{
    gfbg_display_info *display_info = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    struct fb_info *info = TD_NULL;

    display_info = &par->display_info;
    refresh_info = &par->refresh_info;
    info = g_layer[layer_id].info;
    /* NEW modify graphic dynamic range */
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_DYNAMICRANGE) {
        /* Modified to HDR10 */
        if (display_info->dynamic_range == OT_FB_DYNAMIC_RANGE_HDR10) {
            /* gfbg_drv_enable_ghdr */
            if (g_drv_ops.gfbg_drv_enable_ghdr) g_drv_ops.gfbg_drv_enable_ghdr(layer_id, TD_TRUE);
        } else if (display_info->dynamic_range == OT_FB_DYNAMIC_RANGE_SDR8 ||
                   display_info->dynamic_range == OT_FB_DYNAMIC_RANGE_SDR10) {
            /* See gfbg_drv_enable_ghdr */
            if (g_drv_ops.gfbg_drv_enable_ghdr) g_drv_ops.gfbg_drv_enable_ghdr(layer_id, TD_FALSE);
        }
        /*
         * After the modification is completed, the mask is cleared, just like the interrupt
         * processing mechanism. When the interrupt is processed, the interrupt is cleared.
         */
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_DYNAMICRANGE;
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_BMUL) {
        if (g_drv_ops.gfbg_drv_set_pre_mul(layer_id, display_info->is_premul) == TD_FAILURE) {
            if (par->display_info.is_premul == TD_TRUE) {
                par->display_info.is_premul = TD_FALSE;
            } else {
                par->display_info.is_premul = TD_TRUE;
            }
        }
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_BMUL;
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_STRIDE) {
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, refresh_info->user_buffer.canvas.pitch);
        } else {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, gfbg_get_line_length(info));
        }
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_STRIDE;
    }
    return;
}

static td_void callback_init_rect(ot_fb_rect *in_rect, ot_fb_rect *out_rect, const gfbg_display_info *display_info)
{
    in_rect->x = display_info->pos.x_pos;
    in_rect->y = display_info->pos.y_pos;
    in_rect->width = (td_s32)display_info->display_width;
    in_rect->height = (td_s32)display_info->display_height;

    out_rect->x = display_info->pos.x_pos;
    out_rect->y = display_info->pos.y_pos;
    out_rect->width = display_info->screen_width;
    out_rect->height = display_info->screen_height;

    if (out_rect->x + out_rect->width > (td_s32)display_info->max_screen_width) {
        out_rect->width = (td_s32)(display_info->max_screen_width - out_rect->x);
    }

    if (out_rect->y + out_rect->height > (td_s32)display_info->max_screen_height) {
        out_rect->height = (td_s32)(display_info->max_screen_height - out_rect->y);
    }

    /* after cut off, the input rectangle keep rate with output rectangle */
    /* Prevent the occurrence of the divide-by-zero error */
    if ((display_info->screen_width != 0) && (display_info->screen_height != 0)) {
        in_rect->width = in_rect->width * out_rect->width / (td_s32)display_info->screen_width;
        in_rect->height = in_rect->height * out_rect->height / (td_s32)display_info->screen_height;
    }
    return;
}

static td_void callback_modify_sizes(gfbg_par *par, td_u32 layer_id)
{
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_rect in_rect = {0};
    ot_fb_rect out_rect = {0};
    td_bool zme_en;

    display_info = &par->display_info;
    /* Handles requests to modify input and output sizes. */
    if ((par->param_modify_mask & GFBG_LAYER_PARAMODIFY_INRECT) ||
        (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_OUTRECT)) {
        /* if VO can't support scaling,  we set the screen's H/W as the same as display */
        if (!g_drv_ops.capability[layer_id].is_vo_scale) {
            display_info->screen_width = display_info->display_width;
            display_info->screen_height = display_info->display_height;
        }

        /* for init rect */
        callback_init_rect(&in_rect, &out_rect, display_info);
        if (gfbg_check_imagezoomenable(layer_id, &in_rect, &out_rect) == TD_TRUE) {
            /*
             * If you want to go through the zoom module, you need to correct it to 2 alignment,
             * otherwise it will appear abnormal.
             */
            in_rect.width = ALIGN_DOWN((td_u32)in_rect.width, 2); /* 2 align */
            in_rect.height = ALIGN_DOWN((td_u32)in_rect.height, 2); /* 2 align */

            if (g_drv_ops.gfbg_drv_set_layer_rect) {
                g_drv_ops.gfbg_drv_set_layer_rect(layer_id, &in_rect, &out_rect);
            }

            if (g_drv_ops.gfbg_drv_set_layer_src_image_reso) {
                g_drv_ops.gfbg_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }

            /* NEW!!!   Set zoom */
            /* See out width and height not equal to in width and height,then enable zme. */
            zme_en = (out_rect.width != in_rect.width || out_rect.height != in_rect.height);
            if (g_drv_ops.gfbg_drv_enable_zme) {
                g_drv_ops.gfbg_drv_enable_zme(layer_id, &in_rect, &out_rect, zme_en);
            }
        } else {
            /*
             * If scaling is not enabled,
             * the input size is used as the output size and the zoom module is closed.
             */
            if (g_drv_ops.gfbg_drv_set_layer_rect) {
                g_drv_ops.gfbg_drv_set_layer_rect(layer_id, &in_rect, &in_rect);
            }

            if (g_drv_ops.gfbg_drv_set_layer_src_image_reso) {
                g_drv_ops.gfbg_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }

            if (g_drv_ops.gfbg_drv_enable_zme) {
                g_drv_ops.gfbg_drv_enable_zme(layer_id, &in_rect, &out_rect, TD_FALSE);
            }
        }

        /* Processing completed, clear mask */
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_INRECT;
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_OUTRECT;
    }
    return;
}

static td_void callback_set_fmt_and_stride(const gfbg_par *par, td_u32 layer_id)
{
    struct fb_info *info = TD_NULL;
    const gfbg_refresh_info *refresh_info = TD_NULL;

    info = g_layer[layer_id].info;
    refresh_info = &par->refresh_info;
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_FMT) {
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, refresh_info->user_buffer.canvas.format);
        } else {
            g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, par->color_format);
        }
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_STRIDE) {
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, refresh_info->user_buffer.canvas.pitch);
        } else {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, gfbg_get_line_length(info));
        }
    }
    return;
}

static td_void callback_update_refresh_info(gfbg_par *par, td_u32 layer_id)
{
    gfbg_refresh_info *refresh_info = TD_NULL;
    struct fb_info *info = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    td_u32 index;
    td_u32 buf_size;

    refresh_info = &par->refresh_info;
    index = refresh_info->disp_buf_info.index_for_int;
    info = g_layer[layer_id].info;
    buf_size = ((gfbg_get_line_length(info) * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    compress_info = &par->compress_info;
    if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE) && (refresh_info->disp_buf_info.need_flip == TD_TRUE)) {
        /* Work buf to change to free buf. Take free buf to display */
        index = 1 - index;
        refresh_info->disp_buf_info.index_for_int = index;
        /*
         * The display address is set to the address of the free buf,
         * which is set to the screen buf address differently from 0buf
         */
        g_drv_ops.gfbg_drv_set_layer_addr(layer_id, refresh_info->disp_buf_info.phys_addr[index]);
        refresh_info->screen_addr = refresh_info->disp_buf_info.phys_addr[index];
        refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 alg data */
        compress_info->layer_addr_update = TD_TRUE;
#ifdef __LITEOS__
        if (info->oinfo.stride != 0) {
            info->oinfo.sarea.y = (par->refresh_info.disp_buf_info.phys_addr[index] -
                                   (td_u32)(uintptr_t)info->oinfo.fbmem) / info->oinfo.stride;
            if ((info->oinfo.bpp >> 3) != 0) { /* 3 is 8 bits */
                info->oinfo.sarea.x = ((par->refresh_info.disp_buf_info.phys_addr[index] -
                    (td_u32)(uintptr_t)info->oinfo.fbmem) % info->oinfo.stride) /
					(info->oinfo.bpp >> 3); /* 3 is 8 bits */
            }
        }
#else
        info->var.yoffset = osal_div_u64((refresh_info->disp_buf_info.phys_addr[index] - gfbg_get_smem_start(info)),
                                         gfbg_get_line_length_ex(info));
        if ((gfbg_get_line_length(info) != 0) && ((gfbg_get_bits_per_pixel(info)>> 3) != 0)) { /* 3 is 8 bits */
            info->var.xoffset = ((td_ulong)(refresh_info->disp_buf_info.phys_addr[index] - gfbg_get_smem_start(info)) %
                                 gfbg_get_line_length_ex(info)) /
				(gfbg_get_bits_per_pixel(info)>> 3); /* 3 is 8 bits */
        }
#endif
        refresh_info->disp_buf_info.fliped = TD_TRUE;
        refresh_info->disp_buf_info.need_flip = TD_FALSE;
        refresh_info->disp_buf_info.int_pic_num++;
    }
    return;
}

static td_void callback_modify_address(gfbg_par *par, td_u32 layer_id)
{
    gfbg_refresh_info *refresh_info = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    struct fb_info *info = TD_NULL;
    ot_fb_rect in_rect = {0};
    ot_fb_rect out_rect = {0};

    refresh_info = &par->refresh_info;
    display_info = &par->display_info;
    compress_info = &par->compress_info;
    info = g_layer[layer_id].info;
    /* The display address is refreshed and the display address is modified. */
    if (!(par->param_modify_mask & GFBG_LAYER_PARAMODIFY_DISPLAYADDR)) {
        /* according to the index, decide which buf set to the screen */
        callback_update_refresh_info(par, layer_id);
        return;
    }
    /* SetLayerDataFmt and  SetLayerStride */
    callback_set_fmt_and_stride(par, layer_id);

    if ((par->param_modify_mask & GFBG_LAYER_PARAMODIFY_INRECT) ||
        (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_OUTRECT)) {
        /* if VO can't support scaling,  we set the screen's H/W as the same as display */
        if (!g_drv_ops.capability[layer_id].is_vo_scale) {
            display_info->screen_width = display_info->display_width;
            display_info->screen_height = display_info->display_height;
        }

        /* for init rect */
        callback_init_rect(&in_rect, &out_rect, display_info);

        if (gfbg_check_imagezoomenable(layer_id, &in_rect, &out_rect) == TD_TRUE) {
            g_drv_ops.gfbg_drv_set_layer_rect(layer_id, &in_rect, &out_rect);
            if (g_drv_ops.gfbg_drv_set_layer_src_image_reso) {
                g_drv_ops.gfbg_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }
        }
    }

    g_drv_ops.gfbg_drv_set_layer_addr(layer_id, refresh_info->screen_addr);

    par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_DISPLAYADDR;
    compress_info->layer_addr_update = TD_TRUE;

    if ((refresh_info->disp_buf_info.phys_addr[0] != refresh_info->disp_buf_info.phys_addr[1]) &&
        (refresh_info->disp_buf_info.phys_addr[0])) {
        if (refresh_info->screen_addr >=  refresh_info->disp_buf_info.phys_addr[0] &&
            refresh_info->screen_addr < refresh_info->disp_buf_info.phys_addr[1]) {
            refresh_info->disp_buf_info.index_for_int = 0;
        } else if ((refresh_info->screen_addr >= refresh_info->disp_buf_info.phys_addr[1]) &&
                   (refresh_info->screen_addr < (refresh_info->disp_buf_info.phys_addr[0] + gfbg_get_smem_len(info)))) {
            refresh_info->disp_buf_info.index_for_int = 1;
        }
    }
    /* according to the index, decide which buf set to the screen */
    callback_update_refresh_info(par, layer_id);
    return;
}

static td_void callback_modify_smart_rect(const gfbg_par *par, td_u32 layer_id)
{
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIEY_SMART_RECT) {
        if (!g_drv_ops.capability[layer_id].is_osb) {
            return;
        }
        g_drv_ops.gfbg_drv_smart_rect_up_param(par->layer_id);
    }
    return;
}

static td_void callback_modify_clut_up(gfbg_par *par, td_u32 layer_id)
{
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIEY_CLUT_UP) {
        if (!g_drv_ops.capability[layer_id].is_cmap) {
            return;
        }
        g_drv_ops.gfbg_drv_set_color_reg_up(par->layer_id);
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIEY_CLUT_UP;
    }


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART18_H_ */
