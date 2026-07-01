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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART17_SUB01_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART17_SUB01_H_

static td_void ot_fb_cursor_bakup_double_blit(gfbg_par *par, ot_fb_buf *display_buf, ot_fb_buf *cursor_buf,
    const gfbg_blit_opt *blit_opt)
{
    if ((par->refresh_info.buf_mode != OT_FB_LAYER_BUF_DOUBLE) &&
        (par->refresh_info.buf_mode != OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        return;
    }
    gfbg_get_idledispbuf(par, (td_phys_addr_t*)&display_buf->canvas.phys_addr);
    gfbg_get_idlecursorbuf(par, (td_phys_addr_t*)&cursor_buf->canvas.phys_addr);
    if (gfbg_drv_blit(display_buf, cursor_buf, blit_opt, TD_FALSE) < 0) {
        gfbg_error("buf double gfbg_drv_blit err!\n");
    }
}

/* restore or update cursor backup */
static td_void ot_fb_cursor_bakup(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    td_s32 ret;
    ot_fb_buf cursor_buf;
    ot_fb_buf display_buf;
    gfbg_blit_opt blit_opt = {0};

    (td_void)memset_s(&blit_opt, sizeof(gfbg_blit_opt), 0, sizeof(gfbg_blit_opt));

    if (par->refresh_info.buf_mode == OT_FB_LAYER_BUF_NONE) {
        if (par->refresh_info.user_buffer.canvas.phys_addr == 0) {
            return;
        }
        ret = memcpy_s(&display_buf.canvas, sizeof(ot_fb_surface), &(par->refresh_info.user_buffer.canvas),
            sizeof(ot_fb_surface));
        gfbg_unequal_eok_return_void(ret);
    } else {
        display_buf.canvas.format = par->color_format;
        display_buf.canvas.height = par->display_info.display_height;
        display_buf.canvas.width = par->display_info.display_width;
        display_buf.canvas.pitch = gfbg_get_line_length(info);
        gfbg_get_workdispbuf(par, (td_phys_addr_t*)(&display_buf.canvas.phys_addr));
    }

    ret = memcpy_s(&cursor_buf.canvas, sizeof(ot_fb_surface), &par->cursor_info.cursor.cursor,
        sizeof(ot_fb_surface));
    gfbg_unequal_eok_return_void(ret);

    ret = memcpy_s(&cursor_buf.update_rect, sizeof(ot_fb_rect), &(par->cursor_info.rect_in_disp_buf),
        sizeof(ot_fb_rect));
    gfbg_unequal_eok_return_void(ret);
    cursor_buf.update_rect.x = 0;
    cursor_buf.update_rect.y = 0;
    gfbg_get_workcursorbuf(par, (td_phys_addr_t*)(&cursor_buf.canvas.phys_addr));

    ret = memcpy_s(&display_buf.update_rect, sizeof(ot_fb_rect), &(par->cursor_info.rect_in_disp_buf),
        sizeof(ot_fb_rect));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return;
    }

    blit_opt.mirror_mode = par->display_info.mirror_mode;
    if (gfbg_drv_blit(&display_buf, &cursor_buf, &blit_opt, TD_FALSE) < 0) {
        gfbg_error("gfbg_drv_blit err!\n");
        return;
    }
    ot_fb_cursor_bakup_double_blit(par, &display_buf, &cursor_buf, &blit_opt);
}

