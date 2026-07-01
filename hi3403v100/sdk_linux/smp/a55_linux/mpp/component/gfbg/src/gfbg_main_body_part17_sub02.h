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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART17_SUB02_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART17_SUB02_H_


    /* pos no change */
    if ((pos.x_pos == cursor_par->display_info.pos.x_pos) && (pos.y_pos == cursor_par->display_info.pos.y_pos)) {
        return TD_SUCCESS;
    }

    gfbg_get_maxscreensize(cursor_par, &max_screensize.width, &max_screensize.height);
    if (pos.x_pos > max_screensize.width - gfbg_min_width(cursor_id)) {
            gfbg_warning("the sum of x_pos(%d) and min_width(%d) larger than Vodev screen width(%d)!\n",
                pos.x_pos, gfbg_min_width(cursor_id), max_screensize.width);
            pos.x_pos = max_screensize.width - gfbg_min_width(cursor_id);
        }

    if (pos.y_pos > max_screensize.height - gfbg_min_height(cursor_id)) {
        gfbg_warning("the sum of y_pos(%d) and min_height(%d) larger than Vodev screen height(%d)!\n",
            pos.y_pos, gfbg_min_height(cursor_id), max_screensize.height);
        pos.y_pos = max_screensize.height - gfbg_min_height(cursor_id);
    }
    cursor_par->display_info.pos.x_pos = pos.x_pos;
    cursor_par->display_info.pos.y_pos = pos.y_pos;

    if (!cursor_par->show) {
        return TD_FAILURE;
    }

    /* process all layers attached to cursor */
    for (i = 0; i < g_drv_ops.layer_count; i++) {
        struct fb_info *info = g_layer[i].info;
        gfbg_par *par = (gfbg_par *)info->par;

        if (is_cursor_layer(i)) {
            continue;
        }

        if (!par->cursor_info.attached) {
            continue;
        }
        if (gfbg_cursor_hide(i) != TD_SUCCESS) {
            gfbg_error("gfbg_cursor_hide FAILURE\r\n");
        }
        if (gfbg_cursor_show(i) != TD_SUCCESS) {
            gfbg_error("gfbg_cursor_show FAILURE\r\n");
        }
    }

    return TD_SUCCESS;
}

static td_s32 ot_fb_cursor_changestate(gfbg_par *cursor_par, td_bool show)
{
    td_u32 i;
    if (cursor_par == TD_NULL) {
        return TD_FAILURE;
    }

    if (!cursor_par->cursor_info.cursor.cursor.phys_addr) {
        gfbg_error("The cursor image addr is NULL!\n");
        return TD_FAILURE;
    }

    /* no change to state , return */
    if (show == cursor_par->show) {
        return TD_SUCCESS;
    }

    for (i = 0; i < g_drv_ops.layer_count; i++) {
        if (is_cursor_layer(i)) {
            continue;
        }

        if (show) {
            cursor_par->show = TD_TRUE;
            gfbg_cursor_show(i);
        } else {
            gfbg_cursor_hide(i);
        }
    }

    cursor_par->show = show;

    return TD_SUCCESS;
}

static td_s32 cursor_info_check(ot_fb_cursor* cursor)
{
    if (cursor->cursor.width == 0 || cursor->cursor.height == 0) {
        gfbg_error("cursor's width or height shouldn't be 0!\n");
        return TD_FAILURE;
    }

    if (cursor->cursor.pitch == 0) {
        gfbg_error("cursor's pitch shouldn't be 0!\n");
        return TD_FAILURE;
    }

    if (cursor->cursor.format == OT_FB_FORMAT_BUTT) {
        gfbg_error("unknown color format!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

#ifndef __LITEOS__
static td_s32 ot_fb_cursor_update_bitfield(gfbg_par *cursor_par)
{
    td_s32 ret;

    ret = memcpy_s(&g_layer[cursor_par->layer_id].info->var.red, sizeof(struct fb_bitfield),
        &g_argb_bit_field[cursor_par->color_format].red, sizeof(struct fb_bitfield));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    ret = memcpy_s(&g_layer[cursor_par->layer_id].info->var.green, sizeof(struct fb_bitfield),
        &g_argb_bit_field[cursor_par->color_format].green, sizeof(struct fb_bitfield));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    ret = memcpy_s(&g_layer[cursor_par->layer_id].info->var.blue, sizeof(struct fb_bitfield),
        &g_argb_bit_field[cursor_par->color_format].blue, sizeof(struct fb_bitfield));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    ret = memcpy_s(&g_layer[cursor_par->layer_id].info->var.transp, sizeof(struct fb_bitfield),
        &g_argb_bit_field[cursor_par->color_format].transp, sizeof(struct fb_bitfield));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
#endif

static td_s32 ot_fb_cursor_putinfo(gfbg_par *cursor_par, ot_fb_cursor* cursor)
{
    td_s32 ret;

    if (cursor->cursor.phys_addr == 0) {
        gfbg_error("cursor image addr is equal to 0!\n");
        cursor_par->cursor_info.cursor.cursor.phys_addr = 0;
        return TD_SUCCESS;
    }
    /* cursor info check:width,height,pitch,format */
    if (cursor_info_check(cursor) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    cursor_par->color_format = cursor->cursor.format;
    if (cursor_par->color_format >= (sizeof(g_argb_bit_field) / sizeof(g_argb_bit_field[0]))) {
        gfbg_error("unknown color format!\n");
        return TD_FAILURE;
    }
#ifndef __LITEOS__
    ret = ot_fb_cursor_update_bitfield(cursor_par);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif
    /* change hotx or hoty will result in cursor position change */
    cursor->cursor.height = (cursor->cursor.height > GFBG_MAX_CURSOR_HEIGHT) ? GFBG_MAX_CURSOR_HEIGHT :
        cursor->cursor.height;
    cursor->cursor.width = (cursor->cursor.width > GFBG_MAX_CURSOR_WIDTH) ? GFBG_MAX_CURSOR_WIDTH :
        cursor->cursor.width;

    if ((cursor->hot_pos.x_pos < 0) || (cursor->hot_pos.x_pos > cursor->cursor.width) ||
        (cursor->hot_pos.y_pos < 0) || (cursor->hot_pos.y_pos > cursor->cursor.height)) {
        gfbg_error("hotpos_x:(%d) hotpos_y:(%d),hotpos_x should be in [%d, %d], hotpos_y should be in [%d, %d].\n",
            cursor->hot_pos.x_pos, cursor->hot_pos.y_pos, 0, cursor->cursor.width, 0, cursor->cursor.height);
        return TD_FAILURE;
    }

    if ((cursor->hot_pos.x_pos != cursor_par->cursor_info.cursor.hot_pos.x_pos) ||
        (cursor->hot_pos.y_pos != cursor_par->cursor_info.cursor.hot_pos.y_pos)) {
        cursor_par->cursor_info.cursor.hot_pos.x_pos = cursor->hot_pos.x_pos;
        cursor_par->cursor_info.cursor.hot_pos.y_pos = cursor->hot_pos.y_pos;
    }

    /* to do :update backup */
    ret = memcpy_s(&(cursor_par->cursor_info.cursor), sizeof(ot_fb_cursor), cursor, sizeof(ot_fb_cursor));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART17_SUB02_H_ */
