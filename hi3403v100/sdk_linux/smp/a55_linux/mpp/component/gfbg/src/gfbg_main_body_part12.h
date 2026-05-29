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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART12_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART12_H_

    ot_fb_point point;
    point.x_pos = child_rect.x;
    point.y_pos = child_rect.y;
    if ((point.x_pos < parent_rect.x) || (point.x_pos > (parent_rect.x + parent_rect.width)) ||
        (point.y_pos < parent_rect.y) || (point.y_pos > (parent_rect.y + parent_rect.height))) {
        return TD_FALSE;
    }
    point.x_pos = child_rect.x + child_rect.width;
    point.y_pos = child_rect.y + child_rect.height;
    if ((point.x_pos < parent_rect.x) || (point.x_pos > (parent_rect.x + parent_rect.width)) ||
        (point.y_pos < parent_rect.y) || (point.y_pos > (parent_rect.y + parent_rect.height))) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

static td_s32 refresh_2buf_prepare_back_buf(const struct fb_info *info, ot_fb_buf *back_buf, td_u32 *bytes_per_pixel)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_display_info *display_info = &par->display_info;

    refresh_info->disp_buf_info.refresh_handle = 0;

    back_buf->canvas.format = par->color_format;
    back_buf->canvas.height = display_info->display_height;
    back_buf->canvas.width = display_info->display_width;
    back_buf->canvas.pitch = gfbg_get_line_length(info);

    if (display_info->rotate_mode  == OT_FB_ROTATE_90 || display_info->rotate_mode == OT_FB_ROTATE_270) {
        back_buf->canvas.width = display_info->display_height;
        back_buf->canvas.height = display_info->display_width;
        if (back_buf->canvas.format == OT_FB_FORMAT_ARGB1555 || back_buf->canvas.format == OT_FB_FORMAT_ARGB4444) {
            *bytes_per_pixel = 2; /* 2 bit per pixel */
        } else if (back_buf->canvas.format == OT_FB_FORMAT_ARGB8888) {
            *bytes_per_pixel = 4; /* 4 bit per pixel */
        }
        back_buf->canvas.pitch = ((*bytes_per_pixel * back_buf->canvas.width + GFBG_ALIGN - 1) / GFBG_ALIGN) *
                                  GFBG_ALIGN;

        if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
            (par->color_format != OT_FB_FORMAT_ARGB8888)) {
            gfbg_error("The rotate mode only support FB_FORMAT_ARGB4444,FB_FORMAT_ARGB1555," \
                       "FB_FORMAT_ARGB8888 which is %d \n", par->color_format);
            return TD_FAILURE;
        }
        back_buf->canvas.phys_addr = par->rotate_vb;
    } else {
        /* Set the background buf as the target, get the free buf to the background buf */
        gfbg_get_idledispbuf(par, (td_phys_addr_t*)(&back_buf->canvas.phys_addr));
    }
    return TD_SUCCESS;
}

static td_void refresh_2buf_get_new_rect(const ot_fb_buf *canvas_buf, const ot_fb_buf *back_buf,
                                         ot_fb_rect *new_union_rect, gfbg_blit_opt *blit_opt)
{
    drv_tde_rect src_rect = {0};
    drv_tde_rect dst_rect = {0};
    drv_tde_rect inner_src_rect = {0};
    drv_tde_rect inner_dst_rect = {0};

    if (canvas_buf->canvas.height != back_buf->canvas.height || canvas_buf->canvas.width != back_buf->canvas.width) {
        src_rect.width = canvas_buf->canvas.width;
        src_rect.height = canvas_buf->canvas.height;
        dst_rect.width = back_buf->canvas.width;
        dst_rect.height = back_buf->canvas.height;
        inner_src_rect.pos_x = canvas_buf->update_rect.x;
        inner_src_rect.pos_y = canvas_buf->update_rect.y;
        inner_src_rect.width = (td_u32)canvas_buf->update_rect.width;
        inner_src_rect.height = (td_u32)canvas_buf->update_rect.height;
        tde_cal_scale_rect_gfbg(&src_rect, &dst_rect, &inner_src_rect, &inner_dst_rect);

        new_union_rect->x = inner_dst_rect.pos_x;
        new_union_rect->y = inner_dst_rect.pos_y;
        new_union_rect->width = (td_s32)inner_dst_rect.width;
        new_union_rect->height = (td_s32)inner_dst_rect.height;
        blit_opt->scale = TD_TRUE;
    } else {
        *new_union_rect = canvas_buf->update_rect;
    }
    return;
}

