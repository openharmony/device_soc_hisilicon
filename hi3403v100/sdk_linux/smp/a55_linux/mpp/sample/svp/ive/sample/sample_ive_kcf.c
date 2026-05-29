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
#include "sample_ive_main.h"
#include "ot_buffer.h"
#include "ot_common.h"
#include "ot_common_sys.h"
#include "ot_common_svp.h"
#include "ot_common_vpss.h"
#include "sample_comm.h"
#include "sample_common_svp.h"
#include "sample_common_svp_npu.h"
#include "sample_common_svp_npu_model.h"
#include "sample_common_ive.h"
#include "sample_ive_queue.h"
#include "svp_acl_ext.h"

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
#include <pthread.h>
#include <sys/prctl.h>
#include <math.h>

#define OT_SAMPLE_IVE_KCF_ROI_NUM                   64
#define OT_SAMPLE_IVE_KCF_FRAME_NUM                 2
#define OT_SAMPLE_IVE_KCF_NODE_MAX_NUM              64
#define OT_SAMPLE_IVE_KCF_GAUSS_PEAK_TOTAL_SIZE     455680
#define OT_SAMPLE_IVE_KCF_COS_WINDOW_TOTAL_SIZE     832
#define OT_SAMPLE_IVE_KCF_COS_WINDOW_SIZE           64
#define OT_SAMPLE_IVE_KCF_TEMP_BUF_SIZE             47616
#define OT_SAMPLE_IVE_KCF_ROI_PADDING_MAX_WIDTH     1024
#define OT_SAMPLE_IVE_KCF_ROI_PADDING_MIN_WIDTH     40
#define OT_SAMPLE_IVE_KCF_ROI_PADDING_MAX_HEIGHT    1024
#define OT_SAMPLE_IVE_KCF_ROI_PADDING_MIN_HEIGHT    40
#define OT_SAMPLE_IVE_KCF_QUEUE_LEN                 16
#define OT_SAMPLE_IVE_KCF_NUM_THIRTY_TWO            32
#define OT_SAMPLE_IVE_KCF_INTERP_FACTOR             (0.02 * 1024 * 32)
#define OT_SAMPLE_IVE_KCF_LAMDA                     10
#define OT_SAMPLE_IVE_KCF_SIGMA                     (0.5 * 256)
#define OT_SAMPLE_IVE_KCF_NORM_TRUNC_ALFA           (0.2 * 4096)
#define OT_SAMPLE_IVE_KCF_RESP_THR                  32
#define OT_SAMPLE_IVE_KCF_BBOX_RESP_THR             178000
#define OT_SAMPLE_IVE_KCF_PADDING                   (1.5 * 32)
#define OT_SAMPLE_IVE_KCF_QUERY_SLEEP               100
#define OT_SAMPLE_IVE_KCF_MILLIC_SEC                20000
#define OT_SAMPLE_IVE_KCF_TRACK_CAR_INDEX           7
#define OT_SAMPLE_IVE_KCF_NMS_THRESHOLD             0
#define OT_SAMPLE_IVE_KCF_STOP_SIGNAL               2
#define OT_SAMPLE_IVE_KCF_NO_STOP_SIGNAL            3

#define OT_SAMPLE_IVE_KCF_SVP_NPU_MAX_ROI_NUM_OF_CLASS     50
#define OT_SAMPLE_IVE_KCF_SVP_NPU_MAX_CLASS_NUM            100

#define sample_ive_mutex_init_lock(mutex)       \
    do {                                        \
        (void)pthread_mutex_init(&(mutex), TD_NULL); \
    } while (0)
#define sample_ive_mutex_lock(mutex)      \
    do {                                  \
        (void)pthread_mutex_lock(&(mutex)); \
    } while (0)
#define sample_ive_mutex_unlock(mutex)      \
    do {                                    \
        (void)pthread_mutex_unlock(&(mutex)); \
    } while (0)
#define sample_ive_mutex_destroy(mutex)      \
    do {                                     \
        (void)pthread_mutex_destroy(&(mutex)); \
    } while (0)

typedef enum {
    OT_CNN_GET_FRM_START = 0x0,
    OT_CNN_GET_FRM_END = 0x1,

    OT_CNN_GET_FRM_BUTT
} ot_sample_ive_cnn_get_frm_status;

typedef enum {
    OT_CNN_PROC_START = 0x0,
    OT_CNN_PROC_END = 0x1,

    OT_CNN_PROC_BUTT
} ot_sample_ive_cnn_proc_status;

typedef struct {
    td_u32 class_num;
    td_u32 total_num;
    td_u32 roi_num_array[OT_SAMPLE_IVE_KCF_SVP_NPU_MAX_CLASS_NUM];
    ot_sample_svp_rect rect[OT_SAMPLE_IVE_KCF_SVP_NPU_MAX_CLASS_NUM][OT_SAMPLE_IVE_KCF_SVP_NPU_MAX_ROI_NUM_OF_CLASS];
} ot_sample_svp_ive_rect_arr;

typedef struct {
    ot_ive_roi_info roi_info[OT_SAMPLE_IVE_KCF_ROI_NUM];
    td_u32 roi_num;
    ot_svp_mem_info total_mem;
    ot_svp_mem_info list_mem;
    ot_svp_mem_info cos_win_x;
    ot_svp_mem_info cos_win_y;
    ot_svp_mem_info gauss_peak;
    ot_ive_kcf_obj_list obj_list;
    ot_ive_kcf_proc_ctrl kcf_proc_ctrl;
    ot_ive_kcf_bbox bbox[OT_SAMPLE_IVE_KCF_ROI_NUM];
    td_u32 bbox_obj_num;
    ot_ive_kcf_bbox_ctrl kcf_bbox_ctrl;
    td_u3q5 padding;
    td_u8 reserved;

    ot_sample_ive_cnn_get_frm_status cnn_get_frame_status;
    ot_sample_ive_cnn_proc_status cnn_proc_status;
    td_bool is_new_det;
    td_bool is_first_det;

    ot_sample_ive_queue *queue_head;
    ot_video_frame_info frame_info_arr[OT_SAMPLE_IVE_KCF_FRAME_NUM];
    pthread_mutex_t cnn_det_mutex;
    pthread_mutex_t queue_mutex;
    pthread_mutex_t get_frame_mutex;
} ot_sample_ive_kcf_info;

typedef struct {
    ot_ive_roi_info roi[OT_SAMPLE_IVE_KCF_ROI_NUM];
    td_u32 roi_num;
} ot_sample_ive_kcf_roi_info;

typedef struct {
    ot_ive_kcf_bbox bbox[OT_SAMPLE_IVE_KCF_ROI_NUM];
    td_u32 bbox_num;
} ot_sample_ive_kcf_bbox_info;

typedef struct {
    td_u32 class_idx;
    td_u32 num;
    td_u32 roi_offset;
    td_float *x_min;
    td_float *y_min;
    td_float *x_max;
    td_float *y_max;
}ot_sample_ive_kcf_svp_npu_roi_to_rect_info;

/* kcf stop flag */
static td_bool g_ive_kcf_stop_signal = TD_FALSE;

/* vdec thread */
static pthread_t g_ive_kcf_vdec_thread = 0;
/* svp_npu detect thread */
static pthread_t g_ive_detect_thread = 0;
/* get frame thread */
static pthread_t g_ive_get_frm_thread = 0;
/* track thread */
static pthread_t g_ive_track_thread = 0;
/* kcf info */
static ot_sample_ive_kcf_info g_ive_kcf_info = {0};
/* svp_npu dev id */
static td_s32 g_ive_kcf_svp_npu_dev_id = 0;
/* svp_npu task */
static sample_svp_npu_task_info g_ive_kcf_svp_npu_task[SAMPLE_SVP_NPU_MAX_TASK_NUM] = {0};
/* svp_npu rfcn threhold */
static sample_svp_npu_threshold g_ive_kcf_svp_npu_rfcn_threshold[SAMPLE_SVP_NPU_RFCN_THRESHOLD_NUM] = {
    {0.7, 0.0, 16.0, 16.0, "rpn_data"}, {0.3, 0.9, 16.0, 16.0, "rpn_data1"} };
/* vb pool */
static ot_vb_pool_info g_ive_kcf_svp_npu_vb_pool_info;
static td_void *g_ive_kcf_svp_npu_vb_virt_addr = TD_NULL;

static sample_vo_cfg g_ive_kcf_vo_cfg = {0};

