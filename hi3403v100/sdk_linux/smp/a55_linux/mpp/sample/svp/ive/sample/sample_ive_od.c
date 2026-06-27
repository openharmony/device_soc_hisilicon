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
#include "sample_common_ive.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <math.h>

#define OT_SAMPLE_IVE_OD_MILLI_SEC              20000
#define OT_SAMPLE_IVE_OD_QUERY_SLEEP            100
#define OT_SAMPLE_IVE_OD_LINEAR_NUM             2
#define OT_SAMPLE_IVE_OD_LINEAR_POINT0_X        80
#define OT_SAMPLE_IVE_OD_LINEAR_POINT0_Y        0
#define OT_SAMPLE_IVE_OD_LINEAR_POINT1_X        80
#define OT_SAMPLE_IVE_OD_LINEAR_POINT1_Y        20
#define OT_SAMPLE_IVE_OD_THREAD_NAME_LEN        16
#define OT_SAMPLE_IVE_OD_NUM_TWO                2
#define OT_SAMPLE_IVE_OD_POINT_NUM              10
#define OT_SAMPLE_IVE_RIGHT_SHIFT_TWENTY_EIGHT  28

typedef struct {
    ot_svp_src_img src;
    ot_svp_dst_img integ;
    ot_ive_integ_ctrl integ_ctrl;
    td_u32 width;
    td_u32 height;
} ot_sample_ive_od_info;

static td_bool g_stop_signal = TD_FALSE;
static pthread_t g_ive_thread = 0;
static ot_sample_ive_od_info g_od_info;
static ot_sample_svp_switch g_od_switch = { TD_FALSE, TD_TRUE };
static sample_vi_cfg g_vi_config;

static td_void sample_ive_od_uninit(ot_sample_ive_od_info *od_info)
{
    sample_svp_check_exps_return_void(od_info == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "od_info can't be null\n");
    sample_svp_mmz_free(od_info->src.phys_addr[0], od_info->src.virt_addr[0]);
    sample_svp_mmz_free(od_info->integ.phys_addr[0], od_info->integ.virt_addr[0]);
}

static td_s32 sample_ive_od_init(ot_sample_ive_od_info *od_info, td_u32 width, td_u32 height)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;

    sample_svp_check_exps_return(od_info == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "od_info can't be null\n");

    (td_void)memset_s(od_info, sizeof(ot_sample_ive_od_info), 0, sizeof(ot_sample_ive_od_info));

    ret = sample_common_ive_create_image(&od_info->src, OT_SVP_IMG_TYPE_U8C1, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, od_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create src image failed!\n", ret);

    ret = sample_common_ive_create_image(&od_info->integ, OT_SVP_IMG_TYPE_U64C1, width, height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, od_init_fail, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),Create integ image failed!\n", ret);

    od_info->integ_ctrl.out_ctrl = OT_IVE_INTEG_OUT_CTRL_COMBINE;

    od_info->width = width / OT_IVE_CHAR_CALW;
    od_info->height = height / OT_IVE_CHAR_CALH;

od_init_fail:
    if (ret != TD_SUCCESS) {
        sample_ive_od_uninit(od_info);
    }
    return ret;
}

static td_s32 sample_ive_linear_2d_classifer(ot_point *char_point, td_s32 char_num, ot_point *linear_point,
    td_s32 linear_num)
{
    td_s32 result_num;
    td_s32 i;
    td_s32 j;
    td_bool test_flag;
    ot_point *next_linear_point = TD_NULL;

    result_num = 0;
    next_linear_point = &linear_point[1];
    for (i = 0; i < char_num; i++) {
        test_flag = TD_TRUE;
        for (j = 0; j < (linear_num - 1); j++) {
            if (((char_point[i].y - linear_point[j].y) * (next_linear_point[j].x - linear_point[j].x) >
                 (char_point[i].x - linear_point[j].x) * (next_linear_point[j].y - linear_point[j].y) &&
                 (next_linear_point[j].x != linear_point[j].x)) ||
                 ((char_point[i].x > linear_point[j].x) && (next_linear_point[j].x == linear_point[j].x))) {
                test_flag = TD_FALSE;
                break;
            }
        }
        if (test_flag == TD_TRUE) {
            result_num++;
        }
    }
    return result_num;
}

