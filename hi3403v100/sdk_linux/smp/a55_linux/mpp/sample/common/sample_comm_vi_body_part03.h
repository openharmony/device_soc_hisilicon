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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART03_H_


static td_void sample_comm_vi_stop_one_pipe(ot_vi_pipe vi_pipe, const sample_vi_pipe_info *pipe_info)
{
    td_s32 ret;

    ret = sample_comm_vi_stop_chn(vi_pipe, pipe_info->chn_info, pipe_info->chn_num);
    if (ret != TD_SUCCESS) {
        sample_print("vi pipe(%d) stop chn failed!\n", vi_pipe);
    }

    ret = ss_mpi_vi_stop_pipe(vi_pipe);
    if (ret != TD_SUCCESS) {
        sample_print("vi stop pipe(%d) failed with 0x%x!\n", vi_pipe, ret);
    }

    ret = ss_mpi_vi_destroy_pipe(vi_pipe);
    if (ret != TD_SUCCESS) {
        sample_print("vi destroy pipe(%d) failed with 0x%x!\n", vi_pipe, ret);
    }
}

static td_s32 sample_comm_vi_start_pipe(const ot_vi_bind_pipe *bind_pipe, const sample_vi_pipe_info pipe_info[])
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < (td_s32)bind_pipe->pipe_num; i++) {
        ot_vi_pipe vi_pipe = bind_pipe->pipe_id[i];
        ret = sample_comm_vi_start_one_pipe(vi_pipe, &pipe_info[i]);
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

static td_void sample_comm_vi_stop_pipe(const ot_vi_bind_pipe *bind_pipe, const sample_vi_pipe_info pipe_info[])
{
    td_s32 i;
    for (i = bind_pipe->pipe_num - 1; i >= 0; i--) {
        ot_vi_pipe vi_pipe = bind_pipe->pipe_id[i];
        sample_comm_vi_stop_one_pipe(vi_pipe, &pipe_info[i]);
    }
}

static td_s32 sample_comm_vi_register_sensor_lib(ot_vi_pipe vi_pipe, td_u8 pipe_index, const sample_vi_cfg *vi_cfg)
{
    td_s32 ret;
    td_u32 bus_id;
    sample_sns_type sns_type = vi_cfg->sns_info.sns_type;

    ret = sample_comm_isp_sensor_regiter_callback(vi_pipe, sns_type);
    if (ret != TD_SUCCESS) {
        printf("register sensor to ISP %d failed\n", vi_pipe);
        return TD_FAILURE;
    }

    if (pipe_index > 0) {
        bus_id = -1;
    } else {
        bus_id = vi_cfg->sns_info.bus_id;
    }

    ret = sample_comm_isp_bind_sns(vi_pipe, sns_type, bus_id);
    if (ret != TD_SUCCESS) {
        printf("register sensor bus id %d failed\n", bus_id);
        goto exit0;
    }

    ret = sample_comm_isp_ae_lib_callback(vi_pipe);
    if (ret != TD_SUCCESS) {
        printf("isp_mst_comm_ae_lib_callback failed\n");
        goto exit0;
    }

    ret = sample_comm_isp_awb_lib_callback(vi_pipe);
    if (ret != TD_SUCCESS) {
        printf("isp_mst_comm_awb_lib_callback failed\n");
        goto exit1;
    }

    return TD_SUCCESS;

exit1:
    sample_comm_isp_ae_lib_uncallback(vi_pipe);
exit0:
    sample_comm_isp_sensor_unregiter_callback(vi_pipe);
    return ret;
}

static td_void sample_comm_vi_deregister_sensor_lib(ot_vi_pipe vi_pipe)
{
    sample_comm_isp_awb_lib_uncallback(vi_pipe);
    sample_comm_isp_ae_lib_uncallback(vi_pipe);
    sample_comm_isp_sensor_unregiter_callback(vi_pipe);
}

static td_s32 sample_comm_vi_start_one_pipe_isp(ot_vi_pipe vi_pipe, td_u8 pipe_index, const sample_vi_cfg *vi_cfg)
{
    td_s32 ret;

    ret = sample_comm_vi_register_sensor_lib(vi_pipe, pipe_index, vi_cfg);
    if (ret != TD_SUCCESS) {
        printf("register sensor to ISP %d failed\n", vi_pipe);
        return TD_FAILURE;
    }

    ret = ss_mpi_isp_mem_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        printf("OT_MPI_ISP_MemInit failed with 0x%x!\n", ret);
        goto exit0;
    }

    ret = ss_mpi_isp_set_pub_attr(vi_pipe, &vi_cfg->pipe_info[pipe_index].isp_info.isp_pub_attr);
    if (ret != TD_SUCCESS) {
        printf("OT_MPI_ISP_SetPubAttr failed with 0x%x!\n", ret);
        goto exit1;
    }

    ret = ss_mpi_isp_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        printf("OT_MPI_ISP_Init failed with 0x%x!\n", ret);
        return -1;
    }

    ret = sample_comm_isp_sensor_founction_cfg(vi_pipe, vi_cfg->sns_info.sns_type);
    if (ret != TD_SUCCESS) {
        printf("sensor founction cfg failed with 0x%x!\n", ret);
        return -1;
    }

    if ((vi_pipe < OT_VI_MAX_PHYS_PIPE_NUM) &&
        (vi_cfg->pipe_info[pipe_index].isp_need_run == TD_TRUE)) {
        ret = sample_comm_isp_run(vi_pipe);
        if (ret != TD_SUCCESS) {
            printf("ISP Run failed with 0x%x!\n", ret);
            goto exit1;
        }
    }

    g_start_isp[vi_pipe] = TD_TRUE;

    return TD_SUCCESS;

