/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sample_yuv.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <inttypes.h>

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_sys.h"
#include "ot_common_vb.h"
#include "ot_common_vpss.h"
#include "ss_mpi_sys.h"
#include "ss_mpi_vb.h"
#include "ss_mpi_vpss.h"
#include "ss_mpi_vgs.h"

#include "log.h"
#include "frame_cache.h"
#include "sample_comm.h"

#define SAMPLE_UVC_MAX_YUV_NAME_LEN 128

typedef struct {
    td_bool thread_start;
    td_s32 vpss_grp;
    td_s32 vpss_chn;
} sample_get_yuv_para;

typedef struct {
    ot_vb_blk h_block;
    ot_vb_pool h_pool;
    td_u32 pool_id;
    td_u64 phys_addr;
    td_u8 *vir_addr;
    td_s32 mdev;
} dump_mem_buf;

static sample_get_yuv_para g_yuv_para;
static pthread_t g_yuv_pid;

static ot_video_frame_info g_frame;

static ot_vb_pool g_h_pool = OT_VB_INVALID_POOL_ID;
static ot_vgs_handle g_h_handle = -1;

static ot_vb_pool g_h_pool2 = OT_VB_INVALID_POOL_ID;
static dump_mem_buf g_mem2 = {0};
static td_u32 g_blk_size2 = 0;

static td_char *g_user_page_addr = TD_NULL;
static td_u32 g_size = 0;

static FILE *g_pfd = TD_NULL;

static td_s32 g_do_init_yuv = 0;

td_void set_yuv_property(td_void)
{
    g_do_init_yuv = 0;
}

static td_void sample_yuv_get_buf_size(const ot_video_frame *v_frame, td_u32 *size)
{
    ot_pixel_format pixel_format = v_frame->pixel_format;

    if (pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        *size = v_frame->stride[0] * v_frame->height * 3 / 2;   /* 3/2: 1.5 */
    } else if (pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
        *size = v_frame->stride[0] * v_frame->height * 2;       /* 2:double */
    } else if (pixel_format == OT_PIXEL_FORMAT_YUV_400) {
        *size = v_frame->stride[0] * v_frame->height;
    }

    return;
}

static td_void sample_yuv_get_pic_buf_cfg(const ot_video_frame *v_frame_info, ot_vb_calc_cfg *cal_config)
{
    const td_u32 align = 32; /* 32: default align */
    ot_data_bit_width bit_width = OT_DATA_BIT_WIDTH_8;
    ot_compress_mode cmp_mode = OT_COMPRESS_MODE_NONE;
    td_u32 width = v_frame_info->width;
    td_u32 height = v_frame_info->height;
    ot_pixel_format pixel_format = v_frame_info->pixel_format;
    ot_pic_buf_attr buf_attr;

    buf_attr.width = width;
    buf_attr.height = height;
    buf_attr.pixel_format = pixel_format;
    buf_attr.bit_width = bit_width;
    buf_attr.compress_mode = cmp_mode;
    buf_attr.align = align;

    ot_common_get_pic_buf_cfg(&buf_attr, cal_config);
}

static td_s32 sample_yuv_get_vb_block(td_void)
{
    td_u32 retry;

    for (retry = 0; retry < 1000; retry++) { /* 1000: wait up to about 1s for the VB block. */
        g_mem2.h_block = ss_mpi_vb_get_blk(g_mem2.h_pool, g_blk_size2, TD_NULL);
        if (g_mem2.h_block != OT_VB_INVALID_HANDLE) {
            break;
        }
        usleep(1000); /* 1000: 1 ms */
    }
    if (g_mem2.h_block == OT_VB_INVALID_HANDLE) {
        printf("get vb block failed!\n");
        g_mem2.h_pool = OT_VB_INVALID_POOL_ID;
        ss_mpi_vb_destroy_pool(g_h_pool2);
        g_h_pool2 = OT_VB_INVALID_POOL_ID;
        return TD_FAILURE;
    }

    g_mem2.phys_addr = ss_mpi_vb_handle_to_phys_addr(g_mem2.h_block);
    return TD_SUCCESS;
}

