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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART06_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART06_H_

            *dst = *(temp + list);
            dst += 2; /* traverse 2 steps away to the next V-component data */
        }
        dst = chroma_data;
        dst += (row + 1) * luma_stride;
    }

    free(temp);

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_read_file_to_sp42_x(FILE *file, ot_video_frame *frame)
{
    td_u8 *luma = (td_u8*)(td_uintptr_t)frame->virt_addr[0];
    td_u8 *chroma = (td_u8*)(td_uintptr_t)frame->virt_addr[1];
    td_u32 luma_width = frame->width;
    td_u32 chroma_width = luma_width >> 1;
    td_u32 luma_height = frame->height;
    td_u32 chroma_height = luma_height;
    td_u32 luma_stride = frame->stride[0];

    td_u8 *dst = TD_NULL;
    td_u32 row;

    if (frame->video_format == OT_VIDEO_FORMAT_LINEAR) {
        /* Y */
        dst = luma;
        for (row = 0; row < luma_height; ++row) {
            (td_void)fread(dst, luma_width, 1, file);
            dst += luma_stride;
        }

        if (OT_PIXEL_FORMAT_YUV_400 == frame->pixel_format) {
            return TD_SUCCESS;
        } else if (OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420 == frame->pixel_format) {
            chroma_height = chroma_height >> 1;
        }
        if (sample_comm_vi_convert_chroma_planar_to_sp42x(
            file, chroma, luma_stride, chroma_width, chroma_height) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    } else {
        (td_void)fread(luma, luma_stride * luma_height * 3 / 2, 1,  file); /* Tile 64x16 size = stridexheight*3/2 */
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_get_user_pic_frame_info(ot_size *dst_size, sample_vi_user_frame_info *user_frame_info)
{
    td_s32 ret;
    sample_vi_get_frame_vb_cfg vb_cfg;

    vb_cfg.size.width    = dst_size->width;
    vb_cfg.size.height   = dst_size->height;
    vb_cfg.pixel_format  = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vb_cfg.video_format  = OT_VIDEO_FORMAT_LINEAR;
    vb_cfg.compress_mode = OT_COMPRESS_MODE_NONE;
    vb_cfg.dynamic_range = OT_DYNAMIC_RANGE_SDR8;

    ret = sample_comm_vi_get_frame_blk(&vb_cfg, user_frame_info, 1);
    if (ret != TD_SUCCESS) {
        sample_print("get user pic frame vb failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_add_scale_task(ot_video_frame_info *src_frame, ot_video_frame_info *dst_frame)
{
    td_s32 ret;
    ot_vgs_handle handle;
    ot_vgs_task_attr vgs_task_attr;

    ret = ss_mpi_vgs_begin_job(&handle);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vgs_begin_job failed, ret:0x%x", ret);
        return TD_FAILURE;
    }

    if (memcpy_s(&vgs_task_attr.img_in, sizeof(ot_video_frame_info),
        src_frame, sizeof(ot_video_frame_info)) != EOK) {
        sample_print("memcpy_s img_in failed\n");
        return TD_FAILURE;
    }

    if (memcpy_s(&vgs_task_attr.img_out, sizeof(ot_video_frame_info),
        dst_frame, sizeof(ot_video_frame_info)) != EOK) {
        sample_print("memcpy_s img_out failed\n");
        return TD_FAILURE;
    }

    if (ss_mpi_vgs_add_scale_task(handle, &vgs_task_attr, OT_VGS_SCALE_COEF_NORM) != TD_SUCCESS) {
        sample_print("ss_mpi_vgs_add_scale_task failed\n");
        return TD_FAILURE;
    }

    ret = ss_mpi_vgs_end_job(handle);
    if (ret != TD_SUCCESS) {
        ss_mpi_vgs_cancel_job(handle);
        sample_print("ss_mpi_vgs_end_job failed, ret:0x%x", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_read_user_frame_file(ot_vi_pipe vi_pipe, sample_vi_user_frame_info *user_frame_info)
{
    td_s32 ret;
    FILE *pfd = TD_NULL;
    const td_char *frame_file = "./UsePic_3840x2160_sp420.yuv";
    ot_size frame_size = {WIDTH_3840, HEIGHT_2160};
    sample_vi_user_frame_info pic_frame_info;

    ret = sample_comm_vi_get_user_pic_frame_info(&frame_size, &pic_frame_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    pfd = fopen(frame_file, "rb");
    if (pfd == TD_NULL) {
        sample_print("open file \"%s\" failed!\n", frame_file);
        ret = TD_FAILURE;
        goto exit0;
    }

    ret = sample_comm_vi_read_file_to_sp42_x(pfd, &pic_frame_info.frame_info.video_frame);
    if (ret != TD_SUCCESS) {
        goto exit1;
    }

    (td_void)fflush(pfd);

    ret = sample_comm_vi_add_scale_task(&pic_frame_info.frame_info, &user_frame_info->frame_info);
    if (ret != TD_SUCCESS) {
        sample_print("add vgs scale task failed.\n");
    }

exit1:
    (td_void)fclose(pfd);
exit0:
    sample_comm_vi_release_frame_blk(&pic_frame_info, 1);
    return ret;
}

static td_s32 sample_comm_vi_add_coverex_task(ot_video_frame_info *dst_frame)
{
    td_s32 ret;
    ot_vgs_handle handle;
    ot_vgs_task_attr vgs_task_attr;
    ot_cover cover;

    ret = ss_mpi_vgs_begin_job(&handle);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vgs_begin_job failed, ret:0x%x", ret);
        return TD_FAILURE;
    }

    if (memcpy_s(&vgs_task_attr.img_in, sizeof(ot_video_frame_info),
        dst_frame, sizeof(ot_video_frame_info)) != EOK) {
        sample_print("memcpy_s img_in failed\n");
        return TD_FAILURE;
    }

    if (memcpy_s(&vgs_task_attr.img_out, sizeof(ot_video_frame_info),
        dst_frame, sizeof(ot_video_frame_info)) != EOK) {
        sample_print("memcpy_s img_out failed\n");
        return TD_FAILURE;
    }

    cover.type = OT_COVER_RECT;
    cover.rect.x = 0;
    cover.rect.y = 0;
    cover.rect.width = dst_frame->video_frame.width;
    cover.rect.height = dst_frame->video_frame.height;
    cover.color = 0xFF0000;
    if (ss_mpi_vgs_add_cover_task(handle, &vgs_task_attr, &cover, 1) != TD_SUCCESS) {
        sample_print("ss_mpi_vgs_add_scale_task failed\n");
        return TD_FAILURE;
    }

    ret = ss_mpi_vgs_end_job(handle);
    if (ret != TD_SUCCESS) {
        ss_mpi_vgs_cancel_job(handle);
        sample_print("ss_mpi_vgs_end_job failed, ret:0x%x", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_common_vi_load_user_pic(ot_vi_pipe vi_pipe, sample_vi_user_pic_type user_pic_type,
    sample_vi_user_frame_info *user_frame_info)
{
    td_s32 ret;
    ot_vi_pipe_attr pipe_attr;

    ret = ss_mpi_vi_get_pipe_attr(vi_pipe, &pipe_attr);
    if (ret != TD_SUCCESS) {
        sample_print("vi get pipe attr failed!\n");
        return ret;
    }

    ret = sample_comm_vi_get_user_pic_frame_info(&pipe_attr.size, user_frame_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (user_pic_type == VI_USER_PIC_FRAME) {
        ret = sample_comm_vi_read_user_frame_file(vi_pipe, user_frame_info);
    } else {
        ret = sample_comm_vi_add_coverex_task(&user_frame_info->frame_info);
    }

    if (ret != TD_SUCCESS) {
        sample_comm_vi_release_frame_blk(user_frame_info, 1);
        return ret;
    }

    return TD_SUCCESS;
}

td_void sample_common_vi_unload_user_pic(sample_vi_user_frame_info *user_frame_info)
{
    sample_comm_vi_release_frame_blk(user_frame_info, 1);
}

#define WDR_MAX_PTS_DIFF 25000
static td_bool sample_vi_is_frame_pts_suitable(td_u64 pts1, td_u64 pts2)
{
    td_s64 pts_diff;

    pts_diff = pts1 - pts2;
    pts_diff = ((pts_diff >= 0) ? pts_diff : (-pts_diff));

    if (pts_diff <= WDR_MAX_PTS_DIFF) {
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
}

static td_s32 sample_vi_match_wdr_pts(ot_video_frame_info frame_info[], ot_vi_pipe vi_pipe[], td_s32 pipe_num)
{
    td_s32 cnt = 0;
    td_s32 try_time = 5; /* try 5 times */
    const td_s32 millsec = 1000; /* millsec: 1000ms */
    td_s32 ret;
    td_u64 pts_max;
    td_u64 pts_min;
    td_s32 min_id;
    td_s32 i;

    while (cnt++ < try_time) {
        pts_max = frame_info[0].video_frame.pts;
        pts_min = frame_info[0].video_frame.pts;
        min_id = 0;
        for (i = 1; i < pipe_num; i++) {
            pts_max = pts_max > frame_info[i].video_frame.pts ? pts_max : frame_info[i].video_frame.pts;
            pts_min = pts_min < frame_info[i].video_frame.pts ? pts_min : frame_info[i].video_frame.pts;
            if (pts_min == frame_info[i].video_frame.pts) {
                min_id = i;
            }
        }
        if (sample_vi_is_frame_pts_suitable(pts_max, pts_min) == TD_TRUE) {
            return TD_SUCCESS;
        }

        (td_void)ss_mpi_vi_release_pipe_frame(vi_pipe[min_id], &frame_info[min_id]);
        ret = ss_mpi_vi_get_pipe_frame(vi_pipe[min_id], &frame_info[min_id], millsec);
        if (ret != TD_SUCCESS) {
            printf("repeated get pipe[%d] frame failed\n", vi_pipe[min_id]);
            return TD_FAILURE;
        }
    }

    return TD_FAILURE;
}

static td_s32 sample_vi_send_pipe_wdr_frame(ot_vi_pipe send_pipe, ot_video_frame_info frame_info[],
                                            td_s32 pipe_num, td_s32 millsec)
{
    const ot_video_frame_info *wdr_frame_info[OT_VI_MAX_WDR_FRAME_NUM] = {TD_NULL};
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < pipe_num; i++) {
        wdr_frame_info[i] = &frame_info[i];
    }
    ret = ss_mpi_isp_run_once(send_pipe);
    if (ret != TD_SUCCESS) {
        printf("pipe[%d] isp runonce failed\n", send_pipe);
        return ret;
    }
    ret = ss_mpi_vi_send_pipe_raw(send_pipe, wdr_frame_info, pipe_num, millsec);
    if (ret != TD_SUCCESS) {
        printf("pipe[%d] send frame failed\n", send_pipe);
        return ret;
    }
    ret = ss_mpi_isp_get_vd_time_out(send_pipe, OT_ISP_VD_BE_END, millsec);
    if (ret != TD_SUCCESS) {
        printf("pipe[%d] isp wait_be_end failed\n", send_pipe);
        return ret;
    }

    return ret;
}

static td_void *sample_vi_wdr_dump_and_send_proc(td_void *param)
{
    ot_vi_frame_dump_attr dump_attr = {
        .depth = 2,
        .enable = TD_TRUE
    };
    const td_s32 millsec = 1000;
    ot_video_frame_info dump_frame_info[OT_VI_MAX_WDR_FRAME_NUM];
    ot_vi_bind_pipe *bind_pipe = (ot_vi_bind_pipe*)param;
    ot_vi_pipe *vi_pipe = bind_pipe->pipe_id;
    td_s32 pipe_num = bind_pipe->pipe_num;
    ot_vi_pipe send_pipe = vi_pipe[0];
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < pipe_num; i++) {
        ret = ss_mpi_vi_set_pipe_frame_dump_attr(vi_pipe[i], &dump_attr);
        if (ret != TD_SUCCESS) {
            printf("set pipe[%d] dump attr failed\n", vi_pipe[i]);
            return TD_NULL;
        }
    }

    ret = ss_mpi_vi_set_pipe_frame_source(send_pipe, OT_VI_PIPE_FRAME_SOURCE_USER);
    if (ret != TD_SUCCESS) {
        printf("set pipe[%d] user fraem source failed\n", vi_pipe[0]);
        return TD_NULL;
    }

    while (g_send_pipe_pthread) {
        for (i = 0; i < pipe_num; i++) {
            ret = ss_mpi_vi_get_pipe_frame(vi_pipe[i], &dump_frame_info[i], millsec);
            if (ret != TD_SUCCESS) {
                printf("get pipe[%d] frame failed\n", vi_pipe[i]);
                goto release;
            }
        }

        if (sample_vi_match_wdr_pts(dump_frame_info, vi_pipe, pipe_num) != TD_SUCCESS) {
            printf("pipe frame not suitable, lost frame\n");
            goto release;
        }

        ret = sample_vi_send_pipe_wdr_frame(send_pipe, dump_frame_info, pipe_num, millsec);
        if (ret != TD_SUCCESS) {
            printf("pipe[%d] send frame failed\n", send_pipe);
        }

release:
        for (i = i - 1; i >= 0; i--) {
            (td_void)ss_mpi_vi_release_pipe_frame(vi_pipe[i], &dump_frame_info[i]);
        }
    }

    return TD_NULL;
}

td_s32 sample_comm_vi_send_wdr_frame(ot_vi_bind_pipe *bind_pipe)
{
    td_s32 ret;
    pthread_t thread_id = 0;
    g_send_pipe_pthread = TD_TRUE;
    ret = pthread_create(&thread_id, TD_NULL, sample_vi_wdr_dump_and_send_proc, (td_void *)bind_pipe);
    if (ret != TD_SUCCESS) {
        printf("vi create send frame thread failed!\n");
        g_send_pipe_pthread = TD_FALSE;
        return TD_FAILURE;
    }

    printf("wdr send frame thread running, print any key to exit!\n");
    if (getchar() == EOF) {
        g_send_pipe_pthread = TD_FALSE;
        pthread_join(thread_id, TD_NULL);
        return TD_FAILURE;
    }

    g_send_pipe_pthread = TD_FALSE;
    pthread_join(thread_id, TD_NULL);
    return ret;
}


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART06_H_ */