static td_s32 cursor_show_blit(td_u32 layer_id, gfbg_par *cursor_par)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = info->par;
    gfbg_blit_opt blit_opt = {0};
    ot_fb_buf cursor_buf;
    ot_fb_buf display_buf;
    td_s32 ret;

    (td_void)memset_s(&blit_opt, sizeof(gfbg_blit_opt), 0, sizeof(gfbg_blit_opt));

    cursor_buf.canvas = cursor_par->cursor_info.cursor.cursor;
    blit_opt.alpha = cursor_par->alpha;
    blit_opt.ckey = cursor_par->ckey;
    blit_opt.mirror_mode = par->display_info.mirror_mode;

    if (par->refresh_info.buf_mode == OT_FB_LAYER_BUF_NONE) {
        if (par->refresh_info.user_buffer.canvas.phys_addr == 0) {
            return TD_FAILURE;
        }
        ret = memcpy_s(&display_buf, sizeof(ot_fb_buf), &(par->refresh_info.user_buffer), sizeof(ot_fb_buf));
        gfbg_unequal_eok_return(ret);
    } else {
        display_buf.canvas.format = par->color_format;
        display_buf.canvas.height = par->display_info.display_height;
        display_buf.canvas.width = par->display_info.display_width;
        display_buf.canvas.pitch = gfbg_get_line_length(info);
        gfbg_get_idledispbuf(par, (td_phys_addr_t*)&display_buf.canvas.phys_addr);
    }

    ret = memcpy_s(&cursor_buf.update_rect, sizeof(ot_fb_rect), &(par->cursor_info.rect_in_disp_buf),
        sizeof(ot_fb_rect));
    gfbg_unequal_eok_return(ret);
    cursor_buf.update_rect.x = par->cursor_info.pos_in_cursor.x_pos;
    cursor_buf.update_rect.y = par->cursor_info.pos_in_cursor.y_pos;

    ret = memcpy_s(&display_buf.update_rect, sizeof(ot_fb_rect), &(par->cursor_info.rect_in_disp_buf),
        sizeof(ot_fb_rect));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    if (gfbg_drv_blit(&cursor_buf, &display_buf, &blit_opt, TD_FALSE) < 0) {
        gfbg_error("gfbg_drv_blit err!\n");
        return TD_FAILURE;
    }

    if ((par->refresh_info.buf_mode == OT_FB_LAYER_BUF_DOUBLE) ||
        (par->refresh_info.buf_mode == OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        gfbg_get_workdispbuf(par, (td_phys_addr_t*)&display_buf.canvas.phys_addr);
        if (gfbg_drv_blit(&cursor_buf, &display_buf, &blit_opt, TD_FALSE) < 0) {
            gfbg_error("buf double gfbg_drv_blit err!\n");
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_cursor_show(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = info->par;
    struct fb_info *cursor_info = TD_NULL;
    gfbg_par *cursor_par = TD_NULL;

    if (!par->cursor_info.attached || !par->show) {
        gfbg_info("Cursor should be attached to layer%d \n", layer_id);
        return TD_FAILURE;
    }

    cursor_info = g_layer[par->cursor_info.attached_cursor_id].info;
    cursor_par = cursor_info->par;

    if (!cursor_par->show) {
        return TD_SUCCESS;
    }

    if (cursor_par->cursor_info.cursor.cursor.phys_addr == 0) {
        gfbg_info("No cusor img set!\n");
        return TD_FAILURE;
    }

    ot_fb_cursor_calcdispinfo(cursor_par->layer_id, par, &cursor_par->display_info.pos);

    ot_fb_cursor_bakup(layer_id);

    /* fill cursor_buf,display_buf,blit_opt, and blit */
    if (cursor_show_blit(layer_id, cursor_par) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    par->show = TD_TRUE;

    if (par->compress_info.compress_open) {
        par->compress_info.update_finished = TD_FALSE;
        /* Waiting for TDE to complete the blit */
        schedule_timeout(msecs_to_jiffies(40)); /* 40 jiffies */
        par->compress_info.update_rect.x = 0;
        par->compress_info.update_rect.y = 0;
        par->compress_info.update_rect.width = par->display_info.display_width;
        par->compress_info.update_rect.height = par->display_info.display_height;
        par->compress_info.update_finished = TD_TRUE;
    }
    return TD_SUCCESS;
}

static td_s32 cursor_hide_double_blit(gfbg_par *par, ot_fb_buf *cursor_buf, ot_fb_buf *display_buf,
    const gfbg_blit_opt *blit_opt)
{
    if ((par->refresh_info.buf_mode != OT_FB_LAYER_BUF_DOUBLE) &&
        (par->refresh_info.buf_mode != OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        return TD_SUCCESS;
    }
    gfbg_get_workdispbuf(par, (td_phys_addr_t*)&display_buf->canvas.phys_addr);
    gfbg_get_workcursorbuf(par, (td_phys_addr_t*)&cursor_buf->canvas.phys_addr);
    if (gfbg_drv_blit(cursor_buf, display_buf, blit_opt, TD_FALSE) < 0) {
        gfbg_error("buf double gfbg_drv_blit err!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 cursor_hide_blit(td_u32 layer_id, gfbg_par *cursor_par)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = info->par;
    gfbg_blit_opt blit_opt = {0};
    ot_fb_buf cursor_buf;
    ot_fb_buf display_buf;
    td_s32 ret;

    (td_void)memset_s(&blit_opt, sizeof(gfbg_blit_opt), 0, sizeof(gfbg_blit_opt));

    cursor_buf.canvas = par->cursor_info.cursor.cursor;

    if (par->refresh_info.buf_mode == OT_FB_LAYER_BUF_NONE) {
        if (par->refresh_info.user_buffer.canvas.phys_addr == 0) {
            gfbg_info("No user buf send to fb in 0 buf mode!\n");
            return TD_FAILURE;
        }
        ret = memcpy_s(&display_buf, sizeof(ot_fb_buf), &(par->refresh_info.user_buffer), sizeof(ot_fb_buf));
        gfbg_unequal_eok_return(ret);
    } else {
        display_buf.canvas.format = par->color_format;
        display_buf.canvas.height = par->display_info.display_height;
        display_buf.canvas.width = par->display_info.display_width;
        display_buf.canvas.pitch = gfbg_get_line_length(info);
        gfbg_get_idledispbuf(par, (td_phys_addr_t*)&display_buf.canvas.phys_addr);
    }

    ret = memcpy_s(&cursor_buf.update_rect, sizeof(ot_fb_rect), &(par->cursor_info.rect_in_disp_buf),
        sizeof(ot_fb_rect));
    gfbg_unequal_eok_return(ret);
    cursor_buf.update_rect.x = 0;
    cursor_buf.update_rect.y = 0;

    ret = memcpy_s(&display_buf.update_rect, sizeof(ot_fb_rect), &(par->cursor_info.rect_in_disp_buf),
        sizeof(ot_fb_rect));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }

    gfbg_get_idlecursorbuf(par, (td_phys_addr_t*)&cursor_buf.canvas.phys_addr);

    blit_opt.mirror_mode = par->display_info.mirror_mode;
    if (gfbg_drv_blit(&cursor_buf, &display_buf, &blit_opt, TD_FALSE) < 0) {
        gfbg_error("gfbg_drv_blit err!\n");
        return TD_FAILURE;
    }
    return cursor_hide_double_blit(par, &cursor_buf, &display_buf, &blit_opt);
}

static td_s32 gfbg_cursor_hide(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = info->par;
    struct fb_info *cursor_info = TD_NULL;
    gfbg_par *cursor_par = TD_NULL;

    if (!par->cursor_info.attached) {
        gfbg_info("Cursor should be attached to layer%d \n", layer_id);
        return TD_FAILURE;
    }

    cursor_info = g_layer[par->cursor_info.attached_cursor_id].info;
    cursor_par = (gfbg_par *)cursor_info->par;

    if (!cursor_par->show || !par->show) {
        return TD_SUCCESS;
    }

    /* fill cursor_buf,display_buf,blit_opt, and blit */
    if (cursor_hide_blit(layer_id, cursor_par) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (par->compress_info.compress_open) {
        par->compress_info.update_finished = TD_FALSE;
        /* 40 Waiting for TDE to complete the blit */
        schedule_timeout(msecs_to_jiffies(40));
        par->compress_info.update_rect.x = 0;
        par->compress_info.update_rect.y = 0;
        par->compress_info.update_rect.width = par->display_info.display_width;
        par->compress_info.update_rect.height = par->display_info.display_height;
        par->compress_info.update_finished = TD_TRUE;
    }
    return TD_SUCCESS;
}

static td_s32 ot_fb_cursor_attach(td_u32 cursor_id, td_u32 layer_id)
{
    td_u32 cnt;

    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;

    struct fb_info *cursorinfo = g_layer[cursor_id].info;
    gfbg_par *cursor_par = (gfbg_par *)cursorinfo->par;

    unsigned long lock_flag;

    cnt = atomic_read(&par->ref_count);
    if (cnt == 0) {
        gfbg_error("failed to attach layer! The layer :%d is not opened!\n", layer_id);
        return TD_FAILURE;
    }

    if (gfbg_is_clutfmt(par->color_format)) {
        gfbg_error("failed to attach layer! The layer format is clut not supported!\n");
        return TD_FAILURE;
    }

    if (cursor_par->cursor_info.cursor.cursor.phys_addr == 0) {
        gfbg_error("failed to attach layer! The cursor info is not set yet!\n");
        return TD_FAILURE;
    }

    if (par->cursor_info.attached) {
        if (par->cursor_info.attached_cursor_id == cursor_id) {
            return TD_SUCCESS;
        } else {
            gfbg_error("The layer has attached another cursor, you should detach it first!\n");
            return TD_FAILURE;
        }
    }

    par->cursor_info.attached = 1;
    par->cursor_info.attached_cursor_id = cursor_id;

    if ((par->cursor_info.cursor.cursor.height > par->display_info.display_height) ||
        (par->cursor_info.cursor.cursor.width > par->display_info.display_width)) {
        return TD_FAILURE;
    }

    par->cursor_info.cursor.cursor.height = cursor_par->cursor_info.cursor.cursor.height;
    par->cursor_info.cursor.cursor.width = cursor_par->cursor_info.cursor.cursor.width;
    par->cursor_info.cursor.hot_pos = cursor_par->cursor_info.cursor.hot_pos;

    /* when cursor attach to layer, we use the position calculate before */
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    if (gfbg_cursor_show(layer_id) != TD_SUCCESS) {
        gfbg_error("gfbg_cursor_show FAILURE\r\n");
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_s32 ot_fb_cursor_detach(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    if (par == TD_NULL) {
        return TD_FAILURE;
    }
    if (!par->cursor_info.attached) {
        return TD_SUCCESS;
    }

    if (gfbg_cursor_hide(layer_id) != TD_SUCCESS) {
        gfbg_error("gfbg_cursor_hide FAILURE\r\n");
    }

    par->cursor_info.attached = TD_FALSE;
    return TD_SUCCESS;
}

static td_void cursor_calc_display_rect(gfbg_par *par, const ot_fb_point* cursor_new_pos, ot_fb_rect *rect)
{
    drv_tde_rect src_rect = {0};
    drv_tde_rect dst_rect = {0};
    drv_tde_rect in_src_rect = {0};
    drv_tde_rect in_dst_rect = {0};
    if ((par->refresh_info.user_buffer.canvas.height && par->refresh_info.user_buffer.canvas.width &&
        (par->refresh_info.user_buffer.canvas.height != par->display_info.display_height)) ||
        (par->refresh_info.user_buffer.canvas.width && par->refresh_info.user_buffer.canvas.height &&
        (par->refresh_info.user_buffer.canvas.width != par->display_info.display_width))) {
        src_rect.width = par->refresh_info.user_buffer.canvas.width;
        src_rect.height = par->refresh_info.user_buffer.canvas.height;
        dst_rect.width = par->display_info.display_width;
        dst_rect.height = par->display_info.display_height;
        in_src_rect.pos_x = cursor_new_pos->x_pos;
        in_src_rect.pos_y = cursor_new_pos->y_pos;
        tde_cal_scale_rect_gfbg(&src_rect, &dst_rect, &in_src_rect, &in_dst_rect);
        rect->x = in_dst_rect.pos_x;
        rect->y = in_dst_rect.pos_y;
    } else {
        rect->x = cursor_new_pos->x_pos;
        rect->y = cursor_new_pos->y_pos;
    }

    if (rect->x > (td_s32)par->display_info.display_width) {
        rect->x = (td_s32)(par->display_info.display_width - 1);
    }

    if (rect->y > (td_s32)par->display_info.display_height) {
        rect->y = (td_s32)(par->display_info.display_height - 1);
    }
    return;
}

/*
 * calculate cusor display position info such as the start position of copying,
 * display buffer position, width of copying.
 */
static td_void ot_fb_cursor_calcdispinfo(td_u32 cursor_id, gfbg_par *par, const ot_fb_point* cursor_new_pos)
{
    struct fb_info *info = TD_NULL;
    gfbg_par *cursor_par = TD_NULL;
    ot_fb_point pos = {0};
    ot_fb_rect rect = {0};

    info = g_layer[cursor_id].info;
    cursor_par = (gfbg_par *)info->par;

    if ((par == TD_NULL) || (cursor_new_pos == TD_NULL)) {
        return;
    }

    /* calculate in display rect:(x,y) */
    cursor_calc_display_rect(par, cursor_new_pos, &rect);
    rect.x -= cursor_par->cursor_info.cursor.hot_pos.x_pos;
    rect.y -= cursor_par->cursor_info.cursor.hot_pos.y_pos;

    /* calculate in display rect:(w,h) */
    rect.width = (td_s32)cursor_par->cursor_info.cursor.cursor.width;
    rect.height = (td_s32)cursor_par->cursor_info.cursor.cursor.height;
    if ((td_s32)rect.x < 0) {
        rect.x = 0;
        pos.x_pos = cursor_par->cursor_info.cursor.hot_pos.x_pos;
        rect.width -= pos.x_pos;
    }

    if ((td_s32)rect.y < 0) {
        rect.y = 0;
        pos.y_pos = cursor_par->cursor_info.cursor.hot_pos.y_pos;
        rect.height -= pos.y_pos;
    }

    if (rect.x + rect.width > (td_s32)par->display_info.display_width) {
        rect.width = (td_s32)(par->display_info.display_width - rect.x);
    }

    if (rect.y + rect.height > (td_s32)par->display_info.display_height) {
        rect.height = (td_s32)(par->display_info.display_height - rect.y);
    }

    par->cursor_info.pos_in_cursor = pos;
    par->cursor_info.rect_in_disp_buf = rect;

    return;
}

static td_s32 ot_fb_cursor_changepos(td_u32 cursor_id, ot_fb_point pos)
{
    struct fb_info *cursorinfo = g_layer[cursor_id].info;
    gfbg_par *cursor_par = (gfbg_par *)cursorinfo->par;
    ot_size max_screensize = {0};
    td_u32 i;

    if (pos.x_pos < 0 || pos.y_pos < 0) {
        gfbg_error("the cursor pos(%d,%d) is less than 0 !\n", pos.x_pos, pos.y_pos);
        return TD_FAILURE;
    }


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART17_SUB01_H_ */