exit1:
    ss_mpi_isp_exit(vi_pipe);
exit0:
    sample_comm_vi_deregister_sensor_lib(vi_pipe);
    return ret;
}

static td_void sample_comm_vi_stop_one_pipe_isp(ot_vi_pipe vi_pipe)
{
    ss_mpi_isp_exit(vi_pipe);
    sample_comm_isp_stop(vi_pipe);
    sample_comm_vi_deregister_sensor_lib(vi_pipe);

    g_start_isp[vi_pipe] = TD_FALSE;
}

td_s32 sample_comm_vi_get_isp_run_state(td_bool *isp_states, td_u32 size)
{
    if (isp_states == TD_NULL) {
        sample_print("isp_states is NULL\n");
        return TD_FAILURE;
    }
    if (size < OT_VI_MAX_PIPE_NUM) {
        sample_print("array size smaller than %d\n", OT_VI_MAX_PIPE_NUM);
        return TD_FAILURE;
    }
    td_s32 ret = memcpy_s(isp_states, size * sizeof(td_bool), g_start_isp, OT_VI_MAX_PIPE_NUM * sizeof(td_bool));
    if (ret != EOK) {
        sample_print("memcpy_s isp states fail %x\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_vi_start_isp(const sample_vi_cfg *vi_cfg)
{
    td_s8 i;
    td_s8 j;
    td_s32 ret;
    ot_vi_pipe vi_pipe;
    for (i = 0; i < (td_u8)vi_cfg->bind_pipe.pipe_num; i++) {
        vi_pipe = vi_cfg->bind_pipe.pipe_id[i];
        if ((vi_cfg->grp_info.fusion_grp_attr[0].wdr_mode != OT_WDR_MODE_NONE) &&
            (vi_cfg->grp_info.fusion_grp_attr[0].wdr_mode != OT_WDR_MODE_BUILT_IN) &&
            (i > 0)) {
            continue;
        }
        ret = sample_comm_vi_start_one_pipe_isp(vi_pipe, i, vi_cfg);
        if (ret != TD_SUCCESS) {
            for (j = i - 1; (j >= 0) && (i != 0); j--) {
                vi_pipe = vi_cfg->bind_pipe.pipe_id[j];
                sample_comm_vi_stop_one_pipe_isp(vi_pipe);
            }

            return ret;
        }
    }

    return TD_SUCCESS;
}

static td_void sample_comm_vi_stop_isp(const sample_vi_cfg *vi_cfg)
{
    td_u32     i;
    td_bool    start_pipe;
    ot_vi_pipe vi_pipe;

    for (i = 0; i < vi_cfg->bind_pipe.pipe_num; i++) {
        if ((vi_cfg->pipe_info[i].isp_info.isp_pub_attr.wdr_mode == OT_WDR_MODE_NONE) ||
            (vi_cfg->pipe_info[i].isp_info.isp_pub_attr.wdr_mode == OT_WDR_MODE_BUILT_IN)) {
            start_pipe = TD_TRUE;
        } else {
            start_pipe = (i > 0) ? TD_FALSE : TD_TRUE;
        }

        if (start_pipe != TD_TRUE) {
            continue;
        }

        vi_pipe = vi_cfg->bind_pipe.pipe_id[i];
        sample_comm_vi_stop_one_pipe_isp(vi_pipe);
    }
}

td_s32 sample_comm_vi_start_vi(const sample_vi_cfg *vi_cfg)
{
    td_s32 ret;
    ot_vi_dev vi_dev;

    ret = sample_comm_vi_start_mipi_rx(&vi_cfg->sns_info, &vi_cfg->mipi_info);
    if (ret != TD_SUCCESS) {
        sample_print("start mipi rx failed!\n");
        goto start_mipi_rx_failed;
    }

    vi_dev   = vi_cfg->dev_info.vi_dev;
    ret = sample_comm_vi_start_dev(vi_dev, &vi_cfg->dev_info.dev_attr, &vi_cfg->dev_info.bas_attr);
    if (ret != TD_SUCCESS) {
        sample_print("start dev failed!\n");
        goto start_dev_failed;
    }

    ret = sample_comm_vi_dev_bind_pipe(vi_dev, &vi_cfg->bind_pipe);
    if (ret != TD_SUCCESS) {
        sample_print("dev bind pipe failed!\n");
        goto dev_bind_pipe_failed;
    }

    ret = sample_comm_vi_set_grp_info(&vi_cfg->grp_info);
    if (ret != TD_SUCCESS) {
        sample_print("set grp info failed!\n");
        goto set_grp_info_failed;
    }

    ret = sample_comm_vi_start_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
    if (ret != TD_SUCCESS) {
        sample_print("start pipe failed!\n");
        goto start_pipe_failed;
    }

    ret = sample_comm_vi_start_isp(vi_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("start isp failed!\n");
        goto start_isp_failed;
    }

    return TD_SUCCESS;

start_isp_failed:
    sample_comm_vi_stop_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
start_pipe_failed: /* fall through */
set_grp_info_failed:
    sample_comm_vi_dev_unbind_pipe(vi_dev, &vi_cfg->bind_pipe);
dev_bind_pipe_failed:
    sample_comm_vi_stop_dev(vi_dev);
start_dev_failed:
    sample_comm_vi_stop_mipi_rx(&vi_cfg->sns_info, &vi_cfg->mipi_info);
start_mipi_rx_failed:
    return TD_FAILURE;
}

td_void sample_comm_vi_stop_vi(const sample_vi_cfg *vi_cfg)
{
    ot_vi_dev vi_dev = vi_cfg->dev_info.vi_dev;

    sample_comm_vi_stop_isp(vi_cfg);
    sample_comm_vi_stop_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
    sample_comm_vi_dev_unbind_pipe(vi_dev, &vi_cfg->bind_pipe);
    sample_comm_vi_stop_dev(vi_dev);
    sample_comm_vi_stop_mipi_rx(&vi_cfg->sns_info, &vi_cfg->mipi_info);
}

td_void sample_comm_vi_mode_switch_stop_vi(const sample_vi_cfg *vi_cfg)
{
    ot_vi_dev vi_dev = vi_cfg->dev_info.vi_dev;

    sample_comm_vi_stop_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
    sample_comm_vi_dev_unbind_pipe(vi_dev, &vi_cfg->bind_pipe);
    sample_comm_vi_stop_dev(vi_dev);
    sample_comm_vi_stop_mipi_rx(&vi_cfg->sns_info, &vi_cfg->mipi_info);
}

static td_s32 sample_comm_vi_mode_switch_start_one_pipe(ot_vi_pipe vi_pipe, const sample_vi_pipe_info *pipe_info)
{
    td_s32 ret;

    ret = ss_mpi_vi_create_pipe(vi_pipe, &pipe_info->pipe_attr);
    if (ret != TD_SUCCESS) {
        sample_print("vi create pipe(%d) failed with 0x%x!\n", vi_pipe, ret);
        return TD_FAILURE;
    }

    ret = sample_comm_vi_switch_mode_start_chn(vi_pipe, pipe_info->chn_info, pipe_info->chn_num);
    if (ret != TD_SUCCESS) {
        sample_print("vi pipe(%d) start chn failed!\n", vi_pipe);
        goto start_chn_failed;
    }

    return TD_SUCCESS;

start_chn_failed:
    ss_mpi_vi_stop_pipe(vi_pipe);
    return TD_FAILURE;
}

static td_s32 sample_comm_vi_mode_switch_start_pipe(const ot_vi_bind_pipe *bind_pipe,
                                                    const sample_vi_pipe_info pipe_info[])
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < (td_s32)bind_pipe->pipe_num; i++) {
        ot_vi_pipe vi_pipe = bind_pipe->pipe_id[i];
        ret = sample_comm_vi_mode_switch_start_one_pipe(vi_pipe, &pipe_info[i]);
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

td_s32 sample_comm_vi_mode_switch_start_vi(const sample_vi_cfg *vi_cfg, td_bool chg_resolution, const ot_size *size)
{
    td_s32 ret;
    ot_vi_dev vi_dev;

    ret = sample_comm_vi_start_mipi_rx(&vi_cfg->sns_info, &vi_cfg->mipi_info);
    if (ret != TD_SUCCESS) {
        sample_print("start mipi rx failed!\n");
        goto start_mipi_rx_failed;
    }

    vi_dev   = vi_cfg->dev_info.vi_dev;
    ret = sample_comm_vi_start_dev(vi_dev, &vi_cfg->dev_info.dev_attr, &vi_cfg->dev_info.bas_attr);
    if (ret != TD_SUCCESS) {
        sample_print("start dev failed!\n");
        goto start_dev_failed;
    }

    ret = sample_comm_vi_dev_bind_pipe(vi_dev, &vi_cfg->bind_pipe);
    if (ret != TD_SUCCESS) {
        sample_print("dev bind pipe failed!\n");
        goto dev_bind_pipe_failed;
    }

    ret = sample_comm_vi_set_grp_info(&vi_cfg->grp_info);
    if (ret != TD_SUCCESS) {
        sample_print("set grp info failed!\n");
        goto set_grp_info_failed;
    }

    ret = sample_comm_vi_mode_switch_start_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
    if (ret != TD_SUCCESS) {
        sample_print("set grp info failed!\n");
        goto set_grp_info_failed;
    }

    if (chg_resolution == TD_TRUE) {
        ret = sample_comm_vi_switch_isp_resolution(vi_cfg, size);
    } else {
        ret = sample_comm_vi_switch_isp_mode(vi_cfg);
    }
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vi_start_isp failed!\n");
        goto start_isp_failed;
    }

    return TD_SUCCESS;

start_isp_failed:
    sample_comm_vi_stop_pipe(&vi_cfg->bind_pipe, vi_cfg->pipe_info);
set_grp_info_failed:
    sample_comm_vi_dev_unbind_pipe(vi_dev, &vi_cfg->bind_pipe);
dev_bind_pipe_failed:
    sample_comm_vi_stop_dev(vi_dev);
start_dev_failed:
    sample_comm_vi_stop_mipi_rx(&vi_cfg->sns_info, &vi_cfg->mipi_info);
start_mipi_rx_failed:
    return TD_FAILURE;
}

static td_s32 sample_comm_vi_mode_switch_start_one_pipe_chn(ot_vi_pipe vi_pipe, const sample_vi_pipe_info *pipe_info)
{
    td_s32 ret;

    ret = ss_mpi_vi_start_pipe(vi_pipe);
    if (ret != TD_SUCCESS) {
        sample_print("vi start pipe(%d) failed with 0x%x!\n", vi_pipe, ret);
        goto start_pipe_failed;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART03_H_ */