static td_void sample_ive_prepare_dma_data(ot_sample_ive_od_info *od_ptr, ot_video_frame_info *ext_frm_info,
    ot_svp_data *src_data, ot_svp_data *dst_data)
{
    src_data->virt_addr = sample_svp_convert_ptr_to_addr(td_u64, ext_frm_info->video_frame.virt_addr[0]);
    src_data->phys_addr = ext_frm_info->video_frame.phys_addr[0];
    src_data->stride = ext_frm_info->video_frame.stride[0];
    src_data->width = ext_frm_info->video_frame.width;
    src_data->height = ext_frm_info->video_frame.height;

    dst_data->virt_addr = od_ptr->src.virt_addr[0];
    dst_data->phys_addr = od_ptr->src.phys_addr[0];
    dst_data->stride = ext_frm_info->video_frame.stride[0];
    dst_data->width = ext_frm_info->video_frame.width;
    dst_data->height = ext_frm_info->video_frame.height;
}

static td_s32 sample_ive_query_task(ot_ive_handle handle)
{
    td_s32 ret;
    td_bool is_block = TD_TRUE;
    td_bool is_finish = TD_FALSE;
    ret = ss_mpi_ive_query(handle, &is_finish, is_block);
    while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
        usleep(OT_SAMPLE_IVE_OD_QUERY_SLEEP);
        ret = ss_mpi_ive_query(handle, &is_finish, is_block);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_query failed!\n", ret);
    return TD_SUCCESS;
}

static td_s32 sample_ive_get_char_point(ot_sample_ive_od_info *od_ptr, ot_point char_point[], td_u32 length)
{
    td_u64 *vir_data = TD_NULL;
    td_u32 i;
    td_u32 j;
    td_u64 top_left;
    td_u64 top_right;
    td_u64 btm_left;
    td_u64 btm_right;
    td_u64 *top_row_ptr = TD_NULL;
    td_u64 *btm_row_ptr = TD_NULL;
    td_u64 block_sum;
    td_u64 block_sqrt;
    td_float sqrt_val;
    vir_data = sample_svp_convert_addr_to_ptr(td_u64, od_ptr->integ.virt_addr[0]);
    sample_svp_check_exps_return(length > OT_IVE_CHAR_CALW * OT_IVE_CHAR_CALH, OT_ERR_IVE_ILLEGAL_PARAM,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "length(%u) is larger than %u\n", length, OT_IVE_CHAR_CALW * OT_IVE_CHAR_CALH);

    for (j = 0; (j < OT_IVE_CHAR_CALH) && (j < length); j++) {
        top_row_ptr = (0 == j) ? (vir_data) : (vir_data + (j * od_ptr->height - 1) * od_ptr->integ.stride[0]);
        btm_row_ptr = vir_data + ((j + 1) * od_ptr->height - 1) * od_ptr->integ.stride[0];

        for (i = 0; i < OT_IVE_CHAR_CALW; i++) {
            top_left = (0 == j) ? (0) : ((0 == i) ? (0) : (top_row_ptr[i * od_ptr->width - 1]));
            top_right = (0 == j) ? (0) : (top_row_ptr[(i + 1) * od_ptr->width - 1]);
            btm_left = (0 == i) ? (0) : (btm_row_ptr[i * od_ptr->width - 1]);
            btm_right = btm_row_ptr[(i + 1) * od_ptr->width - 1];

            block_sum = (top_left & 0xfffffffLL) + (btm_right & 0xfffffffLL) -
                (btm_left & 0xfffffffLL) - (top_right & 0xfffffffLL);

            block_sqrt = (top_left >> OT_SAMPLE_IVE_RIGHT_SHIFT_TWENTY_EIGHT) +
                        (btm_right >> OT_SAMPLE_IVE_RIGHT_SHIFT_TWENTY_EIGHT) -
                        (btm_left >> OT_SAMPLE_IVE_RIGHT_SHIFT_TWENTY_EIGHT) -
                        (top_right >> OT_SAMPLE_IVE_RIGHT_SHIFT_TWENTY_EIGHT);

            /* mean */
            char_point[j * OT_IVE_CHAR_CALW + i].x = block_sum / (od_ptr->width * od_ptr->height);
            /* sigma=sqrt(1/(w*h)*sum((x(i,j)-mean)^2)= sqrt(sum(x(i,j)^2)/(w*h)-mean^2) */
            sqrt_val = block_sqrt / (od_ptr->width * od_ptr->height) - char_point[j * OT_IVE_CHAR_CALW + i].x *
                char_point[j * OT_IVE_CHAR_CALW + i].x;
            char_point[j * OT_IVE_CHAR_CALW + i].y = (td_u32)sqrt(sqrt_val);
        }
    }
    return TD_SUCCESS;
}

