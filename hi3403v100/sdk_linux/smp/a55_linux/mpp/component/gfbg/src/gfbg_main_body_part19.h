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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART19_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART19_H_

    return;
}

static td_s32 vo_callback_process(gfbg_par *par, td_u32 layer_id, td_bool *is_continue)
{
    unsigned long lock_flag;
    gfbg_sync_attr sync_attr;
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    /* If not displayed, close the graphics layer and exit */
    if (par->show == TD_FALSE) {
        if (g_drv_ops.gfbg_drv_set_layer_enable) {
            g_drv_ops.gfbg_drv_set_layer_enable(layer_id, TD_FALSE);
        }
        if (g_drv_ops.gfbg_drv_updata_layer_reg) {
            g_drv_ops.gfbg_drv_updata_layer_reg(layer_id);
        }
        *is_continue = TD_FALSE;
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_SUCCESS;
    }
    if (layer_id == 0 || layer_id == 1) {
#ifndef __LITEOS__
    #ifdef GFBG_HW_VSYNC_SUPPORT
        gfbg_vsync_notify(); /* vsync signal */
    #endif
#endif
    }

    /* Non-modified status, modified, can be modified */
    if (!par->modifying) {
        /*
         * 1.Get osd data
         * 2.Set layer alpha
         * 3.Set layer keymask
         * 4.Set layer data fmt
         */
        if (callback_get_osd_data(par, layer_id) != TD_SUCCESS) {
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            return TD_FAILURE;
        }

        /*
         * 1.NEW modify graphic dynamic range
         * 2.Set premul
         * 3.Set layer stride
         * 4.Set layer deflicker
         */
        callback_modify_dynamic_range(par, layer_id);

        /*
         * Handles requests to modify input and output sizes
         * Set layer rect
         * Set layer srcImage reso
         * if enable ZME or not
         */
        callback_modify_sizes(par, layer_id);

        /*
         * The display address is refreshed and the display address is modified
         * set layer data fmt
         * set layer stride
         * set layer rect
         * set layer addr
         */
        callback_modify_address(par, layer_id);

        /* set smart rect */
        callback_modify_smart_rect(par, layer_id);

        /* set clut up */
        callback_modify_clut_up(par, layer_id);

        /* gfbg_drv_set_layer_enable */
        g_drv_ops.gfbg_drv_set_layer_enable(layer_id, par->show);
    }
    /* Decompression configuration */
    gfbg_dcmp_config(layer_id);

    /* support low delay (only G0 support low delay) */
    sync_attr.is_sync = TD_TRUE;
    sync_attr.safe_dist = par->display_info.display_width * 3 * /* 3 rgb888 calculate */
                          par->display_info.display_height / 2; /* 2 half frame */
    if (g_drv_ops.gfbg_drv_set_tde_sync != TD_NULL) {
        g_drv_ops.gfbg_drv_set_tde_sync(layer_id, &sync_attr);
    }
    /* gfbg_drv_updata_layer_reg */
    g_drv_ops.gfbg_drv_updata_layer_reg(layer_id);
    *is_continue = TD_TRUE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

/* Callback function for VO vertical timing interrupt */
static td_s32 gfbg_interrupt_process(td_u32 layer_id)
{
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    td_bool is_continue = TD_FALSE;

    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        return TD_FAILURE;
    }
    info = g_layer[layer_id].info;
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    if (info->par == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)(info->par);
    if (vo_callback_process(par, layer_id, &is_continue) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (is_continue != TD_TRUE) {
        return TD_SUCCESS;
    }

    /* Field blanking mark */
    par->vblflag = 1;
    wake_up(&(par->vbl_event));
    return TD_SUCCESS;
}

static td_s32 refresh_0buf_process(gfbg_par *par, const ot_fb_buf *canvas_buf)
{
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    unsigned long lock_flag;

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if (compress_info->compress_open == TD_TRUE) {
        gfbg_error("only FB_LAYER_BUF_DOUBLE or FB_LAYER_BUF_DOUBLE_IMMEDIATE support compress!\n");
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }

    refresh_info->disp_buf_info.compress = TD_FALSE;

    par->modifying = TD_TRUE;
    /* modify by wxl : if change flush type between 2buffer and 0 buffer, the addr couldn't be changed */
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DISPLAYADDR;
    /*
     * The graphic address is taken from the canvas of the user data and
     * filled in the screen address of the refresh information.
     */
    refresh_info->screen_addr = canvas_buf->canvas.phys_addr;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;
    refresh_info->user_buffer.canvas.pitch = canvas_buf->canvas.pitch;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_FMT;
    refresh_info->user_buffer.canvas.format = canvas_buf->canvas.format;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DYNAMICRANGE;
    refresh_info->user_buffer.canvas.dynamic_range = canvas_buf->canvas.dynamic_range;

    /* NEW feature: zme */
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;

    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_s32 gfbg_refresh_0buf(td_u32 layer_id, const ot_fb_buf *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    unsigned long lock_flag;
    td_s32 ret;

    ret = refresh_0buf_process(par, canvas_buf);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * In gfbg_disp_setdispsize, it is possible that kmalloc
     * allocates memory in a non-atomic manner, so the lock must be released first
     */
    if (gfbg_disp_setdispsize(layer_id, canvas_buf->canvas.width, canvas_buf->canvas.height) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_FALSE;
    ret = memcpy_s(&(refresh_info->user_buffer), sizeof(ot_fb_buf), canvas_buf, sizeof(ot_fb_buf));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        return TD_FAILURE;
    }
    if (par->compress_info.compress_open) {
        ret = memcpy_s((td_void *)&par->compress_info.update_rect, sizeof(ot_fb_rect), &canvas_buf->update_rect,
            sizeof(ot_fb_rect));
        gfbg_unlock_unequal_eok_return(ret, &par->lock, lock_flag);
        par->compress_info.update_finished = TD_TRUE;
    }
    par->vblflag = 0;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    /* if the flag "FB_ACTIVATE_VBL" has been set, we should wait for register update finish */
#ifdef __LITEOS__
    if (!in_atomic()) {
#else
    if (!in_atomic() && (info->var.activate & FB_ACTIVATE_VBL)) {
#endif
        gfbg_wait_regconfig_work(layer_id);
    }

    return TD_SUCCESS;
}

static td_void refresh_is_cursor_overlay(const gfbg_par *par, const ot_fb_buf *canvas_buf, td_bool *is_overlay)
{
    gfbg_par *cursor_par = TD_NULL;
    const gfbg_cursor_info *cursor_info = &par->cursor_info;
    ot_fb_rect rc_cursor;
    cursor_par = (gfbg_par *)g_layer[par->cursor_info.attached_cursor_id].info->par;
    if (cursor_info->attached != 0) {
        rc_cursor.x = cursor_par->display_info.pos.x_pos - cursor_par->cursor_info.cursor.hot_pos.x_pos;
        rc_cursor.y = cursor_par->display_info.pos.y_pos - cursor_par->cursor_info.cursor.hot_pos.y_pos;
        rc_cursor.width = cursor_par->cursor_info.cursor.cursor.width;
        rc_cursor.height = cursor_par->cursor_info.cursor.cursor.height;

        /* check the cusor overlay with refresh area */
        if (cursor_par->show && (((rc_cursor.x >= canvas_buf->update_rect.x &&
            rc_cursor.x <=  canvas_buf->update_rect.x + canvas_buf->update_rect.width)) ||
            (rc_cursor.x < canvas_buf->update_rect.x && rc_cursor.x + rc_cursor.width >= canvas_buf->update_rect.x))) {
            if (((rc_cursor.y >= canvas_buf->update_rect.y &&
                rc_cursor.y <= canvas_buf->update_rect.y + canvas_buf->update_rect.height)) ||
                (rc_cursor.y < canvas_buf->update_rect.y &&
                rc_cursor.y + rc_cursor.height >=  canvas_buf->update_rect.y)) {
                *is_overlay = TD_TRUE;
            }
        }
    }
    return;
}

/*
 * Name : gfbg_refresh
 * Desc : It is refreshed according to the canvas information and the layer's buf refresh mode.
 *        It is called indirectly when setting the layer attr.
 * See  : references gfbg_refresh_again,gfbg_onrefresh
 *        calls gfbg_refresh_0buf,gfbg_refresh_1buf,gfbg_refresh_2buf
 */
static td_s32 gfbg_refresh(td_u32 layer_id, const ot_fb_buf *canvas_buf, ot_fb_layer_buf buf_mode)
{
    td_s32 ret = TD_FAILURE;
    gfbg_par *par = (gfbg_par *)g_layer[layer_id].info->par;
    td_bool is_overlay = TD_FALSE; /* is the cusor overlay with refresh area */

    if (canvas_buf == TD_NULL) {
        return TD_FAILURE;
    }
    /*
     * For cursor layer
     * you can quary whether cursor attach to a certain layer for general layer
     * when attached not zero, we should check whether the cusor overlay with refresh area or not
     */
    refresh_is_cursor_overlay(par, canvas_buf, &is_overlay);

#ifdef CURSOR
    if (is_overlay && is_soft_cursor()) {
        gfbg_cursor_hide(layer_id);
    }
#endif
    switch (buf_mode) {
        case OT_FB_LAYER_BUF_DOUBLE:
            ret = gfbg_refresh_2buf(layer_id, canvas_buf);
            break;
        case OT_FB_LAYER_BUF_ONE:
            ret = gfbg_refresh_1buf(layer_id, canvas_buf);
            break;
        case OT_FB_LAYER_BUF_NONE:
            ret = gfbg_refresh_0buf(layer_id, canvas_buf);
            break;
        case OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE:
            ret = gfbg_refresh_2buf_immediate_display(layer_id, canvas_buf);
            break;
        default:
            break;
    }
#ifdef CURSOR
    if (is_overlay && is_soft_cursor()) {
        gfbg_cursor_show(layer_id);
    }
#endif
    return ret;
}

#ifdef __LITEOS__
static void gfbg_free(struct gfbg_info *info)
{
    if (info != TD_NULL) {
        free(info);
    }
}

static struct gfbg_info *gfbg_alloc(size_t size)
{
    td_s32 gfbg_info_len = sizeof(struct gfbg_info);
    struct gfbg_info *info = TD_NULL;

    gfbg_info_len = ALIGN(gfbg_info_len, sizeof(unsigned long));

    info = (struct gfbg_info *)malloc(gfbg_info_len + size);
    if (info == NULL) {
        return NULL;
    }
    (td_void)memset_s(info, (gfbg_info_len + size), 0, (gfbg_info_len + size));

    info->par = (void *)((char *)info + gfbg_info_len);

    return info;
}
#endif


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART19_H_ */
