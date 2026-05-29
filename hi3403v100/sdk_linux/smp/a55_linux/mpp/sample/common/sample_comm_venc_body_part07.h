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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART07_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART07_H_

#ifdef CONFIG_OT_HEIF_SUPPORT
static int32_t sample_heif_create(td_s32 index, const sample_comm_venc_stream_proc_info *stream_proc_info,
    heif_handle *hdl)
{
    heif_config config;
    if (snprintf_s(config.file_desc.input.url, FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "./stream_chn%d_%d%s", index, stream_proc_info->picture_cnt[index], ".heic") < 0) {
        return SAMPLE_RETURN_NULL;
    }
    config.file_desc.file_type = HEIF_FILE_TYPE_URL;
    config.config_type = HEIF_CONFIG_MUXER;
    config.muxer_config.is_grid = false;
    config.muxer_config.row_image_num = 1;
    config.muxer_config.column_image_num = 1;
    config.muxer_config.format_profile = HEIF_PROFILE_HEIC;
    return heif_create(hdl, &config);
}

static td_s32 sample_comm_save_h265_to_heic(td_s32 index, const sample_comm_venc_stream_proc_info *stream_proc_info,
    const ot_venc_stream *stream)
{
    td_u32 i;
    td_u32 total_len = 0;
    td_s32 has_key = 0;
    for (i = 0; i < stream->pack_cnt; i++) {
        if (stream->pack[i].data_type.h265_type == OT_VENC_H265_NALU_IDR_SLICE) {
            has_key = 1;
        }
        total_len += stream->pack[i].len - stream->pack[i].offset;
    }
    if (total_len > 0 && has_key == 1) {
        heif_handle handle = NULL;
        td_s32 ret = sample_heif_create(index, stream_proc_info, &handle);
        if (ret != 0) {
            sample_print("HeifCreate error ret:%d\n", ret);
        }
        td_u8 *data_buffer = (td_u8 *)malloc(total_len);
        if (data_buffer == NULL) {
            sample_print("malloc error\n");
            heif_destroy(handle);
            return SAMPLE_RETURN_NULL;
        }
        td_u32 write_len = 0;
        for (i = 0; i < stream->pack_cnt; i++) {
            if (memcpy_s(data_buffer + write_len, total_len - write_len,
                stream->pack[i].addr + stream->pack[i].offset, stream->pack[i].len - stream->pack[i].offset) != EOK) {
                sample_print("memcpy_s failed\n");
            }
            write_len += stream->pack[i].len;
        }
        heif_image_item item = {0};
        item.timestamp = -1;
        item.data = data_buffer;
        item.length = write_len;
        item.key_frame = true;
        ret = heif_write_master_image(handle, 0, &item, 1);
        if (data_buffer != NULL) {
            free(data_buffer);
        }
        heif_destroy(handle);
        return ret;
    }
    return 0;
}
#endif

