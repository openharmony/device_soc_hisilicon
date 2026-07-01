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
#include "vgs_ext.h"
#include "securec.h"
#include "gfbg_comm.h"
#include "gfbg_vou_drv.h"

static td_void drv_rotate_init_vgs(const ot_fb_buf *img, ot_video_frame_info *img_vgs);
static td_s32 drv_rotate_vgs_check_support(const vgs_task_data *task, ot_fb_rotate_mode rotate_mode);
static td_s32 drv_rotate_start_vgs(ot_vgs_handle vgs_handle, vgs_task_data *task, vgs_job_data *job_data,
                                   const gfbg_rotate_opt *rotate_opt, const vgs_export_func *vgs_func);

/* rotate with vgs */
td_s32 gfbg_drv_rotate_process(const ot_fb_buf *src_img, const ot_fb_buf *dst_img, const gfbg_rotate_opt *rotate_opt,
    td_bool is_refresh_screen)
{
    td_s32 ret;
    ot_vgs_handle vgs_handle;
    vgs_job_data job_data = {0};
    ot_mpp_chn mpp_chn;
    vgs_task_data *task = TD_NULL;
    vgs_export_func *vgs_func = TD_NULL;
    ot_unused(is_refresh_screen);
    vgs_func = func_entry(vgs_export_func, OT_ID_VGS);
    if ((vgs_func == TD_NULL) || (vgs_func->pfn_vgs_begin_job == TD_NULL) || (rotate_opt == TD_NULL) ||
        (vgs_func->pfn_vgs_get_free_task == TD_NULL) || (vgs_func->pfn_vgs_add_rotation_task == TD_NULL) ||
        (vgs_func->pfn_vgs_cancel_job == TD_NULL) || (vgs_func->pfn_vgs_end_job == TD_NULL)) {
        gfbg_error("vgs_export_func is NULL!\n");
        return TD_FAILURE;
    }
    /* 1. begin job */
    job_data.job_type = VGS_JOB_TYPE_NORMAL;
    mpp_chn.mod_id = OT_ID_FB;
    mpp_chn.dev_id = 0;
    mpp_chn.chn_id = 0;
    ret = vgs_func->pfn_vgs_begin_job(&vgs_handle, VGS_JOB_PRI_HIGH, &mpp_chn, &job_data);
    if (ret != TD_SUCCESS) {
        gfbg_error("vgs_begin_job failed!\n");
        return TD_FAILURE;
    }
    /* 2. add rotation job */
    task = vgs_func->pfn_vgs_get_free_task();
    if (task == TD_NULL) {
        vgs_func->pfn_vgs_cancel_job(vgs_handle);
        gfbg_error("can't get VGS free task, it may be busy\n");
        return TD_FAILURE;
    }
    (td_void)memset_s(task, sizeof(vgs_task_data), 0, sizeof(vgs_task_data));
    /* src init */
    drv_rotate_init_vgs(src_img, &(task->img_in));
    /* dst init */
    drv_rotate_init_vgs(dst_img, &(task->img_out));

    ret = drv_rotate_vgs_check_support(task, rotate_opt->rotate_mode);
    if (ret != TD_SUCCESS) {
        vgs_func->pfn_vgs_put_free_task(task);
        vgs_func->pfn_vgs_cancel_job(vgs_handle);
        return TD_FAILURE;
    }

    ret = drv_rotate_start_vgs(vgs_handle, task, &job_data, rotate_opt, vgs_func);
    if (ret != TD_SUCCESS) {
        vgs_func->pfn_vgs_cancel_job(vgs_handle);
        return TD_FAILURE;
    }
    return vgs_handle;
}

