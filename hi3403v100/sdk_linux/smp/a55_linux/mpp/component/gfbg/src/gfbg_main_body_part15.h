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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART15_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART15_H_

static td_void gfbg_set_layerpos(gfbg_par *par, const ot_fb_point *pos)
{
    td_s32 x_pos;
    td_s32 y_pos;
    ot_size max_screensize = {0};
    td_u32 layer_id;
    gfbg_display_info *display_info = TD_NULL;

    layer_id = par->layer_id;
    display_info = &par->display_info;

    gfbg_get_maxscreensize(par, &max_screensize.width, &max_screensize.height);
    x_pos = pos->x_pos;
    y_pos = pos->y_pos;
    if (x_pos > (td_s32)(max_screensize.width - gfbg_min_width(layer_id))) {
        gfbg_warning("the sum of x_pos(%d) and min_width(%d) larger than Vodev screen width(%d)!\n", x_pos,
                     gfbg_min_width(layer_id), max_screensize.width);
        x_pos = max_screensize.width - gfbg_min_width(layer_id);
    }

    if (y_pos > (td_s32)(max_screensize.height - gfbg_min_height(layer_id))) {
        gfbg_warning("the sum of y_pos(%d) and min_height(%d) larger than Vodev screen height(%d)!\n", y_pos,
                     gfbg_min_height(layer_id), max_screensize.height);
        y_pos = max_screensize.height - gfbg_min_height(layer_id);
    }

    display_info->pos.x_pos = x_pos;
    display_info->pos.y_pos = y_pos;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;

    return;
}

#ifdef CURSOR
static inline td_void gfbg_set_cursorinfo(gfbg_par *par, gfbg_cursor_info *cursor_info)
{
    td_s32 ret;
    ret = memcpy_s(&par->cursor_info, sizeof(gfbg_cursor_info), cursor_info, sizeof(gfbg_cursor_info));
    gfbg_unequal_eok_return_void(ret);
    return;
}

static inline td_void gfbg_get_cursorinfo(gfbg_par *par, gfbg_cursor_info *cursor_info)
{
    td_s32 ret;
    ret = memcpy_s(cursor_info, sizeof(gfbg_cursor_info), &par->cursor_info, sizeof(gfbg_cursor_info));
    gfbg_unequal_eok_return_void(ret);
    return;
}
#endif

static inline td_bool gfbg_get_show(const gfbg_par *par)
{
    if (par != TD_NULL) {
        return par->show;
    } else {
        return TD_FALSE;
    }
}

static inline td_void gfbg_set_show(gfbg_par *par, td_bool show)
{
    if (par != TD_NULL) {
        par->show = show;
    }
}

static td_void gfbg_get_layerinfo(const gfbg_par *par, ot_fb_layer_info *layer_info)
{
    ot_fb_point pos = {0};
    if ((par != TD_NULL) && (layer_info != TD_NULL)) {
        gfbg_get_premul(par, &layer_info->is_premul);
        gfbg_get_bufmode(par, &layer_info->buf_mode);
        gfbg_get_antiflickerlevel(par, &layer_info->antiflicker_level);
        gfbg_get_layerpos(par, &pos);
        layer_info->x_pos = pos.x_pos;
        layer_info->y_pos = pos.y_pos;
        gfbg_get_dispsize(par, &layer_info->display_width, &layer_info->display_height);
        gfbg_get_screensize(par, &layer_info->screen_width, &layer_info->screen_height);
        layer_info->canvas_width = par->canvas_sur.width;
        layer_info->canvas_height = par->canvas_sur.height;

        layer_info->mask = OT_FB_LAYER_MASK_BUTT;
    }

    return;
}

static td_void gfbg_get_idledispbuf(const gfbg_par *par, td_phys_addr_t *phy_addr)
{
    const gfbg_refresh_info *refresh_info = TD_NULL;
    td_u32 index_for_int;
    if ((par == TD_NULL) || (phy_addr == TD_NULL)) {
        return;
    }
    refresh_info = &par->refresh_info;
    index_for_int = refresh_info->disp_buf_info.index_for_int;
    /*
     * Only the 2buf refresh mode allows to get free buf, otherwise it is the buf in the current
     * interrupt processing.
     */
    if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE) ||
        (refresh_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        *phy_addr = refresh_info->disp_buf_info.phys_addr[1 - index_for_int];
    } else {
        *phy_addr = refresh_info->disp_buf_info.phys_addr[index_for_int];
    }
    return;
}