static ot_sample_svp_media_cfg g_ive_kcf_media_cfg = {
    .svp_switch = {TD_FALSE, TD_TRUE},
    .pic_type = {PIC_1080P, PIC_CIF},
    .chn_num = OT_SVP_MAX_VPSS_CHN_NUM,
};

static sample_vdec_attr g_ive_kcf_vdec_cfg = {
    .type = OT_PT_H265,
    .mode = OT_VDEC_SEND_MODE_FRAME,
    .width = FHD_WIDTH,
    .height = FHD_HEIGHT,
    .sample_vdec_video.dec_mode = OT_VIDEO_DEC_MODE_IP,
    .sample_vdec_video.bit_width = OT_DATA_BIT_WIDTH_8,
    .sample_vdec_video.ref_frame_num = 2, /* 2:ref_frame_num */
    .display_frame_num = 2,               /* 2:display_frame_num */
    .frame_buf_cnt = 5,                   /* 5:2+2+1 */
};

static vdec_thread_param g_ive_kcf_vdec_param = {
    .chn_id = 0,
    .type = OT_PT_H265,
    .stream_mode = OT_VDEC_SEND_MODE_FRAME,
    .interval_time = 1000, /* 1000:interval_time */
    .pts_init = 0,
    .pts_increase = 0,
    .e_thread_ctrl = THREAD_CTRL_START,
    .circle_send = TD_TRUE,
    .milli_sec = 0,
    .min_buf_size = (FHD_WIDTH * FHD_HEIGHT * 3) >> 1, /* 3:chn_size */
    .c_file_path = "./data/input/kcf",
    .c_file_name = "lighton_indoor.h265",
    .fps = 30,
};

static td_s32 sample_ive_svp_npu_get_data_buffer(const sample_svp_npu_task_info *task,
    const td_char *name, td_float **data, svp_acl_mdl_io_dims *dims, size_t *stride)
{
    svp_acl_error ret;
    size_t num_idx;
    size_t stride_tmp;
    svp_acl_data_buffer *data_buffer = TD_NULL;
    td_float *data_tmp = TD_NULL;
    sample_svp_npu_model_info *model_info = TD_NULL;

    sample_svp_check_exps_return(task == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "task is null!\n");
    sample_svp_check_exps_return(name == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "name is null!\n");
    sample_svp_check_exps_return(data == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "data is null!\n");

    model_info = sample_common_svp_npu_get_model_info(task->cfg.model_idx);
    sample_svp_check_exps_return(model_info == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get model info failed!\n");

    ret = svp_acl_mdl_get_output_index_by_name(model_info->model_desc, name, &num_idx);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get output indxe by name(%s) failed!\n", name);
    if (dims != TD_NULL) {
        ret = svp_acl_mdl_get_output_dims(model_info->model_desc, num_idx, dims);
        sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get num dims failed!\n");
    }

    data_buffer = svp_acl_mdl_get_dataset_buffer(task->output_dataset, num_idx);
    sample_svp_check_exps_return(data_buffer == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get data_buffer is NULL!\n");
    data_tmp = (td_float *)svp_acl_get_data_buffer_addr(data_buffer);
    sample_svp_check_exps_return(data_tmp == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get data is NULL!\n");
    if (stride != TD_NULL) {
        stride_tmp = svp_acl_get_data_buffer_stride(data_buffer);
        sample_svp_check_exps_return(stride_tmp == 0, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "get stride is 0!\n");
        *stride = stride_tmp;
    }

    *data = data_tmp;
    return TD_SUCCESS;
}

static td_void sample_ive_svp_npu_get_point(ot_sample_ive_kcf_svp_npu_roi_to_rect_info *arg_info,
    const ot_video_frame_info *proc_frame, const ot_size *base_frame_size, ot_sample_svp_ive_rect_arr *rect_info)
{
    td_u32 class_idx = arg_info->class_idx;
    td_u32 num = arg_info->num;
    td_u32 roi_offset = arg_info->roi_offset;
    td_float *x_min = arg_info->x_min;
    td_float *y_min = arg_info->y_min;
    td_float *x_max = arg_info->x_max;
    td_float *y_max = arg_info->y_max;

    rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].x = (td_u32)((td_float)x_min[roi_offset] /
        proc_frame->video_frame.width * base_frame_size->width) & (~1);
    rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].y = (td_u32)((td_float)y_min[roi_offset] /
        proc_frame->video_frame.height * base_frame_size->height) & (~1);

    rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].x = (td_u32)((td_float)x_max[roi_offset] /
        proc_frame->video_frame.width * base_frame_size->width) & (~1);
    rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].y =
        rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].y;

    rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].x =
        rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_RIGHT_TOP].x;
    rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].y = (td_u32)((td_float)y_max[roi_offset] /
        proc_frame->video_frame.height * base_frame_size->height) & (~1);

    rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_LEFT_BOTTOM].x =
        rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_LEFT_TOP].x;
    rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_LEFT_BOTTOM].y =
        rect_info->rect[class_idx][num].point[SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM].y;
}

static td_void sample_ive_svp_npu_set_roi_arg(td_float *x_min, size_t stride,
    ot_sample_ive_kcf_svp_npu_roi_to_rect_info *arg_info)
{
    arg_info->x_min = x_min;
    arg_info->y_min = arg_info->x_min + stride / sizeof(td_float);
    arg_info->x_max = arg_info->y_min + stride / sizeof(td_float);
    arg_info->y_max = arg_info->x_max + stride / sizeof(td_float);
}

typedef struct {
    const sample_svp_npu_detection_info *info;
    const ot_video_frame_info *proc_frame;
    const ot_size *base_frame_size;
    ot_sample_svp_ive_rect_arr *rect_info;
} sample_ive_svp_npu_fill_ctx;

static td_void sample_ive_svp_npu_fill_one_class(td_u32 roi_num,
    ot_sample_ive_kcf_svp_npu_roi_to_rect_info *arg_info, const sample_ive_svp_npu_fill_ctx *fill_ctx)
{
    td_u32 j;
    td_u32 tmp = 0;
    td_u32 roi_offset = fill_ctx->info->roi_offset;

    for (j = 0; j < roi_num; j++) {
        if (tmp >= OT_SAMPLE_IVE_KCF_SVP_NPU_MAX_ROI_NUM_OF_CLASS) {
            break;
        }
        arg_info->num = tmp;
        arg_info->roi_offset = roi_offset;
        sample_ive_svp_npu_get_point(arg_info, fill_ctx->proc_frame, fill_ctx->base_frame_size, fill_ctx->rect_info);
        tmp++;
        roi_offset++;
    }
    fill_ctx->rect_info->roi_num_array[arg_info->class_idx] = tmp;
    fill_ctx->rect_info->total_num += tmp;
}

static td_s32 sample_ive_svp_npu_roi_to_rect(const sample_svp_npu_task_info *task,
    sample_svp_npu_detection_info *info, const ot_video_frame_info *proc_frame,
    const ot_size *base_frame_size, ot_sample_svp_ive_rect_arr *rect_info)
{
    svp_acl_error ret;
    size_t stride;
    td_float *x_min = TD_NULL;
    td_float *roi_num_data = NULL;
    td_u32 i;
    td_u32 start = 0;
    td_u32 roi_num;
    svp_acl_mdl_io_dims roi_num_dims;
    ot_sample_ive_kcf_svp_npu_roi_to_rect_info arg_info;
    sample_ive_svp_npu_fill_ctx fill_ctx;
    /* Get Roi Num */
    ret = sample_ive_svp_npu_get_data_buffer(task, info->num_name, &roi_num_data, &roi_num_dims, TD_NULL);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get roi num failed!\n");

    rect_info->class_num = roi_num_dims.dims[roi_num_dims.dim_count - 1];
    info->roi_offset = 0;
    if (info->has_background == TD_TRUE) {
        info->roi_offset = (td_u32)(*roi_num_data);
        start++;
    }

    /* Get Roi */
    ret = sample_ive_svp_npu_get_data_buffer(task, info->roi_name, &x_min, TD_NULL, &stride);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "get roi num failed!\n");
    sample_ive_svp_npu_set_roi_arg(x_min, stride, &arg_info);
    fill_ctx.info = info;
    fill_ctx.proc_frame = proc_frame;
    fill_ctx.base_frame_size = base_frame_size;
    fill_ctx.rect_info = rect_info;
    for (i = start; i < rect_info->class_num; i++) {
        roi_num = (td_u32)*(roi_num_data + i);
        arg_info.class_idx = i;
        sample_ive_svp_npu_fill_one_class(roi_num, &arg_info, &fill_ctx);
        info->roi_offset += roi_num;
    }
    return TD_SUCCESS;
}

