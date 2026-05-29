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
#include "sample_dpu_proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <limits.h>
#include "ot_buffer.h"
#include "ot_common.h"
#include "ot_common_sys.h"
#include "ot_common_dpu_rect.h"

#include "sample_comm.h"
#include "sample_common_dpu.h"

static ot_sample_svp_dpu_cfg g_dpu_cfg;
static td_bool g_stop_signal = TD_FALSE;
static td_void sample_svp_dpu_set_pic_buf_attr(td_u32 width, td_u32 height,
    ot_pixel_format pixel_format, ot_pic_buf_attr *pic_buf_attr)
{
    pic_buf_attr->width = width;
    pic_buf_attr->height = height;
    pic_buf_attr->bit_width = ((pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) ?
        OT_DATA_BIT_WIDTH_8 : OT_DATA_BIT_WIDTH_16);
    pic_buf_attr->pixel_format = pixel_format;
    pic_buf_attr->compress_mode = OT_COMPRESS_MODE_NONE;
    pic_buf_attr->align = OT_DEFAULT_ALIGN;
    return;
}

static td_void sample_svp_dpu_uninit_dpu_rect(td_void)
{
    td_u32 i;

    (td_void)sample_common_svp_dpu_rect_stop(g_dpu_cfg.dpu_rect_grp,
        g_dpu_cfg.dpu_rect_grp_attr.rect_mode);
        for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
            (td_void)sample_common_svp_dpu_rect_unload_lut(&g_dpu_cfg.dpu_rect_mem_info[i], g_dpu_cfg.lut_id[i]);
    }
    return;
}

static td_s32 sample_svp_dpu_start_dpu_rect(const ot_size *src_size, const ot_size *dst_size,
    td_char *lut_file_name[], td_u32 file_num)
{
    td_s32 ret;
    td_u32 i;
    td_u32 rect_pipe_num = 0;
    ot_dpu_rect_chn_attr rect_chn_attr[OT_DPU_RECT_MAX_CHN_NUM] = {0};
    td_char path[PATH_MAX] = {0};

    for (i = 0; i < file_num; i++) {
        sample_svp_dpu_check_exps_return(((strlen(lut_file_name[i]) > PATH_MAX) ||
            (realpath(lut_file_name[i], path) == TD_NULL)),
            OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Error, file_name is invalid!\n");
        ret = sample_common_svp_dpu_rect_load_lut(lut_file_name[i],
            &g_dpu_cfg.dpu_rect_mem_info[i], &g_dpu_cfg.lut_id[i]);
        sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail_0, "load lut failed!\n");
        rect_pipe_num++;
    }

    g_dpu_cfg.dpu_rect_grp_attr.left_lut_id = g_dpu_cfg.lut_id[0];
    g_dpu_cfg.dpu_rect_grp_attr.right_lut_id = g_dpu_cfg.lut_id[1];
    g_dpu_cfg.dpu_rect_grp = 0;
    g_dpu_cfg.dpu_rect_grp_attr.left_image_size.width = src_size->width;
    g_dpu_cfg.dpu_rect_grp_attr.left_image_size.height = src_size->height;
    g_dpu_cfg.dpu_rect_grp_attr.right_image_size.width = src_size->width;
    g_dpu_cfg.dpu_rect_grp_attr.right_image_size.height = src_size->height;
    g_dpu_cfg.dpu_rect_grp_attr.rect_mode = OT_DPU_RECT_MODE_DOUBLE;
    g_dpu_cfg.dpu_rect_grp_attr.depth = 0;
    g_dpu_cfg.dpu_rect_grp_attr.is_need_src_frame = TD_TRUE;
    g_dpu_cfg.dpu_rect_grp_attr.frame_rate.src_frame_rate = SAMPLE_SVP_DPU_NO_RATE_CTRL;
    g_dpu_cfg.dpu_rect_grp_attr.frame_rate.dst_frame_rate = SAMPLE_SVP_DPU_NO_RATE_CTRL;

    for (i = 0; i < OT_DPU_RECT_MAX_CHN_NUM; i++) {
        rect_chn_attr[i].image_size.width = dst_size->width;
        rect_chn_attr[i].image_size.height = dst_size->height;
    }

    ret = sample_common_svp_dpu_rect_start(g_dpu_cfg.dpu_rect_grp,
        &g_dpu_cfg.dpu_rect_grp_attr, rect_chn_attr, OT_DPU_RECT_MAX_CHN_NUM);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail_1, "start dpu rect failed!\n");

    return TD_SUCCESS;
