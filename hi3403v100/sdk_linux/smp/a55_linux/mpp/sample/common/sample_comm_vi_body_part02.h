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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART02_H_

        pipe_info[i].chn_num = 1;
        pipe_info[i].chn_info[0].vi_chn                                  = 0;
        pipe_info[i].chn_info[0].chn_attr.size.width                     = size->width;
        pipe_info[i].chn_info[0].chn_attr.size.height                    = size->height;
        pipe_info[i].chn_info[0].chn_attr.pixel_format                   = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
        pipe_info[i].chn_info[0].chn_attr.dynamic_range                  = OT_DYNAMIC_RANGE_SDR8;
        pipe_info[i].chn_info[0].chn_attr.video_format                   = OT_VIDEO_FORMAT_LINEAR;
        pipe_info[i].chn_info[0].chn_attr.compress_mode                  = OT_COMPRESS_MODE_NONE;
        pipe_info[i].chn_info[0].chn_attr.mirror_en                      = TD_FALSE;
        pipe_info[i].chn_info[0].chn_attr.flip_en                        = TD_FALSE;
        pipe_info[i].chn_info[0].chn_attr.depth                          = 0;
        pipe_info[i].chn_info[0].chn_attr.frame_rate_ctrl.src_frame_rate = -1;
        pipe_info[i].chn_info[0].chn_attr.frame_rate_ctrl.dst_frame_rate = -1;
    }
}

td_void sample_comm_vi_get_default_vi_cfg(sample_sns_type sns_type, sample_vi_cfg *vi_cfg)
{
    (td_void)memset_s(vi_cfg, sizeof(sample_vi_cfg), 0, sizeof(sample_vi_cfg));

    /* sensor info */
    sample_comm_vi_get_default_sns_info(sns_type, &vi_cfg->sns_info);
    /* mipi info */
    sample_comm_vi_get_default_mipi_info(sns_type, &vi_cfg->mipi_info);
    /* dev info */
    sample_comm_vi_get_default_dev_info(sns_type, &vi_cfg->dev_info);
    /* bind info */
    sample_comm_vi_get_default_bind_info(sns_type, &vi_cfg->bind_pipe);
    /* grp info */
    sample_comm_vi_get_default_grp_info(sns_type, &vi_cfg->grp_info);
    /* pipe info */
    sample_comm_vi_get_default_pipe_info(sns_type, &vi_cfg->bind_pipe, vi_cfg->pipe_info);
}

td_void sample_comm_vi_init_vi_cfg(sample_sns_type sns_type, ot_size *size, sample_vi_cfg *vi_cfg)
{
    (td_void)memset_s(vi_cfg, sizeof(sample_vi_cfg), 0, sizeof(sample_vi_cfg));

    /* sensor info */
    sample_comm_vi_get_default_sns_info(sns_type, &vi_cfg->sns_info);
    /* mipi info */
    sample_comm_vi_get_default_mipi_info(sns_type, &vi_cfg->mipi_info);
    /* dev info */
    sample_comm_vi_get_default_dev_info(sns_type, &vi_cfg->dev_info);
    /* bind info */
    sample_comm_vi_get_default_bind_info(sns_type, &vi_cfg->bind_pipe);
    /* grp info */
    sample_comm_vi_get_default_grp_info(sns_type, &vi_cfg->grp_info);
    /* pipe info */
    sample_comm_vi_init_pipe_info(sns_type, size, &vi_cfg->bind_pipe, vi_cfg->pipe_info);
}

