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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART13_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART13_H_
}
static td_s32 refresh_2buf_imediate_blit(gfbg_par *par, ot_fb_buf *back_buf, ot_fb_rect *new_union_rect, td_u32 index)
{
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    ot_fb_buf fore_buf = {0};
    gfbg_blit_opt tmp_opt = {0};
    unsigned long lock_flag;
    td_s32 ret;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if (display_info->rotate_mode != OT_FB_ROTATE_90 && display_info->rotate_mode != OT_FB_ROTATE_270) {
        /*
         * because reverse, the 2 buffer needed to sync contain,
         * if the fresh area has cover last fresh area, then no need to sync
         */
        if (!gfbg_iscontain(*new_union_rect, refresh_info->disp_buf_info.union_rect) &&
            refresh_info->disp_buf_info.union_rect.width && refresh_info->disp_buf_info.union_rect.height) {
            ret = memcpy_s(&fore_buf, sizeof(ot_fb_buf), back_buf, sizeof(ot_fb_buf));
            gfbg_unlock_unequal_eok_return(ret, &par->lock, lock_flag);
            fore_buf.canvas.phys_addr = refresh_info->disp_buf_info.phys_addr[index];
            ret = memcpy_s(&fore_buf.update_rect, sizeof(ot_fb_rect), &refresh_info->disp_buf_info.union_rect,
                sizeof(ot_fb_rect));
            gfbg_unlock_unequal_eok_return(ret, &par->lock, lock_flag);
            ret = memcpy_s(&back_buf->update_rect, sizeof(ot_fb_rect), &fore_buf.update_rect, sizeof(ot_fb_rect));
            gfbg_unlock_unequal_eok_return(ret, &par->lock, lock_flag);
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            if (gfbg_drv_blit(&fore_buf, back_buf, &tmp_opt, TD_TRUE) < 0) {
                return TD_FAILURE;
            }
            gfbg_spin_lock_irqsave(&par->lock, lock_flag);
        }
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}
static td_s32 refresh_2buf_imediate_prepare_opt(gfbg_par *par, const ot_fb_buf *canvas_buf, ot_fb_buf *back_buf,
                                                gfbg_blit_opt *blit_opt)
{
    gfbg_display_info *display_info = &par->display_info;
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    td_s32 ret;

    blit_opt->call_back = TD_TRUE;
    blit_opt->param = &(par->layer_id);
    blit_opt->block = TD_TRUE;

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
    blit_opt->region_deflicker = TD_TRUE;

    if (display_info->rotate_mode == OT_FB_ROTATE_180) {
        blit_opt->mirror_mode = OT_FB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = display_info->mirror_mode;
    }
    if (par->compress_info.compress_open) {
        if (canvas_buf->update_rect.x != 0 || canvas_buf->update_rect.y != 0 ||
            canvas_buf->update_rect.width != back_buf->canvas.width ||
            canvas_buf->update_rect.height != back_buf->canvas.height) {
            gfbg_error("compression mode supports only full-screen refresh!\n");
            return TD_FAILURE;
        }
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE to
         * indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        ret = memcpy_s((void *)&par->compress_info.update_rect, sizeof(ot_fb_rect), &back_buf->update_rect,
            sizeof(ot_fb_rect));
        gfbg_unequal_eok_return(ret);
        compress_info->update_finished = TD_FALSE;
        blit_opt->compress = TD_TRUE;
    } else if (par->compress_info.is_economize_memory == TD_TRUE) {
        gfbg_error("memory not enough ,should be start compress!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
static td_void refresh_2buf_imediate_backup_buffer(const struct fb_info *info, const ot_fb_buf *back_buf,
                                                   const ot_fb_rect *new_union_rect, const gfbg_blit_opt *blit_opt,
                                                   td_u32 *index)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    td_u32 buf_size;
    unsigned long lock_flag;
    td_s32 ret;
    ot_unused(back_buf);
    ot_unused(blit_opt);

    buf_size = ((gfbg_get_line_length(info) * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    *index = 1 - *index;
    par->modifying = TD_TRUE;
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DISPLAYADDR;
    refresh_info->screen_addr = refresh_info->disp_buf_info.phys_addr[*index];
    refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 for half */
    refresh_info->disp_buf_info.compress = compress_info->compress_open;
    par->modifying = TD_FALSE;

    if (par->compress_info.compress_open) {
        par->compress_info.update_finished = TD_TRUE;
        ret = memcpy_s((ot_fb_rect*)(&par->compress_info.update_rect), sizeof(ot_fb_rect), new_union_rect,
            sizeof(ot_fb_rect));
        gfbg_unlock_unequal_eok_return_void(ret, &par->lock, lock_flag);
        /*
         * When blocking, if there is a callback,
         * still get FrameSize0 and FrameSize1 here as early as possible.
         */
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return;
}
/*
 * In this function we should wait the new contain has been show on the screen before return,
 * and the operations such as address configuration no needed do in interrupt handle
 */
static td_s32 gfbg_refresh_record_rect_and_opt(gfbg_par *par, const ot_fb_buf *canvas_buf,
    ot_fb_buf *back_buf, const ot_fb_rect *new_union_rect, gfbg_blit_opt *blit_opt)
{
    unsigned long lock_flag;
    td_s32 ret;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    ret = memcpy_s(&par->refresh_info.disp_buf_info.union_rect, sizeof(ot_fb_rect),
        new_union_rect, sizeof(ot_fb_rect));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }
    if (refresh_2buf_imediate_prepare_opt(par, canvas_buf, back_buf, blit_opt) != TD_SUCCESS) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}
static td_s32 gfbg_refresh_record_user_buffer(gfbg_par *par, const ot_fb_buf *canvas_buf)
{
    unsigned long lock_flag;
    td_s32 ret;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    ret = memcpy_s(&(par->refresh_info.user_buffer), sizeof(ot_fb_buf), canvas_buf, sizeof(ot_fb_buf));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }
    par->vblflag = 0;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}
static td_s32 gfbg_refresh_2buf_immediate_display(td_u32 layer_id, const ot_fb_buf *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_blit_opt blit_opt = {0};
    td_u32 index = refresh_info->disp_buf_info.index_for_int;
    ot_fb_buf back_buf = {0};
    td_s32 ret;
    ot_fb_rect new_union_rect = {0};
    unsigned long lock_flag;
    td_u32 bytes_per_pixel = 2;

    /*
     * Refresh task submitted between VO vertical timing interrupt and frame start interrupt
     * Will cause TDE/VGS to write the buffer being displayed, and a split screen will appear.
     * Blocked here, it is forbidden to submit the refresh task during this time.
     */
    if (wait_event_timeout(par->do_refresh_job, par->refresh_info.do_refresh_job,
        (td_s32)msecs_to_jiffies(40)) == 0) { /* 40 for timeout */
        osal_printk("Func:%s, Line:%d, wait event timeout.\n", __FUNCTION__, __LINE__);
    }
    /*
     * TDE use the blocking mode
     * Immediate refresh mode requires blocking mode, non-callback mode,
     * avoiding callback mode when scrolling subtitles.
     * Return immediately. Maybe the next time the user quickly call the interface refresh,
     * it may flush the last content.
     */
    refresh_info->refresh_num++;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    /* dynamic change compress,when compress to uncompress, reset state */
    gfbg_dynamic_set_compress(info);
    /* prepare for back buf and get bytes per pixel */
    if (refresh_2buf_immediate_prepare_back_buf(info, &back_buf, &bytes_per_pixel, index) != TD_SUCCESS) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }
    /* according to the hw arithmetic, calculate  source and Dst fresh rectangle */
    refresh_2buf_get_new_rect(canvas_buf, &back_buf, &new_union_rect, &blit_opt);
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    /* blit */
    if (refresh_2buf_imediate_blit(par, &back_buf, &new_union_rect, index) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if (gfbg_refresh_record_rect_and_opt(par, canvas_buf, &back_buf, &new_union_rect, &blit_opt) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /* blit with refresh rect */
    if (gfbg_drv_blit(canvas_buf, &back_buf, &blit_opt, TD_TRUE) < 0) {
        return TD_FAILURE;
    }
    ret = EOK;
    refresh_info->disp_buf_info.refresh_handle = ret;
    /* set the backup buffer to register and show it  */
    refresh_2buf_imediate_backup_buffer(info, &back_buf, &new_union_rect, &blit_opt, &index);
    if (gfbg_refresh_record_user_buffer(par, canvas_buf) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /* wait the address register's configuration take effect before return */
    if (in_atomic() == TD_FALSE) {
        gfbg_wait_regconfig_work(layer_id);
    }
    return TD_SUCCESS;
}
static td_void gfbg_refresh_again(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    ot_fb_buf canvas;

    /* Prerequisites for the canvas to be refreshed */
    if (!(par->param_modify_mask & GFBG_LAYER_PARAMODIFY_INRECT)) {
        return;
    }
    if (refresh_info->user_buffer.canvas.phys_addr == 0) {
        return;
    }
    if (refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) {
        return;
    }
    /* Fills the canvas object with refresh information from private data for refresh. */
    canvas = refresh_info->user_buffer;
    canvas.update_rect.x = 0;
    canvas.update_rect.y = 0;
    canvas.update_rect.width = canvas.canvas.width;
    canvas.update_rect.height = canvas.canvas.height;
    gfbg_refresh(layer_id, &canvas, refresh_info->buf_mode);
}
static td_s32 gfbg_disp_check_param(const struct fb_info *info, td_u32 width, td_u32 height)
{
    gfbg_par *par = (gfbg_par *)info->par;
    ot_fb_size max_screen_size = {0};
    td_u32 pitch;

    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        gfbg_get_maxscreensize(par, &max_screen_size.width, &max_screen_size.height);
        if ((par->layer_id == 3 || par->layer_id == 4) && /* 3 G3 clut2 and clut4 support 4K */
            ((gfbg_get_bits_per_pixel(info) == 4) || (gfbg_get_bits_per_pixel(info) == 2))) { /* 4,2 bits */
            if ((width > max_screen_size.width) || (height > max_screen_size.height) ||
                (width > GFBG_MAX_LAYER_WIDTH_CLUT_G3) || (height > GFBG_MAX_LAYER_HEIGTH_CLUT_G3)) {
                gfbg_error("(%u, %u)larger than the max size of the screen(%u, %u) or the layer(%u, %u)!\n", width,
                           height, max_screen_size.width, max_screen_size.height, GFBG_MAX_LAYER_WIDTH_CLUT_G3,
                           GFBG_MAX_LAYER_HEIGTH_CLUT_G3);
                return TD_FAILURE;
            }
        } else {
            if ((width > max_screen_size.width) || (height > max_screen_size.height) ||
                (width > g_drv_ops.capability[par->layer_id].max_width) ||
                (height > g_drv_ops.capability[par->layer_id].max_height)) {
                gfbg_error("(%u, %u)larger than the max size of the screen(%u, %u) or the layer(%u, %u)!\n", width,
                           height, max_screen_size.width, max_screen_size.height,
                           g_drv_ops.capability[par->layer_id].max_width,
                           g_drv_ops.capability[par->layer_id].max_height);
                return TD_FAILURE;
            }
        }
    }
    /* 3 is 8 bits */
    pitch = (((width * gfbg_get_bits_per_pixel(info)) >> 3) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    if (gfbg_check_mem_enough(info, pitch, height) == TD_FAILURE) {
        gfbg_error("check_mem .memory is not enough!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
/*
 * This function has a lock inside, and there is a call to the sleep function.
 * Before calling this function, you must first unlock the spin lock.
 */
#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART13_H_ */