fail_1:
    (td_void)sample_common_svp_dpu_rect_stop(g_dpu_cfg.dpu_rect_grp, g_dpu_cfg.dpu_rect_grp_attr.rect_mode);
fail_0:
    for (i = 0; i < rect_pipe_num; i++) {
        (td_void)sample_common_svp_dpu_rect_unload_lut(&g_dpu_cfg.dpu_rect_mem_info[i], g_dpu_cfg.lut_id[i]);
    }
    return ret;
}

static td_void sample_svp_dpu_uninit_dpu_match(td_void)
{
    (td_void)sample_common_svp_dpu_match_stop(g_dpu_cfg.dpu_match_grp);
    sample_svp_dpu_mmz_free(g_dpu_cfg.dpu_match_grp_attr.assist_buf.phys_addr,
        g_dpu_cfg.dpu_match_grp_attr.assist_buf.virt_addr);
    return;
}

static td_s32 sample_svp_dpu_start_dpu_match(td_u32 width, td_u32 height)
{
    td_s32 ret;
    td_u32 size;
    ot_dpu_match_chn_attr match_chn_attr = {0};

    ret = ss_mpi_dpu_match_get_assist_buf_size(SAMPLE_SVP_DPU_DISPARITY_NUM, height, &size);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret, "get dpu match assist buffer size failed!\n");

    ret = sample_common_svp_dpu_match_create_mem_info(&g_dpu_cfg.dpu_match_grp_attr.assist_buf,
        "sampe_dpu_match_assist_buf", TD_NULL, size);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret, "create dpu match assist buffer failed!\n");

    g_dpu_cfg.dpu_match_grp = 0;
    g_dpu_cfg.dpu_match_grp_attr.disparity_num = SAMPLE_SVP_DPU_DISPARITY_NUM;
    g_dpu_cfg.dpu_match_grp_attr.disparity_start_pos = 0;
    g_dpu_cfg.dpu_match_grp_attr.depth = 1;
    g_dpu_cfg.dpu_match_grp_attr.left_image_size.width = width;
    g_dpu_cfg.dpu_match_grp_attr.left_image_size.height = height;
    g_dpu_cfg.dpu_match_grp_attr.right_image_size.width = width;
    g_dpu_cfg.dpu_match_grp_attr.right_image_size.height = height;
    g_dpu_cfg.dpu_match_grp_attr.density_accuracy_mode = OT_DPU_MATCH_DENSITY_ACCURACY_MODE_D9_A0;
    g_dpu_cfg.dpu_match_grp_attr.disparity_subpixel_en = OT_DPU_MATCH_DISPARITY_SUBPIXEL_ENABLE;
    g_dpu_cfg.dpu_match_grp_attr.match_mask_mode = OT_DPU_MATCH_MASK_9X9_MODE;
    g_dpu_cfg.dpu_match_grp_attr.speed_accuracy_mode = OT_DPU_MATCH_SPEED_ACCURACY_MODE_ACCURACY;
    g_dpu_cfg.dpu_match_grp_attr.is_need_src_frame = TD_TRUE;
    g_dpu_cfg.dpu_match_grp_attr.frame_rate.src_frame_rate = SAMPLE_SVP_DPU_NO_RATE_CTRL;
    g_dpu_cfg.dpu_match_grp_attr.frame_rate.dst_frame_rate = SAMPLE_SVP_DPU_NO_RATE_CTRL;

    match_chn_attr.image_size.width = width;
    match_chn_attr.image_size.height = height;
    ret = sample_common_svp_dpu_match_start(g_dpu_cfg.dpu_match_grp,
        &g_dpu_cfg.dpu_match_grp_attr, &match_chn_attr);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail, "dpu match start failed!\n");
    return TD_SUCCESS;