/*
 * function : Rfcn Proc
 */
static td_s32 sample_ive_svp_npu_acl_frame_proc(const ot_video_frame_info *ext_frame,
    const ot_size *base_frame_size, sample_svp_npu_detection_info *detection_info,
    ot_sample_svp_ive_rect_arr *rect_info)
{
    td_s32 ret;
    td_void *virt_addr = TD_NULL;
    td_u32 size = (td_u32)(ext_frame->video_frame.height * ext_frame->video_frame.stride[0] *
        SAMPLE_SVP_NPU_IMG_THREE_CHN / SAMPLE_SVP_NPU_DOUBLE);

    virt_addr = g_ive_kcf_svp_npu_vb_virt_addr +
        (ext_frame->video_frame.phys_addr[0] - g_ive_kcf_svp_npu_vb_pool_info.pool_phy_addr);
    ret = sample_common_svp_npu_update_input_data_buffer_info(virt_addr, size,
        ext_frame->video_frame.stride[0], 0, &g_ive_kcf_svp_npu_task[0]);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "update data buffer failed!\n");

    ret = sample_common_svp_npu_model_execute(&g_ive_kcf_svp_npu_task[0]);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "model execute failed!\n");

    /* draw result, this sample has 21 classes:
     class 0:background     class 1:plane           class 2:bicycle
     class 3:bird           class 4:boat            class 5:bottle
     class 6:bus            class 7:car             class 8:cat
     class 9:chair          class10:cow             class11:diningtable
     class 12:dog           class13:horse           class14:motorbike
     class 15:person        class16:pottedplant     class17:sheep
     class 18:sofa          class19:train           class20:tvmonitor */
    ret = sample_ive_svp_npu_roi_to_rect(&g_ive_kcf_svp_npu_task[0], detection_info, ext_frame,
        base_frame_size, rect_info);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "roi to rect failed!\n");

    return ret;
}

static td_void sample_ive_rect_to_roi_info(ot_sample_svp_ive_rect_arr *rect,
    td_u32 base_width, td_u32 base_height, ot_sample_ive_kcf_info *kcf_info)
{
    td_u32 i;
    td_u32 j;
    td_u32 k;
    td_u32 pad_dw;
    td_u32 pad_dh;

    k = 0;

    for (i = 0; i < rect->class_num; i++) {
        for (j = 0; j < rect->roi_num_array[i]; j++) {
            kcf_info->roi_info[k].roi.x = rect->rect[i][j].point[0].x * OT_SAMPLE_QUARTER_OF_1M;
            kcf_info->roi_info[k].roi.y = rect->rect[i][j].point[0].y * OT_SAMPLE_QUARTER_OF_1M;

            kcf_info->roi_info[k].roi.width = rect->rect[i][j].point[OT_SAMPLE_IDX_TWO].x -
                rect->rect[i][j].point[0].x;
            kcf_info->roi_info[k].roi.height = rect->rect[i][j].point[OT_SAMPLE_IDX_TWO].y -
                rect->rect[i][j].point[0].y;

            if ((kcf_info->roi_info[k].roi.x >= (td_s32)(base_width * OT_SAMPLE_QUARTER_OF_1M)) ||
                (kcf_info->roi_info[k].roi.y >= (td_s32)(base_height * OT_SAMPLE_QUARTER_OF_1M))) {
                continue;
            }

            pad_dw = kcf_info->roi_info[k].roi.width * kcf_info->padding / OT_SAMPLE_IVE_KCF_NUM_THIRTY_TWO;
            if (pad_dw > OT_SAMPLE_IVE_KCF_ROI_PADDING_MAX_WIDTH) {
                kcf_info->roi_info[k].roi.width = OT_SAMPLE_IVE_KCF_ROI_PADDING_MAX_WIDTH *
                    OT_SAMPLE_IVE_KCF_NUM_THIRTY_TWO / kcf_info->padding;
            } else if (pad_dw < OT_SAMPLE_IVE_KCF_ROI_PADDING_MIN_WIDTH) {
                kcf_info->roi_info[k].roi.width = OT_SAMPLE_IVE_KCF_ROI_PADDING_MIN_WIDTH *
                    OT_SAMPLE_IVE_KCF_NUM_THIRTY_TWO / kcf_info->padding + OT_SAMPLE_NUM_TWO;
            }

            pad_dh = kcf_info->roi_info[k].roi.height * kcf_info->padding / OT_SAMPLE_IVE_KCF_NUM_THIRTY_TWO;
            if (pad_dh > OT_SAMPLE_IVE_KCF_ROI_PADDING_MAX_WIDTH) {
                kcf_info->roi_info[k].roi.height = OT_SAMPLE_IVE_KCF_ROI_PADDING_MAX_HEIGHT *
                    OT_SAMPLE_IVE_KCF_NUM_THIRTY_TWO / kcf_info->padding;
            } else if (pad_dh < OT_SAMPLE_IVE_KCF_ROI_PADDING_MIN_WIDTH) {
                kcf_info->roi_info[k].roi.height = OT_SAMPLE_IVE_KCF_ROI_PADDING_MIN_HEIGHT *
                    OT_SAMPLE_IVE_KCF_NUM_THIRTY_TWO / kcf_info->padding + OT_SAMPLE_NUM_TWO;
            }

            k++;
            kcf_info->roi_info[k - 1].roi_id = k;
            kcf_info->roi_num = k;
            if (k >= OT_SAMPLE_IVE_KCF_ROI_NUM) {
                return;
            }
        }
    }
}

static td_s32 sample_ive_svp_npu_acl_vb_map(td_u32 vb_pool_idx)
{
    td_s32 ret;

    if (g_ive_kcf_svp_npu_vb_virt_addr == TD_NULL) {
        ret = ss_mpi_vb_get_pool_info(g_ive_kcf_media_cfg.vb_pool[vb_pool_idx], &g_ive_kcf_svp_npu_vb_pool_info);
        sample_svp_check_exps_return(ret != TD_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "get pool info failed!\n");
        g_ive_kcf_svp_npu_vb_virt_addr = ss_mpi_sys_mmap(g_ive_kcf_svp_npu_vb_pool_info.pool_phy_addr,
            g_ive_kcf_svp_npu_vb_pool_info.pool_size);
        sample_svp_check_exps_return(g_ive_kcf_svp_npu_vb_virt_addr == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "map vb pool failed!\n");
    }
    return TD_SUCCESS;
}

static td_void sample_ive_rfcn_detect_release_frame(ot_video_frame_info *proc_frm_info)
{
    td_s32 ret;
    const td_s32 vpss_grp = 0;
    ret = ss_mpi_vpss_release_chn_frame(vpss_grp, 1, proc_frm_info);
    if (ret != TD_SUCCESS) {
        sample_svp_trace_err("Error(%#x),ss_mpi_vpss_release_chn_frame failed!\n", ret);
    }

    sample_ive_mutex_lock(g_ive_kcf_info.get_frame_mutex);
    g_ive_kcf_info.cnn_get_frame_status = OT_CNN_GET_FRM_END;
    g_ive_kcf_info.cnn_proc_status = OT_CNN_PROC_END;
    sample_ive_mutex_unlock(g_ive_kcf_info.get_frame_mutex);
}

static td_s32 sample_ive_rfcn_detect_get_frame(ot_size *frame_size, ot_video_frame_info *proc_frm_info)
{
    td_s32 ret;
    sample_ive_mutex_lock(g_ive_kcf_info.get_frame_mutex);
        if (g_ive_kcf_info.cnn_proc_status == OT_CNN_PROC_START) {
            frame_size->width = g_ive_kcf_info.frame_info_arr[0].video_frame.width;
            frame_size->height = g_ive_kcf_info.frame_info_arr[0].video_frame.height;
            ret = memcpy_s(proc_frm_info, sizeof(ot_video_frame_info),
                &g_ive_kcf_info.frame_info_arr[1], sizeof(ot_video_frame_info));
            sample_svp_check_exps_goto(ret != EOK, fail, SAMPLE_SVP_ERR_LEVEL_ERROR, "memcpy_s frame failed!\n");
        } else {
            sample_ive_mutex_unlock(g_ive_kcf_info.get_frame_mutex);
            usleep(OT_SAMPLE_IVE_KCF_QUERY_SLEEP);
            return TD_FAILURE;
        }
    sample_ive_mutex_unlock(g_ive_kcf_info.get_frame_mutex);
    return  TD_SUCCESS;
fail:
    sample_ive_mutex_unlock(g_ive_kcf_info.get_frame_mutex);
    sample_ive_rfcn_detect_release_frame(proc_frm_info);
    return TD_FAILURE;
}

