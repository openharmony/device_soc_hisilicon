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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART06_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART06_H_

    td_u8 tmp1;
    td_u8 tmp2;
    tmp1 = *wp;

    tmp2 = ~(3 << (idx * SAMPLE_VENC_ONE_BLOCK_BITS)); // 3: 0b11
    tmp1 &= tmp2;
    tmp2 = value << (idx * SAMPLE_VENC_ONE_BLOCK_BITS);
    tmp1 |= tmp2;
    *wp = tmp1;
}

static td_void sample_venc_process_jpeg_roi_head(td_u8 **wp, td_u32 start_idx, td_u32 fill_block, td_u32 value)
{
    td_u32 i;

    for (i = 0; i < fill_block; i++) {
        sample_venc_set_2bits(*wp, start_idx + i, value);
    }

    if (i != 0) {
        *wp = *wp + 1;
    }
}

static td_void sample_venc_process_jpeg_roi_middle(td_u8 **wp, td_u32 write_byte, td_u32 value)
{
    td_s32 i;
    if (write_byte != 0) {
        for (i = 0; i < (SAMPLE_VENC_ONE_BYTE_BLOCKS - 1); i++) {
            value |= value << SAMPLE_VENC_ONE_BLOCK_BITS;
        }
        if (memset_s(*wp, write_byte, value, write_byte) != EOK) {
            printf("call memset_s error\n");
        }
        *wp = *wp + write_byte;
    }
}

static td_void sample_venc_process_jpeg_roi_tail(td_u8 **wp, td_u32 fill_block, td_u32 value)
{
    td_u32 i;

    for (i = 0; i < fill_block; i++) {
        sample_venc_set_2bits(*wp, i, value);
    }
}

td_void sample_venc_process_jpeg_roi(td_void *virt_addr, ot_rect *rect, td_u32 roi_level, td_u32 stride)
{
    td_u32 i;
    td_u32 level;
    td_u32 start_block;
    td_u32 start_byte;
    td_u32 start_supple_idx;
    td_u32 start_supple_block;
    td_u32 start_block_height;
    td_u32 start_x;
    td_u32 start_y;
    td_u32 write_block;
    td_u32 write_resi_block;
    td_u32 write_byte;
    td_u32 write_tail_block;
    td_u32 write_block_height;

    start_x = rect->x;
    start_block = start_x / SAMPLE_VENC_BLOCK_WIDTH;
    start_byte = start_block / SAMPLE_VENC_ONE_BYTE_BLOCKS;
    start_supple_idx = start_block % SAMPLE_VENC_ONE_BYTE_BLOCKS;
    start_supple_block = (start_supple_idx == 0) ? 0 : (SAMPLE_VENC_ONE_BYTE_BLOCKS - start_supple_idx);

    write_block = rect->width / SAMPLE_VENC_BLOCK_WIDTH;
    start_supple_block = (start_supple_block < write_block) ? start_supple_block : write_block;
    write_resi_block = write_block - start_supple_block;
    write_byte = write_resi_block / SAMPLE_VENC_ONE_BYTE_BLOCKS;
    write_tail_block = write_resi_block % SAMPLE_VENC_ONE_BYTE_BLOCKS;

    start_y = rect->y;
    start_block_height = start_y / SAMPLE_VENC_BLOCK_HEIGHT;
    write_block_height = rect->height / SAMPLE_VENC_BLOCK_HEIGHT;
    level = roi_level;

    for (i = 0; i < write_block_height; i++) {
        td_u8 *wp = (td_u8 *)virt_addr + (start_block_height + i) * stride + start_byte;
        sample_venc_process_jpeg_roi_head(&wp, start_supple_idx, start_supple_block, level);
        sample_venc_process_jpeg_roi_middle(&wp, write_byte, level);
        sample_venc_process_jpeg_roi_tail(&wp, write_tail_block, level);
    }
}

#define SAMPLE_VENC_ROIMAP_MAX_CHN 2

