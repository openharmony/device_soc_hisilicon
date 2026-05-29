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

#include "gfbg_rotate.h"
#include "mod_ext.h"
#include "ot_drv_tde.h"
#include "gfbg_comm.h"
#include "gfbg_vou_drv.h"
#include "gfbg_blit.h"

static td_void drv_rotate_init_tde(drv_tde_surface *surface, drv_tde_rect *rect, const ot_fb_buf *src_img,
    const ot_fb_buf *dst_img);
static td_void drv_get_rotate_mode(const gfbg_rotate_opt *rotate_opt, drv_tde_rotate_angle *tde_rotate_angle);
static td_s32 drv_rotate_start_tde(td_s32 *handle, drv_tde_single_src *single_src,
    drv_tde_rotate_angle tde_rotate_angle, const gfbg_rotate_opt *rotate_opt,
    const ot_tde_export_func *tde_export_func);

static drv_tde_color_fmt g_gfbg_fmt[OT_FB_FORMAT_BUTT] =  {
    DRV_TDE_COLOR_FMT_RGB565,
    DRV_TDE_COLOR_FMT_RGB888,
    DRV_TDE_COLOR_FMT_RGB444,
    DRV_TDE_COLOR_FMT_RGB555,
    DRV_TDE_COLOR_FMT_ARGB8888,
    DRV_TDE_COLOR_FMT_ARGB4444,
    DRV_TDE_COLOR_FMT_ARGB1555,
    DRV_TDE_COLOR_FMT_ARGB8888,
    DRV_TDE_COLOR_FMT_ARGB8565,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_BGR565,
    DRV_TDE_COLOR_FMT_BGR888,
    DRV_TDE_COLOR_FMT_ABGR4444,
    DRV_TDE_COLOR_FMT_ABGR1555,
    DRV_TDE_COLOR_FMT_ABGR8888,
    DRV_TDE_COLOR_FMT_ABGR8565,
    DRV_TDE_COLOR_FMT_BGR444,
    DRV_TDE_COLOR_FMT_BGR555,
    DRV_TDE_COLOR_FMT_ABGR8888,
    DRV_TDE_COLOR_FMT_CLUT1,
    DRV_TDE_COLOR_FMT_CLUT2,
    DRV_TDE_COLOR_FMT_CLUT4,
    DRV_TDE_COLOR_FMT_CLUT8,
    DRV_TDE_COLOR_FMT_ACLUT44,
    DRV_TDE_COLOR_FMT_ACLUT88,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
};

static drv_tde_color_fmt gfbg_drv_conv_fmt(ot_fb_color_format format)
{
    if (format >= OT_FB_FORMAT_BUTT) {
        return DRV_TDE_COLOR_FMT_MAX;
    }
    return g_gfbg_fmt[format];
}

/* rotate with tde */
td_s32 gfbg_drv_rotate_process(const ot_fb_buf *src_img, const ot_fb_buf *dst_img, const gfbg_rotate_opt *rotate_opt,
    td_bool is_refresh_screen)
{
    td_s32 ret;
    td_s32 handle = -1;
    drv_tde_surface src_surface = {0};
    drv_tde_surface dst_surface = {0};
    drv_tde_rect src_rect = {0};
    drv_tde_rect dst_rect = {0};
    ot_tde_export_func *tde_export_func = TD_NULL;
    drv_tde_rotate_angle  tde_rotate_angle = DRV_TDE_ROTATE_MAX;
    drv_tde_single_src single_src = {0};
    ot_unused(is_refresh_screen);
    tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((tde_export_func == TD_NULL) || (tde_export_func->drv_tde_module_begin_job == TD_NULL) ||
        (tde_export_func->drv_tde_module_rotate == TD_NULL) ||
        (tde_export_func->drv_tde_module_cancel_job == TD_NULL) ||
        (tde_export_func->drv_tde_module_end_job == TD_NULL)) {
        gfbg_error("TDE tde_export_func is NULL!\n");
        return TD_FAILURE;
    }
    if (rotate_opt == TD_NULL) {
        gfbg_error("GFBG rotate_opt is NULL!\n");
        return TD_FAILURE;
    }
    /* src init */
    drv_rotate_init_tde(&src_surface, &src_rect, src_img, TD_NULL);
    /* dst_init */
    drv_rotate_init_tde(&dst_surface, &dst_rect, src_img, dst_img);

    /* get rotate mode */
    drv_get_rotate_mode(rotate_opt, &tde_rotate_angle);

    /* rotate start */
    single_src.src_surface = &src_surface;
    single_src.src_rect = &src_rect;
    single_src.dst_surface = &dst_surface;
    single_src.dst_rect = &dst_rect;
    ret = drv_rotate_start_tde(&handle, &single_src, tde_rotate_angle, rotate_opt, tde_export_func);
    if (ret != TD_SUCCESS) {
        gfbg_error("drv_rotate_start_tde failed!\n");
        return ret;
    }

    return handle;
}