/*
 * function : Rfcn detect thread entry
 */
static td_void *sample_ive_rfcn_detect(td_void *args)
{
    td_s32 ret;
    ot_size frame_size;
    ot_video_frame_info proc_frm_info;
    ot_sample_svp_ive_rect_arr rect_arr;
    td_u32 size;
    td_u32 stride;
    td_u8 *data = TD_NULL;

    sample_svp_npu_detection_info *detection_info = (sample_svp_npu_detection_info *)args;
    sample_svp_check_exps_return(detection_info == TD_NULL, NULL, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "args is null!\n");
    ret = svp_acl_rt_set_device(g_ive_kcf_svp_npu_dev_id);
    sample_svp_check_exps_return(ret != TD_SUCCESS, TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "open device failed!\n");

    ret = sample_ive_svp_npu_acl_vb_map(OT_VPSS_CHN1);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "map vb pool failed!\n");

    ret = sample_common_svp_npu_get_input_data_buffer_info(&g_ive_kcf_svp_npu_task[0], 0, &data, &size, &stride);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),get_input_data_buffer_info failed!\n", ret);

    while (g_ive_kcf_stop_signal == TD_FALSE) {
        ret = sample_ive_rfcn_detect_get_frame(&frame_size, &proc_frm_info);
        if (ret != TD_SUCCESS) {
            continue;
        }

        ret = sample_ive_svp_npu_acl_frame_proc(&proc_frm_info, &frame_size, detection_info, &rect_arr);
        sample_svp_check_exps_goto(ret != TD_SUCCESS, release, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),sample_ive_rfcn_get_rect failed!\n", ret);

        sample_ive_mutex_lock(g_ive_kcf_info.cnn_det_mutex);
        sample_ive_rect_to_roi_info(&rect_arr, frame_size.width, frame_size.height, &g_ive_kcf_info);

        if (g_ive_kcf_info.roi_num != 0) {
            g_ive_kcf_info.is_new_det = TD_TRUE;
        }
        sample_ive_mutex_unlock(g_ive_kcf_info.cnn_det_mutex);

release:
        sample_ive_rfcn_detect_release_frame(&proc_frm_info);
    }
    /* recover data address */
    ret = sample_common_svp_npu_update_input_data_buffer_info(data, size, stride, 0, &g_ive_kcf_svp_npu_task[0]);
    sample_svp_check_exps_trace(ret != TD_SUCCESS, SAMPLE_SVP_ERR_LEVEL_ERROR, "update buffer failed!\n");
fail_1:
    (td_void)ss_mpi_sys_munmap(g_ive_kcf_svp_npu_vb_virt_addr, g_ive_kcf_svp_npu_vb_pool_info.pool_size);
    g_ive_kcf_svp_npu_vb_virt_addr = TD_NULL;
fail_0:
    (td_void)svp_acl_rt_reset_device(g_ive_kcf_svp_npu_dev_id);
    return TD_NULL;
}

static td_s32 sample_ive_update_frames(const ot_video_frame_info *base_video_frame_info, td_s32 milli_sec)
{
    td_s32 ret;
    ot_video_frame_info ext_video_frame_info;

    sample_ive_mutex_lock(g_ive_kcf_info.get_frame_mutex);
    if (g_ive_kcf_info.cnn_get_frame_status == OT_CNN_GET_FRM_END) {
        g_ive_kcf_info.cnn_get_frame_status = OT_CNN_GET_FRM_START;
        ret = ss_mpi_vpss_get_chn_frame(0, 1, &ext_video_frame_info, milli_sec);
        sample_svp_check_exps_goto(ret != TD_SUCCESS, release_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),vpss get grp(%d) chn(%d) failed!\n", ret, 0, 1);
        ret = memcpy_s(&g_ive_kcf_info.frame_info_arr[0], sizeof(ot_video_frame_info),
            base_video_frame_info, sizeof(ot_video_frame_info));
        sample_svp_check_exps_goto(ret != EOK, release_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "memcpy_s frame failed!\n");

        ret = memcpy_s(&g_ive_kcf_info.frame_info_arr[1], sizeof(ot_video_frame_info),
            &ext_video_frame_info, sizeof(ot_video_frame_info));
        sample_svp_check_exps_goto(ret != EOK, release_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "memcpy_s frame failed!\n");
        g_ive_kcf_info.cnn_proc_status = OT_CNN_PROC_START;
    }
    sample_ive_mutex_unlock(g_ive_kcf_info.get_frame_mutex);

    return TD_SUCCESS;
release_1:
    ret = ss_mpi_vpss_release_chn_frame(0, 1, &ext_video_frame_info);
    if (ret != TD_SUCCESS) {
        sample_svp_trace_err("Error(%#x),ss_mpi_vpss_release_chn_frame failed,Grp(0) chn(0)!\n", ret);
    }
release_0:
     g_ive_kcf_info.cnn_get_frame_status = OT_CNN_GET_FRM_END;
    sample_ive_mutex_unlock(g_ive_kcf_info.get_frame_mutex);
    return TD_FAILURE;
}
/*
 * function : Get frame thread entry
 */