static td_s32 sample_comm_venc_send_frame_ex(sample_venc_roimap_frame_para *para, ot_venc_user_frame_info *frame_info,
    ot_video_frame_info *video_frame, td_s32 index, td_s32 venc_roimap_max_chn)
{
    td_s32 ret;
    ot_unused(venc_roimap_max_chn);

    ret = ss_mpi_venc_send_frame_ex(para->venc_chn[index], &frame_info[index], -1);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_send_frame_ex err: 0x%x\n", ret);

        ret = ss_mpi_vpss_release_chn_frame(para->vpss_grp, para->vpss_chn[index], video_frame);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vpss_release_chn_frame err: 0x%x", ret);
            return SAMPLE_RETURN_GOTO;
        }
        return SAMPLE_RETURN_BREAK;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_venc_send_roimap_frame_start(sample_venc_roimap_frame_para *para,
    td_phys_addr_t *roimap_phys_addr)
{
    td_s32 i;
    td_s32 ret;
    ot_venc_user_frame_info frame_info[SAMPLE_VENC_ROIMAP_MAX_CHN] = {0};

    while (para->thread_start == TD_TRUE) {
        for (i = 0; (i < para->cnt) && (i < SAMPLE_VENC_ROIMAP_MAX_CHN) &&
                    (i < OT_VPSS_MAX_PHYS_CHN_NUM) && (i < OT_VENC_MAX_CHN_NUM); i++) {
            ot_video_frame_info *video_frame = &frame_info[i].user_frame;
            ret = ss_mpi_vpss_get_chn_frame(para->vpss_grp, para->vpss_chn[i],
                video_frame, 1000); /* 1000 is a number */
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_get_chn_frame err: 0x%x\n", ret);
                continue;
            }

            frame_info[i].user_roimap.valid = TD_TRUE;
            frame_info[i].user_roimap.phys_addr = roimap_phys_addr[i];

            ret = sample_comm_venc_send_frame_ex(para, frame_info, video_frame, i, SAMPLE_VENC_ROIMAP_MAX_CHN);
            if (ret == SAMPLE_RETURN_BREAK) {
                break;
            } else if (ret == SAMPLE_RETURN_GOTO) {
                return SAMPLE_RETURN_GOTO;
            }

            ret = ss_mpi_vpss_release_chn_frame(para->vpss_grp, para->vpss_chn[i], video_frame);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_release_chn_frame err: 0x%x", ret);
                return SAMPLE_RETURN_GOTO;
            }
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_set_vpss_buffer_depth(sample_venc_roimap_frame_para *para, td_phys_addr_t *roimap_phys_addr)
{
    td_s32 i;
    td_s32 ret;
    ot_vpss_chn_attr vpss_chn_attr;

    for (i = 0; (i < para->cnt) && (i < OT_VPSS_MAX_PHYS_CHN_NUM); i++) {
        ret = ss_mpi_vpss_get_chn_attr(para->vpss_grp, para->vpss_chn[i], &vpss_chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vpss_get_chn_attr err: 0x%x", ret);
            return SAMPLE_RETURN_GOTO;
        }

        vpss_chn_attr.depth = 3; // 3 : depth
        ret = ss_mpi_vpss_set_chn_attr(para->vpss_grp, para->vpss_chn[i], &vpss_chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vpss_set_chn_attr err: 0x%x", ret);
            return SAMPLE_RETURN_GOTO;
        }
    }
    ret = sample_comm_venc_send_roimap_frame_start(para, roimap_phys_addr);
    if (ret != TD_SUCCESS) {
        return SAMPLE_RETURN_GOTO;
    }

    return TD_SUCCESS;
}

static td_u8 sample_comm_get_roimap_init_level(td_u8 init_level)
{
    td_s32 j;

    for (j = 0; j < (4 - 1); j++) { /* 4 is a number */
        init_level |= (init_level << 4); /* 4 is a number */
    }

    return init_level;
}

typedef struct {
    td_u32 *roimap_size;
    td_u32 *roimap_stride;
    td_phys_addr_t *roimap_phys_addr;
    td_void **roimap_virt_addr;
    td_u8 *init_level;
} sample_venc_roimap_buf_ctx;

static td_s32 sample_comm_alloc_roimap(sample_venc_roimap_frame_para *para, td_s32 index,
    sample_venc_roimap_buf_ctx *buf_ctx)
{
    td_s32 ret;
    td_u8 *virt_addr = TD_NULL;
    td_phys_addr_t phys_addr = TD_NULL;
    ot_venc_chn_attr venc_chn_attr;

    ss_mpi_venc_get_chn_attr(para->venc_chn[index], &venc_chn_attr);

    buf_ctx->roimap_size[index] =
        ot_venc_get_roimap_size(venc_chn_attr.venc_attr.type, para->size[index].width, para->size[index].width);
    buf_ctx->roimap_stride[index] = ot_venc_get_roimap_stride(venc_chn_attr.venc_attr.type, para->size[index].width);

    /* alloc roimap memory */
    ret = ss_mpi_sys_mmz_alloc(&phys_addr, (td_void **)&virt_addr, TD_NULL, TD_NULL,
        buf_ctx->roimap_size[index]);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_sys_mmz_alloc err: 0x%x", ret);
        return TD_FAILURE;
    }

    buf_ctx->roimap_phys_addr[index] = phys_addr;
    buf_ctx->roimap_virt_addr[index] = virt_addr;

    *(buf_ctx->init_level) = sample_comm_get_roimap_init_level(*(buf_ctx->init_level));
    if (memset_s(buf_ctx->roimap_virt_addr[index], buf_ctx->roimap_size[index], *(buf_ctx->init_level),
        buf_ctx->roimap_size[index]) != EOK) {
        printf("call memset_s error\n");
    }
    sample_venc_process_jpeg_roi(buf_ctx->roimap_virt_addr[index], &para->roi_attr[index].rect,
        para->roi_attr[index].level, buf_ctx->roimap_stride[index]);

    return TD_SUCCESS;
}