static td_void sample_ive_set_linear_data(ot_sample_ive_linear_data *data)
{
    data->linear_num = OT_SAMPLE_IVE_OD_LINEAR_NUM;
    data->thresh_num = OT_IVE_CHAR_NUM / OT_SAMPLE_IVE_OD_NUM_TWO;
    data->linear_point[0].x = OT_SAMPLE_IVE_OD_LINEAR_POINT0_X;
    data->linear_point[0].y = OT_SAMPLE_IVE_OD_LINEAR_POINT0_Y;
    data->linear_point[1].x = OT_SAMPLE_IVE_OD_LINEAR_POINT1_X;
    data->linear_point[1].y = OT_SAMPLE_IVE_OD_LINEAR_POINT1_Y;
}

static td_void *sample_ive_od_proc(td_void *arg)
{
    td_s32 ret;
    ot_sample_ive_od_info *od_ptr = TD_NULL;
    ot_video_frame_info base_frm_info;
    ot_video_frame_info ext_frm_info;
    const td_s32 vpss_grp = 0;
    td_s32 vpss_chn[] = { OT_VPSS_CHN0, OT_VPSS_CHN1 };
    ot_svp_data src_data;
    ot_svp_data dst_data;
    ot_ive_handle handle;
    ot_point points[OT_SAMPLE_IVE_OD_POINT_NUM] = {{ 0, 0 }};

    ot_point char_point[OT_IVE_CHAR_NUM];
    ot_sample_ive_linear_data data;
    ot_ive_dma_ctrl dma_ctrl = { OT_IVE_DMA_MODE_DIRECT_COPY, 0, 0, 0, 0 };
    data.linear_point = &points[0];

    od_ptr = (ot_sample_ive_od_info *)(arg);

    sample_svp_check_exps_return(od_ptr == TD_NULL, TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "od_ptr can't be null\n");
    /* init data */
    sample_ive_set_linear_data(&data);

    while (g_stop_signal == TD_FALSE) {
        /* get frame */
        ret = ss_mpi_vpss_get_chn_frame(vpss_grp, vpss_chn[1], &ext_frm_info, OT_SAMPLE_IVE_OD_MILLI_SEC);
        sample_svp_check_exps_continue(ret != TD_SUCCESS, SAMPLE_SVP_ERR_LEVEL_DEBUG,
            "Error(%#x),ss_mpi_vpss_get_chn_frame failed, VPSS_GRP(%d), VPSS_CHN(%d)!\n", ret, vpss_grp, vpss_chn[1]);

        ret = ss_mpi_vpss_get_chn_frame(vpss_grp, vpss_chn[0], &base_frm_info, OT_SAMPLE_IVE_OD_MILLI_SEC);
        sample_svp_check_failed_goto(ret, ext_free, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_vpss_get_chn_frame failed, VPSS_GRP(%d), VPSS_CHN(%d)!\n", ret, vpss_grp, vpss_chn[0]);

        /* prepare dma data */
        sample_ive_prepare_dma_data(od_ptr, &ext_frm_info, &src_data, &dst_data);

        /* dma */
        ret = ss_mpi_ive_dma(&handle, &src_data, &dst_data, &dma_ctrl, TD_FALSE);
        sample_svp_check_failed_err_level_goto(ret, base_free, "Error(%#x),ss_mpi_ive_dma failed!\n", ret);

        /* integ */
        ret = ss_mpi_ive_integ(&handle, &od_ptr->src, &od_ptr->integ, &od_ptr->integ_ctrl, TD_TRUE);
        sample_svp_check_failed_err_level_goto(ret, base_free, "Error(%#x),ss_mpi_ive_integ failed!\n", ret);

        /* query task */
        ret = sample_ive_query_task(handle);
        sample_svp_check_failed_err_level_goto(ret, base_free, "ive_query_task failed!\n");

        /* get result */
        ret = sample_ive_get_char_point(od_ptr, char_point, OT_IVE_CHAR_NUM);
        sample_svp_check_failed_err_level_goto(ret, base_free, "sample_ive_get_char_point failed!\n");

        /* classify */
        ret = sample_ive_linear_2d_classifer(char_point, OT_IVE_CHAR_NUM, data.linear_point, data.linear_num);
        if (ret > data.thresh_num) {
            sample_svp_trace_info("\033[0;31m Occlusion detected!\033[0;39m\n");
        }

        /* send vo frame */
        ret = ss_mpi_vo_send_frame(0, 0, &base_frm_info, OT_SAMPLE_IVE_OD_MILLI_SEC);
        sample_svp_check_failed_err_level_goto(ret, base_free, "Error(%#x),sample_vo_send_frame failed!\n", ret);

base_free:
        ret = ss_mpi_vpss_release_chn_frame(vpss_grp, vpss_chn[0], &base_frm_info);
        sample_svp_check_exps_trace(ret != TD_SUCCESS, SAMPLE_SVP_ERR_LEVEL_DEBUG,
            "Error(%#x),release_frame failed,Grp(%d) chn(%d)!\n", ret, vpss_grp, vpss_chn[0]);

ext_free:
        ret = ss_mpi_vpss_release_chn_frame(vpss_grp, vpss_chn[1], &ext_frm_info);
        sample_svp_check_exps_trace(ret != TD_SUCCESS, SAMPLE_SVP_ERR_LEVEL_DEBUG,
            "Error(%#x),release_frame failed,Grp(%d) chn(%d)!\n", ret, vpss_grp, vpss_chn[1]);
    }
    return TD_NULL;
}

static td_s32 sample_ive_od_pause(td_void)
{
    printf("---------------press Enter key to exit!---------------\n");
    if (g_stop_signal == TD_TRUE) {
        if (g_ive_thread != 0) {
            pthread_join(g_ive_thread, TD_NULL);
            g_ive_thread = 0;
        }
        sample_ive_od_uninit(&(g_od_info));
        (td_void)memset_s(&g_od_info, sizeof(g_od_info), 0, sizeof(g_od_info));
        sample_common_svp_stop_vi_vpss_venc_vo(&g_vi_config, &g_od_switch);
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
        return TD_TRUE;
    }
    (void)getchar();
    if (g_stop_signal == TD_TRUE) {
        if (g_ive_thread != 0) {
            pthread_join(g_ive_thread, TD_NULL);
            g_ive_thread = 0;
        }
        sample_ive_od_uninit(&(g_od_info));
        (td_void)memset_s(&g_od_info, sizeof(g_od_info), 0, sizeof(g_od_info));
        sample_common_svp_stop_vi_vpss_venc_vo(&g_vi_config, &g_od_switch);
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_void sample_ive_od(td_void)
{
    td_s32 ret;
    ot_size pic_size;
    ot_pic_size pic_type = PIC_1080P;

    (td_void)memset_s(&g_od_info, sizeof(g_od_info), 0, sizeof(g_od_info));

    /*
     * step 1: start vi vpss venc vo
     */
    ret = sample_common_svp_start_vi_vpss_venc_vo(&g_vi_config, &g_od_switch, &pic_type);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_od_0, SAMPLE_SVP_ERR_LEVEL_DEBUG,
        "Error(%#x),sample_common_svp_start_vi_vpss_venc_vo failed!\n", ret);

    ret = sample_comm_sys_get_pic_size(pic_type, &pic_size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_od_0, SAMPLE_SVP_ERR_LEVEL_DEBUG,
        "Error(%#x),sample_comm_sys_get_pic_size failed!\n", ret);
    /*
     * step 2: Init OD
     */
    ret = sample_ive_od_init(&g_od_info, pic_size.width, pic_size.height);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_od_0, SAMPLE_SVP_ERR_LEVEL_DEBUG,
        "sample_ive_od_init failed, Error(%#x)!\n", ret);
    g_stop_signal = TD_FALSE;

    /*
     * step 3: Create work thread
     */
    ret = prctl(PR_SET_NAME, "ive_od_proc", 0, 0, 0);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_od_0, SAMPLE_SVP_ERR_LEVEL_DEBUG, "set thread name failed!\n");
    ret = pthread_create(&g_ive_thread, 0, sample_ive_od_proc, (td_void *)&g_od_info);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_od_0, SAMPLE_SVP_ERR_LEVEL_DEBUG, "pthread_create failed!\n");

    ret = sample_ive_od_pause();
    sample_svp_check_exps_return_void(ret == TD_TRUE, SAMPLE_SVP_ERR_LEVEL_DEBUG, "od exit!\n");

    g_stop_signal = TD_TRUE;
    pthread_join(g_ive_thread, TD_NULL);
    g_ive_thread = 0;
    sample_ive_od_uninit(&(g_od_info));
    (td_void)memset_s(&g_od_info, sizeof(g_od_info), 0, sizeof(g_od_info));

end_od_0:
    g_ive_thread = 0;
    g_stop_signal = TD_TRUE;
    sample_common_svp_stop_vi_vpss_venc_vo(&g_vi_config, &g_od_switch);
    return;
}

/*
 * function : Od sample signal handle
 */
td_void sample_ive_od_handle_sig(td_void)
{
    g_stop_signal = TD_TRUE;
}