fail:
    (td_void)sample_common_svp_dpu_match_stop(g_dpu_cfg.dpu_match_grp);
    sample_svp_dpu_mmz_free(g_dpu_cfg.dpu_match_grp_attr.assist_buf.phys_addr,
        g_dpu_cfg.dpu_match_grp_attr.assist_buf.virt_addr);
    return ret;
}

static td_s32 sample_svp_dpu_create_pool(const ot_size *pic_size)
{
    td_s32 ret;
    td_u32 i;
    td_u32 stride;
    td_s32 size;
    ot_vb_pool_cfg vb_pool_cfg;

    stride = sample_common_svp_dpu_calc_stride(pic_size->width, SAMPLE_SVP_DPU_ALIGN_16);
    size = stride * pic_size->height;
    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        ret = memset_s(&vb_pool_cfg, sizeof(ot_vb_pool_cfg), 0, sizeof(ot_vb_pool_cfg));
        sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail_0, "memset_s for vb_pool_cfg failed!\n");

        vb_pool_cfg.blk_size  = size;
        vb_pool_cfg.blk_cnt   = 1;
        vb_pool_cfg.remap_mode = OT_VB_REMAP_MODE_NONE;
        g_dpu_cfg.pipe_vb_pool[i] = ss_mpi_vb_create_pool(&vb_pool_cfg);
        sample_svp_dpu_check_exps_goto(g_dpu_cfg.pipe_vb_pool[i] == OT_VB_INVALID_POOL_ID,
            fail_0, "create vb pool failed!\n");

        g_dpu_cfg.vb_blk[i] = ss_mpi_vb_get_blk(g_dpu_cfg.pipe_vb_pool[i], size, TD_NULL);
        sample_svp_dpu_check_exps_goto(g_dpu_cfg.vb_blk[i] == OT_VB_INVALID_HANDLE,
            fail_1, "ss_mpi_vb_get_blk failed!\n");
    }
    return TD_SUCCESS;
fail_1:
    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        ret = ss_mpi_vb_release_blk(g_dpu_cfg.vb_blk[i]);
        g_dpu_cfg.vb_blk[i] = OT_VB_INVALID_HANDLE;
    }
fail_0:
    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_vb_pool[i] != OT_VB_INVALID_POOL_ID) {
            (td_void)ss_mpi_vb_destroy_pool(g_dpu_cfg.pipe_vb_pool[i]);
        }
    }
    return ret;
}