static td_void sample_comm_free_roimap(sample_venc_roimap_frame_para *para, td_phys_addr_t *roimap_phys_addr,
    td_void **roimap_virt_addr)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; (i < para->cnt) && (i < SAMPLE_VENC_ROIMAP_MAX_CHN); i++) {
        if (roimap_phys_addr[i] != 0) {
            ret = ss_mpi_sys_mmz_free(roimap_phys_addr[i], roimap_virt_addr[i]);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_sys_mmz_free err: 0x%x", ret);
            }
        }
    }
}

td_void *sample_comm_send_roimap_frame_proc(td_void *p)
{
    td_s32 i;
    td_u8 init_level = 2;
    td_u32 roimap_size[SAMPLE_VENC_ROIMAP_MAX_CHN];
    td_u32 roimap_stride[SAMPLE_VENC_ROIMAP_MAX_CHN];
    td_phys_addr_t roimap_phys_addr[SAMPLE_VENC_ROIMAP_MAX_CHN] = {0};
    td_void *roimap_virt_addr[SAMPLE_VENC_ROIMAP_MAX_CHN] = {0};
    sample_venc_roimap_frame_para *para = (sample_venc_roimap_frame_para *)p;
    sample_venc_roimap_buf_ctx buf_ctx;

    if (para->cnt > SAMPLE_VENC_ROIMAP_MAX_CHN) {
        sample_print("Current not support venc channal num(%d) > %d\n", para->cnt, SAMPLE_VENC_ROIMAP_MAX_CHN);
        return TD_NULL;
    }

    buf_ctx.roimap_size = roimap_size;
    buf_ctx.roimap_stride = roimap_stride;
    buf_ctx.roimap_phys_addr = roimap_phys_addr;
    buf_ctx.roimap_virt_addr = roimap_virt_addr;
    buf_ctx.init_level = &init_level;

    for (i = 0; (i < para->cnt) && (i < OT_VENC_MAX_CHN_NUM) && (i < SAMPLE_VENC_ROIMAP_MAX_CHN); i++) {
        if (sample_comm_alloc_roimap(para, i, &buf_ctx) != TD_SUCCESS) {
            return TD_NULL;
        }
    }

    /* set vpss buffer depth */
    if (sample_comm_set_vpss_buffer_depth(para, roimap_phys_addr) != TD_SUCCESS) {
        goto error;
    }

error:
    sample_comm_free_roimap(para, roimap_phys_addr, roimap_virt_addr);
    return TD_NULL;
}

td_s32 sample_comm_venc_send_roimap_frame(ot_vpss_grp vpss_grp, sample_venc_roimap_chn_info roimap_chn_info,
    ot_size size[], ot_venc_jpeg_roi_attr roi_attr[])
{
    td_s32 i;

    g_roimap_frame_param.thread_start = TD_TRUE;
    g_roimap_frame_param.vpss_grp = vpss_grp;
    g_roimap_frame_param.cnt = roimap_chn_info.cnt;

    for (i = 0; (i < roimap_chn_info.cnt) && (i < OT_VENC_MAX_CHN_NUM) && (i < OT_VPSS_MAX_PHYS_CHN_NUM); i++) {
        g_roimap_frame_param.venc_chn[i] = roimap_chn_info.venc_chn[i];
        g_roimap_frame_param.vpss_chn[i] = roimap_chn_info.vpss_chn[i];
        g_roimap_frame_param.size[i] = size[i];
        g_roimap_frame_param.roi_attr[i] = roi_attr[i];
    }

    return pthread_create(&g_venc_roimap_pid, 0, sample_comm_send_roimap_frame_proc, (td_void *)&g_roimap_frame_param);
}