static td_s32 sample_comm_save_frame_to_file(td_s32 index, sample_comm_venc_stream_proc_info *stream_proc_info,
    ot_venc_stream *stream, ot_venc_stream_buf_info *stream_buf_info, ot_payload_type *payload_type)
{
    td_s32 ret;
    td_s32 fd;
    if (payload_type[index] == OT_PT_JPEG) {
        if (snprintf_s(stream_proc_info->file_name[index], FILE_NAME_LEN, FILE_NAME_LEN - 1, "./") < 0) {
            free(stream->pack);
            return SAMPLE_RETURN_NULL;
        }
        if (sample_comm_venc_get_realpath(stream_proc_info->file_name[index], stream_proc_info->real_file_name[index],
            sizeof(stream_proc_info->real_file_name[index])) != TD_SUCCESS) {
            free(stream->pack);
            sample_print("chn[%d] stream file path error\n", stream_proc_info->venc_chn);
            return SAMPLE_RETURN_NULL;
        }

        if (snprintf_s(stream_proc_info->real_file_name[index], FILE_NAME_LEN, FILE_NAME_LEN - 1,
            "stream_chn%d_%d%s", index, stream_proc_info->picture_cnt[index], stream_proc_info->file_postfix) < 0) {
            free(stream->pack);
            return SAMPLE_RETURN_NULL;
        }
        stream_proc_info->file[index] = fopen(stream_proc_info->real_file_name[index], "wb");
        if (!stream_proc_info->file[index]) {
            free(stream->pack);
            sample_print("open file err!\n");
            return SAMPLE_RETURN_NULL;
        }
        fd = fileno(stream_proc_info->file[index]);
        fchmod(fd, S_IRUSR | S_IWUSR);
    }

#ifdef CONFIG_OT_HEIF_SUPPORT
    (td_void) sample_comm_save_h265_to_heic(index, stream_proc_info, stream);
#endif
#ifndef __LITEOS__
    ot_unused(stream_buf_info);
    ret = sample_comm_venc_save_stream(stream_proc_info->file[index], stream);
#else
    ret = sample_comm_venc_save_stream_phys_addr(stream_proc_info->file[index], &stream_buf_info[index], stream);
#endif
    if (ret != TD_SUCCESS) {
        free(stream->pack);
        stream->pack = TD_NULL;
        sample_print("save stream failed!\n");
        return SAMPLE_RETURN_BREAK;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_get_stream_from_one_channl(sample_comm_venc_stream_proc_info *stream_proc_info,
    td_s32 index, ot_venc_stream_buf_info *stream_buf_info, ot_payload_type *payload_type)
{
    td_s32 ret;
    ot_venc_stream stream;
    ot_venc_chn_status stat;

    /* step 2.1 : query how many packs in one-frame stream. */
    if (memset_s(&stream, sizeof(stream), 0, sizeof(stream)) != EOK) {
        printf("call memset_s error\n");
    }

    ret = ss_mpi_venc_query_status(index, &stat);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_query_status chn[%d] failed with %#x!\n", index, ret);
        return SAMPLE_RETURN_BREAK;
    }

    if (stat.cur_packs == 0) {
        return SAMPLE_RETURN_CONTINUE;
    }
    /* step 2.3 : malloc corresponding number of pack nodes. */
    stream.pack = (ot_venc_pack *)malloc(sizeof(ot_venc_pack) * stat.cur_packs);
    if (stream.pack == TD_NULL) {
        sample_print("malloc stream pack failed!\n");
        return SAMPLE_RETURN_BREAK;
    }

    /* step 2.4 : call mpi to get one-frame stream */
    stream.pack_cnt = stat.cur_packs;
    ret = ss_mpi_venc_get_stream(index, &stream, TD_TRUE);
    if (ret != TD_SUCCESS) {
        free(stream.pack);
        stream.pack = TD_NULL;
        sample_print("ss_mpi_venc_get_stream failed with %#x!\n", ret);
        return SAMPLE_RETURN_BREAK;
    }

    /* step 2.5 : save frame to file */
    ret = sample_comm_save_frame_to_file(index, stream_proc_info, &stream, stream_buf_info, payload_type);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* step 2.6 : release stream */
    ret = ss_mpi_venc_release_stream(index, &stream);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_release_stream failed!\n");
        free(stream.pack);
        stream.pack = TD_NULL;
        return SAMPLE_RETURN_BREAK;
    }

    /* step 2.7 : free pack nodes */
    free(stream.pack);
    stream.pack = TD_NULL;
    stream_proc_info->picture_cnt[index]++;
    if (payload_type[index] == OT_PT_JPEG && stream_proc_info->file[index] != TD_NULL) {
        fclose(stream_proc_info->file[index]);
        stream_proc_info->file[index] = TD_NULL;
    }

    return TD_SUCCESS;
}