td_s32 sample_comm_vi_set_vi_vpss_mode(ot_vi_vpss_mode_type mode_type, ot_vi_video_mode video_mode)
{
    td_u32 i;
    td_s32 ret;
    ot_vi_vpss_mode_type other_pipe_mode_type;
    ot_vi_vpss_mode vi_vpss_mode;

    if (mode_type == OT_VI_OFFLINE_VPSS_ONLINE) {
        other_pipe_mode_type = OT_VI_OFFLINE_VPSS_ONLINE;
    } else {
        other_pipe_mode_type = OT_VI_OFFLINE_VPSS_OFFLINE;
    }

    vi_vpss_mode.mode[0] = mode_type;
    for (i = 1; i < OT_VI_MAX_PIPE_NUM; i++) {
        vi_vpss_mode.mode[i] = other_pipe_mode_type;
    }

    ret = ss_mpi_sys_set_vi_vpss_mode(&vi_vpss_mode);
    if (ret != TD_SUCCESS) {
        sample_print("set vi vpss mode failed!\n");
        return TD_FAILURE;
    }

    ret = ss_mpi_sys_set_vi_video_mode(video_mode);
    if (ret != TD_SUCCESS) {
        sample_print("set vi video mode failed!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_set_mipi_hs_mode(lane_divide_mode_t hs_mode)
{
    td_s32 fd;
    td_s32 ret;

    fd = open(MIPI_DEV_NAME, O_RDWR);
    if (fd < 0) {
        sample_print("open %s failed!\n", MIPI_DEV_NAME);
        return TD_FAILURE;
    }

    ret = ioctl(fd, OT_MIPI_SET_HS_MODE, &hs_mode);

    close(fd);

    return ret;
}

static td_s32 sample_comm_vi_mipi_ctrl_cmd(td_u32 devno, td_u32 cmd)
{
    td_s32 ret;
    td_s32 fd;

    fd = open(MIPI_DEV_NAME, O_RDWR);
    if (fd < 0) {
        sample_print("open %s failed!\n", MIPI_DEV_NAME);
        return TD_FAILURE;
    }

    ret = ioctl(fd, cmd, &devno);

    close(fd);

    return ret;
}

static td_s32 sample_comm_vi_set_mipi_combo_attr(const combo_dev_attr_t *combo_dev_attr)
{
    td_s32 fd;
    td_s32 ret;

    fd = open(MIPI_DEV_NAME, O_RDWR);
    if (fd < 0) {
        sample_print("open %s failed!\n", MIPI_DEV_NAME);
        return TD_FAILURE;
    }

    ret = ioctl(fd, OT_MIPI_SET_DEV_ATTR, combo_dev_attr);

    close(fd);

    return ret;
}

static td_s32 sample_comm_vi_set_mipi_ext_data_type_attr(const ext_data_type_t *ext_data_type_attr)
{
    td_s32 fd;
    td_s32 ret;

    fd = open(MIPI_DEV_NAME, O_RDWR);
    if (fd < 0) {
        sample_print("open %s failed!\n", MIPI_DEV_NAME);
        return TD_FAILURE;
    }

    ret = ioctl(fd, OT_MIPI_SET_EXT_DATA_TYPE, ext_data_type_attr);

    close(fd);

    return ret;
}

static td_s32 sample_comm_vi_start_mipi_rx(const sample_sns_info *sns_info, const sample_mipi_info *mipi_info)
{
    td_s32 ret;

    ret = sample_comm_vi_set_mipi_hs_mode(mipi_info->divide_mode);
    if (ret != TD_SUCCESS) {
        sample_print("mipi rx set hs_mode failed!\n");
        return TD_FAILURE;
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(mipi_info->mipi_dev, OT_MIPI_ENABLE_MIPI_CLOCK);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d enable mipi rx clock failed!\n", mipi_info->mipi_dev);
        return TD_FAILURE;
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(mipi_info->mipi_dev, OT_MIPI_RESET_MIPI);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d reset mipi rx failed!\n", mipi_info->mipi_dev);
        return TD_FAILURE;
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(sns_info->sns_clk_src, OT_MIPI_ENABLE_SENSOR_CLOCK);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d enable sensor clock failed!\n", sns_info->sns_clk_src);
        return TD_FAILURE;
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(sns_info->sns_rst_src, OT_MIPI_RESET_SENSOR);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d reset sensor failed!\n", sns_info->sns_rst_src);
        return TD_FAILURE;
    }

    ret = sample_comm_vi_set_mipi_combo_attr(&mipi_info->combo_dev_attr);
    if (ret != TD_SUCCESS) {
        sample_print("mipi rx set combo attr failed!\n");
        return TD_FAILURE;
    }

    ret = sample_comm_vi_set_mipi_ext_data_type_attr(&mipi_info->ext_data_type_attr);
    if (ret != TD_SUCCESS) {
        sample_print("mipi rx set ext data attr failed!\n");
        return TD_FAILURE;
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(mipi_info->mipi_dev, OT_MIPI_UNRESET_MIPI);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d unreset mipi rx failed!\n", mipi_info->mipi_dev);
        return TD_FAILURE;
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(sns_info->sns_rst_src, OT_MIPI_UNRESET_SENSOR);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d unreset sensor failed!\n", sns_info->sns_rst_src);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void sample_comm_vi_stop_mipi_rx(const sample_sns_info *sns_info, const sample_mipi_info *mipi_info)
{
    td_s32 ret;

    ret = sample_comm_vi_mipi_ctrl_cmd(mipi_info->mipi_dev, OT_MIPI_RESET_MIPI);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d reset mipi rx failed!\n", mipi_info->mipi_dev);
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(mipi_info->mipi_dev, OT_MIPI_DISABLE_MIPI_CLOCK);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d disable mipi rx clock failed!\n", mipi_info->mipi_dev);
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(sns_info->sns_rst_src, OT_MIPI_RESET_SENSOR);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d reset sensor failed!\n", sns_info->sns_rst_src);
    }

    ret = sample_comm_vi_mipi_ctrl_cmd(sns_info->sns_clk_src, OT_MIPI_DISABLE_SENSOR_CLOCK);
    if (ret != TD_SUCCESS) {
        sample_print("devno %d disable sensor clock failed!\n", sns_info->sns_clk_src);
    }
}

static td_s32 sample_comm_vi_start_dev(ot_vi_dev vi_dev, const ot_vi_dev_attr *dev_attr, const ot_vi_bas_attr *bas_attr)
{
    td_s32 ret;

    ret = ss_mpi_vi_set_dev_attr(vi_dev, dev_attr);
    if (ret != TD_SUCCESS) {
        sample_print("vi set dev attr failed with 0x%x!\n", ret);
        return TD_FAILURE;
    }

    if ((bas_attr->enable == TD_TRUE) && (vi_dev == 0)) {
        ret = ss_mpi_vi_set_bas_attr(vi_dev, bas_attr);
        if (ret != TD_SUCCESS) {
            sample_print("vi set bas attr failed with 0x%x!\n", ret);
            return TD_FAILURE;
        }
    }

    ret = ss_mpi_vi_enable_dev(vi_dev);
    if (ret != TD_SUCCESS) {
        sample_print("vi enable dev failed with 0x%x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void sample_comm_vi_stop_dev(ot_vi_dev vi_dev)
{
    td_s32 ret;

    ret = ss_mpi_vi_disable_dev(vi_dev);
    if (ret != TD_SUCCESS) {
        sample_print("vi disable dev failed with 0x%x!\n", ret);
    }
}

static td_s32 sample_comm_vi_dev_bind_pipe(ot_vi_dev vi_dev, const ot_vi_bind_pipe *bind_pipe)
{
    td_u32 i;
    td_s32 j;
    td_s32 ret;

    for (i = 0; i < bind_pipe->pipe_num; i++) {
        ret = ss_mpi_vi_bind(vi_dev, bind_pipe->pipe_id[i]);
        if (ret != TD_SUCCESS) {
            sample_print("vi dev(%d) bind pipe(%d) failed!\n", vi_dev, bind_pipe->pipe_id[i]);
            goto exit;
        }
    }

    return TD_SUCCESS;

exit:
    for (j = i - 1; j >= 0; j--) {
        ret = ss_mpi_vi_unbind(vi_dev, bind_pipe->pipe_id[j]);
        if (ret != TD_SUCCESS) {
            sample_print("vi dev(%d) unbind pipe(%d) failed!\n", vi_dev, bind_pipe->pipe_id[j]);
        }
    }
    return TD_FAILURE;
}

static td_void sample_comm_vi_dev_unbind_pipe(ot_vi_dev vi_dev, const ot_vi_bind_pipe *bind_pipe)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < bind_pipe->pipe_num; i++) {
        ret = ss_mpi_vi_unbind(vi_dev, bind_pipe->pipe_id[i]);
        if (ret != TD_SUCCESS) {
            sample_print("vi dev(%d) unbind pipe(%d) failed!\n", vi_dev, bind_pipe->pipe_id[i]);
        }
    }
}

static td_s32 sample_comm_vi_set_grp_info(const sample_vi_grp_info *grp_info)
{
    td_s32 ret;
    td_u32 i;
    for (i = 0; i < grp_info->grp_num; i++) {
        ret = ss_mpi_vi_set_wdr_fusion_grp_attr(grp_info->fusion_grp[i], &grp_info->fusion_grp_attr[i]);
        if (ret != TD_SUCCESS) {
            sample_print("vi set wdr fusion grp attr failed!\n");
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_start_chn(ot_vi_pipe vi_pipe, const sample_vi_chn_info chn_info[], td_u32 chn_num)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < chn_num; i++) {
        ot_vi_chn vi_chn = chn_info[i].vi_chn;
        const ot_vi_chn_attr *chn_attr = &chn_info[i].chn_attr;

        ret = ss_mpi_vi_set_chn_attr(vi_pipe, vi_chn, chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("vi set chn(%d) attr failed with 0x%x!\n", vi_chn, ret);
            return TD_FAILURE;
        }

        ret = ss_mpi_vi_enable_chn(vi_pipe, vi_chn);
        if (ret != TD_SUCCESS) {
            sample_print("vi enable chn(%d) failed with 0x%x!\n", vi_chn, ret);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_switch_mode_start_chn(ot_vi_pipe vi_pipe,
                                                   const sample_vi_chn_info chn_info[],
                                                   td_u32 chn_num)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < chn_num; i++) {
        ot_vi_chn vi_chn = chn_info[i].vi_chn;
        const ot_vi_chn_attr *chn_attr = &chn_info[i].chn_attr;

        ret = ss_mpi_vi_set_chn_attr(vi_pipe, vi_chn, chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("vi set chn(%d) attr failed with 0x%x!\n", vi_chn, ret);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_stop_chn(ot_vi_pipe vi_pipe, const sample_vi_chn_info chn_info[], td_u32 chn_num)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < chn_num; i++) {
        ot_vi_chn vi_chn = chn_info[i].vi_chn;

        ret = ss_mpi_vi_disable_chn(vi_pipe, vi_chn);
        if (ret != TD_SUCCESS) {
            sample_print("vi disable chn(%d) failed with 0x%x!\n", vi_chn, ret);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_start_one_pipe(ot_vi_pipe vi_pipe, const sample_vi_pipe_info *pipe_info)
{
    td_s32 ret;

    ret = ss_mpi_vi_create_pipe(vi_pipe, &pipe_info->pipe_attr);
    if (ret != TD_SUCCESS) {
        sample_print("vi create pipe(%d) failed with 0x%x!\n", vi_pipe, ret);
        return TD_FAILURE;
    }

    if (pipe_info->pipe_need_start == TD_TRUE) {
        ret = ss_mpi_vi_start_pipe(vi_pipe);
        if (ret != TD_SUCCESS) {
            sample_print("vi start pipe(%d) failed with 0x%x!\n", vi_pipe, ret);
            goto start_pipe_failed;
        }
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


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART02_H_ */
