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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART05_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART05_H_
    }
    fd = open(stream_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        sample_print("open file err\n");
        free(stream.pack);
        stream.pack = NULL;
        return TD_FAILURE;
    }
    file = fdopen(fd, "wb");
    if (file == NULL) {
        sample_print("fdopen err\n");
        goto error;
    }
    ret = sample_comm_venc_save_stream(file, &stream);
    if (ret != TD_SUCCESS) {
        sample_print("save snap picture failed!\n");
        goto error;
    }
    if (save_thm) {
        char file_dcf[FILE_NAME_LEN]  = {0};
        if (snprintf_s(file_dcf, FILE_NAME_LEN, FILE_NAME_LEN - 1, "snap_thm_%d.jpg", g_snap_cnt) < 0) {
            goto error;
        }
        ret = sample_comm_venc_get_dcf_info(stream_file, FILE_NAME_LEN, file_dcf, FILE_NAME_LEN);
        if (ret != TD_SUCCESS) {
            sample_print("save thm picture failed!\n");
            goto error;
        }
    }
    (td_void)fclose(file);
    file = TD_NULL;
    g_snap_cnt++;

    return TD_SUCCESS;

error:
    free(stream.pack);
    stream.pack = NULL;

    (td_void)fclose(file);
    file = TD_NULL;

    return TD_FAILURE;
}
static td_s32 sample_comm_get_snap_stream(ot_venc_chn venc_chn, td_bool save_jpg, td_bool save_thm)
{
    td_s32 ret;
    ot_venc_chn_status stat;
    ot_venc_stream stream;

    ret = ss_mpi_venc_query_status(venc_chn, &stat);
    if (ret != TD_SUCCESS) {
        sample_print("query_status failed with %#x!\n", ret);
        return SAMPLE_RETURN_BREAK;
    }
    if (stat.cur_packs == 0) {
        return SAMPLE_RETURN_CONTINUE;
    }
    stream.pack = (ot_venc_pack *)malloc(sizeof(ot_venc_pack) * stat.cur_packs);
    if (stream.pack == NULL) {
        sample_print("malloc memory failed!\n");
        return SAMPLE_RETURN_BREAK;
    }
    stream.pack_cnt = stat.cur_packs;
    ret = ss_mpi_venc_get_stream(venc_chn, &stream, -1);
    if (ret != TD_SUCCESS) {
        sample_print("get_stream failed with %#x!\n", ret);
        free(stream.pack);
        stream.pack = NULL;
        return SAMPLE_RETURN_BREAK;
    }
    if (save_jpg || save_thm) {
        ret = sample_comm_save_snap_stream(stream, save_jpg, save_thm);
        if (ret != TD_SUCCESS) {
            sample_print("save_snap_stream failed!\n");
            return SAMPLE_RETURN_BREAK;
        }
    }
    ret = ss_mpi_venc_release_stream(venc_chn, &stream);
    if (ret != TD_SUCCESS) {
        sample_print("release_stream failed with %#x!\n", ret);

        free(stream.pack);
        stream.pack = NULL;
        return SAMPLE_RETURN_BREAK;
    }
    free(stream.pack);
    stream.pack = NULL;

    return TD_SUCCESS;
}
/******************************************************************************
* funciton : snap process
******************************************************************************/
td_s32 sample_comm_venc_snap_process(ot_venc_chn venc_chn, td_u32 snap_cnt, td_bool save_jpg, td_bool save_thm)
{
    td_u32 i;
    td_s32 ret;
#ifndef CONFIG_USER_SPACE
    osal_fd_set read_fds;
    td_s32 venc_fd = 0;
    struct timeval timeout_val;

    /******************************************
     step 4:  recv picture
    ******************************************/

    venc_fd = ss_mpi_venc_get_fd(venc_chn);
    if (venc_fd < 0) {
        sample_print("venc_get_fd faild with%#x!\n", venc_fd);
        return TD_FAILURE;
    }
    for (i = 0; i < snap_cnt; i++) {
        OSAL_FD_ZERO(&read_fds);
        OSAL_FD_SET(venc_fd, &read_fds);
        timeout_val.tv_sec  = 10; // 10 : 10 seconds
        timeout_val.tv_usec = 0;
        ret = OSAL_SELECT(venc_fd + 1, &read_fds, NULL, NULL, &timeout_val);
        if (ret < 0) {
            sample_print("snap select failed!\n");
            return TD_FAILURE;
        } else if (ret == 0) {
            sample_print("snap time out!\n");
            return TD_FAILURE;
        } else {
            if (OSAL_FD_ISSET(venc_fd, &read_fds)) {
                check_return(sample_comm_get_snap_stream(venc_chn, save_jpg, save_thm), "get_snap_stream");
            }
        }
    }
    return TD_SUCCESS;
#else
    i = 0;
    while (i < snap_cnt) {
        ret = sample_comm_get_snap_stream(venc_chn, save_jpg, save_thm);
        if (ret == SAMPLE_RETURN_BREAK) {
            break;
        } else if (ret == SAMPLE_RETURN_CONTINUE) {
            usleep(5000); /* 5000 */
            continue;
        } else {
            i++;
        }
    }
    return TD_SUCCESS;
#endif
}
td_s32 sample_comm_venc_save_jpeg(ot_venc_chn venc_chn, td_u32 snap_cnt)
{
    struct timeval timeout_val;
    osal_fd_set read_fds;
    td_s32 venc_fd;
    td_s32 ret;
    td_u32 i;
    /******************************************
     step:  recv picture
    ******************************************/
    venc_fd = ss_mpi_venc_get_fd(venc_chn);
    if (venc_fd < 0) {
        sample_print("venc_get_fd faild with%#x!\n", venc_fd);
        return TD_FAILURE;
    }
    for (i = 0; i < snap_cnt; i++) {
        OSAL_FD_ZERO(&read_fds);
        OSAL_FD_SET(venc_fd, &read_fds);
        timeout_val.tv_sec  = 10; // 10 : 10 seconds
        timeout_val.tv_usec = 0;
        ret = OSAL_SELECT(venc_fd + 1, &read_fds, NULL, NULL, &timeout_val);
        if (ret < 0) {
            sample_print("snap select failed!\n");
            return TD_FAILURE;
        } else if (ret == 0) {
            sample_print("snap time out!\n");
            return TD_FAILURE;
        } else {
            if (OSAL_FD_ISSET(venc_fd, &read_fds)) {
                check_return(sample_comm_get_snap_stream(venc_chn, TD_TRUE, TD_FALSE), "get_snap_stream");
            }
        }
    }
    return TD_SUCCESS;
}
static td_s32 sample_comm_alloc_qpmap_skipweight_memory(sample_venc_qpmap_sendframe_para *para,
    sample_comm_venc_frame_proc_info *addr_info)
{
    ot_venc_chn_attr venc_chn_attr;
    td_u32 j;
    td_s32 i;
    td_s32 ret;
    td_u8 *vir_addr = TD_NULL;
    td_phys_addr_t phys_addr = 0;

    for (i = 0; (i < para->cnt) && (i < OT_VENC_MAX_CHN_NUM); i++) {
        ss_mpi_venc_get_chn_attr(para->venc_chn[i], &venc_chn_attr);

        addr_info->qpmap_size[i] = ot_venc_get_qpmap_size(venc_chn_attr.venc_attr.type,
            para->size[i].width, para->size[i].height);
        addr_info->skip_weight_size[i] =
            ot_venc_get_skip_weight_size(venc_chn_attr.venc_attr.type, para->size[i].width, para->size[i].height);

        /* alloc qpmap memory */
        ret = ss_mpi_sys_mmz_alloc((td_phys_addr_t *)&phys_addr, (td_void **)&vir_addr, TD_NULL, TD_NULL,
            addr_info->qpmap_size[i] * QPMAP_BUF_NUM);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_sys_mmz_alloc err:0x%x", ret);
            return ret;
        }
        for (j = 0; (i < VENC_QPMAP_MAX_CHN) && (j < QPMAP_BUF_NUM); j++) {
            if ((j > 0) && (addr_info->qpmap_size[i] > (UINT_MAX / j))) {
                sample_print("(j * addr_info->qpmap_size[%d]) upper limit of the multiplie\n", i);
                ss_mpi_sys_mmz_free(phys_addr, vir_addr);
                return TD_FAILURE;
            } else {
                addr_info->qpmap_phys_addr[i][j] = (td_phys_addr_t)(phys_addr + j * addr_info->qpmap_size[i]);
                addr_info->qpmap_vir_addr[i][j] = vir_addr + j * addr_info->qpmap_size[i];
            }
        }
        /* alloc skipWeight memory */
        ret = ss_mpi_sys_mmz_alloc((td_phys_addr_t *)&phys_addr, (td_void **)&vir_addr, TD_NULL, TD_NULL,
            addr_info->skip_weight_size[i] * QPMAP_BUF_NUM);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_sys_mmz_alloc err:0x%x", ret);
            return ret;
        }
        for (j = 0; (i < VENC_QPMAP_MAX_CHN) && (j < QPMAP_BUF_NUM); j++) {
            if ((j > 0) && (addr_info->skip_weight_size[i] > (UINT_MAX / j))) {
                sample_print("(j * addr_info->skip_weight_size[%d]) upper limit of the multiplie\n", i);
                ss_mpi_sys_mmz_free(phys_addr, vir_addr);
                return TD_FAILURE;
            } else {
                addr_info->skip_weight_phys_addr[i][j] =
                    (td_phys_addr_t)(phys_addr + j * addr_info->skip_weight_size[i]);
                addr_info->skip_weight_vir_addr[i][j] = vir_addr + j * addr_info->skip_weight_size[i];
            }
        }
    }
    return TD_SUCCESS;
}
static td_void sample_comm_venc_vir_addr_temp(sample_comm_venc_frame_proc_info *addr_info, td_u32 i, td_u32 frame_id)
{
    td_u32 j;
    td_u8 *vir_addr_temp = TD_NULL;

    vir_addr_temp = (td_u8 *)addr_info->qpmap_vir_addr[i][frame_id];
    for (j = 0; j < addr_info->qpmap_size[i]; j++) {
        *vir_addr_temp = 0x5E; // [7]:skip flag; [6]:QpType Flag; [5:0]:Qp value ==> Set absolute qp = 30
        vir_addr_temp++;
    }
    vir_addr_temp = (td_u8 *)addr_info->skip_weight_vir_addr[i][frame_id];
    for (j = 0; j < addr_info->skip_weight_size[i]; j++) {
        *vir_addr_temp = 0x66; // inter block must be skip
        vir_addr_temp++;
    }
}
static td_s32 sample_comm_qpmap_send_frame_ex(sample_venc_qpmap_sendframe_para *para,
    ot_venc_user_frame_info *frame_info, ot_video_frame_info *video_frame, td_s32 index)
{
    td_s32 ret;

    ret = ss_mpi_venc_send_frame_ex(para->venc_chn[index], frame_info, -1);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_send_frame_ex err:0x%x\n", ret);

        ret = ss_mpi_vpss_release_chn_frame(para->vpss_grp, para->vpss_chn[index], video_frame);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vpss_release_chn_frame err:0x%x", ret);
            return SAMPLE_RETURN_GOTO;
        }
        return SAMPLE_RETURN_BREAK;
    }
    return TD_SUCCESS;
}
static td_s32 sample_comm_qpmap_send_frame_start(sample_venc_qpmap_sendframe_para *para,
    sample_comm_venc_frame_proc_info *addr_info)
{
    td_u32 frame_id = 0;
    td_s32 i;
    td_s32 ret;
    ot_venc_user_frame_info frame_info[VENC_QPMAP_MAX_CHN][QPMAP_BUF_NUM] = { 0 };

    while (para->thread_start == TD_TRUE) {
        for (i = 0; (i < para->cnt) && (i < VENC_QPMAP_MAX_CHN); i++) {
            ot_video_frame_info *video_frame = &frame_info[i][frame_id].user_frame;
            ret = ss_mpi_vpss_get_chn_frame(para->vpss_grp, para->vpss_chn[i],
                video_frame, 1000); /* 1000 is a number */
            if (ret != TD_SUCCESS) {
                sample_print("OT_MPI_VPSS_GetChnFrame err:0x%x\n", ret);
                continue;
            }
            sample_comm_venc_vir_addr_temp(addr_info, i, frame_id);

            frame_info[i][frame_id].user_rc_info.skip_weight_valid = 1;
            frame_info[i][frame_id].user_rc_info.skip_weight_phys_addr = addr_info->skip_weight_phys_addr[i][frame_id];
            frame_info[i][frame_id].user_rc_info.qpmap_valid = 1;
            frame_info[i][frame_id].user_rc_info.qpmap_phys_addr = addr_info->qpmap_phys_addr[i][frame_id];
            frame_info[i][frame_id].user_rc_info.blk_start_qp = 30; /* 30 is a number */
            frame_info[i][frame_id].user_rc_info.frame_type = OT_VENC_FRAME_TYPE_NONE;

            ret = sample_comm_qpmap_send_frame_ex(para, &frame_info[i][frame_id], video_frame, i);
            if (ret == SAMPLE_RETURN_BREAK) {
                break;
            } else if (ret == SAMPLE_RETURN_GOTO) {
                return SAMPLE_RETURN_GOTO;
            }
            ret = ss_mpi_vpss_release_chn_frame(para->vpss_grp, para->vpss_chn[i], video_frame);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_release_chn_frame err:0x%x", ret);
                return SAMPLE_RETURN_GOTO;
            }
            frame_id++;
            if (frame_id >= QPMAP_BUF_NUM) {
                frame_id = 0;
            }
        }
    }
    return TD_SUCCESS;
}
td_void *sample_comm_qpmap_send_frame_proc(td_void *p)
{
    td_s32 i;
    td_s32 ret;
    sample_venc_qpmap_sendframe_para *para = TD_NULL;
    sample_comm_venc_frame_proc_info addr_info = { 0 };
    ot_vpss_chn_attr vpss_chn_attr;

    para = (sample_venc_qpmap_sendframe_para *)p;

    if (para->cnt > VENC_QPMAP_MAX_CHN) {
        sample_print("Current func'sample_comm_qpmap_send_frame_proc' not support Venc channal num(%d) > %d\n",
            para->cnt, VENC_QPMAP_MAX_CHN);
        return TD_NULL;
    }
    if (sample_comm_alloc_qpmap_skipweight_memory(para, &addr_info) != TD_SUCCESS) {
        goto error;
    }
    /* set vpss buffer depth */
    for (i = 0; (i < para->cnt) && (i < OT_VPSS_MAX_PHYS_CHN_NUM); i++) {
        ret = ss_mpi_vpss_get_chn_attr(para->vpss_grp, para->vpss_chn[i], &vpss_chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vpss_get_chn_attr err:0x%x", ret);
            goto error;
        }
        vpss_chn_attr.depth = 3; /* 3 is a number */
        ret = ss_mpi_vpss_set_chn_attr(para->vpss_grp, para->vpss_chn[i], &vpss_chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vpss_set_chn_attr err:0x%x", ret);
            goto error;
        }
    }
        if (sample_comm_qpmap_send_frame_start(para, &addr_info) != TD_SUCCESS) {
            goto error;
        }
error:
    for (i = 0; (i < para->cnt) && (i < VENC_QPMAP_MAX_CHN); i++) {
        if (addr_info.qpmap_phys_addr[i][0] != 0) {
            ret = ss_mpi_sys_mmz_free(addr_info.qpmap_phys_addr[i][0], addr_info.qpmap_vir_addr[i][0]);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_sys_mmz_free err:0x%x", ret);
            }
        }
        if (addr_info.skip_weight_phys_addr[i][0] != 0) {
            ret = ss_mpi_sys_mmz_free(addr_info.skip_weight_phys_addr[i][0], addr_info.skip_weight_vir_addr[i][0]);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_sys_mmz_free err:0x%x", ret);
            }
        }
    }
    return TD_NULL;
}
td_s32 sample_comm_venc_qpmap_send_frame(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn[], ot_venc_chn venc_chn[],
    td_s32 cnt, ot_size size[])
{
    td_s32 i;

    g_qpmap_send_frame_para.thread_start = TD_TRUE;
    g_qpmap_send_frame_para.vpss_grp = vpss_grp;
    g_qpmap_send_frame_para.cnt = cnt;

    for (i = 0; (i < cnt) && (i < OT_VENC_MAX_CHN_NUM) && (i < OT_VPSS_MAX_PHYS_CHN_NUM); i++) {
        g_qpmap_send_frame_para.venc_chn[i] = venc_chn[i];
        g_qpmap_send_frame_para.vpss_chn[i] = vpss_chn[i];
        g_qpmap_send_frame_para.size[i] = size[i];
    }
    return pthread_create(&g_venc_qpmap_pid, 0, sample_comm_qpmap_send_frame_proc, (td_void *)&g_qpmap_send_frame_para);
}
#define SAMPLE_VENC_BLOCK_WIDTH 16
#define SAMPLE_VENC_BLOCK_HEIGHT 16
#define SAMPLE_VENC_ONE_BYTE_BLOCKS 4
#define SAMPLE_VENC_ONE_BLOCK_BITS 2
#define SAMPLE_VENC_MAX_JPEG_ROI_LEVEL 3

static td_void sample_venc_set_2bits(td_u8 *wp, td_u32 idx, td_u32 value)
{
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART05_H_ */