static td_void *sample_ive_get_frame(td_void *args)
{
    ot_video_frame_info base_video_frame_info;
    const td_s32 milli_sec = OT_SAMPLE_IVE_KCF_MILLIC_SEC;
    td_s32 ret;
    ot_unused(args);

    while (g_ive_kcf_stop_signal == TD_FALSE) {
        ret = ss_mpi_vpss_get_chn_frame(0, 0, &base_video_frame_info, milli_sec);
        if (ret != TD_SUCCESS) {
            continue;
        }

        sample_ive_mutex_lock(g_ive_kcf_info.queue_mutex);
        ret = sample_ive_add_queue_node(g_ive_kcf_info.queue_head, &base_video_frame_info);
        if (ret != TD_SUCCESS) {
            sample_svp_trace_err("Error(%#x),sample_ive_add_queue_node failed!\n", ret);
            sample_ive_mutex_unlock(g_ive_kcf_info.queue_mutex);
            goto release_0;
        }
        sample_ive_mutex_unlock(g_ive_kcf_info.queue_mutex);
        ret = sample_ive_update_frames(&base_video_frame_info, milli_sec);
        sample_svp_check_exps_trace(ret != TD_SUCCESS, SAMPLE_SVP_ERR_LEVEL_ERROR, "update frames failed!\n");
        continue;
release_0:
        ret = ss_mpi_vpss_release_chn_frame(0, 0, &base_video_frame_info);
        sample_svp_check_exps_trace(ret != TD_SUCCESS, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_vpss_release_chn_frame failed,Grp(0) chn(0)!\n", ret);
    }

    return TD_NULL;
}

static td_float sample_ive_kcf_calc_iou(ot_svp_rect_s24q8 *rect1, ot_svp_rect_s24q8 *rect2)
{
    td_s32 min_x;
    td_s32 min_y;
    td_s32 max_x;
    td_s32 max_y;
    td_float area1;
    td_float area2;
    td_float inter_area;
    td_float iou;
    td_s32 width;
    td_s32 height;

    min_x = ot_sample_ive_max(rect1->x / OT_SAMPLE_QUARTER_OF_1M, rect2->x / OT_SAMPLE_QUARTER_OF_1M);
    min_y = ot_sample_ive_max(rect1->y / OT_SAMPLE_QUARTER_OF_1M, rect2->y / OT_SAMPLE_QUARTER_OF_1M);
    max_x = ot_sample_ive_min(rect1->x / OT_SAMPLE_QUARTER_OF_1M + rect1->width,
        rect2->x / OT_SAMPLE_QUARTER_OF_1M + rect2->width);
    max_y = ot_sample_ive_min(rect1->y / OT_SAMPLE_QUARTER_OF_1M + rect1->height,
        rect2->y / OT_SAMPLE_QUARTER_OF_1M + rect2->height);

    width = max_x - min_x + 1;
    height = max_y - min_y + 1;

    width = (width > 0) ? width : 0;
    height = (height > 0) ? height : 0;

    inter_area = (td_float)((td_u32)width * (td_u32)height);

    area1 = (td_float)(rect1->width * rect1->height);
    area2 = (td_float)(rect2->width * rect2->height);

    iou = inter_area / (area1 + area2 - inter_area);

    return iou;
}

static td_s32 sample_ive_kcf_updata_bbox_info(td_bool is_suppressed, ot_ive_roi_info *roi,
    ot_ive_kcf_bbox *bbox)
{
    td_s32 ret;
    td_bool is_track_ok = TD_FALSE;
    if (is_suppressed != 0) {
        ret = ss_mpi_ive_kcf_judge_obj_bbox_track_state(roi, bbox, &is_track_ok);
        sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error(%#x),ss_mpi_ive_kcf_judge_obj_bbox_track_state failed!\n", ret);
        /* do not update the bbox's u32RoiId */
        if (is_track_ok == TD_TRUE) {
            bbox->roi_info.roi.x = roi->roi.x;
            bbox->roi_info.roi.y = roi->roi.y;
            bbox->roi_info.roi.width = roi->roi.width;
            bbox->roi_info.roi.height = roi->roi.height;
            bbox->is_roi_refresh = TD_TRUE;

            roi->roi_id = 0;
        } else {
            bbox->is_track_ok = TD_FALSE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_ive_kcf_obj_iou(ot_sample_ive_kcf_roi_info *roi_info,
    ot_sample_ive_kcf_bbox_info *bbox_info, td_u32 max_roi_num, ot_sample_ive_kcf_roi_info *new_roi_info)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 num = 0;
    td_u32 i;
    td_u32 j;

    if (bbox_info->bbox_num == 0) {
        ret = memcpy_s(new_roi_info, sizeof(ot_sample_ive_kcf_roi_info), roi_info, sizeof(ot_sample_ive_kcf_roi_info));
        sample_svp_check_exps_return(ret != EOK, OT_ERR_IVE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "memcpy_s roi_info failed!\n");
        return ret;
    }

    if (roi_info->roi_num == 0) {
        new_roi_info->roi_num = 0;
        return ret;
    }

    for (i = 0; (i < roi_info->roi_num) && (i < OT_SAMPLE_IVE_KCF_ROI_NUM); i++) {
        for (j = 0; (j < bbox_info->bbox_num) && (j < OT_SAMPLE_IVE_KCF_ROI_NUM); j++) {
            td_bool is_suppressed;
            td_float iou_val;

            if (roi_info->roi[i].roi_id == 0) {
                break;
            }
            if ((bbox_info->bbox[j].roi_info.roi_id == 0) || (bbox_info->bbox[j].is_track_ok == TD_FALSE)) {
                continue;
            }

            iou_val = sample_ive_kcf_calc_iou(&roi_info->roi[i].roi, &bbox_info->bbox[j].roi_info.roi);
            is_suppressed = (td_bool)(iou_val >= OT_SAMPLE_IVE_KCF_NMS_THRESHOLD);
            ret = sample_ive_kcf_updata_bbox_info(is_suppressed, &roi_info->roi[i], &bbox_info->bbox[j]);
            sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "Error(%#x), sample_ive_kcf_updata_bbox_info failed!\n", ret);
            if (is_suppressed == TD_TRUE) {
                break;
            }
        }

        if (num == max_roi_num) {
            sample_svp_trace_debug("reaching the maxRoiNum(%d), the rest roi will be cast away!\n", max_roi_num);
            new_roi_info->roi_num = num;
            return TD_SUCCESS;
        }

        /* u32RoiId!=0, means it hasn't been suppressed */
        if (roi_info->roi[i].roi_id != 0) {
            ret = memcpy_s(&new_roi_info->roi[num++], sizeof(ot_ive_roi_info),
                &roi_info->roi[i], sizeof(ot_ive_roi_info));
            sample_svp_check_exps_return(ret != EOK, OT_ERR_IVE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
                "memcpy_s roi_info failed!\n");
        }
    }

    new_roi_info->roi_num = num;
    return ret;
}

static void sample_ive_fill_image(const ot_video_frame_info *frame_info, ot_svp_img *image)
{
    image->phys_addr[0] = frame_info->video_frame.phys_addr[0];
    image->phys_addr[1] = frame_info->video_frame.phys_addr[1];
    image->stride[0] = frame_info->video_frame.stride[0];
    image->stride[1] = frame_info->video_frame.stride[1];
    image->width = frame_info->video_frame.width;
    image->height = frame_info->video_frame.height;

    if (frame_info->video_frame.pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        image->type = OT_SVP_IMG_TYPE_YUV420SP;
    }
}

static td_void sample_ive_rect_to_point(ot_ive_kcf_bbox bbox[], td_u32 bbox_obj_num,
    ot_sample_svp_rect_info *rect)
{
    td_u32 i;
    td_u32 tmp_ux;
    td_u32 tmp_uy;
    td_s32 tmp_sx;
    td_s32 tmp_sy;

    rect->num = 0;
    for (i = 0; (i < bbox_obj_num) && (i < OT_SVP_RECT_NUM); i++) {
        tmp_sx = bbox[i].roi_info.roi.x / OT_SAMPLE_QUARTER_OF_1M;
        tmp_sy = bbox[i].roi_info.roi.y / OT_SAMPLE_QUARTER_OF_1M;

        if (tmp_sx < 0) {
            tmp_ux = *((td_u32 *)&tmp_sx);
            tmp_ux = tmp_ux & (~1);
            tmp_sx = *((td_s32 *)&tmp_ux);
        }
        if (tmp_sy < 0) {
            tmp_uy = *((td_u32 *)&tmp_sy);
            tmp_uy = tmp_uy & (~1);
            tmp_sy = *((td_s32 *)&tmp_sy);
        }
        if (tmp_sx > 0) {
            tmp_ux = (td_u32)tmp_sx;
            tmp_ux = tmp_ux & (~1);
            tmp_sx = tmp_ux;
        }
        if (tmp_sy > 0) {
            tmp_uy = (td_u32)tmp_sy;
            tmp_uy = tmp_uy & (~1);
            tmp_sy = tmp_uy;
        }

        rect->rect[i].point[0].x = tmp_sx;
        rect->rect[i].point[0].y = tmp_sy;
        if ((rect->rect[i].point[0].x < 0) || (rect->rect[i].point[0].y < 0)) {
            rect->rect[i].point[0].x = 0;
            rect->rect[i].point[0].y = 0;
        }
        rect->rect[i].point[1].x = rect->rect[i].point[0].x + (bbox[i].roi_info.roi.width & (~1));
        rect->rect[i].point[1].y = rect->rect[i].point[0].y;
        rect->rect[i].point[OT_SAMPLE_IDX_TWO].x = rect->rect[i].point[0].x +
            (bbox[i].roi_info.roi.width & (~1));
        rect->rect[i].point[OT_SAMPLE_IDX_TWO].y = rect->rect[i].point[0].y +
            (bbox[i].roi_info.roi.height & (~1));
        rect->rect[i].point[OT_SAMPLE_IDX_THREE].x = rect->rect[i].point[0].x;
        rect->rect[i].point[OT_SAMPLE_IDX_THREE].y = rect->rect[i].point[0].y +
            (bbox[i].roi_info.roi.height & (~1));

        rect->num++;
    }
}

static td_s32 sample_ive_disp_process(ot_video_frame_info *frame_info, ot_sample_svp_rect_info *rect)
{
    td_s32 ret;
    const ot_vo_layer vo_layer = 0;
    const ot_vo_chn vo_chn = 0;

    ret = sample_common_svp_vgs_fill_rect(frame_info, rect, 0x0000FF00);
    if (ret != TD_SUCCESS) {
        sample_svp_trace_err("Error(%#x),sample_common_svp_vgs_fill_rect failed!\n", ret);
    }

    ret = ss_mpi_vo_send_frame(vo_layer, vo_chn, frame_info, OT_SAMPLE_IVE_KCF_MILLIC_SEC);
    if (ret != TD_SUCCESS) {
        sample_svp_trace_err("Error(%#x),ss_mpi_vo_send_frame failed!\n", ret);
        return ret;
    }

    return ret;
}
static td_s32 sample_ive_kcf_query_task(ot_ive_handle handle)
{
    td_bool is_finish = TD_FALSE;
    td_bool is_block = TD_TRUE;
    td_s32 ret;
    ret = ss_mpi_ive_query(handle, &is_finish, is_block);
    while (ret == OT_ERR_IVE_QUERY_TIMEOUT) {
        usleep(OT_SAMPLE_IVE_KCF_QUERY_SLEEP);
        ret = ss_mpi_ive_query(handle, &is_finish, is_block);
    }
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_query failed!\n", ret);
    return TD_SUCCESS;
}

static td_s32 sample_ive_kcf_check_queue_is_ok(ot_sample_ive_node **queue_node)
{
    td_s32 queue_len;
    sample_ive_mutex_lock(g_ive_kcf_info.queue_mutex);
    queue_len = sample_ive_queue_size(g_ive_kcf_info.queue_head);
    if (queue_len <= 0) {
        sample_ive_mutex_unlock(g_ive_kcf_info.queue_mutex);
        return TD_FALSE;
    }

    *queue_node = sample_ive_get_queue_node(g_ive_kcf_info.queue_head);
    if (*queue_node == TD_NULL) {
        sample_ive_mutex_unlock(g_ive_kcf_info.queue_mutex);
        return TD_FALSE;
    }
    sample_ive_mutex_unlock(g_ive_kcf_info.queue_mutex);

    return TD_TRUE;
}

static td_s32 sample_ive_kcf_update_kcf_info(ot_sample_ive_kcf_roi_info *roi_info)
{
    td_u32 roi_num;
    td_s32 ret;
    sample_ive_mutex_lock(g_ive_kcf_info.cnn_det_mutex);
    g_ive_kcf_info.is_new_det = TD_FALSE;
    roi_num = g_ive_kcf_info.roi_num;
    g_ive_kcf_info.roi_num = 0;
    roi_info->roi_num = roi_num;

    ret = memcpy_s(roi_info->roi, sizeof(ot_ive_roi_info) * OT_SAMPLE_IVE_KCF_ROI_NUM, g_ive_kcf_info.roi_info,
        sizeof(ot_ive_roi_info) * roi_num);
    if (ret != EOK) {
        sample_ive_mutex_unlock(g_ive_kcf_info.cnn_det_mutex);
        sample_svp_check_exps_return(1, OT_ERR_IVE_ILLEGAL_PARAM, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "Error,memcpy_s kcf roi failed!\n");
    }
    sample_ive_mutex_unlock(g_ive_kcf_info.cnn_det_mutex);
    return TD_SUCCESS;
}

/*
 * function : Kcf tracking and vo display thread entry
 */
static td_void *sample_ive_kcf_tracking(td_void *args)
{
    ot_sample_ive_kcf_roi_info roi_info = {0};
    ot_sample_ive_kcf_roi_info new_roi_info = {0};
    ot_sample_ive_kcf_bbox_info bbox_info = {0};
    ot_sample_svp_rect_info rect = {0};
    ot_ive_handle handle;
    ot_svp_img src;
    ot_sample_ive_node *queue_node = TD_NULL;
    td_s32 ret;
    ot_unused(args);

    while (g_ive_kcf_stop_signal == TD_FALSE) {
        ret = sample_ive_kcf_check_queue_is_ok(&queue_node);
        if (ret == TD_FALSE) {
            usleep(OT_SAMPLE_IVE_KCF_QUERY_SLEEP);
            continue;
        }

        sample_ive_fill_image(&queue_node->frame_info, &src);

        if (g_ive_kcf_info.is_new_det == TD_TRUE) {
            ret = sample_ive_kcf_update_kcf_info(&roi_info);
            sample_svp_check_failed_err_level_goto(ret, fail, "Err(%#x),update_kcf_info failed!\n", ret);

            ret = sample_ive_kcf_obj_iou(&roi_info, &bbox_info, OT_SAMPLE_IVE_KCF_ROI_NUM, &new_roi_info);
            sample_svp_check_failed_err_level_goto(ret, fail, "Err(%#x),sample_ive_kcf_obj_iou failed!\n", ret);

            ret = ss_mpi_ive_kcf_obj_update(&g_ive_kcf_info.obj_list, bbox_info.bbox, bbox_info.bbox_num);
            sample_svp_check_failed_err_level_goto(ret, fail, "Err(%#x),ss_mpi_ive_kcf_obj_update failed!\n", ret);

            ret = ss_mpi_ive_kcf_get_train_obj(g_ive_kcf_info.padding, new_roi_info.roi, new_roi_info.roi_num,
                &g_ive_kcf_info.cos_win_x, &g_ive_kcf_info.cos_win_y, &g_ive_kcf_info.gauss_peak,
                &g_ive_kcf_info.obj_list);
            sample_svp_check_failed_err_level_goto(ret, fail, "Err(%#x),ss_mpi_ive_kcf_get_train_obj failed!\n", ret);
        }

        if (g_ive_kcf_info.obj_list.track_obj_num != 0 || g_ive_kcf_info.obj_list.train_obj_num != 0) {
            ret = ss_mpi_ive_kcf_proc(&handle, &src, &g_ive_kcf_info.obj_list, &g_ive_kcf_info.kcf_proc_ctrl, TD_TRUE);
            sample_svp_check_failed_err_level_goto(ret, fail, "Err(%#x),ss_mpi_ive_kcf_proc failed!\n", ret);

            ret = sample_ive_kcf_query_task(handle);
            sample_svp_check_failed_err_level_goto(ret, fail, "ive query task failed");

            ret = ss_mpi_ive_kcf_get_obj_bbox(&g_ive_kcf_info.obj_list, bbox_info.bbox, &bbox_info.bbox_num,
                &g_ive_kcf_info.kcf_bbox_ctrl);
            sample_svp_check_failed_err_level_trace(ret, "Err(%#x),ss_mpi_ive_kcf_get_obj_bbox failed!\n", ret);

            sample_ive_rect_to_point(bbox_info.bbox, bbox_info.bbox_num, &rect);
        } else {
            rect.num = 0;
        }

        ret = sample_ive_disp_process(&queue_node->frame_info, &rect);
        sample_svp_check_failed_err_level_trace(ret, "Err(%#x),sampel_disp_process failed!\n", ret);
    fail:
        (td_void)ss_mpi_vpss_release_chn_frame(0, 0, &queue_node->frame_info);
        sample_ive_free_queue_node(&queue_node);
    }

    return TD_NULL;
}

static td_void sample_ive_kcf_deinit(ot_sample_ive_kcf_info *kcf_info)
{
    ot_sample_ive_node *queue_node = TD_NULL;
    sample_svp_check_exps_return_void(kcf_info == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "kcf_info can't be null\n");

    if (kcf_info->cnn_proc_status == OT_CNN_PROC_START) {
        (td_void)ss_mpi_vpss_release_chn_frame(0, 1, &kcf_info->frame_info_arr[1]);
        sample_ive_mutex_lock(kcf_info->get_frame_mutex);
        kcf_info->cnn_get_frame_status = OT_CNN_GET_FRM_END;
        kcf_info->cnn_proc_status = OT_CNN_PROC_END;
        sample_ive_mutex_unlock(kcf_info->get_frame_mutex);
    }

    sample_ive_mutex_destroy(kcf_info->queue_mutex);
    sample_ive_mutex_destroy(kcf_info->get_frame_mutex);
    sample_ive_mutex_destroy(kcf_info->cnn_det_mutex);
    (td_void)ss_mpi_ive_kcf_destroy_obj_list(&kcf_info->obj_list);

    while (!sample_ive_is_queue_empty(kcf_info->queue_head)) {
        queue_node = sample_ive_get_queue_node(kcf_info->queue_head);
        if (queue_node != TD_NULL) {
            (td_void)ss_mpi_vpss_release_chn_frame(0, 0, &queue_node->frame_info);
            sample_ive_free_queue_node(&queue_node);
        }
    }
    sample_ive_destory_queue(&kcf_info->queue_head);
    sample_svp_mmz_free(kcf_info->total_mem.phys_addr, kcf_info->total_mem.virt_addr);

    return;
}

static td_void sample_ive_kcf_info_common_init(ot_sample_ive_kcf_info *kcf_info, td_u32 size)
{
    kcf_info->list_mem.phys_addr = kcf_info->total_mem.phys_addr;
    kcf_info->list_mem.virt_addr = kcf_info->total_mem.virt_addr;
    kcf_info->list_mem.size = size;

    kcf_info->gauss_peak.phys_addr = kcf_info->list_mem.phys_addr + size;
    kcf_info->gauss_peak.virt_addr = kcf_info->list_mem.virt_addr + size;
    kcf_info->gauss_peak.size = OT_SAMPLE_IVE_KCF_GAUSS_PEAK_TOTAL_SIZE;

    kcf_info->cos_win_x.phys_addr = kcf_info->gauss_peak.phys_addr + OT_SAMPLE_IVE_KCF_GAUSS_PEAK_TOTAL_SIZE;
    kcf_info->cos_win_x.virt_addr = kcf_info->gauss_peak.virt_addr + OT_SAMPLE_IVE_KCF_GAUSS_PEAK_TOTAL_SIZE;
    kcf_info->cos_win_x.size = OT_SAMPLE_IVE_KCF_COS_WINDOW_TOTAL_SIZE;

    kcf_info->cos_win_y.phys_addr = kcf_info->cos_win_x.phys_addr + OT_SAMPLE_IVE_KCF_COS_WINDOW_TOTAL_SIZE;
    kcf_info->cos_win_y.virt_addr = kcf_info->cos_win_x.virt_addr + OT_SAMPLE_IVE_KCF_COS_WINDOW_TOTAL_SIZE;
    kcf_info->cos_win_y.size = OT_SAMPLE_IVE_KCF_COS_WINDOW_TOTAL_SIZE;

    kcf_info->kcf_proc_ctrl.tmp_buf.phys_addr = kcf_info->cos_win_y.phys_addr +
        OT_SAMPLE_IVE_KCF_COS_WINDOW_TOTAL_SIZE;
    kcf_info->kcf_proc_ctrl.tmp_buf.virt_addr = kcf_info->cos_win_y.virt_addr +
        OT_SAMPLE_IVE_KCF_COS_WINDOW_TOTAL_SIZE;
    kcf_info->kcf_proc_ctrl.tmp_buf.size = OT_SAMPLE_IVE_KCF_TEMP_BUF_SIZE;

    kcf_info->kcf_proc_ctrl.csc_mode = OT_IVE_CSC_MODE_VIDEO_BT709_YUV_TO_RGB;
    kcf_info->kcf_proc_ctrl.interp_factor = OT_SAMPLE_IVE_KCF_INTERP_FACTOR;
    kcf_info->kcf_proc_ctrl.lamda = OT_SAMPLE_IVE_KCF_LAMDA;
    kcf_info->kcf_proc_ctrl.sigma = OT_SAMPLE_IVE_KCF_SIGMA;
    kcf_info->kcf_proc_ctrl.norm_trunc_alfa = OT_SAMPLE_IVE_KCF_NORM_TRUNC_ALFA;
    kcf_info->kcf_proc_ctrl.response_threshold = OT_SAMPLE_IVE_KCF_RESP_THR;

    kcf_info->padding = OT_SAMPLE_IVE_KCF_PADDING;

    kcf_info->kcf_bbox_ctrl.max_bbox_num = OT_SAMPLE_IVE_KCF_NODE_MAX_NUM;
    kcf_info->kcf_bbox_ctrl.response_threshold = OT_SAMPLE_IVE_KCF_BBOX_RESP_THR;
}

static td_s32 sample_ive_kcf_init(ot_sample_ive_kcf_info *kcf_info)
{
    td_s32 ret = OT_ERR_IVE_NULL_PTR;
    td_u32 size;
    td_u32 total_size;
    td_s32 len;

    sample_svp_check_exps_return(kcf_info == TD_NULL, ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "kcf_info can't be null\n");

    (td_void)memset_s(kcf_info, sizeof(ot_sample_ive_kcf_info), 0, sizeof(ot_sample_ive_kcf_info));

    ret = ss_mpi_ive_kcf_get_mem_size(OT_SAMPLE_IVE_KCF_NODE_MAX_NUM, &size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_kcf_get_mem_size failed!\n", ret);

    /* (HOGFeatrue + Alpha + DstBuf) + Guasspeak + CosWinX + CosWinY + TmpBuf */
    total_size = size + OT_SAMPLE_IVE_KCF_GAUSS_PEAK_TOTAL_SIZE + OT_SAMPLE_IVE_KCF_COS_WINDOW_TOTAL_SIZE *
        OT_SAMPLE_NUM_TWO + OT_SAMPLE_IVE_KCF_TEMP_BUF_SIZE;
    ret = sample_common_ive_create_mem_info(&kcf_info->total_mem, total_size);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_common_ive_create_mem_info failed!\n", ret);
    (td_void)memset_s((td_u8 *)(td_uintptr_t)kcf_info->total_mem.virt_addr, total_size, 0x0, total_size);

    sample_ive_kcf_info_common_init(kcf_info, size);

    len = OT_SAMPLE_IVE_KCF_QUEUE_LEN;
    kcf_info->queue_head = sample_ive_create_queue(len);
    sample_svp_check_exps_goto(kcf_info->queue_head == TD_NULL, fail_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error,sample_ive_create_queue failed!\n");

    ret = ss_mpi_ive_kcf_create_obj_list(&kcf_info->list_mem, OT_SAMPLE_IVE_KCF_NODE_MAX_NUM, &kcf_info->obj_list);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_2, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_kcf_create_obj_list failed!\n", ret);

    ret = ss_mpi_ive_kcf_create_gauss_peak(kcf_info->padding, &kcf_info->gauss_peak);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_3, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_kcf_create_gauss_peak failed!\n", ret);

    ret = ss_mpi_ive_kcf_create_cos_win(&kcf_info->cos_win_x, &kcf_info->cos_win_y);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_3, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),ss_mpi_ive_kcf_create_cos_win failed!\n", ret);

    kcf_info->cnn_get_frame_status = OT_CNN_GET_FRM_END;
    kcf_info->cnn_proc_status = OT_CNN_PROC_END;
    kcf_info->is_first_det = TD_FALSE;
    sample_ive_mutex_init_lock(kcf_info->cnn_det_mutex);
    sample_ive_mutex_init_lock(kcf_info->get_frame_mutex);
    sample_ive_mutex_init_lock(kcf_info->queue_mutex);

    return ret;

fail_3:
    (td_void)ss_mpi_ive_kcf_destroy_obj_list(&kcf_info->obj_list);
fail_2:
    sample_ive_destory_queue(&kcf_info->queue_head);
fail_1:
    sample_svp_mmz_free(kcf_info->total_mem.phys_addr, kcf_info->total_mem.virt_addr);
fail_0:
    return ret;
}

static td_s32 sample_ive_svp_npu_acl_dataset_init(sample_svp_npu_task_info *task_info)
{
    td_s32 ret;
    sample_svp_check_exps_return(task_info == TD_NULL, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "task_info is null!\n");

    ret = sample_common_svp_npu_create_input(task_info);
    sample_svp_check_exps_return(ret != TD_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "create input failed!\n");

    ret = sample_common_svp_npu_create_output(task_info);
    if (ret != TD_SUCCESS) {
        sample_common_svp_npu_destroy_input(task_info);
        sample_svp_trace_err("execute create output fail.\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void sample_ive_svp_npu_acl_dataset_deinit(sample_svp_npu_task_info *task_info)
{
    sample_svp_check_exps_return_void(task_info == TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "task_info is null!\n");
    (td_void)sample_common_svp_npu_destroy_input(task_info);
    (td_void)sample_common_svp_npu_destroy_output(task_info);
}

static td_void sample_ive_svp_npu_acl_deinit_task(sample_svp_npu_task_info *task_info_arr, td_u32 task_num)
{
    td_u32 task_idx;
    for (task_idx = 0; task_idx < task_num; task_idx++) {
        (td_void)sample_common_svp_npu_destroy_work_buf(&task_info_arr[task_idx]);
        (td_void)sample_common_svp_npu_destroy_task_buf(&task_info_arr[task_idx]);
        (td_void)sample_ive_svp_npu_acl_dataset_deinit(&task_info_arr[task_idx]);
        (td_void)memset_s(&task_info_arr[task_idx], sizeof(sample_svp_npu_task_cfg), 0,
            sizeof(sample_svp_npu_task_cfg));
    }
}

static td_s32 sample_ive_svp_npu_acl_init_task(sample_svp_npu_task_info *task_info_arr, td_u32 task_num)
{
    td_u32 task_idx;
    td_s32 ret;

    for (task_idx = 0; task_idx < task_num; task_idx++) {
        ret = sample_ive_svp_npu_acl_dataset_init(&task_info_arr[task_idx]);
        sample_svp_check_exps_goto(ret != TD_SUCCESS, task_init_end_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "dataset init failed!\n");

        ret = sample_common_svp_npu_create_task_buf(&task_info_arr[task_idx]);
        sample_svp_check_exps_goto(ret != TD_SUCCESS, task_init_end_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "create task buf failed.\n");
        ret = sample_common_svp_npu_create_work_buf(&task_info_arr[task_idx]);
        sample_svp_check_exps_goto(ret != TD_SUCCESS, task_init_end_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
            "create work buf failed.\n");
    }
    return TD_SUCCESS;

task_init_end_0:
    (td_void)sample_ive_svp_npu_acl_deinit_task(task_info_arr, task_num);
    return ret;
}

static td_void sample_ive_rfcn_deinit()
{
    const td_u32 model_idx = 0;
    sample_ive_svp_npu_acl_deinit_task(g_ive_kcf_svp_npu_task, 1);
    sample_common_svp_npu_unload_model(model_idx);
    sample_common_svp_npu_acl_deinit(g_ive_kcf_svp_npu_dev_id);
}

static td_s32 sample_ive_rfcn_init(const td_char *acl_config_path, const td_char *om_model_path,
    sample_svp_npu_detection_info *detection_info)
{
    td_s32 ret;
    const td_u32 model_idx = 0;
    /* 1. acl init */
    ret = sample_common_svp_npu_acl_init(acl_config_path, g_ive_kcf_svp_npu_dev_id);
    sample_svp_check_exps_return(ret != TD_SUCCESS, ret, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_common_svp_npu_acl_init failed!\n", ret);
    /* 2. load model */
    ret = sample_common_svp_npu_load_model(om_model_path, model_idx, TD_FALSE);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "load model failed!\n");
    /* 3. set cfg */
    g_ive_kcf_svp_npu_task[0].cfg.max_batch_num = 1;
    g_ive_kcf_svp_npu_task[0].cfg.dynamic_batch_num = 1;
    g_ive_kcf_svp_npu_task[0].cfg.total_t = 0;
    g_ive_kcf_svp_npu_task[0].cfg.is_cached = TD_TRUE;
    g_ive_kcf_svp_npu_task[0].cfg.model_idx = model_idx;
    /* 4. init task */
    ret = sample_ive_svp_npu_acl_init_task(g_ive_kcf_svp_npu_task, 1);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "init task failed!\n");

    /* 5. set rpn param */
    ret = sample_common_svp_npu_set_threshold(g_ive_kcf_svp_npu_rfcn_threshold,
        SAMPLE_SVP_NPU_RFCN_THRESHOLD_NUM, &g_ive_kcf_svp_npu_task[0]);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_2, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "set threshold failed!\n");
    return ret;
end_2:
    (td_void)sample_ive_svp_npu_acl_deinit_task(g_ive_kcf_svp_npu_task, 1);
end_1:
    sample_common_svp_npu_unload_model(model_idx);
end_0:
    sample_common_svp_npu_acl_deinit(g_ive_kcf_svp_npu_dev_id);
    return ret;
}
static td_void sample_ive_destory_thread()
{
    if (g_ive_get_frm_thread != 0) {
        pthread_join(g_ive_get_frm_thread, TD_NULL);
        g_ive_get_frm_thread = 0;
    }
    if (g_ive_track_thread != 0) {
        pthread_join(g_ive_track_thread, TD_NULL);
        g_ive_track_thread = 0;
    }
    if (g_ive_detect_thread != 0) {
        pthread_join(g_ive_detect_thread, TD_NULL);
        g_ive_detect_thread = 0;
    }
}

#define IVE_GET_STREAM_PTHREAD_STACK_SIZE (256 * 1024)
static td_s32 sample_ive_create_thread(sample_svp_npu_detection_info *detection_info)
{
    td_s32 ret;
    pthread_attr_t attr;
    g_ive_get_frm_thread = 0;
    g_ive_track_thread = 0;
    g_ive_detect_thread = 0;
    ret = prctl(PR_SET_NAME, "get_frame", 0, 0, 0);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "set thread name failed!\n");
    ret = pthread_create(&g_ive_get_frm_thread, 0, sample_ive_get_frame, TD_NULL);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "pthread_create failed!\n");

    ret = prctl(PR_SET_NAME, "kcf_tracking", 0, 0, 0);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "set thread name failed!\n");
    ret = pthread_create(&g_ive_track_thread, 0, sample_ive_kcf_tracking, TD_NULL);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "pthread_create failed!\n");

    ret = prctl(PR_SET_NAME, "nnie_detect", 0, 0, 0);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "set thread name failed!\n");

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, IVE_GET_STREAM_PTHREAD_STACK_SIZE);
    ret = pthread_create(&g_ive_detect_thread, &attr, sample_ive_rfcn_detect, detection_info);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, fail_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "pthread_create failed!\n");

    return TD_SUCCESS;
