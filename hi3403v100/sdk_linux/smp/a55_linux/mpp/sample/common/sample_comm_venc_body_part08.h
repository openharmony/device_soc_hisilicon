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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART08_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART08_H_
    }
    ret = sample_comm_venc_get_file_postfix(venc_chn_attr.venc_attr.type, stream_proc_info->file_postfix,
        sizeof(stream_proc_info->file_postfix));
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_venc_get_file_postfix [%d] failed with %#x!\n",
            venc_chn_attr.venc_attr.type, ret);
        return SAMPLE_RETURN_NULL;
    }
    ret = sample_comm_open_svc_files(stream_proc_info, index);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /* set venc fd. */
    stream_proc_info->venc_fd[index] = ss_mpi_venc_get_fd(index);
    if (stream_proc_info->venc_fd[index] < 0) {
        sample_print("ss_mpi_venc_get_fd failed with %#x!\n", stream_proc_info->venc_fd[index]);
        return SAMPLE_RETURN_NULL;
    }
    if (stream_proc_info->maxfd <= stream_proc_info->venc_fd[index]) {
        stream_proc_info->maxfd = stream_proc_info->venc_fd[index];
    }
    ret = ss_mpi_venc_get_stream_buf_info(index, &stream_buf_info[index]);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_get_stream_buf_info failed with %#x!\n", ret);
        return SAMPLE_RETURN_FAILURE;
    }
    return TD_SUCCESS;
}
static td_s32 sample_comm_set_name_save_stream_svc_t(sample_comm_venc_stream_proc_info *stream_proc_info,
    ot_venc_stream_buf_info *stream_buf_info, td_s32 venc_max_chn)
{
    td_s32 i;
    td_s32 ret;
    ot_unused(venc_max_chn);

    for (i = 0; i < stream_proc_info->chn_total; i++) {
        ret = sample_comm_prepare_svc_stream_chn(stream_proc_info, stream_buf_info, i);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    return TD_SUCCESS;
}
static td_void sample_comm_save_frame_to_file_svc_t(td_s32 index, sample_comm_venc_stream_proc_info *stream_proc_info,
    ot_venc_stream *stream, ot_venc_stream_buf_info *stream_buf_info)
{
    td_s32 cnt;
    td_s32 ret;

    for (cnt = 0; cnt < 3; cnt++) { /* 3 is a number */
        switch (cnt) {
            case 0: /* 0 is a number */
                if (stream->h264_info.ref_type == OT_VENC_BASE_IDR_SLICE ||
                    stream->h264_info.ref_type == OT_VENC_BASE_P_SLICE_REF_BY_BASE) {
#ifndef __LITEOS__
                    ot_unused(stream_buf_info);
                    ret = sample_comm_venc_save_stream(stream_proc_info->file[index + cnt], stream);
#else
                    ret = sample_comm_venc_save_stream_phys_addr(stream_proc_info->file[index + cnt],
                        &stream_buf_info[index], stream);
#endif
                }
                break;
            case 1: /* 1 is a number */
                if (stream->h264_info.ref_type == OT_VENC_BASE_IDR_SLICE ||
                    stream->h264_info.ref_type == OT_VENC_BASE_P_SLICE_REF_BY_BASE ||
                    stream->h264_info.ref_type == OT_VENC_BASE_P_SLICE_REF_BY_ENHANCE) {
#ifndef __LITEOS__
                    ret = sample_comm_venc_save_stream(stream_proc_info->file[index + cnt], stream);
#else
                    ret = sample_comm_venc_save_stream_phys_addr(stream_proc_info->file[index + cnt],
                        &stream_buf_info[index], stream);
#endif
                }
                break;
            case 2: /* 2 is a number */
#ifndef __LITEOS__
                ret = sample_comm_venc_save_stream(stream_proc_info->file[index + cnt], stream);
#else
                ret = sample_comm_venc_save_stream_phys_addr(stream_proc_info->file[index + cnt],
                    &stream_buf_info[index], stream);
#endif
                break;
            default:
                break;
        }
        if (ret != TD_SUCCESS) {
            free(stream->pack);
            stream->pack = TD_NULL;
            sample_print("save stream failed!\n");
            break;
        }
    }
}
static td_s32 sample_comm_get_stream_from_one_channl_svc_t(sample_comm_venc_stream_proc_info *stream_proc_info,
    td_s32 index, ot_venc_stream_buf_info *stream_buf_info)
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
        sample_print("ss_mpi_venc_query chn[%d] failed with %#x!\n", index, ret);
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
    sample_comm_save_frame_to_file_svc_t(index, stream_proc_info, &stream, stream_buf_info);

    /* step 2.6 : release stream */
    ret = ss_mpi_venc_release_stream(index, &stream);
    if (ret != TD_SUCCESS) {
        free(stream.pack);
        stream.pack = TD_NULL;
        return SAMPLE_RETURN_BREAK;
    }
    /* step 2.7 : free pack nodes */
    free(stream.pack);
    stream.pack = TD_NULL;

    return TD_SUCCESS;
}
static td_void sample_comm_fd_isset_svc_t(sample_comm_venc_stream_proc_info *stream_proc_info,
    osal_fd_set *read_fds, ot_venc_stream_buf_info *stream_buf_info, td_s32 venc_max_chn)
{
    td_s32 i;
    td_s32 ret;
    ot_unused(venc_max_chn);

    for (i = 0; i < stream_proc_info->chn_total; i++) {
        if (OSAL_FD_ISSET(stream_proc_info->venc_fd[i], read_fds)) {
            ret = sample_comm_get_stream_from_one_channl_svc_t(stream_proc_info, i, stream_buf_info);
            if (ret == SAMPLE_RETURN_CONTINUE) {
                continue;
            } else if (ret == SAMPLE_RETURN_BREAK) {
                break;
            }
        }
    }
}
/* get svc_t stream from h264 channels and save them */
td_void *sample_comm_venc_get_venc_stream_proc_svc_t(td_void *p)
{
    td_s32 i;
    td_s32 ret;
    td_s32 cnt = 0;

    sample_venc_getstream_para *para = TD_NULL;
    struct timeval timeout_val;
    osal_fd_set read_fds;
    ot_venc_stream_buf_info stream_buf_info[OT_VENC_MAX_CHN_NUM];
    sample_comm_venc_stream_proc_info stream_proc_info = {0};

    para = (sample_venc_getstream_para *)p;
    stream_proc_info.chn_total = para->cnt;
    stream_proc_info.save_heif = para->save_heif;

    /* step 1:  check & prepare save-file & venc-fd */
    if (stream_proc_info.chn_total >= OT_VENC_MAX_CHN_NUM) {
        sample_print("input count invalid\n");
        return TD_NULL;
    }
    ret = sample_comm_set_name_save_stream_svc_t(&stream_proc_info, stream_buf_info, OT_VENC_MAX_CHN_NUM);
    if (ret == SAMPLE_RETURN_NULL) {
        return TD_NULL;
    } else if (ret == SAMPLE_RETURN_FAILURE) {
        return (void *)TD_FAILURE;
    }
    /* step 2:  start to get streams of each channel. */
    while (para->thread_start == TD_TRUE) {
        OSAL_FD_ZERO(&read_fds);
        for (i = 0; i < stream_proc_info.chn_total; i++) {
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
            sample_comm_fd_isset_svc_t(&stream_proc_info, &read_fds, stream_buf_info, OT_VENC_MAX_CHN_NUM);
        }
    }
    /* step 3 : close save-file */
    for (i = 0; i < stream_proc_info.chn_total; i++) {
        for (cnt = 0; cnt < 3; cnt++) { /* 3 is a number */
            if (stream_proc_info.file[i + cnt]) {
                fclose(stream_proc_info.file[i + cnt]);
            }
        }
    }
    return TD_NULL;
}
td_void sample_comm_venc_set_save_heif(td_bool save_heif)
{
    g_para.save_heif = save_heif;
    sample_print("set save heif flag: %d!\n", save_heif);
}
/* start get venc stream process thread */
#define VENC_GET_STREAM_PTHREAD_STACK_SIZE (512 * 1024)
td_s32 sample_comm_venc_start_get_stream(ot_venc_chn *venc_chn, td_s32 cnt)
{
    td_s32 i;
    pthread_attr_t attr;

    g_para.thread_start = TD_TRUE;
    g_para.cnt = cnt;
    for (i = 0; (i < cnt) && (i < OT_VENC_MAX_CHN_NUM); i++) {
        g_para.venc_chn[i] = venc_chn[i];
    }
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, VENC_GET_STREAM_PTHREAD_STACK_SIZE);
    return pthread_create(&g_venc_pid, &attr, sample_comm_venc_get_venc_stream_proc, (td_void *)&g_para);
}
/******************************************************************************
* function : start rate auto process thread
******************************************************************************/
td_s32 sample_comm_venc_rateauto_start(ot_venc_chn venc_chn[], td_s32 cnt, ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn[])
{
    td_s32 i;

    g_venc_rateauto_frame_param.thread_start = TD_TRUE;
    g_venc_rateauto_frame_param.cnt = cnt;
    g_venc_rateauto_frame_param.vpss_grp = vpss_grp;
    for (i = 0; (i < cnt) && (i < OT_VENC_MAX_CHN_NUM); i++) {
        g_venc_rateauto_frame_param.venc_chn[i] = venc_chn[i];
        g_venc_rateauto_frame_param.vpss_chn[i] = vpss_chn[i];
    }
    return pthread_create(&g_venc_rateauto_pid, 0, sample_comm_venc_rateauto_stream_proc,
        (td_void *)&g_venc_rateauto_frame_param);
}
/* start get venc svc-t stream process thread */
td_s32 sample_comm_venc_start_get_stream_svc_t(td_s32 cnt)
{
    g_para.thread_start = TD_TRUE;
    g_para.cnt = cnt;
    return pthread_create(&g_venc_pid, 0, sample_comm_venc_get_venc_stream_proc_svc_t, (td_void *)&g_para);
}
/* stop get venc stream process. */
td_s32 sample_comm_venc_stop_get_stream(td_s32 chn_num)
{
    td_s32 i;
    for (i = 0; i < chn_num; i++) {
        if (ss_mpi_venc_stop_chn(i) != TD_SUCCESS) {
            sample_print("chn %d ss_mpi_venc_stop_recv_pic failed!\n", i);
            return TD_FAILURE;
        }
    }
    if (g_para.thread_start == TD_TRUE) {
        g_para.thread_start = TD_FALSE;
        pthread_join(g_venc_pid, 0);
    }
    return TD_SUCCESS;
}
td_s32 sample_comm_venc_stop_send_qpmap_frame(td_void)
{
    if (g_qpmap_send_frame_para.thread_start == TD_TRUE) {
        g_qpmap_send_frame_para.thread_start = TD_FALSE;
        pthread_join(g_venc_qpmap_pid, 0);
    }
    return TD_SUCCESS;
}
td_s32 sample_comm_venc_stop_send_roimap_frame(td_void)
{
    if (g_roimap_frame_param.thread_start == TD_TRUE) {
        g_roimap_frame_param.thread_start = TD_FALSE;
        pthread_join(g_venc_roimap_pid, 0);
    }
    return TD_SUCCESS;
}
td_s32 sample_comm_venc_stop_rateauto(ot_venc_chn chn[], td_s32 cnt)
{
    td_s32 i;
    td_s32 ret;

    prctl(PR_SET_NAME, "sample_comm_venc_stop_rateauto", 0, 0, 0);

    ot_venc_svc_param svc_param = {0};

    ot_venc_chn venc_chn;
    ot_venc_svc_rect_info  svc_rect_info = {0};
    if (g_venc_rateauto_frame_param.thread_start == TD_TRUE) {
        g_venc_rateauto_frame_param.thread_start = TD_FALSE;
        pthread_join(g_venc_rateauto_pid, 0);
        for (i = 0; i < cnt; i++) {
            venc_chn = chn[i];
            ret = ss_mpi_venc_enable_svc(venc_chn, TD_TRUE);
            if (ret != TD_SUCCESS) {
                sample_print("Set ss_mpi_venc_enable_svc failed for %#x chn =%d\n", ret, venc_chn);
            }
            if (memset_s(&svc_rect_info, sizeof(ot_venc_svc_rect_info), 0, sizeof(ot_venc_svc_rect_info)) != EOK) {
                printf("call memset_s error\n");
            }
            ret = ss_mpi_venc_send_svc_region(venc_chn, &svc_rect_info);
            if (ret != TD_SUCCESS) {
                sample_print("Set ss_mpi_venc_send_svc_region failed for %#x chn =%d\n", ret, venc_chn);
            }
            ret = ss_mpi_venc_get_svc_param(venc_chn, &svc_param);
            if (ret != TD_SUCCESS) {
                sample_print("Set ss_mpi_venc_set_svc_param failed for %#x chn =%d\n", ret, venc_chn);
            }
            if (memset_s(&svc_param, sizeof(ot_venc_svc_param), 0, sizeof(ot_venc_svc_param)) != EOK) {
                printf("call memset_s error\n");
            }
            ret = ss_mpi_venc_set_svc_param(venc_chn, &svc_param);
            if (ret != TD_SUCCESS) {
                sample_print("Set ss_mpi_venc_set_svc_param failed for %#x chn =%d\n", ret, venc_chn);
            }
            ret = ss_mpi_venc_enable_svc(venc_chn, TD_FALSE);
            if (ret != TD_SUCCESS) {
                sample_print("Set ss_mpi_venc_enable_svc failed for %#x chn =%d\n", ret, venc_chn);
            }
        }
    }
    return TD_SUCCESS;
}
td_s32 sample_comm_venc_plan_to_semi(td_u8 *u, td_s32 u_stride, td_u8 *v, td_s32 pic_height)
{
    td_s32 i;
    td_u8 *tmp_u = TD_NULL;
    td_u8 *ptu = TD_NULL;
    td_u8 *tmp_v = TD_NULL;
    td_u8 *ptv = TD_NULL;
    td_s32 haf_w = u_stride / 2; /* 2: half */
    td_s32 haf_h = pic_height / 2; /* 2: half */
    td_s32 size = haf_w * haf_h;

    tmp_u = malloc(size);
    if (tmp_u == TD_NULL) {
        printf("malloc buf failed\n");
        return TD_FAILURE;
    }
    ptu = tmp_u;

    tmp_v = malloc(size);
    if (tmp_v == TD_NULL) {
        printf("malloc buf failed\n");
        free(tmp_u);
        return TD_FAILURE;
    }
    ptv = tmp_v;

    if (memcpy_s(tmp_u, size, u, size) != EOK) {
        printf("call memcpy_s error\n");
        free(tmp_u);
        free(tmp_v);
        return TD_FAILURE;
    }
    if (memcpy_s(tmp_v, size, v, size) != EOK) {
        printf("call memcpy_s error\n");
        free(tmp_u);
        free(tmp_v);
        return TD_FAILURE;
    }
    for (i = 0; i < (size  / 2); i++) { /* 2: half */
        *u++ = *tmp_v++;
        *u++ = *tmp_u++;
    }
    for (i = 0; i < (size  / 2); i++) { /* 2: half */
        *v++ = *tmp_v++;
        *v++ = *tmp_u++;
    }
    free(ptu);
    free(ptv);
    return TD_SUCCESS;
}
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART08_H_ */