static td_void gfbg_get_workdispbuf(const gfbg_par *par, td_phys_addr_t *phy_addr)
{
    const gfbg_refresh_info *refresh_info = TD_NULL;
    td_u32 index_for_int;
    if ((par == TD_NULL) || (phy_addr == TD_NULL)) {
        return;
    }
    refresh_info = &par->refresh_info;
    index_for_int = refresh_info->disp_buf_info.index_for_int;

    *phy_addr = refresh_info->disp_buf_info.phys_addr[index_for_int];

    return;
}

static td_void gfbg_get_dispsize(const gfbg_par *par, td_u32 *width, td_u32 *height)
{
    const gfbg_display_info *display_info = TD_NULL;

    display_info = &par->display_info;

    if ((width != TD_NULL) && (height != TD_NULL)) {
        *width = display_info->display_width;
        *height = display_info->display_height;
    }

    return;
}

#ifdef CURSOR
static td_void gfbg_get_idlecursorbuf(gfbg_par *par, td_phys_addr_t *phy_addr)
{
    td_u32 index_for_int;
    if ((par == TD_NULL) || (phy_addr == TD_NULL)) {
        return;
    }
    index_for_int = par->refresh_info.disp_buf_info.index_for_int;

    if ((par->refresh_info.buf_mode == OT_FB_LAYER_BUF_DOUBLE) ||
        (par->refresh_info.buf_mode == OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        *phy_addr = par->cursor_info.cursor.cursor.phys_addr +
            (GFBG_CURSOR_DEF_VRAM * 1024) * (td_u64)(1 - index_for_int) / 2; /* 2 1024 alg data */
    } else {
        *phy_addr = par->cursor_info.cursor.cursor.phys_addr +
            (GFBG_CURSOR_DEF_VRAM * 1024) * (td_u64)index_for_int / 2; /* 2 1024 alg data */
    }

    return;
}
#endif

static inline td_void gfbg_get_workcursorbuf(const gfbg_par *par, td_phys_addr_t *phy_addr)
{
    td_u32 index_for_int;
    index_for_int = par->refresh_info.disp_buf_info.index_for_int;

    *phy_addr = par->cursor_info.cursor.cursor.phys_addr +
		(GFBG_CURSOR_DEF_VRAM * 1024) * (td_u64)index_for_int / 2; /* 2 1024 alg data */

    return;
}

static inline td_void gfbg_set_dispbufflip(gfbg_par *par, td_bool need_flip)
{
    par->refresh_info.disp_buf_info.need_flip = need_flip;

    return;
}

static inline td_void gfbg_get_bufmode(const gfbg_par *par, ot_fb_layer_buf *buf_mode)
{
    *buf_mode = par->refresh_info.buf_mode;

    return;
}

static inline td_void gfbg_get_premul(const gfbg_par *par, td_bool *premul)
{
    *premul = par->display_info.is_premul;

    return;
}

static inline td_void gfbg_get_antiflickerlevel(const gfbg_par *par, ot_fb_layer_antiflicker_level *antiflicker_level)
{
    *antiflicker_level = par->display_info.antiflicker_level;

    return;
}

static inline td_void gfbg_get_layerpos(const gfbg_par *par, ot_fb_point *pos)
{
    td_s32 ret;
    ret = memcpy_s(pos, sizeof(ot_fb_point), &par->display_info.pos, sizeof(ot_fb_point));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return;
    }
    return;
}

static td_void gfbg_get_screensize(const gfbg_par *par, td_u32 *width, td_u32 *height)
{
    const gfbg_display_info *display_info = TD_NULL;
    if (par == TD_NULL) {
        return;
    }
    display_info = &par->display_info;

    if ((width != TD_NULL) && (height != TD_NULL)) {
        *width = display_info->screen_width;
        *height = display_info->screen_height;
    }

    return;
}