fail_0:
    g_ive_kcf_stop_signal = TD_TRUE;
    if (g_ive_track_thread != 0) {
        pthread_join(g_ive_track_thread, TD_NULL);
        g_ive_track_thread = 0;
    }
    if (g_ive_get_frm_thread != 0) {
        pthread_join(g_ive_get_frm_thread, TD_NULL);
        g_ive_get_frm_thread = 0;
    }
    return TD_FAILURE;
}
static td_void sample_ive_kcf_stop(td_void)
{
    sample_ive_destory_thread();
    sample_ive_rfcn_deinit();
    sample_ive_kcf_deinit(&g_ive_kcf_info);
    (td_void)sample_common_svp_destroy_vb_stop_vdec_vpss_vo(&g_ive_kcf_vdec_param, &g_ive_kcf_vdec_thread,
        &g_ive_kcf_media_cfg, &g_ive_kcf_vo_cfg);
    (td_void)sample_common_svp_check_sys_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
}

static td_s32 sample_ive_kcf_pause(td_void)
{
    printf("---------------press Enter key to exit!---------------\n");
    (void)getchar();
    if (g_ive_kcf_stop_signal == TD_TRUE) {
        sample_ive_kcf_stop();
        return OT_SAMPLE_IVE_KCF_STOP_SIGNAL;
    }
    return OT_SAMPLE_IVE_KCF_NO_STOP_SIGNAL;
}