static td_s32 sample_svp_dpu_sys_init_file_case(ot_size *pic_size, td_u32 pic_size_num)
{
    td_s32 ret;
    ot_pic_size pic_size_type = PIC_720P;
    ot_vb_cfg vb_cfg;
    ot_pic_buf_attr pic_buf_attr;
    g_stop_signal = TD_FALSE;

    ret = memset_s(&g_dpu_cfg, sizeof(ot_sample_svp_dpu_cfg), 0, sizeof(ot_sample_svp_dpu_cfg));
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, OT_ERR_DPU_RECT_ILLEGAL_PARAM,
        "memset_s for dpu_cfg failed!\n");

    sample_svp_dpu_check_exps_return(pic_size_num < SAMPLE_SVP_DPU_CHN_NUM, TD_FAILURE, "pic_size_num is illegal!\n");
    ret = sample_comm_sys_get_pic_size(pic_size_type, &pic_size[0]);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret, "sys get pic size failed!\n");

    ret = memset_s(&vb_cfg, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg));
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret, "memset_s for vb_cfg failed!\n");

    sample_svp_dpu_set_pic_buf_attr(pic_size[0].width, pic_size[0].height,
        OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420, &pic_buf_attr);

    vb_cfg.max_pool_cnt             = SAMPLE_SVP_DPU_MAX_POOL_CNT;
    vb_cfg.common_pool[0].blk_cnt  = SAMPLE_SVP_DPU_BLK_CNT_10;
    vb_cfg.common_pool[0].blk_size = ot_common_get_pic_buf_size(&pic_buf_attr);

    pic_size[1].width = SAMPLE_SVP_DPU_DST_WIDTH;
    pic_size[1].height = SAMPLE_SVP_DPU_DST_HEIGHT;
    sample_svp_dpu_set_pic_buf_attr(pic_size[1].width, pic_size[1].height,
        OT_PIXEL_FORMAT_S16C1, &pic_buf_attr);
    vb_cfg.common_pool[1].blk_cnt  = SAMPLE_SVP_DPU_BLK_CNT_8;
    vb_cfg.common_pool[1].blk_size = ot_common_get_pic_buf_size(&pic_buf_attr);

    ret = sample_comm_sys_init(&vb_cfg);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail, "init sys failed failed!\n");

    ret = sample_svp_dpu_create_pool(&pic_size[0]);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail, "sample_svp_dpu_create_pool failed!\n");
    return ret;
fail:
    sample_comm_sys_exit();
    return ret;
}

static td_void sample_svp_dpu_sys_uninit_file_case(td_void)
{
    td_u32 i;

    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        (td_void)ss_mpi_vb_release_blk(g_dpu_cfg.vb_blk[i]);
        g_dpu_cfg.vb_blk[i] = OT_VB_INVALID_HANDLE;
    }
    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_vb_pool[i] != OT_VB_INVALID_POOL_ID) {
            (td_void)ss_mpi_vb_destroy_pool(g_dpu_cfg.pipe_vb_pool[i]);
        }
    }
    sample_comm_sys_exit();
    return;
}

static td_s32 sample_svp_dpu_open_file(td_char *src_file_name[OT_DPU_RECT_MAX_PIPE_NUM],
    td_u32 src_file_num, const td_char *match_file_name)
{
    td_u32 i;
    td_char path[PATH_MAX] = {0};

    for (i = 0; i < src_file_num; i++) {
        sample_svp_dpu_check_exps_return((strlen(src_file_name[i]) > PATH_MAX) ||
            (realpath(src_file_name[i], path) == TD_NULL),
            OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Error, file_name is invalid!\n");
        g_dpu_cfg.pipe_file[i] = fopen(path, "rb");
        sample_svp_dpu_check_exps_goto(g_dpu_cfg.pipe_file[i] == TD_NULL,
            fail, "fopen file %s failed!\n", src_file_name[i]);
    }

    g_dpu_cfg.match_file = fopen(match_file_name, "wb");
    sample_svp_dpu_check_exps_goto(g_dpu_cfg.match_file == TD_NULL,
        fail, "fopen file %s failed!\n", match_file_name);

    return TD_SUCCESS;
fail:
    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_file[i] != TD_NULL) {
            fclose(g_dpu_cfg.pipe_file[i]);
            g_dpu_cfg.pipe_file[i] = TD_NULL;
        }
    }
    return OT_ERR_DPU_RECT_ILLEGAL_PARAM;
}

static void sample_svp_dpu_close_file(td_void)
{
    td_u32 i;

    if (g_dpu_cfg.match_file != TD_NULL) {
        fclose(g_dpu_cfg.match_file);
        g_dpu_cfg.match_file = TD_NULL;
    }
    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_file[i] != TD_NULL) {
            fclose(g_dpu_cfg.pipe_file[i]);
            g_dpu_cfg.pipe_file[i] = TD_NULL;
        }
    }
    return;
}

