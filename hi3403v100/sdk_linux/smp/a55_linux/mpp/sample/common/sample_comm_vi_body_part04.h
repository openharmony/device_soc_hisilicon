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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART04_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART04_H_
    }
    ret = sample_comm_vi_start_chn(vi_pipe, pipe_info->chn_info, pipe_info->chn_num);
    if (ret != TD_SUCCESS) {
        sample_print("vi pipe(%d) start chn failed!\n", vi_pipe);
        goto start_chn_failed;
    }
    return TD_SUCCESS;

start_chn_failed:
    ss_mpi_vi_stop_pipe(vi_pipe);
start_pipe_failed:
    ss_mpi_vi_destroy_pipe(vi_pipe);
    return TD_FAILURE;
}
static td_s32 sample_comm_vi_mode_switch_start_pipe_chn(const ot_vi_bind_pipe *bind_pipe,
                                                        const sample_vi_pipe_info pipe_info[])
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < (td_s32)bind_pipe->pipe_num; i++) {
        ot_vi_pipe vi_pipe = bind_pipe->pipe_id[i];
        ret = sample_comm_vi_mode_switch_start_one_pipe_chn(vi_pipe, &pipe_info[i]);
        if (ret != TD_SUCCESS) {
            goto exit;
        }
    }
    return TD_SUCCESS;