static td_s32 gfbg_set_screensize(gfbg_par *par, const td_u32 *width, const td_u32 *height)
{
    td_s32 ret;
    gfbg_display_info *display_info = TD_NULL;

    ret = TD_SUCCESS;
    if ((par == TD_NULL) || (width == TD_NULL) || (height == TD_NULL)) {
        return TD_FAILURE;
    }
    display_info = &par->display_info;

    if (g_drv_ops.capability[par->layer_id].is_vo_scale) {
        display_info->screen_width = *width;
        display_info->screen_height = *height;
    } else {
        if ((display_info->display_width != *width) || (display_info->display_height != *height)) {
            gfbg_error("ScreenSize(%u, %u) can't be different with DisplaySize(%u, %u) \
                       when layer %u don't scale\n",
                       *width, *height, display_info->display_width, display_info->display_height, par->layer_id);
            ret = TD_FAILURE;
        }
    }

    return ret;
}

static td_void gfbg_get_maxscreensize(gfbg_par *par, td_u32 *width, td_u32 *height)
{
    gfbg_osd_data layer_data;
    gfbg_display_info *display_info = TD_NULL;
    if (par == TD_NULL) {
        return;
    }
    display_info = &par->display_info;

    if (g_drv_ops.gfbg_drv_get_osd_data(par->layer_id, &layer_data) == TD_SUCCESS) {
        display_info->max_screen_width = layer_data.screen_width;
        display_info->max_screen_height = layer_data.screen_height;
    }

    if ((width != TD_NULL) && (height != TD_NULL)) {
        *width = display_info->max_screen_width;
        *height = display_info->max_screen_height;
    }

    return;
}

static td_s32 gfbg_tde_rotate_callback(const td_void *paraml, const td_void *paramr)
{
    td_u32 layer_id = 0;
    td_s32  tde_finish_handle;
    gfbg_par *par = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    ot_fb_layer_buf buf_mode;
    unsigned long lockflag;
    gfbg_tde_callback_param *param = (gfbg_tde_callback_param *)paraml;
    tde_finish_handle = *(td_s32 *)paramr;

    if (param != TD_NULL) {
        layer_id = param->layer_id;
    } else {
        gfbg_error("Param is NULL\n");
        return TD_FAILURE;
    }
    if (layer_id <= g_drv_ops.layer_count) {
        par = (gfbg_par *)(g_layer[layer_id].info->par);
    } else {
        kfree(param);
        param = TD_NULL;
        gfbg_error("layer_id = %d is invalid\n", layer_id);
        return TD_FAILURE;
    }
    refresh_info     = &par->refresh_info;
    gfbg_get_bufmode(par, &buf_mode);

    gfbg_spin_lock_irqsave(&par->lock, lockflag);

    if ((buf_mode == OT_FB_LAYER_BUF_DOUBLE) && (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle)) {
        /* Notify VO, it can be updated now */
        gfbg_set_dispbufflip(par, TD_TRUE);

        /* Compression is not supported when there is rotation */
        par->refresh_info.disp_buf_info.compress = TD_FALSE;
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lockflag);
    if (param != TD_NULL) {
        kfree(param);
        param = TD_NULL;
    }
    return TD_SUCCESS;
}

static td_void gfbg_vgs_rotate_callback(ot_mod_id call_mod_id, td_s32 call_dev_id, td_s32 call_chn_id,
    struct ot_vgs_job_data *job_data)
{
    td_u32 layer_id;
    td_s32 vgs_finish_handle;
    td_s32 finish_stat;
    gfbg_par *par = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    ot_fb_layer_buf buf_mode;
    unsigned long lockflag;
    ot_unused(call_mod_id);
    ot_unused(call_dev_id);
    ot_unused(call_chn_id);

    if (job_data == TD_NULL) {
        gfbg_error("job_data is NULL\n");
        return;
    }
    finish_stat = job_data->job_finish_stat;
    layer_id = (td_u32)(job_data->private_data[0]);
    vgs_finish_handle = (td_s32)(job_data->private_data[1]);
    if (layer_id <= g_drv_ops.layer_count) {
        par = (gfbg_par *)(g_layer[layer_id].info->par);
    } else {
        gfbg_error("layer_id = %d is invalid\n", layer_id);
        return;
    }