static td_s32 sample_comm_set_file_name(td_s32 index, ot_venc_chn venc_chn,
    sample_comm_venc_stream_proc_info *stream_proc_info)
{
    if (snprintf_s(stream_proc_info->file_name[index], FILE_NAME_LEN, FILE_NAME_LEN - 1, "./") < 0) {
        return SAMPLE_RETURN_NULL;
    }

    if (sample_comm_venc_get_realpath(stream_proc_info->file_name[index], stream_proc_info->real_file_name[index],
        sizeof(stream_proc_info->real_file_name[index])) != TD_SUCCESS) {
        sample_print("chn[%d] stream file path error\n", venc_chn);
        return SAMPLE_RETURN_NULL;
    }

    if (snprintf_s(stream_proc_info->real_file_name[index], FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "stream_chn%d%s", index, stream_proc_info->file_postfix) < 0) {
        return SAMPLE_RETURN_NULL;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_set_name_save_stream(sample_comm_venc_stream_proc_info *stream_proc_info,
    ot_venc_stream_buf_info *stream_buf_info, ot_payload_type *payload_type,
    sample_venc_getstream_para *para, td_s32 venc_max_chn)
{
    td_s32 i;
    td_s32 ret;
    td_s32 fd;
    ot_venc_chn_attr venc_chn_attr;
    ot_unused(venc_max_chn);

    for (i = 0; (i < stream_proc_info->chn_total) && (i < OT_VENC_MAX_CHN_NUM); i++) {
        /* decide the stream file name, and open file to save stream */
        ot_venc_chn venc_chn = para->venc_chn[i];
        ret = ss_mpi_venc_get_chn_attr(venc_chn, &venc_chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_venc_get_chn_attr chn[%d] failed with %#x!\n", venc_chn, ret);
            return SAMPLE_RETURN_NULL;
        }
        payload_type[i] = venc_chn_attr.venc_attr.type;

        ret = sample_comm_venc_get_file_postfix(payload_type[i], stream_proc_info->file_postfix,
            sizeof(stream_proc_info->file_postfix));
        if (ret != TD_SUCCESS) {
            sample_print("sample_comm_venc_get_file_postfix [%d] failed with %#x!\n",
                venc_chn_attr.venc_attr.type, ret);
            return SAMPLE_RETURN_NULL;
        }

        if (payload_type[i] != OT_PT_JPEG) {
            ret = sample_comm_set_file_name(i, venc_chn, stream_proc_info);
            if (ret != TD_SUCCESS) {
                return ret;
            }

            stream_proc_info->file[i] = fopen(stream_proc_info->real_file_name[i], "wb");
            if (!stream_proc_info->file[i]) {
                sample_print("open file[%s] failed!\n", stream_proc_info->real_file_name[i]);
                return SAMPLE_RETURN_NULL;
            }
            fd = fileno(stream_proc_info->file[i]);
            fchmod(fd, S_IRUSR | S_IWUSR);
        }
        /* set venc fd. */
        stream_proc_info->venc_fd[i] = ss_mpi_venc_get_fd(i);
        if (stream_proc_info->venc_fd[i] < 0) {
            sample_print("ss_mpi_venc_get_fd failed with %#x!\n", stream_proc_info->venc_fd[i]);
            return SAMPLE_RETURN_NULL;
        }

        if (stream_proc_info->maxfd <= stream_proc_info->venc_fd[i]) {
            stream_proc_info->maxfd = stream_proc_info->venc_fd[i];
        }

        ret = ss_mpi_venc_get_stream_buf_info(i, &stream_buf_info[i]);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_venc_get_stream_buf_info failed with %#x!\n", ret);
            return SAMPLE_RETURN_FAILURE;
        }
    }

    return TD_SUCCESS;
}


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART06_H_ */