exit:
    for (i = i - 1; i >= 0; i--) {
        ot_vi_pipe vi_pipe = bind_pipe->pipe_id[i];
        sample_comm_vi_stop_one_pipe(vi_pipe, &pipe_info[i]);
    }
    return TD_FAILURE;
}
static td_void sample_comoon_vi_query_isp_inner_state_info(ot_vi_pipe vi_pipe, td_bool switch_wdr)
{
    ot_isp_inner_state_info inner_state_info;
    td_bool switch_finish = TD_FALSE;
    td_s32 i;
    const td_u32 dev_num = 1;

    while (switch_finish != TD_TRUE) {
        switch_finish = TD_TRUE;
        for (i = 0; i < dev_num; i++) {
            ss_mpi_isp_query_inner_state_info(vi_pipe, &inner_state_info);
            if (switch_wdr == TD_TRUE) {
                switch_finish &= inner_state_info.wdr_switch_finish;
            } else {
                switch_finish &= inner_state_info.res_switch_finish;
            }
        }
        if (switch_finish == TD_TRUE) {
            sample_print("switch finish !\n");
            continue;
        }
        ot_usleep(SLEEP_TIME);
    }
}
static td_bool sample_common_vi_check_need_pipe(ot_vi_pipe vi_pipe, ot_wdr_mode wdr_mode, td_u32 index)
{
    td_bool need_pipe = TD_FALSE;

    if (vi_pipe < 0 || vi_pipe >= OT_VI_MAX_PHYS_PIPE_NUM) {
        return need_pipe;
    }
    if (wdr_mode == OT_WDR_MODE_NONE) {
        need_pipe = TD_TRUE;
    } else {
        need_pipe = (index > 0) ? TD_FALSE : TD_TRUE;
    }
    return need_pipe;
}
td_s32 sample_comm_vi_switch_isp_mode(const sample_vi_cfg *vi_cfg)
{
    td_s32 i;
    td_s32 j;
    td_s32 ret;
    const td_s32  dev_num = 1;
    td_bool need_pipe;
    td_bool switch_wdr[OT_VI_MAX_PHYS_PIPE_NUM] = {TD_FALSE};
    ot_vi_pipe vi_pipe;
    ot_isp_pub_attr pub_attr;
    ot_isp_pub_attr pre_pub_attr;

    for (i = 0; i < dev_num; i++) {
        for (j = 0; j < vi_cfg->bind_pipe.pipe_num; j++) {
            vi_pipe = vi_cfg->bind_pipe.pipe_id[j];
            need_pipe = sample_common_vi_check_need_pipe(vi_pipe,
                vi_cfg->pipe_info[j].isp_info.isp_pub_attr.wdr_mode, j);
            if (TD_TRUE != need_pipe) {
                continue;
            }
            sample_comm_isp_get_pub_attr_by_sns(vi_cfg->sns_info.sns_type, &pub_attr);
            pub_attr.wdr_mode = vi_cfg->pipe_info[j].isp_info.isp_pub_attr.wdr_mode;

            ret = ss_mpi_isp_get_pub_attr(vi_pipe, &pre_pub_attr);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_isp_get_pub_attr failed!\n");
                sample_comm_vi_stop_isp(vi_cfg);
            }
            ret = ss_mpi_isp_set_pub_attr(vi_pipe, &pub_attr);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_isp_set_pub_attr failed!\n");
                sample_comm_vi_stop_isp(vi_cfg);
            }
            if (pre_pub_attr.wdr_mode != pub_attr.wdr_mode) {
                switch_wdr[vi_pipe] = TD_TRUE;
            }
        }
    }
    vi_pipe = vi_cfg->bind_pipe.pipe_id[0];
    sample_comoon_vi_query_isp_inner_state_info(vi_pipe, switch_wdr[vi_pipe]);

    for (i = 0; i < dev_num; i++) {
        ret = sample_comm_vi_mode_switch_start_pipe_chn(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
        if (ret != TD_SUCCESS) {
            sample_print("set grp info failed!\n");
            goto start_pipe_failed;
        }
        return TD_SUCCESS;

        start_pipe_failed: /* fall through */
        sample_comm_vi_dev_unbind_pipe(vi_cfg->dev_info.vi_dev, &vi_cfg->bind_pipe);
    }
    return TD_SUCCESS;
}
static td_void sample_comm_vi_update_isp_resolution(const sample_vi_cfg *vi_cfg, const ot_size *size,
    td_bool *switch_wdr)
{
    td_s32 j;
    td_s32 ret;
    td_bool need_pipe;
    ot_vi_pipe vi_pipe;
    ot_isp_pub_attr pub_attr;
    ot_isp_pub_attr pre_pub_attr;

    for (j = 0; j < vi_cfg->bind_pipe.pipe_num; j++) {
        vi_pipe = vi_cfg->bind_pipe.pipe_id[j];
        need_pipe = sample_common_vi_check_need_pipe(vi_pipe,
            vi_cfg->pipe_info[j].isp_info.isp_pub_attr.wdr_mode, j);
        if (TD_TRUE != need_pipe) {
            continue;
        }
        sample_comm_isp_get_pub_attr_by_sns(vi_cfg->sns_info.sns_type, &pub_attr);
        pub_attr.wdr_mode = vi_cfg->pipe_info[j].isp_info.isp_pub_attr.wdr_mode;
        pub_attr.wnd_rect.width = size->width;
        pub_attr.wnd_rect.height = size->height;
        ret = ss_mpi_isp_get_pub_attr(vi_pipe, &pre_pub_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_isp_get_pub_attr failed!\n");
            sample_comm_vi_stop_isp(vi_cfg);
        }
        ret = ss_mpi_isp_set_pub_attr(vi_pipe, &pub_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_isp_set_pub_attr failed!\n");
            sample_comm_vi_stop_isp(vi_cfg);
        }
        if (pre_pub_attr.wdr_mode != pub_attr.wdr_mode) {
            switch_wdr[vi_pipe] = TD_TRUE;
        }
    }
}
td_s32 sample_comm_vi_switch_isp_resolution(const sample_vi_cfg *vi_cfg, const ot_size *size)
{
    td_s32 i;
    td_s32 ret;
    const td_s32  dev_num = 1;
    td_bool switch_wdr[OT_VI_MAX_PHYS_PIPE_NUM] = {TD_FALSE};
    ot_vi_pipe vi_pipe;

    for (i = 0; i < dev_num; i++) {
        sample_comm_vi_update_isp_resolution(vi_cfg, size, switch_wdr);
    }
    vi_pipe = vi_cfg->bind_pipe.pipe_id[0];
    sample_comoon_vi_query_isp_inner_state_info(vi_pipe, switch_wdr[vi_pipe]);

    for (i = 0; i < dev_num; i++) {
        ret = sample_comm_vi_mode_switch_start_pipe_chn(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
        if (ret != TD_SUCCESS) {
            sample_print("set grp info failed!\n");
            goto start_pipe_failed;
        }
        return TD_SUCCESS;

        start_pipe_failed: /* fall through */
        sample_comm_vi_dev_unbind_pipe(vi_cfg->dev_info.vi_dev, &vi_cfg->bind_pipe);
    }
    return TD_SUCCESS;
}
/* use this func to exit vi when start 4 route of vi */
td_void sample_comm_vi_stop_four_vi(const sample_vi_cfg *vi_cfg, td_s32 route_num)
{
    td_s32 i;
    ot_vi_dev vi_dev;

    for (i = 0; i < route_num; i++) {
        vi_dev = vi_cfg[i].dev_info.vi_dev;
        sample_comm_vi_stop_isp(&vi_cfg[i]);
        sample_comm_vi_stop_pipe(&vi_cfg[i].bind_pipe, vi_cfg[i].pipe_info);
        sample_comm_vi_dev_unbind_pipe(vi_dev, &vi_cfg[i].bind_pipe);
        sample_comm_vi_stop_dev(vi_dev);
    }
    for (i = 0; i < route_num; i++) {
        sample_comm_vi_stop_mipi_rx(&vi_cfg[i].sns_info, &vi_cfg[i].mipi_info);
    }
}
static td_void sample_comm_vi_get_vb_calc_cfg(sample_vi_get_frame_vb_cfg *get_frame_vb_cfg, ot_vb_calc_cfg *calc_cfg)
{
    ot_pic_buf_attr buf_attr;

    buf_attr.width         = get_frame_vb_cfg->size.width;
    buf_attr.height        = get_frame_vb_cfg->size.height;
    buf_attr.align         = OT_DEFAULT_ALIGN;
    buf_attr.bit_width     =
        (get_frame_vb_cfg->dynamic_range == OT_DYNAMIC_RANGE_SDR8) ? OT_DATA_BIT_WIDTH_8 : OT_DATA_BIT_WIDTH_10;
    buf_attr.pixel_format  = get_frame_vb_cfg->pixel_format;
    buf_attr.compress_mode = get_frame_vb_cfg->compress_mode;

    ot_common_get_pic_buf_cfg(&buf_attr, calc_cfg);
}
static td_s32 sample_comm_vi_malloc_frame_blk(ot_vb_pool pool_id,
                                              sample_vi_get_frame_vb_cfg *get_frame_vb_cfg, ot_vb_calc_cfg *calc_cfg,
                                              sample_vi_user_frame_info *user_frame_info)
{
    ot_vb_blk vb_blk;
    td_phys_addr_t phys_addr;
    td_void *virt_addr = TD_NULL;
    ot_video_frame_info *frame_info = TD_NULL;

    vb_blk = ss_mpi_vb_get_blk(pool_id, calc_cfg->vb_size, TD_NULL);
    if (vb_blk == OT_VB_INVALID_HANDLE) {
        sample_print("ss_mpi_vb_get_blk err, size:%d\n", calc_cfg->vb_size);
        return TD_FAILURE;
    }
    phys_addr = ss_mpi_vb_handle_to_phys_addr(vb_blk);
    virt_addr = (td_u8 *)ss_mpi_sys_mmap(phys_addr, calc_cfg->vb_size);
    if (virt_addr == TD_NULL) {
        sample_print("ss_mpi_sys_mmap err!\n");
        ss_mpi_vb_release_blk(vb_blk);
        return TD_FAILURE;
    }
    user_frame_info->vb_blk   = vb_blk;
    user_frame_info->blk_size = calc_cfg->vb_size;

    frame_info = &user_frame_info->frame_info;

    frame_info->pool_id                   = pool_id;
    frame_info->mod_id                    = OT_ID_VI;
    frame_info->video_frame.phys_addr[0]  = phys_addr;
    frame_info->video_frame.phys_addr[1]  = frame_info->video_frame.phys_addr[0] + calc_cfg->main_y_size;
    frame_info->video_frame.virt_addr[0]  = virt_addr;
    frame_info->video_frame.virt_addr[1]  = frame_info->video_frame.virt_addr[0] + calc_cfg->main_y_size;
    frame_info->video_frame.stride[0]     = calc_cfg->main_stride;
    frame_info->video_frame.stride[1]     = calc_cfg->main_stride;
    frame_info->video_frame.width         = get_frame_vb_cfg->size.width;
    frame_info->video_frame.height        = get_frame_vb_cfg->size.height;
    frame_info->video_frame.pixel_format  = get_frame_vb_cfg->pixel_format;
    frame_info->video_frame.video_format  = get_frame_vb_cfg->video_format;
    frame_info->video_frame.compress_mode = get_frame_vb_cfg->compress_mode;
    frame_info->video_frame.dynamic_range = get_frame_vb_cfg->dynamic_range;
    frame_info->video_frame.field         = OT_VIDEO_FIELD_FRAME;
    frame_info->video_frame.color_gamut   = OT_COLOR_GAMUT_BT601;

    return TD_SUCCESS;
}
td_void sample_comm_vi_free_frame_blk(sample_vi_user_frame_info *user_frame_info)
{
    td_s32 ret;
    ot_vb_blk vb_blk = user_frame_info->vb_blk;
    td_u32 blk_size = user_frame_info->blk_size;
    td_void *virt_addr = user_frame_info->frame_info.video_frame.virt_addr[0];

    ret = ss_mpi_sys_munmap(virt_addr, blk_size);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_sys_munmap failure!\n");
    }
    ret = ss_mpi_vb_release_blk(vb_blk);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vb_release_blk block 0x%x failure\n", vb_blk);
    }
    user_frame_info->vb_blk = OT_VB_INVALID_HANDLE;
}
td_s32 sample_comm_vi_get_frame_blk(sample_vi_get_frame_vb_cfg *get_frame_vb_cfg,
                                    sample_vi_user_frame_info user_frame_info[], td_s32 frame_cnt)
{
    td_s32 ret;
    td_s32 i;
    ot_vb_pool pool_id;
    ot_vb_calc_cfg calc_cfg = {0};
    ot_vb_pool_cfg vb_pool_cfg = {0};

    sample_comm_vi_get_vb_calc_cfg(get_frame_vb_cfg, &calc_cfg);

    vb_pool_cfg.blk_size   = calc_cfg.vb_size;
    vb_pool_cfg.blk_cnt    = frame_cnt;
    vb_pool_cfg.remap_mode = OT_VB_REMAP_MODE_NONE;
    pool_id = ss_mpi_vb_create_pool(&vb_pool_cfg);
    if (pool_id == OT_VB_INVALID_POOL_ID) {
        sample_print("ss_mpi_vb_create_pool failed!\n");
        return TD_FAILURE;
    }
    for (i = 0; i < frame_cnt; i++) {
        ret = sample_comm_vi_malloc_frame_blk(pool_id, get_frame_vb_cfg, &calc_cfg, &user_frame_info[i]);
        if (ret != TD_SUCCESS) {
            goto exit;
        }
    }
    return TD_SUCCESS;

exit:
    for (i = i - 1; i >= 0; i--) {
        sample_comm_vi_free_frame_blk(&user_frame_info[i]);
    }
    ss_mpi_vb_destroy_pool(pool_id);
    return TD_FAILURE;
}
td_void sample_comm_vi_release_frame_blk(sample_vi_user_frame_info user_frame_info[], td_s32 frame_cnt)
{
    td_s32 i;
    ot_vb_pool pool_id;

    for (i = 0; i < frame_cnt; i++) {
        sample_comm_vi_free_frame_blk(&user_frame_info[i]);
    }
    pool_id = user_frame_info[0].frame_info.pool_id;
    ss_mpi_vb_destroy_pool(pool_id);
}
static td_s32 sample_comm_vi_get_fpn_frame_info(ot_vi_pipe vi_pipe,
                                                ot_pixel_format pixel_format, ot_compress_mode compress_mode,
                                                sample_vi_user_frame_info *user_frame_info, td_s32 blk_cnt)
{
    td_s32 ret;
    ot_vi_pipe_attr pipe_attr;
    sample_vi_get_frame_vb_cfg vb_cfg;

    ret = ss_mpi_vi_get_pipe_attr(vi_pipe, &pipe_attr);
    if (ret != TD_SUCCESS) {
        sample_print("vi get pipe attr failed!\n");
        return ret;
    }
    vb_cfg.size.width    = pipe_attr.size.width;
    vb_cfg.size.height   = pipe_attr.size.height;
    vb_cfg.pixel_format  = pixel_format;
    vb_cfg.video_format  = OT_VIDEO_FORMAT_LINEAR;
    vb_cfg.compress_mode = compress_mode;
    vb_cfg.dynamic_range = OT_DYNAMIC_RANGE_SDR8;

    ret = sample_comm_vi_get_frame_blk(&vb_cfg, user_frame_info, blk_cnt);
    if (ret != TD_SUCCESS) {
        sample_print("get fpn frame vb failed!\n");
        return ret;
    }
    return TD_SUCCESS;
}
static td_s32 sample_comm_vi_get_fpn_calibrate_frame_info(ot_vi_pipe vi_pipe, ot_pixel_format pixel_format,
                                                          ot_compress_mode compress_mode,
                                                          sample_vi_user_frame_info *user_frame_info, td_s32 blk_cnt)
{
    td_s32 ret;
    sample_vi_user_frame_info *last_frame_info = &user_frame_info[blk_cnt - 1];

    ret = sample_comm_vi_get_fpn_frame_info(vi_pipe, OT_PIXEL_FORMAT_RGB_BAYER_16BPP,
                                            compress_mode, user_frame_info, blk_cnt - 1);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    ret = sample_comm_vi_get_fpn_frame_info(vi_pipe, pixel_format, compress_mode, last_frame_info, 1);
    if (ret != TD_SUCCESS) {
        sample_comm_vi_release_frame_blk(user_frame_info, blk_cnt);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
static td_void sample_comm_vi_get_fpn_file_name(ot_video_frame *video_frame, td_char *file_name, td_u32 length)
{
    (td_void)snprintf_s(file_name, length, length - 1, "./FPN_frame_%dx%d_%dbit.raw",
                        video_frame->width, video_frame->height, ot_vi_get_raw_bit_width(video_frame->pixel_format));
}
static td_s32 sample_comm_vi_get_fpn_file_name_iso(ot_video_frame *video_frame, const td_char *dir_name,
                                                   td_char *file_name, td_u32 length, td_u32 iso)
{
    td_s32 err;
    err = snprintf_s(file_name, length, length - 1, "./%s/FPN_frame_%dx%d_%dbit_iso%d.raw",
                     dir_name, video_frame->width, video_frame->height,
                     ot_vi_get_raw_bit_width(video_frame->pixel_format), iso);
    if (err < 0) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART04_H_ */