static td_void sample_comm_fd_isset(sample_comm_venc_stream_proc_info *stream_proc_info, osal_fd_set *read_fds,
    ot_venc_stream_buf_info *stream_buf_info, ot_payload_type *payload_type, sample_venc_getstream_para *para)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; (i < stream_proc_info->chn_total) && (i < OT_VENC_MAX_CHN_NUM); i++) {
        if (OSAL_FD_ISSET(stream_proc_info->venc_fd[i], read_fds)) {
            stream_proc_info->venc_chn = para->venc_chn[i];
            ret = sample_comm_get_stream_from_one_channl(stream_proc_info, i, stream_buf_info, payload_type);
            if (ret == SAMPLE_RETURN_CONTINUE) {
                continue;
            } else if (ret == SAMPLE_RETURN_BREAK) {
                break;
            }
        }
    }
}

td_void *sample_comm_venc_get_venc_stream_proc(td_void *p)
{
    td_s32 i;
    td_s32 ret;
    sample_venc_getstream_para *para = TD_NULL;
    struct timeval timeout_val;
    osal_fd_set read_fds;
    ot_payload_type payload_type[OT_VENC_MAX_CHN_NUM] = {0};
    ot_venc_stream_buf_info stream_buf_info[OT_VENC_MAX_CHN_NUM];
    sample_comm_venc_stream_proc_info stream_proc_info = {0};

    prctl(PR_SET_NAME, "get_venc_stream", 0, 0, 0);

    para = (sample_venc_getstream_para *)p;
    stream_proc_info.chn_total = para->cnt;
    /* step 1:  check & prepare save-file & venc-fd */
    if (stream_proc_info.chn_total >= OT_VENC_MAX_CHN_NUM) {
        sample_print("input count invalid\n");
        return TD_NULL;
    }

    ret = sample_comm_set_name_save_stream(&stream_proc_info, stream_buf_info, payload_type, para, OT_VENC_MAX_CHN_NUM);
    if (ret == SAMPLE_RETURN_NULL) {
        return TD_NULL;
    } else if (ret == SAMPLE_RETURN_FAILURE) {
        return (void *)TD_FAILURE;
    }

    /* step 2:  start to get streams of each channel. */
    while (para->thread_start == TD_TRUE) {
        OSAL_FD_ZERO(&read_fds);
        for (i = 0; (i < stream_proc_info.chn_total) && (i < OT_VENC_MAX_CHN_NUM); i++) {
            OSAL_FD_SET(stream_proc_info.venc_fd[i], &read_fds);
        }

        timeout_val.tv_sec = 2; /* 2 is a number */
        timeout_val.tv_usec = 0;
        ret = OSAL_SELECT(stream_proc_info.maxfd + 1, &read_fds, TD_NULL, TD_NULL, &timeout_val);
        if (ret < 0) {
            sample_print("select failed!\n");
            break;
        } else if (ret == 0) {
            sample_print("get venc stream time out, exit thread\n");
            continue;
        } else {
            sample_comm_fd_isset(&stream_proc_info, &read_fds, stream_buf_info, payload_type, para);
        }
    }

    /* step 3 : close save-file */
    for (i = 0; i < stream_proc_info.chn_total; i++) {
        if (payload_type[i] != OT_PT_JPEG && stream_proc_info.file[i] != TD_NULL) {
            fclose(stream_proc_info.file[i]);
            stream_proc_info.file[i] = TD_NULL;
        }
    }

    return TD_NULL;
}

/******************************************************************************
* function : bitrate_auto
******************************************************************************/
#define SAMPLE_VENC_WIDHT  640
#define SAMPLE_VENC_HEIGHT 480
#define SAMPLE_VENC_NUM 5
#define SAMPLE_VENC_FG_TYPE 5
#define QUERY_SLEEP   1000