static td_void drv_rotate_init_tde(drv_tde_surface *surface, drv_tde_rect *rect, const ot_fb_buf *src_img,
    const ot_fb_buf *dst_img)
{
    if (dst_img == TD_NULL) {
        surface->phys_addr   = src_img->canvas.phys_addr;
        surface->width  = src_img->canvas.width;
        surface->height = src_img->canvas.height;
        surface->stride = src_img->canvas.pitch;
        surface->alpha_max_is_255 = TD_TRUE;
        surface->is_ycbcr_clut = TD_FALSE;
        surface->color_format = gfbg_drv_conv_fmt(src_img->canvas.format);
        rect->pos_x   = src_img->update_rect.x;
        rect->pos_y   = src_img->update_rect.y;
        rect->width  = src_img->update_rect.width;
        rect->height = src_img->update_rect.height;
    } else {
        surface->phys_addr      = dst_img->canvas.phys_addr;
        surface->width     = dst_img->canvas.width;
        surface->height    = dst_img->canvas.height;
        surface->stride    = dst_img->canvas.pitch;
        surface->alpha_max_is_255 = TD_TRUE;
        surface->is_ycbcr_clut   = TD_FALSE;
        surface->color_format   = gfbg_drv_conv_fmt(dst_img->canvas.format);
        rect->pos_x   = dst_img->update_rect.x;
        rect->pos_y   = dst_img->update_rect.y;
        rect->width  = src_img->update_rect.height;
        rect->height = src_img->update_rect.width;
    }

    return;
}

static td_void drv_get_rotate_mode(const gfbg_rotate_opt *rotate_opt, drv_tde_rotate_angle *tde_rotate_angle)
{
    switch (rotate_opt->rotate_mode) {
        case OT_FB_ROTATE_90:
            *tde_rotate_angle = DRV_TDE_ROTATE_CLOCKWISE_90;
            break;
        case OT_FB_ROTATE_180:
            *tde_rotate_angle = DRV_TDE_ROTATE_CLOCKWISE_180;
            break;
        case OT_FB_ROTATE_270:
            *tde_rotate_angle = DRV_TDE_ROTATE_CLOCKWISE_270;
            break;
        default:
            break;
    }
    return;
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
static td_s32 drv_rotate_start_tde(td_s32 *handle, drv_tde_single_src *single_src,
    drv_tde_rotate_angle tde_rotate_angle, const gfbg_rotate_opt *rotate_opt, const ot_tde_export_func *tde_export_func)
{
    td_s32 ret;
    gfbg_tde_callback_param *param = TD_NULL;
    const td_u32 time_out = 100;
    drv_tde_end_job_cmd end_job;
    ret = tde_export_func->drv_tde_module_begin_job(handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = tde_export_func->drv_tde_module_rotate(*handle, single_src, tde_rotate_angle);
    if (ret != TD_SUCCESS) {
        gfbg_error("tde rotate failed ret = 0x%x handle %d \n", ret, *handle);
        tde_export_func->drv_tde_module_cancel_job(*handle);
        return ret;
    }

    if (rotate_opt->call_back) {
        if (in_atomic()) {
            param = kmalloc(sizeof(gfbg_tde_callback_param), GFP_ATOMIC);
        } else {
            param = kmalloc(sizeof(gfbg_tde_callback_param), GFP_KERNEL);
        }
        if (param == TD_NULL) {
            tde_export_func->drv_tde_module_cancel_job(*handle);
            return TD_FAILURE;
        }

        param->layer_id = *(td_u32 *)rotate_opt->param;
        fb_drv_tde_end_job_val(end_job, *handle, rotate_opt->block, time_out, TD_FALSE);
        ret = tde_export_func->drv_tde_module_end_job(&end_job,
            (drv_tde_func_callback)gfbg_drv_get_tde_rotate_callback(), param);
    } else {
        fb_drv_tde_end_job_val(end_job, *handle, rotate_opt->block, time_out, TD_FALSE);
        ret = tde_export_func->drv_tde_module_end_job(&end_job, TD_NULL, TD_NULL);
    }
    if (ret != TD_SUCCESS) {
        ret = tde_export_func->drv_tde_module_cancel_job(*handle);
        if (ret == TD_SUCCESS) {
            if (param != TD_NULL) {
                kfree(param);
                param = TD_NULL;
            }
        } else {
            gfbg_error("cancel job failed!ret = %x\n", ret);
        }
        return ret;
    }
    return ret;
}
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