static td_void sample_svp_dpu_file_rect_match_stop(td_void)
{
    td_u32 i;
    sample_svp_dpu_close_file();

    (td_void)sample_common_svp_dpu_rect_unbind_match(g_dpu_cfg.dpu_rect_grp, g_dpu_cfg.dpu_match_grp);
    (td_void)sample_common_svp_dpu_match_stop(g_dpu_cfg.dpu_match_grp);
    sample_svp_dpu_mmz_free(g_dpu_cfg.dpu_match_grp_attr.assist_buf.phys_addr,
        g_dpu_cfg.dpu_match_grp_attr.assist_buf.virt_addr);
    (td_void)sample_common_svp_dpu_rect_stop(g_dpu_cfg.dpu_rect_grp, g_dpu_cfg.dpu_rect_grp_attr.rect_mode);

    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        (td_void)sample_common_svp_dpu_rect_unload_lut(&g_dpu_cfg.dpu_rect_mem_info[i], g_dpu_cfg.lut_id[i]);
    }

    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        (td_void)ss_mpi_vb_release_blk(g_dpu_cfg.vb_blk[i]);
        g_dpu_cfg.vb_blk[i] = OT_VB_INVALID_HANDLE;
    }

    for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
        if (g_dpu_cfg.pipe_vb_pool[i] != OT_VB_INVALID_POOL_ID) {
            (td_void)ss_mpi_vb_destroy_pool(g_dpu_cfg.pipe_vb_pool[i]);
        }
    }
    g_stop_signal = TD_FALSE;
    sample_comm_sys_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    return;
}

static td_s32 sample_svp_dpu_proc_frame_from_file(const ot_size *pic_size)
{
    td_s32 ret;
    td_u32 stride;
    td_u32 i;
    td_u32 frame_num;
    td_s32 milli_sec = SAMPLE_SVP_DPU_TIME_OUT;
    ot_video_frame_info dpu_rect_src_frame[OT_DPU_RECT_MAX_PIPE_NUM] = {0};

    stride = sample_common_svp_dpu_calc_stride(pic_size->width, SAMPLE_SVP_DPU_ALIGN_16);
    for (frame_num = 0; frame_num < SAMPLE_SVP_DPU_FRAME_NUM && g_stop_signal == TD_FALSE; frame_num++) {
        for (i = 0; i < OT_DPU_RECT_MAX_PIPE_NUM; i++) {
            ret = sample_common_svp_dpu_rect_get_frame_from_file(g_dpu_cfg.pipe_file[i], pic_size,
                stride, &dpu_rect_src_frame[i], g_dpu_cfg.vb_blk[i]);
            sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret, "get frame failed!\n");
        }
        ret = ss_mpi_dpu_rect_send_frame(g_dpu_cfg.dpu_rect_grp, &dpu_rect_src_frame[OT_DPU_RECT_LEFT_PIPE],
            &dpu_rect_src_frame[OT_DPU_RECT_RIGHT_PIPE], milli_sec);
        sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret, "ss_mpi_dpu_rect_send_frame failed!\n");
       /* get match frame from DPU MATCH */
        milli_sec = SAMPLE_SVP_DPU_BLOCK;
        ret = ss_mpi_dpu_match_get_frame(g_dpu_cfg.dpu_match_grp, milli_sec,
            &g_dpu_cfg.dpu_match_src_frame, &g_dpu_cfg.dpu_match_dst_frame);
        sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret, "get dpu match frame failed!\n");
        /* use match frame to do something */
        ret = sample_common_svp_dpu_write_frame_to_file(g_dpu_cfg.match_file,
            sizeof(td_u16), &g_dpu_cfg.dpu_match_dst_frame);
        sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail,
            "sample_common_svp_dpu_write_frame_to_file failed!\n");
        ret = ss_mpi_dpu_match_release_frame(g_dpu_cfg.dpu_match_grp,
            &g_dpu_cfg.dpu_match_src_frame, &g_dpu_cfg.dpu_match_dst_frame);
        sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail, "release dpu match frame failed!\n");
    }
    sample_svp_dpu_trace_info("Process success!\n");
    return TD_SUCCESS;