td_void sample_comm_venc_set_region(td_u64 time, ot_venc_chn venc_chn, ot_venc_svc_rect_info *pst_svc_rect)
{
    td_s32 j;
    td_s32 ret;
    td_u32 attrx[SAMPLE_VENC_NUM] = {32, 96, 128, 192, 256}; // 32 96 128 192 256 : X-coordinate
    td_u32 attry[SAMPLE_VENC_NUM] = {32, 96, 128, 192, 256}; // 32 96 128 192 256 : Y-coordinate
    td_u32 attrw[SAMPLE_VENC_NUM] = {32, 64, 96, 96, 64}; // 32 64 96 96 64 : width
    td_u32 attrh[SAMPLE_VENC_NUM] = {32, 64, 96, 96, 64}; // 32 64 96 96 64 : height
    td_u32 type[SAMPLE_VENC_NUM] = {0, 3, 4, 1, 2}; // 0 3 4 1 2 : type
    pst_svc_rect->rect_num = SAMPLE_VENC_NUM;
    pst_svc_rect->pts = time;
    pst_svc_rect->base_resolution.width = SAMPLE_VENC_WIDHT;
    pst_svc_rect->base_resolution.height = SAMPLE_VENC_HEIGHT;

    for (j = 0; (j < SAMPLE_VENC_NUM) && (j < OT_VENC_MAX_SVC_RECT_NUM); j++) {
        pst_svc_rect->detect_type[j] = type[j];
        pst_svc_rect->rect_attr[j].x = attrx[j];
        pst_svc_rect->rect_attr[j].y = attry[j];
        pst_svc_rect->rect_attr[j].width = attrw[j];
        pst_svc_rect->rect_attr[j].height = attrh[j];
    }
    ret = ss_mpi_venc_send_svc_region(venc_chn, pst_svc_rect);
    if (ret != TD_SUCCESS) {
        sample_print("Set ss_mpi_venc_send_svc_region failed for %#x chn =%d\n", ret, venc_chn);
    }
}
td_void sample_comm_venc_set_svc_param(ot_venc_chn venc_chn, ot_venc_svc_param *pst_svc_param)
{
    td_s32 j;
    td_s32 ret;
    td_u32 qp_i[SAMPLE_VENC_NUM] = {2, 62, 94, 1, 0}; // 2 62 94 1 0 : fg I frame qp
    td_u32 qp_p[SAMPLE_VENC_NUM] = {4, 58, 94, 2, 0}; // 4 58 94 2 0 : fg P frame qp
    ret = ss_mpi_venc_get_svc_param(venc_chn, pst_svc_param);
    if (ret != TD_SUCCESS) {
        sample_print("Set ss_mpi_venc_set_svc_param failed for %#x chn =%d\n", ret, venc_chn);
    }
    pst_svc_param->fg_protect_adaptive_en = TD_TRUE;
    pst_svc_param->motion_region.qpmap_value_i = 0;
    pst_svc_param->motion_region.qpmap_value_p = 0;
    pst_svc_param->motion_region.skipmap_value = 0;
    pst_svc_param->bg_region.qpmap_value_i = 2;  // 2 :i frame bg qp
    pst_svc_param->bg_region.qpmap_value_p = 6; // 6 :p frame bg qp
    pst_svc_param->bg_region.skipmap_value = 0;
    for (j = 0; (j < SAMPLE_VENC_NUM) && (j < SVC_RECT_TYPE_BUTT); j++) {
        pst_svc_param->fg_region[j].qpmap_value_i = qp_i[j];
        pst_svc_param->fg_region[j].qpmap_value_p = qp_p[j];
        pst_svc_param->fg_region[j].skipmap_value = 0;
    }
    ret = ss_mpi_venc_set_svc_param(venc_chn, pst_svc_param);
    if (ret != TD_SUCCESS) {
        sample_print("Set ss_mpi_venc_set_svc_param failed for %#x!\n", ret);
    }
}