static td_s32 refresh_2buf_blit(gfbg_par *par, td_phys_addr_t osd_buf_addr, ot_fb_buf *back_buf,
    ot_fb_rect *new_union_rect)
{
    td_s32 ret;
    gfbg_blit_opt tmp = {0};
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_display_info *display_info = &par->display_info;
    ot_fb_buf fore_buf = {0};
    td_phys_addr_t work_buf_addr = 0;

    gfbg_get_workdispbuf(par, &work_buf_addr);

    if ((refresh_info->disp_buf_info.fliped == TD_FALSE) ||
        ((osd_buf_addr != work_buf_addr) && (par->compress_info.compress_open  == TD_FALSE))) {
        return TD_SUCCESS;
    }
    /* Background as a target pointing to an idle buf */
    ret = memcpy_s(&fore_buf, sizeof(ot_fb_buf), back_buf, sizeof(ot_fb_buf));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    /* Foreground as a source points to the buf at work */
    gfbg_get_workdispbuf(par, (td_phys_addr_t*)(&fore_buf.canvas.phys_addr));
    /* The union rect is also used as an update area for the foreground and background. */
    ret = memcpy_s(&fore_buf.update_rect, sizeof(ot_fb_rect), &refresh_info->disp_buf_info.union_rect,
        sizeof(ot_fb_rect));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    ret = memcpy_s(&back_buf->update_rect, sizeof(ot_fb_rect), &fore_buf.update_rect, sizeof(ot_fb_rect));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    (td_void)memset_s(&tmp, sizeof(gfbg_blit_opt), 0x0, sizeof(gfbg_blit_opt));
    /* blit with union rect */
    if ((display_info->rotate_mode != OT_FB_ROTATE_90) && (display_info->rotate_mode != OT_FB_ROTATE_270)) {
        if (gfbg_iscontain(*new_union_rect, refresh_info->disp_buf_info.union_rect) == TD_FALSE) {
            if (gfbg_drv_blit(&fore_buf, back_buf, &tmp, TD_TRUE) < 0) {
                gfbg_error("gfbg_drv_blit err!\n");
                return TD_FAILURE;
            }
        }
    }
    /* clear union rect */
    (td_void)memset_s(&(refresh_info->disp_buf_info.union_rect), sizeof(ot_fb_rect), 0, sizeof(ot_fb_rect));

    refresh_info->disp_buf_info.fliped = TD_FALSE;
    return TD_SUCCESS;
}

static td_void refresh_2buf_prepare_opt(gfbg_par *par, const ot_fb_buf *canvas_buf, ot_fb_buf *back_buf,
                                        td_u32 bytes_per_pixel, gfbg_blit_opt *blit_opt)
{
    gfbg_display_info *display_info = &par->display_info;
    blit_opt->call_back = TD_TRUE;
    blit_opt->block = TD_FALSE;
    blit_opt->param = &(par->layer_id);

    if (display_info->antiflicker_mode == GFBG_ANTIFLICKER_TDE) {
        blit_opt->antiflicker_level = OT_FB_LAYER_ANTIFLICKER_NONE;
    }
    if (blit_opt->scale == TD_TRUE) {
        /* actual area, calculate by TDE, here is just use for let pass the test */
        back_buf->update_rect.x = 0;
        back_buf->update_rect.y = 0;
        back_buf->update_rect.width = back_buf->canvas.width;
        back_buf->update_rect.height = back_buf->canvas.height;
    } else {
        back_buf->update_rect = canvas_buf->update_rect;
    }

    if (par->display_info.rotate_mode == OT_FB_ROTATE_90 || par->display_info.rotate_mode == OT_FB_ROTATE_270) {
        back_buf->canvas.height = par->display_info.display_width;
        back_buf->canvas.width = par->display_info.display_height;
        back_buf->canvas.pitch = ((bytes_per_pixel * back_buf->canvas.width +
                                  (GFBG_ALIGN - 1)) / GFBG_ALIGN) * GFBG_ALIGN;
        back_buf->update_rect = canvas_buf->update_rect;
    }

    blit_opt->region_deflicker = TD_TRUE;

    if (display_info->rotate_mode == OT_FB_ROTATE_180) {
        blit_opt->mirror_mode = OT_FB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = display_info->mirror_mode;
    }
    return;
}

