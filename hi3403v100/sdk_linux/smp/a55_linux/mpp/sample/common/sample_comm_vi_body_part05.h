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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART05_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART05_H_

td_void sample_comm_vi_save_fpn_file(ot_isp_fpn_frame_info *fpn_frame_info, FILE *pfd)
{
    td_u8 *virt_addr;
    td_u32 fpn_height;
    td_s32 i;

    fpn_height = fpn_frame_info->fpn_frame.video_frame.height;
    virt_addr = (td_u8 *)fpn_frame_info->fpn_frame.video_frame.virt_addr[0];

    /* save Y
        * ---------------------------------------------------------------- */
    (td_void)fprintf(stderr,
                     "FPN: saving......Raw data......stide: %d, width: %d, "
                     "height: %d, iso: %d.\n",
                     fpn_frame_info->fpn_frame.video_frame.stride[0],
                     fpn_frame_info->fpn_frame.video_frame.width, fpn_height,
                     fpn_frame_info->iso);
    (td_void)fprintf(stderr, "phys addr: 0x%lx\n", (td_ulong)fpn_frame_info->fpn_frame.video_frame.phys_addr[0]);
    (td_void)fprintf(stderr, "please wait a moment to save FPN raw data.\n");
    (td_void)fflush(stderr);

    (td_void)fwrite(virt_addr, fpn_frame_info->frm_size, 1, pfd);

    /* save offset */
    for (i = 0; i < OT_VI_MAX_SPLIT_NODE_NUM; i++) {
        (td_void)fwrite(&fpn_frame_info->offset[i], 4, 1, pfd); /* 4: 4byte */
    }

    /* save compress mode */
    (td_void)fwrite(&fpn_frame_info->fpn_frame.video_frame.compress_mode, 4, 1, pfd); /* 4: 4byte */

    /* save fpn frame size */
    (td_void)fwrite(&fpn_frame_info->frm_size, 4, 1, pfd); /* 4: 4byte */

    /* save iso */
    (td_void)fwrite(&fpn_frame_info->iso, 4, 1, pfd); /* 4: 4byte */
    (td_void)fflush(pfd);
}

static td_void *sample_common_vi_send_pipe_frame_proc(td_void *param)
{
    td_s32 ret;
    td_u32 frame_cnt;
    td_u32 send_cnt;
    const ot_video_frame_info *frame_info[OT_VI_MAX_WDR_FRAME_NUM] = {TD_NULL};
    sample_vi_send_frame_info *vi_send_frame_info = (sample_vi_send_frame_info *)param;
    const td_u32 milli_sec = -1; /* milli_sec: -1 */
    const td_u32 frame_num = 1;

    frame_cnt = vi_send_frame_info->frame_cnt;
    ret = ss_mpi_vi_set_pipe_frame_source(vi_send_frame_info->vi_pipe, OT_VI_PIPE_FRAME_SOURCE_USER);
    if (ret != TD_SUCCESS) {
        printf("vi set pipe frame source failed!\n");
        goto exit;
    }

    send_cnt = 0;
    while (g_send_pipe_pthread) {
        if (send_cnt < frame_cnt) {
            vi_send_frame_info->user_frame_info[send_cnt].frame_info.video_frame.pts = 0;
            frame_info[0] = &vi_send_frame_info->user_frame_info[send_cnt].frame_info;

            ret = ss_mpi_vi_send_pipe_raw(vi_send_frame_info->vi_pipe, frame_info, frame_num, milli_sec);
            if (ret != TD_SUCCESS) {
                printf("vi send pipe frame failed with %#x!\n", ret);
                continue;
            }
            send_cnt += frame_num;
        } else {
            send_cnt = 0;
        }
    }

    ret = ss_mpi_vi_set_pipe_frame_source(vi_send_frame_info->vi_pipe, OT_VI_PIPE_FRAME_SOURCE_FE);
    if (ret != TD_SUCCESS) {
        printf("vi set pipe frame source failed!\n");
    }

exit:
    return TD_NULL;
}