static td_void drv_rotate_init_vgs(const ot_fb_buf *img, ot_video_frame_info *img_vgs)
{
    switch (img->canvas.format) {
        case OT_FB_FORMAT_ARGB1555:
            img_vgs->video_frame.pixel_format = OT_PIXEL_FORMAT_ARGB_1555;
            break;
        case OT_FB_FORMAT_ARGB4444:
            img_vgs->video_frame.pixel_format = OT_PIXEL_FORMAT_ARGB_4444;
            break;
        default:
            img_vgs->video_frame.pixel_format = OT_PIXEL_FORMAT_BUTT;
    }
    img_vgs->video_frame.width = img->canvas.width * 2; /* 2 vgs need */
    img_vgs->video_frame.height = img->canvas.height * 2; /* 2 vgs need */
    img_vgs->video_frame.field = OT_VIDEO_FIELD_FRAME;
    img_vgs->video_frame.video_format = OT_VIDEO_FORMAT_LINEAR;
    img_vgs->video_frame.compress_mode = OT_COMPRESS_MODE_NONE;
    img_vgs->video_frame.stride[0] = (img_vgs->video_frame.width + 0xf) & (~0xf);
    img_vgs->video_frame.stride[1] = (img_vgs->video_frame.width + 0xf) & (~0xf);
    img_vgs->video_frame.phys_addr[0] = img->canvas.phys_addr;
    img_vgs->video_frame.phys_addr[1] = img->canvas.phys_addr;
    img_vgs->pool_id = 0;
    img_vgs->mod_id = OT_ID_FB;
    return;
}

static td_s32 drv_rotate_vgs_check_support(const vgs_task_data *task, ot_fb_rotate_mode rotate_mode)
{
    if (rotate_mode != OT_FB_ROTATE_90 && rotate_mode != OT_FB_ROTATE_270) {
        gfbg_error("only support rotate 90 degree or 270 degree!\n");
        return TD_FAILURE;
    }

    if (task->img_in.video_frame.pixel_format != OT_PIXEL_FORMAT_ARGB_1555 &&
        task->img_in.video_frame.pixel_format != OT_PIXEL_FORMAT_ARGB_4444 &&
        task->img_out.video_frame.pixel_format != OT_PIXEL_FORMAT_ARGB_1555 &&
        task->img_out.video_frame.pixel_format != OT_PIXEL_FORMAT_ARGB_4444) {
        gfbg_error("only support pixel format argb1555 or argb4444!in_format=%d,out_format=%d\n",
            task->img_in.video_frame.pixel_format, task->img_out.video_frame.pixel_format);
        return TD_FAILURE;
    }

    if (task->img_in.video_frame.pixel_format != task->img_out.video_frame.pixel_format) {
        gfbg_error("not support convert pixel format!\n");
        return TD_FAILURE;
    }

    if (task->img_in.video_frame.width != task->img_out.video_frame.height ||
        task->img_in.video_frame.height != task->img_out.video_frame.width) {
        gfbg_error("not support rotate with scale;input:(w:%d,h:%d);output:(w:%d,h:%d)!\n",
            task->img_in.video_frame.width, task->img_in.video_frame.height, task->img_out.video_frame.width,
            task->img_out.video_frame.height);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_rotate_start_vgs(ot_vgs_handle vgs_handle, vgs_task_data *task, vgs_job_data *job_data,
    const gfbg_rotate_opt *rotate_opt, const vgs_export_func *vgs_func)
{
    td_s32 ret;
    task->call_back = NULL;
    task->call_mod_id = OT_ID_FB;
    task->call_dev_id = 0;
    task->call_chn_id = 0;
    task->finish_stat = 0;

    ret = vgs_func->pfn_vgs_add_rotation_task(vgs_handle, task, rotate_opt->rotate_mode);
    if (ret != TD_SUCCESS) {
        gfbg_error("vgs_add_rotation_task fail for %x!\n", ret);
        return TD_FAILURE;
    }
    /* 3. end job */
    job_data->job_type = VGS_JOB_TYPE_NORMAL;
    if (rotate_opt->call_back == TD_TRUE) {
        job_data->job_call_back = gfbg_drv_get_vgs_rotate_callback();
        job_data->private_data[0] = *(td_u32 *)rotate_opt->param;
        job_data->private_data[1] = vgs_handle;
    } else {
        job_data->job_call_back = NULL;
    }
    ret = vgs_func->pfn_vgs_end_job(vgs_handle, TD_TRUE, job_data);
    if (ret != TD_SUCCESS) {
        gfbg_error("vgs_end_job for %x!\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