static td_s32 refresh_2buf_prepare_compress(gfbg_par *par, const ot_fb_buf *back_buf, gfbg_blit_opt *blit_opt,
    const ot_fb_buf *canvas_buf)
{
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    unsigned long lock_flag;
    td_s32 ret;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if (par->compress_info.compress_open) {
        if (canvas_buf->update_rect.x != 0 || canvas_buf->update_rect.y != 0 ||
            canvas_buf->update_rect.width != back_buf->canvas.width ||
            canvas_buf->update_rect.height != back_buf->canvas.height) {
            gfbg_error("compression mode supports only full-screen refresh!\n");
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            return TD_FAILURE;
        }
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE
         * to indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        ret = memcpy_s((void *)&par->compress_info.update_rect, sizeof(ot_fb_rect), &back_buf->update_rect,
            sizeof(ot_fb_rect));
        gfbg_unlock_unequal_eok_return(ret, &par->lock, lock_flag);
        compress_info->update_finished = TD_FALSE;
        blit_opt->compress = TD_TRUE;
    } else if (par->compress_info.is_economize_memory == TD_TRUE) {
        gfbg_error("memory not enough ,should be start compress!\n");
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    } else {
        blit_opt->compress = TD_FALSE;
    }

    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_void refresh_2buf_update_rect(gfbg_refresh_info *refresh_info, const ot_fb_rect *new_union_rect)
{
    td_s32 ret;
    if ((refresh_info->disp_buf_info.union_rect.width == 0) || (refresh_info->disp_buf_info.union_rect.height == 0)) {
        ret = memcpy_s(&refresh_info->disp_buf_info.union_rect, sizeof(ot_fb_rect), new_union_rect, sizeof(ot_fb_rect));
        gfbg_unequal_eok_return_void(ret);
    } else {
        gfbg_unite_rect(&refresh_info->disp_buf_info.union_rect, new_union_rect);
    }
    return;
}

static td_void gfbg_dynamic_set_compress(const struct fb_info *info)
{
    td_u32 uncompress_stride;
    gfbg_par *par = (gfbg_par *)info->par;

    uncompress_stride = (info->var.xres_virtual * info->var.bits_per_pixel / 8 + /* 8 one byte */
        GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    if (uncompress_stride == info->fix.line_length) {
        /* last and next frame are uncompress */
        return;
    }

    if (gfbg_check_memory_enough(info, uncompress_stride) == TD_TRUE) {
        /* memory enough to compress to uncompress */
        return;
    } else {
        if (par->compress_info.compress_open == TD_FALSE) {
            par->compress_info.compress_open = TD_TRUE;
            gfbg_error("memory not enough ,should not be refresh by uncompress!\n");
        }
    }
    return;
}

#ifdef CONFIG_OT_VO_FB_SEPARATE
static td_s32 gfbg_wait_refresh_work(td_u32 layer_id)
{
    td_s32 ret;
    gfbg_par *par = (gfbg_par *)g_layer[layer_id].info->par;

    par->vblflag = 0;
    ret = wait_event_timeout(par->refresh_job_done, par->refresh_info.refresh_job_done,
        (td_s32)msecs_to_jiffies(40)); /* 40 for timeout */
    if (ret < 0) {
        gfbg_error("wait timeout!\n");
        return TD_FAILURE;
    }

    // reset bRefreshJobDone
    par->refresh_info.refresh_job_done = TD_FALSE;
    return TD_SUCCESS;
}
#endif

static inline td_s32 gfbg_refresh_get_osd_buffer_addr(td_u32 layer_id, td_u64 *osd_buffer_phy_addr)
{
    gfbg_osd_data* layer_data = TD_NULL;

    /* Note that vmalloc and vfree need to be placed outside of the spin_lock */
    layer_data = (gfbg_osd_data*)kmalloc(sizeof(gfbg_osd_data), GFP_KERNEL);
    if (layer_data == TD_NULL) {
        return TD_FAILURE;
    }

    g_drv_ops.gfbg_drv_get_osd_data(layer_id, layer_data);
    *osd_buffer_phy_addr = layer_data->buffer_phy_addr;

    kfree(layer_data);
    layer_data = TD_NULL;
    return TD_SUCCESS;
}

/* This function has a lock operation, so you can't call it if the caller has a lock operation. */
static td_s32 gfbg_refresh_2buf(td_u32 layer_id, const ot_fb_buf *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_blit_opt blit_opt = {0};
    ot_fb_buf back_buf = {0};
    ot_fb_rect new_union_rect = {0};
    unsigned long lock_flag;
    td_phys_addr_t osd_buf_addr;
    td_u32 bytes_per_pixel = 2;
    td_s32 handle;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    refresh_info->disp_buf_info.need_flip = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    /*
     * Refresh task submitted between VO vertical timing interrupt and frame start interrupt
     * Will cause TDE/VGS to write the buffer being displayed, and a split screen will appear.
     * Blocked here, it is forbidden to submit the refresh task during this time.
     */
     /* 40 for timeout */
    if (wait_event_timeout(par->do_refresh_job, par->refresh_info.do_refresh_job, (td_s32)msecs_to_jiffies(40)) == 0) {
        gfbg_error("wait timeout!\n");
    }

    refresh_info->refresh_num++;

    /* get osd buffer addr */
    g_drv_ops.gfbg_drv_get_layer_addr(layer_id, &osd_buf_addr);
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    /* dynamic change compress,when compress to uncompress, reset state */
    gfbg_dynamic_set_compress(info);
    /* prepare back buf and get bytes_per_pixel */
    if (refresh_2buf_prepare_back_buf(info, &back_buf, &bytes_per_pixel) != TD_SUCCESS) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    /* according to the hw arithmetic, calculate source and dst fresh rectangle */
    refresh_2buf_get_new_rect(canvas_buf, &back_buf, &new_union_rect, &blit_opt);
    /*
     * We should check is address changed,
     * for make sure that the address configured to the hw register is in effec
     */
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    /* refresh_2buf_blit */
    if (refresh_2buf_blit(par, osd_buf_addr, &back_buf, &new_union_rect) != TD_SUCCESS) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    /* update union rect */
    refresh_2buf_update_rect(refresh_info, &new_union_rect);

    /* prepare opt */
    refresh_2buf_prepare_opt(par, canvas_buf, &back_buf, bytes_per_pixel, &blit_opt);

    /* prepare compress */
    if (refresh_2buf_prepare_compress(par, &back_buf, &blit_opt, canvas_buf) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* blit with refresh rect */
    handle = gfbg_drv_blit(canvas_buf, &back_buf, &blit_opt, TD_TRUE);
    if (handle < 0) {
        return TD_FAILURE;
    }

    refresh_info->disp_buf_info.refresh_handle = handle;
    refresh_info->user_buffer = *canvas_buf;

#ifdef CONFIG_OT_VO_FB_SEPARATE
    /* wait the address register's configuration take effect before return */
    gfbg_wait_refresh_work(layer_id);
#endif

    return TD_SUCCESS;
}

static td_s32 gfbg_wait_regconfig_work(td_u32 layer_id)
{
    td_s32 ret;
    gfbg_par *par = TD_NULL;

    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)g_layer[layer_id].info->par;
    if (par == TD_NULL) {
        return TD_FAILURE;
    }
    par->vblflag = 0;
    /* Assuming TDE is fast enough, 40ms */
    ret = wait_event_timeout(par->vbl_event, par->vblflag, (td_s32)msecs_to_jiffies(40));
    if (ret == 0) {
        gfbg_error("Wait vblank failed!");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 refresh_2buf_immediate_prepare_back_buf(const struct fb_info *info, ot_fb_buf *back_buf,
                                                      td_u32 *bytes_per_pixel, td_u32 index)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;

    /* forbid changing display buffer in interrupt handle */
    refresh_info->disp_buf_info.fliped = TD_FALSE;
    refresh_info->disp_buf_info.need_flip = TD_FALSE;
    refresh_info->disp_buf_info.refresh_handle = 0;

    back_buf->canvas.format = par->color_format;
    back_buf->canvas.height = display_info->display_height;
    back_buf->canvas.width = display_info->display_width;
    back_buf->canvas.pitch = gfbg_get_line_length(info);
    if (display_info->rotate_mode == OT_FB_ROTATE_90 || display_info->rotate_mode == OT_FB_ROTATE_270) {
        back_buf->canvas.width = display_info->display_height;
        back_buf->canvas.height = display_info->display_width;
        if (back_buf->canvas.format == OT_FB_FORMAT_ARGB1555 || back_buf->canvas.format == OT_FB_FORMAT_ARGB4444) {
            *bytes_per_pixel = 2; /* 2 depth per pixel */
        } else if (back_buf->canvas.format == OT_FB_FORMAT_ARGB8888) {
            *bytes_per_pixel = 4; /* 4 depth per pixel */
        }
        back_buf->canvas.pitch = (((*bytes_per_pixel)*back_buf->canvas.width + GFBG_ALIGN - 1) / GFBG_ALIGN) *
                                  GFBG_ALIGN;
        if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
            (par->color_format != OT_FB_FORMAT_ARGB8888)) {
            gfbg_error("The rotate mode only support FB_FORMAT_ARGB4444,FB_FORMAT_ARGB1555," \
                       "FB_FORMAT_ARGB8888 which is %d \n", par->color_format);
            return TD_FAILURE;
        }
        back_buf->canvas.phys_addr = par->rotate_vb;
    } else {
        back_buf->canvas.phys_addr = refresh_info->disp_buf_info.phys_addr[1 - index];
    }
    return TD_SUCCESS;


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART12_H_ */