static td_s32 sample_comm_vi_fpn_multi_calibrate(ot_vi_pipe vi_pipe, sample_vi_user_frame_info *user_frame_info,
    ot_isp_fpn_calibrate_attr *calibrate_attr, td_s32 calib_cnt)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < calib_cnt; i++) {
        /* point each fpn dark frame vb to calibrate_attr */
        (td_void)memcpy_s(&calibrate_attr->fpn_cali_frame.fpn_frame, sizeof(ot_video_frame_info),
                          &user_frame_info[i].frame_info, sizeof(ot_video_frame_info));

        ret = ss_mpi_isp_fpn_calibrate(vi_pipe, calibrate_attr);
        if (ret != TD_SUCCESS) {
            sample_print("vi fpn calibrate failed!\n");
            return TD_FAILURE;
        }
        (td_void)memcpy_s(&user_frame_info[i].frame_info, sizeof(ot_video_frame_info),
                          &calibrate_attr->fpn_cali_frame.fpn_frame, sizeof(ot_video_frame_info));
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_fpn_calibrate_process(ot_vi_pipe vi_pipe, sample_vi_user_frame_info *user_frame_info,
    ot_isp_fpn_calibrate_attr *calibrate_attr, td_s32 calib_cnt)
{
    td_s32 ret;
    sample_vi_user_frame_info *final_user_frame_info = &user_frame_info[FPN_CALIB_TIMES];
    pthread_t thread_id = 0;
    sample_vi_send_frame_info vi_send_frame_info;

    /* first calibrate process, save 8 dark frames to user_frame_info */
    calibrate_attr->fpn_mode = OT_ISP_FPN_OUT_MODE_HIGH;
    ret = sample_comm_vi_fpn_multi_calibrate(vi_pipe, user_frame_info, calibrate_attr, calib_cnt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    printf("first calibrate done, times: %d.\n", calib_cnt);

    vi_send_frame_info.vi_pipe = vi_pipe;
    vi_send_frame_info.frame_cnt = calib_cnt;
    vi_send_frame_info.user_frame_info = user_frame_info;
    g_send_pipe_pthread = TD_TRUE;
    ret = pthread_create(&thread_id, TD_NULL, sample_common_vi_send_pipe_frame_proc, (td_void *)&vi_send_frame_info);
    if (ret != TD_SUCCESS) {
        printf("vi create send frame thread failed!\n");
        g_send_pipe_pthread = TD_FALSE;
        return TD_FAILURE;
    }

    /* second calibrate process */
    calibrate_attr->frame_num = calib_cnt;
    calibrate_attr->fpn_mode = OT_ISP_FPN_OUT_MODE_NORM;
    ret = sample_comm_vi_fpn_multi_calibrate(vi_pipe, final_user_frame_info, calibrate_attr, 1);
    if (ret != TD_SUCCESS) {
        goto exit;
    }
    printf("second calibrate done, times: 1.\n");

exit:
    g_send_pipe_pthread = TD_FALSE;
    pthread_join(thread_id, TD_NULL);
    return ret;
}

td_s32 sample_comm_vi_fpn_calibrate(ot_vi_pipe vi_pipe, sample_vi_fpn_calibration_cfg *calibration_cfg)
{
    td_s32 ret;
    td_s32 i;
    const ot_vi_chn vi_chn = 0;
    FILE *pfd = TD_NULL;
    sample_vi_user_frame_info user_frame_info[FPN_CALIB_TIMES + 1];
    ot_isp_fpn_calibrate_attr calibrate_attr;

    td_char fpn_file_name[FPN_FILE_NAME_LENGTH];

    printf("please turn off camera aperture to start calibrate!\nhit any key ,start calibrate!\n");
    if (getchar() == EOF) {
        return TD_FAILURE;
    }

    ret = ss_mpi_vi_disable_chn(vi_pipe, vi_chn);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    calibrate_attr.threshold = calibration_cfg->threshold;
    calibrate_attr.frame_num = calibration_cfg->frame_num;
    calibrate_attr.fpn_type  = calibration_cfg->fpn_type;
    ret = sample_comm_vi_get_fpn_calibrate_frame_info(vi_pipe, OT_PIXEL_FORMAT_RGB_BAYER_16BPP,
        calibration_cfg->compress_mode, user_frame_info, FPN_CALIB_TIMES + 1);
    if (ret != TD_SUCCESS) {
        ss_mpi_vi_enable_chn(vi_pipe, vi_chn);
        return TD_FAILURE;
    }

    ret = sample_comm_vi_fpn_calibrate_process(vi_pipe, user_frame_info, &calibrate_attr, FPN_CALIB_TIMES);
    if (ret != TD_SUCCESS) {
        sample_print("vi fpn calibrate failed!\n");
        goto exit;
    }

    printf("\nafter calibrate ");
    for (i = 0; i < OT_VI_MAX_SPLIT_NODE_NUM; i++) {
        printf("offset[%d] = 0x%x, ", i, calibrate_attr.fpn_cali_frame.offset[i]);
    }
    printf("frame_size = %d, iso = %d\n", calibrate_attr.fpn_cali_frame.frm_size, calibrate_attr.fpn_cali_frame.iso);

    sample_comm_vi_get_fpn_file_name(&calibrate_attr.fpn_cali_frame.fpn_frame.video_frame,
                                     fpn_file_name, FPN_FILE_NAME_LENGTH);
    printf("save dark frame file: %s!\n", fpn_file_name);
    pfd = fopen(fpn_file_name, "wb");
    if (pfd == TD_NULL) {
        printf("open file %s err!\n", fpn_file_name);
        goto exit;
    }

    sample_comm_vi_save_fpn_file(&calibrate_attr.fpn_cali_frame, pfd);

    (td_void)fclose(pfd);

exit:
    sample_comm_vi_release_frame_blk(user_frame_info, FPN_CALIB_TIMES + 1);
    ret = ss_mpi_vi_enable_chn(vi_pipe, vi_chn);
    return ret;
}

static td_void sample_comm_vi_read_fpn_file(ot_isp_fpn_frame_info *fpn_frame_info, FILE *pfd)
{
    ot_video_frame_info *frame_info;
    td_s32 i;

    frame_info = &fpn_frame_info->fpn_frame;
    (td_void)fread((td_u8 *)frame_info->video_frame.virt_addr[0], fpn_frame_info->frm_size, 1, pfd);

    for (i = 0; i < OT_VI_MAX_SPLIT_NODE_NUM; i++) {
        (td_void)fread((td_u8 *)&fpn_frame_info->offset[i], 4, 1, pfd); /* 4: 4byte */
    }

    (td_void)fread((td_u8 *)&frame_info->video_frame.compress_mode, 4, 1, pfd); /* 4: 4byte */
    (td_void)fread((td_u8 *)&fpn_frame_info->frm_size, 4, 1, pfd); /* 4: 4byte */
    (td_void)fread((td_u8 *)&fpn_frame_info->iso, 4, 1, pfd); /* 4: 4byte */
}

td_s32 sample_comm_vi_enable_fpn_correction(ot_vi_pipe vi_pipe, sample_vi_fpn_correction_cfg *correction_cfg)
{
    td_s32 ret;
    td_u32 i;
    FILE *pfd = TD_NULL;
    ot_isp_fpn_attr correction_attr;
    sample_vi_user_frame_info *user_frame_info = &correction_cfg->user_frame_info;
    td_char fpn_file_name[FPN_FILE_NAME_LENGTH];

    ret = sample_comm_vi_get_fpn_frame_info(vi_pipe, correction_cfg->pixel_format,
                                            correction_cfg->compress_mode, user_frame_info, 1);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    (td_void)memcpy_s(&correction_attr.fpn_frm_info.fpn_frame, sizeof(ot_video_frame_info),
                      &user_frame_info->frame_info, sizeof(ot_video_frame_info));

    sample_comm_vi_get_fpn_file_name(&correction_attr.fpn_frm_info.fpn_frame.video_frame,
                                     fpn_file_name, FPN_FILE_NAME_LENGTH);
    pfd = fopen(fpn_file_name, "rb");
    if (pfd == TD_NULL) {
        printf("open file %s err!\n", fpn_file_name);
        goto exit;
    }

    correction_attr.fpn_frm_info.frm_size = user_frame_info->blk_size;
    sample_comm_vi_read_fpn_file(&correction_attr.fpn_frm_info, pfd);

    (td_void)fclose(pfd);

    for (i = 0; i < OT_VI_MAX_SPLIT_NODE_NUM; i++) {
        printf("offset[%d] = 0x%x; ", i, correction_attr.fpn_frm_info.offset[i]);
    }
    printf("\n");
    printf("frame_size = %d.\n", correction_attr.fpn_frm_info.frm_size);
    printf("iso = %d.\n", correction_attr.fpn_frm_info.iso);

    correction_attr.enable = TD_TRUE;
    correction_attr.op_type = correction_cfg->op_mode;
    correction_attr.fpn_type = correction_cfg->fpn_type;
    correction_attr.manual_attr.strength = correction_cfg->strength;
    ret = ss_mpi_isp_set_fpn_attr(vi_pipe, &correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fpn attr failed!\n");
        goto exit;
    }

    return TD_SUCCESS;

exit:
    sample_comm_vi_release_frame_blk(user_frame_info, 1);
    return ret;
}

td_s32 sample_comm_vi_enable_fpn_correction_for_scene(ot_vi_pipe vi_pipe, sample_vi_fpn_correction_cfg *correction_cfg,
    td_u32 iso, sample_scene_fpn_offset_cfg *scene_fpn_offset_cfg, const td_char *dir_name)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 i;
    FILE *pfd = TD_NULL;
    ot_isp_fpn_attr correction_attr;
    sample_vi_user_frame_info *user_frame_info = &correction_cfg->user_frame_info;
    td_char fpn_file_name[FPN_FILE_NAME_LENGTH];
    check_return(sample_comm_vi_get_fpn_frame_info(vi_pipe, correction_cfg->pixel_format, correction_cfg->compress_mode,
                                                   user_frame_info, 1),
                 "sample_comm_vi_get_fpn_frame_info");
    (td_void)memcpy_s(&correction_attr.fpn_frm_info.fpn_frame, sizeof(ot_video_frame_info),
                      &user_frame_info->frame_info, sizeof(ot_video_frame_info));

    check_return(sample_comm_vi_get_fpn_file_name_iso(&correction_attr.fpn_frm_info.fpn_frame.video_frame, dir_name,
                                                      fpn_file_name, FPN_FILE_NAME_LENGTH, iso),
                 "sample_comm_vi_get_fpn_file_name_iso");
    pfd = fopen(fpn_file_name, "rb");
    if (pfd == TD_NULL) {
        printf("open file %s err!\n", fpn_file_name);
        goto exit;
    }
    printf("open file %s success!\n", fpn_file_name);
    correction_attr.fpn_frm_info.frm_size = user_frame_info->blk_size;
    sample_comm_vi_read_fpn_file(&correction_attr.fpn_frm_info, pfd);
    ret = fclose(pfd);
    if (ret != TD_SUCCESS) {
        goto exit;
    }
    correction_attr.fpn_frm_info.iso = iso;
    for (i = 0; i < OT_VI_MAX_SPLIT_NODE_NUM; i++) {
        correction_attr.fpn_frm_info.offset[i] = scene_fpn_offset_cfg->offset;
        printf("offset[%d] = %#x; ", i, scene_fpn_offset_cfg->offset);
    }
    printf("\n frame_size = %d. iso = %d.\n", correction_attr.fpn_frm_info.frm_size, correction_attr.fpn_frm_info.iso);
    correction_attr.enable = TD_TRUE;
    correction_attr.op_type = correction_cfg->op_mode;
    correction_attr.fpn_type = correction_cfg->fpn_type;
    correction_attr.manual_attr.strength = correction_cfg->strength;
    correction_attr.fpn_frm_info.fpn_frame.video_frame.compress_mode = correction_cfg->compress_mode;
    ret = ss_mpi_isp_set_fpn_attr(vi_pipe, &correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fpn attr failed!\n");
        goto exit;
    }
    return TD_SUCCESS;

exit:
    sample_comm_vi_release_frame_blk(user_frame_info, 1);
    return ret;
}

td_s32 sample_comm_vi_disable_fpn_correction(ot_vi_pipe vi_pipe, sample_vi_fpn_correction_cfg *correction_cfg)
{
    td_s32 ret;
    ot_isp_fpn_attr correction_attr;

    ret = ss_mpi_isp_get_fpn_attr(vi_pipe, &correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("get fpn attr failed!\n");
        return TD_FAILURE;
    }

    correction_attr.enable = TD_FALSE;
    ret = ss_mpi_isp_set_fpn_attr(vi_pipe, &correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fpn attr failed!\n");
        return TD_FAILURE;
    }

    sample_comm_vi_release_frame_blk(&correction_cfg->user_frame_info, 1);

    return TD_SUCCESS;
}

td_s32 sample_comm_vi_start_virt_pipe(const sample_vi_cfg *vi_cfg)
{
    td_s32 ret;

    ret = sample_comm_vi_start_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
    if (ret != TD_SUCCESS) {
        sample_print("start pipe failed!\n");
        goto start_pipe_failed;
    }

    ret = sample_comm_vi_start_isp(vi_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vi_start_isp failed!\n");
        goto start_isp_failed;
    }

    return TD_SUCCESS;

start_isp_failed:
    sample_comm_vi_stop_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
start_pipe_failed:
    return TD_FAILURE;
}

td_void sample_comm_vi_stop_virt_pipe(const sample_vi_cfg *vi_cfg)
{
    sample_comm_vi_stop_isp(vi_cfg);
    sample_comm_vi_stop_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
}

static td_s32 sample_comm_vi_convert_chroma_planar_to_sp42x(FILE *file, td_u8 *chroma_data,
    td_u32 luma_stride, td_u32 chroma_width, td_u32 chroma_height)
{
    td_u32 chroma_stride = luma_stride >> 1;
    td_u8 *dst = TD_NULL;
    td_u32 row;
    td_u32 list;
    td_u8 *temp = TD_NULL;

    temp = (td_u8*)malloc(chroma_stride);
    if (temp == TD_NULL) {
        sample_print("vi malloc failed!\n");
        return TD_FAILURE;
    }
    if (memset_s(temp, chroma_stride, 0, chroma_stride) != EOK) {
        sample_print("vi memset_s failed!\n");
        free(temp);
        temp = TD_NULL;
        return TD_FAILURE;
    }

    /* U */
    dst = chroma_data + 1;
    for (row = 0; row < chroma_height; ++row) {
        (td_void)fread(temp, chroma_width, 1, file); /* sp420 U-component data starts 1/2 way from the beginning */
        for (list = 0; list < chroma_stride; ++list) {
            *dst = *(temp + list);
            dst += 2; /* traverse 2 steps away to the next U-component data */
        }
        dst = chroma_data + 1;
        dst += (row + 1) * luma_stride;
    }

    /* V */
    dst = chroma_data;
    for (row = 0; row < chroma_height; ++row) {
        (td_void)fread(temp, chroma_width, 1, file); /* sp420 V-component data starts 1/2 way from the beginning */
        for (list = 0; list < chroma_stride; ++list) {
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART05_H_ */