    refresh_info = &par->refresh_info;
    gfbg_get_bufmode(par, &buf_mode);
    gfbg_spin_lock_irqsave(&par->lock, lockflag);
    if ((buf_mode == OT_FB_LAYER_BUF_DOUBLE) && (refresh_info->disp_buf_info.refresh_handle == vgs_finish_handle) &&
        (finish_stat == VGS_JOB_FNSH_STAT_OK)) {
        /* Notify VO, it can be updated now */
        gfbg_set_dispbufflip(par, TD_TRUE);

        /* Compression is not supported when there is rotation */
        par->refresh_info.disp_buf_info.compress = TD_FALSE;
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lockflag);

    return;
}


static td_void tde_callback_src_init(const gfbg_par *par, ot_fb_buf *src_img)
{
    const gfbg_display_info *display_info = &par->display_info;
    td_u32 bytes_per_pixel = 2;
    src_img->canvas.phys_addr = par->rotate_vb;
    src_img->canvas.width = display_info->display_height;
    src_img->canvas.height = display_info->display_width;
    src_img->update_rect.x = 0;
    src_img->update_rect.y = 0;
    src_img->update_rect.width = src_img->canvas.width;
    src_img->update_rect.height = src_img->canvas.height;

    src_img->canvas.format = par->color_format;
    if (src_img->canvas.format == OT_FB_FORMAT_ARGB1555 || src_img->canvas.format == OT_FB_FORMAT_ARGB4444) {
        bytes_per_pixel = 2; /* 2 is PerPixel */
    } else if (src_img->canvas.format == OT_FB_FORMAT_ARGB8888) {
        bytes_per_pixel = 4; /* 4 is PerPixel */
    }
    src_img->canvas.pitch = ((bytes_per_pixel * src_img->canvas.width + GFBG_ALIGN - 1) / GFBG_ALIGN) * GFBG_ALIGN;
    return;
}

static td_void tde_callback_dst_init(const gfbg_par *par, ot_fb_buf *dst_img, ot_fb_layer_buf buf_mode)
{
    td_u32 bytes_per_pixel = 2;
    const gfbg_refresh_info *refresh_info = &par->refresh_info;
    const gfbg_display_info *display_info = &par->display_info;
    if (buf_mode == OT_FB_LAYER_BUF_DOUBLE) {
        gfbg_get_idledispbuf(par, (td_phys_addr_t*)(&dst_img->canvas.phys_addr));
    } else if (buf_mode == OT_FB_LAYER_BUF_ONE) {
        dst_img->canvas.phys_addr =
            refresh_info->disp_buf_info.phys_addr[refresh_info->disp_buf_info.index_for_int];
    } else if (buf_mode == OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE) {
        dst_img->canvas.phys_addr =
            refresh_info->disp_buf_info.phys_addr[1 - refresh_info->disp_buf_info.index_for_int];
    }
    dst_img->canvas.width = display_info->display_width;
    dst_img->canvas.height = display_info->display_height;
    dst_img->canvas.format = par->color_format;
    dst_img->update_rect.x = 0;
    dst_img->update_rect.y = 0;
    dst_img->update_rect.width = dst_img->canvas.width;
    dst_img->update_rect.height = dst_img->canvas.height;
    if (dst_img->canvas.format == OT_FB_FORMAT_ARGB1555 || dst_img->canvas.format == OT_FB_FORMAT_ARGB4444) {
        bytes_per_pixel = 2; /* 2 is PerPixel */
    } else if (dst_img->canvas.format == OT_FB_FORMAT_ARGB8888) {
        bytes_per_pixel = 4; /* 4 is PerPixel */
    }
    dst_img->canvas.pitch = ((bytes_per_pixel * dst_img->canvas.width + GFBG_ALIGN - 1) / GFBG_ALIGN) * GFBG_ALIGN;
    return;
}

static td_void tde_callback_with_rotate(gfbg_par *par, ot_fb_layer_buf buf_mode)
{
    ot_fb_buf src_img;
    ot_fb_buf dst_img;
    gfbg_rotate_opt rot_opt;
    td_bool is_refresh_screen;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART15_H_ */