fail:
    (td_void)ss_mpi_dpu_match_release_frame(g_dpu_cfg.dpu_match_grp,
        &g_dpu_cfg.dpu_match_src_frame, &g_dpu_cfg.dpu_match_dst_frame);
    return ret;
}

/* This case only for function design reference */
td_s32 sample_svp_dpu_file_rect_match(td_void)
{
    td_s32 ret;
    ot_size pic_size[SAMPLE_SVP_DPU_CHN_NUM];
    td_char *lut_file_name[OT_DPU_RECT_MAX_PIPE_NUM] = {
        "./data/input/lut/1050x560_LeftMap.dat", "./data/input/lut/1050x560_RightMap.dat"
    };
    td_char *src_file_name[OT_DPU_RECT_MAX_PIPE_NUM] = {
        "./data/input/src/1280x720_LeftSrc.yuv", "./data/input/src/1280x720_RightSrc.yuv"
    };
    const td_char *match_file_name = "./data/output/1050x560_sp400.yuv";

    /*  init SYS, init common VB(for DPU RECT and DPU MATCH) */
    ret = sample_svp_dpu_sys_init_file_case(pic_size, SAMPLE_SVP_DPU_CHN_NUM);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret, "sys_init in file case failed!\n");
    if (g_stop_signal == TD_TRUE) {
        sample_svp_dpu_file_rect_match_stop();
        return TD_FAILURE;
    }
    /*  start DPU RECT */
    ret = sample_svp_dpu_start_dpu_rect(&pic_size[0], &pic_size[1], lut_file_name, OT_DPU_RECT_MAX_PIPE_NUM);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail_0, "start dpu rect failed!\n");
    if (g_stop_signal == TD_TRUE) {
        sample_svp_dpu_file_rect_match_stop();
        return TD_FAILURE;
    }

    ret = sample_svp_dpu_start_dpu_match(pic_size[1].width, pic_size[1].height);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail_1, "dpu match start failed!\n");
    if (g_stop_signal == TD_TRUE) {
        sample_svp_dpu_file_rect_match_stop();
        return TD_FAILURE;
    }
    /* bind DPU RECT to DPU MATCH */
    ret = sample_common_svp_dpu_rect_bind_match(g_dpu_cfg.dpu_rect_grp, g_dpu_cfg.dpu_match_grp);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail_2, "dpu rect bind match failed!\n");
    /* step5:  send frame to DPU RECT */
    ret = sample_svp_dpu_open_file(src_file_name, OT_DPU_RECT_MAX_PIPE_NUM, match_file_name);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail_3, "fopen file failed!\n");
    ret = sample_svp_dpu_proc_frame_from_file(&pic_size[0]);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail_3, "release dpu match frame failed!\n");

fail_3:
    sample_svp_dpu_close_file();
    (td_void)sample_common_svp_dpu_rect_unbind_match(g_dpu_cfg.dpu_rect_grp, g_dpu_cfg.dpu_match_grp);
fail_2:
    sample_svp_dpu_uninit_dpu_match();
fail_1:
    sample_svp_dpu_uninit_dpu_rect();
fail_0:
    sample_svp_dpu_sys_uninit_file_case();
    return ret;
}

td_void sample_svp_dpu_file_rect_match_handle_sig(td_void)
{
    g_stop_signal = TD_TRUE;
}

/* This case only for function design reference */
td_s32 sample_svp_dpu_vi_vpss_rect_match(td_void)
{
    return TD_SUCCESS;
}

td_void sample_svp_dpu_vi_vpss_rect_match_handle_sig(td_void)
{
    return;
}