static td_s32 sample_yuv_map_vb_block(td_void)
{
    g_mem2.vir_addr = (td_u8*)ss_mpi_sys_mmap(g_mem2.phys_addr, g_blk_size2);
    if (g_mem2.vir_addr == TD_NULL) {
        printf("sys mmap failed!\n");
        ss_mpi_vb_release_blk(g_mem2.h_block);
        g_mem2.h_pool = OT_VB_INVALID_POOL_ID;
        ss_mpi_vb_destroy_pool(g_h_pool2);
        g_h_pool2 = OT_VB_INVALID_POOL_ID;
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void sample_yuv_fill_frame(const ot_video_frame *v_frame_info, ot_video_frame *v_frame,
    const ot_vb_calc_cfg *cal_config)
{
    v_frame->phys_addr[0] = g_mem2.phys_addr;
    v_frame->phys_addr[1] = v_frame->phys_addr[0] + cal_config->main_y_size;

    v_frame->virt_addr[0] = (td_void*)g_mem2.vir_addr;
    v_frame->virt_addr[1] = v_frame->virt_addr[0] + cal_config->main_y_size;

    v_frame->width  = v_frame_info->width;
    v_frame->height = v_frame_info->height;
    v_frame->stride[0] = cal_config->main_stride * 2; /* 2: pkg 422, memory double */
    v_frame->stride[1] = cal_config->main_stride;

    v_frame->compress_mode = OT_COMPRESS_MODE_NONE;
    v_frame->pixel_format  = OT_PIXEL_FORMAT_VY1UY0_PACKAGE_422;
    v_frame->video_format  = OT_VIDEO_FORMAT_LINEAR;
    v_frame->dynamic_range = v_frame_info->dynamic_range;

    v_frame->pts = v_frame_info->pts;
    v_frame->time_ref = v_frame_info->time_ref;
}

static td_s32 sample_yuv_prepare_vb(const ot_video_frame *v_frame_info, ot_video_frame *v_frame)
{
    ot_vb_calc_cfg cal_config = { 0 };
    ot_vb_pool_cfg vb_pool_cfg = { 0 };

    sample_yuv_get_pic_buf_cfg(v_frame_info, &cal_config);
    g_blk_size2 = cal_config.vb_size;

    vb_pool_cfg.blk_size  = g_blk_size2;
    vb_pool_cfg.blk_cnt   = 1;
    vb_pool_cfg.remap_mode = OT_VB_REMAP_MODE_NONE;

    g_h_pool2 = ss_mpi_vb_create_pool(&vb_pool_cfg);
    if (g_h_pool2 == OT_VB_INVALID_POOL_ID) {
        printf("create vb pool failed!\n");
        return TD_FAILURE;
    }

    g_mem2.h_pool = g_h_pool2;
    if ((sample_yuv_get_vb_block() != TD_SUCCESS) || (sample_yuv_map_vb_block() != TD_SUCCESS)) {
        return TD_FAILURE;
    }

    sample_yuv_fill_frame(v_frame_info, v_frame, &cal_config);
    return TD_SUCCESS;
}

static td_void sample_yuv_release_vb(td_void)
{
    if (g_mem2.vir_addr != TD_NULL) {
        ss_mpi_sys_munmap((td_void*)g_mem2.vir_addr, g_blk_size2);
        g_mem2.vir_addr = TD_NULL;
        g_mem2.phys_addr = 0;
    }

    if (g_mem2.h_pool != OT_VB_INVALID_POOL_ID) {
        ss_mpi_vb_release_blk(g_mem2.h_block);
        g_mem2.h_pool = OT_VB_INVALID_POOL_ID;
    }

    if (g_h_pool2 != OT_VB_INVALID_POOL_ID) {
        ss_mpi_vb_destroy_pool(g_h_pool2);
        g_h_pool2 = OT_VB_INVALID_POOL_ID;
    }

    return;
}

static td_void sample_yuv_copy_pic_to_v4l2(const ot_video_frame_info *frm_info, frame_node_t *f_node)
{
    td_char *y_ptr = TD_NULL;
    td_u8 *node_ptr = f_node->mem;
    td_u32 h;

    f_node->used = 0;
    for (h = 0; h < frm_info->video_frame.height; ++h) {
        y_ptr = (td_char *)(td_ulong)frm_info->video_frame.virt_addr[0] + h * frm_info->video_frame.stride[0];

        if (f_node->used + frm_info->video_frame.width <= f_node->length) {
            (td_void)memcpy_s(node_ptr + f_node->used, frm_info->video_frame.width * 2, /* 2: pkg 422, memory double */
                y_ptr, frm_info->video_frame.width * 2);                                /* 2: pkg 422, memory double */
            f_node->used += frm_info->video_frame.width * 2;                            /* 2: pkg 422, memory double */
        }
    }
}

static td_s32 sample_uvc_sp_to_pkg(const ot_video_frame *v_frame, td_u32 pool_id, frame_node_t *f_node)
{
    td_s32 ret;
    ot_video_frame_info frm_info = { 0 };
    ot_vgs_task_attr task;
    ot_vgs_handle handle;

    if (sample_yuv_prepare_vb(v_frame, &frm_info.video_frame) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = ss_mpi_vgs_begin_job(&handle);
    if (ret != TD_SUCCESS) {
        sample_yuv_release_vb();
        printf("vgs begin job failed\n");
        return TD_FAILURE;
    }

    (td_void)memcpy_s(&task.img_in.video_frame, sizeof(ot_video_frame),
        &g_frame.video_frame, sizeof(ot_video_frame));
    (td_void)memcpy_s(&task.img_out.video_frame, sizeof(ot_video_frame),
        &frm_info.video_frame, sizeof(ot_video_frame));
    task.img_in.mod_id = OT_ID_VPSS;
    task.img_in.pool_id = pool_id;
    task.img_out.mod_id = OT_ID_VGS;
    task.img_out.pool_id = g_h_pool2;

    ret = ss_mpi_vgs_add_scale_task(handle, &task, OT_VGS_SCALE_COEF_NORM);
    if (ret != TD_SUCCESS) {
        (td_void)ss_mpi_vgs_cancel_job(handle);
        sample_yuv_release_vb();
        printf("vgs add scale_task failed\n");
        return TD_FAILURE;
    }

    ret = ss_mpi_vgs_end_job(handle);
    if (ret != TD_SUCCESS) {
        (td_void)ss_mpi_vgs_cancel_job(handle);
        sample_yuv_release_vb();
        printf("vgs end job failed\n");
        return TD_FAILURE;
    }

    sample_yuv_copy_pic_to_v4l2(&frm_info, f_node);

    sample_yuv_release_vb();

    return TD_SUCCESS;
}

static td_void sample_uvc_send_yuv(ot_video_frame *v_buf, td_u32 pool_id, FILE *g_pfd)
{
    uvc_cache_t *uvc_cache = TD_NULL;
    frame_node_t *fnode = TD_NULL;

    sample_yuv_get_buf_size(v_buf, &g_size);
    g_user_page_addr = (td_char *)ss_mpi_sys_mmap(v_buf->phys_addr[0], g_size);
    if (g_user_page_addr == TD_NULL) {
        goto err;
    }

    /* get free cache node */
    uvc_cache = uvc_cache_get();
    if (uvc_cache != TD_NULL) {
        get_node_from_queue(uvc_cache->free_queue, &fnode);
    }

    if (fnode == TD_NULL) {
        goto err;
    }

    if (sample_uvc_sp_to_pkg(v_buf, pool_id, fnode) != TD_SUCCESS) {
        printf("convert semi-planar to package failed!\n");
    }

err:
    if (fnode != TD_NULL) {
        put_node_to_queue(uvc_cache->ok_queue, fnode);
    }
    if (g_user_page_addr != TD_NULL) {
        ss_mpi_sys_munmap(g_user_page_addr, g_size);
        g_user_page_addr = TD_NULL;
    }
    return;
}

static td_void vpss_restore(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn)
{
    if (g_frame.pool_id != OT_VB_INVALID_POOL_ID) {
        if (ss_mpi_vpss_release_chn_frame(vpss_grp, vpss_chn, &g_frame) != TD_SUCCESS) {
            printf("vpss release chn frame failed!\n");
        }

        g_frame.pool_id = OT_VB_INVALID_POOL_ID;
    }

    if (g_h_handle != -1) {
        if (ss_mpi_vgs_cancel_job(g_h_handle) != TD_SUCCESS) {
            printf("vgs cancel job failed!\n");
        }
        g_h_handle = -1;
    }

    if (g_h_pool != OT_VB_INVALID_POOL_ID) {
        if (ss_mpi_vb_destroy_pool(g_h_pool) != TD_SUCCESS) {
            printf("destroy vb pool failed!\n");
        }
        g_h_pool = OT_VB_INVALID_POOL_ID;
    }

    if (g_user_page_addr != TD_NULL) {
        if (ss_mpi_sys_munmap(g_user_page_addr, g_size) != TD_SUCCESS) {
            printf("sys munmap failed!\n");
        }
        g_user_page_addr = TD_NULL;
    }

    if (g_pfd != TD_NULL) {
        fclose(g_pfd);
        g_pfd = TD_NULL;
    }
}

#if (UVC_SAVE_FILE == 1)
static td_s32 sample_yuv_open_file(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_chn_attr *chn_attr)
{
    td_s32 ret;
    td_char sz_yuv_name[SAMPLE_UVC_MAX_YUV_NAME_LEN];

    ret = snprintf_s(sz_yuv_name, SAMPLE_UVC_MAX_YUV_NAME_LEN, SAMPLE_UVC_MAX_YUV_NAME_LEN - 1,
        "output/vpss_grp%d_chn%d_%dx%d_%s.yuv", grp, chn,
        chn_attr->width, chn_attr->height, "P420");
    if (ret < 0) {
        printf("get yuv name err!\n");
        return TD_FAILURE;
    }

    g_pfd = fopen(sz_yuv_name, "wb");
    if (g_pfd == TD_NULL) {
        printf("open file %s err!\n", sz_yuv_name);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}
#endif

td_void *sample_loop_yuv_frame_thread(td_void *p)
{
    td_s32 ret;
    const td_s32 milli_sec = 200; /* 200:default time */
    sample_get_yuv_para *para = (sample_get_yuv_para *)p;

    ot_vpss_grp grp = para->vpss_grp; /* vpss_grp */
    ot_vpss_chn chn = para->vpss_chn; /* vpss_chn */
    ot_vpss_chn_attr chn_attr;

    if (ss_mpi_vpss_get_chn_attr(grp, chn, &chn_attr) != TD_SUCCESS) {
        printf("vpss get chn attr failed!\n");
        return TD_NULL;
    }

    chn_attr.depth = 2; /* 2:default depth */
    if (ss_mpi_vpss_set_chn_attr(grp, chn, &chn_attr) != TD_SUCCESS) {
        printf("vpss set chn attr failed!\n");
        return TD_NULL;
    }

#if (UVC_SAVE_FILE == 1)
    ret = sample_yuv_open_file(grp, chn, &chn_attr);
    if (ret != TD_SUCCESS) {
        printf("open file failed!\n");
        return TD_NULL;
    }
#endif

    /* get frame first */
    while (para->thread_start) {
        if ((ret = ss_mpi_vpss_get_chn_frame(grp, chn, &g_frame, milli_sec)) != TD_SUCCESS) {
            printf("get frame from VPSS fail(0x%x)!\n", ret);
            return TD_NULL;
        }

        sample_uvc_send_yuv(&g_frame.video_frame, g_frame.pool_id, g_pfd);

        /* release frame after using */
        ret = ss_mpi_vpss_release_chn_frame(grp, chn, &g_frame);
        if (ret != TD_SUCCESS) {
            printf("vpss release chn frame failed!\n");
            vpss_restore(grp, chn);
            return TD_NULL;
        }

        g_frame.pool_id = OT_VB_INVALID_POOL_ID;
    }

    vpss_restore(grp, chn);

    return TD_NULL;
}

td_s32 sample_start_get_yuv(td_s32 vpss_grp, td_s32 vpss_chn)
{
    g_yuv_para.thread_start = TD_TRUE;
    g_yuv_para.vpss_chn = vpss_chn;
    g_yuv_para.vpss_grp = vpss_grp;
    return pthread_create(&g_yuv_pid, TD_NULL, sample_loop_yuv_frame_thread, (td_void *)&g_yuv_para);
}

td_s32 sample_stop_get_yuv(td_void)
{
    if (g_yuv_para.thread_start == TD_TRUE) {
        g_yuv_para.thread_start = TD_FALSE;
        pthread_join(g_yuv_pid, TD_NULL);
    }

    return TD_SUCCESS;
}