td_void *sample_comm_venc_rateauto_stream_proc(td_void *p)
{
    td_s32 i;
    td_s32 ret;
    sample_venc_rateauto_para *para;
    ot_venc_svc_param svc_param;
    ot_vpss_chn_attr  vpss_chn_attr;
    ot_venc_svc_rect_info  svc_rect_info = {0};
    para = (sample_venc_rateauto_para *)p;
    ot_video_frame_info video_frame;
    prctl(PR_SET_NAME, "get_venc_rateauto_stream", 0, 0, 0);
    if (para->cnt >= OT_VENC_MAX_CHN_NUM) {
        sample_print("input count invalid\n");
        return TD_NULL;
    }
    for (i = 0; (i < para->cnt) && (i < OT_VENC_MAX_CHN_NUM); i++) {
        ret = ss_mpi_vpss_get_chn_attr(para->vpss_grp, para->vpss_chn[i], &vpss_chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vpss_get_chn_attr err: 0x%x", ret);
        }

        vpss_chn_attr.depth = 3; /* 3 is a number */
        ret = ss_mpi_vpss_set_chn_attr(para->vpss_grp, para->vpss_chn[i], &vpss_chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vpss_set_chn_attr err: 0x%x", ret);
        }
    }

    while (para->thread_start == TD_TRUE) {
        for (i = 0; (i < para->cnt) && (i < OT_VENC_MAX_CHN_NUM); i++) {
            ret = ss_mpi_vpss_get_chn_frame(para->vpss_grp, para->vpss_chn[i], &video_frame, QUERY_SLEEP);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_get_chn_frame err:0x%x venc_chn = %d\n", ret, para->venc_chn[i]);
                continue;
            }
            ret = ss_mpi_vpss_release_chn_frame(para->vpss_grp, para->vpss_chn[i], &video_frame);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_release_chn_frame err:0x%x\n", ret);
                return TD_NULL;
            }

            ret = ss_mpi_venc_enable_svc(para->venc_chn[i], TD_TRUE);
            if (ret != TD_SUCCESS) {
                sample_print("Set ss_mpi_venc_enable_svc failed for %#x!\n", ret);
                return TD_NULL;
            }
            sample_comm_venc_set_region(video_frame.video_frame.pts, para->venc_chn[i], &svc_rect_info);
            sample_comm_venc_set_svc_param(para->venc_chn[i], &svc_param);
        }
        usleep(QUERY_SLEEP);
    }
    return TD_NULL;
}

static td_s32 sample_comm_set_file_name_svc_t(sample_comm_venc_stream_proc_info *stream_proc_info, td_s32 index)
{
    if (snprintf_s(stream_proc_info->file_name[index], FILE_NAME_LEN, FILE_NAME_LEN - 1, "./") < 0) {
        return TD_NULL;
    }
    if (sample_comm_venc_get_realpath(stream_proc_info->file_name[index], stream_proc_info->real_file_name[index],
        sizeof(stream_proc_info->real_file_name[index])) != TD_SUCCESS) {
        printf("file path error\n");
        return TD_NULL;
    }
    if (snprintf_s(stream_proc_info->real_file_name[index], FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "tid%d%s", index, stream_proc_info->file_postfix) < 0) {
        return TD_NULL;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_open_svc_files(sample_comm_venc_stream_proc_info *stream_proc_info, td_s32 index)
{
    td_s32 cnt;
    td_s32 fd;

    for (cnt = 0; cnt < 3; cnt++) { /* 3 is a number */
        if (sample_comm_set_file_name_svc_t(stream_proc_info, (index + cnt)) != TD_SUCCESS) {
            return SAMPLE_RETURN_NULL;
        }

        stream_proc_info->file[index + cnt] = fopen(stream_proc_info->real_file_name[index + cnt], "wb");
        if (!stream_proc_info->file[index + cnt]) {
            sample_print("open file[%s] failed!\n", stream_proc_info->real_file_name[index + cnt]);
            return SAMPLE_RETURN_NULL;
        }
        fd = fileno(stream_proc_info->file[index + cnt]);
        fchmod(fd, S_IRUSR | S_IWUSR);
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_prepare_svc_stream_chn(sample_comm_venc_stream_proc_info *stream_proc_info,
    ot_venc_stream_buf_info *stream_buf_info, td_s32 index)
{
    td_s32 ret;
    ot_venc_chn_attr venc_chn_attr;

    /* decide the stream file name, and open file to save stream */
    stream_proc_info->venc_chn = index;
    ret = ss_mpi_venc_get_chn_attr(stream_proc_info->venc_chn, &venc_chn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_get_chn_attr chn[%d] failed with %#x!\n", stream_proc_info->venc_chn, ret);
        return SAMPLE_RETURN_NULL;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART07_H_ */