/*
 * function : Rfcn + Kcf  detecting and tracking
 */
void sample_ive_kcf(void)
{
    td_s32 ret;
    td_bool is_mpi_init;
    const td_char *om_model_path = "../svp_npu/data/model/rfcn.om";
    sample_svp_npu_detection_info detection_info = {0};

    detection_info.num_name = "detection_filter_3_0";
    detection_info.roi_name = "detection_filter_3_";
    detection_info.has_background = TD_TRUE;

    /*
    * step 1: init system
    */
    is_mpi_init = sample_common_svp_check_sys_init(TD_FALSE);
    sample_svp_check_exps_return_void(is_mpi_init != TD_TRUE, SAMPLE_SVP_ERR_LEVEL_ERROR, "mpi init failed!\n");

    /*
     * step 2: start vdec vpss venc vo
     */
    ret = sample_common_svp_create_vb_start_vdec_vpss_vo(&g_ive_kcf_vdec_cfg, &g_ive_kcf_vdec_param,
        &g_ive_kcf_vdec_thread, &g_ive_kcf_media_cfg, &g_ive_kcf_vo_cfg);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_0, SAMPLE_SVP_ERR_LEVEL_DEBUG, "init media failed!\n");

    /*
     * step 3: init kcf param
     */
    ret = sample_ive_kcf_init(&g_ive_kcf_info);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_ive_kcf_init failed!\n", ret);
    /*
     * step 4: init rfcn param
     */
    ret = sample_ive_rfcn_init(TD_NULL, om_model_path, &detection_info);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_2, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "Error(%#x),sample_ive_rfcn_init failed!\n", ret);
    /*
     * setp 5: create_thread get_frame -> rfcn_detection + ive_kcf_tracking
     */
    g_ive_kcf_stop_signal = TD_FALSE;
    ret = sample_ive_create_thread(&detection_info);
    sample_svp_check_exps_goto(ret != TD_SUCCESS, end_3, SAMPLE_SVP_ERR_LEVEL_ERROR, "ive create thread failed!\n");

    ret = sample_ive_kcf_pause();
    sample_svp_check_exps_return_void(ret == OT_SAMPLE_IVE_KCF_STOP_SIGNAL, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "kcf abnormally!\n");

    g_ive_kcf_stop_signal = TD_TRUE;

    sample_ive_destory_thread();
end_3:
    sample_ive_rfcn_deinit();
end_2:
    sample_ive_kcf_deinit(&g_ive_kcf_info);
end_1:
    (td_void)sample_common_svp_destroy_vb_stop_vdec_vpss_vo(&g_ive_kcf_vdec_param, &g_ive_kcf_vdec_thread,
        &g_ive_kcf_media_cfg, &g_ive_kcf_vo_cfg);
end_0:
    (td_void)sample_common_svp_check_sys_exit();
    return;
}

/*
 * function : Kcf sample signal handle
 */
void sample_ive_kcf_handle_sig(void)
{
    g_ive_kcf_stop_signal = TD_TRUE;
}

